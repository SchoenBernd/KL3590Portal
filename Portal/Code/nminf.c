/* EasyCODE V8 */
/* EasyCODE ( 0 
Naehmotor-Interface */
/* EasyCODE ( 0 
Beschreibung, Header */
/******************************************************************************
Datei:         nminf.c
Klasse:        Kl3590 Portal
Ersteller:     Halfmann  EWE 2856

Dieses Programm steuert den Automatenmotor von Quick ueber eine serielle Schnittstelle.
Die serielle Schnittstelle ist ein Teil der Hauptantriebsansteuerung auf der BS3. Weitere
Teile sind die Positionsauswertung und die Drehzahlmessung.
Neben den normalen Funktionsaufrufen (Beschreibung unten) ueber den Parameterblock,
werden einige Unterprogramme zu Verfuegung gestellt, die nicht auf die serielle Schnittstelle
zugreifen und direkt ein Ergebnis zurueckmelden:
nadel_ot   ..........   prueft, ob Nadel in OT steht
nadel_ber   .......   prueft, ob Nadel in einem bestimmten Bereich steht
taktnadel_ber ..   prueft, ob Nadel in best. Bereich (takt_bereich) steht
nadel_pos   ......   meldet die aktuelle Nadelposition in Grad zurueck
get_drehzahl ...   meldet die aktuelle Drehzahl in upm zurueck

Ausserdem gibt es zwei spezielle Unterprogramme, die waehrend NIS aufgerufen werden.
Diese dienen zur schnellstmoeglichen Ausgabe des Drehzahl- bzw. Totmann-Kommandos.
Sie duerfen nur waehrend einer DI-Phase aufgerufen werden, damit sie nicht durch die Task
nminfbs3 unterbrochen werden koennen.
gsw_fastout  ....  schnellstmoegliche Ausgabe des Drehzahlkommandos
tot_fastout  .......  schnellstmoegliche Ausgabe des Totmannkommandos

Zur Einstellung der Reaktionszeit des Fadenwaechters dient die Funktion nmpawini, mit der
die Pulsauswertung neu initialisiert wird.
******************************************************************************/

#include "portal.h"
/* EasyCODE ) */
KlasseNminfBS3 nminf (tdini[NmInfIndex], FP_OFF(NM));
/* EasyCODE ( 0 
KlasseNminfBS3::nminftask */
/* EasyCODE F */
/*************************** nminfbs3 ******************************************
Das Naehmotorinterface bedient den Quick-Automatenantrieb.
Alle Meldungen/Rueckmeldungen laufen über den Parameterblock des Nähmotorinterface.
Folgende Befehle sind in befehl definiert:
    0   Totmann          Sendet nach Ablauf der Totmannzeit den Befehl Totmann.
                         Wird nicht alle xx ms gesendet macht der Antrieb Notstop.
    1   Reset            Setzt die serielle Schnittstelle des Antriebs zurück.
    2   Init             1. Kommunikation mit dem Antrieb. Liefert die Versionsnummer vom Antrieb
                         und setzt die Schnittstelle zurück.
    3   Referenz1        Setzt den Parameter 22 im Antrieb. Der Wert muss in parameter stehen.
    4   Referenz2        Setzt den Parameter 23 im Antrieb. Der Wert muss in parameter stehen.
    5   Positioniere     Positioniert den Antrieb auf der Position  position (0 - 127). Dabei wird
                         die in  posart angegebene Positionsart angewandt.
                         posart:  0 0 0 0 U S K R
                                          | | | |___ 1 = rueckwärts, 0 = vorwaerts
                                          | | |____ kuerzester Weg
                                          | |_____ Restbremse aus nach Positionieren (nicht realisiert)
                                          |______ Unsicher, Stop wird nicht abgewartet (nicht realisiert)
    6  Totmann-Zeit      Initialisiert die Totmannzeit im Antrieb. Der Wert muss in parameter stehen.
    7  Drehzahl          Schaltet Drehzahlen auf. Ist der Wert drehzahl positiv, wird vorwärts gedreht.
                         Ist er negativ wird rückwärts gedreht.
    8  Notstop           Sendet den Befehl Notstop (Stop ohne Position).
    9  Parameter         Liest Parameter vom Antrieb in parameter . Die zu lesende Parameternummer
          lesen          steht in parameternr.
  10  Parameter          Schreibt Parameter in den Antrieb. Die zu schreibende Parameternummer steht in
        schreiben        parameternr.  Der Wert steht in  parameter.
  11  Reset Stich-       Setzt den Stichzähler des Antriebs zurück.
          zähler         (Nicht realisert).
  12  StopX              Sendet den Befehl Stop nach X Stichen an den Antrieb.
                         Die Position ist  in position und die Stichanzahl in  stichzahl abgelegt
                         (Nicht realisert).
  13  Restbremse         Sendet den Parameter 7 an den Antrieb. Der Wert der Restbremsung steht in
            ein          parameter.
  14  Restbremse         Setzt den Parameter 7 im Antrieb auf 0.
            aus
  15  Positionsabfrage   Liest die aktuelle Istposition vom Antrieb. Der eingelesene Wert wird in
                         position abgelegt.
                         (Nicht realisert).
  16  Drehzahlabfrage    Liest die aktuelle Istdrehzahl vom Antrieb. Der eingelesene Wert wird in
                         drehzahl abgelegt.
                         (Nicht realisert).
  17  Positionier-       Sendet den Parameter 16 an den Antrieb. Der Wert der Positionierdrehzahl muss
         drehzahl        in parameter stehen.
  18  Drehzahl und       Sendet die in drehzahl stehende Drehzahl und anschliessend einen
         Positionieren   Positionierbefehl an den Antrieb.
                         Die Position und die Positionsart müssen in  position und posart
                         stehen.
  20  Softwarestand      Sendet den Befehl 0x14 an den Hauptantrieb. Dieser antwortet mit einem
         abfragen        ASCII-String im Format 0419/XXX (Softwareversion).

Bedeutung des Fehlerbytes fehler:
    0    .....................    kein Fehler
    1    .....................    _bertragungsfehler gemeldet von der BS3
    2    .....................    Timeout serielle Schnittstelle
    3    ......................    Checksummenfehler bei den empfangenen Daten
    4    ......................    Timeout Befehl
    0x30 - 0x3f    ......    Uebertragungsfehler von Quick
    0x40 - 0xff    ......    Hardware-Fehler Quick
 *****************************************************************************/

void KlasseNminfBS3::nminftask (void)
{
    if ( !nminit_ok )
    {
        /* EasyCODE < */
        // falls Init nicht o.K.
        // - keine Aktionen
        /* EasyCODE > */
    }
    else
    {
        if ( befehl == 8 )
        {
            /* EasyCODE < */
            // Notstop
            /* EasyCODE > */
            if ( nmauftrag != 8 )
            {
                nmauftrag = 8;
                nmkey = 0;
            }
        }
        else
        {
            if ( (nmauftrag == 0) && (nmkey == 0) && (befehl != 0) )
            {
                nmauftrag = befehl;
                lastbef = befehl;
            }
        }
        switch ( nmauftrag )
        {
        case 0 :
            totmann ();
            break;
        case 1 :
            reset ();
            break;
        case 2 :
            init ();
            break;
        case 3 :
            referenz1 ();
            break;
        case 4 :
            referenz2 ();
            break;
        case 5 :
            positionieren ();
            break;
        case 6 :
            tot_zeit ();
            break;
        case 7 :
            drehzahlausgabe ();
            break;
        case 8 :
            not_stop ();
            break;
        case 9 :
            read_param ();
            break;
        case 10 :
            write_param ();
            break;
        case 13 :
            restbremse_ein ();
            break;
        case 14 :
            restbremse_aus ();
            break;
        case 17 :
            pos_drehzahl ();
            break;
        case 18 :
            nmauftrag = 7;
            break;
        case 20 :
            softwareversion();
            break;
        default:
            nmauftrag = 0;
            befehl = 0;
            nmkey = 0;
            break;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
private Funktionen */
/* EasyCODE ( 0 
KlasseNminfBS3::totmann */
/* EasyCODE F */
void KlasseNminfBS3::totmann (void)
{
    #if !defined(buerotest)
        switch ( nmkey )
        {
        case 0 :
            if ( totmannsenddi )
            {
                break;
            }
            else
            {
                if ( tnminf1.tz() )
                {
                    RTKDisableInterrupts ();
                    /* EasyCODE - */
                    sbuffer [0] = 0x20;
                    /* EasyCODE - */
                    sbuffer [1] = 0x20;
                    /* EasyCODE - */
                    writebbuf (sbuffer [0]);
                    /* EasyCODE - */
                    writebbuf (sbuffer [1]);
                    while ( NM->sbef != 0xff )
                    {
                        ;
                    }
                    NM->sbef = 2;
                    /* EasyCODE < */
                    PC104_WR = 1;       // Interrupt bei C167 auslösen
                    /* EasyCODE > */
                    nmkey = 1;
                    /* EasyCODE - */
                    versuche = 3;
                    /* EasyCODE - */
                    tnminf1.rsz();
                    /* EasyCODE - */
                    tnminf1.sz(saf->totmannzt);
                    /* EasyCODE - */
                    tnminf2.rsz();
                    /* EasyCODE - */
                    tnminf2.sz(saf->timeout_ser);
                    /* EasyCODE - */
                    RTKEnableInterrupts ();
                }
                break;
            }
        case 1 :
            if ( rec_antwort (0, 0x20, 0) == 1 )
            {
                nmkey = 0;
            }
            break;
        default:
            break;
        }
    #endif 
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::reset */
/* EasyCODE F */
void KlasseNminfBS3::reset (void)
{
    switch ( nmkey )
    {
    case 0 :
        befsend (1);
        versuche = 3;
        tnminf1.rsz();
        tnminf1.sz(saf->totmannzt);
        tnminf2.rsz();
        tnminf2.sz(saf->timeout_ser);
        nmkey = 1;
        break;
    case 1 :
        switch ( rec_antwort (1,0,0) )
        {
        case 0 :
            break;
        case 1 :
            nmkey = 0;
            if ( befehl != 8 )
            {
                befehl = 0;
                nmauftrag = 0;
                if ( !nmponrdy )
                {
                    #ifdef remote
                        parameter = 0;
                    #else 
                        parameter = 50;
                    #endif 
                    befehl = 6;
                    nmponrdy = TRUE;
                }
            }
            break;
        case 2 :
            if ( !nmponrdy )
            {
                nm_ponerr = TRUE;
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::init */
/* EasyCODE F */
void KlasseNminfBS3::init (void)
{
    switch ( nmkey )
    {
    case 0 :
        befsend (2);
        versuche = 200;
        tnminf2.rsz();
        tnminf2.sz(saf->timeout_ser);
        nmkey = 1;
        break;
    case 1 :
        switch ( rec_antwort (2,0,0) )
        {
        case 0 :
            break;
        case 1 :
            version = rbuffer [0];
            motor_steht = TRUE;
            y34.resAuftrag();   // Greifer”lung 1 aus
            y35.resAuftrag();   // Greifer”lung 2 aus
            nmkey = 0;
            nmauftrag = 1;
            break;
        case 2 :
            nm_ponerr = TRUE;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminBS3::positionieren */
/* EasyCODE F */
void KlasseNminfBS3::positionieren (void)
{
    switch ( nmkey )
    {
    case 0 :
        motor_steht = FALSE;
        y34.setAuftrag();   // Greifer”lung 1 ein
        y35.setAuftrag();   // Greifer”lung 2 ein
        befsend (5);
        versuche = 3;
        tnminf2.rsz();
        tnminf2.sz(saf->timeout_ser);
        tnminf3.rsz();
        tnminf3.sz(saf->timeout_bef);
        nmkey = 1;
        break;
    case 1 :
        if ( posart & KURZ )
        {
            if ( quick_posok )
            {
                if ( rec_antwort(5,4,0)!=1 )
                {
                    return ;
                }
            }
            else
            {
                if ( rec_antwort(5,5,0)!=1 )
                {
                    return ;
                }
            }
        }
        else
        {
            if ( posart & DREHRICHTUNG )
            {
                if ( rec_antwort(5,3,0)!=1 )
                {
                    return ;
                }
            }
            else
            {
                if ( rec_antwort(5,2,0)!=1 )
                {
                    return ;
                }
            }
        }
        befsend (0x80);
        versuche = 3;
        tnminf1.rsz();
        tnminf1.sz(saf->totmannzt);
        tnminf2.rsz();
        tnminf2.sz(saf->timeout_ser);
        tnminf3.rsz();
        tnminf3.sz(50);
        nmkey = 2;
        break;
    case 2 :
        if ( rec_antwort (0x80,7,1) != 1 )
        {
        }
        else
        {
            if ( // steht Nm auf Pos. des letzten 
                 //Positionierbefehls ?
                 (rbuffer[1] & 0x14) == 0x10 )
            {
                motor_steht = TRUE;
                y34.resAuftrag();   // Greifer”lung 1 aus
                y35.resAuftrag();   // Greifer”lung 2 aus
                quick_posok = TRUE;
                HF->enable = FALSE;
                if ( (position == saf->nadelp >> 1) && 
                     ((unsigned int)NM->aktpos == 0x8000) )
                {
                    // Ist erforderlich, falls Index von BS3 nicht erkannt wurde
                    // (ist bei Power On oder beim Umprogrammieren der
                    // Pulsauswertung möglich !!!)
                    NM->aktpos = saf->nadelp;
                }
                if ( befehl != 8 )
                {
                    befehl = 0;
                }
                nmkey = 0;
                nmauftrag = 0;
            }
            else
            {
                if ( tnminf3.tz()
                     // Timeout Befehl ?
                   )
                {
                    fehler = 4;
                    nmkey = 0;
                    nmauftrag = 8;   // Notstop
                    /* EasyCODE - */
                    /* EasyCODE_STRUCTURED_COMMENT_START */
                    /*
                    motor_steht = TRUE;
                    y34.resAuftrag();   // Greifer”lung 1 aus
                    y35.resAuftrag();   // Greifer”lung 2 aus
                    quick_posok = TRUE;
                    HF->enable = FALSE;
                    if ( (position == saf->nadelp >> 1) && 
                         ((unsigned int)NM->aktpos == 0x8000) )
                    {
                        // Ist erforderlich, falls Index von BS3 nicht erkannt wurde
                        // (ist bei Power On oder beim Umprogrammieren der
                        // Pulsauswertung möglich !!!)
                        NM->aktpos = saf->nadelp;
                    }
                    if ( befehl != 8 )
                    {
                        befehl = 0;
                    }
                    nmkey = 0;
                    nmauftrag = 0;
                    */
                    /* EasyCODE_STRUCTURED_COMMENT_END */
}
                else
                {
                    befsend (0x80);
                    versuche = 3;
                    tnminf1.rsz();
                    tnminf1.sz(saf->totmannzt);
                    tnminf2.rsz();
                    tnminf2.sz(saf->timeout_ser);
                }
            }
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::drehzahlausgabe */
/* EasyCODE F */
void KlasseNminfBS3::drehzahlausgabe (void)
{
    switch ( nmkey )
    {
    case 0 :
        motor_steht = FALSE;
        y34.setAuftrag();   // Greifer”lung 1 ein
        y35.setAuftrag();   // Greifer”lung 2 ein
        befsend (7);
        versuche = 3;
        tnminf1.rsz();
        tnminf1.sz(saf->totmannzt);
        tnminf2.rsz();
        tnminf2.sz(saf->timeout_ser);
        tnminf3.rsz();
        tnminf3.sz(saf->timeout_bef);
        nmkey = 1;
        break;
    case 1 :
        if ( drehzahl >= 0 )
        {
            if ( rec_antwort (7,0x10,0) != 1 )
            {
                return ;
            }
        }
        else
        {
            if ( rec_antwort (7,0x11,0) != 1 )
            {
                return ;
            }
        }
        if ( befehl == 18 )
        {
            nmauftrag = 5;
        }
        else
        {
            if ( befehl != 8 )
            {
                befehl = 0;
            }
            nmkey = 0;
            nmauftrag = 0;
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::not_stop */
/* EasyCODE F */
void KlasseNminfBS3::not_stop (void)
{
    switch ( nmkey )
    {
    case 0 :
        befsend (8);
        versuche = 3;
        tnminf2.rsz();
        tnminf2.sz(saf->timeout_ser);
        tnminf3.rsz();
        tnminf3.sz(saf->timeout_bef);
        nmkey = 1;
        break;
    case 1 :
        switch ( rec_antwort (8,1,0) )
        {
        case 0 :
            break;
        case 1 :
            if ( fehler != 0 )
            {
                nmkey = 3;
            }
            else
            {
                befsend (0x80);
                versuche = 3;
                tnminf1.rsz();
                tnminf1.sz(saf->totmannzt);
                tnminf2.rsz();
                tnminf2.sz(saf->timeout_ser);
                nmkey = 2;
            }
            break;
        case 2 :
            nmkey = 3;
            break;
        default:
            break;
        }
        break;
    case 2 :
        switch ( rec_antwort (0x80,7,1) )
        {
        case 0 :
            break;
        case 1 :
            if ( !(rbuffer[1] & 4)  // dreht Nm ?
               )
            {
                motor_steht = TRUE;
                y34.resAuftrag();   // Greifer”lung 1 aus
                y35.resAuftrag();   // Greifer”lung 2 aus
                HF->enable = FALSE;
                befehl = 0;
                nmkey = 0;
                nmauftrag = 0;
            }
            else
            {
                if ( tnminf3.tz()
                     // Timeout Befehl ?
                   )
                {
                    fehler = 4;
                    nmkey = 3;
                }
                else
                {
                    befsend (0x80);
                    versuche = 3;
                    tnminf1.rsz();
                    tnminf1.sz(saf->totmannzt);
                    tnminf2.rsz();
                    tnminf2.sz(saf->timeout_ser);
                }
            }
            break;
        case 2 :
            nmkey = 3;
            break;
        default:
            break;
        }
        break;
    case 3 :
        befehl = 0;
        nmkey = 0;
        nmauftrag = 0;
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::referenz1 */
/* EasyCODE F */
void KlasseNminfBS3::referenz1 (void)
{
    parameternr = 0x16;
    nmauftrag = 10;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::referenz2 */
/* EasyCODE F */
void KlasseNminfBS3::referenz2 (void)
{
    parameternr = 0x17;
    nmauftrag = 10;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::tot_zeit */
/* EasyCODE F */
void KlasseNminfBS3::tot_zeit (void)
{
    parameternr = 0x1c;
    nmauftrag = 10;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::restbremse_ein */
/* EasyCODE F */
void KlasseNminfBS3::restbremse_ein (void)
{
    parameternr = 7;
    nmauftrag = 10;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::restbremse_aus */
/* EasyCODE F */
void KlasseNminfBS3::restbremse_aus (void)
{
    parameternr = 7;
    parameter = 0;
    nmauftrag = 10;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::pos_drehzahl */
/* EasyCODE F */
void KlasseNminfBS3::pos_drehzahl (void)
{
    parameternr = 16;
    nmauftrag = 10;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::read_param */
/* EasyCODE F */
void KlasseNminfBS3::read_param (void)
{
    switch ( nmkey )
    {
    case 0 :
        befsend (9);
        versuche = 3;
        tnminf1.rsz();
        tnminf1.sz(saf->totmannzt);
        tnminf2.rsz();
        tnminf2.sz(saf->timeout_ser);
        nmkey = 1;
        break;
    case 1 :
        if ( rec_antwort (9,8,1) == 1 )
        {
            parameter = rbuffer [1];
            nmkey = 0;
            nmauftrag = 0;
            if ( befehl != 8 )
            {
                befehl = 0;
            }
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::write_param */
/* EasyCODE F */
void KlasseNminfBS3::write_param (void)
{
    switch ( nmkey )
    {
    case 0 :
        befsend (10);
        versuche = 3;
        tnminf1.rsz();
        tnminf1.sz(saf->totmannzt);
        tnminf2.rsz();
        tnminf2.sz(saf->timeout_ser);
        nmkey = 1;
        break;
    case 1 :
        if ( rec_antwort (10,9,0) == 1 )
        {
            nmkey = 0;
            nmauftrag = 0;
            if ( befehl != 8 )
            {
                befehl = 0;
            }
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::softwareversion */
/* EasyCODE F */
void KlasseNminfBS3::softwareversion (void)
{
    unsigned int u;
    switch ( nmkey )
    {
    case 0 :
        befsend (20);
        versuche = 3;
        tnminf1.rsz();
        tnminf1.sz(saf->totmannzt);
        tnminf2.rsz();
        tnminf2.sz(saf->timeout_ser);
        nmkey = 1;
        break;
    case 1 :
        if ( rec_antwort (20, 20, 8) == 1 )
        {
            for ( u=0; u<8; u++ )
            {
                if ( u <= 1 )
                {
                    softvers[u] = rbuffer[u+1];
                }
                else
                {
                    if ( u==2 )
                    {
                        softvers[2] = '-';
                        softvers[3] = rbuffer[3];
                    }
                    else
                    {
                        softvers[u+1] = rbuffer[u+1];
                    }
                }
            }
            softvers[9] = 0;
            nmkey = 0;
            nmauftrag = 0;
            if ( befehl != 8 )
            {
                befehl = 0;
            }
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::befsend */
/* EasyCODE F */
/***************************** befsend ****************************************/
//
// Das Up setzt den übergebenen Befehlscode in eine Befehlssequenz um,
// die über die serielle Schnittstelle an den Quick-Hauptantrieb geschickt
// wird. Der Befehlscode umfaît alle für nmpb.befehl beschriebenen Befehle.
// Zusätzlich werden noch einige Befehle definiert, die nur im Nähmotor-
// Interface benutzt werden:
//   0x80 ... Abfrage Maschinenzustand
//
// Eingangsparameter:  bef     ... der zu sendende Befehl

void KlasseNminfBS3::befsend (unsigned char bef)
{
    unsigned char n; // Anzahl der zu sendenden Bytes
    unsigned char i;
    switch ( bef )
    {
    case 0
         // Totmann-Signal
    :
        sbuffer [0] = 0x20;
        sbuffer [1] = 0x20;
        n = 2;
        break;
    case 1
         // Reset
    :
        sbuffer [0] = 0;
        sbuffer [1] = 0;
        n = 2;
        break;
    case 2
         // Init
    :
        sbuffer [0] = 0;
        n = 1;
        break;
    case 3 :
        /* EasyCODE < */
        // Referenz 1
        // kein eigenständiger Befehl, zum Programmieren
        // der Referenzpos. wird der Befehl
        // Parameter schreiben benutzt.
        /* EasyCODE > */
        break;
    case 4 :
        /* EasyCODE < */
        // Referenz 2
        // kein eigenständiger Befehl, zum Programmieren
        // der Referenzpos. wird der Befehl
        // Parameter schreiben benutzt.
        /* EasyCODE > */
        break;
    case 5 :
        /* EasyCODE < */
        // Positionieren
        /* EasyCODE > */
        if ( posart & KURZ )
        {
            if ( quick_posok )
            {
                sbuffer [0] = 4;
            }
            else
            {
                sbuffer [0] = 5;
                sbuffer [1] = 5;
                n = 2;
                break;
            }
        }
        else
        {
            if ( posart & DREHRICHTUNG )
            {
                sbuffer [0] = 3;
            }
            else
            {
                sbuffer [0] = 2;
            }
        }
        sbuffer [1] = position;
        /* EasyCODE - */
        build_checksum (2);
        /* EasyCODE - */
        n = 3;
        break;
    case 6 :
        /* EasyCODE < */
        // Totmann-Zeit
        // kein eigenständiger Befehl, zum Programmieren
        // der Totmannzeit wird der Befehl
        // Parameter schreiben benutzt.
        /* EasyCODE > */
        break;
    case 7
         // Drehzahlvorgabe
    :
        if ( drehzahl >= 0 )
        {
            sbuffer [0] = 0x10;
            sbuffer [1] = drehzahl;
            sbuffer [2] = (drehzahl>>8);
        }
        else
        {
            sbuffer [0] = 0x11;
            sbuffer [1] = -drehzahl;
            sbuffer [2] = (-drehzahl>>8);
        }
        build_checksum (3);
        /* EasyCODE - */
        n = 4;
        break;
    case 8
         // Notstop
    :
        sbuffer [0] = 1;
        /* EasyCODE - */
        sbuffer [1] = 1;
        /* EasyCODE - */
        n = 2;
        break;
    case 9
         // Parameter lesen
    :
        sbuffer [0] = 8;
        /* EasyCODE - */
        sbuffer [1] = parameternr;
        /* EasyCODE - */
        build_checksum (2);
        /* EasyCODE - */
        n = 3;
        break;
    case 10
         // Parameter schreiben
    :
        sbuffer [0] = 9;
        /* EasyCODE - */
        sbuffer [1] = parameternr;
        /* EasyCODE - */
        sbuffer [2] = parameter;
        /* EasyCODE - */
        build_checksum (3);
        /* EasyCODE - */
        n = 4;
        break;
    case 11 :
        /* EasyCODE < */
        // Reset Stichzähler (nicht realisiert)
        /* EasyCODE > */
        break;
    case 12 :
        /* EasyCODE < */
        // StopX (nicht realisiert)
        /* EasyCODE > */
        break;
    case 13 :
        /* EasyCODE < */
        // Restbremse ein
        // kein eigenständiger Befehl, zum Programmieren
        // der Restbremsung wird der Befehl
        // Parameter schreiben benutzt.
        /* EasyCODE > */
        break;
    case 14 :
        /* EasyCODE < */
        // Restbremse aus
        // kein eigenständiger Befehl, zum Programmieren
        // der Restbremsung wird der Befehl
        // Parameter schreiben benutzt.
        /* EasyCODE > */
        break;
    case 15 :
        /* EasyCODE < */
        // Positionsabfrage (nicht realisiert)
        /* EasyCODE > */
        break;
    case 16 :
        /* EasyCODE < */
        // Drehzahlabfrage (nicht realisiert)
        /* EasyCODE > */
        break;
    case 17 :
        /* EasyCODE < */
        // maximale Drehzahl
        // kein eigenständiger Befehl, zum Programmieren
        // der maximalen Drehzahl wird der Befehl
        // Parameter schreiben benutzt.
        /* EasyCODE > */
        break;
    case 20 :
        sbuffer [0] = 0x14;
        sbuffer [1] = 0x14;
        n = 2;
        break;
    case 0x80
         // Maschinenzustand abfragen
    :
        sbuffer [0] = 7;
        sbuffer [1] = 0;
        sbuffer [2] = 7;
        n = 3;
        break;
    default:
        return ;
    }
    for ( i=0; i<n; i++ )
    {
        writebbuf (sbuffer [i]);
    }
    while ( NM->sbef != 0xff )
    {
        ;
    }
    /* EasyCODE < */
    NM->sbef = 2;
    PC104_WR = 1;       // Interrupt bei C167 auslösen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::rec_antwort */
/* EasyCODE F */
/***************************** rec_antwort ************************************/
//
// Das Up empfaengt eine Antwort vom Quick-Hauptantrieb und schreibt sie in
// den Empfangsbuffer (rbuffer). Es wird geprueft ob die empfangene Antwort
// mit der erwarteten uebereinstimmt. Gibt es beim Empfangen einen Fehler
// wird der Befehl noch 2 mal wiederholt.
//
// Eingangsparameter:
//      bef    ........   der schon gesendete Befehl, es gelten die gleichen
//                        Befehlscodes wie bei befsend
//      antwort .......   das Befehlsbyte der erwarteten Antwort
//      daten  ........   Anzahl der Datenbytes der Antwort
//
// Rueckmeldung:
//      0   ...   nicht fertig
//      1   ...   fertig
//      2   ...   fertig mit Fehler, fehler ist gesetzt

unsigned char KlasseNminfBS3::rec_antwort (unsigned char bef, unsigned char antwort, unsigned char daten)
{
    unsigned char rb, fehlermerker;
    switch ( rb = recbytes () )
    {
    case 0 :
        if ( tnminf2.tz() )
        {
            // Timeout
            fehlermerker = 2;
            break;
        }
        else
        {
            return 0;
        }
    case 1 :
        if ( daten == 0 )
        {
            readbuf ();
            if ( (bef == 2) || (rbuffer [0] == antwort) )
            {
                return 1;
            }
        }
        else
        {
            if ( tnminf2.tz() )
            {
                readbuf ();
            }
            else
            {
                return 0;
            }
        }
        if ( rbuffer [0] >= 0x30 )
        {
            if ( rbuffer [0] >= 0x40 )
            {
                versuche = 1;
            }
            /* EasyCODE < */
            fehlermerker = rbuffer [0]; // Fehlermeldung Quick
            /* EasyCODE > */
        }
        else
        {
            /* EasyCODE < */
            fehlermerker = 2; // Timeout
            /* EasyCODE > */
        }
        break;
    case 0xff :
        if ( fehler == 0 )
        {
            /* EasyCODE < */
            fehler = 1;  // _bertragungsfehler
            /* EasyCODE > */
        }
        /* EasyCODE < */
        nmkey = 0;
        nmauftrag = 8;  // Notstop
        /* EasyCODE > */
        return 2;
    default:
        if ( rb == daten+2 )
        {
            readbuf();
            if ( rbuffer[0] != antwort )
            {
                // Timeout
                fehlermerker = 2;
                break;
            }
            else
            {
                if ( datacheck(daten+2) )
                {
                    return 1;
                }
                else
                {
                    // Checksummenfehler
                    fehlermerker = 3;
                    break;
                }
            }
        }
        else
        {
            if ( tnminf2.tz() )
            {
                // Timeout
                fehlermerker = 2;
                clear_rbuf ();
                break;
            }
            else
            {
                return 0;
            }
        }
    }
    if ( (--versuche) == 0 )
    {
        if ( fehler == 0 )
        {
            fehler = fehlermerker;
        }
        /* EasyCODE < */
        nmkey = 0;
        nmauftrag = 8; // Notstop
        /* EasyCODE > */
        return 2;
    }
    else
    {
        // Befehl noch einmal schicken
        befsend (bef); 
        tnminf2.rsz();
        tnminf2.sz(saf->timeout_ser);
        return 0;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::clear_rbuf */
/* EasyCODE F */
/***************************** clear_rbuf *************************************/
// Das Up löscht den Empfangsringpuffer auf der Hauptantriebsschnittstelle der BS3.

void KlasseNminfBS3::clear_rbuf (void)
{
    NM->rbufrd = NM->rbufwr;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::recbytes */
/* EasyCODE F */
/**************************** recbytes ****************************************/
// Das Up meldet wieviele Datenbytes ueber die serielle Schnittstelle vom
// Quick-Automatenmotor empfangen wurden. Uebertragungsfehler werden gemeldet.
//
// Rückmeldung:
//    0 - 4 ... Anzahl empfangener Datenbytes
//    0xff  ... Uebertragungsfehler

unsigned char KlasseNminfBS3::recbytes (void)
{
    if ( NM->rbufrd == NM->rbufwr )
    {
        /* EasyCODE < */
        // kein Byte empfangen
        /* EasyCODE > */
        return 0;
    }
    else
    {
        if ( NM->rbufst != 0 )
        {
            // Uebertragungsfehler
            return 0xff;
        }
        else
        {
            if ( NM->rbufrd < NM->rbufwr )
            {
                return (NM->rbufwr - NM->rbufrd);
            }
            else
            {
                return (16 - NM->rbufrd + NM->rbufwr);
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::readbuf */
/* EasyCODE F */
/***************************** readbuf ****************************************/
// Das Up liest alle empfangenen Datenbytes in den internen Puffer. Es wird
// nicht geprüft, ob ein _bertragungsfehler vorliegt.

void KlasseNminfBS3::readbuf (void)
{
    unsigned char i;
    for ( i=0; NM->rbufrd != NM->rbufwr; i++ )
    {
        rbuffer [i] = NM->rbuf[NM->rbufrd];
        if ( ++NM->rbufrd == 16 )
        {
            NM->rbufrd = 0;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::writebbuf */
/* EasyCODE F */
/***************************** writebbuf **************************************/
// Das Up schreibt ein Byte in den Senderingpuffer.

void KlasseNminfBS3::writebbuf (unsigned char wert)
{
    NM->sbuf[NM->sbufwr] = wert;
    if ( ++NM->sbufwr == 16 )
    {
        NM->sbufwr = 0;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::datacheck */
/* EasyCODE F */
/***************************** datacheck **************************************/
// Das Up bildet die Checksumme der übermittelten Daten und vergleicht sie
// mit der vom Quick-Hauptantrieb gesendeten Checksumme.
//
// Eingangsparameter:
//      Anzahl der Datenbytes inkl. dem Checksummenbyte von Quick;
//      Die Anzahl kann nur 3 oder 4 sein, da Antworten von Quick entweder
//      keine Daten (dann auch keine Checksumme) oder 1 bzw. 2 Datenbytes
//      plus Befehlsbyte und Ckecksummenbyte enthalten.
// Rückmeldung:
//      TRUE  ..... Daten ok
//      FALSE ... Checksumme stimmt nicht mit der Quick-Checksumme überein.

unsigned char KlasseNminfBS3::datacheck (unsigned char byteanz)
{
    unsigned char i;
    /* EasyCODE_STRUCTURED_COMMENT_START */
    /*
    switch ( byteanz )
    {
    case 3 :
        if ( (rbuffer[0]^rbuffer[1]) == rbuffer[2] )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    case 4 :
        if ( (rbuffer[0]^rbuffer[1]^rbuffer[2]) == rbuffer[3] )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    default:
        return FALSE;
    }
    */
    /* EasyCODE_STRUCTURED_COMMENT_END */
    for ( i = 1; i < byteanz-1; i++ )
    {
        rbuffer[0] = (rbuffer[0]^rbuffer[i]);
    }
    if ( rbuffer[0] == rbuffer[byteanz-1] )
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
KlasseNminfBS3::build_checksum */
/* EasyCODE F */
/************************ build_checksum **************************************/
// Das Up bildet die Checksumme der übermittelten Anzahl Bytes in sbuffer.
// Die ermittelte Checksumme wird anschliessend in sbuffer hinter die Daten
// geschrieben. Die Anzahl der Bytes muss > 1 sein.

void KlasseNminfBS3::build_checksum (unsigned char byteanz)
{
    unsigned char i;
    
    sbuffer[byteanz] = sbuffer[0];
    for ( i=1; i<byteanz; i++ )
    {
        sbuffer [byteanz] ^= sbuffer [i];
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
public Funktionen */
/* EasyCODE ( 0 
KlasseNminfBS3::nadel_ot */
/* EasyCODE F */
// Das Up, prüft ob die Nadel in OT steht.
//
// Rückmeldung:    TRUE  ..... Nadel steht in OT
//                 FALSE ..... Nadel steht nicht in OT

unsigned char KlasseNminfBS3::nadel_ot (void)
{
    #if defined(buerotest)
        return TRUE;
    #else 
        do{
            pos1 = NM->aktpos;
        }while ( pos1 != NM->aktpos );
        if ( !motor_steht || ((unsigned int)pos1==0x8000) || ((unsigned int)pos1==0x8001) )
        {
            return FALSE;
        }
        else
        {
            if ( pos1 < 0 )
            {
                pos1 += PULSE_PRO_UMDR;
            }
            if ( pos1 >= saf->nadelp )
            {
                if ( (pos1 - saf->nadelp <= saf->ot_fenster) ||
                     (saf->nadelp - saf->ot_fenster + PULSE_PRO_UMDR <= pos1) )
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
            else
            {
                if ( (saf->nadelp - pos1 <= saf->ot_fenster) ||
                     (saf->nadelp + saf->ot_fenster - PULSE_PRO_UMDR >= pos1) )
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
        }
    #endif 
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::nadel_bereich */
/* EasyCODE F */
// Das Up prüft, ob die Nadel in einem Bereich (ot_bereich) um OT steht.
//
// Rückmeldung:    TRUE  ..... Nadel steht im Breich
//                 FALSE ..... Nadel steht nicht im Breich

unsigned char KlasseNminfBS3::nadel_bereich (int bereich)
{
    #if defined(buerotest)
        return TRUE;
    #else 
        do{
            pos2 = NM->aktpos;
        }while ( pos2 != NM->aktpos );
        if ( !motor_steht || ((unsigned int)pos2==0x8000) || ((unsigned int)pos2==0x8001) )
        {
            return FALSE;
        }
        else
        {
            if ( pos2 < 0 )
            {
                pos2 += PULSE_PRO_UMDR;
            }
            if ( pos2 >= saf->nadelp )
            {
                if ( (pos2 - saf->nadelp <= bereich) ||
                     (saf->nadelp - bereich + PULSE_PRO_UMDR <= pos2) )
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
            else
            {
                if ( (saf->nadelp - pos2 <= bereich) ||
                     (saf->nadelp + bereich - PULSE_PRO_UMDR >= pos2) )
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
        }
    #endif 
}

/* EasyCODE ) */
/* EasyCODE_STRUCTURED_COMMENT_START */
/*
EasyCODE_COMMENT_START EasyCODE ( 0 
KlasseNminfBS3::taktnadel_ber EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
// Das Up prüft, ob die Nadel in einem Bereich (takt_bereich) um OT steht.
//
// Rückmeldung:    TRUE  ..... Nadel steht im Breich
//                 FALSE ..... Nadel steht nicht im Breich

unsigned char KlasseNminfBS3::taktnadel_ber (void)
{
    do{
        pos3 = NM->aktpos;
    }while ( pos3 != NM->aktpos );
    if ( !motor_steht || ((unsigned int)pos3==0x8000) || ((unsigned int)pos3==0x8001) )
    {
        return FALSE;
    }
    else
    {
        if ( pos3 < 0 )
        {
            pos3 += PULSE_PRO_UMDR;
        }
        if ( pos3 >= saf->nadelp )
        {
            if ( (pos3 - saf->nadelp <= saf->takt_bereich) ||
                 (saf->nadelp - saf->takt_bereich + PULSE_PRO_UMDR <= pos3) )
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            if ( (saf->nadelp - pos3 <= saf->takt_bereich) ||
                 (saf->nadelp + saf->takt_bereich - PULSE_PRO_UMDR >= pos3) )
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
*/
/* EasyCODE_STRUCTURED_COMMENT_END */
/* EasyCODE ( 0 
KlasseNminfBS3::nadel_pos */
/* EasyCODE F */
// Das Up meldet die aktuelle Nadelposition zurück.
// Die Position wird in 480 Pulsen/Umdr. gemeldet.
 
unsigned int KlasseNminfBS3::nadel_pos (void)
{
    do{
        pos4 = NM->aktpos;
    }while ( pos4 != NM->aktpos );
    if ( pos4 < 0 )
    {
        pos4 += PULSE_PRO_UMDR;
    }
    return pos4;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::get_drehzahl */
/* EasyCODE F */
// Das Up meldet die aktuelle Drehzahl in upm zurück.

unsigned int KlasseNminfBS3::get_drehzahl (void)
{
    do{
        drehz = NM->count;
    }while ( drehz != NM->count );
    return (12 * drehz);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::gsw_fastout */
/* EasyCODE F */
// Up zur schnellstmöglichen Drehzahlausgabe.

void KlasseNminfBS3::gsw_fastout (void)
{
    if ( nmkey == 0 )
    {
        NM->sbuf[NM->sbufwr] = 0x10;
        if ( ++NM->sbufwr == 16 )
        {
            NM->sbufwr = 0;
        }
        NM->sbuf[NM->sbufwr] = drehzahl;
        if ( ++NM->sbufwr == 16 )
        {
            NM->sbufwr = 0;
        }
        NM->sbuf[NM->sbufwr] = drehzahl >> 8;
        if ( ++NM->sbufwr == 16 )
        {
            NM->sbufwr = 0;
        }
        NM->sbuf[NM->sbufwr] = 0x10^(unsigned char)drehzahl^(unsigned char)(drehzahl>>8);
        if ( ++NM->sbufwr == 16 )
        {
            NM->sbufwr = 0;
        }
        while ( NM->sbef != 0xff )
        {
            ;
        }
        NM->sbef = 2;
        PC104_WR = 1;       // Interrupt bei C167 auslösen
        versuche = 3;
        tnminf1.rsz();
        tnminf1.sz(saf->totmannzt);
        tnminf2.rsz();
        tnminf2.sz(saf->timeout_ser);
        tnminf3.rsz();
        tnminf3.sz(saf->timeout_bef);
        nmkey = 1;
        nmauftrag = 7;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::tot_fastout */
/* EasyCODE F */
// Up zur schnellstmöglichen Ausgabe des Totmann-Kommandos.

void KlasseNminfBS3::tot_fastout (void)
{
    unsigned char writeoff;
    if ( (nmkey==0) && !totmannsenddi )
    {
        NM->sbuf[NM->sbufwr] = 0x20;
        if ( ++NM->sbufwr == 16 )
        {
            NM->sbufwr = 0;
        }
        NM->sbuf[NM->sbufwr] = 0x20;
        if ( ++NM->sbufwr == 16 )
        {
            NM->sbufwr = 0;
        }
        while ( NM->sbef != 0xff )
        {
            ;
        }
        NM->sbef = 2;
        PC104_WR = 1;   // Interrupt bei C167 auslösen
        versuche = 3;   // (war früher 1)
        tnminf1.rsz();
        tnminf1.sz(saf->totmannzt);
        tnminf2.rsz();
        tnminf2.sz(saf->timeout_ser);
        nmkey = 1;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::winit */
/* EasyCODE F */
void KlasseNminfBS3::winit (void)
{
    /* EasyCODE < */
    int i;
    
    // Pulsauswertung
    NM->stbpaw = STBPAW;      // Steuerbyte für Pulsauswertung
    NM->stbn8 = saf->md.fw_counter;   // Anz. Umdr. pro Fw-Triggerpuls
    // serielle Schnittstelle
    NM->sermod = SERMOD;      // Betriebsart serielle Schnittstelle
    // Drehzahlmessung
    NM->messzt = MESSZT;      // Meîzeit
    // Warten bis Hauptantriebsinterface bereit ist
    /* EasyCODE > */
    while ( (NM->pbef != 0xff) || (NM->sbef != 0xff) || (NM->dbef != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    // _bernahme der Initialisierungswerte veranlassen
    NM->pbef = 0;
    NM->sbef = 0;
    NM->dbef = 0;
    PC104_WR = 1;             // Interrupt bei C167 auslösen
    // Sind die Werte übernommen ?
    /* EasyCODE > */
    while ( (NM->pbef != 0xff) || (NM->sbef != 0xff) || (NM->dbef != 0xff) )
    {
        ;
    }
    /* EasyCODE < */
    // eventuell anstehende Interrupts rücksetzen
    outportb (INTCLR, REF1_SETCLR | REF2_SETCLR | DPR_SETCLR);
    outportb (INTCLR, REF1_RESCLR | REF2_RESCLR | DPR_RESCLR);
    
    nminit_ok = TRUE;
    nmauftrag = 2;                        // Init
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::kinit */
/* EasyCODE F */
void KlasseNminfBS3::kinit (void)
{
    saf->ot_fenster = 16;        // in Inkr. um Nadel OT
    saf->ot_bereich = 67;        // in Inkr. um Nadel OT
    saf->takt_bereich = 75;      // in Inkr. um Nadel OT
    saf->hf_bereich = 43;        // in Inkr. um Nadel OT
    saf->timeout_ser = 2;        // Timeout fuer ser. Schnittstelle 20ms
    saf->timeout_bef = 200;      // Timeout fuer Befehl 2s
    saf->totmannzt = 48;         // Totmannzeit für nminf 480ms
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNminfBS3::pawini */
/* EasyCODE F */
void KlasseNminfBS3::pawini (void)
{
    while ( NM->pbef != 0xff )
    {
        ;
    }
    /* EasyCODE < */
    NM->pbef = 0x01;    // Pulsauswertung stoppen
    PC104_WR = 1;       // Interrupt bei C167 auslösen
    /* EasyCODE > */
    while ( NM->pbef != 0xff )
    {
        ;
    }
    /* EasyCODE < */
    NM->stbn8 = saf->md.fw_counter;   // Anz. Umdr. pro Fw-Triggerpuls
    NM->pbef = 0;                     // Pulsauswertung neu starten
    PC104_WR = 1;                     // Interrupt bei C167 auslösen
    /* EasyCODE > */
    while ( NM->pbef != 0xff )
    {
        ;
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Quick-Parameter */
/* EasyCODE_STRUCTURED_COMMENT_START */
/*
unsigned char quick_param [QP_ANZAHL] [2] = {
{31, 0},     // 0,  Maschinenkennung ruecksetzen
{0, 12},     // 1,  P-Anteil Drehzahlregler
{1, 30},     // 2,  I-Anteil Drehzahlregler
{2, 20},     // 3,  P-Anteil Lageregler
{3, 10},     // 4,  D-Anteil Lageregler
{4, 40},     // 5,  Zeit fuer Lageregelung
{5, 25},     // 6,  P-Anteil fuer Restbremse
{7, 0},      // 7,  Staerke der Restbremse
{16, 18},    // 8,  Positionierdrehzahl
{17, 15},    // 9,  Beschleunigungsrampe
{18, 15},    // 10, Verzoegerungsrampe
//   (22, xx), 1. Referenzposition (ist an Maschine waehlbar)
//   (23, xx), 2. Referenzposition (ist an Maschine waehlbar)
{30, 0},     // 11, Drehrichtung
//   (28, xx), Laenge der Totmannzeit (Sonderbehandlung wegen Debugging)
{35, 15},    // 12, maximaler Motorstrom
{36, 1},     // 13, Positionierverfahren (1-zeitopt.,  2-wegopt.)
{37, 22},    // 14, Vorsteuerung Beschleunigen
{38, 22},    // 15, Vorsteuerung Bremsen
{31, QP_KENNUNG}}; // 16, Maschinenkennung schreiben
EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
*/
/* EasyCODE_STRUCTURED_COMMENT_END */
unsigned char quick_paramnr [QP_ANZAHL] = {
31,    // 0,  Maschinenkennung ruecksetzen
0,     // 1,  P-Anteil Drehzahlregler
1,     // 2,  I-Anteil Drehzahlregler
2,     // 3,  P-Anteil Lageregler
3,     // 4,  D-Anteil Lageregler
4,     // 5,  Zeit fuer Lageregelung
5,     // 6,  P-Anteil fuer Restbremse
7,     // 7,  Staerke der Restbremse
16,    // 8,  Positionierdrehzahl
17,    // 9,  Beschleunigungsrampe
18,    // 10, Verzoegerungsrampe
//   (22, xx), 1. Referenzposition (ist an Maschine waehlbar)
//   (23, xx), 2. Referenzposition (ist an Maschine waehlbar)
30,    // 11, Drehrichtung
//   (28, xx), Laenge der Totmannzeit (Sonderbehandlung wegen Debugging)
35,    // 12, maximaler Motorstrom
36,    // 13, Positionierverfahren (1-zeitopt.,  2-wegopt.)
37,    // 14, Vorsteuerung Beschleunigen
38,    // 15, Vorsteuerung Bremsen
39,    // 16, Filter Drehzahlregler
31,};  // 17, Maschinenkennung schreiben
/* EasyCODE ) */
/* EasyCODE ) */
