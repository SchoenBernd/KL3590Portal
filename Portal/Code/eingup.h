/* EasyCODE V8 */
/* EasyCODE ( 0 
eingup.h */
/* EasyCODE ( 0 
pargvstruct */
/* EasyCODE C */
/* EasyCODE < */
// Struktur fuer den Zugriff auf den Getvalue Parameterblock 
struct pargvstruct
/* EasyCODE > */
{
        unsigned char auftrag;         // Entscheidung ueber Sondertastenbehandlung
        long int altwert;              // alter Wert
        long int maxwert;              // maximaler Wert 
        long int minwert;              // minimaler Wert
        unsigned char kommastellen;    // Anzahl der anzuzeigenden Kommastellen
        unsigned char *pikto;          // Zeiger auf anzuzeigende, unveraenderliche Piktos
        unsigned char *string;         // Zeiger auf den anzuzeigenden String
        // unsigned char *zulstring;   // Zullaessig String 
        unsigned char *sonderstring;   // String zur Anzeige von Sondertasten.
        unsigned char *sonder2string;  // String zur Anzeige von Bereichen und Kommentartext bei pnr Wahl.
        unsigned char tastencode;      // Aktuelle gedrueckte Taste vorm getvalue Aufruf (nur fuer getvalue_scr)
        long int      wert;            // Rueckgabewert von getvalue
        unsigned char taste;           // Rueckgabe des Tochcodes der Sondertasten von getvalue
};
/* EasyCODE E */
/* EasyCODE ) */
extvol struct pargvstruct pargetval;
/* EasyCODE ( 0 
pargetstringstruct */
/* EasyCODE C */
/* EasyCODE < */
// Struktur fuer den Zugriff auf den Getstring Parameterblock 
struct pargetstringstruct
/* EasyCODE > */
{
        /* EasyCODE < */
        unsigned char *altstring;     // alter String (ohne Pikto)
        unsigned char *string;        // Zeiger auf den anzuzeigenden String (Pikto, Formalparameter)
        unsigned char anz_char;       // max. Anzahl der eingebbaren Zeichen
        unsigned char neustring[30] ;     // Rueckgabewert von getstring
        /* EasyCODE > */
};
/* EasyCODE E */
/* EasyCODE ) */
extvol struct pargetstringstruct pargetstring;
/* EasyCODE ( 0 
pargvpinstruct */
/* EasyCODE C */
/* EasyCODE < */
// Struktur fuer den Zugriff auf den Getstring Parameterblock 
struct pargvpinstruct
/* EasyCODE > */
{
        /* EasyCODE < */
        unsigned char auftrag;
        unsigned char *sonderstring;    // String zur Anzeige der beiden Piktos
        unsigned char anz_char;         // max. Anzahl der eingebbaren Zeichen
        long int wert;                  // Rueckgabewert von getvalue
        /* EasyCODE > */
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE < */
extvol struct pargvpinstruct pargvpin;

// Globale Prototypen
unsigned char getvalue_eing();
unsigned char getvalue_abl();
unsigned char getvalue_scr();
unsigned char getstring ();
unsigned char getvalue_scr();
unsigned char diskstart(void);
unsigned char getkey (void);
unsigned char getkey_los (void);
unsigned char taste_los (void);
unsigned char fkeytst(unsigned char fktkz);
void inv_sprachtaste(void);
void error_res(void);

// Externals
extvol unsigned char getvalkey;
extern unsigned char fkey_ur [FKEY_USER_MAX][FKEY_FUNK_MAX];
/* EasyCODE > */
/* EasyCODE ) */
