//****************************************************************************
// @Module        General Purpose Timer Unit 1
// @Filename      GT1.H
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all function prototypes and macros for
//                the GPT1 unit.
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:31
//
//****************************************************************************

//****************************************************************************
// @Prototypes of global functions
//****************************************************************************

void GT1_vInit(void);


// USER CODE BEGIN (Header,1)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Macro          GT1_vStartTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro starts the selected GPT1 timer. The timer continues
//                to count from where it had stopped
//                The following definitions are available for TimerNr:
//                TIMER_2
//                TIMER_3
//                TIMER_4
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be started
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:31
//
//****************************************************************************

#define GT1_vStartTmr(TimerNr)  GT1_vStart(TimerNr,R)
#define GT1_vStart(TimerNr,R) TimerNr##R = 1


//****************************************************************************
// @Macro          GT1_vStopTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro stops the selected GPT1 timer. The contents of
//                the timer register remain unchanged.
//                The following definitions are available for TimerNr:
//                TIMER_2
//                TIMER_3
//                TIMER_4
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be stopped
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:31
//
//****************************************************************************

#define GT1_vStopTmr(TimerNr)  GT1_vStop(TimerNr,R)
#define GT1_vStop(TimerNr,R) TimerNr##R = 0                     


//****************************************************************************
// @Macro          GT1_vClearTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro stops the selected GPT1 timer and sets the timer
//                register to 0.
//                The following definitions are available for TimerNr:
//                TIMER_2
//                TIMER_3
//                TIMER_4
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be cleared
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:31
//
//****************************************************************************

#define GT1_vClearTmr(TimerNr) GT1_vClrTmr(TimerNr,R)
#define GT1_vClrTmr(TimerNr,R) TimerNr##R = 0; TimerNr = 0x0000 


//****************************************************************************
// @Macro          GT1_uwReadTmr(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro returns the contents of the selected GPT1 timer.
//                The timer is not stopped. Access of the CPU to the
//                registers of the timer is of higher priority than a timer
//                increment, a reload or a capture event.
//                The following definitions are available for TimerNr:
//                TIMER_2
//                TIMER_3
//                TIMER_4
//
//----------------------------------------------------------------------------
// @Returnvalue   current 16-bit value of the timer
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be readed
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:31
//
//****************************************************************************

#define GT1_uwReadTmr(TimerNr) TimerNr


//****************************************************************************
// @Macro          GT1_vLoadTmr(TimerNr,Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the selected GPT1 timer with the forwarded
//                value. The timer is not stopped. The access of the CPU to
//                the registers of the timers is of higher priority than a
//                timer increment, a reload or a capture event.
//                The following definitions are available for TimerNr:
//                TIMER_2
//                TIMER_3
//                TIMER_4
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be loaded
// @Parameters    16-bit value to be loaded
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:31
//
//****************************************************************************

#define GT1_vLoadTmr(TimerNr,Value) TimerNr = Value


//****************************************************************************
// @Macro          GT1_vCountUp(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro sets the counter direction of the selected timer
//                to up. The timer is not stopped.
//                Note:
//                Concerning timer x it is possible to make the counter
//                direction dependent on the level at TxEUD. If this
//                functionality has been selected, then function GT1_vCountUp
//                set a  0 at TxEUD enabling counting up of timer x and a
//                1 at TxEUD enabling counting down of timer x.
//                The following definitions are available for TimerNr:
//                TIMER_2
//                TIMER_3
//                TIMER_4
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:31
//
//****************************************************************************

#define GT1_vCountUp(TimerNr) GT1_vCntUp(TimerNr,UD)
#define GT1_vCntUp(TimerNr,UD)  TimerNr##UD = 0                 


//****************************************************************************
// @Macro          GT1_vCountDown(TimerNr)
//
//----------------------------------------------------------------------------
// @Description   This macro sets the counter direction of the selected timer
//                to down. The timer is not stopped.
//                Note:
//                Concerning timer x it is possible to make the counter
//                direction dependent on the level at TxEUD. If this
//                functionality has been selected, then function GT1_vCountDown
//                set a 0 at TxEUD enabling counting down of timer x and
//                a 1 at TxEUD enabling counting up of timer x.
//                The following definitions are available for TimerNr:
//                TIMER_2
//                TIMER_3
//                TIMER_4
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    the name of the timer to be used
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:31
//
//****************************************************************************

#define GT1_vCountDown(TimerNr) GT1_vCntDown(TimerNr,UD)
#define GT1_vCntDown(TimerNr,UD)  TimerNr##UD = 1               



// USER CODE BEGIN (Header,2)

// USER CODE END


//***************************************************************************
// @Defines
//***************************************************************************

#define TIMER_2 T2
#define TIMER_3 T3
#define TIMER_4 T4


// USER CODE BEGIN (Header,3)

// USER CODE END


//***************************************************************************
// @Interrupt vectors
//***************************************************************************

#define T2INT   0x22
#define T3INT   0x23
#define T4INT   0x24


// USER CODE BEGIN (Header,4)

// USER CODE END


