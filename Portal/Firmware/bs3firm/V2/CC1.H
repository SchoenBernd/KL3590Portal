//****************************************************************************
// @Module        CC1
// @Filename      CC1.H
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all function prototypes and macros for
//                the Capture Compare unit 1.
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:22
//
//****************************************************************************

//****************************************************************************
// @Prototypes of global functions
//****************************************************************************

void CC1_vInit(void);


// USER CODE BEGIN (CC1Header,1)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Macro          CC1_vStartTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro starts the selected CAPCOM timer.
//                The timer continues counting from where it stopped.
//                
//                The following definitions for TimerNr are available:
//                TIMER_0
//                TIMER_1
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM timer to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:22
//
//****************************************************************************

#define CC1_vStartTmr(TimerNr)  CC1_vStart(TimerNr,R)
#define CC1_vStart(TimerNr,R) TimerNr##R = 1


//****************************************************************************
// @Macro          CC1_vStopTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro stops the selected CAPCOM timer.
//                The content of the timer register remains unchanged.
//                
//                The following definitions for TimerNr are available:
//                TIMER_0
//                TIMER_1
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM timer to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:22
//
//****************************************************************************

#define CC1_vStopTmr(TimerNr)  CC1_vStop(TimerNr,R)
#define CC1_vStop(TimerNr,R) TimerNr##R = 0                     


//****************************************************************************
// @Macro          CC1_vClearTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro stops the selected CAPCOM timer and
//                sets the timer register to 0.
//                
//                The following definitions for TimerNr are available:
//                TIMER_0
//                TIMER_1
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM timer to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:22
//
//****************************************************************************

#define CC1_vClearTmr(TimerNr) CC1_vClrTmr(TimerNr,R)
#define CC1_vClrTmr(TimerNr,R) TimerNr##R = 0; TimerNr = 0x0000 


//****************************************************************************
// @Macro          CC1_uwReadTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro returns the content of the CAPCOM timer. The
//                timer is not stopped. CPU access to the timer register
//                has a higher priority than a timer increment or a reload.
//                
//                The following definitions for TimerNr are available:
//                TIMER_0
//                TIMER_1
//
//----------------------------------------------------------------------------
// @Returnvalue   Current 16-bit value of the timer register
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM timer to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:23
//
//****************************************************************************

#define CC1_uwReadTmr(TimerNr) TimerNr


//****************************************************************************
// @Macro          CC1_vLoadTmr(TimerNr,Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the timer register of the selected CAPCOM 
//                timer with the requested value. The timer need not be be
//                stopped for this happen as a CPU access to the timer register
//                has a higher priority than a timer increment or a reload.
//                
//                The following definitions for TimerNr are available:
//                TIMER_0
//                TIMER_1
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM timer to be used
// @Parameters    16-bit Value with which the CAPCOM timer is to be loaded
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:23
//
//****************************************************************************

#define CC1_vLoadTmr(TimerNr,Value) TimerNr = Value


//****************************************************************************
// @Macro          CC1_vSetReload(TimerNr,Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the reload register of the selected CAPCOM
//                timer.
//                
//                The following definitions for TimerNr are available:
//                TIMER_0
//                TIMER_1
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM timer to be used
// @Parameters    16-bit Value with which the reload register is to be loaded
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:23
//
//****************************************************************************

#define CC1_vSetReload(TimerNr,Value) CC1_vReload(TimerNr,REL,Value)
#define CC1_vReload(TimerNr,REL,Value) TimerNr##REL = Value 


//****************************************************************************
// @Macro          CC1_vSetCCxReg(RegName,Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the selected capture/compare register
//                with the parameter Value.
//                
//                The following definitions are available for RegName
//                (only for enabled CAPCOM channels):
//                CC_0 - CC_15 (see @Defines in header file)
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM channel to be used
// @Parameters    16-bit Value with which the CCx register is to be loaded
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:23
//
//****************************************************************************

#define CC1_vSetCCxReg(RegName,Value) RegName = Value


//****************************************************************************
// @Macro          CC1_uwGetCCxReg(RegName)
//
//----------------------------------------------------------------------------
// @Description   This macro returns the content of the selected capture/compare
//                register
//                
//                The following definitions are available for RegName
//                (only for enabled CAPCOM channels):
//                CC_0 - CC_15 (see @Defines in header file)
//
//----------------------------------------------------------------------------
// @Returnvalue   Current 16-bit value of the CCx register.
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM channel to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:23
//
//****************************************************************************

#define CC1_uwGetCCxReg(RegName) RegName



// USER CODE BEGIN (CC1Header,2)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

#define TIMER_0 T0
#define TIMER_1 T1

// Defines for enabled CAPCOM1 channels:

#define CC_0 CC0
#define CC_1 CC1


// USER CODE BEGIN (CC1Header,3)

// USER CODE END


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

#define T0INT   0x20
#define T1INT   0x21


// USER CODE BEGIN (CC1Header,4)

// USER CODE END

