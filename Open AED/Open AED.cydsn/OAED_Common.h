/* ========================================
 *
 *  OPEN SOURCE AED
 * This header contains all common
 * functions prototyes and global variable
 * declarations.
 *
 * ========================================
*/

#ifndef OAED_COMMON_H
#define OAED_COMMON_H
    
/* Compilation Options */
#define OAED_TIME               1            // Enable or disable time functionalities

#include <project.h>
#include <stdbool.h>
#include "OAED_Defibrillation.h"
#include "OAED_DMA.h"
#include "OAED_ISR.h"
#include "OAED_USB.h"
#if(OAED_TIME)
    #include "OAED_Time.h"
#endif

/* Numeric constants */
#define EVENT_NO                3            // Number of event registered
#define POSITIVE_EVENT_NO       2            // Number of positive event required for defibrillation
#define ECG_signal_length       4            // Seconds of signal registered
#define ECG_sampling_f          250          // Sampling frequency of ECG signal
#define ECG_Data_size           ECG_signal_length * ECG_sampling_f   
                                             // Size of ECG data/buffer vectors
#define Z_Data_size             100          // Size of Z data/buffer vector at a constant rate of 100 sps (1 second total)
#define Z_min                   25           // Minimum impedance
#define Z_max                   180          // Maximum impedance
#define impedance_deviation     0.50         // Maximum deviation for impedance measurement
#define C              (double) 0.00015      // Condensator capacity
#define V              (double) 1700         // Maximum voltage
#define U              (double) 216          // Maximum energy stored

/* Variable definitions */
extern int16 BufferECG[];                    // ECG Buffer
extern int16 DataECG[];                      // ECG Data Vector
extern int16 BufferZ[];                      // Z Buffer
extern int16 DataZ[];                        // Z Data Vector
extern bool Event_flags[];                   // VT/VF Event Flags
extern double Patient_impedance;             // Patient impedance

/* Declaration of system flags */
extern bool ECG_buffer_full;                 // ECG Buffer Status
extern bool Z_buffer_full;                   // Z Buffer Status
extern bool lead_detected;                   // Lead Detected
extern bool ECG_data_pending;                // New ECG data available
extern bool capacitor_ready;                 // Capacitor status
extern bool ECG_enabled;                     // ECG acquisition status
extern bool Z_enabled;                       // Z acqiuisition status


/* System Status */
enum system_status { lead_off, measurement_mode ,
    charging_capacitor , discharge_enabled ,
    internal_discharge };

/* Function prototypes */
void OAED_Init();
void OAED_CopyECGBuffer();
void OAED_CopyZBuffer();
void OAED_Led(bool, bool, bool);
void OAED_SetSystemStatus(char);

void OAED_ResetEvent();
bool OAED_EvaluateRhythm();
bool OAED_CheckFlags();
bool OAED_EvaluateImpedance();

void OAED_WaitLeadOn();
bool OAED_WaitForData();
bool OAED_WaitForCap();
void OAED_WaitForZ();

bool OAED_StartAcquisition();
void OAED_StopAcquisition();
void OAED_ZAcquisition();

void OAED_InitAcquisition();
void OAED_InitFilter();

void OAED_EnableChargingCircuit();
void OAED_DisableChargingCircuit();

#endif
/* [] END OF FILE */
