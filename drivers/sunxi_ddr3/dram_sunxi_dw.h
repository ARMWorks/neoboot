/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * sun8i H3 platform dram controller register and constant defines
 *
 * (C) Copyright 2007-2015 Allwinner Technology Co.
 *                         Jerry Wang <wangflord@allwinnertech.com>
 * (C) Copyright 2015      Vishnu Patekar <vishnupatekar0510@gmail.com>
 * (C) Copyright 2014-2015 Hans de Goede <hdegoede@redhat.com>
 * (C) Copyright 2015      Jens Kuske <jenskuske@gmail.com>
 */

#ifndef _SUNXI_DRAM_SUN8I_H3_H
#define _SUNXI_DRAM_SUN8I_H3_H

#include <stdint.h>

#define SUNXI_DRAM_COM_BASE		0x01c62000
#define SUNXI_DRAM_CTL0_BASE		0x01c63000
#define SUNXI_DRAM_CTL1_BASE		0x01c64000
#define SUNXI_DRAM_PHY0_BASE		0x01c65000
#define SUNXI_DRAM_PHY1_BASE		0x01c66000

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

struct sunxi_mctl_com_reg {
	uint32_t cr;			/* 0x00 control register */
	uint32_t cr_r1;		/* 0x04 rank 1 control register (R40 only) */
	uint8_t res0[0x4];		/* 0x08 */
	uint32_t tmr;		/* 0x0c (unused on H3) */
	uint32_t mcr[16][2];		/* 0x10 */
	uint32_t bwcr;		/* 0x90 bandwidth control register */
	uint32_t maer;		/* 0x94 master enable register */
	uint32_t mapr;		/* 0x98 master priority register */
	uint32_t mcgcr;		/* 0x9c */
	uint32_t cpu_bwcr;		/* 0xa0 */
	uint32_t gpu_bwcr;		/* 0xa4 */
	uint32_t ve_bwcr;		/* 0xa8 */
	uint32_t disp_bwcr;		/* 0xac */
	uint32_t other_bwcr;		/* 0xb0 */
	uint32_t total_bwcr;		/* 0xb4 */
	uint8_t res1[0x8];		/* 0xb8 */
	uint32_t swonr;		/* 0xc0 */
	uint32_t swoffr;		/* 0xc4 */
	uint8_t res2[0x8];		/* 0xc8 */
	uint32_t cccr;		/* 0xd0 */
	uint8_t res3[0x54];		/* 0xd4 */
	uint32_t mdfs_bwlr[3];	/* 0x128 (unused on H3) */
	uint8_t res4[0x6cc];		/* 0x134 */
	uint32_t protect;		/* 0x800 */
};

#define MCTL_CR_BL8		(0x4 << 20)

#define MCTL_CR_1T		(0x1 << 19)
#define MCTL_CR_2T		(0x0 << 19)

#define MCTL_CR_LPDDR3		(0x7 << 16)
#define MCTL_CR_LPDDR2		(0x6 << 16)
#define MCTL_CR_DDR3		(0x3 << 16)
#define MCTL_CR_DDR2		(0x2 << 16)

#define MCTL_CR_SEQUENTIAL	(0x1 << 15)
#define MCTL_CR_INTERLEAVED	(0x0 << 15)

#define MCTL_CR_FULL_WIDTH	(0x1 << 12)
#define MCTL_CR_HALF_WIDTH	(0x0 << 12)
#define MCTL_CR_BUS_FULL_WIDTH(x)	((x) << 12)

#define MCTL_CR_PAGE_SIZE(x)	((fls(x) - 4) << 8)
#define MCTL_CR_ROW_BITS(x)	(((x) - 1) << 4)
#define MCTL_CR_EIGHT_BANKS	(0x1 << 2)
#define MCTL_CR_FOUR_BANKS	(0x0 << 2)
#define MCTL_CR_DUAL_RANK	(0x1 << 0)
#define MCTL_CR_SINGLE_RANK	(0x0 << 0)

/*
 * CR_R1 is a register found in the R40's DRAM controller. It sets various
 * parameters for rank 1. Bits [11:0] have the same meaning as the bits in
 * MCTL_CR, but they apply to rank 1 only. This implies we can have
 * different chips for rank 1 than rank 0.
 *
 * As address line A15 and CS1 chip select for rank 1 are muxed on the same
 * pin, if single rank is used, A15 must be muxed in.
 */
#define MCTL_CR_R1_MUX_A15	(0x1 << 21)

#define PROTECT_MAGIC		(0x94be6fa3)

struct sunxi_mctl_ctl_reg {
	uint32_t pir;		/* 0x00 PHY initialization register */
	uint32_t pwrctl;		/* 0x04 */
	uint32_t mrctrl;		/* 0x08 */
	uint32_t clken;		/* 0x0c */
	uint32_t pgsr[2];		/* 0x10 PHY general status registers */
	uint32_t statr;		/* 0x18 */
	uint8_t res1[0x10];		/* 0x1c */
	uint32_t lp3mr11;		/* 0x2c */
	uint32_t mr[4];		/* 0x30 mode registers */
	uint32_t pllgcr;		/* 0x40 */
	uint32_t ptr[5];		/* 0x44 PHY timing registers */
	uint32_t dramtmg[9];		/* 0x58 DRAM timing registers */
	uint32_t odtcfg;		/* 0x7c */
	uint32_t pitmg[2];		/* 0x80 PHY interface timing registers */
	uint8_t res2[0x4];		/* 0x88 */
	uint32_t rfshctl0;		/* 0x8c */
	uint32_t rfshtmg;		/* 0x90 refresh timing */
	uint32_t rfshctl1;		/* 0x94 */
	uint32_t pwrtmg;		/* 0x98 */
	uint8_t res3[0x1c];		/* 0x9c */
	uint32_t vtfcr;		/* 0xb8 (unused on H3) */
	uint32_t dqsgmr;		/* 0xbc */
	uint32_t dtcr;		/* 0xc0 */
	uint32_t dtar[4];		/* 0xc4 */
	uint32_t dtdr[2];		/* 0xd4 */
	uint32_t dtmr[2];		/* 0xdc */
	uint32_t dtbmr;		/* 0xe4 */
	uint32_t catr[2];		/* 0xe8 */
	uint32_t dtedr[2];		/* 0xf0 */
	uint8_t res4[0x8];		/* 0xf8 */
	uint32_t pgcr[4];		/* 0x100 PHY general configuration registers */
	uint32_t iovcr[2];		/* 0x110 */
	uint32_t dqsdr;		/* 0x118 */
	uint32_t dxccr;		/* 0x11c */
	uint32_t odtmap;		/* 0x120 */
	uint32_t zqctl[2];		/* 0x124 */
	uint8_t res6[0x14];		/* 0x12c */
	uint32_t zqcr;		/* 0x140 ZQ control register */
	uint32_t zqsr;		/* 0x144 ZQ status register */
	uint32_t zqdr[3];		/* 0x148 ZQ data registers */
	uint8_t res7[0x6c];		/* 0x154 */
	uint32_t sched;		/* 0x1c0 */
	uint32_t perfhpr[2];		/* 0x1c4 */
	uint32_t perflpr[2];		/* 0x1cc */
	uint32_t perfwr[2];		/* 0x1d4 */
	uint8_t res8[0x24];		/* 0x1dc */
	uint32_t acmdlr;		/* 0x200 AC master delay line register */
	uint32_t aclcdlr;		/* 0x204 AC local calibrated delay line register */
	uint32_t aciocr;		/* 0x208 AC I/O configuration register */
	uint8_t res9[0x4];		/* 0x20c */
	uint32_t acbdlr[31];		/* 0x210 AC bit delay line registers */
	uint8_t res10[0x74];		/* 0x28c */
	struct {		/* 0x300 DATX8 modules*/
		uint32_t mdlr;		/* 0x00 master delay line register */
		uint32_t lcdlr[3];		/* 0x04 local calibrated delay line registers */
		uint32_t bdlr[11];		/* 0x10 bit delay line registers */
		uint32_t sdlr;		/* 0x3c output enable bit delay registers */
		uint32_t gtr;		/* 0x40 general timing register */
		uint32_t gcr;		/* 0x44 general configuration register */
		uint32_t gsr[3];		/* 0x48 general status registers */
		uint8_t res0[0x2c];		/* 0x54 */
	} dx[4];
	uint8_t res11[0x388];	/* 0x500 */
	uint32_t upd2;		/* 0x888 */
};

#define PTR3_TDINIT1(x)		((x) << 20)
#define PTR3_TDINIT0(x)		((x) <<  0)

#define PTR4_TDINIT3(x)		((x) << 20)
#define PTR4_TDINIT2(x)		((x) <<  0)

#define DRAMTMG0_TWTP(x)	((x) << 24)
#define DRAMTMG0_TFAW(x)	((x) << 16)
#define DRAMTMG0_TRAS_MAX(x)	((x) <<  8)
#define DRAMTMG0_TRAS(x)	((x) <<  0)

#define DRAMTMG1_TXP(x)		((x) << 16)
#define DRAMTMG1_TRTP(x)	((x) <<  8)
#define DRAMTMG1_TRC(x)		((x) <<  0)

#define DRAMTMG2_TCWL(x)	((x) << 24)
#define DRAMTMG2_TCL(x)		((x) << 16)
#define DRAMTMG2_TRD2WR(x)	((x) <<  8)
#define DRAMTMG2_TWR2RD(x)	((x) <<  0)

#define DRAMTMG3_TMRW(x)	((x) << 16)
#define DRAMTMG3_TMRD(x)	((x) << 12)
#define DRAMTMG3_TMOD(x)	((x) <<  0)

#define DRAMTMG4_TRCD(x)	((x) << 24)
#define DRAMTMG4_TCCD(x)	((x) << 16)
#define DRAMTMG4_TRRD(x)	((x) <<  8)
#define DRAMTMG4_TRP(x)		((x) <<  0)

#define DRAMTMG5_TCKSRX(x)	((x) << 24)
#define DRAMTMG5_TCKSRE(x)	((x) << 16)
#define DRAMTMG5_TCKESR(x)	((x) <<  8)
#define DRAMTMG5_TCKE(x)	((x) <<  0)

#define RFSHTMG_TREFI(x)	((x) << 16)
#define RFSHTMG_TRFC(x)		((x) <<  0)

#define PIR_CLRSR	(0x1 << 27)	/* clear status registers */
#define PIR_QSGATE	(0x1 << 10)	/* Read DQS gate training */
#define PIR_DRAMINIT	(0x1 << 8)	/* DRAM initialization */
#define PIR_DRAMRST	(0x1 << 7)	/* DRAM reset */
#define PIR_PHYRST	(0x1 << 6)	/* PHY reset */
#define PIR_DCAL	(0x1 << 5)	/* DDL calibration */
#define PIR_PLLINIT	(0x1 << 4)	/* PLL initialization */
#define PIR_ZCAL	(0x1 << 1)	/* ZQ calibration */
#define PIR_INIT	(0x1 << 0)	/* PHY initialization trigger */

#define PGSR_INIT_DONE	(0x1 << 0)	/* PHY init done */

#define ZQCR_PWRDOWN	(1U << 31)	/* ZQ power down */

#define ACBDLR_WRITE_DELAY(x)	((x) << 8)

#define DXBDLR_DQ(x)	(x)		/* DQ0-7 BDLR index */
#define DXBDLR_DM	8		/* DM BDLR index */
#define DXBDLR_DQS	9		/* DQS BDLR index */
#define DXBDLR_DQSN	10		/* DQSN BDLR index */

#define DXBDLR_WRITE_DELAY(x)	((x) << 8)
#define DXBDLR_READ_DELAY(x)	((x) << 0)

/*
 * The delay parameters below allow to allegedly specify delay times of some
 * unknown unit for each individual bit trace in each of the four data bytes
 * the 32-bit wide access consists of. Also three control signals can be
 * adjusted individually.
 */
#define BITS_PER_BYTE		(8)
#define NR_OF_BYTE_LANES	(32 / BITS_PER_BYTE)
/* The eight data lines (DQn) plus DM, DQS and DQSN */
#define LINES_PER_BYTE_LANE	(BITS_PER_BYTE + 3)
struct dram_para {
	uint16_t page_size;
	uint8_t bus_full_width;
	uint8_t dual_rank;
	uint8_t row_bits;
	uint8_t bank_bits;
	const uint8_t dx_read_delays[NR_OF_BYTE_LANES][LINES_PER_BYTE_LANE];
	const uint8_t dx_write_delays[NR_OF_BYTE_LANES][LINES_PER_BYTE_LANE];
	const uint8_t ac_delays[31];
};

static inline int ns_to_t(int nanoseconds)
{
	const unsigned int ctrl_freq = CONFIG_DRAM_CLK / 2;

	return DIV_ROUND_UP(ctrl_freq * nanoseconds, 1000);
}

void mctl_set_timing_params(uint16_t socid, struct dram_para *para);

#endif /* _SUNXI_DRAM_SUN8I_H3_H */
