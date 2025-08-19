//****************************************************************************
// @Module        General Purpose Timer Unit 1
// @Filename      GT1.C
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all functions that use the GPT1 unit
//                
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:31
//
//****************************************************************************

//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (GT1_General,1)

// USER CODE END


//***************************************************************************
// @Global Variables
//***************************************************************************

// USER CODE BEGIN (GT1_General,2)

// USER CODE END


//****************************************************************************
// @Function      void GT1_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This function initializes the GPT1 component. It effects all
//                necessary configurations of the SFR, depending on the selected
//                operating mode. The configuration determines whether the GPT1
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
// @Date          18.09.98 10:01:31
//
//****************************************************************************

void GT1_vInit(void)
{
  /// ------------ Timer 3 Control Register ----------
  ///  timer 3 works in counter mode
  ///  positive transition (rising edge) on T3IN
  ///  counter 3 run bit is reset
  ///  up/down control bit is set
  ///  external up/down control is disabled
  ///  alternate output (toggle T3OUT) function is disabled 
  T3CON = 0x0089;
  T3    = 0x0000;  //  load timer 3 register

  DP3  &= 0xFFBF;  /// reset P3.6 direction control (T3IN input)

  ///  enable timer 3 interrupt
  ///  timer 3 interrupt priority level(ILVL) = 12
  ///  timer 3 interrupt group level (GLVL) = 2
  T3IC = 0x0072;

  /// ---------- Timer 2 Control Register ----------
  ///  timer 2 works in counter mode
  ///  positive transition (rising edge) on T2IN
  ///  timer 2 run bit is reset
  ///  up/down control bit is set
  ///  external up/down control is disabled
  T2CON = 0x0089;
  T2    = 0x0000;  //  load timer 2 register

  DP3 &= 0xFF7F;   /// reset P3.7 direction control (T2IN input)

  ///  enable timer 2 interrupt
  ///  timer 2 interrupt priority level(ILVL) = 12
  ///  timer 2 interrupt group level (GLVL) = 3
  T2IC = 0x0073;

  /// ---------- Timer 4 Control Register ----------
  ///  timer 4 works in counter mode
  ///  positive transition (rising edge) on T4IN
  ///  timer 4 run bit is reset
  ///  up/down control bit is set         
  ///  external up/down control is disabled
  T4CON = 0x0089;
  T4    = 0x0000;  //  load timer 4 register


  DP3 &= 0xFFDF;   /// reset P3.5 direction control (T4IN input)

  ///  enable timer 4 interrupt
  ///  timer 4 interrupt priority level(ILVL) = 12
  ///  timer 4 interrupt group level (GLVL) = 1
  T4IC = 0x0071;


// USER CODE BEGIN (GT1_Init,1)

// USER CODE END


}


//****************************************************************************
// @Function      void GT1_viIsrTmr3(void) interrupt T3INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT1 timer 3.
//                It is called up in the case of over or underflow of the 
//                timer 3 register.
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
// @Date          18.09.98 10:01:31
//
//****************************************************************************

void GT1_viIsrTmr3(void) interrupt T3INT
{
  // USER CODE BEGIN (GT1_IsrTmr3,1)

  // USER CODE END
}


//****************************************************************************
// @Function      void GT1_viIsrTmr2(void) interrupt T2INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT1 timer 2.
//                It is called up when the timer 2 register over or under-
//                flows or if timer 3 is reloaded with the content of timer 2
//                (external signal or T3 over/underflow) or if an external
//                capture event arrives.
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
// @Date          18.09.98 10:01:31
//
//****************************************************************************

void GT1_viIsrTmr2(void) interrupt T2INT
{
  // USER CODE BEGIN (GT1_IsrTmr2,1)

  // USER CODE END
}


//****************************************************************************
// @Function      void GT1_viIsrTmr4(void) interrupt T4INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT1 timer 4.
//                It is called up when the timer 4 register is over or under-
//                flows or if timer 3 is reloaded with the content of timer 4
//                (external signal or T3 over/underflow) or if an external
//                capture event arrives.
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
// @Date          18.09.98 10:01:31
//
//****************************************************************************

void GT1_viIsrTmr4(void) interrupt T4INT
{
  // USER CODE BEGIN (GT1_IsrTmr4,1)

  // USER CODE END
}




// USER CODE BEGIN (GT1_General,3)

// USER CODE END

