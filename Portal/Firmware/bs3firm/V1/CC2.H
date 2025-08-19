//****************************************************************************
// @Module        CC2
// @Filename      CC2.H
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all function prototypes and macros for
//                the Capture Compare unit 2.
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:27
//
//****************************************************************************

//****************************************************************************
// @Prototypes of global functions
//****************************************************************************

void CC2_vInit(void);


// USER CODE BEGIN (CC2Header,1)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Macro          CC2_vStartTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro starts the selected CAPCOM timer.
//                The timer continues counting from where it stopped.
//                
//                The following definitions for TimerNr are available:
//                TIMER_7
//                TIMER_8
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM timer to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:27
//
//****************************************************************************

#define CC2_vStartTmr(TimerNr)  CC2_vStart(TimerNr,R)
#define CC2_vStart(TimerNr,R) TimerNr##R = 1


//****************************************************************************
// @Macro          CC2_vStopTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro stops the selected CAPCOM timer.
//                The content of the timer register remains unchanged.
//                
//                The following definitions for TimerNr are available:
//                TIMER_7
//                TIMER_8
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM timer to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:27
//
//****************************************************************************

#define CC2_vStopTmr(TimerNr)  CC2_vStop(TimerNr,R)
#define CC2_vStop(TimerNr,R) TimerNr##R = 0                     


//****************************************************************************
// @Macro          CC2_vClearTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro stops the selected CAPCOM timer and
//                sets the timer register to 0.
//                
//                The following definitions for TimerNr are available:
//                TIMER_7
//                TIMER_8
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM timer to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:27
//
//****************************************************************************

#define CC2_vClearTmr(TimerNr) CC2_vClrTmr(TimerNr,R)
#define CC2_vClrTmr(TimerNr,R) TimerNr##R = 0; TimerNr = 0x0000 


//****************************************************************************
// @Macro          CC2_uwReadTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro returns the content of the CAPCOM timer. The
//                timer is not stopped. CPU access to the timer register
//                has a higher priority than a timer increment or a reload.
//                
//                The following definitions for TimerNr are available:
//                TIMER_7
//                TIMER_8
//
//----------------------------------------------------------------------------
// @Returnvalue   Current 16-bit value of the timer register
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM timer to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:27
//
//****************************************************************************

#define CC2_uwReadTmr(TimerNr) TimerNr


//****************************************************************************
// @Macro          CC2_vLoadTmr(TimerNr,Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the timer register of the selected CAPCOM 
//                timer with the requested value. The timer need not be
//                stopped for this to happen as a CPU access to the timer register
//                has a higher priority than a timer increment or a reload.
//                
//                The following definitions for TimerNr are available:
//                TIMER_7
//                TIMER_8
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM timer to be used
// @Parameters    16-bit Value with which the CAPCOM timer is to be loaded
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:27
//
//****************************************************************************

#define CC2_vLoadTmr(TimerNr,Value) TimerNr = Value


//****************************************************************************
// @Macro          CC2_vSetReload(TimerNr,Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the reload register of the selected CAPCOM
//                timer.
//                
//                The following definitions for TimerNr are available:
//                TIMER_7
//                TIMER_8
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM timer to be used
// @Parameters    16-bit Value with which the reload register is to be loaded
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:27
//
//****************************************************************************

#define CC2_vSetReload(TimerNr,Value) CC2_vReload(TimerNr,REL,Value)
#define CC2_vReload(TimerNr,REL,Value) TimerNr##REL = Value 


//****************************************************************************
// @Macro          CC2_vSetCCxReg(RegName,Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the selected capture/compare register
//                with the parameter Value.
//                
//                The following definitions are available for RegName
//                (only for enabled CAPCOM channels):
//                CC_16 - CC_31 (see @Defines in header file)
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM channel to be used
// @Parameters    16-bit Value with which the CCx register is to be loaded
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:27
//
//****************************************************************************

#define CC2_vSetCCxReg(RegName,Value) RegName = Value


//****************************************************************************
// @Macro          CC2_uwGetCCxReg(RegName)
//
//----------------------------------------------------------------------------
// @Description   This macro returns the contents of the selected capture/compare
//                register
//                
//                The following definitions are available for RegName
//                (only for enabled CAPCOM channels):
//                CC_16 - CC_31 (see @Defines in header file)
//
//----------------------------------------------------------------------------
// @Returnvalue   Current 16-bit value of the CCx register.
//
//----------------------------------------------------------------------------
// @Parameters    Name of the CAPCOM channel to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:27
//
//****************************************************************************

#define CC2_uwGetCCxReg(RegName) RegName



// USER CODE BEGIN (CC2Header,2)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

#define TIMER_7 T7
#define TIMER_8 T8

// Defines for enabled CAPCOM2 channels:

#define CC_28 CC28


// USER CODE BEGIN (CC2Header,3)

// USER CODE END


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

#define T7INT   0x3D
#define T8INT   0x3E


// USER CODE BEGIN (CC2Header,4)

// USER CODE END

