// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#include <stdio.h>
#include "context.h"

extern struct flexio_app *wod_app;
extern flexio_func_t dpa_dev_error_handler;

int32_t create_dpa_host_ctx(struct mlx5_ctx* mlx5_ctx, struct dpa_host_ctx **p_dpa_host_ctx)
{
    struct dpa_host_ctx *dpa_host_ctx = calloc(1, sizeof(struct dpa_host_ctx));
    struct flexio_process_attr process_attr = {};

    dpa_host_ctx->mlx5_ctx = mlx5_ctx;
    process_attr.pd = mlx5_ctx->pd;

    int32_t ret = flexio_process_create(dpa_host_ctx->mlx5_ctx->ibv_ctx, wod_app, &process_attr, &dpa_host_ctx->process);
    if (ret != 0) {
        fprintf(stderr, "failed to create dpa process\n");
        return -1;
    }

    ret = flexio_process_error_handler_set(dpa_host_ctx->process, dpa_dev_error_handler);
    if (ret != 0) {
        fprintf(stderr, "failed to create dpa process error handler\n");
        return -1;
    }

    *p_dpa_host_ctx = dpa_host_ctx;

    return 0;
}
