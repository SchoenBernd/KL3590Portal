/* EasyCODE V8 */
/* EasyCODE ( 0 
nmtrb90.h,  Ersteller: Wilfried Halfmann  EES 2856 */
/* EasyCODE ( 0 
Definition der Klasse KlasseNmTreiber */
/* EasyCODE C */
/* EasyCODE < */
class KlasseNmTreiber
/* EasyCODE > */
{
        private:
        unsigned char slowcount, annaecount, gswcount;
        char ofwcount, ufwcount, fwkcount, ufkcount, zuspgcount;
        int postemp;
public:
        unsigned char nmmodus, nmstop;
        unsigned char bremscount, bremsverzcount, bremsrdy;
        unsigned char ofwaktiv, ufwaktiv, kofwaktiv;
        unsigned char gswakt;
        unsigned char gswinten;
        unsigned char gswind, gsw_out [100];     // Trace
        char gswanz [50][3];                     // Trace
        unsigned char smind, gswanz_ind;         // Trace
        unsigned int verzsm [100][2], adr [100][2];  // Trace
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseNmTreiber(void)
        {
        }

        /* EasyCODE ) */
        void grundzustand (void);
        void nmon (void);
        void annaehen (void);
        void naeh (void);
        void bremsen5 (void);
        void bremsen (void);
        void stopot (void);
        void stopflot (void);
        void bremscut (void);
        void gswoncut (void);
        void gswaus(unsigned char gsw);
        void gswup (unsigned char gsw);
        void naetabausw (unsigned char gsw);
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseNmTreiber nmtrb;
/* EasyCODE ) */
