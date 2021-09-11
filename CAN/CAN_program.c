/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/********************** SWC    : CAN                **********************/
/********************** Author : Ibrahim Saber      **********************/
/********************** Date   : Aug 30, 2021       **********************/
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/

#include "../LIB/STD_TYPES/STD_TYPES.h"
#include "../LIB/BIT_MATH/BIT_MATH.h"

#include "CAN_interface.h"
#include "CAN_config.h"
#include "CAN_private.h"

/* Array of pointers to structures  */
static CAN_Register_t * CANx[2] = {CAN0 , CAN1}; // static to prevents extern it "only valid at this file"



u8 CAN_u8TestModeInit( u8 Copy_u8Channel_ID , const CAN_TEST_Functions_t * Copy_structTsetFunctions )
{

    u8 Local_u8ErrorState = STD_OK; // Local Variable holds the return of the Function

    /****************************************************************************************
     *  It is write-enabled by setting the TEST bit in the CANCTL register.                 *
     * Different test functions may be combined,                                            *
     *  however, CAN transfers are affected if the TX bits in this register are not zero.   *
     ****************************************************************************************/
    if( Copy_u8Channel_ID > 1 ) // only channel Numbers available are 0 and 1
    {
        if( Copy_structTsetFunctions != NULL ) // Check that the pointers does not have a null value
        {
            SET_BIT( CANx[Copy_u8Channel_ID]->CTL , CAN_CTL_INIT ); // Enable the access to the registers by Enable initialization bit
            /* Enable the Test Mode  */
            SET_BIT( CANx[Copy_u8Channel_ID]->CTL , CAN_CTL_TEST );
            /* Select the Test Functions to be Enabled  */

            CANx[Copy_u8Channel_ID]->TST &= ~( 0b111111 << 2); /* Clear the Bits that control the test Function */

            CANx[Copy_u8Channel_ID]->TST |= Copy_structTsetFunctions->CAN_Test_BasicMode | Copy_structTsetFunctions->CAN_Test_LoopBackMode | Copy_structTsetFunctions->CAN_Test_SilentMode | Copy_structTsetFunctions->CAN_Test_TransmitControl;
        }
        else
        {
            Local_u8ErrorState = STD_NOK; // Error the pointer have Null Value
        }
    }
    else
    {
        Local_u8ErrorState = STD_NOK; // Channel Number is not Supported
    }
    return (Local_u8ErrorState);
}

u8 CAN_u8init( u8 Copy_u8Channel_ID )
{
    u8 Local_u8ErrorState = STD_OK; // Local Variable holds the return of the Function
    if( Copy_u8Channel_ID > 1 ) // only channel Numbers available are 0 and 1
    {
        SET_BIT( CANx[Copy_u8Channel_ID]->CTL , CAN_CTL_INIT ); // Enable the access to the registers by Enable initialization bit
        CLR_BIT( CANx[Copy_u8Channel_ID]->CTL , CAN_CTL_TEST ); // Disable test Mode

    }
    else
    {
        Local_u8ErrorState = STD_NOK; // Channel Number is not Supported
    }
    return (Local_u8ErrorState);
}
u8 CAN_u8SetBitTiming(u8 Copy_u8Channel_ID , const CAN_BitTimingparams_t * Copy_structTimingParameters)
{
    u8 Local_u8ErrorState = STD_OK; // Local Variable holds the return of the Function

    if( Copy_u8Channel_ID > 1 ) // only channel Numbers available are 0 and 1
    {

        if( Copy_structTimingParameters != NULL ) // Check that the pointers does not have a null value
        {

            SET_BIT( CANx[Copy_u8Channel_ID]->CTL , CAN_CTL_CCE ); // the Configuration Change Enable
            CANx[Copy_u8Channel_ID]->BIT &= ~(0xFFFF); // Clear the 2-bytes of the Register " Because it is not atomic access"

            /****************************************************************************************************************
             *  In the CANBIT register, the four components TSEG2, TSEG1, SJW, and BRP have to be programmed to a           *
             * numerical value that is one less than its functional value; so instead of values in the range of [1..n],     *
             * values in the range of [0..n-1] are programmed.                                                              *
             ****************************************************************************************************************/
            CANx[Copy_u8Channel_ID]->BIT |= ( Copy_structTimingParameters->u8prescaler - 1 ); // BRP
            CANx[Copy_u8Channel_ID]->BIT |= ((( Copy_structTimingParameters->u8SJW - 1 ) & 0x3 )  << 6 ); // SJW
            CANx[Copy_u8Channel_ID]->BIT |= ((( Copy_structTimingParameters->u8TSEG1 - 1) & 0xF ) << 8 ); // TSEG1
            CANx[Copy_u8Channel_ID]->BIT |= ((( Copy_structTimingParameters->u8TSEG2 -1 ) & 0x7 ) << 12); // TSEG2
            CLR_BIT( CANx[Copy_u8Channel_ID]->CTL , CAN_CTL_CCE ); //Configuration Change is Disable
        }
        else
        {
            Local_u8ErrorState = STD_NOK; // Channel Number is not Supported
        }
    }
    else
    {
        Local_u8ErrorState = STD_NOK; // Channel Number is not Supported
    }
    return (Local_u8ErrorState);
}


u8 CAN_u8Enable( u8 Copy_u8Channel_ID )
{
    u8 Local_u8ErrorState = STD_OK;
    if( Copy_u8Channel_ID > 1 )
    {
        //Out From Initialization Mode To Start Transmit :
        CLR_BIT( CANx[Copy_u8Channel_ID]->CTL , CAN_CTL_INIT );
    }
    else
    {
        Local_u8ErrorState = STD_NOK; // Channel Number is not Supported
    }
    return (Local_u8ErrorState);
}
void CAN_voidDisable( u8 Copy_u8Channel_ID )
{
    /* Enter Initialization Mode ("Not recommended") as it still consumes power */
    //    u8 Local_u8ErrorState = STD_OK;
    //    if( Copy_u8Channel_ID > 1 )
    //    {
    //        //Out From Initialization Mode To Start Transmit :
    //        CLR_BIT( CANx[Copy_u8Channel_ID]->CTL , CAN_CTL_INIT );
    //    }
    //    else
    //    {
    //        Local_u8ErrorState = STD_NOK;
    //    }
    //    return (Local_u8ErrorState);
    /* Disable Clock From RCC peripheral ("Recommended") */
}

/*******************************************************************************************************/
/* Copy_enumInterrupt --> CAN_INTERRUPT_MASTER | CAN_ERROR_INTERRUPT | CAN_STATUS_INTERRUPT  */
u8 CAN_u8EnableInterrupt(u8 Copy_u8Channel_ID , CAN_Interrupt_t Copy_enumInerrupt)
{
    u8 Local_u8ErrorState = STD_OK; // Local Variable holds the return of the Function
    if( Copy_u8Channel_ID > 1 )  // Only Channel 0 and 1 are available in TIM4C123GH6Pm
    {
        CANx[Copy_u8Channel_ID]->CTL &= ~(0b111 << 1); // Clear the Interrupts and select the new Interrupt Source
        CANx[Copy_u8Channel_ID]->CTL |= Copy_enumInerrupt;
    }
    else
    {
        Local_u8ErrorState = STD_NOK; // Channel Number is not Supported
    }
    return (Local_u8ErrorState);
}
u8 CAN_u8DisableInterrupt( u8 Copy_u8Channel_ID , CAN_Interrupt_t Copy_enumInerrupt )
{
    u8 Local_u8ErrorState = STD_OK; // Local Variable holds the return of the Function
    if( Copy_u8Channel_ID > 1 )  // Only Channel 0 and 1 are available in TIM4C123GH6Pm
    {
        CANx[Copy_u8Channel_ID]->CTL &= ~(Copy_enumInerrupt);

    }
    else
    {
        Local_u8ErrorState = STD_NOK; // Channel Number is not Supported
    }
    return (Local_u8ErrorState); // Return the Error State
}
u8 CAN_u8ClearInterrupt(u8 Copy_u8Channel_ID)
{
    u8 Local_u8ErrorState = STD_OK; // Local Variable holds the return of the Function
    if( Copy_u8Channel_ID > 1 )  // Only Channel 0 and 1 are available in TIM4C123GH6Pm
    {
        u32 Local_u32DummyVariable = 0;
        /*  The interrupt line remains active until the INTID field is cleared by reading the CANSTS register,
         *  or until the IE bit in the CANCTL register is cleared. */
        Local_u32DummyVariable = CANx[Copy_u8Channel_ID]->INT;  // Reading the CANSTS register Clears the CANINT register
    }
    else
    {
        Local_u8ErrorState = STD_NOK; // Channel Number is not Supported
    }
    return (Local_u8ErrorState); // Return the Error State
}

u8 CAN_u32InterruptStatus( u8 Copy_u8Channel_ID , CAN_Interrupt_StatusReg_t Copy_enuminterruptStatusoption ,u32 *Copy_ptrInterruptStatus)
{

    u8 Local_u8ErrorState = STD_OK;
    if( Copy_u8Channel_ID > 1 )
    {
        if( Copy_enuminterruptStatusoption == CAN_INT_STATUS_CAUSE )
        {
            /************************************************************************************************************
             *  If several interrupts are pending, the CAN Interrupt (CANINT) register points to the pending interrupt  *
             * with the highest priority,disregarding the order in which the interrupts occurred                        *
             ************************************************************************************************************/

            /********************************************************************************************
             * The number in this field indicates the source of the interrupt.                          *
             * 0x0000        ---> No interrupt pending                                                  *
             * 0x0001-0x0020 ---> Number of the message object that caused the interrupt                *
             * 0x8000        ---> Status Interrupt                                                      *
             ********************************************************************************************/
            *Copy_ptrInterruptStatus = CANx[Copy_u8Channel_ID]->INT;
        }
        else
        {
            /************************************************************************************************************
             * The CANMSG1INT register contains the INTPND bits of the first 16 message objects in the messageRAM;      *
             * the CANMSG2INT register contains the INTPND bits of the second 16 message objects.                       *
             * The corresponding message object is not the source of an interrupt --> 0                                 *
             * The corresponding message object is the source of an interrupt -- > 1                                    *
             ************************************************************************************************************/
            *Copy_ptrInterruptStatus = ( CANx[Copy_u8Channel_ID]->MSG1INT & 0x00FF );
            *Copy_ptrInterruptStatus |= ( ( CANx[Copy_u8Channel_ID]->MSG2INT ) << 16 );
        }
    }
    else
    {
        Local_u8ErrorState = STD_NOK;
    }
    return (Local_u8ErrorState);
}

u8 CAN_u8TransmitSet(u8  Copy_u8Channel_ID , CAN_MsgId_t  CAN_MessageObicetId ,  const CAN_TXMsgConfig_t * Copy_structTXconfigs )
{
    u8 Local_u8ErrorState = STD_OK;
    if( (Copy_u8Channel_ID > 1) && (CAN_MessageObicetId <= MSG_OBJ32) && (Copy_structTXconfigs != NULL))
    {
        while( GET_BIT( CANx[Copy_u8Channel_ID]->IF1CRQ , CAN_CRQ_BUSY) ) // Wait the CAN controller is not Busy
        {}

        CLR_BIT( CANx[Copy_u8Channel_ID]->IF1ARB2 , CAN_ARB2_MSGVAL );

        MSG_Frame_t local_structFrameType = STANDARD_FRAME; // Local variable to have the Frame Type

        if( ( Copy_structTXconfigs->CAN_MessageId > MAX_STANDARD_FRAME_ID ) && ( Copy_structTXconfigs->CAN_MessageId < MAX_EXTENDED_FRAME_ID ) && ( Copy_structTXconfigs->CAN_MessageFrameType == EXTENDED_FRAME ) )
        {
            local_structFrameType = EXTENDED_FRAME; //the user Entered Message Id of Extended Frame
        }
        else if( Copy_structTXconfigs-> CAN_MessageId > MAX_EXTENDED_FRAME_ID )
        {
            Local_u8ErrorState = STD_NOK; // Wrong Frame Type
        }

        /*IF1xxxx : Registers are used While Transmitting */

        /*
         * Set the WRNRD bit to specify a write to the CANIFnCMASK register;
         * specify whether to transfer the IDMASK, DIR, and MXTD of the message object into the CAN IFn registers using the MASK bit
         * */
        SET_BIT( CANx[Copy_u8Channel_ID]->IF1CMSK , CAN_CMSK_WRNRD  );
        /*
         * Specify whether to transfer the ID, DIR, XTD, and MSGVAL of the message object into the
           interface registers using the ARB bit
         */
        SET_BIT( CANx[Copy_u8Channel_ID]->IF1CMSK , CAN_CMSK_ARB    );
        /*
         * Specify whether to transfer the control bits into the interface registers using the CONTROL bit
         * */
        SET_BIT( CANx[Copy_u8Channel_ID]->IF1CMSK , CAN_CMSK_CONTROL);


        /***********************************************************************************************************
         * For a 29-bit identifier, configure ID[15:0] in the CANIFnARB1 register for bits [15:0] of the
         * message identifier and ID[12:0] in the CANIFnARB2 register for bits [28:16] of the message
         * identifier. Set the XTD bit to indicate an extended identifier; set the DIR bit to indicate transmit;
         * and set the MSGVAL bit to indicate that the message object is valid.
         ***********************************************************************************************************/
        /***********************************************************************************************************
         * For an 11-bit identifier, disregard the CANIFnARB1 register and configure ID[12:2] in the
         * CANIFnARB2 register for bits [10:0] of the message identifier. Clear the XTD bit to indicate a
         * standard identifier; set the DIR bit to indicate transmit; and set the MSGVAL bit to indicate that
         * the message object is valid.
         *
         ***********************************************************************************************************/
        if( local_structFrameType == STANDARD_FRAME )
        {
            CLR_BIT( CANx[Copy_u8Channel_ID]->IF1ARB2 , CAN_ARB2_XTD    );
            CLR_BIT( CANx[Copy_u8Channel_ID]->IF1MSK2 , CAN_MSK2_MXTD   );
            CANx[Copy_u8Channel_ID]->IF1ARB2 &= ~(0x1FFF);
            CANx[Copy_u8Channel_ID]->IF1ARB2 |= (Copy_structTXconfigs->CAN_MessageId << 2);
        }
        else if( local_structFrameType == EXTENDED_FRAME )
        {
            SET_BIT( CANx[Copy_u8Channel_ID]->IF1ARB2 , CAN_ARB2_XTD    );
            CANx[Copy_u8Channel_ID]->IF1ARB1 &= ~(0x0000FFFF);
            CANx[Copy_u8Channel_ID]->IF1ARB1 |= ( Copy_structTXconfigs->CAN_MessageId & 0x0000FFFF);
            CANx[Copy_u8Channel_ID]->IF1ARB2 &= ~(0x1FFF);
            CANx[Copy_u8Channel_ID]->IF1ARB2 |= ( ( Copy_structTXconfigs->CAN_MessageId & 0x1FFF0000) >> 16 );
        }
        else
        {

        }
        SET_BIT( CANx[Copy_u8Channel_ID]->IF1ARB2 , CAN_ARB2_DIR    );  // Set the Direction as Transmit
        SET_BIT( CANx[Copy_u8Channel_ID]->IF1ARB2 , CAN_ARB2_MSGVAL );
        SET_BIT( CANx[Copy_u8Channel_ID]->IF1MCTL , CAN_MCTL_EOB    );  // Set the EOB bit for a single message object
        CANx[Copy_u8Channel_ID]->IF1CRQ &= ~(0xFF);
        /* Program the number of the message object to be transmitted in the MNUM field in the CAN IFn
        Command Request (CANIFnCRQ) register. */
        CANx[Copy_u8Channel_ID]->IF1CRQ |= CAN_MessageObicetId ;
    }
    else
    {
        Local_u8ErrorState = STD_NOK;
    }
    return (Local_u8ErrorState);
}

u8 CAN_u8Write(u8 Copy_u8Channel_ID  , CAN_MsgId_t  CAN_MessageObicetId, const CAN_MSG_t* Copy_structMessageInfo )
{
    u8 Local_u8ErrprState = STD_OK; // Local variable to return the Function State
    if ( (Copy_u8Channel_ID > 1) && (CAN_MessageObicetId <= MSG_OBJ32) && ( Copy_structMessageInfo != NULL) )
    {
        while( GET_BIT( CANx[Copy_u8Channel_ID]->IF1CRQ , CAN_CRQ_BUSY) ) // Wait the CAN controller is not Busy
        {}
        CLR_BIT( CANx[Copy_u8Channel_ID]->IF1CMSK , CAN_CMSK_WRNRD );
        CANx[Copy_u8Channel_ID]->IF1CRQ = CAN_MessageObicetId; // Write the requested Message object

        while( GET_BIT( CANx[Copy_u8Channel_ID]->IF1CRQ , CAN_CRQ_BUSY) ) // Wait the CAN controller is not Busy
        {}

        /*
         * Set the WRNRD bit to specify a write to the CANIFnCMASK register;
         * specify whether to transfer the IDMASK, DIR, and MXTD of the message object into the CAN IFn registers using the MASK bit
         * */
        SET_BIT( CANx[Copy_u8Channel_ID]->IF1CMSK , CAN_CMSK_WRNRD );
        /*
         * Specify whether to transfer the control bits into the interface registers using the CONTROL bit
         * */
        SET_BIT( CANx[Copy_u8Channel_ID]->IF1CMSK , CAN_CMSK_CONTROL );
        SET_BIT( CANx[Copy_u8Channel_ID]->IF1CMSK , CAN_CMSK_DATAA );  // Enable the Access to the Data Register :DATAA
        SET_BIT( CANx[Copy_u8Channel_ID]->IF1CMSK , CAN_CMSK_DATAB );  // Enable the Access to the Data Register :DATAB

        CANx[Copy_u8Channel_ID]->IF1MCTL &= ~(0xF); // Clear the Bits Selecting the Message length
        CANx[Copy_u8Channel_ID]->IF1MCTL |= Copy_structMessageInfo->MessageLength;  // Write the message Length

        CANx[Copy_u8Channel_ID]->IF1DA1 &= ~(0xFFFF); // Clear the Data A1 register
        CANx[Copy_u8Channel_ID]->IF1DA1 |= Copy_structMessageInfo->MessageData[0];// Copy the First Byte To be Transmitted into DATA A register
        CANx[Copy_u8Channel_ID]->IF1DA1 |= ( Copy_structMessageInfo->MessageData[1] << 8 ); // Write the Second Byte

        CANx[Copy_u8Channel_ID]->IF1DA2 &= ~(0xFFFF); // Clear the Data A2 register
        CANx[Copy_u8Channel_ID]->IF1DA2 |= Copy_structMessageInfo->MessageData[2]; // Write the third Byte
        CANx[Copy_u8Channel_ID]->IF1DA2 |= ( Copy_structMessageInfo->MessageData[3] << 8 ); // Write the 4th Byte

        CANx[Copy_u8Channel_ID]->IF1DB1 &= ~(0xFFFF);  // Clear the Data A2 register
        CANx[Copy_u8Channel_ID]->IF1DB1 |= Copy_structMessageInfo->MessageData[4]; // Write the 5th Byte
        CANx[Copy_u8Channel_ID]->IF1DB1 |= ( Copy_structMessageInfo->MessageData[5] << 8 );// Write the 6th Byte

        CANx[Copy_u8Channel_ID]->IF1DB2 &= ~(0xFFFF);  // Clear the Data B2 register
        CANx[Copy_u8Channel_ID]->IF1DB2 |= Copy_structMessageInfo->MessageData[6]; // Write the 7th Byte
        CANx[Copy_u8Channel_ID]->IF1DB2 |= ( Copy_structMessageInfo->MessageData[7] << 8 );// Write the 8th Byte

        CANx[Copy_u8Channel_ID]->IF1MCTL |= ( 1 << CAN_MCTL_TXRQST ) | ( 1 << CAN_MCTL_NEWDATA);
        CANx[Copy_u8Channel_ID]->IF1CRQ = CAN_MessageObicetId;  // Write the Message Object Number
    }
    else
    {
        Local_u8ErrprState = STD_NOK;
    }
    return (Local_u8ErrprState);
}

u8 CAN_u8RecieveMessageSet(u8 Copy_u8Channel_ID , CAN_MsgId_t  CAN_MessageObicetId, const CAN_RXMsgConfig_t *Copy_structRXMsg )
{
    /* IF2xxx : Is used for receiving  */
    u8 Local_u8ErrorState = STD_OK;
    if( ( Copy_u8Channel_ID > 1 ) && (CAN_MessageObicetId <= MSG_OBJ32) && (Copy_structRXMsg!= NULL) )
    {
        while( GET_BIT( CANx[Copy_u8Channel_ID]->IF1CRQ , CAN_CRQ_BUSY) ) // Wait the CAN is not busy
        {}

        CANx[Copy_u8Channel_ID]->IF2ARB2 &= ~( 1 << CAN_ARB2_MSGVAL ); // Clear Message Valid bit

        MSG_Frame_t Local_enumFrame = STANDARD_FRAME;
        if( (Copy_structRXMsg->CAN_MessageId > MAX_STANDARD_FRAME_ID ) && (Copy_structRXMsg->CAN_MessageId <= MAX_EXTENDED_FRAME_ID ) && (Copy_structRXMsg->CAN_MessageId==EXTENDED_FRAME))
        {
            Local_enumFrame = EXTENDED_FRAME;
        }
        else
        {
            Local_u8ErrorState = STD_NOK; // Wrong Frame Type
        }
        SET_BIT( CANx[Copy_u8Channel_ID]->IF2CMSK , CAN_CMSK_WRNRD   );
        /*
         * Specify whether to transfer the ID, DIR, XTD, and MSGVAL of the message object into the
                  interface registers using the ARB bit
         */
        SET_BIT( CANx[Copy_u8Channel_ID]->IF2CMSK , CAN_CMSK_ARB     );
        /*
         * Specify whether to transfer the control bits into the interface registers using the CONTROL bit
         * */
        SET_BIT( CANx[Copy_u8Channel_ID]->IF2CMSK , CAN_CMSK_CONTROL );
        /* Specify which bits to transfer using the DATAA and DATAB bits
         * Enable the Access to the Data A And Data B Registers and transmit all bits */
        SET_BIT( CANx[Copy_u8Channel_ID]->IF2CMSK , CAN_CMSK_DATAA );
        SET_BIT( CANx[Copy_u8Channel_ID]->IF2CMSK , CAN_CMSK_DATAB );


        /****************************************************************************************************
         * In the CANIFnMSK1 register, use the MSK[15:0] bits to specify which of the bits in the 29-bit
         * or 11-bit message identifier are used for acceptance filtering. Note that MSK[15:0] in this
         * register are used for bits [15:0] of the 29-bit message identifier and are not used for an 11-bit
         * identifier. A value of 0x00 enables all messages to pass through the acceptance filtering. Also
         * note that in order for these bits to be used for acceptance filtering, they must be enabled by
         * setting the UMASK bit in the CANIFnMCTL register.
         *
         *****************************************************************************************************/
        /******************************************************************************************************
         * In the CANIFnMSK2 register, use the MSK[12:0] bits to specify which of the bits in the 29-bit
         * or 11-bit message identifier are used for acceptance filtering. Note that MSK[12:0] are used
         * for bits [28:16] of the 29-bit message identifier; whereas MSK[12:2] are used for bits [10:0] of
         * the 11-bit message identifier. Use the MXTD and MDIR bits to specify whether to use XTD and
         * DIR for acceptance filtering. A value of 0x00 enables all messages to pass through the
         * acceptance filtering. Also note that in order for these bits to be used for acceptance filtering,
         * they must be enabled by setting the UMASK bit in the CANIFnMCTL register.
         *
         *******************************************************************************************************/
        if( Local_enumFrame == STANDARD_FRAME )
        {
            CLR_BIT( CANx[Copy_u8Channel_ID]->IF2ARB2 , CAN_ARB2_XTD); // Clear the Extended Frame Bit

            if( Copy_structRXMsg->CAN_MsgIdMask == CAN_USE_MSG_OBJ_IDMASK ) // if the MAsking is Enable to Select which messages to be received
            {
                SET_BIT( CANx[Copy_u8Channel_ID]->IF2CMSK , CAN_CMSK_MASK ); // Enable the Masking for the Accepting filter
                SET_BIT( CANx[Copy_u8Channel_ID]->IF2MCTL , CAN_MCTL_UMASK); // Enable the Masking for the Accepting filter

                CANx[Copy_u8Channel_ID]->IF2MSK2 &= ~(0x1FFF); // Clear Bits of the Mask Register
                CANx[Copy_u8Channel_ID]->IF2MSK2 |= ( (Copy_structRXMsg->CAN_ID_MASK) << 2); // Write the mask of the message id as Standard frame

                SET_BIT( CANx[Copy_u8Channel_ID]->IF2MSK2 , CAN_MSK2_MDIR); //Use the MXTD and MDIR bits to specify whether to use XTD and DIR for acceptance filtering.
                CLR_BIT( CANx[Copy_u8Channel_ID]->IF2MSK2 , CAN_MSK2_MXTD );
            }
            CANx[Copy_u8Channel_ID]->IF2ARB2 &= ~(0x1FFF);
            CANx[Copy_u8Channel_ID]->IF2ARB2 |= ( (Copy_structRXMsg->CAN_MessageId) << 2 ); //  the message identifier to be received;

        }
        else if ( Local_enumFrame == EXTENDED_FRAME )
        {

            if( Copy_structRXMsg->CAN_MsgIdMask == CAN_USE_MSG_OBJ_IDMASK )
            {
                SET_BIT( CANx[Copy_u8Channel_ID]->IF2CMSK , CAN_CMSK_MASK );  // Enable the Masking for the Accepting filter
                SET_BIT( CANx[Copy_u8Channel_ID]->IF2MCTL , CAN_MCTL_UMASK);  // Enable the Masking for the Accepting filter
                // Write the mask of the message id as Standard frame
                CANx[Copy_u8Channel_ID]->IF2MSK1 &= ~(0xFFFF);
                CANx[Copy_u8Channel_ID]->IF2MSK1 |= (Copy_structRXMsg->CAN_ID_MASK & 0xFFFF);
                CANx[Copy_u8Channel_ID]->IF2MSK2 &= ~(0x1FFF);
                CANx[Copy_u8Channel_ID]->IF2MSK2 |= ( Copy_structRXMsg->CAN_ID_MASK >> 16);

                CANx[Copy_u8Channel_ID]->IF2MSK2 |= ( 1 << CAN_MSK2_MXTD );    //Use the MXTD and MDIR bits to specify whether to use XTD and DIR for acceptance filtering.
                SET_BIT( CANx[Copy_u8Channel_ID]->IF2MSK2 , CAN_MSK2_MDIR);
            }
            CANx[Copy_u8Channel_ID]->IF2ARB2 |= ( 1 << CAN_ARB2_XTD);  // Select Extended Frame
            CANx[Copy_u8Channel_ID]->IF2ARB1 &= ~(0xFFFF); // Clear the Register
            CANx[Copy_u8Channel_ID]->IF2ARB1 |= (Copy_structRXMsg->CAN_MessageId & 0x0000FFFF ); // Write the Message id to be Received as Extended Frame
            CANx[Copy_u8Channel_ID]->IF2ARB2 &= ~(0x1FFF);
            CANx[Copy_u8Channel_ID]->IF2ARB2 |= ( (Copy_structRXMsg->CAN_MessageId & 0x1FFF0000 ) >> 16 );
        }
        else
        {

        }
        CANx[Copy_u8Channel_ID]->IF2ARB2 &= ~( 1 << CAN_ARB2_DIR    );   // Setting the Direction to be receive
        CANx[Copy_u8Channel_ID]->IF2ARB2 |=  ( 1 << CAN_ARB2_MSGVAL ); // set the message is valid
        if( Copy_structRXMsg->CAN_FIFO_State == CAN_FIFO_ENABLE )
        {
            CLR_BIT ( CANx[Copy_u8Channel_ID]->IF2MCTL , CAN_MCTL_EOB ); // Enable the FIFO Buffer
        }
        else if( Copy_structRXMsg->CAN_FIFO_State == CAN_FIFO_DISABLE)
        {
            SET_BIT ( CANx[Copy_u8Channel_ID]->IF2MCTL , CAN_MCTL_EOB ); // Disable the FIFO Buffer and this only one message
        }
        else
        {
            Local_u8ErrorState = STD_NOK;
        }

        CANx[Copy_u8Channel_ID]->IF2CRQ = CAN_MessageObicetId; // Write the message object
    }
    else
    {
        Local_u8ErrorState = STD_NOK;
    }
    return (Local_u8ErrorState); // Return Error State
}

u8 CAN_u8MessageGet( u8 Copy_u8Channel_ID , CAN_MsgId_t CAN_MessageObicetId , CAN_MSG_t * Copy_structMessage)
{
    u8 Local_u8ErrorState = STD_OK;
    if( (Copy_u8Channel_ID > 1) && ( CAN_MessageObicetId <= MSG_OBJ32 ) && (Copy_structMessage != NULL) )
    {
        while( GET_BIT( CANx[Copy_u8Channel_ID]->IF1CRQ , CAN_CRQ_BUSY) ); // Wait the Can Controller is not busy

        /*
         * Transfer the data in the CAN message object specified by the MNUM field in the CANIFnCRQ register into the CANIFn registers
         * */
        CLR_BIT( CANx[Copy_u8Channel_ID]->IF2CMSK , CAN_CMSK_WRNRD );
        /*
         * Program the number of the message object to be received in the MNUM field in the CAN IFn
         * Command Request (CANIFnCRQ) register. Reception of the message object begins as soon
         * as a matching frame is available on the CAN bus
         * */
        CANx[Copy_u8Channel_ID]->IF2CRQ = CAN_MessageObicetId; // Write the message object id to read data from

        while( GET_BIT( CANx[Copy_u8Channel_ID]->IF1CRQ , CAN_CRQ_BUSY) );   // Wait the Can Controller is not busy
        /*
         * When the message handler stores a data frame in the message object, it stores the received Data
         * Length Code and eight data bytes in the CANIFnDA1, CANIFnDA2, CANIFnDB1, and CANIFnDB2 register.
         * Byte 0 of the CAN data frame is stored in DATA[7:0] in the CANIFnDA1 register. If the
         * Data Length Code is less than 8, the remaining bytes of the message object are overwritten by unspecified values.
         */
        Copy_structMessage->MessageData[0] = ( CANx[Copy_u8Channel_ID]->IF2DA1 & 0x00FF); // read 1st byte and store it
        Copy_structMessage->MessageData[1] = ( ( CANx[Copy_u8Channel_ID]->IF2DA1 & 0xFF00 ) >> 8 ); // read 2nd byte and store it

        Copy_structMessage->MessageData[2] = ( CANx[Copy_u8Channel_ID]->IF2DA2 & 0x00FF); // read 3trd byte and store it
        Copy_structMessage->MessageData[3] = ( ( CANx[Copy_u8Channel_ID]->IF2DA2 & 0xFF00 ) >> 8 ); // read 4th byte and store it

        Copy_structMessage->MessageData[4] = ( CANx[Copy_u8Channel_ID]->IF2DB1 & 0x00FF); // read 5th byte and store it
        Copy_structMessage->MessageData[5] = ( ( CANx[Copy_u8Channel_ID]->IF2DB1 & 0xFF00 ) >> 8 ); // read 6th byte and store it

        Copy_structMessage->MessageData[6] = ( CANx[Copy_u8Channel_ID]->IF2DB2 & 0x00FF); // read 7th byte and store it
        Copy_structMessage->MessageData[7] = ( ( CANx[Copy_u8Channel_ID]->IF2DB2 & 0xFF00 ) >> 8 ); // read 8th byte and store it

        Copy_structMessage->MessageLength = ( CANx[Copy_u8Channel_ID]->IF2MCTL & 0x0F ); // Write the message length
        SET_BIT( CANx[Copy_u8Channel_ID]->IF2CMSK , CAN_CMSK_WRNRD ); // Set the Write not read bit
        CANx[Copy_u8Channel_ID]->IF2CRQ = CAN_MessageObicetId; // Write the Message object id
    }
    else
    {
        Local_u8ErrorState = STD_NOK;
    }
    return (Local_u8ErrorState);
}

u8 CAN_u8TransmitOk(u8 Copy_u8Channel_ID, CAN_MsgId_t CAN_MessageObicetId)
{
    u32 Read_TXRQ_register = 0 ;
    u8 Read_STS_register  = 0 ;

    /*Read Status register to check TXOK transmitted message successfully*/
     Read_STS_register = CANx[Copy_u8Channel_ID]->STS & ( 1 << CAN_STS_TXOK );
    /*The CANTXRQ1 holds the first 16 messages*/
    if(CAN_MessageObicetId<= 16 )
    {
            Read_TXRQ_register = CANx[Copy_u8Channel_ID]->TXRQ1 ;
    }
    /*The CANTXRQ2 holds the last 16 messages*/
    else if(CAN_MessageObicetId<=32)
    {
        Read_TXRQ_register = CANx[Copy_u8Channel_ID]->TXRQ1 << 16 ;
    }
    else
    {
            /**MISRA Rule**/
    }
    /*If TXOK and message request has been cleared and Call Tx_Confirmation*/
   if(!(Read_TXRQ_register &(1<<(CAN_MessageObicetId-1))) && Read_STS_register )
     {
            /*Clear TxOk flag*/
            CANx[Copy_u8Channel_ID]->STS &=~ ( 1 << CAN_STS_TXOK );
            /*Return 1 as message successfully transmitted*/
             return 1 ;
     }
     else
     {
         /*Message is not transmitted ok*/
            return 0;
     }
}

u8 CAN_u8ReceiveOk(u8 Copy_u8Channel_ID, CAN_MsgId_t CAN_MessageObicetId)
{
    u32 Read_NWDAT_register = 0 ;
    u8 Read_STS_register  = 0 ;

    /*Read Status register to check RXOK Received message successfully*/
     Read_STS_register = CANx[Copy_u8Channel_ID]->STS & ( 1 << CAN_STS_TXOK );
    /*The NWDA1 holds the first 16 messages*/
    if(CAN_MessageObicetId<= 16 )
    {
        Read_NWDAT_register = CANx[Copy_u8Channel_ID]->NWDA1 ;
    }
    /*The CANTXRQ2 holds the last 16 messages*/
    else if(CAN_MessageObicetId<=32)
    {
        Read_NWDAT_register = CANx[Copy_u8Channel_ID]->NWDA2 << 16 ;
    }
    else
    {
            /**MISRA Rule**/
    }

   if(!(Read_NWDAT_register &(1<<(CAN_MessageObicetId-1))) && Read_STS_register )
     {
            /*Clear RxOk flag*/
            CANx[Copy_u8Channel_ID]->STS &=~ ( 1 << CAN_STS_RXOK );
            /*Return 1 as message successfully Received*/
             return 1 ;
     }
     else
     {
         /*Message is not Received ok*/
            return 0;
     }
}
