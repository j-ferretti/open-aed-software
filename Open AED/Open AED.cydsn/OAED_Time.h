/* ========================================
 *
 *  OPEN SOURCE AED
 * This header contains all time debug
 * functions prototyes and global variable
 * declarations.
 *
 * ========================================
*/

#ifndef OAED_TIME_H
#define OAED_TIME_H
    
#include <project.h>
#include <stdio.h>
#include "OAED_Common.h"
    
    
    
/* Time Variables */
extern uint16   mscount;
extern uint16   seccount;
extern uint16   mincount;
extern char     TimeStamp[];
    
/* Function prototypes */
void OAED_InitTime();
void OAED_SysTickISRCallback();
void OAED_GetTime();
    
#endif

/* [] END OF FILE */
