/* ========================================
 *
 *  OPEN SOURCE AED
 * This file contains all acquisition
 * function and global variable
 * definitions.
 *
 * ========================================
*/

#include "OAED_Acquisition.h"

/* Function declarations */
void OAED_InitAcquisition(){

    /* Power up and initialize DelSig ADC */
    ADC_DelSig_Start();

    /* Start IDAC source and drain for lead off detection. */
    /* This should also trigger the lead-on/off interrupt. */
    IDAC_Drain_Start();
    //IDAC_Source_Start();

    /* Start p and n lead-on/off comparators. */
    //Comp_p_Start();
    Comp_n_Start();

    return;
}

void OAED_ZAcquisition(){
    /* If already running return. */
    if(Z_enabled)
        return;
    /* Start Z DMA */
    OAED_DMAZStart();
    /* Set status flag */
    Z_enabled = true;
    return;
}

bool OAED_StartAcquisition(){
    /* Start ECG acquisition. */

    /* If lead-off detected return false. */
    if(!lead_detected)
        return false;

    /* If ECG isn't already functioning, start the acquisition. */
    if(!ECG_enabled){
        /* Start ADC DelSig conversion */
        ADC_DelSig_StartConvert();

        /* Enable DMA */
         OAED_DMAECGStart();

         /* Set flag */
         ECG_enabled = true;
    }

    /* In any case start Z acquisition */
    OAED_ZAcquisition();

    return true;
}

void OAED_StopAcquisition(){
    /* Stop ECG acquisition. */

    /* If ECG acquisition isn't running, return. */
    if(!ECG_enabled)
        return;

    /* Stop ADC DelSig conversion */
    ADC_DelSig_StopConvert();
    /* Disable ECG DMA */
    OAED_DMAECGStop();

    /* Clear ECG flag */
    ECG_enabled = false;

    return;

}
/* End of function declarations */

/* [] END OF FILE */
