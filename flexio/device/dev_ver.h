// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2020 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#ifndef __DPA_VER_H__
#define __DPA_VER_H__

#include <libflexio-dev/flexio_dev_ver.h>
#ifdef FLEXIO_DEV_VER_USED
#undef FLEXIO_DEV_VER_USED
#endif
#define FLEXIO_DEV_VER_USED FLEXIO_DEV_VER(24, 7, 0)

#include <libflexio-dev/flexio_dev.h>

#endif // !__DPA_VER_H__
