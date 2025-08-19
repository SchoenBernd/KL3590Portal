//****************************************************************************
// @Module        Interrupt Controller
// @Filename      INT.H
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all function prototypes and macros for
//                the Interrupt Controller.
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:41
//
//****************************************************************************

//****************************************************************************
// @Prototypes of global functions
//****************************************************************************

void INT_vInit(void);


// USER CODE BEGIN (Header,1)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Macro          INT_vEnableInterrupt(IntName)
//
//----------------------------------------------------------------------------
// @Description   This macro enables the selected interrupt source.
//                Note:
//                Enable only those interrupt sources for which a service
//                routine is installed.
//                
//                Available definitions for IntName see in the header file
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of Interrupt (see @Defines in the header file)
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:41
//
//****************************************************************************

#define INT_vEnableInterrupt(IntName) IntName = 1


//****************************************************************************
// @Macro          INT_vDisableInterrupt(IntName)
//
//----------------------------------------------------------------------------
// @Description   This macro disables the selected interrupt source.
//                
//                Available definitions for IntName see in the header file
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Name of Interrupt (see @Defines in the header file)
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:41
//
//****************************************************************************

#define INT_vDisableInterrupt(IntName) IntName = 0


//****************************************************************************
// @Macro          INT_vSetPecCount(Nr,Value)
//
//----------------------------------------------------------------------------
// @Description   This macro loads the COUNT field of PECCx with the parameter
//                value.
//                During each PEC transfer this counter is decremented
//                (if 0xFF > COUNT > 0). If the counter reaches 0, the normal
//                interrupt service routine is executed, in which the PEC
//                counter then can be reloaded with this function. If COUNT
//                is set at 0xFF the PEC counter is switched off.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    number of PEC channel (0 to 7) whose count value is to be changed
// @Parameters    8-bit value for the PEC transfer counter field (0 .. 255)
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:41
//
//****************************************************************************

#define INT_vSetPecCount(Nr,Value) PECC##Nr = (PECC##Nr & 0xFF00) | Value


//****************************************************************************
// @Macro          INT_vSetPecSrcP(Nr,Address)
//
//----------------------------------------------------------------------------
// @Description   This macro sets the source pointer for the chosen PEC
//                channel.
//                Note:
//                The address of the variable must be in segment 0 (data
//                page 0 through 3).
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    number of PEC channel (0 to 7) whose source address is to be set
// @Parameters    source address of the chosen PEC channel
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:41
//
//****************************************************************************

#define INT_vSetPecSrcP(Nr,Address)   SRCP##Nr = _sof_(Address)


//****************************************************************************
// @Macro          INT_vSetPecDesP(Nr,Address)
//
//----------------------------------------------------------------------------
// @Description   This macro sets the destination pointer for the 
//                chosen PEC channel.
//                Note:
//                The address of the variable must be in segment 0 (data
//                page 0 through 3).
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    number of PEC channel (0 to 7) whose source address is to be set
// @Parameters    destination address of the chosen PEC channel
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:41
//
//****************************************************************************

#define INT_vSetPecDesP(Nr,Address)   DSTP##Nr = _sof_(Address)



// USER CODE BEGIN (Header,2)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// Defines for the parameter Name of the macro INT_vEnableInterrupt
// and INT_vDisableInterrupt

#define ADC_INT        ADCIE
#define ADC_ERROR_INT  ADEIE
#define ASC0_TX_INT    S0TIE
#define ASC0_TXBUF_INT S0TBIE
#define ASC0_RX_INT    S0RIE
#define ASC0_ERROR_INT S0EIE
#define CAN_INT        XP0IE
#define CAPREL_INT     CRIE
#define CAPCOM0_INT    CC0IE
#define CAPCOM1_INT    CC1IE
#define CAPCOM2_INT    CC2IE
#define CAPCOM3_INT    CC3IE
#define CAPCOM4_INT    CC4IE
#define CAPCOM5_INT    CC5IE
#define CAPCOM6_INT    CC6IE
#define CAPCOM7_INT    CC7IE
#define CAPCOM8_INT    CC8IE
#define CAPCOM9_INT    CC9IE
#define CAPCOM10_INT    CC10IE
#define CAPCOM11_INT    CC11IE
#define CAPCOM12_INT    CC12IE
#define CAPCOM13_INT    CC13IE
#define CAPCOM14_INT    CC14IE
#define CAPCOM15_INT    CC15IE
#define CAPCOM16_INT    CC16IE
#define CAPCOM17_INT    CC17IE
#define CAPCOM18_INT    CC18IE
#define CAPCOM19_INT    CC19IE
#define CAPCOM20_INT    CC20IE
#define CAPCOM21_INT    CC21IE
#define CAPCOM22_INT    CC22IE
#define CAPCOM23_INT    CC23IE
#define CAPCOM24_INT    CC24IE
#define CAPCOM25_INT    CC25IE
#define CAPCOM26_INT    CC26IE
#define CAPCOM27_INT    CC27IE
#define CAPCOM28_INT    CC28IE
#define CAPCOM29_INT    CC29IE
#define CAPCOM30_INT    CC30IE
#define CAPCOM31_INT    CC31IE
#define EXTERNAL_INT0  CC8IE
#define EXTERNAL_INT1  CC9IE
#define EXTERNAL_INT2  CC10IE
#define EXTERNAL_INT3  CC11IE
#define EXTERNAL_INT4  CC12IE
#define EXTERNAL_INT5  CC13IE
#define EXTERNAL_INT6  CC14IE
#define EXTERNAL_INT7  CC15IE
#define PWM_INT        PWMIE
#define PLL_INT        XP3IE
#define SSC_TX_INT     SSCTIE
#define SSC_RX_INT     SSCRIE
#define SSC_ERROR_INT  SSCEIE
#define TIMER0_INT     T0IE
#define TIMER1_INT     T1IE
#define TIMER2_INT     T2IE
#define TIMER3_INT     T3IE
#define TIMER4_INT     T4IE
#define TIMER5_INT     T5IE
#define TIMER6_INT     T6IE
#define TIMER7_INT     T7IE
#define TIMER8_INT     T8IE



// USER CODE BEGIN (Header,3)

// USER CODE END


/****************************************************************************
* @Interrupt vectors                                                         
****************************************************************************/

#define CC8INT    0x18
#define CC9INT    0x19
#define CC10INT   0x1A


// USER CODE BEGIN (Header,4)

// USER CODE END


