//****************************************************************************
// @Module        CAN
// @Filename      CANUTL.C
// @Project       cantest
//----------------------------------------------------------------------------
// @Controller    Siemens C167CR-L
//
// @Compiler      Keil C166 V3.00
//
// @Codegenerator DAvE 1-0-0
//
// @Description   This file contains all functions that use the CAN peripheral
//                
//
//----------------------------------------------------------------------------
// @Date          28.04.98 14:38:49
//
//****************************************************************************

//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAINBS3.H"


//****************************************************************************
// @Function      void CAN_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This function initializes the CAN component. It effects all
//                necessary configurations of the SFR, depending on the selected
//                operating mode. The configuration determines whether the CAN
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
// @Date          28.04.98 14:38:49
//
//****************************************************************************

void CAN_vInit(void)
{

  //  ------------ CAN Control/Status Register --------------
  //  start the initialization of the CAN Modul
  C1CSR  = 0x0041;  // set INIT and CCE

  ///  ------------ Bit Timing Register ---------------------
  /// baudrate =  370,370 KBaud
  /// there are 5 time quanta before sample point
  /// there are 3 time quanta after sample point
  /// the (re)synchronization jump width is 2 time quanta
  C1BTR  = 0x2442;  // set Bit Timing Register

  C1GMS  = 0xE0FF;  // set Global Mask Short Register

  C1UGML = 0xFFFF;  // set Upper Global Mask Long Register

  C1LGML = 0xF8FF;  // set Lower Global Mask Long Register


  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 1 ---------------------------
  //  ------------------------------------------------------------------------
  /// Message object 1 is valid
  CAN_OBJ[0].MCR  = 0x5695;    // set Message Control Register

  /// message direction is transmit
  /// standard 11-bit identifier
  /// 2 valid data bytes
  CAN_OBJ[0].MCFG = 0x28;      // set Message Configuration Register

  CAN_OBJ[0].UAR  = 0x6026;    // set Upper Arbitration Register
  CAN_OBJ[0].LAR  = 0x0000;    // set Lower Arbitration Register

  CAN_OBJ[0].Data[0] = 0x00;   // set data byte 0
  CAN_OBJ[0].Data[1] = 0x00;   // set data byte 1


  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 2 ---------------------------
  //  ------------------------------------------------------------------------
  /// Message object 2 is valid
  CAN_OBJ[1].MCR  = 0x5695;    // set Message Control Register

  /// message direction is transmit
  /// standard 11-bit identifier
  /// 2 valid data bytes
  CAN_OBJ[1].MCFG = 0x28;      // set Message Configuration Register

  CAN_OBJ[1].UAR  = 0x8026;    // set Upper Arbitration Register
  CAN_OBJ[1].LAR  = 0x0000;    // set Lower Arbitration Register

  CAN_OBJ[1].Data[0] = 0x00;   // set data byte 0
  CAN_OBJ[1].Data[1] = 0x00;   // set data byte 1


  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 3 ---------------------------
  //  ------------------------------------------------------------------------
  /// Message object 3 is valid
  CAN_OBJ[2].MCR  = 0x5695;    // set Message Control Register

  /// message direction is transmit
  /// standard 11-bit identifier
  /// 2 valid data bytes
  CAN_OBJ[2].MCFG = 0x28;      // set Message Configuration Register

  CAN_OBJ[2].UAR  = 0xA026;    // set Upper Arbitration Register
  CAN_OBJ[2].LAR  = 0x0000;    // set Lower Arbitration Register

  CAN_OBJ[2].Data[0] = 0x00;   // set data byte 0
  CAN_OBJ[2].Data[1] = 0x00;   // set data byte 1


  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 4 ---------------------------
  //  ------------------------------------------------------------------------
  /// Message object 4 is valid
  CAN_OBJ[3].MCR  = 0x5695;    // set Message Control Register

  /// message direction is transmit
  /// standard 11-bit identifier
  /// 2 valid data bytes
  CAN_OBJ[3].MCFG = 0x28;      // set Message Configuration Register

  CAN_OBJ[3].UAR  = 0xC026;    // set Upper Arbitration Register
  CAN_OBJ[3].LAR  = 0x0000;    // set Lower Arbitration Register

  CAN_OBJ[3].Data[0] = 0x00;   // set data byte 0
  CAN_OBJ[3].Data[1] = 0x00;   // set data byte 1


  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 5 ---------------------------
  //  ------------------------------------------------------------------------
  /// Message object 5 is valid
  CAN_OBJ[4].MCR  = 0x5695;    // set Message Control Register

  /// message direction is transmit
  /// standard 11-bit identifier
  /// 2 valid data bytes
  CAN_OBJ[4].MCFG = 0x28;      // set Message Configuration Register

  CAN_OBJ[4].UAR  = 0xE026;    // set Upper Arbitration Register
  CAN_OBJ[4].LAR  = 0x0000;    // set Lower Arbitration Register

  CAN_OBJ[4].Data[0] = 0x00;   // set data byte 0
  CAN_OBJ[4].Data[1] = 0x00;   // set data byte 1


  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 6 ---------------------------
  //  ------------------------------------------------------------------------
  /// Message object 6 is valid
  CAN_OBJ[5].MCR  = 0x5595;    // set Message Control Register

  /// message direction is receive
  /// standard 11-bit identifier
  CAN_OBJ[5].MCFG = 0x00;      // set Message Configuration Register

  CAN_OBJ[5].UAR  = 0x6006;    // set Upper Arbitration Register
  CAN_OBJ[5].LAR  = 0x0000;    // set Lower Arbitration Register



  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 7 ---------------------------
  //  ------------------------------------------------------------------------
  /// Message object 7 is valid
  CAN_OBJ[6].MCR  = 0x5595;    // set Message Control Register

  /// message direction is receive
  /// standard 11-bit identifier
  CAN_OBJ[6].MCFG = 0x00;      // set Message Configuration Register

  CAN_OBJ[6].UAR  = 0x8006;    // set Upper Arbitration Register
  CAN_OBJ[6].LAR  = 0x0000;    // set Lower Arbitration Register



  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 8 ---------------------------
  //  ------------------------------------------------------------------------
  /// Message object 8 is valid
  CAN_OBJ[7].MCR  = 0x5595;    // set Message Control Register

  /// message direction is receive
  /// standard 11-bit identifier
  CAN_OBJ[7].MCFG = 0x00;      // set Message Configuration Register

  CAN_OBJ[7].UAR  = 0xA006;    // set Upper Arbitration Register
  CAN_OBJ[7].LAR  = 0x0000;    // set Lower Arbitration Register



  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 9 ---------------------------
  //  ------------------------------------------------------------------------
  /// Message object 9 is valid
  CAN_OBJ[8].MCR  = 0x5595;    // set Message Control Register

  /// message direction is receive
  /// standard 11-bit identifier
  CAN_OBJ[8].MCFG = 0x00;      // set Message Configuration Register

  CAN_OBJ[8].UAR  = 0xC006;    // set Upper Arbitration Register
  CAN_OBJ[8].LAR  = 0x0000;    // set Lower Arbitration Register



  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 10 ---------------------------
  //  ------------------------------------------------------------------------
  /// Message object 10 is valid
  CAN_OBJ[9].MCR  = 0x5595;    // set Message Control Register

  /// message direction is receive
  /// standard 11-bit identifier
  CAN_OBJ[9].MCFG = 0x00;      // set Message Configuration Register

  CAN_OBJ[9].UAR  = 0xE006;    // set Upper Arbitration Register
  CAN_OBJ[9].LAR  = 0x0000;    // set Lower Arbitration Register



  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 11 ---------------------------
  //  ------------------------------------------------------------------------
  /// Message object 11 is valid
  CAN_OBJ[10].MCR  = 0x5695;    // set Message Control Register

  /// message direction is transmit
  /// standard 11-bit identifier
  /// 6 valid data bytes
  CAN_OBJ[10].MCFG = 0x68;      // set Message Configuration Register

  CAN_OBJ[10].UAR  = 0x4046;    // set Upper Arbitration Register
  CAN_OBJ[10].LAR  = 0x0000;    // set Lower Arbitration Register

  CAN_OBJ[10].Data[0] = 0x00;   // set data byte 0
  CAN_OBJ[10].Data[1] = 0x00;   // set data byte 1
  CAN_OBJ[10].Data[2] = 0x00;   // set data byte 2
  CAN_OBJ[10].Data[3] = 0x00;   // set data byte 3
  CAN_OBJ[10].Data[4] = 0x00;   // set data byte 4
  CAN_OBJ[10].Data[5] = 0x00;   // set data byte 5


  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 12 ---------------------------
  //  ------------------------------------------------------------------------
  /// message object 12 is not valid
  CAN_OBJ[11].MCR  = 0x5555;    // set Message Control Register


  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 13 ---------------------------
  //  ------------------------------------------------------------------------
  /// message object 13 is not valid
  CAN_OBJ[12].MCR  = 0x5555;    // set Message Control Register


  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 14 ---------------------------
  //  ------------------------------------------------------------------------
  /// message object 14 is not valid
  CAN_OBJ[13].MCR  = 0x5555;    // set Message Control Register


  //  ------------------------------------------------------------------------
  /// ----------------- Configure Message Object 15 ---------------------------
  //  ------------------------------------------------------------------------
  /// message object 15 is not valid
  CAN_OBJ[14].MCR  = 0x5555;    // set Message Control Register


  /// ------------ CAN Control/Status Register --------------
  //  reset CCE and INIT
  C1CSR = 0x0000;


  // USER CODE BEGIN (CAN_Init,1)

  // USER CODE END

}


//****************************************************************************
// @Function      void CAN_vGetMsgObj(ubyte ObjNr, TCAN_Obj *pstObj)
//
//----------------------------------------------------------------------------
// @Description   This function fills the forwarded SW message object with the 
//                content of the chosen HW message object.
//                
//                The structure of the SW message object is defined in the 
//                header file CAN.H (see TCAN_Obj).
//
//----------------------------------------------------------------------------
// @Returnvalue   0 - no data or error,   1 - new data read
//
//----------------------------------------------------------------------------
// @Parameters    Number of the message object to be read (1-15)
// @Parameters    Pointer on a message object to be filled by this function.
//
//----------------------------------------------------------------------------
// @Date          28.04.98 14:38:49
//
//****************************************************************************

bit CAN_vGetMsgObj(ubyte ObjNr, TCAN_Obj *pstObj)
{
  ubyte i;
  ulong v;
  unsigned char tries=0;

  if((CAN_OBJ[ObjNr-1].MCR & 0x0300) == 0x0100) return 0;  // if not NEWDAT
  do
  {
     CAN_OBJ[ObjNr-1].MCR = 0xf5ff;       // reset NEWDAT, reset MSGLST
     pstObj->ubMsgCfg = CAN_OBJ[ObjNr-1].MCFG;

     v = 0;
     if(CAN_OBJ[ObjNr-1].MCFG & 0x04)  // extended identifier
     {
       v += (CAN_OBJ[ObjNr-1].LAR & 0xf800) >> 11;  // ID  4.. 0
       v += (CAN_OBJ[ObjNr-1].LAR & 0x00ff) <<  5;  // ID 12.. 5
       v += (CAN_OBJ[ObjNr-1].UAR & 0xff00) <<  5;  // ID 13..20
       v += (CAN_OBJ[ObjNr-1].UAR & 0x00ff) << 21;  // ID 21..28
     }
     else                              // standard identifier 
     {
       v += (CAN_OBJ[ObjNr-1].UAR & 0xe000) >> 13;  // ID 18..20
       v += (CAN_OBJ[ObjNr-1].UAR & 0x00ff) <<  3;  // ID 21..28
     }
     pstObj->ulArbitr = v;

     for(i = 0; i < (CAN_OBJ[ObjNr-1].MCFG & 0xf0) >> 4; i++)
     {
       pstObj->ubData[i] = CAN_OBJ[ObjNr-1].Data[i];
     }

     pstObj->ubUser = CAN_OBJ[ObjNr-1].Customer;
     
     if (++tries > 2) return 0;
     
  } while ((CAN_OBJ[ObjNr-1].MCR & 0x0300) == 0x0200);     // while NEWDAT
  
  return 1;
}


//****************************************************************************
// @Function      bit CAN_bRequestMsgObj(ubyte ObjNr)
//
//----------------------------------------------------------------------------
// @Description   If a TRANSMIT OBJECT is to be reconfigured it must first be
//                accessed. The access to the transmit object is exclusive.
//                This function checks whether the choosen message object is
//                still executing a transmit request, or if the object can 
//                be accessed exclusively.
//                After the message object is reserved, it can be reconfigured
//                by using the function CAN_vConfigMsgObj or CAN_vLoadData.
//                Both functions enable access to the object for the CAN
//                controller.
//                By calling the function CAN_vTransmit transfering of data
//                is started.
//
//----------------------------------------------------------------------------
// @Returnvalue   0 message object is busy (a transfer is actice), else 1
//
//----------------------------------------------------------------------------
// @Parameters    Number of the message object (1-14)
//
//----------------------------------------------------------------------------
// @Date          28.04.98 14:38:49
//
//****************************************************************************

bit CAN_bRequestMsgObj(ubyte ObjNr)
{
  bit bReturn;

  bReturn = 0;
  if((CAN_OBJ[ObjNr-1].MCR & 0x3000) == 0x1000)  // if reset TXRQ 
  {
    CAN_OBJ[ObjNr-1].MCR = 0xfbff;               // set CPUUPD 
    bReturn = 1;
  }
  return(bReturn);
}


//****************************************************************************
// @Function      bit CAN_bNewData(ubyte ObjNr)
//
//----------------------------------------------------------------------------
// @Description   This function checks whether the selected RECEIVE OBJECT has 
//                received a new message. If so the function returns the 
//                value "1". 
//
//----------------------------------------------------------------------------
// @Returnvalue   1 the message object has received a new message, else 0
//
//----------------------------------------------------------------------------
// @Parameters    Number of the message object (1-15)
//
//----------------------------------------------------------------------------
// @Date          28.04.98 14:38:49
//
//****************************************************************************

bit CAN_bNewData(ubyte ObjNr)
{
  bit bReturn;

  bReturn = 0;
  if((CAN_OBJ[ObjNr-1].MCR & 0x0300) == 0x0200)  // if NEWDAT
  {
    bReturn = 1;
  }
  return(bReturn);
}


//****************************************************************************
// @Function      void CAN_vTransmit(ubyte ObjNr)
//
//----------------------------------------------------------------------------
// @Description   This function triggers the CAN controller to send the 
//                selected message.
//                If the selected message object is a TRANSMIT OBJECT then 
//                this function triggers the sending of a data frame.
//                If however the selected message object is a RECEIVE OBJECT
//                this function triggers the sending of a remote frame.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Number of the message object to be sent (1-14)
//
//----------------------------------------------------------------------------
// @Date          28.04.98 14:38:49
//
//****************************************************************************

void CAN_vTransmit(ubyte ObjNr)
{
  CAN_OBJ[ObjNr-1].MCR = 0xe7ff;  // set TXRQ,reset CPUUPD
}


//****************************************************************************
// @Function      void CAN_vConfigMsgObj(ubyte ObjNr, TCAN_Obj *pstObj)
//
//----------------------------------------------------------------------------
// @Description   This function sets up the message objects. This includes
//                the 8 data bytes, the identifier (11- or 29-bit), the data 
//                number (0-7 bytes) and the XTD-bit.
//                The message is not sent; for this the function
//                CAN_vTransmit must be called.
//                
//                The structure of the SW message object is defined in the 
//                header file CAN.H (see TCAN_Obj).
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Number of the message object to be configured (1-15)
// @Parameters    Pointer on a message object
//
//----------------------------------------------------------------------------
// @Date          28.04.98 14:38:49
//
//****************************************************************************

void CAN_vConfigMsgObj(ubyte ObjNr, TCAN_Obj *pstObj)
{
  ubyte i;
  ulong v;

  CAN_OBJ[ObjNr-1].MCR = 0xfb7f;     // set CPUUPD, reset MSGVAL

  if(pstObj->ubMsgCfg & 0x04)        // extended identifier
  {
    v = 0x00000000;
    v += (pstObj->ulArbitr & 0x0000001f) << 11;  // ID  4.. 0
    v += (pstObj->ulArbitr & 0x00001fe0) >>  5;  // ID 12.. 5
    CAN_OBJ[ObjNr-1].LAR  = v;

    v = 0x00000000;
    v += (pstObj->ulArbitr & 0x001fe000) >>  5;  // ID 13..20
    v += (pstObj->ulArbitr & 0x1fe00000) >> 21;  // ID 21..28
    CAN_OBJ[ObjNr-1].UAR  = v;
  }
  else                               // standard identifier
  {
    CAN_OBJ[ObjNr-1].LAR  = 0x00000000;

    v = 0x00000000;
    v += (pstObj->ulArbitr & 0x00000007) << 13;  // ID 18..20
    v += (pstObj->ulArbitr & 0x000007f8) >>  3;  // ID 21..28
    CAN_OBJ[ObjNr-1].UAR  = v;
  }

  if(CAN_OBJ[ObjNr-1].MCFG & 0x08)   // if transmit direction
  {
    CAN_OBJ[ObjNr-1].MCFG = pstObj->ubMsgCfg | 0x08;

    for(i = 0; i < (pstObj->ubMsgCfg & 0xf0) >> 4;i++)
    {
      CAN_OBJ[ObjNr-1].Data[i] = pstObj->ubData[i];
    }
    CAN_OBJ[ObjNr-1].MCR  = 0xf6bf;  // set NEWDAT, reset CPUUPD, set MSGVAL
  }
  else                               // if receive direction
  {
    CAN_OBJ[ObjNr-1].MCFG = pstObj->ubMsgCfg & 0xf7;

    CAN_OBJ[ObjNr-1].MCR  = 0xf7bf;  // reset CPUUPD, set MSGVAL
  }

  CAN_OBJ[ObjNr-1].Customer = pstObj->ubUser;
}


//****************************************************************************
// @Function      void CAN_vLoadData(ubyte ObjNr, ubyte *pubData)
//
//----------------------------------------------------------------------------
// @Description   If a harware TRANSMIT OBJECT has to be loaded with data
//                but not with a new identifier, this function may be used
//                instead of the function CAN_vConfigMsgObj.
//                The message object should be accessed by calling the function
//                CAN_bRequestMsgObj before calling this function. This
//                prevents the CAN controller from working with invalid data.
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Number of the message object to be configured (1-15)
// @Parameters    Pointer on a data buffer
//
//----------------------------------------------------------------------------
// @Date          28.04.98 14:38:49
//
//****************************************************************************

void CAN_vLoadData(ubyte ObjNr, ubyte *pubData)
{
  ubyte i;

  CAN_OBJ[ObjNr-1].MCR = 0xfaff;  // set CPUUPD and NEWDAT

  for(i = 0; i < (CAN_OBJ[ObjNr-1].MCFG & 0xf0) >> 4; i++)
  {
    CAN_OBJ[ObjNr-1].Data[i] = *(pubData++);
  }

  CAN_OBJ[ObjNr-1].MCR = 0xf7ff;  // reset CPUUPD
}


//****************************************************************************
// @Function      bit CAN_bMsgLost(ubyte ObjNr)
//
//----------------------------------------------------------------------------
// @Description   If a RECEIVE OBJECT receives new data before the old object
//                has been read, the old object is lost. The CAN controller
//                indicates this by setting the message lost bit (MSGLST).
//                This function returns the status of this bit.
//                
//                Note:
//                This function resets the message lost bit (MSGLST).
//
//----------------------------------------------------------------------------
// @Returnvalue   1 the message object has lost a message, else 0
//
//----------------------------------------------------------------------------
// @Parameters    Number of the message object (1-15)
//
//----------------------------------------------------------------------------
// @Date          28.04.98 14:38:49
//
//****************************************************************************

bit CAN_bMsgLost(ubyte ObjNr)
{
  bit bReturn;

  bReturn = 0;
  if((CAN_OBJ[ObjNr-1].MCR & 0x0c00) == 0x0800)  // if set MSGLST 
  {
    CAN_OBJ[ObjNr-1].MCR = 0xf7ff;               // reset MSGLST 
    bReturn = 1;
  }
  return(bReturn);
}


//****************************************************************************
// @Function      bit CAN_bDelMsgObj(ubyte ObjNr)
//
//----------------------------------------------------------------------------
// @Description   This function marks the selected message object as not valid.
//                This means that this object cannot be sent or received.
//                If the selected object is busy (meaning the object is
//                transmitting a message or has received a new message) this
//                function returns the value "0" and the object is not deleted.
//
//----------------------------------------------------------------------------
// @Returnvalue   1 the message object was deleted, else 0
//
//----------------------------------------------------------------------------
// @Parameters    Number of the message object (1-15)
//
//----------------------------------------------------------------------------
// @Date          28.04.98 14:38:49
//
//****************************************************************************

bit CAN_bDelMsgObj(ubyte ObjNr)
{
  bit bReturn;

  bReturn = 0;
  if(!(CAN_OBJ[ObjNr-1].MCR & 0xa200)) // if set RMTPND, TXRQ or NEWDAT
  {
    CAN_OBJ[ObjNr-1].MCR = 0xff7f;     // reset MSGVAL
    bReturn = 1;
  }
  return(bReturn);
}


//****************************************************************************
// @Function      void CAN_vReleaseObj(ubyte ObjNr)
//
//----------------------------------------------------------------------------
// @Description   This function resets the NEWDAT flag of the selected RECEIVE
//                OBJECT, so that the CAN controller have access to it.
//                This function must be called if the function CAN_bNewData
//                detects, that new data are present in the message object and
//                the actual data have been read by calling the function
//                CAN_vGetMsgObj. 
//
//----------------------------------------------------------------------------
// @Returnvalue   none
//
//----------------------------------------------------------------------------
// @Parameters    Number of the message object (1-15)
//
//----------------------------------------------------------------------------
// @Date          28.04.98 14:38:49
//
//****************************************************************************

void CAN_vReleaseObj(ubyte ObjNr)
{
  CAN_OBJ[ObjNr-1].MCR = 0xfdff;     // reset NEWDAT
}




// USER CODE BEGIN (CAN_General,3)

// USER CODE END


