//****************************************************************************
// @Module        CC2
// @Filename      CC2.C
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all functions that use the CAPCOM 2
//                peripheral
//                
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:26
//
//****************************************************************************

//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (CC2_General,1)

// USER CODE END


//***************************************************************************
// @Global Variables
//***************************************************************************

// USER CODE BEGIN (CC2_General,2)

// USER CODE END


//****************************************************************************
// @Function      void CC2_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This function initializes the CAPCOM 2 component. It effects
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
// @Date          18.09.98 10:01:26
//
//****************************************************************************

void CC2_vInit(void)
{
  /// ---- Timer 7 Configuration -------------
  ///  timer 7 works in counter mode, counted by:
  ///  positive transition (rising edge) on pin T7IN
  ///  timer 7 run bit is reset

  /// ---- Timer 8 Configuration -------------
  ///  timer 8 works in timer mode
  ///  prescaler factor is 8
  ///  timer 8 run bit is reset

  T78CON = 0x0009;
  T7     = 0x0000;    // load timer 7 register
  T8     = 0x1388;    // load timer 8 register
  T7REL  = 0x0000;    // load timer 7 reload register
  T8REL  = 0x1388;    // load timer 8 reload register

  ///  enable timer 7 interrupt
  ///  timer 7 interrupt priority level(ILVL) = 14
  ///  timer 7 interrupt group level (GLVL) = 2
  T7IC = 0x007A;

  ///  use PEC channel 2 for timer 7 interrupt
  ///  - normal interrupt
  ///  - word transfer
  ///  - pointers are not modified
  PECC2 = 0x0000;

  ///  enable timer 8 interrupt
  ///  timer 8 interrupt priority level(ILVL) = 9
  ///  timer 8 interrupt group level (GLVL) = 3
  T8IC = 0x0067;

  DP2  &= 0x7FFF;     /// reset P2.15 direction control (T7IN input)

  /// P7.4 is used as output for CAPCOM channel 28

  // initializes port 7 for the capture/compare channels
  P7   |= 0x0000;     //  set port data register
  ODP7 |= 0x0000;     //  set port open drain control register
  DP7  |= 0x0010;     //  set port direction register


  /// ---- Capture Compare Channel 16 -------------
  ///  disable capture compare channel 16

  /// ---- Capture Compare Channel 17 -------------
  ///  disable capture compare channel 17

  /// ---- Capture Compare Channel 18 -------------
  ///  disable capture compare channel 18

  /// ---- Capture Compare Channel 19 -------------
  ///  disable capture compare channel 19

  /// ---- Capture Compare Channel 20 -------------
  ///  disable capture compare channel 20

  /// ---- Capture Compare Channel 21 -------------
  ///  disable capture compare channel 21

  /// ---- Capture Compare Channel 22 -------------
  ///  disable capture compare channel 22

  /// ---- Capture Compare Channel 23 -------------
  ///  disable capture compare channel 23

  /// ---- Capture Compare Channel 24 -------------
  ///  disable capture compare channel 24

  /// ---- Capture Compare Channel 25 -------------
  ///  disable capture compare channel 25

  /// ---- Capture Compare Channel 26 -------------
  ///  disable capture compare channel 26

  /// ---- Capture Compare Channel 27 -------------
  ///  disable capture compare channel 27

  /// ---- Capture Compare Channel 28 -------------
  ///  compare mode 3:  set output pin CC28IO on each match
  ///  CC28 allocated to timer T7

  /// ---- Capture Compare Channel 29 -------------
  ///  disable capture compare channel 29

  /// ---- Capture Compare Channel 30 -------------
  ///  disable capture compare channel 30

  /// ---- Capture Compare Channel 31 -------------
  ///  disable capture compare channel 31

  CCM7 = 0x0007;
  CC28  = 0xFFFF;  // load CC28 register


  // USER CODE BEGIN (CC2_Init,1)

  // USER CODE END

}


//****************************************************************************
// @Function      void CC2_viIsrTmr7(void) interrupt T7INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM timer 7.
//                It is called when the timer 7 register overflows.
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
// @Date          18.09.98 10:01:26
//
//****************************************************************************

void CC2_viIsrTmr7(void) interrupt T7INT
{
  // USER CODE BEGIN (CC2_IsrTmr7,0)

  // USER CODE END
}


//****************************************************************************
// @Function      void CC2_viIsrTmr8(void) interrupt T8INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM timer 8.
//                It is called when the timer 8 register overflows.
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
// @Date          18.09.98 10:01:26
//
//****************************************************************************

void CC2_viIsrTmr8(void) interrupt T8INT
{
  // USER CODE BEGIN (CC2_IsrTmr8,0)

  // USER CODE END
}



// USER CODE BEGIN (CC2_General,3)

// USER CODE END


