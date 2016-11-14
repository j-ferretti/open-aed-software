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

/* ECG Cache Replenished ISR custom call definition. */
CY_ISR(isr_CacheECGRe){
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
    if(Continuous_USBECG)
        OAED_USBSendData16(CacheECG,8);
    if(n == ECG_DATA_SIZE){
        ECG_buffer_full = true;
        n = 0;
        OAED_ISRECGDisable();
    }
}

#if(RAW_MODE)
/* RAW Cache Replenished ISR custom call definition. */
CY_ISR(isr_CacheRAWRe){
    static uint16 n = 0;
    BufferRAW[n++] = CacheRAW[0];
    BufferRAW[n++] = CacheRAW[1];
    BufferRAW[n++] = CacheRAW[2];
    BufferRAW[n++] = CacheRAW[3];
    BufferRAW[n++] = CacheRAW[4];
    BufferRAW[n++] = CacheRAW[5];
    BufferRAW[n++] = CacheRAW[6];
    BufferRAW[n++] = CacheRAW[7];
    if(Continuous_USBRAW)
        OAED_USBSendData16(CacheRAW,8);
    if(n == RAW_DATA_SIZE){
        RAW_buffer_full = true;
        n = 0;
        OAED_ISRRAWDISABLE();
    }
}
#endif

/* Z Buffer Replenished ISR custom call definition. */
CY_ISR(isr_BufferZRe){
    OAED_ISRZDisable();
    Z_buffer_full = true;
}

/* Lead-off ISR custom call definition. */
CY_ISR(isr_LeadOff){
    isr_Lead_off_Disable();
    lead_detected = false;
    isr_Lead_on_Enable();

    //OAED_Led(false,false,false); // DEBUG MODE ONLY //
}
CY_ISR(isr_LeadOn){
    isr_Lead_on_Disable();
    lead_detected = true;
    isr_Lead_off_Enable();

    //OAED_Led(false,true,false); // DEBUG MODE ONLY //
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
    /* Pause ECG acquisition. */
    OAED_AcquisitionECGPause();
    /* Perform a biphasic defibrillation. */
    OAED_BiphasicDefibrillation(50);
    /* Reset Event data. */
    OAED_ResetEvent();      // Not sure about this
}

/* ISR Init */
void OAED_ISRInit(){

    /* Enable custom irs call */
    isr_CacheECGReplenished_StartEx(isr_CacheECGRe);
                                                  /* ECG Buffer Replenished   */
    isr_BufferZReplenished_StartEx(isr_BufferZRe);
                                                  /* Z Buffer Replenished     */
    isr_Lead_off_StartEx(isr_LeadOff);            /* Lead-off                 */
    isr_Lead_on_StartEx(isr_LeadOn);              /* Lead-on                  */
    isr_CapReady_StartEx(isr_Cap_Ready);          /* Capacitor ready          */
    isr_CapLow_StartEx(isr_Cap_Low);              /* Capacitor low V          */
    isr_Defibrillate_StartEx(isr_Defibrillation); /* Defibrillate             */
    #if(RAW_MODE)                                 /* Raw cache replenished    */
    isr_CacheRAWReplenished_StartEx(isr_CacheRAWRe);
    #endif

    OAED_ISRECGDisable();
    OAED_ISRZDisable();
    #if(RAW_MODE)
    OAED_ISRRAWDISABLE();
    #endif

    return;
}

inline void OAED_ISRECGEnable(){
    ECG_enabled = true;
    isr_CacheECGReplenished_Enable();
}

inline void OAED_ISRECGDisable(){
    ECG_enabled = false;
    isr_CacheECGReplenished_Disable();
}

inline void OAED_ISRZEnable(){
    Z_enabled = true;
    isr_BufferZReplenished_Enable();
}

inline void OAED_ISRZDisable(){
    Z_enabled = false;
    isr_BufferZReplenished_Disable();
}
/* End of function declarations */

/* [] END OF FILE */
