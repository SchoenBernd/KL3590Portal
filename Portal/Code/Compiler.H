/* EasyCODE V6.5
 */
/* EasyCODE ( 1 */
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

/* This include file defines some compiler and memory model dependent macros
*/

#ifndef _COMPILER_H
#define _COMPILER_H

#ifdef __DPMI16__
   #define DPMI
#elif (defined(DPMI) && !defined(__DPMI32__))
   #define __DPMI16__
#endif

#ifdef __DPMI32__
   #define DPMI
#endif

#ifdef _MSC_VER

   #ifdef M_I86TM
      #define LCODE 0
      #define LDATA 0
   #elif M_I86SM
      #define LCODE 0
      #define LDATA 0
   #elif M_I86MM
      #define LCODE 1
      #define LDATA 0
   #elif M_I86CM
      #define LCODE 0
      #define LDATA 1
   #elif M_I86LM
      #define LCODE 1
      #define LDATA 1
   #elif M_I86HM
      #define LCODE 1
      #define LDATA 1
   #else
      #error RTKernel error: Unable to determine memory model
   #endif

   #include "C:\bc\INCLUDE\conio.h"
   #define ASM      _asm
   #define IN(P)    inp(P)
   #define OUT(P,V) outp(P,V)
   #define SETVECT  _dos_setvect
   #define GETVECT  _dos_getvect

/* EasyCODE > */
/* EasyCODE ( 2 */
/* EasyCODE C */
/* EasyCODE < */
typedef union
/* EasyCODE > */
{
    /* EasyCODE ( 3 */
    /* EasyCODE C */
    /* EasyCODE < */
struct
/* EasyCODE > */
    {
        unsigned int  es, ds, di, si, bp, sp;
        unsigned int  bx, dx, cx, ax;
        unsigned int  ip, cs, flags;
    }
    X;
    /* EasyCODE E */
    /* EasyCODE ) */
    /* EasyCODE ( 4 */
    /* EasyCODE C */
    /* EasyCODE < */
struct
/* EasyCODE > */
    {
        unsigned int  es, ds, di, si, bp, sp;
        unsigned char bl, bh, dl, dh, cl, ch, al, ah;
        unsigned int  ip, cs, flags;
    }
    x;
    /* EasyCODE E */
    /* EasyCODE ) */
}
Regs;
/* EasyCODE E */
/* EasyCODE ) */
#elif defined __TURBOC__

   #if __TINY__
      #define LCODE 0
      #define LDATA 0
   #elif __SMALL__
      #define LCODE 0
      #define LDATA 0
   #elif __MEDIUM__
      #define LCODE 1
      #define LDATA 0
   #elif __COMPACT__
      #define LCODE 0
      #define LDATA 1
   #elif __LARGE__
      #define LCODE 1
      #define LDATA 1
   #elif __HUGE__
      #define LCODE 1
      #define LDATA 1
   #elif __FLAT__
      #define LCODE 0
      #define LDATA 0
   #else
      #error RTKernel error: Unable to determine memory model
   #endif

   #define ASM      asm
   #define IN(P)    inportb(P)
   #define OUT(P,V) outportb(P,V)
   #define SETVECT  setvect
   #define GETVECT  getvect
/* EasyCODE ( 5 */
/* EasyCODE C */
/* EasyCODE < */
typedef union
/* EasyCODE > */
{
    /* EasyCODE ( 6 */
    /* EasyCODE C */
    /* EasyCODE < */
struct
/* EasyCODE > */
    {
        unsigned int  bp, di, si, ds, es;
        unsigned int  dx, cx, bx, ax;
        unsigned int  ip, cs, flags;
    }
    X;
    /* EasyCODE E */
    /* EasyCODE ) */
    /* EasyCODE ( 7 */
    /* EasyCODE C */
    /* EasyCODE < */
struct
/* EasyCODE > */
    {
        unsigned int  bp, di, si, ds, es;
        unsigned char dl, dh, cl, ch, bl, bh, al, ah;
        unsigned int  ip, cs, flags;
    }
    x;
    /* EasyCODE E */
    /* EasyCODE ) */
}
Regs;
/* EasyCODE E */
/* EasyCODE ) */
//#else
//   #error RTKernel error: Unsupported compiler
#endif

#include "C:\bc\INCLUDE\dos.h"

#ifndef FP_OFF
   #define FP_OFF(fp)  ((unsigned int) fp)
#endif

#ifndef __FLAT__

   #ifndef MK_FP
      #define MK_FP(seg, ofs)  (void far *) (((long) seg << 16) + ofs)
   #endif
   #ifndef FP_SEG
      #define FP_SEG(fp) ((unsigned) (((long) (fp)) >> 16))
   #endif

   /* the following symbols ease writing memory model independent assembler
      code */

   #define IntFunc(Name) void far interrupt Name(Regs R)

   #if defined(__cplusplus) && defined(__TURBOC__)
      typedef void interrupt (far * ISRPtr)(...);
   #else
      typedef void (interrupt far * ISRPtr)();
   #endif

   typedef void far * FarPointer;

   #define FarPointerIsNULL(FP) (FP == NULL)
   #define FarPointerEqual(P1, P2) (P1 == P2)

   #define BCFlatOp

#else

// #if (__TURBOC__ < 0x460)
      #undef inportb     /* bad bug in BC 4.02/4.5 -> remove as soon as bug is fixed */
      #undef outportb
      #define inportb(addr)       (_EDX = (addr), __emit__(0xEC,0xEB,0x00), _AL)
      #define outportb(addr,val)  (_ESI = (val), _EDX = (addr), _EAX = _ESI, __emit__(0xEE,0xEB,0x00))
// #endif

   #if (__TURBOC__ < 0x460)
      #define BCFlatOp  volatile     /* BC++ 4.02 optimizer bug -> undef as soon as bug is fixed */
   #else
      #define BCFlatOp
   #endif

   #define far       /* no far  pointers :-(   */
   #define near      /* no near pointers       */
   #define interrupt /* no interrupt functions */

   #define IntFunc(Name) void Name(void);   /* flat memory model */

   typedef void (* ISRPtr)(void);

   #pragma option -a-
/* EasyCODE ( 8 */
/* EasyCODE C */
/* EasyCODE < */
typedef struct
/* EasyCODE > */
{
    unsigned int   Offset;
    unsigned short Selector;
}
FarPointer;
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE < */
#pragma option -a.

#define FarPointerIsNULL(FP) (FP.Selector == 0)
#define FarPointerEqual(P1, P2) ((P1.Selector == P2.Selector) && (P1.Offset == P2.Offset))

#endif

#if LDATA
#define mLDS lds
#define mLES les        /* memory model dependent LES     */
#define mES  es:        /* memory model dependent ES (ds) */
#else
#define mLDS mov
#define mLES mov
#define mES
#endif

#ifdef __FLAT__
#define mSI esi
#define mDI edi
#define mBX ebx
#define mCX ecx
#define mDX edx
#else
#define mSI si
#define mDI di
#define mBX bx
#define mCX cx
#define mDX dx
#endif

#if !defined( __TTYPES_H) && !defined (__TCOLLECT_H)
#define False 0
#define True  1
#endif

#ifdef __FLAT__
void setvect(unsigned char vector, unsigned short sel, unsigned int ofs);
FarPointer getvect(unsigned char vector);
#endif

#endif

/* EasyCODE > */
/* EasyCODE ) */
