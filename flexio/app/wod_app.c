// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#include <common.h>
#include <mlx5.h>

#include "app.h"

int32_t create_app_ctx(struct app_ctx **app_ctx_p, uint32_t rank_id, uint32_t rank_cnt)
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

    app_ctx->rank_id = rank_id;
    app_ctx->rank_cnt = rank_cnt;

    create_mlx5_ctx(&app_ctx->mlx5_ctx, "mlx5_0");

    return 0;
}

void destroy_app_ctx(struct app_ctx *app_ctx)
{
    destroy_mlx5_ctx(&app_ctx->mlx5_ctx);

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

    ret = create_app_ctx(&app_ctx, rank_id, rank_cnt);
    if (app_ctx == NULL) {
        ret = -1;
    }
    cfg_run_info(app_ctx, argc, argv);

    create_dpa_host_ctx(&app_ctx->mlx5_ctx, &app_ctx->dpa_host_ctx);

    destroy_app_ctx(app_ctx);
    app_ctx = NULL;

    return ret;
}
