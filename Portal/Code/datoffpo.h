/* EasyCODE V8 */
/* EasyCODE ( 0 
datoff90.h */
// Definition der Offsets im Datensatz
// Datei: datoff90.h
// Klasse: 3590
// Ersteller: Wilfried Halfmann  EES 2856


/* 1. PROGRAMMKOPF */
#define KPROGNR    0    /* Programmnummer */
#define KKOMMENTAR 1    /* Kommentar */
#define KPROGLNG   21   /* Programml„nge */
#define KSTICHDAT  25   /* rel. Anf.adr. des Stichdatensatzes */
#define KNAHTBILD  29   /* rel. Anf.adr. des Nahtbildprogrammes */

/* 2. Geometrischen Datensatz in Bezug zur Programmstartadresse */
#define GPROGNR    33   /* Abschnitt Programmnummer */
#define GKENNUNG   38   /* Abschnitt Maschinenkennung */
#define GKLCODE    43   /* Abschnitt Klammercode */
#define GHIND      48   /* Abschnitt Hindernisse */
#define GAP        58   /* Abschnitt Anfangspunkt */

/* 3. Stichdatensatz, relativ zum Start des Stichdatensatzes */
#define SKLCODE    0    /* Klammercode */
#define SHIND      1    /* Hindernisse */
#define SSTICHZAHL 2    /* Stichanzahl */
#define SSTICHL    4    /* Stichl„nge */
#define SAP_X      6    /* Anfangspunkt, X-Koordinate */
#define SAP_Y      8    /* Anfangspunkt, Y-Koordinate */
#define SEP_X      10   /* Endpunkt, X-Koordinate */
#define SEP_Y      12   /* Endpunkt, Y-Koordinate */
#define SNPSTART   18   /* N„hprogramm-Start */
/* EasyCODE ) */
