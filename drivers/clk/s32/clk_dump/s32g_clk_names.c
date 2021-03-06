// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright 2020-2021 NXP
 */
#include <dt-bindings/clock/s32g-clock.h>
#include <linux/kernel.h>
#include <s32gen1_clk_dump.h>

#define S32G_CLK_PREFIX	"S32G_CLK_"

#define CLK_NAME(ID_DEF)     \
	S32GEN1_CLK_NAME_INIT(ID_DEF, #ID_DEF)

static struct s32gen1_clk_blk s32g_clk_blks[] = {
	CLK_NAME(S32GEN1_CLK_PER),
	CLK_NAME(S32GEN1_CLK_CAN_PE),
	CLK_NAME(S32G_CLK_ACCEL_PLL_PHI0),
	CLK_NAME(S32G_CLK_ACCEL_PLL_PHI1),
	CLK_NAME(S32G_CLK_SERDES0_LANE1_CDR),
	CLK_NAME(S32G_CLK_SERDES0_LANE1_TX),
	CLK_NAME(S32G_CLK_PFE_MAC0_EXT_TX),
	CLK_NAME(S32G_CLK_PFE_MAC0_EXT_RX),
	CLK_NAME(S32G_CLK_PFE_MAC0_EXT_REF),
	CLK_NAME(S32G_CLK_PFE_MAC1_EXT_TX),
	CLK_NAME(S32G_CLK_PFE_MAC1_EXT_RX),
	CLK_NAME(S32G_CLK_PFE_MAC1_EXT_REF),
	CLK_NAME(S32G_CLK_PFE_MAC2_EXT_TX),
	CLK_NAME(S32G_CLK_PFE_MAC2_EXT_RX),
	CLK_NAME(S32G_CLK_PFE_MAC2_EXT_REF),
	CLK_NAME(S32G_CLK_SERDES1_LANE0_TX),
	CLK_NAME(S32G_CLK_SERDES1_LANE0_CDR),
	CLK_NAME(S32G_CLK_PFE_MAC0_REF_DIV),
	CLK_NAME(S32G_CLK_PFE_MAC1_REF_DIV),
	CLK_NAME(S32G_CLK_PFE_MAC2_REF_DIV),
	CLK_NAME(S32G_CLK_SERDES1_LANE1_TX),
	CLK_NAME(S32G_CLK_SERDES1_LANE1_CDR),
	/* PFE_MAC0 */
	CLK_NAME(S32G_CLK_PFE_MAC0_RX),
	CLK_NAME(S32G_CLK_PFE_MAC0_TX_DIV),
	CLK_NAME(S32G_CLK_MC_CGM2_MUX1),
	CLK_NAME(S32G_CLK_MC_CGM2_MUX4),
	CLK_NAME(S32G_CLK_MC_CGM2_MUX7),
	/* PFE_MAC1 */
	CLK_NAME(S32G_CLK_PFE_MAC1_RX),
	CLK_NAME(S32G_CLK_PFE_MAC1_TX),
	CLK_NAME(S32G_CLK_MC_CGM2_MUX2),
	CLK_NAME(S32G_CLK_MC_CGM2_MUX5),
	CLK_NAME(S32G_CLK_MC_CGM2_MUX8),
	/* PFE_MAC2 */
	CLK_NAME(S32G_CLK_PFE_MAC2_RX),
	CLK_NAME(S32G_CLK_PFE_MAC2_TX),
	CLK_NAME(S32G_CLK_MC_CGM2_MUX3),
	CLK_NAME(S32G_CLK_MC_CGM2_MUX6),
	CLK_NAME(S32G_CLK_MC_CGM2_MUX9),
	CLK_NAME(S32G_CLK_MC_CGM2_MUX0),
	CLK_NAME(S32G_CLK_PFE_SYS),
	CLK_NAME(S32G_CLK_PFE_PE),
};

const char *plat_clk_name(const char *name)
{
	static size_t s32gen1_len = sizeof(S32GEN1_CLK_PREFIX) - 1;
	static size_t s32g_len = sizeof(S32G_CLK_PREFIX) - 1;

	if (!name)
		return NULL;

	if (!strncmp(name, S32GEN1_CLK_PREFIX, s32gen1_len))
		return name + s32gen1_len;

	return name + s32g_len;
}

__weak struct s32gen1_clk_blk *s32g_get_plat_clk_blk(u32 id)
{
	return NULL;
}

struct s32gen1_clk_blk *s32gen1_get_plat_clk_blk(u32 id)
{
	if (id < ARRAY_SIZE(s32g_clk_blks))
		return &s32g_clk_blks[id];

	id -= ARRAY_SIZE(s32g_clk_blks);

	return s32g_get_plat_clk_blk(id);
}

const char *s32gen1_get_plat_clock_name(struct s32gen1_clk_blk *clk)
{
	if (!clk)
		return NULL;

	return plat_clk_name(clk->name);
}

