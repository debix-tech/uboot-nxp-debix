#
# Copyright 2019 NXP
# Copyright 2023 Gilles Talis <gilles.talis@gmail.com>
# Copyright 2026 Polyhex John_gao <john@polyhex.net>
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y += imx8mp_debix_model_a.o
obj-y += ../../freescale/common/

ifdef CONFIG_XPL_BUILD
obj-y += spl.o
#obj-$(CONFIG_IMX8M_LPDDR4) += lpddr4_timing.o
obj-$(CONFIG_IMX8M_LPDDR4) += lpddr4_timing_K4F6E3S4HM.o 
obj-$(CONFIG_IMX8M_LPDDR4) += lpddr4_timing_K4FBE3D4HM.o
obj-$(CONFIG_IMX8M_LPDDR4) += lpddr4_timing_D9ZCL.o
obj-$(CONFIG_IMX8M_LPDDR4) += lpddr4_timing_D8BJG.o
obj-$(CONFIG_IMX8M_LPDDR4) += lpddr4_timing-2G-1500M_H9HCNNNBPUMLHR.o
endif
