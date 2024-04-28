# SPDX-License-Identifier: Apache-2.0
# Copyright(c) 2024 Liu, Changcheng <changcheng.liu@aliyun.com>

wod_app_dep = declare_dependency(
    include_directories: ['../include', flexio_lib_dir],
    link_with: [wod_lib])

executable('wod_app', 'wod_app.c', c_args : '-DJX=1',
    dependencies: [wod_app_dep, ibverbs_dep, mlx5_dep],
    include_directories: ['./'],
    install: true,
    install_dir: 'test')
