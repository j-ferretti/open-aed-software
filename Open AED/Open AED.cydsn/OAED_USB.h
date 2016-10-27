/* ========================================
 *
 *  OPEN SOURCE AED
 * This header contains all USB
 * functions prototyes and global variable
 * declarations.
 *
 * ========================================
*/

#ifndef OAED_USB_H
#define OAED_USB_H

/* Include */
#include <project.h>
#include <stdio.h>
#include "OAED_Common.h"
#if(OAED_TIME)
    #include "OAED_Time.h"
#endif
/* End of include */

/* Numeric Constants */
#define USBFS_DEVICE        0       // Device
#define USBFS_BUFFER_SIZE   512     // Buffer Size (max 512)
#define OAED_USB_ECHO       false   // GetData echoes (Default false)
/* End of numeric constants */

/* Macro */
#define OAED_USBSendData(message) OAED_USBSendDataVoid(message, sizeof(message))
#define OAED_USBSendData16(message,n) OAED_USBSendData8((int8*) message, 2*n)
/* Send data as 8-bit array. LSB first. */

#define OAED_ShiftNAdd(data,flag) (data << 1) + flag
/* Shift left one bit and add the flag. */

/* End of Macro*/

/* Global variables */
extern bool flag;                   // Flag used for debug purpose
/* End of global variables */

/* Function prototypes */

    /* WARNING: Any Send/Print function is a blocking function. */

/* Basic functions */
void OAED_USBInit();
void OAED_USBConfigure();
void OAED_USBSendString(char[]);
void OAED_USBSendData8(int8[], uint16);
void OAED_USBSendDataVoid(void*, size_t);
bool OAED_USBGetCommand();

/* Send data as String. MOSTLY DEPRECATED. */
void OAED_USBPrintECG();
void OAED_USBPrintECGB();
void OAED_USBPrintZ();
void OAED_USBPrintSystemImage();
void OAED_USBPrintTimeStamp();
void OAED_USBPrintFlag();

/* Interactive functions */
uint16 OAED_USBGetData(uint8[], bool);
void OAED_USBInteractiveMode();

/* Send data */
void OAED_USBSendSystemImage();
void OAED_USBSendECG();
void OAED_USBSendZ();
void OAED_USBSendBuffer();
#if(RAW_MODE)
void OAED_USBSendRAW();
#endif
/* End of function prototypes */

#endif

/* [] END OF FILE */
