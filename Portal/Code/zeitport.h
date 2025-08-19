/* EasyCODE V8 */
/* EasyCODE ( 0 
zeitport.h,   Header fuer die Zeitglieder,
              Ersteller: Wilfried Halfmann   EWE 2856 */
#define ANZAHL_ZEITGL 28

#define tkoor1 zeit.Zt[0]
#define tkoor2 zeit.Zt[1]
#define teing1 zeit.Zt[2]
#define teing2 zeit.Zt[3]
#define teing3 zeit.Zt[4]
#define tabl1 zeit.Zt[5]   // Doppelstart
#define tnae1 zeit.Zt[6]
#define tnae2 zeit.Zt[7]
#define tnae3 zeit.Zt[8]
#define tnae4 zeit.Zt[9]   // Zykluszeitmessung
#define tzuf1 zeit.Zt[10]
#define tio1 zeit.Zt[11]
#define tio2 zeit.Zt[12]   // Blasluft y1
#define tio3 zeit.Zt[13]   // Unterfadenklemme
#define tio4 zeit.Zt[14]   // Fadenwedlerklemme
#define tio5 zeit.Zt[15]   // Fadenwedler-Funktion, y2fkt
#define tio6 zeit.Zt[16]   // Fadenklemme-Funktion, y4fkt
#define tio7 zeit.Zt[17]   // Niederhalter y8
#define tio8 zeit.Zt[18]   // Oberfaden rueckziehen y9fkt
#define tio9 zeit.Zt[19]   // SW013
#define tio10 zeit.Zt[20]   // SW001DIS
#define tio11 zeit.Zt[21]   // SW001DIS
#define tio12 zeit.Zt[22]   // SW001DIS
                               
#define ttanz1 zeit.Zt[23]
#define tnminf1 zeit.Zt[24]
#define tnminf2 zeit.Zt[25]
#define tnminf3 zeit.Zt[26]
#define tschabl1 zeit.Zt[27]
/* EasyCODE ( 0 
Definition der Klasse zeitglied */
/* EasyCODE C */
/* EasyCODE < */
class zeitglied
/* EasyCODE > */
{
        private:
        unsigned char enable;
        unsigned int wert;
public:
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        zeitglied ()
        {
            enable = FALSE;
            wert = 0;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        sz,   Zeitglied setzen */
        /* EasyCODE F */
        void sz(unsigned int w)
        {
            if ( enable==0 )
            {
                wert=w; enable=0xff;
            }
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        rsz,   Zeitglied rÅcksetzen */
        /* EasyCODE F */
        void rsz(void)
        {
            enable=0; wert=0x100;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        lz,  Zeitglied lˆschen */
        /* EasyCODE F */
        void lz(void)
        {
            enable=0; wert=0;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        tz,   Zeitglied abfragen */
        /* EasyCODE F */
        unsigned char tz(void)
        {
            if ( wert==0 )
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        dekrz,   Zeitglied dekrementieren */
        /* EasyCODE F */
        void dekrz (void)
        {
            if ( enable==0xff )
            {
                RTKDisableInterrupts ();
                if ( wert != 0 )
                {
                    wert--;
                }
                RTKEnableInterrupts ();
            }
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        getwert,   Wert des Zeitglied abfragen */
        /* EasyCODE F */
        unsigned int getwert (void)
        {
            return wert;
        }

        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse KlasseZeit */
/* EasyCODE C */
/* EasyCODE < */
class KlasseZeit:public KlasseTask
/* EasyCODE > */
{
        private:
        unsigned int btrcount;
        unsigned int prodcount;
        unsigned char btrcounten;
        unsigned char prodcounten;
public:
        zeitglied Zt[ANZAHL_ZEITGL];
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseZeit(taskdata tdini):KlasseTask (tdini)
        {
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Enable Betriebsstundenz‰hler,   set_btrcounten */
        /* EasyCODE F */
        void set_btrcounten (void)
        {
            btrcounten = TRUE;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Enable Produktionsstundenz‰hler,   set_prodcounten */
        /* EasyCODE F */
        void set_prodcounten (void)
        {
            prodcounten = TRUE;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Disable Produktionsstundenz‰hler,   res_prodcounten */
        /* EasyCODE F */
        void res_prodcounten (void)
        {
            prodcounten = FALSE;
        }

        /* EasyCODE ) */
        void zeittask (void);
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseZeit zeit;
/* EasyCODE ) */
