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

/* Numeric Constants */
#define USBFS_DEVICE        0       /* Device                   */
#define USBFS_BUFFER_SIZE   512     /* Buffer Size (max 512)    */

/* Macro */
#define OAED_USBSendData(data) OAED_USBSendData_impl(data, sizeof(data))
#define OAED_USB_ECHO           false

/* Global variables */
extern uint8 USB_buffer[USBFS_BUFFER_SIZE];     /* USB Buffer */
extern bool flag;                               /* Flag, used mostly for debug purpose */


/* Function prototypes */

/* -- Basic functions */
void OAED_USBInit();
void OAED_USBConfigure();
void OAED_USBSendString(char[]);
void OAED_USBSendData8(int8[], uint16);
void OAED_USBSendData16(int16[], uint16);
void OAED_USBSendData_impl(void*, size_t);

/* -- Send data as String */
void OAED_USBPrintECG();
void OAED_USBPrintECGB();
void OAED_USBPrintZ();
void OAED_USBPrintSystemImage();
void OAED_USBPrintTimeStamp();
void OAED_USBPrintFlag();

/* -- Interactive functions */
bool OAED_USBGetCommand();
void OAED_USBInteractiveMode();

/* -- Send raw data */
void OAED_USBSendSystemImage();
void OAED_USBSendECG();
void OAED_USBSendZ();
void OAED_USBSendBuffer();




#endif


/* [] END OF FILE */