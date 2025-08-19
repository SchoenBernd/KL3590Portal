//****************************************************************************
// @Module        Asynchronous / Synchronous Serial Interface (ASC)
// @Filename      ASC.C
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all functions that use the ASC
//                
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:30
//
//****************************************************************************

//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (ASC_General,1)

// USER CODE END


//***************************************************************************
// @Global Variables
//***************************************************************************

// USER CODE BEGIN (ASC_General,2)

// USER CODE END


//****************************************************************************
// @Function      void ASC_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This function initializes the ASC component. It effects all
//                necessary configurations of the SFR, depending on the selected
//                operating mode. The configuration determines whether the ASC
//                interrupts are to be released, and the priority of the 
//                released interrupt.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:30
//
//****************************************************************************

void ASC_vInit(void)
{
  ///  8-bit data + parity asynchronous operation
  ///  even parity (parity bit set on odd number of '1's in data)
  ///  one stop bit
  ///  check parity error
  ///  check framing error
  ///  check overrun error
  ///  divide clock by reload value + constant
  ///  receiver enabled
  S0CON = 0x00F7;

  //   ASC baudrate reload register
  ///  baudrate =  9615,38 Baud
  S0BG = 0x0040;

  ///  enable transmit buffer interrupt
  ///  transmit buffer interrupt priority level(ILVL) = 10
  ///  transmit buffer interrupt group level (GLVL) = 1
  S0TBIC = 0x0069;

  ///  enable receive interrupt
  ///  receive interrupt priority level(ILVL) = 10
  ///  receive interrupt group level (GLVL) = 2
  S0RIC = 0x006A;

  ///  enable error interrupt
  ///  error interrupt priority level(ILVL) = 10
  ///  error interrupt group level (GLVL) = 3
  S0EIC = 0x006B;

  P3   |= 0x0400;     //  set P3.10 output latch (TXD0)
  DP3  |= 0x0400;     /// set P3.10 direction control (TXD0 output)
  DP3  &= 0xF7FF;     /// reset P3.11 direction control (RXD0 input)

  S0TIR = 1;          // indicate that the transmit register is empty

// USER CODE BEGIN (ASC_Init,1)

// USER CODE END

  S0R = 1;            // enable baudrate generator
}


//****************************************************************************
// @Function      void ASC_vSendData(uword Data)
//
//----------------------------------------------------------------------------
// @Description   This function writes a send data initialization word into
//                the S0TBUF register.
//                
//                Note:
//                In a multiprocessor system the master with this function
//                has the possibility to send data to the selected slave. To
//                achieve this, the 9th bit must set on zero.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    data to be written into the S0TBUF register
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:30
//
//****************************************************************************

void ASC_vSendData(uword Data)
{
  S0TBIR = 0;           // reset transmit buffer interrupt request flag
  S0TBUF = Data;        // load transmit buffer register
}


//****************************************************************************
// @Function      uword ASC_uwGetData(void)
//
//----------------------------------------------------------------------------
// @Description   This function reads out the content of the S0RBUF register
//                which contains a received data byte. 
//
//----------------------------------------------------------------------------
// @Returnvalue   data that has been received
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:30
//
//****************************************************************************

uword ASC_uwGetData(void)
{
  S0RIR = 0;             // reset receive interrupt request flag
  return(S0RBUF);        // return receive buffer register
}


//****************************************************************************
// @Function      void ASC_viIsrError(void) interrupt S0EINT
//
//----------------------------------------------------------------------------
// @Description   This is the error interrupt service routine for the ASC0.
//                It is called if during receipt of a data byte a framing,
//                parity or overflow error occured (only if enabled).
//                Note:
//                It is important that the user adds to this function with
//                Application Specific Code. User Code sections are available
//                in the function for this purpose.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:30
//
//****************************************************************************

void ASC_viIsrError(void) interrupt S0EINT
{

  if(S0PE)            // if parity error flag
  {
    // USER CODE BEGIN (ASC_IsrError,1)

    // USER CODE END
  }
  if(S0FE)            // if framing error flag
  {
    // USER CODE BEGIN (ASC_IsrError,2)

    // USER CODE END
  }
  if(S0OE)            // if overrun error flag
  {
    // USER CODE BEGIN (ASC_IsrError,3)

    // USER CODE END
  }

  S0CON &= 0xF8FF;    // reset the error flags

}


//****************************************************************************
// @Function      void ASC_viIsrRx(void) interrupt S0RINT
//
//----------------------------------------------------------------------------
// @Description   This is the receive interrupt service routine for the ASC0.
//                It is called if a byte has been received via ASC0
//                (S0RIR is set).
//                Note:
//                It is important that the user adds to this function with
//                Application Specific Code. User Code sections are available
//                in the function for this purpose.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:30
//
//****************************************************************************

void ASC_viIsrRx(void) interrupt S0RINT
{
  // USER CODE BEGIN (ASC_IsrRx,1)

  // USER CODE END
}


//****************************************************************************
// @Function      void ASC_viIsrTxBuf(void) interrupt S0TBINT
//
//----------------------------------------------------------------------------
// @Description   This is the transmit buffer interrupt service routine for
//                the ASC0. It is called if the content of the TX-buffer has
//                been loaded into the TX-shift register.
//                Note:
//                It is important that the user adds to this function with
//                Application Specific Code. User Code sections are available
//                in the function for this purpose.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:30
//
//****************************************************************************

void ASC_viIsrTxBuf(void) interrupt S0TBINT
{
  // USER CODE BEGIN (ASC_IsrTxBuf,1)

  // USER CODE END
}


//****************************************************************************
// @Function      bit ASC_bTxDataReady(void)
//
//----------------------------------------------------------------------------
// @Description   This function can be used for checking up the status of the
//                ASC transmitter interrupt flags (S0TIR). This shows when
//                the sending of a byte has terminated. By continuously 
//                polling the S0TIR bit after the function
//                ASC_vSendData has been called, it is
//                possible to establish when the ASC has terminated its task.
//
//----------------------------------------------------------------------------
// @Returnvalue   0 if transmitter is busy, else 1
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:30
//
//****************************************************************************

bit ASC_bTxDataReady(void)
{
  bit bReturnValue;

  bReturnValue = 0;

  if(S0TIR)             // if sending of data is terminated 
  {
    bReturnValue = 1;
    S0TIR = 0;          // clear transmit interrupt request flag
  }
  return(bReturnValue);
}


//****************************************************************************
// @Function      void ASC_vReceiverEnable(void)
//
//----------------------------------------------------------------------------
// @Description   This function releases the receive function of the ASC0.
//                After initialization this function does not need to be 
//                recalled.
//                Note:
//                The following function must be called everytime data is to
//                be received in synchronous mode.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:30
//
//****************************************************************************

void ASC_vReceiverEnable(void)
{
  S0REN = 1;               // enable the receiver
}




// USER CODE BEGIN (ASC_General,3)

// USER CODE END

