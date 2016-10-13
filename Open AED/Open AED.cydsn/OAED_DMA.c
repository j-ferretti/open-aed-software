/* ========================================
 *
 *  OPEN SOURCE AED
 * This file contains all DMA related
 * function and global variable
 * definitions.
 *
 * ========================================
*/

#include "OAED_DMA.h"

/* Variable declarations for DMA_DelSig */
uint8 DMA_DelSig_Chan;
uint8 DMA_DelSig_TD[1];

/* Variable declarations for DMA_Filter_A */
uint8 DMA_Filter_A_Chan;
uint8 DMA_Filter_A_TD[1];

/* Variable declarations for DMA_Filter_B */
uint8 DMA_Filter_B_Chan;
uint8 DMA_Filter_B_TD[1];

/* Variable declarations for DMA_Impedance */
uint8 DMA_Impedance_Chan;
uint8 DMA_Impedance_TD[1];

void OAED_DMA_Init(){

    /* DMA Configuration for DMA_DelSig */
    DMA_DelSig_Chan = DMA_DelSig_DmaInitialize(DMA_DelSig_BYTES_PER_BURST, DMA_DelSig_REQUEST_PER_BURST, 
        HI16(DMA_DelSig_SRC_BASE), HI16(DMA_DelSig_DST_BASE));
    DMA_DelSig_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_DelSig_TD[0], DMA_DelSig_BYTES_PER_BURST, DMA_DelSig_TD[0], 0 );
    CyDmaTdSetAddress(DMA_DelSig_TD[0], LO16((uint32)ADC_DelSig_DEC_SAMP_PTR), LO16((uint32)Filter_STAGEA_PTR));
    CyDmaChSetInitialTd(DMA_DelSig_Chan, DMA_DelSig_TD[0]);
    
    
    /* DMA Configuration for DMA_Filter_A */
    DMA_Filter_A_Chan = DMA_Filter_A_DmaInitialize(DMA_Filter_A_BYTES_PER_BURST, DMA_Filter_A_REQUEST_PER_BURST, 
        HI16(DMA_Filter_A_SRC_BASE), HI16(DMA_Filter_A_DST_BASE));
    DMA_Filter_A_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_Filter_A_TD[0], DMA_Filter_A_BYTES_PER_BURST, DMA_Filter_A_TD[0], DMA_Filter_A__TD_TERMOUT_EN );
    CyDmaTdSetAddress(DMA_Filter_A_TD[0], LO16((uint32)Filter_HOLDA_PTR), LO16((uint32)Filter_STAGEB_PTR));
    CyDmaChSetInitialTd(DMA_Filter_A_Chan, DMA_Filter_A_TD[0]);
    
    /* DMA Configuration for DMA_Filter_B */
    DMA_Filter_B_Chan = DMA_Filter_B_DmaInitialize(DMA_Filter_B_BYTES_PER_BURST, DMA_Filter_B_REQUEST_PER_BURST, 
        HI16(DMA_Filter_B_SRC_BASE), HI16(DMA_Filter_B_DST_BASE));
    DMA_Filter_B_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_Filter_B_TD[0], DMA_Filter_B_BYTES_PER_BURST * ECG_Data_size, DMA_Filter_B_TD[0],
        DMA_Filter_B__TD_TERMOUT_EN | TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_Filter_B_TD[0], LO16((uint32)Filter_HOLDB_PTR), LO16((uint32)BufferECG));
    CyDmaChSetInitialTd(DMA_Filter_B_Chan, DMA_Filter_B_TD[0]);
    

    /* DMA Configuration for DMA_Impedance */
    DMA_Impedance_Chan = DMA_Impedance_DmaInitialize(DMA_Impedance_BYTES_PER_BURST, DMA_Impedance_REQUEST_PER_BURST, 
        HI16(DMA_Impedance_SRC_BASE), HI16(DMA_Impedance_DST_BASE));
    DMA_Impedance_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_Impedance_TD[0], DMA_Impedance_BYTES_PER_BURST * Z_Data_size, DMA_Impedance_TD[0],
        DMA_Impedance__TD_TERMOUT_EN | TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_Impedance_TD[0], LO16((uint32)Filter_HOLDA_PTR), LO16((uint32)BufferZ));
    CyDmaChSetInitialTd(DMA_Impedance_Chan, DMA_Impedance_TD[0]);
        
    return;
}

void OAED_DMAECGStart(){
    /* Clear ECG DMA requests */
    CyDmaClearPendingDrq(DMA_DelSig_Chan);
    CyDmaClearPendingDrq(DMA_Filter_A_Chan);
    CyDmaClearPendingDrq(DMA_Filter_B_Chan);
    
    /* Enable ECG DMA */
    CyDmaChEnable(DMA_DelSig_Chan, 1);
    CyDmaChEnable(DMA_Filter_A_Chan, 1);
    CyDmaChEnable(DMA_Filter_B_Chan, 1);
    
    return;
    
}

void OAED_DMAECGStop(){
    /* Disable ECG DMA */
    CyDmaChDisable(DMA_DelSig_Chan);
    CyDmaChDisable(DMA_Filter_A_Chan);
    CyDmaChDisable(DMA_Filter_B_Chan);
    
    return;
}

void OAED_DMAZStart(){
    /* Enable Z DMA */
    CyDmaChEnable(DMA_Impedance_Chan, 1);
    
    return;
}

void OAED_DMAZStop(){
    /* Disable Z DMA */
    CyDmaChDisable(DMA_Impedance_Chan);
    
    return;
}

/* [] END OF FILE */
