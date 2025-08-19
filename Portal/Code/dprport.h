/* EasyCODE V8 */
/* EasyCODE ( 0 
dpr3590.h */
// Datei: dpr3590.h, Headerdatei fuer PC104 auf der BS3
// Ersteller: Wilfried Halfmann

// Basisadresse
#define DPRBASE ((unsigned char *) MK_FP(dprseg,0))
/* EasyCODE ( 0 
Definition der Struktur eines Schrittmotor-Interfaces im
Dual-Port-Ram auf der BS3 */
/* EasyCODE C */
/* EasyCODE < */
typedef struct
/* EasyCODE > */
{
        /* EasyCODE < */
        unsigned char bef;               // Handshakebyte
        unsigned char mod;               // Status
        unsigned int adr;                // Rampenauswahl
        unsigned int vort;               // Vorteiler
        unsigned int verz;               // Verzˆgerung
        unsigned int gesp;               // Gesamtpulszahl
        unsigned int dachp;              // Pulse bis Dachende
        unsigned int apul;               // ausgegebene Pulse
        unsigned char stat;              // RÅckmeldung
        unsigned char eing;              // EingÑnge
        unsigned char stop;              // Stopbyte
        /* EasyCODE > */
}smstruct;
/* EasyCODE E */
#define SMZ ((volatile smstruct *) MK_FP(dprseg,0x200))
#define SMX ((volatile smstruct *) MK_FP(dprseg,0x220))
#define SMY ((volatile smstruct *) MK_FP(dprseg,0x240))
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Struktur eines Interfaces zum Ansteuern 
eines Schrittmotors zum Bewegen des HÅpferfu· wÑhrend
des NÑhens im Dual-Port-Ram auf der BS3 */
/* EasyCODE C */
/* EasyCODE < */
typedef struct
/* EasyCODE > */
{
        unsigned char enable;            // Enable
        unsigned char fehler;            // Fehler
        unsigned char smplus;            // Bitmuster Plus-Richtung des Sm
        unsigned char smminus;           // Bitmuster Minus-Richtung des Sm
        int posoben;                     // Position oben (Motorkoord.)
        int posunten;                    // Position unten (Motorkoord.)
        unsigned int startauf;           // Startposition auf (Inkr.)
        unsigned int startab;            // Startposition ab (Inkr.)
        unsigned int istpos;             // Istposition (Motorkoord.)
}huepferstruct;
/* EasyCODE E */
#define HF ((volatile huepferstruct *) MK_FP(dprseg,0x260))
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Struktur eines Naehmotor-Interfaces im
Dual-Port-Ram auf der BS3 */
/* EasyCODE C */
/* EasyCODE < */
typedef struct
/* EasyCODE > */
{
        unsigned char pbef;              // Handshakebyte f¸r die Pulsauswertung
        unsigned char stbpaw;            // Steuerbyte f¸r die Pulsauswertung
        unsigned char stbn8;             // Steuerbyte f¸r die Sonderfunktion Nocken 8
        unsigned char posmsk;            // aktueller Nockenwert
        int aktpos;                      // aktuelle Position
        unsigned char nu_1 [8];          // freie Bytes
        unsigned char postab [256+1];    // Tabelle der Nockenwerte
        unsigned char nu_2;              // freies Byte
            
        unsigned char sbef;              // Handshakebyte f¸r die serielle Schnittstelle
        unsigned char sermod;            // Betriebsart der seriellen Schnittstelle
        unsigned char rbufst;            // Status Empfangsringpuffer
        unsigned char rbufrd;            // Read-Pointer des Empfangsringpuffers
        unsigned char rbufwr;            // Write-Pointer des Empfangsringpuffers
        unsigned char sbufrd;            // Read-Pointer des Senderingpuffers
        unsigned char sbufwr;            // Write-Pointer des Senderingpuffers
        unsigned char nu_3 [9];          // freie Bytes
        unsigned char rbuf [16];         // Empfangsringpuffer
        unsigned char sbuf [16];         // Senderingpuffer
           
        unsigned char dbef;              // Handshakebyte f¸r die Drehzahlmessung
        unsigned char nu_4;              // freies Byte
        unsigned int messzt;             // Messzeit
        unsigned int count;              // Anzahl Pulse x 4 in der Messzeit
}nmstruct;
/* EasyCODE E */
/* EasyCODE < */
#define NM ((volatile nmstruct *) MK_FP(dprseg,0x300))
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Struktur eines CAN-Interfaces im
Dual-Port-Ram auf der BS3 */
/* EasyCODE C */
/* EasyCODE < */
typedef struct
/* EasyCODE > */
{
        /* EasyCODE < */
        unsigned int out_soll [8];       // Sollwerte f¸r die Ausgangsknoten
        unsigned char nu_1 [16];         // freie Bytes
            
        unsigned int in [8];             // Abbild der Eingangsknoten
        unsigned char nu_2 [16];         // freie Bytes
           
        unsigned char status [8];        // Status der Knoten
        unsigned char nu_3 [24];         // freie Bytes
            
        unsigned int out_ist [8];        // Istwerte f¸r die Ausgangsknoten
        unsigned char nu_4 [16];         // freie Bytes
            
        unsigned char version;           // Firmware-Version
        unsigned char adr;               // Knotenadresse des Interfaces
        unsigned char maxknotennr;       // hˆchste Knotennr.
        unsigned char start;             // Start-Signal vom PC104
        unsigned char chmakt;            // Auftrag Chopper-Mode aktualisieren vom PC104
        unsigned char nu_5;              // freies Byte
        unsigned char reset;             // Soft-Reset
        unsigned char akterror;          // aktuelle fehlerhafte Knotennummer
        unsigned char error;             // vorletzte fehlerhafte Knotennummer
        unsigned char errorstat;         // Status des fehlerhaften Knotens
        unsigned char anzknoten;         // Anzahl aktive Knoten
        unsigned char nu_6 [5];          // freie Bytes
            
        unsigned int choppermask [8];    // Choppermaske f¸r die Ausgangsknoten
        unsigned int startzt [8];        // Startzeitpunkt zum Choppern
        unsigned char auszt [8];         // Aus-Zeit beim Choppern
        unsigned char einzt [8];         // Ein-Zeit beim Choppern
        /* EasyCODE > */
}canstruct;
/* EasyCODE E */
/* EasyCODE < */
#define CAN ((volatile canstruct *) MK_FP(dprseg,0x530))
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE ( 0 
allg. Adressen und Adressen beim downladen */
/* EasyCODE < */
// Allgemeine Adressen

#define DPRVersion ((unsigned char *) MK_FP(dprseg,0x7e0))           // Zeiger auf Softwareversion im DPR
#define C167_WATCH (*(volatile unsigned char *) MK_FP(dprseg,0x7fb)) // Watchdog: wird von C167 zyklisch auf 0xff gesetzt
#define C167_ERR (*(volatile unsigned char *) MK_FP(dprseg,0x7fc))   // Fehlermeldung des C167
#define C167_READY (*(volatile unsigned char *) MK_FP(dprseg,0x7fd)) // Fertigmeldung Power On des C167
#define C167_WR (*(volatile unsigned char *) MK_FP(dprseg,0x7fe))    // Schreibzugriff des C167 lˆst Interrupt bei PC104 aus
#define PC104_WR (*(volatile unsigned char *) MK_FP(dprseg,0x7ff))   // Schreibzugriff des PC104 lˆst Interrupt bei C167 aus
/* EasyCODE > */
/* EasyCODE C */
/* EasyCODE < */
typedef struct
/* EasyCODE > */
{
        unsigned long int adr;
        unsigned int anz;
        unsigned int data [100];
}bootstruct;
/* EasyCODE E */
/* EasyCODE < */
#define FLASH ((volatile bootstruct *) MK_FP(dprseg,0x700))
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE ) */
