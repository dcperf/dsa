// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#ifndef __MLX5_CTX__
#define __MLX5_CTX__

#include <stdlib.h>
#include <infiniband/verbs.h>
#include <infiniband/mlx5dv.h>

struct mlx5_ctx {
    struct ibv_context *ibv_ctx;
    struct ibv_pd *pd;
    struct ibv_port_attr port_attr;
    union ibv_gid       gid;

    pthread_t async_thread;
};

extern void create_mlx5_ctx(struct mlx5_ctx *mlx5_ctx, const char *dev_name);
extern void destroy_mlx5_ctx(struct mlx5_ctx *mlx5_ctx);

#endif /* __MLX5_CTX__ */
