/* EasyCODE V8 */
/* EasyCODE ( 0 
sminf.h */
/* EasyCODE ( 0 
Definitionen */
// Motoren
//#define SMOT1 1
//#define SMOT2 2

#define SMOTX 1
#define SMOTY 2
#define SMOTZ 3
#define PlsUmdr 1000


// Masken fÅr Status
#define RICHTUNG 0x80
#define PAWRAMPPLL 0x00                // PAW-RAMPE VON PLL
#define PAWRAMPCV  0x02                // PAW-RAMPE CLK/VORTEILER
#define PAWSVZPLL  0x00                // PAW-VERZ. VON PLL
#define PAWSVZCV   0x01                // PAW-VERZ. CLK/VORTEILER

#define VORTMSK 0x0e                   // Maske der Vorteilerbits der 3. Achse
#define EFR_CLK_32 0x04                // Erzeugungsfrequenz Rampe = Clock/32 (3. Achse)
#define EFR_CLK_64 0x06                // Erzeugungsfrequenz Rampe = Clock/64 (3. Achse)
#define EFR_CLK_128 0x08               // Erzeugungsfrequenz Rampe = Clock/128 (3. Achse)
#define EFR_CLK_256 0x0a               // Erzeugungsfrequenz Rampe = Clock/256 (3. Achse)
#define EFR_CLK_512 0x0c               // Erzeugungsfrequenz Rampe = Clock/512 (3. Achse)
#define EFR_CLK_1024 0x0e              // Erzeugungsfrequenz Rampe = Clock/1024 (3. Achse)

// RICHTUNGEN
#define SMZPLUS  0x00                   // POS. RICHTUNG SMZ
#define SMZMINUS 0x80                   // NEG.    "      "
#define SMXPLUS  0x00                   // POS. RICHTUNG SMX
#define SMXMINUS 0x80                   // NEG.    "      "
#define SMYPLUS  0x80                   // POS. RICHTUNG SMY
#define SMYMINUS 0x00                   // NEG.    "      "

// EingÑnge
#define  EING1MSK    0x01        // Maske fuer Eingang 1 (Bereitsignal der Endstufe)

// Fehlerbits
#define  FEHLERMSK   0x0f        // Maske fuer Fehlerbits
/* EasyCODE - */
//#define MAXINK1 200             // vorlÑufige Definition
//#define MAXINK2 63              //        ''
#define MAXINK 120
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse sminfbs3 */
/* EasyCODE C */
/* EasyCODE < */
class KlasseSminfBS3
/* EasyCODE > */
{
        private:
        smstruct *dprptr;   // DPR-Adresse
        unsigned char key;
public:
        /* EasyCODE ( 0 
        Parameterblock */
        // Parameterblock
        unsigned char status;
        unsigned int rampadr;
        unsigned int vort;
        unsigned int verz;
        unsigned int gespls;
        unsigned int dachpls;
        unsigned int ausgebpls;
        unsigned char pll_faktor;
        
        // Dabei hat das Status-Byte folgende Bedeutung:
        //
        //      | Richtung |     |     |     |     |     | PAW Rampe | PAW Verz. |
        //
        //       PAW Rampe:   0 .... vervielfachte Pulse von PLL
        //                    1 .... CLK (20 MHz) / Vorteiler
        //       PAW Verz.    0 .... vervielfachte Pulse von PLL
        
        
        
        
        /******* Daten fuer das Fahren mit berechnetem dynamischem Pulsabstand ******/
        
        unsigned char dynbef;   // Befehlsbyte
        float a_beschl;         // alpha Rotationsbeschleunigung  1/ss
        float a_brems;          // alpha Rotationsverzoegerung    1/ss
        
        
        unsigned int delay_ms;
        unsigned int nmotstart;
        unsigned long beschlfak;
        unsigned long bremsfak;
        unsigned int beschlcnt;
        unsigned int beschlanz;
        unsigned int bremsanz;
        //unsigned int fmot;
        unsigned int nmot;
        unsigned long istPlsAbst;
        
        unsigned int plse;    // noch moegliche Pulse bis zum Anschlag
        unsigned int plse1;   // Pulse benoetigt fuer konst. langsam fahren
        unsigned int plse2;   // Pulse benoetigt fuer beschl.
        unsigned int plse3;   // Pulse benoetigt fuer bremsen
        float erzeuger;
        unsigned char rmpabbruch;
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseSminfBS3 (unsigned int dproffs)
        {
            dprptr = (smstruct *)MK_FP(axtoi(getenv("SAFSEG"))+0x700, dproffs);
        }

        /* EasyCODE ) */
        void init (void);
        void start_Freq (void);
        void start_Pulse (void);
        void start_Rampe (void);
        void start_Rampe_Verz (void);
        void start_Rampe_Dach (void);
        void start_Rampe_Dach_Verz (void);
        void stop (void);
        unsigned char ready (void);
        unsigned int Pulse (void);
        void Fkt1_on (void);
        void Fkt1_off (void);
        void Fkt2_on (void);
        void Fkt2_off (void);
        unsigned char Status (void);
        unsigned char Eing (void);
        void notstop (void);
        unsigned char start_Rampe_Errorcheck (void);
        unsigned char start_Rampe_Verz_Errorcheck (void);
        void start_dyn_Pulse (void);
        void dynamic (void);
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseSminfBS3 smz,smx,smy;
/* EasyCODE ) */
