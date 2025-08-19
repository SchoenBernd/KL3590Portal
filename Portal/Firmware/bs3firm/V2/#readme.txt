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




=========================================================================================

Leerfahrten im porgrammieren verbessert:
Testen:



crc16 Algorithm according to ISO-14443-A or ITU-T V.41
 wCrc = 0x6363; /* ITU-V.41 */




ISO/IEC 14443-3 Type B ISO/IEC 3309 and CCITT X.25 2.2.7 and V.42 8.1.1.6.1 for further details. 
Initial Value = 'FFFF'
  wCrc = 0xFFFF; /* ISO/IEC 13239 (formerly ISO/IEC 3309) */






smx.nmotstart = 5;    0,05 1/s   // Motordrehzahl für konst fahren   1/100 1/s
smx.delay_ms = 1000;  1s     // Zeit konstant fahren
smx.a_beschl = 15.7  ;    // 2s       (2 Pi n / t)
smx.a_brems  = 157 ;      // 0,2s     (2 Pi n / t)
smx.nmot     = 500;  5  1/s    // 1/100 1/s    nicht frequenz !!!




 saf->xnaehfeld_min
 saf->xnaehfeld_max 
 saf->ynaehfeld_min 
 saf->ynaehfeld_max

rechts
saf->xnaehfeld_max - nup.workbuf.koorist.xbild

links
nup.workbuf.koorist.xbild - saf->xnaehfeld_min 


up 
saf->ynaehfeld_max - nup.workbuf.koorist.ybild

down
nup.workbuf.koorist.ybild - saf->ynaehfeld_min 





 
int xbild;
int ybild;
int motorx;
int motory;


Eingabe.Programmieren.0003.Enter.1.Im Vorbereitetem Prog. dann auf Abschnitt 6 gehen.INS.Gerade



FW:  030412

unsigned char bef;               // Handshakebyte     12 = endlos fahren
unsigned char mod;               // Status             ja
unsigned int adr;                // Rampenauswahl    
unsigned int vort;               // Vorteiler          ja im init()
unsigned int verz;               // Verzögerung
unsigned int gesp;               // Gesamtpulszahl    jetzt Pulsabstand
unsigned int dachp;              // Pulse bis Dachende
unsigned int apul;               // ausgegebene Pulse   echte aufaddierte gefahrene Pulse
unsigned char stat;              // Rückmeldung
unsigned char eing;              // Eingänge
unsigned char stop;              // Stopbyte
******smstruct

#define SM1  ((volatile smstruct *) 0x0200)   // portal Huepferfussantrieb
#define SM2  ((volatile smstruct *) 0x0220)   // portal X-Achse
#define SM3  ((volatile smstruct *) 0x0240)   // portal Y-Achse
#define SMH  ((volatile smhstruct*) 0x0260)   // portal Huepferfussantrieb Sondereinstellungen, verwendet SM1.


PC104

2,          2,        SMINF_PRIOR,      0,         sminf_handler
#define SMInfIndex   11
#define SMINF_PRIOR  3




KlasseSminftask sminftask (tdini [SMInfIndex]);




void sminftask (void);


#define SMZ ((volatile smstruct *) MK_FP(dprseg,0x200))
#define SMX ((volatile smstruct *) MK_FP(dprseg,0x220))
#define SMY ((volatile smstruct *) MK_FP(dprseg,0x240))




smx.status = PAWRAMPCV | SMXMINUS;
smx.vort = saf->xvorteiler1;


KlasseSminfBS3::init(void)
 dprptr->vort = vort;
 dprptr->bef = 0;
 PC104_WR = 1;   // Interrupt bei C167 ausl”sen
 outportb (PLLOUT, pll_faktor-1);


KlasseSminfBS3::start_Pulse (void)
 dprptr->mod = status;
 dprptr->adr = rampadr;
 dprptr->gesp =gespls;
 dprptr->bef = 3;   // Rampe mit Gesamtpulszahl ohne Startverz”gerung starten
 PC104_WR = 1;  // Interrupt bei C167 ausl”sen



