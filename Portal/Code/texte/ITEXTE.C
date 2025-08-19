/* EasyCODE V8 */
/* EasyCODE ( 0 
itexte.c - fuer die KL3590 */
/* EasyCODE < */
/* Datei: ITEXTE.C     Klasse: 3590 mit Touch       Ersteller: Halfmann EES 2856

Kurzbeschreibung:
================
In dieser Datei befinden sich die Anzeigentexte in Italienisch.
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
    struct text itexte = {
    
    // unsigned char * textanf =
    {"@@@ANF"},       // Textanfang
    // Koordinatortexte
    // unsigned char * ausschalts  =
    {Z6 "SPEGNERE BREVEMENTE MACCHINA"},
    
    // unsigned char * koor2  =
    {ESCS "RC" TENTERS Z3 "SCARICAMENTO PORTAL FIRMWARE"  DELLINE Z4 DELLINE},
    
    // unsigned char * errs  =
    {Z3 "ERRORE :  %3u"},
    
    // Fehlermeldungen fr faterror
    // unsigned char * faterrors []  =
    {
    {Z3 "ERR.: SOMMA DI CONTROLLO DATI ESERCIZIO"
    Z4 "      (ESEGUITA RI-INIZIALIZZAZIONE)"},
    {Z3 "NUOVO SOFTWARE DI ESERCIZIO"
    Z4 "(ESEGUITA RI-INIZIALIZZAZIONE)"},
    {Z3 "ESEGUITA RI-INIZIALIZZAZIONE"},
    },
    
    // Fehlermeldungen fr den Koordinator
    // unsigned char * koorerrs []  =
    {
    {Z3 "   "},           //koor_err = 50
    },
    
    // Fehlermeldungen fr den Ablauf
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
    
    
    // Fehlermeldungen fr Eingabe
    // unsigned char * eingerrs []  =
    {
    {Z3 "PROGRAMMA %2u NON IN MEMORIA"},          //eing_err = 50
    {Z3 "CAMBIATO  MODIFICARE"},                  //eing_err = 51
    },
    
    // Fehlermeldungen fr io_err
    // unsigned char * ioerr_urs  =
    {Z3 "ERRORE :  %-s"},
    
    
    // Fehlermeldungen fr Unterfehler
    // unsigned char * secerr_urs  =
    {Z5 "CAUSA : %-s"},
    // unsigned char * eingkaltstart =
    { Z3 "ESEGUIRE RI-INIZIALIZZAZIONE ? "},
    // unsigned char * eing_cmd1 =
    {ESCS "=010160""SOVRASCRIVERE %s ?"},
    // unsigned char * eing_cmd3 =
    {ESCS "=010150" ESCS "S11"" SD-CARD EINLEGEN" ESCS "S11"
     ESCS "=010164" ESCS "S11"" ( ALLE DATEIEN WERDEN GEL™SCHT ! )" ESCS "S11"},        
    
    // Hinweis beim Programm ueberschreiben
    // unsigned char * proghin1  =
    { Z3 "SOVRASCRIVERE PROGRAMMA %4u ?"},
    // unsigned char * proghin2  =
    { ESCS "=010160" "CANCELLARE PROGRAMMA %s ?"},       
    
    // unsigned char * progtext1  =
    { Z3 "GENERARE PUNTI ?"},
    // unsigned char * progtext2 =
    { Z3 "INSERIRE PUNTO DI SIMMETRIA"},
    
    // unsigned char * textend =
    {"@@@END"},              // Textende
    };
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ) */
