/* EasyCODE V8 */
/* EasyCODE ( 0 
nmint90.h,   Header fuer die Interrupts Nis und Ref2
             Ersteller: Wilfried Halfmann   EES 2856 */
/* EasyCODE ( 0 
Definition der Klasse KlasseNis */
/* EasyCODE C */
/* EasyCODE < */
class KlasseNis:public KlasseTask
/* EasyCODE > */
{
        private:
public:
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseNis(taskdata tdini):KlasseTask (tdini)
        {
        }

        /* EasyCODE ) */
        void nistask (void);
        void start(void);
        void niserr (void);
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse KlasseRef2 */
/* EasyCODE C */
/* EasyCODE < */
class KlasseRef2:public KlasseTask
/* EasyCODE > */
{
        private:
public:
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseRef2(taskdata tdini):KlasseTask (tdini)
        {
        }

        /* EasyCODE ) */
        void ref2task (void);
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseNis nis;
extern KlasseRef2 ref2;
/* EasyCODE ) */
