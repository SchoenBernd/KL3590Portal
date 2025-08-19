/* EasyCODE V8 */
/* EasyCODE ( 0 
naeup.h,   Header fÅr Unterprogramme fuer das Naehen,
           Ersteller: Wilfried Halfmann   EES 2856 */
/* EasyCODE ( 0 
Defines */
// Reihenfolge der M-Befehle im array mbef, Reihenfolge darf nicht geÑndert
// werden
#define XYVALID     0         // Koordinaten
#define ANNAEHEN    1
#define SCHNEIDEN   2
#define PROGSTOP    3
#define UFWSTOP	    4 
#define NAEHENAUS   5
#define REDGSWON    6
#define REDGSWOFF   7
#define PROGENDE    8
#define OUT1SET     9
#define OUT1RES     10
#define OUT2SET     11
#define OUT2RES     12
#define OUT3SET     13
#define OUT3RES     14
#define OUT4SET     15
#define OUT4RES     16
#define OUT6SET     17        // Oberteil heben beim Verfahren ein
#define OUT6RES     18        // Oberteil heben beim Verfahren aus
#define ZUSPGON     19        // Zusatzspannung ein
#define ZUSPGOFF    20        // Zusatzspannung aus
#define INHIGH1     21
#define INLOW1      22
#define INHIGH2     23
#define INLOW2      24
#define HFNIVEAU    25
#define HFHUB       26
#define HFNIVEAULEV 27
#define HFHUBLEV    28
#define DELAY       30
#define BREMSEN     31        // M-Befehl der zum Bremsen fÅhrt
#define STOP        32        // M-Befehl der zum Stop fÅhrt
#define FLAG1SET    33
#define FLAG1RES    34
#define LAST        35        // legt Dimension des Arrays mbef fest

// Geschwindigkeit fÅr carmov
#define STANDARD   0
#define SLOW       1
#define FAST       2
#define TAKTEN     3


// Unterfadenschieber Funktion
#define CHKEIN   0   // Check welcher Quadrant
#define CHKAUS   1   // berechne den Tangens
#define CHKQUA   2   // Checke ob wir im betreffenden Quadranten sind oder nicht.
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition koord */
/* EasyCODE C */
/* EasyCODE < */
typedef struct
/* EasyCODE > */
{
        int xbild;
        int ybild;
        int motorx;
        int motory;
}koord;
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition lpoord */
/* EasyCODE C */
/* EasyCODE < */
typedef struct
/* EasyCODE > */
{
        int ybild;
        int motory;
}lpkoord;
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition nbuf */
/* EasyCODE C */
/* EasyCODE < */
typedef struct
/* EasyCODE > */
{
        unsigned char * datpoi;
        koord koorist;
        unsigned char nbtart;
        unsigned char naeout;         // xx------
                                      //   |||||+--- out1
                                      //   ||||+---- out2
                                      //   |||+----- out3
                                      //   ||+------ out4
                                      //   |+------- frei
                                      //   +-------- Oberteil heben beim Verfahren
        unsigned char flag;           // --------
                                      // |||||||+--- Flag1
                                      // ||||||+---- Flag2
                                      // |||||+----- Flag3
                                      // ||||+------ Flag4
                                      // |||+------- Flag5
                                      // ||+-------- Flag6
                                      // |+--------- Flag7
                                      // +---------- Flag8
        unsigned char naegsw;
        unsigned char redgswen;
        unsigned char zuspgon;
        unsigned int posunten;        // Position Huepferfuss unten in 0,1 mm
        unsigned int hub;             // Huepferfusshub in 0,1 mm
}nbuf;
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse KlasseNaeUp */
/* EasyCODE C */
/* EasyCODE < */
class KlasseNaeUp
/* EasyCODE > */
{
        private:
        /* EasyCODE ( 0 
        Daten */
        unsigned char xinit_key, yinit_key;
        unsigned char TPSTAT_edge;
        unsigned char footm;
        unsigned char schneidenaus_flag, spgauf_flag;
        float faktorx, faktory;   // korrigierter Wert fuer FAKTOR
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Funktionen */
        void transport (void);
        void nae_stop (void);
        void naeakt (void);
        void bremsen (void);
        void ende (void);
        void klammer_edge (void);
        void fsedge (void);
        unsigned char sonderstart (void);
        /* EasyCODE ) */
public:
        /* EasyCODE ( 0 
        Daten */
        //progstruct * proganf;
        unsigned char *proganfadr, *stianfadr, *endadr;
        unsigned char mbef[LAST];
        nbuf workbuf;
        koord koormerker;
        int deltax, deltay;
        unsigned char *datpoi5, *datpoi10;
        koord koorsoll[11];
        unsigned char gswvor[11];
        unsigned char vorind;
        unsigned char fcutkey, initkey;
        unsigned char gswkoor, stigsw, fussgsw;
        unsigned char carlp;
        unsigned char unterspannung;
        unsigned char dfv10en;
        unsigned int verzzt;
        unsigned char TKLAMMER_edge, TKLAMMER_mem, TKLAMMER_merker;
        unsigned char fs1_edge;         // Vorderflanke bei Fu·schalter Stufe 1
        unsigned char fs2_edge;         // Vorderflanke bei Fu·schalter Stufe 2
        unsigned char fs1_mem;          // Merker fÅr Flanke Fu·schalter 1
        unsigned char fs2_mem;          // Merker fÅr Flanke Fu·schalter 2
        unsigned char fs1_merker;       // Merker fÅr Fu·schalter Stufe 1
        unsigned char tlinksm, trechtsm;
        unsigned char autostart_en, autoclampverz, clopendi, klammerAufMerker;
        unsigned char naposact;
        unsigned char barcode_lesen;
        unsigned int barcode_alt;
        unsigned char biggeststitch;
        unsigned char schneidkey;
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseNaeUp(void)
        {
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Funktionen */
        void dfetnsew (void);
        void dfetsew (void);
        void sewstop (void);
        void dfetvor5 (void);
        void dfetvor10 (void);
        unsigned char poscheck (koord *koord);
        void bitomo_init (void);
        void bitomo (koord *koord);
        void motobi (koord *koord);
        unsigned char napos (void);
        void carmov (unsigned char mode);
        void carmovdyn(unsigned char achse, unsigned char richtung, unsigned char speed);
        unsigned char fcutportal (void);
        void nacont90 (void);
        unsigned char movinit (void);
        unsigned char xinit (void);
        unsigned char yinit (void);
        void workbuf_res (void);
        unsigned char nadeldreh (unsigned char cmd, int xbild2, int ybild2, int xbild1, int ybild1);
        void getprpar (void);
        unsigned char startup (void);
        void foot (void);
        void nasere (void);
        void nasere_back (void);
        unsigned char get_naefeldind (int ybildkoord);
        unsigned char find_out1 (void);
        unsigned char read_jigpnr (void);
        unsigned char clampstart (void);
        void progstat (void);
        unsigned char jig_ctrl (void);
        void naeout_akt (void);
        void hfuss_akt (void);
        unsigned char get_biggeststitch (void);
        unsigned char schneidablauf (void);
        void anschlaege (void);
        unsigned char get_hubgsw (void);
        unsigned char find_UFWSTOP (void);
        unsigned char Prog_vervollst (void);
        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseNaeUp nup;
/* EasyCODE ) */
