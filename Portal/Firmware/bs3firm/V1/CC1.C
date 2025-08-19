//****************************************************************************
// @Module        CC1
// @Filename      CC1.C
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all functions that use the CAPCOM 1
//                peripheral
//                
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:22
//
//****************************************************************************

//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (CC1_General,1)

// USER CODE END


//***************************************************************************
// @Global Variables
//***************************************************************************

// USER CODE BEGIN (CC1_General,2)

// USER CODE END


//****************************************************************************
// @Function      void CC1_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This function initializes the CAPCOM 1 component. It effects
//                all necessary configurations of the SFR, depending on the
//                selected operating mode. The configuration determines
//                whether the CAPCOM interrupts are to be released, and 
//                the priority of the released interrupt.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:22
//
//****************************************************************************

void CC1_vInit(void)
{
  /// ---- Timer 0 Configuration -------------
  ///  timer 0 works in counter mode, counted by:
  ///  positive transition (rising edge) on pin T0IN
  ///  timer 0 run bit is reset

  /// ---- Timer 1 Configuration -------------
  ///  timer 1 works in timer mode
  ///  prescaler factor is 8
  ///  timer 1 run bit is reset

  T01CON = 0x0009;
  T0     = 0x0000;    // load timer 0 register
  T1     = 0x0000;    // load timer 1 register
  T0REL  = 0x0000;    // load timer 0 reload register
  T1REL  = 0x0000;    // load timer 1 reload register

  ///  enable timer 0 interrupt
  ///  timer 0 interrupt priority level(ILVL) = 14
  ///  timer 0 interrupt group level (GLVL) = 3
  T0IC = 0x007B;

  ///  use PEC channel 3 for timer 0 interrupt
  ///  - normal interrupt
  ///  - word transfer
  ///  - pointers are not modified
  PECC3 = 0x0000;

  ///  enable timer 1 interrupt
  ///  timer 1 interrupt priority level(ILVL) = 14
  ///  timer 1 interrupt group level (GLVL) = 1
  T1IC = 0x0079;

  ///  use PEC channel 1 for timer 1 interrupt
  ///  - normal interrupt
  ///  - word transfer
  ///  - pointers are not modified
  PECC1 = 0x0000;

  DP3  &= 0xFFFE;     /// reset P3.0 direction control (T0IN input)

  /// P2.0 is used as output for CAPCOM channel 0
  /// P2.1 is used as output for CAPCOM channel 1

  // initializes port 2 for the capture/compare channels
  P2   |= 0x0000;     //  set port data register
  ODP2 |= 0x0000;     //  set port open drain control register
  DP2  |= 0x0003;     //  set port direction register


  /// ---- Capture Compare Channel 0 -------------
  ///  compare mode 3:  set output pin CC0IO on each match
  ///  CC0 allocated to timer T0

  /// ---- Capture Compare Channel 1 -------------
  ///  compare mode 3:  set output pin CC1IO on each match
  ///  CC1 allocated to timer T1

  /// ---- Capture Compare Channel 2 -------------
  ///  disable capture compare channel 2

  /// ---- Capture Compare Channel 3 -------------
  ///  disable capture compare channel 3

  CCM0 = 0x00F7;
  CC0  = 0xFFFF;  // load CC0 register
  CC1  = 0xFFFF;  // load CC1 register


  /// ---- Capture Compare Channel 4 -------------
  ///  disable capture compare channel 4

  /// ---- Capture Compare Channel 5 -------------
  ///  disable capture compare channel 5

  /// ---- Capture Compare Channel 6 -------------
  ///  disable capture compare channel 6

  /// ---- Capture Compare Channel 7 -------------
  ///  disable capture compare channel 7

  /// ---- Capture Compare Channel 8 -------------
  ///  disable capture compare channel 8

  /// ---- Capture Compare Channel 9 -------------
  ///  disable capture compare channel 9

  /// ---- Capture Compare Channel 10 -------------
  ///  disable capture compare channel 10

  /// ---- Capture Compare Channel 11 -------------
  ///  disable capture compare channel 11
  ///  enable CC8 interrupt
  ///  CC8 interrupt priority level(ILVL) = 15
  ///  CC8 interrupt group level (GLVL) = 2
  CC8IC = 0x007E;

  ///  use PEC channel 6 for CC8 interrupt
  ///  - count = 1
  ///  - word transfer
  ///  - pointers are not modified
  PECC6 = 0x0001;

  SRCP6 = 0xFE48;
  /// User defined destination pointer is selected for PECC Channel 6

  ///  enable CC9 interrupt
  ///  CC9 interrupt priority level(ILVL) = 15
  ///  CC9 interrupt group level (GLVL) = 3
  CC9IC = 0x007F;

  ///  use PEC channel 7 for CC9 interrupt
  ///  - count = 1
  ///  - word transfer
  ///  - pointers are not modified
  PECC7 = 0x0001;

  /// User defined source pointer is selected for PECC Channel 7
  DSTP7 = 0xFE48;

  ///  enable CC10 interrupt
  ///  CC10 interrupt priority level(ILVL) = 11
  ///  CC10 interrupt group level (GLVL) = 3
  CC10IC = 0x006F;


  /// ---- Capture Compare Channel 12 -------------
  ///  disable capture compare channel 12

  /// ---- Capture Compare Channel 13 -------------
  ///  disable capture compare channel 13

  /// ---- Capture Compare Channel 14 -------------
  ///  disable capture compare channel 14

  /// ---- Capture Compare Channel 15 -------------
  ///  disable capture compare channel 15

  // USER CODE BEGIN (CC1_Init,1)
// DO NOT FORGET TO SET THE SOURCE-POINTER FOR PECC CHANNEL 7
// DO NOT FORGET TO SET THE DESTINATION-POINTER FOR PECC CHANNEL 6

  // USER CODE END

}


//****************************************************************************
// @Function      void CC1_viIsrTmr0(void) interrupt T0INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM timer 0.
//                It is called when overflow of the timer 0 register occurs.
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
// @Date          18.09.98 10:01:22
//
//****************************************************************************

void CC1_viIsrTmr0(void) interrupt T0INT
{
  // USER CODE BEGIN (CC1_IsrTmr0,0)

  // USER CODE END
}


//****************************************************************************
// @Function      void CC1_viIsrTmr1(void) interrupt T1INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM timer 1.
//                It is called when overflow of the timer 1 register occurs.
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
// @Date          18.09.98 10:01:22
//
//****************************************************************************

void CC1_viIsrTmr1(void) interrupt T1INT
{
  // USER CODE BEGIN (CC1_IsrTmr1,0)

  // USER CODE END
}




// USER CODE BEGIN (CC1_General,3)

// USER CODE END

