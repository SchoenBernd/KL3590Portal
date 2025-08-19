In UV option for Target C167 eintragen:

FIRM480   für Standard BS3 Firmware mit 480 Handradpulse
FIRM256   für Standard BS3 Firmware mit 256 Handradpulse


Basis für Portal firmware war firm480 V2 und gilt nun für beide
Handradpulszahlen.

Notwendige Änderungen um einen Code zu haben für beide Handradpulszahen:

- CANBS3.c   Bed. Compilierung für CAN->maxknotennr = 8 oder = 3, wurde immer auf 3 gelassen;
- SMBS3.C    xhuge Pointer für die Rampenpointer übernommen (gelassen)
- NMBS3.C    HPULSE eingeführt anstelle 480 "Hard Coded", Ein Kommentar anders
- CANBS3.H   Bed. Compilierung für EWRN und LEC CAN Error 
- DPRBS3.H   Bed. Compilierung für HPULSE  480 oder 256, 480 ersetzt durch HPULSE.




Timer8 wäre ein 2ms Interrupt Timer 
CC2_viIsrTmr8

DSTP6 = (unsigned int)(&NM->aktpos); // destination pointer





do{
    pos1 = NM->aktpos;
}while ( pos1 != NM->aktpos );


dprptr->mod = status;
    dprptr->adr = rampadr;
    dprptr->bef = 1;   // Rampe ohne Startverzögerung starten



 1      Rampe starten ohne Startverzoegerung                                                 */
/*          erforderliche Parameter: mod, adr                                                */
/*          Aktionen: Pruefung "ramp not finished", PAW Rampe setzen, Richtung setzen,       */
/*                    apul=0, stat setzen, Rampe starten       

  
  
  sm1.status &= ~RICHTUNG;
  
  if (nup.koorsoll [nup.vorind].motor1 - nup.workbuf.koorist.motor1) < 0)
      
      sm1.status |= SM1MINUS;
      
      sm1.status |= SM1PLUS;          
      
      
      
sm1.status=(sm1.status & RICHTUNG) | PAWRAMPPLL;
SM1->mod = 

sm1.status = SM1MINUS;
// sm1.status = (sm1.status & RICHTUNG) | PAWRAMPPLL | PAWSVZPLL; // nicht nötig, da sowieso 0x00


#define RICHTUNG 0x80
#define PAWRAMPPLL 0x00                // PAW-RAMPE VON PLL
#define PAWRAMPCV  0x02                // PAW-RAMPE CLK/VORTEILER
#define PAWSVZPLL  0x00                // PAW-VERZ. VON PLL
#define PAWSVZCV   0x01                // PAW-VERZ. CLK/VORTEILER
#define EFR_CLK_32 0x04                // Erzeugungsfrequenz Rampe = Clock/32 (Z-Achse)
#define EFR_CLK_64 0x06                // Erzeugungsfrequenz Rampe = Clock/64 (Z-Achse)
#define EFR_CLK_128 0x08               // Erzeugungsfrequenz Rampe = Clock/128 (Z-Achse)
#define EFR_CLK_256 0x0a               // Erzeugungsfrequenz Rampe = Clock/256 (Z-Achse)
#define EFR_CLK_512 0x0c               // Erzeugungsfrequenz Rampe = Clock/512 (Z-Achse)
#define EFR_CLK_1024 0x0e              // Erzeugungsfrequenz Rampe = Clock/1024 (Z-Achse)

// RICHTUNGEN
#define SM1PLUS  0x80                   // POS. RICHTUNG SM1
#define SM1MINUS 0x00                   // NEG.    "      "
#define SM2PLUS  0x80                   // POS. RICHTUNG SM2
#define SM2MINUS 0x00                   // NEG.    "      "

