/* EasyCODE V8 */
/* EasyCODE ( 0 
naeport.h,   Header fuer das Naehen,
             Ersteller: Wilfried Halfmann   EES 2856 */
/* EasyCODE ( 0 
Maschinennullpunkt (Initiatoren) */
/* EasyCODE [ 
Naehfeld 1200 x 700 */
#define MNP_XBILD_12070   6000
#define MNP_YBILD_12070   2534
#define MNP_MOTORX_12070  6000
#define MNP_MOTORY_12070  2534
/* EasyCODE ] */
/* EasyCODE [ 
Naehfeld 1200 x 1700 */
#define MNP_XBILD_120170   6000
#define MNP_YBILD_120170   2534
#define MNP_MOTORX_120170  6000
#define MNP_MOTORY_120170  2534
/* EasyCODE ] */
/* EasyCODE [ 
Naehfeld 1200 x 2000 */
#define MNP_XBILD_120200   6000
#define MNP_YBILD_120200   2534
#define MNP_MOTORX_120200  6000
#define MNP_MOTORY_120200  2534
/* EasyCODE ] */
/* EasyCODE ) */
/* EasyCODE ( 0 
Ladepunkt */
/* EasyCODE [ 
Naehfeld 1200 x 700 */
#define LP_YBILD_12070  2100
#define LP_MOTORY_12070 2100
/* EasyCODE ] */
/* EasyCODE [ 
Naehfeld 1200 x 1700 */
#define LP_YBILD_120170  2100
#define LP_MOTORY_120170 2100
/* EasyCODE ] */
/* EasyCODE [ 
Naehfeld 1200 x 2000 */
#define LP_YBILD_120200  (2100 + 35)
#define LP_MOTORY_120200 (2100 + 35)
/* EasyCODE ] */
/* EasyCODE ) */
/* EasyCODE ( 0 
Serviceposition */
/* EasyCODE [ 
Naehfeld 1200 x 700 */
#define SP_XBILD_12070   6000
#define SP_YBILD_12070   -8600
#define SP_MOTORX_12070  6000
#define SP_MOTORY_12070  -8600
/* EasyCODE ] */
/* EasyCODE [ 
Naehfeld 1200 x 1700 */
#define SP_XBILD_120170   6000
#define SP_YBILD_120170   -18600
#define SP_MOTORX_120170  6000
#define SP_MOTORY_120170  -18600
/* EasyCODE ] */
/* EasyCODE [ 
Naehfeld 1200 x 2000 */
#define SP_XBILD_120200   6000
#define SP_YBILD_120200   -21600
#define SP_MOTORX_120200  6000
#define SP_MOTORY_120200  -21600
/* EasyCODE ] */
/* EasyCODE ) */
#define smy_ststadr 170 // Rampendaresse SMY fuer konst. Frequenz fahren
/* EasyCODE ( 0 
Definition der Klasse KlasseNaehen */
/* EasyCODE C */
/* EasyCODE < */
class KlasseNaehen
/* EasyCODE > */
{
        private:
        //Keys
        unsigned char poweronkey;
        unsigned char wartenkey;
        unsigned char weiterinfo;
        unsigned char ablaufkey;
        unsigned char homekey;
        unsigned char fstepkey;
        unsigned char nextfwdkey;
        unsigned char backkey;
        unsigned char bstepkey;
        unsigned char faedelkey;
        unsigned char nextfaedelkey;
        unsigned char findkey;
        unsigned char nozufkey;
        // Variablen
        unsigned char fastcount;   //Zaehler fuer rueckwaerts Takten nach Fadenstoerung
        unsigned int dauercount;
        unsigned char varstring [100];
        int aktpos;
        unsigned char taste, tastenmerk;
        
        // Flags
        unsigned char stopb;
        unsigned char ponrdy;
        unsigned char thomeanz;
        unsigned char tfaedelstartanz;
        unsigned char qp_index;
        unsigned char stepmerker;
        unsigned char dauertakten;
        unsigned char carposok_mem;
        unsigned char smaktiv;
        unsigned char sewpoint;
        unsigned char enter_mem;
        unsigned char stopmerker;
        unsigned char SchablVorhanden;
        
        // Funktionen
        void poweron (void);
        void home (void);
        void stopzustand (void);
        void warten (void);
        void ablauf (void);
        void fwd (void);
        void fwdctrl ( void);
        unsigned char fstep (void);
        void back (void);
        void backctrl (void);
        unsigned char bstep (void);
        void faedel (void);
        void find (void);
        unsigned char naehen (void);
public:
        unsigned char key;
        unsigned char fwdkey;
        unsigned char naekey;
        unsigned char prodbtrart; // Betriebsart Produktion (MAN, AUTO)
        unsigned char errpar;
        unsigned char dauerstart;
        unsigned char cutact, cutreq;
        unsigned char ofastoe, ufastoe, uf_contnae, kofastoe; // Ober-/Unterfadenstoerung
        unsigned char kofastoe_ursache; // SW003DIS
        unsigned char ufastoe_stopNachSchneiden;  //SW18-1
        
        unsigned char homereq;
        unsigned char anzboreq, uebertemp;
        unsigned char faedelreq;
        unsigned char findreq;
        unsigned char naestop;
        unsigned char firsthome;
        unsigned int pnrmem;  // Merker fÅr Programmnummer bei automatischer Programmnummerwahl
        float zykluszeit;
        unsigned char vfindex;
        unsigned char demomodus;
        unsigned int barcode;
        unsigned char deckelueberwachung;
        hfkoord hfsollpos;
        unsigned char kurzerAnffad;
        
        void verw (void);
        void kinit (void);
        void winit (void);
        void naetab_boot (void);
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseNaehen(void)
        {
            //prodbtrart = AUTO;
            deckelueberwachung = 1;
            /* EasyCODE - */
            ufastoe_stopNachSchneiden = FALSE;
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
        /* EasyCODE ( 0 
        unsigned char get_ablaufkey(void) */
        /* EasyCODE F */
        unsigned char get_ablaufkey(void)
        {
            return ablaufkey;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        set_barcode */
        /* EasyCODE F */
        void set_barcode(void)
        {
            barcode = schabl.bc.get_barcode();
            return ;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        res_barcode */
        /* EasyCODE F */
        void res_barcode(void)
        {
            barcode = 0xffff;
            return ;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        get_barcode */
        /* EasyCODE F */
        unsigned int get_barcode(void)
        {
            return barcode;
        }

        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseNaehen nae;
/* EasyCODE ) */
