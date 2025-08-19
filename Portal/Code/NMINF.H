/* EasyCODE V8 */
/* EasyCODE ( 0 
nminf.h */
/* EasyCODE ( 0 
Definitionen */
/***********************************************************************
* Klasse:     3590 Portal                                                    *
* Ersteller:  Halfmann  EWE 2856                                       *
************************************************************************/

/***** Initialisierungswerte fÅr die Pulsauswertung *****/
#define  STBPAW  1                     // Steuerbyte fÅr die Pulsauswertung
// 00000001
// ||||||||______ Sonderfunktion Nocken 8 ein
// |||||||_______ Nockenschaltwerk aus
// ||||||________ Modus Nockenschaltwerk
// |||||_________ Z‰hlrichtung Pulse
// ||||__________ frei
// |||___________ frei
// ||____________ frei
// |_____________ frei

/***** Initialisierungswerte fÅr die serielle Schnittstelle *****/
#define  SERMOD   0X06                 // Betriebsart serielle Schnittstelle
// 00000110
// ||||||++_______ 9600 Baud
// ||||||_________ Parity Enable
// |||||__________ gerade Parit‰t
// ||||___________ frei
// |||____________ frei
// ||_____________ frei
// |______________ frei

/***** Initialisierungswerte fuer die Drehzahlmessung *****/
/* EasyCODE - */
#define  MESSZT   1526                 // Initialisierung der Messzeit
// Mit dieser Initialisierung wird ein Messzyklus von 4,883ms fuer die Drehzahl
// erreicht. Fuer die Drehzahl x (in upm) gilt: x = 12 * COUNT
/* EasyCODE - */
/***** Definition der Struktur des Parameterblocks von nminfbs3 *****/
/***** Definition der Bitpositionen im Byte nmpb.posart *****/
#define DREHRICHTUNG  0x01
#define KURZ      0x02
#define VORWAERTS 0
#define RUECKWAERTS 0x01

/***** sonstige Definitionen *****/
#define PULSE_PRO_UMDR 256 // Handradpulse pro Umdrehung (rueckgemeldet bei Pos.-Abfrage)
#define MAXPOS 128         // max. Positionsvorgabe in Pulse
#define GSWON 2            // Einschaltgeschwindigkeit
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse NminfBS3 */
/* EasyCODE C */
/* EasyCODE < */
class KlasseNminfBS3:public KlasseTask
/* EasyCODE > */
{
        private:
        // Daten
        nmstruct *dprptr;
        unsigned char rbuffer [15]; // Empfangsbuffer
        unsigned char sbuffer [5]; // Sendebuffer
        unsigned char versuche, nmkey, nmauftrag;
        unsigned char nminit_ok, quick_posok, nmponrdy, nm_ponerr;
        unsigned char totmannsenddi; // Abschaltmîglichkeit fÅr das Senden der Totmann-Sequenz
        // Speicher fuer verschiedene UPs, die erforderlich sind, damit der
        //  Compiler die volatile definierten Speicher richtig behandelt !
        int pos1;   // Position fuer UP nadel_ot
        int pos2;   // Position fuer UP nadel_ber
        //int pos3;   // Position fuer UP taktnadel_ber
        int pos4;   // Position fuer UP nadel_pos
        unsigned int drehz;  // Drehzahl fuer UP get_drehzahl
        // Funktionen
        void befsend (unsigned char bef);
        unsigned char rec_antwort (unsigned char bef, unsigned char antwort, unsigned char daten);
        unsigned char recbytes (void);
        void clear_rbuf (void);
        void readbuf (void);
        void writebbuf (unsigned char wert);
        unsigned char datacheck (unsigned char byteanz);
        void build_checksum (unsigned char byteanz);
        
        void totmann (void);
        void reset (void);
        void init (void);
        void positionieren (void);
        void drehzahlausgabe (void);
        void not_stop (void);
        void referenz1 (void);
        void referenz2 (void);
        void restbremse_ein (void);
        void restbremse_aus (void);
        void pos_drehzahl (void);
        void tot_zeit (void);
        void read_param (void);
        void write_param (void);
        void softwareversion (void);
public:
        // Parameterblock
        unsigned char befehl;
        unsigned char position;
        unsigned char posart;
        int drehzahl;
        int stichzahl;
        unsigned char parameternr;
        unsigned char parameter;
        unsigned char motor_steht;
        unsigned char fehler;
        unsigned char version;
        unsigned char softvers[10];
        // Zur Fehlersuche
        unsigned char lastbef;
        int pos;
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseNminfBS3 (taskdata tdini, unsigned int dproffs):KlasseTask (tdini)
        {
            dprptr = (nmstruct *)MK_FP(axtoi(getenv("SAFSEG"))+0x700, dproffs);
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        get_ponrdy */
        /* EasyCODE F */
        unsigned char get_ponrdy ()
        {
            return nmponrdy;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        get_ponerr */
        /* EasyCODE F */
        unsigned char get_ponerr ()
        {
            return nm_ponerr;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        get_quick_posok */
        /* EasyCODE F */
        unsigned char get_quick_posok ()
        {
            return quick_posok;
        }

        /* EasyCODE ) */
        void nminftask (void);
        void winit (void);
        void kinit (void);
        void pawini (void);
        unsigned char nadel_ot (void);
        unsigned char nadel_bereich (int bereich);
        //unsigned char taktnadel_ber (void);
        unsigned int nadel_pos (void);
        unsigned int get_drehzahl (void);
        void gsw_fastout (void);
        void tot_fastout (void);
};
/* EasyCODE E */
/* EasyCODE ) */
extern unsigned char quick_paramnr [QP_ANZAHL];
extern KlasseNminfBS3 nminf;
/* EasyCODE ) */
