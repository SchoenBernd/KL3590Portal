/* EasyCODE V8 */
/* EasyCODE ( 0 
abl3590.h,   Header fÅr die AblÑufe,
             Ersteller: Wilfried Halfmann   EES 2856 */
/* EasyCODE ( 0 
Definition der Klasse KlasseAblauf */
/* EasyCODE C */
/* EasyCODE < */
class KlasseAblauf:public KlasseTask
/* EasyCODE > */
{
        private:
        unsigned char anzauftrag;
        unsigned char next_anzauftrag;
        unsigned char varstr1[200];
        unsigned char varstr2[300];
        unsigned char varstr3[500];
        
        void ablanz (void);
public:
        unsigned char acanz, redanz;
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseAblauf(taskdata tdini):KlasseTask (tdini)
        {
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        set_anzauftrag(unsigned char auftrag) */
        /* EasyCODE F */
        void KlasseAblauf::set_anzauftrag (unsigned char auftrag)
        {
            anzauftrag = auftrag;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        unsigned char anzauftrag_ready(void) */
        /* EasyCODE F */
        unsigned char KlasseAblauf::anzauftrag_ready (void)
        {
            if ( anzauftrag == 0 )
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }

        /* EasyCODE ) */
        void abltask (void);
        void kinit(void);
        void winit (void);
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseAblauf abl;
/* EasyCODE ) */
