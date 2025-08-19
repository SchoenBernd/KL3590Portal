/* EasyCODE V8 */
/* EasyCODE ( 0 
comport.h */
// EMS
extern int emsseg;
#define EMS_INT 0x67
#define EMS_ERR -1

// serielle Schnittstellen
#define  COM1_ADR 0x03f8               // Ser. Schnittstelle com1
#define  COM2_ADR 0x02f8               // Ser. Schnittstelle com2
#define  LCCOM1 COM1_ADR+3             // Line Control Com1, read/write
#define  LCCOM2 COM2_ADR+3             // Line Control Com2, read/write
#define  BREAK_MSK 0x40                // Maske Break in Line Control

// LPT-Adresse
#define  LPTADR 0x378

/***** Definition der Boolean-Konstanten *****/
#define TRUE  1
#define FALSE 0

/***** Definition der maximalen ProgrammlÑnge *****/
#define MAXPROGLEN 0xfff0

/***** Definition Hauptbetriebsarten, (Inhalt von hbtart) *****/
#define POWER_ON 0
#define EINGABE 1
#define PRODUKTION 2

// Definitionen fuer Schablonenerkennung
#define NOCODE 0
#define RFID 1
#define BARCODE 2
//SW005-1
#define STATBARC 3

// Definitionen fuer Schlittentransport (Parameter 113)
#define INTERMITTIEREND 0
#define KONTINUIERLICH 1
#define AUTOMATISCH 2

// Definitionen der Materialdicke
#define DUENN 0
#define DICK  1

// Definition zur Erkennung der Maschinenklasse im geom. Datensatz
#define PORTAL 0x23
#define extvol extern volatile  // Kurzschreibweise

/***** Definitionen fÅr das Laden des Automatenantriebes von QUICK *****/
#define QP_ANZAHL 18       // Anzahl im Array quick_param
#define QP_KENNUNG 1
/* EasyCODE ( 0 
prodstruct */
/* EasyCODE C */
/* EasyCODE < */
/***** Struktur fÅr Produktionsdaten *****/
struct prodstruct
/* EasyCODE > */
{
        /* EasyCODE < */
        unsigned long betriebs_h;    // Betriebsstunden in 0,01 Stunden, Industriestunden.
            unsigned long produkt_h;     // Produktionsstunden in 0,01 Stunden, Industriestunden.
            unsigned long total_piececount; // Gesamtstueckzaehler
        /* EasyCODE > */
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
progpar,  Definition der Struktur progpar (Parameter von parprog) */
/* EasyCODE C */
/* EasyCODE < */
struct progpar
/* EasyCODE > */
{
        unsigned int stichl;
        int klcode;
        unsigned long laenge;
        unsigned int anzstiche;
        unsigned char kommentar [21];
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
highlow */
/* EasyCODE C */
/* EasyCODE < */
/***** union um byteweise auf einen Integer zugreifen zu kînnen *****/
union highlow
/* EasyCODE > */
{
        unsigned int word;
        /* EasyCODE C */
        /* EasyCODE < */
        struct
        /* EasyCODE > */
        {
                unsigned char low;
                unsigned char high;
        }byte;
        /* EasyCODE E */
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
highlowlong */
/* EasyCODE C */
/* EasyCODE < */
/***** union um int-weise auf einen long zugreifen zu kînnen *****/
union highlowlong
/* EasyCODE > */
{
        unsigned long dword;
        /* EasyCODE C */
        /* EasyCODE < */
        struct
        /* EasyCODE > */
        {
                unsigned int low;
                unsigned int high;
        }word;
        /* EasyCODE E */
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
Naehfeldgrenzen */
#ifdef NF120x70
    // Naehfeld 1200x700
    #define XNAEHFELD_MIN -6000  // in 1/10 mm
    #define XNAEHFELD_MAX  6000  // in 1/10 mm
    #define YNAEHFELD_MIN -7000  // in 1/10 mm
    #define YNAEHFELD_MAX  0     // in 1/10 mm
#endif 
#ifdef NF120x170
    // Naehfeld 1200x1700
    #define XNAEHFELD_MIN -6000  // in 1/10 mm
    #define XNAEHFELD_MAX  6000  // in 1/10 mm
    #define YNAEHFELD_MIN -17000 // in 1/10 mm
    #define YNAEHFELD_MAX  0     // in 1/10 mm
#endif 
#ifdef NF100x50
    // Naehfeld 1000x500
    #define XNAEHFELD_MIN -5000  // in 1/10 mm
    #define XNAEHFELD_MAX  5000  // in 1/10 mm
    #define YNAEHFELD_MIN -5000  // in 1/10 mm
    #define YNAEHFELD_MAX  0     // in 1/10 mm
#endif 
#ifdef NF120x200
    // Naehfeld 1200x2000
    #define XNAEHFELD_MIN -6000  // in 1/10 mm
    #define XNAEHFELD_MAX  6000  // in 1/10 mm
    #define YNAEHFELD_MIN -20000 // in 1/10 mm
    #define YNAEHFELD_MAX  0     // in 1/10 mm
#endif //
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition hfkoord */
/* EasyCODE C */
/* EasyCODE < */
typedef struct
/* EasyCODE > */
{
        int pos;    // in 0,1mm
        int motor;  // in Sm-Pulsen
}hfkoord;
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ) */
