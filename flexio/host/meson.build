# SPDX-License-Identifier: Apache-2.0
# Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>

wod_sources = ['context.c', 'mlx5.c', 'mem.c']

wod_inc = ['../include', flexio_include_dir]

wod_lib_dep = declare_dependency(
    link_with: [wod_dev])

wod_lib = static_library('wod', wod_sources,
    include_directories : include_directories(wod_inc),
    dependencies : [flexio_host_dep, mlx5_dep, wod_lib_dep],
    install : true)

wod_lib = shared_library('wod', wod_sources,
    native: true,
    include_directories: include_directories(wod_inc),
    dependencies : [wod_lib_dep, ibverbs_dep, mlx5_dep, flexio_host_dep],
    install : true)
