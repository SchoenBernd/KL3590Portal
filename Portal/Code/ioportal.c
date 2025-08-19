/* EasyCODE V8 */
/* EasyCODE ( 0 
ioportal */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
/*
Ausgaenge schalten

Datei: ioportal.c
Klasse: 3590 Portal
Ersteller:  Wilfried Halfmann  EES 2856

Jeder Ausgang hat ein Setz- und ein Ruecksetzprogramm. Setz- bzw. Ruecksetz-
auftraege werden im Auftragsbyte gegeben. Nach der Ausfuehrung des entspre-
chenden Programms wird der Auftrag rueckgesetzt, was gleichzeitig als Fer-
tigmeldung benutzt wird. Es wird ein Abbild der Zustaende der Ausgaenge im
Speicher abgelegt. Hierbei bedeutet '1' Ausgang gesetzt. Jedem Bit im
Speicherabbild ist ein Auftragsbyte, jedem Auftragsbyte zwei Unterprogram-
me fuer Setzen und Ruecksetzen zugeordnet (siehe auch io-header).
Es wird eine Fehlerueberwachung durchgef_hrt. Kann ein Auftrag innerhalb
der Ueberwachungszeit nicht abgeschlossen werden, so wird dies mit zwei
Fehlernummern gemeldet. Hierbei geben der Hauptfehler den betroffenen Aus-
gang und der Unterfehler die Fehlerursache an.

Weiterhin werden folgende Unterprogramme zur Verf_gung gestellt:
- io_kinit ..... Initialisierung fuer Kaltstart
- io_winit ..... Initialisierung fuer Warmstart
- io_off ....... Direktes Ausschalten verschiedener Ausgaenge

Da die Klasse 3590 hinsichtlich der Ablaeufe nicht netzausfallsicher ist
und der netzausfallsichere Teil des Naehens alle Ausgaenge wieder entspre-
chend schaltet, koennen hier alle Abbildbits und Auftraege im Init ge-
loescht werden.
Bei netzausfallsicheren Maschinen muessen im Init bestimmte Auftraege ge-
loescht werden, andere muessen bestehen bleiben. Ebenso muessen bestimmte ge-
setzte Ausgaenge im Abbild geloescht, andere wieder in Auftrag gegeben wer-
den. Weiterhin werden alle Ruecksetzauftraege und die korrespondierenden
Bits im Abbild geloescht.
*/

#include "portal.h"

/* Datendefinitionen */
volatile unsigned int in_image[2];
volatile unsigned int out_image[2];
KlasseIO io (tdini [IoIndex]);
/* EasyCODE ( 0 
Funktionsprototypen */
void sy1 (void);
void ry1 (void);
void sy4 (void);
void ry4 (void);
void sy5 (void);
void ry5 (void);
void sy6 (void);
void ry6 (void);
void sy7 (void);
void ry7 (void);
void sy8 (void);
void ry8 (void);
void sy9 (void);
void ry9 (void);
void sy10 (void);
void ry10 (void);
void sy30 (void);
void ry30 (void);
void sy31 (void);
void ry31 (void);
void sy32 (void);
void ry32 (void);
void sy33 (void);
void ry33 (void);
void sy34 (void);
void ry34 (void);
void sy35 (void);
void ry35 (void);
void sy50u1 (void);
void ry50u1 (void);
void sy50u2 (void);
void ry50u2 (void);
void sy51u1 (void);
void ry51u1 (void);
void sy51u2 (void);
void ry51u2 (void);
void sy52 (void);
void ry52 (void);
void sy53 (void);
void ry53 (void);
void sy54 (void);
void ry54 (void);

void sy61 (void);
void ry61 (void);

void sy62 (void);
void ry62 (void);

void sy63 (void);
void ry63 (void);


void sbobres1 (void);
void rbobres1 (void);
void sbobres2 (void);
void rbobres2 (void);
void sout1 (void);
void rout1 (void);
void sout2 (void);
void rout2 (void);
void sout3 (void);
void rout3 (void);
void sout4 (void);
void rout4 (void);
void sy4fkt (void);
void sy2fkt_ein (void);
void sy2fkt_aus (void);
void sy2fkt_drucklos (void);
void sy2fkt (void);
void sy9fkt (void);
void shfuss_auf (void);
void sy7_y32fkt (void);
void ry7_y32fkt (void);
void frei (void);
void sndr (void);
void sonderfrei (void);
/* EasyCODE ) */
/* EasyCODE ( 0 
Initialisierung der Ausgaenge */
/* EasyCODE F */
// Initialisierungswerte fuer die Ausgaenge
outdata outini[ANZAHL_AUSG] =
/*
Auftrag, Maske, Knotennr., Zeiger Setzpr., Zeiger Ruecksetzpr. */
{
    0,       0x0001,   1,           sy1,             ry1,
    0,       0x0002,   1,           frei,            frei,
    0,       0x0004,   1,           frei,            frei,
    0,       0x0008,   1,           sy4,             ry4, 
    0,       0x0010,   1,           sy5,             ry5,
    0,       0x0020,   1,           sy6,             ry6,
    0,       0x0040,   1,           sy7,             ry7,
    0,       0x0080,   1,           sy8,             ry8,
    0,       0x0100,   1,           sy9,             ry9,
    0,       0x0200,   1,           sy30,            ry30,
    0,       0x0400,   1,           sy31,            ry31,
    0,       0x0800,   1,           sy32,            ry32,
    0,       0x1000,   1,           sy33,            ry33,
    0,       0x2000,   1,           sy34,            ry34,
    0,       0x4000,   1,           sy35,            ry35,
    0,       0x8000,   1,           sy10,            ry10,
    0,       0x0001,   2,           sy50u1,          ry50u1,
    0,       0x0002,   2,           sy50u2,          ry50u2,
    0,       0x0004,   2,           sy51u1,          ry51u1,
    0,       0x0008,   2,           sy51u2,          ry51u2,
    0,       0x0010,   2,           sy52,            ry52,
    0,       0x0020,   2,           sy53,            ry53,
    0,       0x0040,   2,           sy54,            ry54,
    0,       0x0080,   2,           sbobres1,        rbobres1,
    0,       0x0100,   2,           sbobres2,        rbobres2,
    0,       0x0200,   2,           sy61,            ry61,
    0,       0x0400,   2,           sy62,            ry62,
    0,       0x0800,   2,           sy63,            ry63,
    0,       0x1000,   2,           sout1,           rout1,
    0,       0x2000,   2,           sout2,           rout2,
    0,       0x4000,   2,           sout3,           rout3,
    0,       0x8000,   2,           sout4,           rout4,
}

;
/* EasyCODE ) */
/* EasyCODE ( 0 
Initialisierung der Sonderauftraege */
/* EasyCODE F */
// Initialisierungswerte fuer die Ausgaenge
sonderauftrag sonderini[ANZAHL_SONDERAUFTRAEGE] =
/* 
Auftrag,   Zeiger Setzpr.,  Zeiger Ruecksetzpr. */
{
    0,         sy4fkt,              sonderfrei,       // Oberfadenklemmenfunktion
    0,         sy2fkt_aus,          sonderfrei,       // Fadenwedler in Grundstellung
    0,         sy2fkt_ein,          sonderfrei,       // Fadenwedler ein
    0,         sy2fkt_drucklos,     sonderfrei,       // Fadenwedler drucklos
    0,         sy2fkt,              sonderfrei,       // Fadenwedler-Funktion
    0,         shfuss_auf,          sonderfrei,       // Huepferfuss auf
    0,         sy9fkt,              sonderfrei,       // Oberfaden ziehen beim Annaehen
    0,         sy7_y32fkt,          ry7_y32fkt,       // Nadeldreheinrichtung und Unterfadenschieber ein/aus
    0,         sonderfrei,          sonderfrei,       // Reserve
}

;
/* EasyCODE ) */
/* EasyCODE ( 0 
Initialisierung der Eingaenge */
// Initialisierungswerte fuer die Eingaenge
indata inini[ANZAHL_EING] ={
/*
Knotennr., Maske */
1,      0x0001,       // e29
1,      0x0002,       // therr1
1,      0x0004,       // therr2
1,      0x0008,       // e28
1,      0x0010,       // temp
1,      0x0020,       // e33
1,      0x0040,       // boberr1
1,      0x0080,       // boberr2
1,      0x0100,       // ac_ok
1,      0x0200,       // press
1,      0x0400,       // fkey
1,      0x0800,       // foot1
1,      0x1000,       // foot2
1,      0x2000,       // tlinks_stop
1,      0x4000,       // trechts_stop
1,      0x8000,       // e81
2,      0x0001,       // e50u1
2,      0x0002,       // e50u2
2,      0x0004,       // e51u2_1
2,      0x0008,       // e51u2_2
2,      0x0010,       // e80
2,      0x0020,       // e79
2,      0x0040,       // e78
2,      0x0080,       // e77
2,      0x0100,       // e76
2,      0x0200,       // e75
2,      0x0400,       // e74
2,      0x0800,       //       kone1
2,      0x1000,       // in1   kone2
2,      0x2000,       // in2
2,      0x4000,       // in3
2,      0x8000,       // in4
};
/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseIO::iotask */
/* EasyCODE F */
void KlasseIO::iotask (void)
{
    CAN->start=0;
    switch ( key )
    {
    case 0 :
        if ( koor.iostop() )
        {
            off();
            stopb = TRUE;
            key = 1;
        }
        else
        {
            if ( mainerr==0 || mainerr!=mainerr_merk )
            {
                tio1.rsz();
            }
            else
            {
                tio1.sz(saf->io_errtime);
                if ( tio1.tz() )
                {
                    koor.io_err = mainerr;
                    koor.io_secerr = secerr;
                    tio1.rsz();
                }
            }
            mainerr_merk = mainerr;
            mainerr = 0;
            secerr = 0;
            for ( index=0; index<ANZAHL_AUSG; index++ )
            {
                #if defined(buerotest)
                    Ausg[index].clearAuftrag();
                #endif 
                switch ( Ausg[index].getAuftrag() )
                {
                case 0 :
                    break;
                case SET :
                    Ausg[index].setup();
                    break;
                case RES :
                    Ausg[index].resetup();
                    break;
                default:
                    break;
                }
            }
            for ( index=0; index<ANZAHL_SONDERAUFTRAEGE; index++ )
            {
                #if defined(buerotest)
                    Sonder[index].clearAuftrag();
                #endif 
                switch ( Sonder[index].getAuftrag() )
                {
                case 0 :
                    break;
                case SET :
                    Sonder[index].setup();
                    break;
                case RES :
                    Sonder[index].resetup();
                    break;
                default:
                    break;
                }
            }
            abbild();
        }
        break;
    case 1 :
        if ( koor.iostop() )
        {
        }
        else
        {
            key = 0;
            tio1.rsz();
            stopb = FALSE;
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseIO::off */
/* EasyCODE F */
/********** off - Direktes Ausschalten verschiedener Ausgaenge **********/
// !!! Darf nur von io selbst und von Powerfail benutzt werden !!!
void KlasseIO::off (void)
{
    y1.clearAuftrag();   // Blasluft Nadelkuehlung aus
    y1.resAusgang();
    y34.clearAuftrag();  // Greiferoelung 1 aus
    y34.resAusgang();
    y35.clearAuftrag();  // Greiferoelung 2 aus
    y35.resAusgang();
    /* EasyCODE - */
    io.clamp_merker = FALSE;  // SW013
    
    abbild();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ausgang::IsFrei() */
/* EasyCODE F */
char ausgang::IsFrei()
{
    if ( (out.sa == frei) && (out.ra == frei) )
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
ausgang::IsSonder() */
/* EasyCODE F */
char ausgang::IsSonder()
{
    if ( (out.sa == sndr) || (out.ra == sndr) )
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
sonderauftr::IsFrei() */
/* EasyCODE F */
char sonderauftr::IsFrei()
{
    if ( (sauf.sa == frei) && (sauf.ra == frei) )
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
KlasseIO::abbild */
/* EasyCODE F */
/********** abbild - Ausgabe des Speicherabbildes ins DPR **********/
// !!! Darf nur von io selbst benutzt werden !!!
void KlasseIO::abbild (void)
{
    unsigned char i;
    for ( i=0; i<ANZ_KNOTEN; i++ )
    {
        CAN->out_soll[i] = out_image[i];
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseIO::kinit, Initialisierung bei Kaltstart */
/* EasyCODE F */
void KlasseIO::kinit (void)
{
    saf->io_errtime = 300;   // Fehler_berwachungszeit = 3 s
    saf->fadensp_time = 1000; // Fadenspannungsausloesung = 10 s
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseIO::winit, Initialisierung bei Warmstart */
/* EasyCODE F */
void KlasseIO::winit (void)
{
    unsigned char i;
    for ( i=0; i<ANZAHL_AUSG; i++ )
    {
        Ausg[i].Init(outini[i]);
    }
    for ( i=0; i<ANZAHL_SONDERAUFTRAEGE; i++ )
    {
        Sonder[i].Init(sonderini[i]);
    }
    for ( i=0; i<ANZAHL_EING; i++ )
    {
        Eing[i].Init(inini[i]);
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
frei */
/* EasyCODE F */
/* unbenutzte Auftragsbytes */
static void frei (void)
{
    io.Ausg[io.index].clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sonder */
/* EasyCODE F */
/* sonderbenutzte Auftragsbytes */
static void sndr (void)
{
    frei();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy1,   Blasluft Nadelkuehlung ein */
/* EasyCODE F */
static void sy1 (void)
{
    tio2.lz ();
    y1.setAusgang();
    y1.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry1,   Blasluft Nadelkuehlung aus (wird verzoegert ausgeschaltet) */
/* EasyCODE F */
static void ry1 (void)
{
    tio2.sz (saf->md.nadelkuehl_time);
    if ( tio2.tz() )
    {
        tio2.lz();
        y1.resAusgang();
        y1.clearAuftrag();
    }
}

/* EasyCODE ) */
/* EasyCODE_STRUCTURED_COMMENT_START */
/*
EasyCODE_COMMENT_START EasyCODE ( 0 
sy2u1,   Fadenwedler vor 1 EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
static void sy2u1 (void)
{
    y2u1.setAusgang();
    y2u1.clearAuftrag();
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE ( 0 
ry2u1,   Fadenwedler vor 1 EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
static void ry2u1 (void)
{
    y2u1.resAusgang();
    y2u1.clearAuftrag();
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE ( 0 
sy2u2,   Fadenwedler vor 2 EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
static void sy2u2 (void)
{
    y2u2.setAusgang();
    y2u2.clearAuftrag();
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE ( 0 
ry2u2,   Fadenwedler vor 2 EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
static void ry2u2 (void)
{
    y2u2.resAusgang();
    y2u2.clearAuftrag();
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
*/
/* EasyCODE_STRUCTURED_COMMENT_END */
/* EasyCODE ( 0 
sy4,   Oberfadenklemme zu */
/* EasyCODE F */
static void sy4 (void)
{
    y4.setAusgang();
    y4.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry4,   Oberfadenklemme auf */
/* EasyCODE F */
static void ry4 (void)
{
    y4.resAusgang();
    y4.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy5,   Fadenspannung auf */
/* EasyCODE F */
static void sy5 (void)
{
    y5.setAusgang();
    y5.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry5,   Fadenspannung zu */
/* EasyCODE F */
static void ry5 (void)
{
    y5.resAusgang();
    y5.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy6,   Zusatzfadenspannung zu */
/* EasyCODE F */
static void sy6 (void)
{
    y6.setAusgang();
    y6.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry6,   Zusatzfadenspannung auf */
/* EasyCODE F */
static void ry6 (void)
{
    y6.resAusgang();
    y6.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy7,   Nadeldreheinrichtung ein */
/* EasyCODE F */
static void sy7 (void)
{
    if ( saf->md.nadelanzahl != 1 )
    {
        io.secerr=NADELANZAHL;
        io.mainerr=Y7_HF1;
    }
    else
    {
        y7.setAusgang();
        y7.clearAuftrag();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry7,   Nadeldreheinrichtung aus */
/* EasyCODE F */
static void ry7 (void)
{
    y7.resAusgang();
    y7.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy8,   Niederhalter ab */
/* EasyCODE F */
static void sy8 (void)
{
    if ( y8.getZustand() )
    {
        if ( tio7.tz() )
        {
            y8.clearAuftrag();
        }
    }
    else
    {
        y8.setAusgang();
        tio7.rsz();
        tio7.sz(20);
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry8,   Niederhalter auf */
/* EasyCODE F */
static void ry8 (void)
{
    if ( !y8.getZustand() )
    {
        if ( tio7.tz() )
        {
            y8.clearAuftrag();
        }
    }
    else
    {
        /* EasyCODE < */
        // Huepferfuss oben
        /* EasyCODE > */
        if ( hfuss.getpos().pos != saf->md.posoben.pos )
        {
            io.secerr=HUEPFER_OBEN;
            io.mainerr=Y50U2_HF;
        }
        else
        {
            y8.resAusgang();
            tio7.rsz();
            tio7.sz(20);
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy9,   Oberfaden rueckziehen ein */
/* EasyCODE F */
static void sy9 (void)
{
    y9.setAusgang();
    y9.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry9,   Oberfaden rueckziehen aus */
/* EasyCODE F */
static void ry9 (void)
{
    y9.resAusgang();
    y9.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy10,   Bandanschlag ab (Webasto) */
/* EasyCODE F */
static void sy10 (void)
{
    y10.setAusgang();
    y10.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry10,   Bandanschlag auf (Webasto) */
/* EasyCODE F */
static void ry10 (void)
{
    y10.resAusgang();
    y10.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy30,   Fadenschneiden ein */
/* EasyCODE F */
static void sy30 (void)
{
    if ( y30.getZustand() )
    {
        y30.clearAuftrag();
    }
    else
    {
        /* EasyCODE < */
        // falls Service Ausgaenge schalten:
                //   Nadel im gueltigen Bereich
        /* EasyCODE > */
        if ( eing.serv.get_out_akt() && !nminf.nadel_bereich(saf->ot_bereich) )
        {
            io.secerr=NEEDLEPOS_UF1;
            io.mainerr=Y30_HF1;
        }
        else
        {
            y30.setAusgang();
            y30.clearAuftrag();
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry30,   Fadenschneiden aus */
/* EasyCODE F */
static void ry30 (void)
{
    y30.resAusgang();
    y30.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy31,   Unterfadenklemme auf */
/* EasyCODE F */
static void sy31 (void)
{
/* EasyCODE_STRUCTURED_COMMENT_START */
    /*
    if ( y31.getZustand() )
    {
        if ( tio3.tz() )
        {
            // Unterfadenklemme zu
            y31.resAusgang();
            y31.clearAuftrag();
        }
    }
    else
    {
        y31.setAusgang();
        tio3.rsz();
        tio3.sz (saf->md.ufk_time);
    }
    */
    /* EasyCODE_STRUCTURED_COMMENT_END */
    y31.setAusgang();
    y31.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry31,   Unterfadenklemme zu */
/* EasyCODE F */
static void ry31 (void)
{
    y31.resAusgang();
    y31.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy32,   Unterfadenschieber ein */
/* EasyCODE F */
static void sy32 (void)
{
    if ( saf->md.nadelanzahl != 1 )
    {
        io.secerr=NADELANZAHL;
        io.mainerr=Y32_HF1;
    }
    else
    {
        y32.setAusgang();
        y32.clearAuftrag();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry32,   Unterfadenschieber aus */
/* EasyCODE F */
static void ry32 (void)
{
    y32.resAusgang();
    y32.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy33,   Stoffgegendruecker auf */
/* EasyCODE F */
static void sy33 (void)
{
    if ( saf->stoffgegendrueckerf )
    {
        if ( y33.getZustand() )
        {
            if ( !e33.getZustand() )
            {
                y33.clearAuftrag();
            }
            else
            {
                io.mainerr = Y33_HF1;
                io.secerr = E33_UF0;
            }
        }
        else
        {
            y33.setAusgang();
        }
    }
    else
    {
        y33.clearAuftrag();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry33,   Stoffgegendruecker ab */
/* EasyCODE F */
static void ry33 (void)
{
    if ( saf->stoffgegendrueckerf )
    {
        if ( !y33.getZustand() )
        {
            if ( e33.getZustand() )
            {
                y33.clearAuftrag();
            }
            else
            {
                io.mainerr = Y33_HF0;
                io.secerr = E33_UF1;
            }
        }
        else
        {
            y33.resAusgang();
        }
    }
    else
    {
        y33.clearAuftrag();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy34,   Greiferoelung 1 ein */
/* EasyCODE F */
static void sy34 (void)
{
    y34.setAusgang();
    y34.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry34,   Greiferoelung 1 aus */
/* EasyCODE F */
static void ry34 (void)
{
    y34.resAusgang();
    y34.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy35,   Greiferoelung 2 ein */
/* EasyCODE F */
static void sy35 (void)
{
    y35.setAusgang();
    y35.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry35,   Greiferoelung 2 aus */
/* EasyCODE F */
static void ry35 (void)
{
    y35.resAusgang();
    y35.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy50u1,   Oberteil auf */
/* EasyCODE F */
static void sy50u1 (void)
{
    if ( e50u1.getZustand() && !e50u2.getZustand() )
    {
        /* EasyCODE ( 0 
        Oberteil oben */
        if ( !y54.getZustand() && !e54.getZustand() )
        {
            y50u1.resAusgang();
            y50u1.clearAuftrag();
        }
        else
        {
            if ( !y54.getZustand() )
            {
                io.mainerr = Y50U1_HF;
                io.secerr = E54_UF0;
            }
            else
            {
                // Oberteilarretierung ein
                y54.resAusgang();
            }
        }
        /* EasyCODE ) */
    }
    else
    {
        /* EasyCODE ( 0 
        Oberteil unten */
        if ( y54.getZustand () && e54.getZustand() )
        {
            if ( y50u1.getZustand() )
            {
                io.mainerr=Y50U1_HF;
                if ( !e50u1.getZustand() )
                {
                    io.secerr=E50U1_UF1;
                }
                else
                {
                    io.secerr=E50U2_UF0;
                }
            }
            else
            {
                y50u2.resAusgang();
                y50u1.setAusgang();
            }
        }
        else
        {
            if ( y54.getZustand() )
            {
                io.mainerr = Y50U1_HF;
                io.secerr = E54_UF1;
            }
            else
            {
                /* EasyCODE ( 0 
                verriegelungen pr_fen */
                if ( nae.faedelreq )
                {
                    // Oberteilarretierung aus
                    y54.setAusgang();
                }
                else
                {
                    /* EasyCODE < */
                    // Faden geschnitten
                    /* EasyCODE > */
                    if ( !saf->cutready && !((nup.workbuf.naeout & 0x20) == 0x20) //SW014
                       )
                    {
                        io.secerr=CUTREADY_UF1;
                        io.mainerr=Y50U1_HF;
                    }
                    else
                    {
                        /* EasyCODE < */
                        // Nadel im gueltigen Bereich
                        /* EasyCODE > */
                        if ( !nminf.nadel_bereich(saf->ot_bereich) )
                        {
                            io.secerr=NEEDLEPOS_UF1;
                            io.mainerr=Y50U1_HF;
                        }
                        else
                        {
                            // Oberteilarretierung aus
                            y54.setAusgang();
                        }
                    }
                }
                /* EasyCODE ) */
            }
        }
        /* EasyCODE ) */
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry50u1,   Oberteil auf */
/* EasyCODE F */
static void ry50u1 (void)
{
    y50u1.resAusgang();
    y50u1.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy50u2,  Oberteil ab */
/* EasyCODE F */
static void sy50u2 (void)
{
    if ( e50u2.getZustand() && !e50u1.getZustand() )
    {
        /* EasyCODE ( 0 
        Oberteil unten */
        if ( !y54.getZustand() && !e54.getZustand() )
        {
            y50u2.resAusgang();
            y50u2.clearAuftrag();
        }
        else
        {
            if ( !y54.getZustand() )
            {
                io.mainerr = Y50U2_HF;
                io.secerr = E54_UF0;
            }
            else
            {
                // Oberteilarretierung ein
                y54.resAusgang();
            }
        }
        /* EasyCODE ) */
    }
    else
    {
        /* EasyCODE ( 0 
        Oberteil oben */
        if ( y54.getZustand () && e54.getZustand() )
        {
            if ( y50u2.getZustand() )
            {
                io.mainerr=Y50U2_HF;
                if ( !e50u2.getZustand() )
                {
                    io.secerr=E50U2_UF1;
                }
                else
                {
                    io.secerr=E50U1_UF0;
                }
            }
            else
            {
                y50u1.resAusgang();
                y50u2.setAusgang();
            }
        }
        else
        {
            if ( y54.getZustand() )
            {
                io.mainerr = Y50U2_HF;
                io.secerr = E54_UF1;
            }
            else
            {
                /* EasyCODE ( 0 
                Verriegelungen
                pruefen */
                if ( nae.faedelreq )
                {
                    // Oberteilarretierung aus
                    y54.setAusgang();
                }
                else
                {
                    /* EasyCODE < */
                    // Huepferfuss oben
                    /* EasyCODE > */
                    if ( hfuss.getpos().pos != saf->md.posoben.pos )
                    {
                        io.secerr=HUEPFER_OBEN;
                        io.mainerr=Y50U2_HF;
                    }
                    else
                    {
                        // Niederhalter oben
                        if ( y8.getZustand() && (saf->variante != 1) // SW002
                           )
                        {
                            io.secerr=Y8_UF0;
                            io.mainerr=Y50U2_HF;
                        }
                        else
                        {
                            // Schablonendeckel geschlossen falls
                            // Schablone eingelegt
                            if ( ((e80.getZustand() || e79.getZustand()) && nae.deckelueberwachung) && !e76.getZustand() )
                            {
                                io.secerr=E76_UF1;
                                io.mainerr=Y50U2_HF;
                            }
                            else
                            {
                                /* EasyCODE < */
                                // Nadel im gueltigen Bereich
                                /* EasyCODE > */
                                if ( !nminf.nadel_bereich(saf->ot_bereich) )
                                {
                                    io.secerr=NEEDLEPOS_UF1;
                                    io.mainerr=Y50U2_HF;
                                }
                                else
                                {
                                    // Oberteilarretierung aus
                                    y54.setAusgang();
                                }
                            }
                        }
                    }
                }
                /* EasyCODE ) */
            }
        }
        /* EasyCODE ) */
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry50u2,  Oberteil ab */
/* EasyCODE F */
static void ry50u2 (void)
{
    y50u2.resAusgang();
    y50u2.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy51u1,   Schablonenverriegelung auf */
/* EasyCODE F */
static void sy51u1 (void)
{
    if ( !e51u2_1.getZustand() && !e51u2_2.getZustand() )
    {
        y51u1.resAusgang();
        y51u1.clearAuftrag();
    }
    else
    {
        if ( y51u1.getZustand() )
        {
            io.mainerr = Y51U1_HF;
            if ( e51u2_1.getZustand() )
            {
                io.secerr = E51U2_1_UF0;
            }
            else
            {
                io.secerr = E51U2_2_UF0;
            }
        }
        else
        {
            y51u2.resAusgang();
            y51u1.setAusgang();
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry51u1,   Schablonenverriegelung auf */
/* EasyCODE F */
static void ry51u1 (void)
{
    y51u1.resAusgang();
    y51u1.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy51u2,   Schablonenverriegelung zu */
/* EasyCODE F */
static void sy51u2 (void)
{
    if ( e51u2_1.getZustand() && e51u2_2.getZustand() )
    {
        y51u2.resAusgang();
        y51u2.clearAuftrag();
    }
    else
    {
        if ( y51u2.getZustand() || io.clamp_merker )
        {
            io.mainerr = Y51U2_HF;
            if ( !e51u2_1.getZustand() )
            {
                io.secerr = E51U2_1_UF1;
            }
            else
            {
                io.secerr = E51U2_2_UF1;
            }
            // SW013   begin
            if ( io.clamp_merker == FALSE )
            {
                if ( tio9.tz() )
                {
                    // Klammer oeffnen
                    
                    y51u1.setAusgang(); // im off geht er aus
                    y51u2.resAusgang();
                    
                    tio9.rsz ();    // SW013
                    tio9.sz (40);  // 0,4s
                    io.clamp_merker = TRUE;
                    
                    //y51u1.getZustand()
                    
                    // nach clear Auftrag werden die Error
                    // wieder genullt, darum koor err direkt
                    // setzen
                    
                    
                    
                    //koor.io_err = io.mainerr;
                    //koor.io_secerr = io.secerr;
                    
                    // nun vergehen noch 0,5s
                }
            }
            else
            {
                if ( tio9.tz() )
                {
                    y51u1.resAusgang(); // im off geht er aus
                    y51u2.resAusgang();
                    
                    y51u1.clearAuftrag(); 
                    y51u2.clearAuftrag(); 
                    io.clamp_merker = FALSE; 
                    koor.io_err = io.mainerr;
                    koor.io_secerr = io.secerr;
                }
            }
            // SW013  end
        }
        else
        {
            /* EasyCODE ( 0 
            verriegelungen pruefen */
            // falls Klammer eingelegt links:
            //   Klammer eingelegt rechts
            if ( e79.getZustand() && !e80.getZustand() )
            {
                io.secerr=E80_UF1;
                io.mainerr=Y51U2_HF;
            }
            else
            {
                // falls  Klammer eingelegt rechts:
                //   keine Klammer eingelegt links
                if ( !e79.getZustand() && e80.getZustand() )
                {
                    io.secerr=E79_UF1;
                    io.mainerr=Y51U2_HF;
                }
                else
                {
                    y51u1.resAusgang();
                    y51u2.setAusgang();
                    
                    
                    tio9.rsz ();    // SW013
                    tio9.sz (200);  // 2s
                    io.clamp_merker = FALSE; // SW013
                }
            }
            /* EasyCODE ) */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry51u2,   Schablonenverriegelung zu */
/* EasyCODE F */
static void ry51u2 (void)
{
    y51u2.resAusgang();
    y51u2.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy52,   Schablonen-Anschlag links auf */
/* EasyCODE F */
static void sy52 (void)
{
    y52.setAusgang();
    y52.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry52,   Schablonen-Anschlag links ab */
/* EasyCODE F */
static void ry52 (void)
{
    y52.resAusgang();
    y52.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy53,   Schablonen-Anschlag rechts auf */
/* EasyCODE F */
static void sy53 (void)
{
    y53.setAusgang();
    y53.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry53,   Schablonen-Anschlag rechts ab */
/* EasyCODE F */
static void ry53 (void)
{
    y53.resAusgang();
    y53.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy54,   Oberteilarretierung aus */
/* EasyCODE F */
static void sy54 (void)
{
    if ( y54.getZustand() )
    {
        if ( e54.getZustand() )
        {
            y54.clearAuftrag();
        }
        else
        {
            io.mainerr = Y54_HF1;
            io.secerr = E54_UF1;
        }
    }
    else
    {
        y54.setAusgang();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry54,   Oberteilarretierung ein */
/* EasyCODE F */
static void ry54 (void)
{
    if ( !e50u1.getZustand() && !e50u2.getZustand() )
    {
        // Oberteil oben oder unten
        io.secerr=E50U1_E50U2_UF1;
        io.mainerr=Y54_HF0;
    }
    else
    {
        if ( !y54.getZustand() )
        {
            if ( !e54.getZustand() )
            {
                y54.clearAuftrag();
            }
            else
            {
                io.mainerr = Y54_HF0;
                io.secerr = E54_UF0;
            }
        }
        else
        {
            y54.resAusgang();
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy61,   Clamp is closed */
/* EasyCODE F */
static void sy61 (void)
{
    if ( y61.getZustand() )
    {
        if ( tio10.tz() )
        {
            y61.resAusgang();
            y61.clearAuftrag();
        }
    }
    else
    {
        y61.setAusgang();
        tio10.rsz();
        tio10.sz(100);
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry61,   Clamp is closed */
/* EasyCODE F */
static void ry61 (void)
{
    y61.resAusgang();
    y61.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy62,   Jig is finished */
/* EasyCODE F */
static void sy62 (void)
{
    if ( y62.getZustand() )
    {
        if ( tio11.tz() )
        {
            y62.resAusgang();
            y62.clearAuftrag();
        }
    }
    else
    {
        y62.setAusgang();
        tio11.rsz();
        tio11.sz(100);
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry62,   Jig is finished */
/* EasyCODE F */
static void ry62 (void)
{
    y62.resAusgang();
    y62.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy63,   Clamp is open */
/* EasyCODE F */
static void sy63 (void)
{
    if ( y63.getZustand() )
    {
        if ( tio12.tz() )
        {
            y63.resAusgang();
            y63.clearAuftrag();
        }
    }
    else
    {
        y63.setAusgang();
        tio12.rsz();
        tio12.sz(100);
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry63,   Clamp is open */
/* EasyCODE F */
static void ry63 (void)
{
    y63.resAusgang();
    y63.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sbobres1,   Unterfadenwaechter ruecksetzen */
/* EasyCODE F */
static void sbobres1 (void)
{
    bobres1.setAusgang();
    bobres1.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
rbobres1,   Unterfadenwaechter ruecksetzen aus */
/* EasyCODE F */
static void rbobres1 (void)
{
    bobres1.resAusgang();
    bobres1.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sbobres2,   Unterfadenwaechter ruecksetzen */
/* EasyCODE F */
static void sbobres2 (void)
{
    bobres2.setAusgang();
    bobres2.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
rbobres2,   Unterfadenwaechter ruecksetzen aus */
/* EasyCODE F */
static void rbobres2 (void)
{
    bobres2.resAusgang();
    bobres2.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sout1,   programmierbarer Ausgang 1 ein (Unterfadenschieber) */
/* EasyCODE F */
static void sout1 (void)
{
    out1.setAusgang();
    out1.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
rout1,   programmierbarer Ausgang 1 aus (Unterfadenschieber) */
/* EasyCODE F */
static void rout1 (void)
{
    out1.resAusgang();
    out1.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sout2,   programmierbarer Ausgang 2 ein */
/* EasyCODE F */
static void sout2 (void)
{
    out2.setAusgang();
    out2.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
rout2,   programmierbarer Ausgang 2 aus */
/* EasyCODE F */
static void rout2 (void)
{
    out2.resAusgang();
    out2.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sout3,   programmierbarer Ausgang 3 ein */
/* EasyCODE F */
static void sout3 (void)
{
    out3.setAusgang();
    out3.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
rout3,   programmierbarer Ausgang 3 aus */
/* EasyCODE F */
static void rout3 (void)
{
    out3.resAusgang();
    out3.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sout4,   programmierbarer Ausgang 4 ein */
/* EasyCODE F */
static void sout4 (void)
{
    out4.setAusgang();
    out4.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
rout4, ,   programmierbarer Ausgang 4 aus */
/* EasyCODE F */
static void rout4 (void)
{
    out4.resAusgang();
    out4.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy4fkt,   Oberfadenklemmen-Funktion ein, Oberfadenklemme wird verzoegert auf geschaltet */
/* EasyCODE F */
static void sy4fkt (void)
{
    if ( saf->md.wedelnfussoben )
    {
        y4.resAusgang();
        y4fkt.clearAuftrag();
    }
    else
    {
        tio6.sz (saf->md.fadklemme_time);
        if ( tio6.tz() )
        {
            tio6.lz();
            y4.resAusgang();
            y4fkt.clearAuftrag();
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy2fkt_aus,   Fadenwedler in Grundstellung */
/* EasyCODE F */
static void sy2fkt_aus (void)
{
    if ( !y2u1.getZustand() && !y2u2.getZustand() )
    {
        if ( e2.getZustand() )
        {
            y2fkt_aus.clearAuftrag();
        }
        else
        {
            io.mainerr = Y2FKT_AUS_HF;
            io.secerr = E2_UF1;
        }
    }
    else
    {
        y2u1.resAusgang();
        y2u2.resAusgang();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy2fkt_ein,   Fadenwedler ein */
/* EasyCODE F */
static void sy2fkt_ein (void)
{
    if ( y2u1.getZustand() && y2u2.getZustand() )
    {
        if ( !e2.getZustand() )
        {
            y2fkt_ein.clearAuftrag();
        }
        else
        {
            io.mainerr = Y2FKT_EIN_HF;
            io.secerr = E2_UF0;
        }
    }
    else
    {
        y2u1.setAusgang();
        y2u2.setAusgang();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy2fkt_drucklos,   Fadenwedler drucklos */
/* EasyCODE F */
static void sy2fkt_drucklos (void)
{
    if ( !y2u1.getZustand() && y2u2.getZustand() )
    {
        if ( tio4.tz() )
        {
            // Fadenwedler in Grundstellung
            y2u1.resAusgang();
            y2u2.resAusgang();
            y2fkt_drucklos.clearAuftrag();
        }
    }
    else
    {
        y2u1.resAusgang();
        y2u2.setAusgang();
        tio4.rsz();
        tio4.sz (saf->md.fwk_time);
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy2fkt  Fadenwedlerfunktion */
/* EasyCODE F */
static void sy2fkt (void)
{
    if ( y2u1.getZustand() && y2u2.getZustand() )
    {
        if ( !e2.getZustand() )
        {
            if ( tio5.tz() )
            {
                y2u1.resAusgang();
                y2u2.resAusgang();
            }
        }
        else
        {
            io.secerr=E2_UF0;
            io.mainerr=Y2FKT_HF1;
        }
    }
    else
    {
        if ( saf->md.fadwedler_time!=0 )
        {
            if ( !io.ruecksetzmerker )
            {
                /* EasyCODE ( 0 
                Verriegelungen pruefen */
                if ( !saf->md.wedelnfussoben &&
                     ((hfuss.getpos()).pos != hfuss.akt_posunten.pos) )
                {
                    // Huepfer unten
                    io.secerr=HUEPFER_UNTEN;
                    io.mainerr=Y2FKT_HF1;
                }
                else
                {
                    if ( !y8.getZustand() && 
                         ((saf->variante==0) ||
                         ((saf->variante==1) && nup.find_UFWSTOP ())) )
                    {
                        // Niederhalter
                        // unten
                        io.secerr=Y8_UF1;
                        io.mainerr=Y2FKT_HF1;
                    }
                    else
                    {
                        if ( !nminf.nadel_ot() )
                        {
                            // Nadel in OT
                            io.secerr=NEEDLEPOS_UF1;
                            io.mainerr=Y2FKT_HF1;
                        }
                        else
                        {
                            tio5.rsz();
                            tio5.sz (saf->md.fadwedler_time);
                            y2u1.setAusgang();
                            y2u2.setAusgang();
                            io.ruecksetzmerker = TRUE;
                        }
                    }
                }
                /* EasyCODE ) */
            }
            else
            {
                if ( e2.getZustand() )
                {
                    io.ruecksetzmerker = FALSE;
                    y2fkt.clearAuftrag();
                }
                else
                {
                    io.secerr=E2_UF1;
                    io.mainerr=Y2FKT_HF1;
                }
            }
        }
        else
        {
            y2fkt.clearAuftrag();
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy9fkt,   Oberfaden ziehen beim Annaehen */
/* EasyCODE F */
static void sy9fkt (void)
{
    if ( y9.getZustand() )
    {
        if ( tio8.tz() )
        {
            y9.resAusgang();
            y9fkt.clearAuftrag();
        }
    }
    else
    {
        y9.setAusgang();
        tio8.rsz ();
        tio8.sz (saf->md.fza_time);
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
shfuss_auf  Hupferfuss auf */
/* EasyCODE F */
static void shfuss_auf (void)
{
    if ( saf->md.hfsicher )
    {
        if ( hfuss.auf_init() )
        {
            hfuss_auf.clearAuftrag();
        }
    }
    else
    {
        if ( hfuss.ready() )
        {
            hfuss_auf.clearAuftrag();
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sy7_y32fkt,   Nadeldreheinrichtung und Unterfadenschieber ein */
/* EasyCODE F */
static void sy7_y32fkt (void)
{
    if ( saf->md.nadelanzahl != 1 )
    {
        io.secerr=NADELANZAHL;
        io.mainerr=Y7_HF1;
    }
    else
    {
        y7.setAusgang();
        y32.setAusgang();
        y7_y32fkt.clearAuftrag();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ry7_y32fkt,   Nadeldreheinrichtung und Unterfadenschieber aus */
/* EasyCODE F */
static void ry7_y32fkt (void)
{
    y7.resAusgang();
    y32.resAusgang();
    y7_y32fkt.clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
sonderfrei */
/* EasyCODE F */
/* unbenutzte Auftragsbytes */
static void sonderfrei (void)
{
    io.Sonder[io.index].clearAuftrag();
}

/* EasyCODE ) */
/* EasyCODE ) */
