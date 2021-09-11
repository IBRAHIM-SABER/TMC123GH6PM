/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/********************** SWC    : CAN                **********************/
/********************** Author : Ibrahim Saber      **********************/
/********************** Date   : Aug 30, 2021       **********************/
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/

#ifndef MCAL_CAN_PRIVATE_H_
#define MCAL_CAN_PRIVATE_H_

#define MAX_STANDARD_FRAME_ID       0x7FF
#define MAX_EXTENDED_FRAME_ID       0x1FFFFFFF

#define CAN0_BASE_ADDRESS       0x40040000
#define CAN1_BASE_ADDRESS       0x40041000

typedef struct{

    volatile u32 CTL           ;
    volatile u32 STS           ;
    volatile u32 ERR           ;
    volatile u32 BIT           ;
    volatile u32 INT           ;
    volatile u32 TST           ;
    volatile u32 BRPE          ;
    volatile u32 Reserved1     ;
    volatile u32 IF1CRQ        ;
    volatile u32 IF1CMSK       ;
    volatile u32 IF1MSK1       ;
    volatile u32 IF1MSK2       ;
    volatile u32 IF1ARB1       ;
    volatile u32 IF1ARB2       ;
    volatile u32 IF1MCTL       ;
    volatile u32 IF1DA1        ;
    volatile u32 IF1DA2        ;
    volatile u32 IF1DB1        ;
    volatile u32 IF1DB2        ;
    volatile u32 Reserved2[13] ;
    volatile u32 IF2CRQ        ;
    volatile u32 IF2CMSK       ;
    volatile u32 IF2MSK1       ;
    volatile u32 IF2MSK2       ;
    volatile u32 IF2ARB1       ;
    volatile u32 IF2ARB2       ;
    volatile u32 IF2MCTL       ;
    volatile u32 IF2DA1        ;
    volatile u32 IF2DA2        ;
    volatile u32 IF2DB1        ;
    volatile u32 IF2DB2        ;
    volatile u32 Reserved3[21] ;
    volatile u32 TXRQ1         ;
    volatile u32 TXRQ2         ;
    volatile u32 Reserved4[6]  ;
    volatile u32 NWDA1         ;
    volatile u32 NWDA2         ;
    volatile u32 Reserved5[6]  ;
    volatile u32 MSG1INT       ;
    volatile u32 MSG2INT       ;
    volatile u32 Reserved6[6]  ;
    volatile u32 MSG1VAL       ;
    volatile u32 MSG2VAL       ;
}CAN_Register_t;

#define CAN0        ( (CAN_Register_t * ) CAN0_BASE_ADDRESS )
#define CAN1        ( (CAN_Register_t * ) CAN1_BASE_ADDRESS )

/***************************************************************************/
/**************************** CTL Register Bits ****************************/
#define  CAN_CTL_TEST       (u8)7
#define  CAN_CTL_CCE        (u8)6
#define  CAN_CTL_DAR        (u8)5
#define  CAN_CTL_EIE        (u8)3
#define  CAN_CTL_SIE        (u8)2
#define  CAN_CTL_IE         (u8)1
#define  CAN_CTL_INIT       (u8)0
/***************************************************************************/

/***************************************************************************/
/**************************** CTL Register Bits ****************************/
#define  CAN_CTL_TEST       (u8)7
#define  CAN_CTL_CCE        (u8)6
#define  CAN_CTL_DAR        (u8)5
#define  CAN_CTL_EIE        (u8)3
#define  CAN_CTL_SIE        (u8)2
#define  CAN_CTL_IE         (u8)1
#define  CAN_CTL_INIT       (u8)0
/***************************************************************************/

/***************************************************************************/
/**************************** CMSK Register Bits ****************************/
#define  CAN_CMSK_WRNRD         (u8)7
#define  CAN_CMSK_MASK          (u8)6
#define  CAN_CMSK_ARB           (u8)5
#define  CAN_CMSK_CONTROL       (u8)4
#define  CAN_CMSK_CLRINTPND     (u8)3
#define  CAN_CMSK_NEWDAT        (u8)2
#define  CAN_CMSK_TXRQST        (u8)2
#define  CAN_CMSK_DATAA         (u8)1
#define  CAN_CMSK_DATAB         (u8)0
/***************************************************************************/

/***************************************************************************/
/**************************** IFnARB2 Register Bits ************************/
#define  CAN_ARB2_MSGVAL        (u8)15
#define  CAN_ARB2_XTD           (u8)14
#define  CAN_ARB2_DIR           (u8)13
/***************************************************************************/

/***************************************************************************/
/**************************** IFnMSK2 Register Bits ************************/
#define  CAN_MSK2_MXTD          (u8)15
#define  CAN_MSK2_MDIR          (u8)14
/***************************************************************************/

/***************************************************************************/
/**************************** IF1MCTL Register Bits ************************/
#define  CAN_MCTL_NEWDATA       (u8)15
#define  CAN_MCTL_MSGLST        (u8)14
#define  CAN_MCTL_INTPND        (u8)13
#define  CAN_MCTL_UMASK         (u8)12
#define  CAN_MCTL_TXIE          (u8)11
#define  CAN_MCTL_RXIE          (u8)10
#define  CAN_MCTL_RMTEN         (u8)9
#define  CAN_MCTL_TXRQST        (u8)8
#define  CAN_MCTL_EOB           (u8)7
#define  CAN_MCTL_DLC           (u8)0
/***************************************************************************/

/**************************** IFnCRQ Register Bits ************************/
#define  CAN_CRQ_BUSY          (u8)15
/***************************************************************************/

/**************************** STS Register Bits ************************/
#define  CAN_STS_BOFF           (u8)7
#define  CAN_STS_EWARN          (u8)6
#define  CAN_STS_EPASS          (u8)5
#define  CAN_STS_RXOK           (u8)4
#define  CAN_STS_TXOK           (u8)3





/***************************************************************************/





#endif /* MCAL_CAN_PRIVATE_H_ */
