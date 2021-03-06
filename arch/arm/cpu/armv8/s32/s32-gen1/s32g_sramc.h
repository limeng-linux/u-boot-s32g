/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright 2020-2021 NXP
 */
#ifndef S32G_SRAMC_H
#define S32G_SRAMC_H

/* SRAM is actually at 0x3400_0000; we are just mirroring it in the
 * Virtual Code RAM
 */
#define S32G_SRAM_BASE		0x34000000
#define S32G_SRAM_SIZE		0x00800000
#define S32G_SRAM_END		(S32G_SRAM_BASE + S32G_SRAM_SIZE)

/* SRAM controller is able to erase 64 bits at once */
#define SRAM_BLOCK		512
#define SRAM_BLOCK_MASK		(SRAM_BLOCK - 1)
#define SRAM_INV_BLOCK_MASK	(~(SRAM_BLOCK_MASK))

#define SRAMC0_BASE_ADDR	0x4019C000
#define SRAMC0_BASE_ADDR_H	(SRAMC0_BASE_ADDR >> 16)
#define SRAMC0_BASE_ADDR_L	((SRAMC0_BASE_ADDR & 0xFFFF))
#define SRAMC1_BASE_ADDR	0x401A0000
#define SRAMC1_BASE_ADDR_H	(SRAMC1_BASE_ADDR >> 16)
#define SRAMC1_BASE_ADDR_L	((SRAMC1_BASE_ADDR & 0xFFFF))
#ifdef CONFIG_NXP_S32G3XX
#define SRAMC2_BASE_ADDR	0x4055A000
#define SRAMC2_BASE_ADDR_H	(SRAMC0_BASE_ADDR >> 16)
#define SRAMC2_BASE_ADDR_L	((SRAMC0_BASE_ADDR & 0xFFFF))
#define SRAMC3_BASE_ADDR	0x4055E000
#define SRAMC3_BASE_ADDR_H	(SRAMC1_BASE_ADDR >> 16)
#define SRAMC3_BASE_ADDR_L	((SRAMC1_BASE_ADDR & 0xFFFF))
#endif
#define SSRAMC_BASE_ADDR	0x44028000
#define SRAMC_SIZE		0x3000

/* Block ranges */

#if defined(CONFIG_NXP_S32G2XX) || defined(CONFIG_NXP_S32R45)
#define SRAMC0_MIN_ADDR		(0x0)
#define SRAMC0_MAX_ADDR		(0x7FFF)
#define SRAMC1_MIN_ADDR		(SRAMC0_MAX_ADDR + 1)
#define SRAMC1_MAX_ADDR		(0x10000)
#elif CONFIG_NXP_S32G3XX
#define SRAMC0_MIN_ADDR		(0x0)
#define SRAMC0_MAX_ADDR		(0x7FFF)
#define SRAMC1_MIN_ADDR		(SRAMC0_MAX_ADDR + 1)
#define SRAMC1_MAX_ADDR		(0xFFFF)
#define SRAMC2_MIN_ADDR		(SRAMC1_MAX_ADDR + 1)
#define SRAMC2_MAX_ADDR		(0x17FFF)
#define SRAMC2_MAX_ADDR_H	(SRAMC2_MAX_ADDR >> 16)
#define SRAMC2_MAX_ADDR_L	(SRAMC2_MAX_ADDR & 0xFFFF)
#define SRAMC3_MIN_ADDR		(SRAMC2_MAX_ADDR + 1)
#define SRAMC3_MIN_ADDR_H	(SRAMC3_MIN_ADDR >> 16)
#define SRAMC3_MIN_ADDR_L	(SRAMC3_MIN_ADDR & 0xFFFF)
#define SRAMC3_MAX_ADDR		(0x1FFFF)
#define SRAMC3_MAX_ADDR_H	(SRAMC3_MAX_ADDR >> 16)
#define SRAMC3_MAX_ADDR_L	(SRAMC3_MAX_ADDR & 0xFFFF)
#endif

#define SRAMC_PRAMCR_OFFSET	0x0
#define SRAMC_PRAMCR_INITREQ	1
#define SRAMC_PRAMIAS_OFFSET	0x4
#define SRAMC_PRAMIAE_OFFSET	0x8
#define SRAMC_PRAMSR_OFFSET	0xC
#define SRAMC_PRAMSR_IDONE	1

#define SSRAM_MAX_ADDR		0x7FF

#endif

