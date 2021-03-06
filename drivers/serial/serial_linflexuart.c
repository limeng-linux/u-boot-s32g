// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2013-2016 Freescale Semiconductor, Inc.
 * Copyright 2017, 2019, 2021 NXP
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <watchdog.h>
#include <asm/io.h>
#include <serial.h>
#include <linux/compiler.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/clock.h>

#define LINCR1_INIT			BIT(0)
#define LINCR1_MME			BIT(4)
/* This bit is marked as Reserved on S32GEN1 */
#ifdef CONFIG_S32V234
#define LINCR1_BF			BIT(7)
#endif
#define LINSR_LINS_INITMODE		(0x00001000)
#define LINSR_LINS_MASK			(0x0000F000)
#define UARTCR_UART			BIT(0)
#define UARTCR_WL0			BIT(1)
#define UARTCR_PC0			BIT(3)
#define UARTCR_TXEN			BIT(4)
#define UARTCR_RXEN			BIT(5)
#define UARTCR_PC1			BIT(6)
#define UARTCR_TFBM			BIT(8)
#define UARTCR_RFBM			BIT(9)
#define UARTSR_DTF			BIT(1)
#define UARTSR_RFE			BIT(2)
#define UARTSR_RMB			BIT(9)
#define LINFLEXD_UARTCR_OSR_MASK	(0xF<<24)
#define LINFLEXD_UARTCR_OSR(uartcr)	(((uartcr) \
					& LINFLEXD_UARTCR_OSR_MASK) >> 24)

#define LINFLEXD_UARTCR_ROSE		BIT(23)
#define LINFLEX_LDIV_MULTIPLIER		(16)

DECLARE_GLOBAL_DATA_PTR;

static u32 linflex_ldiv_multiplier(struct linflex_fsl *base)
{
	u32 mul = LINFLEX_LDIV_MULTIPLIER;
	u32 cr;

	cr = __raw_readl(&base->uartcr);

	if (cr & LINFLEXD_UARTCR_ROSE)
		mul = LINFLEXD_UARTCR_OSR(cr);

	return mul;
}

static void _linflex_serial_setbrg(struct linflex_fsl *base, int baudrate)
{
	u32 clk = mxc_get_clock(MXC_UART_CLK);
	u32 ibr, fbr, divisr, dividr;

	if (!baudrate)
		baudrate = CONFIG_BAUDRATE;

	divisr = clk;
	dividr = (u32)(gd->baudrate * linflex_ldiv_multiplier(base));

	ibr = (u32)(divisr / dividr);
	fbr = (u32)((divisr % dividr) * 16 / dividr) & 0xF;

	__raw_writel(ibr, &base->linibrr);
	__raw_writel(fbr, &base->linfbrr);
}

static int _linflex_serial_getc(struct linflex_fsl *base)
{
	while (__raw_readl(&base->uartsr) & UARTSR_RFE == UARTSR_RFE)
		;

	return __raw_readb(&base->bdrm);
}

static int _linflex_serial_putc(struct linflex_fsl *base, const char c)
{
	/* wait for Tx FIFO not full */
	while (__raw_readb(&base->uartsr) & UARTSR_DTF)
		;

	__raw_writeb(c, &base->bdrl);
}

/*
 * Initialise the serial port with the given baudrate. The settings
 * are always 8 data bits, no parity, 1 stop bit, no start bits.
 */
static int _linflex_serial_init(struct linflex_fsl *base)
{
	volatile u32 ctrl;

	/* set the Linflex in master|init mode and activate by-pass filter
	 * (where supported) */
	ctrl = LINCR1_MME | LINCR1_INIT;
#ifdef CONFIG_S32V234
	ctrl |= LINCR1_BF;
#endif
	__raw_writel(ctrl, &base->lincr1);

	/* waiting for init mode entry - TODO: add a timeout */
	while ((__raw_readl(&base->linsr) & LINSR_LINS_MASK) !=
	       LINSR_LINS_INITMODE);

	/* set UART bit to allow writing other bits */
	__raw_writel(UARTCR_UART, &base->uartcr);

#ifdef VIRTUAL_PLATFORM
	/* Set preset timeout register value. Otherwise, print is very slow. */
	__raw_writel(0xf, &base->uartpto);
#endif

	/* 8 bit data, no parity, Tx and Rx enabled, UART mode */
	__raw_writel(UARTCR_PC1 | UARTCR_RXEN | UARTCR_TXEN | UARTCR_PC0
		     | UARTCR_WL0 | UARTCR_UART | UARTCR_RFBM | UARTCR_TFBM,
		     &base->uartcr);

	/* provide data bits, parity, stop bit, etc */
	serial_setbrg();

	ctrl = __raw_readl(&base->lincr1);
	ctrl &= ~LINCR1_INIT;
	__raw_writel(ctrl, &base->lincr1);	/* end init mode */

	return 0;
}

struct linflex_serial_platdata {
	struct linflex_fsl *base_addr;
	u8 port_id; /* do we need this? */
};

struct linflex_serial_priv {
	struct linflex_fsl *lfuart;
};

int linflex_serial_setbrg(struct udevice *dev, int baudrate)
{
	struct linflex_serial_priv *priv = dev_get_priv(dev);

	_linflex_serial_setbrg(priv->lfuart, baudrate);

	return 0;
}

static int linflex_serial_getc(struct udevice *dev)
{
	struct linflex_serial_priv *priv = dev_get_priv(dev);

	return _linflex_serial_getc(priv->lfuart);
}

static int linflex_serial_putc(struct udevice *dev, const char ch)
{

	struct linflex_serial_priv *priv = dev_get_priv(dev);

	return _linflex_serial_putc(priv->lfuart, ch);
}

static int linflex_serial_pending(struct udevice *dev, bool input)
{
	struct linflex_serial_priv *priv = dev_get_priv(dev);
	uint32_t uartsr = __raw_readl(&priv->lfuart->uartsr);

	if (input)
		return ((uartsr & UARTSR_RFE) && (uartsr & UARTSR_RMB)) ? 1 : 0;
	else
		return uartsr & UARTSR_DTF ? 0 : 1;
}

static void linflex_serial_init_internal(struct linflex_fsl *lfuart)
{
	_linflex_serial_init(lfuart);
	_linflex_serial_setbrg(lfuart, CONFIG_BAUDRATE);
	return;
}

static int linflex_serial_probe(struct udevice *dev)
{
	struct linflex_serial_platdata *plat = dev->platdata;
	struct linflex_serial_priv *priv = dev_get_priv(dev);

	priv->lfuart = (struct linflex_fsl *)plat->base_addr;
	linflex_serial_init_internal(priv->lfuart);

	return 0;
}

static const struct dm_serial_ops linflex_serial_ops = {
	.putc = linflex_serial_putc,
	.pending = linflex_serial_pending,
	.getc = linflex_serial_getc,
	.setbrg = linflex_serial_setbrg,
};

U_BOOT_DRIVER(serial_linflex) = {
	.name	= "serial_linflex",
	.id	= UCLASS_SERIAL,
	.probe = linflex_serial_probe,
	.ops	= &linflex_serial_ops,
	.flags = DM_FLAG_PRE_RELOC,
	.priv_auto_alloc_size	= sizeof(struct linflex_serial_priv),
};

#ifdef CONFIG_DEBUG_UART_LINFLEXUART

#include <debug_uart.h>


static inline void _debug_uart_init(void)
{
	struct linflex_fsl *base = (struct linflex_fsl *)CONFIG_DEBUG_UART_BASE;

	linflex_serial_init_internal(base);
}

static inline void _debug_uart_putc(int ch)
{
	struct linflex_fsl *base = (struct linflex_fsl *)CONFIG_DEBUG_UART_BASE;

	/* XXX: Is this OK? Should this use the non-DM version? */
	_linflex_serial_putc(base, ch);
}

DEBUG_UART_FUNCS

#endif /* CONFIG_DEBUG_UART_LINFLEXUART */
