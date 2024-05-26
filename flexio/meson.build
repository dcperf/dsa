# SPDX-License-Identifier: Apache-2.0
# Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>

project('flexio_tcs', 'c',
    version : '0.0.1')

flexio_include_dir = get_option('flexio_include_path')
flexio_lib_dir = get_option('flexio_lib_path')
flexio_host_dep = declare_dependency(
    link_args: ['-L' + flexio_lib_dir, '-lflexio'],
    include_directories: [flexio_include_dir])

ibverbs_dep = dependency('libibverbs')
mlx5_dep = dependency('libmlx5')

compilation_opts = get_option('enable_debug') ? '-DDEBUG -g -O0' : '-O3'
add_global_arguments(compilation_opts, language : 'c')

subdir('include')

subdir('device')
subdir('host')

subdir('app')
