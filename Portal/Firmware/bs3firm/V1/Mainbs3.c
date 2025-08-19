/* EasyCODE V8 */
/* EasyCODE ( 0 */
/*********************************************************************************************/
/* Datei:        mainbs3.c                                                                   */
/* Ersteller:    Edi Bastian   ESE 2856                                                      */
/*                                                                                           */
/* In dieser Datei befinden sich folgende Programme:                                         */
/*   main ............. Hauptprogramm                                                        */
/*   INT_viIsrEx2 ..... DPR-Interrupt (anstehender Auftrag)                                  */
/*   CC2_viIsrTmr8 .... Zeittakt-Interrupt                                                   */
/*   NMI_Trap ......... Non-Maskable Interrupt (Hardware Trap)                               */
/*   STKOF_Trap ....... Stack Overflow (Hardware Trap)                                       */
/*   STKUF_Trap ....... Stack Underflow (Hardware Trap)                                      */
/*   CLASSB_Trap ...... Class B (Hardware Trap)                                              */
/*                                                                                           */
/*********************************************************************************************/

#include "mainbs3.h"
#include "SMBS3.h"
#include "dprbs3.h"

// Daten

unsigned char hpfkey;   // Huepferfusskey



// externe Programme
extern void sm1stop (void);
extern void sm2stop (void);
extern void sm3stop (void);
extern void sm1bef (void);
extern void sm2bef (void);
extern void sm3bef (void);
extern void nmsbef (void);
extern void nmpbef (void);
extern void nmdbef (void);
extern void nmnocken (void);
extern void smeingang (void);
extern void CANTask (void);
extern void nminit (void);
extern void sminit (void);
extern void CANInit (void);


// Prototypen
void far ResRoutine(void);
/* EasyCODE ( 0 
main */
/* EasyCODE F */
//****************************************************************************
// @Function      void main(void)
//
//----------------------------------------------------------------------------
// @Description   This is the main function.
//
//****************************************************************************
// Hauptprogramm mit Initialisierungen und Hintergrundprogramm

void main(void)
{
    int aktpos;       // portal Zwischenspeicher fuer aktuelle Position
    //    int startab_tmp;
    /* EasyCODE ( 0 
    DPR Reset Routine */
    unsigned char i;
    unsigned char xhuge * srcptr;
    
       /// --- initialization of the SYSCON Register ---
       /// the following initialization is made in the Start.asm
       ///- the internal ROM area is mapped to segment 0
       ///- the segmentation is enabled (CSP is saved/restored during interrupt entry/exit)
       ///- the internal ROM is disabled: accesses to the ROM area use the external bus
       ///- the pin #BHE (byte high enable) is enabled
       ///- the system clock output is enabled
       ///- the pins #WR and #BHE retain their normal functions
       ///- the on-chip X-Peripherals are enabled and can be accessed
       ///- accesses to the XBUS peripherals are done internally
       ///- external accesses to the XBUS peripherals are disabled
    
       /// --- initialization of the BUSCON 0-4 and ADRRSEL Registers 1-4 ---
       /// for the Keil Compiler the initialization is made in the Start.asm
    
       /// ---------- external bus 0 is enabled ----------
       ///- 8-bit demultiplexed bus
       ///- memory cycle time control: asynchronous READY signal, 0  waitstates
       ///- read/write delay control: activate signal 1 TCL after falling edge of ALE  
       ///- signal configuration: enable READY signal 
       ///- chip select mode: address chip select 
       ///- memory tristate control: no waitstate
       ///- ALE lengthening control: normal ALE signal
    
       /// ---------- external bus 1 is enabled ----------
       ///- 16-bit demultiplexed bus
       ///- memory cycle time control: 0 waitstates
       ///- read/write delay control: activate signal 1 TCL after falling edge of ALE  
       ///- signal configuration: disable READY signal 
       ///- chip select mode: address chip select 
       ///- memory tristate control: no waitstate
       ///- ALE lengthening control: normal ALE signal
    
       // Fang-Routine fuer unzulaessigen Reset eintragen
       srcptr = (unsigned char xhuge *) ResRoutine;
    for ( i=0; i<32; i++ )
    {
        * (DPR_ResRoutine + i) = * (srcptr + i);
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Init HW */
    * ((unsigned char *)DPRVectorTable + 1) = 0;
    * (DPRVectorTable + 1) = (int) DPR_ResRoutine;
    C167_ERR = 0;
    
    // initializes the IO ports
    // Die IO-Pins, die bestimmten Funktionsgruppen zugeordnet sind, werden in deren Init-
    // Programmen initialisiert.
    _PICON = 0x0000;      // standard TTL input levels
    
    /// no pin of port 0 low is used as general IO
    /// no pin of port 0 high is used as general IO
    /// no pin of port 1 low is used as general IO
    /// no pin of port 1 high is used as general IO
    
    //  initializes port 2
    /// P2.11 Reserve, vorlaeufig input
    /// P2.12 Reserve, vorlaeufig input
    P2   = 0x0000;      // set port data register
    ODP2 = 0x0000;      // set port open drain control register
    DP2  = 0x0000;      // set port direction register
    _nop_();
    
    //  initializes port 3
    /// P3.1 Reserve, vorlaeufig output
    P3   = 0x0000;      // set port data register
    ODP3 = 0x0000;      // set port open drain control register
    DP3  = 0x0002;      // set port direction register
    _nop_();
    
    //  initializes port 4
    /// P4.4 Reserve, vorlaeufig output
    /// P4.7 Reserve, vorlaeufig output
    P4   = 0x0000;      // set port data register
    DP4  = 0x0090;      // set port direction register
    _nop_();
    
    /// P5.6 Reserve, input
    /// P5.7 Reserve, input
    /// P5.8 Reserve, input
    /// P5.9 Reserve, input
    /// P5.11 Reserve, input
    /// P5.14 Reserve, input
    /// P5.15 Reserve, input
    
    /// no pin of port 6 is used as general IO
    
    //  initializes port 7
    /// P7.7 Performance-Messung, output
    P7   = 0x0000;      // set port data register
    ODP7 = 0x0000;      // set port open drain control register
    DP7  = 0x0080;      // set port direction register
    _nop_();
    
    //  initializes port 8
    P8   = 0x0000;      // set port data register
    ODP8 = 0x0000;      // set port open drain control register
    _nop_();
    
    // initializes the external interrupts
    EXICON = 0;
    
    // Initialisierungen fuer DPR-Interrupt (EX2)
    EXICON &= 0xFFCF;       // EX2: external interrupt 2, neg. edge at pin P2.10
    EXICON |= 0x0020;
    DP2 &= 0xFBFF;          // P2.10 input
    _nop_();
    CC10IC = 0x002F;        // EX2: interrupt disable
                            //                proirity level ILVL=11
                            //                group level GLVL=3
    
    // Initialisierungen fuer Zeittakt-Interrupt (Timer 8)
    T78CON &= 0x00FF;       // timer mode, CLK/8, timer stopped
    T78CON |= 0x0000;
    T8 = -0x1388;           // load timer 8 register (Zeittakt 2ms)
    T8REL = -0x1388;        // load timer 8 reload register (Zeittakt 2ms)
    T8IC = 0x0027;          // T8: interrupt disable
                            //               proirity level ILVL=9
                            //               group level GLVL=3
    
    // Initialisierung fuer Hauptantrieb-Interface
    nminit();
    
    // Initialisierung fuer Schrittmotor-Interface
    sminit();
    
    // Initialisierung fuer CAN
    CANInit();
    
    // enable interrupts
    i = PC104_WR;           // DPR-Interrupt-Request ruecksetzen
    CC10IR = 0;             // DPR-Interrupt-Request loeschen
    CC10IE = 1;             // DPR-Interrupt freigeben
    T8IR = 0;               // Zeittakt-Interrupt-Request loeschen
    T8IE = 1;               // Zeittakt-Interrupt freigeben
    T8R = 1;                // Zeittakt-Zaehler starten
    IEN = 1;                // generelles enable
    
    // weitere Initialisierungen fuer DPR
    C167_WATCH = 0xFF;         // Watchdog-Meldung fuer PC104
    C167_READY = 0xFF;
    /* EasyCODE ) */
    hpfkey = 0;
    /* EasyCODE - */
    // portal
    
    // Power on ready
    
    // Hintergrundprogramm
    while ( 1 )
    {
        P7_7 = 0;            // Performance-Messung
        //_nop_();   portal
        /* EasyCODE - */
        /*
        Huepferfussablauf:
        Wenn ENABLE sitzt, dann wird auf die Positionen startauf und startab gepollt
        und wenn erreicht, dann wird abhaengig von istpos der Huepferfuss automatisch gefahren.
        Wenn Enable sitzt, dann darf der PC104 nicht auf das SM1 Interface schreiben !!!
        Falls zufälligerweise ein DPR Interrupt vom PC104 dann kommt wegen SM2 oder SM3 waehrend 
        das ENABLE sitzt, dann ist sm1bef in hier in der BS3 Firmware verriegelt zur Vermeidung 
        von unerlaubten Mehrfachaufrufen.
        */
        if ( SMH->ena )
        {
            do{
                aktpos = NM->aktpos;
            }while ( aktpos != NM->aktpos );
            /* EasyCODE_STRUCTURED_COMMENT_START */
            /*
            EasyCODE_COMMENT_START EasyCODE ( 0 
            wenn alles geht, diesen Block endgueltig loeschen EasyCODE_COMMENT_END
            if ( SMH->startab < SMH->startauf )
            {
                // Ueberlauf der Startup Position
                startab_tmp = SMH->startab + HPULSE;
            }
            else
            {
                // Kein Ueberlauf
                startab_tmp = SMH->startab;
            }
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
            */
            /* EasyCODE_STRUCTURED_COMMENT_END */
            switch ( hpfkey )
            {
            case 0 :
                // Pollen auf Startposition Huepferfuss auf
                /* EasyCODE ( 0 
                Huepferfuss auf */
                if ( // Normalfall (vermeiden dass nach startab sofort startauf ausgefuehrt wird)
                     ((SMH->startab >= SMH->startauf)  && (aktpos >= SMH->startauf) && (aktpos < SMH->startab) )   
                                     
                                                       ||
                     
                     // Startab Ueberlauf ueber 255 Handradpulse
                     ((SMH->startab < SMH->startauf)  && (aktpos >= SMH->startauf)  ) )
                {
                    if ( (SMH->posoben > SMH->istpos) )
                    {
                        SM1->mod = SMH->smzplus;
                    }
                    else
                    {
                        SM1->mod = SMH->smzminus;
                    }
                    //SMH->mod = (SMH->mod & RICHTUNG) | PAWRAMPPLL | PAWSVZPLL; 
                    // nicht nötig, da sowieso der Rest 0x00 ist.
                    /* EasyCODE - */
                    SM1->adr = abs (SMH->posoben - SMH->istpos);
                    SM1->bef = 1;
                    sm1bef();
                    SMH->istpos = SMH->posoben;
                    hpfkey = 2;
                }
                /* EasyCODE ) */
                break;
            case 2 :
                // Pollen auf Startposition Huepferfuss ab
                /* EasyCODE ( 0 
                Huepferfuss ab */
                if ( // Normalfall
                     ((SMH->startab >= SMH->startauf)  && (aktpos >= SMH->startab))   
                                                       
                                                       || 
                     
                     // Normalfall mit moeglichem Ueberlauf (aktpos schon ueber 255)
                     ((SMH->startab >= SMH->startauf)  && (aktpos > 0) && (aktpos < SMH->startauf) )   
                     
                                     
                                                       ||
                     
                     // Startab Ueberlauf über 255 Handradpulse
                     ((SMH->startab < SMH->startauf)  && (aktpos >= SMH->startab) && (aktpos < SMH->startauf) ) )
                {
                    if ( SM1->stat & (RPLAEUFT | SVLAEUFT) )
                    {
                        // Im Falle Ramp not finised
                        // dennoch nach unten fahren
                        // damit die Nadelstange nicht den 
                        // Huepferfuss beschaedigt auch wenn
                        // die Huepferfuss Position dann nicht genau 
                        // stimmt.
                        sm1stop();
                        SMH->fehler = 1;
                    }
                    if ( (SMH->posunten > SMH->istpos)  )
                    {
                        SM1->mod = SMH->smzplus;
                    }
                    else
                    {
                        SM1->mod = SMH->smzminus;
                    }
                    //SMH->mod = (SMH->mod & RICHTUNG) | PAWRAMPPLL | PAWSVZPLL; 
                    // nicht nötig, da sowieso der Rest 0x00 ist.
                    /* EasyCODE - */
                    SM1->adr = abs (SMH->posunten - SMH->istpos);
                    SM1->bef = 1;
                    sm1bef();
                    SMH->istpos = SMH->posunten;
                    hpfkey = 0;
                }
                /* EasyCODE ) */
                break;
            default:
                break;
            }
        }
        else
        {
            hpfkey = 0;
            SMH->fehler = 0;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
INT_viIsrEx2 */
/* EasyCODE F */
//****************************************************************************
// @Function      void INT_viIsrEx2(void) interrupt CC10INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the external
//                interrupt 2 (P2.10).
//                It is called when the configured event is captured.
//****************************************************************************
// Dieser Interrupt wird vom PC104 ueber das DPR ausgeloest, um Auftraege fuer die verschiedenen
// Einheiten ausfuehren zu lassen.
// Der Interrupt-Request wird am Anfang rueckgesetzt, damit ein weiterer Auftrag einen erneuten
// Request ausloesen kann und somit nicht verloren gehen kann.

void INT_viIsrEx2(void) interrupt CC10INT using DPRBANK
{
    unsigned char dummy;
    
       P7_7 = 1;                  // Performance-Messung
       dummy = PC104_WR;          // Interrupt-Request ruecksetzen
    if ( SM1->stop != 0xFF )
    {
        sm1stop();
    }
    if ( SM2->stop != 0xFF )
    {
        sm2stop();
    }
    if ( SM3->stop != 0xFF )
    {
        sm3stop();
    }
    if ( (SM1->bef != 0xFF)
               &&
         (!SMH->ena)  // portal
       )
    {
        sm1bef();
    }
    else
    {
        /*   !SMH->ena   portal
         
        Im Fall wenn der Huepferfussablauf selbst sm1bef aufruft, darf ein zufälliger Interrupt für andere
        Achsen nicht sm1bef nochmal aufrufen. Darum hier die Verriegelung mit dem neuen Enable Flag.
        
        */
    }
    if ( SM2->bef != 0xFF )
    {
        sm2bef();
    }
    if ( SM3->bef != 0xFF )
    {
        sm3bef();
    }
    if ( NM->sbef != 0xFF )
    {
        nmsbef();
    }
    if ( NM->pbef != 0xFF )
    {
        nmpbef();
    }
    if ( NM->dbef != 0xFF )
    {
        nmdbef();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
CC2_viIsrTmr8 */
/* EasyCODE F */
//****************************************************************************
// @Function      void CC2_viIsrTmr8(void) interrupt T8INT
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM timer 8.
//                It is called when the timer 8 register overflows.
//****************************************************************************
// Zeittakt-Interrupt

void CC2_viIsrTmr8(void) interrupt T8INT using ZEITBANK
{
    P7_7 = 1;                  // Performance-Messung
    nmnocken();
    smeingang();
    CANTask();
    C167_WATCH = 0xFF;         // Watchdog-Meldung fuer PC104
}

/* EasyCODE ) */
/* EasyCODE ( 0 
NMI_Trap */
/* EasyCODE F */
//****************************************************************************
// Non-Maskable Interrupt (Hardware Trap)
// nicht zulaessig - schwerer Fehler

void NMI_Trap(void) interrupt 0x02
{
    IEN = 0;
    T0R = 0;          // damit keine SM-Pulse mehr kommen koennen - SM1
    T7R = 0;          //                                          - SM2
    T1R = 0;          //                                          - SM3
    C167_ERR = NMI_Error;
    while ( 1 )
    {
        _nop_();       // Warten auf Ausschalten
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
STKOF_Trap */
/* EasyCODE F */
//****************************************************************************
// Stack Overflow (Hardware Trap)
// nicht zulaessig - schwerer Fehler

void STKOF_Trap(void) interrupt 0x04
{
    IEN = 0;
    T0R = 0;          // damit keine SM-Pulse mehr kommen koennen - SM1
    T7R = 0;          //                                          - SM2
    T1R = 0;          //                                          - SM3
    C167_ERR = STKOF_Error;
    while ( 1 )
    {
        _nop_();       // Warten auf Ausschalten
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
STKUF_Trap */
/* EasyCODE F */
//****************************************************************************
// Stack Underflow (Hardware Trap)
// nicht zulaessig - schwerer Fehler

void STKUF_Trap(void) interrupt 0x06
{
    IEN = 0;
    T0R = 0;          // damit keine SM-Pulse mehr kommen koennen - SM1
    T7R = 0;          //                                          - SM2
    T1R = 0;          //                                          - SM3
    C167_ERR = STKUF_Error;
    while ( 1 )
    {
        _nop_();       // Warten auf Ausschalten
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
CLASSB_Trap */
/* EasyCODE F */
//****************************************************************************
// Class B (Hardware Trap)
// nicht zulaessig - schwerer Fehler

void CLASSB_Trap(void) interrupt 0x0A
{
    IEN = 0;
    T0R = 0;          // damit keine SM-Pulse mehr kommen koennen - SM1
    T7R = 0;          //                                          - SM2
    T1R = 0;          //                                          - SM3
    C167_ERR = CLASSB_Error;
    while ( 1 )
    {
        _nop_();       // Warten auf Ausschalten
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ResRoutine */
/* EasyCODE F */
//****************************************************************************
// Fang-Routine fuer unzulaessigen Reset
// Da nach einem Reset der Code-Bereich 0x10000 nicht direkt erreicht werden kann, wird diese
// Routine in das DPR geladen und der Reset-Vektor entsprechend veraendert. Es werden 32 Bytes
// geladen, d.h. diese Routine darf maximal 32 Bytes Code erzeugen !

void far ResRoutine(void)
{
    IEN = 0;
    T0R = 0;          // damit keine SM-Pulse mehr kommen koennen - SM1
    T7R = 0;          //                                          - SM2
    T1R = 0;          //                                          - SM3
    C167_ERR = RES_Error;
    while ( 1 )
    {
        _nop_();       // Warten auf Ausschalten
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
