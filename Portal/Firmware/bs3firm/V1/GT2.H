//****************************************************************************
// @Module        General Purpose Timer Unit 2
// @Filename      GT2.H
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all function prototypes and macros for 
//                the GPT2 unit.
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:33
//
//****************************************************************************

//****************************************************************************
// @Prototypes of global functions
//****************************************************************************

void GT2_vInit(void);


// USER CODE BEGIN (Header,1)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Macro          GT2_vStartTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro starts the selected GPT2 timer. The timer continues
//                to count from where it has stopped.
//                The following definitions are available for TimerNr:
//                TIMER_5
//                TIMER_6
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be started
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:33
//
//****************************************************************************

#define GT2_vStartTmr(TimerNr)  GT2_vStart(TimerNr,R)
#define GT2_vStart(TimerNr,R) TimerNr##R = 1


//****************************************************************************
// @Macro          GT2_vStopTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro stops the selected GPT2 timer. The contents of
//                the timer register remain unchanged.
//                The following definitions are available for TimerNr:
//                TIMER_5
//                TIMER_6
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be stopped
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:33
//
//****************************************************************************

#define GT2_vStopTmr(TimerNr)  GT2_vStop(TimerNr,R)
#define GT2_vStop(TimerNr,R) TimerNr##R = 0                     


//****************************************************************************
// @Macro          GT2_vClearTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro stops the selected GPT2 timer and sets the timer
//                register to 0.
//                The following definitions are available for TimerNr:
//                TIMER_5
//                TIMER_6
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be cleared
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:33
//
//****************************************************************************

#define GT2_vClearTmr(TimerNr) GT2_vClrTmr(TimerNr,R)
#define GT2_vClrTmr(TimerNr,R) TimerNr##R = 0; TimerNr = 0x0000 


//****************************************************************************
// @Macro          GT2_uwReadTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro returns the content of the selected GPT2 timer.
//                The timer is not stopped. Access of the CPU to the
//                registers of the timer is of a higher priority than a timer
//                increment, a reload or a capture event.
//                The following definitions are available for TimerNr:
//                TIMER_5
//                TIMER_6
//
//----------------------------------------------------------------------------
// @Returnvalue   current 16-bit value of timer
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be readed
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:33
//
//****************************************************************************

#define GT2_uwReadTmr(TimerNr) TimerNr


//****************************************************************************
// @Macro          GT2_vLoadTmr(TimerNr,Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the selected GPT2 timer with the requested
//                value. The timer is not stopped. The access of the CPU to
//                the registers of the timers is of a higher priority than a
//                timer increment, a reload or a capture event.
//                The following definitions are available for TimerNr:
//                TIMER_5
//                TIMER_6
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be loaded
// @Parameters    16-bit value to be loaded
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:33
//
//****************************************************************************

#define GT2_vLoadTmr(TimerNr,Value) TimerNr = Value


//****************************************************************************
// @Macro          GT2_vCountUp(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro sets the counter direction of the selected timer
//                to up. The timer is not stopped.
//                Note:
//                Concerning timer x it is possible to make the counter
//                direction dependent on the level at TxEUD. If this
//                functionality has been selected, then function GT2_vCountUp
//                set a 0 at TxEUD enabling counting up of timer x and a
//                1 at TxEUD enabling counting down of timer x.
//                The following definitions are available for TimerNr:
//                TIMER_5
//                TIMER_6
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:33
//
//****************************************************************************

#define GT2_vCountUp(TimerNr) GT2_vCntUp(TimerNr,UD)
#define GT2_vCntUp(TimerNr,UD)  TimerNr##UD = 0                 


//****************************************************************************
// @Macro          GT2_vCountDown(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro sets the counter direction of the selected timer
//                to down. The timer is not stopped.
//                Note:
//                Concerning timer x it is possible to make the counter
//                direction dependent on the level at TxEUD. If this
//                functionality has been selected, then function GT2_vCountDown
//                set a 0 at TxEUD enablinf counting down of timer x and
//                a 1 at TxEUD enabling counting up of timer x.
//                The following definitions are available for TimerNr:
//                TIMER_5
//                TIMER_6
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:33
//
//****************************************************************************

#define GT2_vCountDown(TimerNr) GT2_vCntDown(TimerNr,UD)
#define GT2_vCntDown(TimerNr,UD)  TimerNr##UD = 1               


//****************************************************************************
// @Macro          GT2_uwReadCAPREL()
//
//----------------------------------------------------------------------------
// @Description   This macro returns the content of the CAPREL register.
//                This can be the capture value of T5 or the reload value of
//                T6.
//
//----------------------------------------------------------------------------
// @Returnvalue   current 16-bit value of the CAPREL register
//
//----------------------------------------------------------------------------
// @Parameters    none
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:33
//
//****************************************************************************

#define GT2_uwReadCAPREL()  CAPREL


//****************************************************************************
// @Macro          GT2_vLoadCAPREL(Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the CAPREL register with the forwarded
//                value.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    16-bit value to loaded in the CAPREL register
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:33
//
//****************************************************************************

#define GT2_vLoadCAPREL(Value) CAPREL = Value



// USER CODE BEGIN (Header,2)

// USER CODE END


/****************************************************************************
* @Defines                                                                   
****************************************************************************/

#define TIMER_5 T5
#define TIMER_6 T6


// USER CODE BEGIN (Header,3)

// USER CODE END


/****************************************************************************
* @Interrupt vectors                                                         
****************************************************************************/

#define T5INT   0x25
#define CRINT   0x27


// USER CODE BEGIN (Header,4)

// USER CODE END


