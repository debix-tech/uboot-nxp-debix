/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2017 NXP
 * Copyright 2020 Linaro 
 * Copyright 2026 Polyhex John_gao <john@polyhex.net>
 */

#ifndef __DEBIX_LPDDR4_H__
#define __DEBIX_LPDDR4_H__
u32 lpddr4_mr_read(u32 mr_rank, u32 mr_addr);
extern struct dram_timing_info dram_timing_k4f6e3d4hm;
extern struct dram_timing_info dram_timing_k4fbe3d4hm;
extern struct dram_timing_info dram_timing_d9zcl;
extern struct dram_timing_info dram_timing_d8bjg;
extern struct dram_timing_info dram_timing_H9HCNNNBPUMLHR;

// all ddr use the same [ddr_ddrphy_trained_csr]
// define in lpddr4_timing_D9ZCL.c
extern struct dram_cfg_param ddr_ddrphy_trained_csr[];

// ddrs dram
struct debix_dram {
	u32 id;
	struct dram_timing_info *pdram_timing;
	int size;
};


#endif
