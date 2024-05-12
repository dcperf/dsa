# SPDX-License-Identifier: Apache-2.0
# Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>

wod_app_dep = declare_dependency(
    include_directories: ['../include', flexio_include_dir],
    link_with: [wod_lib])

mpi_loc = get_option('mpi')
mpi_header_dir = mpi_loc + '/include'
mpi_lib_dir = mpi_loc + '/lib'
mpi_dep = declare_dependency(
    link_args: ['-L' + mpi_lib_dir, '-lmpi'],
    include_directories: include_directories(mpi_header_dir))

executable('wod_app', 'wod_app.c', c_args : '-DJX=1',
    dependencies: [wod_app_dep, ibverbs_dep, mlx5_dep, mpi_dep],
    include_directories: ['./'],
    install: true,
    install_dir: 'test')
