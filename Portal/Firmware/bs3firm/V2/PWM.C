//****************************************************************************
// @Module        PWM
// @Filename      PWM.C
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all functions that use the PWM peripheral
//                
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:28
//
//****************************************************************************

//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (PWM_General,1)

// USER CODE END


//***************************************************************************
// @Global Variables
//***************************************************************************

// USER CODE BEGIN (PWM_General,2)

// USER CODE END


//****************************************************************************
// @Function      void PWM_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This function initializes the PWM component. It effects all
//                necessary configurations of the SFR, depending on the selected
//                operating mode. The configuration determines whether the PWM
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
// @Date          18.09.98 10:01:28
//
//****************************************************************************

void PWM_vInit(void)
{
  /// ------------- PWM channel 0 --------------
  /// PWM channel 0 operates in mode 0 (edge aligned PWM)
  /// PWM channel 0 output signal (P7.0) enabled
  /// timer PT0 is disconnected from its input clock
  /// timer PT0 is clocked with fcpu
  /// interrupt from channel 0 is disabled
  /// the period of a PWM0 cycle is 500,000 탎
  /// the duty cycle of PWM0 is 50,00 %

  /// ------------- PWM channel 1 --------------
  /// PWM channel 1 operates in mode 0 (edge aligned PWM)
  /// PWM channel 1 output signal (P7.1) enabled
  /// timer PT1 is disconnected from its input clock
  /// timer PT1 is clocked with fcpu
  /// interrupt from channel 1 is disabled
  /// the period of a PWM1 cycle is 500,000 탎
  /// the duty cycle of PWM1 is 50,00 %

  /// ------------- PWM channel 2 --------------
  /// PWM channel 2 operates in mode 0 (edge aligned PWM)
  /// PWM channel 2 output signal (P7.2) enabled
  /// timer PT2 is disconnected from its input clock
  /// timer PT2 is clocked with fcpu
  /// interrupt from channel 2 is disabled
  /// the period of a PWM2 cycle is 500,000 탎
  /// the duty cycle of PWM2 is 50,00 %

  /// ------------- PWM channel 3 --------------
  /// PWM channel 3 operates in mode 0 (edge aligned PWM)
  /// PWM channel 3 output signal (P7.3) enabled
  /// timer PT3 is disconnected from its input clock
  /// timer PT3 is clocked with fcpu / 64
  /// interrupt from channel 3 is disabled
  /// the period of a PWM3 cycle is 499,200 탎
  /// the duty cycle of PWM3 is 50,00 %

  PT0 = 0x0000;   // PWM counter register 0
  PP0 = 0x270F;   // PWM period register 0
  PW0 = 0x1388;   // PWM pulse width register 0
  PT1 = 0x0000;   // PWM counter register 1
  PP1 = 0x270F;   // PWM period register 1
  PW1 = 0x1388;   // PWM pulse width register 1
  PT2 = 0x0000;   // PWM counter register 2
  PP2 = 0x270F;   // PWM period register 2
  PW2 = 0x1388;   // PWM pulse width register 2
  PT3 = 0x0000;   // PWM counter register 3
  PP3 = 0x009B;   // PWM period register 3
  PW3 = 0x004E;   // PWM pulse width register 3

  PWMCON0 = 0x0080;
  PWMCON1 = 0x000F;

  /// PWM channel 0 output the signal on P7.0
  /// - the signal is driven directly to P7.0
  /// PWM channel 1 output the signal on P7.1
  /// - the signal is driven directly to P7.1
  /// PWM channel 2 output the signal on P7.2
  /// - the signal is driven directly to P7.2
  /// PWM channel 3 output the signal on P7.3
  /// - the signal is driven directly to P7.3

  // initializes port 7 for the PWM channels
  P7   |= 0x0000;     //  set port data register
  ODP7 |= 0x0000;     //  set port open drain control register
  DP7  |= 0x000F;     //  set port direction register


  // USER CODE BEGIN (PWM_Init,1)

  // USER CODE END


}


//****************************************************************************
// @Function      void PWM_vStatus(void)
//
//----------------------------------------------------------------------------
// @Description   All channel interrupt request flags that are not checked
//                within the ISR, are checked in this polling function. 
//                The individual interrupt request flag of a channel is set
//                at the beginning of a new PWM cycle. This indicates that
//                register PPx and PWx are now ready to receive a new value.
//                Note:
//                It is important that the user adds to this function with
//                Application Specific Code. User Code sections are available.
//                in the function for this purpose.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:28
//
//****************************************************************************

void PWM_vStatus(void)
{
  if(PIR0)  // a new cycle of PWM channel 0 begins
  {
    // USER CODE BEGIN (PWM_Status,0)

    // USER CODE END

    PIR0 = 0;
  }
  if(PIR1)  // a new cycle of PWM channel 1 begins
  {
    // USER CODE BEGIN (PWM_Status,1)

    // USER CODE END

    PIR1 = 0;
  }
  if(PIR2)  // a new cycle of PWM channel 2 begins
  {
    // USER CODE BEGIN (PWM_Status,2)

    // USER CODE END

    PIR2 = 0;
  }
  if(PIR3)  // a new cycle of PWM channel 3 begins
  {
    // USER CODE BEGIN (PWM_Status,3)

    // USER CODE END

    PIR3 = 0;
  }
  
  // USER CODE BEGIN (PWM_Status,4)

  // USER CODE END
}




// USER CODE BEGIN (PWM_General,3)

// USER CODE END

