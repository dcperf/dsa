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

struct app_ctx {
    char proc_name[MPI_MAX_PROCESSOR_NAME];

    struct ibv_context *ibv_ctx;
    struct ibv_pd *pd;

    pthread_t async_thread;
};

#endif /* __APP_COMMON_H__ */
