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
#define OAED_TIME               true         // Enable or disable time
                                             // functionalities.
#define RAW_MODE                true         // Enable or disable raw ECG
                                             // acquisition.
/* End of compilation options */

/* Include */
#include <project.h>
#include <stdbool.h>
#include "OAED_Acquisition.h"
#include "OAED_Defibrillation.h"
#include "OAED_DMA.h"
#include "OAED_ISR.h"
#include "OAED_SystemStatus.h"
#include "OAED_USB.h"
#include "OAED_Wait.h"
#if(OAED_TIME)
    #include "OAED_Time.h"
#endif
/* End of include */

/* Numeric constants */
/* Events */
#define EVENT_NO                3            // Number of event registered
#define POSITIVE_EVENT_NO       2            // Number of positive event
                                             // required for defibrillation
/* ECG */
#define ECG_SIGNAL_LENGTH       4            // Seconds of signal registered [s]
#define ECG_SAMPLING_F          500          // Sampling frequency of ECG signal
                                             // [Hz]
#define ECG_DATA_SIZE           ECG_SIGNAL_LENGTH * ECG_SAMPLING_F
                                             // Size of ECG data/buffer vectors
/* Impedance */
#define Z_DATA_SIZE             125          // Size of Z data/buffer vector at
                                             // a constant rate of 125 sps
                                             // (1 second total) [Hz]
#define Z_MIN                   25           // Minimum impedance [Ohm]
#define Z_MAX                   180          // Maximum impedance [Ohm]
#define IMPEDANCE_DEVIATION     0.50         // Maximum deviation for impedance
                                             // measurement

/* Hardware */
#define C              (double) 0.00015      // Condensator capacity [F]
#define V              (double) 1700         // Maximum voltage [V]
#define U              (double) 200          // Target energy to deliver [J]
#define U_MAX          (double) 216          // Maximum storable energy [J]

/* End of numeric constants */


/* Variable definitions */
// Buffers
extern int16 BufferECG[];                    // ECG Buffer
extern int16 BufferZ[];                      // Z Buffer
// Data
extern int16 DataECG[];                      // ECG Data Vector
extern int16 DataZ[];                        // Z Data Vector
// Impedance
extern double Patient_impedance;             // Patient impedance
// Raw data
#if(RAW_MODE)
    extern int16 rawECG[];                   // Raw ECG Data
    extern int16 rawECGBuffer[];             // Raw ECG Buffer
#endif
/* End of variable definitions */

/* Declaration of system flags */
extern bool ECG_buffer_full;                 // ECG Buffer Status
extern bool Z_buffer_full;                   // Z Buffer Status
extern bool ECG_data_pending;                // New ECG data available

extern bool ECG_enabled;                     // ECG acquisition status
extern bool Z_enabled;

extern bool lead_detected;                   // Lead Detected
extern bool capacitor_ready;                 // Capacitor status

extern bool Event_flags[];                   // VT/VF Event Flags

/* End of system flags */

/* Function prototypes */
void OAED_Init();
void OAED_InitFilter();

void OAED_CopyECGBuffer();
void OAED_CopyZBuffer();

void OAED_Led(bool, bool, bool);

void OAED_ResetEvent();
bool OAED_CheckFlags();

bool OAED_EvaluateRhythm();
bool OAED_EvaluateImpedance();

void OAED_EnableChargingCircuit();
void OAED_DisableChargingCircuit();
/* End of function prototypes */

#endif
/* [] END OF FILE */
