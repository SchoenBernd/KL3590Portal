//****************************************************************************
// @Module        Interrupt Controller
// @Filename      INT.C
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all selected ISRs for fast external
//                interrupts and general functions for interrupts and PEC.
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:41
//
//****************************************************************************

//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (IR_General,1)

// USER CODE END


//***************************************************************************
// @Global Variables
//***************************************************************************

// USER CODE BEGIN (IR_General,2)

// USER CODE END


//****************************************************************************
// @Function      void INT_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This function initializes the fast external
//                interrupts 0 - 7. It effects all necessary
//                configurations of the SFR, depending on the selected
//                operating mode. The configuration determines whether the 
//                external interrupts are to be released, and the priority 
//                of the released interrupt.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:41
//
//****************************************************************************

void INT_vInit(void)
{

  /// ------- external interrupt 0 edge selection -------
  ///  external interrupt 0 on positive edge (rising) at pin P2.8

  /// ------- external interrupt 1 edge selection -------
  ///  external interrupt 1 on positive edge (rising) at pin P2.9

  /// ------- external interrupt 2 edge selection -------
  ///  external interrupt 2 on positive edge (rising) at pin P2.10

  EXICON = 0x0015;
  DP2   &= 0xF8FF;


  ///  enable external 0 interrupt
  ///  external 0 interrupt priority level(ILVL) = 15
  ///  external 0 interrupt group level (GLVL) = 2
  CC8IC = 0x007E;

  ///  use PEC channel 6 for external 0 interrupt
  ///  - count = 1
  ///  - word transfer
  ///  - pointers are not modified
  PECC6 = 0x0001;

  SRCP6 = 0xFE48;
  /// User defined destination pointer is selected for PECC Channel 6

  ///  enable external 1 interrupt
  ///  external 1 interrupt priority level(ILVL) = 15
  ///  external 1 interrupt group level (GLVL) = 3
  CC9IC = 0x007F;

  ///  use PEC channel 7 for external 1 interrupt
  ///  - count = 1
  ///  - word transfer
  ///  - pointers are not modified
  PECC7 = 0x0001;

  /// User defined source pointer is selected for PECC Channel 7
  DSTP7 = 0xFE48;

  ///  enable external 2 interrupt
  ///  external 2 interrupt priority level(ILVL) = 11
  ///  external 2 interrupt group level (GLVL) = 3
  CC10IC = 0x006F;



// USER CODE BEGIN (IR_Init,1)
// DO NOT FORGET TO SET THE SOURCE-POINTER FOR PECC CHANNEL 7
// DO NOT FORGET TO SET THE DESTINATION-POINTER FOR PECC CHANNEL 6

// USER CODE END


}


//****************************************************************************
// @Function      void INT_viIsrEx0(void) interrupt CC8INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the external
//                interrupt 0 (P2.8).
//                It is called when the configured event is captured.
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
// @Date          18.09.98 10:01:41
//
//****************************************************************************

void INT_viIsrEx0(void) interrupt CC8INT
{
  // USER CODE BEGIN (IR_IsrEx0,1)

  // USER CODE END
}


//****************************************************************************
// @Function      void INT_viIsrEx1(void) interrupt CC9INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the external
//                interrupt 1 (P2.9).
//                It is called when the configured event is captured.
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
// @Date          18.09.98 10:01:41
//
//****************************************************************************

void INT_viIsrEx1(void) interrupt CC9INT
{
  // USER CODE BEGIN (IR_IsrEx1,1)

  // USER CODE END
}


//****************************************************************************
// @Function      void INT_viIsrEx2(void) interrupt CC10INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the external
//                interrupt 2 (P2.10).
//                It is called when the configured event is captured.
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
// @Date          18.09.98 10:01:41
//
//****************************************************************************

void INT_viIsrEx2(void) interrupt CC10INT
{
  // USER CODE BEGIN (IR_IsrEx2,1)

  // USER CODE END
}




// USER CODE BEGIN (IR_General,3)

// USER CODE END


