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

/* Include */
#include <project.h>
#include "OAED_Common.h"
/* End of Include*/

/* Numeric constants */
/* Defines for DMA_DelSig */
#define DMA_DelSig_BYTES_PER_BURST 2
#define DMA_DelSig_REQUEST_PER_BURST 1
#define DMA_DelSig_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_DelSig_DST_BASE (CYDEV_PERIPH_BASE)

/* Defines for DMA_DelSig_B */
#define DMA_DelSig_B_BYTES_PER_BURST 2
#define DMA_DelSig_B_REQUEST_PER_BURST 1
#define DMA_DelSig_B_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_DelSig_B_DST_BASE (CYDEV_PERIPH_BASE)

#if(RAW_MODE)
/* Defines for DMA_DelSig_RAW */
#define DMA_DelSig_RAW_BYTES_PER_BURST 2
#define DMA_DelSig_RAW_REQUEST_PER_BURST 1
#define DMA_DelSig_RAW_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_DelSig_RAW_DST_BASE (CYDEV_SRAM_BASE)
#endif

/* Defines for DMA_FilterECG */
#define DMA_FilterECG_BYTES_PER_BURST 2
#define DMA_FilterECG_REQUEST_PER_BURST 1
#define DMA_FilterECG_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_FilterECG_DST_BASE (CYDEV_SRAM_BASE)

/* Defines for DMA_FilterZ */
#define DMA_FilterZ_BYTES_PER_BURST 2
#define DMA_FilterZ_REQUEST_PER_BURST 1
#define DMA_FilterZ_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_FilterZ_DST_BASE (CYDEV_SRAM_BASE)
/* End of numeric constants */

/* Function prototypes */
void OAED_DMA_Init();
void OAED_DMAECGStart();
void OAED_DMAECGStop();
void OAED_DMAZStart();
void OAED_DMAZStop();
void OAED_DMAADCStart();
void OAED_DMAADCStop();
/* End of Function prototypes */

#endif
/* [] END OF FILE */
