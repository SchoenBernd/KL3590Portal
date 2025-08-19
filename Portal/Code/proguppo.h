/* EasyCODE V8 */
/* EasyCODE ( 0 
progup90.h */
/* EasyCODE ( 0 
Definition der Klasse KlasseProgUp */
/* EasyCODE C */
/* EasyCODE < */
class KlasseProgUp
/* EasyCODE > */
{
        private:
        unsigned char dauercount, stopmerker;
public:
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseProgUp(void)
        {
        }

        /* EasyCODE ) */
        /* EasyCODE < */
        unsigned char dauertakten;
        char lastabsch[5];
        unsigned char koorflag;
        
        unsigned char nafupos (void);
        void carmov (void);
        void set_pzbl (struct p_abschstruct * ptr);
        void set_pzbl_abschnitt (struct p_abschstruct * ptr);
        unsigned char mark_check (void);
        unsigned char mark_enable_test (struct p_abschstruct *ptr);
        unsigned char ptr_in_block (struct p_abschstruct *ptr);
        void pstatuszeile(void);
        void abschparanz(void);
        void kooranz(void);
        unsigned char prog_exit (void);
        struct p_abschstruct * find_absch (unsigned char abschart, struct p_abschstruct * ptr);
        struct p_abschstruct * rfind_absch (unsigned char abschart, struct p_abschstruct * ptr);
        unsigned char pfoot (void);
        unsigned char absch_koord (struct p_abschstruct * ptr);
        unsigned char pfwd (void);
        unsigned char next_abschfwd (void);
        unsigned char absch_fwd (void);
        unsigned char movstich_fwd (void);
        unsigned char pback (void);
        unsigned char next_abschback (void);
        unsigned char absch_back (void);
        unsigned char movstich_back (void);
        void pout_exe (void);
        unsigned char find (void);
        unsigned char movlastp (void);
        void absch_del (struct p_abschstruct *ptr, unsigned int n);
        void ins_curvend (void);
        void curvend (void);
        unsigned char inskoord (unsigned char *piktostr);
        unsigned char absch_einf (struct p_abschstruct *ptr, unsigned char n);
        unsigned char car_to_blockanf (void);
        /* EasyCODE > */
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseProgUp pup;
/* EasyCODE ) */
