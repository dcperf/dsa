// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#include <stdint.h>
#include <errno.h>

#include <infiniband/verbs.h>
#include <infiniband/mlx5dv.h>

#include <libflexio/flexio.h>
#include <libflexio/flexio_ver.h>

#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

struct dpa_host_ctx;
struct mlx5_ctx;

extern int32_t create_dpa_host_ctx(struct mlx5_ctx* mlx5_ctx, struct dpa_host_ctx **p_dpa_host_ctx);

extern int32_t destroy_dpa_host_ctx(struct dpa_host_ctx *dpa_host_ctx);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __COMMON_H__ */
