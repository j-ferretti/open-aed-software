/* ========================================
 *
 *  OPEN SOURCE AED
 * This header contains all USB
 * functions prototyes and global variable
 * declarations.
 *
 * ========================================
*/

// DEBUG MODE ONLY //

#ifndef DEBUG_COMMON_H
#define DEBUG_COMMON_H

#include <project.h>
#include <stdio.h>
#include "OAED_Common.h"
#if(OAED_TIME)
    #include "OAED_Time.h"
#endif
//#include "DEBUG_Time.h"

#define USBFS_DEVICE        0       /* Device                   */
#define USBFS_BUFFER_SIZE   512     /* Buffer Size (max 512)    */
    
/* Global variables */
extern uint8 USB_buffer[USBFS_BUFFER_SIZE];     /* USB Buffer */
extern bool flag;                               /* Flag, used mostly for debug purpose */
    
/* Function prototypes */
void OAED_USBInit();
void OAED_USBSendString(char[]);
void OAED_USBConfigure();
void OAED_USBPrintECG();
void OAED_USBPrintECGB();
void OAED_USBPrintZ();
void OAED_USBPrintSystemImage();
void OAED_USBPtintTimeStamp();
void OAED_USBPrintFlag();
void OAED_USBGetCommand();
    
#endif


/* [] END OF FILE */