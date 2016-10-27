/* ========================================
 *
 *  OPEN SOURCE AED
 * This header contains all acquisition
 * functions prototyes and global variable
 * declarations.
 *
 * ========================================
*/

#ifndef OAED_ACQUISITION_H
#define OAED_ACQUISITION_H

/* Include */
#include <project.h>
#include "OAED_Common.h"
/* End of include */

/* Function prototypes */
void OAED_InitAcquisition();

bool OAED_StartAcquisition();
void OAED_StopAcquisition();
void OAED_ZAcquisition();
/* End of function prototypes */

#endif
/* [] END OF FILE */
