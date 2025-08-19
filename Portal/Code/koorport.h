/* EasyCODE V8 */
/* EasyCODE ( 0 
koor3590   Header fuer den Koordinator,
           Ersteller: Wilfried Halfmann   EES 2856 */
#define TOUCH 0
#define PANEL 1
#define PANELPC 2
/* EasyCODE ( 0 
Definition der Klasse KlasseKoordinator */
/* EasyCODE C */
/* EasyCODE < */
class KlasseKoordinator:public KlasseTask
/* EasyCODE > */
{
        private:
        unsigned char key;
        unsigned char poweronkey;
        unsigned char poncontkey;
        unsigned char eingkey;
        unsigned char prodkey;
        unsigned char errkey;
        unsigned char auskey;
        unsigned char freskey;
        unsigned char hbtartb;
        unsigned char bdferror;
        unsigned char bdf_load;
        unsigned char koor_err2;
        unsigned char varstr1[60];
        unsigned char varstr2[60];
        unsigned char naestopflag;
        unsigned char zufstopflag;
        unsigned char iostopflag;
        unsigned char eingstopflag;
        unsigned char notstopflag;
        unsigned char firmprogflag;
        void poweron (void);
        void eingabe (void);
        void produktion (void);
        void fehler (void);
        void ausschaltfehler (void);
        void in_image_up (void);
        unsigned char faterror_res (void);
        unsigned char notstop_ok (void);
        unsigned char error_ok (void);
        void secerranz (unsigned char secerr);
public:
        unsigned char koor_err;
        unsigned char nae_err;
        unsigned char zuf_err;
        unsigned char io_err;
        unsigned char eing_err;
        unsigned char io_secerr;
        unsigned char bdfbreakrdy;
        unsigned char bdfbaudrdy;
        unsigned char bdf;
        unsigned char ponrdy;
        void koortask (void);
        void kinit (void);
        void winit (void);
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseKoordinator(taskdata tdini):KlasseTask (tdini)
        {
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        hbtart */
        /* EasyCODE F */
        unsigned char hbtart (void)
        {
            return hbtartb;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        naestop */
        /* EasyCODE F */
        unsigned char naestop (void)
        {
            return naestopflag;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        zufstop */
        /* EasyCODE F */
        unsigned char zufstop (void)
        {
            return zufstopflag;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        iostop */
        /* EasyCODE F */
        unsigned char iostop (void)
        {
            return iostopflag;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        eingstop */
        /* EasyCODE F */
        unsigned char eingstop (void)
        {
            return eingstopflag;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        notstop */
        /* EasyCODE F */
        unsigned char notstop (void)
        {
            return notstopflag;
        }

        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
extern int prog_handle [PNRINDEX_MAX+1];
extern char *ndatanf;
extern KlasseKoordinator koor;
/* EasyCODE ) */
