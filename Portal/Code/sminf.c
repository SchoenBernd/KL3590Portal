/* EasyCODE V8 */
/* EasyCODE ( 0 
Schrittmotor-Interface */
// Programm:       Schrittmotor-Interface
// Klasse:         3590 Portal mit Basisteuerung 3 und Bedienfeld P1
// Ersteller:      Halfmann  EWE 2856
//
#include "portal.h"
KlasseSminfBS3 smz(FP_OFF(SMZ));
KlasseSminfBS3 smx(FP_OFF(SMX));
KlasseSminfBS3 smy(FP_OFF(SMY));
/* EasyCODE ( 0 
KlasseSminfBS3::init()
Vorteiler und PLL-Faktor initialisieren */
/* EasyCODE F */
void KlasseSminfBS3::init(void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    dprptr->vort = vort;
    dprptr->bef = 0;
    PC104_WR = 1;   // Interrupt bei C167 ausl”sen
    outportb (PLLOUT, pll_faktor-1);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::start_Freq
Dauerfahren mit fester Frequenz
Parameter: Status, Rampenadresse */
/* EasyCODE F */
void KlasseSminfBS3::start_Freq (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    dprptr->mod = status;
        dprptr->adr = rampadr;
        dprptr->gesp = 0xffff;
        dprptr->bef = 3;   // Rampe mit Gesamtpulszahl ohne Startverz”gerung starten
        PC104_WR = 1;  // Interrupt bei C167 ausl”sen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::start_Pulse
best. Pulszahl mit fester Frequenz
Parameter: Status, Rampenadresse, Gesamtpulszahl */
/* EasyCODE F */
void KlasseSminfBS3::start_Pulse (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    dprptr->mod = status;
        dprptr->adr = rampadr;
        dprptr->gesp =gespls;
        dprptr->bef = 3;   // Rampe mit Gesamtpulszahl ohne Startverz”gerung starten
        PC104_WR = 1;  // Interrupt bei C167 ausl”sen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::start_Rampe
Rampe Fahren ohne Startverz”gerung
Parameter: Status, Rampenadresse */
/* EasyCODE F */
void KlasseSminfBS3::start_Rampe (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    dprptr->mod = status;
        dprptr->adr = rampadr;
        dprptr->bef = 1;   // Rampe ohne Startverzögerung starten
        PC104_WR = 1;  // Interrupt bei C167 auslösen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::start_Rampe_Verz
Rampe Fahren mit Startverz”gerung
Parameter: Status, Rampenadresse, Verz”gerung */
/* EasyCODE F */
void KlasseSminfBS3::start_Rampe_Verz (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    dprptr->mod = status;
        dprptr->adr = rampadr;
        dprptr->verz = verz;
        dprptr->bef = 2;   // Rampe mit Startverzögerung starten
        PC104_WR = 1;  // Interrupt bei C167 auslösen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::start_Rampe_Dach
Rampe Fahren mit Dachverl„ngerung ohne Startverz”gerung
Parameter: Status, Rampenadresse, Gesamtpulszahl, Pulszahl bis Dachende */
/* EasyCODE F */
void KlasseSminfBS3::start_Rampe_Dach (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    dprptr->mod = status;
        dprptr->adr = rampadr;
        dprptr->dachp = dachpls;
        dprptr->gesp = gespls;
        dprptr->bef = 4;   // Rampe mit Gesamtpulsz. und Dachverl„ngerung ohne Startverz”gerung starten
        PC104_WR = 1;  // Interrupt bei C167 auslösen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::start_Rampe_Dach_Verz
Rampe Fahren mit Dachverl„ngerung mit Startverz”gerung
Parameter: Status, Rampenadresse, Verz”gerung, Gesamtpulszahl, Pulszahl bis Dachende */
/* EasyCODE F */
void KlasseSminfBS3::start_Rampe_Dach_Verz (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    dprptr->mod = status;
        dprptr->adr = rampadr;
        dprptr->dachp = dachpls;
        dprptr->gesp = gespls;
        dprptr->verz = verz;
        dprptr->bef = 5;   // Rampe mit Gesamtpulsz. und Dachverl„ngerung mit Startverz”gerung starten
        PC104_WR = 1;  // Interrupt bei C167 auslösen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::stop
Schrittmotor stoppen
Parameter: keine */
/* EasyCODE F */
void KlasseSminfBS3::stop (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    dprptr->bef = 6;   // Bewegung stoppen
        PC104_WR = 1;  // Interrupt bei C167 auslösen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::ready
Fertigmeldung abfragen
Parameter: keine */
/* EasyCODE F */
unsigned char KlasseSminfBS3::ready (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    if ( (dprptr->stat & 0x03) == 0 )
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
KlasseSminfBS3::Pulse
ausgefhrte Pulse abfragen
Parameter: keine */
/* EasyCODE F */
unsigned int KlasseSminfBS3::Pulse (void)
{
    unsigned int i;
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    do{
        i = dprptr->apul;
    }while ( i != dprptr->apul );
    ausgebpls = i;
    return i;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::Fkt1_on
Funktion 1 ein, Enable fr Endstufe
Parameter: keine */
/* EasyCODE F */
void KlasseSminfBS3::Fkt1_on (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    dprptr->bef = 7;   // FKT1 ein
        PC104_WR = 1;  // Interrupt bei C167 auslösen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::Fkt1_off
Funktion 1 aus, Disable fr Endstufe
Parameter: keine */
/* EasyCODE F */
void KlasseSminfBS3::Fkt1_off (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    dprptr->bef = 8;   // FKT1 aus
        PC104_WR = 1;  // Interrupt bei C167 auslösen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::Fkt2_on
Funktion 2 ein, Softstepping ein
Parameter: keine */
/* EasyCODE F */
void KlasseSminfBS3::Fkt2_on (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    dprptr->bef = 9;   // FKT2 ein
        PC104_WR = 1;  // Interrupt bei C167 auslösen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::Fkt2_off
Funktion 2 aus, Softstepping aus
Parameter: keine */
/* EasyCODE F */
void KlasseSminfBS3::Fkt2_off (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    dprptr->bef = 10;   // FKT2 aus
        PC104_WR = 1;  // Interrupt bei C167 auslösen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::Status
Status DPR abfragen
Parameter: keine */
/* EasyCODE F */
unsigned char KlasseSminfBS3::Status (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    return dprptr->stat;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::Eing
Eing„nge abfragen, Eing1 ist Bereitsignal der Endstufe
Parameter: keine */
/* EasyCODE F */
unsigned char KlasseSminfBS3::Eing (void)
{
    return dprptr->eing;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::notstop
Parameter: keine */
/* EasyCODE F */
void KlasseSminfBS3::notstop (void)
{
    /* EasyCODE < */
    dprptr->stop = 0;
        PC104_WR = 1;   // Interrupt bei C167 ausl”sen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::start_Rampe_Errorcheck
nach Prfen der Fehlerbits Rampe Fahren ohne Startverz”gerung
Parameter: Status, Rampenadresse */
/* EasyCODE F */
unsigned char KlasseSminfBS3::start_Rampe_Errorcheck (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    if ( (dprptr->stat & 0x06) != 0 )
    {
        /* EasyCODE < */
        // Fehler Ramp not finished
        /* EasyCODE > */
        return FALSE;
    }
    else
    {
        /* EasyCODE < */
        dprptr->mod = status;
        dprptr->adr = rampadr;
        dprptr->bef = 1;// Rampe ohne Startverzoegerung starten
        PC104_WR = 1;   // Interrupt bei C167 ausloesen
        /* EasyCODE > */
        return TRUE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::start_Rampe_Verz_Errorcheck
nach Prfen der Fehlerbits Rampe Fahren mit Startverz”gerung
Parameter: Status, Rampenadresse, Verz”gerung */
/* EasyCODE F */
unsigned char KlasseSminfBS3::start_Rampe_Verz_Errorcheck (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    if ( (dprptr->stat & 0x06) != 0 )
    {
        /* EasyCODE < */
        // Fehler Ramp not finished
        /* EasyCODE > */
        return FALSE;
    }
    else
    {
        /* EasyCODE < */
        dprptr->mod = status;
        dprptr->adr = rampadr;
        dprptr->verz = verz;
        dprptr->bef = 2;// Rampe mit Startverz”gerung starten
        PC104_WR = 1;   // Interrupt bei C167 ausl”sen
        /* EasyCODE > */
        return TRUE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::start_dyn_Pulse
Fahren mit dynamischem Pulsabstand 
(nach Tastendruck kont. beschleunigen ohne Rampenauswahl)
Parameter: Status, gespls = aktueller Pulsabstand */
/* EasyCODE F */
void KlasseSminfBS3::start_dyn_Pulse (void)
{
    while ( (dprptr->bef != 0xff) || (dprptr->stop != 0xff) )
    {
        ;
    }
    dprptr->mod =  status;
    dprptr->gesp = gespls;  // hier verwendet als Pulsabstand
    dprptr->bef =  12;  // Pls Abstand (gesp) endlos fahren.
    PC104_WR = 1;      // Interrupt bei C167 ausl”sen
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSminfBS3::dynamic */
/* EasyCODE F */
void KlasseSminfBS3::dynamic (void)
{
    // Funktion wird alle 5 ms aufgerufen von abl.
    
    unsigned int nmottemp;
    unsigned int pls3tmp;
    switch ( key )
    {
    case 0 :
        /* EasyCODE ( 0 
        Idle (warten auf Start) */
        /* EasyCODE ( 0 
        Formeln */
        /* Herleitung Beschlfaktor:
        
        nmot und nmotstart (Vorgabe in 1/100 1/s) und alpha sind bezogen auf Drehzahl und nicht 
        auf Pulsfrequenz !!!!
        
        fmot ist bezogen auf Pulsfrequenz.
        
        
        
        v-t Diagramm der Wunschbewegung.
        
                                                                              
                    v                                                          
                    ^                                                          
                    |                                                          
          nmot      +	         +------------+                                
                    |	        /              \                               
                    |	       /phi2            \                              
          nmotstart +   +-----+                  \                             
                    |   |phi1                     \                            
                    |--	+-----+----------------+---+-------------> t           
                    	delay_ms                                               
        
        
        Eine Zeit t zuerst fahren mit konst. Freq. 
        =========================================
        
        1) PulsAbsMax = erzeuger / fmot;    
        2) fmot = PlsUmdr * nmotstart   
          2) in 1) =>   PulsAbsMax = erzeuger / (PlsUmdr * nmotstart)
                      ===============================================
        
        
        anzahl =  delay_ms / 5      //  anzahl sind die Anzahl Taskumläufe um ein Delay zu erzeugen
        ==========================
        
        
        
        Herleitung der Rotationswege (phi) bzw. Pulsanzahlen
        =====================================================
        
        1) phi1 = omega * t = 2 * pi * nmotstart * delay_ms/1000
        2) phi1 = plse1 * pi / 500  => plse1 = 500/pi * ( phi1 )
        
        2) in 1) 
        
        plse1 = 500/pi  *  (2 * pi * nmotstart * delay_ms/1000)  =  nmotstart * delay_ms 
        
        plse1 = nmotstart * delay_ms     fuer konst. Drehzahl bei start
        ============================
        
        
        phi2  = (omega hoch 2) / (2 * alpha)
        1) phi2  = ((2 * pi * nmot) hoch2) / ( 2 * alpha)
        2) phi2 = plse2 * pi / 500
        2) in 1) 
        plse2 = 500/pi *  ((2 * pi * nmot) hoch2) / ( 2 * alpha)
        
        plse2 = 3141.6 * (nmot hoch 2) / alpha.        gilt auch für pls2 analog
        =======================================
        
        
        
        double pow(double a, double b);
        
        
        
        
        Herleitung der dynamischen Rampenparameter
        ===========================================
        
        
           fmot =  erzeuger/PulsAbsMax   umgestellt nach PulsAbsMax
        1) PulsAbsMax = erzeuger / fmot;    
        2) omega = alpha * t  
           Bei uns ist t = Taskzyklus mal Anzahl Zyklen = 5msTakt * beschlcnt 
        
        =>  2 * pi * nmot = alpha * beschlcnt * 5msTakt  =>  nmot = alpha * beschlcnt * 5msTakt / (2 * Pi)
        
        3) fmot = PlsUmdr * nmot                         => nmot = fmot/PlsUmdr
        
        2) u. 3) in 1) einsetzen:
        PulsAbsMax = erzeuger * 2 * Pi / (PlsUmdr * alpha * beschlcnt * 5msTakt) ;     
        
                                                                   
        beschlfak = erzeuger * 2 * Pi * 1000 / (PlsUmdr * alpha)   // beschlcnt und 5ms kommen haeppchenweise dazu 
        ========================================                   // * 1000 wegen 5msTakt in ms.
        
        istPlsAbst = beschlfak / (5 * beschlcnt)                   // jetzt 5 und nicht 5ms da die 1000 in  beschlfak drin ist   
        ==============================================
        
        bremsfak =  (erzeuger * 2.0 * PI * 1000) / (PlsUmdr * a_brems  ) + 0.5 ;  // * 1000 wegen ms
        
        
        Nun fehlt uns noch beschlanz, also wann wir aufhoeren sollen zu beschleunigen:
        
        t = (2 * PI * nmot) / alpha;  // Zeit für die Rampe von 0 bis max. Speed
        
        beschlanz = t * 1000 / 5   //  Taskzeit (5ms) = 5 / 1000 s
        
        beschlanz = ((2 * PI * nmot) / alpha)  *  (1000 / 5) ;
        
        beschlanz = fmot/alpha  *  1256,64
        ==================================
        
        
        */
        /* EasyCODE ) */
        if ( dynbef == 1 )
        {
            //smx.erzeuger = 20000000 / (float)smx.vort;
            //smy.erzeuger = 20000000 / 64.0;   //EFR_CLK_64
            erzeuger = 20000000/64.0;
            
            istPlsAbst = erzeuger * 100.0 / (PlsUmdr * (float)nmotstart);
            beschlanz = (float)delay_ms / 5.0 + 1.0;
            beschlcnt = 0;
            
            gespls = 65535  -  istPlsAbst;
            start_dyn_Pulse(); 
            
            plse1 = (float)nmotstart/100.0 * (float)delay_ms;
            plse2 = 3141.6 * pow(((float)nmot/100.0) , 2) / (float)a_beschl + 1.0;
            plse3 = 3141.6 * pow(((float)nmot/100.0) , 2) / (float)a_brems + 1.0;
            
            rmpabbruch = FALSE;
            
            key = 1;
        }
        else
        {
            // Ruhe
        }
        /* EasyCODE ) */
        break;
    case 1 :
        /* EasyCODE ( 0 
        Langsam konstant fahren */
        if ( dynbef == 2 )
        {
            stop();
            key = 0;
        }
        else
        {
            if ( beschlcnt < beschlanz )
            {
                if ( plse  < (plse1 + 20) )
                {
                    if ( Pulse() >= plse )
                    {
                        dynbef = 0;   
                        stop();
                        key = 0;
                    }
                }
                else
                {
                    beschlcnt ++;
                }
            }
            else
            {
                beschlfak = (erzeuger * 2.0 * PI * 1000) / (PlsUmdr * a_beschl ) + 0.5 ;  // * 1000 wegen ms
                bremsfak =  (erzeuger * 2.0 * PI * 1000) / (PlsUmdr * a_brems  ) + 0.5 ;  // * 1000 wegen ms
                beschlanz = (float)nmot / (100 * a_beschl) * 1256.64  + 1.0;   // aufrunden nmot in 1/100
                bremsanz  = (float)nmot / (100 * a_brems) * 1256.64  + 1.0;    // aufrunden nmot in 1/100
                
                
                //Aus: istPlsAbst = beschlfak / (5 * beschlcnt) folgt:
                beschlcnt = beschlfak / (istPlsAbst * 5);
                
                key = 2;
            }
        }
        /* EasyCODE ) */
        break;
    case 2 :
        /* EasyCODE ( 0 
        Beschleunigen auf fmot (max Frequenz) */
        if ( dynbef == 2 )
        {
            beschlcnt = bremsfak / (istPlsAbst * 5);
            key = 4;
        }
        else
        {
            if ( beschlcnt < beschlanz )
            {
                beschlcnt ++;
                
                istPlsAbst = (beschlfak / (5 * beschlcnt));
                if ( istPlsAbst > 65000 )
                {
                    istPlsAbst   = 65000;
                }
                gespls = 65535  -  istPlsAbst;
                start_dyn_Pulse();             // Rampe fahren mit dyn. Pulsabstand
                /* EasyCODE - */
                nmottemp = erzeuger * 100.0 / (PlsUmdr * (float)istPlsAbst);
                pls3tmp = 3141.6 * pow(((float)nmottemp/100.0) , 2) / (float)a_brems + 1.0;
                if ( (plse)  > (Pulse() +  pls3tmp + 70) )
                {
                    // weiter beschl.
                }
                else
                {
                    beschlcnt = bremsfak / (istPlsAbst * 5);
                    rmpabbruch = TRUE;
                    dynbef = 0;
                    key = 4;
                }
            }
            else
            {
                key = 3;
            }
        }
        /* EasyCODE ) */
        break;
    case 3 :
        /* EasyCODE ( 0 
        Fahren mit konst fmot */
        if ( dynbef == 2 )
        {
            beschlcnt = bremsanz;
            key = 4;
        }
        else
        {
            if ( (plse)  > (Pulse() + plse3 + 70) )
            {
                // weiter fahren
            }
            else
            {
                beschlcnt = bremsanz;
                rmpabbruch = TRUE;
                dynbef = 0;
                key = 4;
            }
        }
        /* EasyCODE ) */
        break;
    case 4 :
        /* EasyCODE ( 0 
        Bremsen und Anhalten */
        if ( beschlcnt > 1 )
        {
            beschlcnt--;
            /* EasyCODE - */
            istPlsAbst = (bremsfak / (5 * beschlcnt));
            if ( istPlsAbst > 65000 )
            {
                istPlsAbst   = 65000;
            }
            gespls = 65535  -  istPlsAbst;
            start_dyn_Pulse();   // Rampe fahren mit dyn. Pulsabstand
        }
        else
        {
            if ( rmpabbruch == TRUE )
            {
                istPlsAbst = erzeuger * 100.0 / (PlsUmdr * (float)nmotstart);
                
                //smx.istPlsAbst /= 2; // Doppelt so schnell aufs Ziel fahren als mit 
                     // zielen
                
                //smx.istPlsAbst = 3120;
                istPlsAbst = 4000;
                
                
                gespls = 65535  -  istPlsAbst;
                start_dyn_Pulse();
                key = 5;
            }
            else
            {
                stop();
                dynbef = 0; 
                key = 0;
            }
        }
        /* EasyCODE ) */
        break;
    case 5 :
        /* EasyCODE ( 0 
        Restpulse fahren */
        if ( dynbef == 2 )
        {
            stop();
            key = 0;
        }
        else
        {
            if ( Pulse() >= plse )
            {
                dynbef = 0;   
                stop();
                key = 0;
            }
        }
        /* EasyCODE ) */
        break;
    case 6 :
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE_STRUCTURED_COMMENT_START */
/*
EasyCODE_COMMENT_START EasyCODE ( 0 
KlasseSminfBS3::dynamic EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
void KlasseSminfBS3::dynamic (void)
{
    // Task alle 5 ms aufgerufen vom abl.
    // Neue Task anlegen scheiterte wegen 
    // Linker fehler: Symboltabelle ueberlauf.
    
    unsigned int nmottemp;
    unsigned int pls3tmp;
    switch ( smxkey )
    {
    case 0 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Idle (warten auf Start) EasyCODE_COMMENT_END
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Formeln EasyCODE_COMMENT_END
        EasyCODE_COMMENT_START Herleitung Beschlfaktor:
        
        // nmot und nmotstart (Vorgabe in 1/100 1/s) und alpha sind bezogen auf Drehzahl und nicht 
        // auf Pulsfrequenz !!
        
        
        Eine Zeit t zuerst fahren mit konst. Freq. 
        =========================================
        
        1) PulsAbsMax = erzeuger / fmot;    
        2) fmot = PlsUmdr * nmotstart   
          2) in 1) =>   PulsAbsMax = erzeuger / (PlsUmdr * nmotstart)
                      ===============================================
        
        
        anzahl =  delay_ms / 5ms      //  anzahl sind die Anzahl Taskumläufe um ein Delay zu erzeugen
        ==========================
        
        
        
        Herleitung der Rotationswege (phi) bzw. Pulsanzahlen
        =====================================================
        
        1) phi1 = omega * t = 2 * pi * nmotstart * delay_ms/1000
        2) phi1 = plse1 * pi / 500
        
        2) in 1) 
        
        plse1 = 500/pi  *  (2 * pi * nmotstart * delay_ms/1000)  =  nmotstart * delay_ms 
        
        plse1 = nmotstart * delay_ms     fuer konst. Drehzahl bei start
        ============================
        
        
        phi2  = (omega hoch 2) / (2 * alpha)
        1) phi2  = ((2 * pi * nmot) hoch2) / ( 2 * alpha)
        2) phi2 = plse2 * pi / 500
        2) in 1) 
        plse2 = 500/pi *  ((2 * pi * nmot) hoch2) / ( 2 * alpha)
        
        plse2 = 3141.6 * (nmot hoch 2) / alpla.        gilt auch für pls2 analog
        =======================================
        
        
        
        double pow(double a, double b);
        
        
        
        
        
        
        Herleitung der dynamischen Rampenparameter
        ===========================================
        
        
           fmot =  erzeuger/PulsAbsMax   umgestellt nach PulsAbsMax
        1) PulsAbsMax = erzeuger / fmot;    
        2) omega = alpha * t  
           Bei uns ist t = Taskzyklus mal Anzahl Zyklen = 5msTakt * beschlcnt 
        
        =>  2 * pi * nmot = alpha * beschlcnt * 5msTakt  =>  nmot = alpha * beschlcnt * 5 / (2 * Pi)
        
        3) fmot = PlsUmdr * nmot                            => 
        
        2) u. 3) in 1) einsetzen:
        PulsAbsMax = erzeuger * 2 * Pi / (PlsUmdr * alpha * beschlcnt * 5) ;     
        
                                                                   
        beschlfak = erzeuger * 2 * Pi / (PlsUmdr * alpha)       // beschlcnt und 5ms kommen haeppchenweise dazu 
        ========================================
        
        istPlsAbst = beschlfak / (5 * beschlcnt)
        ===========================================
        
        Nun fehlt uns noch beschlanz, also wann wir aufhoeren sollen zu beschleunigen:
        
        t = (2 * PI * nmot) / alpha;  // Zeit für die Rampe von 0 bis max. Speed
        
        beschlanz = t * 1000 / 5   //  Taskzeit (5ms) = 5 / 1000 s
        
        beschlanz = ((2 * PI * nmot) / alpha)  *  (1000 / 5) ;
        
        beschlanz = fmot/alpha  *  1256,64
        ==================================
        
        
        EasyCODE_COMMENT_END
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        if ( smx.dynbef == 1 )
        {
            smx.erzeuger = 20000000 / (float)smx.vort;
            //erzeuger = 20000000 / 64.0;   //EFR_CLK_256
            
            smx.istPlsAbst = smx.erzeuger * 100.0 / (PlsUmdr * (float)smx.nmotstart);
            smx.beschlanz = (float)smx.delay_ms / 5.0 + 1.0;
            smx.beschlcnt = 0;
            
            smx.gespls = 65535  -  smx.istPlsAbst;
            smx.start_dyn_Pulse(); 
            
            smx.plse1 = (float)smx.nmotstart/100.0 * (float)smx.delay_ms;
            smx.plse2 = 3141.6 * pow(((float)smx.nmot/100.0) , 2) / (float)smx.a_beschl + 1.0;
            smx.plse3 = 3141.6 * pow(((float)smx.nmot/100.0) , 2) / (float)smx.a_brems + 1.0;
            
            smx.rmpabbruch = FALSE;
            
            smxkey = 1;
        }
        else
        {
            // Ruhe
        }
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        break;
    case 1 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Langsam konstant fahren EasyCODE_COMMENT_END
        if ( smx.dynbef == 2 )
        {
            smx.stop();
            smxkey = 0;
        }
        else
        {
            if ( smx.beschlcnt < smx.beschlanz )
            {
                if ( smx.plse  < (smx.plse1 + 20) )
                {
                    if ( smx.Pulse() >= smx.plse )
                    {
                        smx.dynbef = 0;   
                        smx.stop();
                        smxkey = 0;
                    }
                }
                else
                {
                    smx.beschlcnt ++;
                }
            }
            else
            {
                smx.beschlfak = (smx.erzeuger * 2.0 * PI * 1000) / (PlsUmdr * smx.a_beschl ) + 0.5 ;  // * 1000 wegen ms
                smx.bremsfak =  (smx.erzeuger * 2.0 * PI * 1000) / (PlsUmdr * smx.a_brems  ) + 0.5 ;  // * 1000 wegen ms
                smx.beschlanz = (float)smx.nmot / (100 * smx.a_beschl) * 1256.64  + 1.0;           // aufrunden nmot in 1/100
                smx.bremsanz  = (float)smx.nmot / (100 * smx.a_brems) * 1256.64  + 1.0;           // aufrunden nmot in 1/100
                
                
                //Aus: istPlsAbst = beschlfak / (5 * beschlcnt) folgt:
                smx.beschlcnt = smx.beschlfak / (smx.istPlsAbst * 5);
                
                smxkey = 2;
            }
        }
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        break;
    case 2 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Beschleunigen auf fmot (max Frequenz) EasyCODE_COMMENT_END
        if ( smx.dynbef == 2 )
        {
            smx.beschlcnt = smx.bremsfak / (smx.istPlsAbst * 5);
            smxkey = 4;
        }
        else
        {
            if ( smx.beschlcnt < smx.beschlanz )
            {
                smx.beschlcnt ++;
                
                smx.istPlsAbst = (smx.beschlfak / (5 * smx.beschlcnt));
                if ( smx.istPlsAbst > 65000 )
                {
                    smx.istPlsAbst   = 65000;
                }
                smx.gespls = 65535  -  smx.istPlsAbst;
                smx.start_dyn_Pulse();             // Rampe fahren mit dyn. Pulsabstand
                EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                nmottemp = smx.erzeuger * 100.0 / (PlsUmdr * (float)smx.istPlsAbst);
                pls3tmp = 3141.6 * pow(((float)nmottemp/100.0) , 2) / (float)smx.a_brems + 1.0;
                if ( (smx.plse)  > (smx.Pulse() +  pls3tmp + 70) )
                {
                    // weiter beschl.
                }
                else
                {
                    smx.beschlcnt = smx.bremsfak / (smx.istPlsAbst * 5);
                    smx.rmpabbruch = TRUE;
                    smx.dynbef = 0;
                    smxkey = 4;
                }
            }
            else
            {
                smxkey = 3;
            }
        }
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        break;
    case 3 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Fahren mit konst fmot EasyCODE_COMMENT_END
        if ( smx.dynbef == 2 )
        {
            smx.beschlcnt = smx.bremsanz;
            smxkey = 4;
        }
        else
        {
            if ( (smx.plse)  > (smx.Pulse() + smx.plse3 + 70) )
            {
                // weiter fahren
            }
            else
            {
                smx.beschlcnt = smx.bremsanz;
                smx.rmpabbruch = TRUE;
                smx.dynbef = 0;
                smxkey = 4;
            }
        }
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        break;
    case 4 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Bremsen und Anhalten EasyCODE_COMMENT_END
        if ( smx.beschlcnt > 1 )
        {
            smx.beschlcnt--;
            EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
            smx.istPlsAbst = (smx.bremsfak / (5 * smx.beschlcnt));
            if ( smx.istPlsAbst > 65000 )
            {
                smx.istPlsAbst   = 65000;
            }
            smx.gespls = 65535  -  smx.istPlsAbst;
            smx.start_dyn_Pulse();   // Rampe fahren mit dyn. Pulsabstand
        }
        else
        {
            if ( smx.rmpabbruch == TRUE )
            {
                smx.istPlsAbst = smx.erzeuger * 100.0 / (PlsUmdr * (float)smx.nmotstart);
                
                //smx.istPlsAbst /= 2; // Doppelt so schnell aufs Ziel fahren als mit 
                     // zielen
                
                //smx.istPlsAbst = 3120;
                smx.istPlsAbst = 4000;
                
                
                smx.gespls = 65535  -  smx.istPlsAbst;
                smx.start_dyn_Pulse();
                smxkey = 5;
            }
            else
            {
                smx.stop();
                smx.dynbef = 0; 
                smxkey = 0;
            }
        }
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        break;
    case 5 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Restpulse fahren EasyCODE_COMMENT_END
        if ( smx.dynbef == 2 )
        {
            smx.stop();
            smxkey = 0;
        }
        else
        {
            if ( smx.Pulse() >= smx.plse )
            {
                smx.dynbef = 0;   
                smx.stop();
                smxkey = 0;
            }
        }
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        break;
    case 6 :
        break;
    default:
        break;
    }
    switch ( smykey )
    {
    case 0 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Idle (warten auf Start) EasyCODE_COMMENT_END
        if ( smy.dynbef == 1 )
        {
            //smx.erzeuger = 20000000 / (float)smx.vort;
            smy.erzeuger = 20000000 / 64.0;   //EFR_CLK_256
            
            smy.istPlsAbst = smy.erzeuger * 100.0 / (PlsUmdr * (float)smy.nmotstart);
            smy.beschlanz = (float)smy.delay_ms / 5.0 + 1.0;
            smy.beschlcnt = 0;
            
            smy.gespls = 65535  -  smy.istPlsAbst;
            smy.start_dyn_Pulse(); 
            
            smy.plse1 = (float)smy.nmotstart/100.0 * (float)smy.delay_ms;
            smy.plse2 = 3141.6 * pow(((float)smy.nmot/100.0) , 2) / (float)smy.a_beschl + 1.0;
            smy.plse3 = 3141.6 * pow(((float)smy.nmot/100.0) , 2) / (float)smy.a_brems + 1.0;
            
            smy.rmpabbruch = FALSE;
            
            smykey = 1;
        }
        else
        {
            // Ruhe
        }
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        break;
    case 1 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Langsam konstant fahren EasyCODE_COMMENT_END
        if ( smy.dynbef == 2 )
        {
            smy.stop();
            smykey = 0;
        }
        else
        {
            if ( smy.beschlcnt < smy.beschlanz )
            {
                if ( smy.plse  < (smy.plse1 + 20) )
                {
                    if ( smy.Pulse() >= smy.plse )
                    {
                        smy.dynbef = 0;   
                        smy.stop();
                        smykey = 0;
                    }
                }
                else
                {
                    smy.beschlcnt ++;
                }
            }
            else
            {
                smy.beschlfak = (smy.erzeuger * 2.0 * PI * 1000) / (PlsUmdr * smy.a_beschl ) + 0.5 ;  // * 1000 wegen ms
                smy.bremsfak =  (smy.erzeuger * 2.0 * PI * 1000) / (PlsUmdr * smy.a_brems  ) + 0.5 ;  // * 1000 wegen ms
                smy.beschlanz = (float)smy.nmot / (100 * smy.a_beschl) * 1256.64  + 1.0;           // aufrunden nmot in 1/100
                smy.bremsanz  = (float)smy.nmot / (100 * smy.a_brems) * 1256.64  + 1.0;           // aufrunden nmot in 1/100
                
                
                //Aus: istPlsAbst = beschlfak / (5 * beschlcnt) folgt:
                smy.beschlcnt = smy.beschlfak / (smy.istPlsAbst * 5);
                
                smykey = 2;
            }
        }
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        break;
    case 2 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Beschleunigen auf fmot (may Frequenz) EasyCODE_COMMENT_END
        if ( smy.dynbef == 2 )
        {
            smy.beschlcnt = smy.bremsfak / (smy.istPlsAbst * 5);
            smykey = 4;
        }
        else
        {
            if ( smy.beschlcnt < smy.beschlanz )
            {
                smy.beschlcnt ++;
                
                smy.istPlsAbst = (smy.beschlfak / (5 * smy.beschlcnt));
                if ( smy.istPlsAbst > 65000 )
                {
                    smy.istPlsAbst   = 65000;
                }
                smy.gespls = 65535  -  smy.istPlsAbst;
                smy.start_dyn_Pulse();             // Rampe fahren mit dyn. Pulsabstand
                EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                nmottemp = smy.erzeuger * 100.0 / (PlsUmdr * (float)smy.istPlsAbst);
                pls3tmp = 3141.6 * pow(((float)nmottemp/100.0) , 2) / (float)smy.a_brems + 1.0;
                if ( (smy.plse)  > (smy.Pulse() +  pls3tmp + 70) )
                {
                    // weiter beschl.
                }
                else
                {
                    smy.beschlcnt = smy.bremsfak / (smy.istPlsAbst * 5);
                    smy.rmpabbruch = TRUE;
                    smy.dynbef = 0;
                    smykey = 4;
                }
            }
            else
            {
                smykey = 3;
            }
        }
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        break;
    case 3 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Fahren mit konst fmot EasyCODE_COMMENT_END
        if ( smy.dynbef == 2 )
        {
            smy.beschlcnt = smy.bremsanz;
            smykey = 4;
        }
        else
        {
            if ( (smy.plse)  > (smy.Pulse() + smy.plse3 + 70) )
            {
                // weiter fahren
            }
            else
            {
                smy.beschlcnt = smy.bremsanz;
                smy.rmpabbruch = TRUE;
                smy.dynbef = 0;
                smykey = 4;
            }
        }
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        break;
    case 4 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Bremsen und Anhalten EasyCODE_COMMENT_END
        if ( smy.beschlcnt > 1 )
        {
            smy.beschlcnt--;
            EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
            smy.istPlsAbst = (smy.bremsfak / (5 * smy.beschlcnt));
            if ( smy.istPlsAbst > 65000 )
            {
                smy.istPlsAbst   = 65000;
            }
            smy.gespls = 65535  -  smy.istPlsAbst;
            smy.start_dyn_Pulse();   // Rampe fahren mit dyn. Pulsabstand
        }
        else
        {
            if ( smy.rmpabbruch == TRUE )
            {
                smy.istPlsAbst = smy.erzeuger * 100.0 / (PlsUmdr * (float)smy.nmotstart);
                
                //smy.istPlsAbst /= 2; // Doppelt so schnell aufs Ziel fahren als mit 
                     // zielen
                
                //smy.istPlsAbst = 3120;
                smy.istPlsAbst = 4000;
                
                
                smy.gespls = 65535  -  smy.istPlsAbst;
                smy.start_dyn_Pulse();
                smykey = 5;
            }
            else
            {
                smy.stop();
                smy.dynbef = 0; 
                smykey = 0;
            }
        }
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        break;
    case 5 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Restpulse fahren EasyCODE_COMMENT_END
        if ( smy.dynbef == 2 )
        {
            smy.stop();
            smykey = 0;
        }
        else
        {
            if ( smy.Pulse() >= smy.plse )
            {
                smy.dynbef = 0;   
                smy.stop();
                smykey = 0;
            }
        }
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        break;
    case 6 :
        break;
    default:
        break;
    }
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
*/
/* EasyCODE_STRUCTURED_COMMENT_END */
/* EasyCODE ) */
