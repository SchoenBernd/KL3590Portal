/* EasyCODE V8 */
/* EasyCODE ( 0 
hfportal.h,   Header fÅr Unterprogramme fuer motorisch angetriebenen Huepferfuss,
              Ersteller: Wilfried Halfmann   EWE 2856 */
/* EasyCODE ( 0 
Definition Initiatorpunkt */
#define INITIATOR_POS 140
#define INITIATOR_MOTOR 59
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition hoechster Punkt */
#define MAX_POS 200
#define MAX_MOTOR 85
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition Fadenschneidposition */
#define FCUT_POS 200
#define FCUT_MOTOR 85
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition Einstellposition */
#define HFEINSTELL_POS 50
#define HFEINSTELL_MOTOR 21
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition Voreinstellung Position unten */
#define POSUNTEN_POS 20
#define POSUNTEN_MOTOR 9
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse KlasseHuepferfuss */
/* EasyCODE C */
/* EasyCODE < */
class KlasseHuepferfuss
/* EasyCODE > */
{
        private:
        /* EasyCODE [ 
        Daten */
        /* EasyCODE ] */
public:
        /* EasyCODE [ 
        Daten */
        // aktuelle Werte (koennen durch Naehprogramm veraendert werden)
        hfkoord akt_posunten;
        unsigned int akt_hub;   // in 0,1 mm
        
        hfkoord istpos;
        unsigned char posok;
        unsigned char posinitkey;
        unsigned char auf_initkey;
        /* EasyCODE ] */
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseHuepferfuss(void)
        {
        }

        /* EasyCODE ) */
        /* EasyCODE F */
        unsigned char ready (void)
        {
            return smz.ready();
        }

        /* EasyCODE F */
        hfkoord getpos (void)
        {
            return istpos;
        }

        /* EasyCODE F */
        void set_istpos (hfkoord position)
        {
            istpos = position;
        }

        /* EasyCODE [ 
        Prototypen */
        void auf (void);
        void ab (void);
        void hfmove (int sollpos);
        void postomo (hfkoord *pos);
        unsigned char posinit(void);
        void kaltinit (void);
        unsigned char auf_init ();
        /* EasyCODE ] */
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseHuepferfuss hfuss;
/* EasyCODE ) */
