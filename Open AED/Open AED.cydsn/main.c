/* ========================================
 *
 *  OPEN SOURCE AED
 * Main
 *
 * ========================================
*/

#include <project.h>
#include "OAED_Common.h"
#include "OAED_DMA.h"
#include "OAED_ISR.h"
#include "OAED_Defibrillation.h"


int main()
{
    /* Local variables declaration */
    char Status = measurement_mode;
    uint8 false_positive_count = 0;

    /* System initialization */
    OAED_Init();
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Main Loop */
    for(;;)
    {
        OAED_SetSystemStatus(Status); //Enable or disable specific state functionalities
        
        switch(Status){
            case lead_off:
                /* Reset event data */
                OAED_ResetEvent();
                /* While in lead-off mode the system will wait for lead detection.  */
                OAED_WaitLeadOn();
                
                OAED_ZAcquisition();
                OAED_WaitForZ();
                
                if(lead_detected)
                    Status = measurement_mode;
                continue;
            
            case measurement_mode:
                while(Status == measurement_mode){
                    /* While in measurement mode the system checks for, lead-off,   */
                    /* patient impedance and VT or VF.                              */
                    
                    
                    /* Start ECG acquisition and check for lead-off */
                    if(!OAED_StartAcquisition()){
                        Status = lead_off;
                        break;
                    }
                    
                    /* Check for new data */
                    if(ECG_data_pending){
                        /* If new data is available the system evaluate the rhytm */
                        if(OAED_EvaluateRhythm()){
                            /* If VF/VT are found the system start charging the capacitor */
                            Status = charging_capacitor;
                        }
                    }
                    
                    /* Wait for new data */
                    if(!OAED_WaitForData()){
                        Status = lead_off;
                        break;
                    }
                    
                    /* Stop ECG acquisition */
                    OAED_StopAcquisition();
                    
                }
                continue;
                
            case charging_capacitor:
                false_positive_count = 0; /* Reset the false positive counter */
                while(Status == charging_capacitor){
                    /* In this mode of operation the system works the same as in    */
                    /* measurement mode, but meanwhile the capacitor charging       */
                    /* circuit is enabled. This mode is only entered if VF or VT    */
                    /* are detected for 2 out of 3 period of time.                  */
                    
                    /* Start ECG acquisition and check for lead-off */
                    if(!OAED_StartAcquisition()){
                        /* In case the system can't detect the leads on it activate */
                        /* the internal discharge to avoid possible hazardous       */
                        /* situations.                                              */
                        Status = internal_discharge;
                        break;
                    }
                    
                    /* Check for new data */
                    if(ECG_data_pending){
                        /* If new data is available the system evaluate the rhytm   */
                        if(!OAED_EvaluateRhythm()){
                            /* If no VF/VT are found the false positive counter is  */
                            /* increased. Then if in the last 5 time period no      */
                            /* VT/VF events are found, it means that it was a false */
                            /* positive.                                            */
                            if(++false_positive_count > 5){
                                Status = internal_discharge;
                                break;
                            }
                        }
                        else{
                            /* If VF/VT is detected the system wait for the         */
                            /* capacitor to charge.                                 */
                            if(OAED_WaitForCap()){
                                /* If the capacitor is charged before new data is   */
                                /* available the system switch to discharge enabled */
                                /* mode.                                            */
                                Status = discharge_enabled;
                                break;
                            }
                        }
                    }
                    
                    /* Wait for new data */
                    if(!OAED_WaitForData()){
                        Status = internal_discharge;
                        break;
                    }
                    
                    /* Stop ECG acquisition */
                    OAED_StopAcquisition();
                    
                }
                continue;
                
            case discharge_enabled:
                while(Status == discharge_enabled){
                    /* In this mode the capacitor is charged and the system is      */
                    /* waiting for the operator to push the discharge button.       */
                    
                    /* Start ECG acquisition and check for lead-off */
                    if(!OAED_StartAcquisition()){
                        /* In case the system can't detect the leads on it activate */
                        /* the internal discharge to avoid possible hazardous       */
                        /* situations.                                              */
                        Status = internal_discharge;
                        break;
                    }
                    
                    /* Check for new data */
                    if(ECG_data_pending){
                        /* If new data is available the system evaluate the rhytm   */
                        if(!OAED_EvaluateRhythm()){
                            /* If no VF/VT are found the system go back in charging */
                            /* mode. After 5 more period of time without VT/VF its  */
                            /* assumed it was a false positive.                     */
                            Status = charging_capacitor;
                            break;
                        }
                    }
                    
                    /* Wait for new data */
                    if(!OAED_WaitForData()){
                        Status = internal_discharge;
                        break;
                    }
                    
                    /* Stop ECG acquisition */
                    OAED_StopAcquisition();
                    
                }
                continue;
                
            case internal_discharge:
                /* This mode is entered when the charge of the capacitor is no  */
                /* longer needed, or is potentially dangerous (ie lead off      */
                /* detected).                                                   */
                
                /* Stop ECG acquisition.                                        */
                if(ECG_enabled)
                    OAED_StopAcquisition();
                /* Disarm and release the charge.                               */
                OAED_DisarmDefibrillator(true);
                /* Reset event data                                             */
                OAED_ResetEvent();
                
            default:
                /* This shouldn't happen.                                        */
                Status = lead_off;
                continue;
        }
        
    }
}

/* [] END OF FILE */
