/* ========================================
 *
 *  OPEN SOURCE AED
 * This header contains all ISR related
 * function prototypes and variable
 * declarations.
 *
 * ========================================
*/

#ifndef OAED_ISR_H
#define OAED_ISR_H

#include <project.h>
#include <stdbool.h>
#include "OAED_Defibrillation.h"

/* Function prototypes */
void OAED_ISR_Init();

/* External variable definition */
extern bool capacitor_ready;
extern bool buffer_full;
extern bool lead_detected;

/* Custom ISR prototypes */
CY_ISR_PROTO(isr_BufferZRe);
CY_ISR_PROTO(isr_BufferECGRe);
CY_ISR_PROTO(isr_LeadOff);
CY_ISR_PROTO(isr_LeadOn);
CY_ISR_PROTO(isr_CapReady);
CY_ISR_PROTO(isr_CapLow);
CY_ISR_PROTO(isr_Defibrillation);

#endif


/* [] END OF FILE */
