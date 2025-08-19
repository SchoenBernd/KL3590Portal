//****************************************************************************
// @Module        Main
// @Filename      MAIN.C
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains the Project initialization function.
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:00:57
//
//****************************************************************************

/*
******************************************************************************
@Note




******************************************************************************
*/

//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"


//***************************************************************************
// @Global Variables
//***************************************************************************

// USER CODE BEGIN (Main,0)

// USER CODE END


//****************************************************************************
// @Function      void Project_Init(void)
//
//----------------------------------------------------------------------------
// @Description   This function initializes the microcontroller.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:00:57
//
//****************************************************************************

void Project_Init(void)
{
  /// --- initialization of the SYSCON Register ---
  /// the following initialization is made in the Start.asm
  ///- the internal ROM area is mapped to segment 0
  ///- the segmentation is enabled (CSP is saved/restored during interrupt entry/exit)
  ///- the internal ROM is disabled: accesses to the ROM area use the external bus
  ///- the pin #BHE (byte high enable) is enabled
  ///- the system clock output is enabled
  ///- the pins #WR and #BHE retain their normal functions
  ///- the on-chip X-Peripherals are enabled and can be accessed
  ///- accesses to the XBUS peripherals are done internally
  ///- external accesses to the XBUS peripherals are disabled


  /// --- initialization of the BUSCON 0-4 and ADRRSEL Registers 1-4 ---
  /// for the Keil Compiler the initialization is made in the Start.asm

  /// ---------- external bus 0 is enabled ----------
  ///- 8-bit demultiplexed bus
  ///- memory cycle time control: asynchronous READY signal, 1  waitstates
  ///- read/write delay control: activate signal with falling edge of ALE 
  ///- signal configuration: enable READY signal 
  ///- chip select mode: address chip select 
  ///- memory tristate control: no waitstate
  ///- ALE lengthening control: normal ALE signal

  /// ---------- external bus 1 is enabled ----------
  ///- 16-bit demultiplexed bus
  ///- memory cycle time control: 0 waitstates
  ///- read/write delay control: activate signal with falling edge of ALE 
  ///- signal configuration: disable READY signal 
  ///- chip select mode: address chip select 
  ///- memory tristate control: no waitstate
  ///- ALE lengthening control: normal ALE signal


  // initializes the IO ports
  IO_vInit();

  // initializes the ASC peripheral
  ASC_vInit();

  // initializes the CAN peripheral
  CAN_vInit();

  // initializes the PWM peripheral
  PWM_vInit();

  // initializes the CAPCOM 1 peripheral
  CC1_vInit();

  // initializes the CAPCOM 2 peripheral
  CC2_vInit();

  // initializes the general purpose timer unit 1
  GT1_vInit();

  // initializes the general purpose timer unit 2
  GT2_vInit();

  // initializes the external interrupts
  INT_vInit();

  // USER CODE BEGIN (Project_Init,1)

  // USER CODE END

  // globally enable interrupts
  IEN = 1;
}


//****************************************************************************
// @Function      void main(void)
//
//----------------------------------------------------------------------------
// @Description   This is the main function.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:00:58
//
//****************************************************************************

void main(void)
{
  // USER CODE BEGIN (Main,1)

  // USER CODE END

  Project_Init();

  // USER CODE BEGIN (Main,2)

  // USER CODE END
}

