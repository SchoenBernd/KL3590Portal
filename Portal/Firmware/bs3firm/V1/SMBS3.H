// Datei: smbs3.h
//        Headerdatei für das Schrittmotor-Interface (C167)
// Ersteller: Edi Bastian

// Definitionen fuer die Ausgaenge
#define FKT11 P7_5      // SM1: FKT1
#define FKT21 P7_6      //      FKT2
#define FKT12 P8_3      // SM2: FKT1
#define FKT22 P8_4      //      FKT2
#define FKT13 P2_7      // SM3: FKT1
#define FKT23 P2_13     //      FKT2
#define FKTON 0         // Funktion ein
#define FKTOFF 1        // Funktion aus

// Definitionen fuer die Richtung
#define DIR1 P8_2       // SM1
#define DIR2 P8_5       // SM2
#define DIR3 P2_14      // SM3

// Definitionen fuer die Eingaenge
#define EING11 P5_0     // SM1: EING1
#define EING21 P5_1     //      EING2
#define EING12 P5_2     // SM2: EING1
#define EING22 P5_3     //      EING2
#define EING13 P5_4     // SM3: EING1
#define EING23 P5_5     //      EING2

// Definitionen fuer die Pulsauswahl
#define PAWSV1 P2_2     // SM1: Pulsauswahl Startverzoegerung
#define PAWRP1 P2_5     //      Pulsauswahl Rampe
#define PAWSV2 P2_3     // SM2: Pulsauswahl Startverzoegerung
#define PAWRP2 P2_6     //      Pulsauswahl Rampe
#define PAWSV3 P2_4     // SM3: Pulsauswahl Startverzoegerung
#define PAWPLL 0        // Pulsauswahl PLL
#define PAWCLK 1        // Pulsauswahl Clock/Vorteiler

// Definitionen fuer die Rueckmeldungen
#define RPLAEUFT 0x01   // Maske: Rampe laeuft
#define SVLAEUFT 0x02   //        Startverzoegerung laeuft
#define RPERROR 0x04    //        Error (ramp not finished)
#define RPWARN 0x08     //        Warnung (Pulsanzahl: ausgegebene Pulse >< Gesamtpulszahl)
#define RPFERTIG 0      // Fertig ohne Fehler

// Definitionen der Startadressen der Rampenadressen
#define RAMPEN1 ((unsigned char huge * huge * huge *) 0x104000)    // SM 1
#define RAMPEN2 ((unsigned char huge * huge * huge *) 0x104004)    // SM 2
#define RAMPEN3 ((unsigned char huge * huge * huge *) 0x104008)    // SM 3

// Definitionen der Rampenbetriebsarten
#define ORIGINAL 0      // original (ohne Zaehler)
#define BIS_DACH 1      // bis Dachbit
#define DACHVERL 2      // Dachverlaengerung (bis dachp erreicht)
#define BIS_STOP 3      // bis Stopbit
#define ENDE     4      // Ende (bis gesp erreicht)

