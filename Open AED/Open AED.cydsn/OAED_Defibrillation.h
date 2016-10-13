/* ========================================
 *
 *  OPEN SOURCE AED
 * This header contains all defibrillation
 * related function prototypes and variable
 * declarations.
 *
 * ========================================
*/

#ifndef OAED_DEFIBRILLATION_H
#define OAED_DEFIBRILLATION_H
    
#include <stdbool.h>
#include <project.h>
#include <math.h>
#include "OAED_Common.h"
    
#define phi_0           (uint8)(0b00000000)                     
#define phi_1           (uint8)(0b00000010)
#define phi_2           (uint8)(0b00000001)
#define PWM_length      5000
#define PWM_duty        50
#define PWM_on          (uint16)( PWM_length * PWM_duty / 100 )
#define PWM_off         (uint16)( PWM_length - PWM_on )
    
/* Function prototypes */
void OEAD_EnableDefibrillation();
void OAED_DisableDefibrillation();
void OAED_ArmDefibrillator();
void OAED_DisarmDefibrillator(bool);
void OAED_HBridgeControl(uint8);
uint32 OAED_EvaluateDischargeTime();

void OAED_InternalDischarge();
void OAED_MonophasicDefibrillation();
void OAED_PolyphasicDefibrillation(uint8);
void OAED_BiphasicDefibrillation(uint8);
    
#endif



/* [] END OF FILE */
