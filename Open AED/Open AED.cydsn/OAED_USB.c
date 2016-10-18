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
    /* WARNING: Any Send/Print function is a blocking function. */


void OAED_USBInit(){
    
    USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);
    OAED_USBConfigure();
    
    return;
}

void OAED_USBConfigure(){
    if( USBUART_IsConfigurationChanged() != 0 ){
        if( USBUART_GetConfiguration() != 0 ){
            USBUART_CDC_Init();
        }
    }
    return;
}

void OAED_USBSendString(char message[]){
    /* Check USBUART configuration. */
    OAED_USBConfigure();
    /* Wait CDC to be ready. */
    while( USBUART_CDCIsReady() == 0 ){}
    /* Send the message. */
    USBUART_PutString(message);
    return;
}

void OAED_USBSendData_impl(void* data, size_t nsize){
    /* Used with macro OAED_USBSendData(data) defined on header.            */
    
    /* Treat any data type to int8.                                         */
    /* If data type is wider than 8-bit is required a reconstruction on PC  */
    /* side, on this purpose LSB is sent first and MSB last.                */
    
    OAED_USBSendData8((int8*)data, nsize );
    return;
}

void OAED_USBSendData8(int8 message[], uint16 n){
    /* Check USBUART configuration. */
    OAED_USBConfigure();
    
    uint16 BC;              /* Byte count */
    uint16 MI = 0;          /* Message index */
    
    do{
        /* Wait cdc to be ready. */
        while( USBUART_CDCIsReady() == 0 ){}
        
        /* USBUART_PutData can only transfer 64 byte per call.              */
        /* Everything else is lost, so we divide the messaga in 64 byte     */
        /* packages.                                                        */
        BC = (n>64) ? 64 : n;
        
        /* Send message package. */
        USBUART_PutData( (const uint8*) &message[MI], BC);
        
        /* Update the index and message dimension. */
        MI += BC;
        n -= BC;
        
    }while(n>0); /* Repeat until the whole message is sent. */
    
    return;
}

void OAED_USBSendData16(int16 message[], uint16 n){
    /* Send data as 8-bit array. LSB first.         */
    OAED_USBSendData8((int8*) message, 2*n);
    /* USBUART only support 8-bit data transfer.    */
    return;
}

void OAED_USBPrintECG(){
    uint16 i;
    char message[USBFS_BUFFER_SIZE];
    
    sprintf(message,"-- Data ECG\n");
    OAED_USBSendString(message);
    
    for( i = 0 ; i < ECG_Data_size ; i++ ){
        sprintf(message,"%d - %d\n",i,DataECG[i]);
        OAED_USBSendString(message);
    }
    OAED_USBSendString("-- Data ECG\n");
    
    return;
}

void OAED_USBPrintECGB(){
    uint16 i;
    char message[USBFS_BUFFER_SIZE];
    
    sprintf(message,"-- Buffer ECG\n");
    OAED_USBSendString(message);
    
    for( i = 0 ; i < ECG_Data_size ; i++ ){
        if(BufferECG[i] != 0){
            sprintf(message,"%d - %d\n",i,BufferECG[i]);
            OAED_USBSendString(message);
        }
    }
    OAED_USBSendString("-- Buffer ECG\n");
    
    return;
}

void OAED_USBPrintZ(){
    uint16 i;
    char message[USBFS_BUFFER_SIZE];
    
    sprintf(message,"-- Data Z\n");
    OAED_USBSendString(message);
    
    for( i = 0 ; i < Z_Data_size ; i++ ){
        sprintf(message,"%d - %d\n",i,DataZ[i]);
        OAED_USBSendString(message);
    }
    OAED_USBSendString("\n");
    
    return;
}

void OAED_USBPrintTimeStamp(){
    #if(OAED_TIME)
    OAED_USBSendString("\n");
        OAED_GetTime();
        OAED_USBSendString(TimeStamp);
    OAED_USBSendString("\n");
    #endif
    return;
}

void OAED_USBPrintFlag(){
    char message[USBFS_BUFFER_SIZE];
    sprintf(message, "%1d\n", flag);
    OAED_USBSendString(message);
    return;
}

void OAED_USBPrintSystemImage(){
    char message[USBFS_BUFFER_SIZE];
    bool tmp;
    
    OAED_USBPrintTimeStamp();
    OAED_USBSendString("\n");
    sprintf(message,"-- System Status\n");
    OAED_USBSendString(message);
    sprintf(message,"ECG_buffer_full    : %1d\n",ECG_buffer_full);
    OAED_USBSendString(message);
    sprintf(message,"ECG_data_pending   : %1d\n",ECG_data_pending);
    OAED_USBSendString(message);
    sprintf(message,"Z_buffer_full      : %1d\n",Z_buffer_full);
    OAED_USBSendString(message);
    sprintf(message,"lead_detected      : %1d\n",lead_detected);
    OAED_USBSendString(message);
    sprintf(message,"capacitor_ready    : %1d\n",capacitor_ready);
    OAED_USBSendString(message);
    sprintf(message,"ECG_enabled        : %1d\n",ECG_enabled);
    OAED_USBSendString(message);
    
    sprintf(message,"Patient Impedance  : %ld\n",(int32)floor(Patient_impedance));
    OAED_USBSendString(message);
    
    OAED_USBSendString("\n");
    sprintf(message,"-- Pin Status\n");
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Status_Led_Red) != 0;
    sprintf(message,"Red led            : %1d\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Status_Led_Yellow) != 0;
    sprintf(message,"Yellow led         : %1d\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Status_Led_Green) != 0;
    sprintf(message,"Green led          : %1d\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Charge_En_0) != 0;
    sprintf(message,"Charge EN          : %1d\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Defibrillation_En_Inner) != 0;
    sprintf(message,"Inner Defib EN     : %1d\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Defibrillation_En_Outer) != 0;
    sprintf(message,"Outer Defib EN     : %1d\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Phase_Pin_Phi1) != 0;
    sprintf(message,"H-Bridge Phase 1   : %1d\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Phase_Pin_Phi2) != 0;
    sprintf(message,"H-Bridge Phase 2   : %1d\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Comp_Pin_n) != 0;
    sprintf(message,"n-Comparator       : %1d\n",tmp);
    OAED_USBSendString(message);
    tmp = CyPins_ReadPin(Comp_Pin_p) != 0;
    sprintf(message,"p-Comparator       : %1d\n",tmp);
    OAED_USBSendString(message);
    OAED_USBSendString("\n");
    
    
    return;
}

void OAED_USBSendI(){
    char message[USBFS_BUFFER_SIZE];
    bool tmp;

    OAED_USBPrintTimeStamp();
    OAED_USBSendString("\n");
    //tmp = CyPins_ReadPin(Comp_Pin_n) != 0;
    tmp = Comp_n_GetCompare() != 0;
    sprintf(message,"n-Comparator       : %1d\n",tmp);
    OAED_USBSendString(message);
    //tmp = CyPins_ReadPin(Comp_Pin_p) != 0;
    tmp = Comp_p_GetCompare() != 0;
    sprintf(message,"p-Comparator       : %1d\n",tmp);
    OAED_USBSendString(message);
    OAED_USBSendString("\n");
}

void OAED_USBSendSystemImage(){
    int16 data = 0;
    
    data += ECG_buffer_full;
    
    data = data << 1;
    data += Z_buffer_full;
    
    data = data << 1;
    data += lead_detected;
    
    data = data << 1;
    data += ECG_data_pending;
    
    data = data << 1;
    data += capacitor_ready;
    
    data = data << 1;
    data += ECG_enabled;
    
    data = data << 1;
    data += Z_enabled;
    
    data = data << 1;
    data += CyPins_ReadPin(Charge_En_0);
    
    data = data << 1;
    data += CyPins_ReadPin(Phase_Pin_Phi1);
    
    data = data << 1;
    data += CyPins_ReadPin(Phase_Pin_Phi2);
    
    data = data << 1;
    data += CyPins_ReadPin(Comp_Pin_p);
    
    data = data << 1;
    data += CyPins_ReadPin(Comp_Pin_n);
    
    OAED_USBSendData16(&data,1);
    
    return;
}

void OAED_USBSendECG(){
    /* OAED_USBSendData need explicit definition of what is sending. */
    extern int16 DataECG[ECG_Data_size];
    OAED_USBSendData(DataECG);
    return;
}

void OAED_USBSendZ(){
    /* OAED_USBSendData need explicit definition of what is sending. */
    extern int16 DataZ[Z_Data_size];
    OAED_USBSendData(DataZ);
    return;
}

void OAED_USBSendBuffer(){
    /* OAED_USBSendData need explicit definition of what is sending. */
    extern int16 BufferECG[ECG_Data_size];
    extern int16 BufferZ[Z_Data_size];
    OAED_USBSendData(BufferECG);
    OAED_USBSendData(BufferZ);
    return;
}

uint16 OAED_USBGetData(uint8 message[], bool echo){
    /* Check USBUART configuration. */
    OAED_USBConfigure();
    
    /* This is a non-blocking function, so long there's no data to get.     */
    if( USBUART_DataIsReady() == 0 ){
        return 0;
    }
    
    uint16 count;
    
    /* Get data. */
    count = USBUART_GetAll(message);
    
    /* Echo the message received. */
    if(echo){
        char command;
        command = message[0];
        OAED_USBSendString("\n");
        OAED_USBSendString(&command);
        OAED_USBSendString("\n");
    }
    return count;
}

bool OAED_USBGetCommand(){
    uint16 count;
    uint8 message[USBFS_BUFFER_SIZE] = {' '};
    
    /* Fetch data. */
    count = OAED_USBGetData(message, OAED_USB_ECHO);
    
    /* This is a non-blocking function, so long there's no data to get.     */
    if(count == 0) return false;
    
    /* The first character is the command. */
    switch(message[0]){
        //case 'I':
          //  /* Toggle Interactive mode. */
            //OAED_USBInteractiveMode();
            //return true;
        case 'S':
            /* Send System State Flags. */
            OAED_USBSendSystemImage();
            return false;
        case 'E':
            /* Send ECG Data array. */
            OAED_USBSendECG();
            return false;
        case 'Z':
            /* Send Z Data array. */
            OAED_USBSendZ();
            return false;
        case 'B':
            /* Send Z and ECG buffer array. */
            OAED_USBSendBuffer();
            return false;
        case 'K':
            OAED_USBPrintSystemImage();
            return false;
        case 'I':
            OAED_USBSendI();
            return false;
        default:
            return false;
    }
    
    return false;
}

void OAED_USBInteractiveMode(){
    /* WARNING: This function is only for debug and instruction purpose.    */
    /* WARNING: this is a blocking function.                                */
    
    static bool Imode = false;
    
    /* If already in Imode return. */
    if(Imode) return;
    
    /* Set Imode On. */
    Imode = true;
    
    /* Repeat until toggle interactive mode off. */
    while(!OAED_USBGetCommand()){}
    
    /* Set Imode Off. */
    Imode = false;
    return;
}


/* [] END OF FILE */
