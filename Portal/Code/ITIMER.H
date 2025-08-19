/**************************************************************************/
/*                                                                        */
/*  Module:  ITimer (Integer Timer)                Copyright (c) 1989,94  */
/*  Version: 4.5                                 On Time Informatik GmbH  */
/*                                                                        */
/*                                                                        */
/*                                      On Time        /úúúúúúúúúúú/ÄÄÄÄÄ */
/*                                    Informatik GmbH /úúúúúúúúúúú/       */
/* ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ/úúúúúúúúúúú/        */
/*                                  Echtzeit- und Systemsoftware          */
/*                                                                        */
/**************************************************************************/

/* High level, non-floating point interface to RTKernel's timer device
   driver.

   This module offers numerous functions for time measurements and time
   conversions for the high resolution timer. With the default device
   (channel 0 of the PC timer chip), a resolution of 0.838 microseconds
   is achieved. To install other timer devices, please refer to RTTIMER.H
   and RTTIMER.C.

   An arbitrary number of interleaved time periods can be measured. The
   maximum length of a time period to be measured is 2 147 483 647 timer
   ticks for the default device.

   This module can change the frequency of the timer interrupt to any value
   supported by the timer device. using function SetTimerIntVal.
   SetTimerIntVal will round to the nearest value supported by the timer
   device. For the default device, this is any value between 0 and 55
   milliseconds exact to 0.838 microseconds. For the RTClock device, this is
   1/(2^x), where x is an integer in the range 1 to 13. Please note that
   functions Synchronize and FTimeSinceTimeInt are not supported by the
   RTClock device.

   Before any function of the module is used, ITimerInit() must have been
   called.

   If you prefere using floating point, refere to module Timer. It offers
   a higher level of abstraction for time measurements. */

#ifndef _ITIMER_H
#define _ITIMER_H

typedef unsigned long ul;

#define TimeRec FineTime

#ifdef __cplusplus
extern "C" {
#endif

/* Time measurements */

void MarkTime(FineTime * T);        /* Mark the start of a period */
ul   ElapsedMicroSecs(FineTime T);  /* since last MarkTime        */
ul   ElapsedMilliSecs(FineTime T);  /* dito                       */
ul   ElapsedSeconds  (FineTime T);  /* dito                       */
/*   floating point function ElapsedTime() is declared in TIMER.H */


/* Conversions Time -> Micro-, Milli-, Seconds */

ul   TimeToMicroSecs (FineTime T);   /* max 1 hour and 11 minutes */
ul   TimeToMilliSecs (FineTime T);   /* max 50 days               */
ul   TimeToSeconds   (FineTime T);   /* max 7.5 years             */


/* Conversions Micro-, Milli-, Seconds -> Time */

void MicroSecsToTime (FineTime *T, ul Micros);
void MilliSecsToTime (FineTime *T, ul Millis);
void SecondsToTime   (FineTime *T, ul Secs);


/* Conversions Micro-, Milli-, Seconds -> Ticks */

ul   MicroSecsToTicks(ul Micros);    /* max one hour for non-standard interrupt rates */
ul   MilliSecsToTicks(ul Millis);    /* dito */
ul   SecondsToTicks  (ul Secs);      /* dito */


/* Miscellaneous */

void ITimerInit      (void);         /* call before using this module */
void Synchronize     (void);         /* wait for timer interrupt (not for RTClock device) */
void SetTimerIntVal  (ul Micros);    /* set time between timer interrupts */
void FTimeSinceTimeInt(FineTime *T); /* FineTime since last timer interrupt (not for RTClock device) */

#ifdef  __cplusplus
}
#endif

#endif
