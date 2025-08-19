//****************************************************************************
// @Module        CAN
// @Filename      CAN.H
// @Project       Bs3
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all function prototypes and macros for CAN
//                
//
//----------------------------------------------------------------------------
// @Date          18.09.98 10:01:18
//
//****************************************************************************

//****************************************************************************
// @Definition of a structure for the CAN data
//****************************************************************************

// The following data type serves as a software message object. Each access to
// a hardware message object has to be made by forward a pointer to a software
// message object (TCAN_Obj). The data type has the following fields:
//
// ubMsgCfg:
// this byte has the same structure as the message configuration register of a
// hardware message object. It contains the "Data Lenght Code" (DLC), the "Extended
// Identifier" (XTD) and the "Message Direction" (DIR, read only access).
//
//         7     6     5      4    3     2     1     0
//      |-----------------------------------------------|
//      |        DLC            | DIR | XTD |  0  |  0  |
//      |-----------------------------------------------|
//
// ulArbitr: 
// this field is four bytes long and contains either the 11-bit identifier 
// or the 29-bit identifier (as a HEX-Value)
//
// ubData[8]:
// 8 bytes containing the data of a frame
//
// ubUser:
// this field serves for storing application specific data. It will not be
// sended or received.
//

typedef struct
  {
     ubyte ubMsgCfg;   // 8-bit Message Configuration Register
     ulong ulArbitr;   // standard (11-bit)/extended (29-bit) identifier
     ubyte ubData[8];  // 8-bit Data Bytes
     ubyte ubUser;     // for application specific states
  }TCAN_Obj;


//****************************************************************************
// @Prototypes of global functions
//****************************************************************************

void CAN_vInit(void);
void CAN_vGetMsgObj(ubyte ObjNr, TCAN_Obj *pstObj);
bit CAN_bRequestMsgObj(ubyte ObjNr);
bit CAN_bNewData(ubyte ObjNr);
void CAN_vTransmit(ubyte ObjNr);
void CAN_vConfigMsgObj(ubyte ObjNr, TCAN_Obj *pstObj);
void CAN_vLoadData(ubyte ObjNr, ubyte *pubBuffer);
bit CAN_bDelMsgObj(ubyte ObjNr);
bit CAN_bMsgLost(ubyte ObjNr);
void CAN_vReleaseObj(ubyte ObjNr);


// USER CODE BEGIN (CANHeader,1)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************


// USER CODE BEGIN (CANHeader,2)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// Control/Status Register
#define C1CSR   (*((uword volatile sdata *) 0xEF00))

// Interrupt Register
#define C1IR    (*((uword volatile sdata *) 0xEF02))

// Bit Timing Register
#define C1BTR   (*((uword volatile sdata *) 0xEF04))

// Global Mask Short
#define C1GMS   (*((uword volatile sdata *) 0xEF06))

// Upper Global Mask Long
#define C1UGML  (*((uword volatile sdata *) 0xEF08))

// Lower Global Mask Long
#define C1LGML  (*((uword volatile sdata *) 0xEF0A))

// Upper Mask of Last Message
#define C1UMLM  (*((uword volatile sdata *) 0xEF0C))

// Lower Mask of Last Message
#define C1LMLM  (*((uword volatile sdata *) 0xEF0E))


// Structure for a single CAN object
// A total of 15 such object structures exists (starting at EF10H)

struct can_obj 
{
  uword  MCR;       // Message Control Register
  uword  UAR;       // Upper Arbitration Register
  uword  LAR;       // Lower Arbitration Register
  ubyte  MCFG;      // Message Configuration Register
  ubyte  Data[8];   // Message Data 0 .. 7
  ubyte  Customer;  // Reserved for application specific data
};

#define CAN_OBJ ((struct can_obj volatile sdata *) 0xEF10)


// USER CODE BEGIN (CANHeader,3)

// USER CODE END


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************



// USER CODE BEGIN (CANHeader,4)

// USER CODE END

