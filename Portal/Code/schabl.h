/* EasyCODE V8 */
/* EasyCODE ( 0 
schabl.h,   Header fÅr die Schablonenrkennung,
                Ersteller: Wilfried Halfmann   EES 2856 */
/* EasyCODE ( 0 
rfid / BC define */
/*
#define	ACK	0x06
#define	NAK	0x15
#define	STX	"\x02"
#define	ETX 	"\x03"
#define	ENTER_CONFIG 	"\x02\x39 \x39 \x39 \x39 \x39 \x39 \x03"
#define	EXIT_CONFIG	  "\x02\x39 \x39 \x39 \x39 \x39 \x39 \x03"
#define	DEFAULT_CONFIG	"\x02\x39 \x39 \x39 \x39 \x39 \x38\x03"

*/


// BC Define
#define BCCODE_LENGTH 4
#define STAT_SCANON 0x02 //(STX)
#define STAT_SCANOFF 0x03 //(ETX)
//  RFID Define
// =========

//Auftrag fuer   check_set_checksum
#define	CHECK_CHECKSUM  0
#define	SET_CHECKSUM  1
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse KlasseRfid */
/* EasyCODE C */
/* EasyCODE < */
class KlasseRfid
/* EasyCODE > */
{
        private:
        /* EasyCODE ( 0 
        rfid Daten */
        /* EasyCODE < */
        /*   noch loeschen
        unsigned char timer_lauft;
        unsigned char merker_zeit_lauft;
        unsigned char barcode_ok;
        unsigned char barcode_asc[50];
        unsigned char merker_falsche_zeichen;              
        unsigned char config_key;
        unsigned int help_key; 
        */
        
        
        
        union highlow recdata;           // empfangenes Zeichen incl. Fehler
        
        
        // RFID
        unsigned int rfid_sub_key; 
        unsigned char rcv_frame_key;
        unsigned char sndbuffer[60];
        unsigned char rcvbuffer[60];
        unsigned char zeichen_count;
        unsigned char frame_length;
        unsigned char checksum; 
        unsigned char checksumwrite; 
        unsigned char maklassestr[20];
        unsigned int rfidprognummer;
        unsigned int rfidprognummerwrite;
        unsigned char rfidprognummerstr[5];
        unsigned char rfid_enable_read;
        unsigned char rfid_enable_write;
        unsigned char rfid_retry;
        unsigned char forceinit;
        unsigned char readerIDchanged;
        unsigned char sangoma;
        
        
        
        
        
        // RFID Reader Befehle Mode1:
        unsigned char *RFID_RESET_STR;
        unsigned char *RFID_START_SCAN_STR;
        unsigned char *RFID_GET_VER_STR;
        unsigned char *RFID_READ_STR;
        unsigned char *RFID_ANTENNA_OFF;
        unsigned char *RFID_PIC_115200_BAUD;
        unsigned char *RFID_TAMA_115200_BAUD;
        unsigned char *RFID_EEPROM_LOGIN;
        unsigned char *RFID_READERID_CHANGE;
        unsigned char *RFID_EEPROM_LOGOUT;
        unsigned char *RFID_SANGOMA_POLL;
        unsigned char *RFID_SANGOMA_READ;
        
        
        // RFID Reader Response (Soll) Mode1:
        unsigned char *RFID_MODE1_RESPONSE;
        
        
        // RFID Reader Befehle Mode3:
        unsigned char *RFID_MxRtyPassiveAct_STR;
        /* EasyCODE > */
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        rfid Prototypen */
        //unsigned char init_config(void);
        void send_mode1(unsigned char * cmd_ptr,  unsigned char normal_frame);
        void send_mode3(unsigned char * cmd_ptr);
        unsigned char receive_mode1(void);
        unsigned char receive_mode3(void);
        void asciihex_to_char(unsigned char asciiHigh, unsigned char asciiLow, unsigned char* bytechar);
        void char_to_asciihex(unsigned char bytechar, unsigned char* asciiHigh, unsigned char* asciiLow );
        unsigned char check_set_checksum (unsigned char auftrag);
        void send_SANGOMA(unsigned char * cmd_ptr);
        unsigned char receive_SANGOMA(void);
        /* EasyCODE ) */
public:
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseRfid(void)
        {
            rfidprognummer = 0xFFFD;
            /* EasyCODE - */
            // ungueltig
            
            maklassestr[0] = 0x30; // ASCII "0"
            maklassestr[1] = 0x00; // Nullterminator
            /* EasyCODE - */
            // RFID Reader Befehle Mode1:
            RFID_RESET_STR = "ar";
            RFID_START_SCAN_STR = "s";
            RFID_GET_VER_STR = "av";
            RFID_READ_STR = "r04" ;
            RFID_ANTENNA_OFF = "of00" ;
            RFID_PIC_115200_BAUD = "ah04" ;
            RFID_TAMA_115200_BAUD = "at04" ;
            RFID_EEPROM_LOGIN = "aliffffffff" ;
            RFID_READERID_CHANGE = "aew000500" ;
            RFID_EEPROM_LOGOUT = "alo" ;
            
            
            // RFID Reader Response (Soll) Mode1:
            RFID_MODE1_RESPONSE = "FF000000";
            
            // RFID Reader Befehle Mode3 (1. Byte = laenge, dann TAMA Nutzdaten):
            RFID_MxRtyPassiveAct_STR =  "\x05\x32\x05\xFF\x00\x02";  // MxRtyPassiveActivation = 0x02 = 3 times
            
            readerIDchanged = FALSE;
            sangoma = FALSE;
            
            
            // RFID Reader Befehle SANGOMA:
            RFID_SANGOMA_POLL  = "\x00" ;  // Dummy Command hat keine Nutzdaten
            RFID_SANGOMA_READ  = "\x06RH1,16";    //
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        set_rfid_read(void) */
        /* EasyCODE F */
        void set_rfid_read(void)
        {
            // RFID noch nicht fertig
            // rfid_enable = FALSE wird automatisch nach jedem erfolgreichem Lesen von der rfid task selbst gemacht.
            // Nach rfid_enable = TRUE wird somit immer nur einmal vom Transponder gelesen.
            /* EasyCODE - */
            rfidprognummer = 0xFFFE;
            maklassestr[0] = 0x30; // ASCII "0"
            maklassestr[1] = 0x00; // Nullterminator
            /* EasyCODE - */
            rfid_enable_read = TRUE;
            return ;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        unsigned int get_rfidprognummer (unsigned char * maklassestr_loc) */
        /* EasyCODE F */
        unsigned int get_rfidprognummer (unsigned char * maklassestr_loc)
        {
            /*
            maklassestr_loc = pointer auf den Maschinen Klassenstring der Maschine, zum Beispiel:
            Pointer auf den Text "3590-01/01".
            
            
            Rueckmeldungen:
            
            0xFFFD:    Programmnummer ist ungueltig.
            0xFFFE:    RFID Task noch nicht fertig mit lesen.
            0xFFFF:    RFID Task hat einen Fehler festgestellt (rfid_err sitzt).
            sonst:      Programmnummer ist gueltig und kein Fehler
            
            */
            if ( rfidprognummer != 0xFFFE )
            {
                // RFID Task fertig
                if ( rfid_err > 0 )
                {
                    return 0xFFFF;
                }
                else
                {
                    // RFID fertig und kein Fehler aktiv
                    if ( rfidprognummer == 0xFFFD )
                    {
                        return 0xFFFD;
                    }
                    else
                    {
                        if ( strcmp(maklassestr, maklassestr_loc) == 0 )
                        {
                            // String OK
                            return rfidprognummer;
                        }
                        else
                        {
                            // Fehler
                            rfid_err = 6;
                            return 0xFFFF;
                        }
                    }
                }
            }
            else
            {
                // RFID Task noch nicht fertig
                // Solange mittels get_rfidprognummer pollen
                // bis eine g¸ltige rfidprognummer kommt oder ein
                // Fehler gesetzt wurde.
                if ( rfid_err > 0 )
                {
                    return 0xFFFF;
                }
                else
                {
                    return 0xFFFE;
                }
            }
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        void delete_rfidprognummer (void) */
        /* EasyCODE F */
        void delete_rfidprognummer (void)
        {
            /*
            */
            /* EasyCODE - */
            rfidprognummer = 0xFFFD; // RFID auf ungueltig setzen
            maklassestr[0] = 0x30; // ASCII "0"
            maklassestr[1] = 0x00; // Nullterminator
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        void set_rfidprognummer (unsigned int prognummer_loc,   unsigned char * maklassestr_loc) */
        /* EasyCODE F */
        void set_rfidprognummer (unsigned int prognummer_loc,   unsigned char * maklassestr_loc)
        {
            rfidprognummerwrite = prognummer_loc;
            strcpy(maklassestr, maklassestr_loc);
            rfid_enable_write = TRUE;    // Nur wenn RFID Read fertig ist und der Reader initialisiert wurde und in Wartestellung ist
                                                        // wird ein Write ausgef¸hrt.
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        unsigned char check_rfidprognummer_write_rdy (void) */
        /* EasyCODE F */
        unsigned char check_rfidprognummer_write_rdy (void)
        {
            /*
            Output:
            
            0xFF:  Fehler,  Errorbyte sitzt
            0xFE:  Noch nicht fertig mit schreiben
            0x00:  Erfolgreich geschrieben bzw. kein schreiben gestartet bzw. Fehler rfid_err entstˆrt.
            
            */
            if ( !rfid_enable_write )
            {
                // RFID Write fertig
                if ( rfid_err > 0 )
                {
                    return 0xFF;
                }
                else
                {
                    return 0x00;
                }
            }
            else
            {
                // RFID Write noch nicht fertig
                if ( rfid_err > 0 )
                {
                    return 0xFF;
                }
                else
                {
                    return 0xFE;
                }
            }
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        rfid Public Flags und Daten */
        unsigned int rfid_key;
        unsigned char rfid_err;
        unsigned char initcomrdy;   // Wenn True, dann hat der RFID Ablauf die COM Schnittstelle auf 115200 Baud umgestellt.
        /* EasyCODE ) */
        void rfid (void);
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse KlasseBC */
/* EasyCODE C */
/* EasyCODE < */
class KlasseBC
/* EasyCODE > */
{
        private:
        // interne Speicherdefinition
        /* EasyCODE [ 
        interne Daten */
        unsigned char merker_zeit_lauft;
        unsigned char barcode_ok;
        unsigned char barcode_asc[20];
        unsigned int  zeichen_count; 
        unsigned char merker_falsche_zeichen;              
        unsigned char scanablauf_key;
        unsigned int barcode;
        unsigned int scan_key;
        unsigned int initBC_key;
        union highlow recdata;           // empfangenes Zeichen incl. Fehler
        unsigned char bcvarstr [50];
        unsigned char anzmerker;
        unsigned char bc_enable_read;
        unsigned char bc_enable_write;
        unsigned int bcprognummer;
        unsigned int bcconfigres;
        unsigned int bcstat_sub_key;
        
        unsigned char *BC_STARTCOMM;
        unsigned char *BC_STARTCOMM_RES;
        unsigned char *BC_STARTCONF;
        unsigned char *BC_STARTCONF_RES;
        unsigned char *BC_KONFDAT_RES;
        unsigned char *BC_ENDCONF;
        unsigned char *BC_ENDCONF_RES;
        unsigned char *BC_CONFIGDATA;
        /* EasyCODE ] */
public:
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        //Konstruktor
        KlasseBC(void)
        {
            bcprognummer = 0xFFFD;
            BC_STARTCOMM= "1B5B43";
            BC_STARTCONF= "1B5B42";
            BC_STARTCOMM_RES = "1B480D0A";
            BC_STARTCONF_RES = "1B500D0A";
            BC_KONFDAT_RES = "1B4B0D0A";
            BC_ENDCONF= "1B5B41";
            BC_ENDCONF_RES = "1B580D0A";
            /* EasyCODE - */
            BC_CONFIGDATA ="1B4241201B4743211B424F221B4B4B181B4341421B4342201B4344201B4343201B4345201B434F201B4350231B434D201B4346201B4347201B43487F1B43497F1B434A7F1B434B7F1B434C181B4441471B4442201B4444201B4443201B4445201B444F201B4450231B444D201B4446201B4447201B44487F1B44497F1B444A7F1B444B7F1B444C181B4541491B4542201B4544201B4543201B4545201B454F201B4550231B454D201B4546201B4547201B45487F1B45497F1B454A7F1B454B7F1B454C181B4641481B4642201B4644201B4643201B4645201B464F201B4650291B464D201B4646201B4647201B46487F1B46497F1B464A7F1B464B7F1B464C181B4141411B4142201B4144201B4143201B4145201B414F201B4150231B414D201B4146201B4147201B41487F1B41497F1B414A7F1B414B7F1B414C181B48414C1B4842201B4844201B4843201B4845281B484F201B48504F1B484D201B4846201B4847201B48487F1B48497F1B484A7F1B484B7F1B484C181B4C43261B4C42211B4C483F1B4C49221B4C4A261B4C4B201B4C4C211B4C4D201B4C4E201B4761201B4768201B4748261B4762211B4769201B4749261B477B201B4B43021B4B447F1B4B457F1B4B467F1B4B51241B4B52201B4B53201B4B4C071B4B4D0D1B4B4E0A1B4B470D1B4B480A1B4B497F1B4B4A7F1B47477F1B4B4F0D1B4B500A1B4D30201B4B41211B4B42201B4D315D1B4D32491B4D33301B4D345D1B4D35411B4D36301B4D375D1B4D38411B4D39341B4D405D1B4D41461B4D42301B4D435D1B4D44431B4D45301B4D465D1B4D47431B4D48311B4D495D1B4D4A471B4D4B301B4D735D1B4D74501B4D75301B4D762E1B4D772E1B4D782E1B4D4F5D1B4D50451B4D51301B4D525D1B4D53451B4D54341B4D555D1B4D56451B4D57301B4D585D1B4D59451B4D5A371B4D5B5D1B4D5C451B4D5D311B4D5E5D1B4D5F451B4D60351B4D615D1B4D62451B4D63311B4D645D1B4D65451B4D66381B4D675D1B4D68451B4D69321B4D6A5D1B4D6B451B4D6C361B4D6D5D1B4D6E451B4D6F321B4D705D1B4D71451B4D72391B4741211B4764201B477A201B4744021B4745031B4746201B4765201B4742291B474C201B474B201B475B201B4751211B4759241B4E41201B4E5D211B4B587F1B4E43201B4E447F1B4E457F1B4E467F1B4E477F1B4E487F1B4E497F1B4E4A7F1B4E4B7F1B4E4C7F1B4E4D7F1B4E4E7F1B4E4F7F1B4E507F1B4E517F1B4E527F1B4E537F1B4E547F1B4E557F1B4E567F1B4E577F1B4E587F1B4E597F1B4E5A7F1B4E617F1B4E627F1B4E637F1B4E647F1B4E657F1B4E667F1B4E677F1B4E687F1B4E697F1B494120";
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        res_barcode (void) */
        /* EasyCODE F */
        void res_barcode(void)
        {
            barcode = 0xffff;
            return ;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        set_bc_read(void) */
        /* EasyCODE F */
        void set_bc_read(void)
        {
            // statischer barcodereader noch nicht fertig
            // bc_enable_read = FALSE wird automatisch nach jedem erfolgreichem Lesen von der bc task selbst gemacht.
            // Nach bc_enable_read = TRUE wird somit immer nur einmal gelesen.
            /* EasyCODE - */
            bcprognummer = 0xFFFE;
            bc_enable_read = TRUE;
            bc_err = 0;
            return ;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        unsigned int get_barcode (void) */
        /* EasyCODE F */
        unsigned int get_barcode(void)
        {
            return barcode;
        }

        /* EasyCODE ) */
/* EasyCODE ( 0 
        unsigned int get_bcprognummer () */
        /* EasyCODE F */
        unsigned int get_bcprognummer ()
{
            return bcprognummer;
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        void delete_bcprognummer (void) */
        /* EasyCODE F */
        void delete_bcprognummer (void)
        {
            bcprognummer = 0xFFFD; // BC auf ungueltig setzen
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Public Flags und Daten */
        unsigned char scanner_err;
/* EasyCODE ) */
        /* EasyCODE [ 
        KlasseBC Prototypen */
        void ablauf(void);
        unsigned char receivebc_mode1(void);
        unsigned char receivebc_res(unsigned char * cmd_ptr);
        void sendbc_cmd(unsigned char * cmd_ptr);
        void statScanOn(void);
        void statScanOff(void);
        void statBC(void);
        unsigned char initStatBC(void);
        void send_mode1(unsigned char * cmd_ptr);
        void send_mode2(unsigned char * cmd_ptr);
        void asciihex_to_char(unsigned char asciiHigh, unsigned char asciiLow, unsigned char* bytechar);
        /* EasyCODE ] */
        /* EasyCODE [ 
        bc Daten */
        unsigned char bc_err;
        unsigned char getBC_key;
        unsigned char bcstat_key;
        /* EasyCODE ] */
};
/* EasyCODE E */
/* EasyCODE ) */
        /* EasyCODE ( 0 
Definition der Klasse KlasseSchabl */
/* EasyCODE C */
/* EasyCODE < */
class KlasseSchabl:public KlasseTask
/* EasyCODE > */
{
        private:
        //SW005-1
        unsigned char schablkey;
        unsigned char mode;
public:
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseSchabl(taskdata tdini):KlasseTask (tdini)
        {
        }

        /* EasyCODE ) */
        unsigned char rfiden;
        
        KlasseRfid rf;
        KlasseBC bc;
        
        // Prototypen
        void schabltask (void);
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseSchabl schabl;
/* EasyCODE ) */
