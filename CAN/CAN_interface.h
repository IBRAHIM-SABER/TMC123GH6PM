/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/********************** SWC    : CAN                **********************/
/********************** Author : Ibrahim Saber      **********************/
/********************** Date   : Aug 30, 2021       **********************/
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/

#ifndef MCAL_CAN_INTERFACE_H_
#define MCAL_CAN_INTERFACE_H_

#define CAN_USE_MSG_OBJ_IDMASK      1
#define CAN_NUSE_MSG_OBJ_IDMASK     0

#define CAN_FIFO_ENABLE             1
#define CAN_FIFO_DISABLE            0

typedef enum{
    CAN_ERROR_INTERRUPT  = (1 << 3),
    CAN_STATUS_INTERRUPT = (1 << 2),
    CAN_INTERRUPT_MASTER = (1 << 1)
}CAN_Interrupt_t;

typedef enum{
    CAN_INT_STATUS_CAUSE,
    CAN_INT_STATUS_OBJECT
}CAN_Interrupt_StatusReg_t;

typedef enum{
    LOOPBACK_MODE_DISABLE = ( 0 << 4 ),
    LOOPBACK_MODE_ENABLE  = ( 1 << 4 )
}CAN_Test_Loopback_Mode_t;

typedef enum{
    SILENT_MODE_DISABLE = ( 0 << 3 ),
    SILENT_MODE_ENABLE  = ( 1 << 3 )
}CAN_Test_SilentMode_t;

typedef enum{
    BASIC_MODE_DISABLE = ( 0 << 2 ),
    BASIC_MODE_ENABLE  = ( 1 << 2 )
}CAN_Test_BasicMode_t;

typedef enum{
    CAN_MODULE_CONTROL = ( 0 << 5 ),
    SAMPLE_POINT       = ( 1 << 5 ),
    DRIVEN_LOW         = ( 2 << 5 ),
    DRIVEN_HIGH        = ( 3 << 5 )
}CAN_Test_TransmitCTRL_t;

typedef struct{
    CAN_Test_Loopback_Mode_t    CAN_Test_LoopBackMode       ;
    CAN_Test_SilentMode_t       CAN_Test_SilentMode         ;
    CAN_Test_BasicMode_t        CAN_Test_BasicMode          ;
    CAN_Test_TransmitCTRL_t     CAN_Test_TransmitControl    ;
}CAN_TEST_Functions_t;

typedef struct{
    u8 u8prescaler   ;
    u8 u8SJW            ;
    u8 u8TSEG1          ;
    u8 u8TSEG2            ;
}CAN_BitTimingparams_t;

typedef enum{
    MSG_OBJ1 = 0x01,
    MSG_OBJ2,
    MSG_OBJ3,
    MSG_OBJ4,
    MSG_OBJ5,
    MSG_OBJ6,
    MSG_OBJ7,
    MSG_OBJ8,
    MSG_OBJ9,
    MSG_OBJ10,
    MSG_OBJ11,
    MSG_OBJ12,
    MSG_OBJ13,
    MSG_OBJ14,
    MSG_OBJ15,
    MSG_OBJ16,
    MSG_OBJ17,
    MSG_OBJ18,
    MSG_OBJ19,
    MSG_OBJ20,
    MSG_OBJ21,
    MSG_OBJ22,
    MSG_OBJ23,
    MSG_OBJ24,
    MSG_OBJ25,
    MSG_OBJ26,
    MSG_OBJ27,
    MSG_OBJ28,
    MSG_OBJ29,
    MSG_OBJ30,
    MSG_OBJ31,
    MSG_OBJ32
}CAN_MsgId_t;

typedef enum{
    STANDARD_FRAME,
    EXTENDED_FRAME
}MSG_Frame_t;

typedef enum {
    ZERO_BYTE = 0,
    ONE_BYTE = 1,
    TWO_BYTE = 2,
    THREE_BYTE = 3,
    FOUR_BYTE = 4,
    FIVE_BYTE = 5,
    SIX_BYTE = 6,
    SEVEN_BYTE = 7,
    EIGHT_BYTE = 8,
}CAN_MSG_LENGTH_t;

typedef struct{
    u32             CAN_MessageId       ;
    MSG_Frame_t     CAN_MessageFrameType;
}CAN_TXMsgConfig_t;

typedef struct{
    u8                  MessageData[8] ;
    u8    MessageLength  ;
}CAN_MSG_t;

typedef struct{
    u32             CAN_MessageId       ;
    u32             CAN_ID_MASK         ;
    MSG_Frame_t     CAN_MessageFrameType;
    u8              CAN_MsgIdMask       ;
    u8              CAN_FIFO_State      ;
}CAN_RXMsgConfig_t;



u8 CAN_u8TestModeInit( u8 Copy_u8Channel_ID , const CAN_TEST_Functions_t * Copy_structTsetFunctions );
u8 CAN_u8init( u8 Copy_u8Channel_ID );

u8 CAN_u8SetBitTiming(u8 Copy_u8Channel_ID , const CAN_BitTimingparams_t * Copy_structTimingParameters);

u8 CAN_u8Enable( u8 Copy_u8Channel_ID );
void CAN_voidDisable( u8 Copy_u8Channel_ID );

u8 CAN_u8EnableInterrupt(u8 Copy_u8Channel_ID , CAN_Interrupt_t Copy_enumInerrupt);
u8 CAN_u8DisableInterrupt( u8 Copy_u8Channel_ID , CAN_Interrupt_t Copy_enumInerrupt );
u8 CAN_u8ClearInterrupt(u8 Copy_u8Channel_ID);
u8 CAN_u32InterruptStatus( u8 Copy_u8Channel_ID , CAN_Interrupt_StatusReg_t Copy_enuminterruptStatusoption ,u32 *Copy_ptrInterruptStatus);

u8 CAN_u8TransmitSet(u8  Copy_u8Channel_ID , CAN_MsgId_t  CAN_MessageObicetId ,  const CAN_TXMsgConfig_t * Copy_structTXconfigs );
u8 CAN_u8Write(u8 Copy_u8Channel_ID  , CAN_MsgId_t  CAN_MessageObicetId, const CAN_MSG_t* Copy_structMessageInfo );

u8 CAN_u8RecieveMessageSet(u8 Copy_u8Channel_ID , CAN_MsgId_t  CAN_MessageObicetId, const CAN_RXMsgConfig_t *Copy_structRXMsg );
u8 CAN_u8MessageGet( u8 Copy_u8Channel_ID , CAN_MsgId_t CAN_MessageObicetId , CAN_MSG_t * Copy_structMessage);
u8 CAN_u8TransmitOk(u8 Copy_u8Channel_ID, CAN_MsgId_t CAN_MessageObicetId);
u8 CAN_u8ReceiveOk(u8 Copy_u8Channel_ID, CAN_MsgId_t CAN_MessageObicetId);
#endif /* MCAL_CAN_INTERFACE_H_ */
