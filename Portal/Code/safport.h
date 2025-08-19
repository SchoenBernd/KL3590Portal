/* EasyCODE V8 */
/* EasyCODE ( 0 
saf3590.h */
/* EasyCODE ( 0 
safdat_struct */
/* EasyCODE C */
/* EasyCODE < */
/*****************************************************************************/
/*                                                                           */
/* Datei:          saf3590.h                                                 */
/* Klasse:         Kl3590                                                    */
/* Ersteller:      Halfmann  EES 2856                                        */
/*                                                                           */
/* Hier wird die Struktur der netzausfallsicheren Daten definiert. Diese     */
/* Daten werden bei einem Warmstart nicht veraendert. Bei einem Kaltstart    */
/* werden sie von den zustaendigen Dateien urgeladen.                        */
/* Mit diesen Daten wird der Datencheck durchgefuehrt.                       */
/*                                                                           */
/*****************************************************************************/

struct safdat_struct
/* EasyCODE > */
{
        // Hauptprogramm
        unsigned char kaltstart;
        unsigned char bs3download;          // wird nur von Master reset gesetzt
        unsigned char safdatsum_gueltig;    // Checksumme der netzausfallsicheren Daten gueltig
        unsigned int safdatsum;             // Checksumme der netzausfallsicheren Daten
        unsigned char testtext[20];         // hier ist die Teilenummer der akt.Software gespeichert
        
        // Koordinator
        unsigned char faterror;             // Systemfehler
        unsigned int exormatrix[ANZ_KNOTEN];// Exklusiv-Oder-Matrix fuer Eingaenge
        unsigned int chopstartzt;           // Start-Zeit fuer Chopper bei CAN
        unsigned char chopauszt;            // Aus-Zeit fuer Chopper bei CAN (Achtung: nur Byte)
        unsigned char chopeinzt;            // Ein-Zeit fuer Chopper bei CAN (Achtung: nur Byte)
        unsigned int chopmask [ANZ_KNOTEN]; // Choppermasken Can-Knoten
        unsigned int c167_timeout;          // Timeout beim Booten des C167
        unsigned int delflash_zt;           // Timeout beim Loeschen des Flashs des C167
        unsigned int sm_ontime;             // max. Wartezeit fuer Bereitsignal SM-Endst.
        
        // Naehen
        nbuf buf;
        int xnaehfeld_min;                  // Naehfeldgrenzen
        int xnaehfeld_max;                  //       "
        int ynaehfeld_min;                  //       "
        int ynaehfeld_max;                  //       "
        ikoordtyp refpkt [7];               // Referenzpunkte fuer Schlittentestfunktion
        float fadschein_winkel1;            // Winkel ab dem Unterfadenschieber ein geschaltet wird
        float fadschein_winkel2;            // Winkel bis zu dem Unterfadenschieber ein geschaltet wird
        float fadschaus_winkel1;            // Winkel ab dem Unterfadenschieber aus geschaltet wird
        float fadschaus_winkel2;            // Winkel bis zu dem Unterfadenschieber aus geschaltet wird
        unsigned int footzt;                // Zeit f_r Fuss ab
        unsigned int nmstopzt;              // Zeit zum Stoppen des N"hmotors
        unsigned int smdelayzt;             // Verz"gerungszeit vor dem genauen Positionieren der Sm
        unsigned int tadelzt;               // Verz"gerungszeit zwischen Einzeltakt und Dauertakten
        unsigned int taktzt;                // Wiederholrate beim Takten
        unsigned int niszeit;               // sberwachungszeit von Freigabe bis Auftreten des 1. Nis
        unsigned int naezeit1;              // Zeit f_r N"hablauf
        unsigned int initzt;                // Zeit f_r Schlitten in Grundstellung zu bringen
        unsigned int startverz;             // Startverz"gerung beim Automatikstart
        unsigned int autoclampverz_zt;      // Verz"gerung Klammer zu beim Automatikstart
        unsigned int fsverz;                // Verz"gerung beim Start mit Fussschalter
        unsigned int prodelzt;              // Zeitwerte zum Steuern des langsamen
        unsigned int profastzt;             // und schnellen Schlittenverfahren beim Programmieren
        unsigned int bobcnt;                // Unterfadenz"hler
        unsigned int piececounter;          // St_ckz"hler
        // Vorteiler f_r die Schrittmotoren
        unsigned int move_vort;             // Vorteiler Rampe mit Dachverl"ngerung
        unsigned int move_vort_uspg;        // Vorteiler Rampe mit Dachverl"ngerung bei Unterspannung
        unsigned int move2_vort;            // Vorteiler Rampe2 mit Dachverl"ngerung
        unsigned int move2_vort_uspg;       // Vorteiler Rampe2 mit Dachverl"ngerung bei Unterspannung
        unsigned int move3_vort;            // Vorteiler Rampe3 mit Dachverl"ngerung
        unsigned int takt_vort;             // Vorteiler Takten
        unsigned int takt_vort_uspg;        // Vorteiler Takten bei Unterspannung
        unsigned int takt_vort2;            // Vorteiler Takten bei Pulse fahren mit gleicher Freq.(= leiser)
        unsigned int ramp_vort;             // Vorteiler N"hen
        unsigned int stst_vort;             // Vorteiler Start/Stop
        unsigned int slow_vort;             // Vorteiler langsames Fahren zum genauen Positionieren
        unsigned int vorserv;               // Vorteiler f_r Service-Schrittmotor
        
        unsigned int xvorteiler1;           // Vorteiler1 fÅr X-Motor bei progup
        unsigned int xvorteiler2;           // Vorteiler2 fÅr X-Motor bei progup
        unsigned int xvorteiler3;           // Vorteiler3 fÅr X-Motor bei progup
        unsigned int xvorteiler4;           // Vorteiler4 fÅr X-Motor bei progup
        
        unsigned char pll;                  // PLL-Faktor f_r Sm's
        unsigned int move_adr;              // Adresse der Rampe mit Dachverl"ngerung
        unsigned int move_abramp;           // Pulse in der Abrampe bei der Rampe mit Dachverl"ngerung
        unsigned int move2_adr;             // Adresse der Rampe2 mit Dachverl"ngerung
        unsigned int move2_abramp;          // Pulse in der Abrampe bei der Rampe2 mit Dachverl"ngerung
        unsigned int move3_adr;             // Adresse der Rampe3 mit Dachverl"ngerung
        unsigned int move3_abramp;          // Pulse in der Abrampe bei der Rampe3 mit Dachverl"ngerung
        unsigned char gswred_uspg;          // Geschwindigkeitsreduzierung bei Unterspannung
        unsigned char bremstab [6];         // Bremstabelle
        unsigned char inttab [15];          // Geschwindigkeits-Tabelle zum integrierenden Hochlauf
                                            // und Bremsen. Achtung !!! Brems- bzw. Beschleunigungsstufen
                                            // muessen getroffen werden wegen "ramp not finished".
        unsigned char stigswtab [32];
        unsigned char koorgswtab1 [32];
        unsigned char koorgswtab2 [32];
        
        lpkoord lp;                         // Ladepunkt
        koord servicepos;                   // Serviceposition
        
        
        // Huepferfuss
        unsigned int movevorteiler;
        unsigned int startstopvorteiler;
        unsigned int servvorteiler;
        unsigned int startauf;              // Inkr.
        unsigned int startab;               // Inkr.
        
        // abl
        
        // Naehmotor-Interface
        unsigned int timeout_ser; 
        unsigned int timeout_bef;
        unsigned int totmannzt;
        int ot_fenster;
        int ot_bereich;
        int takt_bereich;
        int hf_bereich;
        unsigned int quick_delayzt;
        
        // IO
        unsigned int io_errtime;            // Fehler_berwachungszeit
        unsigned int fadensp_time;
        
        // Eingabe
        unsigned int slowtime;              // Zeitwerte zum Steuern des langsamen
        unsigned int fasttime;              //   und schnellen Rollens.
        unsigned int slowdelay;             //          "
        unsigned int fastdelay;             //          "
        /* EasyCODE < */
        // Konfiguration/Nullpunkte
        unsigned char konfig_gueltig;
        unsigned char stoffgegendrueckerf;  // TRUE - Stoffgegendruecker elektrisch schaltbar,
                                            // FALSE - mechanisch gekoppelt                              
        unsigned char jigctrf;
        unsigned char variante;             // 0 - Standard, 
                                            // 1 - Stop bei Unterfadenst. wird als Stop zum Band einelegen interpretiert.
        
        unsigned char mnp_gueltig;
        koord mnp;
        char xkor;                          // Korrekturwerte fuer Nadelposition
        char ykor;
        unsigned char hfp_gueltig;
        char hfkor;                         // Korrekturwert fuer Huepferfussposition
        unsigned char inkrkor_gueltig;
        char inkr_korx;                     // Korrekturwerte fuer Inkrement
        char inkr_kory;
        char schabl_kor;                    // Korrekturwert mech. Anbringung der Schablone
        unsigned char nadelp_gueltig;
        int nadelp;                         // Position Nadel OT in 480 Inkr./Umdr.
        // Maschinendaten
        struct masch_daten md;
        
        // Flags
        unsigned char cutready;             // Fertigmeldung Fadenschneiden
        unsigned char carposok;             // Schlittenposition ok
        unsigned char upfahren_dis;         // Disable fahren auf den unterbrochenen Punkt im Power On
        unsigned char bcinit_flag;          //SW005-1
        /* EasyCODE > */
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ) */
