/* EasyCODE V8 */
/* EasyCODE ( 0 
Unterprogramme fÅr die Eingabe */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
/* EasyCODE < */
/*
Datei:        eingup.c
Klasse:       Kl3590 Portal
Ersteller:    Halfmann   EWE 2856

Folgende Unterprogramme fÅr die Eingabe stehen zur VerfÅgung:
getvalue_eing........... liest einen Wert Åber das Bedienfeld ein
getvalue_abl............. liest einen Wert Åber das Bedienfeld ein ohne haengen zu bleiben.
getstring.................... liest einen String (Buchstaben, Zahlen und Sonderzeichen) Åber das Bedienfeld ein
getvalue_scr............. zum scrollen ohne haengen zu bleiben.
getkey ...................... liest eine Taste Åber das Bedienfeld ein
getkeylos ..................liest eine Taste Åber das Bedienfeld ein nachdem diese Taste wieder losgelassen wurde.
diskstart .................... nur noch DiskettenprÅfung

error_res....................warten bis Fehleranzeige und entstîren Åber koordinator fertig 
*/

#include "portal.h"

#pragma option -O1   // Erzeugt den kleinstmîglichen Code

/* interne Funktionsprototypen */

static void anzeige (unsigned char *string,unsigned char kommastellen);
void stringanzeige (unsigned char *string, unsigned char maxchar);
void pinanzeige (unsigned char maxchar);
void stringclear (unsigned char cur_pos,  unsigned char maxchar);
static void wertinkr (long int maxwert,long int minwert);
static void wertdekr (long int maxwert,long int minwert);
static long int get_grenzwert (long int maxwert,long int minwert);
static unsigned char plus_funct (long int maxwert,long int minwert,unsigned char kommastellen,unsigned char *string);
static unsigned char minus_funct (long int maxwert,long int minwert,unsigned char kommastellen,unsigned char *string);
static unsigned char left_funct(unsigned char *cur_pos);
static unsigned char right_funct( unsigned char anz_zeichen, unsigned char *cur_pos, unsigned char maxchar);
static void ziffer_funct (unsigned char kommastellen,unsigned char *string,unsigned char ziffer);
static void string_funct (unsigned char  maxchar, unsigned char *anz_zeichen,  unsigned char zeichen, unsigned char *cur_pos);
static del_funct (unsigned char  maxchar, unsigned char *zeichen, unsigned char cur_pos);
static void wrong_value (unsigned char *zulstring);
static void pnrwahlupd (void);

/* Datendefinitionen */
unsigned char zulvarstr[400];
volatile struct pargvstruct pargetval;  
volatile struct pargetstringstruct pargetstring;  
volatile struct pargvpinstruct pargvpin;

unsigned char ziffer;
volatile unsigned char getvalkey;

/* interne Daten */
static unsigned char firstziffer;
static unsigned char firstchar;
static unsigned char getval_varstr [200];  // var. String von getvalue
static unsigned char varstr [200];  // var. String von getvalue
static long int grenzwert;                         // Grenzwert bei Zifferneingabe
static unsigned char anzupd;                 // fuer Getvalue Programmnummerwahl

/* Interne Daten fuer die PIN Eingabe */
unsigned char akt_user;          // Nach dem Einschalten ist der User 1 (akt_user = 0) aktiv.
unsigned char anz_pinchar;   


// 0 = Frei; 1 = gesperrt;  default = alles frei bis auf die Funkt. zum Veraendern der Freigabe.
unsigned char fkey_ur [FKEY_USER_MAX][FKEY_FUNK_MAX] =
{
//0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19               User1
{ 0,1,0,0,1,0,1,1,1,1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 
//20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,}, 
//0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19               User2
{ 0,0,0,0,1,0,0,1,1,0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 
//20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39
  1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,}, 
//0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19               User3
{ 0,0,0,0,0,0,0,0,0,0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 
//20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39
  1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,}, 
};
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE ( 0 
Alle mîglichen getval.. und getkey Varianten */
/* EasyCODE ( 0 
getvalue_eing */
/* EasyCODE ( 0 
Beschreibung */
/* EasyCODE < */
/********** getvalue_eing**********/
/*
Eingabe eines Zahlenwertes:
========================
Das Up liest einen Wert Åber das Bedienfeld ein. Dazu werden die Tasten
bearbeitet und der aktuelle Wert angezeigt. Die Wertegrenzen werden berÅck-
sichtigt. Der Wert ist immer eine Integerzahl. Wenn dieser Wert als
Realzahl angezeigt werden soll, muss die Anzahl der
Kommastellen Åbergeben werden. Ein eingegebener, gÅltiger Wert wird in dem
struct wert zurÅckgemeldet.
Die Eingabe von Werten kann Åber die Zifferntasten, als
auch Åber die Rolltasten erfolgen. Beim Rollen wird zwischen
Einzelpuls, langsamen und schnellem Rollen unterschieden. 
Die Touch +/- Taste dient zur Eingabe des Vorzeichens (Toggelfunktion). 

Sonderzeichenbehandlung  :
=======================
Abhaengig vom Auftragsbyte erzeugt Getvalue 
den Standardbildschirm (Linie, Ziffernblock, Clear, up, down, ESC, ENTER)
und die dazugehoerigen Touchcodes zur Eingabe von Werten.
Auch Abhaengig vom Auftragsbyte wird der Sonderstring (Sondertasten).
angezeigt.
Das Auftragsbyte dient zum flackerfreien Anzeigen von einem veraenderten
Sonderstring nach einem Sondertastendruck.
Denn nach druecken einer Sondertaste setzt der Aufrufer den Auftrag so, dass der 
Standardbildschirm nicht angezeigt wird, er bleibt einfach auf dem Touch unveraendert stehen,
und nur der Sonderstring wird angezeigt .

Achtung: Falls das druecken einer Sondertaste das Verschwinden oder Veraendern einer Sondertaste oder Pikto zur
Folge hat, dann muss der Aufrufer im Sonderstring auch das entsprechende Touch Element 
loeschen, um Element Ueberlagerungen zu vermeiden.
Grund: Getvalue ruft ja in diesem Fall keine Bildschirmvorlage auf und es wird kein CLRHOME
durchgefuehrt, alle Elemente bleiben sichtbar.

getvalue_eing und getvalue_abl sehen alle unbekannten Tastendruecke als Sondertasten
an und geben auch den Tastencode dieser Sondertasten in dem struct taste zurueck.
Druecken einer Sondertaste beendet somit getvalue.


Der Aufrufer muss folgende Struktur vor dem Aufruf von getvalue versorgen.
*/
// Parameter Struct:
// struct pargvstruct
//   {
//   unsigned char auftrag;
//   long int altwert;           // alter Wert
//   long int maxwert;         // maximaler Wert 
//   long int minwert;         // minimaler Wert
//   unsigned char kommastellen; // Anzahl der anzuzeigenden Kommastellen
//   unsigned char *pikto;         // Zeiger auf anzuzeigende, unveraenderliche Piktos
//   unsigned char *string;         // Zeiger auf den anzuzeigenden String
//   unsigned char *zulstring;    // Zullaessig String aus den Texten
//   unsigned char *sonderstring;    // String zur Anzeige von Sondertasten.
//   unsigned char *sonder2string;   // String zur Anzeige von Gruppe und Kommentartext bei pnr Wahl.
//   long int wert;                      // Rueckgabewert von getvalue
//   unsigned char taste;           // Rueckgabe des Tochcodes der Sondertasten von getvalue
//   };

// auftrag = 0 :    Standardgetvalue anzeige  und   keine Sonderstringbehandlung.  (Normalfall)
// auftrag = 1 :    Standardgetvalue anzeige   und  Sonderstring anzeige. (Erste mal mit Sonderzeichen)
// auftrag = 2 :    Keine Standardgetvalue anzeige, nur Sonderstring anzeige. (Weitere Aufrufe mit Sonderzeichen)
// auftrag = 3 :    Programmnummerwahl (Standardgetvalue plus Gruppenanzeige und Kommentartext im Sonder2string)
// auftrag = 4 :    Wie 3 und 1, also mit Sondersting und Sonder2string fÅr pnr Wahl.
// auftrag = 5 :    Wie 4 jedoch wird firstziffer auf False gesetzt. (fÅr den öbergang von Hand nach pnr Wahl)
// auftrag = 6 :    Wie 2 jedoch wird bei Sondertasten keine Zullaessig String Pruefung gemacht (z.B Luftmenge).
// Zul-String wird immer (bei allen auftraegen) geloescht, neu bestimmt und anzeigt.



// FunktionsrÅckmeldungen:
//    0 ... EingabebestÑtigung mit Enter, wert ist gÅltig
//    1 ... Abbruch durch ESC, wert ist ungÅltig
//    2 ... Stopanforderung, wert ist ungÅltig
//    3 ... Touchcode der Sondertasten falls Sondertaste gedrueckt, wert ist gÅltig.
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE F */
unsigned char getvalue_eing()
{
    /* EasyCODE ( 0 
    Vorbereitung */
    unsigned char ziffer;
    if ( taste_los() == 0xff )
    {
        /* EasyCODE < */
        // Stoppanf
        /* EasyCODE > */
        return 2;
    }
    if ( (pargetval.auftrag == 0)
                           ||
         (pargetval.auftrag == 1)
                           ||
         (pargetval.auftrag == 3)
                           ||
         (pargetval.auftrag == 4)
                           ||
         (pargetval.auftrag == 5) )
    {
        /* EasyCODE ( 0 */
        /* EasyCODE < */
        getval_botsch1.senden(BV011);             // getvalue
        getval_botsch1.senden(pargetval.pikto);   // Piktos anzeigen
        /* EasyCODE > */
        if ( pargetval.auftrag == 5 )
        {
            firstziffer=FALSE;
        }
        else
        {
            firstziffer=TRUE;
        }
        /* EasyCODE < */
        pargetval.wert=pargetval.altwert;   // Initialisierung auf den alten Wert
        pargetval.taste = 0xff;             // Initialisierung auf keine Taste
        anzeige(pargetval.string,pargetval.kommastellen); // Abhaengig von den pargetval.kommastellenstring den String anzeigen.
        /* EasyCODE > */
        /* EasyCODE ) */
    }
    // Anzahl der eingebbaren Ziffern immer ermitteln
    grenzwert=get_grenzwert(pargetval.maxwert,pargetval.minwert);
    /* EasyCODE ( 0 
    Zul-String loeschen neu bestimmen und anzeigen */
    switch ( pargetval.kommastellen )
    {
    case 1 :
        sprintf(zulvarstr,zulf1s,((float)pargetval.minwert/pow10(pargetval.kommastellen)), ((float)pargetval.maxwert/pow10(pargetval.kommastellen)));
        break;
    case 2 :
        sprintf(zulvarstr,zulf2s,((float)pargetval.minwert/pow10(pargetval.kommastellen)), ((float)pargetval.maxwert/pow10(pargetval.kommastellen)));
        break;
    case 3 :
        sprintf(zulvarstr,zulf3s,((float)pargetval.minwert/pow10(pargetval.kommastellen)), ((float)pargetval.maxwert/pow10(pargetval.kommastellen)));
        break;
    default:
        /* EasyCODE < */
        // 0:
        /* EasyCODE > */
        sprintf(zulvarstr,zulis,pargetval.minwert, pargetval.maxwert);
        break;
    }
    strcpy(varstr, zulloesch);
    strcat (varstr, zulvarstr);
    getval_botsch1.senden(varstr);
    /* EasyCODE ) */
    if ( (pargetval.auftrag == 1)
                           ||
         (pargetval.auftrag == 2)
                           ||
         (pargetval.auftrag == 4)
                           ||
         (pargetval.auftrag == 5)
                           ||
         (pargetval.auftrag == 6) )
    {
        getval_botsch1.senden(pargetval.sonderstring);
    }
    anzupd = FALSE;
    /* EasyCODE ( 0 
    Programmnummerwahl */
    if ( (pargetval.auftrag == 3)   
                           ||
         (pargetval.auftrag == 4)
                           ||
         (pargetval.auftrag == 5) )
    {
        /* EasyCODE < */
        teing3.rsz();
        teing3.sz(1);  // Nur alle halbe Sekunde ein Update der Bereichswahl und des Kommentartext durchfuehren.
        // Das erste Update soll jedoch schneller erfolgen.
        anzupd = TRUE; // Merker fuer getkey
        /* EasyCODE > */
    }
    /* EasyCODE ) */
    /* EasyCODE ) */
    while ( TRUE )
    {
        switch ( (ziffer = getkey()) )
        {
        case 0xff
                 // Stopanforderung
        :
            anzupd = FALSE;
            return 2;
        case TESC :
            anzupd = FALSE;
            return 1;
        case TCLEAR :
            /* EasyCODE ( 0 */
            if ( (pargetval.auftrag == 3)   
                                   || 
                 (pargetval.auftrag == 4)
                                   ||
                 (pargetval.auftrag == 5) )
            {
                teing3.rsz();
                teing3.sz(1);  
                anzupd = TRUE;
            }
            pargetval.wert=0;
            anzeige(pargetval.string,pargetval.kommastellen);
            firstziffer=TRUE;
            /* EasyCODE ) */
            break;
        case TENTER :
            /* EasyCODE ( 0 */
            if ( ( pargetval.wert>=pargetval.minwert) && ( pargetval.wert<=pargetval.maxwert) )
            {
                anzupd = FALSE;
                return 0;
            }
            else
            {
                wrong_value(zulvarstr);
                break;
            }
            /* EasyCODE ) */
        case TSCRUP :
            /* EasyCODE ( 0 */
            if ( plus_funct(pargetval.maxwert,pargetval.minwert,pargetval.kommastellen,pargetval.string) )
            {
                return 2;
            }
            else
            {
                if ( (pargetval.auftrag == 3)   
                                       || 
                     (pargetval.auftrag == 4)
                                       ||
                     (pargetval.auftrag == 5) )
                {
                    teing3.rsz();
                    teing3.sz(1);  
                    anzupd = TRUE;
                }
                break;
            }
            /* EasyCODE ) */
        case TSCRDOWN :
            /* EasyCODE ( 0 */
            if ( minus_funct(pargetval.maxwert,pargetval.minwert,pargetval.kommastellen,pargetval.string) )
            {
                return 2;
            }
            else
            {
                if ( (pargetval.auftrag == 3)   
                                       || 
                     (pargetval.auftrag == 4)
                                       ||
                     (pargetval.auftrag == 5) )
                {
                    teing3.rsz();
                    teing3.sz(1);  
                    anzupd = TRUE;
                }
                break;
            }
            /* EasyCODE ) */
        case TPLUSMINUS :
            /* EasyCODE ( 0 */
            if ( (pargetval.minwert<0) && (pargetval.wert != 0) )
            {
                pargetval.wert=-pargetval.wert;   // Vorzeichen Toggeln
                anzeige(pargetval.string,pargetval.kommastellen);
            }
            /* EasyCODE ) */
            break;
        case TNULL :
            /* EasyCODE = */
        case T1 :
            /* EasyCODE = */
        case T2 :
            /* EasyCODE = */
        case T3 :
            /* EasyCODE = */
        case T4 :
            /* EasyCODE = */
        case T5 :
            /* EasyCODE = */
        case T6 :
            /* EasyCODE = */
        case T7 :
            /* EasyCODE = */
        case T8 :
            /* EasyCODE = */
        case T9 :
            /* EasyCODE ( 0 */
            /* EasyCODE ( 0 
            Ziffer ermitteln */
            switch ( ziffer )
            {
            case TNULL :
                ziffer  = 0;
                break;
            case T1 :
                ziffer  = 1;
                break;
            case T2 :
                ziffer  = 2;
                break;
            case T3 :
                ziffer  = 3;
                break;
            case T4 :
                ziffer  = 4;
                break;
            case T5 :
                ziffer  = 5;
                break;
            case T6 :
                ziffer  = 6;
                break;
            case T7 :
                ziffer  = 7;
                break;
            case T8 :
                ziffer  = 8;
                break;
            default:
                // T9
                ziffer  = 9;
                break;
            }
            /* EasyCODE ) */
            if ( (pargetval.auftrag == 3)   
                                   || 
                 (pargetval.auftrag == 4)
                                   ||
                 (pargetval.auftrag == 5) )
            {
                teing3.rsz();
                teing3.sz(50);  // Nur alle Sekunde ein Update der Bereichswahl und des Kommentartext durchfuehren.
                anzupd = TRUE;
            }
            ziffer_funct(pargetval.kommastellen,pargetval.string,ziffer);
            /* EasyCODE ) */
            break;
        default:
            /* EasyCODE ( 0 */
            /* EasyCODE < */
            // Sondertaste gedrueckt (Wert muess gueltig sein)
            /* EasyCODE > */
            if ( (( pargetval.wert>=pargetval.minwert) && ( pargetval.wert<=pargetval.maxwert))
                                                                               || 
                                                             (pargetval.auftrag == 6) )
            {
                anzupd = FALSE;
                pargetval.taste = ziffer;
                return 3;
            }
            else
            {
                wrong_value(zulvarstr);
                break;
            }
            /* EasyCODE ) */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
getvalue_abl */
/* EasyCODE F */
/* EasyCODE < */
/********** getvalue_abl **********/
/*
getvalue_abl ist aehnlich wie getvalue, jedoch bleibt getvalue_abl nicht haengen und
bei getvalue_abl gibt es keine Behandlung der Stopanforderung. Der Aufrufer mu· somit
die Eingabestopanforderung behandeln.

Eingabe eines Zahlenwertes:
========================
Das Up liest einen Wert Åber das Bedienfeld ein. Dazu werden die Tasten
bearbeitet und der aktuelle Wert angezeigt. Die Wertegrenzen werden berÅck-
sichtigt. Der Wert ist immer eine Integerzahl. Wenn dieser Wert als
Realzahl angezeigt werden soll, muss die Anzahl der
Kommastellen Åbergeben werden. Ein eingegebener, gÅltiger Wert wird in dem
struct wert zurÅckgemeldet.
Die Eingabe von Werten kann Åber die Zifferntasten, als
auch Åber die Rolltasten erfolgen. Beim Rollen wird zwischen
Einzelpuls, langsamen und schnellem Rollen unterschieden. 
Die Touch +/- Taste dient zur Eingabe des Vorzeichens (Toggelfunktion). 

Sonderzeichenbehandlung (neue Strategie) :
====================================
Getvalue erzeugt nur den Standardbildschirm (Linie, Ziffernblock, Clear, up, down, ESC, ENTER)
und die dazugehoerigen Touchcodes zur Eingabe von Werten .
Der Aufrufer muss vor dem Aufruf von getvalue den Bildschirminhalt mit CLRHOME 
immer selbst loeschen und danach, falls gewuenscht, die Piktos bzw. Touchfelder fuer die
Sondertasten selbst hochladen. Zusaetzlich muss der Aufrufer von getvalue_abl den 
getvalkey auf 0 setzen !

getvalue_eing und getvalue_abl sehen alle unbekannten Tastendruecke als Sondertasten
an und geben auch den Tastencode dieser Sondertasten in dem struct taste zurueck.
Druecken einer Sondertaste beendet somit getvalue.

Der Aufrufer muss folgende Structur vor dem Aufruf von getvalue versorgen.
*/
// Parameter Struct:
// struct pargvstruct
//   {
//   int altwert;           // alter Wert
//   int maxwert;         // maximaler Wert 
//   int minwert;         // minimaler Wert
//   unsigned char kommastellen; // Anzahl der anzuzeigenden Kommastellen
//   unsigned char *string;         // Zeiger auf den anzuzeigenden String
//   unsigned char *zulstring;    // Zullaessig String aus den Texten
//   long int wert;                      // Rueckgabewert von getvalue
//   unsigned char taste;           // Rueckgabe des Tochcodes der Sondertasten von getvalue
//   };

// FunktionsrÅckmeldungen:
//    0 ... EingabebestÑtigung mit Enter, wert ist gÅltig
//    1 ... Abbruch durch ESC, wert ist ungÅltig
//    (.... !! getvalue fast behandelt keine Stopanforderung !! )
//    3 ... Tochcode der Sondertasten falls Sondertaste gedrueckt
//    4 ... Noch nicht fertig, abgeben.


unsigned char getvalue_abl()
/* EasyCODE > */
{
    switch ( getvalkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        Getvalue Seite aufbauen */
        if ( !getval_botsch1.busycheck() )
        {
            /* EasyCODE < */
            getval_botsch1.senden(BV011); // getvalue
            /* EasyCODE > */
            /* EasyCODE ( 0 
            Zul-String bestimmen */
            switch ( pargetval.kommastellen )
            {
            case 1 :
                sprintf(zulvarstr,zulf1s,((float)pargetval.minwert/pow10(pargetval.kommastellen)), ((float)pargetval.maxwert/pow10(pargetval.kommastellen)));
                break;
            case 2 :
                sprintf(zulvarstr,zulf2s,((float)pargetval.minwert/pow10(pargetval.kommastellen)), ((float)pargetval.maxwert/pow10(pargetval.kommastellen)));
                break;
            case 3 :
                sprintf(zulvarstr,zulf3s,((float)pargetval.minwert/pow10(pargetval.kommastellen)), ((float)pargetval.maxwert/pow10(pargetval.kommastellen)));
                break;
            default:
                /* EasyCODE < */
                // 0:
                /* EasyCODE > */
                sprintf(zulvarstr,zulis,pargetval.minwert, pargetval.maxwert);
                break;
            }
            /* EasyCODE ) */
            firstziffer=TRUE;
            /* EasyCODE < */
            pargetval.wert=pargetval.altwert;                                  // Initialisierung auf den alten Wert
            /* EasyCODE > */
            /* EasyCODE < */
            pargetval.taste = 0xff;                                                    // Initialisierung auf keine Taste
            /* EasyCODE > */
            /* EasyCODE < */
            grenzwert=get_grenzwert(pargetval.maxwert,pargetval.minwert);  // Anzahl der eingebbaren Ziffern ermitteln
            /* EasyCODE > */
            getvalkey = 1;
        }
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        String und Zullaessigstring anzeigen */
        if ( (!getval_botsch1.busycheck()) && (!getval_botsch2.busycheck()) )
        {
            anzeige(pargetval.string,pargetval.kommastellen);      // Abhaengig von den pargetval.kommastellenstring den String anzeigen.
            
            getval_botsch2.senden(zulvarstr);                                        // Zullaessig String anzeigen.
            getvalkey = 2;
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        Solange abgeben bis aktuelle Taste nicht mehr gedrueckt ist */
        if ( keyb.taste() == 0xff )
        {
            // gedrueckte (alte) Taste wurde losgelassen
            getvalkey = 3;
        }
        else
        {
            /* EasyCODE < */
            // Alte Taste ist noch gedrueckt
            /* EasyCODE > */
        }
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        Neuen Tastendruck erkennen und auswerten */
        if ( (ziffer=keyb.taste()) != 0xff )
        {
            /* EasyCODE < */
            // Taste wurde gedrueckt
            /* EasyCODE > */
            /* EasyCODE ( 0 
            Taste / Ziffer auswerten und entsprechend verzweigen */
            switch ( ziffer )
            {
            case TESC :
                return 1;
            case TCLEAR :
                getvalkey = 4;
                break;
            case TENTER :
                getvalkey = 5;
                break;
            case TSCRUP :
                getvalkey = 22;
                break;
            case TSCRDOWN :
                getvalkey = 26;
                break;
            case TPLUSMINUS :
                getvalkey = 8;
                break;
            case TNULL :
                /* EasyCODE = */
            case T1 :
                /* EasyCODE = */
            case T2 :
                /* EasyCODE = */
            case T3 :
                /* EasyCODE = */
            case T4 :
                /* EasyCODE = */
            case T5 :
                /* EasyCODE = */
            case T6 :
                /* EasyCODE = */
            case T7 :
                /* EasyCODE = */
            case T8 :
                /* EasyCODE = */
            case T9 :
                /* EasyCODE ( 0 
                Ziffer ermitteln */
                switch ( ziffer )
                {
                case TNULL :
                    ziffer  = 0;
                    break;
                case T1 :
                    ziffer  = 1;
                    break;
                case T2 :
                    ziffer  = 2;
                    break;
                case T3 :
                    ziffer  = 3;
                    break;
                case T4 :
                    ziffer  = 4;
                    break;
                case T5 :
                    ziffer  = 5;
                    break;
                case T6 :
                    ziffer  = 6;
                    break;
                case T7 :
                    ziffer  = 7;
                    break;
                case T8 :
                    ziffer  = 8;
                    break;
                default:
                    // T9
                    ziffer  = 9;
                    break;
                }
                /* EasyCODE ) */
                getvalkey = 9;
                break;
            default:
                // Sondertaste gedrueckt
                pargetval.taste = ziffer;
                return 3;
            }
            /* EasyCODE ) */
            break;
        }
        else
        {
            /* EasyCODE < */
            // Keine Taste gedrueckt
            /* EasyCODE > */
        }
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        TCLEAR */
        if ( (!getval_botsch1.busycheck()) )
        {
            pargetval.wert=0;
            anzeige(pargetval.string,pargetval.kommastellen);      // Abhaengig von den pargetval.kommastellenstring den String anzeigen.
            firstziffer=TRUE;
            getvalkey = 2;
        }
        break;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        TENTER */
        if ( ( pargetval.wert>=pargetval.minwert) && ( pargetval.wert<=pargetval.maxwert) )
        {
            return 0;
        }
        else
        {
            /* EasyCODE < */
            getvalkey = 20;          // wrong value
            /* EasyCODE > */
        }
        break;
        /* EasyCODE ) */
    case 8 :
        /* EasyCODE ( 0 
        TPLUSMINUS */
        if ( !getval_botsch1.busycheck() )
        {
            if ( (pargetval.minwert<0) && (pargetval.wert != 0) )
            {
                /* EasyCODE < */
                pargetval.wert = -pargetval.wert;   // Vorzeichen Toggeln
                anzeige(pargetval.string,pargetval.kommastellen);      // Abhaengig von den pargetval.kommastellenstring den String anzeigen.
                /* EasyCODE > */
            }
            else
            {
                /* EasyCODE < */
                // Vorzeichen
                // nicht aendern
                /* EasyCODE > */
            }
            getvalkey = 2;
        }
        break;
        /* EasyCODE ) */
    case 9 :
        /* EasyCODE ( 0 
        Ziffer auswerten */
        if ( (!getval_botsch1.busycheck()) )
        {
            if ( firstziffer )
            {
                if ( pargetval.wert<0 )
                {
                    pargetval.wert = -ziffer;
                }
                else
                {
                    pargetval.wert = ziffer;
                }
                firstziffer = FALSE;
            }
            else
            {
                /* EasyCODE ( 0 
                weitere Ziffern */
                if ( pargetval.wert<0 )
                {
                    if ( ( pargetval.wert = pargetval.wert*10-ziffer) < -grenzwert )
                    {
                        pargetval.wert = -ziffer;
                    }
                }
                else
                {
                    if ( ( pargetval.wert = pargetval.wert*10+ziffer) > grenzwert )
                    {
                        pargetval.wert = ziffer;
                    }
                }
                /* EasyCODE ) */
            }
            anzeige(pargetval.string,pargetval.kommastellen);      // Abhaengig von den pargetval.kommastellenstring den String anzeigen.
            getvalkey = 2;
        }
        break;
        /* EasyCODE ) */
    case 20 :
        /* EasyCODE ( 0 
        Wrong Value (Teil1) */
        if ( (!getval_botsch1.busycheck()) && (!getval_botsch2.busycheck()) && (!getval_botsch3.busycheck()) )
        {
            firstziffer=TRUE;
            getval_botsch1.senden(hupe_ion);    // hupen und invers on   
            getval_botsch2.senden(zulvarstr);       
            getval_botsch3.senden(IOFF);            // invers aus
            
            teing1.rsz();
            teing1.sz(25);
            getvalkey = 21;
        }
        break;
        /* EasyCODE ) */
    case 21 :
        /* EasyCODE ( 0 
        Wrong Value (Teil2) */
        if ( (!getval_botsch1.busycheck()) && (teing1.tz()) )
        {
            getval_botsch1.senden(zulvarstr);
            getvalkey = 2;
        }
        break;
        /* EasyCODE ) */
    case 22 :
        /* EasyCODE ( 0 
        Hoch Scrollen (Teil1) */
        if ( (!getval_botsch1.busycheck()) )
        {
            firstziffer=TRUE;
            wertinkr(pargetval.maxwert,pargetval.minwert);
            teing1.rsz();
            teing1.sz(saf->slowdelay);  // Nach dem ersten Tastendruck warten
            anzeige(pargetval.string,pargetval.kommastellen);      // Abhaengig von den pargetval.kommastellenstring den String anzeigen.
            
            getvalkey = 23;
        }
        break;
        /* EasyCODE ) */
    case 23 :
        /* EasyCODE ( 0 
        Hoch Scrollen (Teil2) */
        if ( (keyb.taste()!=TSCRUP) )
        {
            // Taste wurde losgelassen
            getvalkey = 2;
        }
        else
        {
            if ( teing1.tz() )
            {
                if ( (!getval_botsch1.busycheck()) )
                {
                    // Slowdelay abgelaufen
                    
                    teing1.rsz();
                    teing1.sz(saf->fastdelay);
                    wertinkr(pargetval.maxwert,pargetval.minwert);
                    teing2.rsz();
                    teing2.sz(saf->slowtime);
                    
                    anzeige(pargetval.string,pargetval.kommastellen);      
                    
                    getvalkey = 24;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 24 :
        /* EasyCODE ( 0 
        Hoch Scrollen (Teil3) */
        if ( (keyb.taste()!=TSCRUP) )
        {
            // Taste wurde losgelassen
            getvalkey = 2;
        }
        else
        {
            if ( teing2.tz() )
            {
                /* EasyCODE ( 0 
                Slowtime abgelaufen */
                if ( (!getval_botsch1.busycheck()) )
                {
                    if ( teing1.tz() )
                    {
                        // Fastdelay abgelaufen
                        getvalkey = 25;
                    }
                    else
                    {
                        wertinkr(pargetval.maxwert,pargetval.minwert);
                        teing2.rsz();
                        teing2.sz(saf->slowtime);
                        anzeige(pargetval.string,pargetval.kommastellen);
                    }
                }
                /* EasyCODE ) */
            }
        }
        break;
        /* EasyCODE ) */
    case 25 :
        /* EasyCODE ( 0 
        Hoch Scrollen (Teil4) */
        if ( (keyb.taste()!=TSCRUP) )
        {
            // Taste wurde losgelassen
            getvalkey = 2;
        }
        else
        {
            if ( teing2.tz() )
            {
                if ( (!getval_botsch1.busycheck()) )
                {
                    wertinkr(pargetval.maxwert,pargetval.minwert);
                    teing2.rsz();
                    teing2.sz(saf->fasttime);
                    anzeige(pargetval.string,pargetval.kommastellen);
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 26 :
        /* EasyCODE ( 0 
        Runter Scrollen (Teil1) */
        if ( (!getval_botsch1.busycheck()) )
        {
            firstziffer=TRUE;
            wertdekr(pargetval.maxwert,pargetval.minwert);
            teing1.rsz();
            teing1.sz(saf->slowdelay);  // Nach dem ersten Tastendruck warten
            
            anzeige(pargetval.string,pargetval.kommastellen);      // Abhaengig von den pargetval.kommastellenstring den String anzeigen.
            getvalkey = 27;
        }
        break;
        /* EasyCODE ) */
    case 27 :
        /* EasyCODE ( 0 
        Runter Scrollen (Teil2) */
        if ( (keyb.taste()!=TSCRDOWN) )
        {
            // Taste wurde losgelassen
            getvalkey = 2;
        }
        else
        {
            if ( teing1.tz() )
            {
                if ( (!getval_botsch1.busycheck()) )
                {
                    // Slowdelay abgelaufen
                    
                    teing1.rsz();
                    teing1.sz(saf->fastdelay);
                    wertdekr(pargetval.maxwert,pargetval.minwert);
                    teing2.rsz();
                    teing2.sz(saf->slowtime);
                    anzeige(pargetval.string,pargetval.kommastellen);      
                    
                    getvalkey = 28;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 28 :
        /* EasyCODE ( 0 
        Runter Scrollen (Teil3) */
        if ( (keyb.taste()!=TSCRDOWN) )
        {
            // Taste wurde losgelassen
            getvalkey = 2;
        }
        else
        {
            if ( teing2.tz() )
            {
                /* EasyCODE ( 0 
                Slowtime abgelaufen */
                if ( (!getval_botsch1.busycheck()) )
                {
                    if ( teing1.tz() )
                    {
                        // Fastdelay abgelaufen
                        getvalkey = 29;
                    }
                    else
                    {
                        wertdekr(pargetval.maxwert,pargetval.minwert);
                        teing2.rsz();
                        teing2.sz(saf->slowtime);
                        anzeige(pargetval.string,pargetval.kommastellen);
                    }
                }
                /* EasyCODE ) */
            }
        }
        break;
        /* EasyCODE ) */
    case 29 :
        /* EasyCODE ( 0 
        Runter Scrollen (Teil4) */
        if ( (keyb.taste()!=TSCRDOWN) )
        {
            // Taste wurde losgelassen
            getvalkey = 2;
        }
        else
        {
            if ( teing2.tz() )
            {
                if ( (!getval_botsch1.busycheck()) )
                {
                    wertdekr(pargetval.maxwert,pargetval.minwert);
                    
                    teing2.rsz();
                    teing2.sz(saf->fasttime);
                    
                    anzeige(pargetval.string,pargetval.kommastellen);
                }
            }
        }
        break;
        /* EasyCODE ) */
    default:
        // Key ist ungueltig. Neuinitialisierung starten.
        getvalkey = 0;
        break;
    }
    /* EasyCODE < */
    // Noch nicht fertig, abgeben
    /* EasyCODE > */
    return 4;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
getvalue_scr */
/* EasyCODE F */
/* EasyCODE < */
/********** getvalue_scr **********/
/*


Dieses getvalue bleibt nicht haengen und dient nur zum Rollen der Eingabewerte
ohne BV Aenderung. Es gibt auch keine Behandlung der Stopanforderung. 
Der Aufrufer muÓ somit die Eingabestopanforderung selbst behandeln.

Eingabe von Zahlenwerten:
======================
Der Wert ist immer eine Integerzahl. Wenn dieser Wert als
Realzahl angezeigt werden soll, muss die Anzahl der
Kommastellen ¸bergeben werden. Ein eingegebener, g¸ltiger Wert wird in dem
struct wert zur¸ckgemeldet.
Die Eingabe von Werten kann nur ¸ber die Rolltasten erfolgen.
Es werden alle moeglichen SCR up und SCR down Tasten abgefragt und gleich behandelt.
Beim Rollen wird zwischen Einzelpuls, langsamen und schnellem Rollen unterschieden. 

Achtung:
=======
Der Aufrufer von getvalue_abl muss den getvalkey auf 0 setzen, nur beim ersten Aufruf !

Der Aufrufer muss folgende Structur vor dem Aufruf von getvalue_scr versorgen.
*/
// Parameter Struct:
// struct pargvstruct
//   {
//   int altwert;           // alter Wert
//   int maxwert;         // maximaler Wert 
//   int minwert;         // minimaler Wert
//   unsigned char kommastellen; // Anzahl der anzuzeigenden Kommastellen
//   unsigned char *string;         // Zeiger auf den anzuzeigenden String
//   unsigned char tastencode   // Aktuelle gedrueckte Taste vorm getvalue Aufruf
//   long int wert;                      // Rueckgabewert von getvalue
//   };

// Funktionsr¸ckmeldungen:
//    0 ... Scr Taste losgelassen. Wert ist immer gueltig. Fertig.
//    1 ... Noch nicht fertig, Scr Taste noch gedrueckt, abgeben.
//    (.... !! getvalue scr behandelt keine Stopanforderung !! )

unsigned char getvalue_scr()
/* EasyCODE > */
{
    switch ( getvalkey )
    {
    case 0 :
        firstziffer = TRUE;                                                              
        pargetval.wert = pargetval.altwert;                                  // Initialisierung auf den alten Wert
        grenzwert = get_grenzwert(pargetval.maxwert,pargetval.minwert);  // Anzahl der eingebbaren Ziffern ermitteln
        getvalkey = 1;
        break;
    case 1 :
        if ( (!getval_botsch1.busycheck()) && (!getval_botsch2.busycheck()) )
        {
            anzeige(pargetval.string,pargetval.kommastellen);      // Abhaengig von den pargetval.kommastellen den String anzeigen.
            getvalkey = 2;
        }
        break;
    case 2 :
        if ( (ziffer=keyb.taste()) != 0xff )
        {
            /* EasyCODE < */
            // Taste wurde gedrueckt
            /* EasyCODE > */
            switch ( ziffer )
            {
            case TGSW_UP :
                /* EasyCODE = */
            case TPLUS :
                /* EasyCODE = */
            case TSCRUP1 :
                /* EasyCODE = */
            case TSCRUP :
                getvalkey = 22;
                break;
            case TGSW_DOWN :
                /* EasyCODE = */
            case TMINUS :
                /* EasyCODE = */
            case TSCRDOWN1 :
                /* EasyCODE = */
            case TSCRDOWN :
                getvalkey = 26;
                break;
            default:
                /* EasyCODE < */
                // Falsche Taste
                /* EasyCODE > */
                return 0;
            }
            break;
        }
        else
        {
            /* EasyCODE < */
            // gedrueckte Taste wurde losgelassen
            /* EasyCODE > */
            return 0;
        }
    case 22 :
        if ( (!getval_botsch1.busycheck()) )
        {
            firstziffer=TRUE;
            wertinkr(pargetval.maxwert,pargetval.minwert);
            teing1.rsz();
            teing1.sz(saf->slowdelay);  // Nach dem ersten Tastendruck warten
            anzeige(pargetval.string,pargetval.kommastellen);      // Abhaengig von den pargetval.kommastellenstring den String anzeigen.
            getvalkey = 23;
        }
        break;
    case 23 :
        if ( (keyb.taste()!=pargetval.tastencode) )
        {
            // Taste wurde losgelassen
            getvalkey = 2;
        }
        else
        {
            if ( teing1.tz() )
            {
                if ( (!getval_botsch1.busycheck()) )
                {
                    // Slowdelay abgelaufen
                    
                    teing1.rsz();
                    teing1.sz(saf->fastdelay);
                    wertinkr(pargetval.maxwert,pargetval.minwert);
                    teing2.rsz();
                    teing2.sz(saf->slowtime);
                    
                    anzeige(pargetval.string,pargetval.kommastellen);      
                    
                    getvalkey = 24;
                }
            }
        }
        break;
    case 24 :
        if ( (keyb.taste()!=pargetval.tastencode) )
        {
            // Taste wurde losgelassen
            getvalkey = 2;
        }
        else
        {
            if ( teing2.tz() )
            {
                if ( (!getval_botsch1.busycheck()) )
                {
                    if ( teing1.tz() )
                    {
                        // Fastdelay abgelaufen
                        getvalkey = 25;
                    }
                    else
                    {
                        wertinkr(pargetval.maxwert,pargetval.minwert);
                        teing2.rsz();
                        teing2.sz(saf->slowtime);
                        anzeige(pargetval.string,pargetval.kommastellen);
                    }
                }
            }
        }
        break;
    case 25 :
        if ( (keyb.taste()!=pargetval.tastencode) )
        {
            // Taste wurde losgelassen
            getvalkey = 2;
        }
        else
        {
            if ( teing2.tz() )
            {
                if ( (!getval_botsch1.busycheck()) )
                {
                    wertinkr(pargetval.maxwert,pargetval.minwert);
                    teing2.rsz();
                    teing2.sz(saf->fasttime);
                    anzeige(pargetval.string,pargetval.kommastellen);
                }
            }
        }
        break;
    case 26 :
        if ( (!getval_botsch1.busycheck()) )
        {
            firstziffer=TRUE;
            wertdekr(pargetval.maxwert,pargetval.minwert);
            teing1.rsz();
            teing1.sz(saf->slowdelay);  // Nach dem ersten Tastendruck warten
            
            anzeige(pargetval.string,pargetval.kommastellen);      // Abhaengig von den pargetval.kommastellenstring den String anzeigen.
            getvalkey = 27;
        }
        break;
    case 27 :
        if ( (keyb.taste()!=pargetval.tastencode) )
        {
            // Taste wurde losgelassen
            getvalkey = 2;
        }
        else
        {
            if ( teing1.tz() )
            {
                if ( (!getval_botsch1.busycheck()) )
                {
                    // Slowdelay abgelaufen
                    
                    teing1.rsz();
                    teing1.sz(saf->fastdelay);
                    wertdekr(pargetval.maxwert,pargetval.minwert);
                    teing2.rsz();
                    teing2.sz(saf->slowtime);
                    anzeige(pargetval.string,pargetval.kommastellen);      
                    
                    getvalkey = 28;
                }
            }
        }
        break;
    case 28 :
        if ( (keyb.taste()!=pargetval.tastencode) )
        {
            // Taste wurde losgelassen
            getvalkey = 2;
        }
        else
        {
            if ( teing2.tz() )
            {
                if ( (!getval_botsch1.busycheck()) )
                {
                    if ( teing1.tz() )
                    {
                        // Fastdelay abgelaufen
                        getvalkey = 29;
                    }
                    else
                    {
                        wertdekr(pargetval.maxwert,pargetval.minwert);
                        teing2.rsz();
                        teing2.sz(saf->slowtime);
                        anzeige(pargetval.string,pargetval.kommastellen);
                    }
                }
            }
        }
        break;
    case 29 :
        if ( (keyb.taste()!=pargetval.tastencode) )
        {
            // Taste wurde losgelassen
            getvalkey = 2;
        }
        else
        {
            if ( teing2.tz() )
            {
                if ( (!getval_botsch1.busycheck()) )
                {
                    wertdekr(pargetval.maxwert,pargetval.minwert);
                    
                    teing2.rsz();
                    teing2.sz(saf->fasttime);
                    
                    anzeige(pargetval.string,pargetval.kommastellen);
                }
            }
        }
        break;
    default:
        // Key ist ungueltig. Neuinitialisierung starten.
        getvalkey = 0;
        break;
    }
    /* EasyCODE < */
    // Noch nicht fertig, abgeben
    /* EasyCODE > */
    return 1;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
getvalue_pin */
/* EasyCODE F */
/* EasyCODE < */
/********** getvalue_pin**********/
/*
Eingabe eines User Pincodess:
========================
Das Up liest einen vier stelligen PIN Code Åber das Bedienfeld ein. Dazu werden die Tasten
bearbeitet und der aktuelle PIN angezeigt. Der Wert ist immer eine Integerzahl. 
Ein eingegebener, gÅltiger PIN wird in dem
struct wert zurÅckgemeldet.
Bei der PIN Eingabe werden die Ziffern symbolisch als * dargestellt.


Sonderzeichenbehandlung :
=======================
Abhaengig vom Auftragsbyte erzeugt Getvalue 
den Standardbildschirm (Linie, Ziffernblock, Clear, ESC, ENTER)
und die dazugehoerigen Touchcodes zur Eingabe von Werten.
Auch Abhaengig vom Auftragsbyte wird der Sonderstring angezeigt.
Der Sonderstring besteht aus zwei Piktos (Pin gueltig und Tochfunktion gueltig).

Das Auftragsbyte dient zum flackerfreien Anzeigen von einem veraenderten
Sonderstring nach ENTER.

Der Aufrufer muss folgende Structur vor dem Aufruf von getvalue versorgen.
*/
// Parameter Struct:
// struct pargvpinstruct
//   {
//   unsigned char auftrag;
//   unsigned char *sonderstring;    // String zur Anzeige der beiden Piktos
//   unsigned char anz_char;          // max. Anzahl der eingebbarer Ziffern
//   long int wert;                            // Rueckgabewert von getvalue
//   };

// auftrag = 0 :    Standardgetvalue anzeige  und   keine Piktos.                              
// auftrag = 1 :    Standardgetvalue anzeige   und  Piktos anzeigen.                         (Erste mal mit Piktos)
// auftrag = 2 :    Keine Standardgetvalue anzeige, nur Piktos anzeigen.                 (Weitere Aufrufe mit Piktos)

// FunktionsrÅckmeldungen:
//    0 ... EingabebestÑtigung mit Enter, PIN ist eingegeben
//    1 ... Abbruch durch ESC, PIN ist ungÅltig
//    2 ... Stopanforderung, PIN ist ungÅltig

unsigned char getvalue_pin()
/* EasyCODE > */
{
    unsigned char ziffer;
    if ( taste_los() == 0xff )
    {
        /* EasyCODE < */
        // Stoppanf
        /* EasyCODE > */
        return 2;
    }
    if ( (pargvpin.auftrag == 0)
                           ||
         (pargvpin.auftrag == 1) )
    {
        /* EasyCODE < */
        getval_botsch1.senden(BV011);  
        pargvpin.wert=0;     // initialisieren      
        anz_pinchar = 0;     // initialisieren           
        grenzwert = 9999;   // maximaler Pincode
        pinanzeige(pargvpin.anz_char);      // String anzeigen.
        /* EasyCODE > */
    }
    /* EasyCODE ( 0 
    Pikto Behandlung */
    if ( (pargvpin.auftrag == 1)
                           ||
         (pargvpin.auftrag == 2) )
    {
        getval_botsch1.senden(pargvpin.sonderstring);
        if ( pargvpin.auftrag == 2 )
        {
            teing1.rsz();
            teing1.sz(30);
            while ( !teing1.tz() )
            {
                ;
            }
            /* EasyCODE < */
            getval_botsch1.senden(pinpiktos);  // Invers wieder loeschen
            /* EasyCODE > */
            /* EasyCODE < */
            pargvpin.wert=0;                     // Pincode anzeige loeschen
            anz_pinchar = 0;
            pinanzeige(pargvpin.anz_char);      // String anzeigen.
            /* EasyCODE > */
        }
    }
    /* EasyCODE ) */
    while ( TRUE )
    {
        switch ( (ziffer = getkey()) )
        {
        case 0xff :
            /* EasyCODE < */
            // Stopanforderung
            /* EasyCODE > */
            return 2;
        case TCLEAR :
            /* EasyCODE < */
            pargvpin.wert=0;
            anz_pinchar = 0;
            pinanzeige(pargvpin.anz_char);      // String anzeigen.
            /* EasyCODE > */
            break;
        case TENTER :
            if ( anz_pinchar < 4 )
            {
                getval_botsch1.senden(BELLBELL);  //hupen 
                getval_botsch1.senden(pin_inv);
                teing1.rsz();
                teing1.sz(30);
                while ( !teing1.tz() )
                {
                    ;
                }
                getval_botsch1.senden(pinpiktos);
            }
            else
            {
                return 0;
            }
            break;
        case TNULL :
            /* EasyCODE = */
        case T1 :
            /* EasyCODE = */
        case T2 :
            /* EasyCODE = */
        case T3 :
            /* EasyCODE = */
        case T4 :
            /* EasyCODE = */
        case T5 :
            /* EasyCODE = */
        case T6 :
            /* EasyCODE = */
        case T7 :
            /* EasyCODE = */
        case T8 :
            /* EasyCODE = */
        case T9 :
            /* EasyCODE ( 0 
            Ziffer ermitteln */
            switch ( ziffer )
            {
            case TNULL :
                ziffer  = 0;
                break;
            case T1 :
                ziffer  = 1;
                break;
            case T2 :
                ziffer  = 2;
                break;
            case T3 :
                ziffer  = 3;
                break;
            case T4 :
                ziffer  = 4;
                break;
            case T5 :
                ziffer  = 5;
                break;
            case T6 :
                ziffer  = 6;
                break;
            case T7 :
                ziffer  = 7;
                break;
            case T8 :
                ziffer  = 8;
                break;
            default:
                // T9
                ziffer  = 9;
                break;
            }
            /* EasyCODE ) */
            if ( anz_pinchar < 4 )
            {
                pargvpin.wert = pargvpin.wert*10+ziffer;
                anz_pinchar++;
                pinanzeige(pargvpin.anz_char);
            }
            else
            {
                /* EasyCODE < */
                getval_botsch1.senden(BELLBELL);      //hupen
                /* EasyCODE > */
            }
            break;
        case TSCRUP :
            /* EasyCODE = */
        case TSCRDOWN :
            /* EasyCODE = */
        case TPLUSMINUS :
            break;
        default:
            /* EasyCODE < */
            // TESC
            /* EasyCODE > */
            while ( keyb.taste() != 0xff )
            {
                if ( koor.eingstop() )
                {
                    return 2;
                }
            }
            return 1;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
getstring */
/* EasyCODE F */
/* EasyCODE < */
/* ********* getstring ********* */
/* 
Eingabe eines Strings:
==================
Das Up liest einen String Åber das Bedienfeld ein. Dazu werden die Tasten
bearbeitet und der aktuelle String angezeigt. Ein eingegebener, gÅltiger String wird in dem
struct string zurÅckgemeldet.

Zur Eingabe von Sonderzeichen wie z.B. Punkt Komma,... und auch Zahlen 
dient die Touch   .,...1 2...   Taste welche einen neuen Bildschirmaufbau mit Sonderzeichen veranlasst.
Mittels der Touch  A B C...  Taste kommt man wieder zurÅck in den Buchstabenbildschirm.

Getstring erzeugt den Standardbildschirm (Linie, Buchstaben/Sonderzeichen, left, right,  Clear, Del, ESC, ENTER)
und die dazugehoerigen Touchcodes.
Der Aufrufer muss vor dem Aufruf von getstring den Anzeigestring mit eventuell Piktos und mit einer eckigen Klammer auf
vorbereitet Åbergeben.

AbhÑngig von anz_char wird der einzugebene Text in eckigen Klammern dargestellt um die
Anzahl der eingebbaren Zeichen optisch zu begrenzen. z.B.: anz_char = 20     =>      [TEST TEXT               ]
Dazu mu· im Textstring die eckige Klammer auf zwischen dem Pikto und dem Formatbezeichner stehen ! 

Der Aufrufer muss folgende Structur vor dem Aufruf von getstring versorgen.
 */
// Parameter Struct:           
// struct pargetstringstruct
//   {
//   unsigned char *altstring;         // alter String (ohne Pikto)
//   unsigned char *string;             // Zeiger auf den anzuzeigenden String (Pikto, Klammer auf, Formalparameter)
//   unsigned char anz_char;        // max. Anzahl der eingebbaren Zeichen
//   unsigned char neustring[30];  // Rueckgabewert von getstring
//   };

// FunktionsrÅckmeldungen:
//    0 ... EingabebestÑtigung mit Enter, neustring ist gÅltig
//    1 ... Abbruch durch ESC, neustring ist ungÅltig
//    2 ... Stopanforderung, neustring ist ungÅltig

unsigned char getstring ()
/* EasyCODE > */
{
    /* EasyCODE < */
    char i;
    unsigned char zeichen;
    unsigned char anz_zeichen;  // Anzahl Zeichen von pargetstring.neustring (von 1 bis ...)
    unsigned char maxchar;        // Grenzwert bei der Stringeingabe (max. Anzahl von Zeichen von 1 bis ...) 
    unsigned char cur_pos;          // Cursor position (von 1 bis ...). cur_pos ist normalerweise = anz_zeichen +1
    /* EasyCODE > */
    if ( taste_los() == 0xff )
    {
        /* EasyCODE < */
        // Stoppanf
        /* EasyCODE > */
        return /* EasyCODE < */
        2
        /* EasyCODE > */
        ;
    }
    getval_botsch1.senden(BV080); // Buchstaben Vorlage
    for ( i=0;i<pargetstring.anz_char;i++ )
    {
        pargetstring.neustring[i] = 0;
    }
    strcpy((char*) pargetstring.neustring, (char*) pargetstring.altstring);   // Initialisierung auf den alten Wert
    anz_zeichen = strlen((char*) pargetstring.neustring);
    maxchar = pargetstring.anz_char;
    cur_pos = anz_zeichen +1;
    stringanzeige(pargetstring.string, maxchar);      // Abhaengig von der maximalen Anzahl Zeichen den String (altstring) anzeigen.
    if ( anz_zeichen == maxchar )
    {
        /* EasyCODE < */
        getval_botsch1.senden(CURSORLEFT);  // In diesem Fall soll der Cursor dem letztem Char stehen.
        cur_pos-- ;
        /* EasyCODE > */
    }
    while ( TRUE )
    {
        switch ( /* EasyCODE < */
        (zeichen = getkey())
        /* EasyCODE > */
         )
        {
        case /* EasyCODE < */
        0xff
                                                                                                               // Stopanforderung
             /* EasyCODE > */
         :
            /* EasyCODE < */
            getval_botsch1.senden(getvalende);
            /* EasyCODE > */
            return /* EasyCODE < */
            2
            /* EasyCODE > */
            ;
        case /* EasyCODE < */
        TESC
        /* EasyCODE > */
         :
            /* EasyCODE < */
            getval_botsch1.senden(getvalende);
            /* EasyCODE > */
            return /* EasyCODE < */
            1
            /* EasyCODE > */
            ;
        case /* EasyCODE < */
        TCLEAR
        /* EasyCODE > */
         :
            /* EasyCODE < */
            stringclear(cur_pos, maxchar);
            pargetstring.neustring[0] =  0x00; // Nullterminator
            anz_zeichen = 0;
            cur_pos = 1;
            /* EasyCODE > */
            break;
        case /* EasyCODE < */
        TENTER
        /* EasyCODE > */
         :
            /* EasyCODE < */
            getval_botsch1.senden(getvalende);
            /* EasyCODE > */
            return /* EasyCODE < */
            0
            /* EasyCODE > */
            ;
        case /* EasyCODE < */
        TLEFT
        /* EasyCODE > */
         :
            if ( left_funct(&cur_pos) )
            {
                /* EasyCODE < */
                // Stopanforderung
                getval_botsch1.senden(getvalende);
                /* EasyCODE > */
                return /* EasyCODE < */
                2
                /* EasyCODE > */
                ;
            }
            else
            {
                break;
            }
        case /* EasyCODE < */
        TRIGHT
        /* EasyCODE > */
         :
            if ( right_funct(anz_zeichen, &cur_pos, maxchar) )
            {
                /* EasyCODE < */
                // Stopanforderung
                getval_botsch1.senden(getvalende);
                /* EasyCODE > */
                return /* EasyCODE < */
                2
                /* EasyCODE > */
                ;
            }
            else
            {
                break;
            }
        case /* EasyCODE < */
        TDEL
        /* EasyCODE > */
         :
            /* EasyCODE < */
            del_funct(maxchar, &anz_zeichen,  cur_pos);
            /* EasyCODE > */
            break;
        case /* EasyCODE < */
        TZAHLEN
        /* EasyCODE > */
         :
            getval_botsch1.senden(BV081);     // Zahlen Vorlage
            stringanzeige(pargetstring.string, maxchar);      // Abhaengig von der maximalen Anzahl Zeichen den String anzeigen.
            /* EasyCODE < */
            // Nach Verlassen von stringanzeige steht der Cursor hinter dem letzten Zeichen. Er soll aber unveraendert bleiben.
            /* EasyCODE > */
            for ( i = 1 ;  (i <=  ((anz_zeichen+1) - cur_pos)); i++ )
            {
                /* EasyCODE < */
                getval_botsch1.senden(CURSORLEFT);
                /* EasyCODE > */
            }
            break;
        case /* EasyCODE < */
        TTEXT
        /* EasyCODE > */
         :
            getval_botsch1.senden(BV080);     // Buchstaben Vorlage
            stringanzeige(pargetstring.string, maxchar);      // Abhaengig von der maximalen Anzahl Zeichen den String anzeigen.
            /* EasyCODE < */
            // Nach Verlassen von stringanzeige steht der Cursor hinter dem letzten Zeichen. Er soll aber unveraendert bleiben.
            /* EasyCODE > */
            for ( i = 1 ;  (i <=  ((anz_zeichen+1) - cur_pos)); i++ )
            {
                /* EasyCODE < */
                getval_botsch1.senden(CURSORLEFT);
                /* EasyCODE > */
            }
            break;
        default:
            /* EasyCODE < */
            // A B C, .....  bzw. 0 1 2 3 4 .,- ,....
            // Zeichen enthÑlt den Dezimal Code des entsprechenden ASCII Zeichens (Buchstaben, Zahlen und Sonderzeichen).
            /* EasyCODE > */
            /* EasyCODE ( 0 
            Zeichen ermitteln */
            switch ( /* EasyCODE < */
            zeichen
            /* EasyCODE > */
             )
            {
            case /* EasyCODE < */
            T65
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 65;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T66
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 66;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T67
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 67;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T68
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 68;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T69
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 69;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T70
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 70;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T71
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 71;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T72
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 72;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T73
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 73;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T74
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 74;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T75
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 75;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T76
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 76;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T77
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 77;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T78
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 78;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T79
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 79;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T80
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 80;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T81
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 81;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T82
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 82;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T83
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 83;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T84
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 84;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T85
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 85;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T86
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 86;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T87
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 87;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T88
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 88;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T89
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 89;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T90
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 90;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T32
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 32;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T49
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 49;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T50
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 50;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T51
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 51;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T52
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 52;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T53
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 53;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T54
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 54;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T55
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 55;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T56
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 56;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T57
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 57;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T48
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 48;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T43
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 43;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T45
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 45;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T42
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 42;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T61
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 61;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T40
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 40;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T41
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 41;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T46
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 46;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T44
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 44;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T58
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 58;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T59
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 59;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T95
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 95;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T33
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 33;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T37
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 37;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T38
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 38;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T34
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 34;
                /* EasyCODE > */
                break;
            case /* EasyCODE < */
            T47
            /* EasyCODE > */
             :
                /* EasyCODE < */
                zeichen = 47;
                /* EasyCODE > */
                break;
            default:
                /* EasyCODE < */
                ziffer  = 65; // = A zum Test
                /* EasyCODE > */
                break;
            }
            /* EasyCODE ) */
            /* EasyCODE < */
            string_funct(maxchar, &anz_zeichen, zeichen, &cur_pos);
            /* EasyCODE > */
            break;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
plus_funct */
/* EasyCODE F */
/********** plus_funct, Up von getvalue **********/
//
// R¸ckmeldung:
//   0 ... Taste Plus losgelassen
//   1 ... Stopanforderung

unsigned char plus_funct(long int maxwert,long int minwert,unsigned char kommastellen,unsigned char *string)
{
    /* EasyCODE < */
    firstziffer=TRUE;
    wertinkr(maxwert,minwert);
    teing1.rsz();
    teing1.sz(saf->slowdelay);
    anzeige(string,kommastellen);
    
    // Nach dem ersten Tastendruck warten
    /* EasyCODE > */
    do{
        if ( keyb.taste()!=TSCRUP )
        {
            return 0;
        }
        if ( koor.eingstop() )
        {
            return 1;
        }
    }while ( !teing1.tz() );
    teing1.rsz();
    /* EasyCODE - */
    teing1.sz(saf->fastdelay);
    do{
        wertinkr(maxwert,minwert);
        teing2.rsz();
        teing2.sz(saf->slowtime);
        anzeige(string,kommastellen);
        do{
            if ( keyb.taste()!=TSCRUP )
            {
                return 0;
            }
            if ( koor.eingstop() )
            {
                return 1;
            }
        }while ( !teing2.tz() );
    }while ( !teing1.tz() );
    while ( keyb.taste()==TSCRUP )
    {
        wertinkr(maxwert,minwert);
        teing2.rsz();
        teing2.sz(saf->fasttime);
        anzeige(string,kommastellen);
        do{
            if ( keyb.taste()!=TSCRUP )
            {
                return 0;
            }
            if ( koor.eingstop() )
            {
                return 1;
            }
        }while ( !teing2.tz() );
    }
    return 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
minus_funct */
/* EasyCODE F */
/********** minus_funct, Up von getvalue **********/
//
// R¸ckmeldung:
//   0 ... Taste Minus losgelassen
//   1 ... Stopanforderung

unsigned char minus_funct(long int maxwert,long int minwert,unsigned char kommastellen,unsigned char *string)
{
    firstziffer=TRUE;
    wertdekr(maxwert,minwert);
    teing1.rsz();
    teing1.sz(saf->slowdelay);
    anzeige(string,kommastellen);
    do{
        if ( keyb.taste()!=TSCRDOWN )
        {
            return 0;
        }
        if ( koor.eingstop() )
        {
            return 1;
        }
    }while ( !teing1.tz() );
    teing1.rsz();
    teing1.sz(saf->fastdelay);
    do{
        wertdekr(maxwert,minwert);
        teing2.rsz();
        teing2.sz(saf->slowtime);
        anzeige(string,kommastellen);
        do{
            if ( keyb.taste()!=TSCRDOWN )
            {
                return 0;
            }
            if ( koor.eingstop() )
            {
                return 1;
            }
        }while ( !teing2.tz() );
    }while ( !teing1.tz() );
    while ( keyb.taste()==TSCRDOWN )
    {
        wertdekr(maxwert,minwert);
        teing2.rsz();
        teing2.sz(saf->fasttime);
        anzeige(string,kommastellen);
        do{
            if ( keyb.taste()!=TSCRDOWN )
            {
                return 0;
            }
            if ( koor.eingstop() )
            {
                return 1;
            }
        }while ( !teing2.tz() );
    }
    return 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
left_funct */
/* EasyCODE F */
/********** Scroll left_funct, Up von getstring **********/
//
// R¸ckmeldung:
//   0 ... Taste CURSORLEFT losgelassen
//   1 ... Stopanforderung

unsigned char left_funct(  unsigned char *cur_pos)
{
    teing1.rsz();
    teing1.sz(saf->slowdelay);
    if ( (*cur_pos) > 1 )
    {
        getval_botsch1.senden(CURSORLEFT);
        (*cur_pos) -- ;
    }
    do{
        /* EasyCODE < */
        // Nach dem ersten Tastendruck warten
        /* EasyCODE > */
        if ( keyb.taste()!=TLEFT )
        {
            return 0;
        }
        if ( koor.eingstop() )
        {
            return 1;
        }
    }while ( !teing1.tz() );
    /* EasyCODE < */
    // Der Cursor wird nun kontinuierlich nach links bewegt.
    /* EasyCODE > */
    while ( keyb.taste()==TLEFT )
    {
        teing2.rsz();
        teing2.sz(saf->slowtime);
        if ( (*cur_pos) > 1 )
        {
            getval_botsch1.senden(CURSORLEFT);
            (*cur_pos) -- ;
        }
        do{
            if ( keyb.taste()!=TLEFT )
            {
                return 0;
            }
            if ( koor.eingstop() )
            {
                return 1;
            }
        }while ( !teing2.tz() );
    }
    return 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
right_funct */
/* EasyCODE F */
/********** Scroll right_funct, Up von getstring **********/
//
// R¸ckmeldung:
//   0 ... Taste CURSORRIGHT losgelassen
//   1 ... Stopanforderung

unsigned char right_funct( unsigned char anz_zeichen, unsigned char *cur_pos, unsigned char maxchar)
{
    teing1.rsz();
    teing1.sz(saf->slowdelay);
    if ( ((*cur_pos) < (anz_zeichen+1))
                          &&
                   ((*cur_pos) < maxchar ) )
    {
        getval_botsch1.senden(CURSORRIGHT);
        (*cur_pos) ++ ;
    }
    do{
        /* EasyCODE < */
        // Nach dem ersten Tastendruck warten
        /* EasyCODE > */
        if ( keyb.taste()!=TRIGHT )
        {
            return 0;
        }
        if ( koor.eingstop() )
        {
            return 1;
        }
    }while ( !teing1.tz() );
    /* EasyCODE < */
    // Der Cursor wird nun kontinuierlich nach rechts bewegt.
    /* EasyCODE > */
    while ( keyb.taste()==TRIGHT )
    {
        teing2.rsz();
        teing2.sz(saf->slowtime);
        if ( ((*cur_pos) < (anz_zeichen+1))
                              &&
                       ((*cur_pos) < maxchar ) )
        {
            getval_botsch1.senden(CURSORRIGHT);
            (*cur_pos) ++ ;
        }
        do{
            if ( keyb.taste()!=TRIGHT )
            {
                return 0;
            }
            if ( koor.eingstop() )
            {
                return 1;
            }
        }while ( !teing2.tz() );
    }
    return 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ziffer_funct */
/* EasyCODE F */
/********** weitere UPs von getvalue **********/

void ziffer_funct (unsigned char kommastellen,unsigned char *string,unsigned char ziffer)
{
    if ( firstziffer )
    {
        if ( pargetval.wert<0 )
        {
            pargetval.wert = -ziffer;
        }
        else
        {
            pargetval.wert = ziffer;
        }
        firstziffer = FALSE;
    }
    else
    {
        if ( pargetval.wert<0 )
        {
            if ( ( pargetval.wert = pargetval.wert*10-ziffer) < -grenzwert )
            {
                pargetval.wert = -ziffer;
            }
        }
        else
        {
            if ( ( pargetval.wert = pargetval.wert*10+ziffer) > grenzwert )
            {
                pargetval.wert = ziffer;
            }
        }
    }
    anzeige(string,kommastellen);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
string_funct */
/* EasyCODE F */
/********** weitere UPs von getvalue **********/
// Abhaengig von der Anzahl Zeichen wird der neustring erweitert (falls erforderlich begrenzt) und angezeigt.

void string_funct (unsigned char  maxchar, unsigned char *anz_zeichen, unsigned char zeichen, unsigned char * cur_pos)
{
    unsigned char tempstring [100]; // var. String
    unsigned char hilfstring [100]; // var. String
    unsigned char i;
    if ( (*anz_zeichen) < (*cur_pos) )
    {
        if ( (*anz_zeichen) < (maxchar - 1) )
        {
            // Cursor wandert um eins nach rechts.
            sprintf(getval_varstr,"%c",zeichen); 
            strcat((char*) pargetstring.neustring, getval_varstr);
            (*anz_zeichen) ++;
            (*cur_pos) ++;
            stringanzeige(pargetstring.string, maxchar);
        }
        else
        {
            // Letzt moegliches Zeichen anhaengen 
            // und Cursor um eins nach links.
            
            sprintf(getval_varstr,"%c",zeichen); 
            strcat((char*) pargetstring.neustring, getval_varstr);
            (*anz_zeichen) ++;
            stringanzeige(pargetstring.string, maxchar);
            getval_botsch1.senden(CURSORLEFT);
        }
    }
    else
    {
        if ( (*anz_zeichen) < maxchar )
        {
            /* EasyCODE < */
            // Insert ist moeglich, da rechts noch Zeichen frei sind.
            strcpy(tempstring, (char*) pargetstring.neustring);
            strcpy(hilfstring, (char*) pargetstring.neustring);
            tempstring[(*cur_pos)-1] = zeichen;
            
            // Restliche Zeichen eins nach rechts schieben.
            /* EasyCODE > */
            for ( i = (*cur_pos); i <= ((*anz_zeichen)); i++ )
            {
                tempstring[i] = hilfstring[i-1] ;
            }
            /* EasyCODE < */
            tempstring[(*anz_zeichen)+1] = 0x00;    // Nullterminator
            strcpy((char*)pargetstring.neustring,tempstring);
            stringanzeige(pargetstring.string, maxchar);
            (*anz_zeichen)++;
            
            // Cursor wieder auf (urspruengliche Position +1) zurueckholen.
            // Nach Verlassen von stringanzeige steht der 
            // Cursor hinter dem letzten Zeichen
            /* EasyCODE > */
            for ( i = 1 ;  (i <=  (((*anz_zeichen)+1) - ((*cur_pos)+1))); i++ )
            {
                getval_botsch1.senden(CURSORLEFT);
            }
            (*cur_pos)++;
        }
        else
        {
            if ( (*cur_pos) ==  (*anz_zeichen) )
            {
                // (*anz_zeichen)  =  maxchar = (*cur_pos)
                // Letztes Zeichen mit neuem Zeichen ¸berschreiben
                // und Cursor um eins nach links.
                // Anzahl Zeichen wird nicht mehr hochgezaehlt.
                
                strncpy(tempstring, (char*) pargetstring.neustring, ((*anz_zeichen)-1));
                /* EasyCODE - */
                tempstring[(*anz_zeichen)-1] = zeichen;
                /* EasyCODE < */
                tempstring[(*anz_zeichen)] = 0x00;    // Nullterminator
                /* EasyCODE > */
                strcpy((char*)pargetstring.neustring,tempstring);
                /* EasyCODE - */
                stringanzeige(pargetstring.string, maxchar);
                /* EasyCODE - */
                getval_botsch1.senden(CURSORLEFT);
            }
            else
            {
                getval_botsch1.senden(BELLBELL);
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
del_funct */
/* EasyCODE F */
/********** weitere UPs von getvalue **********/
// Es wird das Zeichen, auf dem der Cursor steht, geloescht und alle anderen Zeichen werden um
// eins nach links verschoben.  (Klassische Editor Delete Funktion bekannt aus der PC Welt).
// Der Cursor laesst seine Position hier immer gleich. 
// Als Bonus wird bei laengerem druecken der Delete Taste das loeschen wiederholt.
//
// R¸ckmeldung:
//   0 ... Taste Delete losgelassen
//   1 ... Stopanforderung 

del_funct (unsigned char  maxchar, unsigned char * anz_zeichen,  unsigned char cur_pos)
{
    unsigned char tempstring [100]; // var. String
    unsigned char i;
    teing1.rsz();
    teing1.sz(saf->slowdelay);
    if ( cur_pos <= (*anz_zeichen) )
    {
        strcpy(tempstring, (char*)pargetstring.neustring);
        for ( i = cur_pos; i <= ((*anz_zeichen)-1); i++ )
        {
            tempstring[i-1] = tempstring[i] ;
        }
        /* EasyCODE < */
        tempstring[(*anz_zeichen)-1] = 0x00;  // Nullterminator
        strcpy((char*)pargetstring.neustring,tempstring);
        getval_botsch1.senden(DELLINE);
        stringanzeige(pargetstring.string, maxchar);
        
        (*anz_zeichen)--;
        // Cursor wieder auf urspruengliche Position zurueckholen.
        // Nach Verlassen von stringanzeige steht der Cursor hinter dem letzten Zeichen. 
        // Es gilt hier immer : (*anz_zeichen)  <  maxchar
        /* EasyCODE > */
        for ( i = 1 ;  (i <=  (((*anz_zeichen)+1) - cur_pos)); i++ )
        {
            getval_botsch1.senden(CURSORLEFT);
        }
    }
    else
    {
        /* EasyCODE < */
        // Nichts machen,
        // da wir am Ende sind.
        /* EasyCODE > */
    }
    do{
        /* EasyCODE < */
        // Nach dem ersten Tastendruck warten
        /* EasyCODE > */
        if ( keyb.taste()!=TDEL )
        {
            return 0;
        }
        if ( koor.eingstop() )
        {
            return 1;
        }
    }while ( !teing1.tz() );
    /* EasyCODE < */
    // Delete wird nun kontinuierlich ausgeguehrt.
    /* EasyCODE > */
    while ( keyb.taste()==TDEL )
    {
        teing2.rsz();
        teing2.sz(saf->slowtime);
        if ( cur_pos <= (*anz_zeichen) )
        {
            strcpy(tempstring, (char*)pargetstring.neustring);
            for ( i = cur_pos; i <= ((*anz_zeichen)-1); i++ )
            {
                tempstring[i-1] = tempstring[i] ;
            }
            /* EasyCODE < */
            tempstring[(*anz_zeichen)-1] = 0x00;  // Nullterminator
            strcpy((char*)pargetstring.neustring,tempstring);
            getval_botsch1.senden(DELLINE);
            stringanzeige(pargetstring.string, maxchar);
            
            (*anz_zeichen)--;
            // Cursor wieder auf urspruengliche Position zurueckholen.
            // Nach Verlassen von stringanzeige steht der Cursor hinter dem letzten Zeichen. 
            // Es gilt hier immer : (*anz_zeichen)  <  maxchar
            /* EasyCODE > */
            for ( i = 1 ;  (i <=  (((*anz_zeichen)+1) - cur_pos)); i++ )
            {
                getval_botsch1.senden(CURSORLEFT);
            }
        }
        else
        {
            /* EasyCODE < */
            // Nichts machen,
            // da wir am Ende sind.
            /* EasyCODE > */
        }
        do{
            if ( keyb.taste()!=TDEL )
            {
                return 0;
            }
            if ( koor.eingstop() )
            {
                return 1;
            }
        }while ( !teing2.tz() );
    }
    /* EasyCODE < */
    // Taste losgelassen
    /* EasyCODE > */
    return 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
wrong_value */
/* EasyCODE F */
void wrong_value (unsigned char *zulstring)
{
    firstziffer=TRUE;
    getval_botsch1.senden(ESCS "F001" );      //hupen und invers on   
    getval_botsch1.senden(hupe_ion);      //hupen und invers on   
    getval_botsch1.senden(zulstring);       
    getval_botsch1.senden(ESCS "F001" IOFF ESCS "F1");       // invers aus
    
    teing1.rsz();
    teing1.sz(40);
    while ( !teing1.tz() )
    {
        ;
    }
    getval_botsch1.senden(zulstring);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
getkey */
/* EasyCODE F */
/********** getkey **********/
//
// Das Up wartet bis keine Taste mehr gedrueckt ist und meldet die n‰chste ge-
// drueckte Taste bzw. Stopanforderung zur¸ck.
//
// Rueckmeldung:
//   gedrueckte Taste
//   0xff ... Stopanforderung

unsigned char getkey (void)
{
    unsigned char tastmem;
    while ( keyb.taste() != 0xff )
    {
        if ( koor.eingstop() )
        {
            return 0xff;
        }
    }
    while ( (tastmem=keyb.taste())==0xff )
    {
        if ( koor.eingstop() )
        {
            return 0xff;
        }
        if ( anzupd &&  (teing3.tz()) )
        {
            teing3.rsz();
            teing3.sz(50);  // Nur alle halbe Sekunde nach Zifferndruck ein Update der 
                                    // Bereichswahl und des Kommentartext durchfuehren.
            anzupd = FALSE;
            pnrwahlupd();
        }
        /* EasyCODE < */
        // screen.sanduhrreq_set(80,200); //sanduhrtest einschalten AK
        /* EasyCODE > */
    }
    return tastmem;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
getkey_los */
/* EasyCODE F */
/********** getkey_los **********/
//
// Das Up wartet bis keine Taste mehr gedr¸ckt ist und meldet die n‰chste ge-
// dr¸ckte Taste bzw. Stopanforderung zur¸ck, nachdem diese wieder losgelassen
// wurde.
//
// R¸ckmeldung:
//   gedr¸ckte Taste
//   0xff ... Stopanforderung

unsigned char getkey_los (void)
{
    unsigned char tastmem;
    while ( keyb.taste() != 0xff )
    {
        if ( koor.eingstop() )
        {
            return 0xff;
        }
    }
    while ( (tastmem=keyb.taste())==0xff )
    {
        if ( koor.eingstop() )
        {
            return 0xff;
        }
    }
    while ( keyb.taste() != 0xff )
    {
        if ( koor.eingstop() )
        {
            return 0xff;
        }
    }
    return tastmem;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
taste_los */
/* EasyCODE F */
/********** taste_los **********/
//
// Das Up wartet bis keine Taste mehr gedr¸ckt ist 
// R¸ckmeldung:
//   0   .... Keine Taste mehr gedr¸ckt
//   0xff ...Stopanforderung

unsigned char taste_los (void)
{
    while ( keyb.taste() != 0xff )
    {
        if ( koor.eingstop() )
        {
            return 0xff;
        }
    }
    return 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
wertinkr */
/* EasyCODE F */
/********** Inkrement Hilfsroutine **********/

void wertinkr (long int maxwert, long int minwert)
{
    if ( pargetval.wert<maxwert )
    {
        if ( pargetval.wert>=minwert )
        {
            pargetval.wert += 1;
        }
        else
        {
            pargetval.wert=minwert;
        }
    }
    else
    {
        pargetval.wert=maxwert;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
wertdekr */
/* EasyCODE F */
/********** Dekrement Hilfsroutine **********/

void wertdekr (long int maxwert, long int minwert)
{
    if ( pargetval.wert>minwert )
    {
        if ( pargetval.wert<=maxwert )
        {
            pargetval.wert -= 1;
        }
        else
        {
            pargetval.wert=maxwert;
        }
    }
    else
    {
        pargetval.wert=minwert;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
anzeige */
/* EasyCODE F */
void anzeige (unsigned char *string,unsigned char kommastellen)
{
    if ( kommastellen==0 )
    {
        sprintf(getval_varstr,string,pargetval.wert);
    }
    else
    {
        sprintf(getval_varstr,string,
             (float)(pargetval.wert /pow10(kommastellen)));
    }
    getval_botsch1.senden(getval_varstr);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
stringanzeige */
/* EasyCODE F */
void stringanzeige (unsigned char *string, unsigned char maxchar)
{
    /* EasyCODE < */
    char i;
    // Im string muss die eckige Klammer auf vor dem Formatbezeichner stehen !! 
    // In dieser Funktion wird nur Klammer zu ausgegeben.
    // Nach dieser Funktion steht der Cursor immer hinter dem letzten Zeichen des Neustrings. Der Aufrufer muss somit 
    // selbst fuer die richtige Cursor Position sorgen, falls der Cursor mitten im Text war.
    /* EasyCODE > */
    if ( pargetstring.neustring[0] != 0x00 )
    {
        /* EasyCODE < */
        sprintf(getval_varstr ,string ,pargetstring.neustring);
        getval_botsch1.senden(getval_varstr);
        getval_botsch1.senden(CURSOROFF);     //  tempor‰r ausschalten
        /* EasyCODE > */
        while ( getval_botsch1.busycheck() )
        {
        }
        /* EasyCODE < */
        getval_varstr[0] = 0x00; // Nullterminator
        /* EasyCODE > */
        for ( i = 1 ;  (i <=  ((char)(maxchar - strlen((char*)pargetstring.neustring)))); i++ )
        {
            strcat (getval_varstr, CURSORRIGHT);
        }
        strcat (getval_varstr, "]");
        /* EasyCODE - */
        getval_botsch1.senden(getval_varstr);
        for ( i = 1 ;  (i <=  ((char)(maxchar - strlen((char*)pargetstring.neustring+1)))); i++ )
        {
            getval_botsch1.senden(CURSORLEFT);
        }
        getval_botsch1.senden(CURSORON);
    }
    else
    {
        /* EasyCODE < */
        sprintf(getval_varstr ,string ," ");
        getval_botsch1.senden(getval_varstr);
        getval_botsch1.senden(CURSOROFF);     //  tempor‰r ausschalten
        /* EasyCODE > */
        while ( getval_botsch1.busycheck() )
        {
        }
        /* EasyCODE < */
        getval_varstr[0] = 0x00; // Nullterminator
        /* EasyCODE > */
        for ( i = 1 ;  (i <=  ((char)(maxchar -1 ))); i++ )
        {
            strcat (getval_varstr, CURSORRIGHT);
        }
        strcat (getval_varstr, "]");
        /* EasyCODE - */
        getval_botsch1.senden(getval_varstr);
        for ( i = 1 ;  (i <=  ((char)(maxchar+1))); i++ )
        {
            getval_botsch1.senden(CURSORLEFT);
        }
        getval_botsch1.senden(CURSORON);
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
stringclear */
/* EasyCODE F */
void stringclear (unsigned char cur_pos,  unsigned char maxchar)
{
    /* EasyCODE < */
    char i;
    // Es wird zuerst der Cursor ganz nach links positioniert, dann wird del line ausgegeben, danach
    // wird die rechte Klammer zu ausgegeben und der Cursor wieder ganz links positioniert.
    getval_botsch1.senden(CURSOROFF);     //  tempor‰r ausschalten
    /* EasyCODE > */
    for ( i = 1 ;  ( i <=  (char)(cur_pos-1) ); i++ )
    {
        getval_botsch1.senden(CURSORLEFT);
    }
    /* EasyCODE < */
    getval_botsch1.senden(DELLINE);
    getval_varstr[0] = 0x00; // Nullterminator
    /* EasyCODE > */
    for ( i = 1 ;  (i <=  (char)(maxchar) ); i++ )
    {
        strcat (getval_varstr, CURSORRIGHT);
    }
    strcat (getval_varstr, "]");
    /* EasyCODE - */
    getval_botsch1.senden(getval_varstr);
    for ( i = 1 ;  (i <=  (char)(maxchar+1) ); i++ )
    {
        getval_botsch1.senden(CURSORLEFT);
    }
    getval_botsch1.senden(CURSORON);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
pinanzeige */
/* EasyCODE F */
void pinanzeige (unsigned char maxchar)
{
    unsigned char i;
    // Es wird der Pinstring und der Pincode ausgegeben, wobei nicht eingegebene Ziffern als _ und
    // eingegebene Ziffern als * dargestellt werden.
    getval_botsch1.senden(pinstring);
    for ( i = 1 ;  (i <=  (anz_pinchar)); i++ )
    {
        getval_botsch1.senden("*");
    }
    /* EasyCODE < */
    // getval_botsch1.senden(CURSOROFF);      //  tempor‰r ausschalten
    /* EasyCODE > */
    for ( i = 1 ;  (i <=  (maxchar - anz_pinchar)); i++ )
    {
        getval_botsch1.senden("-");
    }
    for ( i = 1 ;  (i <=  (maxchar - anz_pinchar)); i++ )
    {
        getval_botsch1.senden(CURSORLEFT);
    }
    /* EasyCODE < */
    // getval_botsch1.senden(CURSORON);   // wieder einschalten
    getval_botsch1.senden(getvalende);   // Skalierung wieder ruecksetzen
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
get_grenzwert */
/* EasyCODE F */
// Anzahl der eingebbaren Ziffern ermitteln
long int get_grenzwert (long int maxwert,long int minwert)
{
    long int maxbetrag;
    if ( minwert>=0 )
    {
        maxbetrag=maxwert;
    }
    else
    {
        if ( maxwert>=-minwert )
        {
            maxbetrag=maxwert;
        }
        else
        {
            maxbetrag=-minwert;
        }
    }
    if ( maxbetrag<=9 )
    {
        return 9;
    }
    if ( maxbetrag<=99 )
    {
        return 99;
    }
    if ( maxbetrag<=999 )
    {
        return 999;
    }
    if ( maxbetrag<=9999 )
    {
        return 9999;
    }
    return 99999;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
pnrwahlupd */
/* EasyCODE F */
// Update des Kommentars bei der Programmnummerwahl
void pnrwahlupd (void)
{
    /* EasyCODE < */
    struct progpar progdat;
    /* EasyCODE > */
    switch ( di.parprog(pargetval.wert,&progdat,DRIVE_C) )
    {
    case 1 :
    case 2 :
        sprintf(varstr,pargetval.sonder2string,DELLINE);
        getval_botsch1.senden(varstr);
        getval_botsch1.senden(&progdat.kommentar[0]);
        break;
    default:
        // nicht vorhanden oder falsche Kennung
        sprintf(varstr,pargetval.sonder2string,DELLINE "--------------------" );
        getval_botsch1.senden(varstr);
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
inv_sprachtaste */
/* EasyCODE F */
void inv_sprachtaste(void)
{
    while ( eing_botsch1.busycheck() )
    {
    }
    switch ( saf->md.sprache )
    {
    case ENGL :
        sprintf(eing.varstring,tinvs ,TENGL);
        break;
    case FRANZ :
        sprintf(eing.varstring,tinvs ,TFRANZ);
        break;
    case SPAN :
        sprintf(eing.varstring,tinvs ,TSPAN);
        break;
    case ITAL :
        sprintf(eing.varstring,tinvs ,TITAL);
        break;
    default:
        sprintf(eing.varstring,tinvs ,TDEUTSCH);
        break;
    }
    eing_botsch1.senden(eing.varstring);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
error_res */
/* EasyCODE F */
void error_res(void)
{
    while ( !koor.eingstop() )
    {
    }
    eing.stopb = TRUE;
    while ( koor.eingstop() )
    {
    }
    eing.stopb = FALSE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
fkeytst */
/* EasyCODE F */
/********** fkeytst **********/
// Dieses UP prueft die Zugriffsberechtigung von Eingabe-Funktionen.
// ( frei, mit Code abhaengig vom User, mit Schl¸ssel )
// 
// Alle Teilfunktionen der Eingabe kˆnnen f¸r verschiedene User gesperrt bzw. freigegeben werden.
// Die einzelnen User sind durch einen PIN gesch¸tzt.
// Soll eine Eingabefunktion ausgef¸hrt werden, f¸r die der aktuelle User keine Zugangsberechtigung hat 
// wird eine PIN - Abfrage ausgelˆst um auf einen anderen User umzuschalten. Wird nun wieder eine Funktion 
// gew‰hlt, die f¸r diesen User (PIN) gesperrt ist wird eine erneute PIN - Abfrage ausgelˆst usw.
// Der aktuell freigeschaltete User bleibt bis zum ausschalten der Maschine erhalten. 
// Nach dem Einschalten der Maschinen ist immer der User 1 aktiv.
// Die Pinabfrage wird solange wiederholt bis der User einen gueltigen und passenden Pincode eingegeben
// hat, oder mit ESC die Pineingabe abbricht.

// Die PIN - Festlegung und die Tabelle der gesperrten Funktionen kann nur durch den Master - 
// User (Schl¸sselschalter) ausgef¸hrt werden.
//
// Sonderfall: fktkz = 200: Aufruf von Parametereingabe um den aktuellen User zu wechseln, keine Abfrage der
//                          Zugriffsberechtigung
//
//
// Eingabeparameter: Kennziffer der Funktion
//
// Funktionsr¸ckmeldungen:
//    0 ... Funktion erlaubt bzw. Master User (Schluesselschalter) ist aktiv. 
//           => BV wurde nicht geaendert !             
//    1 ... Funktion erlaubt bzw. der User hat den entsprechenden Code richtig eingegeben oder
//           Master User (Schluesselschalter) ist aktiv geworden. => BV wurde geaendert !
//    2 ... Funktion nicht erlaubt. (Code falsch oder diese Funktion erfordert einen anderen User PIN).
//           Der User hat mittels ESC die Codeeingabe abgebrochen.    => BV wurde geaendert !
//    3 ... Stopanforderung.

unsigned char fkeytst (unsigned char fktkz)
{
    unsigned char i, pin_gueltig;
    if ( fkey.getZustand() && (fktkz!= 19) && (fktkz != 20) && (fktkz != 7) )
    {
        eing.par.SetFreigabe(ACC3);
        return 0;
    }
    else
    {
        /* EasyCODE < */
        // Schluesselschalter steckt nicht
        /* EasyCODE > */
        if ( (akt_user >= FKEY_USER_MAX) || ((fktkz >= FKEY_FUNK_MAX)&&(fktkz!=200)) )
        {
            /* EasyCODE < */
            // Range check
            /* EasyCODE > */
            return 2;
        }
        else
        {
            /* EasyCODE ( 0 
            Pin eingeben */
            if ( (fktkz!=200) && (fktkz!=7) && (saf->md.fkey_array [akt_user] [fktkz] == 0) )
            {
                /* EasyCODE < */
                // frei
                /* EasyCODE > */
                return 0;
            }
            else
            {
                /* EasyCODE ( 0 
                Pin-Abfrage */
                /* EasyCODE < */
                // User hat keine Berechtigung, Pinabfrage einleiten.
                pargvpin.auftrag = 1;   // getvalue und piktos
                pargvpin.sonderstring = pinpiktos;
                pargvpin.anz_char = 4;
                // codenr_arr  [FKEY_USER_MAX];                     // akt_user
                /* EasyCODE > */
                while ( TRUE )
                {
                    switch ( getvalue_pin() )
                    {
                    case 0 :
                        if ( pargvpin.wert == 1861 )
                        {
                            eing.par.SetFreigabe(ACC4);
                            akt_user = FKEY_USER_MAX-1;
                            return 1;
                        }
                        else
                        {
                            //    0 ... Eingabebest‰tigung mit Enter, PIN ist eingegeben
                            pin_gueltig = FALSE;
                            for ( i = 0 ;  (i <  (FKEY_USER_MAX)); i++ )
                            {
                                if ( saf->md.codenr_arr[i] == pargvpin.wert )
                                {
                                    pin_gueltig = TRUE;
                                    akt_user = i;
                                    break;
                                }
                            }
                            if ( fkey.getZustand()&& (fktkz != 19) && (fktkz != 20) && (fktkz != 7) )
                            {
                                eing.par.SetFreigabe(ACC3);
                                return 1;
                            }
                            else
                            {
                                /* EasyCODE < */
                                // Schluesselschalter steckt immer noch nicht
                                /* EasyCODE > */
                                if ( fktkz != 200 )
                                {
                                    /* EasyCODE ( 0 
                                    Freigabe pruefen */
                                    if ( pin_gueltig )
                                    {
                                        eing.par.SetFreigabe(akt_user+1);
                                        switch ( saf->md.fkey_array [akt_user] [fktkz] )
                                        {
                                        case 0 :
                                            /* EasyCODE < */
                                            // frei
                                            /* EasyCODE > */
                                            return 1;
                                        default:
                                            /* EasyCODE < */
                                            pargvpin.auftrag = 2;   // Nur Piktos
                                            /* EasyCODE > */
                                            pargvpin.sonderstring = fnk_inv;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        pargvpin.auftrag = 2;   // Nur Piktos
                                        pargvpin.sonderstring = pin_inv;
                                    }
                                    /* EasyCODE ) */
                                    break;
                                }
                                else
                                {
                                    eing.par.SetFreigabe(akt_user+1);
                                    return 1;
                                }
                            }
                        }
                    case 1 :
                        /* EasyCODE < */
                        //    1 ... Abbruch durch ESC, PIN ist ung¸ltig
                        /* EasyCODE > */
                        return 2;
                    case 2 :
                        /* EasyCODE < */
                        //    2 ... Stopanforderung, PIN ist ung¸ltig
                        /* EasyCODE > */
                        return 3;
                    default:
                        break;
                    }
                }
                /* EasyCODE ) */
            }
            /* EasyCODE ) */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
