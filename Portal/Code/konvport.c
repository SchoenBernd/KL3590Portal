/* EasyCODE V8 */
/* EasyCODE ( 0 
konvport */
/* EasyCODE ( 0 
Kopf, Header */
/* EasyCODE < */
/*---------------------------------------------------------------------------------------------------
                       Wilfried Halfmann
                               07.9.2009
Unterprogramme zur Konvertierung des Abschnittdatensatzes
der Klasse 3590 Portal in den entsprechenden Maschinendatensatz
Modul konvport,   Dateiname: konvport.C
------------------------------------------------------------------------------------------------------

// énderungen (fuer Klasse 3588)
//
// 1.8.97:
// Stapelpunkt ist immer der letzte Punkt des Nahtbildes
// egal ob das Bild einen Patten besitzt oder nicht.
//
// 8.9.97:
// Funktion init_masch_param:
// Die Variable naehdatoffs wurde falsch initialisiert (alter Wert: 6,
// neuer Wert: 18). Dies fÅhrte zum falschen Verschieben von M-Funktionen
// mit negativen V-Parameteren Åber den Dateianfang hinaus.
//
// 19.5.98:
// Funktion is_koordabschn:
// Der Abschnitt pattenanf zaehlt nicht mehr als koordabschnitt.
// Funktion bearbeitefunktion:
// Die Koordinaten des Abschnitts pattenanf werden nicht mehr
// beruecksichtigt.
//
// 16.6.99:
// Umbau des Moduls fuer die Klasse 3587
// - Anpassung der Naehfeldgrenzen auf
//    X: -250mm .. + 250mm
//    Y: 0 .. -300mm
// - Die Funktion Zickzack Ein/Aus entfaellt.
// - Der Eintrag der Pattenposition im Stichdatensatz wird fuer die
//   Klasse 3587 nicht mehr benutzt. Es werden beide Werte fest auf
//   30000 gesetzt.
// - Der Stapelpunkt der Klasse 3588 wird hier als Programmendpunkt
//   benutzt.

//#define debug_info   /* Debug-Meldungen ausgeben */
#define clearmem

/* Folgende Fehlercodes werden von Konvert erzeugt:

  0 : kein Fehler
  1 : falsche Maschinenkennung in geometr. Datensatz       (Hauptprogramm)
  2 : Abschnitt Abschnitt 'Klammerform' oder
       Abschnitt 'Hindernis' fehlt oder an falscher Stelle  (HauptProgramm)
  3 : Inkrement zu gross                                   (StichPunktAusgeben)
  4 : Programmende ohne Fadenschneiden                     (BearbeiteFunktion)
  5 : Unzulaessige Stichlaengenangabe                      (BearbeiteFunktion)
  6 : falsches Element in geometr. Datensatz               (BearbeiteFunktion)
  7 : Eilgang obwohl Maschine naeht                        (Hauptprogramm / Eilgang)
  8 : Unzulaessige Stichlaengenangabe                      (BearbeiteGerade)
  9 : Unzulaessige Stichlaengenangabe                      (BearbeiteKreis)
 10 : Kreis-Stuetzpunkt = Kreis-Endpunkt                   (KreisAusDreiPunkten und Abschnittwinkel)
 11 : Division durch Null                                  (KreisAusDreiPunkten)
 12 : Unzulaessige Stichlaengenangabe                      (BearbeiteKurve)
 13 : Kein KoordAbschnitt vor Kurvenstuetzpunkt            (BearbeiteKurve)
 14 : Naefeldgrenze ueberschritten                         (StichPunktAusgeben)
 15 : Kurve ohne Endpunkt                                  (BearbeiteKurve)
 16 : MFktPuffer-Ueberlauf                                 (PufferSchreiben)
 17 : Annaeh - Befehl im Ladepunktprogramm                 (BearbeiteFunktion)
 18 : falscher Kurvenstuetzpunkt                           (CF)
 19 : falscher Kurvenstuetzpunkt                           (CF)
 20 : falscher Kurvenstuetzpunkt                           (CF)
 21 : falscher Kurvenstuetzpunkt                           (CF)
 22 : Stichlaenge nicht initialisiert                      (Hauptprogramm)
 23 : Ladepunktprogramm nicht beendet                      (BearbeiteFunktion)
 24 : Stichbreitenbefehl im Ladepunktprogramm              (BearbeiteFunktion)
 25 : Unzulaessiger Wert fuer Abschnitt.Stichr             (BearbeiteFunktion)
 26 : Schneidbefehl, obwohl Faden geschnitten              (BearbeiteFunktion)
 27 : Annaehbefehl, obwohl Maschine naeht                  (BearbeiteFunktion)
 28 : Schneidbefehl im Naehen-Aus-Bereich                  (BearbeiteFunktion)
 29 : Naehen-Aus-Befehl, obwohl Faden geschnitten     (BearbeiteFunktion)
 30 : Schneidbefehl direkt nach Annaehbefehl               (BearbeiteFunktion)
 31 : Anfangsriegel zu lang                                            (BearbeiteFunktion)

0H..0FFH: reserviert fuer Speicherueberlauf

0H..0FAH: noch frei

BH : Hauptprogramm (Bearbeitung eines Eilgangs)
CH : Hauptprogramm (Bearbeitung eines Eilgangs)
DH : StichpunktAusgeben
EH : MFktAusgeben
FH : Hauptprogramm (zu begin von Konvert)                                    }

*/

#include "portal.h"

KlasseKonvert konv;
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::konvert */
/* EasyCODE F */
byte KlasseKonvert::konvert (byte huge * proganf,    /* Zeiger auf das erste Byte des Programms */
                             word std_stichl,        /* StandardstichlÑnge */
                             word mem_avail,         /* Grî·e des Speicherbereichs */
                             word * abschnr)         /* Nummer des fehlerhaften Abschnitts */
                                                     /* Programmnummer = Abschnitt Nummer 1 */
                                                     /* wenn konvert != 0 */
{
    /* EasyCODE ( 0 
    Initialisierung */
    /* EasyCODE < */
    int             minfree, i;
    int             progklasse;
    boolean         vollstprog;
    abschntype      abschnitt;
    /* EasyCODE > */
    #if defined debug_info
        /* EasyCODE < */
        printf ("Konvertierung lÑuft\n");
        /* EasyCODE > */
    #endif 
    /* EasyCODE < */
    /* Initialisierung */
    prog_anf = proganf;
    init_masch_param ();
    
    fehler = 0;
    fehler14 = FALSE;
    
    bildanfkoord.x = 0;
    bildanfkoord.y = 0;
    lpkoord.x = 0;
    lpkoord.y = 0;
    stapelkoord.x = 0;
    stapelkoord.y = 0;
    pattenanfkoord.x = 30000;
    pattenanfkoord.y = 30000;
    
    progmitpatten = FALSE;
    maschinenaeht = FALSE;
    fadengeschnitten = TRUE;
    stichgemacht = FALSE;
    stini = FALSE;
    /* EasyCODE > */
    for ( i=0; i<=MaxRiegelstichzahl; i++ )
    {
        AnfRiegelStatus.Stich [i].x = 99999;
        AnfRiegelStatus.Stich [i].y = 99999;
        AnfRiegelStatus.fertig = TRUE;
        AnfRiegelStatus.AktStichnr = 0;
        AnfRiegelStatus.RStiche = 0;
        AnfRiegelStatus.VStiche = 0;
        AnfRiegelStatus.RTeilstiche = 1;
        AnfRiegelStatus.VTeilstiche = 1;
    }
    for ( i=0; i<=MaxRiegelstichzahl; i++ )
    {
        EndRiegelKoord [i].x = 99999;
        EndRiegelKoord [i].y = 99999;
    }
    //  ausgaenge = emptySet;
    for ( i = 0; i < 8; i++ )
    {
        ausgang_gesetzt[ i ] = 0;
    }
    ausg_niveau = 0;
    ausg_lochabs = 0;
    ausg_liklammer = 0;
    ausg_reklammer = 0;
    ausg_zuspann = 0;
    ausg_zickzack = 0;
    
    anzahlstiche = 0;
    fstichl = 0;
    stichlausdatens = 0;
    stichbr = 0;
    stichrichtungli = TRUE;
    stichlage = mittigli;
    stichnachlinks.x = 0;
    stichnachlinks.y = 0;
    riegelanfang = FALSE;
    riegelende = FALSE;
    for ( i = 1; i <= MFKTPUFFERGR; i++ )
    {
        mfktpuffer[i - 1].verschieb = 0;
    }
    mfktinpuffer = FALSE;
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Kennung prÅfen */
    /* EasyCODE < */
    /* Maschinen-Kennung abpruefen */
    /* mu· nur bei vollstÑndigen Programmen Åbereinstimmen */
    
    abschanf = geooffs;   /* Anfangsadresse Abschnittdatensatz */
    rdabsch(&abschnitt, (abschanf + kennoffs));   /* KennAbschnitt einlesen */
    /* EasyCODE > */
    if ( (abschnitt.abschnittart == naehprogkenn) )
    {
        /* EasyCODE < */
        vollstprog = (abschnitt._VAR.shilfskonst2.firstword / 256) == 0; /* MSB (Abschnitt.FirstWord) */
        progklasse = abschnitt._VAR.shilfskonst2.firstword % 256;        /* LSB (Abschnitt.FirstWord) */
        /* EasyCODE > */
        if ( vollstprog )
        {
            if ( progklasse != kportal )
            {
                fehler = 1;
                abschadr = abschanf + kennoffs;
            }
        }
    }
    else
    {
        fehler = 1;
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Anfangsadresse des Maschinendatensatzes bestimmen */
    if ( vollstprog )
    {
        rdabsch (&abschnitt, (24));
    }
    else
    {
        rdabsch (&abschnitt, 20);
    }
    /* EasyCODE < */
    maschanf = (word)abschnitt._VAR.shilfskonstr.memdwort;
    /* EasyCODE > */
    #if defined clearmem
        /* EasyCODE < */
        /* 512 Byte im NÑhdatenspeicher lîschen */
        memset (prog_anf + maschanf, 0, 0x200);
        /* EasyCODE > */
    #endif 
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Speicherplatz fÅr Kopf vorhanden ? */
    /* EasyCODE < */
    maxmem = mem_avail;
    minfree = 21;               /* Kopf (18) + ProgEnd(3) */
    /* EasyCODE > */
    if ( (maschanf + minfree - 1) >= maxmem )
    {
        fehler = 0xFF;
    }
    /* EasyCODE ) */
    if ( fehler == 0 )
    {
        /* KlammerCode bzw. Klammerart ausgeben */
        rdabsch(&abschnitt, abschanf + klammeroffs);
        if ( (abschnitt.abschnittart == klammercode) )
        {
            wrbyte(maschanf, 0);//abschnitt._VAR.sklammercode.code);
            /* EasyCODE - */
            /* Hindernisse ausgeben */
            rdabsch(&abschnitt, abschanf + hindernoffs);
            if ( (abschnitt.abschnittart == hindernis) )
            {
                wrbyte(maschanf + 1, (abschnitt._VAR.shindernis.vorhanden ? 255 : 0));
                /* Stichanzahl an Offset maschanf + 2 wird spÑter eingetragen */
                /* Standardstichlaenge ausgeben */
                wrword((maschanf + 4), std_stichl);
                fstichl = std_stichl / (float)(10);
                i_stdstichl = std_stichl;
                maschadr = maschanf + 18;
                lpprogramm = FALSE;
                /* EasyCODE ( 0 
                Konvertieren */
                abschadr = abschanf + abschdataoffs;
                letztereinstich.x = 0;
                letztereinstich.y = 0;
                letztekoord = letztereinstich;
                while ( (abschadr < maschanf) && (maschadr < maxmem) && (fehler == 0) )
                {
                    /* EasyCODE < */
                    readnextabsch(&abschnitt);
                    /* EasyCODE > */
                    #if defined debug_info
                        /* EasyCODE < */
                        abschn_ausgeben (abschnitt);
                        /* EasyCODE > */
                    #endif 
                    switch ( abschnitt.abschnittart )
                    {
                    case anfpkt :
                        /* EasyCODE ( 0 
                        Anfangspunkt */
                        /* EasyCODE < */
                        bildanfkoord = abschnitt._VAR.skoordabsch.koord;
                        /* Pruefen der Naehfeldgrenzen */
                        /* EasyCODE > */
                        if ( (bildanfkoord.x < minnaehfeld_x) || (bildanfkoord.x > maxnaehfeld_x) ||
                             (bildanfkoord.y < minnaehfeld_y) || (bildanfkoord.y > maxnaehfeld_y) )
                        {
                            fehler = 14;
                        }
                        else
                        {
                            letztekoord.x = abschnitt._VAR.skoordabsch.koord.x;
                            letztekoord.y = abschnitt._VAR.skoordabsch.koord.y;
                            letztereinstich = letztekoord;
                        }
                        break;
                        /* EasyCODE ) */
                    case stich :
                        /* EasyCODE ( 0 
                        Einzelstich */
                        bearbeitestich(abschnitt);
                        break;
                        /* EasyCODE ) */
                    case gerade :
                        /* EasyCODE = */
                    case krsstuetzpkt :
                        /* EasyCODE = */
                    case bgnstuetzpkt :
                        /* EasyCODE = */
                    case kurve :
                        /* EasyCODE = */
                    case kurvenende :
                        /* EasyCODE ( 0 
                        Gerade, KreisstÅtzpunkt, BogenstÅtzpunkt, Kurvenpunkt, Kurvenendpunkt */
                        if ( lpprogramm )
                        {
                            fstichl = LPSTICHL;
                        }
                        else
                        {
                            if ( stini )
                            {
                                fstichl = stichlausdatens;
                            }
                            else
                            {
                                fehler = 22;
                                break;
                            }
                        }
                        switch ( abschnitt.abschnittart )
                        {
                        case gerade :
                            bearbeitegerade(abschnitt);
                            break;
                        case krsstuetzpkt :
                            /* EasyCODE = */
                        case bgnstuetzpkt :
                            bearbeitekreis(abschnitt, FALSE);
                            break;
                        case kurve :
                            /* EasyCODE = */
                        case kurvenende :
                            bearbeitekurve(FALSE);
                            break;
                        default:
                            ;
                        }
                        break;
                        /* EasyCODE ) */
                    case eilgang :
                        /* EasyCODE ( 0 
                        Eilgang */
                        if ( maschinenaeht )
                        {
                            fehler = 7;
                        }
                        else
                        {
                            if ( maschadr + 5 < maxmem )
                            {
                                wrword (maschadr, 0x20FF);
                                maschadr = maschadr + 2;
                                
                                eilinc.x = round(abschnitt._VAR.skoordabsch.koord.x - letztereinstich.x);
                                eilinc.y = round(abschnitt._VAR.skoordabsch.koord.y - letztereinstich.y);
                                while ( (abs(eilinc.x) > MAXINCR) || (abs(eilinc.y) > MAXINCR) && (fehler == 0) )
                                {
                                    if ( abs(eilinc.x) > MAXINCR )
                                    {
                                        if ( eilinc.x > 0 )
                                        {
                                            wrbyte (maschadr, MAXINCR);
                                            eilinc.x = eilinc.x - MAXINCR;
                                        }
                                        else
                                        {
                                            wrbyte(maschadr, MAXINCR + 128);
                                            eilinc.x = eilinc.x + MAXINCR;
                                        }
                                    }
                                    else
                                    {
                                        wrbyte(maschadr, 0);
                                    }
                                    if ( abs(eilinc.y) > MAXINCR )
                                    {
                                        if ( eilinc.y > 0 )
                                        {
                                            wrbyte(maschadr + 1, MAXINCR);
                                            eilinc.y = eilinc.y - MAXINCR;
                                        }
                                        else
                                        {
                                            wrbyte(maschadr + 1, MAXINCR + 128);
                                            eilinc.y = eilinc.y + MAXINCR;
                                        }
                                    }
                                    else
                                    {
                                        wrbyte(maschadr + 1, 0);
                                    }
                                    maschadr = maschadr + 2;
                                    if ( maschadr + 3 >= maxmem )
                                    {
                                        fehler = 0xFB;
                                    }
                                }
                                if ( fehler == 0 )
                                {
                                    wrbyte(maschadr, vorzbetr(eilinc.x));
                                    wrbyte(maschadr + 1, vorzbetr(eilinc.y));
                                    wrbyte(maschadr + 2, 0xFF);
                                }
                                letztereinstich.x = abschnitt._VAR.skoordabsch.koord.x;
                                letztereinstich.y = abschnitt._VAR.skoordabsch.koord.y;
                                letztekoord = letztereinstich;
                                maschadr = maschadr + 3;
                            }
                            else
                            {
                                fehler = 0xFC;
                            }
                        }
                        break;
                        /* EasyCODE ) */
                    default:
                        /* EasyCODE ( 0 
                        Maschinenfunktion */
                        bearbeitefunktion(abschnitt);
                        break;
                        /* EasyCODE ) */
                    }
                    if ( fehler14 )
                    {
                        fehler = 14;
                    }
                }
                /* EasyCODE ) */
            }
            else
            {
                fehler = 2;
            }
        }
        else
        {
            fehler = 2;
        }
    }
    /* EasyCODE ( 0 
    End- und Fehlerbehandlung */
    /* EasyCODE < */
    /* Fehlerbehandlung: - setzen des Flags ProgGueltig - setzen der Variablen
     * KonvErr und der Nummer des fehlerhaften Abschnitts
     *
     * Im Fehlerfall zeigen die relative Anfangsadresse des Stichdatensatzes und
     * des Nahtbildprogramms auf MaschAnf. Die Laenge des Programms entspricht
     * der Laenge des Geometrischen Datensatzes */
    /* EasyCODE > */
    if ( fehler == 0 )
    {
        /* fehlende Informationen nachtragen */
        /* Bildanfangspunkt */
        wrword((maschanf + 6), bildanfkoord.x);
        wrword((maschanf + 8), bildanfkoord.y);
        
        /* Stapelposition */
        wrword((maschanf + 10), stapelkoord.x);
        wrword((maschanf + 12), stapelkoord.y);
        
        /* Pattenanfangspunkt */
        wrword((maschanf + 14), pattenanfkoord.x);
        wrword((maschanf + 16), pattenanfkoord.y);
        
        /* Stichanzahl nachtragen */
        wrword(maschanf + 2, anzahlstiche);
        
        /* Programm als vollstaendig kennzeichnen */
        wrbyte(abschanf + 7, 0);
        
        *abschnr = 0;
    }
    else
    {
        maschadr = maschanf;
        lpmaschadr = maschanf - 3;
        /* Programm als unvollstaendig kennzeichnen */
        wrbyte(abschanf + 7, 0xFF);
        
        *abschnr = trunc((abschadr - abschanf) / (float)(5)) - 1;
        if ( *abschnr < 1 )
        {
            *abschnr = 1;
        }
    }
    /* 0FFH hinter das Programm schreiben */
    wrbyte(maschadr, 0xFF);
    
    /* Laenge des Programms in Datensatz eintragen */
    wrdword(21, (dword)maschadr);
    
    /* AnfangsAdr des StichDatensatzes eintragen */
    wrdword(25, (dword)maschanf);
    return (fehler);
    /* EasyCODE ) */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::init_masch_param */
/* EasyCODE F */
// Initialisierung der MaschinenabhÑngigen Variable

void KlasseKonvert::init_masch_param (void)
{
    geooffs = 33;        /*begin des geometrischen datensatzes*/
    
    /*offsetwerte relativ zum anfang des geometrischen datensatzes*/
      kennoffs      =  5;  /*maschinenkennung  */
      klammeroffs   = 10;  /*klammerart bei 3574, klammercode bei 3568*/
      hindernoffs   = 15;  /*hindernisse       */
      mnullpoffs    = 20;  /*maschinennullpunkt*/
      abschdataoffs = 25;  /*abschnittdatensatz*/
    
    
    /*maschinenspez. parameter*/
      maschmitlpprogramm = FALSE;
      naehdatoffs   = 18;         /*erstes byte des frei programmierbaren naehprogramms*/
      mindrehzahl   = 200;
      maxdrehzahl   = 2000;
      maxstichl     = 120;        /*maximale stichlaenge in 1/10 mm*/
    #ifdef NF120x70
        minnaehfeld_x = -6000;        /*naehfeldgrenzen in 1/10 mm*/
        maxnaehfeld_x =  6000;
        minnaehfeld_y = -7000;
        maxnaehfeld_y =     0;
    #endif 
    #ifdef NF120x170
        minnaehfeld_x = -6000;        /*naehfeldgrenzen in 1/10 mm*/
        maxnaehfeld_x =  6000;
        minnaehfeld_y = -17000;
        maxnaehfeld_y =     0;
    #endif 
    #ifdef NF100x50
        minnaehfeld_x = -5000;        /*naehfeldgrenzen in 1/10 mm*/
        maxnaehfeld_x =  5000;
        minnaehfeld_y = -5000;
        maxnaehfeld_y =     0;
    #endif 
    #ifdef NF120x200
        minnaehfeld_x = -6000;        /*naehfeldgrenzen in 1/10 mm*/
        maxnaehfeld_x =  6000;
        minnaehfeld_y = -20000;
        maxnaehfeld_y =     0;
    #endif 
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::is_koordabschn */
/* EasyCODE F */
boolean KlasseKonvert::is_koordabschn (abschnarttype abschnittart)
{
    boolean ret_val;
    switch ( abschnittart )
    {
    case gerade :
        /* EasyCODE = */
    case kreisbogen :
        /* EasyCODE = */
    case kreis :
        /* EasyCODE = */
    case kurve :
        /* EasyCODE = */
    case bgnstuetzpkt :
        /* EasyCODE = */
    case krsstuetzpkt :
        /* EasyCODE = */
    case kurvenende :
        /* EasyCODE = */
    case stich :
        /* EasyCODE = */
    case eilgang :
        /* EasyCODE = */
    case bnullpkt :
        /* EasyCODE = */
    case mnullpkt :
        /* EasyCODE = */
    case ladepkt :
        /* EasyCODE = */
    case anfpkt :
        /* EasyCODE < */
        ret_val = 1;
        /*    case pattenanf: ret_val = 1;*/
        /* EasyCODE > */
        break;
    default:
        ret_val = 0;
        break;
    }
    return (ret_val);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::is_kurvenpunkt */
/* EasyCODE F */
boolean KlasseKonvert::is_kurvenpunkt (abschnarttype abschnittart)
{
    boolean ret_val;
    switch ( abschnittart )
    {
    case kurve :
        /* EasyCODE = */
    case kurvenende :
        ret_val = 1;
        break;
    default:
        ret_val = 0;
        break;
    }
    return (ret_val);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::wrabsch */
/* EasyCODE F */
void KlasseKonvert::wrabsch (abschntype * abschnitt, offs_adr offset)
{
    byte i;
    byte * adr = (byte *) abschnitt;
    
    prog_anf[offset] = * adr;
    for ( i = 1; i < 5; i++ )
    {
        prog_anf[offset + i] = adr[i + 1];
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::rdabsch */
/* EasyCODE F */
void KlasseKonvert::rdabsch (abschntype * abschnitt, offs_adr offset)
{
    abschnitt->abschnittart = prog_anf[offset];
    abschnitt->_VAR.skoordabsch.koord = * (ikoordtyp*) (&prog_anf[offset + 1]);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::wrdword */
/* EasyCODE F */
void KlasseKonvert::wrdword (offs_adr offset, dword datenwort)
{
    * (dword *)(&prog_anf[offset]) = datenwort;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::wrword */
/* EasyCODE F */
void KlasseKonvert::wrword  (offs_adr offset, word datenwort)
{
    * (word *) (&prog_anf[offset]) = datenwort;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::wrbyte */
/* EasyCODE F */
void KlasseKonvert::wrbyte  (offs_adr offset, byte maschbyte)
{
    prog_anf[offset] = maschbyte;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::membyte */
/* EasyCODE F */
byte KlasseKonvert::membyte (offs_adr offset)
{
    return (prog_anf[offset]);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::movblock */
/* EasyCODE F */
void KlasseKonvert::movblock (offs_adr blockanf, offs_adr blockende, word anzahl)
{
    word i;
    for ( i=blockende;i>=blockanf;i-- )
    {
        prog_anf[i + anzahl] = prog_anf[i];
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::vorg */
/* EasyCODE F */
offs_adr KlasseKonvert::vorg (offs_adr offset)
{
    return (offset - 5);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::nachf */
/* EasyCODE F */
offs_adr KlasseKonvert::nachf (offs_adr offset)
{
    return (offset + 5);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::readnextabsch */
/* EasyCODE F */
void KlasseKonvert::readnextabsch (abschntype * abschnitt)
{
    rdabsch (abschnitt, abschadr);
    abschadr = nachf (abschadr);
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::drehwinkel */
/* EasyCODE F */
float KlasseKonvert::drehwinkel (koordinate mittelpunkt, koordinate endpunkt)

/* Winkel zwischen positiver x-Achse und Vektor Mittelpunkt-Endpunkt */
{
    float       _ret_drehwinkel;
    koordinate  abst;
    float       winkel;
    
    abst.x = endpunkt.x - mittelpunkt.x;
    abst.y = endpunkt.y - mittelpunkt.y;
    if ( fabs(abst.x) < 0.01 )
    {
        if ( fabs(abst.y) < 0.01 )
        {
        }
        else
        {
            if ( abst.y < 0 )
            {
                _ret_drehwinkel = (3 / (float) (2)) * PI;
            }
            else
            {
                _ret_drehwinkel = PI / 2;
            }
        }
    }
    else
    {
        if ( abst.x < 0 )
        {
            _ret_drehwinkel = atan(abst.y / abst.x) + PI;
        }
        else
        {
            winkel = atan(abst.y / abst.x);
            if ( fabs(abst.y) < 0.01 )
            {
                _ret_drehwinkel = 0;
            }
            else
            {
                if ( abst.y < 0 )
                {
                    _ret_drehwinkel = winkel + 2 * PI;
                }
                else
                {
                    _ret_drehwinkel = winkel;
                }
            }
        }
    }
    return _ret_drehwinkel;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::kreisaus3punkten */
/* EasyCODE F */
void KlasseKonvert::kreisaus3punkten (koordinate punkt1,
                       koordinate punkt2,
                       koordinate punkt3,
                       float * radius,
                       koordinate * mittelpunkt,
                       float * richtung,
                       byte * fehler)
{
    koordinate abst12,
               abst13;
    float      anfwinkel,
               stuetzpwinkel,
               endwinkel,
               halbesekante,
               quotient,
               nenner;
    
    *fehler = 0;
    abst12.x = punkt2.x - punkt1.x;
    abst12.y = punkt2.y - punkt1.y;
    abst13.x = punkt3.x - punkt1.x;
    abst13.y = punkt3.y - punkt1.y;
    if ( (abst13.x == 0) && (abst13.y == 0) )
    {
        *fehler = 10;
    }
    else
    {
        halbesekante = (sqrt(fsqr(abst12.x) + fsqr(abst12.y))) / 2;
        nenner = (abst12.y * abst13.x - abst13.y * abst12.x);
        quotient = 0;
        if ( nenner == 0 )
        {
            *fehler = 11;
        }
        else
        {
            quotient = ((abst13.x - abst12.x) * abst13.x + (abst13.y - abst12.y) * abst13.y) / nenner;
        }
        *radius = halbesekante * sqrt(1 + fsqr(quotient));
        mittelpunkt->x = punkt1.x + (abst12.x + quotient * abst12.y) / 2;
        mittelpunkt->y = punkt1.y + (abst12.y - quotient * abst12.x) / 2;
    }
    /* EasyCODE < */
    /* Drehrichtung ermitteln (+ 1 fuer math. positiv, - 1 fuer math. negativ) */
    /* EasyCODE > */
    if ( *fehler == 0 )
    {
        /* EasyCODE < */
        anfwinkel = drehwinkel(*mittelpunkt, punkt1);       /* Winkel zwischen
                                                             * Vektor
                                                             * Mittelpunkt-AnfPunkt
                                                             * und X-Achse */
        stuetzpwinkel = drehwinkel(*mittelpunkt, punkt2);   /* Winkel zwischen
                                                             * Vektor
                                                             * Mittelpunkt-Stuetzpunk
                                                             * t und X-Achse */
        endwinkel = drehwinkel(*mittelpunkt, punkt3);       /* Winkel zwischen
                                                             * Vektor
                                                             * Mittelpunkt-Endpunkt
                                                             * und X-Achse */
        /* EasyCODE > */
        if ( stuetzpwinkel < anfwinkel )
        {
            stuetzpwinkel = stuetzpwinkel + 2 * PI;
        }
        if ( endwinkel < anfwinkel )
        {
            endwinkel = endwinkel + 2 * PI;
        }
        *richtung = 1;
        if ( (stuetzpwinkel - anfwinkel) > (endwinkel - anfwinkel) )
        {
            *richtung = -1;
        }
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::abschnittwinkel */
/* EasyCODE F */
float KlasseKonvert::abschnittwinkel (koordinate anfpunkt,
                       koordinate endpunkt,
                       float richtung,
                       koordinate mittelpunkt,
                       byte * fehler)

/* Winkel zwischen Vektor Mittelpunkt->Anfangspunkt und Vektor
 * Mittelpunkt->Endpunkt (ueber Kreispunkt)        */
{
    float  _ret_abschnittwinkel;
    float  winkel;
    
    winkel = drehwinkel(mittelpunkt, endpunkt) - drehwinkel(mittelpunkt, anfpunkt);
    if ( fabs(winkel) < 0.000001 )
    {
        winkel = 0;
        *fehler = 10;
    }
    if ( (richtung > 0) && (winkel < 0) )
    {
        winkel = winkel + 2 * PI;
    }
    if ( (richtung < 0) && (winkel > 0) )
    {
        winkel = winkel - 2 * PI;
    }
    _ret_abschnittwinkel = winkel;
    return _ret_abschnittwinkel;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::berechnevektor */
/* EasyCODE F */
void KlasseKonvert::berechnevektor (float radius,
                     float stichwinkel,
                     koordinate * vektor,
                     float betrstichnlinks,
                     boolean * stichrichtungli,
                     stichrtyp stichlage)

/* Berechnung des Vektors vom Mittelpunkt zum Einstichpunkt bei
 * Kreisberechnug */
{
    float ueberradius;
    switch ( stichlage )
    {
    case mittigli :
        /* EasyCODE = */
    case mittigre :
        /* EasyCODE :
         */
        {
            if ( *stichrichtungli )
            {
                ueberradius = radius + betrstichnlinks;
            }
            else
            {
                ueberradius = radius - betrstichnlinks;
            }
        }

        break;
    case revongrundl :
        /* EasyCODE :
         */
        {
            if ( *stichrichtungli )
            {
                ueberradius = radius;
            }
            else
            {
                ueberradius = radius - 2 * betrstichnlinks;
            }
        }

        break;
    case livongrundl :
        /* EasyCODE :
         */
        {
            if ( *stichrichtungli )
            {
                ueberradius = radius + 2 * betrstichnlinks;
            }
            else
            {
                ueberradius = radius;
            }
        }

        break;
    default:
        ;
    }
    *stichrichtungli = !(*stichrichtungli);
    vektor->x = ueberradius * cos(stichwinkel);
    vektor->y = ueberradius * sin(stichwinkel);
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::initmatrix */
/* EasyCODE F */
void KlasseKonvert::initmatrix (abbmatrixtyp abbmatrix)

/* Vorbesetzen einer AbbildungsMatrix fuer 1:1 - Abbildung */
{
    int i, j;
    for ( i = 1; i <= 2; i++ )
    {
        for ( j = 1; j <= 3; j++ )
        {
            if ( i == j )
            {
                abbmatrix[i - 1][j - 1] = 1;
            }
            else
            {
                abbmatrix[i - 1][j - 1] = 0;
            }
        }
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::matmultipl */
/* EasyCODE F */
void KlasseKonvert::matmultipl (abbmatrixtyp matrix1,
                 abbmatrixtyp matrix2)

/* Multiplikation 2er Abbildungsmatritzen */
/* Matrix1 := Matrix2 * Matrix1 */
{
    abbmatrixtyp    hilfsmatrix;
    
    memcpy(hilfsmatrix, (*matrix1), sizeof(hilfsmatrix));
    matrix1[0][0] = matrix2[0][0] * hilfsmatrix[0][0] + matrix2[0][1] * hilfsmatrix[1][0];
    matrix1[1][0] = matrix2[1][0] * hilfsmatrix[0][0] + matrix2[1][1] * hilfsmatrix[1][0];
    matrix1[0][1] = matrix2[0][0] * hilfsmatrix[0][1] + matrix2[0][1] * hilfsmatrix[1][1];
    matrix1[1][1] = matrix2[1][0] * hilfsmatrix[0][1] + matrix2[1][1] * hilfsmatrix[1][1];
    matrix1[0][2] = matrix2[0][0] * hilfsmatrix[0][2] + matrix2[0][1] * hilfsmatrix[1][2] + matrix2[0][2];
    matrix1[1][2] = matrix2[1][0] * hilfsmatrix[0][2] + matrix2[1][1] * hilfsmatrix[1][2] + matrix2[1][2];
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::vorzbetr */
/* EasyCODE F */
/*----------------------------------------------------------------------------
| Hilfsunterprogrammme zur Datenausgabe
----------------------------------------------------------------------------*/

byte KlasseKonvert::vorzbetr (int intzahl)
{
    byte _ret_vorzbetr;
    if ( intzahl >= 0 )
    {
        _ret_vorzbetr = abs(intzahl);
    }
    else
    {
        _ret_vorzbetr = abs(intzahl) + 128;
    }
    return _ret_vorzbetr;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::writefkt */
/* EasyCODE F */
void KlasseKonvert::writefkt (mfunkttyp maschfkt, offs_adr adresse)

/* Schreiben einer Maschinenfunktion an 'Adresse' */
{
    wrbyte(adresse, 0xFF);
    wrbyte(adresse + 1, maschfkt.befehl);
    if ( maschfkt.laenge == 4 )
    {
        wrbyte(adresse + 2, maschfkt.operand);
        wrbyte(adresse + 3, 0xFF);
    }
    else
    {
        wrbyte(adresse + 2, 0xFF);
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::pufferschreiben */
/* EasyCODE F */
/* Pufferverwaltung zur Verschiebung von Maschinenfunktionen */

void KlasseKonvert::pufferschreiben (mfunkttyp maschfkt)
{
    int     i;
    boolean fertig;
    
    i = 1;
    fertig = FALSE;
    while ( (i <= MFKTPUFFERGR) && !fertig )
    {
        if ( mfktpuffer[i - 1].verschieb == 0 )
        {
            mfktpuffer[i - 1] = maschfkt;
            fertig = TRUE;
        }
        else
        {
            i = i + 1;
        }
    }
    if ( fertig )
    {
        mfktinpuffer = TRUE;
    }
    else
    {
        fehler = 16;
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::mfktausgeben */
/* EasyCODE F */
void KlasseKonvert::mfktausgeben (mfunkttyp maschfkt)

/* Ausgabe einer Maschinenfunktion; Berechnen der richtigen Adresse,
 * entsprechend dem VerschiebeParameter (Funktionen mit Positivem V -
 * Parameter werden in einen Puffer geschrieben) Pruefen ob der Speicheplatz
 * ausreicht */
{
    offs_adr     einfuegepos;
    if ( maschfkt.laenge > 2 )
    {
        if ( maschadr + maschfkt.laenge < maxmem )
        {
            if ( maschfkt.verschieb <= 0 )
            {
                if ( maschfkt.verschieb == 0 )
                {
                    writefkt(maschfkt, maschadr);
                }
                else
                {
                    /* EasyCODE < */
                    einfuegepos = maschadr;
                    /* Rueckwaertslesen bis zur Einfuegeposition */
                    /* EasyCODE > */
                    while ( maschfkt.verschieb < 0 )
                    {
                        if ( membyte(einfuegepos - 1) == 0xFF )
                        {
                            einfuegepos = einfuegepos - 1;
                            do{
                                /* Maschinenfunktion ueberspringen */
                                einfuegepos = einfuegepos - 1;
                            }while ( !(membyte(einfuegepos) == 0xFF) );
                        }
                        else
                        {
                            /* Koordinatenpaar ueberspringen */
                            einfuegepos = einfuegepos - 2;
                            maschfkt.verschieb = maschfkt.verschieb + 1;
                        }
                    }
                    /* EasyCODE < */
                    /* Wenn Verschiebeparameter zu gross, einfuegen am Programmanfang
                     * bzw hinter dem Ladepunktprogramm */
                    /* EasyCODE > */
                    if ( maschmitlpprogramm )
                    {
                        if ( einfuegepos < (lpmaschadr + 3) )
                        {
                            einfuegepos = lpmaschadr + 3;
                        }
                    }
                    else
                    {
                        if ( einfuegepos < (maschanf + naehdatoffs) )
                        {
                            einfuegepos = maschanf + naehdatoffs;
                        }
                    }
                    /* Daten hinter der Einfuegeposition verschieben */
                    movblock(einfuegepos, maschadr - 1, maschfkt.laenge);
                    
                    /* Funktion ausgeben */
                    writefkt(maschfkt, einfuegepos);
                }
                maschadr = maschadr + maschfkt.laenge;
            }
            else
            {
                pufferschreiben(maschfkt);
            }
        }
        else
        {
            fehler = 0xFE;
        }
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::stichpunktausgeben */
/* EasyCODE F */
void KlasseKonvert::stichpunktausgeben (fkoordtyp koord)

/* Umrechnung von Absolut- In RelativKoordinaten und Ausgabe der
 * RelativKoordinaten; Die Variablen MaschAdr, AnzahlStiche und
 * LetzterEinstich werden aktualisiert */
{
    mfunkttyp maschfkt;
    int i, j, k, anzmfkt;
    koordinate Vektor;
    float SticheVProStichR;
    if ( (maschadr + 2) < maxmem )
    {
        if ( fehler == 0 )
        {
            /* EasyCODE ( 0 
            NÑhfeldgrenzen prÅfen */
            if ( koord.x < minnaehfeld_x )
            {
                fehler14 = TRUE;
                koord.x = minnaehfeld_x;
            }
            if ( koord.x > maxnaehfeld_x )
            {
                fehler14 = TRUE;
                koord.x = maxnaehfeld_x;
            }
            if ( koord.y < minnaehfeld_y )
            {
                fehler14 = TRUE;
                koord.y = minnaehfeld_y;
            }
            if ( koord.y > maxnaehfeld_y )
            {
                fehler14 = TRUE;
                koord.y = maxnaehfeld_y;
            }
            /* EasyCODE ) */
            if ( EndRiegelKoord[MaxRiegelstichzahl].x == 99999 )
            {
                EndRiegelKoord[MaxRiegelstichzahl] = letztereinstich;
            }
            if ( AnfRiegelStatus.fertig )
            {
                /* EasyCODE ( 0 
                Normaler Stich */
                if ( maschinenaeht && !stichgemacht )
                {
                    // AnnÑhen einfÅgen
                    maschfkt.laenge = 3;
                    maschfkt.befehl = 1;
                    maschfkt.verschieb = 0;
                    mfktausgeben (maschfkt);
                }
                stichout (koord, &letztereinstich);
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                Riegelstich */
                if ( AnfRiegelStatus.AktStichnr == 0 )
                {
                    AnfRiegelStatus.Stich[0] = letztereinstich;
                    AnfRiegelStatus.Stich[1] = koord;
                    AnfRiegelStatus.AktStichnr = 2;
                    letztereinstich_mem = letztereinstich;
                }
                else
                {
                    AnfRiegelStatus.Stich [AnfRiegelStatus.AktStichnr] = koord;
                    AnfRiegelStatus.AktStichnr++;
                }
                letztereinstich = koord;
                if ( AnfRiegelStatus.AktStichnr == (int)((float)AnfRiegelStatus.RStiche/AnfRiegelStatus.RTeilstiche +0.9)+1 )
                {
                    for ( i=0; i<37; i++ )
                    {
                        RiegelsticheV [i].x  = 99999;
                        RiegelsticheV [i].y  = 99999;
                        RiegelsticheR [i].x  = 99999;
                        RiegelsticheR [i].y  = 99999;
                    }
                    letztereinstich = letztereinstich_mem;
                    /* EasyCODE ( 0 
                    RÅckwÑrtsstiche berechnen und zwischenspeichern */
                    k = 1;
                    RiegelsticheR [0] = AnfRiegelStatus.Stich[0];
                    for ( i=0; i<AnfRiegelStatus.AktStichnr-1; i++ )
                    {
                        Vektor.x =AnfRiegelStatus.Stich[i+1].x - AnfRiegelStatus.Stich[i].x;
                        Vektor.y =AnfRiegelStatus.Stich[i+1].y - AnfRiegelStatus.Stich[i].y;
                        /* EasyCODE ( 0 
                        Teilstiche berechnen */
                        for ( j=1; j<=AnfRiegelStatus.RTeilstiche; j++ )
                        {
                            if ( j == AnfRiegelStatus.RTeilstiche )
                            {
                                RiegelsticheR [k] = AnfRiegelStatus.Stich [i+1];
                            }
                            else
                            {
                                RiegelsticheR[k].x = AnfRiegelStatus.Stich[i].x + ((Vektor.x*j)/AnfRiegelStatus.RTeilstiche);
                                RiegelsticheR[k].y = AnfRiegelStatus.Stich[i].y + ((Vektor.y*j)/AnfRiegelStatus.RTeilstiche);
                            }
                            k++;
                            if ( k > AnfRiegelStatus.RStiche )
                            {
                                break;
                            }
                        }
                        /* EasyCODE ) */
                    }
                    /* EasyCODE ) */
                    /* EasyCODE ( 0 
                    VorwÑrtsstiche berechnen und zwischenspeichern */
                    SticheVProStichR = AnfRiegelStatus.VTeilstiche/AnfRiegelStatus.RTeilstiche;
                    RiegelsticheV[0] = RiegelsticheR[0];
                    for ( i=0, k=0; i<AnfRiegelStatus.RStiche; i++ )
                    {
                        Vektor.x  = (RiegelsticheR[i+1].x - RiegelsticheR[i].x)/SticheVProStichR;
                        Vektor.y  = (RiegelsticheR[i+1].y - RiegelsticheR[i].y)/SticheVProStichR;
                        /* EasyCODE ( 0 
                        Teilstiche berechnen */
                        for ( j=1; j<=(int)(SticheVProStichR+0.5); j++ )
                        {
                            if ( j == (int)(SticheVProStichR+0.5) )
                            {
                                RiegelsticheV [k+1] = RiegelsticheR [i+1];
                            }
                            else
                            {
                                RiegelsticheV[k+1].x = RiegelsticheV[k].x + Vektor.x;
                                RiegelsticheV[k+1].y = RiegelsticheV[k].y + Vektor.y;
                            }
                            k++;
                        }
                        /* EasyCODE ) */
                    }
                    /* EasyCODE ) */
                    /* EasyCODE ( 0 
                    ev Eilgang, AnnÑhen und VorwÑrtsstiche ausgeben */
                    if ( SticheVProStichR == 1 )
                    {
                        if ( (i=AnfRiegelStatus.RStiche-AnfRiegelStatus.VStiche) > 0 )
                        {
                            if ( RiegelsticheR[i].x != 99999 )
                            {
                                /* EasyCODE ( 0 
                                Eilgang einfÅgen */
                                if ( maschadr + 5 < maxmem )
                                {
                                    wrword (maschadr, 0x20FF);
                                    maschadr = maschadr + 2;
                                    
                                    eilinc.x = round(RiegelsticheR[i].x - letztereinstich.x);
                                    eilinc.y = round(RiegelsticheR[i].y - letztereinstich.y);
                                    while ( (abs(eilinc.x) > MAXINCR) || (abs(eilinc.y) > MAXINCR) && (fehler == 0) )
                                    {
                                        if ( abs(eilinc.x) > MAXINCR )
                                        {
                                            if ( eilinc.x > 0 )
                                            {
                                                wrbyte (maschadr, MAXINCR);
                                                eilinc.x = eilinc.x - MAXINCR;
                                            }
                                            else
                                            {
                                                wrbyte(maschadr, MAXINCR + 128);
                                                eilinc.x = eilinc.x + MAXINCR;
                                            }
                                        }
                                        else
                                        {
                                            wrbyte(maschadr, 0);
                                        }
                                        if ( abs(eilinc.y) > MAXINCR )
                                        {
                                            if ( eilinc.y > 0 )
                                            {
                                                wrbyte(maschadr + 1, MAXINCR);
                                                eilinc.y = eilinc.y - MAXINCR;
                                            }
                                            else
                                            {
                                                wrbyte(maschadr + 1, MAXINCR + 128);
                                                eilinc.y = eilinc.y + MAXINCR;
                                            }
                                        }
                                        else
                                        {
                                            wrbyte(maschadr + 1, 0);
                                        }
                                        maschadr = maschadr + 2;
                                        if ( maschadr + 3 >= maxmem )
                                        {
                                            fehler = 0xFB;
                                        }
                                    }
                                    if ( fehler == 0 )
                                    {
                                        wrbyte(maschadr, vorzbetr(eilinc.x));
                                        wrbyte(maschadr + 1, vorzbetr(eilinc.y));
                                        wrbyte(maschadr + 2, 0xFF);
                                    }
                                    letztereinstich.x = RiegelsticheR[i].x ;
                                    letztereinstich.y = RiegelsticheR[i].y;
                                    letztekoord = letztereinstich;
                                    maschadr = maschadr + 3;
                                }
                                else
                                {
                                    fehler = 0xFC;
                                }
                                /* EasyCODE ) */
                            }
                        }
                        // AnnÑhen einfÅgen
                        maschfkt.laenge = 3;
                        maschfkt.befehl = 1;
                        maschfkt.verschieb = 0;
                        mfktausgeben (maschfkt);
                        stichgemacht = TRUE;
                        while ( i < AnfRiegelStatus.RStiche )
                        {
                            stichout (RiegelsticheR[i+1], &letztereinstich);
                            i++;
                        }
                    }
                    else
                    {
                        // SticheVProStich R > 1
                        // SticheVProStichR < 1 durch Eingabe im OSCA verhindert
                        i = (AnfRiegelStatus.RStiche*SticheVProStichR) - AnfRiegelStatus.VStiche;
                        if ( i > 0 )
                        {
                            if ( RiegelsticheV[i].x != 99999 )
                            {
                                /* EasyCODE ( 0 
                                Eilgang einfÅgen */
                                if ( maschadr + 5 < maxmem )
                                {
                                    wrword (maschadr, 0x20FF);
                                    maschadr = maschadr + 2;
                                    
                                    eilinc.x = round(RiegelsticheV[i].x - letztereinstich.x);
                                    eilinc.y = round(RiegelsticheV[i].y - letztereinstich.y);
                                    while ( (abs(eilinc.x) > MAXINCR) || (abs(eilinc.y) > MAXINCR) && (fehler == 0) )
                                    {
                                        if ( abs(eilinc.x) > MAXINCR )
                                        {
                                            if ( eilinc.x > 0 )
                                            {
                                                wrbyte (maschadr, MAXINCR);
                                                eilinc.x = eilinc.x - MAXINCR;
                                            }
                                            else
                                            {
                                                wrbyte(maschadr, MAXINCR + 128);
                                                eilinc.x = eilinc.x + MAXINCR;
                                            }
                                        }
                                        else
                                        {
                                            wrbyte(maschadr, 0);
                                        }
                                        if ( abs(eilinc.y) > MAXINCR )
                                        {
                                            if ( eilinc.y > 0 )
                                            {
                                                wrbyte(maschadr + 1, MAXINCR);
                                                eilinc.y = eilinc.y - MAXINCR;
                                            }
                                            else
                                            {
                                                wrbyte(maschadr + 1, MAXINCR + 128);
                                                eilinc.y = eilinc.y + MAXINCR;
                                            }
                                        }
                                        else
                                        {
                                            wrbyte(maschadr + 1, 0);
                                        }
                                        maschadr = maschadr + 2;
                                        if ( maschadr + 3 >= maxmem )
                                        {
                                            fehler = 0xFB;
                                        }
                                    }
                                    if ( fehler == 0 )
                                    {
                                        wrbyte(maschadr, vorzbetr(eilinc.x));
                                        wrbyte(maschadr + 1, vorzbetr(eilinc.y));
                                        wrbyte(maschadr + 2, 0xFF);
                                    }
                                    letztereinstich.x = RiegelsticheV[i].x ;
                                    letztereinstich.y = RiegelsticheV[i].y;
                                    letztekoord = letztereinstich;
                                    maschadr = maschadr + 3;
                                }
                                else
                                {
                                    fehler = 0xFC;
                                }
                                /* EasyCODE ) */
                            }
                        }
                        // AnnÑhen einfÅgen
                        maschfkt.laenge = 3;
                        maschfkt.befehl = 1;
                        maschfkt.verschieb = 0;
                        mfktausgeben (maschfkt);
                        stichgemacht = TRUE;
                        while ( i < (AnfRiegelStatus.RStiche*SticheVProStichR) )
                        {
                            stichout (RiegelsticheV[i+1], &letztereinstich);
                            i++;
                        }
                    }
                    /* EasyCODE ) */
                    /* EasyCODE ( 0 
                    RÅckwÑrtsstiche ausgeben */
                    for ( i=AnfRiegelStatus.RStiche; i > 0; i -- )
                    {
                        if ( RiegelsticheR[i-1].x != 99999 )
                        {
                            stichout (RiegelsticheR[i-1], &letztereinstich);
                        }
                    }
                    /* EasyCODE ) */
                    /* EasyCODE ( 0 
                    VorwÑrtsstiche ausgeben */
                    for ( i= 1; i<AnfRiegelStatus.AktStichnr ; i ++ )
                    {
                        if ( AnfRiegelStatus.Stich[i].x != 99999 )
                        {
                            stichout (AnfRiegelStatus.Stich[i], &letztereinstich);
                        }
                    }
                    /* EasyCODE ) */
                    AnfRiegelStatus.fertig = TRUE;
                    AnfRiegelStatus.AktStichnr = 0;
                }
                /* EasyCODE ) */
            }
            for ( i=0; i<MaxRiegelstichzahl; i++ )
            {
                EndRiegelKoord[i] = EndRiegelKoord[i+1];
            }
            EndRiegelKoord [MaxRiegelstichzahl] = koord;
        }
    }
    else
    {
        fehler = 0xFD;
    }
    /* EasyCODE ( 0 
    M-Funktionspuffer aktualisieren */
    if ( mfktinpuffer )
    {
        anzmfkt = 0;
        i = 1;
        while ( (fehler == 0) && (i <= MFKTPUFFERGR) )
        {
            if ( mfktpuffer[i - 1].verschieb > 1 )
            {
                anzmfkt = anzmfkt + 1;
                mfktpuffer[i - 1].verschieb = mfktpuffer[i - 1].verschieb - 1;
            }
            else
            {
                if ( mfktpuffer[i - 1].verschieb == 1 )
                {
                    mfktpuffer[i - 1].verschieb = 0;
                    mfktausgeben(mfktpuffer[i - 1]);
                }
            }
            i = i + 1;
        }
        mfktinpuffer = anzmfkt > 0;
    }
    /* EasyCODE ) */
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::stichout */
/* EasyCODE F */
// Ausgabe eines Stiches von istkoord nach zielkoord.
// Beim Verlassen des ups ist istkoord aktualisiert.

void KlasseKonvert::stichout (fkoordtyp zielkoord, fkoordtyp *istkoord)
{
    ikoordtyp maschkoord;
    float fq_sum;
    int stichl;
    if ( fehler == 0 )
    {
        /* EasyCODE < */
        /* Relativkoordinate berechnen und ausgeben */
        maschkoord.x = round(zielkoord.x - istkoord->x);
        maschkoord.y = round(zielkoord.y - istkoord->y);
        istkoord->x += (float)maschkoord.x;
        istkoord->y += (float)maschkoord.y;
        fq_sum = fsqr(maschkoord.x) + fsqr(maschkoord.y);
        stichl = round (sqrt(fq_sum));
        /* EasyCODE > */
        #if defined debug_info
            /* EasyCODE < */
            printf("\nStichpunktausgeben, koord:");
            f_koord_ausgeben (koord);
            printf(", maschkoord:");
            i_koord_ausgeben (maschkoord);
            printf(", stichl: %2d", stichl);
            /* EasyCODE > */
        #endif 
        if ( stichl <= (maxstichl + 2) )
        {
            if ( maschadr + 2 < maxmem )
            {
                stichgemacht = TRUE;
                wrbyte(maschadr, vorzbetr(maschkoord.x));
                maschadr = maschadr + 1;
                wrbyte(maschadr, vorzbetr(maschkoord.y));
                maschadr = maschadr + 1;
                anzahlstiche = anzahlstiche + 1;
            }
            else
            {
                fehler = 0xfb;
            }
        }
        else
        {
            fehler = 3;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::suchekoord */
/* EasyCODE F */
/*----------------------------------------------------------------------------
|
|  Achtung !!! die folgenden beiden  Unterprogramme werden auch von anderen
|  Modulen aus aufgerufen (fuer Nahtbildkorrektur). Sie duerfen nicht
|  geaendert werden
|
----------------------------------------------------------------------------*/

void KlasseKonvert::suchekoord (offs_adr puffer, ikoordtyp * punkt)
{
    abschntype      abschnitt;
    do{
        puffer = vorg(puffer);
        rdabsch(&abschnitt, puffer);
    }while ( !(is_koordabschn(abschnitt.abschnittart)) );
    if ( abschnitt.abschnittart == mnullpkt )
    {
        punkt->x = 0;
        punkt->y = 0;
    }
    else
    {
        *punkt = abschnitt._VAR.skoordabsch.koord;
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::verschiebe_abschnitt */
/* EasyCODE F */
void KlasseKonvert::verschiebe_abschnitt (offs_adr puffer, ikoordtyp punkt)

/* Die Endkoordinaten des durch Puffer adressierten Abschnittes (gilt auch
   fuer M-Funktionen) sowie ggf. der vorhergehenden und nachfolgenden
   Abschnitte (z.B. bei Kreisen oder Funktionen) werden auf Punkt verschoben.

   Achtung: Zeigt puffer auf den Maschinennullpunkt, so wird die
            Funkition ohne Aktion beendet. (frÅher Fehler 2)           */
{
    offs_adr     savedpuffer;
    abschntype   abschnitt,
                 savedabschnitt;
    
    savedpuffer = puffer;
    
    /* Ein Verschieben des Punktes ist nicht moeglich , wenn der aktuelle Punkt
     * ein Kreisendpunkt ist, der im Maschinennullpunkt liegt. Dies wird in der
     * Folgenden Schleife getestet. */
    
    rdabsch(&abschnitt, puffer);
    while ( ! ( is_koordabschn (abschnitt.abschnittart)
            && (abschnitt.abschnittart != kreis)
            && (abschnitt.abschnittart != mnullpkt)) )
    {
        /* EasyCODE < */
        /* Schleifenbedingung erfuellt fuer Kreisendpunkte und M-Funktionen */
        /* EasyCODE > */
        if ( (abschnitt.abschnittart == kreis) )
        {
            puffer = vorg(vorg(puffer));
        }
        puffer = vorg(puffer);
        rdabsch(&abschnitt, puffer);
    }
    /* vorhergehende Kreisendpunkte verschieben */
    puffer = savedpuffer;
    rdabsch(&abschnitt, puffer);
    while ( ! ( is_koordabschn (abschnitt.abschnittart)
            && (abschnitt.abschnittart != kreis)
            && (abschnitt.abschnittart != mnullpkt)) )
    {
        /* EasyCODE < */
        /* Schleifenbedingung erfuellt fuer Kreisendpunkte und M-Funktionen */
        /* EasyCODE > */
        if ( (abschnitt.abschnittart == kreis) )
        {
            abschnitt._VAR.skoordabsch.koord = punkt;
            wrabsch(&abschnitt, puffer);
            puffer = vorg(vorg(puffer));
        }
        puffer = vorg(puffer);
        rdabsch(&abschnitt, puffer);
    }
    /* EasyCODE < */
    /* bei Kreisendpunkt vorhergehenden Abschnitt <> Kreisendpunkt,
     * andernfalls Abschnitt selbst verschieben */
    /* EasyCODE > */
    if ( is_koordabschn (abschnitt.abschnittart) )
    {
        abschnitt._VAR.skoordabsch.koord = punkt;
        wrabsch(&abschnitt, puffer);
    }
    /* Nachfolgende KreisEndpunkte verschieben */
    puffer = savedpuffer;
    rdabsch(&abschnitt, puffer);
    savedabschnitt = abschnitt;
    puffer = nachf(puffer);
    rdabsch(&abschnitt, puffer);
    while ( (!(is_koordabschn(abschnitt.abschnittart)) || (abschnitt.abschnittart == krsstuetzpkt) && (savedabschnitt.abschnittart != krsstuetzpkt))
            && (abschnitt.abschnittart != programmende) )
    {
        /* EasyCODE < */
        /* Bedingung erfuellt fuer MFunktionen und 1.Kreisstuetzpunkt */
        /* EasyCODE > */
        if ( (abschnitt.abschnittart == krsstuetzpkt) )
        {
            /* Abschnitt ist erster Kreisstuetzpunkt */
            puffer = nachf(nachf(puffer));
            rdabsch (&abschnitt, puffer);
            abschnitt._VAR.skoordabsch.koord = punkt;
            savedabschnitt = abschnitt;
            wrabsch (&abschnitt, puffer);
        }
        savedabschnitt = abschnitt;
        puffer = nachf(puffer);
        rdabsch (&abschnitt, puffer);
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::bearbeitefunktion */
/* EasyCODE F */
// Bearbeitung von Maschinenfunktionen

void KlasseKonvert::bearbeitefunktion (abschntype abschnitt)
{
    mfunkttyp       maschfkt;
    int i, j, k ,l, u, Index;
    koordinate RiegelsticheV [37], RiegelsticheR [37];
    koordinate Vektor;
    float SticheVProStichR;
    
    //  ausgangtyp      ausgang;
    
      maschfkt.laenge = 0;
      maschfkt.verschieb = 0;
    switch ( abschnitt.abschnittart )
    {
    case annaehen :
        /* EasyCODE ( 0 
        AnnÑhen */
        if ( lpprogramm )
        {
            fehler = 17;
        }
        else
        {
            if ( maschinenaeht )
            {
                fehler = 27;
            }
            else
            {
                for ( i=0; i<=MaxRiegelstichzahl; i++ )
                {
                    EndRiegelKoord [i].x = 99999;
                    EndRiegelKoord [i].y = 99999;
                }
                //maschfkt.laenge = 3;
                //maschfkt.befehl = 1;
                maschinenaeht = TRUE;
                fadengeschnitten = FALSE;
                stichgemacht = FALSE;
                if ( (abschnitt._VAR.sriegel.VorwStiche != 0) || (abschnitt._VAR.sriegel.RueckwStiche != 0) )
                {
                    AnfRiegelStatus.fertig = FALSE;
                    AnfRiegelStatus.AktStichnr = 0;
                    AnfRiegelStatus.RStiche = abschnitt._VAR.sriegel.RueckwStiche;
                    AnfRiegelStatus.RTeilstiche = abschnitt._VAR.sriegel.RueckwTeilstiche;
                    AnfRiegelStatus.VStiche = abschnitt._VAR.sriegel.VorwStiche ;
                    AnfRiegelStatus.VTeilstiche = abschnitt._VAR.sriegel.VorwTeilstiche ;
                }
                return ;
            }
        }
        break;
        /* EasyCODE ) */
    case schneiden :
        /* EasyCODE ( 0 
        Fadenschneiden */
        if ( (abschnitt._VAR.sriegel.VorwStiche != 0) || (abschnitt._VAR.sriegel.RueckwStiche != 0) )
        {
            abschnitt.abschnittart = endriegel;
            /* EasyCODE ( 0 
            Endriegel */
            if ( !AnfRiegelStatus.fertig )
            {
                fehler = 31;
            }
            else
            {
                if ( fehler == 0 )
                {
                    if ( maschinenaeht )
                    {
                        for ( i=0; i<37; i++ )
                        {
                            RiegelsticheV [i].x  = 99999;
                            RiegelsticheV [i].y  = 99999;
                            RiegelsticheR [i].x  = 99999;
                            RiegelsticheR [i].y  = 99999;
                        }
                        /* EasyCODE ( 0 
                        Riegelstiche rÅckwÑrts berechnen */
                        k = 1;
                        RiegelsticheR [0] = EndRiegelKoord [MaxRiegelstichzahl];
                        for ( i=0;
                               i<=(((float)abschnitt._VAR.sriegel.RueckwStiche/abschnitt._VAR.sriegel.RueckwTeilstiche) + 0.9); i++ )
                        {
                            Index = MaxRiegelstichzahl - i;
                            Vektor.x = EndRiegelKoord [Index].x   - EndRiegelKoord [Index-1].x;
                            Vektor.y = EndRiegelKoord [Index].y   - EndRiegelKoord [Index-1].y;
                            /* EasyCODE ( 0 
                            Teilstiche berechnen */
                            for ( j=1;
                                   (j<=abschnitt._VAR.sriegel.RueckwTeilstiche) && (k<=abschnitt._VAR.sriegel.RueckwStiche); j++,k++ )
                            {
                                if ( j == abschnitt._VAR.sriegel.RueckwTeilstiche )
                                {
                                    RiegelsticheR [k] = EndRiegelKoord [Index-1];
                                }
                                else
                                {
                                    RiegelsticheR[k].x = EndRiegelKoord[Index].x - ((Vektor.x*j)/abschnitt._VAR.sriegel.RueckwTeilstiche);
                                    RiegelsticheR[k].y = EndRiegelKoord[Index].y - ((Vektor.y*j)/abschnitt._VAR.sriegel.RueckwTeilstiche);
                                }
                            }
                            /* EasyCODE ) */
                            if ( k > abschnitt._VAR.sriegel.RueckwStiche )
                            {
                                break;
                            }
                        }
                        /* EasyCODE ) */
                        /* EasyCODE ( 0 
                        Riegelstiche vorwÑrts berechnen */
                        k--;
                        SticheVProStichR = abschnitt._VAR.sriegel.VorwTeilstiche/abschnitt._VAR.sriegel.RueckwTeilstiche;
                        RiegelsticheV[0] = RiegelsticheR[k];
                        for ( i=1, l= 1; i<=abschnitt._VAR.sriegel.RueckwStiche; i++ )
                        {
                            Vektor.x = (RiegelsticheR[k-i].x - RiegelsticheR[k-i+1].x)/SticheVProStichR;
                            Vektor.y = (RiegelsticheR[k-i].y - RiegelsticheR[k-i+1].y)/SticheVProStichR;
                            for ( j=1; j<= (u = SticheVProStichR); j++, l++ )
                            {
                                if ( j == u )
                                {
                                    RiegelsticheV[l] = RiegelsticheR[k-i];
                                }
                                else
                                {
                                    RiegelsticheV[l].x = RiegelsticheV[l-1].x + Vektor.x;
                                    RiegelsticheV[l].y = RiegelsticheV[l-1].y + Vektor.y;
                                }
                            }
                        }
                        /* EasyCODE ) */
                        /* EasyCODE ( 0 
                        Stiche ausgeben */
                        for ( i=1; i<=abschnitt._VAR.sriegel.RueckwStiche; i++ )
                        {
                            if ( (RiegelsticheR[i].x !=99999) &&
                                 ((RiegelsticheR[i].x != letztereinstich.x) ||
                                 (RiegelsticheR[i].y != letztereinstich.y)) )
                            {
                                stichout (RiegelsticheR[i], &letztereinstich);
                            }
                        }
                        for ( i=1; i<=abschnitt._VAR.sriegel.VorwStiche; i++ )
                        {
                            if ( (RiegelsticheV[i].x != 99999) &&
                                 ((RiegelsticheV[i].x  != letztereinstich.x) ||
                                 (RiegelsticheV[i].y != letztereinstich.y)) )
                            {
                                stichout (RiegelsticheV[i], &letztereinstich);
                            }
                        }
                        letztekoord = letztereinstich;
                        /* EasyCODE ) */
                    }
                    else
                    {
                        fehler = 33;
                    }
                }
            }
            /* EasyCODE ) */
            if ( fehler != 0 )
            {
                return ;
            }
        }
        if ( !AnfRiegelStatus.fertig )
        {
            fehler = 31;
        }
        else
        {
            if ( fadengeschnitten )
            {
                fehler = 26;
            }
            else
            {
                if ( maschinenaeht )
                {
                    if ( stichgemacht )
                    {
                        if ( riegelende && !riegelanfang )
                        {
                            riegelanfang = TRUE;
                            stichpunktausgeben (letztekoord);
                        }
                        fadengeschnitten = TRUE;
                        stichgemacht = FALSE;
                        maschinenaeht = FALSE;
                        maschfkt.laenge = 3;
                        maschfkt.befehl = 2;
                    }
                    else
                    {
                        fehler = 30;
                    }
                }
                else
                {
                    fehler = 28;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case stop :
        /* EasyCODE ( 0 
        Stop */
        maschfkt.laenge = 3;
        maschfkt.befehl = 3;
        maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        if ( ausg_liklammer )
        {
            ausg_liklammer = 0;
        }
        if ( ausg_reklammer )
        {
            ausg_reklammer = 0;
        }
        break;
        /* EasyCODE ) */
    case ufwstop :
        /* EasyCODE ( 0 
        Stop bei Unterfadenstoerung */
        maschfkt.laenge = 3;
        maschfkt.befehl = 9;
        maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        if ( ausg_liklammer )
        {
            ausg_liklammer = 0;
        }
        if ( ausg_reklammer )
        {
            ausg_reklammer = 0;
        }
        break;
        /* EasyCODE ) */
    case naehenaus :
        /* EasyCODE ( 0 
        NÑhen aus */
        if ( maschinenaeht )
        {
            maschinenaeht = FALSE;
            maschfkt.laenge = 3;
            maschfkt.befehl = 4;
            maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        }
        else
        {
            fehler = 29;
        }
        break;
        /* EasyCODE ) */
    case reddrehzahlein :
        /* EasyCODE ( 0 
        reduzierte Drehzahl ein */
        /* EasyCODE :
         */
        {
            maschfkt.laenge = 3;
            maschfkt.befehl = 7;
            maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        }

        break;
        /* EasyCODE ) */
    case reddrehzahlaus :
        /* EasyCODE ( 0 
        reduzierte Drehzahl aus */
        /* EasyCODE :
         */
        {
            maschfkt.laenge = 3;
            maschfkt.befehl = 8;
            maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        }

        break;
        /* EasyCODE ) */
    case pattenanf :
        /* EasyCODE ( 0 
        Pattenstop */
        /* EasyCODE :
         */
        {
            /* Aktuelle Koordinate vor Pattenanfangspunkt merken */
            stapelkoord.x = letztekoord.x;
            stapelkoord.y = letztekoord.y;
            
            /* Pattenanfangspunkt (rel. zu Maschinennullpunkt) berechnen */
            /*letztekoord.x = abschnitt._VAR.skoordabsch.koord.x;*/
            /*letztekoord.y = abschnitt._VAR.skoordabsch.koord.y;*/
            
            pattenanfkoord.x = letztekoord.x;
            pattenanfkoord.y = letztekoord.y;
            
            letztereinstich = letztekoord;
            
            progmitpatten = TRUE;
            
            maschfkt.laenge = 3;
            maschfkt.befehl = 9;
        }

        break;
        /* EasyCODE ) */
    case lpprogende :
        /* EasyCODE ( 0 
        LP-Progende */
        /* EasyCODE :
         */
        {
            maschfkt.laenge = 3;
            maschfkt.befehl = 0x0E;
            lpprogramm = FALSE;
            lpmaschadr = maschadr;
            lpkoord.x = round(letztereinstich.x);
            lpkoord.y = round(letztereinstich.y);
            if ( ausg_liklammer )
            {
                ausg_liklammer = 0;
            }
            if ( ausg_reklammer )
            {
                ausg_reklammer = 0;
            }
        }

        break;
        /* EasyCODE ) */
    case programmende :
        /* EasyCODE ( 0 
        Programmende */
        /* EasyCODE :
         */
        {
            if ( lpprogramm )
            {
                fehler = 23;
            }
            else
            {
                if ( !fadengeschnitten )
                {
                    fehler = 4;
                }
            }
            /* EasyCODE < */
            // Stapelpunkt ist immer der letzte Punkt des Bildes
            // W. Halfmann 28.7.97)
            //    if (! progmitpatten)
            //    {
                    stapelkoord.x = letztekoord.x;
                    stapelkoord.y = letztekoord.y;
            //    }
            
                  maschfkt.laenge = 3;
                  maschfkt.befehl = 0x0F;
            /* EasyCODE > */
        }

        break;
        /* EasyCODE ) */
    case absdrehz :
        /* EasyCODE ( 0 
        Drehzahl */
        /* EasyCODE :
         */
        {
            maschfkt.laenge = 4;
            maschfkt.befehl = 0x10;
            if ( abschnitt._VAR.sabsdrehz.sonst > maxdrehzahl )
            {
                abschnitt._VAR.sabsdrehz.sonst = maxdrehzahl;
            }
            if ( abschnitt._VAR.sabsdrehz.sonst < mindrehzahl )
            {
                abschnitt._VAR.sabsdrehz.sonst = mindrehzahl;
            }
            maschfkt.operand = round(abschnitt._VAR.sabsdrehz.sonst / (float)(100));
            maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        }

        break;
        /* EasyCODE ) */
    case outset :
        /* EasyCODE ( 0 
        Ausgang setzen */
        /* EasyCODE :
         */
        {
            if ( ! ausgang_gesetzt[abschnitt._VAR.sabsdrehz.sonst] )
            {
                maschfkt.laenge = 4;
                maschfkt.befehl = 0x11;
                maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
                maschfkt.operand = abschnitt._VAR.sabsdrehz.sonst;
                ausgang_gesetzt[abschnitt._VAR.sabsdrehz.sonst] = 1;
                if ( abschnitt._VAR.sabsdrehz.sonst == 5 )
                {
                    ausg_niveau = 1;
                }
            }
        }

        break;
        /* EasyCODE ) */
    case outres :
        /* EasyCODE ( 0 
        Ausgang rÅcksetzen */
        /* EasyCODE :
         */
        {
            if ( ausgang_gesetzt[abschnitt._VAR.sabsdrehz.sonst] )
            {
                maschfkt.laenge = 4;
                maschfkt.befehl = 0x12;
                maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
                maschfkt.operand = abschnitt._VAR.sabsdrehz.sonst;
                ausgang_gesetzt[abschnitt._VAR.sabsdrehz.sonst] = 0;
                if ( abschnitt._VAR.sabsdrehz.sonst == 5 )
                {
                    ausg_niveau = 0;
                }
            }
        }

        break;
        /* EasyCODE ) */
    case niveaufuss :
        /* EasyCODE ( 0 
        Niveau Huepferfuss (Wert) */
        maschfkt.laenge = 4;
        maschfkt.befehl = 0x18;
        maschfkt.operand = abschnitt._VAR.sabsdrehz.sonst;
        maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        break;
        /* EasyCODE ) */
    case hubfuss :
        /* EasyCODE ( 0 
        Huepferfuss-Hub (Wert) */
        maschfkt.laenge = 4;
        maschfkt.befehl = 0x19;
        maschfkt.operand = abschnitt._VAR.sabsdrehz.sonst;
        maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        break;
        /* EasyCODE ) */
    case fussniveaulevel :
        /* EasyCODE ( 0 
        Niveau Huepferfuss (Level) */
        maschfkt.laenge = 4;
        maschfkt.befehl = 0x1c;
        maschfkt.operand = abschnitt._VAR.sabsdrehz.sonst;
        maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        break;
        /* EasyCODE ) */
    case fusshublevel :
        /* EasyCODE ( 0 
        Huepferfuss-Hub (Level) */
        maschfkt.laenge = 4;
        maschfkt.befehl = 0x1d;
        maschfkt.operand = abschnitt._VAR.sabsdrehz.sonst;
        maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        break;
        /* EasyCODE ) */
    case stlochabsein :
        /* EasyCODE ( 0 
        Stichlochabsenkung ein */
        /* EasyCODE :
         */
        {
            if ( !ausg_lochabs )
            {
                maschfkt.laenge = 4;
                maschfkt.befehl = 0x12;
                maschfkt.operand = 6;
                maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
                ausg_lochabs = 1;
            }
        }

        break;
        /* EasyCODE ) */
    case stlochabsaus :
        /* EasyCODE ( 0 
        Stichlochabsenkung aus */
        /* EasyCODE :
         */
        {
            if ( ausg_lochabs )
            {
                maschfkt.laenge = 4;
                maschfkt.befehl = 0x11;
                maschfkt.operand = 6;
                maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
                ausg_lochabs = 0;
            }
        }

        break;
        /* EasyCODE ) */
    case liklammerauf :
        /* EasyCODE ( 0 
        linke Klammer auf */
        /* EasyCODE :
         */
        {
            if ( !ausg_liklammer )
            {
                maschfkt.laenge = 4;
                maschfkt.befehl = 0x11;
                maschfkt.operand = 8;
                maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
                ausg_liklammer = 1;
            }
        }

        break;
        /* EasyCODE ) */
    case liklammerab :
        /* EasyCODE ( 0 
        linke Klammer ab */
        /* EasyCODE :
         */
        {
            if ( ausg_liklammer )
            {
                maschfkt.laenge = 4;
                maschfkt.befehl = 0x12;
                maschfkt.operand = 8;
                maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
                ausg_liklammer = 0;
            }
        }

        break;
        /* EasyCODE ) */
    case reklammerauf :
        /* EasyCODE ( 0 
        rechte Klammer auf */
        /* EasyCODE :
         */
        {
            if ( !ausg_reklammer )
            {
                maschfkt.laenge = 4;
                maschfkt.befehl = 0x11;
                maschfkt.operand = 9;
                maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
                ausg_reklammer = 1;
            }
        }

        break;
        /* EasyCODE ) */
    case reklammerab :
        /* EasyCODE ( 0 
        rechte Klammer ab */
        /* EasyCODE :
         */
        {
            if ( ausg_reklammer )
            {
                maschfkt.laenge = 4;
                maschfkt.befehl = 0x12;
                maschfkt.operand = 9;
                maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
                ausg_reklammer = 0;
            }
        }

        break;
        /* EasyCODE ) */
    case zuspannein :
        /* EasyCODE ( 0 
        Zusatzspannung ein */
        /* EasyCODE :
         */
        {
            if ( !ausg_zuspann )
            {
                maschfkt.laenge = 4;
                maschfkt.befehl = 0x13;
                maschfkt.operand = 1;
                maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
                ausg_zuspann = 1;
            }
        }

        break;
        /* EasyCODE ) */
    case zuspannaus :
        /* EasyCODE ( 0 
        Zusatzspannung aus */
        /* EasyCODE :
         */
        {
            if ( ausg_zuspann )
            {
                maschfkt.laenge = 4;
                maschfkt.befehl = 0x14;
                maschfkt.operand = 1;
                maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
                ausg_zuspann = 0;
            }
        }

        break;
        /* EasyCODE ) */
    case wartenh :
        /* EasyCODE ( 0 
        Warten bis Eingang = high */
        /* EasyCODE :
         */
        {
            maschfkt.laenge = 4;
            maschfkt.befehl = 0x15;
            maschfkt.operand = abschnitt._VAR.sabsdrehz.sonst;
            maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        }

        break;
        /* EasyCODE ) */
    case wartenl :
        /* EasyCODE ( 0 
        Warten bis Eingang = low */
        /* EasyCODE ( 0 */
        /* EasyCODE :
         */
        {
            maschfkt.laenge = 4;
            maschfkt.befehl = 0x16;
            maschfkt.operand = abschnitt._VAR.sabsdrehz.sonst;
            maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        }

        break;
        /* EasyCODE ) */
        /* EasyCODE ) */
    case warten :
        /* EasyCODE ( 0 
        Warten auf Zeit */
        /* EasyCODE :
         */
        {
            maschfkt.laenge = 4;
            maschfkt.befehl = 0x17;
            maschfkt.operand = abschnitt._VAR.sabsdrehz.sonst;
            maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        }

        break;
        /* EasyCODE ) */
    case stichbreitenaend :
        /* EasyCODE ( 0 
        Stichbreite */
        /* EasyCODE :
         */
        {
            if ( lpprogramm )
            {
                fehler = 24;
            }
            else
            {
                if ( (abschnitt._VAR.sstichbreitenaend.stichr == mittigli)
                     || (abschnitt._VAR.sstichbreitenaend.stichr == mittigre)
                     || (abschnitt._VAR.sstichbreitenaend.stichr == livongrundl)
                     || (abschnitt._VAR.sstichbreitenaend.stichr == revongrundl) )
                {
                    if ( (abschnitt._VAR.sstichbreitenaend.stichbreite == 0)
                         || (abschnitt._VAR.sstichbreitenaend.stichr == revongrundl)
                         || (abschnitt._VAR.sstichbreitenaend.stichr == livongrundl) )
                    {
                        if ( riegelende && !riegelanfang )
                        {
                            stichpunktausgeben (letztekoord);
                        }
                        riegelende = FALSE;
                        riegelanfang = FALSE;
                    }
                    if ( (abschnitt._VAR.sstichbreitenaend.stichbreite != stichbr) || (abschnitt._VAR.sstichbreitenaend.stichr != stichlage) )
                    {
                        if ( (stichbr == 0)
                             || ((stichlage == revongrundl) || (stichlage == livongrundl))
                             && (abschnitt._VAR.sstichbreitenaend.stichbreite != 0)
                             && ( (abschnitt._VAR.sstichbreitenaend.stichr == mittigre)
                                  || (abschnitt._VAR.sstichbreitenaend.stichr == mittigli)) )
                        {
                            riegelende = TRUE;
                            riegelanfang = TRUE;
                        }
                        stichbr = abschnitt._VAR.sstichbreitenaend.stichbreite;
                        stichlage = abschnitt._VAR.sstichbreitenaend.stichr;
                    }
                    stichrichtungli = ( (abschnitt._VAR.sstichbreitenaend.stichr == mittigli)
                                        || (abschnitt._VAR.sstichbreitenaend.stichr == livongrundl) );
                }
                else
                {
                    fehler = 25;
                }
            }
        }

        break;
        /* EasyCODE ) */
    case standardstichl :
        /* EasyCODE ( 0 
        StandardstichlÑnge */
        /* EasyCODE :
         */
        {
            stini = TRUE;
            stichlausdatens = i_stdstichl / (float) (10);
            abschnitt._VAR.sstichlaengenaend.stichlaenge = i_stdstichl;
            wrabsch(&abschnitt, vorg(abschadr));
        }

        break;
        /* EasyCODE ) */
    case stichlaengenaend :
        /* EasyCODE ( 0 
        StichlÑnge */
        stini = TRUE;
        stichlausdatens = abschnitt._VAR.sstichlaengenaend.stichlaenge / (float) (10);
        if ( !(abschnitt._VAR.sstichlaengenaend.stichlaenge > 0) && (abschnitt._VAR.sstichlaengenaend.stichlaenge <= (maxstichl * 10)) )
        {
            fehler = 5;
        }
        break;
        /* EasyCODE ) */
    case anfriegel :
        /* EasyCODE ( 0 
        Anfangsriegel */
        if ( maschinenaeht )
        {
            AnfRiegelStatus.fertig = FALSE;
            AnfRiegelStatus.AktStichnr = 0;
            AnfRiegelStatus.RStiche = abschnitt._VAR.sriegel.RueckwStiche;
            AnfRiegelStatus.RTeilstiche = abschnitt._VAR.sriegel.RueckwTeilstiche;
            AnfRiegelStatus.VStiche = abschnitt._VAR.sriegel.VorwStiche ;
            AnfRiegelStatus.VTeilstiche = abschnitt._VAR.sriegel.VorwTeilstiche ;
        }
        else
        {
            fehler = 32;
        }
        return ;
        /* EasyCODE ) */
    case endriegel :
        /* EasyCODE ( 0 
        Endriegel */
        if ( !AnfRiegelStatus.fertig )
        {
            fehler = 31;
        }
        else
        {
            if ( fehler == 0 )
            {
                if ( maschinenaeht )
                {
                    for ( i=0; i<37; i++ )
                    {
                        RiegelsticheV [i].x  = 99999;
                        RiegelsticheV [i].y  = 99999;
                        RiegelsticheR [i].x  = 99999;
                        RiegelsticheR [i].y  = 99999;
                    }
                    /* EasyCODE ( 0 
                    Riegelstiche rÅckwÑrts berechnen */
                    k = 1;
                    RiegelsticheR [0] = EndRiegelKoord [MaxRiegelstichzahl];
                    for ( i=0;
                           i<=(((float)abschnitt._VAR.sriegel.RueckwStiche/abschnitt._VAR.sriegel.RueckwTeilstiche) + 0.9); i++ )
                    {
                        Index = MaxRiegelstichzahl - i;
                        Vektor.x = EndRiegelKoord [Index].x   - EndRiegelKoord [Index-1].x;
                        Vektor.y = EndRiegelKoord [Index].y   - EndRiegelKoord [Index-1].y;
                        /* EasyCODE ( 0 
                        Teilstiche berechnen */
                        for ( j=1;
                               (j<=abschnitt._VAR.sriegel.RueckwTeilstiche) && (k<=abschnitt._VAR.sriegel.RueckwStiche); j++,k++ )
                        {
                            if ( j == abschnitt._VAR.sriegel.RueckwTeilstiche )
                            {
                                RiegelsticheR [k] = EndRiegelKoord [Index-1];
                            }
                            else
                            {
                                RiegelsticheR[k].x = EndRiegelKoord[Index].x - ((Vektor.x*j)/abschnitt._VAR.sriegel.RueckwTeilstiche);
                                RiegelsticheR[k].y = EndRiegelKoord[Index].y - ((Vektor.y*j)/abschnitt._VAR.sriegel.RueckwTeilstiche);
                            }
                        }
                        /* EasyCODE ) */
                        if ( k > abschnitt._VAR.sriegel.RueckwStiche )
                        {
                            break;
                        }
                    }
                    /* EasyCODE ) */
                    /* EasyCODE ( 0 
                    Riegelstiche vorwÑrts berechnen */
                    k--;
                    SticheVProStichR = abschnitt._VAR.sriegel.VorwTeilstiche/abschnitt._VAR.sriegel.RueckwTeilstiche;
                    RiegelsticheV[0] = RiegelsticheR[k];
                    for ( i=1, l= 1; i<=abschnitt._VAR.sriegel.RueckwStiche; i++ )
                    {
                        Vektor.x = (RiegelsticheR[k-i].x - RiegelsticheR[k-i+1].x)/SticheVProStichR;
                        Vektor.y = (RiegelsticheR[k-i].y - RiegelsticheR[k-i+1].y)/SticheVProStichR;
                        for ( j=1; j<= (u = SticheVProStichR); j++, l++ )
                        {
                            if ( j == u )
                            {
                                RiegelsticheV[l] = RiegelsticheR[k-i];
                            }
                            else
                            {
                                RiegelsticheV[l].x = RiegelsticheV[l-1].x + Vektor.x;
                                RiegelsticheV[l].y = RiegelsticheV[l-1].y + Vektor.y;
                            }
                        }
                    }
                    /* EasyCODE ) */
                    /* EasyCODE ( 0 
                    Stiche ausgeben */
                    for ( i=1; i<=abschnitt._VAR.sriegel.RueckwStiche; i++ )
                    {
                        if ( (RiegelsticheR[i].x !=99999) &&
                             ((RiegelsticheR[i].x != letztereinstich.x) ||
                             (RiegelsticheR[i].y != letztereinstich.y)) )
                        {
                            stichout (RiegelsticheR[i], &letztereinstich);
                        }
                    }
                    for ( i=1; i<=abschnitt._VAR.sriegel.VorwStiche; i++ )
                    {
                        if ( (RiegelsticheV[i].x != 99999) &&
                             ((RiegelsticheV[i].x  != letztereinstich.x) ||
                             (RiegelsticheV[i].y != letztereinstich.y)) )
                        {
                            stichout (RiegelsticheV[i], &letztereinstich);
                        }
                    }
                    letztekoord = letztereinstich;
                    /* EasyCODE ) */
                }
                else
                {
                    fehler = 33;
                }
            }
        }
        return ;
        /* EasyCODE ) */
    case setflag :
        /* EasyCODE ( 0 
        Flag setzen */
        maschfkt.laenge = 4;
        maschfkt.befehl = 0x1a;
        maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        maschfkt.operand = abschnitt._VAR.sabsdrehz.sonst;
        break;
        /* EasyCODE ) */
    case resflag :
        /* EasyCODE ( 0 
        Flag rÅcksetzen */
        maschfkt.laenge = 4;
        maschfkt.befehl = 0x1b;
        maschfkt.verschieb = abschnitt._VAR.sabsdrehz.del;
        maschfkt.operand = abschnitt._VAR.sabsdrehz.sonst;
        break;
        /* EasyCODE ) */
    default:
        fehler = 6;
    }
    mfktausgeben(maschfkt);
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::berechneueberstich */
/* EasyCODE F */
/*----------------------------------------------------------------------------
| Stichgenerierung auf Geraden
----------------------------------------------------------------------------*/
void KlasseKonvert::berechneueberstich (fkoordtyp einstichpunkt,
                         fkoordtyp * ueberstich,
                         fkoordtyp stichnachlinks,
                         boolean   * stichrichtungli,
                         stichrtyp stichlage)
{
    switch ( stichlage )
    {
    case mittigli :
        /* EasyCODE = */
    case mittigre :
        /* EasyCODE :
         */
        {
            if ( *stichrichtungli )
            {
                ueberstich->x = einstichpunkt.x + stichnachlinks.x;
                ueberstich->y = einstichpunkt.y + stichnachlinks.y;
            }
            else
            {
                ueberstich->x = einstichpunkt.x - stichnachlinks.x;
                ueberstich->y = einstichpunkt.y - stichnachlinks.y;
            }
        }

        break;
    case revongrundl :
        /* EasyCODE :
         */
        {
            if ( *stichrichtungli )
            {
                ueberstich->x = einstichpunkt.x;
                ueberstich->y = einstichpunkt.y;
            }
            else
            {
                ueberstich->x = einstichpunkt.x - 2 * stichnachlinks.x;
                ueberstich->y = einstichpunkt.y - 2 * stichnachlinks.y;
            }
        }

        break;
    case livongrundl :
        /* EasyCODE :
         */
        {
            if ( *stichrichtungli )
            {
                ueberstich->x = einstichpunkt.x + 2 * stichnachlinks.x;
                ueberstich->y = einstichpunkt.y + 2 * stichnachlinks.y;
            }
            else
            {
                ueberstich->x = einstichpunkt.x;
                ueberstich->y = einstichpunkt.y;
            }
        }

        break;
    default:
        ;
    }
    *stichrichtungli = !(*stichrichtungli);
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::bearbeitestich */
/* EasyCODE F */
void KlasseKonvert::bearbeitestich (abschntype abschnitt)
{
    /* EasyCODE < */
    fkoordtyp einstichpunkt;  /* Berechneter Stichabstand */
    /* EasyCODE > */
    if ( riegelende && (!riegelanfang) )
    {
        stichpunktausgeben (letztekoord);
        riegelanfang = TRUE;
    }
    /* EasyCODE < */
    einstichpunkt.x = abschnitt._VAR.skoordabsch.koord.x;
    einstichpunkt.y = abschnitt._VAR.skoordabsch.koord.y;
    letztekoord = einstichpunkt;      /* fuer Riegelende */
    /* EasyCODE > */
    if ( fehler == 0 )
    {
        stichpunktausgeben(einstichpunkt);
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::bearbeitegerade */
/* EasyCODE F */
void KlasseKonvert::bearbeitegerade (abschntype abschnitt)
{
    fkoordtyp einstichpunkt,
              ueberstich,
              vektor;       /* Berechneter Stichabstand */
    float laenge;
    int sticheproabschnitt,
        stichnrabschn;
    if ( (fstichl <= 0) || (fstichl > maxstichl) )
    {
        fehler = 8;
    }
    else
    {
        /* EasyCODE :
         */
        {
            if ( fehler == 0 )
            {
                laenge = sqrt(fsqr(abschnitt._VAR.skoordabsch.koord.y - letztekoord.y) + fsqr(letztekoord.x - abschnitt._VAR.skoordabsch.koord.x));
                if ( laenge != 0 )
                {
                    stichnachlinks.x = (letztekoord.y - abschnitt._VAR.skoordabsch.koord.y) * stichbr / (2 * laenge);
                    stichnachlinks.y = (abschnitt._VAR.skoordabsch.koord.x - letztekoord.x) * stichbr / (2 * laenge);
                }
                else
                {
                    if ( stichbr == 0 )
                    {
                        stichnachlinks.x = 0;
                        stichnachlinks.y = 0;
                    }
                    /* EasyCODE < */
                    /* Wenn Laenge = 0 und Stichbreite <> 0 bleibt die Variable
                     * StichNachLinks wie beim vorhergehenden Element */
                    /* EasyCODE > */
                }
                if ( stichbr == 0 )
                {
                    sticheproabschnitt = trunc(laenge / fstichl + 0.99);
                    if ( sticheproabschnitt == 0 )
                    {
                        sticheproabschnitt = 1;
                    }
                }
                else
                {
                    sticheproabschnitt = 2 * trunc(laenge / (2 * fstichl) + 0.99);
                    if ( sticheproabschnitt == 0 )
                    {
                        sticheproabschnitt = 2;
                    }
                }
                /* Vektor = Vektor zum jeweils naechsten EinstichPunkt, wenn
                 * Stichbreite = 0 */
                vektor.x = (abschnitt._VAR.skoordabsch.koord.x - letztekoord.x) / sticheproabschnitt;
                vektor.y = (abschnitt._VAR.skoordabsch.koord.y - letztekoord.y) / sticheproabschnitt;
                if ( riegelende && !fadengeschnitten   /* d.h. Stichlage mittig */ )
                {
                    einstichpunkt.x = letztekoord.x + vektor.x / 2;
                    einstichpunkt.y = letztekoord.y + vektor.y / 2;
                    berechneueberstich(einstichpunkt, &ueberstich, stichnachlinks, &stichrichtungli, stichlage);
                    stichpunktausgeben(ueberstich);
                }
                else
                {
                    /* EasyCODE < */
                    /* d.h. Stichlage links oder rechts oder Stichbreite = 0 */
                    /* EasyCODE > */
                    einstichpunkt = letztekoord;
                }
                if ( fadengeschnitten )
                {
                    stichnachlinks.x = 0;
                    stichnachlinks.y = 0;
                }
                else
                {
                    riegelanfang = FALSE;
                }
                stichnrabschn = 1;
                while ( (stichnrabschn < sticheproabschnitt) && (fehler == 0) )
                {
                    stichnrabschn = stichnrabschn + 1;
                    einstichpunkt.x = einstichpunkt.x + vektor.x;
                    einstichpunkt.y = einstichpunkt.y + vektor.y;
                    berechneueberstich(einstichpunkt, &ueberstich, stichnachlinks, &stichrichtungli, stichlage);
                    stichpunktausgeben(ueberstich);
                }
                if ( (fadengeschnitten || !riegelende) && (fehler == 0) )
                {
                    berechneueberstich(einstichpunkt, &ueberstich, stichnachlinks, &stichrichtungli, stichlage);        /* um Stichrichtung zu
                                                                                                                         * setzen */
                    einstichpunkt.x = abschnitt._VAR.skoordabsch.koord.x;
                    einstichpunkt.y = abschnitt._VAR.skoordabsch.koord.y;
                    stichpunktausgeben(einstichpunkt);
                }
                /* Im anderen Fall wird der Stich spaeter erzeugt */
                letztekoord.x = abschnitt._VAR.skoordabsch.koord.x;      /* fuer Riegelende */
                letztekoord.y = abschnitt._VAR.skoordabsch.koord.y;
            }
        }

    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::bearbeitekreis */
/* EasyCODE F */
/*----------------------------------------------------------------------------
| Stichgenerierung auf Kreis und Kreisbogen
----------------------------------------------------------------------------*/

void KlasseKonvert::bearbeitekreis (abschntype abschnitt,
                     boolean    abschnittsweise)
{
    /* EasyCODE < */
    fkoordtyp punkt1,
              punkt2,
              punkt3,
              endpunkt,
              vektor,
              mittelpunkt,
              einstichpunkt;
    ikoordtyp integpunkt;
    abschnarttype aktabschart;
    
    /* Winkel zw. pos. x-Achse und Vektor Mittelpunkt -> Einstichpunkt  */
    /* Winkeldifferenz zw. aktuellem und naechstem Einstich                   */
    float           radius,
                    segmentwinkel,
                    stichwinkel,
                    stichwinkeldiff,
                    betrstichnlinks,
                    bogenlaenge;
    offs_adr        aktadr;
    float           richtung;     /* Drehrichtung des Kreises */
    int             sticheproabschnitt,
                    stichnrabschn;
    
    /* Koordinaten der Stuetzpunkte einlesen */
    /* EasyCODE > */
    if ( (fstichl <= 0) || (fstichl > maxstichl) )
    {
        fehler = 9;
    }
    else
    {
        if ( abschnittsweise )
        {
            /* EasyCODE < */
            abschadr = vorg(abschadr);/* Adresse zeigt bereits auf den naechsten
                                       * Abschnitt */
            aktadr = abschadr;
            aktabschart = abschnitt.abschnittart;
            endpunkt.x = abschnitt._VAR.skoordabsch.koord.x;
            endpunkt.y = abschnitt._VAR.skoordabsch.koord.y;
            /* auf 1. Stuetzpunkt des Kreises bzw. Kreisbogens positionieren */
            /* Endpunkte duerfen nicht in der while-Schleife uebersprungen werden,
             * fuer den Fall dass zwei Kreise hintereinander stehen */
            /* EasyCODE > */
            if ( (abschnitt.abschnittart == kreis)
                 || (abschnitt.abschnittart == kreisbogen) )
            {
                abschadr = vorg(abschadr);
                rdabsch(&abschnitt, abschadr);
            }
            while ( (abschnitt.abschnittart == bgnstuetzpkt)
                    || (abschnitt.abschnittart == krsstuetzpkt) )
            {
                abschadr = vorg(abschadr);
                rdabsch(&abschnitt, abschadr);
            }
            /* EasyCODE < */
            /* Anfangspunkt des Kreises besetzen */
            abschadr = nachf(abschadr);       /* While-Schleife positioniert VOR
                                               * den ersten Stuetzpunkt */
            suchekoord (abschadr, &integpunkt);  /* sucht ab Vorg (AbschAdr) */
            punkt1.x = integpunkt.x;
            punkt1.y = integpunkt.y;
            readnextabsch(&abschnitt);/* Abschnitt = erster Stuetzpunkt */
            /* EasyCODE > */
        }
        else
        {
            punkt1.x = letztekoord.x;
            punkt1.y = letztekoord.y;
        }
        punkt2.x = abschnitt._VAR.skoordabsch.koord.x;
        punkt2.y = abschnitt._VAR.skoordabsch.koord.y;
        readnextabsch(&abschnitt);
        punkt3.x = abschnitt._VAR.skoordabsch.koord.x;
        punkt3.y = abschnitt._VAR.skoordabsch.koord.y;
        if ( abschnitt.abschnittart == krsstuetzpkt )
        {
            readnextabsch(&abschnitt);
            if ( !abschnittsweise )
            {
                endpunkt = punkt1;
            }
        }
        else
        {
            if ( !abschnittsweise )
            {
                endpunkt = punkt3;
            }
        }
    }
    if ( fehler == 0 )
    {
        kreisaus3punkten(punkt1, punkt2, punkt3, &radius, &mittelpunkt, &richtung, &fehler);
        if ( fehler == 0 )
        {
            if ( abschnittsweise )
            {
                switch ( aktabschart )
                {
                case kreis :
                    segmentwinkel = abschnittwinkel(punkt3, punkt1, richtung, mittelpunkt, &fehler);
                    break;
                case kreisbogen :
                    segmentwinkel = abschnittwinkel(punkt2, punkt3, richtung, mittelpunkt, &fehler);
                    break;
                case bgnstuetzpkt :
                    segmentwinkel = abschnittwinkel(punkt1, punkt2, richtung, mittelpunkt, &fehler);
                    break;
                case krsstuetzpkt :
                    /* EasyCODE :
                     */
                    {
                        rdabsch(&abschnitt, nachf(aktadr));
                        if ( abschnitt.abschnittart == kreis )
                        {
                            segmentwinkel = abschnittwinkel(punkt2, punkt3, richtung, mittelpunkt, &fehler);
                        }
                        else
                        {
                            segmentwinkel = abschnittwinkel(punkt1, punkt2, richtung, mittelpunkt, &fehler);
                        }
                    }

                    break;
                default:
                    ;
                }
                abschnitt.abschnittart = kreisbogen;
            }
            else
            {
                segmentwinkel = abschnittwinkel(punkt1, punkt3, richtung, mittelpunkt, &fehler);
            }
        }
        if ( abschnitt.abschnittart == kreis )
        {
            segmentwinkel = 2 * PI * richtung;
        }
        bogenlaenge = fabs(segmentwinkel * radius);
    }
    /* EasyCODE < */
    /* Anzahl Stiche und StichWinkel berechnen */
    /* EasyCODE > */
    if ( fehler == 0 )
    {
        if ( stichbr == 0 )
        {
            sticheproabschnitt = trunc(bogenlaenge / fstichl + 0.99);
        }
        else
        {
            sticheproabschnitt = 2 * trunc(bogenlaenge / (2 * fstichl) + 0.99);
        }
        stichwinkeldiff = segmentwinkel / sticheproabschnitt;
        if ( segmentwinkel >= 0 )
        {
            betrstichnlinks = -stichbr / 2;
        }
        else
        {
            betrstichnlinks = stichbr / 2;
        }
        if ( riegelende && !fadengeschnitten     /* d.h. Stichlage mittig */ )
        {
            stichwinkel = drehwinkel(mittelpunkt, letztekoord) + stichwinkeldiff / 2;
            berechnevektor(radius, stichwinkel, &vektor, betrstichnlinks, &stichrichtungli, stichlage);
            einstichpunkt.x = mittelpunkt.x + vektor.x;
            einstichpunkt.y = mittelpunkt.y + vektor.y;
            stichpunktausgeben(einstichpunkt);
        }
        else
        {
            stichwinkel = drehwinkel(mittelpunkt, letztekoord);
        }
        if ( fadengeschnitten )
        {
            betrstichnlinks = 0;
        }
        else
        {
            /* EasyCODE < */
            riegelanfang = FALSE;
            /* Stiche ausgeben */
            /* EasyCODE > */
        }
        stichnrabschn = 1;
        while ( (stichnrabschn < sticheproabschnitt) && (fehler == 0) )
        {
            stichnrabschn = stichnrabschn + 1;
            stichwinkel = stichwinkel + stichwinkeldiff;
            berechnevektor(radius, stichwinkel, &vektor, betrstichnlinks, &stichrichtungli, stichlage);
            einstichpunkt.x = mittelpunkt.x + vektor.x;
            einstichpunkt.y = mittelpunkt.y + vektor.y;
            stichpunktausgeben(einstichpunkt);
        }
        if ( (fadengeschnitten || !riegelende) && (fehler == 0) )
        {
            stichwinkel = drehwinkel(mittelpunkt, endpunkt);
            berechnevektor(radius, stichwinkel, &vektor, betrstichnlinks, &stichrichtungli, stichlage);
            einstichpunkt.x = mittelpunkt.x + vektor.x;
            einstichpunkt.y = mittelpunkt.y + vektor.y;
            stichpunktausgeben(einstichpunkt);
        }
        letztekoord = endpunkt;
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::berechnekurvenlaenge */
/* EasyCODE F */
/*----------------------------------------------------------------------------
| Stichgenerierung auf Kurven
----------------------------------------------------------------------------*/

void KlasseKonvert::berechnekurvenlaenge (offs_adr  puffer,
                           float     *laenge)

  /* Vereinfachte Berechnung der Laenge einer Kurve
    vom KurvenAnfang bis einschliesslich Puffer.

    Puffer groesser als FirstC sein !!

    Laenge :  LÑnge vom Kurvenanfang bis PufferElement */
{
    offs_adr     intpuffer;
    abschntype   abschnitt;
    koordinate   punkt;
    
    *laenge = 0;
    intpuffer = firstc;
    rdabsch (&abschnitt,intpuffer);
    if ( abschnitt.abschnittart == mnullpkt )
    {
        punkt.x = 0;
        punkt.y = 0;
    }
    else
    {
        punkt.x = abschnitt._VAR.skoordabsch.koord.x;
        punkt.y = abschnitt._VAR.skoordabsch.koord.y;
    }
    do{
        intpuffer = nachf(intpuffer);
        rdabsch (&abschnitt,intpuffer);
        if ( is_kurvenpunkt (abschnitt.abschnittart) )
        {
            *laenge = *laenge + sqrt(fsqr(abschnitt._VAR.skoordabsch.koord.x - punkt.x)
                                   + fsqr(abschnitt._VAR.skoordabsch.koord.y - punkt.y));
            punkt.x = abschnitt._VAR.skoordabsch.koord.x;
            punkt.y = abschnitt._VAR.skoordabsch.koord.y;
        }
    }while ( !(intpuffer == puffer) );
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::berechne_t */
/* EasyCODE F */
void KlasseKonvert::berechne_t (offs_adr  puffer, tarraytyp t)
{
    offs_adr     intpuffer;
    abschntype   abschnitt;
    fkoordtyp    punkt;
    
    
    intpuffer = firstc;
    
    t[1] = 0;
    t[2] = 0;
    t[3] = 0;
    
    rdabsch (&abschnitt, intpuffer);
    if ( abschnitt.abschnittart == mnullpkt )
    {
        punkt.x = 0;
        punkt.y = 0;
    }
    else
    {
        punkt.x = abschnitt._VAR.skoordabsch.koord.x;
        punkt.y = abschnitt._VAR.skoordabsch.koord.y;
    }
    do{
        intpuffer = nachf (intpuffer);
        rdabsch (&abschnitt, intpuffer);
        if ( is_kurvenpunkt (abschnitt.abschnittart) )
        {
            t[0] = t[1];
            t[1] = t[2];
            t[2] = t[3];
            t[3] = t[2] + sqrt(fsqr(abschnitt._VAR.skoordabsch.koord.x - punkt.x) + fsqr(abschnitt._VAR.skoordabsch.koord.y - punkt.y));
            punkt.x = abschnitt._VAR.skoordabsch.koord.x;
            punkt.y = abschnitt._VAR.skoordabsch.koord.y;
        }
    }while ( !(intpuffer == puffer) );
    if ( intpuffer != lastc )
    {
        do{
            intpuffer = nachf(intpuffer);
            rdabsch(&abschnitt, intpuffer);
        }while ( !((abschnitt.abschnittart == kurve) || (intpuffer == lastc)) );
        t[4] = t[3] + sqrt(fsqr(abschnitt._VAR.skoordabsch.koord.x - punkt.x) +
                           fsqr(abschnitt._VAR.skoordabsch.koord.y - punkt.y));
    }
    else
    {
        t[4] = 0;
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::suchekurvenpunkte */
/* EasyCODE F */
void KlasseKonvert::suchekurvenpunkte (offs_adr       puffer,
                                       punktarraytyp  pktarray,
                                       boolean        * kurvenanfpkt,
                                       boolean        * kurvenendpkt,
                                       tarraytyp      t)
{
    /* EasyCODE < */
    offs_adr     intpuffer;
    abschntype   abschnitt;
    
    *kurvenanfpkt = TRUE;
    *kurvenendpkt = TRUE;
    
    intpuffer = puffer;
    
    /* Parametrisierung bestimmen */
    berechne_t (puffer, t);
    
    /* Koord[1] (aktuellen Kurvenstuetzpunkt) bestimmen */
    rdabsch (&abschnitt, puffer);
    pktarray[3].x = abschnitt._VAR.skoordabsch.koord.x;
    pktarray[3].y = abschnitt._VAR.skoordabsch.koord.y;
    
    /* Koord[2] (nachfolgenden Kurvenstuetzpunkt) und KurvenEndPkt bestimmen */
    /* EasyCODE > */
    if ( puffer < lastc )
    {
        *kurvenendpkt = FALSE;
        do{
            puffer = nachf(puffer);
            rdabsch(&abschnitt, puffer);
        }while ( !( is_kurvenpunkt(abschnitt.abschnittart)
                 || (puffer == lastc) ) );
        pktarray[4].x = abschnitt._VAR.skoordabsch.koord.x;
        pktarray[4].y = abschnitt._VAR.skoordabsch.koord.y;
    }
    /* EasyCODE < */
    /* Koord[0] (vorausgehenden Stuetzpunkt) bestimmen */
    /* EasyCODE > */
    do{
        intpuffer = vorg(intpuffer);
        rdabsch(&abschnitt, intpuffer);
    }while ( !((abschnitt.abschnittart == kurve) || (intpuffer == firstc)) );
    pktarray[2].x = abschnitt._VAR.skoordabsch.koord.x;
    pktarray[2].y = abschnitt._VAR.skoordabsch.koord.y;
    if ( intpuffer != firstc )
    {
        /* KurvenAnfPkt, KurvenPkt2, Koord[-1] und Koord[-2] bestimmen */
        
        *kurvenanfpkt = FALSE;
        do{
            intpuffer = vorg(intpuffer);
            rdabsch(&abschnitt, intpuffer);
        }while ( !((abschnitt.abschnittart == kurve) || (intpuffer == firstc)) );
        pktarray[1].x = abschnitt._VAR.skoordabsch.koord.x;
        pktarray[1].y = abschnitt._VAR.skoordabsch.koord.y;
        if ( (abschnitt.abschnittart == kurve) )
        {
            do{
                intpuffer = vorg(intpuffer);
                rdabsch(&abschnitt, intpuffer);
            }while ( !((abschnitt.abschnittart == kurve) || (intpuffer == firstc)) );
            pktarray[0].x = abschnitt._VAR.skoordabsch.koord.x;
            pktarray[0].y = abschnitt._VAR.skoordabsch.koord.y;
        }
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::berech_polynom */
/* EasyCODE F */
void KlasseKonvert::berech_polynom (float     t,
                                    parraytyp p,
                                    float     arg,
                                    fkoordtyp * polypkt)

/* Auswertung des kubischen Polynoms */
{
    float            hilf;
    
    hilf = arg - t;
    polypkt->x = ((p[3].x * hilf + p[2].x) * hilf + p[1].x) * hilf + p[0].x;
    polypkt->y = ((p[3].y * hilf + p[2].y) * hilf + p[1].y) * hilf + p[0].y;
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::berechnenormalenvektor */
/* EasyCODE F */
void KlasseKonvert::berechnenormalenvektor (parraytyp p,
                                            tarraytyp t,
                                            float     aktt,
                                            float     betrstichnlinks,
                                            fkoordtyp *stichnachlinks)
{
    float      laenge;
    fkoordtyp  vektor;
    parraytyp pb;
    
    /* Ableitung berechnen */
    pb[0].x = p[1].x;
    pb[1].x = 2 * p[2].x;
    pb[2].x = 3 * p[3].x;
    pb[3].x = 0;
    pb[0].y = p[1].y;
    pb[1].y = 2 * p[2].y;
    pb[2].y = 3 * p[3].y;
    pb[3].y = 0;
    berech_polynom (t[2], pb, aktt, &vektor);
    
    /* Normieren und neue Laenge berechnen */
    laenge = betrstichnlinks / (sqrt(fsqr(vektor.x) + fsqr(vektor.y)));
    stichnachlinks->x = -vektor.y * laenge;
    stichnachlinks->y = vektor.x * laenge;
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::cf */
/* EasyCODE F */
void KlasseKonvert::cf (punktarraytyp punkt,
                        tarraytyp     t,
                        boolean       kurvenanfang,
                        boolean       kurvenende,
                        parraytyp     p,
                        byte          *fehler)

/* Die Routine CF berechnet die Koeffizienten der kubischen Polynome in X-
 * und Y-Richtung im Intervall (T(0), T(1)). Die Koeffizienten werden im Feld
 * P [1..4] uebergeben. */
{
    koordinate d[ /* -1.. 1 */ 3];
    koordinate ableit[ /* 0.. 1 */ 2];
    koordinate del;
    float      hilf, hilf0, delquad, cos2w, sin2w;
    
    
    *fehler = 0;
    hilf = t[3] - t[2];
    if ( hilf < 0.001 )
    {
        *fehler = 18;
    }
    else
    {
        /* EasyCODE < */
        del.x = (punkt[3].x - punkt[2].x) / hilf;
        del.y = (punkt[3].y - punkt[2].y) / hilf;
        
        /* wenn Punkt [0] und Punkt [1] Anfang bzw Ende der Kurve, dann Kurve =
         * Gerade */
        /* EasyCODE > */
        if ( kurvenanfang && kurvenende )
        {
            p[0] = punkt[2];
            p[1] = del;
            p[2].x = 0;
            p[2].y = 0;
            p[3].x = 0;
            p[3].y = 0;
        }
        else
        {
            if ( !kurvenende )
            {
                hilf0 = t[4] - t[2];
                if ( hilf0 < 0.001 )
                {
                    *fehler = 19;
                }
                else
                {
                    d[2].x = (punkt[4].x - punkt[2].x) / hilf0;
                    d[2].y = (punkt[4].y - punkt[2].y) / hilf0;
                }
            }
            if ( !kurvenanfang && (*fehler == 0) )
            {
                hilf0 = t[3] - t[1];
                if ( hilf0 < 0.001 )
                {
                    *fehler = 20;
                }
                else
                {
                    d[1].x = (punkt[3].x - punkt[1].x) / hilf0;
                    d[1].y = (punkt[3].y - punkt[1].y) / hilf0;
                }
            }
            if ( *fehler == 0 )
            {
                if ( kurvenanfang || kurvenende )
                {
                    delquad = fsqr(del.x) + fsqr(del.y);
                    if ( delquad != 0 )
                    {
                        cos2w = (fsqr(del.x) - fsqr(del.y)) / delquad;
                        sin2w = (2 * del.x * del.y) / delquad;
                        if ( kurvenanfang )
                        {
                            d[1].x = cos2w * d[2].x + sin2w * d[2].y;
                            d[1].y = sin2w * d[2].x - cos2w * d[2].y;
                        }
                        else
                        {
                            d[2].x = cos2w * d[1].x + sin2w * d[1].y;
                            d[2].y = sin2w * d[1].x - cos2w * d[1].y;
                        }
                    }
                    else
                    {
                        *fehler = 21;
                    }
                }
            }
            if ( *fehler == 0 )
            {
                ableit[1].x = d[2].x;
                ableit[0].x = d[1].x;
                ableit[1].y = d[2].y;
                ableit[0].y = d[1].y;
                
                /* Die Ableitungen in den Punkten T(I), Punkt(I) und T(I+1),
                 * Punkt(I+1) sind Ableit [0] und Ableit [1] */
                
                /* Auswertung der Koeffizienten in X- und Y-Richtung */
                p[0] = punkt[2];
                p[1] = ableit[0];
                
                p[2].x = (-2 * ableit[0].x - ableit[1].x + 3 * del.x) / hilf;
                p[3].x = (ableit[0].x + ableit[1].x - 2 * del.x) / (hilf * hilf);
                
                p[2].y = (-2 * ableit[0].y - ableit[1].y + 3 * del.y) / hilf;
                p[3].y = (ableit[0].y + ableit[1].y - 2 * del.y) / (hilf * hilf);
            }
        }
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::bearbeitekurve */
/* EasyCODE F */
void KlasseKonvert::bearbeitekurve (boolean abschnittsweise)
{
    punktarraytyp   punktarray;
    parraytyp       p;
    tarraytyp       t;
    fkoordtyp       einstichpunkt, ueberstich;
    float           betrstichnlinks, aktt, deltat, kurvenlaenge;
    int             sticheproabschnitt, stichnrabschn;
    
    offs_adr        aktadr;
    abschntype      abschnitt;
    
    boolean         kurvenanfpkt, kurvenendpkt, stichzulang;
    if ( (fstichl <= 0) || (fstichl > maxstichl) )
    {
        fehler = 12;
    }
    else
    {
        /* EasyCODE ( 0 
        Variablen vorbelegen */
        /* EasyCODE < */
        /* Besetzen der Variablen FirstC, LastC und CurrentC: FirstC   = Letzter
         * KoordAbschnitt vor der Kurve LastC    = Letzter KoordAbschnitt der
         * Kurve (Kurvenendpunkt) CurrentC = Aktueller Abschnitt innerhalb der
         * Kurve (= LastC zu Anfang der Berechnung) */
        
        /* CurrentC besetzen */
        currentc = vorg(abschadr);  /* CurrentC = Adresse des aktuellen
                                     * Abschnitts */
        aktadr = currentc;
        
        /* FirstC besetzen */
        /* EasyCODE > */
        do{
            currentc = vorg(currentc);
            rdabsch(&abschnitt, currentc);
        }while ( ! ( ( (is_koordabschn (abschnitt.abschnittart)
                 && (abschnitt.abschnittart != kurve) ) )
                 || (currentc <= abschanf) ) );
        if ( currentc > abschanf )
        {
            firstc = currentc;
        }
        else
        {
            fehler = 13;
        }
        /* EasyCODE ) */
    }
    if ( fehler == 0       /* LastC besetzen */ )
    {
        /* EasyCODE < */
        /* bei Abschnittsweiser Stichgenerierung ist auch */
        /* das Programmende als Abbruchbedingung erlaubt  */
        /* EasyCODE > */
        do{
            currentc = nachf(currentc);
            rdabsch(&abschnitt, currentc);
        }while ( !( (abschnitt.abschnittart == programmende)
                 || (currentc >= maschanf)
                 || (is_koordabschn (abschnitt.abschnittart)
                   && (abschnitt.abschnittart != kurve) ) ) );
        if ( abschnitt.abschnittart == kurvenende )
        {
            lastc = currentc;
        }
        else
        {
            /* EasyCODE ( 0 */
            if ( !abschnittsweise )
            {
                fehler = 15;
            }
            else
            {
                do{
                    currentc = vorg(currentc);
                    rdabsch(&abschnitt, currentc);
                }while ( !((abschnitt.abschnittart == kurve) || (currentc <= aktadr)) );
                if ( (currentc > aktadr) && (abschnitt.abschnittart == kurve) )
                {
                    lastc = currentc;
                }
                else
                {
                    fehler = 15;
                }
            }
            /* EasyCODE ) */
        }
    }
    /* EasyCODE < */
    /* einfacher Algorithmus mit aequidistanter Teilung der Bogenlaenge,
     * vereinfacht dargestellt durch den Parameter t */
    /* EasyCODE > */
    if ( fehler == 0 )
    {
        if ( abschnittsweise )
        {
            /* EasyCODE ( 0 */
            currentc = aktadr;
            berechne_t(currentc, t);
            sticheproabschnitt = trunc((t[3] - t[2]) / fstichl + 0.99);
            if ( sticheproabschnitt == 0       /* um Division durch 0 zu vermeiden */ )
            {
                sticheproabschnitt = 1;
            }
            deltat = (t[3] - t[2]) / sticheproabschnitt;
            aktt = t[2];
            /* EasyCODE ) */
        }
        else
        {
            /* CurrentC auf 1. Kurvenstuetzpunkt positionieren */
            currentc = firstc;
            do{
                currentc = nachf(currentc);
                rdabsch(&abschnitt, currentc);
            }while ( !is_kurvenpunkt(abschnitt.abschnittart) );
            /* BogenLaenge und Schrittweite (DeltaT) berechnen */
            berechnekurvenlaenge(lastc, &aktt);
            kurvenlaenge = aktt;
            if ( stichbr == 0 )
            {
                sticheproabschnitt = trunc(aktt / fstichl + 0.99);
                if ( sticheproabschnitt == 0 )
                {
                    sticheproabschnitt = 1;
                }
            }
            else
            {
                sticheproabschnitt = 2 * trunc(aktt / (2 * fstichl) + 0.99);
                if ( sticheproabschnitt == 0 )
                {
                    sticheproabschnitt = 2;
                }
            }
            deltat = aktt / sticheproabschnitt;
            aktt = 0;
        }
        betrstichnlinks = stichbr / 2;
        suchekurvenpunkte(currentc, punktarray, &kurvenanfpkt, &kurvenendpkt, t);
        cf(punktarray, t, kurvenanfpkt, kurvenendpkt, p, &fehler);
    }
    if ( fehler == 0 )
    {
        if ( riegelende && !fadengeschnitten         /* nicht bei Absch_gen */ )
        {
            aktt = deltat / 2;
            while ( (aktt >= t[3]) && (fehler == 0) )
            {
                do{
                    currentc = nachf(currentc);
                    rdabsch(&abschnitt, currentc);
                    if ( ! is_kurvenpunkt(abschnitt.abschnittart) )
                    {
                        if ( !abschnittsweise )
                        {
                            bearbeitefunktion(abschnitt);
                        }
                    }
                }while ( !(is_kurvenpunkt(abschnitt.abschnittart)) );
                suchekurvenpunkte(currentc, punktarray, &kurvenanfpkt, &kurvenendpkt, t);
                cf(punktarray, t, kurvenanfpkt, kurvenendpkt, p, &fehler);
            }
            if ( fehler == 0 )
            {
                berech_polynom (t[2], p, aktt, &einstichpunkt);
                if ( stichbr != 0 )
                {
                    berechnenormalenvektor(p, t, aktt, betrstichnlinks, &stichnachlinks);
                }
                berechneueberstich(einstichpunkt, &ueberstich, stichnachlinks, &stichrichtungli, stichlage);
                stichpunktausgeben(ueberstich);
            }
        }
        else
        {
            einstichpunkt = letztekoord;
        }
    }
    if ( !fadengeschnitten )
    {
        riegelanfang = FALSE;
    }
    stichnrabschn = 1;
    while ( (stichnrabschn < sticheproabschnitt) && (fehler == 0) )
    {
        /* EasyCODE ( 0 
        Stiche berechnen mit Algorithmus, der Stiche > 6mm verhindert */
        stichnrabschn = stichnrabschn + 1;
        do{
            stichzulang = FALSE;
            aktt = aktt + deltat;
            while ( (aktt >= t[3]) && (fehler == 0) )
            {
                do{
                    currentc = nachf(currentc);
                    rdabsch(&abschnitt, currentc);
                    if ( !(is_kurvenpunkt(abschnitt.abschnittart)) )
                    {
                        if ( !abschnittsweise )
                        {
                            bearbeitefunktion(abschnitt);
                        }
                    }
                }while ( !(is_kurvenpunkt(abschnitt.abschnittart)) );
                suchekurvenpunkte(currentc, punktarray, &kurvenanfpkt, &kurvenendpkt, t);
                cf(punktarray, t, kurvenanfpkt, kurvenendpkt, p, &fehler);
            }
            if ( fehler == 0 )
            {
                berech_polynom (t[2], p, aktt, &einstichpunkt);
                if ( stichbr != 0 )
                {
                    berechnenormalenvektor(p, t, aktt, betrstichnlinks, &stichnachlinks);
                    berechneueberstich(einstichpunkt, &ueberstich, stichnachlinks, &stichrichtungli, stichlage);
                }
                else
                {
                    ueberstich = einstichpunkt;
                }
                if ( round(sqrt(fsqr(einstichpunkt.x-letztereinstich.x) +
                     fsqr(einstichpunkt.y-letztereinstich.y))) > maxstichl )
                {
                    stichzulang = TRUE;
                    sticheproabschnitt++;
                    aktt -= deltat;
                    deltat = (kurvenlaenge-aktt)/(sticheproabschnitt-stichnrabschn+2);
                }
                else
                {
                    stichpunktausgeben(ueberstich);
                }
            }
        }while ( stichzulang );
        /* EasyCODE ) */
    }
    rdabsch(&abschnitt, currentc);
    while ( (abschnitt.abschnittart != kurvenende) && (fehler == 0) && (currentc < lastc) && (!abschnittsweise) )
    {
        currentc = nachf(currentc);
        rdabsch(&abschnitt, currentc);
        if ( !(is_kurvenpunkt(abschnitt.abschnittart)) )
        {
            bearbeitefunktion(abschnitt);
        }
    }
    if ( (fadengeschnitten || !riegelende) && (fehler == 0) )
    {
        berechneueberstich(einstichpunkt, &ueberstich, stichnachlinks,
                  &stichrichtungli, stichlage); /* Um Stichrichtung zu setzten */
        einstichpunkt.x = abschnitt._VAR.skoordabsch.koord.x;
        einstichpunkt.y = abschnitt._VAR.skoordabsch.koord.y;
        stichpunktausgeben(einstichpunkt);
    }
    letztekoord.x = abschnitt._VAR.skoordabsch.koord.x;
    letztekoord.y = abschnitt._VAR.skoordabsch.koord.y;
    abschadr = nachf(lastc);
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::block_abb */
/* EasyCODE F */
void KlasseKonvert::block_abb (offs_adr blockanf,
                offs_adr blockende,
                ikoordtyp nullpunkt,
                float scalex,
                float scaley,
                float drehwinkel,
                byte spiegeln)


  /* Abbildung eines Blocks nach folgender Vorschrift:

    - Verschieben des Blocks in den uebergebenen Nullpunkt
    - wenn Spiegeln = TRUE, Spiegeln des Block an der Geraden
      in Y-Richtung durch den Nullpunkt
    - Drehen des Blocks um Drehwinkel (Angabe in Grad)
    - skalieren des Blocks um den Faktor ScaleX bzw ScaleY
    - Verschieben des Blocks, an die urspruengliche Position

    Achtung !! bei der Manipulation eines Blocks verschiebt sich dessen
    Endpunkt. Um unmittelbar auf den Block folgende Kreise an diese
    Verschiebung anzupassen, muessen nach dem Unteprogramm BlockAbb
    folgende Schritte ausgefÅhrt werden:

    SucheKoord (BlockEnde, EndPunkt);
    MemErr := VerschiebeAbschnitt (BlockEnde, EndPunkt); */
{
    /* EasyCODE < */
    offs_adr        blockelem;
    abschntype      abschnitt;
    abbmatrixtyp    abbmatrix, hilfsmatrix;
    int             xwert;
    word            memerr;
    
    
    
    /* Variablen initialisieren  */
    memerr = 0;
    initmatrix (abbmatrix);
    initmatrix (hilfsmatrix);
    
    /* Verschiebung in Nullpunkt  */
    abbmatrix[0][2] = -nullpunkt.x;
    abbmatrix[1][2] = -nullpunkt.y;
    
    /* Spiegelung  */
    /* EasyCODE > */
    if ( spiegeln )
    {
        hilfsmatrix[0][0] = -1;
        matmultipl(abbmatrix, hilfsmatrix);
    }
    /* EasyCODE < */
    /* Drehung  */
    /* EasyCODE > */
    if ( drehwinkel != 0 )
    {
        initmatrix(hilfsmatrix);
        drehwinkel = drehwinkel * (PI / 180);
        hilfsmatrix[0][0] = cos(drehwinkel);
        hilfsmatrix[0][1] = -sin(drehwinkel);
        hilfsmatrix[1][0] = sin(drehwinkel);
        hilfsmatrix[1][1] = cos(drehwinkel);
        matmultipl(abbmatrix, hilfsmatrix);
    }
    /* EasyCODE < */
    /* Skalierung  */
    /* EasyCODE > */
    if ( (scalex != 1) || (scaley != 1) )
    {
        initmatrix(hilfsmatrix);
        hilfsmatrix[0][0] = scalex;
        hilfsmatrix[1][1] = scaley;
        matmultipl(abbmatrix, hilfsmatrix);
    }
    /* Rueckverschiebung  */
    initmatrix(hilfsmatrix);
    hilfsmatrix[0][2] = nullpunkt.x;
    hilfsmatrix[1][2] = nullpunkt.y;
    matmultipl(abbmatrix, hilfsmatrix);
    
    /* Block durchgehen und Abbildung durchfuehren  */
    blockelem = blockanf;
    while ( (blockelem != blockende) && (memerr == 0) )
    {
        blockelem = nachf(blockelem);
        rdabsch (&abschnitt, blockelem);
        if ( is_koordabschn(abschnitt.abschnittart) )
        {
            xwert = abschnitt._VAR.skoordabsch.koord.x;
            abschnitt._VAR.skoordabsch.koord.x = round(xwert * abbmatrix[0][0] + abschnitt._VAR.skoordabsch.koord.y * abbmatrix[0][1] + abbmatrix[0][2]);
            abschnitt._VAR.skoordabsch.koord.y = round(xwert * abbmatrix[1][0] + abschnitt._VAR.skoordabsch.koord.y * abbmatrix[1][1] + abbmatrix[1][2]);
            wrabsch (&abschnitt, blockelem);
        }
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKonvert::absch_gen */
/* EasyCODE F */
/*----------------------------------------------------------------------------
| Unterprogramm 'absch_gen' zur abschnittsweisen Stichgenerierung
----------------------------------------------------------------------------*/

byte KlasseKonvert::absch_gen (byte huge * proganf,  /* Zeiger auf das erste Byte des Programms   */
                    word std_stichl,      /* StandardstichlÑnge                        */
                    word mem_avail,       /* Grî·e des Speicherbereichs                */
                    offs_adr absch_adr,   /* Adresse des zu konvertierenden Abschnitts */
                    offs_adr * masch_adr) /* Adresse der Stichdaten                    */



/* Eingangsparameter:

   proganf    : Anfangsadresse des Naehprogramms im Speicher
   std_stichl : gewuenschte Stichlaenge in 1/100 mm
   mem_avail  : Grî·e des zur VerfÅgung stehenden Speicherbereichs
   absch_adr  : Adresse des zu konvertierenden Abschnitts
                (Offset bezogen auf proganf)
   masch_adr  : Adresse, an die die Stichdaten geschrieben werden
                sollen (Offset bezogen auf proganf). Zeigt nach
                Beendigung von absch_gen auf das erste Byte hinter
                den generierten Stichdaten.

   Die im Naehprogrammkopf eingetragene Gesamtlaenge des Programms muss
   korrekt sein (Zeigt wie bei unvollstaendigem Programm auf das Byte
   hinter dem geometrischen Datensatz (FF))

   RÅckmeldung:

   absch_gen == 0: Stichgenerierung ohne Fehler abgeschlossen.
   absch_gen != 0: Fehlernummer entsprechend obiger Tabelle.

   Die Ausgabe der Naehdaten erfolgt in den freien Speicherbereich hinter den
   geometrischen Datensatz (einschliesslich des FF).  */
{
    /* EasyCODE < */
    ikoordtyp hilfkoord;
    abschntype abschnitt;
    
    prog_anf = proganf;
    init_masch_param ();
    
    abschanf = geooffs; /* Anfangsadresse Abschnittdatensatz */
    abschadr = absch_adr;
    maschadr = *masch_adr;
    
    maschanf = *masch_adr;
    
    maxmem = mem_avail - 2;
    
    fehler = 0;
    fehler14 = FALSE;
    
    fstichl = std_stichl / (float)(10);
    stichbr = 0;
    anzahlstiche = 0;
    stichlage = mittigli;
    stichrichtungli = TRUE;
    stichnachlinks.x = 0;
    stichnachlinks.y = 0;
    riegelanfang = FALSE;
    riegelende = FALSE;
    lpprogramm = FALSE;
    mfktinpuffer = FALSE;
    fadengeschnitten = TRUE;
    maschinenaeht = FALSE;
    stichgemacht = FALSE;
    
    suchekoord(abschadr, &hilfkoord);
    letztekoord.x = hilfkoord.x;
    letztekoord.y = hilfkoord.y;
    
    /* Pruefen der Naehfeldgrenzen */
    /* EasyCODE > */
    if ( hilfkoord.x < minnaehfeld_x )
    {
        fehler14 = TRUE;
        hilfkoord.x = minnaehfeld_x;
    }
    if ( hilfkoord.x > maxnaehfeld_x )
    {
        fehler14 = TRUE;
        hilfkoord.x = maxnaehfeld_x;
    }
    if ( hilfkoord.y < minnaehfeld_y )
    {
        fehler14 = TRUE;
        hilfkoord.y = minnaehfeld_y;
    }
    if ( hilfkoord.y > maxnaehfeld_y )
    {
        fehler14 = TRUE;
        hilfkoord.y = maxnaehfeld_y;
    }
    letztereinstich.x = hilfkoord.x;
    letztereinstich.y = hilfkoord.y;
    
    /* Stichgenerierung */
    readnextabsch(&abschnitt);
    switch ( abschnitt.abschnittart )
    {
    case stich :
        bearbeitestich(abschnitt);
        break;
    case gerade :
        bearbeitegerade(abschnitt);
        break;
    case kreisbogen :
        /* EasyCODE = */
    case kreis :
        /* EasyCODE = */
    case krsstuetzpkt :
        /* EasyCODE = */
    case bgnstuetzpkt :
        bearbeitekreis(abschnitt, TRUE);
        break;
    case kurve :
        /* EasyCODE = */
    case kurvenende :
        bearbeitekurve(TRUE);
        break;
    default:
        /* EasyCODE < */
        stichpunktausgeben(letztereinstich);        /* Nullstich ausgeben */
        /* EasyCODE > */
    }
    if ( (fehler == 0) && fehler14 )
    {
        fehler = 14;
    }
    *masch_adr = maschadr;
    return fehler;
}

/* EasyCODE ) */
#if defined debug_info
    /* EasyCODE ( 0 
    KlasseKonvert::abschn_ausgeben */
    /* EasyCODE F */
    void KlasseKonvert::abschn_ausgeben (abschntype abschnitt)
    {
        switch ( abschnitt.abschnittart )
        {
        case anfpkt :
            printf ("\nAnfangspunkt: ");
            i_koord_ausgeben (abschnitt._VAR.skoordabsch.koord);
            break;
        case gerade :
            printf ("\nGerade:       ");
            i_koord_ausgeben (abschnitt._VAR.skoordabsch.koord);
            break;
        default:
            ;
        }
    }

    /* EasyCODE ) */
    /* EasyCODE ( 0 
    KlasseKonvert::i_koord_ausgeben */
    /* EasyCODE F */
    void KlasseKonvert::i_koord_ausgeben (ikoordtyp koord)
    {
        printf("%3d, %3d", koord.x, koord.y);
    }

    /* EasyCODE ) */
    /* EasyCODE ( 0 
    KlasseKonvert::f_koord_ausgeben */
    /* EasyCODE F */
    void KlasseKonvert::f_koord_ausgeben (fkoordtyp koord)
    {
        printf("%5.2f, %5.2f", koord.x, koord.y);
    }

    /* EasyCODE ) */
#endif 
/* EasyCODE ) */
