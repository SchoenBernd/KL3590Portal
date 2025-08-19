/* EasyCODE V8 */
/* EasyCODE ( 0 
progport.h */
// progport.h, Definitionen fuer das Programmieren/Korrigieren
// Klasse: 3590 Portal
// Ersteller: Halfmann   EWE 2856
/* EasyCODE ( 0 
Definition der Abschnittsarten */
#define    AB_ANNAEH          0x5F      /* ANNAEHEN */
#define    AB_SCHNEID         0x60      /* SCHNEIDEN */
#define    AB_ANFRIEGEL       0x79      /* Anfangsriegel */
#define    AB_ENDRIEGEL       0x7A      /* Endriegel */
#define    AB_NAEHAUS         0x65      /* NAEHEN AUS */
#define    AB_PRENDE          0x66      /* PROGRAMMENDE */
#define    AB_HIND            0x76      /* HINDERNIS */
#define    AB_STDST           0x72      /* STANDARD-STICHLAENGE */
#define    AB_STICHL          0x84      /* STICHLAENGE */
#define    AB_STICHB          0x85      /* STICHBREITE */
#define    AB_PNR             0x5C      /* PROGRAMMNUMMER */
#define    AB_KLCODE          0x61      /* KLAMMERCODE */
#define    AB_MNP             0x8E      /* MASCHINEN-NULLPUNKT */
#define    AB_AP              0x90      /* ANFANGSPUNKT */
#define    AB_EIL             0x67      /* EILGANG */
#define    AB_STICH           0x55      /* STICH */
#define    AB_GERADE          0x4E      /* GERADE */
#define    AB_BOGENSTP        0x52      /* BOGENSTUETZPUNKT */
#define    AB_BOGENENDP       0x4F      /* BOGENENDPUNKT */
#define    AB_KREISSTP        0x53      /* KREISSTUETZPUNKT */
#define    AB_KREISENDP       0x50      /* KREISENDPUNKT */
#define    AB_KURVSTP         0x51      /* KURVENSTUETZPUNKT */
#define    AB_KURVENDP        0x54      /* KURVENENDPUNKT */
#define    AB_UFWSTOP         0x63      /* STOP bei Unterfadenstoerung*/
#define    AB_PSTOP           0x64      /* PROGRAMMIERTER STOP */
#define    AB_REDGSWEIN       0x86      /* REDUZIERTE GESCHWINDIGKEIT EIN */
#define    AB_REDGSWAUS       0x87      /* REDUZIERTE GESCHWINDIGKEIT AUS */
#define    AB_ZUSPGEIN        0x81      /* ZUSATZSPANNUNG EIN */
#define    AB_ZUSPGAUS        0x82      /* ZUSATZSPANNUNG AUS */
#define    AB_OUTSET          0x6B      /* AUSGANG SETZEN */
#define    AB_OUTRES          0x6C      /* AUSGANG RUECKSETZEN */
#define    AB_FUSSNIVEAU      0x8A      /* Niveau Huepferfuss (Wert)*/
#define    AB_FUSSHUB         0x8B      /* Hub Huepferfuss (Wert)*/
#define    AB_FUSSNIVEAUSTUFE 0x94      /* Niveau Huepferfuss (Stufe)*/
#define    AB_FUSSHUBSTUFE    0x95      /* Hub Huepferfuss (Stufe)*/
#define    AB_WHIGH           0x6D      /* WARTEN AUF HIGH */
#define    AB_WLOW            0x6E      /* WARTEN AUF LOW */
#define    AB_WZEIT           0x70      /* WARTEN AUF ZEIT */
#define    AB_DREHZ           0xA6      /* DREHZAHL */
#define    AB_FLAGSET         0xB0      /* Flag setzen */
#define    AB_FLAGRES         0xB1      /* Flag ruecksetzen */
#define    AB_MKENN           0xFF      /* MASCHINEN-KENNUNG */
/* EasyCODE ) */
/* EasyCODE ( 0 
Weitere Defines */
/* EasyCODE < */
#define SYMX 0
#define SYMY 1
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE ( 0 
abschstruct */
/* EasyCODE C */
/* EasyCODE < */
struct abschstruct
/* EasyCODE > */
{
        unsigned char art;
        int par1;
        int par2;
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
p_abschstruct */
/* EasyCODE < */
/***** Struktur der Abschnitte (byte allignment) *****/
// nur fuer Programmbereich benutzbar (nur 5 Bytes lang)
// nicht fuer Arbeitsbereich benutzbar (wegen word allignment 6 Bytes lang)
/* EasyCODE > */
/* EasyCODE < */
#pragma option -a-
/* EasyCODE > */
/* EasyCODE C */
/* EasyCODE < */
struct p_abschstruct
/* EasyCODE > */
{
        unsigned char art;
        int par1;
        int par2;
};
/* EasyCODE E */
/* EasyCODE < */
#pragma option -a.
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE ( 0 
pzblstruct */
/* EasyCODE C */
/* EasyCODE < */
// Struktur des Programmzustandsblocks
struct pzblstruct
/* EasyCODE > */
{
        unsigned char faden;
        unsigned char pout1;
        unsigned char pout2;
        unsigned char pout3;
        unsigned char pout4;
        unsigned char pout6;          // Oberteil heben beim Verfahren
        unsigned char zusp;
        unsigned char redgsw;
        unsigned char kurve;
        unsigned int stichl;
        unsigned int posunten;        // Position Huepferfuss unten in 0,1 mm
        unsigned int hub;             // Huepferfusshub in 0,1 mm
        unsigned char flag1;          // Nadeldreheinrichtung aus
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProg */
/* EasyCODE C */
/* EasyCODE < */
class KlasseProg
/* EasyCODE > */
{
        private:
        /* EasyCODE < */
        struct progpar pnr_par;
        
        unsigned char brotherpos(void);
        /* EasyCODE > */
public:
        /* EasyCODE < */
        unsigned char key;
        unsigned char konverr;
        offs_adr masch_adr;
        unsigned int pnr, absch_fehler;
        struct p_abschstruct * ppointer;       // Zeiger auf Programmdaten
        struct p_abschstruct * blockanf;       // Zeiger auf Blockanfang im Programmdatenbereich (NULL = ungltig)
        struct p_abschstruct * blockend;       // Zeiger auf Blockende im Programmdatenbereich (NULL = ungltig)
        unsigned int abschnr;                  // Abschnittsnummer
        ikoordtyp koorref;                     // Koordinaten - Referenzpunkt
        koord koorptr;                         // Zum aktuellen Abschnitt geh”rende Koordinaten
        struct pzblstruct pzbl;                // Programm-Zustandsblock
        struct abschstruct abschnitt [3];      // Arbeitsspeicher fr max. 3 Abschnitte
        /* EasyCODE > */
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseProg(void)
        {
        }

        /* EasyCODE ) */
        unsigned char grundzustand(void);
        unsigned char einf (void);
        unsigned char del (void);
        unsigned char block (void);
        unsigned char bild (void);
        unsigned char aender (void);
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseProg prog;
/* EasyCODE ) */
