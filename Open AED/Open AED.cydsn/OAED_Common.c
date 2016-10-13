/* ========================================
 *
 *  OPEN SOURCE AED
 * This file contains all common
 * function and global variable
 * definitions.
 *
 * ========================================
*/

#include "OAED_Common.h"

/* Declaration of global variables */
int16 BufferECG[ECG_Data_size]      = {0};
int16 DataECG[ECG_Data_size]        = {0};
int16 BufferZ[Z_Data_size]          = {0};
int32 DataZ[Z_Data_size]            = {0};
bool Event_flags[EVENT_NO]          = {false};
double Patient_impedance            = 0;

/* Declaration of system flags */
bool ECG_buffer_full                = false;
bool Z_buffer_full                  = false;
bool lead_detected                  = false;
bool ECG_data_pending               = false;
bool capacitor_ready                = false;
bool ECG_enabled                    = false;
bool Z_enabled                      = false;

/* Function declarations */
void OAED_Init(){
    
    OAED_InitAcquisition(); /* Initialize acquisition circuitery. */
    OAED_InitFilter();      /* Initialize filter component. */
    OAED_DMA_Init();        /* DMA Initialization. */
    OAED_ISR_Init();        /* ISR initialization. */    
    OAED_ResetEvent();      /* Event data initialization. */
    
    /* Secure the defibrillation circuitery. */
    OAED_DisableChargingCircuit();
    OAED_DisableDefibrillation();
    OAED_DisarmDefibrillator(true);
    
    OAED_USBInit();
    #if(OAED_TIME)
        OAED_InitTime();
    #endif
    
    return;
}

void OAED_InitAcquisition(){
    
    ADC_DelSig_Start();     /* Power up and initialize DelSig ADC */
    
    IDAC_Drain_Start();     /* Start IDAC source and drain for lead off detection  */
    IDAC_Source_Start();    /* This should also trigger the lead-on/off interrupt. */
    
    Comp_p_Start();         /* Start p lead-on/off comparator. */
    Comp_n_Start();         /* Start n lead-on/off comparator. */
    
    return;
}

void OAED_InitFilter(){
    
    Filter_Start();         /* Init and start the filter component. */
    
    /* Set coherency and dalign for 16bit DMA transfer. */
    Filter_SetCoherency(Filter_CHANNEL_A, Filter_KEY_MID);
    Filter_SetCoherency(Filter_CHANNEL_B, Filter_KEY_MID);
    Filter_SetDalign(Filter_STAGEA_DALIGN, Filter_ENABLED);
    Filter_SetDalign(Filter_STAGEB_DALIGN, Filter_ENABLED);
    Filter_SetDalign(Filter_HOLDA_DALIGN, Filter_ENABLED);
    Filter_SetDalign(Filter_HOLDB_DALIGN, Filter_ENABLED);
    return;
}

void OAED_CopyECGBuffer(){
    /* The Buffer is copied on the Data vector and  */
    /* re-initialized to zeroes.                    */
    uint16 i;
    
    for(i = 0; i<ECG_Data_size ; i++){
        DataECG[i] = BufferECG[i];
        BufferECG[i] = 0;
    }
    
    ECG_buffer_full = false;
    ECG_data_pending = true;
    
    return;
}

void OAED_CopyZBuffer(){
    /* The Buffer is copied on the Data vector and  */
    /* re-initialized to zeroes.                    */
    uint16 i;
    
    for(i = 0; i<Z_Data_size ; i++){
        DataZ[i] = (int32)BufferZ[i];
        BufferZ[i] = 0;
    }
    
    Z_buffer_full = false;
    
    return;
}

void OAED_Led(bool red, bool yellow, bool green){
    
    if(red)
        CyPins_SetPin(Status_Led_Red);
    else
        CyPins_ClearPin(Status_Led_Red);
        
    if(yellow)
        CyPins_SetPin(Status_Led_Yellow);
    else
        CyPins_ClearPin(Status_Led_Yellow);
        
    if(green)
        CyPins_SetPin(Status_Led_Green);
    else
        CyPins_ClearPin(Status_Led_Green);
    
    return;
}

void OAED_SetSystemStatus(char Status){
    
    switch(Status){
        case charging_capacitor:
            OAED_Led(true, true, false);    // Red and yellow
            /* Disarm the Defibrillator.                        */
            OAED_DisarmDefibrillator(false);
            /* Enable charging circuit.                         */
            OAED_EnableChargingCircuit();
            return;
        case discharge_enabled:
            OAED_Led(true, true, true);     // Red, yellow and green
            /* Charging circuit still operative.                */
            /* Arm the defibrillator.                           */
            OAED_ArmDefibrillator();
            return;
        case internal_discharge:
            OAED_Led(false, false, false);  // No led
            OAED_DisableChargingCircuit();
            return;
        case measurement_mode:
            OAED_Led(true, false, false);   // Only red
            OAED_DisableChargingCircuit();
            return;
        default:
            /* Lead-Off                                         */
            OAED_Led(false, false, false);  // No led
            OAED_DisableChargingCircuit();
            return;
    }
}

void OAED_ResetEvent(){
    /* This function reinitialize the event data. */
    
    uint8 i;
    for(i = 0 ; i < EVENT_NO ; i++)
        Event_flags[i] = false;
        
    ECG_data_pending = false;
    return;
}

void OAED_WaitLeadOn(){
    /* This function wait indefinitely until the electrodes are */
    /* attached to the patient.                                 */
    /* WARNING: THIS IS A BLOCKING FUNCTION                     */
    bool Led_Red = false;
    uint8 i;
    
    while(true){
        /* Blink the red led. */
        for(i = 0 ; i<5 ; i++){
            if(lead_detected){
                return;
            }
            CyDelay(100);
        }
        Led_Red = !Led_Red;
        OAED_Led(Led_Red, false, false);
    }
}

bool OAED_WaitForData(){
    /* Return false if detect lead-off, otherwise return true   */
    /* when new data is available.                              */
    /* WARNING: THIS IS A BLOCKING FUNCTION                     */
    while(!ECG_buffer_full){
        
        if(!lead_detected)
            return false;
        /* Check for z Data */
        if(Z_buffer_full){
            OAED_CopyZBuffer();
            if(!OAED_EvaluateImpedance())
                return false;
        }
        /* Wait 100 ms */
        CyDelay(100);
    }
    
    /* Get data */
    OAED_CopyECGBuffer();
    
    return true;
}

void OAED_WaitForZ(){
    /* Return false if detect lead-off, otherwise return true   */
    /* when new data is available.                              */
    /* WARNING: THIS IS A BLOCKING FUNCTION                     */
    
    /* Wait for data */
    while(!Z_buffer_full){
        if(!lead_detected)
            return;
        CyDelay(10);
    }
    
    /* Get data */
    OAED_CopyZBuffer();
    return;
}

bool OAED_WaitForCap(){
    /* Wait until the capacitor is ready.                       */
    /* If meanwhile new data is available or a lead-off is      */
    /* detected return false.                                   */
    /* WARNING: THIS IS A BLOCKING FUNCTION                     */
    while(!ECG_buffer_full){
        if(!lead_detected)
            return false;
        /* Check for new Z data */
        if(Z_buffer_full){
            OAED_CopyZBuffer();
            if(!OAED_EvaluateImpedance())
                return false;
        }
        /* Check for capacitor ready */
        if(capacitor_ready)
            return true;
        /* Wait 100 ms */
        CyDelay(100);
    }
    
    /* Get Data */
    OAED_CopyECGBuffer();
    return false;
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
    /* Start ECG acquisition                                    */
    
    /* If lead-off detected return false. */
    if(!lead_detected)
        return false;
    
    /* If ECG isn't already functioning, start the acquisition. */
    if(!ECG_enabled){
        /* Start ADC DelSig conversion */
        ADC_DelSig_StartConvert();
        
        /* Enable DMA */
         OAED_DMAECGStart();
    }
    
    /* In any case start Z acquisition */
    OAED_ZAcquisition();
    
    ECG_enabled = true;
    
    return true;
}

void OAED_StopAcquisition(){
    /* Stop ECG acquisition                                     */
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

bool OAED_CheckFlags(){
    /* Return true in case the positive events are equal or     */
    /* greater than the required POSITIVE_EVENT_NO.             */
    /* Default set as 2(POSITIVE_EVENT_NO) out of 3(EVENT_NO).  */
    uint8 i, counter;
    counter = 0;
    
    for( i=0 ; i<EVENT_NO ; i++){
        if(Event_flags[i])
            counter++;
        if(counter == POSITIVE_EVENT_NO)
            return true;
    }
    return false;
}

bool OAED_EvaluateRhythm(){
    /* Evaluate the Data vector, update the event flags and     */
    /* return true if VT/VF.                                    */
    
    /* Clear data pending flag.                                 */
    ECG_data_pending = false;
    
    return OAED_CheckFlags();
}

bool OAED_EvaluateImpedance(){
    /* Evaluate the patient impedance from data and store it in */
    /* patient_impedance variable.                              */
    /* Return false if a lead off is assumed, otherwise true.   */
    uint16 i;
    double new_impedance = 0;
    
    /* Data Z contains voltages count used to evaluate the      */
    /* impedance.                                               */
    for( i=0; i<Z_Data_size ; i++){
        /* This loop convert all the ADC counts to uV and sum   */
        /* them all.                                            */
        new_impedance += (double)(ADC_DelSig_CountsTo_uVolts(DataZ[i]));
    }
    /* Get the mean value. */
    new_impedance /= (double)Z_Data_size;
    /* Calculate the impedance from the IDAC current            */
    /* (expressed in 1/8 uA).                                   */
    new_impedance /= (double)(IDAC_Drain_Data / 8);
    
    /* If the new impedance is outside the human limits it      */
    /* means that the electrodes may not be attached to the     */
    /* patient. Therefore we assume a lead-off.                 */
    if(new_impedance < Z_min || new_impedance > Z_max){
        Patient_impedance = 0;
        lead_detected = false;
        return false;
    }
    
    /* This is optional, but enabled by default.                */
    /* If the last acquisition was drastically different it is  */
    /* possible that the electrodes went off.                   */
    if(Patient_impedance != 0){
        double imp_ratio = new_impedance / Patient_impedance;
        if(imp_ratio > 1 + impedance_deviation){
            Patient_impedance = 0;
            lead_detected = false;
            return false;
        }
        if(imp_ratio < 1 - impedance_deviation){
            Patient_impedance = 0;
            lead_detected = false;
            return false;
        }
    }
    
    /* If the impedance calculations are correct the old        */
    /* impedance is overwritten.                                */
    Patient_impedance = new_impedance;
    return true;
}

void OAED_EnableChargingCircuit(){
    /* Disable the protection resistor connection.              */
    OAED_HBridgeControl( phi_0 );
    /* Disarm Defibrillator.                                    */
    OAED_DisarmDefibrillator(false);
    
    /* Enable the charging circuit.                             */
    CyPins_SetPin(Charge_En_0);
    
    /* Enable capacitor sense.                                  */
    Comp_CapReady_Start();
    Comp_CapLow_Start();
    isr_CapReady_Enable();
    
    return;
}

void OAED_DisableChargingCircuit(){    
    /* Disable Charging circuit.                                */
    CyPins_ClearPin(Charge_En_0);
    
    /* Disable capacitor sense.                                 */
    isr_CapReady_Disable();
    Comp_CapReady_Stop();
    Comp_CapLow_Stop();
    
    return;
}

/* [] END OF FILE */
