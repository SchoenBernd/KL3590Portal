/* EasyCODE V8 */
/* EasyCODE ( 0 
stexte.c - fuer die KL3590 */
/* EasyCODE < */
/* Datei: STEXTE.C     Klasse: 3590       Ersteller: Halfmann EES 2856

Kurzbeschreibung:
================
In dieser Datei befinden sich die Anzeigentexte in Spanisch.
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
    struct text stexte = {
    
    // unsigned char * textanf =
    {"@@@ANF"},       // Textanfang
    // Koordinatortexte
    // unsigned char * ausschalts  =
    {Z6 "DESCONECTAR MAQUINA BREVEMENTE"},
    
    // unsigned char * koor2  =
    {ESCS "RC" TENTERS Z3 "DESCARGAMENTO FIRMWARE PORTAL MARCHA"  DELLINE Z4 DELLINE},
    
    // unsigned char * errs  =
    {Z3 "ERROR :  %3u"},
    
    // Fehlermeldungen fr faterror
    // unsigned char * faterrors []  =
    {
    { Z3 "ERROR :  DATOS DE REGIMEN SUMA CHEQUEO"
    Z4 "         (ARRANQUE EN FRIO REALIZADO)"},
    { Z3 "NUEVO SOFTWARE OPERADOR"
    Z4 "(ARRANQUE EN FRIO REALIZADO)"},
    { Z3 "ARRANQUE EN FRIO REALIZADO"},
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
    {Z3 "PROGRAMA %2u NO ESTA EN MEMORIA"},          //eing_err = 50
    {Z3 "PLANTILLA MODIFICAR"},                      //eing_err = 51
    },
    
    // Fehlermeldungen fr io_err
    // unsigned char * ioerr_urs  =
    {Z3 "ERROR :  %-s"},
    
    
    // Fehlermeldungen fr Unterfehler
    // unsigned char * secerr_urs  =
    {Z5 "CAUSA : %-s"},
    // unsigned char * eingkaltstart =
    { Z3 "REALIZAR ARRANQUE EN FRIO ? "},
    // unsigned char * eing_cmd1 =
    {ESCS "=010160""SOBREESCRIBIR %s ?"},
    // unsigned char * eing_cmd3 =
    {ESCS "=010150" ESCS "S11"" PONOR SD-CARD" ESCS "S11"
     ESCS "=010164" ESCS "S11"" ( TODOS ARCHIVOS SE BORRAN ! )" ESCS "S11"},        
    
    // Hinweis beim Programm ueberschreiben
    // unsigned char * proghin1  =
    { Z3 "SOBREESCRIBIR PROGRAMA %4u ?"},
    // unsigned char * proghin2  =
    { ESCS "=010160" "BORRAR PROGRAMA %s ?"},
    
    // unsigned char * progtext1  =
    { Z3 "PRODUCIR PUNTADAS ?"},
    // unsigned char * progtext2 =
    { Z3 "INTRODUCIR PUNTO DE SIMETRIA"},
    
    // unsigned char * textend =
    {"@@@END"},              // Textende
    };
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ) */
