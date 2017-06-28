/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef __CTP_GIC_H
#define __CTP_GIC_H

//#define GIC1_BASE					(0x19000000)
//#define GIC_BASE					(0x10240000)
#define GIC_BASE					(0x0C000000)


//#define INT_POL_CTL0        		(MCUCFG_BASE + 0x20)
//#define GICD_BASE                   	(GIC1_BASE)
//#define GICR_BASE                    	(GIC1_BASE + 0x200000)
#define GICD_BASE                   	(GIC_BASE)
#define GICR_BASE                    	(GIC_BASE + 0x200000)

#define GICR_SGI_BASE                   (GICR_BASE + 0x10000)

#if 1 /* FIXME */
#define GIC_CPU_BASE                    (GIC_BASE + 0x2000)
#define GIC_DIST_BASE  (0x0C000000) 

#endif

#define GIC_CPU_CTRL			0x00
#define GIC_CPU_PRIMASK			0x04
#define GIC_CPU_BINPOINT		0x08
#define GIC_CPU_INTACK			0x0c
#define GIC_CPU_EOI			0x10
#define GIC_CPU_RUNNINGPRI		0x14
#define GIC_CPU_HIGHPRI			0x18
#define GIC_CPU_AIAR			0x20
#define GIC_CPU_AEOI			0x24

#define GIC_DIST_CTRL			0x000
#define GIC_DIST_CTR			0x004
#define GIC_DIST_IGROUP			0x080
#define GIC_DIST_ENABLE_SET		0x100
#define GIC_DIST_ENABLE_CLEAR		0x180
#define GIC_DIST_PENDING_SET		0x200
#define GIC_DIST_PENDING_CLEAR		0x280
#define GIC_DIST_ACTIVE_SET		0x300
#define GIC_DIST_ACTIVE_CLEAR		0x380
#define GIC_DIST_PRI			0x400
#define GIC_DIST_TARGET			0x800
#define GIC_DIST_CONFIG			0xc00
#define GIC_DIST_SOFTINT		0xf00

/*
 * Distributor registers. We assume we're running non-secure, with ARE
 * being set. Secure-only and non-ARE registers are not described.
 */
#define GICD_CTLR			0x0000
#define GICD_TYPER			0x0004
#define GICD_IIDR			0x0008
#define GICD_STATUSR			0x0010
#define GICD_SETSPI_NSR			0x0040
#define GICD_CLRSPI_NSR			0x0048
#define GICD_SETSPI_SR			0x0050
#define GICD_CLRSPI_SR			0x0058
#define GICD_SEIR			0x0068
#define GICD_ISENABLER			0x0100
#define GICD_ICENABLER			0x0180
#define GICD_ISPENDR			0x0200
#define GICD_ICPENDR			0x0280
#define GICD_ISACTIVER			0x0300
#define GICD_ICACTIVER			0x0380
#define GICD_IPRIORITYR			0x0400
#define GICD_ICFGR			0x0C00
#define GICD_IGRPMODR			0x0D00
#define GICD_IROUTER			0x6000
#define GICD_PIDR2			0xFFE8

#define GICD_CTLR_RWP			(1U << 31)
#define GICD_CTLR_ARE_NS		(1U << 4)
#define GICD_CTLR_ENABLE_G1S		(1U << 2)
#define GICD_CTLR_ENABLE_G1NS		(1U << 1)
#define GICD_CTLR_ENABLE_G0		(1U << 0)

#define GICD_TYPER_ID_BITS(typer)	((((typer) >> 19) & 0x1f) + 1)
#define GICD_TYPER_IRQS(typer)		((((typer) & 0x1f) + 1) * 32)
#define GICD_TYPER_LPIS			(1U << 17)

#define GICD_IROUTER_SPI_MODE_ONE	(0U << 31)
#define GICD_IROUTER_SPI_MODE_ANY	(1U << 31)

#define GIC_PIDR2_ARCH_MASK		0xf0
#define GIC_PIDR2_ARCH_GICv3		0x30
#define GIC_PIDR2_ARCH_GICv4		0x40

/*
 * Re-Distributor registers, offsets from RD_base
 */
#define GICR_CTLR			GICD_CTLR
#define GICR_IIDR			0x0004
#define GICR_TYPER			0x0008
#define GICR_STATUSR			GICD_STATUSR
#define GICR_WAKER			0x0014
#define GICR_SETLPIR			0x0040
#define GICR_CLRLPIR			0x0048
#define GICR_SEIR			GICD_SEIR
#define GICR_PROPBASER			0x0070
#define GICR_PENDBASER			0x0078
#define GICR_INVLPIR			0x00A0
#define GICR_INVALLR			0x00B0
#define GICR_SYNCR			0x00C0
#define GICR_MOVLPIR			0x0100
#define GICR_MOVALLR			0x0110
#define GICR_PIDR2			GICD_PIDR2

#define GICR_CTLR_ENABLE_LPIS		(1UL << 0)

#define GICR_TYPER_CPU_NUMBER(r)	(((r) >> 8) & 0xffff)

#define GICR_WAKER_ProcessorSleep	(1U << 1)
#define GICR_WAKER_ChildrenAsleep	(1U << 2)

#define GICR_PROPBASER_NonShareable	(0U << 10)
#define GICR_PROPBASER_InnerShareable	(1U << 10)
#define GICR_PROPBASER_OuterShareable	(2U << 10)
#define GICR_PROPBASER_SHAREABILITY_MASK (3UL << 10)
#define GICR_PROPBASER_nCnB		(0U << 7)
#define GICR_PROPBASER_nC		(1U << 7)
#define GICR_PROPBASER_RaWt		(2U << 7)
#define GICR_PROPBASER_RaWb		(3U << 7)
#define GICR_PROPBASER_WaWt		(4U << 7)
#define GICR_PROPBASER_WaWb		(5U << 7)
#define GICR_PROPBASER_RaWaWt		(6U << 7)
#define GICR_PROPBASER_RaWaWb		(7U << 7)
#define GICR_PROPBASER_IDBITS_MASK	(0x1f)

/*
 * Re-Distributor registers, offsets from SGI_base
 */
#define GICR_ISENABLER0			GICD_ISENABLER
#define GICR_ICENABLER0			GICD_ICENABLER
#define GICR_ISPENDR0			GICD_ISPENDR
#define GICR_ICPENDR0			GICD_ICPENDR
#define GICR_ISACTIVER0			GICD_ISACTIVER
#define GICR_ICACTIVER0			GICD_ICACTIVER
#define GICR_IPRIORITYR0		GICD_IPRIORITYR
#define GICR_ICFGR0			GICD_ICFGR

#define GICR_TYPER_PLPIS		(1U << 0)
#define GICR_TYPER_VLPIS		(1U << 1)
#define GICR_TYPER_LAST			(1U << 4)

#define LPI_PROP_GROUP1			(1 << 1)
#define LPI_PROP_ENABLED		(1 << 0)


#define MPIDR_LEVEL_BITS_SHIFT	3
#define MPIDR_LEVEL_BITS	(1 << MPIDR_LEVEL_BITS_SHIFT)
#define MPIDR_LEVEL_MASK ((1 << MPIDR_LEVEL_BITS) - 1)

#define MPIDR_LEVEL_SHIFT(level) \
	(((1 << level) >> 1) << MPIDR_LEVEL_BITS_SHIFT)

#define MPIDR_AFFINITY_LEVEL(mpidr, level) \
	((mpidr >> MPIDR_LEVEL_SHIFT(level)) & MPIDR_LEVEL_MASK)

#define sys_reg(op0, op1, crn, crm, op2) \
	((((op0)-2)<<19)|((op1)<<16)|((crn)<<12)|((crm)<<8)|((op2)<<5))


/*
 * CPU interface registers
 */
#define ICC_CTLR_EL1_EOImode_drop_dir	(0U << 1)
#define ICC_CTLR_EL1_EOImode_drop	(1U << 1)
#define ICC_SRE_EL1_SRE			(1U << 0)
#define ICC_MSRE_SRE			(1U << 0)
#define ICC_MSRE_ENABLE			(1U << 3)

/*
 * Hypervisor interface registers (SRE only)
 */
#define ICH_LR_VIRTUAL_ID_MASK		((1UL << 32) - 1)

#define ICH_LR_EOI			(1UL << 41)
#define ICH_LR_GROUP			(1UL << 60)
#define ICH_LR_STATE			(3UL << 62)
#define ICH_LR_PENDING_BIT		(1UL << 62)
#define ICH_LR_ACTIVE_BIT		(1UL << 63)

#define ICH_MISR_EOI			(1 << 0)
#define ICH_MISR_U			(1 << 1)

#define ICH_HCR_EN			(1 << 0)
#define ICH_HCR_UIE			(1 << 1)

#define ICH_VMCR_CTLR_SHIFT		0
#define ICH_VMCR_CTLR_MASK		(0x21f << ICH_VMCR_CTLR_SHIFT)
#define ICH_VMCR_BPR1_SHIFT		18
#define ICH_VMCR_BPR1_MASK		(7 << ICH_VMCR_BPR1_SHIFT)
#define ICH_VMCR_BPR0_SHIFT		21
#define ICH_VMCR_BPR0_MASK		(7 << ICH_VMCR_BPR0_SHIFT)
#define ICH_VMCR_PMR_SHIFT		24
#define ICH_VMCR_PMR_MASK		(0xffUL << ICH_VMCR_PMR_SHIFT)

#define ICC_EOIR1_EL1			sys_reg(3, 0, 12, 12, 1)
#define ICC_IAR1_EL1			sys_reg(3, 0, 12, 12, 0)
#define ICC_SGI1R_EL1			sys_reg(3, 0, 12, 11, 5)
#define ICC_PMR_EL1			sys_reg(3, 0, 4, 6, 0)
#define ICC_CTLR_EL1			sys_reg(3, 0, 12, 12, 4)
#define ICC_SRE_EL1			sys_reg(3, 0, 12, 12, 5)
#define ICC_GRPEN1_EL1			sys_reg(3, 0, 12, 12, 7)

#define ICC_IAR1_EL1_SPURIOUS		0x3ff

#define ICC_SRE_EL2			sys_reg(3, 4, 12, 9, 5)

#define ICC_SRE_EL2_SRE			(1 << 0)
#define ICC_SRE_EL2_ENABLE		(1 << 3)

/*
 * System register definitions
 */
#define ICH_VSEIR_EL2			sys_reg(3, 4, 12, 9, 4)
#define ICH_HCR_EL2			sys_reg(3, 4, 12, 11, 0)
#define ICH_VTR_EL2			sys_reg(3, 4, 12, 11, 1)
#define ICH_MISR_EL2			sys_reg(3, 4, 12, 11, 2)
#define ICH_EISR_EL2			sys_reg(3, 4, 12, 11, 3)
#define ICH_ELSR_EL2			sys_reg(3, 4, 12, 11, 5)
#define ICH_VMCR_EL2			sys_reg(3, 4, 12, 11, 7)

#define __LR0_EL2(x)			sys_reg(3, 4, 12, 12, x)
#define __LR8_EL2(x)			sys_reg(3, 4, 12, 13, x)

#define ICH_LR0_EL2			__LR0_EL2(0)
#define ICH_LR1_EL2			__LR0_EL2(1)
#define ICH_LR2_EL2			__LR0_EL2(2)
#define ICH_LR3_EL2			__LR0_EL2(3)
#define ICH_LR4_EL2			__LR0_EL2(4)
#define ICH_LR5_EL2			__LR0_EL2(5)
#define ICH_LR6_EL2			__LR0_EL2(6)
#define ICH_LR7_EL2			__LR0_EL2(7)
#define ICH_LR8_EL2			__LR8_EL2(0)
#define ICH_LR9_EL2			__LR8_EL2(1)
#define ICH_LR10_EL2			__LR8_EL2(2)
#define ICH_LR11_EL2			__LR8_EL2(3)
#define ICH_LR12_EL2			__LR8_EL2(4)
#define ICH_LR13_EL2			__LR8_EL2(5)
#define ICH_LR14_EL2			__LR8_EL2(6)
#define ICH_LR15_EL2			__LR8_EL2(7)

#define __AP0Rx_EL2(x)			sys_reg(3, 4, 12, 8, x)
#define ICH_AP0R0_EL2			__AP0Rx_EL2(0)
#define ICH_AP0R1_EL2			__AP0Rx_EL2(1)
#define ICH_AP0R2_EL2			__AP0Rx_EL2(2)
#define ICH_AP0R3_EL2			__AP0Rx_EL2(3)

#define __AP1Rx_EL2(x)			sys_reg(3, 4, 12, 9, x)
#define ICH_AP1R0_EL2			__AP1Rx_EL2(0)
#define ICH_AP1R1_EL2			__AP1Rx_EL2(1)
#define ICH_AP1R2_EL2			__AP1Rx_EL2(2)
#define ICH_AP1R3_EL2			__AP1Rx_EL2(3)


extern void gic_secondary_init(void);

#endif

