#!/bin/bash
# -*- mode:sh; tab-width:4; sh-basic-offset:4; indent-tabs-mode:nil -*-
# vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=sh fileencoding=utf-8
#
# SPDX-License-Identifier: Apache-2.0
# Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>

set -e -x

CLEAN=''
MESONEXTARGS=''
INSTALL=''
PREFIX='/usr/local'
BUILDDIR='builddir'

FLEXIO_INCLUDE_DIR='/opt/mellanox/flexio/include'
FLEXIO_LIB_DIR='/opt/mellanox/flexio/lib'
MPI='/usr/lib/x86_64-linux-gnu/openmpi'

while [[ $# -gt 0 ]]; do
    key="$1"
    shift # past argument
    case $key in
    --clean)
        CLEAN="$key"
        ;;
    -v)
        VERBOSE="-v"
        MESONEXTARGS+=" -Dverbose=true"
        ;;
    --debug)
        MESONEXTARGS+=" -Denable_debug=true"
        ;;
    --flexio_include_path)
        FLEXIO_INCLUDE_DIR="$1"
        shift
        ;;
    --flexio_lib_path)
        FLEXIO_LIB_DIR="$1"
        shift
        ;;
    --mpi)
        MPI="$1"
        shift
        ;;
    --install)
        INSTALL="$key"
        ;;
    --prefix)
        PREFIX="$1"
        shift
        ;;
    --builddir)
        BUILDDIR="$1"
        shift
        ;;
    -h|--help)
        echo "usage: build.sh [--clean] [--flexio_include_path] [--flexio_lib_path] [--debug] [-v] "
        exit 1
        ;;
    *) # unknown option
        echo "ERROR: unknown parameter: $key"
        exit 1
        ;;
    esac
done

if [ $CLEAN ]; then
    rm -rf $BUILDDIR
fi

meson setup $BUILDDIR \
    -Dflexio_include_path=$FLEXIO_INCLUDE_DIR -Dflexio_lib_path=$FLEXIO_LIB_DIR $MESONEXTARGS -Dmpi=$MPI --prefix=$PREFIX

ninja -C $BUILDDIR $VERBOSE

if [ $INSTALL ]; then
    meson install -C $BUILDDIR
fi
