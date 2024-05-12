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

int dpa_dev_cq_create(struct dpa_host_ctx *dpa_host_ctx,
    int32_t log_cq_depth, flexio_func_t *cq_handler,
    uint64_t cq_handler_arg, flexio_uintptr_t cq_dbr_daddr,
    flexio_uintptr_t cq_ring_daddr, struct dpa_generic_cq *cq)
{
    uint8_t overrun_ignore = 0;
    int ret, element_type;
    struct mlx5_cqe64 cqe[4];
    struct flexio_cq_attr cq_fattr = {0};
    struct flexio_event_handler_attr event_handler_attr = {0};
    struct flexio_thread *dpa_thread;

    if (cq_handler != NULL) {
        event_handler_attr.host_stub_func = cq_handler;
        event_handler_attr.affinity.type = FLEXIO_AFFINITY_NONE;
        event_handler_attr.arg = cq_handler_arg;

        ret = flexio_event_handler_create(dpa_host_ctx->process, &event_handler_attr, &cq->event_handler);
        if (ret != 0) {
            fprintf(stderr, "failed to create flexio cq with handler\n");
        }
        element_type = FLEXIO_CQ_ELEMENT_TYPE_DPA_THREAD;
        dpa_thread = flexio_event_handler_get_thread(cq->event_handler);
    } else {
        cq->event_handler = (struct flexio_event_handler *)(-1);
        element_type = FLEXIO_CQ_ELEMENT_TYPE_NON_DPA_CQ;
        dpa_thread = NULL;
        overrun_ignore = 1;
    }

    ret = flexio_buf_dev_memset(dpa_host_ctx->process, 0, sizeof(uint64_t), cq_dbr_daddr);
    if (ret != 0) {
        fprintf(stderr, "failed to clear cq dbr\n");
    }

    /* Init CQEs and set ownership bit */
    for (uint32_t i  = 0; i < 4; i++)
        mlx5dv_set_cqe_owner(&cqe[i], 1);

    /* Copy CQEs from host to Flex IO CQ ring */
        ret = flexio_host2dev_memcpy(dpa_host_ctx->process, cqe, sizeof(cqe), cq_ring_daddr);

    cq_fattr.cq_dbr_daddr = cq_dbr_daddr;
    cq_fattr.cq_ring_qmem.daddr = cq_ring_daddr;

    cq_fattr.log_cq_depth = log_cq_depth;

    cq_fattr.element_type = element_type;
    cq_fattr.uar_id = flexio_uar_get_id(flexio_process_get_uar(dpa_host_ctx->process));
    cq_fattr.uar_base_addr = NULL;

    cq_fattr.thread = dpa_thread;
    cq_fattr.overrun_ignore = overrun_ignore;

    ret = flexio_cq_create(dpa_host_ctx->process, dpa_host_ctx->mlx5_ctx->ibv_ctx, &cq_fattr, &cq->f_cq);
    if (ret != 0) {
        fprintf(stderr, "failed to create cq\n");
    }

    return 0;
}

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

    ret = basic_devmem_alloc(dpa_host_ctx);

    for (uint32_t i = 0; i < 8; i++) {
        ret = dpa_dev_cq_create(dpa_host_ctx, 2, NULL, 0,
                dpa_host_ctx->worker_ctx + i * sizeof(struct worker_ctx) + offsetof(struct worker_ctx, data_cq_sq_dbr),
                dpa_host_ctx->worker_ctx + i * sizeof(struct worker_ctx) + offsetof(struct worker_ctx, data_cq_buf), &dpa_host_ctx->data_cq[i]);
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

    for (uint32_t i = 0; i < 8; i++) {
        struct dpa_generic_cq *data_cq = &dpa_host_ctx->data_cq[i];
        ret = flexio_cq_destroy(data_cq->f_cq);
        if (ret != 0) {
            fprintf(stderr, "failed to destroy data_cq[%d]\n", i);
        }

        if (data_cq->event_handler != (struct flexio_event_handler *)(-1)) {
            ret = flexio_event_handler_destroy(data_cq->event_handler);
            if (ret != 0) {
                fprintf(stderr, "failed to create data_cq[%d].handler\n", i);
            }
        }
    }

    ret = basic_devmem_free(dpa_host_ctx);

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
