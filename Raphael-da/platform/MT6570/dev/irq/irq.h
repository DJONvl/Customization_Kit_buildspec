
#ifndef __MT_IRQ_H__
#define __MT_IRQ_H__

#include "inc/register_base.h"
/*
* Define IRQ code.
*/

#define MT65xx_EDGE_SENSITIVE 0
#define MT65xx_LEVEL_SENSITIVE 1
#define MT65xx_POLARITY_LOW   0
#define MT65xx_POLARITY_HIGH  1

#define GIC_IGROUPER    (GIC_DIST_BASE + 0x80)
#define GIC_ICDISER0    (GIC_DIST_BASE + 0x100)
#define GIC_ICDISER1    (GIC_DIST_BASE + 0x104)
#define GIC_ICDISER2    (GIC_DIST_BASE + 0x108)
#define GIC_ICDISER3    (GIC_DIST_BASE + 0x10C)
#define GIC_ICDISER4    (GIC_DIST_BASE + 0x110)
#define GIC_ICDISER5    (GIC_DIST_BASE + 0x114)
#define GIC_ICDISER6    (GIC_DIST_BASE + 0x118)
#define GIC_ICDISER7    (GIC_DIST_BASE + 0x11C)

#define GIC_ICDICER0    (GIC_DIST_BASE + 0x180)
#define GIC_ICDICER1    (GIC_DIST_BASE + 0x184)
#define GIC_ICDICER2    (GIC_DIST_BASE + 0x188)
#define GIC_ICDICER3    (GIC_DIST_BASE + 0x18C)
#define GIC_ICDICER4    (GIC_DIST_BASE + 0x190)
#define GIC_ICDICER5    (GIC_DIST_BASE + 0x194)
#define GIC_ICDICER6    (GIC_DIST_BASE + 0x198)
#define GIC_ICDICER7    (GIC_DIST_BASE + 0x19C)

#define INT_POL_CTL0  (MCUCFG_BASE + 0x100)

#define GIC_CPU_CTRL			0x00
#define GIC_CPU_PRIMASK			0x04
#define GIC_CPU_BINPOINT		0x08
#define GIC_CPU_INTACK			0x0c
#define GIC_CPU_EOI			0x10
#define GIC_CPU_RUNNINGPRI		0x14
#define GIC_CPU_HIGHPRI			0x18

#define GICR_WAKER_ProcessorSleep       (1U << 1)
#define GICR_WAKER_ChildrenAsleep       (1U << 2)
#define GIC_DIST_CTRL			0x000
#define GIC_DIST_CTR			0x004
#define GIC_DIST_WAKER		0x014
#define GIC_DIST_ENABLE_SET		0x100
#define GIC_DIST_ENABLE_CLEAR		0x180
#define GIC_DIST_PENDING_SET		0x200
#define GIC_DIST_PENDING_CLEAR		0x280
#define GIC_DIST_ACTIVE_BIT		0x300
#define GIC_DIST_PRI			0x400
#define GIC_DIST_TARGET			0x800
#define GIC_DIST_CONFIG			0xc00
#define GIC_DIST_SOFTINT		0xf00
#define GIC_DIST_ROUTE          0x6100
#define GIC_DIST_ICDISR  		0x80
enum {IRQ_MASK_HEADER = 0xF1F1F1F1, IRQ_MASK_FOOTER = 0xF2F2F2F2};

struct mtk_irq_mask
{
   unsigned int header;   /* for error checking */
   unsigned int mask0;
   unsigned int mask1;
   unsigned int mask2;
   unsigned int mask3;
   unsigned int mask4;
   unsigned int mask5;
   unsigned int mask6;
   unsigned int mask7;
   unsigned int footer;   /* for error checking */
};


/*
* Define hadware registers.
*/

/*
* Define IRQ code.
*/

#define GIC_PRIVATE_SIGNALS (32)

#define GIC_PPI_OFFSET          (27)
#define GIC_PPI_GLOBAL_TIMER    (GIC_PPI_OFFSET + 0)
#define GIC_PPI_LEGACY_FIQ      (GIC_PPI_OFFSET + 1)
#define GIC_PPI_PRIVATE_TIMER   (GIC_PPI_OFFSET + 2)
#define GIC_PPI_WATCHDOG_TIMER  (GIC_PPI_OFFSET + 3)
#define GIC_PPI_LEGACY_IRQ      (GIC_PPI_OFFSET + 4)

#define MT_USB0_IRQ_ID  64
#define MT_GPT_IRQ_ID   124
#define MT_SSUSB_IRQ_ID 105
#define MT_NFI_IRQ_ID   128 /* fixme: need to confirm after SB do nand flash boot */
#define MT_MSDC0_IRQ_ID 70
#define MT_MSDC1_IRQ_ID 71

#define MT_NR_PPI   (5)
#define MT_NR_SPI   (158)
#define NR_IRQ_LINE  (GIC_PPI_OFFSET + MT_NR_PPI + MT_NR_SPI)    // 5 PPIs and 224 SPIs

#define MT65xx_EDGE_SENSITIVE 0
#define MT65xx_LEVEL_SENSITIVE 1

#define MT65xx_POLARITY_LOW   0
#define MT65xx_POLARITY_HIGH  1

void mt_gic_cpu_init(void);
void mt_gic_dist_init(void);
void mt_gic_deinit(void);
int mt_irq_mask_all(struct mtk_irq_mask *mask); //(This is ONLY used for the sleep driver)
int mt_irq_mask_restore(struct mtk_irq_mask *mask); //(This is ONLY used for the sleep driver)
void mt_irq_set_sens(unsigned int irq, unsigned int sens);
void mt_irq_set_polarity(unsigned int irq, unsigned int polarity);
int mt_irq_mask_restore(struct mtk_irq_mask *mask);
void mt_irq_unmask(unsigned int irq);
void mt_irq_ack(unsigned int irq);


#endif  /* !__MT_IRQ_H__ */
