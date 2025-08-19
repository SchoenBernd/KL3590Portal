//****************************************************************************
// @Module        Ports
// @Filename      IO.C
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all functions that use the general IO ports
//                
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:39
//
//****************************************************************************

//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (IO_General,1)

// USER CODE END


//***************************************************************************
// @Global Variables
//***************************************************************************

// USER CODE BEGIN (IO_General,2)

// USER CODE END


//****************************************************************************
// @Function      void IO_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This function initializes the IO component. It effects all
//                necessary configurations of the SFR, depending on the selected
//                operating mode.
//                Note:
//                Only those pins that are initialized are used as general IO.
//                If the alternate function of a pin is used, this pin is
//                initialized in the initialization function of the peripheral.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:40
//
//****************************************************************************

void IO_vInit(void)
{
  /// P8.0 .. P8.7  switch on standard TTL input levels
  /// P7.0 .. P7.7  switch on standard TTL input levels
  /// P3.8 .. P3.15 switch on standard TTL input levels
  /// P3.0 .. P3.7  switch on standard TTL input levels
  /// P2.8 .. P2.15 switch on standard TTL input levels
  /// P2.0 .. P2.7  switch on standard TTL input levels
  _PICON = 0x0000;

  /// no pin of port 0 low is used as general IO

  /// no pin of port 0 high is used as general IO

  /// no pin of port 1 low is used as general IO

  /// no pin of port 1 high is used as general IO

  //  initializes port 2
  /// P2.2 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P2.3 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P2.4 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P2.5 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P2.6 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P2.7 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P2.11 is used as general IO
  /// -  the pin is configured as input
  /// P2.12 is used as general IO
  /// -  the pin is configured as input
  /// P2.13 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P2.14 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  P2   = 0x0000;      // set port data register
  ODP2 = 0x0000;      // set port open drain control register
  DP2  = 0x60FC;      // set port direction register

  //  initializes port 3
  /// P3.1 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P3.3 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P3.4 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  P3   = 0x0000;      // set port data register
  ODP3 = 0x0000;      // set port open drain control register
  DP3  = 0x001A;      // set port direction register

  //  initializes port 4
  /// P4.4 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin status is low level
  /// P4.7 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin status is low level
  P4   = 0x0000;      // set port data register
  DP4  = 0x0090;      // set port direction register

  /// P5.0 is used as general input
  /// P5.1 is used as general input
  /// P5.2 is used as general input
  /// P5.3 is used as general input
  /// P5.4 is used as general input
  /// P5.5 is used as general input
  /// P5.6 is used as general input
  /// P5.7 is used as general input
  /// P5.8 is used as general input
  /// P5.9 is used as general input
  /// P5.11 is used as general input
  /// P5.14 is used as general input
  /// P5.15 is used as general input

  /// no pin of port 6 is used as general IO

  //  initializes port 7
  /// P7.5 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P7.6 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P7.7 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  P7   = 0x0000;      // set port data register
  ODP7 = 0x0000;      // set port open drain control register
  DP7  = 0x00E0;      // set port direction register

  //  initializes port 8
  /// P8.0 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P8.1 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P8.2 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P8.3 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P8.4 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P8.5 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P8.6 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  /// P8.7 is used as general IO
  /// -  the pin is configured as output
  /// -  the pin is configured to push/pull mode
  /// -  the pin status is low level
  // initializes port 8
  P8   = 0x0000;      // set port data register
  ODP8 = 0x0000;      // set port open drain control register
  DP8  = 0x00FF;      // set port direction register


  // USER CODE BEGIN (IO_Init,1)

  // USER CODE END

}


// USER CODE BEGIN (IO_General,3)

// USER CODE END

