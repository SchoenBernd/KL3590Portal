/* EasyCODE V8 */
/* EasyCODE ( 0 
keyb8590.h */
// Datei: keyb3590.h
// Klasse: 3590 
// Ersteller: Wilfried Halfmann EES 2856

#define  ACK         0x06      // ACK - Code festlegen
/* EasyCODE ( 0 
Definition der Klasse KlasseKeyb */
/* EasyCODE C */
/* EasyCODE < */
class KlasseKeyb:public KlasseTask
/* EasyCODE > */
{
        private:
        // interne Speicherdefinition
        unsigned char bdf_err_feld[3];   // Touch Fehlernummer
        
        unsigned char keyb_zeichenkey;
        unsigned char auswertung;        // Flag, Taste bearbeiten
        unsigned char keyb_count;        // universeller Schleifenz+hler
        unsigned char tasten_count;      // Zaehler fuer erkannte Tasten
        unsigned char keyb_taste_temp;   // gemerkter erkannter Tastencode
        union highlow recdata;           // empfangenes Zeichen incl. Fehler
        unsigned char olddata;           // Merker fuer empfangene Daten
        
        unsigned char asc_zeichen[4];
        
        unsigned char keyb_taste;        // zurueckgemeldeter Tastencode
        
        unsigned char KartenmodusFlag;
        unsigned char KartenmodusMerkerFlag;
        
        void zeichen_auswerten(void);
        void SD_zeichen_auswerten(void);
        void taste_auswerten(void);
public:
        /* EasyCODE ( 0 
        KlasseKeyb */
        /* EasyCODE F */
        /*
        Konstruktor */
        KlasseKeyb(taskdata tdini):KlasseTask (tdini)
        {
            keyb_taste = 0xff;
            asc_zeichen[0] = 0x00;
            asc_zeichen[1] = 0x00;
            asc_zeichen[2] = 0x00;
            asc_zeichen[3] = 0x00;
            // Das Touch Bedienfeld schickt 3 Ziffern (Ziffer 0 bis 2), aber atoi wandelt so lange,
            // bis im String eine 0x00 kommt. Darum muss mindestens das Feld 3 mit 0x00 vorinitialisiert werden.
            
            bdf_err_feld[0] = 0x00;
            bdf_err_feld[1] = 0x00;
            bdf_err_feld[2] = 0x00;
            // Hier gilt das Gleiche wie oben.
        }

        /* EasyCODE ) */
        /*
        Public Flags und Daten */
        unsigned char keyb_err;       // 0 < keyb_err < 11 schwere BDF Fehler.
                                      // keyb_err > 10 leichte BDF Fehler.  
        unsigned char bdf_err;        // Fehlernummer welche direkt vom Touch 
                                      // gemeldet wurde.
        
        unsigned char bed_kennung[8];          // Kennung des Bedienfeldes
        unsigned char bed_version[8];          // Version des Bedienfeldes
        unsigned char akt_seitennr[3];         // Aktuelle Touch Seitennummer 
        unsigned char pikto_speicher[5];       // Freier Speicher fuer Piktogramme
        unsigned char pikto_anz[3];            // Aktuelle Anzahl der Piktogramme
        unsigned char touchfelder_anz[3];      // Aktuelle Anzahl der Touchfelder
        unsigned char ipadr [20];
        
        unsigned char ipadr_gueltig;
        unsigned char bed_kennung_gueltig;     // Inhalt von bed_kennung gueltig                        
        unsigned char bed_version_gueltig;     // Inhalt von bed_version gueltig                        
        unsigned char akt_seitennr_gueltig;    // Aktuelle Touch Seite gueltig                      
        unsigned char pikto_speicher_gueltig;  // Freier Piktogramm Speicher gueltig                
        unsigned char pikto_anz_gueltig;       // Aktuelle Anzahl der Piktogramme gueltig           
        unsigned char touchfelder_anz_gueltig; // Aktuelle Anzahl der Touchfelder gueltig
        unsigned char bed_ack;                 // bedfeld hat ACK vom Touch empfangen
        /*
        keybtask (RTK Task) */
        void keybtask(void);
        /* EasyCODE ( 0 
        taste */
        /* EasyCODE F */
        /*
        taste */
        unsigned char taste(void)
        {
            return keyb_taste;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Kartenmodus */
        /* EasyCODE F */
        /*
        Kartenmodus */
        unsigned char Kartenmodus(void)
        {
            return KartenmodusFlag;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Kartenmodus_res */
        /* EasyCODE F */
        /*
        Kartenmodus_res */
        void Kartenmodus_res(void)
        {
            KartenmodusFlag = FALSE;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        KartenmodusMerker */
        /* EasyCODE F */
        /*
        KartenmodusMerker */
        unsigned char KartenmodusMerker(void)
        {
            return KartenmodusMerkerFlag;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        KartenmodusMerker_res */
        /* EasyCODE F */
        /*
        KartenmodusMerker_res */
        void KartenmodusMerker_res(void)
        {
            KartenmodusMerkerFlag = FALSE;
        }

        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseKeyb keyb;
/* EasyCODE ) */
