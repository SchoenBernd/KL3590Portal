/* EasyCODE V8 */
/* EasyCODE ( 0 
ftexte - fuer die KL3590 */
/* EasyCODE < */
/* Datei: FTEXTE.C     Klasse: 3590 mit Touch       Ersteller: Halfmann EES 2856

Kurzbeschreibung:
================
In dieser Datei befinden sich die Anzeigentexte in Französisch.
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
    struct text ftexte = {
    
    // unsigned char * textanf =
    {"@@@ANF"},       // Textanfang
    
    // Koordinatortexte
    // unsigned char * ausschalts  =
    {Z6 "TEINDRE MACHINE BRIEVEMENT"},
    
    // unsigned char * koor2  =
    {ESCS "RC" TENTERS Z3 "FIRMWARE PORTAL SE CHARGE"  DELLINE Z4 DELLINE},
    
    // unsigned char * errs  =
    {Z3 "ERREUR :  %3u"},
    
    // Fehlermeldungen fr faterror
    // unsigned char * faterrors []  =
    {
    { Z3 "ERREUR :  CHECK-LIST CARACTER. DE REGIME"
    Z4 "          (DEMARRAGE A FROID EFFECTUE)"},
    { Z3 "NOUVEAU LOGICIEL D'EXPLOITATION"
    Z4 "(DEMARRAGE A FROID EFFECTUE)"},
    { Z3 "DEMARRAGE A FROID EFFECTUE"},
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
    {Z3 "PROGRAMME %2u PAS EN MEMOIRE"},          //eing_err = 50
    {Z3 "TYP DE GABARIT CHANGE"},                 //eing_err = 51
    },
    
    // Fehlermeldungen fr io_err
    // unsigned char * ioerr_urs  =
    {Z3 "ERREUR :  %-s"},
    
    
    // Fehlermeldungen fr Unterfehler
    // unsigned char * secerr_urs  =
    {Z5 "CAUSE : %-s"},
    // unsigned char * eingkaltstart =
    { Z3 "EFFECTUER UN DEMARRAGE A FROID ? "},
    // unsigned char * eing_cmd1 =
    {ESCS "=010160""REMPLACER %s ?"},
    // unsigned char * eing_cmd3 =
    {ESCS "=010150" ESCS "S11"" INSRER SD-CARD" ESCS "S11"
     ESCS "=010164" ESCS "S11"" (TOUT VA ETRE EFFACER)" ESCS "S11"},
    
    // Hinweis beim Programm ueberschreiben
    // unsigned char * proghin1  =
    { Z3 "REMPLACER PROGRAMME %4u ?"},
    // unsigned char * proghin2  =
    { ESCS "=010160" "EFFACER PROGRAMME %s ?"},    
    
    // unsigned char * progtext1  =
    { Z3 "CREER DES POINTS ?"},
    // unsigned char * progtext2 =
    { Z3 "ENTRER POINT DE SYMETRIE"},
    
    // unsigned char * textend =
    {"@@@END"},              // Textende
    };
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ) */
