/**************************************************************************/
/*                                                                        */
/*  Module:  RTCom                                 Copyright (c) 1989,94  */
/*  Version: 4.5                                 On Time Informatik GmbH  */
/*                                                                        */
/*                                                                        */
/*                                      On Time        /úúúúúúúúúúú/ÄÄÄÄÄ */
/*                                    Informatik GmbH /úúúúúúúúúúú/       */
/* ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ/úúúúúúúúúúú/        */
/*                                  Echtzeit- und Systemsoftware          */
/*                                                                        */
/**************************************************************************/

/* Module to send and receive data on serial ports using interrupts or
   polling.

   In interrupt-driven mode, data is buffered up to a user-defined limit
   before data is lost. In polling mode, no data is buffered. If an 16550A
   UART chip is used by a serial port, its internal 16-byte buffer is used.

   If interrupt-driven communication is desired on any "standard" port
   (i.e., a port not on a Hostess or DigiBoard card), the following steps
   should be taken (after RTComInit has been called):

   1. Call function PortInstalled to check that the port is available.

   2. Initialise the port using function InitPort.

   3. If you need a protocol, call SetProtocol.

   4. Call EnableCOMInterrupt. All incoming data is put into
      the mailbox ReceiveBuffer[x], where x is the corresponding port. The
      mailboxes contain records of type COMData containing the actual
      data received and an error status byte.

   5. Tasks wishing to receive data should perform RTKGet, RTKGetCond,
      or RTKGetTimed on the corresponding mailboxes. Tasks wishing to Send
      should use SendChar. SendChar places the data in a send buffer
      which is emptied by an interrupt handler.

   Function EnableCOMInterrupt allocates both receive and send buffers with
   the same size. If you need different sizes, call AllocateCOMBuffers first,
   EnableCOMInterrupt will not allocate any buffers again.

   If polled communication is desired, the following steps should be taken:

   1. Initialise the port using InitPort.

   2. To receive, call ReceiveCharPolled.

   3. To send data, use SendCharPolled.


   Handshake protocols:
   --------------------

   By default, RTCom uses no protocol to prevent overflow errors. In this
   mode, it is assumed, that all communication participants are ready to
   accept data. If a ReceiveBuffer of RTCom flows over, the error
   "BufferFullError" is reported.

   In interrupt driven mode, RTCom also supports the protocols XOn/XOff,
   RTS/CTS, and DTR/DSR. These protocols can prevent buffer overflows.
   A receiver checks after every byte received, whether the receive buffer
   has filled up to a dangerous point. If so, the sender is informed to stop
   sending. As soon as the receive buffer has been emptied to a safe level,
   the sender is allowed to continue sending.

   The XOn/XOff protocol sends special ASCII characters over the line to
   send the necessary information to the sender. It can only be used, if
   the actual data stream never contains XOn/XOff characters. Protocols
   RTS/CTS and DTR/DSR use handshake lines. They require, however, that the
   corresponding lines are actually connected in the cable used.

   RTCom supports these protocols in "Passive" and in "Active" modes. In
   Passive mode, RTCom will understand and adhere to XOn/XOff characters
   respective CTS or DSR signals. This way, a buffer overflow error can be
   prevented on the remote device. The local receive buffer can still
   overflow, if the receiving task does not collect the data fast enough.
   In Active mode, RTCom will also protect its own receive buffer. If a
   buffer fills to over 70% of its size, an XOff is sent to the remote device
   (respectively, signal RTS or DTR is reset). The remote device should then
   stop sending. A protocol task in RTCom will then periodically check, if
   the buffer is filled to less than 30% of its size. If so, the remote
   device is allowed to continue to send (by sending an XOn to it or by
   setting the RTS or DTR signal).

   Please note that RTCom's performance degrades, if protocols are used.
   Passive protocols are faster than active protocols. No protocol is faster
   than passive protocols. Protocols should be defined using function
   SetProtocol after the respective port is initialized.


   Hardware configuration:
   -----------------------

   This module expects the following IRQ settings for COM1 to COM4:

      COM1  ->  IRQ4
      COM2  ->  IRQ3
      COM3  ->  IRQ4
      COM4  ->  IRQ3

   Please make sure that your serial I/O board actually supports interrupt
   sharing. If it does not, you cannot use COM1 and COM3 or COM2 and COM4
   at the same time in interrupt driven mode. SetIRQ may be used to define
   different IRQs for ports COM1 .. COM4. Interrupt sharing can be defined
   on any IRQ, however, only for the port pairs COM1/COM3 and COM2/COM4.
   Best performance is achieved, if every port has its own IRQ.

   Port addresses of the UART chips for COM1 to COM4 are taken from the
   BIOS data area at address 0x0040:0x0000. If they are not found here,
   use SetIOBase to define them.


   DigiBoard cards (PC/4, PC/8, PC/16):
   ------------------------------------------
   Before you can use one or more DigiBoard cards, you have to inform RTCom
   about the card type, the status register address, the interrupt
   request to be used, and which port number you would like to assign to
   the first port on the board. Please note that the card(s) must be
   configured to use only one interrupt request. If you have several cards
   installed they should all use the same address for the status register.
   For example, if you have set the status register to 0x140 and the IRQ to 7
   and you would like the ports on your card to start at COM5,
   you should include the following statement in your program before the
   card can be used:

      SetCOMBoardTyp(DigiBoard, 0x140, 7, 4);

   Subsequently, the I/O-address of each port must be defined individually using
   SetIOBase. If you have a card with 4 ports and you have left the
   I/O-addresses at the factory settings, the following statements would
   be required:

      SetIOBase(4, 0x100);
      SetIOBase(5, 0x108);
      SetIOBase(6, 0x110);
      SetIOBase(7, 0x118);

   After this initialisation, ports 4 through 7 can be used as explained
   above. Both interrupt-driven and polling modes are supported.

   Hostess cards (4, 8, or 16 ports):
   --------------
   Before you can use a Hostess card, you have to inform RTCom about the
   card type, its base address, the interrupt request to be used, and which
   port number you would like to assign to the first port on the board.
   Cards with 4 or 8 ports are identified with AddOnBoardType = Hostess.
   Cards with 16 ports have AdOnBoardType = Hostess16. For example, if your
   board with 4 ports is installed at address 0x280 and uses IRQ 7, and you
   already have COM1 and COM2 on your motherboard, then you should include
   the following statement in your program before the card can be used:

      SetCOMBoardTyp(Hostess, 0x280, 7, 2);

   Then, the I/O-address of each port must be defined individually using
   SetIOBase. Using the example above, the following statements would be
   required:

      SetIOBase(2, 0x280);
      SetIOBase(3, 0x288);
      SetIOBase(4, 0x290);
      SetIOBase(5, 0x298);

   After this initialisation, ports 2 through 5 (COM3 .. COM6) can be used
   as explained above. Both interrupt-driven and polling modes are supported.

   Other interrupt sharing cards (2 to 32 ports):
   ----------------------------------------------
   DigiBoard and Hostess cards have a special status register which can
   identify the port which has generated an interrupt. If your I/O card does
   not have such a register, you can use the 'GenericCOMBoard' type. It
   checks all ports on the card on every interrupt. Before you can use a
   generic card, you have to inform RTCom about the card type, the interrupt
   request to be used, which port number you would like to assign to the
   first port on the board, and how many ports there are in the board.
   For example, if your board with 4 ports uses IRQ 7 and you
   already have COM1 and COM2 on your motherboard, then you should include
   the following statement in your program before the card can be used:

      SetCOMBoardTyp(GenericCOMBoard, 4, 7, 2);

   Then, the I/O-address of each port must be defined individually using
   SetIOBase. Assuming that the first port uses I/O address 0x280, the
   following statements would be required:

      SetIOBase(2, 0x280);
      SetIOBase(3, 0x288);
      SetIOBase(4, 0x290);
      SetIOBase(5, 0x298);

   After this initialisation, ports 2 through 5 can be used as discribed
   above. Both interrupt-driven and polling modes are supported.

   Please not that DigiBoards and Hostess card show better performance than
   'generic' cards due to their hardware support for interrupt sharing.

   Further details about this module are gievn with each function prototype.
   See program COMDemo for a demonstration of how to use RTCom.

*/

#ifndef _RTCOM_H
#define _RTCOM_H

#define MINBAUD      50l
#define MAXBAUD      115200l
#define DEFAULT_TRIGGER_LEVEL 8 /* for 16550 UARTs          */

#define COM1         0        /* serial ports supported     */
#define COM2         1
#define COM3         2
#define COM4         3
#define COM5         4
#define COM6         5
/* etc. */
#define MAX_PORTS    38

/* status masks, several bits may be set simultaneously     */
/* status information is returned by LineStatus and as      */
/* the high byte of COMData                                 */

#define DATA_READY   0x01     /* not an error               */
#define OVERRUN      0x02     /* error detected by hardware */
#define PARITY       0x04     /* error detected by hardware */
#define FRAME        0x08     /* error detected by hardware */
#define BREAK        0x10     /* not an error               */
#define BUFFER_FULL  0x80     /* error detected by software */
#define TXB_EMPTY    0x20     /* not an error               */
#define TX_SHIFT_EMPTY 0x40   /* dito                       */
#define HARD_ERROR   (OVERRUN | PARITY | FRAME | BREAK)

/* status masks returned by ModemStatus                     */
#define DCTS         0x01     /* Delta Clear To Send        */
#define DDSR         0x02     /* Delta Data Set Ready       */
#define DRI          0x04     /* Delta Ring Indicator       */
#define DDCD         0x08     /* Delta Data Carrier Det.    */
#define CTS          0x10     /* Clear To Send              */
#define DSR          0x20     /* Data Set Ready             */
#define RI           0x40     /* Ring Indicator             */
#define DCD          0x80     /* Data Carrier Detected      */

/* bit masks which may be written to the MCR using ModemControl */
#define DTR          0x01     /* Data Terminal Ready        */
#define RTS          0x02     /* Request To Send            */
#define Out1         0x04
#define Out2         0x08
#define LoopBack     0x10

/* Protocol priority for passive protocols                  */
#define Passive      (MIN_PRIO - 1)

/* Default base addresses of COM1..COM4 (not used by RTCOM) */
/* unsigned short DefaultIOBase[4] = {0x3F8, 0x2F8, 0x3E8, 0x2E8} */

/* boards supported for COM5..COM38                         */
typedef enum { NoAddOnBoard, GenericCOMBoard, DigiBoard, Hostess, Hostess16 } AddOnPortType;

/* protocols supported                                      */
typedef enum { NoProtocol, XOnXOff, RTSCTS, DTRDSR } Protocol;

#define PARITY_NONE  0        /* for port initialisation    */
#define PARITY_ODD   1
#define PARITY_EVEN  2

typedef unsigned short COMData; /* low byte = data; high byte = errors */
typedef unsigned char  Byte;

#ifdef __cplusplus
extern "C" {
#endif

extern Mailbox SendBuffer[];     /* Don't supply range! (incorrect code with MS-C 8.0) */
   /* mailbox cotaining data to be sent out. Do not access directly! */

extern Mailbox ReceiveBuffer[];  /* Don't supply range! (incorrect code with MS-C 8.0) */
   /* mailboxes containing the data received by interrupt */

void RTComInit(void);
   /* Initialises this module */

int  PortInstalled(int Port);
   /* Checks whether the specified port is available. If the port is
      installed but PortInstalled returns 0, use SetIOBase to define
      the base address of the port. */

void InitPort(int  Port,
              long BaudRate,
              int  Parity,
              int  StopBits,
              int  WordLength);
   /* Sets port parameters and disables interrupts. If a 16550a UART is
      detected, its FIFO is automatically enabled at trigger level 8.
   */

void SetProtocol(int      Port,
                 Protocol Prot,
                 unsigned ProtTaskPrio,
                 Duration PollCycle);
   /* Defines the protocol to be used for a specific port. If this function
      is never called for a specific port, no protocol is assumed. If the
      value supplied for ProtTaskPrio is unequal do Passive (0), RTCom will
      not only respond to handshakes from the remote device, but will also
      use the protocol to prevent Buffer Full Errors. To implement active
      protocols, a task with the given priority is created (if it has not
      been created already). It will check every PollCycle ticks, whether a
      protocol action is necessary. Protocols are only supported in interrupt
      driven mode. Please note that parameters ProtTaskPrio and PollCycle are
      global for all ports. The values supplied in the last call to
      SetProtocol apply for all ports. */

void EnableCOMInterrupt(int Port, unsigned BufferSize);
   /* Enables interrupts for sending/receiving and allocates send and receive
      mailbox with BufferSize slots each, if no buffers have been allocated
      already. If you need different sizes for receive and send buffers, call
      AllocateCOMBUffers first. If the port uses a 16550a UART, the interrupt
      trigger level is set to 8 characters. */

void DisableCOMInterrupt(int Port);
   /* Disable Interrupts for sending/receiving.
      The send and receive buffers are not cleared or deleted. */

COMData ReceiveCharPolled(int Port);
   /* Polls the port until a character is received. Then, the character
      and any error bits are returned. Do not (!) use this function
      together with function LineStatus. LineStatus will delete any errors
      that may have occurred. */

void SendCharPolled(int Port, Byte Data);
   /* Polls the port until the transmit register is empty. Then, the Data
      character is transmitted. */

void SendChar(int Port, Byte Data);
   /* Data is placed in the send buffer to be transmitted by the interrupt
      handler as soon as the transmit register becomes empty. */

int  SendCharTimed(int Port, Byte Data, Duration Timeout);
   /* Same as SendChar, but this function returns False (0) if the data
      could not be placed in the send buffer within the time given in
      parameter Timeout. Please note that the data is merely in the send
      buffer and is not guaranteed to be sent even when this function
      returns True (1). */

void SendBlock(int Port, void * Data, unsigned int Length);
   /* Send a block of data. This is equivalent to
            for(i=0; i<Length; i++)
               SendChar(P, Data[i]);
      but is faster.
   */

unsigned int SendBlockTimed(int Port, void * Data, unsigned int Length, Duration Timeout);
   /* Same as SendBlock, but this function returns the number of bytes placed
      in the send buffer without getting a timeout. If all data was transferred
      to the send buffer, Length is returned. The timeout applies to the
      complete data block. */

int  WaitSendBufferEmpty(int Port, Duration Timeout);
   /* Waits until the send buffer of Port is empty, that is, all pending data
      has been sent to the UART. WaitSendBufferEmpty does not use polling,
      rather, the calling task is blocked until the send buffer is empty.
      If the function return True (1), the Sendbuffer is empty. Otherwise
      Data may still be in the send buffer and the timeout has expired.
      Please note that the last byte may still be in the send shift
      register. To be sure all data has been sent out, check bit
      TX_SHIFT_EMPTY in the Line Status register after a call to
      WaitSendBufferEmpty. Example:

         if (!WaitSendBufferEmpty(P, 180))
            printf("Send buffer not empty after 10 seconds...");
         else
            while (!(LineStatus(P) & TX_SHIFT_EMPTY))
               RTKScheduler();
   */

char *COMError(COMData Data);
   /* Returns a pointer to a string corresponding to the most severe
      error set in Data */

Byte LineStatus(int Port);
   /* Reads the Line Status Register. See status mask constants for all
      possible return values. Several bits may be set simultaneously. */

void SetModemStatusHook(int Port, void (*Hook)(int Port, Byte Status));
   /* Define a function of the application to be called by RTCom on
      every modem status change. Such a hook may be used to trap the change
      of modem status signals such as Data Carrier or Ring Indicator. */

Byte ModemStatus(int Port);
   /* Reads the Modem Status Register. See status mask constants for all
      possible returned values. Several bits may be set simultaneously. */

void ModemControl(int Port, int SetToOneZero, int NewValue);
   /* Sets the Modem Control Register. See status mask constants for all
      possible values. If SetToOneZero == 1, the value is "ored" to the
      register, otherwise, it is "not-anded". */

int  HasFIFO(int Port);
   /* This function may be used to find out whether a port is equipped
      with a 16550A UART chip. */

void EnableFIFO(int Port, int Trigger);
   /* If the port uses a 16550A UART, this function can be used to set
      the interrupt trigger level. The trigger level specifies after how
      many received bytes an interrupt is generated. If no bytes are
      received during a time sufficient to transmit 4 bytes, the chip
      will generate a time-out interrupt for all trigger levels. Thus,
      no data will stay in the FIFO forever. By default, RTCom sets the
      trigger level to 8 if it detects a 16550A. You may change this value
      to 1, 4, 8, or 14. With 0, the FIFO buffer is disabled. Low values
      for the trigger level may result in faster response of your tasks
      waiting for data; however, overall system performance suffers
      due to many interrupts. With high values (e.g. 8 or 14), you get
      less interrupts, thus a higher system throughput. At 14, overrun
      errors may occur at very high baud rates. For most applications,
      trigger level 8 is a good compromise. */


   /* The following routines are needed if you use a non-standard hardware
      configuration. They should be used before any ports are initialised
      using InitPort. */

void SetCOMBoardTyp(AddOnPortType Typ,
                    unsigned      Address,
                    int           IRQ,
                    int           FirstPort);
   /* If you intend to use a generic, Hostess, or DigiBoard, its parameters
      must be defined with this function. For the GenericCOMBoard type,
      parameter Address is interpreted as the number of ports on the card. */

void SetIOBase(int Port, unsigned short IOBase);
   /* Any ports not registered in the BIOS data segment must be defined
      using this function. This is always necessary for port numbers
      greater than 3. If your I/O card is not recognized by your BIOS,
      even the addresses for COM3 and COM4 must be set using SetIOBase. */

void SetIRQ(int Port, int IRQ);
   /* Defines the Interrupt ReQuest line to be used for the port. You may
      share IRQs for the pairs COM1/COM3 and/or COM2/COM4. The best
      performance is achieved if every port uses its own IRQ.

      Only ports 0 .. 5 (COM1 .. COM6) are supported by this call. */

void AllocateCOMBuffers(int P, unsigned ReceiveBufferSize, unsigned SendBufferSize);
   /* Allocate receive and send buffers for the port P. If the buffers have
      been allocated already, this call has no effect. The call to this
      function is not mandatory, function EnableCOMInterrupt will call

         AllocateCOMBuffers(P, BufferSize, BufferSize)

      to ensure that buffers are available. */

/* old style for compatibility with older versions of RTCom: */

#define V24Data  COMData
#define V24Error COMError

#ifdef  __cplusplus
}
#endif

#endif
