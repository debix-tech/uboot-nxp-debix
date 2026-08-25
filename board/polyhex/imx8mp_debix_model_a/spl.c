// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2018-2019, 2021 NXP
 * Copyright 2023 Gilles Talis <gilles.talis@gmail.com>
 * Copyright 2026 Polyhex John_gao <john@polyhex.net>
 */

#include <hang.h>
#include <init.h>
#include <log.h>
#include <spl.h>
#include <asm/global_data.h>
#include <asm/arch/imx8mp_pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/boot_mode.h>
#include <power/pmic.h>

#include <power/pca9450.h>
#include <asm/arch/clock.h>
#include <dm/uclass.h>
#include <dm/device.h>
#include <dm/uclass-internal.h>
#include <dm/device-internal.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <fsl_esdhc_imx.h>
#include <mmc.h>
#include <asm/arch/ddr.h>
#include <asm/sections.h>
#include "debix_lpddr4.h"
//#include <handoff.h>

DECLARE_GLOBAL_DATA_PTR;

struct debix_dram debix_drams[] = {
	{0xff060018, &dram_timing_d9zcl, 8},
	{0xff070018, NULL, 8},
	{0xff000110, &dram_timing_k4fbe3d4hm, 4},
	{0x1061010,  NULL, 4},
	{0xff070010, NULL, 4},
	{0xff041010, &dram_timing_k4f6e3d4hm, 2},
	{0x1061010,  NULL, 2},
	{0x6040008,  &dram_timing_H9HCNNNBPUMLHR, 2},
	{0xff070018, &dram_timing_d8bjg, 4},
	{0xff060018, NULL, 4},
};
int spl_board_boot_device(enum boot_device boot_dev_spl)
{
#ifdef CONFIG_SPL_BOOTROM_SUPPORT
	return BOOT_DEVICE_BOOTROM;
#else
	switch (boot_dev_spl) {
	case SD1_BOOT:
	case MMC1_BOOT:
	case SD2_BOOT:
	case MMC2_BOOT:
		return BOOT_DEVICE_MMC1;
	case SD3_BOOT:
	case MMC3_BOOT:
		return BOOT_DEVICE_MMC2;
	case QSPI_BOOT:
		return BOOT_DEVICE_NOR;
	case NAND_BOOT:
		return BOOT_DEVICE_NAND;
	case USB_BOOT:
		return BOOT_DEVICE_BOARD;
	default:
		return BOOT_DEVICE_NONE;
	}
#endif
}

//0xff041010   //Micron D9WGB
//0x1061010
u32 ddr_id=0;
u8  ddr_size=0;

void spl_dram_init(void)
{

	int i = 0 ; 
	int ret = 0;

	for(i = 0; i < ARRAY_SIZE(debix_drams); i++) {
		if (i!=0){
			// 0 => debix_lpddr4_array[D9ZCL]
                        // all ddr use the same [ddr_ddrphy_trained_csr]
                        // define in lpddr4_timing_D9ZCL.c
			if(debix_drams[i].pdram_timing != NULL)
				debix_drams[i].pdram_timing->ddrphy_trained_csr_num = debix_drams[0].pdram_timing->ddrphy_trained_csr_num;
		}
		if(debix_drams[i].pdram_timing != NULL){
			ret =  ddr_init(debix_drams[i].pdram_timing);
			if(ret) {
                        printf("DDRINFO: ddr_init fail try other\n");
                        continue;
            		}

		}
		if(ret) continue;
		ddr_id = 0;
		ddr_id |= (lpddr4_mr_read(1, 5)&0xff);
		ddr_id = (ddr_id<<8) | (lpddr4_mr_read(1, 6)&0xff);
		ddr_id = (ddr_id<<8) | (lpddr4_mr_read(1, 7)&0xff);
		ddr_id = (ddr_id<<8) | (lpddr4_mr_read(1, 8)&0xff);
		printf("DDRINFO: ID=%x checkID=%x \n", ddr_id,debix_drams[i].id);
		if(ddr_id == debix_drams[i].id){
			printf("Found Debix DDR : 0x%x(%dGB)\n", ddr_id, debix_drams[i].size);
			ddr_size = debix_drams[i].size;
			break;
		}
	}
}

#define SPL_DATA_ADDR    0x40100000
#define SPL_DATA_MAGIC   0xDEADBEEF
struct spl_shared_data {
    uint32_t magic;
    uint32_t ddr_size;
};


void spl_board_init(void)
{
	arch_misc_init();

	/*
	 * Set GIC clock to 500Mhz for OD VDD_SOC. Kernel driver does
	 * not allow to change it. Should set the clock after PMIC
	 * setting done. Default is 400Mhz (system_pll1_800m with div = 2)
	 * set by ROM for ND VDD_SOC
	 */
#if defined(CONFIG_IMX8M_LPDDR4) && !defined(CONFIG_IMX8M_VDD_SOC_850MV)
	clock_enable(CCGR_GIC, 0);
	clock_set_target_val(GIC_CLK_ROOT, CLK_ROOT_ON | CLK_ROOT_SOURCE_SEL(5));
	clock_enable(CCGR_GIC, 1);

	puts("Normal Boot\n");
#endif
#if 0
	printf("SPL: Initializing handoff mechanism\n");
    
    /* 1. 分配 handoff 结构体内存 */
    ho = malloc(sizeof(struct spl_handoff));
    if (!ho) {
        printf("SPL: Failed to allocate handoff\n");
        return;
    }
    
    /* 2. 清空结构体 */
    memset(ho, 0, sizeof(struct spl_handoff));
    
    /* 3. 设置架构相关数据 */
    arch_ho = &ho->arch;
    arch_ho->usable_ram_top = gd->ram_top;
    arch_ho->boot_device = spl_boot_device();
    arch_ho->reset_cause = readl(WDOG1_BASE_ADDR + 0x4) & 0x3;
    arch_ho->ddr_type = 0;  /* DDR4 */
    arch_ho->pmic_voltage = 0x800;  /* 1.2V */
    
    /* 4. 设置 RAM 信息 */
    ho->ram_size = gd->ram_size;
    
    /* 5. 设置 RAM bank 信息 */
    for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
        ho->ram_bank[i].start = gd->bd->bi_dram[i].start;
        ho->ram_bank[i].size = gd->bd->bi_dram[i].size;
    }
    
    /* 6. 保存到 gd */
    gd->spl_handoff = ho;
    
    /* 7. 调试输出 */
    printf("SPL: Handoff data:\n");
    printf("  - RAM top: 0x%lx\n", arch_ho->usable_ram_top);
    printf("  - Boot device: %d\n", arch_ho->boot_device);
    printf("  - Reset cause: 0x%x\n", arch_ho->reset_cause);
    printf("  - RAM size: %lld MB\n", ho->ram_size / (1024 * 1024));
    
    for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
        if (ho->ram_bank[i].size) {
            printf("  - Bank %d: start=0x%llx, size=%lld MB\n",
                   i, ho->ram_bank[i].start,
                   ho->ram_bank[i].size / (1024 * 1024));
        }
    }
#endif
    /*write ddr size */
    if(ddr_size!=0){ 
	struct spl_shared_data *data = (struct spl_shared_data *)SPL_DATA_ADDR;
	memset(data, 0, sizeof(struct spl_shared_data));
	data->magic = SPL_DATA_MAGIC;
	data->ddr_size = ddr_size; 
	printf("SPL: Wrote data to 0x%x\n", SPL_DATA_ADDR);
    	printf("  - Magic: 0x%x\n", data->magic);
    	printf("  - DDR size: %dGB\n", data->ddr_size );
    }
}

#if CONFIG_IS_ENABLED(DM_PMIC_PCA9450)
int power_init_board(void)
{
	struct udevice *dev;
	int ret;

	ret = pmic_get("pmic@25", &dev);
	if (ret == -ENODEV) {
		puts("No pmic@25\n");
		return 0;
	}
	if (ret < 0)
		return ret;

	/* BUCKxOUT_DVS0/1 control BUCK123 output */
	pmic_reg_write(dev, PCA9450_BUCK123_DVS, 0x29);

#ifdef CONFIG_IMX8M_LPDDR4
	/*
	 * Increase VDD_SOC to typical value 0.95V before first
	 * DRAM access, set DVS1 to 0.85V for suspend.
	 * Enable DVS control through PMIC_STBY_REQ and
	 * set B1_ENMODE=1 (ON by PMIC_ON_REQ=H)
	 */
	if (IS_ENABLED(CONFIG_IMX8M_VDD_SOC_850MV))
		pmic_reg_write(dev, PCA9450_BUCK1OUT_DVS0, 0x14);
	else
		pmic_reg_write(dev, PCA9450_BUCK1OUT_DVS0, 0x1C);

	pmic_reg_write(dev, PCA9450_BUCK1OUT_DVS1, 0x14);
	pmic_reg_write(dev, PCA9450_BUCK1CTRL, 0x59);

	/*
	 * Kernel uses OD/OD freq for SOC.
	 * To avoid timing risk from SOC to ARM,increase VDD_ARM to OD
	 * voltage 0.95V.
	 */

	pmic_reg_write(dev, PCA9450_BUCK2OUT_DVS0, 0x1C);
#elif defined(CONFIG_IMX8M_DDR4)
	/* DDR4 runs at 3200MTS, uses default ND 0.85v for VDD_SOC and VDD_ARM */
	pmic_reg_write(dev, PCA9450_BUCK1CTRL, 0x59);

	/* Set NVCC_DRAM to 1.2v for DDR4 */
	pmic_reg_write(dev, PCA9450_BUCK6OUT, 0x18);
#endif

	return 0;
}
#endif

#ifdef CONFIG_SPL_LOAD_FIT
int board_fit_config_name_match(const char *name)
{
	/* Just empty function now - can't decide what to choose */
	debug("%s: %s\n", __func__, name);

	return 0;
}
#endif

void board_init_f(ulong dummy)
{
	struct udevice *dev;
	int ret;

	/* Clear the BSS. */
	memset(__bss_start, 0, __bss_end - __bss_start);

	arch_cpu_init();

	board_early_init_f();

	timer_init();

	ret = spl_early_init();
	if (ret) {
		debug("spl_early_init() failed: %d\n", ret);
		hang();
	}

	ret = uclass_get_device_by_name(UCLASS_CLK,
					"clock-controller@30380000",
					&dev);
	if (ret < 0) {
		printf("Failed to find clock node. Check device tree\n");
		hang();
	}

	preloader_console_init();

	enable_tzc380();

	power_init_board();

	/* DDR initialization */
	spl_dram_init();

	board_init_r(NULL, 0);
}
