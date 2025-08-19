/* EasyCODE V6.5
 */
/* EasyCODE ( 0 */
/* EasyCODE < */
/**************************************************************************/
/*                                                                        */
/*  Module:  RTKernel                              Copyright (c) 1989,94  */
/*  Version: 4.5                                 On Time Informatik GmbH  */
/*                                                                        */
/*                                                                        */
/*                                      On Time        /úúúúúúúúúúú/ÄÄÄÄÄ */
/*                                    Informatik GmbH /úúúúúúúúúúú/       */
/* ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ/úúúúúúúúúúú/        */
/*                                  Echtzeit- und Systemsoftware          */
/*                                                                        */
/**************************************************************************/

/* RealTime Multitasking Kernel for the programming language C.

   This header file contains all visible declarations of RTKernel.
   Documentation for the application interface can be found in the RTKernel
   owners manual.                                                         */

/* EasyCODE > */
/* EasyCODE ( 0 */
/* EasyCODE C */
/* EasyCODE < */
#ifdef __cplusplus
extern "C"

/* EasyCODE > */
{
    #endif
    #include "RTKernel.CFG"     /* Configuration of RTKernel */
    
    #include "Compiler.H"       /* includes some macros to ease writing portable code */
#ifdef DPMI
/* EasyCODE < */
    #define RTK4             /* force new style RTKCreateMailbox in p-mode */

/* EasyCODE > */
#endif 
/* EasyCODE < */
    /* Flags for RTKTaskInfo */
    
    #define TASKHANDLE   0x00000001L
    #define TASKNAME     0x00000002L
    #define BASEPRIO     0x00000004L
    #define PRIO         0x00000008L
    #define STATE        0x00000010L
    #define RELDELAY     0x00000020L
    #define ABSDELAY     0x00000040L
    #define FREESTACK    0x00000080L
    #define MINSTACK     0x00000100L
    #define COPROCESSOR  0x00000200L
    #define TASKSWITCHES 0x00000400L
    #define CPUTIME      0x00000800L
    #define RELCPUTIME   0x00001000L
    #define WAITINGATPOS 0x00002000L
    #define WAITINGATOBJ 0x00004000L
    #define RESOURCES    0x00008000L
    #define LISTALL      0xFFFFFFFFL
    
    /* Hardware dependencies */
    
    #define LastIRQ        15
    #define RTKInt_Master  0x20
    #define RTKInt_Slave   0xA0
    
    /* some data types used by RTKernel */
    typedef long        TimerTicks;
    typedef TimerTicks  Time;               /* absolute times */
    typedef TimerTicks  Duration;           /* time periods   */
    typedef enum { Counting, Binary, Resource } SemaType;
    typedef enum { Ready, Current, Suspended, Delaying,
                   BlockedWait, TimedWait,
                   BlockedPut, BlockedGet, TimedPut, TimedGet,
              BlockedSend, BlockedReceive, TimedSend, TimedReceive,
                   Deadlocked, Illegal, Terminated
                 } TaskState;
    
    #include "RTKernel.PRV"            /* kernel data structures */
    typedef TaskRef TaskHandle;
    typedef SemaRef Semaphore;
    typedef MCBRef  Mailbox;
    typedef void ** MemoryPool;
    typedef void (far * IRQHandler)(void);
    
    #define NOTASK      NoTask
    #define NOMAILBOX   NULL
    #define NOSEMAPHORE NULL
    #define EMPTYPOOL   NULL
    
    #include "RTTrace.H"               /* kernel tracer data structures */
    
    /* -------- global data and flags -- */
    extern unsigned char RTKVectorOfIRQ[];     /* default = { 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77 } */
    extern int   RTKLPSemas;                   /* Set by RTKernelInit() */
    extern int   RTKProtectLibrary;            /* default is False      */
    extern int   RTKDefault8087Protection;     /* default is True       */
    extern int   RTKFarStacks;                 /* default is False      */
    extern int   RTKSwap32BitRegs;             /* default is False (16 bit only) */
    extern int   RTKTrapDOS2535;               /* default is True       */
    extern char *RTKListTitles[];              /* title strings for RTKTaskInfo */
    extern struct TraceBufferType * RTKTraceBuffer;
    extern TaskHandle RTKMainTaskHandle;       /* Handle of Main Task */
    extern TaskHandle RTKIdleTaskHandle;       /* Handle of Idle Task */
    
    /* -------- general task management - */
    TaskHandle  RTKCreateTask    (void      (*TaskCode)(void),
                                  unsigned    Priority,
                                  unsigned    Stack,
                                  char       *Name);
    void        RTKTerminateTask (TaskHandle *Handle);
    void        RTKSuspend       (TaskHandle  Handle);
    void        RTKResume        (TaskHandle  Handle);
    void        RTKSetPriority   (TaskHandle  Handle, unsigned Priority);
    void        RTKProtect8087   (void);
    void        RTKFree8087      (void);
    int         RTKAllocUserData (void);
    void        RTKSetUserData   (TaskHandle  Handle, int Index, void * UserData);
    void *      RTKGetUserData   (TaskHandle  Handle, int Index);
    void *      RTKGetLocalData  (int Index);
    
    
    /* -------- inquiries about tasks -- */
    TaskHandle  RTKCurrentTaskHandle(void);
    TaskState   RTKGetTaskState  (TaskHandle Handle);
    unsigned    RTKGetTaskPrio   (TaskHandle Handle);
    unsigned    RTKGetTaskStack  (TaskHandle Handle);
    unsigned    RTKGetMinStack   (TaskHandle Handle);
    void        RTKTaskInfo      (char *Buffer, unsigned BufferLen, unsigned long ListFlags);
    
    
    /* -------- time-related functions - */
    void        RTKSetTime       (Time NewTime);
    Time        RTKGetTime       (void);
    void        RTKDelay         (Duration Ticks);
    void        RTKDelayUntil    (Time Ticks);
    void        RTKTimeSlice     (Duration Ticks);
    
    
    /* -------- semaphores ------------- */
    Semaphore   RTKCreateSemaphore(SemaType Typ, unsigned InitialValue, char * Name);
    void        RTKDeleteSemaphore(Semaphore *S);
    unsigned    RTKSemaValue     (Semaphore S);
    void        RTKSignal        (Semaphore S);
    void        RTKWait          (Semaphore S);
    int         RTKWaitCond      (Semaphore S);
    int         RTKWaitTimed     (Semaphore S, Duration Timeout);
    TaskHandle  RTKResourceOwner (Semaphore S);
    
    
    /* -------- mailboxes -------------- */

/* EasyCODE > */
#ifdef RTK4
    Mailbox     RTKCreateMailbox (unsigned DataLen, unsigned Slots, char * Name);
#else 
    Mailbox     RTKCreateMailbox (unsigned DataLen, unsigned Slots, ...);
#endif 
/* EasyCODE < */
    void        RTKDeleteMailbox (Mailbox *Box);
    void        RTKClearMailbox  (Mailbox Box);
    unsigned    RTKMessages      (Mailbox Box);
    void        RTKPut           (Mailbox Box, void *Data);
    void        RTKPutFront      (Mailbox Box, void *Data);
    void        RTKGet           (Mailbox Box, void *Data);
    int         RTKPutCond       (Mailbox Box, void *Data);
    int         RTKPutFrontCond  (Mailbox Box, void *Data);
    int         RTKGetCond       (Mailbox Box, void *Data);
    int         RTKNextCond      (Mailbox Box, void *Data);
    int         RTKPutTimed      (Mailbox Box, void *Data, Duration Timeout);
    int         RTKPutFrontTimed (Mailbox Box, void *Data, Duration Timeout);
    int         RTKGetTimed      (Mailbox Box, void *Data, Duration Timeout);
    
    
    /* -------- message-passing -------- */
    void        RTKSend          (TaskHandle Receiver, void *Data);
    int         RTKSendCond      (TaskHandle Receiver, void *Data);
    int         RTKSendTimed     (TaskHandle Receiver, void *Data, Duration Timeout);
    void        RTKReceive       (void *Data, unsigned DataLen);
    int         RTKReceiveCond   (void *Data, unsigned DataLen);
    int         RTKReceiveTimed  (void *Data, unsigned DataLen, Duration Timeout);
    
    
    /* -------- DOS and BIOS handling -- */
    void        RTKDOSProtectionON(void);
    void        RTKDOSProtectionOFF(void);
    int         RTKExec          (const char * Path, const char *CmdLine);
    void        RTKDiskIntsON    (void);
    void        RTKDiskIntsOFF   (void);
    void        RTKSetDiskTimeout(Duration Diskette, Duration MotorStart, Duration HardDisk);
    
    
    /* -------- interrupt handling ----- */
    void        RTKSetIRQHandler (int IRQ, IRQHandler Handler);
    IRQHandler  RTKGetIRQHandler (int IRQ);
    void        RTKCallInt       (ISRPtr Handler);
    void        RTKIRQTopPriority(int Master, int Slave);
    void        RTKEnableIRQ     (int IRQ);
    void        RTKDisableIRQ    (int IRQ);
    void        RTKSetIRQStack   (int IRQ, unsigned StackSize);
    void        RTKIRQInfo       (char *Buffer, unsigned BufferLen);
    #define     RTKEnableInterrupts()      _enable()
    #define     RTKDisableInterrupts()     _disable()
    #define     RTKEndOfInterrupt()        OUT(RTKInt_Master, 0x20)
    #define     RTKSlave_EndOfInterrupt()  OUT(RTKInt_Slave, 0x20)
    void        RTKSetIRQHandlerFar(int IRQ, FarPointer Handler);
    FarPointer  RTKGetIRQHandlerFar(int IRQ);
    void        RTKCallIntFar      (FarPointer Handler);
    
    /* -------- memory management ------ */
    void        RTKAllocMemPool(MemoryPool * Pool, unsigned BlockSize, unsigned Blocks);
    void *      RTKGetBuffer(MemoryPool * Pool);
    void        RTKFreeBuffer(MemoryPool * Pool, void * Buffer);
    
    
    /* -------- Kernel tracer ---------- */
    void        RTKUserTrace(TraceEvent E, long Parameter);
    void        RTKSetTraceBufferSize(unsigned Size);
    void        RTKClearTraceBuffer(void);
    void        RTKEnableTrace(TraceEvent E);
    void        RTKDisableTrace(TraceEvent E);
    void        RTKStopTracing(void);
    void        RTKTraceAll(void);
    char *      RTKTraceHeader(char * Buffer);
    char *      RTKDumpTrace(char * Buffer, int Entry);
    
    
    /* --------  miscellaneous functions */
    TaskHandle  RTKernelInit     (unsigned MainPrio);
    void        RTKDeallocTerminatedTasks(void);
    int         RTKDebugVersion  (void);
    int         RTKUses8087      (void);
    void        RTKStackCheck    (void);
    void        RTKNoStackChecks (void);     /* not for Borland-C, near data */
    void        RTKScheduler     (void);
    void        RTKSchedulerON   (void);
    void        RTKSchedulerOFF  (void);
    void        RTKPreemptionsON (void);
    void        RTKPreemptionsOFF(void);
    void        RTKSetMessageHandler(void (*Handler)(char *));
    int         RTKLoadSymbols   (char *FileName);
    void        RTKClearStatistic(void);
    int         RTKCPU           (void);
    void        RTKSetTaskSwitchHook(void (*Hook)(TaskHandle OldTask, TaskHandle NewTask));
    #ifdef  __cplusplus

/* EasyCODE > */
}
#endif

/* EasyCODE E */
/* EasyCODE ) */
#ifndef _RTKERNEL_H
/* EasyCODE < */
#define _RTKERNEL_H

#define RTK_VER 455

/* EasyCODE > */
/* EasyCODE - */
/* -------- old style calls -------- */

#define     RTKListTasks(B,L)   RTKTaskInfo(B, L, TASKNAME+PRIO+STATE+ABSDELAY+FREESTACK+MINSTACK+COPROCESSOR+WAITINGATOBJ)
#define     RTKCreateSema(T,I)  RTKCreateSemaphore(T, I, "Semaphore")
#define     RTKSetIdleDelay(D,K)
#define     RTKSetTimerFraction(V)
typedef void TaskFunction(void);
#endif 
/* EasyCODE ) */
