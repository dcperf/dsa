// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#ifndef __HOST_CONTEXT__
#define __HOST_CONTEXT__

#include <stdint.h>
#include <infiniband/verbs.h>
#include <infiniband/mlx5dv.h>
#include <libflexio/flexio.h>

#include <common.h>
#include <mlx5.h>

struct dpa_generic_cq {
    struct flexio_cq    *f_cq;
    struct flexio_event_handler *event_handler;
};

struct worker_ctx {
    struct mlx5_cqe64 data_cq_buf[4];
    uint32_t data_cq_sq_dbr;
    uint32_t data_cq_rq_dbr;
    uint32_t data_cq_hw_owner_bit;
    uint32_t data_cq_idx;

    uint8_t data_sq_buf[4 * 64];

    uint32_t data_qpn;
};

struct dpa_host_ctx {
    struct mlx5_ctx *mlx5_ctx;

    struct flexio_process *process;
    struct flexio_msg_stream *msg_stream;

    struct dpa_generic_cq data_cq[8];

    struct flexio_qp *data_qp[8];

    struct flexio_mkey  *base_mkey;
    flexio_uintptr_t worker_ctx; // worker_ctx[8];
};

extern int basic_devmem_alloc(struct dpa_host_ctx *dpa_host_ctx);

extern int basic_devmem_free(struct dpa_host_ctx *dpa_host_ctx);

#endif /* __HOST_CONTEXT__ */
