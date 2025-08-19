/* EasyCODE V8 */
/* EasyCODE ( 0 */
/* EasyCODE < */
/* Datei: DTEXTE.C     Klasse: 3590       Ersteller: Halfmann EES 2856

Kurzbeschreibung:
================
In dieser Datei befinden sich die Anzeigentexte in Deutsch

Die invertierte Darstellung am Bedienfeld soll immer ausgeschaltet sein. D.h.
wird in einem String die invertierte Darstellung eingeschaltet, muá im gleichen
String die invertierte Darstellung auch wieder ausgeschaltet werden.

Alle Strings mssen ihre Cursorpositionierung selbst ausfhren.
*/

#pragma warn -aus
#include "textport.h"
#include "touch.h"
/* EasyCODE > */
/* EasyCODE ( 0 
main */
/* EasyCODE F */
void main ()
{
    /* EasyCODE < */
    struct text dtexte = {
    
    // unsigned char * textanf =
    {"@@@ANF"},       // Textanfang
    // Koordinatortexte
    // unsigned char * ausschalts  =
    {Z6 "MASCHINE KURZ AUSSCHALTEN "},
    
    // unsigned char * koor2  =
    {ESCS "RC" TENTERS Z3 "LADEN DER FIRMWARE PORTAL LŽUFT" DELLINE Z4 DELLINE},
    
    // unsigned char * errs  =
    {Z3 "FEHLER :  %3u"},
    
    // Fehlermeldungen fr faterror
    // unsigned char * faterrors []  =
    {
    { Z3 "FEHLER :  BETRIEBSDATEN CHECKSUMME"
    Z4 "          (KALTSTART AUSGEFšHRT)"},
    { Z3 "NEUE BETRIEBSSOFTWARE"
    Z4 "(KALTSTART AUSGEFšHRT)"},
    { Z3 "KALTSTART AUSGEFšHRT"},
    },
    
    // Fehlermeldungen für den Koordinator
    // unsigned char * koorerrs []  =
    {
    {Z3 "   "},           //koor_err = 50
    },
    
    // Fehlermeldungen für den Ablauf
    // unsigned char * naeerrs []  =
    {
    {Z3 "   "},           //nae_err = 50
    },
    // unsigned char * zuferrs []  =
    {
    {Z3 "   "},           //zuf_err = 50
    },
    // unsigned char * staperrs []  =
    {
    {Z3 "   "},           //stap_err = 50
    },
    
    // Fehlermeldungen für Eingabe
    // unsigned char * eingerrs []  =
    {
    {Z3 "PROGRAMM %2u NICHT IM SPEICHER"},          //eing_err = 50
    {Z3 "SCHABLONENTYP GEWECHSELT"},                //eing_err = 51
    },
    
    // Fehlermeldungen fr io_err
    // unsigned char * ioerr_urs  =
    {Z3 "FEHLER :  %-s"},
    // Fehlermeldungen fr Unterfehler
    // unsigned char * secerr_urs  =
    {Z5 "URSACHE : %-s"},
    // unsigned char * eingkaltstart =
    { Z3 "KALTSTART AUSFšHREN ? "},
    // unsigned char * eing_cmd1 =
    {ESCS "=010160""%s šBERSCHREIBEN ?"},
    // unsigned char * eing_cmd3 =
    {ESCS "=010150" ESCS "S11"" SD-CARD EINLEGEN" ESCS "S11"
     ESCS "=010164" ESCS "S11"" ( ALLE DATEIEN WERDEN GEL™SCHT ! )" ESCS "S11"},
    
    // Hinweis beim Programm ueberschreiben
    // unsigned char * proghin1  =
    { Z3 "PROGRAMM %4u  šBERSCHREIBEN ?"},
    // Hinweis beim Programm l”schen
    // unsigned char * proghin2  =
    { ESCS "=010160" "PROGRAMM %s  L™SCHEN ?"},
    
    // unsigned char * progtext1  =
    { Z3 "STICHE GENERIEREN ?"},
    // unsigned char * progtext2 =
    { Z3 "SYMMETRIEPUNKT EINGEBEN"},
    
    // unsigned char * textend =
    {"@@@END"},              // Textende
    };
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ) */
