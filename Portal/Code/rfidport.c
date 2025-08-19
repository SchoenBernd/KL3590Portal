/* EasyCODE V8 */
/* EasyCODE ( 0 
rfidport */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
/* Datei: rfidport        Klasse: 3590  Portal                 Ersteller: Kretschmann	

Kurzbeschreibung:
=================
In dieser Datei befindet sich die zyklische Task RFID (Radio Frequency Identification) 
Reader fuer ISO14443 A Transponder zur Klammer Erkennung.
(Zuordung Klammer zu Programmnummer).

Der RFID Reader/Writer wird immer ueber die serielle Schnittstelle com1 angeschlossen.
Default nach Power on:  RS232   9600,8,1,N  Power über 5V vom Floppy Stecker.

Reader Produkt Bezeichnung:
ARYGON APPA 2 32 05


rfid_err  Fehler Byte:
0:  kein Fehler
1:  Com Port Hardware Fehler
2:  RFID Reader Protokoll Checksumme ist falsch (_bertragungsfehler)
3:  RFID Reader Frame Receive Time Out (Reader reagiert nicht)
4:  RFID Reader unerwartete RFID Response Frame Received
5:  Falscher RFID Transponder gefunden (kein Mifare Ultralight)
6:  Maschinenklassenstring passt nicht zum Transponder Klassenstring
     Zum Beispiel: 
     Maschinenklassenstring =      "3590-01/01"
     Transponder Klassenstring  = "3590-01/02"
7: Karte nach 5 Wiederholversuche immer noch nicht lesbar/schreibbar.
     (Wahrscheinlich keine Karte im Feld)
8: Transponder Nutzdatenchecksumme falsch

rfid_err wird vom Ablauf zurückgesetzt.


10.04.2017
==========
Wegen Abkuendigung wird ein neuer Reader so implementiert, dass er sich abwaertskompatibel verhaehlt.
Es wird zuerst geprueft ob der alte ARYGON Reader angeschlossen ist, falls nicht wird versucht mit 
dem neuen SANGOMA Reader Kontakt aufzunehmen.







*/


#include "portal.h"
/* EasyCODE - */
// Die Com initialisierung wurde verschoben

/*
!defined(remote)

InitPort (COM1,9600,PARITY_NONE,1,8);  // Default für ARYGON RS232 Reader
SetProtocol (COM1,NoProtocol,2,1);	      // HW Handshake disable
AllocateCOMBuffers (COM1,50,1);		// Allocate receive and send buffers for the port
//EnableFIFO (COM1,0);				// 0 = FIFO buffer disabled.
RTKClearMailbox (SendBuffer[COM1]);
RTKClearMailbox (ReceiveBuffer[COM1]);
EnableCOMInterrupt (COM1,1);                 // wegen AllocateCOMBuffers ist der 2. Parameter ohne Bedeutung

defined(com3)

InitPort (COM3,9600,PARITY_NONE,1,8);  // Default für ARYGON RS232 Reader
SetProtocol (COM3,NoProtocol,2,1);	      // HW Handshake disable
AllocateCOMBuffers (COM3,50,1);		// Allocate receive and send buffers for the port
//EnableFIFO (COM3,0);				// 0 = FIFO buffer disabled.
RTKClearMailbox (SendBuffer[COM3]);
RTKClearMailbox (ReceiveBuffer[COM3]);
EnableCOMInterrupt (COM3,1);                 // wegen AllocateCOMBuffers ist der 2. Parameter ohne Bedeutung

*/
/* EasyCODE ) */
/* EasyCODE_STRUCTURED_COMMENT_START */
/*
EasyCODE_COMMENT_START EasyCODE ( 0 
KlasseRfid::rfid     Orginal EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
void KlasseRfid::rfid(void)
{
    unsigned char prognummerstrloc[20];
    unsigned char *temp_ptr;
    unsigned char i;
    unsigned char highchar;
    unsigned char lowchar;
    switch ( rfid_key )
    {
    case 0 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Parameter initialisieren EasyCODE_COMMENT_END
        //i = 0;
        EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
        zeichen_count = 0;
        rfid_sub_key = 0;
        rcv_frame_key = 0;
        rfid_retry = 0;
        rfid_key = 1;
        break;
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
    case 1 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        RFID Reader initialisieren EasyCODE_COMMENT_END
        switch ( rfid_sub_key )
        {
        case 0 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Com init mit Reset oder nur Standard init ausfuehren EasyCODE_COMMENT_END
            if ( EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
            !initcomrdy  ||  forceinit
            EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
             )
            {
                // Init nach power on oder nach Fehler Entstoerung
                send_mode1(RFID_RESET_STR, TRUE);
                rcv_frame_key = 0;
                rfid_sub_key = 1;
            }
            else
            {
                // Standard init (nur Antenne ausschalten)
                send_mode1(RFID_ANTENNA_OFF, TRUE);
                rcv_frame_key = 0;
                rfid_sub_key = 20;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 1 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Reset Response Packet empfangen und Baudrate zum Reader PIC erhoehen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    send_mode1(RFID_PIC_115200_BAUD, TRUE);
                    rcv_frame_key = 0;
                    rfid_sub_key = 2;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                if ( readerIDchanged )
                {
                    // Reader ID wurde schon 
                    // versucht zu aendern, ab 
                    // jetzt nur noch Error 
                    // ausgeben
                    rfid_key = 100;
                }
                else
                {
                    // Reader ID wurde seit power on noch nicht
                    // versucht einmal zu aendern 
                    rfid_err = 0; // Error ist noch kein Error beim ersten Durchlauf
                    send_mode1(RFID_EEPROM_LOGIN, FALSE); // Das Ack hierfuer geht verloren 
                                                          // wegen Reader ID noch auf 0x01
                    send_mode1(RFID_READERID_CHANGE, FALSE);
                    
                    readerIDchanged = TRUE; // nur einmal das EEPROM aendern
                    rcv_frame_key = 0;
                    rfid_sub_key = 30;
                }
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 2 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            PIC Baudrate Response ACK Packet empfangen und BS3 Baudrate erhoehen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    #if !defined(remote)
                        InitPort (COM1,115200,PARITY_NONE,1,8);
                    #elif defined(com3)
                        InitPort (COM3,115200,PARITY_NONE,1,8);
                    #endif 
                    rcv_frame_key = 0;
                    
                    tschabl1.rsz ();
                    //tschabl1.sz (6);  // 60 ms Umschaltzeit.
                    tschabl1.sz (50);  // 500 ms Umschaltzeit.
                    
                    rfid_sub_key = 3;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 3 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            BS3 Umschaltung abwarten EasyCODE_COMMENT_END
            if ( tschabl1.tz() )
            {
                send_mode1(RFID_TAMA_115200_BAUD, TRUE);
                rcv_frame_key = 0;
                rfid_sub_key = 7;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 7 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            TAMA Baudrate Response ACK Packet empfangen und TAMA RFID_MxRtyPassiveAct Einstellung EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    send_mode3(RFID_MxRtyPassiveAct_STR);
                    rcv_frame_key = 0;
                    rfid_sub_key = 8;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 8 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            RFID_MxRtyPassiveAct ACK empfangen EasyCODE_COMMENT_END
            switch ( receive_mode3() )
            {
            case 0 :
                // TAMA ACK empfangen
                rcv_frame_key = 0;
                rfid_sub_key = 9;
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 9 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            RFID_MxRtyPassiveAct Response empfangen EasyCODE_COMMENT_END
            switch ( receive_mode3() )
            {
            case 0 :
                if ( (rcvbuffer[0] == 0xD5)   &&   (rcvbuffer[1] == 0x33) )
                {
                    EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
                    initcomrdy = TRUE;
                    forceinit = FALSE;
                    
                    
                    rfid_sub_key = 0; 
                    rfid_key = 2;       // Fertig
                    EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 20 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Response ACK Packet empfangen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 21;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 21 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Antenna OFF Response Packet empfangen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     (rcvbuffer[6] == 0x30) &&  
                     (rcvbuffer[7] == 0x32) &&  
                     (rcvbuffer[8] == 0x33)   &&  
                     (rcvbuffer[9] == 0x33) )
                {
                    rfid_sub_key = 0; 
                    rfid_key = 2;       // Fertig
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 30 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Reader ID change Response ACK Packet empfangen und EEPROM Logout ausgeben EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    send_mode1(RFID_EEPROM_LOGOUT, TRUE);
                    rcv_frame_key = 0;
                    rfid_sub_key = 31;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 31 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            EEPROM Logout Response ACK Packet empfangen und zum Amnfang gehen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    // fertig, starte nochmal ein Reader IC Reset an.
                    rcv_frame_key = 0;
                    rfid_sub_key = 0;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        default:
            break;
        }
        break;
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
    case 2 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        RFID Kommunikation Read EasyCODE_COMMENT_END
        switch ( rfid_sub_key )
        {
        case 0 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            RFID Reader Start Scan EasyCODE_COMMENT_END
            if ( rfid_enable_read )
            {
                send_mode1(RFID_START_SCAN_STR, TRUE);
                rcv_frame_key = 0;
                rfid_sub_key = 1;
            }
            else
            {
                if ( rfid_enable_write )
                {
                    rfid_sub_key = 0;
                    rfid_key = 3;
                }
                else
                {
                    // Warten
                }
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 1 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Response ACK Packet empfangen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 2;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 2 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Scan Response empfangen und Lesen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                EasyCODE_COMMENT_START 
                Beispiel für ein Frame, keine Karte gefunden.
                38 00 0C 
                46 46 30 30 30 30 30 34 34 42 30 30  // Nutzdaten
                          2   3   4  5   6   7   8   9  10 11    
                6E 
                
                Beispiel für ein Frame, Karte gefunden.
                Receive Packet: <==== 10:54:59
                38 00 26 
                46 46 30 30 30 30 31 45 34 42 30 31 30 31 34 34 30 30 30 30 30 38 38 38 30 34 36 36 30 34 41 31 31 32 30 30 31 30 
                          2   3   4  5   6   7   8   9  10 11                            18 19           (  UID ....                                                         )
                16
                EasyCODE_COMMENT_END
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x42) )
                {
                    // Reader Response Error Free
                    if ( (rcvbuffer[10] == 0x30)   &&  
                         (rcvbuffer[11] == 0x30) )
                    {
                        // Keine Karte im Feld
                        if ( rfid_retry > 5 )
                        {
                            rfid_err = 7;
                            rfid_key = 100;
                        }
                        else
                        {
                            rfid_retry++;
                            rfid_sub_key = 0;
                        }
                    }
                    else
                    {
                        // Karte im Feld
                        if ( (rcvbuffer[18] == 0x30)   &&  
                             (rcvbuffer[19] == 0x30) )
                        {
                            // Karte ist Mifare UL
                            EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                            send_mode1(RFID_READ_STR, TRUE);
                            rcv_frame_key = 0;
                            rfid_sub_key = 3;
                        }
                        else
                        {
                            // Falscher Kartentyp
                            rfid_err = 5;
                            rfid_key = 100;
                        }
                    }
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 3 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Response ACK Packet empfangen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 4;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 4 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Read Response empfangen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                EasyCODE_COMMENT_START 
                Beispiel fuer Read Response Beispiel:
                
                Receive Packet: <==== 11:55:46    (FF 00 00 24 41 00 33 35 39 30 2D 30 31 2F 30 31 2D 39 39 39 39 FF )
                38 00 2C 
                46 46 30 30 30 30 32 34 34 31 30 30 33 33 33 35 33 39  33 30    32 44 33 30 33 31  32 46  33 30 33 31 32 44 33 39 33 39 33 39 33 39 46 46  92 
                          2   3   4  5   6   7   8   9  10 11 12                                                                                                32      34                             41      43
                                                                        Nutzdaten .....                                                                                                        .... Nutzdaten
                Checksum                                                                             9d                                  52 
                nach der Wandlung aciihex_to_char ...
                33 35 39 30 2D 30 31 2F 30 31 2D 39 39 39 39 FF
                
                EasyCODE_COMMENT_END
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x31)   && 
                     (rcvbuffer[10] == 0x30)  &&  
                     (rcvbuffer[11] == 0x30) )
                {
                    // Reader Response Error Free, Nutzdaten alle da
                    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                    // Karte ist Mifare UL
                    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                    i = 0;
                    while ( i < 10 )
                    {
                        asciihex_to_char(rcvbuffer[(i*2) + 12], rcvbuffer[(i*2) + 13], &maklassestr[i]);
                        i ++;
                    }
                    maklassestr[i] = 0x00; // Nullterminator
                    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                    i = 0;
                    while ( i < 4 )
                    {
                        asciihex_to_char(rcvbuffer[(i*2) + 34], rcvbuffer[(i*2) + 35], &prognummerstrloc[i]);
                        i ++;
                    }
                    prognummerstrloc[i] = 0x00; // Nullterminator
                    rfidprognummer = atoi(prognummerstrloc);
                    if ( check_set_checksum(CHECK_CHECKSUM) )
                    {
                        rfid_enable_read = FALSE;   // Init durchführen um Zeit zu sparen 
                        rfid_key = 0;               // jedoch noch kein neuer Scan ausführen.
                    }
                    else
                    {
                        // Nutzdaten Checksumme falsch
                        rfid_err = 8;
                        rfid_key = 100;
                    }
                }
                else
                {
                    EasyCODE_COMMENT_START EasyCODE ( 0 
                    Karte nicht mehr da EasyCODE_COMMENT_END
                    // Fehler in der Read Response
                    // Starte ein Neulesen oder Error
                    if ( rfid_retry > 5 )
                    {
                        rfid_err = 7;
                        rfid_key = 100;
                    }
                    else
                    {
                        rfid_retry++;
                        rfid_sub_key = 0;
                    }
                    EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        default:
            break;
        }
        break;
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
    case 3 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        RFID Kommunikation Write EasyCODE_COMMENT_END
        switch ( rfid_sub_key )
        {
        case 0 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            RFID Reader Start Scan EasyCODE_COMMENT_END
            send_mode1(RFID_START_SCAN_STR, TRUE);
            rcv_frame_key = 0;
            rfid_sub_key = 1;
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 1 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Response ACK Packet empfangen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 2;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 2 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Scan Response empfangen und Write 1 ausgeben EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                EasyCODE_COMMENT_START 
                Beispiel für ein Frame, keine Karte gefunden.
                38 00 0C 
                46 46 30 30 30 30 30 34 34 42 30 30  // Nutzdaten
                          2   3   4  5   6   7   8   9  10 11    
                6E 
                
                Beispiel für ein Frame, Karte gefunden.
                Receive Packet: <==== 10:54:59
                38 00 26 
                46 46 30 30 30 30 31 45 34 42 30 31 30 31 34 34 30 30 30 30 30 38 38 38 30 34 36 36 30 34 41 31 31 32 30 30 31 30 
                          2   3   4  5   6   7   8   9  10 11                            18 19           (  UID ....                                                         )
                16
                EasyCODE_COMMENT_END
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x42) )
                {
                    // Reader Response Error Free
                    if ( (rcvbuffer[10] == 0x30)   &&  
                         (rcvbuffer[11] == 0x30) )
                    {
                        // Keine Karte im Feld
                        if ( rfid_retry > 5 )
                        {
                            rfid_err = 7;
                            rfid_key = 100;
                        }
                        else
                        {
                            rfid_retry++;
                            rfid_sub_key = 0;
                        }
                    }
                    else
                    {
                        // Karte im Feld
                        if ( (rcvbuffer[18] == 0x30)   &&  
                             (rcvbuffer[19] == 0x30) )
                        {
                            // Karte ist Mifare UL
                            EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                            sndbuffer[0] = 'w';
                            sndbuffer[1] = '4';
                            sndbuffer[2] = '0';
                            sndbuffer[3] = '4';   // Transponder block 4
                            
                            
                            //Beispiel Daten
                            //3590-01/01-9999x
                            //33 35 39 30   2D   30 31 2F 30 31   2D   39 39 39 39 x
                            // 0             3    4              7   8                11 12          15
                            EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                            checksumwrite = 0;
                            sprintf(rfidprognummerstr,"%04u",rfidprognummerwrite);
                            temp_ptr = &maklassestr[0];
                            zeichen_count = 0;
                            while ( zeichen_count < 4 )
                            {
                                char_to_asciihex(*temp_ptr, &highchar, &lowchar);
                                sndbuffer[4 + (2 * zeichen_count)] = highchar;
                                checksumwrite += highchar;
                                sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                                checksumwrite += lowchar;
                                temp_ptr++;
                                zeichen_count++;
                            }
                            sndbuffer[4 + (2 * zeichen_count)] = 0x00; // Nullterminator
                            send_mode1(&sndbuffer[0], TRUE);
                            rcv_frame_key = 0;
                            rfid_sub_key = 3;
                        }
                        else
                        {
                            // Falscher Kartentyp
                            rfid_err = 5;
                            rfid_key = 100;
                        }
                    }
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 3 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Response ACK Packet empfangen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 4;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 4 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Write Response 1 empfangen und Write 2 ausgeben EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                EasyCODE_COMMENT_START 
                Beispiel fuer erfolgreiche Write Response Beispiel:
                
                38 00 0C 
                46 46 30 30 30 30 30 34 34 31 30 30    7F 
                          2   3   4  5   6   7   8   9  10 11     
                
                
                
                
                
                EasyCODE_COMMENT_END
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x31)   && 
                     (rcvbuffer[10] == 0x30)  &&  
                     (rcvbuffer[11] == 0x30) )
                {
                    // Reader Response Error Free, Nutzdaten geschrieben
                    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                    sndbuffer[0] = 'w';
                    sndbuffer[1] = '4';
                    sndbuffer[2] = '0';
                    sndbuffer[3] = '5';   // Transponder block 5
                    
                    
                    //Beispiel Daten
                    //3590-01/01-9999x
                    //33 35 39 30   2D   30 31 2F 30 31   2D   39 39 39 39 x
                    // 0             3    4              7   8                11 12          15
                    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                    temp_ptr = &maklassestr[4];
                    zeichen_count = 0;
                    while ( zeichen_count < 4 )
                    {
                        char_to_asciihex(*temp_ptr, &highchar, &lowchar);
                        sndbuffer[4 + (2 * zeichen_count)] = highchar;
                        checksumwrite += highchar;
                        sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                        checksumwrite += lowchar;
                        temp_ptr++;
                        zeichen_count++;
                    }
                    sndbuffer[4 + (2 * zeichen_count)] = 0x00; // Nullterminator
                    send_mode1(&sndbuffer[0], TRUE);
                    rcv_frame_key = 0;
                    rfid_sub_key = 5;
                }
                else
                {
                    EasyCODE_COMMENT_START EasyCODE ( 0 
                    Karte nicht mehr da EasyCODE_COMMENT_END
                    // Fehler in der Write Response
                    // Starte ein Neuschreiben oder Error
                    if ( rfid_retry > 5 )
                    {
                        rfid_err = 7;
                        rfid_key = 100;
                    }
                    else
                    {
                        rfid_retry++;
                        rfid_sub_key = 0;
                    }
                    EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 5 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Response ACK Packet empfangen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 6;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 6 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Write Response 2 empfangen und Write 3 ausgeben EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                EasyCODE_COMMENT_START 
                Beispiel fuer erfolgreiche Write Response Beispiel:
                
                38 00 0C 
                46 46 30 30 30 30 30 34 34 31 30 30    7F 
                          2   3   4  5   6   7   8   9  10 11     
                
                
                
                
                
                EasyCODE_COMMENT_END
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x31)   && 
                     (rcvbuffer[10] == 0x30)  &&  
                     (rcvbuffer[11] == 0x30) )
                {
                    // Reader Response Error Free, Nutzdaten geschrieben
                    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                    sndbuffer[0] = 'w';
                    sndbuffer[1] = '4';
                    sndbuffer[2] = '0';
                    sndbuffer[3] = '6';   // Transponder block 6
                    
                    
                    //Beispiel Daten
                    //3590-01/01-9999x
                    //33 35 39 30   2D   30 31 2F 30 31   2D   39 39 39 39 x
                    // 0             3    4              7   8                11 12          15
                    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                    temp_ptr = &maklassestr[8];
                    zeichen_count = 0;
                    while ( zeichen_count < 2 )
                    {
                        char_to_asciihex(*temp_ptr, &highchar, &lowchar);
                        sndbuffer[4 + (2 * zeichen_count)] = highchar;
                        checksumwrite += highchar;
                        sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                        checksumwrite += lowchar;
                        temp_ptr++;
                        zeichen_count++;
                    }
                    char_to_asciihex(0x2D, &highchar, &lowchar);
                    sndbuffer[4 + (2 * zeichen_count)] = highchar;
                    checksumwrite += highchar;
                    sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                    checksumwrite += lowchar;
                    zeichen_count++;
                    
                    char_to_asciihex(rfidprognummerstr[0], &highchar, &lowchar);
                    sndbuffer[4 + (2 * zeichen_count)] = highchar;
                    checksumwrite += highchar;
                    sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                    checksumwrite += lowchar;
                    zeichen_count++;
                    
                    sndbuffer[4 + (2 * zeichen_count)] = 0x00; // Nullterminator
                    send_mode1(&sndbuffer[0], TRUE);
                    rcv_frame_key = 0;
                    rfid_sub_key = 7;
                }
                else
                {
                    EasyCODE_COMMENT_START EasyCODE ( 0 
                    Karte nicht mehr da EasyCODE_COMMENT_END
                    // Fehler in der Write Response
                    // Starte ein Neuschreiben oder Error
                    if ( rfid_retry > 5 )
                    {
                        rfid_err = 7;
                        rfid_key = 100;
                    }
                    else
                    {
                        rfid_retry++;
                        rfid_sub_key = 0;
                    }
                    EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 7 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Response ACK Packet empfangen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 8;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 8 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Write Response 3 empfangen und Write 4 ausgeben EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                EasyCODE_COMMENT_START 
                Beispiel fuer erfolgreiche Write Response Beispiel:
                
                38 00 0C 
                46 46 30 30 30 30 30 34 34 31 30 30    7F 
                          2   3   4  5   6   7   8   9  10 11     
                
                
                
                
                
                EasyCODE_COMMENT_END
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x31)   && 
                     (rcvbuffer[10] == 0x30)  &&  
                     (rcvbuffer[11] == 0x30) )
                {
                    // Reader Response Error Free, Nutzdaten geschrieben
                    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                    sndbuffer[0] = 'w';
                    sndbuffer[1] = '4';
                    sndbuffer[2] = '0';
                    sndbuffer[3] = '7';   // Transponder block 7
                    
                    
                    //Beispiel Daten
                    //3590-01/01-9999x
                    //33 35 39 30   2D   30 31 2F 30 31   2D   39 39 39 39 x
                    // 0             3    4              7   8                11 12          15
                    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                    zeichen_count = 0;
                    while ( zeichen_count < 3 )
                    {
                        char_to_asciihex(rfidprognummerstr[1 + zeichen_count], &highchar, &lowchar);
                        sndbuffer[4 + (2 * zeichen_count)] = highchar;
                        checksumwrite += highchar;
                        sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                        checksumwrite += lowchar;
                        zeichen_count++;
                    }
                    checksumwrite = ((unsigned char)-checksumwrite);  
                    char_to_asciihex(checksumwrite, &highchar, &lowchar);
                    sndbuffer[4 + (2 * zeichen_count)] = highchar;
                    //checksumwrite += highchar;
                    sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                    //checksumwrite += lowchar;
                    zeichen_count++;
                     
                    sndbuffer[4 + (2 * zeichen_count)] = 0x00; // Nullterminator
                    send_mode1(&sndbuffer[0], TRUE);
                    rcv_frame_key = 0;
                    rfid_sub_key = 9;
                }
                else
                {
                    EasyCODE_COMMENT_START EasyCODE ( 0 
                    Karte nicht mehr da EasyCODE_COMMENT_END
                    // Fehler in der Write Response
                    // Starte ein Neuschreiben oder Error
                    if ( rfid_retry > 5 )
                    {
                        rfid_err = 7;
                        rfid_key = 100;
                    }
                    else
                    {
                        rfid_retry++;
                        rfid_sub_key = 0;
                    }
                    EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 9 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Response ACK Packet empfangen EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 10;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 10 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Write Response 4 empfangen und Ende EasyCODE_COMMENT_END
            switch ( receive_mode1() )
            {
            case 0 :
                EasyCODE_COMMENT_START 
                Beispiel fuer erfolgreiche Write Response Beispiel:
                
                38 00 0C 
                46 46 30 30 30 30 30 34 34 31 30 30    7F 
                          2   3   4  5   6   7   8   9  10 11     
                
                
                
                
                
                EasyCODE_COMMENT_END
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x31)   && 
                     (rcvbuffer[10] == 0x30)  &&  
                     (rcvbuffer[11] == 0x30) )
                {
                    // Reader Response Error Free, Nutzdaten geschrieben
                    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                    rfid_enable_write = FALSE;
                    rfid_key = 0;
                }
                else
                {
                    EasyCODE_COMMENT_START EasyCODE ( 0 
                    Karte nicht mehr da EasyCODE_COMMENT_END
                    // Fehler in der Write Response
                    // Starte ein Neuschreiben oder Error
                    if ( rfid_retry > 5 )
                    {
                        rfid_err = 7;
                        rfid_key = 100;
                    }
                    else
                    {
                        rfid_retry++;
                        rfid_sub_key = 0;
                    }
                    EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        default:
            break;
        }
        break;
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
    case 100 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Enable Auftraege ruecksetzen EasyCODE_COMMENT_END
        rfid_enable_read = FALSE;
        rfid_enable_write = FALSE;
        rfid_key = 101;
        break;
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
    case 101 :
        EasyCODE_COMMENT_START EasyCODE ( 0 
        Warte bis Fehler rückgesetzt ist EasyCODE_COMMENT_END
        if ( (rfid_err == 0) )
        {
            EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
            rfidprognummer = 0xFFFD;
            forceinit = TRUE;
            rfid_key = 0;
            EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
        }
        else
        {
            // Fehler aktiv
            // Warten bis der Ablauf den Fehler auf 0 gesetzt hat.
        }
        break;
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
    default:
        break;
    }
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
*/
/* EasyCODE_STRUCTURED_COMMENT_END */
/* EasyCODE ( 0 
KlasseRfid::rfid   mit ARYGON und SANGOMA Reader */
/* EasyCODE F */
void KlasseRfid::rfid(void)
{
    unsigned char prognummerstrloc[20];
    unsigned char *temp_ptr;
    unsigned char i;
    unsigned char highchar;
    unsigned char lowchar;
    switch ( rfid_key )
    {
    case 0 :
        /* EasyCODE ( 0 
        Parameter initialisieren */
        //i = 0;
        /* EasyCODE - */
        zeichen_count = 0;
        rfid_sub_key = 0;
        rcv_frame_key = 0;
        rfid_retry = 0;
        rfid_key = 1;
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        RFID Reader initialisieren */
        switch ( rfid_sub_key )
        {
        case 0 :
            /* EasyCODE ( 0 
            Com init mit Reset oder nur Standard init ausfuehren */
            if ( sangoma )
            {
                rfid_sub_key = 0; 
                rfid_key = 2;       // Fertig
            }
            else
            {
                if ( /* EasyCODE < */
                !initcomrdy  ||  forceinit
                /* EasyCODE > */
                 )
                {
                    // Init nach power on oder nach Fehler Entstoerung
                    send_mode1(RFID_RESET_STR, TRUE);
                    rcv_frame_key = 0;
                    rfid_sub_key = 1;
                }
                else
                {
                    // Standard init (nur Antenne ausschalten)
                    send_mode1(RFID_ANTENNA_OFF, TRUE);
                    rcv_frame_key = 0;
                    rfid_sub_key = 20;
                }
            }
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            Reset Response Packet empfangen und Baudrate zum Reader PIC erhoehen */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    send_mode1(RFID_PIC_115200_BAUD, TRUE);
                    rcv_frame_key = 0;
                    rfid_sub_key = 2;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                if ( readerIDchanged )
                {
                    // Reader ID wurde schon 
                    // versucht zu aendern, ab 
                    // jetzt nur noch Error 
                    // ausgeben
                    rfid_key = 100;
                }
                else
                {
                    // Reader ID wurde seit power on noch nicht
                    // versucht einmal zu aendern 
                    rfid_err = 0; // Error ist noch kein Error beim ersten Durchlauf
                    send_mode1(RFID_EEPROM_LOGIN, FALSE); // Das Ack hierfuer geht verloren 
                                                          // wegen Reader ID noch auf 0x01
                    send_mode1(RFID_READERID_CHANGE, FALSE);
                    
                    readerIDchanged = TRUE; // nur einmal das EEPROM aendern
                    rcv_frame_key = 0;
                    rfid_sub_key = 30;
                }
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            PIC Baudrate Response ACK Packet empfangen und BS3 Baudrate erhoehen */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    #if !defined(remote)
                        InitPort (COM1,115200,PARITY_NONE,1,8);
                    #elif defined(com3)
                        InitPort (COM3,115200,PARITY_NONE,1,8);
                    #endif 
                    rcv_frame_key = 0;
                    
                    tschabl1.rsz ();
                    //tschabl1.sz (6);  // 60 ms Umschaltzeit.
                    tschabl1.sz (50);  // 500 ms Umschaltzeit.
                    
                    rfid_sub_key = 3;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            BS3 Umschaltung abwarten */
            if ( tschabl1.tz() )
            {
                send_mode1(RFID_TAMA_115200_BAUD, TRUE);
                rcv_frame_key = 0;
                rfid_sub_key = 7;
            }
            break;
            /* EasyCODE ) */
        case 7 :
            /* EasyCODE ( 0 
            TAMA Baudrate Response ACK Packet empfangen und TAMA RFID_MxRtyPassiveAct Einstellung */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    send_mode3(RFID_MxRtyPassiveAct_STR);
                    rcv_frame_key = 0;
                    rfid_sub_key = 8;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 8 :
            /* EasyCODE ( 0 
            RFID_MxRtyPassiveAct ACK empfangen */
            switch ( receive_mode3() )
            {
            case 0 :
                // TAMA ACK empfangen
                rcv_frame_key = 0;
                rfid_sub_key = 9;
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 9 :
            /* EasyCODE ( 0 
            RFID_MxRtyPassiveAct Response empfangen */
            switch ( receive_mode3() )
            {
            case 0 :
                if ( (rcvbuffer[0] == 0xD5)   &&   (rcvbuffer[1] == 0x33) )
                {
                    /* EasyCODE < */
                    initcomrdy = TRUE;
                    forceinit = FALSE;
                    
                    
                    rfid_sub_key = 0; 
                    rfid_key = 2;       // Fertig
                    /* EasyCODE > */
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 20 :
            /* EasyCODE ( 0 
            Response ACK Packet empfangen */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 21;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 21 :
            /* EasyCODE ( 0 
            Antenna OFF Response Packet empfangen */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     (rcvbuffer[6] == 0x30) &&  
                     (rcvbuffer[7] == 0x32) &&  
                     (rcvbuffer[8] == 0x33)   &&  
                     (rcvbuffer[9] == 0x33) )
                {
                    rfid_sub_key = 0; 
                    rfid_key = 2;       // Fertig
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 30 :
            /* EasyCODE ( 0 
            Reader ID change Response ACK Packet empfangen und EEPROM Logout ausgeben */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    send_mode1(RFID_EEPROM_LOGOUT, TRUE);
                    rcv_frame_key = 0;
                    rfid_sub_key = 31;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                if ( rfid_err == 3 // Time Out
                   )
                {
                    // ARYGON Reader umprogrammieren klappt auch nicht.
                    // Nun Versuch mit SANGOMA Reader.
                    #if !defined(remote)
                        InitPort (COM1,115200,PARITY_NONE,1,8);
                    #elif defined(com3)
                        InitPort (COM3,115200,PARITY_NONE,1,8);
                    #endif //
                    rfid_err = 0; // Error ist noch kein Error 
                    tschabl1.rsz ();
                    tschabl1.sz (50);  // 500 ms Umschaltzeit.
                    rfid_sub_key = 35;
                }
                else
                {
                    //Fehler:
                    rfid_key = 100;
                }
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 31 :
            /* EasyCODE ( 0 
            EEPROM Logout Response ACK Packet empfangen und zum Amnfang gehen */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    // fertig, starte nochmal ein Reader IC Reset an.
                    rcv_frame_key = 0;
                    rfid_sub_key = 0;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 35 :
            /* EasyCODE ( 0 
            SANGOMA BS3 Umschaltung abwarten */
            if ( tschabl1.tz() )
            {
                send_SANGOMA(RFID_SANGOMA_POLL);
                rcv_frame_key = 0;
                rfid_sub_key = 36;
            }
            break;
            /* EasyCODE ) */
        case 36 :
            /* EasyCODE ( 0 
            SANGOMA POLL ACK Packet empfangen und fertig */
            switch ( receive_SANGOMA() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], "PE") == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 2;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                //SANGOMA Reader reagiert auf das falsche Polling 
                // Also dann ist ein Sangoma Reader dran.
                
                sangoma = TRUE;
                initcomrdy = TRUE;
                forceinit = FALSE;
                
                rfid_sub_key = 0; 
                rfid_key = 2;       // fertig
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        default:
            break;
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        RFID Kommunikation Read */
        switch ( rfid_sub_key )
        {
        case 0 :
            /* EasyCODE ( 0 
            RFID Reader Start Scan */
            if ( rfid_enable_read )
            {
                rcv_frame_key = 0;
                if ( sangoma )
                {
                    send_SANGOMA(RFID_SANGOMA_READ);
                    rfid_sub_key = 10;
                }
                else
                {
                    send_mode1(RFID_START_SCAN_STR, TRUE);
                    rfid_sub_key = 1;
                }
            }
            else
            {
                if ( rfid_enable_write )
                {
                    rfid_sub_key = 0;
                    rfid_key = 3;
                }
                else
                {
                    // Warten
                }
            }
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            Response ACK Packet empfangen */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 2;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            Scan Response empfangen und Lesen */
            switch ( receive_mode1() )
            {
            case 0 :
                /*
                Beispiel für ein Frame, keine Karte gefunden.
                38 00 0C 
                46 46 30 30 30 30 30 34 34 42 30 30  // Nutzdaten
                          2   3   4  5   6   7   8   9  10 11    
                6E 
                
                Beispiel für ein Frame, Karte gefunden.
                Receive Packet: <==== 10:54:59
                38 00 26 
                46 46 30 30 30 30 31 45 34 42 30 31 30 31 34 34 30 30 30 30 30 38 38 38 30 34 36 36 30 34 41 31 31 32 30 30 31 30 
                          2   3   4  5   6   7   8   9  10 11                            18 19           (  UID ....                                                         )
                16
                */
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x42) )
                {
                    // Reader Response Error Free
                    if ( (rcvbuffer[10] == 0x30)   &&  
                         (rcvbuffer[11] == 0x30) )
                    {
                        // Keine Karte im Feld
                        if ( rfid_retry > 5 )
                        {
                            rfid_err = 7;
                            rfid_key = 100;
                        }
                        else
                        {
                            rfid_retry++;
                            rfid_sub_key = 0;
                        }
                    }
                    else
                    {
                        // Karte im Feld
                        if ( (rcvbuffer[18] == 0x30)   &&  
                             (rcvbuffer[19] == 0x30) )
                        {
                            // Karte ist Mifare UL
                            /* EasyCODE - */
                            send_mode1(RFID_READ_STR, TRUE);
                            rcv_frame_key = 0;
                            rfid_sub_key = 3;
                        }
                        else
                        {
                            // Falscher Kartentyp
                            rfid_err = 5;
                            rfid_key = 100;
                        }
                    }
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            Response ACK Packet empfangen */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 4;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 4 :
            /* EasyCODE ( 0 
            Read Response empfangen */
            switch ( receive_mode1() )
            {
            case 0 :
                /*
                Beispiel fuer Read Response Beispiel:
                
                Receive Packet: <==== 11:55:46    (FF 00 00 24 41 00 33 35 39 30 2D 30 31 2F 30 31 2D 39 39 39 39 FF )
                38 00 2C 
                46 46 30 30 30 30 32 34 34 31 30 30 33 33 33 35 33 39  33 30    32 44 33 30 33 31  32 46  33 30 33 31 32 44 33 39 33 39 33 39 33 39 46 46  92 
                          2   3   4  5   6   7   8   9  10 11 12                                                                                                32      34                             41      43
                                                                        Nutzdaten .....                                                                                                        .... Nutzdaten
                Checksum                                                                             9d                                  52 
                nach der Wandlung aciihex_to_char ...
                33 35 39 30 2D 30 31 2F 30 31 2D 39 39 39 39 FF
                
                */
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x31)   && 
                     (rcvbuffer[10] == 0x30)  &&  
                     (rcvbuffer[11] == 0x30) )
                {
                    // Reader Response Error Free, Nutzdaten alle da
                    /* EasyCODE - */
                    // Karte ist Mifare UL
                    /* EasyCODE - */
                    i = 0;
                    while ( i < 10 )
                    {
                        asciihex_to_char(rcvbuffer[(i*2) + 12], rcvbuffer[(i*2) + 13], &maklassestr[i]);
                        i ++;
                    }
                    maklassestr[i] = 0x00; // Nullterminator
                    /* EasyCODE - */
                    i = 0;
                    while ( i < 4 )
                    {
                        asciihex_to_char(rcvbuffer[(i*2) + 34], rcvbuffer[(i*2) + 35], &prognummerstrloc[i]);
                        i ++;
                    }
                    prognummerstrloc[i] = 0x00; // Nullterminator
                    rfidprognummer = atoi(prognummerstrloc);
                    if ( check_set_checksum(CHECK_CHECKSUM) )
                    {
                        rfid_enable_read = FALSE;   // Init durchführen um Zeit zu sparen 
                        rfid_key = 0;               // jedoch noch kein neuer Scan ausführen.
                    }
                    else
                    {
                        // Nutzdaten Checksumme falsch
                        rfid_err = 8;
                        rfid_key = 100;
                    }
                }
                else
                {
                    /* EasyCODE ( 0 
                    Karte nicht mehr da */
                    // Fehler in der Read Response
                    // Starte ein Neulesen oder Error
                    if ( rfid_retry > 5 )
                    {
                        rfid_err = 7;
                        rfid_key = 100;
                    }
                    else
                    {
                        rfid_retry++;
                        rfid_sub_key = 0;
                    }
                    /* EasyCODE ) */
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 10 :
            /* EasyCODE ( 0 
            SANGOMA Response empfangen und Lesen */
            switch ( receive_SANGOMA() )
            {
            case 0 :
                /*
                Beispiel für ein Frame, keine Karte gefunden.
                
                4E 54 
                 N  T    // 2 Byte
                
                Beispiel für einen Netto Frame, Karte gefunden.
                
                33 33 33 35 33 39 33 30 32 64 33 31 33 32 33 30 33 37 33 30 32 64 33 30 33 30 33 32 33 33 63 64 0D 0A 4F 4B   //36 Byte (30 Byte Nutzdaten)
                 0                                                       19
                
                                                          
                */
                if ( ( (strlen(rcvbuffer) == 2)   &&
                       (rcvbuffer[0] == 'P')      &&  
                       (rcvbuffer[1] == 'E')
                     )
                                              ||
                     ( (strlen(rcvbuffer) == 2)   &&
                       (rcvbuffer[0] == 'N')      &&  
                       (rcvbuffer[1] == 'T')
                     )
                                               ||
                     ( (strlen(rcvbuffer) == 36) &&
                       (rcvbuffer[34] == 'O')    &&  
                       (rcvbuffer[35] == 'K')
                     ) )
                {
                    // Reader Response Error Free
                    if ( (strlen(rcvbuffer) == 36) &&
                         (rcvbuffer[34] == 'O')    &&  
                         (rcvbuffer[35] == 'K') )
                    {
                        // Karte im Feld (Mifare UL wird angenommen)
                        /* EasyCODE - */
                        // Reader Response Error Free, Nutzdaten alle da
                        /* EasyCODE - */
                        i = 0;
                        while ( i < 10 )
                        {
                            asciihex_to_char(rcvbuffer[(i*2) + 0], rcvbuffer[(i*2) + 1], &maklassestr[i]);
                            i ++;
                        }
                        maklassestr[i] = 0x00; // Nullterminator
                        /* EasyCODE - */
                        i = 0;
                        while ( i < 4 )
                        {
                            asciihex_to_char(rcvbuffer[(i*2) + 22], rcvbuffer[(i*2) + 23], &prognummerstrloc[i]);
                            i ++;
                        }
                        prognummerstrloc[i] = 0x00; // Nullterminator
                        rfidprognummer = atoi(prognummerstrloc);
                        if ( check_set_checksum(CHECK_CHECKSUM) )
                        {
                            // Init durchführen um Zeit zu sparen 
                            // jedoch noch kein neuer Scan ausführen.
                            rfid_enable_read = FALSE;   
                            rfid_key = 0;               
                        }
                        else
                        {
                            // Nutzdaten Checksumme falsch
                            rfid_err = 8;
                            rfid_key = 100;
                        }
                    }
                    else
                    {
                        // Keine Karte im Feld
                        if ( rfid_retry > 5 )
                        {
                            rfid_err = 7;
                            rfid_key = 100;
                        }
                        else
                        {
                            rfid_retry++;
                            rfid_sub_key = 0;
                        }
                    }
                }
                else
                {
                    // unerwartete, 
                    // falsche Response
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        default:
            break;
        }
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        RFID Kommunikation Write */
        switch ( rfid_sub_key )
        {
        case 0 :
            /* EasyCODE ( 0 
            RFID Reader Start Scan */
            if ( sangoma )
            {
                rfid_sub_key = 20;
            }
            else
            {
                send_mode1(RFID_START_SCAN_STR, TRUE);
                rcv_frame_key = 0;
                rfid_sub_key = 1;
            }
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            Response ACK Packet empfangen */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 2;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            Scan Response empfangen und Write 1 ausgeben */
            switch ( receive_mode1() )
            {
            case 0 :
                /*
                Beispiel für ein Frame, keine Karte gefunden.
                38 00 0C 
                46 46 30 30 30 30 30 34 34 42 30 30  // Nutzdaten
                          2   3   4  5   6   7   8   9  10 11    
                6E 
                
                Beispiel für ein Frame, Karte gefunden.
                Receive Packet: <==== 10:54:59
                38 00 26 
                46 46 30 30 30 30 31 45 34 42 30 31 30 31 34 34 30 30 30 30 30 38 38 38 30 34 36 36 30 34 41 31 31 32 30 30 31 30 
                          2   3   4  5   6   7   8   9  10 11                            18 19           (  UID ....                                                         )
                16
                */
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x42) )
                {
                    // Reader Response Error Free
                    if ( (rcvbuffer[10] == 0x30)   &&  
                         (rcvbuffer[11] == 0x30) )
                    {
                        // Keine Karte im Feld
                        if ( rfid_retry > 5 )
                        {
                            rfid_err = 7;
                            rfid_key = 100;
                        }
                        else
                        {
                            rfid_retry++;
                            rfid_sub_key = 0;
                        }
                    }
                    else
                    {
                        // Karte im Feld
                        if ( (rcvbuffer[18] == 0x30)   &&  
                             (rcvbuffer[19] == 0x30) )
                        {
                            // Karte ist Mifare UL
                            /* EasyCODE - */
                            sndbuffer[0] = 'w';
                            sndbuffer[1] = '4';
                            sndbuffer[2] = '0';
                            sndbuffer[3] = '4';   // Transponder block 4
                            
                            
                            //Beispiel Daten
                            //3590-01/01-9999x
                            //33 35 39 30   2D   30 31 2F 30 31   2D   39 39 39 39 x
                            // 0             3    4              7   8                11 12          15
                            /* EasyCODE - */
                            checksumwrite = 0;
                            sprintf(rfidprognummerstr,"%04u",rfidprognummerwrite);
                            temp_ptr = &maklassestr[0];
                            zeichen_count = 0;
                            while ( zeichen_count < 4 )
                            {
                                char_to_asciihex(*temp_ptr, &highchar, &lowchar);
                                sndbuffer[4 + (2 * zeichen_count)] = highchar;
                                checksumwrite += highchar;
                                sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                                checksumwrite += lowchar;
                                temp_ptr++;
                                zeichen_count++;
                            }
                            sndbuffer[4 + (2 * zeichen_count)] = 0x00; // Nullterminator
                            send_mode1(&sndbuffer[0], TRUE);
                            rcv_frame_key = 0;
                            rfid_sub_key = 3;
                        }
                        else
                        {
                            // Falscher Kartentyp
                            rfid_err = 5;
                            rfid_key = 100;
                        }
                    }
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            Response ACK Packet empfangen */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 4;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 4 :
            /* EasyCODE ( 0 
            Write Response 1 empfangen und Write 2 ausgeben */
            switch ( receive_mode1() )
            {
            case 0 :
                /*
                Beispiel fuer erfolgreiche Write Response Beispiel:
                
                38 00 0C 
                46 46 30 30 30 30 30 34 34 31 30 30    7F 
                          2   3   4  5   6   7   8   9  10 11     
                
                
                
                
                
                */
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x31)   && 
                     (rcvbuffer[10] == 0x30)  &&  
                     (rcvbuffer[11] == 0x30) )
                {
                    // Reader Response Error Free, Nutzdaten geschrieben
                    /* EasyCODE - */
                    sndbuffer[0] = 'w';
                    sndbuffer[1] = '4';
                    sndbuffer[2] = '0';
                    sndbuffer[3] = '5';   // Transponder block 5
                    
                    
                    //Beispiel Daten
                    //3590-01/01-9999x
                    //33 35 39 30   2D   30 31 2F 30 31   2D   39 39 39 39 x
                    // 0             3    4              7   8                11 12          15
                    /* EasyCODE - */
                    temp_ptr = &maklassestr[4];
                    zeichen_count = 0;
                    while ( zeichen_count < 4 )
                    {
                        char_to_asciihex(*temp_ptr, &highchar, &lowchar);
                        sndbuffer[4 + (2 * zeichen_count)] = highchar;
                        checksumwrite += highchar;
                        sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                        checksumwrite += lowchar;
                        temp_ptr++;
                        zeichen_count++;
                    }
                    sndbuffer[4 + (2 * zeichen_count)] = 0x00; // Nullterminator
                    send_mode1(&sndbuffer[0], TRUE);
                    rcv_frame_key = 0;
                    rfid_sub_key = 5;
                }
                else
                {
                    /* EasyCODE ( 0 
                    Karte nicht mehr da */
                    // Fehler in der Write Response
                    // Starte ein Neuschreiben oder Error
                    if ( rfid_retry > 5 )
                    {
                        rfid_err = 7;
                        rfid_key = 100;
                    }
                    else
                    {
                        rfid_retry++;
                        rfid_sub_key = 0;
                    }
                    /* EasyCODE ) */
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 5 :
            /* EasyCODE ( 0 
            Response ACK Packet empfangen */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 6;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 6 :
            /* EasyCODE ( 0 
            Write Response 2 empfangen und Write 3 ausgeben */
            switch ( receive_mode1() )
            {
            case 0 :
                /*
                Beispiel fuer erfolgreiche Write Response Beispiel:
                
                38 00 0C 
                46 46 30 30 30 30 30 34 34 31 30 30    7F 
                          2   3   4  5   6   7   8   9  10 11     
                
                
                
                
                
                */
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x31)   && 
                     (rcvbuffer[10] == 0x30)  &&  
                     (rcvbuffer[11] == 0x30) )
                {
                    // Reader Response Error Free, Nutzdaten geschrieben
                    /* EasyCODE - */
                    sndbuffer[0] = 'w';
                    sndbuffer[1] = '4';
                    sndbuffer[2] = '0';
                    sndbuffer[3] = '6';   // Transponder block 6
                    
                    
                    //Beispiel Daten
                    //3590-01/01-9999x
                    //33 35 39 30   2D   30 31 2F 30 31   2D   39 39 39 39 x
                    // 0             3    4              7   8                11 12          15
                    /* EasyCODE - */
                    temp_ptr = &maklassestr[8];
                    zeichen_count = 0;
                    while ( zeichen_count < 2 )
                    {
                        char_to_asciihex(*temp_ptr, &highchar, &lowchar);
                        sndbuffer[4 + (2 * zeichen_count)] = highchar;
                        checksumwrite += highchar;
                        sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                        checksumwrite += lowchar;
                        temp_ptr++;
                        zeichen_count++;
                    }
                    char_to_asciihex(0x2D, &highchar, &lowchar);
                    sndbuffer[4 + (2 * zeichen_count)] = highchar;
                    checksumwrite += highchar;
                    sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                    checksumwrite += lowchar;
                    zeichen_count++;
                    
                    char_to_asciihex(rfidprognummerstr[0], &highchar, &lowchar);
                    sndbuffer[4 + (2 * zeichen_count)] = highchar;
                    checksumwrite += highchar;
                    sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                    checksumwrite += lowchar;
                    zeichen_count++;
                    
                    sndbuffer[4 + (2 * zeichen_count)] = 0x00; // Nullterminator
                    send_mode1(&sndbuffer[0], TRUE);
                    rcv_frame_key = 0;
                    rfid_sub_key = 7;
                }
                else
                {
                    /* EasyCODE ( 0 
                    Karte nicht mehr da */
                    // Fehler in der Write Response
                    // Starte ein Neuschreiben oder Error
                    if ( rfid_retry > 5 )
                    {
                        rfid_err = 7;
                        rfid_key = 100;
                    }
                    else
                    {
                        rfid_retry++;
                        rfid_sub_key = 0;
                    }
                    /* EasyCODE ) */
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 7 :
            /* EasyCODE ( 0 
            Response ACK Packet empfangen */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 8;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 8 :
            /* EasyCODE ( 0 
            Write Response 3 empfangen und Write 4 ausgeben */
            switch ( receive_mode1() )
            {
            case 0 :
                /*
                Beispiel fuer erfolgreiche Write Response Beispiel:
                
                38 00 0C 
                46 46 30 30 30 30 30 34 34 31 30 30    7F 
                          2   3   4  5   6   7   8   9  10 11     
                
                
                
                
                
                */
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x31)   && 
                     (rcvbuffer[10] == 0x30)  &&  
                     (rcvbuffer[11] == 0x30) )
                {
                    // Reader Response Error Free, Nutzdaten geschrieben
                    /* EasyCODE - */
                    sndbuffer[0] = 'w';
                    sndbuffer[1] = '4';
                    sndbuffer[2] = '0';
                    sndbuffer[3] = '7';   // Transponder block 7
                    
                    
                    //Beispiel Daten
                    //3590-01/01-9999x
                    //33 35 39 30   2D   30 31 2F 30 31   2D   39 39 39 39 x
                    // 0             3    4              7   8                11 12          15
                    /* EasyCODE - */
                    zeichen_count = 0;
                    while ( zeichen_count < 3 )
                    {
                        char_to_asciihex(rfidprognummerstr[1 + zeichen_count], &highchar, &lowchar);
                        sndbuffer[4 + (2 * zeichen_count)] = highchar;
                        checksumwrite += highchar;
                        sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                        checksumwrite += lowchar;
                        zeichen_count++;
                    }
                    checksumwrite = ((unsigned char)-checksumwrite);  
                    char_to_asciihex(checksumwrite, &highchar, &lowchar);
                    sndbuffer[4 + (2 * zeichen_count)] = highchar;
                    //checksumwrite += highchar;
                    sndbuffer[5 + (2 * zeichen_count)] = lowchar;
                    //checksumwrite += lowchar;
                    zeichen_count++;
                     
                    sndbuffer[4 + (2 * zeichen_count)] = 0x00; // Nullterminator
                    send_mode1(&sndbuffer[0], TRUE);
                    rcv_frame_key = 0;
                    rfid_sub_key = 9;
                }
                else
                {
                    /* EasyCODE ( 0 
                    Karte nicht mehr da */
                    // Fehler in der Write Response
                    // Starte ein Neuschreiben oder Error
                    if ( rfid_retry > 5 )
                    {
                        rfid_err = 7;
                        rfid_key = 100;
                    }
                    else
                    {
                        rfid_retry++;
                        rfid_sub_key = 0;
                    }
                    /* EasyCODE ) */
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 9 :
            /* EasyCODE ( 0 
            Response ACK Packet empfangen */
            switch ( receive_mode1() )
            {
            case 0 :
                if ( strcmp(&rcvbuffer[0], RFID_MODE1_RESPONSE) == 0 )
                {
                    rcv_frame_key = 0;
                    rfid_sub_key = 10;
                }
                else
                {
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 10 :
            /* EasyCODE ( 0 
            Write Response 4 empfangen und Ende */
            switch ( receive_mode1() )
            {
            case 0 :
                /*
                Beispiel fuer erfolgreiche Write Response Beispiel:
                
                38 00 0C 
                46 46 30 30 30 30 30 34 34 31 30 30    7F 
                          2   3   4  5   6   7   8   9  10 11     
                
                
                
                
                
                */
                if ( (rcvbuffer[2] == 0x30)   &&  
                     (rcvbuffer[3] == 0x30)   &&
                     (rcvbuffer[4] == 0x30)   &&  
                     (rcvbuffer[5] == 0x30)   &&
                     //(rcvbuffer[6] == 0x30) &&  
                     //(rcvbuffer[7] == 0x36) &&  
                     (rcvbuffer[8] == 0x34)   &&  
                     (rcvbuffer[9] == 0x31)   && 
                     (rcvbuffer[10] == 0x30)  &&  
                     (rcvbuffer[11] == 0x30) )
                {
                    // Reader Response Error Free, Nutzdaten geschrieben
                    /* EasyCODE - */
                    rfid_enable_write = FALSE;
                    rfid_key = 0;
                }
                else
                {
                    /* EasyCODE ( 0 
                    Karte nicht mehr da */
                    // Fehler in der Write Response
                    // Starte ein Neuschreiben oder Error
                    if ( rfid_retry > 5 )
                    {
                        rfid_err = 7;
                        rfid_key = 100;
                    }
                    else
                    {
                        rfid_retry++;
                        rfid_sub_key = 0;
                    }
                    /* EasyCODE ) */
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 20 :
            /* EasyCODE ( 0 
            SANGOMA write ausgeben */
            i = 1;
            sndbuffer[i] = 'W';  i++;
            sndbuffer[i] = 'H';  i++;
            sndbuffer[i] = '1';  i++;
            sndbuffer[i] = ',';  i++;
            
            
            //Beispiel Daten
            //3590-01/01-9999x
            //33 35 39 30   2D   30 31 2F 30 31   2D   39 39 39 39 x
            // 0             3    4              7   8                11 12          15
            /* EasyCODE - */
            checksumwrite = 0;
            sprintf(rfidprognummerstr,"%04u",rfidprognummerwrite);
            temp_ptr = &maklassestr[0];
            zeichen_count = 0;
            while ( zeichen_count < 10 )
            {
                char_to_asciihex(*temp_ptr, &highchar, &lowchar);
                sndbuffer[i] = highchar;  i++;
                checksumwrite += highchar;
                sndbuffer[i] = lowchar;  i++;
                checksumwrite += lowchar;
                temp_ptr++;
                zeichen_count++;
            }
                        char_to_asciihex(0x2D, &highchar, &lowchar);
            sndbuffer[i] = highchar;    i++;
            checksumwrite += highchar;
            sndbuffer[i] = lowchar;     i++;
            checksumwrite += lowchar;
            /* EasyCODE - */
            zeichen_count = 0;
            while ( zeichen_count < 4 )
            {
                char_to_asciihex(rfidprognummerstr[zeichen_count], &highchar, &lowchar);
                sndbuffer[i] = highchar;   i++;
                checksumwrite += highchar;
                sndbuffer[i] = lowchar;    i++;
                checksumwrite += lowchar;
                zeichen_count++;
            }
            checksumwrite = ((unsigned char)-checksumwrite);  
            char_to_asciihex(checksumwrite, &highchar, &lowchar);
            sndbuffer[i] = highchar;     i++;
            //checksumwrite += highchar;
            sndbuffer[i] = lowchar;      i++;
            
            //checksumwrite += lowchar;
            sndbuffer[0] = i-1;
            
            
            send_SANGOMA(sndbuffer);
            rcv_frame_key = 0;
            rfid_sub_key = 21;
            break;
            /* EasyCODE ) */
        case 21 :
            /* EasyCODE ( 0 
            SANGOMA Response empfangen  */
            switch ( receive_SANGOMA() )
            {
            case 0 :
                /*
                Beispiel für ein Frame, keine Karte gefunden.
                
                4E 54 
                 N  T    // 2 Byte
                                                          
                */
                if ( ( (strlen(rcvbuffer) == 2)   &&
                       (rcvbuffer[0] == 'P')      &&  
                       (rcvbuffer[1] == 'E')
                     )
                                              ||
                     ( (strlen(rcvbuffer) == 2)   &&
                       (rcvbuffer[0] == 'N')      &&  
                       (rcvbuffer[1] == 'T')
                     )
                                               ||
                     ( (strlen(rcvbuffer) == 2) &&
                       (rcvbuffer[0] == 'O')    &&  
                       (rcvbuffer[1] == 'K')
                     ) )
                {
                    // Reader Response Error Free
                    if ( (strlen(rcvbuffer) == 2) &&
                         (rcvbuffer[0] == 'O')    &&  
                         (rcvbuffer[1] == 'K') )
                    {
                        rfid_enable_write = FALSE;
                        rfid_key = 0;
                    }
                    else
                    {
                        // Keine Karte im Feld
                        if ( rfid_retry > 5 )
                        {
                            rfid_err = 7;
                            rfid_key = 100;
                        }
                        else
                        {
                            rfid_retry++;
                            rfid_sub_key = 0;
                        }
                    }
                }
                else
                {
                    // unerwartete, 
                    // falsche Response
                    rfid_err = 4;
                    rfid_key = 100;
                }
                break;
            case 1 :
                //warte und abgeben bis der Frame da ist
                break;
            case 2 :
                //Fehler:
                rfid_key = 100;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        default:
            break;
        }
        break;
        /* EasyCODE ) */
    case 100 :
        /* EasyCODE ( 0 
        Enable Auftraege ruecksetzen */
        rfid_enable_read = FALSE;
        rfid_enable_write = FALSE;
        rfid_key = 101;
        break;
        /* EasyCODE ) */
    case 101 :
        /* EasyCODE ( 0 
        Warte bis Fehler rückgesetzt ist */
        if ( (rfid_err == 0) )
        {
            /* EasyCODE < */
            rfidprognummer = 0xFFFD;
            forceinit = TRUE;
            rfid_key = 0;
            /* EasyCODE > */
        }
        else
        {
            // Fehler aktiv
            // Warten bis der Ablauf den Fehler auf 0 gesetzt hat.
        }
        break;
        /* EasyCODE ) */
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRfid::send_mode1(void) */
/* EasyCODE F */
void KlasseRfid::send_mode1(unsigned char * cmd_ptr, unsigned char normal_frame)
{
    /*
    Der Frame für das ARYGON Reader Protokoll im Mode 1 
    (Industrieprotokoll mit Checksumme) wird aufgesetzt und über
    den Com Port ausgegeben. 
    
    Input:
    Pointer welcher auf den zu sendenden Befehl zeigt (nullterminierende Nutzdaten)
    
    Output:
    nichts
    
    */
    /* EasyCODE - */
    unsigned char sndbufferloc[50];
    unsigned char *sndbuffptr;
    unsigned char i;
    unsigned char anz;
    unsigned char checksum;
    /* EasyCODE - */
    checksum = 0;
    sndbufferloc[0] = 0x31;   // Mode Select '1' = Netzwerkprotokoll
    if ( normal_frame )
    {
        // Reader ID = 0x00. Protokoll mit Checksumme
        sndbufferloc[1] = 0x00;
    }
    else
    {
        // Reader ID = 0x01. Protokoll mit Checksumme
        // Im Falle aller erstes Power On bei neuen Readern.
        sndbufferloc[1] = 0x01;
    }
    checksum = sndbufferloc[1];
    // sndbufferloc[2]  = ?              // laenge, noch unbekannt
    /* EasyCODE - */
    i = 0;
    while ( (*cmd_ptr) !=  0 )
    {
        sndbufferloc[3 + i] = *cmd_ptr;
        checksum += *cmd_ptr;
        cmd_ptr++;
        i++;
    }
    sndbufferloc[2] = i;  // laenge ist jetzt bekannt
    checksum += i;
    sndbufferloc[3 + i] =  ((unsigned char)-checksum);  
    i++;
    //sndbufferloc[3 + i ] = 0x00;  // Nullterminator
    anz = i + 3;
    sndbuffptr = &sndbufferloc[0];
    for ( i=0; i < anz; i++ )
    {
        #if !defined(remote)
            SendChar (COM1,*sndbuffptr);
        #elif defined(com3)
            SendChar (COM3,*sndbuffptr);
        #endif //
        sndbuffptr++;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRfid::send_mode3() */
/* EasyCODE F */
void KlasseRfid::send_mode3(unsigned char * cmd_ptr)
{
    /*
    Der Frame für das ARYGON Reader Protokoll im Mode 3 
    (Industrieprotokoll mit Checksumme) wird aufgesetzt und über
    den Com Port ausgegeben. 
    Mode 3 ist der Low Level Frame, welcher direkt zum NXP Reader IC
    durchgeschleift wird. Nur Low Level Register Funktionen werden
    damit umgeschaltet.
    Wir benötigen dieses um zum Beispiel dem Reader IC dazu zu bringen, dass er 
    sofort meldet, wenn keine Karte im Feld ist. Dieses Verhalten ist nicht default.
    
    Input:
    Pointer welcher auf den zu sendenden Befehl zeigt (nullterminierende Nutzdaten)
    
    
    
    Output:
    nichts
    
    */
    /* EasyCODE - */
    unsigned char sndbufferloc[50];
    unsigned char *sndbuffptr;
    unsigned char anz;
    unsigned char checksum;
    unsigned char i;
    /* EasyCODE < */
    /* TAMA low Level Beispiel:
    *RFID_MxRtyPassiveAct_STR =  "\x05\x32\x05\xFF\x00\x02"
    0x05 = Laenge der Nutzdaten. Nullterminierender String geht nicht, da 0x00 auch im Protokoll vorkommt !
    0x32 ... 0x02 = 5 Bytes Nutzdaten
    /* EasyCODE > */
    // Konstanter Protokoll Part:
    
    anz = *cmd_ptr + 1;   // Alle Nutzdaten plus TAMA TFI Byte 
    sndbufferloc[0] = 0x33; // ModeSelect '3'
    sndbufferloc[1] = 0x00; // Reader ID
    sndbufferloc[2] = 0x00; // preamble  TAMA ....
    sndbufferloc[3] = 0x00; // start code
    sndbufferloc[4] = 0xFF; // start code
    sndbufferloc[5] = (anz); // LEN
    sndbufferloc[6] = (((unsigned char)-anz)); // LEN checksum, uchar => autom. int erweiterung verhindern */
    sndbufferloc[7] = 0xD4; // TFI (from Host to TAMA)
    /* EasyCODE - */
    // Dynamischer Protokoll Part:
    i = 0;
    anz = *cmd_ptr;  // Anzahl Bytes der Nutzdaten ohne TFI
    cmd_ptr++;  // setze ptr auf das erste Nutzdatenbyte
    checksum = 0xD4;
    while ( i < (anz) )
    {
        sndbufferloc[8+i] = *cmd_ptr;
        checksum += *cmd_ptr;
        cmd_ptr++;
        i++;
    }
    sndbufferloc[8+i] = ((unsigned char)-checksum);
    i++;
    sndbufferloc[8+i] = 0x00; // TAMA postamble
    i++; 
    
    anz = 8 + i;  // komplette Anzahl Bytes des Frames
    sndbuffptr = &sndbufferloc[0];
    for ( i=0; i < anz; i++ )
    {
        #if !defined(remote)
            SendChar (COM1,*sndbuffptr);
        #elif defined(com3)
            SendChar (COM3,*sndbuffptr);
        #endif //
        sndbuffptr++;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRfid::send_SANGOMA() */
/* EasyCODE F */
void KlasseRfid::send_SANGOMA(unsigned char * cmd_ptr)
{
    /*
    Der Frame für das SANGOMA Reader Protokoll wird aufgesetzt und über
    den Com Port ausgegeben. 
    
    Input:
    Pointer welcher auf den zu sendenden Befehl zeigt (nullterminierende Nutzdaten) wobei auch
    im Nutzdatenstring ein 0x00 vorkommen kann !!!!
    
    */
    /* EasyCODE - */
    unsigned char sndbufferloc[50];
    unsigned char *sndbuffptr;
    unsigned char anz;
    unsigned char checksum;
    unsigned char i;
    /* EasyCODE < */
    /* SANGOMA Beispiel:
    *RFID_SANGOMA_POLL =  "\x04\x02\x0D\x0A\x03"   "\x0A\x02RT1,16\x0D\x0A\x03"
    0x04 = Laenge der Nutzdaten. Nullterminierender String geht nicht, da 0x00 auch im Protokoll vorkommt !
    0x02 ... 0x03 = 4 Bytes Nutzdaten
    /* EasyCODE > */
    // Konstanter Protokoll Part:
    sndbufferloc[0] = 0x02;
    /* EasyCODE - */
    // Dynamischer Protokoll Part:
    i = 0;
    anz = *cmd_ptr;  // Anzahl Bytes 
    cmd_ptr++;  // setze ptr auf das erste Nutzdatenbyte
    while ( i < (anz) )
    {
        sndbufferloc[1+i] = *cmd_ptr;
        cmd_ptr++;
        i++;
    }
    sndbufferloc[1+i] = 0x0D;
    i++;
    sndbufferloc[1+i] = 0x0A; 
    i++; 
    sndbufferloc[1+i] = 0x03; 
    i++; 
    
    anz = 1 + i;  // komplette Anzahl Bytes des Frames    
    sndbuffptr = &sndbufferloc[0];
    for ( i=0; i < anz; i++ )
    {
        #if !defined(remote)
            SendChar (COM1,*sndbuffptr);
        #elif defined(com3)
            SendChar (COM3,*sndbuffptr);
        #endif 
        sndbuffptr++;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRfid::receive_mode1(void) */
/* EasyCODE F */
unsigned char KlasseRfid::receive_mode1(void)
{
    /*
    Der Frame für das ARYGON Reader Protokoll im Mode 1 
    (Industrieprotokoll mit Checksumme) wurde komplett empfangen und steht bereit.
    
    
    Input:
    
    
    Output:
    Die empfangenen und mittels Checksumme (nur Protokoll Checksumme) geprüften Daten stehen 
    im rcvbuffer.
    Nur die Nutzdaten werden in den rcvbuffer kopiert. Protokollbytes werden abgeschnitten.
    
    Return Meldungen:
    0:  Fertig und Protokoll Checksumme ist OK.
    1:  Noch nicht fertig (noch nicht alle Bytes da)
    2:  Fehler ist aufgetaucht (siehe rfid_err)
    
    
     
    */
    unsigned char i;
    unsigned char zeichen_vorhanden;
    switch ( rcv_frame_key )
    {
    case 0 :
        tschabl1.rsz ();
        tschabl1.sz (100);  // 1 s Reaktionszeit
        zeichen_count = 0;
        frame_length = 0;   // laenge der Nutzdaten
        checksum = 0;
        rcv_frame_key = 1;
        break;
    case 1 :
        zeichen_vorhanden = TRUE;
        while ( zeichen_vorhanden )
        {
            #if !defined(remote)
                if ( RTKGetCond (ReceiveBuffer[COM1],&recdata.word) )
                {
                    if ( recdata.byte.high & HARD_ERROR )
                    {
                        // Fehler aufgetreten
                        rfid_err = 1; // HW Fehler
                        return 2;
                    }
                    else
                    {
                        // OK Zeichen da
                        /* EasyCODE - */
                        //rcv_frame_key=2;
                    }
                }
                else
                {
                    zeichen_vorhanden = FALSE;
                    if ( tschabl1.tz() )
                    {
                        rfid_err = 3; // Time Out
                        return 2;
                    }
                    else
                    {
                        // Warte und 
                        // abgeben
                    }
                }
            #elif defined(com3)
                if ( RTKGetCond (ReceiveBuffer[COM3],&recdata.word) )
                {
                    if ( recdata.byte.high & HARD_ERROR )
                    {
                        // Fehler aufgetreten
                        rfid_err = 1; // HW Fehler
                        return 2;
                    }
                    else
                    {
                        // OK Zeichen da
                        /* EasyCODE - */
                        //rcv_frame_key=2;
                    }
                }
                else
                {
                    zeichen_vorhanden = FALSE;
                    if ( tschabl1.tz() )
                    {
                        rfid_err = 3; // Time Out
                        return 2;
                    }
                    else
                    {
                        // Warte und 
                        // abgeben
                    }
                }
            #endif //
            if ( zeichen_vorhanden )
            {
                rcvbuffer[zeichen_count] = recdata.byte.low;
                zeichen_count++;
                if ( (zeichen_count > 1)  && (zeichen_count < (frame_length + 4)) )
                {
                    checksum += recdata.byte.low;
                }
                if ( zeichen_count == 3 )
                {
                    frame_length = recdata.byte.low;
                }
                if ( (frame_length + 4) == zeichen_count )
                {
                    rcv_frame_key = 3;   // kompletter Frame ist da
                    /* EasyCODE - */
                    // 18.11.09 break war falsch
                    break;
                }
                else
                {
                    rcv_frame_key = 1;  // noch nicht fertig
                }
            }
        }
        break;
    case 3 :
        if ( rcvbuffer[zeichen_count-1] == ((unsigned char)-checksum) )
        {
            // Checksum OK
            /* EasyCODE - */
            i = 0;
            while ( i < frame_length )
            {
                rcvbuffer[i] = rcvbuffer[i+3]; // only loaddata
                i++;
            }
            rcvbuffer[i] = 0x00;  // Nullterminator
            return 0;
        }
        else
        {
            // Checksum wrong
            rfid_err = 2;
            return 2;
        }
    default:
        break;
    }
    return 1;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRfid::receive_mode3(void) */
/* EasyCODE F */
unsigned char KlasseRfid::receive_mode3(void)
{
    /*
    Der Frame für das ARYGON Reader Protokoll im Mode 1 
    (Industrieprotokoll mit Checksumme) wurde komplett empfangen und steht bereit.
    
    
    Input:
    
    
    Output:
    Die empfangenen und mittels Checksumme (nur Protokoll Checksumme) geprüften Daten stehen 
    im rcvbuffer.
    Nur die Nutzdaten werden in den rcvbuffer kopiert. Protokollbytes werden abgeschnitten.
    
    Return Meldungen:
    0:  Fertig und Protokoll Checksumme ist OK oder TAMA (Reader IC) ACK received
    1:  Noch nicht fertig (noch nicht alle Bytes da)
    2:  Fehler ist aufgetaucht (siehe rfid_err)
    
    
    
    Beispiel von TAMA Rueckmeldungen
    0x39 0x00 0x00 0x00 0xFF 0x00 0xFF 0x00                                           // TAMA ACK packet
    0x39 0x00 0x00 0x00 0xFF 0x04 0xFC 0xD5 0x03 0x02 0x02 0x24 0x00  // Response packet
    
    oder
    
    Send Packet: ====>   16:59:08
    33 00 00 00 FF 06 FA D4 32 05 FF 00 02 F4 00 
    
    Receive Packet: <==== 16:59:08
    39 00 00 00 FF 00 FF 00 
    
    Receive Packet: <==== 16:59:08
    39 00 00 00 FF 02 FE D5 33 F8 00 
    00 01 02 03 04 05 06 07 08 09 10      Index
    
    */
    unsigned char i;
    unsigned char zeichen_vorhanden;
    switch ( rcv_frame_key )
    {
    case 0 :
        tschabl1.rsz ();
        tschabl1.sz (100);  // 1 s Reaktionszeit
        zeichen_count = 0;
        frame_length = 0;   // lange der Nutzdaten  mind. Wert = 2
        checksum = 0;
        rcv_frame_key = 1;
        break;
    case 1 :
        zeichen_vorhanden = TRUE;
        while ( zeichen_vorhanden )
        {
            #if !defined(remote)
                if ( RTKGetCond (ReceiveBuffer[COM1],&recdata.word) )
                {
                    if ( recdata.byte.high & HARD_ERROR )
                    {
                        // Fehler aufgetreten
                        rfid_err = 1; // HW Fehler
                        return 2;
                    }
                    else
                    {
                        // OK Zeichen da
                        /* EasyCODE - */
                        //rcv_frame_key=2;
                    }
                }
                else
                {
                    zeichen_vorhanden = FALSE;
                    if ( tschabl1.tz() )
                    {
                        rfid_err = 3; // Time Out
                        return 2;
                    }
                    else
                    {
                        // Warte und 
                        // abgeben
                    }
                }
            #elif defined(com3)
                if ( RTKGetCond (ReceiveBuffer[COM3],&recdata.word) )
                {
                    if ( recdata.byte.high & HARD_ERROR )
                    {
                        // Fehler aufgetreten
                        rfid_err = 1; // HW Fehler
                        return 2;
                    }
                    else
                    {
                        // OK Zeichen da
                        /* EasyCODE - */
                        //rcv_frame_key=2;
                    }
                }
                else
                {
                    zeichen_vorhanden = FALSE;
                    if ( tschabl1.tz() )
                    {
                        rfid_err = 3; // Time Out
                        return 2;
                    }
                    else
                    {
                        // Warte und 
                        // abgeben
                    }
                }
            #endif //
            if ( zeichen_vorhanden )
            {
                rcvbuffer[zeichen_count] = recdata.byte.low;
                zeichen_count++;
                if ( (zeichen_count > 7)  && (zeichen_count < (frame_length + 8)) )
                {
                    checksum += recdata.byte.low;
                }
                if ( zeichen_count == 6 )
                {
                    frame_length = recdata.byte.low;
                }
                if ( (frame_length + 9) == zeichen_count )
                {
                    // kompletter Frame ist da
                    rcv_frame_key = 3;
                    /* EasyCODE - */
                    // 18.11.09 break war falsch
                    break;
                }
                else
                {
                    if ( (frame_length == 0)
                                  &&
                         (zeichen_count == 8) )
                    {
                        // TAMA Ack Frame received
                        return 0;
                    }
                    else
                    {
                        // noch nicht fertig
                        rcv_frame_key = 1;
                    }
                }
            }
        }
        break;
    case 3 :
        if ( rcvbuffer[zeichen_count-2] == ((unsigned char)-checksum) )
        {
            // Checksum OK
            /* EasyCODE - */
            i = 0;
            while ( i < frame_length )
            {
                rcvbuffer[i] = rcvbuffer[i+7]; // only loaddata with TFI
                i++;
            }
            rcvbuffer[i] = 0x00;  // Nullterminator
            return 0;
        }
        else
        {
            // Checksum wrong
            rfid_err = 2;
            return 2;
        }
    default:
        break;
    }
    return 1;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRfid::receive_SANGOMA(void) */
/* EasyCODE F */
unsigned char KlasseRfid::receive_SANGOMA(void)
{
    /*
    Der Frame für das SANGOMA Reader Protokoll:
    Es wird solange empfangen bis ETX empfangen wurde.
    
    02 33 33 33 35 33 39 33 30 32 64 33 31 33 32 33 30 33 37 33 30 32 64 33 30 33 30 33 32 33 33 63 64 0D 0A 4F 4B 0D 0A 03 
    
    Input:
    
    
    Output:
    Die empfangenen Daten stehen im rcvbuffer aber um 5 unnoetige Byte kuerzer 
       33 33 33 35 33 39 33 30 32 64 33 31 33 32 33 30 33 37 33 30 32 64 33 30 33 30 33 32 33 33 63 64 0D 0A 4F 4B  
    
    
    Nur die Nutzdaten werden in den rcvbuffer kopiert. Protokollbytes werden abgeschnitten. 
    
    Return Meldungen:
    0:  Fertig und Protokoll Checksumme ist OK.
    1:  Noch nicht fertig (noch nicht alle Bytes da)
    2:  Fehler ist aufgetaucht (siehe rfid_err)
    
    
     
    */
    unsigned char i,k;
    unsigned char zeichen_vorhanden;
    switch ( rcv_frame_key )
    {
    case 0 :
        tschabl1.rsz ();
        tschabl1.sz (100);  // 1 s Reaktionszeit
        zeichen_count = 0;
        frame_length = 0;   // laenge der Nutzdaten
        checksum = 0;
        rcv_frame_key = 1;
        break;
    case 1 :
        zeichen_vorhanden = TRUE;
        while ( zeichen_vorhanden )
        {
            #if !defined(remote)
                if ( RTKGetCond (ReceiveBuffer[COM1],&recdata.word) )
                {
                    if ( recdata.byte.high & HARD_ERROR )
                    {
                        // Fehler aufgetreten
                        rfid_err = 1; // HW Fehler
                        return 2;
                    }
                    else
                    {
                        // OK Zeichen da
                        /* EasyCODE - */
                        //rcv_frame_key=2;
                    }
                }
                else
                {
                    zeichen_vorhanden = FALSE;
                    if ( tschabl1.tz() )
                    {
                        rfid_err = 3; // Time Out
                        return 2;
                    }
                    else
                    {
                        // Warte und 
                        // abgeben
                    }
                }
            #elif defined(com3)
                if ( RTKGetCond (ReceiveBuffer[COM3],&recdata.word) )
                {
                    if ( recdata.byte.high & HARD_ERROR )
                    {
                        // Fehler aufgetreten
                        rfid_err = 1; // HW Fehler
                        return 2;
                    }
                    else
                    {
                        // OK Zeichen da
                        /* EasyCODE - */
                        //rcv_frame_key=2;
                    }
                }
                else
                {
                    zeichen_vorhanden = FALSE;
                    if ( tschabl1.tz() )
                    {
                        rfid_err = 3; // Time Out
                        return 2;
                    }
                    else
                    {
                        // Warte und 
                        // abgeben
                    }
                }
            #endif //
            if ( zeichen_vorhanden )
            {
                rcvbuffer[zeichen_count] = recdata.byte.low;
                zeichen_count++;
                if ( recdata.byte.low == 0x03 )
                {
                    rcv_frame_key = 3;   // kompletter Frame ist da
                    /* EasyCODE - */
                    // 18.11.09 break war falsch
                    break;
                }
                else
                {
                    rcv_frame_key = 1;  // noch nicht fertig
                }
            }
        }
        break;
    case 3 :
        // Daten alle da
        if ( (zeichen_count  >  5)  &&
             (rcvbuffer[0] == 0x02) &&
             (rcvbuffer[zeichen_count-1] == 0x03) )
        {
            i = 0; k = 0;
            while ( i < zeichen_count )
            {
                if ( i < (zeichen_count - 5 - 1) )
                {
                    rcvbuffer[k] = rcvbuffer[i+1]; // only loaddata
                    k++;
                }
                i++;
            }
            rcvbuffer[k] = rcvbuffer[zeichen_count-5];
            k++;
            rcvbuffer[k] = rcvbuffer[zeichen_count-4];
            k++;
            rcvbuffer[k] = 0x00;  // Nullterminator
            k++;
            return 0;
        }
        else
        {
/* EasyCODE_STRUCTURED_COMMENT_START */
            /*
            if ( poll                                  &&
                 (zeichen_count  >  5)                 &&
                 (rcvbuffer[zeichen_count-5] == 'P')   &&
                 (rcvbuffer[zeichen_count-4] == 'E') )
            {
                // Polling, dann PE = OK
                return 0;
            }
            */
            /* EasyCODE_STRUCTURED_COMMENT_END */
            // Wrong Response
            rfid_err = 4;
            return 2;
        }
    default:
        break;
    }
    return 1;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRfid::asciihex_to_char(...) */
/* EasyCODE F */
void KlasseRfid::asciihex_to_char(unsigned char asciiHigh, unsigned char asciiLow, unsigned char* bytechar)
{
    /* 
    
    Wandlung von ASCII HEX zu ASCII
    
    Die Inputwerte duerfen nur in folgendem Range liegen (ASCII HEX):
    
    0x30 ...  0x39     oder      0x41 ... 0x46     (' 0 ... 9  oder A ...  F ')
    
    Beispiel:
    
    Input:
    0x33 0x35  = Zeichen '3'  und Zeichen  '5'
    oder 
    0x32 0x44   = zeichen '2'  'D'
     
    
    Output:
    0x35   =  Zeichen  '5'
    oder
    0x2D   = Zeichen '-'
    
    
    */
    if ( asciiHigh > 0x39 )
    {
        //Buchstabe   klein oder gross
        if ( asciiHigh > 0x46 )
        {
            asciiHigh -= 0x57;
        }
        else
        {
            asciiHigh -= 0x37;
        }
    }
    else
    {
        //Zahl
        asciiHigh -= 0x30;
    }
    if ( asciiLow > 0x39 )
    {
        //Buchstabe   klein oder gross
        if ( asciiLow > 0x46 )
        {
            asciiLow -= 0x57;
        }
        else
        {
            asciiLow -= 0x37;
        }
    }
    else
    {
        //Zahl
        asciiLow -= 0x30;
    }
    *bytechar = asciiLow;
    asciiHigh = (asciiHigh << 4) ;
    *bytechar += asciiHigh;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRfid::char_to_asciihex(...) */
/* EasyCODE F */
void KlasseRfid::char_to_asciihex(unsigned char bytechar, unsigned char* asciiHigh, unsigned char* asciiLow )
{
    /*
    
    Wandlung von Zeichen (Byte) zu ASCII HEX
    
    Beispiel:
    
    Input:
    bytechar = 0x2D   = Zeichen '-'
     
    
    Output:
    *asciiHigh = 0x32  = '2'
    *asciiLow = 0x44  = 'D'
    
    
    */
    /* EasyCODE - */
    *asciiHigh = (bytechar >> 4) ;
    if ( *asciiHigh > 9 )
    {
        // Buchstabe
        *asciiHigh += 0x37;
    }
    else
    {
        //zahl
        *asciiHigh += 0x30;
    }
    *asciiLow = (bytechar & 0x0f) ;
    if ( *asciiLow > 9 )
    {
        // Buchstabe
        *asciiLow += 0x37;
    }
    else
    {
        //zahl
        *asciiLow += 0x30;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRfid::check_set_checksum (...) */
/* EasyCODE F */
unsigned char KlasseRfid::check_set_checksum (unsigned char auftrag)
{
    /*
    Das letzte Byte in den Transponder Nutzdaten ist das Checksum Byte.
    Es ist das 2er Complement der Summe aller Bytes:
    Also Summe aller Bytes im ASCII HEX Format dann Negation aller Bits + 1
    In C sieht das wie folgt aus;
    Checksum = 0-(Summer aller Bytes).
    bzw. Kurzform und um eine automatische Int erweiterung zu vermeiden;
    Checksum = (unsigned char)-(Summer aller Bytes);
    
    Input:
    Auftrag ob Checksumme checken oder setzen mittels den defines:
    CHECK_CHECKSUM   und   SET_CHECKSUM
    
    Output:
    Im Falle CHECK_CHECKSUM: 
    Das Ergebnis des Vergleichs:  TRUE oder FALSE
    
    Im Falle SET_CHECKSUM: 
    Die berechnete Checksumme selbst zum schreiben in den Transponder
    
    */
    
    unsigned char i;
    unsigned char checksumist;
    unsigned char checksumsoll;
    switch ( auftrag )
    {
    case CHECK_CHECKSUM :
        checksumist = 0;
        //checksumsoll = 0; 
        i = 0;
        while ( i < 30 )
        {
            if ( sangoma )
            {
                if ( rcvbuffer[i + 0] > 0x60 )
                {
                    // Alle HEX Kleinbuchstaben sind als grosse generiert worden
                    // Darum in der Checks. diese nur als grosse rechnen
                    /* EasyCODE - */
                    checksumist += (rcvbuffer[i + 0] - 0x20); // wieder Gross
                }
                else
                {
                    checksumist += rcvbuffer[i + 0];
                }
            }
            else
            {
                checksumist += rcvbuffer[i + 12];
            }
            i ++;
        }
        checksumist = ((unsigned char)-checksumist);
        if ( sangoma )
        {
            asciihex_to_char(rcvbuffer[i], rcvbuffer[i+1], &checksumsoll);
        }
        else
        {
            asciihex_to_char(rcvbuffer[42], rcvbuffer[43], &checksumsoll);
        }
        if ( checksumsoll != checksumist )
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    case SET_CHECKSUM :
        // Wird berechnet wenn das Send Packet aufgesetzt wird
        return 0;
    default:
        return 0;
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
