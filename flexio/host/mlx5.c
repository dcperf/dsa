// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#include <mlx5.h>

void* mlx5_async_event_watch(void *arg)
{
    struct ibv_async_event event;
    struct mlx5_ctx *mlx5_ctx = (struct mlx5_ctx *)arg;

    ibv_get_async_event(mlx5_ctx->ibv_ctx, &event);

     fprintf(stderr, "Got event %d\n", event.event_type);

     return NULL;
}

int start_async_watch(struct mlx5_ctx *mlx5_ctx)
{
    pthread_create(&mlx5_ctx->async_thread, NULL, mlx5_async_event_watch, (void *)mlx5_ctx);
    return 0;
}

int stop_async_watch(struct mlx5_ctx *mlx5_ctx)
{
    pthread_cancel(mlx5_ctx->async_thread);

    return 0;
}

void create_mlx5_ctx(struct mlx5_ctx *mlx5_ctx, const char *dev_name)
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

    mlx5_ctx->ibv_ctx = ibv_open_device(dev);
    if (mlx5_ctx->ibv_ctx == NULL) {
        fprintf(stderr, "Failed to create ibv_ctx at %s\n", dev_name);
        exit(EXIT_FAILURE);
    }

    ibv_free_device_list(dev_list);
    dev_list = NULL;

    start_async_watch(mlx5_ctx);

    mlx5_ctx->pd = ibv_alloc_pd(mlx5_ctx->ibv_ctx);
    if (mlx5_ctx->pd == NULL) {
        fprintf(stderr, "Failed to allocte ibv_pd at %s\n", dev_name);
        ibv_close_device(mlx5_ctx->ibv_ctx);
        mlx5_ctx->ibv_ctx = NULL;
        exit(EXIT_FAILURE);
    }

    int rst = ibv_query_port(mlx5_ctx->ibv_ctx, 1, &mlx5_ctx->port_attr);
    if (rst) {
        fprintf(stderr, "Failed to query port attr at %s\n", dev_name);
        exit(EXIT_FAILURE);
    }

    rst = ibv_query_gid(mlx5_ctx->ibv_ctx, 1, 1, &mlx5_ctx->gid);
    if (rst) {
        fprintf(stderr, "Failed to gid at %s\n", dev_name);
        exit(EXIT_FAILURE);
    }
}

void destroy_mlx5_ctx(struct mlx5_ctx *mlx5_ctx)
{
    if (mlx5_ctx == NULL) {
        return;
    }

    if (mlx5_ctx->pd) {
        ibv_dealloc_pd(mlx5_ctx->pd);
        mlx5_ctx->pd = NULL;
    }

    if (mlx5_ctx->ibv_ctx) {
        stop_async_watch(mlx5_ctx);

        ibv_close_device(mlx5_ctx->ibv_ctx);
        mlx5_ctx->ibv_ctx = NULL;
    }

    return;
}
