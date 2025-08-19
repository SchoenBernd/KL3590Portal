/* EasyCODE V8 */
/* EasyCODE ( 0 
texte.c */
/* Datei: TEXTE.C     Klasse: 3590 Portal      Ersteller: Halfmann EWE 2856

Kurzbeschreibung:
================
In dieser Datei befinden sich die sprachunabhaengigen Texte fuer obige Maschine.

*/

#include "portal.H"


unsigned char * gibtsnicht  = {Z3 "Diese Seite gibts noch nicht"};
unsigned char * hupe_ion = {BELLBELL ION};
// Achtung: Strings mit Piktos, welche wiederholt und ohne BV-Wechsel dazwischen aufgerufen werden, 
//          muessen auch den Pikto-loesch-Befehl am Anfang beinhalten (ESCS "Rc" ...) ! 
//          Dies ist wichtig, damit das Touch nicht ueberlaeuft und undefiniert abstuerzt.
//          Das Touch merkt sich alle angezeigten Piktos, auch wenn sie uebereinander liegen.
//          Bei 255 angezeigten Piktos stirbt das Touch.
/* EasyCODE ( 0 
Strings fuer Commander */
unsigned char * dirs = {"%8s%4u %-10s"};
unsigned char * dir1s = {"%8s%-14s"};

unsigned char * dirloeschs = {"%8s              "};
unsigned char * dirstatus [2]= 
{
{ESCS "=030068""%3u / %-3u Files"},
{ESCS "=195068""%3u / %-3u Files"}
};
unsigned char * drives [4]=
{
{ESCS "=030050" " C:""\x5c""DATEN""\x5c"},
{ESCS "=030050" " C:""\x5c""      "},
{ESCS "=195050"" SD:""\x5cPortal\x5c"},
{ESCS "=195050"" LAN:""\x5cPortal\x5c"}

};
unsigned char * lans = {ESCS "=195050"" LAN"};

unsigned char * dirposs[2][DIRZ_MAX] = 
{
{   
{ESCS "=005084"},
{ESCS "=005098"},
{ESCS "=005112"},
{ESCS "=005126"},
{ESCS "=005140"},
{ESCS "=005154"},
{ESCS "=005168"}
},
{   
{ESCS "=165084"},
{ESCS "=165098"},
{ESCS "=165112"},
{ESCS "=165126"},
{ESCS "=165140"},
{ESCS "=165154"},
{ESCS "=165168"}
},
};
unsigned char * wrongcarderr =    {ESCS "=190066"" WRONG CARD    "};
unsigned char * nocarderr =       {ESCS "=190066"" NO CARD       "};
unsigned char * writeprotecterr = {ESCS "=190066""WRITE PROTECTED"};
unsigned char * sdcarderr =    {ESCS "=190066"" CARD ERROR (%u)"};
unsigned char * dialogfenster = {ESCS "Rc" PSTERN ESCS "Rc"PCURSORAB
                                 ESCS "RC" TSTERNS ESCS "RC" TSTERN1S ESCS "RC" TSCRDOWNS ESCS "RC" TSCRDOWN1S
                                 ESCS "Rc"PCOPY ESCS "Rc"PDELETE ESCS "Rc"PMDATSCHREIBEN ESCS "Rc"PFORMAT 
                                 ESCS "RC" TCOPYS ESCS "RC" TDELS ESCS "RC" TMDATS ESCS "RC" TFORMATS
                                 ESCS "=000120\x14"
                                 ESCS "DL2" ESCS "DR001120""318119""1"};
unsigned char * dialogcopy0 = {ESCS "F001" ESCS "=010129""COPY %s TO %s" ESCS "F1"};
unsigned char * dialogcopy1 = {ESCS "F001" ESCS "=010129""COPY %s TO SD-CARD" ESCS "F1"};
unsigned char * dialogcopy2 = {ESCS "=010160""                                   "};
unsigned char * dialogcopy3 = {ESCS "F001" ESCS "=010129""COPY %s FROM SD-CARD" ESCS "F1"};
unsigned char * dialogtasten0 = {ESCS "RT003184""078047""4" TESCS ESCS "P"PESC"030""200""0""255"
                                 ESCS "RT240184""078047""4" TENTERS ESCS "P"PENTER"260""200""0""255"};
unsigned char * dialogtasten1 = {ESCS "RT082184""078047""4" TNEXTS ESCS "P"PNEXT"103""200""0""255"
                                 ESCS "RT161184""078047""4" TALLS ESCS "P"PALL"188""200""0""255"};
unsigned char * dialogtasten2 = {ESCS "RC" TESCS ESCS"RC" TENTERS ESCS"RC" TNEXTS  ESCS "RC" TALLS
                                 ESCS "Rc"PESC ESCS"Rc"PENTER ESCS"Rc"PNEXT ESCS"Rc"PALL};
unsigned char * dialogdel0 = {ESCS "F001" ESCS "=010129""DELETE %s" ESCS "F1"};
unsigned char * dialogstatus0 = {ESCS "F001" ESCS "=010129" "LOADING DIRECTORY FROM FLASH" ESCS "F1"};
unsigned char * dialogstatus1 = {ESCS "F001" ESCS "=010129" "LOADING DIRECTORY FROM DISK " ESCS "F1"};
unsigned char * dialogstatus2 = {ESCS "F001" ESCS "=010129" "LOADING DIRECTORY FROM SD-CARD" ESCS "F1"};
unsigned char * dialogformat0 = {ESCS "F001" ESCS "=010129""FORMAT DISK A:" ESCS "F1"};
unsigned char * dialogformat1 = {ESCS "F001" ESCS "=010129""FORMAT SD-CARD, PLEASE WAIT" ESCS "F1"};
unsigned char * dialogformat2 = {ESCS "RT003184""078047""4" TESCS ESCS "P"PESC"030""200""0""255"
                                 ESCS "RT240184""078047""4" TENTERS ESCS "P"PENTER"260""200""0""255"};
unsigned char * dialogformat3 = {ESCS "=010150" ESCS "S11""                                   " ESCS "S11"
                                 ESCS "=010164" ESCS "S11""                                   " ESCS "S11"
                                 ESCS "RC" TESCS ESCS"RC" TENTERS ESCS "Rc"PESC ESCS"Rc"PENTER};
/* EasyCODE - */
unsigned char * laufwerk = {ESCS "P" "%s" "172""050""0""255"};
/* EasyCODE - */
unsigned char * lwwechsel1s = {ESCS "P" PSDCARD "192""006""0""255"};
unsigned char * lwwechsel2s = {ESCS "=195015"" LAN"};
/* EasyCODE ) */
/* EasyCODE ( 0 
Koordinator */
unsigned char * ioerrs [20]  =
{                            // Index
      {"Y30 (1)"},           //  1
      {"Y50.1 (1)"},         //  2         
      {"Y50.2 (1)"},         //  3
      {"Y51.1 (1)"},         //  4
      {"Y51.2 (1)"},         //  5
      {"Y2FKT (1)"},         //  6
      {"Y7 (1)"},            //  7
      {"Y32 (1)"},           //  8
      {"Y54 (0)"},           //  9
      {"Y54 (1)"},           // 10 
      {"Y33 (0)"},           // 11  
      {"Y33 (1)"},           // 12  
      {"Y2FKT_AUS"},         // 13
      {"Y2FKT_EIN"},         // 14 
      {"      "},            // 15 Reserve
      {"      "},            // 16 Reserve
      {"      "},            // 17 Reserve
      {"      "},            // 18 Reserve
      {"      "},            // 19 Reserve
      {"      "},            // 20 Reserve
};

unsigned char * secerrs [50]  =
{                            // Index
      {"E50.1 (0)"},         //  3
      {"E50.1 (1)"},         //  4
      {"E50.2 (0)"},         //  5
      {"E50.2 (1)"},         //  6
      {"E51.2_1 (0)"},       //  5
      {"E51.2_1 (1)"},       //  6
      {"E51.2_2 (0)"},       //  7
      {"E51.2_2 (1)"},       //  8
      {"E33 (0)"},           //  9
      {"E33 (1)"},           // 10
      {"E75 (0)"},           // 11
      {"E75 (1)"},           // 12
      {"E76 (1)"},           // 13
      {"E50.1_E50.2 (1)"},   // 14
      {"E79 (0)"},           // 15
      {"E79 (1)"},           // 16 
      {"E80 (0)"},           // 17
      {"E80 (1)"},           // 18 
      {"E54 (0)"},           // 19 
      {"E54 (1)"},           // 20 
      {"E2 (0)"},            // 21 
      {"E2 (1)"},            // 22 
      {"E81 (1)"},           // 23 
      {""},                  // 24 Reserve
      {"Y8 (0)"},            // 25 
      {"Y8 (1)"},            // 26 
      {""},                  // 27 Reserve
      {""},                  // 28 Reserve
      {""},                  // 29 Reserve
      {""},                  // 30 Reserve
      {""},                  // 31 Reserve
      {""},                  // 32 Reserve
      {""},                  // 33 Reserve
      {""},                  // 34 Reserve
      {""},                  // 35 Reserve
      {""},                  // 36 Reserve
      {""},                  // 37 Reserve
      {""},                  // 38 Reserve
      {""},                  // 39 Reserve
      {"CARPOSOK (1)"},      // 40
      {"1/2 NEEDLES"},       // 41
      {"CUTREADY (1)"},      // 42
      {"NEEDLEPOS (1)"},     // 43
      {"NMSTEHT (1)"},       // 44
      {"FOOT UP"},           // 45 Reserve
      {"FOOT DOWN"},         // 46 Reserve
      {""},                  // 47 Reserve
      {""},                  // 48 Reserve
      {""},                  // 49 Reserve
      {""},                  // 50 Reserve
};
/* EasyCODE - */
unsigned char * errpars = {"   (#%2u)"};
unsigned char * errpar2s = {"   (#%2u)   (#%2u)"};
unsigned char * errpar3s = {"   (#%2u)   (#%2x)"};
unsigned char * errpiktos = {ESCS "P" "%s" "150""080""0""255"};
unsigned char * touchinv = {ESCS "RI" "%s"};
unsigned char * piktoinv = {ESCS "Ri" "%s"};
unsigned char * touchdel = {ESCS "RC" "%s"};
unsigned char * pikdel = {ESCS "Rc" "%s"};
#ifdef NF120x70
    // Naehfeld 1200x700
    unsigned char * powerons = {CLRHOME FONT1 ESCS "=040050" ESCS "S22" "Portal 1200x700"
                                ESCS "=040100" "   Power On   " ESCS "S11"};
#endif //
#ifdef NF120x170
    // Naehfeld 1200x1700
    unsigned char * powerons = {CLRHOME FONT1 ESCS "=040050" ESCS "S22" "Portal 1200x1700"
                                ESCS "=040100" "   Power On   " ESCS "S11"};
#endif //
#ifdef NF100x50
    unsigned char * powerons = {CLRHOME FONT1 ESCS "=040050" ESCS "S22" "Portal 1000x500"
                                ESCS "=040100" "   Power On   " ESCS "S11"};
#endif //
#ifdef NF120x200
    // Naehfeld 1200x2000
    unsigned char * powerons = {CLRHOME FONT1 ESCS "=040050" ESCS "S22" "Portal 1200x2000"
                                ESCS "=040100" "   Power On   " ESCS "S11"};
#endif //
unsigned char * poweron2s = {ESCS "=040175" "Software: %s"};
/* EasyCODE ) */
/* EasyCODE ( 0 
Zulaessig-Strings */
/* EasyCODE < */
unsigned char * zulloesch = {ESCS "F001" Z1 "            " ESCS "F1"};
unsigned char * zulis  = {ESCS "F001" Z1 "  %li - %li" ESCS "F1"}; 
unsigned char * zulf1s = {ESCS "F001" Z1 "  %.1f - %.1f"  ESCS "F1"};
unsigned char * zulf2s = {ESCS "F001" Z1 " %.2f - %.2f"  ESCS "F1"};
unsigned char * zulf3s = {ESCS "F001" Z1 " %.3f - %.3f" ESCS "F1"};
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE ( 0 
Strings fuer getvalue-pincode */
/* EasyCODE < */
unsigned char * pinpiktos = {ESCS "Rc" P_PIN  ESCS "Rc" P_FUNK ESCS "P" P_PIN "0300700255" 
ESCS "P" P_FUNK "0301300255"} ; 
unsigned char * pinstring = {ESCS "S21" ESCS "=010006PIN: "};      
unsigned char * pin_inv = {ESCS "Rc" P_PIN  ESCS "Rc" P_FUNK ESCS "P" P_PIN "0300700255" 
ESCS "P" P_FUNK "0301300255" ESCS "Ri" P_PIN BELLBELL }; 

unsigned char * fnk_inv = {ESCS "Rc" P_PIN  ESCS "Rc" P_FUNK ESCS "P" P_PIN "0300700255" 
ESCS "P" P_FUNK "0301300255"ESCS "Ri" P_FUNK BELLBELL }; 

// unsigned char * sanduhrs = {ESCS"Rc%03u" ESCS "P%03u%03u%03u0255"};
unsigned char * sanduhrs = {ESCS"Rc%s" ESCS "P%s%03u%03u0255"};

// unsigned char * sanddels = {ESCS"Rc%03u"};
unsigned char * sanddels = {ESCS"Rc%s"};

unsigned char * getvalende = {CURSOROFF ESCS "S11"};
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE ( 0 
Eingabe */
/* EasyCODE < */
//unsigned char * progsaveps = {ESCS "P" P_SAVEAS "005""004""0""255"};
//unsigned char * pnrprogs = {ESCS "=057007" ESCS "S12" "%3u" ESCS "S11"};
//unsigned char * pnrprog1s = {ESCS "=119005" "%s" };
//unsigned char * pnrprog2s = {ESCS "P" PPROG "021""112""0""255" ESCS "=056111" ESCS "S12" "%-3u" ESCS "S11"};  // Save as Sonders
unsigned char * eings = {ESCS "RT" "281000" "039039" "4" TEINGABES ESCS "P" PEINGABE "2870040255" ESCS "P" PMENUEKLEIN "3070280255"};
/* EasyCODE > */
// Info
unsigned char * tinvs = {ESCS "RI%03d"};
unsigned char * pinvs = {ESCS "Ri%03d"};
unsigned char * tdels = {ESCS "RC%03d"};
unsigned char * swversions = {ESCS "=013050SOFTWARE : ""%s" 
ESCS "=013064FIRMWARE : ""%s" 
ESCS "=013078DISPLAY  : ""%s"
ESCS "=013092MOTOR    : ""%x,   79-001 1%s"
ESCS "=033120" CHRCODE "\x18" ": %5u"};
unsigned char * stundens = {ESCS "=040190""%4.2f h" ESCS "=202190""%4.2f h"};
unsigned char *ipadrs = {ESCS "=197120" "%s"};
unsigned char *vncstarts = {ESCS "Rc" PSTOP ESCS "P" PSTART "2221400255"};
unsigned char *vncstops = {ESCS "Rc" PSTART ESCS "P" PSTOP "2221400255"};
unsigned char *vncruns = {ESCS "EP" "\\NOR Flash\\CeVncSvr.exe\r"};
unsigned char *vncnotruns = {ESCS "Ep" "\\NOR Flash\\CeVncSvr.exe\r"};
unsigned char *vncPCruns = {ESCS "EP" "d:\\UltraVNC\\winvnc.exe\r"};
unsigned char *vncPCnotruns = {ESCS "Ep" "d:\\UltraVNC\\winvnc.exe\r"};
/* EasyCODE - */
// Programmnummer-Eingabe
unsigned char * pnrps = {ESCS "P" PPNR "020""008""0""255"};
unsigned char * pnrs = {ESCS "=057007" ESCS "S12" "%4u" ESCS "S11"};
unsigned char * pnrkomms = {ESCS "=130007" "%s" };
unsigned char * pnrp2s = {ESCS "RT""001""193""079""047""4" TSEQS ESCS "P" PSEQ "028""204""0""255"
                          ESCS "RT""081""193""079""047""4" TVPNRS ESCS "P" PVPNR "109""204""0""255"
                          ESCS "RT""161""193""079""047""4" TPAUTOS ESCS "P" PPAUTO "188""204""0""255"};

// Klammercode-Eingabe
unsigned char * klcodeps = {ESCS "P" PCODE "020""015""0""255"};
// Stichlaengen-Eingabe
unsigned char * stips = {ESCS "P" PSTI "020""008""0""255"};
// Standardstichlaengen-Eingabe
unsigned char * ststips = {ESCS "P" PSTANDARDSTICHL "020""008""0""255"};
// Stichbreiten-Eingabe
unsigned char * stibreiteps = {ESCS "P" PSTICHBREITE "020""008""0""255"};
// Huepferfuss-Hub-Eingabe
unsigned char * hfhubps = {ESCS "P" PFUSS "020""008""0""255"};
// Huepferfuss-Niveau-Eingabe
unsigned char * hflevelps = {ESCS "P" PHFLEVEL "020""008""0""255"};
// Zeit-Eingabe
unsigned char * zeitps = {ESCS "P" P_TIME "020""008""0""255"};
// Verschiebe-Parameter
unsigned char * vparps = {ESCS "P" PVPAR "020""008""0""255"};
// Drehzahl-Eingabe
unsigned char * gswps = {ESCS "P" PGSWMAX "020""008""0""255"};
// Unterfaden-Vorgabe
unsigned char * ufwps = {ESCS "P" PCOUNTER "020""008""0""255"};
unsigned char * ufwp2s = {ESCS "RT""001""193""079""047""4" TCLEAR2S ESCS "P" PCLEAR "021""209""0""255"};
// ScaleX-Eingabe
unsigned char * scalexps = {ESCS "P" PSCALEX "020""008""0""255"};
unsigned char * scaleyps = {ESCS "P" PSCALEY "020""008""0""255"};
unsigned char * drehwinkelps = {ESCS "P" PDREHEN "020""008""0""255"};
// Symmetriepunkt_Eingabe
unsigned char * symxps = {ESCS "P" P_X "020""008""0""255"};
unsigned char * symyps = {ESCS "P" P_Y "020""008""0""255"};
unsigned char * symxp2s = {ESCS "RT""001""193""079""047""4" TXS ESCS "P" P_X "028""204""0""255"};
unsigned char * symyp2s = {ESCS "RT""001""193""079""047""4" TYS ESCS "P" P_Y "028""204""0""255"};
/* EasyCODE < */
// Sequenz-/Verbundene Programme-Eingabe
unsigned char * pnrloeschs = {ESCS "Rc" PPNR ESCS "=057007" ESCS "S12" "    " ESCS "S11" DELLINE};
unsigned char * seqps = {ESCS "Rc" PSEQ ESCS "Rc" PVPNR ESCS "Rc" PPNR
                         ESCS "P" PPNR "025""205""0""255"
                         ESCS "P" PVPNR "109""205""0""255"
                         ESCS "P" PSEQ "020""008""0""255"};
unsigned char * vpnrps = {ESCS "Rc" PSEQ ESCS "Rc" PVPNR ESCS "Rc" PPNR
                          ESCS "P" PPNR "025""205""0""255"
                          ESCS "P" PSEQ "109""205""0""255"
                          ESCS "P" PVPNR "020""008""0""255"};
unsigned char * seqprogtastes = {ESCS "RT""001""066""119""119""4" TSEQPROGS};
unsigned char * seqprogs [2] = {{"%s"},{"%4u"}};
unsigned char * seqpnrvars = {ESCS "=015075""%s" ESCS "=070075""%s" ESCS "=015100""%s" ESCS "=070100""%s"
                              ESCS "=015125""%s" ESCS "=070125""%s" ESCS "=015150""%s" ESCS "=070150""%s"};
unsigned char * seqcursorvars = {ESCS "=050075""%s" ESCS "=105075""%s" ESCS "=050100""%s" ESCS "=105100""%s"
                                 ESCS "=050125""%s" ESCS "=105125""%s" ESCS "=050150""%s" ESCS "=105150""%s"};
unsigned char * seqpnrs [8] = 
{
{ESCS "=015075""%4u"},
{ESCS "=070075""%4u"},
{ESCS "=015100""%4u"},
{ESCS "=070100""%4u"},
{ESCS "=015125""%4u"},
{ESCS "=070125""%4u"},
{ESCS "=015150""%4u"},
{ESCS "=070150""%4u"},
};
unsigned char * seqpikleistes[3] =
{
{ESCS "Rc" PPNR ESCS "Rc" PENTER ESCS "Rc" PVPNR ESCS "Rc" PPAUTO
ESCS "RC" TSEQS ESCS "RC" TENTERS ESCS "RC" TVPNRS ESCS "RC" TPAUTOS},
{ESCS "RT""001""193""039""047""4" TLEFTS ESCS "P" PCURSORLINKS "010""208""0""255"
ESCS "RT""041""193""039""047""4" TRIGHTS ESCS "P" PCURSORRECHTS "053""208""0""255"
ESCS "RT""081""193""039""047""4" TINSS ESCS "P" PINS "089""208""0""255"
ESCS "RT""121""193""039""047""4" TDELS ESCS "P" PDEL "129""208""0""255"
ESCS "RT""281""193""039""047""4" TENDES ESCS "P" PENDE "288""204""0""255"},
{ESCS "Rc" PPNR ESCS "Rc" PENTER ESCS "Rc" PSEQ ESCS "Rc" PPAUTO
ESCS "RC" TSEQS ESCS "RC" TENTERS ESCS "RC" TVPNRS ESCS "RC" TPAUTOS},
};
/* EasyCODE > */
// Laendereingabe
unsigned char * anzsprache = {ESCS "=012062"ESCS "S22" "D" //"DEUTSCH"
ESCS "=092062""GB" //"ENGLISH"
ESCS "=169062""F" //"FRANCAIS"
ESCS "=253062""E" //"ESPA­OL"
ESCS "=009110""I" ESCS "S11" //"ITALIANO"
};
// Funktionen sperren
unsigned char * users = {ESCS"F001" ESCS"=137079USER 1" ESCS"=201079USER 2" ESCS"=265079USER 3" ESCS"F1"};   
unsigned char * schlossloeschs = {ESCS "Rc" P_SCHLOSS_ZU  ESCS "Rc" P_SCHLOSS_AUF };
unsigned char * fktnrzusts = {ESCS "=035105" ESCS "S12" "F : %3u " ESCS "S11" "%s%s%s"};
unsigned char * pschloss1s [2] =
{
{ESCS "P" P_SCHLOSS_AUF "148""105""0""255"},
{ESCS "P" P_SCHLOSS_ZU "154""105""0""255"}
};
unsigned char * pschloss2s [2] = 
{
{ESCS "P" P_SCHLOSS_AUF "214""105""0""255"},
{ESCS "P" P_SCHLOSS_ZU "220""105""0""255"}
};
unsigned char * pschloss3s [2]= 
{
{ESCS "P" P_SCHLOSS_AUF "277""105""0""255"},
{ESCS "P" P_SCHLOSS_ZU "283""105""0""255"}
};
unsigned char * val_pik_einhs = {"%s" POS_GETVAL_EINH ESCS"F001""%s" ESCS"F1"}; 
unsigned char * val_ints = {POS_GETVAL ESCS "S12" "%5i" ESCS "S11"};
unsigned char * val_int4s = {POS_GETVAL ESCS "S12" " %04i" ESCS "S11"};
unsigned char * ppins = {ESCS "P" P_PINCODE "020""006""0""255"};
// Direktfunktion smref
unsigned char * klammers =   {ESCS "RT""001201""063039""4" TSCHABLONES ESCS "P" PSCHABLONE "021""209""0""255"};
unsigned char * showenters = {ESCS "RT""257201""063039""4" TENTERS ESCS "P" PENTER "270211" "0" "255"};
unsigned char * delenters =  {ESCS "RC" TENTERS ESCS "Rc" PENTER};
unsigned char * refs ={ESCS "RC" TENTERS ESCS "Rc" PENTER ESCS "RC" TSCHABLONES ESCS "Rc" PSCHABLONE
                       ESCS "RT""001201""063039""4" TFUSSS ESCS "P" PFUSS "022""213""0""255"
                       ESCS "RT""193201""063039""4" TFWDS ESCS "P" PFWD "213""209""0""255"
                       ESCS "RT""257057""063039""4" TPARS ESCS "P" PPAR "275""069""0""255"
                       ESCS "RT""257201""063039""4" TBACKS ESCS "P" PBACK "277""209""0""255"};
unsigned char * reftexts = {Z3 "REF %u     X: %-5i  Y: %-5i"};
/* EasyCODE - */
// Direktfunktion rfid
unsigned char * showcodes = {ESCS "RT""257201""063039""4" TENTERS ESCS "P" PZIFFER "270211" "0" "255"};
unsigned char * delcodes =  {ESCS "RC" TENTERS ESCS "Rc" PZIFFER};
unsigned char *rfidtext1 = (ESCS "=020150" "RFID-CODE: " "%4u");
unsigned char *rfidtext2 = (ESCS "=020150" "RFID-CODE: " "----");
unsigned char *rfidps = {ESCS "P" PRFID "020""008""0""255"};
unsigned char *rfidstr = {ESCS "=060007" ESCS "S12" "%4u" ESCS "S11"};
/* EasyCODE - */
// Direktfunktion bctest
unsigned char *bcanztext1 = (ESCS "=020150" "BARCODE: " "%4u");
unsigned char *bcanztext2 = (ESCS "=020150" "BARCODE: " "----");
unsigned char *bctaste = {ESCS "RT""257201""063039""4" TBARCODES ESCS "P" PBARCODE_KLEIN "282214" "0" "255"};
unsigned char *bctaste2 = {ESCS "RT""001193""063047""4" TBARCODES ESCS "P" PBARCODE_KLEIN "025210" "0" "255"};
unsigned char *delbctaste = {ESCS "RC" TBARCODES ESCS "Rc" PBARCODE_KLEIN};
/* EasyCODE - */
// Direktfunktion Nm drehen
unsigned char * nmgeschws = {ESCS "S12" ESCS "=010090""%2u00" ESCS "=052098" ESCS "S11" "rpm"};
unsigned char * nmistgeschws = {ESCS "=010120" "%4u"};
unsigned char * nmistposs = {ESCS "=270175" "%3u"};
unsigned char * nmistposdels = {ESCS "=270175" "   "};
unsigned char * nmps = {ESCS "P" PNAEHMOTOR "015""008""0""255" ESCS "=090015 " "rpm"};
/* EasyCODE - */
// Service
unsigned char * pkaltstarts = {ESCS "P" PKALTSTART "147""006""0""255"};
//unsigned char * hfusshubs = {ESCS "=019125" "%3.1fmm"};
unsigned char * hfusshubs = {ESCS "=019125" "%4.1fmm"};    // SW014
//unsigned char * hfussniveaus = {ESCS "=019182" "%3.1fmm"};
unsigned char * hfussniveaus = {ESCS "=019182" "%4.1fmm"};  // SW014
//unsigned char * hfussposs = {ESCS "S12" ESCS "=220140" "%-3.1fmm" ESCS "S11"};
unsigned char * hfussposs = {ESCS "S12" ESCS "=220140" "%-4.1fmm" ESCS "S11"};    // SW014
/* EasyCODE - */
// Nullpunkte einstellen
unsigned char * nullpstops =   {ESCS "RC" TSTARTS ESCS "Rc" PSTART ESCS "RT""001201""063039""4" TSTOPS ESCS "P" PSTOP "020""209""0""255"};
unsigned char * nullpstarts =  {ESCS "RC" TSTOPS ESCS "Rc" PSTOP ESCS "RT""001201""063039""4" TSTARTS ESCS "P" PSTART "020""209""0""255"};
unsigned char * nullpdelstops= {ESCS "RC" TSTOPS ESCS "Rc" PSTOP};
/* EasyCODE - */
//unsigned char * nullpkoords=   {ESCS "S12" ESCS "=055080" "SMX: %-3i" ESCS "=215080" "SMY: %-3i" ESCS "S11"};
unsigned char * nullpkoord2s=  {ESCS "S12" ESCS "=090210" "X: %-3i     Y: %-3i" ESCS "S11"};
//unsigned char * nullphfs=  {ESCS "S12" ESCS "=130210" "Z: %-2i" ESCS "S11"};
unsigned char * nullphfs=  {ESCS "S12" ESCS "=130210" "Z: %+3i" ESCS "S11"};   // SW012 

unsigned char * nullppiktos =  {ESCS "P" "%s" "200008" "0255"};
unsigned char * nullpschabls = {ESCS "RT""256050""063039""4" TSCHABLONES ESCS "P" PSCHABLONE "277""059""0""255"};
/* EasyCODE - */
unsigned char * floatstr = {ESCS "=055007" ESCS "S12" "%3.1f" ESCS "S11"};
unsigned char * floatstr2 = {ESCS "=055007" ESCS "S12" "%4.2f" ESCS "S11"};
unsigned char * intstr = {ESCS "=060009" ESCS "S12" "%5i" ESCS "S11"};
unsigned char * ufwstr = {ESCS "=060007" ESCS "S12" "%5u" ESCS "S11"};
unsigned char * gswstr = {ESCS "=055007" ESCS "S12" "%2u" ESCS "S11" ESCS "=075010" "(x100 rpm)" };
unsigned char * zeitstr2 = {ESCS "=055007" ESCS "S12" "%4.2f" ESCS "S11"" s"};
unsigned char * scalestr = {ESCS "=065009" ESCS "S12" "%3u%" ESCS "S11"};
unsigned char * drehwinkelstr = {ESCS "=065009" ESCS "S12" "%3uø" ESCS "S11"};
unsigned char * koordstr = {ESCS "=055007" ESCS "S12" "%4i" ESCS "S11" " (x0,1mm)"};
/* EasyCODE - */
// Programm erstellen
unsigned char * statuszeilenstr = {ESCS "=130013" "%3u:" IOFF " %s"}; 
unsigned char * koordinatenstr1 = {ESCS "=005050X:%-5i  Y:%-5i"};
unsigned char * koordinatenstr2 = {ESCS "=005050X:       Y:     "};
unsigned char * apconfirm = {ESCS "P" PAPBESTAETIGEN "100""085""0""255"};
unsigned char * progpikto = {ESCS "=120012" "[%s"}; // Kommentar

// Abschnittsanzeigen
unsigned char * annaehstr = {ESCS "P" PANNAEHEN "1750070255"};
unsigned char * schneidstr = {ESCS "P" PFADENSCHNEIDEN "1750070255"};
unsigned char * anfriegelstr = {ESCS "P" PANFANGSRIEGEL "1750090255"};
unsigned char * endriegelstr = {ESCS "P" PENDRIEGEL "1750090255"};
unsigned char * naehausstr = {ESCS "P" PNAEHENAUS "1750090255"};
unsigned char * progendestr = {ESCS "P" PEND "1750140255"};
unsigned char * hindstr = {ESCS "P" PHINDERNIS "1750090255"};
unsigned char * stdststr = {ESCS "P" PSTANDARDSTICHL "1750090255"};
unsigned char * stichlstr = {ESCS "P" PSTI "1750090255"};
unsigned char * stichbstr = {ESCS "P" PSTICHBREITE "1750070255"};
unsigned char * pnrstr = {ESCS "P" PPNR "1750070255"};
unsigned char * klcodestr = {ESCS "P" PCODE "1650140255"};
unsigned char * apstr = {ESCS "P" PANFANGSPUNKT "1750070255"};
unsigned char * eilstr = {ESCS "P" PEILGANG "1750090255"};
unsigned char * stichstr = {ESCS "P" PSTICH "1750090255"};
unsigned char * geradestr = {ESCS "P" PGERADE "1750060255"};
unsigned char * bogenstr1 = {ESCS "P" PBOGENSTP1 "1750090255"};
unsigned char * bogenstr2 = {ESCS "P" PBOGENSTP2 "1750090255"};
unsigned char * kreisstr1 = {ESCS "P" PKREISSTP1 "1700060255"};
unsigned char * kreisstr2 = {ESCS "P" PKREISSTP2 "1700060255"};
unsigned char * kreisstr3 = {ESCS "P" PKREISSTP3 "1700060255"};
unsigned char * kurvstpstr = {ESCS "P" PKURVENPUNKT "1700110255"};
unsigned char * kurvendstr = {ESCS "P" PKURVENENDE "1700110255"};
unsigned char * ufwstopstr = {ESCS "P" PUFWSTOP "1730070255"};
unsigned char * stopstr = {ESCS "P" PSTOP "1730070255"};
unsigned char * redgsweinstr = {ESCS "P" PREDGSWEIN "1730070255"};
unsigned char * redgswausstr = {ESCS "P" PREDGSWAUS "1730070255"};
unsigned char * zuspgeinstr = {ESCS "P" PZUSSPGEIN "1750110255"};
unsigned char * zuspgausstr = {ESCS "P" PZUSSPGAUS "1750110255"};
unsigned char * oberthebeneinstr = {ESCS "P" POBERTEILHEBENEIN "1750080255"};
unsigned char * oberthebenausstr = {ESCS "P" POBERTEILHEBENAUS "1750080255"};
unsigned char * outstr = {ESCS "P" POUT "1750120255"};
unsigned char * nadeldrehenausstr = {ESCS "P" PNADELDREHEN_AUS "1750120255"};
unsigned char * nadeldreheneinstr = {ESCS "P" PNADELDREHEN_EIN "1750120255"};
unsigned char * fusshubstr = {ESCS "P" PFUSS "1750120255"};
unsigned char * fussniveaustr = {ESCS "P" PHFLEVEL "1750120255"};
unsigned char * instr = {ESCS "P" PIN "1750120255"};
unsigned char * zeitstr = {ESCS "P" P_TIME "1750070255"};
unsigned char * drehzstr = {ESCS "P" PDREHZAHL "1730070255"};

// Abschnittsanzeige loeschen
unsigned char * fusspikto = {ESCS "P" PFUSS "0252130255"};
unsigned char * endepikto = {ESCS "P" PENDE "2760060255"};
unsigned char * stoppikto = {ESCS "P" PSTOP "0202090255"};
/* EasyCODE - */
//Abscnittsparameter anzeigen
unsigned char * parastr1 = {ESCS "=205013" "%4.2fmm"};
unsigned char * parastr2 = {ESCS "=205025" "%s"};
unsigned char * parastr3 = {ESCS "=205013" " %-4u"}; 
unsigned char * parastr4 = {ESCS "=205013" " %s"};
unsigned char * parastr5 = {ESCS "=200025" "%3ist."};
unsigned char * parastr6 = {ESCS "=200013" " %s"};
unsigned char * parastr7 = {ESCS "=205013" "%u %s"};
unsigned char * parastr8 = {ESCS "=205013" "%3.2fs"};
unsigned char * parastr9 = {ESCS "=205013" "%u/%uST." ESCS "=205025" "%u/%uST."}; 
unsigned char * parastr10 = {ESCS "=205013" "%3.1fmm"};
unsigned char * parastr11 = {ESCS "=200013" " %-4u" ESCS "P" "%s" "2430130255"}; 
unsigned char * parastr12 = {ESCS "=205013" "STEP %u"}; 
unsigned char * delparastr = {ESCS "=200013" "       " ESCS "=200025" "       " ESCS "Rc" P1NADEL ESCS "Rc" P2NADEL };
/* EasyCODE - */
// Stop bei Dauertakten im Programmieren
unsigned char *dauertaktstr =  {ESCS "RC" TFUSSS ESCS "Rc" PFUSS ESCS "RC" TFINDS ESCS "Rc" PFIND
                                ESCS "RC" TFWDS ESCS "Rc" PFWD ESCS "RC" TBACKS ESCS "Rc" PBACK
                                ESCS "RT" "001201" "063039" "4" TSTOPS ESCS "P" PSTOP "0202090255"};
unsigned char *stopdauertaktstr = {ESCS "RC" TSTOPS ESCS "Rc" PSTOP
                                   ESCS "RT" "001201" "063039" "4" TFUSSS ESCS "P" PFUSS "0252130255"
                                   ESCS "RT" "065201" "063039" "4" TFINDS ESCS "P" PFIND "0852070255"
                                   ESCS "RT" "193201" "063039" "4" TFWDS ESCS "P" PFWD "2122090255"
                                   ESCS "RT" "257201" "063039" "4" TBACKS ESCS "P" PBACK "2762090255"};
/* EasyCODE - */
// Programmieren/Einfuegen
unsigned char *pannaehstr = {ESCS "RT""136081""047039""4" TANNAEHENS ESCS "P" PANNAEHEN "148090""0""255" 
                             ESCS "RT""136121""047039""4" TANFANGSRIEGELS ESCS "P" PANFANGSRIEGEL "152131""0""255"};
unsigned char *pschneidstr = {ESCS "RT""136081""047039""4" TFADENSCHNEIDENS ESCS "P" PFADENSCHNEIDEN "148090""0""255" 
                              ESCS "RT""136121""047039""4" TENDRIEGELS ESCS "P" PENDRIEGEL "152131""0""255"
                              ESCS "RT""136161""047039""4" TNAEHENAUSS ESCS "P" PNAEHENAUS "148170""0""255"};
unsigned char *pnadeldrehenaus = {ESCS "RT""136201""047039""4" TNADELDREHENS ESCS "P" PNADELDREHEN_AUS "148210""0""255"};
unsigned char *pnadeldrehenein = {ESCS "RT""136201""047039""4" TNADELDREHENS ESCS "P" PNADELDREHEN_EIN "148210""0""255"};
unsigned char *blockconfirm = {BV034 ESCS "P" PINS "1400100255" ESCS "P" PBLOCKGROSS "1200800255"
                               ESCS "S23" ESCS "=190093" "?" ESCS "S11"};
unsigned char *nostichl = {BV001 ESCS "P" PSTIGROSS "1400920255" ESCS "S23" ESCS "=180095" "?" ESCS "S11"};
unsigned char *ikpiktos = {ESCS "P" "%s" "0100100255"};
unsigned char *out1setres = {ESCS "P" "%s" "0101020255"};
unsigned char *out2setres = {ESCS "P" "%s" "0101420255"};
unsigned char *out3setres = {ESCS "P" "%s" "0101820255"};
unsigned char *out4setres = {ESCS "P" "%s" "0102220255"};
unsigned char *zuspgpiktos = {ESCS "P" "%s" "1640920255"};
unsigned char *othebenpiktos = {ESCS "P" "%s" "1601300255"};
unsigned char *redgswpiktos = {ESCS "P" "%s" "1622090255"};
unsigned char *fusshubs = {ESCS "=056105" ESCS"F001""%3.1fmm" ESCS"F1"};
unsigned char *fusslevels = {ESCS "=056148" ESCS"F001""%3.1fmm" ESCS"F1"};
unsigned char *fusshublevels = {ESCS "=056185" ESCS"F001""Level" ESCS"F1"};
unsigned char *fusslevellevels = {ESCS "=056228" ESCS"F001""Level" ESCS"F1"};
unsigned char *vpars = {ESCS "=293088" "%-3i" ESCS "=295100" "st."};
/* EasyCODE < */
// Riegel aendern
unsigned char *riegelpiktos = {ESCS "P" "%s" "1400100255"};
unsigned char *riegelwertes = {ESCS "=048072" "%u  /  %u"
                               ESCS "=125072" "ST."
                               ESCS "=190072" "%u  /  %u"
                               ESCS "=268072" "ST."};
/* EasyCODE > */
// Programmnummer aendern
unsigned char * prog_pnr1s = {ESCS "=057007" ESCS "S12" "%4u" ESCS "S11" ESCS "P" P1NADEL "0950120255"};
unsigned char * prog_pnr2s = {ESCS "=057007" ESCS "S12" "%4u" ESCS "S11" ESCS "P" P2NADEL "0950120255"};
unsigned char * prog_pnrp2s = {ESCS "RT""001""193""079""047""4" T1NADELS ESCS "P" P1NADEL "040""210""0""255"
                               ESCS "RT""081""193""079""047""4" T2NADELS ESCS "P" P2NADEL "115""210""0""255"};
/* EasyCODE - */
// Block
unsigned char *blockmanipwertes = {ESCS "=203103" "%3u%%"
                                   ESCS "=285103" "%3u%%"
                                   ESCS "=283143" "%3uø"};
unsigned char *moveblockapconfirm = {BV030 ESCS "P" PBLOCKANFANGGROSS "1200800255" ESCS "S23" ESCS "=190093" "?" ESCS "S11"};
unsigned char *stopmoveblockap = {BV029 ESCS "RT" "001201" "063039" "4" TSTOPS ESCS "P" PSTOP "0202090255"};
/* EasyCODE - */
// Brother-Programm positionieren
unsigned char *fusss = {ESCS "RT" "001201" "063039" "4" TFUSSS ESCS "P" PFUSS "0202080255"};
/* EasyCODE ) */
/* EasyCODE ( 0 
Produktion */
// Statuszeile - Pikto Programmwahl, Programmnummer, Seq/Vpnr, StichlÝnge, Geschwindigkeit, Kommentar
unsigned char * prodstatpiktopnrs = {ESCS "P" "%s" "0050080255"};
unsigned char * prodstatpnrs[2] = {{ESCS "S12" ESCS "=038008" "----" ESCS "S11"}, 
                                   {ESCS "S12" ESCS "=038008" "%-4u" ESCS "S11"}};
unsigned char * prodstatstls[2] = {{ESCS "S12" ESCS "=128008" "---" ESCS "S11"},
                                   {ESCS "S12" ESCS "=128008" "%3.1f" ESCS "S11"}};
unsigned char * prodstatgsws = {ESCS "S12" ESCS "=193007" "%4u" ESCS "S11"};
unsigned char * prodstatpiktokleins = {ESCS "P" "%s" "0770050255"};
unsigned char * prodstatseqvpnrs = {ESCS "=080020" "%u"};
unsigned char * prodstatkomm1s = {FONT001 ESCS "=230005" "%-6s" FONT1};
unsigned char * prodstatkomm2s = {FONT001 ESCS "=230017" "%-6s" FONT1};
unsigned char * prodstatkomm3s = {FONT001 ESCS "=230029" "%-6s" FONT1};

// Tastenblock
unsigned char * prodtbfinds = {ESCS "RT" "238142" "039039" "4" TFINDS ESCS "P" PFIND "2451500255"};
unsigned char * prodtbautomans = {ESCS "RT" "196100" "039039" "4" "%s" ESCS "P" "%s" "2051080255"};
unsigned char * prodtbautomandels = {ESCS "RC" TAUTOS ESCS "Rc" PAUTO ESCS "RC" TMANS ESCS "Rc" PMAN};
unsigned char * prodtbufcounts = {ESCS "RT" "001100" "096039" "4" TCOUNTERS ESCS "P" PCOUNTER "0061070255"
                                  ESCS "=042107" "%5u/" ESCS "=042120" "%5u"};
unsigned char * prodtbfastoes = {ESCS "P" PFADENSTOERUNG "0061050255"};
unsigned char * prodbtdelfastoes = {ESCS "Rc" PFADENSTOERUNG};
unsigned char * prodtbspulenwechsels = {ESCS "P" PSPULENWECHSEL "0061150255"};
unsigned char * prodbtdelspulenwechsels = {ESCS "Rc" PSPULENWECHSEL};
unsigned char * uebertemps = {ESCS "P" PTEMP "1501050255"};
unsigned char * uebertemp2s = {ESCS "P" PTEMP "2901050255"};
unsigned char * bobcounts = {ESCS "=042107" "%5u"};
unsigned char * mbefs = {ESCS "P" "%s" "0051500255"};
unsigned char * unterspannungs = {ESCS "=270150" CHRCODE "\x19" CHRCODE "\x19" " AC " CHRCODE "\x19" CHRCODE "\x19"};
unsigned char * akthuepferwerte = {ESCS "P" PHFLEVEL "0011500255" ESCS "=019155" "%4.1fmm" 
                                   ESCS "P" PFUSS "0901550255" ESCS "=109155" "%4.1fmm"}; 

// Strings zur Anzeige der einer Sequenz/VPNR zugeordneten Programmnummern
unsigned char * pstats[8] = {{ESCS "RT" "001045" "039039" "4" TPSTAT0S ESCS "=004057" "%4u"},
                             {ESCS "RT" "041045" "039039" "4" TPSTAT1S ESCS "=044057" "%4u"},
                             {ESCS "RT" "081045" "039039" "4" TPSTAT2S ESCS "=084057" "%4u"},
                             {ESCS "RT" "121045" "039039" "4" TPSTAT3S ESCS "=124057" "%4u"},
                             {ESCS "RT" "161045" "039039" "4" TPSTAT4S ESCS "=164057" "%4u"},
                             {ESCS "RT" "201045" "039039" "4" TPSTAT5S ESCS "=204057" "%4u"},
                             {ESCS "RT" "241045" "039039" "4" TPSTAT6S ESCS "=244057" "%4u"},
                             {ESCS "RT" "281045" "039039" "4" TPSTAT7S ESCS "=284057" "%4u"}};
unsigned char *pstattasts[8] = {{TPSTAT0S}, {TPSTAT1S}, {TPSTAT2S}, {TPSTAT3S}, {TPSTAT4S}, {TPSTAT5S}, {TPSTAT6S}, {TPSTAT7S}};
/* EasyCODE - */
// Tasten bei laufender Maschine
unsigned char *tastlaufend =  {ESCS "RC" "000" ESCS "RC"TFUSSS ESCS "RC"TFAEDELS ESCS "RC"TSCHABLONES
                               ESCS "RC"TFINDS ESCS "RC"TMANS ESCS "RC"TAUTOS
                               ESCS "RC"THOMES ESCS "RC"TSTARTS ESCS "RC"TFWDS ESCS "RC"TBACKS
                               ESCS "RT" "001201" "063039" "4" TSTOPS ESCS "P" PSTOP "0202090255"};
unsigned char *tastlaufend2 = {ESCS "RC" "000" ESCS "RC"TFUSSS ESCS "RC"TFAEDELS ESCS "RC"TSCHABLONES
                               ESCS "RC"TFINDS ESCS "RC"TMANS ESCS "RC"TAUTOS ESCS "RC"TCOUNTERS
                               ESCS "RC"THOMES ESCS "RC"TSTARTS ESCS "RC"TFWDS ESCS "RC"TBACKS
                               ESCS "=001100" DELABCURSOR ESCS "RT" "060100" "200140" "4" TSTOPS
                               TEXTROT ESCS "P" PSTOPPSCHILD "1051150255" TEXTSCHWARZ};
/* EasyCODE - */
// Tastenstrings
unsigned char *starts = {ESCS "RT" "001201" "063039" "4" "000" ESCS "RT" "017201" "030039" "0" TSTARTS ESCS "P" PSTART "0202080255"};
unsigned char *homes = {ESCS "RT" "065201" "063039" "4" "000" ESCS "RT" "081201" "030039" "0" THOMES ESCS "P" PHOME "0842090255"};
/* EasyCODE - */
unsigned char *fwds = {ESCS "RT" "193201" "063039" "4" "000" ESCS "RT" "209201" "030039" "0" TFWDS ESCS "P" PFWD "2122090255"};
unsigned char *backs = {ESCS "RT" "257201" "063039" "4" "000" ESCS "RT" "273201" "030039" "0" TBACKS ESCS "P" PBACK "2762090255"};
unsigned char *takts = {ESCS "RT" "193201" "063039" "4" "000" ESCS "RT" "209201" "030039" "0" TFWDS ESCS "P" PFWD "2122090255"
                        ESCS "RT" "257201" "063039" "4" "000" ESCS "RT" "273201" "030039" "0" TBACKS ESCS "P" PBACK "2762090255"};
unsigned char * prodploeschs = {ESCS "RC" "000" ESCS "RC" TSTOPS ESCS "Rc" PSTOP ESCS "RC" TSTARTS ESCS "Rc" PSTART ESCS "RC" TFWDS ESCS "Rc" PFWD ESCS "RC" TBACKS ESCS "Rc" PBACK
                                ESCS "RC" THOMES ESCS "Rc" PHOME};
/* EasyCODE - */
// Home
unsigned char *hometext1 = {ESCS "P" PCUTGROSS "0900800255"};
unsigned char *hometext2 = {ESCS "P" PUPUNKT "0900800255"};
unsigned char *hometext3 = {ESCS "RC" TSTOPS ESCS "Rc" PSTOP
                            ESCS "RT" "001201" "063039" "4" TSTARTS ESCS "P" PSTART "0202080255"
                            ESCS "RT" "257201" "063039" "4" TAUFS ESCS "P" PCURSORAUF "2812110255"};
unsigned char *hometext4 = {ESCS "RC" TSTARTS ESCS "Rc" PSTART ESCS "RC" TAUFS ESCS "Rc" PCURSORAUF
                            ESCS "RT" "001201" "063039" "4" TSTOPS ESCS "P" PSTOP "0202080255"};
// Find
unsigned char * findkoords = {ESCS "S12" ESCS "=065075" "X:%-5i" ESCS "=200075" "Y:%-6i" ESCS "S11"};
unsigned char * findpiktos = {ESCS "P" PFIND "0100100255" ESCS "RC" TFUSSS ESCS "Rc" PFUSS};
/* EasyCODE - */
// Spulenwechsel
unsigned char * showfaedels = {ESCS "RT""257201""063039""4" TFAEDELS ESCS "P" PFAEDEL "270211" "0" "255"};
unsigned char * delfaedels = {ESCS "RC" TFAEDELS};
unsigned char * faedelhfposs = {ESCS "=260096" "%-3.1fmm"};
unsigned char * tnadeldrehens = {ESCS "RT""001096""063039""4" TNADELDREHENS ESCS "P" PNADELDREHEN_EIN "021103" "0" "255"};
/* EasyCODE - */
// Barcode
unsigned char *bctext1 = (ESCS "=237023" "%4u");
unsigned char *bctext2 = (ESCS "=237023" "----");
unsigned char *bcpiktoklein = (ESCS "P" PBARCODE_KLEIN "2450050255");
/* EasyCODE ) */
/* EasyCODE ) */
