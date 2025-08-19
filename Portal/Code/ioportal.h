/* EasyCODE V8 */
/* EasyCODE ( 0 
ioportal.h,   Header fuer die Ein-/Ausgaenge,
              Ersteller: Wilfried Halfmann   EWE 2856 */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
// Allgemeine Definitionen
#define ANZ_KNOTEN 2
#define ANZAHL_AUSG 16*ANZ_KNOTEN   // Anzahl der Ausgangsobjekte
#define ANZAHL_EING 16*ANZ_KNOTEN
#define ANZAHL_SONDERAUFTRAEGE 9
#define SET 0x0f      // Setzauftrag
#define RES 0xf0      // Ruecksetzauftrag
#define READY 0       // Fertigmeldung
/* EasyCODE ( 0 
Ausgaenge */
// Knoten 1
#define y1    io.Ausg[0]        // Blasluft Nadelkuehlung ein
#define y2u1  io.Ausg[1]        // Fadenwedler vor 1
#define y2u2  io.Ausg[2]        // Fadenwedler vor 2
#define y4    io.Ausg[3]        // Oberfadenklemme zu
#define y5    io.Ausg[4]        // Fadenspannung auf
#define y6    io.Ausg[5]        // Zusatzfadenspannung auf
#define y7    io.Ausg[6]        // Nadeldreheinrichtung ein
#define y8    io.Ausg[7]        // Niederhalter ab

#define y9    io.Ausg[8]        // Oberfaden rueckziehen
#define y30   io.Ausg[9]        // Fadenschneiden ein
#define y31   io.Ausg[10]       // Unterfadenklemme auf
#define y32   io.Ausg[11]       // Unterfadenschieber ein
#define y33   io.Ausg[12]       // Stoffgegendruecker auf
#define y34   io.Ausg[13]       // Greiferoelung 1 ein
#define y35   io.Ausg[14]       // Greiferoelung 2 ein
#define y10   io.Ausg[15]       // Bandanschlag ab

// Knoten 2
#define y50u1 io.Ausg[16]       // Oberteil auf
#define y50u2 io.Ausg[17]       // Oberteil ab
#define y51u1 io.Ausg[18]       // Schablonenverriegelung auf
#define y51u2 io.Ausg[19]       // Schablonenverriegelung zu
#define y52   io.Ausg[20]       // Schablonenanschlag links auf
#define y53   io.Ausg[21]       // Schablonenanschlag rechts auf
#define y54   io.Ausg[22]       // Nadeldreheinrichtung ein
#define bobres1 io.Ausg[23]     // Reset Unterfadenwaechter 1

#define bobres2 io.Ausg[24]     // Reset Unterfadenwaechter 2

#define y61   io.Ausg[25]       
#define y62   io.Ausg[26]       
#define y63   io.Ausg[27]       

#define out1  io.Ausg[28]       // progr. Ausgang 1
#define out2  io.Ausg[29]       // progr. Ausgang 2
#define out3  io.Ausg[30]       // progr. Ausgang 3
#define out4  io.Ausg[31]       // progr. Ausgang 4
/* EasyCODE ) */
/* EasyCODE ( 0 
Sonderauftraege */
#define y4fkt  io.Sonder[0]            // Oberfadenklemme verzîgert auf
#define y2fkt_aus  io.Sonder[1]        // Fadenwedler in Grundstellung
#define y2fkt_ein  io.Sonder[2]        // Fadenwedler ein
#define y2fkt_drucklos  io.Sonder[3]   // Fadenwedler drucklos
#define y2fkt io.Sonder[4]             // Fadenwedler-Funktion
#define hfuss_auf io.Sonder[5]         // Huepferfuss auf
#define y9fkt io.Sonder[6]             // Oberfaden-Rueckziehfunktion
#define y7_y32fkt io.Sonder[7]         // Nadeldreheinrichtung und Unterfadenschieber ein
/* EasyCODE ) */
/* EasyCODE ( 0 
Eingaenge */
// Knoten 1
#define e29   io.Eing[0]       // Nullstellung SMZ (Huepferfuss)
#define therr1 io.Eing[1]      // Oberfadenstoerung 1
#define therr2 io.Eing[2]      // Oberfadenstoerung 2
#define e2   io.Eing[3]        // Fadenwedler in Grundstellung
#define temp io.Eing[4]        // TemperaturÅberwachung
#define e33   io.Eing[5]       // Stoffgegendruecker unten
#define boberr1 io.Eing[6]     // Oberfadenstoerung 1
#define boberr2 io.Eing[7]     // Oberfadenstoerung 2

#define ac_ok io.Eing[8]       // UnterspannungsÅberwachung (Option)
#define press io.Eing[9]       // Druckluft ok
#define fkey  io.Eing[10]      // Schluessel (fuer gesicherte Funktionen)
#define foot1 io.Eing[11]      // Fussschalter 1. Stufe
#define foot2 io.Eing[12]      // Fussschalter 2. Stufe
#define tlinks_stop io.Eing[13] // Schablonenanschlag links auf bzw. Stop
#define trechts_stop io.Eing[14] // Schablonenanschlag rechts auf bzw. Stop
#define e81 io.Eing[15]        // Tisch nicht abgeklappt

// Knoten 2
#define e50u1 io.Eing[16]      // Oberteil oben
#define e50u2 io.Eing[17]      // Oberteil unten
#define e51u2_1 io.Eing[18]    // Schablonenverriegelung geschlossen rechts
#define e51u2_2 io.Eing[19]    // Schablonenverriegelung geschlossen links
#define e80   io.Eing[20]      // Schablonen eingelegt rechts
#define e79   io.Eing[21]      // Schablonen eingelegt links
#define e78   io.Eing[22]      // Nullstellung SMX
#define e77   io.Eing[23]      // Nullstellung SMY

#define e76   io.Eing[24]      // Schablonendeckel geschlossen
#define e75   io.Eing[25]      // Spulenabdeckung geschlossen
#define e54   io.Eing[26]      // Oberteilarretierung entriegelt
#define kone1 io.Eing[27]      // Oberteilarretierung entriegelt     SW003DIS
#define in1   io.Eing[28]      // programmierbarer Eingang 1      
#define kone2 io.Eing[28]      // Garnkone 2, doppelt mit in1 !!!!!  SW003DIS
#define in2   io.Eing[29]      // programmierbarer Eingang 2
#define in3   io.Eing[30]      // programmierbarer Eingang 3
#define in4   io.Eing[31]      // programmierbarer Eingang 4
/* EasyCODE ) */
// Abbilder
extvol unsigned int out_image [];
/* EasyCODE - */
extvol unsigned int in_image [];
/* EasyCODE C */
/* EasyCODE < */
// Struktur der Ausgangsdaten
typedef struct
/* EasyCODE > */
{
        unsigned char auftrag;
        unsigned int maske;
        unsigned char knotennr;
        void (* sa)(void);
        void (* ra)(void);
}outdata;
/* EasyCODE E */
/* EasyCODE C */
/* EasyCODE < */
// Struktur der Sonderauftraege
typedef struct
/* EasyCODE > */
{
        unsigned char auftrag;
        void (* sa)(void);
        void (* ra)(void);
}sonderauftrag;
/* EasyCODE E */
/* EasyCODE C */
/* EasyCODE < */
// Struktur der Eingangsdaten
typedef struct
/* EasyCODE > */
{
        unsigned char knotennr;
        unsigned int maske;
}indata;
/* EasyCODE E */
/* EasyCODE ( 0 
Fehlernummern */
//  Hauptfehler-Nummern

#define Y30_HF1          1 
#define Y50U1_HF         2
#define Y50U2_HF         3
#define Y51U1_HF         4
#define Y51U2_HF         5
#define Y2FKT_HF1        6
#define Y7_HF1           7
#define Y32_HF1          8
#define Y54_HF0          9
#define Y54_HF1          10
#define Y33_HF0          11
#define Y33_HF1          12
#define Y2FKT_AUS_HF     13
#define Y2FKT_EIN_HF     14
/* EasyCODE - */
// Unterfehler

/* Eingaenge */
#define E50U1_UF0        1
#define E50U1_UF1        2
#define E50U2_UF0        3
#define E50U2_UF1        4
#define E51U2_1_UF0      5
#define E51U2_1_UF1      6
#define E51U2_2_UF0      7
#define E51U2_2_UF1      8
#define E33_UF0          9
#define E33_UF1          10
#define E75_UF0          11
#define E75_UF1          12
#define E76_UF1          13
#define E50U1_E50U2_UF1  14
#define E79_UF0          15
#define E79_UF1          16
#define E80_UF0          17
#define E80_UF1          18
#define E54_UF0          19
#define E54_UF1          20
#define E2_UF0           21
#define E2_UF1           22
#define E81_UF1          23

/* Ausgaenge */
#define Y8_UF0           25
#define Y8_UF1           26

/* sonstige */
#define CARPOSOK_UF1     40
#define NADELANZAHL      41
#define CUTREADY_UF1     42
#define NEEDLEPOS_UF1    43
#define NMSTEHT_UF1      44
#define HUEPFER_OBEN     45
#define HUEPFER_UNTEN    46
/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse ausgang */
/* EasyCODE C */
/* EasyCODE < */
class ausgang
/* EasyCODE > */
{
        private:
        outdata out;
public:
        /* EasyCODE ( 0 
        const outdata* GetStruct()const */
        /* EasyCODE F */
        const outdata* GetStruct()const
        {
            return &out;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        const unsigned char& GetCanNr()const */
        /* EasyCODE F */
        const unsigned char& GetCanNr()const
        {
            return out.knotennr;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        const unsigned int& GetMaske()const */
        /* EasyCODE F */
        const unsigned int& GetMaske()const
        {
            return out.maske;
        }

        /* EasyCODE ) */
        char IsFrei();
        char IsSonder();
        /* EasyCODE ( 0 
        Init */
        /* EasyCODE F */
        void Init (outdata outini)
        {
            out = outini;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        setAuftrag,   Setz-Auftrag setzen */
        /* EasyCODE F */
        void setAuftrag (void)
        {
            out.auftrag = SET;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        resAuftrag,   Ruecksetz-Auftrag setzen */
        /* EasyCODE F */
        void resAuftrag (void)
        {
            out.auftrag = RES;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        getAuftrag,   Auftrag abfragen */
        /* EasyCODE F */
        unsigned char getAuftrag (void)
        {
            return out.auftrag;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        clearAuftrag,   Auftrag loeschen */
        /* EasyCODE F */
        void clearAuftrag (void)
        {
            out.auftrag = READY;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        fertig,   Fertigmeldung abfragen */
        /* EasyCODE F */
        unsigned char fertig (void)
        {
            return (out.auftrag == READY);
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        setAusgang,   Ausgang setzen */
        /* EasyCODE F */
        void setAusgang (void)
        {
            out_image [out.knotennr-1] |= out.maske;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        resAusgang,   Ausgang ruecksetzen */
        /* EasyCODE F */
        void resAusgang (void)
        {
            out_image [out.knotennr-1] &= ~out.maske;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        getZustand,   Zustand des Ausgangs abfragen */
        /* EasyCODE F */
        unsigned char getZustand (void)
        {
            return ((out_image [out.knotennr-1] & out.maske) == out.maske);
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        setup,   Unterprogramm Setzauftrag */
        /* EasyCODE F */
        void setup (void)
        {
            out.sa ();
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        resetup,   Unterprogramm Ruecksetzauftrag */
        /* EasyCODE F */
        void resetup (void)
        {
            out.ra ();
        }

        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse sonderauftr */
/* EasyCODE C */
/* EasyCODE < */
class sonderauftr
/* EasyCODE > */
{
        private:
        sonderauftrag sauf;
public:
        /* EasyCODE ( 0 
        outdata* GetStruct() */
        /* EasyCODE F */
        const sonderauftrag* GetStruct()const
        {
            return &sauf;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Init */
        /* EasyCODE F */
        void Init (sonderauftrag sonderini)
        {
            sauf = sonderini;
        }

        /* EasyCODE ) */
        char IsFrei();
        /* EasyCODE ( 0 
        setAuftrag,   Setz-Auftrag setzen */
        /* EasyCODE F */
        void setAuftrag (void)
        {
            sauf.auftrag = SET;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        resAuftrag,   R¸cksetz-Auftrag setzen */
        /* EasyCODE F */
        void resAuftrag (void)
        {
            sauf.auftrag = RES;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        getAuftrag,   Auftrag abfragen */
        /* EasyCODE F */
        unsigned char getAuftrag (void)
        {
            return sauf.auftrag;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        clearAuftrag,   Auftrag lˆschen */
        /* EasyCODE F */
        void clearAuftrag (void)
        {
            sauf.auftrag = READY;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        fertig,   Fertigmeldung abfragen */
        /* EasyCODE F */
        unsigned char fertig (void)
        {
            return (sauf.auftrag == READY);
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        setup,   Unterprogramm Setzauftrag */
        /* EasyCODE F */
        void setup (void)
        {
            sauf.sa ();
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        resetup,   Unterprogramm R¸cksetzauftrag */
        /* EasyCODE F */
        void resetup (void)
        {
            sauf.ra ();
        }

        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse eingang */
/* EasyCODE C */
/* EasyCODE < */
class eingang
/* EasyCODE > */
{
        private:
        indata in;
public:
        unsigned char knotennr;
        unsigned int maske;
        /* EasyCODE ( 0 
        const indata* GetStruct()const */
        /* EasyCODE F */
        const indata* GetStruct()const
        {
            return &in;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        const unsigned char& GetCanNr()const */
        /* EasyCODE F */
        const unsigned char& GetCanNr()const
        {
            return in.knotennr;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        const unsigned int& GetMaske()const */
        /* EasyCODE F */
        const unsigned int& GetMaske()const
        {
            return in.maske;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Init */
        /* EasyCODE F */
        void Init (indata inini)
        {
            in = inini;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        getZustand,   Zustand des Eingangs abfragen */
        /* EasyCODE F */
        unsigned char getZustand (void)
        {
            return ((in_image [in.knotennr-1] & in.maske) == in.maske);
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        getZustandPort,   Zustand des Eingangsports abfragen */
        /* EasyCODE F */
        unsigned int getZustandPort (void)
        {
            return in_image [in.knotennr-1];
        }

        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse KlasseIO */
/* EasyCODE C */
/* EasyCODE < */
class KlasseIO:public KlasseTask
/* EasyCODE > */
{
        private:
        unsigned char key;
        unsigned char mainerr_merk;
        unsigned char stopb;
        void abbild (void);
public:
        /* EasyCODE < */
        ausgang  Ausg[ANZAHL_AUSG];   // Ausgangsobjekte
        eingang Eing[ANZAHL_EING];    // Eingangsobjekte
        sonderauftr Sonder[ANZAHL_SONDERAUFTRAEGE]; // Objekte der Sonderauftr‰ge
        /* EasyCODE > */
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseIO(taskdata tdini):KlasseTask (tdini)
        {
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        int GetInAnz() */
        /* EasyCODE F */
        int GetInAnz()
        {
            return ANZAHL_EING;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        int GetOutAnz() */
        /* EasyCODE F */
        int GetOutAnz()
        {
            return ANZAHL_AUSG;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        int GetSonderAnz() */
        /* EasyCODE F */
        int GetSonderAnz()
        {
            return ANZAHL_SONDERAUFTRAEGE;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        ausgang* GetAusgang(int n) */
        /* EasyCODE F */
        ausgang* GetAusgang(int n)
        {
            /* EasyCODE < */
            // returns a pointer to ausgang array member n
                    // returns NULL if n is greather than the size of array
                    // 0 =< n < GetOutDim
            /* EasyCODE > */
            if ( n >= ANZAHL_AUSG )
            {
                return NULL;
            }
            else
            {
                return &Ausg[n];
            }
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        eingang* GetEingang(int n) */
        /* EasyCODE F */
        eingang* GetEingang(int n)
        {
            /* EasyCODE < */
            // returns a pointer to ausgang array member n
                    // returns NULL if n is greather than the size of array
                    // 0 =< n < GetInDim()
            /* EasyCODE > */
            if ( n >= ANZAHL_EING )
            {
                return NULL;
            }
            else
            {
                return &Eing[n];
            }
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        ausgang* GetSonder(int n) */
        /* EasyCODE F */
        sonderauftr* GetSonder(int n)
        {
            /* EasyCODE < */
            // returns a pointer to sonder outputs array, member n
                    // returns NULL if n is greather than the size of array
                    // 0 =< n < GetSonderDim()
            /* EasyCODE > */
            if ( n >= ANZAHL_SONDERAUFTRAEGE )
            {
                return NULL;
            }
            else
            {
                return &Sonder[n];
            }
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        unsigned char stop(void) */
        /* EasyCODE F */
        unsigned char stop(void)
        {
            return stopb;
        }

        /* EasyCODE ) */
        unsigned char mainerr;
        unsigned char secerr;
        unsigned char index;
        unsigned char ruecksetzmerker;
        unsigned char clamp_merker ;
        void iotask (void);
        void off (void);
        void kinit (void);
        void winit (void);
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseIO io;
/* EasyCODE ) */
