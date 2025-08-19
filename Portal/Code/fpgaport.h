/* EasyCODE V8 */
/* EasyCODE ( 0 
fpga3590.h */
// Datei: fpga3590.h, Headerdatei fÅr PC104 auf der BS3
// Ersteller: Wilfried Halfmann, EES 2856

// Definitionen fÅr FPGA

#define PLLOUT 0x340         // Faktor fÅr PLL-Vervielfachung wird auf diese
                             // Adresse ausgegeben

#define STATIN 0x340         // Status lesen
#define PEGELIN 0x341        // Pegel lesen
                             // Bit: 7 6 5 4 3 2 1 0
#define RESET_MSK 0x01       //      | | | | | | | |__ Reset C167 (aktiv low)
#define BATTOK_MSK 0x02      //      | | | | | | |____ Batterie ok
#define P1_MSK 0x04          //      | | | | | |______ Nocken P1
#define P2_MSK 0x08          //      | | | | |________ Nocken P2
#define DPR_MSK 0x10         //      | | | |__________ Interrupt DPR (aktiv low)
#define PF_MSK 0x20          //      | | |____________ Power Fail
#define REF2_MSK 0x40        //      | |______________ Ref 2
#define REF1_MSK 0x80        //      |________________ Ref 1


#define RES167OUT 0x341      // Adresse
                             // durch Ausgeben einer bestimmten Datenfolge
                             // wird der Resetausgang fÅr den C167 invertiert
#define RESDATA1 0x33
#define RESDATA2 0x55
#define RESDATA3 0xaa


#define INTMASK 0x342        // Adresse
                             // Durch Ausgeben der entsprechenden Masken werden
                             // Interrupts gesperrt oder freigegeben. Masken der
                             // gleichen Art (DI oder EI) kˆnnen geodert werden.
#define REF1DI 0x80
#define REF1EI 0x81
#define REF2DI 0x40
#define REF2EI 0x41
#define PFDI 0x20
#define PFEI 0x21
#define DPRDI 0x10
#define DPREI 0x11
#define P2DI 0x08
#define P2EI 0x09
#define P1DI 0x04
#define P1EI 0x05

#define INTCLR 0x343         // Adresse
                             // Durch Ausgeben der entsprechenden Masken werden
                             // Interruptanforderungen gelîscht. Die Clearmasken
                             // kˆnnen geodert werden.
#define REF1_SETCLR 0x81
#define REF1_RESCLR 0x80
#define REF2_SETCLR 0x41
#define REF2_RESCLR 0x40
#define PF_SETCLR 0x21
#define PF_RESCLR 0x20
#define DPR_SETCLR 0x11
#define DPR_RESCLR 0x10
#define P2_SETCLR 0x09
#define P2_RESCLR 0x08
#define P1_SETCLR 0x05
#define P1_RESCLR 0x04

#define LED1ADR 0x342        // Leuchtdioden
#define LED1ON 0x02
#define LED1OFF 0x03
#define LED2ADR 0x343
#define LED2ON 0x02
#define LED2OFF 0x03

#define SAFADR 0x344         // hier wird die Adresse fÅr das Fenster der
                             // netzausfallsicheren Daten programmiert
/* EasyCODE ) */
