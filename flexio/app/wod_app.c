// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#include "app.h"

int32_t create_app_ctx(struct app_ctx **app_ctx_p)
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

    return 0;
}

void destroy_app_ctx(struct app_ctx *app_ctx)
{
    free(app_ctx);

    return;
}

int main(int argc, char **argv)
{
    int ret = 0, rank_id, rank_cnt;
    struct app_ctx *app_ctx = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
    MPI_Comm_size(MPI_COMM_WORLD, &rank_cnt);

    ret = create_app_ctx(&app_ctx);
    if (app_ctx == NULL) {
        ret = -1;
    }

    destroy_app_ctx(app_ctx);
    app_ctx = NULL;

    return ret;
}
