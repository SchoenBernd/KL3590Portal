/**************************************************************************/
/*                                                                        */
/*  Module:  RTKernel (Tracer)                     Copyright (c) 1989,94  */
/*  Version: 4.5                                 On Time Informatik GmbH  */
/*                                                                        */
/*                                                                        */
/*                                      On Time        /úúúúúúúúúúú/ÄÄÄÄÄ */
/*                                    Informatik GmbH /úúúúúúúúúúú/       */
/* ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ/úúúúúúúúúúú/        */
/*                                  Echtzeit- und Systemsoftware          */
/*                                                                        */
/**************************************************************************/

/* Interface for the Kernel Trace Buffer.

   The Debug Version of RTKernel includes a kernel tracer. Many events
   inside the kernel are written to a ring buffer. In addition, 10 user
   event types are available for the application. The buffer may be dumped
   to the screen, a file or it may be inspected using a debugger. This
   feature is useful to analyse what exactly happend just before an event,
   which may not be fully understood.

   The Tracebuffer is a structure normally allocated on the heap.
   RTKTraceBuffer points to this structure. The record contains the index
   of the last written event (BufferEnd), the number of entries the
   buffer can hold (BufferSize), and the actual array of events to store the
   events. You should never access events beyond BufferSize.
   Initially, the buffer has 64 entries. In many cases, this will not be
   sufficient; you may use procedure RTKSetTraceBufferSize to allocate
   a new buffer. Up to about 8000 entries are supported.

   An Event consists of the event itsself and in most cases one additional
   parameter. Depending on the event, this parameter may be a task, a
   semaphore, a mailbox, an IRQ, or a long integer. The array
   RTKTraceParameterOf may be inspected to find out, which event uses
   what parameter.

   Alle events supported are listet in type TraceEvent below. For
   further details, please read the declarations in the rest of this file.  */

typedef enum {
   tNoEvent,             /* No Event                                       */
   tStateReady,          /* A task enters state Ready.          Parm: Task */
   tStateCurrent,        /* A task enters state Current.        Parm: Task */
   tStateSuspended,      /* A task enters state Suspended.      Parm: Task */
   tStateDelaying,       /* CurrTask enters state Delaying.     Parm: None */
   tStateBlockedWait,    /* CurrTask enters state BlockedWait.  Parm: Sema */
   tStateTimedWait,      /* CurrTask enters state TimedWait.    Parm: Sema */
   tStateBlockedPut,     /* CurrTask enters state BlockedPut.   Parm: MBox */
   tStateBlockedGet,     /* CurrTask enters state BlockedGet.   Parm: MBox */
   tStateTimedPut,       /* CurrTask enters state TimedPut.     Parm: MBox */
   tStateTimedGet,       /* CurrTask enters state TimedGet.     Parm: MBox */
   tStateBlockedSend,    /* A task enters state BlockedSend.    Parm: Task (Receiver) */
   tStateBlockedReceive, /* CurrTask enters state BlockedReceive. Parm: None */
   tStateTimedSend,      /* A task enters state TimedSend.      Parm: Task (Receiver) */
   tStateTimedReceive,   /* CurrTask enters state TimedReceive. Parm: None */
   tStateDeadlocked,     /* A task enters state Deadlocked.     Parm: Task */
   tStateIllegal,        /* A task enters state Illegal.        Parm: Task */
   tStateTerminated,     /* A task enters state Terminated.     Parm: Task */
   tInterruptStart,      /* A hardware interrupt has occured.   Parm: IRQ */
   tInterruptEnd,        /* An IRQ-Handler has completed.       Parm: IRQ */
   tNewTime,             /* The RTKernel-Clock is incremented   Parm: Number */
   tPriorityDropped,     /* The priority of a task is lowered.  Parm: Task */
   tPriorityRaised,      /* The priority of a task is raised.   Parm: Task */
   tTimeout,             /* The timeout of a task is expired.   Parm: Task */
   tSchedOn,             /* CurrTask calls EnableScheduler(True). Parm: None */
   tSchedOff,            /* CurrTask calls EnableScheduler(False). Parm: None */
   tSemaInc,             /* CurrTask increments a semphore.     Parm: Sema */
   tSemaDec,             /* CurrTask decrements a semphore.     Parm: Sema */
   tMBIn,                /* CurrTask writes message into mailbox. Parm: MBox */
   tMBOut,               /* CurrTask reads message from mailbox. Parm: MBox */
   tMessageSend,         /* CurrTask sends message to a task.   Parm: Task */
   tMessageReceive,      /* CurrTask receives message from task. Parm: Task */
   tUserEvent_1,         /* User Events. Parm is determined by TraceParameterOf */
   tUserEvent_2,         /* dito */
   tUserEvent_3,         /* etc. */
   tUserEvent_4,
   tUserEvent_5,
   tUserEvent_6,
   tUserEvent_7,
   tUserEvent_8,
   tUserEvent_9,
   tUserEvent_10
} TraceEvent;

struct TraceRec {
   TraceEvent Event;
#ifndef __FLAT__
   unsigned   Fill;
#endif
   union {
      TaskHandle Task;
      Semaphore  Sema;
      Mailbox    MBox;
      long       Num;
      void far * Ptr;
   } P;
} ;

struct TraceBufferType {
   unsigned int    BufferEnd, BufferSize;
#ifndef __FLAT__
   unsigned int    fill1, fill2;
#endif
   struct TraceRec Events[1];
} ;

enum TraceParameterType { NoParameter, TaskParameter, SemaParameter,
                          MBoxParameter, NumberParameter, PointerParameter } ;

extern enum TraceParameterType RTKTraceParameterOf[];
extern char * RTKTraceName[];
extern struct TraceBufferType * RTKTraceBuffer;
