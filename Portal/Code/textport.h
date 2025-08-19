/* EasyCODE V8 */
/* EasyCODE ( 0 
textport.h */
/* EasyCODE ( 0 
Defines */
/*****************************************************************************/
/* Datei:          text3590.h                                                */
/* Klasse:         Kl3590                                                    */
/* Ersteller:      Halfmann  EES 2856                                        */   
/*****************************************************************************/

#define TEXTLENGTH 2000            // maximale Textlaenge (Sprachheap)

#define ESCS            "\x1b"          // ESC - Code als string festlegen
#define CLRHOME         "\x1a"          // Touchscreen loeschen und Cursor auf HOME-Position.
#define HOME            "\x1e"          // Touch Cursor auf Home positionieren.
#define CHRCODE         "\x01"          // Naechster Code ist ein CHR Code
#define BELL            "\x07"          // Kurzer Signalton 250ms
#define BELLBELL        "\x1f"          // Zwei kurze Signaltoene.
#define ION             "\x0e"          // Reverse ON
#define IOFF            "\x0f"          // Reverse OFF
#define CURSORLEFT      "\x08"          // Der Cursor wird ein Zeichen nach links bewegt
#define CURSORRIGHT     "\x0C"          // Der Cursor wird ein Zeichen nach links bewegt
#define CURSOROFF       "\x09"          // Der Cursor wird ausgeschaltet
#define CURSORON        "\x06"          // Der Cursor wird eingeschaltet
#define DELABCURSOR     "\x14"          // Delete ab Cursorposition
#define DELLINE         "\x15"          // Loeschen bis Zeilenende
#define BLINKENEIN      "\x10"          // Der Cursor wird ausgeschaltet
#define BLINKENAUS      "\x12"          // Der Cursor wird eingeschaltet
#define VERSION         "\x17"          // Versionsabfrage, Antwort VxxxHyzz
                                        // xxx = Softwareversion
                                        // y = BDF-Typ, (0=OC-Top, 1=BDF-S1, 2=BDF-S2, 3=BDF-T1, 5=BDF-P1
                                        // zz = Hardware-Version

#define Z1      ESCS"=000047"           // Textzeile 1
#define Z2      ESCS"=000079"           // Textzeile 2
#define Z3      ESCS"=000111"           // Textzeile 3
#define Z4      ESCS"=000143"           // Textzeile 4
#define Z5      ESCS"=000175"           // Textzeile 5
#define Z6      ESCS"=000207"           // Textzeile 6

#define FONT1   ESCS"F1"                // Zeichensatz Font1 auswählen
#define FONT2   ESCS"F2"                // Zeichensatz Font2 auswählen
#define FONT001 ESCS"F001"              // Zeichensatz Font001 auswählen

#define POS_GETVAL_EINH  ESCS"=122005"  // Position der Einheiten bei getval...
#define POS_GETVAL      ESCS"=057007"   // Position des Value bei getval...

#define TEXTROT         ESCS "c255000000"
#define TEXTBLAU        ESCS "c000000255"
#define TEXTSCHWARZ     ESCS "c000000000"
#define LINIEBLAU       ESCS "DC000000255"
#define LINIESCHWARZ    ESCS "DC000000000"
#define LINIEGRUEN      ESCS "DC000128000"
#define LINIEDUNKELBLAU ESCS "DC000000070"
#define KOMMENTARFARBE  ESCS "c000128000"
/* EasyCODE ) */
/* EasyCODE ( 0 
externels der sprachunabhaengigen Texte */
extern unsigned char * eings;
extern unsigned char * gibtsnicht;
extern unsigned char * hupe_ion;
extern unsigned char * errpars;
extern unsigned char * errpar2s;
extern unsigned char * errpar3s;
extern unsigned char * errpiktos;
extern unsigned char * ioerrs [];
extern unsigned char * secerrs [];
extern unsigned char * touchinv;
extern unsigned char * piktoinv;
extern unsigned char * touchdel;
extern unsigned char * pikdel;
extern unsigned char * powerons;
extern unsigned char * poweron2s;
extern unsigned char * dirs;
extern unsigned char * dir1s;
extern unsigned char * dirloeschs;
extern unsigned char * dirstatus[];
extern unsigned char * drives[];
extern unsigned char * lans;
extern unsigned char * dirposs[2][7];
extern unsigned char * wrongcarderr;
extern unsigned char * nocarderr;
extern unsigned char * sdcarderr;
extern unsigned char * writeprotecterr;
extern unsigned char * dialogfenster;
extern unsigned char * dialogcopy0;
extern unsigned char * dialogcopy1;
extern unsigned char * dialogcopy2;
extern unsigned char * dialogcopy3;
extern unsigned char * dialogtasten0;
extern unsigned char * dialogtasten1;
extern unsigned char * dialogtasten2;
extern unsigned char * dialogdel0;
extern unsigned char * dialogstatus0;
extern unsigned char * dialogstatus1;
extern unsigned char * dialogstatus2;
extern unsigned char * dialogformat0;
extern unsigned char * dialogformat1;
extern unsigned char * dialogformat2;
extern unsigned char * dialogformat3;
extern unsigned char * laufwerk;
extern unsigned char * lwwechsel1s;
extern unsigned char * lwwechsel2s;
extern unsigned char * zulloesch;
extern unsigned char * zulis;
extern unsigned char * zulf1s;
extern unsigned char * zulf2s;
extern unsigned char * zulf3s;
extern unsigned char * pinpiktos; 
extern unsigned char * pinstring;      
extern unsigned char * pin_inv; 
extern unsigned char * fnk_inv; 
extern unsigned char * sanduhrs;
extern unsigned char * sanddels;
extern unsigned char * getvalende;
extern unsigned char * tinvs;
extern unsigned char * pinvs;
extern unsigned char * tdels;
extern unsigned char * swversions;
extern unsigned char * stundens;
extern unsigned char * vncstarts;
extern unsigned char * vncstops;
extern unsigned char * vncruns;
extern unsigned char *vncPCruns;
extern unsigned char * vncPCnotruns;
extern unsigned char * vncnotruns;
extern unsigned char * ipadrs;

extern unsigned char * pnrps;
extern unsigned char * pnrs;
extern unsigned char * pnrkomms;
extern unsigned char * pnrp2s;
extern unsigned char * pnrloeschs;
extern unsigned char * seqps;
extern unsigned char * vpnrps;
extern unsigned char * seqprogtastes;
extern unsigned char * seqprogs[2];
extern unsigned char * seqpnrvars;
extern unsigned char * seqcursorvars;
extern unsigned char * seqpnrs [8];
extern unsigned char * seqpikleistes[3];
extern unsigned char * klcodeps;
extern unsigned char * stips;
extern unsigned char * ststips;
extern unsigned char * stibreiteps;
extern unsigned char * hfhubps;
extern unsigned char * hflevelps;
extern unsigned char * zeitps;
extern unsigned char * vparps;
extern unsigned char * gswps;
extern unsigned char * floatstr;
extern unsigned char * floatstr2;
extern unsigned char * intstr;
extern unsigned char * gswstr;
extern unsigned char * zeitstr2;
extern unsigned char * koordstr;
extern unsigned char * ufwps;
extern unsigned char * ufwp2s;
extern unsigned char * ufwstr;
extern unsigned char * scalexps;
extern unsigned char * scaleyps;
extern unsigned char * drehwinkelps; 
extern unsigned char * symxps;
extern unsigned char * symyps;
extern unsigned char * symxp2s;
extern unsigned char * symyp2s;

extern unsigned char * anzsprache;
extern unsigned char * users;
extern unsigned char * schlossloeschs;
extern unsigned char * fktnrzusts;
extern unsigned char * pschloss1s [2];
extern unsigned char * pschloss2s [2];
extern unsigned char * pschloss3s [2];
extern unsigned char * val_pik_einhs;
extern unsigned char * val_ints;
extern unsigned char * val_int4s;
extern unsigned char * ppins;
extern unsigned char * nmgeschws;
extern unsigned char * nmistgeschws;
extern unsigned char * nmistposs;
extern unsigned char * nmistposdels;
extern unsigned char * nmps;
extern unsigned char * klammers;
extern unsigned char * showenters;
extern unsigned char * delenters;
extern unsigned char * showcodes;
extern unsigned char * delcodes;
extern unsigned char * rfidtext1;
extern unsigned char * rfidtext2;
extern unsigned char * rfidps;
extern unsigned char * rfidstr;
//SW005-1
extern unsigned char * bcanztext1;
extern unsigned char * bcanztext2;
extern unsigned char * bctaste;
extern unsigned char * bctaste2;
extern unsigned char * delbctaste;

extern unsigned char * refs; 
extern unsigned char * reftexts;
extern unsigned char * nullpstops;
extern unsigned char * nullpstarts;
extern unsigned char * nullpdelstops;
extern unsigned char * nullpkoord2s;
extern unsigned char * nullphfs;
extern unsigned char * nullppiktos;

extern unsigned char * prodstatpiktopnrs;
extern unsigned char * prodstatpnrs[2];
extern unsigned char * prodstatstls[2];
extern unsigned char * prodstatgsws;
extern unsigned char * prodstatpiktokleins;
extern unsigned char * prodstatseqvpnrs;
extern unsigned char * prodstatkomm1s;
extern unsigned char * prodstatkomm2s;
extern unsigned char * prodstatkomm3s;

extern unsigned char * prodtbfinds;
extern unsigned char * prodtbautomans;
extern unsigned char * prodtbautomandels;
extern unsigned char * prodtbufcounts;
extern unsigned char * pstats[8];
extern unsigned char * pstattasts[8];
  
extern unsigned char * tastenblocks;
extern unsigned char * uebertemps;
extern unsigned char * uebertemp2s;
extern unsigned char * bobcounts;
extern unsigned char * prodtbfastoes;
extern unsigned char * prodbtdelfastoes;
extern unsigned char * prodtbspulenwechsels;
extern unsigned char * prodbtdelspulenwechsels;
extern unsigned char * tastlaufend;
extern unsigned char * tastlaufend2;
extern unsigned char * mbefs;
extern unsigned char * unterspannungs;
extern unsigned char * homes;
extern unsigned char * starts;
extern unsigned char * fwds;
extern unsigned char * backs;
extern unsigned char * takts;
extern unsigned char * prodploeschs;
extern unsigned char * nullpschabls;
extern unsigned char * akthuepferwerte;
/* EasyCODE - */
extern unsigned char * pkaltstarts;
extern unsigned char * hfusshubs;
extern unsigned char * hfussniveaus;
extern unsigned char * hfussposs;

extern unsigned char * hometext1;
extern unsigned char * hometext2;
extern unsigned char * hometext3;
extern unsigned char * hometext4;
extern unsigned char * findkoords;
extern unsigned char * findpiktos;

extern unsigned char * showfaedels;
extern unsigned char * delfaedels;
extern unsigned char * faedelhfposs;
extern unsigned char * tnadeldrehens;
extern unsigned char * bctext1;
extern unsigned char * bctext2;
extern unsigned char * bcpiktoklein;
/* EasyCODE - */
extern unsigned char * statuszeilenstr;
extern unsigned char * koordinatenstr1;
extern unsigned char * koordinatenstr2;
extern unsigned char * apconfirm;
extern unsigned char * progpikto;
extern unsigned char * annaehstr;
extern unsigned char * schneidstr;
extern unsigned char * anfriegelstr;
extern unsigned char * endriegelstr;
extern unsigned char * naehausstr;
extern unsigned char * progendestr;
extern unsigned char * hindstr;
extern unsigned char * stdststr;
extern unsigned char * stichlstr;
extern unsigned char * stichbstr;
extern unsigned char * pnrstr;
extern unsigned char * klcodestr;
extern unsigned char * apstr;
extern unsigned char * eilstr;
extern unsigned char * stichstr;
extern unsigned char * geradestr;
extern unsigned char * bogenstr1;
extern unsigned char * bogenstr2;
extern unsigned char * kreisstr1;
extern unsigned char * kreisstr2;
extern unsigned char * kreisstr3;
extern unsigned char * kurvstpstr;
extern unsigned char * kurvendstr;
extern unsigned char * ufwstopstr;
extern unsigned char * stopstr;
extern unsigned char * redgsweinstr;
extern unsigned char * redgswausstr;
extern unsigned char * zuspgeinstr;
extern unsigned char * zuspgausstr;
extern unsigned char * oberthebeneinstr;
extern unsigned char * oberthebenausstr;
extern unsigned char * outstr;
extern unsigned char * nadeldrehenausstr;
extern unsigned char * nadeldreheneinstr;
extern unsigned char * fussniveaustr;
extern unsigned char * fusshubstr;
extern unsigned char * instr;
extern unsigned char * zeitstr;
extern unsigned char * drehzstr;
extern unsigned char * scalestr;
extern unsigned char * drehwinkelstr;
extern unsigned char * fusspikto;
extern unsigned char * endepikto;
extern unsigned char * stoppikto;
extern unsigned char * parastr1;
extern unsigned char * parastr2;
extern unsigned char * parastr3; 
extern unsigned char * parastr4;
extern unsigned char * parastr5;
extern unsigned char * parastr6;
extern unsigned char * parastr7;
extern unsigned char * parastr8;
extern unsigned char * parastr9; 
extern unsigned char * parastr10; 
extern unsigned char * parastr11;
extern unsigned char * parastr12;
extern unsigned char * delparastr;
extern unsigned char * dauertaktstr;
extern unsigned char * stopdauertaktstr;
extern unsigned char * pannaehstr;
extern unsigned char * pschneidstr;
extern unsigned char * pnadeldrehenaus;
extern unsigned char * pnadeldrehenein;
extern unsigned char * blockconfirm;
extern unsigned char * nostichl;
extern unsigned char * ikpiktos;
extern unsigned char * out1setres;
extern unsigned char * out2setres;
extern unsigned char * out3setres;
extern unsigned char * out4setres;
extern unsigned char * zuspgpiktos;
extern unsigned char * othebenpiktos;
extern unsigned char * redgswpiktos;
extern unsigned char * fusslevels;
extern unsigned char * fusshubs;
extern unsigned char * fusshublevels;
extern unsigned char * fusslevellevels;
extern unsigned char * vpars;
extern unsigned char * riegelpiktos;
extern unsigned char * riegelwertes;
extern unsigned char * prog_pnr1s;
extern unsigned char * prog_pnr2s;
extern unsigned char * prog_pnrp2s;
extern unsigned char * blockmanipwertes;
extern unsigned char * moveblockapconfirm;
extern unsigned char * stopmoveblockap;
extern unsigned char * fusss;
/* EasyCODE ) */
/* EasyCODE ( 0 
structur der sprachabhaengigen Texte */
/* EasyCODE C */
/* EasyCODE < */
struct text
/* EasyCODE > */
{
        unsigned char * textanf; 
        unsigned char * ausschalts;
        unsigned char * koor2;  
        unsigned char * errs;  
        unsigned char * faterrors [3];
        unsigned char * koorerrs[1];
        unsigned char * naeerrs [1];
        unsigned char * zuferrs [1];
        unsigned char * staperrs [1];
        unsigned char * eingerrs [2];
        unsigned char * ioerr_urs;
        unsigned char * secerr_urs;
        unsigned char * eingkaltstart;
        unsigned char * eing_cmd1;
        unsigned char * eing_cmd3;
        unsigned char * proghin1;
        unsigned char * proghin2;
        unsigned char * progtext1;
        unsigned char * progtext2;
        unsigned char * textend;
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ) */
