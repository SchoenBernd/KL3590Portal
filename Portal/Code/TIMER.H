/**************************************************************************/
/*                                                                        */
/*  Module:  Timer                                 Copyright (c) 1989,94  */
/*  Version: 4.5                                 On Time Informatik GmbH  */
/*                                                                        */
/*                                                                        */
/*                                      On Time        /úúúúúúúúúúú/ÄÄÄÄÄ */
/*                                    Informatik GmbH /úúúúúúúúúúú/       */
/* ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ/úúúúúúúúúúú/        */
/*                                  Echtzeit- und Systemsoftware          */
/*                                                                        */
/**************************************************************************/

/* High level interface to RTKernel's timer device.

   The default timer device is the PC timer chip, channel 0, which supports a
   resolution of 0.838 microseconds for time measurements and timer
   interrupts in the range of 0.838 microseconds up to approx. 55
   milliseconds. The driver for the real-time clock offers timer interrupt
   interval ranging from approx. 122 microseconds (8192 Hertz) up to 0.5
   seconds (2 Hertz). Information on how different timer device drivers are
   installed can be found in file RTTIMER.H.

   This module used floating point. If this must be avoided, use module
   ITimer instead.

   Before any function of the module is used, TimerInit() must have been
   called.  */

#ifndef _TIMER_H
#define _TIMER_H

#include "ITimer.H"

#ifdef __cplusplus
extern "C" {
#endif

extern float SecPerTick; /* current timer-interrupt-interval in seconds */
extern float Resolution; /* accuracy for time measurements in seconds   */

void TimerInit(void);
   /* initialise this module */


/*------------------ Routines to waste time ---------------------------*/

void Synchronize(void);
   /* waits in a busy loop for the next timer interrupt to occur.
      If you are using RTKernel, use Delay(1) instead.
      This function is not supported by the RTClock device. */

float TimeSinceTimeInt(void);
   /* returns the time in seconds since the last timer interrupt. This
      function is not supported by the RTClock device. */


/*------------------ Routines to measure time periods -----------------*/

void MarkTime(FineTime *T);
   /* mark the start of a period to be timed */

float ElapsedTime(FineTime *T);
   /* returns the time in seconds since the last call to MarkTime or
      ElapsedAndMark with the same parameter */

float ElapsedAndMark(FineTime *T);
   /* returns the time in seconds since the last call to MarkTime or
      ElapsedAndMark. An implicit MarkTime is performed. */


/*------------- Routines to change the timer interrupt frequency ------*/

void SetTimerInterval(float Seconds);
   /* set the timer interrupt interval to the specified value in seconds. The
      parameter is rounded to the nearest value supported by the hardware.
      For the PC driver this is a multiple of 0.838 microseconds with a
      maximum of approx. 54.93 milliseconds. For the real-time clock, the
      allowed frequencies are powers of two in the range 1/2 .. 1/8192.
      The exact value in effect can be found in variable SecPerTick after
      a call to this function. */

long Ticks(float Time);
   /* translate time in seconds to timer ticks */

#ifdef  __cplusplus
}
#endif

#endif
