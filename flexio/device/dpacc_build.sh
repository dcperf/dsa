#!/bin/bash
# -*- mode:sh; tab-width:4; sh-basic-offset:4; indent-tabs-mode:nil -*-
# vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=sh fileencoding=utf-8
#
# SPDX-License-Identifier: Apache-2.0
# Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>

set -e -x

#wod_dev_srcdir
SRCDIR=$1
COMMON_INCLUDE_DIR=$1/../include

#wod_builddir
BUILDDIR=$2

#flexio_include_dir
FLEXIO_INCLUDE_DIR=$3

#flexio_lib_dir
FLEXIO_LIB_DIR=$4

DEBUGOPT=$5

OPT_FLAGS=''
DBG_FLAGS=''

if [ "$DEBUGOPT" == "DEBUG_OFF" ]; then
    OPT_FLAGS+='-O3 -flto'
else
    DBG_FLAGS+='-DDEBUG -g -O0'
fi

dpacc --hostcc=gcc $SRCDIR/wod_dev.c $SRCDIR/dev_context.c \
    -o $BUILDDIR/device/libwod_dev.a \
    --devicecc-options="$OPT_FLAGS $DBG_FLAGS -I$COMMON_INCLUDE_DIR -I$FLEXIO_INCLUDE_DIR" \
    --hostcc-options="-fPIC" \
    --device-libs="-L$FLEXIO_LIB_DIR" \
    --app-name=wod_app
