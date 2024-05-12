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
    flexio_msg_stream_attr_t stream_fattr = {};

    dpa_host_ctx->mlx5_ctx = mlx5_ctx;
    process_attr.pd = mlx5_ctx->pd;

    int32_t ret = flexio_process_create(dpa_host_ctx->mlx5_ctx->ibv_ctx, wod_app, &process_attr, &dpa_host_ctx->process);
    if (ret != 0) {
        fprintf(stderr, "failed to create dpa process\n");
        goto err_process;
    }

    ret = flexio_process_error_handler_set(dpa_host_ctx->process, dpa_dev_error_handler);
    if (ret != 0) {
        fprintf(stderr, "failed to create dpa process error handler\n");
        goto err_set_handler;
    }

    stream_fattr.uar = flexio_process_get_uar(dpa_host_ctx->process);
    stream_fattr.data_bsize = 4 * 2048;
    stream_fattr.sync_mode = FLEXIO_LOG_DEV_SYNC_MODE_SYNC;
    stream_fattr.level = FLEXIO_MSG_DEV_ALWAYS_PRINT;
    ret = flexio_msg_stream_create(dpa_host_ctx->process, &stream_fattr, stdout, NULL, &dpa_host_ctx->msg_stream);
    if (ret != 0) {
        fprintf(stderr, "failed to dpa process msg stream\n");
        goto err_create_msg_stream;
    }

    *p_dpa_host_ctx = dpa_host_ctx;
    return 0;

err_create_msg_stream:
    ;

err_set_handler:
    flexio_process_destroy(dpa_host_ctx->process);

err_process:
    return ret;
}

int32_t destroy_dpa_host_ctx(struct dpa_host_ctx *dpa_host_ctx)
{
    int32_t ret;

    ret = flexio_msg_stream_destroy(dpa_host_ctx->msg_stream);
    if (ret != 0) {
        fprintf(stderr, "failed to destroy dpa process msg stream\n");
        return -1;
    }

    ret = flexio_process_destroy(dpa_host_ctx->process);
    if (ret != 0) {
        fprintf(stderr, "failed to destroy dpa process\n");
        return -1;
    }

    free(dpa_host_ctx);

    return 0;
}
