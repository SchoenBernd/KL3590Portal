/* EasyCODE V8 */
/* EasyCODE ( 0 
datinf.h */
/* EasyCODE ( 0 
Klassedatinf */
/* EasyCODE C */
/* EasyCODE < */
class Klassedatinf
/* EasyCODE > */
{
        private:
public:
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        Klassedatinf (void)
        {
        }

        /* EasyCODE ) */
        /* EasyCODE < */
        unsigned char loadprog (unsigned int prognr);
        unsigned char storeprog (void);
        unsigned char delprog (unsigned int prognr);
        unsigned char delall (void);
        unsigned char exprog (unsigned char quelle, unsigned int prognr);
        unsigned char parprog (unsigned int prognr, struct progpar *parptr, unsigned char drive);
        unsigned long freememory (void);
        unsigned char readprog_to_ems (void);
        /* EasyCODE > */
};
/* EasyCODE E */
/* EasyCODE ) */
extern Klassedatinf di;
/* EasyCODE ) */
