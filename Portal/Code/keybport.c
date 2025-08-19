/* EasyCODE V8 */
/* EasyCODE ( 0 
keybport */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
/* Datei: keybport.c        Klasse: 3590 Portal        Ersteller: Halfmann

Kurzbeschreibung:
=================
In dieser Datei befindet sich die zyklische Tasks keyboard 
(Tastatur des Maschinenbedienfeldes).
Das Maschinenbedienfeld wird immer ueber die serielle Schnittstelle com2
angeschlossen.

keyboard:
=========
Der Tastaturteil wertet die empfangenen Zeichen aus. Falls ein Tastatur-
fehler auftritt, wird das Fehlerbyte keyb_err gesetzt. Dabei wird der Fehler
wie folgt gewichtet:

keyb_err = 0                 kein Fehler.
0 < keyb_err < 11            schwere Fehler. 
keyb_err > 10                leichter Fehler (nur Warnung).  

keyb_err =  1      HW Fehler
keyb_err = 11      Falsches Touch Zeichen empfangen. (nur Warnung)
keyb_err = 12      Falsche Verzweigung, interner Fehler.
keyb_err = 13      Fehler gemeldet direkt vom Touchscreen.
Fehler Nummer siehe Variable  bdf_err.
z.B.: bdf_err = 01      Timeout,  
bdf_err = 02      Parameterfehler,  
bdf_err = 14      unbekanntes Kommando,  


Die SW Module, welche den keyb_err auswerten, muessen 
die Attribute keyb_err und bdf_err nach dem Lesen wieder auf Null setzen.                             
Dies ist moeglich ueber die Methoden keyb_err_res und bdf_err_res .
Ein Keyboard Error fuehrt nicht zur Blockade innerhalb der Keyboardtask.

Mittels der Methode taste hat die Anwendersoftware Lesezugriff
auf das Attribut keyb_taste.
Es wird 0xff (falls keine Taste bzw. eine ungueltige Taste gedrueckt),
bzw. der Tastencode (falls nur eine gueltige Taste gedrueckt) zurueckgemeldet.

Für alle weiteren Meldungen des Bedienfeldeldes wie z.B. bed_version,...
gibt es jeweils Methoden zum lesen und setzen des dazugehoerigen gueltig Flags.
(siehe keyb8310.h)
*/

#include "portal.h"

// Globale Daten                                               
KlasseKeyb  keyb (tdini [KeybIndex]);   // Objektdefinition
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKeyb::keybtask (RTK Task) */
/* EasyCODE ( 0 
KlasseKeyb::keybtask */
/* EasyCODE F */
// Task keyboard
//  Es wird von der Empfangs-Mailbox ein Zeichen eingelesen. Falls ein 
//  Empfangsfehler aufgetreten ist, wird die Fehlerbehandlung
//  ausgeloest. Sonst wird das Zeichen ausgewertet.

void KlasseKeyb::keybtask(void)
{
    if ( KartenmodusFlag )
    {
        /* EasyCODE ( 0 
        Kommunikation
        SD-Karte */
        while ( (sdbuf.status()!=1) &&
                (sdbuf.status()!=2) )
        {
            if ( RTKGetCond (ReceiveBuffer[COM2],&recdata.word) )
            {
                if ( recdata.byte.high & HARD_ERROR )
                {
                    // Fehler aufgetreten
                    keyb_err=1; // HW Fehler
                    keyb_zeichenkey=0;
                    KartenmodusFlag = FALSE;
                    break;
                }
                else
                {
                    SD_zeichen_auswerten();
                }
            }
            else
            {
                break;
            }
        }
        /* EasyCODE ) */
    }
    else
    {
        /* EasyCODE ( 0 
        Kommunikation
        Bedienfeld */
        while ( RTKGetCond (ReceiveBuffer[COM2],&recdata.word) )
        {
            if ( recdata.byte.high & HARD_ERROR )
            {
                if ( koor.bdfbreakrdy )
                {
                    // Fehler aufgetreten
                    keyb_err=1; // HW Fehler
                }
                keyb_zeichenkey=0;
                keyb_taste = 0xff;
            }
            else
            {
                if ( koor.bdfbaudrdy )
                {
                    // Zeichen ohne Fehler empfangen
                    zeichen_auswerten();
                    if ( KartenmodusFlag )
                    {
                        break;
                    }
                }
                else
                {
                    // Das Touch sendet nach dem 
                    // Umschalten der Baudrate noch
                    // irgend einen Text. Dieser Text 
                    // mu+ gelesen werden, er wird jedoch 
                    // nicht gewertet.
                }
            }
        }
        if ( auswertung )
        {
            taste_auswerten();
        }
        /* EasyCODE ) */
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKeyb::zeichen_auswerten */
/* EasyCODE F */
// Funktion bed_zeichen_auswerten
//  Es wird das erste empfangene Zeichen interpretiert und entsprechend
//  verzweigt, bis die dazugehoerigen empfangenen Zeichen abgearbeitet sind.

void KlasseKeyb::zeichen_auswerten(void)
{
    switch ( keyb_zeichenkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        Erstes Touch Meldungszeichen auswerten.
        Aufgrund des empfangenen Zeichens verzweigen. */
        keyb_count=0;
        switch ( recdata.byte.low )
        {
        case 'R' :
            keyb_zeichenkey=1; // Eine Touch Taste wurde gedrueckt.
            break;
        case 'r' :
            keyb_zeichenkey=2; // Die gedrueckte Taste wurde losgelassen
            break;
        case 'V' :
            // Touch SW Versionsnummer
            bed_version[0]='V';
            keyb_count = 1;
            keyb_zeichenkey=3;
            break;
        case 'i' :
            keyb_zeichenkey=4;  // Programmierbare Kennung
            break;
        case 'A' :
            keyb_zeichenkey=5;  //aktuelle Seitennummer
            break;
        case 'M' :
            keyb_zeichenkey=6;  //Freier Piktospeicher
            break;
        case 'N' :
            keyb_zeichenkey=11;  // IP-Adresse
            break;
        case 'p' :
            keyb_zeichenkey=7;  //aktuelle Anzahl der Piktogramme
            break;
        case 't' :
            keyb_zeichenkey=8;  // Touchfelder Anzahl
            break;
        case 'E' :
            keyb_zeichenkey=9;  // Touch Error
            break;
        case 'S' // Beim Pikto Download sendet das Touch nach jedem empfangenen Record 
                 // ein S als Info zur+ck. Dieses S kann ignoriert werden.
        :
            break;
        case ACK :
            bed_ack = TRUE;
            break;
        case 0x0f :
            keyb_zeichenkey=10;  // Umschaltung in Kartenmodus
            break;
        default:
            if ( koor.bdfbreakrdy )
            {
                // Es wurde ein unerwartetes oder ein unbekanntes Zeichen empfangen.
                keyb_err=11; // Falsches Touch Zeichen empfangen. (nur Warnung)
            }
            break;
        }
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        Taste gedrueckt. Die 3 Ziffern sammeln und zu einem Byte zusammenfassen.
        Tastencode ermitteln. Der Tastencode besteht aus 3 dezimale Ziffern,
        welche zusammen ein Byte ergeben. */
        asc_zeichen[keyb_count] = recdata.byte.low;
        if ( keyb_count == 2 )
        {
            // Die drei Ziffern in einem Byte zusammenfassen
            keyb_taste_temp = atoi(asc_zeichen);
            auswertung=TRUE;
            keyb_zeichenkey = 0;
        }
        else
        {
            keyb_count++;
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        Taste losgelassen. Die 3 Ziffern sammeln.
        Tastencode ermitteln. Der Tastencode besteht aus 3 dezimale Ziffern,
        welche zusammen ein Byte ergeben.
        Beim Taste Loslassen werden jedoch die Ziffern nicht ausgewertet sondern nur gelesen. */
        asc_zeichen[keyb_count] = recdata.byte.low;
        if ( keyb_count == 2 )
        {
            keyb_taste_temp = 0;
            auswertung=TRUE;
            keyb_zeichenkey = 0;
        }
        else
        {
            keyb_count++;
        }
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        Touch SW Versionsnummer abspeichern. */
        bed_version[keyb_count]=recdata.byte.low;
        if ( keyb_count==7 )
        {
            bed_version_gueltig=TRUE;
            keyb_zeichenkey=0;
        }
        else
        {
            keyb_count++;
        }
        break;
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        Programmierbare Kennung abspeichern. */
        bed_kennung[keyb_count]=recdata.byte.low;
        if ( keyb_count==7 )
        {
            bed_kennung_gueltig=TRUE;
            keyb_zeichenkey=0;
        }
        else
        {
            keyb_count++;
        }
        break;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        Aktuelle Seitennummer abspeichern. */
        akt_seitennr[keyb_count]=recdata.byte.low;
        if ( keyb_count==2 )
        {
            akt_seitennr_gueltig=TRUE;
            keyb_zeichenkey=0;
        }
        else
        {
            keyb_count++;
        }
        break;
        /* EasyCODE ) */
    case 6 :
        /* EasyCODE ( 0 
        Freier Piktogramm Speicher abspeichern */
        pikto_speicher[keyb_count]=recdata.byte.low;
        if ( keyb_count==4 )
        {
            pikto_speicher_gueltig=TRUE;
            keyb_zeichenkey=0;
        }
        else
        {
            keyb_count++;
        }
        break;
        /* EasyCODE ) */
    case 7 :
        /* EasyCODE ( 0 
        Aktuelle Anzahl der Piktogramme abspeichern */
        pikto_anz[keyb_count]=recdata.byte.low;
        if ( keyb_count==2 )
        {
            pikto_anz_gueltig=TRUE;
            keyb_zeichenkey=0;
        }
        else
        {
            keyb_count++;
        }
        break;
        /* EasyCODE ) */
    case 8 :
        /* EasyCODE ( 0 
        Aktuelle Anzahl der Touchfelder auf dem Display */
        touchfelder_anz[keyb_count]=recdata.byte.low;
        if ( keyb_count==2 )
        {
            touchfelder_anz_gueltig=TRUE;
            keyb_zeichenkey=0;
        }
        else
        {
            keyb_count++;
        }
        break;
        /* EasyCODE ) */
    case 9 :
        /* EasyCODE ( 0 
        Touch Fehlernummer abspeichern */
        bdf_err_feld[keyb_count]=recdata.byte.low;
        if ( keyb_count==1 )
        {
            if ( koor.bdfbreakrdy )
            {
                bdf_err = atoi(bdf_err_feld); // Die zwei Ziffern in einem Byte zusammenfassen
                keyb_err = 13;
                keyb_zeichenkey=0;
            }
        }
        else
        {
            keyb_count++;
        }
        break;
        /* EasyCODE ) */
    case 10 :
        /* EasyCODE ( 0 
        Umschalten auf Kartenmodus (nach 0x0f) */
        if ( recdata.byte.low != 0x0e )
        {
            // Falsches Zeichen
            keyb_err = 14;
            keyb_zeichenkey = 0;
        }
        else
        {
            KartenmodusFlag = TRUE;
            KartenmodusMerkerFlag = TRUE;
            keyb_zeichenkey = 0;
        }
        break;
        /* EasyCODE ) */
    case 11 :
        /* EasyCODE ( 0 
        IP-Adresse abspeichern */
        if ( recdata.byte.low == 'i' )
        {
            keyb_count = 0;
        }
        else
        {
            if ( recdata.byte.low == '\r' )
            {
                ipadr[keyb_count]=0;
                ipadr_gueltig = TRUE;
                keyb_zeichenkey = 0;
            }
            else
            {
                ipadr[keyb_count]=recdata.byte.low;
                keyb_count++;
            }
        }
        break;
        /* EasyCODE ) */
    default:
        /* EasyCODE ( 0 
        sonst */
        if ( koor.bdfbreakrdy )
        {
            keyb_err=12; // Falsche Verzweigung (nur Warnung).
            keyb_zeichenkey = 0;
        }
        break;
        /* EasyCODE ) */
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKeyb::SD_zeichen_auswerten */
/* EasyCODE F */
// Funktion SD_zeichen_auswerten
// Daten vom SD-Kartenteil auswerten.

void KlasseKeyb::SD_zeichen_auswerten(void)
{
    if ( recdata.byte.low == OPCODEB )
    {
        if ( olddata == OPCODEB )
        {
            olddata = 0;
            sdbuf.writebyte(recdata.byte.low);
        }
        else
        {
            olddata = recdata.byte.low;
        }
    }
    else
    {
        if ( recdata.byte.low  == OPCODEA )
        {
            /* EasyCODE ( 0 
            evtl. auf Bedienfeldmode umschalten */
            if ( olddata == OPCODEB )
            {
                // auf Bedienfeld umschalten
                olddata = 0;
                KartenmodusFlag = FALSE;
            }
            else
            {
                sdbuf.writebyte(recdata.byte.low);
            }
            /* EasyCODE ) */
        }
        else
        {
            sdbuf.writebyte(recdata.byte.low);
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKeyb::taste_auswerten */
/* EasyCODE F */
void KlasseKeyb::taste_auswerten(void)
{
    if ( keyb_taste_temp == 0 )
    {
        // keine Taste gedrueckt
        keyb_taste=0xff;
    }
    else
    {
        keyb_taste=keyb_taste_temp;
    }
    auswertung=FALSE;
}

/* EasyCODE ) */
/* EasyCODE ) */
