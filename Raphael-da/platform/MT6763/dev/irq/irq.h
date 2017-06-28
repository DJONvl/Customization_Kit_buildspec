
#ifndef __MT_IRQ_H__
#define __MT_IRQ_H__

#include "inc/register_base.h"
/*
* Define IRQ code.
*/

#define GIC_CPU_CTRL			0x00
#define GIC_CPU_PRIMASK			0x04
#define GIC_CPU_BINPOINT		0x08
#define GIC_CPU_INTACK			0x0c
#define GIC_CPU_EOI			0x10
#define GIC_CPU_RUNNINGPRI		0x14
#define GIC_CPU_HIGHPRI			0x18

#define GIC_DIST_CTRL			0x000
#define GIC_DIST_ENABLE_SET		0x100
#define GIC_DIST_ENABLE_CLEAR		0x180
#define GIC_DIST_PENDING_SET		0x200
#define GIC_DIST_PENDING_CLEAR		0x280
#define GIC_DIST_ACTIVE_SET        0x300
#define GIC_DIST_ACTIVE_CLEAR      0x380
#define GIC_DIST_PRI			0x400
#define GIC_DIST_CONFIG			0xc00
#define GIC_DIST_IGRPMODR       0xd00
#define GIC_DIST_ROUTE          0x6100
#define GIC_REDIS_WAKER         0x14

#define INT_POL_CTL0  (MCUCFG_BASE + 0x620)

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


#define MT_GPT_IRQ_ID   198
#define MT_USB0_IRQ_ID  105
#define MT_SSUSB_IRQ_ID 159
#define MT_NFI_IRQ_ID   128 /* fixme: need to confirm after SB do nand flash boot */
#define MT_MSDC0_IRQ_ID 111
#define MT_MSDC1_IRQ_ID 112

#define MT_NR_PPI   (5)
#define MT_NR_SPI   (241)//(224)
#define NR_IRQ_LINE  (GIC_PPI_OFFSET + MT_NR_PPI + MT_NR_SPI)    // 5 PPIs and 224 SPIs

#define MT65xx_EDGE_SENSITIVE 0
#define MT65xx_LEVEL_SENSITIVE 1

#define MT65xx_POLARITY_LOW   0
#define MT65xx_POLARITY_HIGH  1

#define IRQ_REGS    ((NR_IRQ_LINE + (32 - 1)) >> 5)

enum {IRQ_MASK_HEADER = 0xF1F1F1F1, IRQ_MASK_FOOTER = 0xF2F2F2F2};

struct mtk_irq_mask {
	unsigned int header;   /* for error checking */
	unsigned int mask[IRQ_REGS];
	unsigned int footer;   /* for error checking */
};

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
void mt_gic_icc_msre_write(void);
void mt_gic_dist_init(void);
unsigned int mt_irq_get(void);
void mt_gic_redist_init(void);


#endif  /* !__MT_IRQ_H__ */
