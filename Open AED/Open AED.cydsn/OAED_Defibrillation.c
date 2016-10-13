/* ========================================
 *
 *  OPEN SOURCE AED
 * This file contains all defibrillation
 * related function and global variable
 * definitions.
 *
 * ========================================
*/

#include "OAED_Defibrillation.h"

uint32 OAED_EvaluateDischargeTime(){
    uint32 time;
    /* Time formula. */
    time = (uint32)( - Patient_impedance * C * log( 1 - 2*U/(V * V * C) ) );
    /* Express time in ms */
    time = time * 1000;
    return time;
}

void OAED_EnableDefibrillation(){
    /* Stop ECG acquisition.*/
    if(ECG_enabled)
        OAED_StopAcquisition();
    
    /* Switch inner and outer security relay. */
    CyPins_SetPin(Defibrillation_En_Inner);
    CyPins_SetPin(Defibrillation_En_Outer);
    
    /* Wait for the relay to set. */
    CyDelay(20);
    
    return;
}

void OAED_DisableDefibrillation(){
    /* Switch inner and outer security relay. */
    CyPins_ClearPin(Defibrillation_En_Inner);
    CyPins_ClearPin(Defibrillation_En_Outer);
    
    /* Wait for the relay to set. */
    CyDelay(20);
    
    return;
}

void OAED_ArmDefibrillator(){
    /* Enable defibrillator switch interrupt. */
    isr_Defibrillate_Enable();
    return;
}

void OAED_DisarmDefibrillator(bool charge_release){
    /* Disable defibrillator switch interrupt. */
    isr_Defibrillate_Disable();
    
    /* If true, activate internal discharge. */
    if(charge_release)
        OAED_InternalDischarge();
    return;
}

void OAED_HBridgeControl(uint8 phase){
    Phase_Reg_Write( phase );
    return;
}

void OAED_InternalDischarge(){
    /* Avoid any possibilities to release the charge to the     */
    /* patient. Since a lot of current need to be released, the */
    /* discharge is modulated with a PWM like control.          */
    
    /* Avoid any risk of accidental defibrillation.             */
    OAED_DisableDefibrillation();
    
    /* The PWM discharge last until the capacitor voltage go    */
    /* below the threshold.                                     */
    while(capacitor_ready){
        /* Activate a monophasic PWM discharge.                 */
        OAED_HBridgeControl( phi_0 );
        CyDelayUs(PWM_off);
   
        OAED_HBridgeControl( phi_1 );
        CyDelayUs(PWM_on);
    }
    /* After reaching the threshold it's safe to release the    */
    /* remaining charge as a continuous discharge.              */
    return;
    /* We don't need to wait for the capacitor to fully         */
    /* discharge because it's going to need a lot of time.      */
    /* Also, this way is safer since the capacitor can't store  */
    /* energy until the circuit is opened again when the next   */
    /* charge command is called.                                */
}

void OAED_MonophasicDefibrillation(){
    uint32 time;
    
    /* Get discharge time. */
    time = OAED_EvaluateDischargeTime();
    
    /* Enable patient defibrillation. */
    OAED_EnableDefibrillation();
    
    /* Start Defibrillation. */
    OAED_HBridgeControl( phi_1 );
    /* Wait for the charge to be delivered. */
    CyDelay(time);
    /* Stop Defibrillation */
    OAED_HBridgeControl( phi_0 );
    
    /* Disable patient defibrillation. */
    OAED_DisableDefibrillation();
    
    /* Discharge residual charge. (optional) */
    //OAED_InternalDischarge();
    
    return;
}

void OAED_PolyphasicDefibrillation(uint8 phase_no){
    uint32 total_time;
    uint32 phase1_time;
    uint32 phasen_time;
    uint8 i;
    
    /* Get discharge time. */
    total_time = OAED_EvaluateDischargeTime();
    phase1_time = total_time / phase_no;
    phasen_time = total_time - phase_no * phase1_time;
    if(phasen_time <3){
        OAED_BiphasicDefibrillation(50);
        return;
    }
    
    /* Enable patient defibrillation. */
    OAED_EnableDefibrillation();
    /* Start Defibrillation. */
    Phase_Reg_Write( phi_1 );
    /* Wait for the phase 1 charge to be delivered. */
    CyDelay(phase1_time);
    /* Stop Defibrillation for 1 msec */
    OAED_HBridgeControl( phi_0 );
    CyDelay(1);
    for( i=1; i<phase_no ; i++){
        /* Invert phase */
        if(i%2 == 1)
            OAED_HBridgeControl(phi_2);
        else
            OAED_HBridgeControl(phi_1);
        /* Wait for the charge to be delivered. */
        CyDelay(phasen_time);
        /* Stop Defibrillation. */
        OAED_HBridgeControl( phi_0 );
        CyDelayUs(500);
       
    }
    /* Disable patient defibrillation. */
    OAED_DisableDefibrillation();
    
    /* Discharge residual charge. (optional) */
    //OAED_InternalDischarge();
    
    return;
}

void OAED_BiphasicDefibrillation(uint8 phase_one_perc){
    uint32 total_time;
    uint32 phase1_time;
    uint32 phase2_time;
    
    /* Get discharge time. */
    total_time = OAED_EvaluateDischargeTime();
    phase1_time = total_time * phase_one_perc / 100;
    phase2_time = total_time - phase1_time;
    
    /* Enable patient defibrillation. */
    OAED_EnableDefibrillation();
    
    /* Start Defibrillation. */
    OAED_HBridgeControl( phi_1 );
    /* Wait for the phase 1 charge to be delivered. */
    CyDelay(phase1_time);
    /* Stop Defibrillation for 1 msec */
    OAED_HBridgeControl( phi_0 );
    CyDelay(1);
    /* Invert phase */
    Phase_Reg_Write(phi_2);
    /* Wait for the phase 2 charge to be delivered. */
    CyDelay(phase2_time);
    /* Stop Defibrillation. */
    OAED_HBridgeControl( phi_0 );
    
    /* Disable patient defibrillation. */
    OAED_DisableDefibrillation();
    
    /* Discharge residual charge. (optional) */
    //OAED_InternalDischarge();
    
    return;
}

/* [] END OF FILE */