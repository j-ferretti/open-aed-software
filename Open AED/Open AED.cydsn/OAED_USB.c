/* ========================================
 *
 *  OPEN SOURCE AED
 * This file contains all USB
 * function and global variable
 * definitions.
 *
 * ========================================
*/

#include <project.h>
#include "OAED_USB.h"

/* Declaration of global variables */
uint8 OAED_USBbuffer[USBFS_BUFFER_SIZE];
bool flag = false;

/* Function declarations */
void OAED_USBInit(){
    
    USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);
    OAED_USBConfigure();
    //OAED_InitTime();
    
    return;
}

void OAED_USBConfigure(){
    /* Host can send double SET_INTERFACE request. */
    if( USBUART_IsConfigurationChanged() != 0 ){
        /* Initialize IN endpoints when device is configured. */
        if( USBUART_GetConfiguration() != 0 ){
            /* Enumeration is done, enable OUT endpoint to receive data 
             * from host. */
            USBUART_CDC_Init();
        }
    }
    return;
}

void OAED_USBSendString(char message[]){
    OAED_USBConfigure();
    while( USBUART_CDCIsReady() == 0 ){
        /* Wait cdc to be ready */
    }
        /* Send the message */
    USBUART_PutString(message);
    return;
}

void OAED_USBPrintECG(){
    uint16 i;
    char message[USBFS_BUFFER_SIZE];
    
    sprintf(message,"-- Data ECG\r\n");
    OAED_USBSendString(message);
    
    for( i = 0 ; i < ECG_Data_size ; i++ ){
        sprintf(message,"%d - %d\r\n",i,DataECG[i]);
        OAED_USBSendString(message);
    }
    OAED_USBSendString("-- Data ECG\r\n");
    
    return;
}

void OAED_USBPrintECGB(){
    uint16 i;
    char message[USBFS_BUFFER_SIZE];
    
    sprintf(message,"-- Buffer ECG\r\n");
    OAED_USBSendString(message);
    
    for( i = 0 ; i < ECG_Data_size ; i++ ){
        if(BufferECG[i] != 0){
            sprintf(message,"%d - %d\r\n",i,BufferECG[i]);
            OAED_USBSendString(message);
        }
    }
    OAED_USBSendString("-- Buffer ECG\r\n");
    
    return;
}

void OAED_USBPrintZ(){
    uint16 i;
    char message[USBFS_BUFFER_SIZE];
    
    sprintf(message,"-- Data Z\r\n");
    OAED_USBSendString(message);
    
    for( i = 0 ; i < Z_Data_size ; i++ ){
        sprintf(message,"%d - %ld\r\n",i,DataZ[i]);
        OAED_USBSendString(message);
    }
    OAED_USBSendString("\r\n");
    
    return;
}

void OAED_USBPtintTimeStamp(){
    OAED_USBSendString("\r\n");
    #if(OAED_TIME)
        OAED_GetTime();
        OAED_USBSendString(TimeStamp);
    #endif
    OAED_USBSendString("\r\n");
    return;
}

void OAED_USBPrintFlag(){
    char message[USBFS_BUFFER_SIZE];
    sprintf(message, "%1d\r\n", flag);
    OAED_USBSendString(message);
    return;
}

void OAED_USBPrintSystemImage(){
    char message[USBFS_BUFFER_SIZE];
    bool tmp;
    
    OAED_USBPtintTimeStamp();
    OAED_USBSendString("\r\n");
    sprintf(message,"-- System Status\r\n");
    OAED_USBSendString(message);
    sprintf(message,"ECG_buffer_full    : %1d\r\n",ECG_buffer_full);
    OAED_USBSendString(message);
    sprintf(message,"ECG_data_pending   : %1d\r\n",ECG_data_pending);
    OAED_USBSendString(message);
    sprintf(message,"Z_buffer_full      : %1d\r\n",Z_buffer_full);
    OAED_USBSendString(message);
    sprintf(message,"lead_detected      : %1d\r\n",lead_detected);
    OAED_USBSendString(message);
    sprintf(message,"capacitor_ready    : %1d\r\n",capacitor_ready);
    OAED_USBSendString(message);
    sprintf(message,"ECG_enabled        : %1d\r\n",ECG_enabled);
    OAED_USBSendString(message);
    sprintf(message,"Patient Impedance  : %lf\r\n",Patient_impedance);
    OAED_USBSendString(message);
    
    OAED_USBSendString("\r\n");
    sprintf(message,"-- Pin Status\r\n");
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Status_Led_Red) != 0;
    sprintf(message,"Red led            : %1d\r\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Status_Led_Yellow) != 0;
    sprintf(message,"Yellow led         : %1d\r\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Status_Led_Green) != 0;
    sprintf(message,"Green led          : %1d\r\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Charge_En_0) != 0;
    sprintf(message,"Charge EN          : %1d\r\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Defibrillation_En_Inner) != 0;
    sprintf(message,"Inner Defib EN     : %1d\r\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Defibrillation_En_Outer) != 0;
    sprintf(message,"Outer Defib EN     : %1d\r\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Phase_Pin_Phi1) != 0;
    sprintf(message,"H-Bridge Phase 1   : %1d\r\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Phase_Pin_Phi2) != 0;
    sprintf(message,"H-Bridge Phase 2   : %1d\r\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Comp_Pin_n) != 0;
    sprintf(message,"n-Comparator       : %1d\r\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Comp_Pin_p) != 0;
    sprintf(message,"p-Comparator       : %1d\r\n",tmp);
    OAED_USBSendString(message);
    OAED_USBSendString("\r\n");
    
    
    return;

}

void OAED_USBGetCommand(){
    OAED_USBConfigure();
    if( USBUART_DataIsReady() == 0 ){
        return;
    }
    OAED_Led(false,true,false);
    
    uint16 count;
    uint8 message[USBFS_BUFFER_SIZE];
    count = USBUART_GetAll(message);
    if(count == 0)
        return;
    
    char command;
    command = message[0];
    OAED_USBSendString((char*)message);
    
    switch(command){
        case 's':
            OAED_USBPrintSystemImage();
            return;
        case 'e':
            OAED_USBPrintECG();
            return;
        case 'z':
            OAED_USBPrintZ();
            return;
        case 'b':
            OAED_USBPrintECGB();
            return;
        default:
            return;
    }
    OAED_Led(false,false,false);
    
    return;
}

/* [] END OF FILE */
