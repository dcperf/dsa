// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#include "context.h"

int basic_devmem_alloc(struct dpa_host_ctx *dpa_host_ctx)
{
    int32_t ret;
    uint32_t devmem_size;
    struct flexio_mkey_attr mkey_attr = {};
    flexio_uintptr_t alloc_addr;

    devmem_size = sizeof(struct worker_ctx) * 8;
    ret = flexio_buf_dev_alloc(dpa_host_ctx->process, devmem_size, &alloc_addr);

    mkey_attr.pd = dpa_host_ctx->mlx5_ctx->pd;
    mkey_attr.daddr = alloc_addr;
    mkey_attr.len = devmem_size;
    mkey_attr.access = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE;

    ret = flexio_device_mkey_create(dpa_host_ctx->process, &mkey_attr, &dpa_host_ctx->base_mkey);
    if (ret != 0) {
        fprintf(stderr, "failed to create allocted buf mkey\n");
        return ret;
    }

    dpa_host_ctx->worker_ctx = alloc_addr;

    return 0;
}

int32_t basic_devmem_free(struct dpa_host_ctx *dpa_host_ctx)
{
    int32_t ret;

    ret = flexio_device_mkey_destroy(dpa_host_ctx->base_mkey);
    if (ret != 0) {
        fprintf(stderr, "failed to destroy flexio allocted buf mkey\n");
    }

    ret = flexio_buf_dev_free(dpa_host_ctx->process, dpa_host_ctx->worker_ctx);
    if (ret != 0) {
        fprintf(stderr, "failed to free flexio allocted buf\n");
    }

    return ret;
}
