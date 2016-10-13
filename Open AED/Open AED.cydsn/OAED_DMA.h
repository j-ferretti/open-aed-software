/* ========================================
 *
 *  OPEN SOURCE AED
 * This header contains all DMA related
 * function prototypes and variable
 * declarations.
 *
 * ========================================
*/

#ifndef OAED_DMA_H
#define OAED_DMA_H
    
#include <project.h>
#include "OAED_Common.h"

/* Function prototypes */
void OAED_DMA_Init();
void OAED_DMAECGStart();
void OAED_DMAECGStop();
void OAED_DMAZStart();
void OAED_DMAZStop();

/* Extern variable definition */
extern int16 BufferECG[];
extern int16 BufferZ[];


/* Defines for DMA_DelSig */
#define DMA_DelSig_BYTES_PER_BURST 2
#define DMA_DelSig_REQUEST_PER_BURST 1
#define DMA_DelSig_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_DelSig_DST_BASE (CYDEV_PERIPH_BASE)

/* Defines for DMA_Filter_A */
#define DMA_Filter_A_BYTES_PER_BURST 2
#define DMA_Filter_A_REQUEST_PER_BURST 1
#define DMA_Filter_A_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_Filter_A_DST_BASE (CYDEV_PERIPH_BASE)

/* Defines for DMA_Filter_B */
#define DMA_Filter_B_BYTES_PER_BURST 2
#define DMA_Filter_B_REQUEST_PER_BURST 1
#define DMA_Filter_B_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_Filter_B_DST_BASE (CYDEV_SRAM_BASE)

/* Defines for DMA_Impedance */
#define DMA_Impedance_BYTES_PER_BURST 2
#define DMA_Impedance_REQUEST_PER_BURST 1
#define DMA_Impedance_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_Impedance_DST_BASE (CYDEV_SRAM_BASE)

#endif
/* [] END OF FILE */
