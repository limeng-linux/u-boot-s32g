/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2015-2016 Freescale Semiconductor, Inc.
 * (C) Copyright 2017-2018, 2020 NXP
 *
 */

#ifndef __ASM_ARCH_CLOCK_H
#define __ASM_ARCH_CLOCK_H

#ifdef __KERNEL__
#include <common.h>
#endif

#if defined(CONFIG_S32V234)
enum mxc_clock {
	MXC_ARM_CLK = 0,
	MXC_BUS_CLK,
	MXC_PERIPHERALS_CLK,
	MXC_UART_CLK,
	MXC_USDHC_CLK,
	MXC_ESDHC_CLK,
	MXC_FEC_CLK,
	MXC_I2C_CLK,
	MXC_SYS3_CLK,
	MXC_SYS6_CLK,
	MXC_QSPI_CLK,
	MXC_DCU_PIX_CLK,
	MXC_DSPI_CLK,
	MXC_XBAR_CLK,
	MXC_DDR_CLK,
};

enum pll_type {
	ARM_PLL = 0,
	PERIPH_PLL,
	ENET_PLL,
	DDR_PLL,
	VIDEO_PLL,
};
#endif

#ifdef CONFIG_S32_GEN1
enum mxc_clock {
	MXC_UART_CLK,
	MXC_ESDHC_CLK,
	MXC_I2C_CLK,
	MXC_DSPI_CLK,
};
#endif

#ifdef __KERNEL__
#if defined(CONFIG_S32V234)
void clock_init(void);
#endif

unsigned int mxc_get_clock(enum mxc_clock clk);
void entry_to_target_mode( u32 mode );

#define imx_get_fecclk() mxc_get_clock(MXC_FEC_CLK)
#endif /* __KERNEL__ */

#endif /* __ASM_ARCH_CLOCK_H */
