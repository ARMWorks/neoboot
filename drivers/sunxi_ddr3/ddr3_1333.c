#include "dram_sunxi_dw.h"
#include <regs.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

void mctl_set_timing_params(uint16_t socid, struct dram_para *para)
{
	struct sunxi_mctl_ctl_reg * const mctl_ctl =
			(struct sunxi_mctl_ctl_reg *)SUNXI_DRAM_CTL0_BASE;

	uint8_t tccd		= 2;
	uint8_t tfaw		= ns_to_t(50);
	uint8_t trrd		= max(ns_to_t(10), 4);
	uint8_t trcd		= ns_to_t(15);
	uint8_t trc		= ns_to_t(53);
	uint8_t txp		= max(ns_to_t(8), 3);
	uint8_t twtr		= max(ns_to_t(8), 4);
	uint8_t trtp		= max(ns_to_t(8), 4);
	uint8_t twr		= max(ns_to_t(15), 3);
	uint8_t trp		= ns_to_t(15);
	uint8_t tras		= ns_to_t(38);
	uint16_t trefi	= ns_to_t(7800) / 32;
	uint16_t trfc	= ns_to_t(350);

	uint8_t tmrw		= 0;
	uint8_t tmrd		= 4;
	uint8_t tmod		= 12;
	uint8_t tcke		= 3;
	uint8_t tcksrx	= 5;
	uint8_t tcksre	= 5;
	uint8_t tckesr	= 4;
	uint8_t trasmax	= 24;

	uint8_t tcl		= 6; /* CL 12 */
	uint8_t tcwl		= 4; /* CWL 8 */
	uint8_t t_rdata_en	= 4;
	uint8_t wr_latency	= 2;

	uint32_t tdinit0	= (500 * CONFIG_DRAM_CLK) + 1;		/* 500us */
	uint32_t tdinit1	= (360 * CONFIG_DRAM_CLK) / 1000 + 1;	/* 360ns */
	uint32_t tdinit2	= (200 * CONFIG_DRAM_CLK) + 1;		/* 200us */
	uint32_t tdinit3	= (1 * CONFIG_DRAM_CLK) + 1;		/* 1us */

	uint8_t twtp		= tcwl + 2 + twr;	/* WL + BL / 2 + tWR */
	uint8_t twr2rd	= tcwl + 2 + twtr;	/* WL + BL / 2 + tWTR */
	uint8_t trd2wr	= tcl + 2 + 1 - tcwl;	/* RL + BL / 2 + 2 - WL */

	/* set mode register */
	writel(0x1c70, &mctl_ctl->mr[0]);	/* CL=11, WR=12 */
	writel(0x40, &mctl_ctl->mr[1]);
	writel(0x18, &mctl_ctl->mr[2]);		/* CWL=8 */
	writel(0x0, &mctl_ctl->mr[3]);

	/* set DRAM timing */
	writel(DRAMTMG0_TWTP(twtp) | DRAMTMG0_TFAW(tfaw) |
	       DRAMTMG0_TRAS_MAX(trasmax) | DRAMTMG0_TRAS(tras),
	       &mctl_ctl->dramtmg[0]);
	writel(DRAMTMG1_TXP(txp) | DRAMTMG1_TRTP(trtp) | DRAMTMG1_TRC(trc),
	       &mctl_ctl->dramtmg[1]);
	writel(DRAMTMG2_TCWL(tcwl) | DRAMTMG2_TCL(tcl) |
	       DRAMTMG2_TRD2WR(trd2wr) | DRAMTMG2_TWR2RD(twr2rd),
	       &mctl_ctl->dramtmg[2]);
	writel(DRAMTMG3_TMRW(tmrw) | DRAMTMG3_TMRD(tmrd) | DRAMTMG3_TMOD(tmod),
	       &mctl_ctl->dramtmg[3]);
	writel(DRAMTMG4_TRCD(trcd) | DRAMTMG4_TCCD(tccd) | DRAMTMG4_TRRD(trrd) |
	       DRAMTMG4_TRP(trp), &mctl_ctl->dramtmg[4]);
	writel(DRAMTMG5_TCKSRX(tcksrx) | DRAMTMG5_TCKSRE(tcksre) |
	       DRAMTMG5_TCKESR(tckesr) | DRAMTMG5_TCKE(tcke),
	       &mctl_ctl->dramtmg[5]);

	/* set two rank timing */
	#ifdef PLATFORM_SUNXI_H3
	clrsetbits_le32(&mctl_ctl->dramtmg[8], (0xff << 8) | (0xff << 0),
			(0x66 << 8) | (0x10 << 0));
	#elif PLATFORM_SUNXI_H5
	clrsetbits_le32(&mctl_ctl->dramtmg[8], (0xff << 8) | (0xff << 0),
			(0x33 << 8) | (0x10 << 0));
	#endif

	/* set PHY interface timing, write latency and read latency configure */
	writel((0x2 << 24) | (t_rdata_en << 16) | (0x1 << 8) |
	       (wr_latency << 0), &mctl_ctl->pitmg[0]);

	/* set PHY timing, PTR0-2 use default */
	writel(PTR3_TDINIT0(tdinit0) | PTR3_TDINIT1(tdinit1), &mctl_ctl->ptr[3]);
	writel(PTR4_TDINIT2(tdinit2) | PTR4_TDINIT3(tdinit3), &mctl_ctl->ptr[4]);

	/* set refresh timing */
	writel(RFSHTMG_TREFI(trefi) | RFSHTMG_TRFC(trfc), &mctl_ctl->rfshtmg);
}