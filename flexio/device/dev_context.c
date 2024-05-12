// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2020 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#include <libflexio-dev/flexio_dev.h>
#include <libflexio-dev/flexio_dev_err.h>

__dpa_global__ void dpa_dev_error_handler(uint64_t arg1, uint64_t arg2)
{
    struct flexio_dev_thread_ctx *thread_ctx = (struct flexio_dev_thread_ctx *)arg1;

    flexio_dev_print("DPA Error handler! (err = %lu)\n", flexio_dev_get_errno(thread_ctx));
    flexio_dev_thread_reschedule();
}
