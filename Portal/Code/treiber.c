/* EasyCODE V8 */
/* EasyCODE ( 0 
Treiberprogramme */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
/* Datei: treiber.c     Klasse: 3590 Portal     Ersteller: Halfmann  EWE 2856

Kurzbeschreibung:
================

Treiber fuer die Basissteuerung 3.

Folgende Funktionen stehen zur VerfÅgung:

pwrgood   -   Einlesen des PWRGOOD-Signals
              Ausgabeparameter: TRUE, wenn Power good
                                FALSE, wenn Power fail

battok    -   Einlesen des LOWBATT/ -Signals (Ack.)
              Ausgabeparameter: TRUE, wenn Batterie ok.
                                FALSE, wenn Batterie leer

led1on    -   LED 1 ein

led1off   -   LED 1 aus

led2on    -   LED 2 ein

led2off   -   LED 2 aus

com2breakon  -   BREAK auf com2 setzen

com2breakoff -   BREAK auf com2 rÅcksetzen
*/

#include "portal.h"
/* EasyCODE ) */
/* EasyCODE ( 0 
pwrgood */
/* EasyCODE F */
unsigned char pwrgood(void)
{
    if ( (inportb(STATIN) & PF_MSK)==PF_MSK )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
battok */
/* EasyCODE F */
unsigned char battok(void)
{
    if ( (inportb(STATIN) & BATTOK_MSK)==BATTOK_MSK )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
led1on */
/* EasyCODE F */
void led1on(void)
{
    outportb (LED1ADR,LED1ON);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
led1off */
/* EasyCODE F */
void led1off(void)
{
    outportb (LED1ADR,LED1OFF);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
led2on */
/* EasyCODE F */
void led2on(void)
{
    outportb (LED2ADR,LED2ON);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
led2off */
/* EasyCODE F */
void led2off(void)
{
    outportb (LED2ADR,LED2OFF);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
com2breakon */
/* EasyCODE F */
void com2breakon(void)
{
    outportb (LCCOM2,inportb(LCCOM2) | BREAK_MSK);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
com2breakoff */
/* EasyCODE F */
void com2breakoff(void)
{
    outportb (LCCOM2,inportb(LCCOM2) & ~BREAK_MSK);
}

/* EasyCODE ) */
/* EasyCODE ) */
