/* EasyCODE V8 */
/* EasyCODE ( 0 
etexte fuer die KL3590 */
/* EasyCODE < */
/* Datei: ETEXTE.C     Klasse: 3590       Ersteller: Halfmannn EES 2856

Kurzbeschreibung:
================
In dieser Datei befinden sich die Anzeigentexte in Englisch.*/

#pragma warn -aus
#include "textport.H"
#include "touch.h"
/* EasyCODE > */
/* EasyCODE ( 0 
main */
/* EasyCODE F */
void main ()
{
    /* EasyCODE < */
    struct text etexte = {
    
    // unsigned char * textanf =
    {"@@@ANF"},       // Textanfang
    // Koordinatortexte
    // unsigned char * ausschalts  =
    {Z6"SWITCH MACHINE OFF BRIEFLY"},
    
    // unsigned char * koor2  =
    {ESCS "RC" TENTERS Z3 "DOWNLOADING FIRMWARE PORTAL IS RUNNING"  DELLINE Z4 DELLINE},
    
    // unsigned char * errs  =
    {Z3 "ERROR :  %3u"},
    
    // Fehlermeldungen fr faterror
    // unsigned char * faterrors []  =
    {
    { Z3 "ERROR :  OPERATIONAL DATA CHECK SUM"
    Z4 "         (COLD START CARRIED OUT)"},
    { Z3 "NEW OPERATING SOFTWARE"
    Z4 "(COLD START CARRIED OUT)"},
    { Z3 "COLD START CARRIED OUT"},
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
    {Z3 "PROGRAMM %2u NOT IN MEMORY"},          //eing_err = 50
    {Z3 "CLAMP TYPE CHANGED"},                  //eing_err = 51
    },
    
    // Fehlermeldungen fr io_err
    // unsigned char * ioerr_urs  =
    {Z3 "ERROR :  %-s"},
    
    
    // Fehlermeldungen fr Unterfehler
    // unsigned char * secerr_urs  =
    {Z5 "CAUSE : %-s"},
    // unsigned char * eingkaltstart =
    { Z3 " CARRY OUT COLD START ? "},
    // unsigned char * eing_cmd1 =
    {ESCS "=010160""OVERWRITE %s ?"},
    // unsigned char * eing_cmd3 =
    {ESCS "=010150" ESCS "S11"" INSERT SD-CARD " ESCS "S11"
     ESCS "=010164" ESCS "S11"" ( ALL FILES WILL BE DELETED ! )" ESCS "S11"},
    
    // Hinweis beim Programm ueberschreiben
    // unsigned char * proghin1  =
    { Z3 "WRITE OVER PROGRAM %4u ?"},
    // unsigned char * proghin2  =
    { ESCS "=010160" "DELETE PROGRAM %s ?"},
    
    // unsigned char * progtext1  =
    { Z3 "GENERATE STITCHES ?"},
    // unsigned char * progtext2 =
    { Z3 "ENTER SYMMETRY POINT"},
    
    // unsigned char * textend =
    {"@@@END"},              // Textende
    };
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ) */
