// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>

#include <infiniband/verbs.h>

struct dpa_host_ctx;

struct app_ctx {
    char proc_name[MPI_MAX_PROCESSOR_NAME];
    uint32_t rank_id, rank_cnt;
    uint32_t msg_size;
    uint32_t iteration;

    struct mlx5_ctx mlx5_ctx;

    struct dpa_host_ctx *dpa_host_ctx;
};

#endif /* __APP_COMMON_H__ */
