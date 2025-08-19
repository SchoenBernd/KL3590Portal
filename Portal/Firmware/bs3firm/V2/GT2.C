//****************************************************************************
// @Module        General Purpose Timer Unit 2
// @Filename      GT2.C
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all functions that use the GPT2 unit
//                
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:32
//
//****************************************************************************

//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (GT2_General,1)

// USER CODE END


//***************************************************************************
// @Global Variables
//***************************************************************************

// USER CODE BEGIN (GT2_General,2)

// USER CODE END


//****************************************************************************
// @Function      void GT2_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This function initializes the GPT2 component. It effects all
//                necessary configurations of the SFR, depending on the selected
//                operating mode. The configuration determines whether the GPT2
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
// @Date          18.09.98 10:01:32
//
//****************************************************************************

void GT2_vInit(void)
{
  /// ---------- CAPREL ----------
  ///  timer 6 reload from register CAPREL is disabled
  ///  timer 5 capture into register CAPREL is enabled
  ///  CAPREL input selection:
  ///  - positive transition (rising edge) on CAPIN (P3.2)
  ///  timer 5 is cleared on a capture

  CAPREL = 0x0000; //  load CAPREL register
  DP3   &= 0xFFFB; /// reset P3.2 direction control (CAPIN input)

  ///  enable CAPREL interrupt
  ///  CAPREL interrupt priority level(ILVL) = 14
  ///  CAPREL interrupt group level (GLVL) = 0
  CRIC = 0x0078;

  ///  use PEC channel 0 for GPT2 CAPREL interrupt
  ///  - continuous transfer
  ///  - word transfer
  ///  - pointers are not modified
  PECC0 = 0x00FF;

  SRCP0 = 0xFE4A;
  /// User defined destination pointer is selected for PECC Channel 0

  /// ---------- Timer 6 Control Register ----------
  ///  timer 6 works in counter mode
  ///  positive transition (rising edge) on T6IN
  ///  counter 6 run bit is reset
  ///  up/down control bit is reset
  ///  external up/down control is enabled
  ///  alternate output function is disabled
  ///  timer 6 output toggle latch is set to 0
  T6CON = 0x0109;
  T6    = 0x0000;  //  load timer 6 register

  /// ---------- Timer 5 Control Register ----------
  ///  timer 5 works in counter mode
  ///  positive transition (rising edge) on T5IN
  ///  timer 5 run bit is reset
  ///  up/down control bit is reset
  ///  external up/down control is disabled
  T5CON = 0xD009;
  T5    = 0x0000;  //  load timer 5 register

  ///  enable timer 5 interrupt
  ///  timer 5 interrupt priority level(ILVL) = 13
  ///  timer 5 interrupt group level (GLVL) = 3
  T5IC = 0x0077;


// USER CODE BEGIN (GT2_Init,1)
// DO NOT FORGET TO SET THE DESTINATION-POINTER FOR PECC CHANNEL 0

// USER CODE END


}


//****************************************************************************
// @Function      void GT2_viIsrCAPREL(void) interrupt CRINT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT2 capture/ 
//                reload register CAPREL. The CAPREL register has been 
//                configured into capture mode and a capture event is
//                at pin P3.2.
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
// @Date          18.09.98 10:01:32
//
//****************************************************************************

void GT2_viIsrCAPREL(void) interrupt CRINT
{
  // USER CODE BEGIN (GT2_IsrCaprel,1)

  // USER CODE END
}


//****************************************************************************
// @Function      void GT2_viIsrTmr5(void) interrupt T5INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT2 timer 5. 
//                It is called when over or underflow of the timer 5
//                register occurs.
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
// @Date          18.09.98 10:01:32
//
//****************************************************************************

void GT2_viIsrTmr5(void) interrupt T5INT
{
  // USER CODE BEGIN (GT2_IsrTmr5,1)

  // USER CODE END
}




// USER CODE BEGIN (GT2_General,3)

// USER CODE END

