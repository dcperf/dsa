// -*- mode:C; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab ff=unix ft=c fileencoding=utf-8
/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright(c) 2020 Liu, Changcheng <changcheng.liu@aliyun.com>
 */

#ifndef __DPA_WOD_DEV_H__
#define __DPA_WOD_DEV_H__

#include <stdint.h>

// Send WQE sizes (in octowords)
enum {
    MLX5_CTRL_SEG_DS_WOD            = 0x3,
    MLX5_CTRL_SEG_DS_RDMA_WRITE     = 0x3,
    MLX5_CTRL_SEG_DS_ATOMIC_FETCH_AND_ADD = 0x4
};

// Send WQE opcodes modifiers
enum {
    MLX5_CTRL_SEG_OPC_MOD_WAIT_ON_DATA = 0x1,
};

// wait-on-data operations
enum {
    WAIT_ON_DATA_OP_ALWAYS_TRUE    = 0x0,
    WAIT_ON_DATA_OP_EQUAL          = 0x1,
    WAIT_ON_DATA_OP_BIGGER         = 0x2,
    WAIT_ON_DATA_OP_SMALLER        = 0x3,
    WAIT_ON_DATA_OP_CYCLIC_BIGGER  = 0x4,
    WAIT_ON_DATA_OP_CYCLIC_SMALLER = 0x5,
};

// Wait-on-Data segment PRM layout
struct dpa_wqe_prm_wod_seg {
    uint32_t op_inv;           // Compare operation
    uint32_t mkey;             // Mkey
    uint64_t va_63_3_fail_act; // Mkey's virtual address and fail operation
    uint64_t data;             // Actual data for comparison
    uint64_t dmask;            // Mask for data
};

// Error type CQE
struct dpa_mlx5_err_cqe64 {
    uint8_t  rsvd0[32];
    uint32_t srqn;
    uint8_t  rsvd1[16];
    uint8_t  hw_error_syndrome;
    uint8_t  hw_syndrome_type;
    uint8_t  vendor_err_synd;
    uint8_t  syndrome;
    uint32_t s_wqe_opcode_qpn;
    uint16_t wqe_counter;
    uint8_t  signature;
    uint8_t  op_own;
};

void flexio_format_trig_wod(uint64_t loc, uint32_t lkey, uint8_t op,
        uint8_t inv, uint32_t ce, uint64_t data, uint64_t mask);

#endif // !__DPA_WOD_DEV_H__
