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
int16 BufferECG[ECG_DATA_SIZE]          = {0};
int16 BufferZ[Z_DATA_SIZE]              = {0};

int16 DataECG[ECG_DATA_SIZE]            = {0};
int16 DataZ[Z_DATA_SIZE]                = {0};

double Patient_impedance                = 0;

#if(RAW_MODE)
    int16 rawECG[ECG_DATA_SIZE]         = {0};
    int16 rawECGBuffer[ECG_DATA_SIZE]   = {0};
#endif
/* End of global variables declaration */

/* Declaration of system flags */
bool ECG_buffer_full                = false;
bool Z_buffer_full                  = false;
bool ECG_data_pending               = false;

bool ECG_enabled                    = false;
bool Z_enabled                      = false;

bool lead_detected                  = false;
bool capacitor_ready                = false;

bool Event_flags[EVENT_NO]          = {false};
/* End of system flags declaration */

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

    OAED_DMAADCStart();

    CyGlobalIntEnable;      /* Enable global interrupts. */
    return;
}

void OAED_InitFilter(){
    /* Init and start the filter component. */
    Filter_Start();

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
    /* The Buffer is copied on the Data vector and re-initialized to zeroes. */
    uint16 i;

    for(i = 0; i < ECG_DATA_SIZE ; i++){
        /* ECG */
        DataECG[i] = BufferECG[i];
        BufferECG[i] = 0;

        /* Raw ECG */
        #if(RAW_MODE)
            rawECG[i] = rawECGBuffer[i];
        #endif
    }

    /* Set system flags */
    ECG_buffer_full = false;
    ECG_data_pending = true;

    return;
}

void OAED_CopyZBuffer(){
    /* The Buffer is copied on the Data vector and re-initialized to zeroes. */
    uint16 i;

    for(i = 0; i < Z_DATA_SIZE ; i++){
        DataZ[i] = BufferZ[i];
        BufferZ[i] = 0;
    }

    /* Set system flag */
    Z_buffer_full = false;

    return;
}

void OAED_Led(bool red, bool yellow, bool green){
    /* Set or clear pins according to the parameters. */
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

void OAED_ResetEvent(){
    /* This function reinitialize the event data. */

    uint8 i;
    for(i = 0 ; i < EVENT_NO ; i++)
        Event_flags[i] = false;

    ECG_data_pending = false;
    return;
}

bool OAED_CheckFlags(){
    /* Return true in case the positive events registered are equal or */
    /* greater than the required positive events. */

    /* Default settings: */
    /* POSITIVE_EVENT_NO    2 */
    /* EVENT_NO             3 */


    uint8 i;
    uint8 counter = 0;

    for( i=0 ; i < EVENT_NO ; i++){
        if(Event_flags[i])
            counter++;
        if(counter == POSITIVE_EVENT_NO)
            return true;
    }
    return false;
}

bool OAED_EvaluateRhythm(){
    /* Evaluate the Data vector, update the event flags and return true */
    /* if VT/VF is detected. */

    /* Clear data pending flag. */
    ECG_data_pending = false;

    /* Black magic here */

    return OAED_CheckFlags();
}

bool OAED_EvaluateImpedance(){
    /* Evaluate the patient impedance from data and store it in */
    /* patient_impedance variable. */
    /* This function return false if a lead off is assumed, otherwise true.*/
    uint16 i;
    uint16 TempMax = 0;
    double new_impedance = 0;

    /* Find maximum abolute value */
    for( i=0; i<Z_DATA_SIZE ; i++){
        if(fabs(DataZ[i]) > TempMax)
            TempMax = fabs(DataZ[i]);
    }

    /* Data Z contains voltages count used to evaluate the impedance. */
    for( i=0; i<Z_DATA_SIZE ; i++){
        if(fabs(DataZ[i]) > 0.75 * TempMax)
            /* This loop convert all the ADC counts to uV and sum them all. */
            new_impedance += (double)(ADC_DelSig_CountsTo_uVolts(fabs(DataZ[i])));
    }
    /* Get the mean value. */
    new_impedance /= (double)Z_DATA_SIZE;

    /* Calculate the impedance from the IDAC current (expressed in 1/8 uA). */
    new_impedance /= ( (double)(IDAC_Drain_Data) * 2 / 8 );

    // DISABLED FOR DEBUG PURPOSE //
    /* If the new impedance is outside the human limits it means that the
       electrodes may not be attached to the patient. Therefore we assume
       a lead-off.
       */
    /*if(new_impedance < Z_MIN || new_impedance > Z_MAX){
        Patient_impedance = 0;
        lead_detected = false;
        return false;
    }*/
    // DISABLED FOR DEBUG PURPOSE //

    // DISABLED FOR DEBUG PURPOSE //
    /* This is optional, but enabled by default. */
    /* If the last acquisition was drastically different it is likely that the
       electrodes went off.
       */
    /*
    if(Patient_impedance != 0){
        double imp_ratio = new_impedance / Patient_impedance;
        if(imp_ratio > 1 + IMPEDANCE_DEVIATION){
            Patient_impedance = 0;
            lead_detected = false;
            return false;
        }
        if(imp_ratio < 1 - IMPEDANCE_DEVIATION){
            Patient_impedance = 0;
            lead_detected = false;
            return false;
        }
    }*/
    // DISABLED FOR DEBUG PURPOSE //

    /* If the impedance calculations are correct, overwrite the old impedance */
    Patient_impedance = new_impedance;
    return true;
}

void OAED_EnableChargingCircuit(){
    /* Disable the protection resistor connection. */
    OAED_HBridgeControl( OPEN_CIRCUIT );
    /* Disarm Defibrillator. */
    OAED_DisarmDefibrillator(false);

    /* Enable the charging circuit. */
    CyPins_SetPin(Charge_En_0);

    /* Enable capacitor sense. */
    Comp_CapReady_Start();
    Comp_CapLow_Start();
    isr_CapReady_Enable();

    return;
}

void OAED_DisableChargingCircuit(){
    /* Disable Charging circuit. */
    CyPins_ClearPin(Charge_En_0);

    /* Disable capacitor sense. */
    isr_CapReady_Disable();
    Comp_CapReady_Stop();
    Comp_CapLow_Stop();

    return;
}
/* End of function declarations */

/* [] END OF FILE */
