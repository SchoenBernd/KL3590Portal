//****************************************************************************
// @Module        PWM
// @Filename      PWM.H
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all function prototypes and macros for
//                the Pulse Width Modulation Module
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:28
//
//****************************************************************************

//****************************************************************************
// @Prototypes of global functions
//****************************************************************************

void PWM_vInit(void);
void PWM_vStatus(void);


// USER CODE BEGIN (PWMHeader,1)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Macro          PWM_vStart(Channel)
//
//----------------------------------------------------------------------------
// @Description   This macro starts the timer of the selected PWM channel.
//                The timer starts with the last content.
//                
//                The following definitions for Channel are available:
//                CHANNEL_0
//                CHANNEL_1
//                CHANNEL_2
//                CHANNEL_3
//                
//                Note:
//                To start all PWM channels simultaneously the macro is
//                used as follows:
//                PWM_vStart(CHANNEL_0 + CHANNEL_1..);
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the channel whose timer is to be started
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:28
//
//****************************************************************************

#define PWM_vStart(Channel) PWMCON0 |= Channel


//****************************************************************************
// @Macro          PWM_vStop(Channel)
//
//----------------------------------------------------------------------------
// @Description   This macro stops the timer of the selected PWM channel.
//                The timer content is not changed.
//                
//                The following definitions for Channel are available:
//                CHANNEL_0
//                CHANNEL_1
//                CHANNEL_2
//                CHANNEL_3
//                
//                Note:
//                To stop all PWM channels simultaneously the macro is
//                used as follows:
//                PWM_vStop(CHANNEL_0 + CHANNEL_1..);
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of the channel whose timer is to be stopped
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:29
//
//****************************************************************************

#define PWM_vStop(Channel) PWMCON0 &= ~(Channel)


//****************************************************************************
// @Macro          PWM_vSetPeriod(Channel,Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the given parameter into the period
//                register PPx of the selected PWM channel. This determines
//                the period of the PWM signal. The period shadow register
//                (with which the hardware compares) is actualized with the
//                new value when a new period begins, or upon a write 
//                access to PPx, while the timer is stopped.
//                
//                The following definitions for Channel are available:
//                PWM_REG0
//                PWM_REG1
//                PWM_REG2
//                PWM_REG3
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Channel whose period is to be changed
// @Parameters    16-bit Value to be loaded into the period register
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:29
//
//****************************************************************************

#define PWM_vSetPeriod(Channel,Value) PWM_Period(PP,Channel,Value)
#define PWM_Period(PP,Channel,Value) PP##Channel = Value


//****************************************************************************
// @Macro          PWM_vSetPw(Channel,Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the given parameter into the pulse width
//                register PWx of the selected PWM channel. This determines the
//                duty cycle of the PWM signal. The pulse width shadow 
//                register (with which the hardware compares) is only
//                actualized with the new value when a new period begins,
//                or upon a write access to PWx, while the timer is stopped.
//                
//                The following definitions for Channel are available:
//                PWM_REG0
//                PWM_REG1
//                PWM_REG2
//                PWM_REG3
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Channel whose duty cycle is to be changed
// @Parameters    16-bit Value to be loaded into the pulse width register
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:29
//
//****************************************************************************

#define PWM_vSetPw(Channel,Value) PWM_Pw(PW,Channel,Value)
#define PWM_Pw(PW,Channel,Value) PW##Channel = Value


//****************************************************************************
// @Macro          PWM_vSetTimer(Channel,Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the given parameter into the timer register
//                PTx of the selected PWM channel. This retriggers the output
//                signal in single shot mode.
//                
//                The following definitions for Channel are available:
//                PWM_REG0
//                PWM_REG1
//                PWM_REG2
//                PWM_REG3
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Channel whose timer is to be changed
// @Parameters    16-bit Value to be loaded into the timer register
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:29
//
//****************************************************************************

#define PWM_vSetTimer(Channel,Value) PWM_Timer(PT,Channel,Value)
#define PWM_Timer(PT,Channel,Value) PT##Channel = Value



// USER CODE BEGIN (PWMHeader,2)

// USER CODE END


/****************************************************************************
* @Defines                                                                   
****************************************************************************/

#define CHANNEL_0 1
#define CHANNEL_1 2
#define CHANNEL_2 4
#define CHANNEL_3 8

#define PWM_REG0 0
#define PWM_REG1 1
#define PWM_REG2 2
#define PWM_REG3 3


// USER CODE BEGIN (PWMHeader,3)

// USER CODE END


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************



// USER CODE BEGIN (PWMHeader,4)

// USER CODE END

