# SPDX-License-Identifier: Apache-2.0
# Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>

wod_builddir = meson.build_root()
wod_dev_srcdir = meson.current_source_dir()

wod_debug = get_option('enable_debug') ? 'DEBUG_ON' : 'DEBUG_OFF'

wod_dev = custom_target('wod_dev',
    command: ['./dpacc_build.sh', wod_dev_srcdir, wod_builddir, flexio_include_dir, flexio_lib_dir, wod_debug],
    output: 'libwod_dev.a',
    build_by_default: true)
