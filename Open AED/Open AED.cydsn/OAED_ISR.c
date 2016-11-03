/* ========================================
 *
 *  OPEN SOURCE AED
 * This file contains all ISR related
 * function and global variable
 * definitions.
 *
 * ========================================
*/

#include "OAED_ISR.h"

/* Function Declarations */

/* ECG Buffer Replenished ISR custom call definition. */
CY_ISR(isr_BufferECGRe){
    static uint16 n = 0;
    int32 t;
    t = CacheECG[0];
    t += CacheECG[1];
    t += CacheECG[2];
    t += CacheECG[3];
    t += CacheECG[4];
    t += CacheECG[5];
    t += CacheECG[6];
    t += CacheECG[7];
    BufferECG[n++] = (int16)(t>>3);
    if(n == 2000){
        ECG_buffer_full = true;
        n = 0;
    }
}

/* Z Buffer Replenished ISR custom call definition. */
CY_ISR(isr_BufferZRe){
    Z_buffer_full = true;
    OAED_DMAZStop();
    Z_enabled = false;
}

/* Lead-off ISR custom call definition. */
CY_ISR(isr_LeadOff){
    isr_Lead_off_Disable();
    lead_detected = false;
    isr_Lead_on_Enable();

    OAED_Led(false,false,false); // DEBUG MODE ONLY //
}
CY_ISR(isr_LeadOn){
    isr_Lead_on_Disable();
    lead_detected = true;
    isr_Lead_off_Enable();

    OAED_Led(false,true,false); // DEBUG MODE ONLY //
}

/* CapReady and CapLow ISR custom call definition. */
CY_ISR(isr_Cap_Ready){
    capacitor_ready = true;
    isr_CapReady_Disable();
}
CY_ISR(isr_Cap_Low){
    capacitor_ready = false;
    isr_CapReady_Enable();
}

/* Defibrillation ISR custom call definition. */
CY_ISR(isr_Defibrillation){
    /* Stop ECG acquisition. */
    OAED_StopAcquisition();
    /* Perform a biphasic defibrillation. */
    OAED_BiphasicDefibrillation(50);
    /* Reset Event data. */
    OAED_ResetEvent();      // Not sure about this
}

/* ISR Init */
void OAED_ISR_Init(){

    /* Disable the Delta Sigma ADC ISR as it is not required    */
    ADC_DelSig_IRQ_Disable();

    /* Enable custom irs call */
    isr_Buffer_ECG_Replenished_StartEx(isr_BufferECGRe);
                                                  /* ECG Buffer Replenished   */
    isr_Buffer_Z_Replenished_StartEx(isr_BufferZRe);
                                                  /* Z Buffer Replenished     */
    isr_Lead_off_StartEx(isr_LeadOff);            /* Lead-off                 */
    isr_Lead_on_StartEx(isr_LeadOn);              /* Lead-on                  */
    isr_CapReady_StartEx(isr_Cap_Ready);          /* Capacitor ready          */
    isr_CapLow_StartEx(isr_Cap_Low);              /* Capacitor low V          */
    isr_Defibrillate_StartEx(isr_Defibrillation);
                                                  /* Defibrillate             */

    return;
}
/* End of function declarations */

/* [] END OF FILE */
