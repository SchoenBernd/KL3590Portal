/* EasyCODE V8 */
/* EasyCODE ( 0 
SMBS3 */
/*********************************************************************************************/
/* Datei:        smbs3.c                                                                     */
/* Ersteller:    Edi Bastian   ESE 2856                                                      */
/*                                                                                           */
/* In dieser Datei befinden sich die Programme fuer das Schrittmotor-Interface.              */
/*   sminit ........... Init fuer die Schrittmotoren 1, 2 und 3                              */
/*   sm1bef ........... Schrittmotor 1: Bearbeitung des Auftrages                            */
/*   sm1stop ..........                 Bearbeitung des Stop-Auftrages                       */
/*   GT1_viIsrTmr2 ....                 Interrupt fuer Startverzoegerung                     */
/*   CC1_viIsrTmr0 ....                 Interrupt fuer Schrittpuls                           */
/*   sm2bef ........... Schrittmotor 2: Bearbeitung des Auftrages                            */
/*   sm2stop ..........                 Bearbeitung des Stop-Auftrages                       */
/*   GT1_viIsrTmr3 ....                 Interrupt fuer Startverzoegerung                     */
/*   CC2_viIsrTmr7 ....                 Interrupt fuer Schrittpuls                           */
/*   sm3bef ........... Schrittmotor 3: Bearbeitung des Auftrages                            */
/*   sm3stop ..........                 Bearbeitung des Stop-Auftrages                       */
/*   GT1_viIsrTmr4 ....                 Interrupt fuer Startverzoegerung                     */
/*   CC1_viIsrTmr1 ....                 Interrupt fuer Schrittpuls                           */
/*   smeingang ........ Bearbeitung der Eingangssignale fuer SM1, SM2 und SM3 (zyklisch)     */
/*                                                                                           */
/*********************************************************************************************/


/*********************************************************************************************/
/*                                                                                           */
/* Beschreibung der Auftraege (bef)                                                          */
/*                                                                                           */
/* Auftrag  Funktion                                                                         */
/*                                                                                           */
/*   0      Rampen-Initialisierung                                                           */
/*          erforderliche Parameter: vort                                                    */
/*          Aktionen: aktuelle Bewegung abbrechen, Vorteiler neu programmieren, apul=0,      */
/*                    stat=0                                                                 */
/*   1      Rampe starten ohne Startverzoegerung                                             */
/*          erforderliche Parameter: mod, adr                                                */
/*          Aktionen: Pruefung "ramp not finished", PAW Rampe setzen, Richtung setzen,       */
/*                    apul=0, stat setzen, Rampe starten                                     */
/*   2      Rampe starten mit Startverzoegerung                                              */
/*          erforderliche Parameter: mod, adr, verz                                          */
/*          Aktionen: Pruefung "ramp not finished", PAW Verzoegerung setzen, stat setzen,    */
/*                    Verzoegerung starten,                                                  */
/*                    wenn Verz. abgelaufen: Pruefung "ramp not finished", PAW Rampe setzen, */
/*                       Richtung setzen, apul=0, stat setzen, Rampe starten                 */
/*   3      Rampe mit Gesamtpulszahl starten ohne Startverzoegerung                          */
/*          erforderliche Parameter: mod, adr, gesp                                          */
/*          Aktionen: Pruefung "ramp not finished", PAW Rampe setzen, Richtung setzen,       */
/*                    apul=0, stat setzen, Rampe starten                                     */
/*                    wenn Rampe abgelaufen: Pruefung apul><gesp                             */
/*   4      Rampe mit Gesamtpulszahl und Dachverlaengerung starten ohne Startverzoegerung    */
/*          erforderliche Parameter: mod, adr, gesp, dachp                                   */
/*          Aktionen: Pruefung "ramp not finished", PAW Rampe setzen, Richtung setzen,       */
/*                    apul=0, stat setzen, Rampe starten                                     */
/*                    wenn Rampe abgelaufen: Pruefung apul><gesp                             */
/*   5      Rampe mit Gesamtpulszahl und Dachverlaengerung starten mit Startverzoegerung     */
/*          erforderliche Parameter: mod, adr, verz, gesp, dachp                             */
/*          Aktionen: Pruefung "ramp not finished", PAW Verzoegerung setzen, stat setzen,    */
/*                    Verzoegerung starten,                                                  */
/*                    wenn Verz. abgelaufen: Pruefung "ramp not finished", PAW Rampe setzen, */
/*                       Richtung setzen, apul=0, stat setzen, Rampe starten                 */
/*                    wenn Rampe abgelaufen: Pruefung apul><gesp                             */
/*   6      Bewegung stoppen                                                                 */
/*          erforderliche Parameter: keine                                                   */
/*          Aktionen: Bewegung wird abgebrochen, stat setzen, apul bleibt auf aktuellem Wert */
/*   7      Funktion 1 (FKT1) ein                                                            */
/*          erforderliche Parameter: keine                                                   */
/*          Aktionen: Ausgang wird gesetzt                                                   */
/*   8      Funktion 1 (FKT1) aus                                                            */
/*          erforderliche Parameter: keine                                                   */
/*          Aktionen: Ausgang wird rueckgesetzt                                              */
/*   9      Funktion 2 (FKT2) ein                                                            */
/*          erforderliche Parameter: keine                                                   */
/*          Aktionen: Ausgang wird gesetzt                                                   */
/*   10     Funktion 2 (FKT2) aus                                                            */
/*          erforderliche Parameter: keine                                                   */
/*          Aktionen: Ausgang wird rueckgesetzt                                              */
/*   12     Endlose Bewegung mit Pulsabstandvorgabe starten ohne Startverzoegerung           */
/*          erforderliche Parameter: mod, gesp = aktuelle Pulsabstaende                      */
/*          Aktionen: PAW Rampe setzen, Richtung setzen,                                     */
/*                    apul muß mit init vorher auf 0 gesetzt werden, stat setzen,            */
/*                    Rampe starten, apul kann nach Stop ausgewertet werden und ist          */
/*                    gueltig bis zum naechsten Init                                         */

/*                                                                                           */
/* Bemerkungen:                                                                              */
/*   - Die Rampe hat Prioritaet gegenueber Gesamtpulszahl (gesp) und Pulszahl bis Dachende   */
/*     (dachp), d.h. gesp und dachp koennen nur als Verlaengerungen benutzt werden. Ist die  */
/*     Anzahl der ausgefuehrten Pulse (apul) ungleich der Gesamtpulszahl (gesp), wird bei    */
/*     den Auftraegen 3, 4 und 5 eine Warnung ausgegeben.                                    */
/*   - Bei Auftraegen mit Dachverlaengerung (4 und 5) muss die ausgewaehlte Rampe einen      */
/*     Rampenabschnitt mit gesetztem Dachbit haben.                                          */
/*   - Mit Hilfe des Stopbytes (stop) kann jederzeit ein Stoppen der Bewegung durchgefuehrt  */
/*     werden. Das Stopbyte hat Prioritaet gegenueber dem Auftrag und ist als Notstop-Moeg-  */
/*     lichkeit gedacht. Ansonsten passiert das gleiche wie bei Auftrag 6.                   */
/*   - Der Vorteiler (vort) muss groesser 19 sein.                                           */
/*   - Die ausgefuehrte Verzoegerung liegt zwischen der gewaehlten Pulszahl verz und verz+1  */
/*                                                                                           */
/*********************************************************************************************/


/*********************************************************************************************/
/*                                                                                           */
/* Beschreibung der Rampen                                                                   */
/*                                                                                           */
/* Zu jeder der drei Schrittmotorachsen gibt es je einen huge-pointer, der auf die Tabelle   */
/* der Rampenadressen zeigt. In dieser Tabelle werden den virtuellen Rampenadressen die      */
/* absoluten Rampenadressen (huge-pointer auf den ersten Rampenabschnitt) zugeordnet. Die    */
/* Rampen selbst bestehen aus drei Bytes grossen Rampenabschnitten, die lueckenlos hinter-   */
/* einander im Flashspeicher abgelegt sind. Innerhalb einer Rampe darf keine 64k-Grenze      */
/* ueberschritten werden.                                                                    */
/* Jeder Rampenabschnitt hat folgenden Aufbau:                                               */
/*    1. Byte ... Pulsabstand Low Byte                                                       */
/*    2. Byte ... Pulsabstand High Byte                                                      */
/*    3. Byte ... Wiederholfaktor                                                            */
/* Fuer den Pulsabstand gilt:                                                                */
/*    Pulsabstand = - Anzahl der Erzeugerpuls    (Beispiel: 0xFFFE fuer 2 Erzeugerpulse)     */
/*    mit Anzahl der Erzeugerpulse > 1                                                       */
/* Der Wiederholfaktor hat folgenden Aufbau:                                                 */
/*    Bit 0 bis 5 ... Faktor = Anzahl der Wiederholungen - 1                                 */
/*                    (Beispiel: Faktor=0 ergibt einen Schrittpuls)                          */
/*    Bit 6 ......... Dachbit                                                                */
/*    Bit 7 ......... Stopbit                                                                */
/*                                                                                           */
/* Bemerkungen:                                                                              */
/*   - Nullrampe: Stopbit=1 und Dachbit=1 beim 1. Rampenabschnitt                            */
/*   - Die Anzahl der Wiederholungen bei gesetztem Stopbit wird durchgefuehrt.               */
/*     Beispiel: Wiederholfaktor = 0x82 ... noch 3 Schrittpulse, dann fertig.                */
/*   - Die Anzahl der Wiederholungen bei gesetztem Dachbit werden im Dach durchgefuehrt.     */
/*     Beispiel: Wiederholfaktor = 0x41 ... noch 2 Schrittpulse im Dach, dann Abrampe.       */
/*   - Es wird nochmals darauf hingewiesen, dass mit Gesamtpulszahl (gesp) und Pulse bis     */
/*     Dachende (dachp) nur Verlaengerungen aber keine Abbrueche oder Verkuerzungen erzielt  */
/*     werden koennen (siehe auch oben !).                                                   */
/*   - Es duerfen nur Rampen mit Dachverlaengerung gefahren werden, die einen Abschnitt mit  */
/*     gesetztem Dachbit beinhalten.                                                         */
/*                                                                                           */
/*********************************************************************************************/

#include "mainbs3.h"
#include "dprbs3.h"
#include "smbs3.h"

// Daten

                                       // SM 1:
unsigned char bdata eing1;             //   Eingangs-Aufbereitung
   sbit eing11 = eing1^0;              //     Eingang 1 - Ergebnis 
   sbit eing21 = eing1^1;              //     Eingang 2
   sbit eing11v = eing1^2;             //       Entprellung - voriger Wert
   sbit eing21v = eing1^3;
   sbit eing11p = eing1^4;             //       Entprellung - aktueller Wert
   sbit eing21p = eing1^5;
unsigned char bdata mod1;              //   Status
   sbit spawsv1 = mod1^0;              //     PAW Startverzoegerung
   sbit spawrp1 = mod1^1;              //     PAW Rampe
   sbit sdir1 = mod1^7;                //     Richtung
unsigned int sdata gesp1;              //   Gesamtpulszahl
unsigned int sdata dachp1;             //   Pulszahl bis Dachende - 1
unsigned char sdata modus1;            //   Rampen-Betriebsart
unsigned char xhuge * sdata rampptr1;   //   Rampenpointer AK
unsigned char bdata repfactor1;        //   Wiederholfaktor
   sbit stopbit1 = repfactor1^7;       //     Stopbit
   sbit dachbit1 = repfactor1^6;       //     Dachbit
unsigned char sdata repcount1;         //   Wiederholzaehler
/* EasyCODE ( 0 */
/* EasyCODE C */
/* EasyCODE < */
union
/* EasyCODE > */
{
        /* EasyCODE ( 0 */
        /* EasyCODE C */
        /* EasyCODE < */
        //   Pulsabstand
            struct
        /* EasyCODE > */
        {
                unsigned char low;
                unsigned char high;
        }byte;
        /* EasyCODE E */
        /* EasyCODE ) */
        unsigned int word;
}sdata pabst1;
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE < */
// SM 1 mit Startverzoegerung:
unsigned char sdata bef1m;             //   gemerkter Auftrag
unsigned char bdata mod1m;             //   gemerkter Status
   sbit spawsv1m = mod1m^0;            //     PAW Startverzoegerung
   sbit spawrp1m = mod1m^1;            //     PAW Rampe
   sbit sdir1m = mod1m^7;              //     Richtung
unsigned int sdata adr1m;              //   gemerkte Rampenauswahl
unsigned int sdata gesp1m;             //   gemerkte Gesamtpulszahl
unsigned int sdata dachp1m;            //   gemerkte Pulszahl bis Dachende - 1

                                       // SM 2:
unsigned char bdata eing2;             //   Eingangs-Aufbereitung
   sbit eing12 = eing2^0;              //     Eingang 1
   sbit eing22 = eing2^1;              //     Eingang 2
   sbit eing12v = eing2^2;             //       Entprellung - voriger Wert
   sbit eing22v = eing2^3;
   sbit eing12p = eing2^4;             //       Entprellung - aktueller Wert
   sbit eing22p = eing2^5;
unsigned char bdata mod2;              //   Status
   sbit spawsv2 = mod2^0;              //     PAW Startverzoegerung
   sbit spawrp2 = mod2^1;              //     PAW Rampe
   sbit sdir2 = mod2^7;                //     Richtung
unsigned int sdata gesp2;              //   Gesamtpulszahl
unsigned int sdata dachp2;             //   Pulszahl bis Dachende - 1
unsigned char sdata modus2;            //   Rampen-Betriebsart
unsigned char xhuge * sdata rampptr2;   //   Rampenpointer  AK
unsigned char bdata repfactor2;        //   Wiederholfaktor
   sbit stopbit2 = repfactor2^7;       //     Stopbit
   sbit dachbit2 = repfactor2^6;       //     Dachbit
unsigned char sdata repcount2;         //   Wiederholzaehler
/* EasyCODE > */
/* EasyCODE ( 0 */
/* EasyCODE C */
/* EasyCODE < */
union
/* EasyCODE > */
{
        /* EasyCODE ( 0 */
        /* EasyCODE C */
        /* EasyCODE < */
        //   Pulsabstand
            struct
        /* EasyCODE > */
        {
                unsigned char low;
                unsigned char high;
        }byte;
        /* EasyCODE E */
        /* EasyCODE ) */
        unsigned int word;
}sdata pabst2;
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE < */
// SM 2 mit Startverzoegerung:
unsigned char sdata bef2m;             //   gemerkter Auftrag
unsigned char bdata mod2m;             //   gemerkter Status
   sbit spawsv2m = mod2m^0;            //     PAW Startverzoegerung
   sbit spawrp2m = mod2m^1;            //     PAW Rampe
   sbit sdir2m = mod2m^7;              //     Richtung
unsigned int sdata adr2m;              //   gemerkte Rampenauswahl
unsigned int sdata gesp2m;             //   gemerkte Gesamtpulszahl
unsigned int sdata dachp2m;            //   gemerkte Pulszahl bis Dachende - 1

                                       // SM 3:
unsigned char bdata eing3;             //   Eingangs-Aufbereitung
   sbit eing13 = eing3^0;              //     Eingang 1
   sbit eing23 = eing3^1;              //     Eingang 2
   sbit eing13v = eing3^2;             //       Entprellung - voriger Wert
   sbit eing23v = eing3^3;
   sbit eing13p = eing3^4;             //       Entprellung - aktueller Wert
   sbit eing23p = eing3^5;
unsigned char bdata mod3;              //   Status
   sbit spawsv3 = mod3^0;              //     PAW Startverzoegerung
                                       //     PAW Rampe nicht vorhanden !
   sbit sdir3 = mod3^7;                //     Richtung
unsigned int sdata gesp3;              //   Gesamtpulszahl
unsigned int sdata dachp3;             //   Pulszahl bis Dachende - 1
unsigned char sdata modus3;            //   Rampen-Betriebsart
unsigned char xhuge * sdata rampptr3;   //   Rampenpointer  AK
unsigned char bdata repfactor3;        //   Wiederholfaktor
   sbit stopbit3 = repfactor3^7;       //     Stopbit
   sbit dachbit3 = repfactor3^6;       //     Dachbit
unsigned char sdata repcount3;         //   Wiederholzaehler
/* EasyCODE > */
/* EasyCODE ( 0 */
/* EasyCODE C */
/* EasyCODE < */
union
/* EasyCODE > */
{
        /* EasyCODE ( 0 */
        /* EasyCODE C */
        /* EasyCODE < */
        //   Pulsabstand
            struct
        /* EasyCODE > */
        {
                unsigned char low;
                unsigned char high;
        }byte;
        /* EasyCODE E */
        /* EasyCODE ) */
        unsigned int word;
}sdata pabst3;
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE < */
// SM 3 mit Startverzoegerung:
unsigned char sdata bef3m;             //   gemerkter Auftrag
unsigned char bdata mod3m;             //   gemerkter Status
   sbit spawsv3m = mod3m^0;            //     PAW Startverzoegerung
                                       //     PAW Rampe nicht vorhanden !
   sbit sdir3m = mod3m^7;              //     Richtung
unsigned int sdata adr3m;              //   gemerkte Rampenauswahl
unsigned int sdata gesp3m;             //   gemerkte Gesamtpulszahl
unsigned int sdata dachp3m;            //   gemerkte Pulszahl bis Dachende - 1
/* EasyCODE > */
/* EasyCODE ( 0 
sminit */
/* EasyCODE F */
//****************************************************************************
// Initialisierungen fuer das Schrittmotor-Interface

void sminit (void)
{
    // Hardware
    P2 &= 0x9F03;           // IO-Pins
    P2 |= 0x2080;           //   (FKT1, FKT2, DIR, EING1, EING2 und PAW laut Plan)
    DP2 |= 0x60FC;
    _nop_();
    P7 &= 0xFF9F;
    P7 |= 0x0060;
    DP7 |= 0x0060;
    _nop_();
    P8 &= 0xFFC3;
    P8 |= 0x0018;
    DP8 |= 0x003C;
    _nop_();
    
    PWMCON0 &= 0x8888;      // Initialisierungen fuer PWM 0 bis 2 (Vorteiler)
    PWMCON1 &= 0xAF8F;      //   timer stopped, clock/1, interrupt disable, no interrupt request
    PWMCON1 |= 0x0007;      //   output enable, mode 0 (standard PWM generation)
    P7 &= 0xFFF8;           //   output P7.0 bis P7.2, not inverted
    DP7 |= 0x0007;
    _nop_();
    
    T2CON = 0x0089;         // Initialisierungen fuer Timer 2 (Startverzoegerung SM 1)
                            //   rising edge on T2IN, counter mode, timer stopped, count down,
    DP3 &= 0xFF7F;          //   P3.7 input (T2IN)
    _nop_();
    T2IC = 0x0033;          //   interrupt (underflow) disable
                            //                         priority level ILVL=12
                            //                         group level GLVL=3
    
    T3CON = 0x0089;         // Initialisierungen fuer Timer 3 (Startverzoegerung SM 2)
                            //   rising edge on T3IN, counter mode, timer stopped, count down,
                            //   alternate output function disabled
    DP3 &= 0xFFBF;          //   P3.6 input (T3IN)
    _nop_();
    T3IC = 0x0032;          //   interrupt (underflow) disable
                            //                         priority level ILVL=12
                            //                         group level GLVL=2
    
    T4CON = 0x0089;         // Initialisierungen fuer Timer 4 (Startverzoegerung SM 3)
                            //   rising edge on T2IN, counter mode, timer stopped, count down,
    DP3 &= 0xFFDF;          //   P3.5 input (T4IN)
    _nop_();
    T4IC = 0x0031;          //   interrupt (underflow) disable
                            //                         priority level ILVL=12
                            //                         group level GLVL=1
    
    T01CON = 0x0209;        // Init fuer Timer 0 (Schrittpuls SM1) und Timer 1 (Schrittpuls SM3)
                            //   T0: counter mode, rising edge on T0IN, timer stopped
                            //   T1: timer mode, CLK/32, timer stopped
    T0IC = 0x003B;          //   T0: interrupt (overflow) disable
                            //                            priority level ILVL=14
                            //                            group level GLVL=3
    PECC3 = 0x0000;         //       zugehoeriger PECC nur Interrupt
    T1IC = 0x0039;          //   T1: interrupt (overflow) disable
                            //                            priority level ILVL=14
                            //                            group level GLVL=1
    PECC1 = 0x0000;         //       zugehoeriger PECC nur Interrupt
    DP3 &= 0xFFFE;          //   P3.0 input (T0IN)
    _nop_();
    P2 &= 0xFFFC;           //   P2.0, P2.1 outputs von CC0 und CC1
    DP2 |= 0x0003;
    _nop_();
    CCM0 = 0x00F7;          //   CC0: compare mode 3 (set output on each match), allocated to T0
                            //   CC1: compare mode 3 (set output on each match), allocated to T1
                            //   CC2: disabled
                            //   CC3: disabled
    CC0 = 0xFFFF;           //   load CC0 register
    CC1 = 0xFFFF;           //   load CC1 register
    
    T78CON &= 0xFF00;       // Init fuer Timer 7 (Schrittpuls SM2)
    T78CON |= 0x0009;       //   T7: counter mode, rising edge on T7IN, timer stopped
                            //   T8: wird fuer Zeittakt benutzt und hier nicht initialisiert !!!
    T7IC = 0x003A;          //   T7: interrupt (overflow) disable
                            //                            priority level ILVL=14
                            //                            group level GLVL=2
    PECC2 = 0x0000;         //       zugehoeriger PECC nur Interrupt
    DP2 &= 0x7FFF;          //   P2.15 input (T7IN)
    _nop_();
    P7 &= 0xFFEF;           //   P7.4 output von CC28
    DP7 |= 0x0010;
    _nop_();
    CCM7 = 0x0007;          //   CC28: compare mode 3 (set output on each match), allocated to T7
                            //   CC29: disabled
                            //   CC30: disabled
                            //   CC31: disabled
    CC28 = 0xFFFF;          //   load CC28 register
    
    // DPR
    SM1->apul = 0;          // SM1
    SM1->stat = RPFERTIG;
    SM1->eing = 0;
    SM1->stop = 0xFF;
    SM1->bef = 0xFF;
    SMH->ena = 0x00;        // Huepferfussablauf disabled
    SM2->apul = 0;          // SM2
    SM2->stat = RPFERTIG;
    SM2->eing = 0;
    SM2->stop = 0xFF;
    SM2->bef = 0xFF;
    SM3->apul = 0;          // SM3
    SM3->stat = RPFERTIG;
    SM3->eing = 0;
    SM3->stop = 0xFF;
    SM3->bef = 0xFF;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sm1bef */
/* EasyCODE F */
//****************************************************************************
// Bearbeitung des Auftrages fuer den Schrittmotor 1

void sm1bef (void)
{
    unsigned char bef;      // Zwischenspeicher fuer Auftrag
    
       bef = SM1->bef;
    switch ( bef )
    {
    // Initialisierung
    case 0 :
        T2R = 0;             // SV-Zaehler stoppen
        T2IE = 0;            // SV-Interrupt sperren
        T0R = 0;             // Pulsabstands-Zaehler stoppen
        T0IE = 0;            // Puls-Interrupt sperren
        PTR0 = 0;            // Vorteiler-Zaehler stoppen
        PT0 = 0;             // Vorteiler neu laden
        PP0 = SM1->vort-1;
        PW0 = PP0 / 2;
        PTR0 =1;             // Vorteiler-Zaehler starten
        SM1->apul=0;
        SM1->stat = RPFERTIG;
        break;
        /* EasyCODE < */
        // Starten ohne Startverzoegerung
        /* EasyCODE > */
    case 1 :
        /* EasyCODE = */
    case 3 :
        /* EasyCODE = */
    case 4 :
        if ( SM1->stat & (RPLAEUFT | SVLAEUFT) )
        {
            /* EasyCODE ( 0 */
            /* EasyCODE < */
            T2R = 0;                // SV-Zaehler stoppen
            T2IE = 0;               // SV-Interrupt sperren
            T0R = 0;                // Pulsabstands-Zaehler stoppen
            T0IE = 0;               // Puls-Interrupt sperren
            /* EasyCODE > */
            if ( SM1->stat & SVLAEUFT )
            {
                SM1->apul = 0;
            }
            SM1->stat = RPERROR;
            /* EasyCODE ) */
        }
        else
        {
            SM1->apul = 0;
            mod1 = SM1->mod;
            gesp1 = SM1->gesp;
            if ( (dachp1=SM1->dachp-1) == 0xFFFF )
            {
                dachp1 = 0;
            }
            rampptr1 = (* RAMPEN1) [SM1->adr];
            pabst1.byte.low = * rampptr1;
            pabst1.byte.high = * (rampptr1+1);
            repfactor1 = * (rampptr1+2);
            repcount1 = repfactor1 & 0x3F;
            if ( stopbit1 && dachbit1 )
            {
                if ( (bef==1) || (gesp1==0) )
                {
                    SM1->stat = RPFERTIG;
                }
                else
                {
                    SM1->stat = RPWARN;
                }
            }
            else
            {
                /* EasyCODE < */
                SM1->stat = RPLAEUFT;
                PAWRP1 = spawrp1;
                DIR1 = sdir1;
                T0 = T0REL = pabst1.word;     // Abstandszaehler und Reload laden
                /* EasyCODE > */
                if ( stopbit1 )
                {
                    if ( bef == 1 )
                    {
                        modus1 = ORIGINAL;
                    }
                    else
                    {
                        modus1 = BIS_STOP;
                    }
                }
                else
                {
                    if ( dachbit1 )
                    {
                        if ( repcount1!=0 )
                        {
                            repcount1--;
                            switch ( bef )
                            {
                            case 1 :
                                modus1 = ORIGINAL;
                                break;
                            case 3 :
                                modus1 = BIS_STOP;
                                break;
                            default:
                                modus1 = BIS_DACH;
                                break;
                            }
                        }
                        else
                        {
                            if ( (bef==4) && (dachp1!=0) )
                            {
                                modus1 = DACHVERL;
                            }
                            else
                            {
                                rampptr1 += 3;
                                pabst1.byte.low = * rampptr1;
                                pabst1.byte.high = * (rampptr1+1);
                                repfactor1 = * (rampptr1+2);
                                repcount1 = repfactor1 & 0x3F;
                                T0REL = pabst1.word;
                                if ( stopbit1 )
                                {
                                    repcount1++;
                                }
                                if ( bef == 1 )
                                {
                                    modus1 = ORIGINAL;
                                }
                                else
                                {
                                    modus1 = BIS_STOP;
                                }
                            }
                        }
                    }
                    else
                    {
                        if ( repcount1 == 0 )
                        {
                            rampptr1 += 3;
                            pabst1.byte.low = * rampptr1;
                            pabst1.byte.high = * (rampptr1+1);
                            repfactor1 = * (rampptr1+2);
                            repcount1 = repfactor1 & 0x3F;
                            T0REL = pabst1.word;
                            if ( stopbit1 )
                            {
                                repcount1++;
                            }
                        }
                        else
                        {
                            repcount1--;
                        }
                        switch ( bef )
                        {
                        case 1 :
                            modus1 = ORIGINAL;
                            break;
                        case 3 :
                            modus1 = BIS_STOP;
                            break;
                        default:
                            modus1 = BIS_DACH;
                            break;
                        }
                    }
                }
                /* EasyCODE < */
                T0IR = 0;         // Puls-Interrupt-Request loeschen
                T0IE = 1;         // Puls-Interrupt freigeben
                T0R = 1;          // Pulsabstands-Zaehler starten
                /* EasyCODE > */
            }
        }
        break;
        /* EasyCODE < */
        // Starten mit Startverzoegerung
        /* EasyCODE > */
    case 2 :
        /* EasyCODE = */
    case 5 :
        if ( SM1->stat & SVLAEUFT )
        {
            T2R = 0;                // SV-Zaehler stoppen
            T2IE = 0;               // SV-Interrupt sperren
            T0R = 0;                // Pulsabstands-Zaehler stoppen
            T0IE = 0;               // Puls-Interrupt sperren
            SM1->apul = 0;
            SM1->stat = RPERROR;
        }
        else
        {
            _atomic_(0);
            SM1->stat |= SVLAEUFT;
            _endatomic_();
            bef1m = bef;
            mod1m = SM1->mod;
            PAWSV1 = spawsv1m;
            T2 = SM1->verz;         // SV-Zaehler laden
            adr1m = SM1->adr;
            gesp1m = SM1->gesp;
            if ( (dachp1m=SM1->dachp-1) == 0xFFFF )
            {
                dachp1m = 0;
            }
            /* EasyCODE < */
            T2IR = 0;               // SV-Interrupt-Request loeschen
            T2IE = 1;               // SV-Interrupt freigeben
            T2R = 1;                // SV-Zaehler starten
            /* EasyCODE > */
        }
        break;
        /* EasyCODE < */
        // Stoppen
        /* EasyCODE > */
    case 6 :
        T2R = 0;             // SV-Zaehler stoppen
        T2IE = 0;            // SV-Interrupt sperren
        T0R = 0;             // Pulsabstands-Zaehler stoppen
        T0IE = 0;            // Puls-Interrupt sperren
        SM1->stat &= ~ (RPLAEUFT | SVLAEUFT);
        break;
        /* EasyCODE < */
        // Funktion 1 ein
        /* EasyCODE > */
    case 7 :
        FKT11 = FKTON;
        break;
        /* EasyCODE < */
        // Funktion 1 aus
        /* EasyCODE > */
    case 8 :
        FKT11 = FKTOFF;
        break;
        /* EasyCODE < */
        // Funktion 2 ein
        /* EasyCODE > */
    case 9 :
        FKT21 = FKTON;
        break;
        /* EasyCODE < */
        // Funktion 2 aus
        /* EasyCODE > */
    case 10 :
        FKT21 = FKTOFF;
        break;
    case 11 :
        // Reserviert für Drehrichtungswechsel von der Airbag Schwaechungs Firmware
        break;
    case 12 :
        // Endlos fahren, Starten ohne Startverzoegerung  (Basis war Case3 bzw. 4) 030412
        /* EasyCODE - */
        gesp1 = SM1->gesp;
        if ( SM1->apul == 0 )
        {
            // Erster Aufruf
            mod1 = SM1->mod;
            SM1->stat = RPLAEUFT;
            PAWRP1 = spawrp1;
            DIR1 = sdir1;
            T0 = T0REL = gesp1 ;       // Abstandszaehler laden
            modus1 = ENDLOS;
            T0IR = 0;         // Puls-Interrupt-Request loeschen
            T0IE = 1;         // Puls-Interrupt freigeben
            T0R = 1;          // Pulsabstands-Zaehler starten
        }
        else
        {
            // Weitere Aufrufe
            T0REL = gesp1;     // Reload laden
        }
        break;
    default:
        break;
    }
    /* EasyCODE < */
    SM1->bef = 0xFF;              // Auftrag rücksetzen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sm1stop */
/* EasyCODE F */
//****************************************************************************
// Bearbeitung des Stop-Auftrages fuer den Schrittmotor 1

void sm1stop (void)
{
    T2R = 0;             // SV-Zaehler stoppen
    T2IE = 0;            // SV-Interrupt sperren
    T0R = 0;             // Pulsabstands-Zaehler stoppen
    T0IE = 0;            // Puls-Interrupt sperren
    SM1->stat &= ~ (RPLAEUFT | SVLAEUFT);
    SM1->bef = 0xFF;
    SM1->stop = 0xFF;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GT1_viIsrTmr2 */
/* EasyCODE F */
//****************************************************************************
// @Function      void GT1_viIsrTmr2(void) interrupt T2INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT1 timer 2.
//                It is called up when the timer 2 register over or under-
//                flows or if timer 3 is reloaded with the content of timer 2
//                (external signal or T3 over/underflow) or if an external
//                capture event arrives.
//****************************************************************************
// Interrupt fuer Startverzoegerung abgelaufen, Schrittmotor 1

void GT1_viIsrTmr2(void) interrupt T2INT using SVBANK
{
    /* EasyCODE < */
    P7_7 = 1;                  // Performance-Messung
    T2R = 0;                   // SV-Zaehler stoppen
    T2IE = 0;                  // SV-Interrupt sperren
    /* EasyCODE > */
    if ( SM1->stat & RPLAEUFT )
    {
        T0R = 0;                // Pulsabstands-Zaehler stoppen
        T0IE = 0;               // Puls-Interrupt sperren
        SM1->stat = RPERROR;
    }
    else
    {
        SM1->apul = 0;
        mod1 = mod1m;
        gesp1 = gesp1m;
        dachp1 = dachp1m;
        rampptr1 = (* RAMPEN1) [adr1m];
        pabst1.byte.low = * rampptr1;
        pabst1.byte.high = * (rampptr1+1);
        repfactor1 = * (rampptr1+2);
        repcount1 = repfactor1 & 0x3F;
        if ( stopbit1 && dachbit1 )
        {
            if ( (bef1m==2) || (gesp1==0) )
            {
                SM1->stat = RPFERTIG;
            }
            else
            {
                SM1->stat = RPWARN;
            }
        }
        else
        {
            /* EasyCODE < */
            PAWRP1 = spawrp1;
            DIR1 = sdir1;
            T0 = T0REL = pabst1.word;     // Abstandszaehler und Reload laden
            /* EasyCODE > */
            if ( stopbit1 )
            {
                if ( bef1m == 2 )
                {
                    modus1 = ORIGINAL;
                }
                else
                {
                    modus1 = BIS_STOP;
                }
            }
            else
            {
                if ( dachbit1 )
                {
                    if ( repcount1!=0 )
                    {
                        repcount1--;
                        if ( bef1m == 2 )
                        {
                            modus1 = ORIGINAL;
                        }
                        else
                        {
                            modus1 = BIS_DACH;
                        }
                    }
                    else
                    {
                        if ( (bef1m==5) && (dachp1!=0) )
                        {
                            modus1 = DACHVERL;
                        }
                        else
                        {
                            rampptr1 += 3;
                            pabst1.byte.low = * rampptr1;
                            pabst1.byte.high = * (rampptr1+1);
                            repfactor1 = * (rampptr1+2);
                            repcount1 = repfactor1 & 0x3F;
                            T0REL = pabst1.word;
                            if ( stopbit1 )
                            {
                                repcount1++;
                            }
                            modus1 = ORIGINAL;
                        }
                    }
                }
                else
                {
                    if ( repcount1 == 0 )
                    {
                        rampptr1 += 3;
                        pabst1.byte.low = * rampptr1;
                        pabst1.byte.high = * (rampptr1+1);
                        repfactor1 = * (rampptr1+2);
                        repcount1 = repfactor1 & 0x3F;
                        T0REL = pabst1.word;
                        if ( stopbit1 )
                        {
                            repcount1++;
                        }
                    }
                    else
                    {
                        repcount1--;
                    }
                    if ( bef1m == 2 )
                    {
                        modus1 = ORIGINAL;
                    }
                    else
                    {
                        modus1 = BIS_DACH;
                    }
                }
            }
            /* EasyCODE < */
            SM1->stat = RPLAEUFT;
            T0IR = 0;         // Puls-Interrupt-Request loeschen
            T0IE = 1;         // Puls-Interrupt freigeben
            T0R = 1;          // Pulsabstands-Zaehler starten
            /* EasyCODE > */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
CC1_viIsrTmr0 */
/* EasyCODE F */
//****************************************************************************
// @Function      void CC1_viIsrTmr0(void) interrupt T0INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM timer 0.
//                It is called when overflow of the timer 0 register occurs.
//****************************************************************************
// Interrupt fuer Schrittpuls, Schrittmotor 1

void CC1_viIsrTmr0(void) interrupt T0INT using PULSBANK
{
    /* EasyCODE < */
    P7_7 = 1;                           // Performance-Messung
    /* EasyCODE > */
    switch ( modus1 )
    {
    // Rampe original (ohne Zaehler)
    case ORIGINAL :
        SM1->apul++;
        if ( repcount1!=0 )
        {
            repcount1--;
        }
        else
        {
            if ( stopbit1 )
            {
                T0R = 0;                   // Pulsabstands-Zaehler stoppen
                T0IE = 0;                  // Puls-Interrupt sperren
                _atomic_(0);
                SM1->stat &= ~RPLAEUFT;
                _endatomic_();
            }
            else
            {
                rampptr1 += 3;
                pabst1.byte.low = * rampptr1;
                pabst1.byte.high = * (rampptr1+1);
                repfactor1 = * (rampptr1+2);
                repcount1 = repfactor1 & 0x3F;
                T0REL = pabst1.word;
                if ( stopbit1 )
                {
                    repcount1++;
                }
            }
        }
        break;
        /* EasyCODE < */
        // Rampe bis Dachbit
        /* EasyCODE > */
    case BIS_DACH :
        SM1->apul++;
        if ( repcount1!=0 )
        {
            repcount1--;
        }
        else
        {
            if ( !dachbit1 )
            {
                rampptr1 += 3;
                pabst1.byte.low = * rampptr1;
                pabst1.byte.high = * (rampptr1+1);
                repfactor1 = * (rampptr1+2);
                repcount1 = repfactor1 & 0x3F;
                T0REL = pabst1.word;
                if ( stopbit1 )
                {
                    repcount1++;
                }
            }
            else
            {
                if ( !stopbit1 )
                {
                    if ( SM1->apul < dachp1 )
                    {
                        modus1 = DACHVERL;
                    }
                    else
                    {
                        rampptr1 += 3;
                        pabst1.byte.low = * rampptr1;
                        pabst1.byte.high = * (rampptr1+1);
                        repfactor1 = * (rampptr1+2);
                        repcount1 = repfactor1 & 0x3F;
                        T0REL = pabst1.word;
                        if ( stopbit1 )
                        {
                            repcount1++;
                        }
                        modus1 = BIS_STOP;
                    }
                }
                else
                {
                    if ( SM1->apul < gesp1 )
                    {
                        modus1 = ENDE;
                    }
                    else
                    {
                        if ( SM1->apul > gesp1 )
                        {
                            _atomic_(0);
                            SM1->stat |= RPWARN;
                            _endatomic_();
                        }
                        T0R = 0;                   // Pulsabstands-Zaehler stoppen
                        T0IE = 0;                  // Puls-Interrupt sperren
                        _atomic_(0);
                        SM1->stat &= ~RPLAEUFT;
                        _endatomic_();
                    }
                }
            }
        }
        break;
        /* EasyCODE < */
        // Dachverlaengerung
        /* EasyCODE > */
    case DACHVERL :
        if ( ++SM1->apul >= dachp1 )
        {
            rampptr1 += 3;
            pabst1.byte.low = * rampptr1;
            pabst1.byte.high = * (rampptr1+1);
            repfactor1 = * (rampptr1+2);
            repcount1 = repfactor1 & 0x3F;
            T0REL = pabst1.word;
            if ( stopbit1 )
            {
                repcount1++;
            }
            modus1 = BIS_STOP;
        }
        break;
        /* EasyCODE < */
        // Rampe bis Stopbit
        /* EasyCODE > */
    case BIS_STOP :
        SM1->apul++;
        if ( repcount1!=0 )
        {
            repcount1--;
        }
        else
        {
            if ( !stopbit1 )
            {
                rampptr1 += 3;
                pabst1.byte.low = * rampptr1;
                pabst1.byte.high = * (rampptr1+1);
                repfactor1 = * (rampptr1+2);
                repcount1 = repfactor1 & 0x3F;
                T0REL = pabst1.word;
                if ( stopbit1 )
                {
                    repcount1++;
                }
            }
            else
            {
                if ( SM1->apul < gesp1 )
                {
                    modus1 = ENDE;
                }
                else
                {
                    if ( SM1->apul > gesp1 )
                    {
                        _atomic_(0);
                        SM1->stat |= RPWARN;
                        _endatomic_();
                    }
                    T0R = 0;                   // Pulsabstands-Zaehler stoppen
                    T0IE = 0;                  // Puls-Interrupt sperren
                    _atomic_(0);
                    SM1->stat &= ~RPLAEUFT;
                    _endatomic_();
                }
            }
        }
        break;
        /* EasyCODE < */
        // Rampenende
        /* EasyCODE > */
    case ENDE :
        if ( ++SM1->apul >= gesp1 )
        {
            T0R = 0;                   // Pulsabstands-Zaehler stoppen
            T0IE = 0;                  // Puls-Interrupt sperren
            _atomic_(0);
            SM1->stat &= ~RPLAEUFT;
            _endatomic_();
        }
        break;
        /* EasyCODE < */
        // unzulaessig
        /* EasyCODE > */
    case ENDLOS :
        // 030412
        /* EasyCODE - */
        SM1->apul++;
        if ( SM1->apul < 0xFFFF )
        {
        }
        else
        {
            T0R = 0;                   // Pulsabstands-Zaehler stoppen
            T0IE = 0;                  // Puls-Interrupt sperren
            _atomic_(0);
            SM1->stat &= ~RPLAEUFT;
            _endatomic_();
        }
        break;
    default:
        /* EasyCODE < */
        T0R = 0;                   // Pulsabstands-Zaehler stoppen
        T0IE = 0;                  // Puls-Interrupt sperren
        /* EasyCODE > */
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sm2bef */
/* EasyCODE F */
//****************************************************************************
// Bearbeitung des Auftrages fuer den Schrittmotor 2

void sm2bef (void)
{
    unsigned char bef;      // Zwischenspeicher fuer Auftrag
    
       bef = SM2->bef;
    switch ( bef )
    {
    // Initialisierung
    case 0 :
        T3R = 0;             // SV-Zaehler stoppen
        T3IE = 0;            // SV-Interrupt sperren
        T7R = 0;             // Pulsabstands-Zaehler stoppen
        T7IE = 0;            // Puls-Interrupt sperren
        PTR1 = 0;            // Vorteiler-Zaehler stoppen
        PT1 = 0;             // Vorteiler neu laden
        PP1 = SM2->vort-1;
        PW1 = PP1 / 2;
        PTR1 =1;             // Vorteiler-Zaehler starten
        SM2->apul=0;
        SM2->stat = RPFERTIG;
        break;
        /* EasyCODE < */
        // Starten ohne Startverzoegerung
        /* EasyCODE > */
    case 1 :
        /* EasyCODE = */
    case 3 :
        /* EasyCODE = */
    case 4 :
        if ( SM2->stat & (RPLAEUFT | SVLAEUFT) )
        {
            /* EasyCODE ( 0 */
            /* EasyCODE < */
            T3R = 0;                // SV-Zaehler stoppen
            T3IE = 0;               // SV-Interrupt sperren
            T7R = 0;                // Pulsabstands-Zaehler stoppen
            T7IE = 0;               // Puls-Interrupt sperren
            /* EasyCODE > */
            if ( SM2->stat & SVLAEUFT )
            {
                SM2->apul = 0;
            }
            SM2->stat = RPERROR;
            /* EasyCODE ) */
        }
        else
        {
            SM2->apul = 0;
            mod2 = SM2->mod;
            gesp2 = SM2->gesp;
            if ( (dachp2=SM2->dachp-1) == 0xFFFF )
            {
                dachp2 = 0;
            }
            rampptr2 = (* RAMPEN2) [SM2->adr];
            pabst2.byte.low = * rampptr2;
            pabst2.byte.high = * (rampptr2+1);
            repfactor2 = * (rampptr2+2);
            repcount2 = repfactor2 & 0x3F;
            if ( stopbit2 && dachbit2 )
            {
                if ( (bef==1) || (gesp2==0) )
                {
                    SM2->stat = RPFERTIG;
                }
                else
                {
                    SM2->stat = RPWARN;
                }
            }
            else
            {
                /* EasyCODE < */
                SM2->stat = RPLAEUFT;
                PAWRP2 = spawrp2;
                DIR2 = sdir2;
                T7 = T7REL = pabst2.word;     // Abstandszaehler und Reload laden
                /* EasyCODE > */
                if ( stopbit2 )
                {
                    if ( bef == 1 )
                    {
                        modus2 = ORIGINAL;
                    }
                    else
                    {
                        modus2 = BIS_STOP;
                    }
                }
                else
                {
                    /* EasyCODE ( 0 */
                    if ( dachbit2 )
                    {
                        if ( repcount2!=0 )
                        {
                            repcount2--;
                            switch ( bef )
                            {
                            case 1 :
                                modus2 = ORIGINAL;
                                break;
                            case 3 :
                                modus2 = BIS_STOP;
                                break;
                            default:
                                modus2 = BIS_DACH;
                                break;
                            }
                        }
                        else
                        {
                            if ( (bef==4) && (dachp2!=0) )
                            {
                                modus2 = DACHVERL;
                            }
                            else
                            {
                                rampptr2 += 3;
                                pabst2.byte.low = * rampptr2;
                                pabst2.byte.high = * (rampptr2+1);
                                repfactor2 = * (rampptr2+2);
                                repcount2 = repfactor2 & 0x3F;
                                T7REL = pabst2.word;
                                if ( stopbit2 )
                                {
                                    repcount2++;
                                }
                                if ( bef == 1 )
                                {
                                    modus2 = ORIGINAL;
                                }
                                else
                                {
                                    modus2 = BIS_STOP;
                                }
                            }
                        }
                    }
                    else
                    {
                        if ( repcount2 == 0 )
                        {
                            rampptr2 += 3;
                            pabst2.byte.low = * rampptr2;
                            pabst2.byte.high = * (rampptr2+1);
                            repfactor2 = * (rampptr2+2);
                            repcount2 = repfactor2 & 0x3F;
                            T7REL = pabst2.word;
                            if ( stopbit2 )
                            {
                                repcount2++;
                            }
                        }
                        else
                        {
                            repcount2--;
                        }
                        switch ( bef )
                        {
                        case 1 :
                            modus2 = ORIGINAL;
                            break;
                        case 3 :
                            modus2 = BIS_STOP;
                            break;
                        default:
                            modus2 = BIS_DACH;
                            break;
                        }
                    }
                    /* EasyCODE ) */
                }
                /* EasyCODE < */
                T7IR = 0;         // Puls-Interrupt-Request loeschen
                T7IE = 1;         // Puls-Interrupt freigeben
                T7R = 1;          // Pulsabstands-Zaehler starten
                /* EasyCODE > */
            }
        }
        break;
        /* EasyCODE < */
        // Starten mit Startverzoegerung
        /* EasyCODE > */
    case 2 :
        /* EasyCODE = */
    case 5 :
        if ( SM2->stat & SVLAEUFT )
        {
            T3R = 0;                // SV-Zaehler stoppen
            T3IE = 0;               // SV-Interrupt sperren
            T7R = 0;                // Pulsabstands-Zaehler stoppen
            T7IE = 0;               // Puls-Interrupt sperren
            SM2->apul = 0;
            SM2->stat = RPERROR;
        }
        else
        {
            _atomic_(0);
            SM2->stat |= SVLAEUFT;
            _endatomic_();
            bef2m = bef;
            mod2m = SM2->mod;
            PAWSV2 = spawsv2m;
            T3 = SM2->verz;         // SV-Zaehler laden
            adr2m = SM2->adr;
            gesp2m = SM2->gesp;
            if ( (dachp2m=SM2->dachp-1) == 0xFFFF )
            {
                dachp2m = 0;
            }
            /* EasyCODE < */
            T3IR = 0;               // SV-Interrupt-Request loeschen
            T3IE = 1;               // SV-Interrupt freigeben
            T3R = 1;                // SV-Zaehler starten
            /* EasyCODE > */
        }
        break;
        /* EasyCODE < */
        // Stoppen
        /* EasyCODE > */
    case 6 :
        T3R = 0;             // SV-Zaehler stoppen
        T3IE = 0;            // SV-Interrupt sperren
        T7R = 0;             // Pulsabstands-Zaehler stoppen
        T7IE = 0;            // Puls-Interrupt sperren
        SM2->stat &= ~ (RPLAEUFT | SVLAEUFT);
        break;
        /* EasyCODE < */
        // Funktion 1 ein
        /* EasyCODE > */
    case 7 :
        FKT12 = FKTON;
        break;
        /* EasyCODE < */
        // Funktion 1 aus
        /* EasyCODE > */
    case 8 :
        FKT12 = FKTOFF;
        break;
        /* EasyCODE < */
        // Funktion 2 ein
        /* EasyCODE > */
    case 9 :
        FKT22 = FKTON;
        break;
        /* EasyCODE < */
        // Funktion 2 aus
        /* EasyCODE > */
    case 10 :
        FKT22 = FKTOFF;
        break;
    case 11 :
        // Reserviert für Drehrichtungswechsel von der Airbag Schwaechungs Firmware
        break;
    case 12 :
        // Endlos fahren, Starten ohne Startverzoegerung  (Basis war Case3 bzw. 4) 030412
        /* EasyCODE - */
        gesp2 = SM2->gesp;
        if ( SM2->apul == 0 )
        {
            // Erster Aufruf
            mod2 = SM2->mod;
            SM2->stat = RPLAEUFT;
            PAWRP2 = spawrp2;
            DIR2 = sdir2;
            T7 = T7REL = gesp2;       // Abstandszaehler laden
            modus2 = ENDLOS;
            T7IR = 0;         // Puls-Interrupt-Request loeschen
            T7IE = 1;         // Puls-Interrupt freigeben
            T7R = 1;          // Pulsabstands-Zaehler starten
        }
        else
        {
            // Weitere Aufrufe
            T7REL = gesp2;     // Reload laden
        }
        break;
    default:
        break;
    }
    /* EasyCODE < */
    SM2->bef = 0xFF;              // Auftrag rücksetzen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sm2stop */
/* EasyCODE F */
//****************************************************************************
// Bearbeitung des Stop-Auftrages fuer den Schrittmotor 2

void sm2stop (void)
{
    T3R = 0;             // SV-Zaehler stoppen
    T3IE = 0;            // SV-Interrupt sperren
    T7R = 0;             // Pulsabstands-Zaehler stoppen
    T7IE = 0;            // Puls-Interrupt sperren
    SM2->stat &= ~ (RPLAEUFT | SVLAEUFT);
    SM2->bef = 0xFF;
    SM2->stop = 0xFF;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GT1_viIsrTmr3 */
/* EasyCODE F */
//****************************************************************************
// @Function      void GT1_viIsrTmr3(void) interrupt T3INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT1 timer 3.
//                It is called up in the case of over or underflow of the 
//                timer 3 register.
//****************************************************************************
// Interrupt fuer Startverzoegerung abgelaufen, Schrittmotor 2

void GT1_viIsrTmr3(void) interrupt T3INT using SVBANK
{
    /* EasyCODE < */
    P7_7 = 1;                  // Performance-Messung
    T3R = 0;                   // SV-Zaehler stoppen
    T3IE = 0;                  // SV-Interrupt sperren
    /* EasyCODE > */
    if ( SM2->stat & RPLAEUFT )
    {
        T7R = 0;                // Pulsabstands-Zaehler stoppen
        T7IE = 0;               // Puls-Interrupt sperren
        SM2->stat = RPERROR;
    }
    else
    {
        SM2->apul = 0;
        mod2 = mod2m;
        gesp2 = gesp2m;
        dachp2 = dachp2m;
        rampptr2 = (* RAMPEN2) [adr2m];
        pabst2.byte.low = * rampptr2;
        pabst2.byte.high = * (rampptr2+1);
        repfactor2 = * (rampptr2+2);
        repcount2 = repfactor2 & 0x3F;
        if ( stopbit2 && dachbit2 )
        {
            if ( (bef2m==2) || (gesp2==0) )
            {
                SM2->stat = RPFERTIG;
            }
            else
            {
                SM2->stat = RPWARN;
            }
        }
        else
        {
            /* EasyCODE < */
            PAWRP2 = spawrp2;
            DIR2 = sdir2;
            T7 = T7REL = pabst2.word;     // Abstandszaehler und Reload laden
            /* EasyCODE > */
            if ( stopbit2 )
            {
                if ( bef2m == 2 )
                {
                    modus2 = ORIGINAL;
                }
                else
                {
                    modus2 = BIS_STOP;
                }
            }
            else
            {
                if ( dachbit2 )
                {
                    if ( repcount2!=0 )
                    {
                        repcount2--;
                        if ( bef2m == 2 )
                        {
                            modus2 = ORIGINAL;
                        }
                        else
                        {
                            modus2 = BIS_DACH;
                        }
                    }
                    else
                    {
                        if ( (bef2m==5) && (dachp2!=0) )
                        {
                            modus2 = DACHVERL;
                        }
                        else
                        {
                            rampptr2 += 3;
                            pabst2.byte.low = * rampptr2;
                            pabst2.byte.high = * (rampptr2+1);
                            repfactor2 = * (rampptr2+2);
                            repcount2 = repfactor2 & 0x3F;
                            T7REL = pabst2.word;
                            if ( stopbit2 )
                            {
                                repcount2++;
                            }
                            modus2 = ORIGINAL;
                        }
                    }
                }
                else
                {
                    if ( repcount2 == 0 )
                    {
                        rampptr2 += 3;
                        pabst2.byte.low = * rampptr2;
                        pabst2.byte.high = * (rampptr2+1);
                        repfactor2 = * (rampptr2+2);
                        repcount2 = repfactor2 & 0x3F;
                        T7REL = pabst2.word;
                        if ( stopbit2 )
                        {
                            repcount2++;
                        }
                    }
                    else
                    {
                        repcount2--;
                    }
                    if ( bef2m == 2 )
                    {
                        modus2 = ORIGINAL;
                    }
                    else
                    {
                        modus2 = BIS_DACH;
                    }
                }
            }
            /* EasyCODE < */
            SM2->stat = RPLAEUFT;
            T7IR = 0;         // Puls-Interrupt-Request loeschen
            T7IE = 1;         // Puls-Interrupt freigeben
            T7R = 1;          // Pulsabstands-Zaehler starten
            /* EasyCODE > */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
CC2_viIsrTmr7 */
/* EasyCODE F */
//****************************************************************************
// @Function      void CC2_viIsrTmr7(void) interrupt T7INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM timer 7.
//                It is called when the timer 7 register overflows.
//****************************************************************************
// Interrupt fuer Schrittpuls, Schrittmotor 2

void CC2_viIsrTmr7(void) interrupt T7INT using PULSBANK
{
    /* EasyCODE < */
    P7_7 = 1;                           // Performance-Messung
    /* EasyCODE > */
    switch ( modus2 )
    {
    // Rampe original (ohne Zaehler)
    case ORIGINAL :
        SM2->apul++;
        if ( repcount2!=0 )
        {
            repcount2--;
        }
        else
        {
            if ( stopbit2 )
            {
                T7R = 0;                   // Pulsabstands-Zaehler stoppen
                T7IE = 0;                  // Puls-Interrupt sperren
                _atomic_(0);
                SM2->stat &= ~RPLAEUFT;
                _endatomic_();
            }
            else
            {
                rampptr2 += 3;
                pabst2.byte.low = * rampptr2;
                pabst2.byte.high = * (rampptr2+1);
                repfactor2 = * (rampptr2+2);
                repcount2 = repfactor2 & 0x3F;
                T7REL = pabst2.word;
                if ( stopbit2 )
                {
                    repcount2++;
                }
            }
        }
        break;
        /* EasyCODE < */
        // Rampe bis Dachbit
        /* EasyCODE > */
    case BIS_DACH :
        SM2->apul++;
        if ( repcount2!=0 )
        {
            repcount2--;
        }
        else
        {
            if ( !dachbit2 )
            {
                rampptr2 += 3;
                pabst2.byte.low = * rampptr2;
                pabst2.byte.high = * (rampptr2+1);
                repfactor2 = * (rampptr2+2);
                repcount2 = repfactor2 & 0x3F;
                T7REL = pabst2.word;
                if ( stopbit2 )
                {
                    repcount2++;
                }
            }
            else
            {
                if ( !stopbit2 )
                {
                    if ( SM2->apul < dachp2 )
                    {
                        modus2 = DACHVERL;
                    }
                    else
                    {
                        rampptr2 += 3;
                        pabst2.byte.low = * rampptr2;
                        pabst2.byte.high = * (rampptr2+1);
                        repfactor2 = * (rampptr2+2);
                        repcount2 = repfactor2 & 0x3F;
                        T7REL = pabst2.word;
                        if ( stopbit2 )
                        {
                            repcount2++;
                        }
                        modus2 = BIS_STOP;
                    }
                }
                else
                {
                    if ( SM2->apul < gesp2 )
                    {
                        modus2 = ENDE;
                    }
                    else
                    {
                        if ( SM2->apul > gesp2 )
                        {
                            _atomic_(0);
                            SM2->stat |= RPWARN;
                            _endatomic_();
                        }
                        T7R = 0;                   // Pulsabstands-Zaehler stoppen
                        T7IE = 0;                  // Puls-Interrupt sperren
                        _atomic_(0);
                        SM2->stat &= ~RPLAEUFT;
                        _endatomic_();
                    }
                }
            }
        }
        break;
        /* EasyCODE < */
        // Dachverlaengerung
        /* EasyCODE > */
    case DACHVERL :
        if ( ++SM2->apul >= dachp2 )
        {
            rampptr2 += 3;
            pabst2.byte.low = * rampptr2;
            pabst2.byte.high = * (rampptr2+1);
            repfactor2 = * (rampptr2+2);
            repcount2 = repfactor2 & 0x3F;
            T7REL = pabst2.word;
            if ( stopbit2 )
            {
                repcount2++;
            }
            modus2 = BIS_STOP;
        }
        break;
        /* EasyCODE < */
        // Rampe bis Stopbit
        /* EasyCODE > */
    case BIS_STOP :
        SM2->apul++;
        if ( repcount2!=0 )
        {
            repcount2--;
        }
        else
        {
            if ( !stopbit2 )
            {
                rampptr2 += 3;
                pabst2.byte.low = * rampptr2;
                pabst2.byte.high = * (rampptr2+1);
                repfactor2 = * (rampptr2+2);
                repcount2 = repfactor2 & 0x3F;
                T7REL = pabst2.word;
                if ( stopbit2 )
                {
                    repcount2++;
                }
            }
            else
            {
                if ( SM2->apul < gesp2 )
                {
                    modus2 = ENDE;
                }
                else
                {
                    if ( SM2->apul > gesp2 )
                    {
                        _atomic_(0);
                        SM2->stat |= RPWARN;
                        _endatomic_();
                    }
                    T7R = 0;                   // Pulsabstands-Zaehler stoppen
                    T7IE = 0;                  // Puls-Interrupt sperren
                    _atomic_(0);
                    SM2->stat &= ~RPLAEUFT;
                    _endatomic_();
                }
            }
        }
        break;
        /* EasyCODE < */
        // Rampenende
        /* EasyCODE > */
    case ENDE :
        if ( ++SM2->apul >= gesp2 )
        {
            T7R = 0;                   // Pulsabstands-Zaehler stoppen
            T7IE = 0;                  // Puls-Interrupt sperren
            _atomic_(0);
            SM2->stat &= ~RPLAEUFT;
            _endatomic_();
        }
        break;
        /* EasyCODE < */
        // unzulaessig
        /* EasyCODE > */
    case ENDLOS :
        // 030412
        /* EasyCODE - */
        SM2->apul++;
        if ( SM2->apul < 0xFFFF )
        {
        }
        else
        {
            T7R = 0;                   // Pulsabstands-Zaehler stoppen
            T7IE = 0;                  // Puls-Interrupt sperren
            _atomic_(0);
            SM2->stat &= ~RPLAEUFT;
            _endatomic_();
        }
        break;
    default:
        /* EasyCODE < */
        T7R = 0;                   // Pulsabstands-Zaehler stoppen
        T7IE = 0;                  // Puls-Interrupt sperren
        /* EasyCODE > */
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sm3bef */
/* EasyCODE F */
//****************************************************************************
// Bearbeitung des Auftrages fuer den Schrittmotor 3

void sm3bef (void)
{
    unsigned char bef;      // Zwischenspeicher fuer Auftrag
    unsigned int erzfreq;   // Zwischenspeicher fuer Erzeugungsfrequenz der Rampe
    
       bef = SM3->bef;
    switch ( bef )
    {
    // Initialisierung
    case 0 :
        T4R = 0;             // SV-Zaehler stoppen
        T4IE = 0;            // SV-Interrupt sperren
        T1R = 0;             // Pulsabstands-Zaehler stoppen
        T1IE = 0;            // Puls-Interrupt sperren
        PTR2 = 0;            // Vorteiler-Zaehler stoppen
        PT2 = 0;             // Vorteiler neu laden
        PP2 = SM3->vort-1;
        PW2 = PP2 / 2;
        PTR2 =1;             // Vorteiler-Zaehler starten
        SM3->apul=0;
        SM3->stat = RPFERTIG;
        break;
        /* EasyCODE < */
        // Starten ohne Startverzoegerung
        /* EasyCODE > */
    case 1 :
        /* EasyCODE = */
    case 3 :
        /* EasyCODE = */
    case 4 :
        if ( SM3->stat & (RPLAEUFT | SVLAEUFT) )
        {
            /* EasyCODE < */
            T4R = 0;                // SV-Zaehler stoppen
            T4IE = 0;               // SV-Interrupt sperren
            T1R = 0;                // Pulsabstands-Zaehler stoppen
            T1IE = 0;               // Puls-Interrupt sperren
            /* EasyCODE > */
            if ( SM3->stat & SVLAEUFT )
            {
                SM3->apul = 0;
            }
            SM3->stat = RPERROR;
        }
        else
        {
            SM3->apul = 0;
            mod3 = SM3->mod;
            gesp3 = SM3->gesp;
            if ( (dachp3=SM3->dachp-1) == 0xFFFF )
            {
                dachp3 = 0;
            }
            rampptr3 = (* RAMPEN3) [SM3->adr];
            pabst3.byte.low = * rampptr3;
            pabst3.byte.high = * (rampptr3+1);
            repfactor3 = * (rampptr3+2);
            repcount3 = repfactor3 & 0x3F;
            if ( stopbit3 && dachbit3 )
            {
                if ( (bef==1) || (gesp3==0) )
                {
                    SM3->stat = RPFERTIG;
                }
                else
                {
                    SM3->stat = RPWARN;
                }
            }
            else
            {
                /* EasyCODE < */
                SM3->stat = RPLAEUFT;
                erzfreq = (mod3 & 0x0E) << 7;   // Erzeugungsfrequenz neu setzen
                _atomic_(0);
                T01CON &= 0xF8FF;
                T01CON |= erzfreq;
                _endatomic_();
                DIR3 = sdir3;
                T1 = T1REL = pabst3.word;     // Abstandszaehler und Reload laden
                /* EasyCODE > */
                if ( stopbit3 )
                {
                    if ( bef == 1 )
                    {
                        modus3 = ORIGINAL;
                    }
                    else
                    {
                        modus3 = BIS_STOP;
                    }
                }
                else
                {
                    if ( dachbit3 )
                    {
                        if ( repcount3!=0 )
                        {
                            repcount3--;
                            switch ( bef )
                            {
                            case 1 :
                                modus3 = ORIGINAL;
                                break;
                            case 3 :
                                modus3 = BIS_STOP;
                                break;
                            default:
                                modus3 = BIS_DACH;
                                break;
                            }
                        }
                        else
                        {
                            if ( (bef==4) && (dachp3!=0) )
                            {
                                modus3 = DACHVERL;
                            }
                            else
                            {
                                rampptr3 += 3;
                                pabst3.byte.low = * rampptr3;
                                pabst3.byte.high = * (rampptr3+1);
                                repfactor3 = * (rampptr3+2);
                                repcount3 = repfactor3 & 0x3F;
                                T1REL = pabst3.word;
                                if ( stopbit3 )
                                {
                                    repcount3++;
                                }
                                if ( bef == 1 )
                                {
                                    modus3 = ORIGINAL;
                                }
                                else
                                {
                                    modus3 = BIS_STOP;
                                }
                            }
                        }
                    }
                    else
                    {
                        if ( repcount3 == 0 )
                        {
                            rampptr3 += 3;
                            pabst3.byte.low = * rampptr3;
                            pabst3.byte.high = * (rampptr3+1);
                            repfactor3 = * (rampptr3+2);
                            repcount3 = repfactor3 & 0x3F;
                            T1REL = pabst3.word;
                            if ( stopbit3 )
                            {
                                repcount3++;
                            }
                        }
                        else
                        {
                            repcount3--;
                        }
                        switch ( bef )
                        {
                        case 1 :
                            modus3 = ORIGINAL;
                            break;
                        case 3 :
                            modus3 = BIS_STOP;
                            break;
                        default:
                            modus3 = BIS_DACH;
                            break;
                        }
                    }
                }
                /* EasyCODE < */
                T1IR = 0;         // Puls-Interrupt-Request loeschen
                T1IE = 1;         // Puls-Interrupt freigeben
                T1R = 1;          // Pulsabstands-Zaehler starten
                /* EasyCODE > */
            }
        }
        break;
        /* EasyCODE < */
        // Starten mit Startverzoegerung
        /* EasyCODE > */
    case 2 :
        /* EasyCODE = */
    case 5 :
        if ( SM3->stat & SVLAEUFT )
        {
            T4R = 0;                // SV-Zaehler stoppen
            T4IE = 0;               // SV-Interrupt sperren
            T1R = 0;                // Pulsabstands-Zaehler stoppen
            T1IE = 0;               // Puls-Interrupt sperren
            SM3->apul = 0;
            SM3->stat = RPERROR;
        }
        else
        {
            _atomic_(0);
            SM3->stat |= SVLAEUFT;
            _endatomic_();
            bef3m = bef;
            mod3m = SM3->mod;
            PAWSV3 = spawsv3m;
            T4 = SM3->verz;         // SV-Zaehler laden
            adr3m = SM3->adr;
            gesp3m = SM3->gesp;
            if ( (dachp3m=SM3->dachp-1) == 0xFFFF )
            {
                dachp3m = 0;
            }
            /* EasyCODE < */
            T4IR = 0;               // SV-Interrupt-Request loeschen
            T4IE = 1;               // SV-Interrupt freigeben
            T4R = 1;                // SV-Zaehler starten
            /* EasyCODE > */
        }
        break;
        /* EasyCODE < */
        // Stoppen
        /* EasyCODE > */
    case 6 :
        T4R = 0;             // SV-Zaehler stoppen
        T4IE = 0;            // SV-Interrupt sperren
        T1R = 0;             // Pulsabstands-Zaehler stoppen
        T1IE = 0;            // Puls-Interrupt sperren
        SM3->stat &= ~ (RPLAEUFT | SVLAEUFT);
        break;
        /* EasyCODE < */
        // Funktion 1 ein
        /* EasyCODE > */
    case 7 :
        FKT13 = FKTON;
        break;
        /* EasyCODE < */
        // Funktion 1 aus
        /* EasyCODE > */
    case 8 :
        FKT13 = FKTOFF;
        break;
        /* EasyCODE < */
        // Funktion 2 ein
        /* EasyCODE > */
    case 9 :
        FKT23 = FKTON;
        break;
        /* EasyCODE < */
        // Funktion 2 aus
        /* EasyCODE > */
    case 10 :
        FKT23 = FKTOFF;
        break;
    case 11 :
        // Reserviert für Drehrichtungswechsel von der Airbag Schwaechungs Firmware
        break;
    case 12 :
        // Endlos fahren, Starten ohne Startverzoegerung  (Basis war Case3 bzw. 4) 030412
        /* EasyCODE - */
        gesp3 = SM3->gesp;
        if ( SM3->apul == 0 )
        {
            mod3 = SM3->mod;
            SM3->stat = RPLAEUFT;
            erzfreq = (mod3 & 0x0E) << 7;   // Erzeugungsfrequenz neu setzen
            _atomic_(0);
            T01CON &= 0xF8FF;
            T01CON |= erzfreq;
            _endatomic_();
            
            DIR3 = sdir3;
            
            // Erster Aufruf
            T1 = T1REL = gesp3;     // Abstandszaehler laden
            modus3 = ENDLOS;
            T1IR = 0;         // Puls-Interrupt-Request loeschen
            T1IE = 1;         // Puls-Interrupt freigeben
            T1R = 1;          // Pulsabstands-Zaehler starten
        }
        else
        {
            // Weitere Aufrufe
            T1REL = gesp3;     // Reload laden
        }
        break;
    default:
        break;
    }
    /* EasyCODE < */
    SM3->bef = 0xFF;              // Auftrag rücksetzen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sm3stop */
/* EasyCODE F */
//****************************************************************************
// Bearbeitung des Stop-Auftrages fuer den Schrittmotor 3

void sm3stop (void)
{
    T4R = 0;             // SV-Zaehler stoppen
    T4IE = 0;            // SV-Interrupt sperren
    T1R = 0;             // Pulsabstands-Zaehler stoppen
    T1IE = 0;            // Puls-Interrupt sperren
    SM3->stat &= ~ (RPLAEUFT | SVLAEUFT);
    SM3->bef = 0xFF;
    SM3->stop = 0xFF;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GT1_viIsrTmr4 */
/* EasyCODE F */
//****************************************************************************
// @Function      void GT1_viIsrTmr4(void) interrupt T4INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT1 timer 4.
//                It is called up when the timer 4 register is over or under-
//                flows or if timer 3 is reloaded with the content of timer 4
//                (external signal or T3 over/underflow) or if an external
//                capture event arrives.
//****************************************************************************
// Interrupt fuer Startverzoegerung abgelaufen, Schrittmotor 3

void GT1_viIsrTmr4(void) interrupt T4INT using SVBANK
{
    /* EasyCODE < */
    unsigned int erzfreq;   // Zwischenspeicher fuer Erzeugungsfrequenz der Rampe
    
       P7_7 = 1;                  // Performance-Messung
       T4R = 0;                   // SV-Zaehler stoppen
       T4IE = 0;                  // SV-Interrupt sperren
    /* EasyCODE > */
    if ( SM3->stat & RPLAEUFT )
    {
        T1R = 0;                // Pulsabstands-Zaehler stoppen
        T1IE = 0;               // Puls-Interrupt sperren
        SM3->stat = RPERROR;
    }
    else
    {
        SM3->apul = 0;
        mod3 = mod3m;
        gesp3 = gesp3m;
        dachp3 = dachp3m;
        rampptr3 = (* RAMPEN3) [adr3m];
        pabst3.byte.low = * rampptr3;
        pabst3.byte.high = * (rampptr3+1);
        repfactor3 = * (rampptr3+2);
        repcount3 = repfactor3 & 0x3F;
        if ( stopbit3 && dachbit3 )
        {
            if ( (bef3m==2) || (gesp3==0) )
            {
                SM3->stat = RPFERTIG;
            }
            else
            {
                SM3->stat = RPWARN;
            }
        }
        else
        {
            /* EasyCODE < */
            erzfreq = (mod3 & 0x0E) << 7;   // Erzeugungsfrequenz neu setzen
            _atomic_(0);
            T01CON &= 0xF8FF;
            T01CON |= erzfreq;
            _endatomic_();
            DIR3 = sdir3;
            T1 = T1REL = pabst3.word;     // Abstandszaehler und Reload laden
            /* EasyCODE > */
            if ( stopbit3 )
            {
                if ( bef3m == 2 )
                {
                    modus3 = ORIGINAL;
                }
                else
                {
                    modus3 = BIS_STOP;
                }
            }
            else
            {
                if ( dachbit3 )
                {
                    if ( repcount3!=0 )
                    {
                        repcount3--;
                        if ( bef3m == 2 )
                        {
                            modus3 = ORIGINAL;
                        }
                        else
                        {
                            modus3 = BIS_DACH;
                        }
                    }
                    else
                    {
                        if ( (bef3m==5) && (dachp3!=0) )
                        {
                            modus3 = DACHVERL;
                        }
                        else
                        {
                            rampptr3 += 3;
                            pabst3.byte.low = * rampptr3;
                            pabst3.byte.high = * (rampptr3+1);
                            repfactor3 = * (rampptr3+2);
                            repcount3 = repfactor3 & 0x3F;
                            T1REL = pabst3.word;
                            if ( stopbit3 )
                            {
                                repcount3++;
                            }
                            modus3 = ORIGINAL;
                        }
                    }
                }
                else
                {
                    if ( repcount3 == 0 )
                    {
                        rampptr3 += 3;
                        pabst3.byte.low = * rampptr3;
                        pabst3.byte.high = * (rampptr3+1);
                        repfactor3 = * (rampptr3+2);
                        repcount3 = repfactor3 & 0x3F;
                        T1REL = pabst3.word;
                        if ( stopbit3 )
                        {
                            repcount3++;
                        }
                    }
                    else
                    {
                        repcount3--;
                    }
                    if ( bef3m == 2 )
                    {
                        modus3 = ORIGINAL;
                    }
                    else
                    {
                        modus3 = BIS_DACH;
                    }
                }
            }
            /* EasyCODE < */
            SM3->stat = RPLAEUFT;
            T1IR = 0;         // Puls-Interrupt-Request loeschen
            T1IE = 1;         // Puls-Interrupt freigeben
            T1R = 1;          // Pulsabstands-Zaehler starten
            /* EasyCODE > */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
CC1_viIsrTmr1 */
/* EasyCODE F */
//****************************************************************************
// @Function      void CC1_viIsrTmr1(void) interrupt T1INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM timer 1.
//                It is called when overflow of the timer 1 register occurs.
//****************************************************************************
// Interrupt fuer Schrittpuls, Schrittmotor 3

void CC1_viIsrTmr1(void) interrupt T1INT using PULSBANK
{
    /* EasyCODE < */
    P7_7 = 1;                           // Performance-Messung
    /* EasyCODE > */
    switch ( modus3 )
    {
    // Rampe original (ohne Zaehler)
    case ORIGINAL :
        SM3->apul++;
        if ( repcount3!=0 )
        {
            repcount3--;
        }
        else
        {
            if ( stopbit3 )
            {
                T1R = 0;                   // Pulsabstands-Zaehler stoppen
                T1IE = 0;                  // Puls-Interrupt sperren
                _atomic_(0);
                SM3->stat &= ~RPLAEUFT;
                _endatomic_();
            }
            else
            {
                rampptr3 += 3;
                pabst3.byte.low = * rampptr3;
                pabst3.byte.high = * (rampptr3+1);
                repfactor3 = * (rampptr3+2);
                repcount3 = repfactor3 & 0x3F;
                T1REL = pabst3.word;
                if ( stopbit3 )
                {
                    repcount3++;
                }
            }
        }
        break;
        /* EasyCODE < */
        // Rampe bis Dachbit
        /* EasyCODE > */
    case BIS_DACH :
        SM3->apul++;
        if ( repcount3!=0 )
        {
            repcount3--;
        }
        else
        {
            if ( !dachbit3 )
            {
                rampptr3 += 3;
                pabst3.byte.low = * rampptr3;
                pabst3.byte.high = * (rampptr3+1);
                repfactor3 = * (rampptr3+2);
                repcount3 = repfactor3 & 0x3F;
                T1REL = pabst3.word;
                if ( stopbit3 )
                {
                    repcount3++;
                }
            }
            else
            {
                if ( !stopbit3 )
                {
                    if ( SM3->apul < dachp3 )
                    {
                        modus3 = DACHVERL;
                    }
                    else
                    {
                        rampptr3 += 3;
                        pabst3.byte.low = * rampptr3;
                        pabst3.byte.high = * (rampptr3+1);
                        repfactor3 = * (rampptr3+2);
                        repcount3 = repfactor3 & 0x3F;
                        T1REL = pabst3.word;
                        if ( stopbit3 )
                        {
                            repcount3++;
                        }
                        modus3 = BIS_STOP;
                    }
                }
                else
                {
                    if ( SM3->apul < gesp3 )
                    {
                        modus3 = ENDE;
                    }
                    else
                    {
                        if ( SM3->apul > gesp3 )
                        {
                            _atomic_(0);
                            SM3->stat |= RPWARN;
                            _endatomic_();
                        }
                        T1R = 0;                   // Pulsabstands-Zaehler stoppen
                        T1IE = 0;                  // Puls-Interrupt sperren
                        _atomic_(0);
                        SM3->stat &= ~RPLAEUFT;
                        _endatomic_();
                    }
                }
            }
        }
        break;
        /* EasyCODE < */
        // Dachverlaengerung
        /* EasyCODE > */
    case DACHVERL :
        if ( ++SM3->apul >= dachp3 )
        {
            rampptr3 += 3;
            pabst3.byte.low = * rampptr3;
            pabst3.byte.high = * (rampptr3+1);
            repfactor3 = * (rampptr3+2);
            repcount3 = repfactor3 & 0x3F;
            T1REL = pabst3.word;
            if ( stopbit3 )
            {
                repcount3++;
            }
            modus3 = BIS_STOP;
        }
        break;
        /* EasyCODE < */
        // Rampe bis Stopbit
        /* EasyCODE > */
    case BIS_STOP :
        SM3->apul++;
        if ( repcount3!=0 )
        {
            repcount3--;
        }
        else
        {
            if ( !stopbit3 )
            {
                rampptr3 += 3;
                pabst3.byte.low = * rampptr3;
                pabst3.byte.high = * (rampptr3+1);
                repfactor3 = * (rampptr3+2);
                repcount3 = repfactor3 & 0x3F;
                T1REL = pabst3.word;
                if ( stopbit3 )
                {
                    repcount3++;
                }
            }
            else
            {
                if ( SM3->apul < gesp3 )
                {
                    modus3 = ENDE;
                }
                else
                {
                    if ( SM3->apul > gesp3 )
                    {
                        _atomic_(0);
                        SM3->stat |= RPWARN;
                        _endatomic_();
                    }
                    T1R = 0;                   // Pulsabstands-Zaehler stoppen
                    T1IE = 0;                  // Puls-Interrupt sperren
                    _atomic_(0);
                    SM3->stat &= ~RPLAEUFT;
                    _endatomic_();
                }
            }
        }
        break;
        /* EasyCODE < */
        // Rampenende
        /* EasyCODE > */
    case ENDE :
        if ( ++SM3->apul >= gesp3 )
        {
            T1R = 0;                   // Pulsabstands-Zaehler stoppen
            T1IE = 0;                  // Puls-Interrupt sperren
            _atomic_(0);
            SM3->stat &= ~RPLAEUFT;
            _endatomic_();
        }
        break;
        /* EasyCODE < */
        // unzulaessig
        /* EasyCODE > */
    case ENDLOS :
        // 030412
        /* EasyCODE - */
        SM3->apul++;
        if ( SM3->apul < 0xFFFF )
        {
        }
        else
        {
            T1R = 0;                   // Pulsabstands-Zaehler stoppen
            T1IE = 0;                  // Puls-Interrupt sperren
            _atomic_(0);
            SM3->stat &= ~RPLAEUFT;
            _endatomic_();
        }
        break;
    default:
        /* EasyCODE < */
        T1R = 0;                   // Pulsabstands-Zaehler stoppen
        T1IE = 0;                  // Puls-Interrupt sperren
        /* EasyCODE > */
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
smeingang */
/* EasyCODE F */
//*****************************************************************************************
// Dieses Programm muss vom Zeittakt aufgerufen werden.
// Bearbeitung der Eingangssignale fuer SM1, SM2 und SM3
// Entprellung nach folgender Formel :  eingp = Port
//                                      eing  = (eingv and eing)  or  eingp and (eingv  or eing)
//                                      eingv = eingp

void smeingang (void)
{
    eing11p = EING11;    // Port lesen SM1
    eing21p = EING21;
    eing11 = (eing11v & eing11) | (eing11p & (eing11v | eing11));
    eing21 = (eing21v & eing21) | (eing21p & (eing21v | eing21));
    eing11v = eing11p;
    eing21v = eing21p;
    SM1->eing = eing1;
    eing12p = EING12;    // Port lesen SM2
    eing22p = EING22;
    eing12 = (eing12v & eing12) | (eing12p & (eing12v | eing12));
    eing22 = (eing22v & eing22) | (eing22p & (eing22v | eing22));
    eing12v = eing12p;
    eing22v = eing22p;
    SM2->eing = eing2;
    eing13p = EING13;    // Port lesen SM3
    eing23p = EING23;
    eing13 = (eing13v & eing13) | (eing13p & (eing13v | eing13));
    eing23 = (eing23v & eing23) | (eing23p & (eing23v | eing23));
    eing13v = eing13p;
    eing23v = eing23p;
    SM3->eing = eing3;
}

/* EasyCODE ) */
/* EasyCODE ) */
