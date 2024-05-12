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

struct data_cq {
    struct flexio_cq    *f_cq;
    struct flexio_event_handler *event_handler;
};

struct dpa_host_ctx {
    struct mlx5_ctx *mlx5_ctx;

    struct flexio_process *process;
    struct flexio_msg_stream *msg_stream;

    struct data_cq data_cq[8];

    struct flexio_qp *f_qps[];
};

#endif /* __HOST_CONTEXT__ */
