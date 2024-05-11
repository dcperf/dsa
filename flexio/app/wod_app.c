// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#include "app.h"

void* mlx5_async_event_watch(void *arg)
{
    struct ibv_async_event async_event;
    struct app_ctx *app_ctx = (struct app_ctx *)arg;

    ibv_get_async_event(app_ctx->ibv_ctx, &async_event);

    fprintf(stderr, "Got async event %d\n", async_event.event_type);
    return NULL;
 }

int start_async_watch(struct app_ctx *app_ctx)
{
    pthread_create(&app_ctx->async_thread, NULL, mlx5_async_event_watch, (void *)app_ctx);
    return 0;
}

int stop_async_watch(struct app_ctx *app_ctx)
{
    pthread_cancel(app_ctx->async_thread);

    return 0;
}

void create_mlx5_ctx(struct app_ctx *app_ctx, const char *dev_name)
{
    struct ibv_device **dev_list = ibv_get_device_list(NULL);
    struct ibv_device *dev = NULL;

    for (int32_t idx = 0; dev_list && dev_list[idx]; idx++) {
        if (!strcmp(ibv_get_device_name(dev_list[idx]), dev_name)) {
            dev = dev_list[idx];
            break;
        }
    }

    if (dev == NULL) {
        fprintf(stderr, "Not found %s\n", dev_name);
        exit(EXIT_FAILURE);
    }

    app_ctx->ibv_ctx = ibv_open_device(dev);
    if (app_ctx->ibv_ctx == NULL) {
        fprintf(stderr, "Failed to create ibv_ctx at %s\n", dev_name);
        exit(EXIT_FAILURE);
    }

    ibv_free_device_list(dev_list);
    dev_list = NULL;

    start_async_watch(app_ctx);

    app_ctx->pd = ibv_alloc_pd(app_ctx->ibv_ctx);
    if (app_ctx->pd == NULL) {
        fprintf(stderr, "Failed to allocte ibv_pd at %s\n", dev_name);
        ibv_close_device(app_ctx->ibv_ctx);
        app_ctx->ibv_ctx = NULL;
        exit(EXIT_FAILURE);
    }
}

void destroy_mlx5_ctx(struct app_ctx *app_ctx)
{
    if (app_ctx == NULL) {
        return;
    }

    if (app_ctx->pd) {
        ibv_dealloc_pd(app_ctx->pd);
        app_ctx->pd = NULL;
    }

    if (app_ctx->ibv_ctx) {
        stop_async_watch(app_ctx);

        ibv_close_device(app_ctx->ibv_ctx);
        app_ctx->ibv_ctx = NULL;
    }

    return;
}

int32_t create_app_ctx(struct app_ctx **app_ctx_p)
{
    struct app_ctx* app_ctx = calloc(1, sizeof(struct app_ctx));
    *app_ctx_p = app_ctx;

    if (app_ctx == NULL) {
        return -1;
    }

    int proc_name_len;
    char proc_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(proc_name, &proc_name_len);
    strncpy(app_ctx->proc_name, proc_name, MPI_MAX_PROCESSOR_NAME);

    return 0;
}

void destroy_app_ctx(struct app_ctx *app_ctx)
{
    destroy_mlx5_ctx(app_ctx);

    free(app_ctx);

    return;
}

void cfg_run_info(struct app_ctx *app_ctx, int argc, char **argv)
{
    if (app_ctx == NULL) {
        return;
    }

    app_ctx->msg_size = argc >= 2 ? atoi(argv[1]) : 512 * 1024 * 1024;
    app_ctx->iteration = argc >= 3 ? atoi(argv[2]) : 10000;

    if (app_ctx->msg_size == 0 || app_ctx->msg_size >= 1024 * 1024 * 1024) {
        app_ctx->msg_size = 512 * 1024 * 1024;
    }

    if (app_ctx->iteration == 0 || app_ctx->iteration >= 100000) {
        app_ctx->iteration = 100000;
    }
}

int main(int argc, char **argv)
{
    int ret = 0, rank_id, rank_cnt;
    struct app_ctx *app_ctx = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
    MPI_Comm_size(MPI_COMM_WORLD, &rank_cnt);

    ret = create_app_ctx(&app_ctx);
    if (app_ctx == NULL) {
        ret = -1;
    }
    cfg_run_info(app_ctx, argc, argv);

    create_mlx5_ctx(app_ctx, "mlx5_0");

    destroy_app_ctx(app_ctx);
    app_ctx = NULL;

    return ret;
}
