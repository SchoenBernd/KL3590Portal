/* EasyCODE V8 */
/* EasyCODE ( 0 
hpportal, Hauptprogramm */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
/* EasyCODE < */
// Datei:          hpportal
// Klasse:         Kl3590 Portal
// Ersteller:      Halfmann   EWE 2856

#include "portal.h"

// Funktionsprototypen
void koor_handler (void);
void zeit_handler (void);
void abl_handler (void);
void io_handler (void);
void screen_handler (void);
void keyb_handler (void);
void nminf_handler (void);
void eing_handler (void);
void nis_handler (void);
void ref2_handler (void);
void schabl_handler (void);
//void sminf_handler (void);


void pfail_cont (void);
void timer (void);
void IRQ7_handler (void);
void IRQ5_handler (void);
void catcher_ABRT (int);
void catcher_FPE (int);
void catcher_ILL (int);
void catcher_SEGV (int);
void catcher_TERM (int);

// Daten
int blink_cnt;
unsigned char blinkdi;
jmp_buf jumper;
struct safdat_struct *saf;
struct prodstruct *prod;
Mailbox anzbox;
MemoryPool textpool;
unsigned char *datdir;   // Directory für Naehdaten
unsigned int dprseg, safseg;
int emsseg;
struct text *tz;

// initialisierte Daten
taskdata tdini[ANZTASK] = {
//Wert  Intervall     Prioritaet         Semaphore     Task-Handler
1,          1,        KOOR_PRIOR,           0,         koor_handler,  
2,          2,        ZEIT_PRIOR,           0,         zeit_handler,  
1,          1,        ABL_PRIOR,            0,         abl_handler, 
2,          2,        IO_PRIOR,             0,         io_handler,  
1,          1,        SCREEN_PRIOR,         0,         screen_handler, 
1,          1,        KEYB_PRIOR,           0,         keyb_handler, 
1,          1,        NMINF_PRIOR,          0,         nminf_handler,
1,          2,        EING_PRIOR,           0,         eing_handler,
1,          1,        NIS_PRIOR,            0,         nis_handler,
1,          1,        REF2_PRIOR,           0,         ref2_handler,
1,          1,        SCHABL_PRIOR,         0,         schabl_handler
//2,          2,        SMINF_PRIOR,          0,         sminf_handler
};
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE ( 0 
main */
/* EasyCODE F */
// Hauptprogramm
void main ()
{
    /* EasyCODE < */
    TaskHandle mainhandle;
    unsigned int i;
    
    datdir = getenv ("DATDRV");                         // Directory fuer Naehdaten aus Environment lesen
    i = axtoi(getenv("SAFSEG"));
    safseg = i;
    saf = (struct safdat_struct *) MK_FP (i,0);
    dprseg = i + 0x700;
    
    prod =  (struct prodstruct *) MK_FP (i + 0x600, 0);  // 4095 Byte bis zum SAF ende sind fuer prod verfügbar.
                                                         // Eigentlich errechnet sich die Adresse wie folgt:
                                                         // MK_FP(i,0x6000) jedoch sollte aus philosophischen
                                                         // Gruenden der Offset bei 0000 beginnen, da man 
                                                         // prod als neues Datensegment ansehen kann.
                                                         // Physikalisch ist die absolute Adresse jedoch gleich.
    emsseg = axtoi(getenv("EMSSEG"));
    
    // Fenster fÝr netzausfallsichere Daten definieren
    outportb (SAFADR, i >> 8);
    
    // C167 noch nicht betriebsbereit
    C167_READY=0;
    
    // Error-Handler installieren
    signal (SIGABRT,catcher_ABRT);  // abnormal termination
    signal (SIGFPE,catcher_FPE);    // arithmetik error
    signal (SIGILL,catcher_ILL);    // illegal operation
    signal (SIGSEGV,catcher_SEGV);  // segment violation
    signal (SIGTERM,catcher_TERM);  // request for program termination
    saf->faterror=setjmp(jumper);
    /* EasyCODE > */
    if ( saf->faterror!=0 )
    {
        // hier wird das Programm nach einem der oben genannten
        //   Fehler fortgesetzt
        pfail_cont();
    }
    /* EasyCODE < */
    // Kaltstart-Pruefung und geg. Durchfuehrung
    /* EasyCODE > */
    if ( strcmp(softversion,saf->testtext) )
    {
        // Testtext nicht ok
        saf->faterror = 51;
        saf->kaltstart=TRUE;
    }
    else
    {
        if ( saf->kaltstart )
        {
            // Funktion Kaltstart (Service)
            saf->faterror = 52;
        }
        else
        {
            if ( saf->safdatsum_gueltig )
            {
                // Checksumme der netzausfallsicheren
                //   Daten prüfen
                i=saf->safdatsum;
                safdat_check();
                if ( i!=saf->safdatsum )
                {
                    saf->faterror = 50;
                    saf->kaltstart=TRUE;
                }
            }
        }
    }
    saf->safdatsum_gueltig=FALSE;
    eing.par.parameterinit_enable = TRUE;
    if ( saf->kaltstart )
    {
        // Kalt-Inits (Reihenfolge nicht veraendern!)
        koor.kinit();
        io.kinit();
        eing.kinit();
        nminf.kinit();
        abl.kinit();
        hfuss.kaltinit();
        // Testtext neu schreiben
        strcpy (saf->testtext,softversion);
        saf->kaltstart=FALSE;
    }
    // Warm-Inits (Reihenfolge nicht veraendern!)
    koor.winit();
    io.winit();
    eing.winit();
    abl.winit();
    /* EasyCODE - */
    RTKAllocMemPool (&textpool,TEXTLENGTH,1);
    if ( (tz=(struct text *)RTKGetBuffer(&textpool))==NULL )
    {
        // kein Buffer zugewiesen
        textload_err=3;
    }
    else
    {
        textload ();
    }
    mainhandle = RTKernelInit(MAX_PRIO);
    anzbox = RTKCreateMailbox(sizeof(&eing_botsch1),ANZBOTSCH+2,"Anzeige Box");
    TimerInit();
    RTComInit();
    com1_com3_init();
    InitPort (COM2,9600,PARITY_ODD,1,8);
    SetProtocol (COM2,RTSCTS,2,1);  // HW Handshake enable
    EnableFIFO (COM2,0);            // 0 = FIFO buffer disabled.
    AllocateCOMBuffers (COM2,RWLAENGE,RWLAENGE); // Allocate receive and send buffers for the port
    EnableCOMInterrupt (COM2,1);
    com2breakon(); // BREAK auf com2 setzen
    RTKPreemptionsON();
    RTKDiskIntsON();
    /* EasyCODE ( 0 
    Tasks installieren */
    for ( i=0; i< ANZTASK; i++ )
    {
        switch ( i )
        {
        case KoorIndex :
            koor.inst();
            break;
        case ZeitIndex :
            zeit.inst();
            break;
        case AblIndex :
            abl.inst();
            break;
        case IoIndex :
            io.inst();
            break;
        case ScreenIndex :
            screen.inst();
            break;
        case KeybIndex :
            keyb.inst();
            break;
        case NmInfIndex :
            nminf.inst();
            break;
        case EingIndex :
            eing.inst();
            break;
        case NisIndex :
            nis.inst();
            break;
        case Ref2Index :
            ref2.inst();
            break;
        case SchablIndex :
            schabl.inst();
            break;
        /* EasyCODE_STRUCTURED_COMMENT_START */
        /*
        case SMInfIndex :
            sminf.inst();
            break;
        */
        /* EasyCODE_STRUCTURED_COMMENT_END */
        default:
            break;
        }
    }
    /* EasyCODE ) */
    /* EasyCODE < */
    RTKCreateTask (timer,TIMER_PRIOR,1024,"Timer");
    RTKSetIRQHandler (5,IRQ5_handler);
    RTKSetIRQHandler (7,IRQ7_handler);
    outportb (INTMASK, REF1DI | REF2DI | PFDI | DPRDI | P2DI | P1DI);
    RTKEnableIRQ(5);
    RTKEnableIRQ(7);
    SetTimerInterval (0.005);
    blink_cnt=0;
        
    // Warten bis Power-fail-Signal ok
    /* EasyCODE > */
    while ( !pwrgood() )
    {
        ;
    }
    /* EasyCODE < */
    RTKEnableIRQ(5);
    EnableCOMInterrupt (COM2,100);
    outportb (INTCLR,PF_SETCLR);
    outportb (INTCLR,PF_RESCLR);
    outportb (INTMASK,PFEI);
    RTKSetPriority (mainhandle,MAIN_PRIOR);
        
    // Hintergrundprogramm
    /* EasyCODE > */
    while ( TRUE )
    {
        led1on();
        led1off();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
koor_handler */
/* EasyCODE F */
void koor_handler (void)
{
    while ( 1 )
    {
        RTKWait (koor.td.s);
        koor.koortask();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
zeit_handler */
/* EasyCODE F */
void zeit_handler (void)
{
    while ( 1 )
    {
        RTKWait (zeit.td.s);
        zeit.zeittask();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
abl_handler */
/* EasyCODE F */
void abl_handler (void)
{
    while ( 1 )
    {
        RTKWait (abl.td.s);
        abl.abltask();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
io_handler */
/* EasyCODE F */
void io_handler (void)
{
    while ( 1 )
    {
        RTKWait (io.td.s);
        io.iotask();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
screen_handler */
/* EasyCODE F */
void screen_handler (void)
{
    while ( 1 )
    {
        RTKWait (screen.td.s);
        screen.screentask();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
keyb_handler */
/* EasyCODE F */
void keyb_handler (void)
{
    while ( 1 )
    {
        RTKWait (keyb.td.s);
        keyb.keybtask();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
nminf_handler */
/* EasyCODE F */
void nminf_handler (void)
{
    while ( 1 )
    {
        RTKWait (nminf.td.s);
        nminf.nminftask();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
eing_handler */
/* EasyCODE F */
void eing_handler (void)
{
    while ( 1 )
    {
        RTKWait (eing.td.s);
        eing.eingtask();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
nis_handler */
/* EasyCODE F */
void nis_handler (void)
{
    while ( 1 )
    {
        RTKWait (nis.td.s);
        nis.nistask();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ref2_handler */
/* EasyCODE F */
void ref2_handler (void)
{
    while ( 1 )
    {
        RTKWait (ref2.td.s);
        ref2.ref2task();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
schabl_handler */
/* EasyCODE F */
void schabl_handler (void)
{
    while ( 1 )
    {
        RTKWait (schabl.td.s);
        schabl.schabltask();
    }
}

/* EasyCODE ) */
/* EasyCODE_STRUCTURED_COMMENT_START */
/*
EasyCODE_COMMENT_START EasyCODE ( 0 
sminf_handler EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
void sminf_handler (void)
{
    while ( 1 )
    {
        RTKWait (sminf.td.s);
        sminf.sminftask();
    }
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
*/
/* EasyCODE_STRUCTURED_COMMENT_END */
/* EasyCODE ( 0 
timer */
/* EasyCODE F */
void timer (void)
{
    unsigned char i;
    while ( 1 )
    {
        RTKDelay (1);
        if ( !blinkdi )
        {
            if ( (blink_cnt--)<0 )
            {
                led2on();
                blink_cnt=199;
            }
            if ( blink_cnt==99 )
            {
                led2off();
            }
        }
        /* EasyCODE ( 0 
        Werte der Tasks dekrementieren */
        for ( i=0; i< ANZTASK; i++ )
        {
            switch ( i )
            {
            case KoorIndex :
                koor.dekr();
                break;
            case ZeitIndex :
                zeit.dekr();
                break;
            case AblIndex :
                abl.dekr();
                break;
            case IoIndex :
                io.dekr();
                break;
            case ScreenIndex :
                screen.dekr();
                break;
            case KeybIndex :
                keyb.dekr();
                break;
            case NmInfIndex :
                nminf.dekr();
                break;
            case EingIndex :
                eing.dekr();
                break;
            case SchablIndex :
                schabl.dekr();
                break;
            default:
                break;
            }
        }
        /* EasyCODE ) */
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
IRQ5_handler */
/* EasyCODE F */
// High-Level-Interrupt-Handler
void IRQ5_handler (void)
{
    /* EasyCODE < */
    // Interrupt-Quelle feststellen:
        //   Ref1, Ref2 oder Power Fail
    /* EasyCODE > */
    do{
        if ( inportb(STATIN) & PF_MSK )
        {
            /* EasyCODE ( 0 
            Power Fail */
            if ( inportb(PEGELIN) & PF_MSK )
            {
                /* EasyCODE < */
                // Power fail
                RTKDisableInterrupts();
                pfail_cont(); // laeuft in Halt
                /* EasyCODE > */
            }
            else
            {
                outportb (INTCLR,PF_SETCLR);
                outportb (INTCLR,PF_RESCLR);
            }
            /* EasyCODE ) */
        }
        else
        {
            if ( inportb (STATIN) & REF1_MSK )
            {
                /* EasyCODE ( 0 
                Nis */
                // Ref1 bzw. Nis
                outportb (INTMASK,REF1DI); // Ref1 sperren
                outportb (INTCLR,REF1_SETCLR);
                outportb (INTCLR,REF1_RESCLR);
                if ( nae.cutact )
                {
                    nae.cutreq=TRUE;
                }
                else
                {
                    outportb (LPTADR,inportb(LPTADR) | 0x02);
                    nis.start ();
                    outportb (LPTADR,inportb(LPTADR) & ~0x02);
                }
                RTKSignal (nis.td.s);
                /* EasyCODE ) */
            }
            else
            {
                if ( inportb(STATIN) & REF2_MSK )
                {
                    /* EasyCODE ( 0 
                    Ref2 */
                    // Ref2
                    outportb (INTMASK,REF2DI); // Ref2 sperren
                    outportb (INTCLR,REF2_SETCLR);
                    outportb (INTCLR,REF2_RESCLR);
                    RTKSignal (ref2.td.s);
                    /* EasyCODE ) */
                }
                else
                {
                    outportb (INTCLR,DPR_SETCLR);
                    outportb (INTCLR,DPR_RESCLR);
                }
            }
        }
    }while ( (inportb(STATIN) & (REF1_MSK | REF2_MSK | PF_MSK | DPR_MSK))!=0 );
    RTKDisableInterrupts();
    RTKEndOfInterrupt();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
IRQ7_handler */
/* EasyCODE F */
// Default-Interrupt
// High-Level-Interrupt-Handler
void IRQ7_handler (void)
{
    RTKEndOfInterrupt();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
pfail_cont */
/* EasyCODE F */
// Unterprogramm zur Fortsetzung der Power-fail-Routine
// Achtung: das Programm endet im Halt !!
void pfail_cont (void)
{
    RTKDisableInterrupts();
    // Break fuer Bedienfeld
    com2breakon();
    if ( (inportb(STATIN) & RESET_MSK)!=0 )
    {
        outportb (RES167OUT,RESDATA1);
        outportb (RES167OUT,RESDATA2);
        outportb (RES167OUT,RESDATA3);
    }
    /* EasyCODE < */
    // Checksumme der netzausfallsicheren Daten berechnen
        safdat_check();
        saf->safdatsum_gueltig=TRUE;
        
        // Endlosschleife (quasi Halt)
    /* EasyCODE > */
    while ( TRUE )
    {
        led1on();
        led1off();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
catcher_ABRT */
/* EasyCODE F */
// Error-Handler

// abnormal termination
void catcher_ABRT (int)
{
    longjmp (jumper,1);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
catcher_FPE */
/* EasyCODE F */
// arithmetik error
void catcher_FPE (int)
{
    longjmp (jumper,2);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
catcher_ILL */
/* EasyCODE F */
// illegal operation
void catcher_ILL (int)
{
    longjmp (jumper,1);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
catcher_SEGV */
/* EasyCODE F */
// segment violation
void catcher_SEGV (int)
{
    longjmp (jumper,1);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
catcher_TERM */
/* EasyCODE F */
// request for program termination
void catcher_TERM (int)
{
    longjmp (jumper,1);
}

/* EasyCODE ) */
/* EasyCODE ) */
