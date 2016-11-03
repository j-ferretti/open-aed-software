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

/* Variable declarations for DMA_DelSig_B */
uint8 DMA_DelSig_B_Chan;
uint8 DMA_DelSig_B_TD[1];

#if(RAW_MODE)
/* Variable declarations for DMA_DelSig_RAW */
uint8 DMA_DelSig_RAW_Chan;
uint8 DMA_DelSig_RAW_TD[1];
#endif

/* Variable declarations for DMA_FilterECG */
uint8 DMA_FilterECG_Chan;
uint8 DMA_FilterECG_TD[1];

/* Variable declarations for DMA_FilterZ */
uint8 DMA_FilterZ_Chan;
uint8 DMA_FilterZ_TD[4];

void OAED_DMA_Init(){

    /* DMA Configuration for DMA_DelSig */
    DMA_DelSig_Chan = DMA_DelSig_DmaInitialize(
        DMA_DelSig_BYTES_PER_BURST,
        DMA_DelSig_REQUEST_PER_BURST,
        HI16(DMA_DelSig_SRC_BASE),
        HI16(DMA_DelSig_DST_BASE)
    );
    DMA_DelSig_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(
        DMA_DelSig_TD[0],
        DMA_DelSig_BYTES_PER_BURST,
        DMA_DelSig_TD[0],
        0
    );
    CyDmaTdSetAddress(
        DMA_DelSig_TD[0],
        LO16((uint32)ADC_DelSig_DEC_SAMP_PTR),
        LO16((uint32)Filter_STAGEA_PTR)
    );
    CyDmaChSetInitialTd(
        DMA_DelSig_Chan,
        DMA_DelSig_TD[0]
    );

    /* DMA Configuration for DMA_DelSig_B */
    DMA_DelSig_B_Chan = DMA_DelSig_B_DmaInitialize(
        DMA_DelSig_B_BYTES_PER_BURST,
        DMA_DelSig_B_REQUEST_PER_BURST,
        HI16(DMA_DelSig_B_SRC_BASE),
        HI16(DMA_DelSig_B_DST_BASE)
    );
    DMA_DelSig_B_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(
        DMA_DelSig_B_TD[0],
        DMA_DelSig_B_BYTES_PER_BURST,
        DMA_DelSig_B_TD[0],
        0
    );
    CyDmaTdSetAddress(
        DMA_DelSig_B_TD[0],
        LO16((uint32)ADC_DelSig_DEC_SAMP_PTR),
        LO16((uint32)Filter_STAGEB_PTR)
    );
    CyDmaChSetInitialTd(
        DMA_DelSig_B_Chan,
        DMA_DelSig_B_TD[0]
    );

#if(RAW_MODE)
    /* DMA Configuration for DMA_DelSig_RAW */
    DMA_DelSig_RAW_Chan = DMA_DelSig_RAW_DmaInitialize(
        DMA_DelSig_RAW_BYTES_PER_BURST,
        DMA_DelSig_RAW_REQUEST_PER_BURST,
        HI16(DMA_DelSig_RAW_SRC_BASE),
        HI16(DMA_DelSig_RAW_DST_BASE)
    );
    DMA_DelSig_RAW_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(
        DMA_DelSig_RAW_TD[0],
        16,
        DMA_DelSig_RAW_TD[0],
        CY_DMA_TD_INC_DST_ADR
    );
    CyDmaTdSetAddress(
        DMA_DelSig_RAW_TD[0],
        LO16((uint32)ADC_DelSig_DEC_SAMP_PTR),
        LO16((uint32)rawECGBuffer)
    );
    CyDmaChSetInitialTd(
        DMA_DelSig_RAW_Chan,
        DMA_DelSig_RAW_TD[0]
    );
#endif

    /* DMA Configuration for DMA_FilterECG */
    DMA_FilterECG_Chan = DMA_FilterECG_DmaInitialize(
        DMA_FilterECG_BYTES_PER_BURST,
        DMA_FilterECG_REQUEST_PER_BURST,
        HI16(DMA_FilterECG_SRC_BASE),
        HI16(DMA_FilterECG_DST_BASE)
    );
    DMA_FilterECG_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(
        DMA_FilterECG_TD[0],
        16,
        DMA_FilterECG_TD[0],
        DMA_FilterECG__TD_TERMOUT_EN | TD_INC_DST_ADR
    );
    CyDmaTdSetAddress(
        DMA_FilterECG_TD[0],
        LO16((uint32)Filter_HOLDA_PTR),
        LO16((uint32)CacheECG)
    );
    CyDmaChSetInitialTd(
        DMA_FilterECG_Chan,
        DMA_FilterECG_TD[0]
    );

    /* DMA Configuration for DMA_FilterZ */
    DMA_FilterZ_Chan = DMA_FilterZ_DmaInitialize(
        DMA_FilterZ_BYTES_PER_BURST,
        DMA_FilterZ_REQUEST_PER_BURST,
        HI16(DMA_FilterZ_SRC_BASE),
        HI16(DMA_FilterZ_DST_BASE)
    );
    DMA_FilterZ_TD[0] = CyDmaTdAllocate();
    DMA_FilterZ_TD[1] = CyDmaTdAllocate();
    DMA_FilterZ_TD[2] = CyDmaTdAllocate();
    DMA_FilterZ_TD[3] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(
        DMA_FilterZ_TD[0],
        (DMA_FilterZ_BYTES_PER_BURST * Z_DATA_SIZE) / 4,
        DMA_FilterZ_TD[1],
        TD_INC_DST_ADR
    );
    CyDmaTdSetAddress(
        DMA_FilterZ_TD[0],
        LO16((uint32)Filter_HOLDB_PTR),
        LO16((uint32)BufferZ)
    );
    CyDmaTdSetConfiguration(
        DMA_FilterZ_TD[1],
        (DMA_FilterZ_BYTES_PER_BURST * Z_DATA_SIZE) / 4,
        DMA_FilterZ_TD[2],
        TD_INC_DST_ADR
    );
    CyDmaTdSetAddress(
        DMA_FilterZ_TD[1],
        LO16((uint32)Filter_HOLDB_PTR),
        LO16((uint32)(BufferZ + Z_DATA_SIZE / 4))
    );
    CyDmaTdSetConfiguration(
        DMA_FilterZ_TD[2],
        (DMA_FilterZ_BYTES_PER_BURST * Z_DATA_SIZE) / 4,
        DMA_FilterZ_TD[3],
        TD_INC_DST_ADR
    );
    CyDmaTdSetAddress(
        DMA_FilterZ_TD[2],
        LO16((uint32)Filter_HOLDB_PTR),
        LO16((uint32)(BufferZ + Z_DATA_SIZE / 2))
    );
    CyDmaTdSetConfiguration(
        DMA_FilterZ_TD[3],
        (DMA_FilterZ_BYTES_PER_BURST * Z_DATA_SIZE) / 4,
        DMA_FilterZ_TD[0],
        DMA_FilterZ__TD_TERMOUT_EN | TD_INC_DST_ADR
    );
    CyDmaTdSetAddress(
        DMA_FilterZ_TD[3],
        LO16((uint32)Filter_HOLDB_PTR),
        LO16((uint32)(BufferZ + Z_DATA_SIZE * 3 / 4))
    );
    CyDmaChSetInitialTd(
        DMA_FilterZ_Chan,
        DMA_FilterZ_TD[0]
    );

    return;
}

void OAED_DMAADCStart(){
    CyDmaClearPendingDrq(DMA_DelSig_Chan);
    CyDmaClearPendingDrq(DMA_DelSig_B_Chan);
    CyDmaChEnable(DMA_DelSig_Chan, 1);
    CyDmaChEnable(DMA_DelSig_B_Chan, 1);

    #if(RAW_MODE)
    CyDmaChEnable(DMA_DelSig_RAW_Chan, 1);
    #endif
}

void OAED_DMAADCStop(){
    /* Disable ECG DMA */
    CyDmaChDisable(DMA_DelSig_Chan);
    CyDmaChDisable(DMA_DelSig_Chan);

    #if(RAW_MODE)
    CyDmaChDisable(DMA_DelSig_RAW_Chan);
    #endif

}

void OAED_DMAECGStart(){
    /* Clear ECG DMA requests */
    //CyDmaClearPendingDrq(DMA_DelSig_Chan);
    CyDmaClearPendingDrq(DMA_FilterECG_Chan);

    /* Enable ECG DMA */
    //CyDmaChEnable(DMA_DelSig_Chan, 1);
    CyDmaChEnable(DMA_FilterECG_Chan, 1);

    #if(RAW_MODE)
    //CyDmaChEnable(DMA_DelSig_RAW_Chan, 1);
    #endif

    return;

}

void OAED_DMAECGStop(){
    /* Disable ECG DMA */
    //CyDmaChDisable(DMA_DelSig_Chan);
    CyDmaChDisable(DMA_FilterECG_Chan);

    #if(RAW_MODE)
    //CyDmaChDisable(DMA_DelSig_RAW_Chan);
    #endif

    return;
}

void OAED_DMAZStart(){
    /* Enable Z DMA */
    //CyDmaChEnable(DMA_DelSig_B_Chan, 1);
    CyDmaChEnable(DMA_FilterZ_Chan, 1);

    return;
}

void OAED_DMAZStop(){
    /* Disable Z DMA */
    //CyDmaChDisable(DMA_DelSig_B_Chan);
    CyDmaChDisable(DMA_FilterZ_Chan);

    return;
}

/* [] END OF FILE */
