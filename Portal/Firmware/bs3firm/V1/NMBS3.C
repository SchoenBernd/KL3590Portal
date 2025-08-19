/* EasyCODE V8 */
/* EasyCODE ( 0 */
/*********************************************************************************************/
/* Datei:        nmbs3.c                                                                     */
/* Ersteller:    Edi Bastian   ESE 2856                                                      */
/*                                                                                           */
/* In dieser Datei befinden sich die Programme fuer das Hauptantriebs-Interface.             */
/*   pos_overflow ..... internes Unterprogramm fuer Zaehlerueberlauf bei den Handradpulsen   */
/*   nminit ........... Init fuer Schnittstelle, Drehzahlmessung, Pulsauswertung             */
/*   nmsbef ........... Bearbeitung des Auftrages fuer die Schnittstelle                     */
/*   ASC_viIsrError ... Interrupt bei Fehler der Schnittstelle                               */
/*   ASC_viIsrRx ...... Empfangsinterrupt                                                    */
/*   ASC_viIsrTxBuf ... Sendeinterrupt (Sendepuffer)                                         */
/*   nmdbef ........... Bearbeitung des Auftrages fuer die Drehzahlmessung                   */
/*   GT2_viIsrCAPREL .. Interrupt fuer Messzeit abgelaufen (keine Funktion, da nur PEC)      */
/*   GT2_viIsrTmr5 .... Interrupt bei Zaehlerueberlauf der Drehzahlmessung                   */
/*   nmpbef ........... Bearbeitung des Auftrages fuer die Pulsauswertung                    */
/*   nmnocken ......... zyklischer Teil der Pulsauswertung                                   */
/*   INT_viIsrEx0 ..... Interrupt fuer Handradpulse                                          */
/*   INT_viIsrEx1 ..... Interrupt fuer Index                                                 */
/*                                                                                           */
/*********************************************************************************************/


/*********************************************************************************************/
/*                                                                                           */
/* Bemerkungen zur Schnittstelle :                                                           */
/*   - Beschreibung der Auftraege etc. siehe auch Uebersichtsblatt                           */
/*   - Nach Auftreten von Uebertragungsfehler oder Pufferueberlauf muss die Schnittstelle    */
/*     neu initialisiert werden, um den Fehler rueckzusetzen.                                */
/*                                                                                           */
/* Bemerkungen zur Drehzahlmessung :                                                         */
/*   - Beschreibung der Auftraege etc. siehe auch Uebersichtsblatt                           */
/*   - Nach Auftreten von Zaehlerueberlauf muss die Drehzahlerfassung neu initialisiert      */
/*     werden, um den Fehler rueckzusetzen.                                                  */
/*                                                                                           */
/* Bemerkungen zur Pulsauswertung :                                                          */
/*   - Beschreibung der Auftraege etc. siehe auch Uebersichtsblatt                           */
/*   - Im Initialisierungsprogramm wird die Pulsauswertung gestoppt, POSMSK und Nockenport   */
/*     werden Null gesetzt, AKTPOS wird ungueltig gesetzt.                                   */
/*   - Auftrag 0 (Init und Start):                                                           */
/*       AKTPOS wird ungueltig gesetzt; falls Sonderfunktion Nocken 8 ein, wird Nocken 8     */
/*       Null gesetzt; falls Nockenschaltwerk ein, wird Nockenport entsprechend POSMSK ge-   */
/*       setzt (POSMSK muss in diesem Fall entsprechend vorbelegt sein).                     */
/*   - Auftrag 1 (Stopp):                                                                    */
/*       AKTPOS wird ungueltig gesetzt; POSMSK und Nockenport bleiben unveraendert.          */
/*   - Vor Auftrag 0 (Init und Start) muss die Pulsauswertung gestoppt sein.                 */
/*   - Auftrag 2 (Umschaltung Modus) kann durchgefuehrt werden, ohne die Pulsauswertung vor- */
/*     her zu stoppen.                                                                       */
/*   - Sonderfunktion Nocken 8:                                                              */
/*       Nocken 8 wird nach STBN8-Umdrehungen fuer 2 bis 4ms gesetzt.                        */
/*   - Nockenschaltwerk geometrisch:                                                         */
/*       Nocken werden bei jedem Handradpuls aktualisiert; Positionszaehler wird auf Ueber-  */
/*       lauf kontrolliert.                                                                  */
/*   - Nockenschaltwerk zyklisch:                                                            */
/*       Nocken werden im Zyklus von 2ms aktualisiert; Positionszaehler wird im Zyklus von   */
/*       2ms auf Ueberlauf kontrolliert.                                                     */
/*   - Positionszaehler-Ueberlauf bei Positionszaehler < -HPULSE oder Positionszaehler>HPULSE*/
/*     Die Pruefung erfolgt nur bei eingeschaltetem Nockenschaltwerk.                        */
/*   - Wenn Sonderfunktion Nocken 8 eingeschaltet ist, wird der Nocken 8 nicht vom Nocken-   */
/*     schaltwerk auf den Port ausgegeben, sondern nur von der Sonderfunktion. In POSMSK     */
/*     wird der Nocken 8 weiterhin aus POSTAB gesetzt, falls das Nockenschaltwerk einge-     */
/*     schaltet ist.                                                                         */
/*   - Falls die Handradposition unbekannt ist oder nach Zaehlerueberlauf erfolgt bis zum    */
/*     naechsten Index keine Aktion.                                                         */
/*                                                                                           */
/*********************************************************************************************/


#include "mainbs3.h"
#include "dprbs3.h"

// Daten
unsigned char bdata control;           // Funktionsmerker
   sbit nocken8 = control^0;           //   Sonderfunktion Nocken 8 ein
   sbit nocken_zykl = control^1;       //   Nockenschaltwerkmodus zyklisch ein
   sbit nocken_geom = control^2;       //   Nockenschaltwerkmodus geometrisch ein
unsigned char bdata stb_stbpaw;        // Zwischenspeicher fuer Steuerbyte Pulsauswertung
   sbit stb_nock8 = stb_stbpaw^0;      //   Sonderfunktion Nocken 8 ein
   sbit stb_nockein = stb_stbpaw^1;    //   Nockenschaltwerk ein
   sbit stb_nock_geom = stb_stbpaw^2;  //   Nockenschaltwerkmodus geometrisch ein
   sbit stb_richtung = stb_stbpaw^3;   //   Zaehlrichtung Pulse
unsigned char bdata pos_posmsk;        // Zwischenspeicher fuer aktuellen Nockenwert
   sbit pos_nock1 = pos_posmsk^0;      //   Nocken 1
   sbit pos_nock2 = pos_posmsk^1;      //   Nocken 2
   sbit pos_nock7 = pos_posmsk^6;      //   Nocken 7
   sbit pos_nock8 = pos_posmsk^7;      //   Nocken 8
unsigned char sdata index_cnt;         // Index-Zaehler fuer Sonderfunktion Nocken 8
unsigned int sdata nocken8_cnt;
/* EasyCODE ( 0 
pos_overflow */
/* EasyCODE F */
// Impulsdauer-Zaehler fuer Sonderfunktion Nocken 8



//********************************************************************************
// internes Unterprogramm fuer Zaehlerueberlauf beim Pulszaehler fuer die Position (T6)

void near pos_overflow (void)
{
    _atomic_(0);
    CC9IE = 0;                       // Interrupt/PEC fuer Index sperren
    CC8IE = 0;                       // Interrupt/PEC fuer Pulse sperren
    _endatomic_();
    T6R = 0;                         // T6 (Pulszaehler) stoppen
    NM->aktpos = 0x8001;             // aktuelle Position auf Ueberlauf setzen
    PECC7 = 0x0001;                  // bei Index - PEC und Interrupt
    CC9IR = 0;                       // Interrupt-Anforderung fuer Index loeschen
    CC9IE = 1;                       // Interrupt/PEC fuer Index freigeben
}

/* EasyCODE ) */
/* EasyCODE ( 0 
nminit */
/* EasyCODE F */
//****************************************************************************
// Initialisierungen fuer das Hauptantriebs-Interface

void nminit (void)
{
    // Schnittstelle, Hardware
    S0CON = 0x00E7;      // 8-bit + parity, 1 stop bit, receiver disable, parity check,
                         // framing check, overrun check, errorflags=0, even parity,
                         // divide clock by reload-value + constant, kein loopback,
                         // baudrate generator disable
    S0TBIC = 0x0029;     // transmit buffer interrupt disable,
                         //                           priority level ILVL=10,
                         //                           group level GLVL=1
    S0RIC = 0x002A;      // receive interrupt disable,
                         //                   priority level ILVL=10,
                         //                   group level GLVL=2
    S0EIC = 0x002B;      // error interrupt disable,
                         //                 priority level ILVL=10,
                         //                 group level GLVL=3
    P3 |= 0x0400;        // set P3.10 output latch (TXD0)
    DP3 |= 0x0400;       // set P3.10 direction control (TXD0 output)
    DP3 &= 0xF7FF;       // reset P3.11 direction control (RXD0 input)
    _nop_();
    P3 |= 0x0010;        // /ENRS485 = 1 im output latch (RS422)
    DP3 |= 0x0010;       // /ENRS485 Ausgang (set P3.4 direction control)
    _nop_();
    P3 &= 0xFFF7;        // /ENTX = 0 im output latch (Sender aktivieren)
    DP3 |= 0x0008;       // /ENTX Ausgang (set P3.3 direction control)
    _nop_();
    
    // Schnittstelle, DPR
    NM->rbufst = 0;      // Ringpuffer
    NM->rbufrd = 0;
    NM->rbufwr = 0;
    NM->sbufrd = 0;
    NM->sbufwr = 0;
    NM->sbef = 0xFF;     // Auftrag ruecksetzen
    
    // Drehzahlmessung, Hardware
    CAPREL = 0;          // Initialisierungen fuer CAPREL und T5 (Zaehlung Pulse x 4)
    DP3 &= 0xFFFB;       //   P3.2 direction control, CAPIN input
    _nop_();
    CRIC = 0x0038;       //   CAPREL interrupt disable
                         //                    priority level ILVL=14
                         //                    group level GLVL=0
    PECC0 = 0x00FF;      //   PEC channel 0 for GPT2 CAPREL interrupt
                         //     continuous transfer, word transfer, pointers not modified
    SRCP0 = 0xFE4A;      //     source pointer
    DSTP0 = (unsigned int)(&NM->count); // destination pointer
    T5CON = 0xD009;      //   T5: counter mode, pos. transition on T5IN, T5 stopped, count up,
                         //       pos. transition on CAPIN, clear on capture, capture enable
    T5 = 0;              //       counter=0
    T5IC = 0x0037;       //   T5 interrupt (overflow) disable
                         //                           priority level ILVL=13
                         //                           group level GLVL=3
    PWMCON0 &= 0x7777;   // Initialisierungen fuer PWM 3 (Messzeit)
    PWMCON0 |= 0x0080;   //   timer stopped, clock/64, interrupt disable, no interrupt request
    PWMCON1 &= 0x7F77;   //   output enable, mode 0 (standard PWM generation)
    PWMCON1 |= 0x0008;
    P7 &= 0xFFF7;        //   output P7.3, not inverted
    DP7 |= 0x0008;
    _nop_();
    
    // Drehzahlmessung, DPR
    NM->count = 0xFFFF;  // Zaehler ungueltig setzen
    NM->dbef = 0xFF;     // Auftrag ruecksetzen
    
    // Pulsauswertung, Hardware
    T6CON = 0x0109;      // Initialisierungen fuer T6 (Zaehlung Pulse)
                         //   counter mode, pos. transition on T6IN, T6 stopped, up/down reset,
                         //   external up/down, no alternate output function, output toggle latch=0,
                         //   no reload from CAPREL
    T6 = 0;              //   counter=0
    P8 &= 0xFF3C;        // Initialisierungen fuer Nockenport P8
    DP8 |= 0x00C3;       //   P8.0 - Nocken 1, P8.1 - Nocken 2, P8.6 - Nocken 7, P8.7 - Nocken 8
                         //   jeweils output mit Pegel=0
    _nop_();
    EXICON &= 0xFFF0;    // Initialisierungen fuer EX0 (Pulse) und EX1 (Index)
    EXICON |= 0x0005;    //   EX0: external interrupt 0, pos. edge at pin P2.8
                         //   EX1: external interrupt 1, pos. edge at pin P2.9
    DP2 &= 0xFCFF;       //   P2.8 input, P2.9 input
    _nop_();
    CC8IC = 0x003E;      //   EX0: interrupt disable
                         //                  priority level ILVL=15
                         //                  group level GLVL=2
    PECC6 = 0x0001;      //        use PEC channel 6
                         //          count=1 (vorlaeufig), word transfer, pointers not modified
    SRCP6 = 0xFE48;      //          source pointer = T6
    DSTP6 = (unsigned int)(&NM->aktpos); // destination pointer
    CC9IC = 0x003F;      //   EX1: interrupt disable
                         //                  priority level ILVL=15
                         //                  group level GLVL=3
    PECC7 = 0x0001;      //        use PEC channel 7
                         //          count=1 (vorlaeufig), word transfer, pointers not modified
    SRCP7 = (unsigned int)(&ZEROS); // source pointer = ZEROS
    DSTP7 = 0xFE48;      //          destination pointer = T6
    
    // Pulsauswertung, DPR
    NM->posmsk = 0;      // Nockenwert=0
    NM->aktpos = 0x8000; // aktuelle Position ungueltig
    NM->pbef = 0xFF;     // Auftrag ruecksetzen
}

/* EasyCODE ) */
/* EasyCODE ( 0 
nmsbef */
/* EasyCODE F */
//****************************************************************************
// Bearbeitung des Auftrages fuer die serielle Schnittstelle

void nmsbef (void)
{
    unsigned char rptr;
    unsigned char mod;
    switch ( NM->sbef )
    {
    // Schnittstelle initialisieren und starten
    case 0 :
        S0EIE = 0;                       // Interrupts sperren
        S0RIE = 0;
        S0TBIE = 0;
        S0CON &= 0x78EF;                 // Baudrate Gen. und Empfaenger sperren, Errorflags=0
        mod = NM->sermod;
        switch ( mod & 0x03 )
        {
        // Baudrate setzen
        case 0 :
            S0BG = 0x103;              //    2400 Baud
            break;
        case 1 :
            S0BG = 0x81;               //    4800 Baud
            break;
        case 2 :
            S0BG = 0x40;               //    9600 Baud
            break;
        case 3 :
            S0BG = 0x20;               //    19200 Baud
            break;
        }
        if ( mod & 0x04 )
        {
            S0PEN = 1;
            /* EasyCODE - */
            // Parity enable bearbeiten
        }
        else
        {
            S0PEN = 0;
        }
        if ( mod & 0x08 )
        {
            S0ODD = 1;
            /* EasyCODE - */
            // Parity odd/even bearbeiten
        }
        else
        {
            S0ODD = 0;
        }
        NM->rbufst = 0;                  // Ringpuffer im DPR initialisieren
        NM->rbufrd = 0;
        NM->rbufwr = 0;
        NM->sbufrd = 0;
        NM->sbufwr = 0;
        S0CON &= 0xF8FF;                 // Errorflags=0
        S0RIR = 0;                       // Empfaengerinterrupt freigeben
        S0RIE = 1;
        S0EIR = 0;                       // Fehlerinterrupt freigeben
        S0EIE = 1;
        S0TIR = 1;                       // transmit register empty
        S0CON |= 0x8010;                 // Baudrate Gen. und Empfaenger freigeben
        break;
        // Schnittstelle stoppen
    case 1 :
        S0EIE = 0;                 // Interrupts sperren
        S0RIE = 0;
        S0TBIE = 0;
        S0CON &= 0x78EF;           // Baudrate Gen. und Empfaenger sperren, Errorflags ruecksetzen
        break;
        // Sendeauftrag
    case 2 :
        if ( !S0TBIE )
        {
            // falls Sendeinterrupt enable: keine Aktion
            rptr = NM->sbufrd;
            if ( rptr!=NM->sbufwr )
            {
                // falls Senderingpuffer nicht leer:
                S0TBIR = 0;                //    transmit buffer empty ruecksetzen
                S0TBUF = NM->sbuf[rptr];   //    Zeichen in transmit buffer
                rptr++;                    //    read pointer inkr. mod 16
                if ( rptr>15 )
                {
                    rptr = 0;
                }
                NM->sbufrd = rptr;
                S0TBIE = 1;                //    Sendeinterrupt freigeben
            }
        }
        break;
    default:
        break;
    }
    NM->sbef = 0xFF;              // Auftrag ruecksetzen
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ASC_viIsrError */
/* EasyCODE F */
//****************************************************************************
// @Function      void ASC_viIsrError(void) interrupt S0EINT
//
//----------------------------------------------------------------------------
// @Description   This is the error interrupt service routine for the ASC0.
//                It is called if during receipt of a data byte a framing,
//                parity or overflow error occured (only if enabled).
//****************************************************************************
// Die Interruptanforderung dieses Interrupts wird simultan zum Empfaengerinterrupt gesetzt,
// falls ein Fehler auftrat. Er sollte die gleiche Interruptprioritaet, aber eine hoehere
// Gruppenprioritaet als der Empfangsinterrupt haben, damit die Fehlermeldung unmittelbar vor
// dem empfangenen Zeichen im DPR eingetragen wird.
// Nach Auftreten eines solchen Fehlers muss die Schnittstelle neu initialisiert werden, um den
// Fehler rueckzusetzen !!!

void ASC_viIsrError(void) interrupt S0EINT
{
    P7_7 = 1;               // Performance-Messung
    NM->rbufst |= 0x01;
    S0EIE = 0;              // Interrupts sperren
    S0RIE = 0;
    S0TBIE = 0;
    S0CON &= 0x78EF;        // Baudrate Gen. und Empfaenger sperren, Errorflags ruecksetzen
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ASC_viIsrRx */
/* EasyCODE F */
//****************************************************************************
// @Function      void ASC_viIsrRx(void) interrupt S0RINT
//
//----------------------------------------------------------------------------
// @Description   This is the receive interrupt service routine for the ASC0.
//                It is called if a byte has been received via ASC0
//                (S0RIR is set).
//****************************************************************************
// Das empfangene Zeichen wird im DPR-Empfangsringpuffer abgelegt, wobei ein Pufferueberlauf
// als Fehler gemeldet wird.
// Nach Auftreten eines solchen Fehlers muss die Schnittstelle neu initialisiert werden, um den
// Fehler rueckzusetzen !!!

void ASC_viIsrRx(void) interrupt S0RINT
{
    unsigned char wptr;
    
       P7_7 = 1;                                    // Performance-Messung
       wptr = NM->rbufwr;
       NM->rbuf[wptr] = (unsigned char) S0RBUF;     // Zeichen in Empfangsringpuffer
       wptr++;                                      // write pointer inkr. mod 16
    if ( wptr>15 )
    {
        wptr = 0;
    }
    NM->rbufwr = wptr;
    if ( wptr==NM->rbufrd )
    {
        NM->rbufst |= 0x02;
    }
    // geg. Pufferueberlauf setzen
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ASC_viIsrTxBuf */
/* EasyCODE F */
//****************************************************************************
// @Function      void ASC_viIsrTxBuf(void) interrupt S0TBINT
//
//----------------------------------------------------------------------------
// @Description   This is the transmit buffer interrupt service routine for
//                the ASC0. It is called if the content of the TX-buffer has
//                been loaded into the TX-shift register.
//****************************************************************************
// Das naechste zu sendende Zeichen wird aus dem DPR-Senderingpuffer geholt und in den transmit
// buffer geschrieben. Ist der Senderingpuffer leer, wird der Interrupt gesperrt.

void ASC_viIsrTxBuf(void) interrupt S0TBINT
{
    unsigned char rptr;
    
       P7_7 = 1;                           // Performance-Messung
       rptr = NM->sbufrd;
    if ( rptr!=NM->sbufwr )
    {
        // falls Senderingpuffer nicht leer:
        S0TBUF = NM->sbuf[rptr];         //    Zeichen in transmit buffer
        rptr++;                          //    read pointer inkr. mod 16
        if ( rptr>15 )
        {
            rptr = 0;
        }
        NM->sbufrd = rptr;
    }
    else
    {
        S0TBIE = 0;
    }
    // falls Senderingpuffer leer: Interrupt sperren
}

/* EasyCODE ) */
/* EasyCODE ( 0 
nmdbef */
/* EasyCODE F */
//****************************************************************************
// Bearbeitung des Auftrages fuer die Drehzahlmessung

void nmdbef (void)
{
    switch ( NM->dbef )
    {
    // Drehzahlmessung initialisieren und starten
    case 0 :
        CRIE = 0;                  // Interrupt/PEC fuer Drehzahlwert sperren
        T5IE = 0;                  // T5 overflow interrupt sperren
        PTR3 = 0;                  // PWM 3 (Messzeit) stoppen
        T5R = 0;                   // T5 (Pulszaehler) stoppen
        NM->count = 0xFFFF;        // Zaehler im DPR ungueltig setzen
        PT3 = 0;                   // PWM 3 (Messzeit) laden
        PP3 = PW3 = NM->messzt-1;
        T5 = 0;                    // T5 (Pulszaehler) = 0
        CRIR = 0;                  // Interrupt/PEC-Request fuer Drehzahlwert loeschen
        T5IR = 0;                  // T5 overflow request loeschen
        CRIE = 1;                  // Interrupt/PEC fuer Drehzahlwert freigeben
        T5IE = 1;                  // T5 overflow interrupt freigeben
        T5R = 1;                   // T5 (Pulszaehler) starten
        PTR3 = 1;                  // PWM 3 (Messzeit) starten
        break;
        // Drehzahlmessung stoppen
    case 1 :
        CRIE = 0;                  // Interrupt/PEC fuer Drehzahlwert sperren
        T5IE = 0;                  // T5 overflow interrupt sperren
        PTR3 = 0;                  // PWM 3 (Messzeit) stoppen
        T5R = 0;                   // T5 (Pulszaehler) stoppen
        NM->count = 0xFFFF;        // Zaehler im DPR ungueltig setzen
        break;
    default:
        break;
    }
    NM->dbef = 0xFF;                 // Auftrag ruecksetzen
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GT2_viIsrCAPREL */
/* EasyCODE F */
//****************************************************************************
// @Function      void GT2_viIsrCAPREL(void) interrupt CRINT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT2 capture/ 
//                reload register CAPREL. The CAPREL register has been 
//                configured into capture mode and a capture event is
//                at pin P3.2.
//****************************************************************************
// Der Capture-Wert wird per PEC ins DPR transferiert. Die Interrupt-Routine wird nie aufgerufen.

void GT2_viIsrCAPREL(void) interrupt CRINT
{
    P7_7 = 1;               // Performance-Messung
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GT2_viIsrTmr5 */
/* EasyCODE F */
//****************************************************************************
// @Function      void GT2_viIsrTmr5(void) interrupt T5INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT2 timer 5. 
//                It is called when over or underflow of the timer 5
//                register occurs.
//****************************************************************************
// Ein Ueberlauf bei T5 ist ein Fehler. Die Messzeit ist fuer die Drehzahl zu gross.

void GT2_viIsrTmr5(void) interrupt T5INT
{
    P7_7 = 1;                  // Performance-Messung
    CRIE = 0;                  // Interrupt/PEC fuer Drehzahlwert sperren
    T5IE = 0;                  // T5 overflow interrupt sperren
    PTR3 = 0;                  // PWM 3 (Messzeit) stoppen
    T5R = 0;                   // T5 (Pulszaehler) stoppen
    NM->count = 0xFFFE;        // Zaehler im DPR auf Ueberlauf setzen
}

/* EasyCODE ) */
/* EasyCODE ( 0 
nmpbef */
/* EasyCODE F */
//****************************************************************************
// Bearbeitung des Auftrages fuer die Pulsauswertung

void nmpbef (void)
{
    switch ( NM->pbef )
    {
    // Pulsauswertung initialisieren und starten
    case 0 :
        _atomic_(0);
        CC9IE = 0;                 // Interrupt/PEC fuer Index sperren
        CC8IE = 0;                 // Interrupt/PEC fuer Pulse sperren
        _endatomic_();
        T6R = 0;                   // T6 (Pulszaehler) stoppen
        NM->aktpos = 0x8000;       // aktuelle Position ungueltig
        control = 0;
        stb_stbpaw = NM->stbpaw;
        if ( stb_nock8 )
        {
            nocken8_cnt = 0;        // Sonderfunktion Nocken 8
            nocken8 = 1;
            index_cnt = NM->stbn8;
            P8_7 = 0;
        }
        T6UD = stb_richtung;       // Zaehlrichtung Pulse setzen
        if ( stb_nockein )
        {
            pos_posmsk = NM->posmsk;   // Nockenschaltwerk ein:
            if ( nocken8 )
            {
                //   Nockenport bearbeiten
                _atomic_(0);
                P8_0 = pos_nock1;       //     ohne Nocken 8
                P8_1 = pos_nock2;
                P8_6 = pos_nock7;
                _endatomic_();
            }
            else
            {
                _atomic_(0);
                P8_0 = pos_nock1;       //     mit Nocken 8
                P8_1 = pos_nock2;
                P8_6 = pos_nock7;
                P8_7 = pos_nock8;
                _endatomic_();
            }
            if ( stb_nock_geom )
            {
                nocken_geom = 1;        //   geometrisch
                PECC6 = 0x0001;         //     bei Pulse - PEC und Interrupt
            }
            else
            {
                nocken_zykl = 1;        //   zyklisch
                PECC6 = 0x00FF;         //     bei Pulse - nur PEC, kein Interrupt
            }
        }
        else
        {
            PECC6 = 0x00FF;         // Nockenschaltwerk aus: bei Pulse - nur PEC, kein Interrupt
        }
        PECC7 = 0x0001;            // bei Index - PEC und Interrupt
        CC9IR = 0;                 // Interrupt-Anforderung fuer Index loeschen
        CC9IE = 1;                 // Interrupt/PEC fuer Index freigeben
        break;
        // Pulsauswertung stoppen
    case 1 :
        _atomic_(0);
        CC9IE = 0;                 // Interrupt/PEC fuer Index sperren
        CC8IE = 0;                 // Interrupt/PEC fuer Pulse sperren
        _endatomic_();
        T6R = 0;                   // T6 (Pulszaehler) stoppen
        control = 0;               // damit der zyklische Programmteil keine Aktionen durchfuehrt
        NM->aktpos = 0x8000;       // aktuelle Position ungueltig
        break;
        // Umschaltung Nockenschaltwerkmodus
        //   Hierbei wird die Pulsauswertung nicht gestoppt. Dieser Auftrag kann jederzeit gegeben
        //   werden.
    case 2 :
        stb_stbpaw = NM->stbpaw;
        if ( stb_nockein )
        {
            if ( stb_nock_geom )
            {
                nocken_zykl = 0;
                nocken_geom = 1;
                PECC6 = 0x0001;         // bei Pulse - PEC und Interrupt
            }
            else
            {
                nocken_geom = 0;
                nocken_zykl = 1;
                PECC6 = 0x00FF;         // bei Pulse - nur PEC, kein Interrupt
            }
        }
        else
        {
            nocken_geom = 0;
            nocken_zykl = 0;
            PECC6 = 0x00FF;            // bei Pulse - nur PEC, kein Interrupt
        }
        break;
    default:
        break;
    }
    NM->pbef = 0xFF;              // Auftrag ruecksetzen
}

/* EasyCODE ) */
/* EasyCODE ( 0 
nmnocken */
/* EasyCODE F */
//*****************************************************************************************
// Dieses Programm muss vom Zeittakt aufgerufen werden.
// Bei Sonderfunktion Nocken 8:  Ruecksetzen des Nocken 8
// Bei Nockenschaltwerkmodus zyklisch: Pruefen der aktuellen Position
//                                     Bearbeiten des aktuellen Nockenwertes und Nockenports

void nmnocken (void)
{
    int aktpos;       // Zwischenspeicher fuer aktuelle Position
    if ( nocken8 && (nocken8_cnt!=0) )
    {
        // Nocken 8 bearbeiten
        if ( --nocken8_cnt==0 )
        {
            P8_7 = 0;
        }
    }
    if ( nocken_zykl && CC8IE )
    {
        // Positionspruefung und Nockenport bearbeiten
        aktpos = NM->aktpos;
        if ( aktpos >= 0 )
        {
            if ( aktpos > HPULSE )
            {
                //   aktuelle Position >= 0
                pos_overflow();
                return ;
            }
        }
        else
        {
            aktpos += HPULSE;                   //   aktuelle Position < 0
            if ( aktpos < 0 )
            {
                pos_overflow();
                return ;
            }
        }
        pos_posmsk = NM->posmsk = NM->postab[aktpos];  // Nockenwert
        if ( nocken8 )
        {
            //   Nockenport bearbeiten
            _atomic_(0);
            P8_0 = pos_nock1;             //     ohne Nocken 8
            P8_1 = pos_nock2;
            P8_6 = pos_nock7;
            _endatomic_();
        }
        else
        {
            _atomic_(0);
            P8_0 = pos_nock1;             //     mit Nocken 8
            P8_1 = pos_nock2;
            P8_6 = pos_nock7;
            P8_7 = pos_nock8;
            _endatomic_();
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
INT_viIsrEx0 */
/* EasyCODE F */
//****************************************************************************
// @Function      void INT_viIsrEx0(void) interrupt CC8INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the external
//                interrupt 0 (P2.8).
//                It is called when the configured event is captured.
//****************************************************************************
// Externer Interrupt 0 (EX0) fuer Handradpulse.
// Der zugehoerige PEC wird ausgefuehrt, falls Pulsauswertung nicht gestoppt und Position bekannt.
// Der Interrupt wird nur aufgerufen, falls Pulsauswertung nicht gestoppt, Position bekannt und
// das Nockenschaltwerk eingeschaltet und geometrisch bearbeitet wird.



void INT_viIsrEx0(void) interrupt CC8INT
{
    int aktpos;       // Zwischenspeicher fuer aktuelle Position
    
       P7_7 = 1;                           // Performance-Messung
       PECC6 = 0x0001;                     // bei Pulse - PEC und Interrupt
       aktpos = NM->aktpos;
    if ( aktpos >= 0 )
    {
        if ( aktpos > HPULSE )
        {
            //   aktuelle Position >= 0
            pos_overflow();
            return ;
        }
    }
    else
    {
        aktpos += HPULSE;                   //   aktuelle Position < 0
        if ( aktpos < 0 )
        {
            pos_overflow();
            return ;
        }
    }
    pos_posmsk = NM->posmsk = NM->postab[aktpos];  // Nockenwert
    if ( nocken8 )
    {
        //   Nockenport bearbeiten
        _atomic_(0);
        P8_0 = pos_nock1;             //     ohne Nocken 8
        P8_1 = pos_nock2;
        P8_6 = pos_nock7;
        _endatomic_();
    }
    else
    {
        _atomic_(0);
        P8_0 = pos_nock1;             //     mit Nocken 8
        P8_1 = pos_nock2;
        P8_6 = pos_nock7;
        P8_7 = pos_nock8;
        _endatomic_();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
INT_viIsrEx1 */
/* EasyCODE F */
//****************************************************************************
// @Function      void INT_viIsrEx1(void) interrupt CC9INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the external
//                interrupt 1 (P2.9).
//                It is called when the configured event is captured.
//****************************************************************************
// Externer Interrupt 1 (EX1) fuer Index.
// Der zugehoerige PEC wird ausgefuehrt, falls Pulsauswertung nicht gestoppt.
// Der Interrupt wird nur aufgerufen, falls Pulsauswertung nicht gestoppt und Position unbekannt
// oder die Sonderfunktion Nocken 8 eingeschaltet ist.

void INT_viIsrEx1(void) interrupt CC9INT
{
    P7_7 = 1;                  // Performance-Messung
    if ( !CC8IE )
    {
        // Interrupt/PEC fuer Pulse gesperrt ?
        CC8IR = 0;              // Interrupt/PEC fuer Pulse gesperrt: Interrupt-Anf. Pulse loeschen
        T6R = 1;                //   T6 (Pulszaehler) starten
        NM->aktpos = 0;
        if ( nocken_geom )
        {
            //   falls Nockenschaltwerk geometrisch, Nocken bearbeiten
            pos_posmsk = NM->posmsk = NM->postab[0];
            if ( nocken8 )
            {
                _atomic_(0);
                P8_0 = pos_nock1;
                P8_1 = pos_nock2;
                P8_6 = pos_nock7;
                _endatomic_();
            }
            else
            {
                _atomic_(0);
                P8_0 = pos_nock1;
                P8_1 = pos_nock2;
                P8_6 = pos_nock7;
                P8_7 = pos_nock8;
                _endatomic_();
            }
        }
        CC8IE = 1;              //   Interrupt/PEC fuer Pulse freigeben
    }
    if ( nocken8 )
    {
        // falls Sonderfunktion Nocken 8 ein, Nocken 8 bearbeiten
        PECC7 = 0x0001;         //   bei Index - PEC und Interrupt
        if ( --index_cnt==0 )
        {
            index_cnt = NM->stbn8;
            P8_7 = 1;
            #ifdef FIRM480
                nocken8_cnt = 5;     //   Signaldauer des Nocken 8: 4-5 Zeittakte (8-10ms)
            #else //   Code war gleich aber anderer Kommentar
                nocken8_cnt = 5;     //   Signaldauer des Nocken 8: 1-2 Zeittakte (2-4ms)
            #endif 
        }
    }
    else
    {
        PECC7 = 0x00FF;
    }
    // sonst, bei Index - nur PEC, kein Interrupt
}

/* EasyCODE ) */
/* EasyCODE ) */
