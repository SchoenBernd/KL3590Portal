/* EasyCODE V8 */
/* EasyCODE ( 0 
screen.h */
/* EasyCODE < */
// Datei: screen.h
// Klasse: 3590
// Ersteller: Halfmann

// Download

typedef struct 
{
unsigned char * esc_z;     
unsigned char * path;
} download_zeiger;

#define PIKTOLOAD 0
#define VORLAGENLOAD 1


// Sanduhr
// #define SANDUHRPIKTO  PSANDUHR1   // Erstes von 8 Sanduhrpiktos. 

#define ANZBOTSCH 14      // Anzahl der Botschaften, wird benoetigt um eine
// ausreichend grosse Mailbox zur Anzeige zu definieren

#define eing_botsch1      Botsch[0]
#define nae_botsch1       Botsch[1]
#define nae_botsch2       Botsch[2]
#define abl_botsch1       Botsch[3]
#define abl_botsch2       Botsch[4]
#define abl_botsch3       Botsch[5]
#define abl_botsch4       Botsch[6]
#define schabl_botsch1    Botsch[7]
#define screen_botsch1    Botsch[8]
#define getval_botsch1    Botsch[9]
#define getval_botsch2    Botsch[10]
#define getval_botsch3    Botsch[11]
#define koor_botsch1      Botsch[12]
#define koor_botsch2      Botsch[13]
/* EasyCODE > */
/* EasyCODE ( 0 
Definition der Klasse KlasseBotschaft */
/* EasyCODE C */
/* EasyCODE < */
class KlasseBotschaft
/* EasyCODE > */
{
        private:
        /* EasyCODE < */
        struct botsch {
            unsigned char *string;
            unsigned int busy;
            } b;
        /* EasyCODE > */
public:
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseBotschaft()
        {
            b.busy = FALSE;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        busycheck */
        /* EasyCODE F */
        unsigned char busycheck(void)
        {
            if ( b.busy == 0xffff )
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
        get_busyvalue */
        /* EasyCODE F */
        unsigned int get_busyvalue(void)
        {
            return b.busy;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        clear busy (Busy auf FALSE setzen) */
        /* EasyCODE F */
        void clearbusy(void)
        {
            b.busy=FALSE;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        get string */
        /* EasyCODE F */
        unsigned char * getstring(void)
        {
            return b.string;
        }

        /* EasyCODE ) */
        char* senden(unsigned char *anzstring);
        char* binsenden(unsigned char *daten, unsigned int anzahl);
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseBotschaft Botsch[ANZBOTSCH];
/* EasyCODE ( 0 
Definition der Klasse KlasseScreen */
/* EasyCODE C */
/* EasyCODE < */
class KlasseScreen:public KlasseTask
/* EasyCODE > */
{
        private:
        /* EasyCODE ( 0 
        interne Daten */
        /* EasyCODE < */
        struct sanduhrstruct {
            unsigned char sanduhrreq;  // einschalten = True, ausschalten = False
            unsigned int pos_x;        // Die Sanduhrposition ist bezogen auf die
            unsigned int pos_y;        // obere Linke Touchecke
            };
            struct sanduhrstruct sanduhrpar; 
            
            unsigned char screen_varstr[60];        // variabler String 
            unsigned char screen_varstr2[60];        // variabler String 
            
            
            unsigned char screen_key;
            unsigned char sanduhrkey;
            unsigned char aktsanduhrstr[4];
            unsigned char delsanduhrstr[4];
            
            unsigned char linerunkey;               // AK
            
            unsigned char * stringzeiger;           // Adresse des akt. String
            unsigned int sanduhrcount;              // Zähler für sanduhr
            unsigned int linecount;                 // Zähler für sanduhr mit linerun  AK
            
            
            
            // Daten zum Touch Download
            
            unsigned char start_download;           // Falls True, dann downladen und keine Botschaften
            // bearbeiten.
            unsigned char downloadind;              // Welche Datei ist zu laden ?
            unsigned char download_status;
            char* pMerker;
        /* EasyCODE > */
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Prototypen */
        void sanduhr(void);
        void linerun(void);
        unsigned char touch_laden(void);
        /* EasyCODE ) */
public:
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseScreen(taskdata tdini):KlasseTask (tdini)
        {
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        screentask (RTK Task) */
        void screentask(void);
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        sanduhrreq_get */
        /* EasyCODE F */
        unsigned char sanduhrreq_get(void)
        {
            return sanduhrpar.sanduhrreq;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        sanduhrreq_set */
        /* EasyCODE F */
        void sanduhrreq_set(unsigned int sanduhr_position_x, unsigned int sanduhr_position_y)
        {
            /* EasyCODE < */
            sanduhrpar.pos_x = sanduhr_position_x;    // Bezogen auf oben links
                    sanduhrpar.pos_y = sanduhr_position_y;    // Bezogen auf oben links
                    sanduhrpar.sanduhrreq = TRUE;             // Sanduhr aktivieren
            /* EasyCODE > */
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        sanduhrreq_res */
        /* EasyCODE F */
        void sanduhrreq_res(void)
        {
            sanduhrpar.sanduhrreq=FALSE;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        download */
        /* EasyCODE F */
        void download(unsigned char downloadindex)
        {
            /* EasyCODE < */
            downloadind = downloadindex; // Gibt an, welche Datei mit welcher ESC Z Sequenz zu downloaden ist. 
                    download_status = 0;         // Auf noch nicht fertig initialisieren.
                    start_download = TRUE;       // Touch downladen anstarten
            /* EasyCODE > */
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        download_check */
        /* EasyCODE F */
        // Der Returnwert kann folgende Werte annehmen:
        //   0 ... Download ist noch nicht fertig.
        //   1 ... Download fehlerfrei und erfolgreich beendet.
        //   2 ... Fehler beim Öffnen oder Lesen der Datei, Download abgebrochen.
        //   3 ... Kein ACK vom Touch empfangen (_berwachungstimer abgelaufen)
        unsigned char download_check (void)
        {
            return download_status;
        }

        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseScreen screen ;
/* EasyCODE ) */
