/* EasyCODE V8 */
/* EasyCODE ( 0 */
// Datei: dprbs3.h, Headerdatei für C167
// Ersteller: Wilfried Halfmann, Edi Bastian

// Definition der Struktur eines Schrittmotor-Interfaces im
// Dual-Port-Ram auf der BS3
// (Einzelne Bitpositionen siehe gesonderte DPR-Beschreibung)
/* EasyCODE ( 0 
smstruct */
/* EasyCODE C */
/* EasyCODE < */
typedef struct
/* EasyCODE > */
{
        unsigned char bef;               // Handshakebyte
        unsigned char mod;               // Status
        unsigned int adr;                // Rampenauswahl
        unsigned int vort;               // Vorteiler
        unsigned int verz;               // Verzögerung
        unsigned int gesp;               // Gesamtpulszahl
        unsigned int dachp;              // Pulse bis Dachende
        unsigned int apul;               // ausgegebene Pulse
        unsigned char stat;              // Rückmeldung
        unsigned char eing;              // Eingänge
        unsigned char stop;              // Stopbyte
}smstruct;
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
smhstruct */
/* EasyCODE C */
/* EasyCODE < */
typedef struct
/* EasyCODE > */
{
        unsigned char ena;                // Enable des automatischen Huepferfussablaufs
        unsigned char fehler;             // Huepferfussfehler
        unsigned char smzplus;            // Plus = hoch
        unsigned char smzminus;           // Minus = runter
                 int  posoben;            // Soll Position oben in Motorkoordinaten
                 int  posunten;           // Soll Position unten in Motorkoordinaten
        unsigned int  startauf;           // Startpostion Huepferfuss auf in Inkremente
        unsigned int  startab;            // Startpostion Huepferfuss ab in Inkremente
        unsigned int  istpos;             // Istposition (Ausgangsposition) in Motorkoordinaten
}smhstruct;
/* EasyCODE E */
/* EasyCODE ) */
#define SM1  ((volatile smstruct *) 0x0200)   // portal Huepferfussantrieb
#define SM2  ((volatile smstruct *) 0x0220)   // portal X-Achse
#define SM3  ((volatile smstruct *) 0x0240)   // portal Y-Achse
#define SMH  ((volatile smhstruct*) 0x0260)   // portal Huepferfussantrieb Sondereinstellungen, verwendet SM1.

// Definition der Struktur eines Nähmotor-Interfaces im
// Dual-Port-Ram auf der BS3
#ifdef FIRM480
    #define HPULSE 480   // Anzahl Handradpulse
#else 
    #define HPULSE 256   // Anzahl Handradpulse
#endif 
/* EasyCODE ( 0 
nmstruct */
/* EasyCODE C */
typedef struct
{
        unsigned char pbef;              // Handshakebyte für die Pulsauswertung
        unsigned char stbpaw;            // Steuerbyte für die Pulsauswertung
        unsigned char stbn8;             // Steuerbyte für Sonderfunktion Nocken 8
        unsigned char posmsk;            // aktueller Nockenwert
        int aktpos;                      // aktuelle Position
        unsigned char nu_1 [8];          // freie Bytes
        unsigned char postab [HPULSE + 1];  // Tabelle der Nockenwerte
        unsigned char nu_2;              // freies Byte
        
        unsigned char sbef;              // Handshakebyte für die serielle Schnittstelle
        unsigned char sermod;            // Betriebsart der seriellen Schnittstelle
        unsigned char rbufst;            // Status Empfangsringpuffer
        unsigned char rbufrd;            // Read-Pointer des Empfangsringpuffers
        unsigned char rbufwr;            // Write-Pointer des Empfangsringpuffers
        unsigned char sbufrd;            // Read-Pointer des Senderingpuffers
        unsigned char sbufwr;            // Write-Pointer des Senderingpuffers
        unsigned char nu_3 [9];          // freie Bytes
        unsigned char rbuf [16];         // Empfangsringpuffer
        unsigned char sbuf [16];         // Senderingpuffer
        
        unsigned char dbef;              // Handshakebyte für die Drehzahlmessung
        unsigned char nu_4;              // freies Byte
        unsigned int messzt;             // Messzeit
        unsigned int count;              // Anzahl Pulse x 4 in der Messzeit
}nmstruct;
/* EasyCODE E */
/* EasyCODE ) */
#define NM ((volatile nmstruct *) 0x0300)
/* EasyCODE ( 0 
canstruct */
/* EasyCODE C */
// Definition der Struktur eines Can-Interfaces im
// Dual-Port-Ram auf der BS3

typedef struct
{
        unsigned int out_soll [8];       // Sollwerte für die Ausgangsknoten
        unsigned char nu_1 [16];         // freie Bytes
        
        unsigned int in [8];             // Abbild der Eingangsknoten
        unsigned char nu_2 [16];         // freie Bytes
        
        unsigned char status [8];        // Status der Knoten
        unsigned char nu_3 [24];         // freie Bytes
        
        unsigned int out_ist [8];        // Istwerte für die Ausgangsknoten
        unsigned char nu_4 [16];         // freie Bytes
        
        unsigned char version;           // Firmware-Version
        unsigned char adr;               // Knotenadresse des Interfaces
        unsigned char maxknotennr;       // höchste Knotennr.
        unsigned char start;             // Start-Signal vom PC104
        unsigned char chmakt;            // Auftrag Chopper-Mode aktualisieren vom PC104
        unsigned char nu_5;              // freies Byte
        unsigned char reset;             // Soft-Reset
        unsigned char akterror;          // aktuelle fehlerhafte Knotennummer
        unsigned char error;             // vorletzte fehlerhafte Knotennummer
        unsigned char errorstat;         // Status des fehlerhaften Knotens
        unsigned char anzknoten;         // Anzahl aktive Knoten
        unsigned char nu_6 [5];          // freie Bytes
        
        unsigned int choppermask [8];    // Choppermaske für die Ausgangsknoten
        unsigned int startzt [8];        // Startzeitpunkt zum Choppern
        unsigned char auszt [8];         // Aus-Zeit beim Choppern
        unsigned char einzt [8];         // Ein-Zeit beim Choppern
}canstruct;
/* EasyCODE E */
/* EasyCODE ) */
#define CAN ((volatile canstruct *) 0x0530)


// Allgemeine Adressen

#define DPRVectorTable ((unsigned int *) 0)               // Zeiger auf Vectortable im DPR
#define DPR_ResRoutine ((unsigned char *) 0x7b0)          // Zeiger auf Fang-Routine fuer unzulaessigen Reset
#define DPRVersion ((unsigned char *) 0x7e0)              // Zeiger auf Softwareversion im DPR
#define C167_WATCH (*(volatile unsigned char *) 0x07fb)   // Watchdog: wird von C167 zyklisch auf 0xff gesetzt
#define C167_ERR (*(volatile unsigned char *) 0x07fc)     // Fehlermeldung des C167
#define C167_READY (*(volatile unsigned char *) 0x07fd)   // Fertigmeldung Power On des C167
#define C167_WR (*(volatile unsigned char *) 0x07fe)      // Schreibzugriff des C167 löst Interrupt bei PC104 aus
#define PC104_WR (*(volatile unsigned char *) 0x07ff)     // Schreibzugriff des PC104 löst Interrupt bei C167 aus
/* EasyCODE ( 0 
bootstruct */
/* EasyCODE C */
// Adressen beim Downladen

typedef struct
{
        unsigned int xhuge * adr;
        unsigned int anz;
        unsigned int data [100];
}bootstruct;
/* EasyCODE E */
/* EasyCODE ) */
#define FLASH ((volatile bootstruct *) 0x700)


// Fehlermeldungen des C167 (C167_ERR)
                      // 0 - kein Fehler
                      // 1 - Fehler beim Flash loeschen
                      // 2 - Fehler beim Flash programmieren
#define NMI_Error 10       // 10 - Non-Maskable Interrupt
#define STKOF_Error 11     // 11 - Stack Overflow
#define STKUF_Error 12     // 12 - Stack Underflow
#define CLASSB_Error 13    // 13 - Class B Hardware Trap
#define RES_Error 14       // 14 - unzulaessiger Reset
/* EasyCODE ) */
