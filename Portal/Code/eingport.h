/* EasyCODE V8 */
/* EasyCODE ( 0 
eing3590.h */
/* EasyCODE ( 0 
struct masch_daten */
#define MASCHDAT_KENNUNG        4   // SW001DIS
/* EasyCODE C */
/* EasyCODE < */
struct masch_daten
/* EasyCODE > */
{
        unsigned char prsel;          // Programmnummerselect (0-pnr, 1-seq, 2-vpnr)
        unsigned int pnr;             // Programmnummer (0 - 9999)
        unsigned char seq;            // Sequenzprogrammnummer (0 - 9)
        unsigned char vpnr;           // verbundene Programmnummer (0 - 9)
        unsigned char pnrindex;       // Index zeigt auf akt. Programmnummer
                                      //   bei Sequenz bzw. verbundenen Progr.
        unsigned char next_pnrindex;  // Index zeigt auf naechste Programmnummer
                                      //   bei Sequenz bzw. verbundenen Progr.
        unsigned int seq_array [SEQVPNR_MAX+1][PNRINDEX_MAX+1];  // Sequenz-Programmnummer-Array
        unsigned int vpnr_array [SEQVPNR_MAX+1][PNRINDEX_MAX+1]; // verbundene Programmnummern-Array
        unsigned char fkey_array [FKEY_USER_MAX] [FKEY_FUNK_MAX];
        unsigned int codenr_arr  [FKEY_USER_MAX];         // Codenr. fÝr gesperrte Funktionen
        char sprache;                 // Landessprachen
        unsigned char tastenpiep;     // Reaktion auf Tastendruck - Tastenpiep
        unsigned char kontrast;       // Bedienfeld-Kontrast
        unsigned char annaetab [8];   // Annõhtabelle, letzte Geschw. mu¯ MAXGSW_MAX sein
        unsigned char fw_counter;     // Reaktionszeit OFW
        unsigned char ofwen;
        unsigned char ufwen;
        unsigned char drehmodus;      // Modus fuer Unterfadenschieber und Nadeldreheinrichtung
        unsigned char ofwpreset;      // Verzoegerungsstiche fuer Oberfadenueberwachung
        unsigned char ufwpreset;      // Verzoegerungsstiche fuer Unterfadenueberwachung
        unsigned char backst;         // Stichanzahl fuer Ruecktakten bei Fadenst÷rung
        unsigned char ffmode;         // Flip-Flop-Modus fuer Fussschalter
        unsigned char openclampen;    // automatisches Klammeroeffnen
        unsigned char autoseq;        // automatische Sequenzweiterschaltung
        unsigned char jigctren;       // Enable KlammerÝberwachung
        unsigned char autoen;         // Enable Automatik-Start
        unsigned int bobcnt_wert;     // Startwert fuer Unterfadenzaehler in Stichen
        unsigned int ufwrestpreset;   // noch zu naehende Teile nach Ansprechen des Sensors
        unsigned char slowst;
        unsigned char cutgsw;         // Schneiddrehzahl
        unsigned char maxgsw;         // max. Geschw. (Eingabe)
        unsigned char redgsw;         // red. Geschwindigkeit (Riegeldrehzahl)
        unsigned char wedelnfussoben; // 0 - Wedeln mit Fuss unten,   1 - Wedeln mit Fuss oben
        unsigned int nisvograd;       // Vorgabe fuer NIS in Grad
        unsigned int ref2;            // Vorgabe fuer Fadenschneiden in Grad
        unsigned int y30offvograd;    // Position Fadenschneiden aus in Grad
        unsigned int fadlot;          // Fadenleger OT in Grad
        unsigned int fadspvograd;     // Handradposition (in Grad), bei der die Fadenspannung
                                      // waehrend dem Schneidstich auf geschaltet wird.
        unsigned int spgzuvograd;     // Position Spannung zu beim Annaehen
        unsigned int fadklemme_time;  // Zeit fuer Fadenklemme auf
        unsigned int fadwedler_time;  // Zeit fuer Fadenwedler
        unsigned int nadelkuehl_time; // Zeit fuer Nachblasen der Nadelkuehlung
        unsigned int fwk_time;        // Zeit fuer Fadenwedlerklemme auf
        unsigned int ufk_time;        // Zeit fuer Unterfadenklemme auf
        unsigned int fza_time;        // Zeit Fadenzieher Anfangsfaden
        unsigned char matdicke;       // Materialdicke, 0 -  duenn,  1 - dick
        unsigned char wedler;         // Ablauf mit Fadenwedler
        unsigned char nadelanzahl;    // 1- oder 2-Nadel-Balken
        unsigned char fwk_verzst;     // Verzoegerungsstiche Fadenwedlerklemme auf beim Ann„hen
        unsigned char ufk_verzst;     // Verzoegerungsstiche Unterfadenklemme auf beim Ann„hen
        unsigned char kurzerAnffad;   // verkuerzter Anfangsfaden
        
        // Werte fuer Huepferfuss
        hfkoord posoben;
        hfkoord posunten;       // einstellbar
        unsigned int hub;       // in 0,1 mm, einstellbar
        unsigned char hfsicher; // Sicheres Huepferfuss heben, bei jedem Heben wird Initiator angefahren
        unsigned int hfniveau[9];
        unsigned int hfhub[9];
        
        // Werte fuer Naehmotorinterface
        unsigned char quick_paramwert [QP_ANZAHL];
        
        unsigned char automatic;   // SW001DIS
        unsigned char koueberw;    // SW003DIS
             
        unsigned char Nadeldreh1ein;  // fuer saf->fadschein_winkel1   
        unsigned char Nadeldreh2ein;  // fuer saf->fadschein_winkel2    
        unsigned char VorhStcheDreh;  // Anzahl Vorhaltestiche fuer die Nadeldreheinrichtung   
        unsigned char dummy5;     
        unsigned char dummy6;     
        unsigned char dummy7;     
        unsigned char dummy8;     
        unsigned char dummy9;     
        unsigned char dummy10;     
        unsigned int  dummy11;     
        unsigned int  dummy12;     
        unsigned int  dummy13;     
        unsigned int  dummy14;     
        unsigned int  dummy15;
        unsigned int  dummy16;
        unsigned int  dummy17;
        unsigned int  dummy18;     
        unsigned int  dummy19;     
        unsigned int  dummy20;     
        int           dummy21;
        int           dummy22;
        int           dummy23;
        int           dummy24;
        int           dummy25;
        int           dummy26;
        int           dummy27;
        int           dummy28;
        int           dummy29;
        int           dummy30;
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
Defines */
#define DEUTSCH 0
#define ENGL    1
#define FRANZ   2
#define SPAN    3
#define ITAL    4

// Defines fuer quelle
#define DRIVE_C   0
#define DRIVE_A   1
#define DRIVE_SD  2
#define DRIVE_LAN 3

#define LINKS 0
#define RECHTS 1
/* EasyCODE ( 0 
Programmverwaltung (Commander) */
#define RWLAENGE 2500

#define MAX_PROGANZAHL 500
#define DIRZ_MAX  7

// Fehler
#define KEINEKARTE 1
#define FALSCHEKARTE 2
#define KARTENSTECKFEHLER 3
#define KARTESCHREIBSCHUTZ 4
#define DATENFEHLERKARTE 5
#define FORMATKARTE 6
#define FALSCHEKARTENDATEI 7
#define FALSCHEKARTENDATEIGROESSE 8
#define UEBERTRAGUNGSFEHLERKARTE 9
#define DATEILOESCHEN 10
/* EasyCODE ) */
// klappt nicht mit STDLIB.h
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

// RÝckmeldung von getkey
#define STOPANF      0xff

// Werte fuer prsel
#define PNR 0
#define SEQ 1
#define VPNR 2
/* EasyCODE ) */
/* EasyCODE ( 0 
Offsets im Flashfile kfportal.bin */
// Achtung: Die Offsets werden mit Ausnahme der Gueltigflags
//          durch die Reihenfolge beim Schreiben festgelegt !!!

#define FF_KONFIG_GUE 0
//#define FF_STOFFGEGENDRUECKERF 1
//#define FF_JIGCTRF 2

#define FF_MNP_GUE 10
//#define FF_XKOR 11
//#define FF_YKOR 12

#define FF_HF_GUE 15
//#define FF_HFKOR 16

#define FF_INKRKOR_GUE 36
//#define FF_INKR_KOR1 37
//#define FF_INKR_KOR2 38
//#define FF_SCHABL_KOR 39

#define FF_NADELPOS_GUE 40
//#define FF_NADELPOS 41

#define FF_CHAR_MAX 50
/* EasyCODE ) */
/* EasyCODE ( 0 
Hauptkeystellungen der Eingabe */
#define E_PON        0
#define E_RUHE       1
#define E_EXIT       2
#define E_GST        3
#define E_PNRW       4
#define E_SEQ        5
#define E_PROG       6
#define E_PROGVER    7
#define E_PAR        8
#define E_INFO       9
#define E_LAENDER   10
#define E_FUNKT     11
#define E_SM        12
#define E_NM        13
#define E_SERV      14
#define E_REF       15
#define E_VPNR      16
#define E_STI       17
#define E_GSW       18
#define E_UFW       19
#define E_RFID      20
#define E_BARCODE   21
#define E_REF2      22

#define S_START      0
#define S_IO         1
#define S_FEHLER     2
#define S_EXIT       3
#define S_AUS        4
#define S_PIEPS      5
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgverw */
/* EasyCODE C */
/* EasyCODE < */
class KlasseProgverw
/* EasyCODE > */
{
        private:
        /* EasyCODE ( 0 
        Daten */
        /* EasyCODE < */
        unsigned char key, dirkey;
        unsigned char physdrive;   // physikalisches Laufwerk (SD, LAN, Diskette, ...)
        int ineof;
        char quellpath[20], zielpath[20];
        long quellfilesize;
        FILE *in,*out, *f1;
        char arbeitsverzeichnis [15];
        unsigned char dirvarstring[DIRZ_MAX][24];
        struct ffblk ffblk;
        unsigned char daten[RWLAENGE];
        struct progpar cmdppar;
        /* EasyCODE > */
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Prototypen */
        void scrup(unsigned char drive);
        void scrdown(unsigned char drive);
        void dirpnrload(void);
        void dirfileload(void);
        void diraktanz(unsigned char drive);
        void diranz(unsigned char drive);
        unsigned char diskstart(void);
        void diskdirload(void);
        void diskdirfileload(void);
        void flashdirload(void);
        void sddirfileload(void);
        void sddirload(void);
        void copydiraktuallisieren(unsigned char drive, unsigned int index);
        unsigned int direintrag(unsigned char drive, unsigned int pnr, unsigned int index);
        unsigned char copy(void);
        unsigned char sdcopy(void);
        unsigned char filecopy(unsigned char * quelle, unsigned char * ziel);
        void deldiraktuallisieren(unsigned int index);
        unsigned char del(void);
        unsigned char filedel(unsigned char * quelle);
        unsigned char diskformat(void);
        void creatmdat(void);
        unsigned char WriteFileToSd(unsigned char *quelle, unsigned char *ziel);
        unsigned char ReadFileFromSd(unsigned char *quelle, unsigned char *ziel);
        unsigned char ReadCharFromSd(unsigned char *quelle, unsigned char *ziel,unsigned int anzahl, unsigned int position);
        unsigned char sdcardcheck (void);
        unsigned char sdcardformat (char *verzeichnis);
        unsigned char sourceSD(void);
        unsigned char sourceNet(void);
        /* EasyCODE ) */
public:
        /* EasyCODE ( 0 
        Daten */
        unsigned char filekonf[2], filemdat[2], aktdrive, sdcarderror;
        unsigned int pnrdir[2][MAX_PROGANZAHL+1];
        unsigned int filedir[2][DIRZ_MAX];
        unsigned char komentardir[2][MAX_PROGANZAHL+1][11];
        unsigned char anzreq[2], anzkommreq[2];
        unsigned int zindex[2], dirindex[2], dirindexmax[2], fileindexmax[2];
        unsigned int block[2];
        unsigned int zindexblock[2], dirindexblock[2];
        unsigned char sterninv[2];
        unsigned char copyall;
        char datenstring[3000];
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Prototypen */
        unsigned char grundzustand(void);
        void loadmdat(void);
        void removemdat(void);
        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseInfo */
/* EasyCODE C */
/* EasyCODE < */
class KlasseInfo
/* EasyCODE > */
{
        private:
        unsigned char vncrunning;
public:
        unsigned char key;
        unsigned char grundzustand(void);
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseLaender */
/* EasyCODE C */
/* EasyCODE < */
class KlasseLaender
/* EasyCODE > */
{
        private:
public:
        unsigned char key;
        unsigned char grundzustand(void);
        void inv_sprachtaste(void);
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseFunkt */
/* EasyCODE C */
/* EasyCODE < */
class KlasseFunkt
/* EasyCODE > */
{
        private:
public:
        unsigned char key;
        unsigned char grundzustand(void);
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseDirekt */
/* EasyCODE C */
/* EasyCODE < */
class KlasseDirekt
/* EasyCODE > */
{
        private:
        int aktpos;
public:
        unsigned char movesm (void);
        unsigned char nm (void);
        unsigned char smref (unsigned char carposit);
        unsigned char rfid (void);
        unsigned char bctest (void);
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseService */
/* EasyCODE < */
#define MODE_SONDER 1
#define MODE_NORMAL 2
/* EasyCODE > */
/* EasyCODE C */
/* EasyCODE < */
class KlasseService
/* EasyCODE > */
{
        private:
        //unsigned char i, index, indexm, loopflag, retwert, sanduhrmerker;
        unsigned char out_akt; // Service-Funktion Ausgõnge schalten aktiv
        //unsigned char inp_anz[3], out_anz[2];
        //unsigned int ainp_anz[4], aout_anz[2];
        unsigned char kaltstart(void);
        unsigned char bootsoftware(void);
        void softupdate(void);
        void ioanzup(void);
        void auftraganzup(void);
        unsigned char nullpunkte (void);
        unsigned char schlitten (void);
        unsigned char movehf(void);
        unsigned char hfussnullpunkt(void);
        unsigned char hfusseinstellen(void);
        KlasseIO* pIO;
        //unsigned char inAnz,outAnz,sonderAnz;
        unsigned char lastKey;// help variable to keyboard handle
        unsigned char spage; // page number of service
        unsigned int maske;
        unsigned char sonder;
        unsigned char dispMode; // DISP_SONDER , DISP_NORMAL
        char str [100];
        unsigned char index; // 0..15
        unsigned int auftragID[10]; // auftragmasken bis zu 10 Seite
        unsigned char tastKey;
public:
        unsigned char key;
        unsigned char grundzustand(void);
        void Init(KlasseIO* const pio);
        unsigned char GetInAnz()const;
        unsigned char GetOutAnz()const;
        unsigned char GetSonderAnz()const;
        char Invalidate(char refresh=TRUE);
        unsigned char Operate(unsigned char taste);
        char CheckPage();
        void GetInputStr(unsigned char,char* const);
        void GetOutputStr(unsigned char can,char* const inStr);
        void GetSRStr(unsigned char,char* );
        void FormatStr(unsigned int,char*,unsigned int = 0,char = ' ');
        unsigned char GetNextIndex();
        unsigned char GetPrevIndex();
        void GetPfeilStr(char* fstr,unsigned char pos);
        char Auftrag(unsigned char ,unsigned char ,char = TRUE);
        char CheckOut(unsigned char can,unsigned int outMask);
        void GetTitleStr(char* const fstr);
        /* EasyCODE ( 0 
        get_out_akt */
        /* EasyCODE F */
        unsigned char get_out_akt (void)
        {
            return out_akt;
        }

        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseEing */
/* EasyCODE C */
/* EasyCODE < */
class KlasseEing:public KlasseTask
/* EasyCODE > */
{
        private:
        unsigned char key;            // Verwaltungskey der Eingabe
        unsigned char ende;           // Eingabe beenden
        unsigned char dummypnr;
        unsigned char ponrdy;         // Eingabe PowerOn fertig
        unsigned char poweron(void);
        unsigned char ruhe(void);
        unsigned char exit(void);
        unsigned char grundzustand(void);
        unsigned char pnrwahl(void);
        unsigned char seqwahl(void);
        unsigned char stichl(void);
        unsigned char drehzahl(void);
        unsigned char ufw (void);
        KlasseInfo info;
        KlasseLaender laender;
        KlasseFunkt funkt;
        KlasseDirekt direkt;
public:
        KlasseService serv;
        KlassePar par;
        KlasseProgverw progverw;
        KlasseProg prog;
        KlasseProgUp pup;
        unsigned char stopb;      // Stopbestaetigung der Eingabe
        unsigned char varstring[200];
        unsigned char varstring1[RWLAENGE];
        unsigned char mdloadflag, konfloadflag;
        unsigned char ioact;    // io laufen lassen bzw. stoppen
        unsigned int errpar;    // Uebergabeparameter fuer Fehleranzeigen durch Koordinator
        unsigned int errpar2;   // Uebergabeparameter fuer Fehleranzeigen durch Koordinator 
        unsigned int fadsppos;  // Position bei der die Fadenspannung beim Schneidstich aufgeschaltet wird
        
        unsigned char auftrag;   // Auftragsbyte fuer Betriebsartenwechsel mit folgender Bedeutung
                                 // 0  - Eingabe fertig, Wechsel in Produktion
                                 // 1  - Eingabe exit nach PowerOn, Wechsel in Produktion
                                 // 2  - Eingabe, Wechsel in Gst. Eingabe
                                 // 3  - PNR-Wahl, Wechsel in Eingabe -> PNR-Wahl und danach automatisch zurueck in Produktion
                                 // 4  - Programmieren, Wechsel in Eingabe -> Progr. und danach wieder zurueck in Produktion
                                 // 5  - Eingabe, ohne die Moeglichkeit in die Produktion zu wechseln (Koordinator im PowerOn)
                                 // 6  - Stichlaenge eingeben, Wechsel in Eingabe -> Stichlaengen-Wahl und danach automatisch zurÝck in Produktion
                                 // 7  - Drehzahl eingeben, Wechsel in Eingabe -> Drehzahl-Wahl und danach automatisch zurÝck in Produktion
                                 // 8  - Unterfadenzaehler-Vorgabe eingeben, Wechsel in Eingabe -> Wert eingeben und danach automatisch zurueck in Produktion
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseEing(taskdata tdini):KlasseTask (tdini)
        {
            /* EasyCODE = */
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        geteingkey */
        /* EasyCODE F */
        unsigned char geteingkey (void)
        {
            return key;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        unsigned char stop(void) */
        /* EasyCODE F */
        unsigned char stop(void)
        {
            return stopb;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        unsigned char ponready(void) */
        /* EasyCODE F */
        unsigned char ponready(void)
        {
            return ponrdy;
        }

        /* EasyCODE ) */
        void eingtask(void);
        void kinit(void);
        void winit(void);
        void konfinit(void);
        void parameterinit(void);
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseEing eing;
/* EasyCODE ) */
