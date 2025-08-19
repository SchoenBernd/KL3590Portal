/* EasyCODE V8 */
/* EasyCODE ( 0 
bcportal */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
/* Datei: bcportal.C        Klasse: 3590 Portal        Ersteller:Foko

Kurzbeschreibung:
=================
In dieser Datei befindet sich die zyklische Task barcode 
(Treiber fuer Barcode-Leser).
Der Barcode-Leser wird immer ueber die serielle Schnittstelle com1 angeschlossen.
*/

#include "portal.h"
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseBC::ablauf */
/* EasyCODE F */
// Task Barcode
//  Es wird von der Empfangs-Puffer der Com1 ein Zeichen eingelesen. 
//  Falls ein Hardware Fehler aufgetreten soll, wird scanner_err auf true gesetzt

void KlasseBC::ablauf(void)
{
    switch ( scan_key )
    {
    case 0 :
        /* EasyCODE ( 0 
        Variablen Initialisierung */
        res_barcode();
        zeichen_count = 0;
        scanner_err = FALSE;
        anzmerker = FALSE;
        scanablauf_key = 0;
        initBC_key = 0; //SW005-1
        merker_zeit_lauft = False;
        merker_falsche_zeichen = False;
        #if !defined(remote)
            RTKClearMailbox (SendBuffer[COM1]);
            RTKClearMailbox (ReceiveBuffer[COM1]);
            EnableCOMInterrupt (COM1,1);
        #elif defined(com3)
            RTKClearMailbox (SendBuffer[COM3]);
            RTKClearMailbox (ReceiveBuffer[COM3]);
            EnableCOMInterrupt (COM3,1);
        #endif 
        scan_key = 1;
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        Kommunikation */
        switch ( scanablauf_key )
        {
        case 0 :
            #if !defined(remote)
                if ( RTKGetCond (ReceiveBuffer[COM1],&recdata.word) )
                {
                    if ( recdata.byte.high & HARD_ERROR )
                    {
                        /* EasyCODE ( 0 
                        Fehler */
                        // Fehler aufgetreten
                        scanner_err = True; // HW Fehler
                        scanablauf_key=2;
                        /* EasyCODE - */
                        scan_key = 2;
                        /* EasyCODE ) */
                    }
                    else
                    {
                        /* EasyCODE ( 0 
                        Botschaft da */
                        scanablauf_key=1;
                        merker_zeit_lauft = False;
                        /* EasyCODE ) */
                    }
                }
                else
                {
                    /* EasyCODE ( 0 
                    keine Zeichen empfangen */
                    if ( zeichen_count==0 )
                    {
                        if ( (saf->md.jigctren==1) && (saf->jigctrf==BARCODE) &&
                             (barcode==0xffff) && !anzmerker )
                        {
                            schabl_botsch1.senden( bctext2);
                            anzmerker = TRUE;
                        }
                    }
                    else
                    {
                        if ( merker_zeit_lauft )
                        {
                        }
                        else
                        {
                            tschabl1.rsz ();
                            /* EasyCODE - */
                            //SW005-1
                            //tschabl1.sz (3);
                            tschabl1.sz (130);
                            /* EasyCODE - */
                            merker_zeit_lauft = True;
                        }
                        if ( tschabl1.tz() )
                        {
                            merker_zeit_lauft = False;
                            if ( (zeichen_count==4) &&(!merker_falsche_zeichen) )
                            {
                                barcode_ok = True;
                                scanablauf_key=2;
                            }
                            else
                            {
                                barcode_ok = False;
                                scanablauf_key = 2;
                            }
                        }
                    }
                    /* EasyCODE ) */
                }
            #elif defined(com3)
                if ( RTKGetCond (ReceiveBuffer[COM3],&recdata.word) )
                {
                    if ( recdata.byte.high & HARD_ERROR )
                    {
                        /* EasyCODE ( 0 
                        Fehler */
                        // Fehler aufgetreten
                        scanner_err = True; // HW Fehler
                        scanablauf_key=2;
                        /* EasyCODE - */
                        scan_key = 2;
                        /* EasyCODE ) */
                    }
                    else
                    {
                        /* EasyCODE ( 0 
                        Botschaft da */
                        // Botschaft da
                        scanablauf_key=1;
                        merker_zeit_lauft = False;
                        /* EasyCODE ) */
                    }
                }
                else
                {
                    /* EasyCODE ( 0 
                    keine Zeichen empfangen */
                    if ( zeichen_count==0 )
                    {
                        if ( (saf->md.jigctren==1) && (saf->jigctrf==BARCODE) &&
                             (barcode==0xffff) && !anzmerker )
                        {
                            schabl_botsch1.senden( bctext2);
                            anzmerker = TRUE;
                        }
                    }
                    else
                    {
                        if ( merker_zeit_lauft )
                        {
                        }
                        else
                        {
                            tschabl1.rsz ();
                            /* EasyCODE - */
                            //SW005-1
                            //tschabl1.sz (3);
                            tschabl1.sz (13);
                            /* EasyCODE - */
                            merker_zeit_lauft = True;
                        }
                        if ( tschabl1.tz() )
                        {
                            merker_zeit_lauft = False;
                            if ( (zeichen_count == 4) &&(!merker_falsche_zeichen) )
                            {
                                barcode_ok = True;
                                scanablauf_key=2;
                            }
                            else
                            {
                                barcode_ok = False;
                                scanablauf_key = 2;
                            }
                        }
                    }
                    /* EasyCODE ) */
                }
            #endif 
            break;
        case 1 :
            if ( ( recdata.byte.low < 0x30 ) || (recdata.byte.low > 0x39) )
            {
                merker_falsche_zeichen = True;
            }
            else
            {
                // Nur Ziffern in Barcode
            }
            barcode_asc[zeichen_count]=recdata.byte.low;	//Barcode abspeichern
            zeichen_count++;
            scanablauf_key=0;
            break;
        case 2 :
            if ( barcode_ok )
            {
                if ( nup.barcode_lesen )
                {
                    barcode = atoi(barcode_asc);
                }
            }
            else
            {
                res_barcode();
            }
            /* EasyCODE ( 0 
            Anzeige Barcode */
            if ( ((saf->md.jigctren==1) && nup.barcode_lesen) &&
                 (saf->jigctrf==BARCODE) && (koor.hbtart()==PRODUKTION) )
            {
                if ( barcode != 0xffff )
                {
                    sprintf(bcvarstr, bctext1, barcode);
                    schabl_botsch1.senden(bcvarstr);
                }
                else
                {
                    schabl_botsch1.senden( bctext2);
                }
            }
            /* EasyCODE ) */
            zeichen_count = 0;
            merker_falsche_zeichen = False;
            merker_zeit_lauft = False;
            scanablauf_key=0;
            anzmerker = FALSE;
            for ( int i=0;i<=20; i++ )
            {
                barcode_asc[i]= NULL;
            }
            break;
        default:
            scanablauf_key=0;
            anzmerker = FALSE;
            break;
        }
        break;
        /* EasyCODE ) */
    case 2 :
        if ( !scanner_err )
        {
            scan_key = 0;
            anzmerker = FALSE;
        }
        else
        {
            //SW005-1
            scan_key = 0;
            anzmerker = FALSE;
        }
        break;
    default:
        scan_key = 0;
        anzmerker = FALSE;
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseBC::statBC */
/* EasyCODE F */
void KlasseBC::statBC(void)
{
    unsigned char prognummerstrloc[20];
    unsigned char *temp_ptr;
    unsigned char i;
    unsigned char highchar;
    unsigned char lowchar;
    switch ( bcstat_key )
    {
    case 0 :
        /* EasyCODE ( 0 
        Parameter initialisieren */
        zeichen_count = 0;
        bcstat_sub_key = 0;
        bcstat_key = 1;
        bc_err = 0;
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        BC Reader initialisieren */
        switch ( bcstat_sub_key )
        {
        case 0 :
            /* EasyCODE ( 0 
            Com init mit Reset oder nur Standard init ausfuehren */
            if ( saf->bcinit_flag )
            {
                switch ( initStatBC() )
                {
                case 0 :
                    bcstat_key = 0;
                    /* EasyCODE - */
                    statScanOff();
                    bcstat_key = 2;
                    bc_err = 0;
                    /* EasyCODE - */
                    saf->bcinit_flag = FALSE;
                    break;
                default:
                    bc_err = 4; //bc initialisierungsfehler
                    bcprognummer = 0xffff;
                    break;
                }
            }
            else
            {
                statScanOff();
                bcstat_key = 2;
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
        StatBC Kommunikation Read */
        switch ( bcstat_sub_key )
        {
        case 0 :
            /* EasyCODE ( 0 
            BC READER Start Scan */
            if ( bc_enable_read )
            {
                statScanOn();
                bcstat_sub_key = 1;
            }
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            Read Response empfangen */
            switch ( receivebc_mode1() )
            {
            case 0 :
                //fertig
                bcstat_key = 0;
                bcstat_sub_key = 0;
                bc_enable_read = FALSE;
                break;
            case 1 :
                //warten bis fertig
                break;
            case 2 :
                //error
                bcstat_key = 100;
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
        bc_enable_read = FALSE;
        bc_enable_write = FALSE;
        bcstat_key = 101;
        break;
        /* EasyCODE ) */
    case 101 :
        /* EasyCODE ( 0 
        Warte bis Fehler rueckgesetzt ist */
        if ( bc_err == 0 )
        {
            bcprognummer = 0xFFFD;
            bcstat_key = 0;
        }
        break;
        /* EasyCODE ) */
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseBC::receivebc_res(void) */
/* EasyCODE F */
unsigned char KlasseBC::receivebc_res(unsigned char *rescmd)
{
    // Return Meldungen:
    // 0:  Fertig und Protokoll Checksumme ist OK.
    // 1:  Noch nicht fertig (noch nicht alle Bytes da)
    // 2:  Fehler ist aufgetaucht (siehe bc_err)
    // 3: Daten empfangen, vergleich aber negativ
    
    unsigned char i;
    unsigned int j;
    unsigned char zeichen_vorhanden;
    unsigned int dataRead;
    unsigned char tmpbuff[10];
    unsigned char bufferloc[10];
    unsigned char cmdbuff[10];
    unsigned char prognummerstrloc[20];
    switch ( getBC_key )
    {
    case 0 :
        tschabl1.rsz ();
        tschabl1.sz (200);  // 2 s Reaktionszeit
        zeichen_count = 0;
        dataRead = 0;
        getBC_key = 1;
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
                        //checkpoint
                        bc_err = 1; // HW Fehler
                        bcprognummer = 0xffff;
                        getBC_key=0;
                        return 2;
                    }
                    else
                    {
                        // OK Zeichen da
                        /* EasyCODE - */
                        //getBC_key=2;
                    }
                }
                else
                {
                    zeichen_vorhanden = FALSE;
                    if ( tschabl1.tz() )
                    {
                        bc_err = 3; // Time Out
                        getBC_key=0;
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
                        bc_err = 1; // HW Fehler
                        bcprognummer = 0xffff;
                        getBC_key=0;
                        return 2;
                    }
                    else
                    {
                        // OK Zeichen da
                        /* EasyCODE - */
                        //getBC_key=2;
                    }
                }
                else
                {
                    zeichen_vorhanden = FALSE;
                    if ( tschabl1.tz() )
                    {
                        bc_err = 3; // Time Out
                        getBC_key=0;
                        return 2;
                    }
                    else
                    {
                        // Warte und 
                        // abgeben
                    }
                }
            #endif 
            if ( zeichen_vorhanden )
            {
                /* EasyCODE ( 0 
                Ein Zeichen lesen */
                tmpbuff[zeichen_count] = recdata.byte.low;
                zeichen_count++;
                /* EasyCODE ) */
                if ( zeichen_count == 4 )
                {
                    //in rcvbuffer kopieren
                    /* EasyCODE_STRUCTURED_COMMENT_START */
                    /*
                    for ( int j = 0; j < zeichen_count;j++ )
                    {
                        rcvbuffer[j] = tmpbuff[j];
                    }
                    rcvbuffer[zeichen_count] = 0x00; //Nullterminator
                    for ( j = 0; j < zeichen_count;j++ )
                    {
                        asciihex_to_char(tmpbuff[j], &prognummerstrloc[j]);
                    }
                    bcconfigres = atoi(tmpbuff);
                    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                    */
                    /* EasyCODE_STRUCTURED_COMMENT_END */
                    getBC_key = 3;   // kompletter code ist da
                    break;
                }
            }
        }
        break;
    case 3 :
        //Response received, compare
        /* EasyCODE - */
        unsigned char *readdata = &tmpbuff[0];
        /* EasyCODE - */
        unsigned char *rescmddat;
        /* EasyCODE - */
        int anz = 0;
        while ( anz < 8 )
        {
            bufferloc[anz] = *rescmd;
            rescmd++;
            anz++;
        }
        bufferloc[anz ] = 0x00;  // Nullterminator
        int k=0;
        /* EasyCODE ( 0 
        Ascii-Hex String in unsigned char array umwandeln */
        for ( int j=0; j < anz-1; j=j+2 )
        {
            asciihex_to_char(bufferloc[j],bufferloc[j+1],&cmdbuff[k]);
            k++;
        }
        /* EasyCODE ) */
        rescmddat = &cmdbuff[0];
        for ( int i = 0; i < k; i++ )
        {
            if ( *readdata == *rescmddat )
            {
                readdata++;
                rescmddat++;
            }
            else
            {
                /* EasyCODE ( 0 
                Gelesene Daten != Vorgabe */
                getBC_key=0;
                return 3;
                /* EasyCODE ) */
            }
        }
        /* EasyCODE ( 0 
        Gelesene Daten stimmen mit Vorgabe Åberein */
        getBC_key=0;
        return 0;
        /* EasyCODE ) */
    default:
        break;
    }
    return 1;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseBC::receivebc_mode1(void) */
/* EasyCODE F */
unsigned char KlasseBC::receivebc_mode1(void)
{
    // Return Meldungen:
    // 0:  Fertig und Protokoll Checksumme ist OK.
    // 1:  Noch nicht fertig (noch nicht alle Bytes da)
    // 2:  Fehler ist aufgetaucht (siehe bc_err)
    
    unsigned char i;
    unsigned int j;
    unsigned char zeichen_vorhanden;
    unsigned int dataRead;
    unsigned char tmpbuff[10];
    unsigned char prognummerstrloc[20];
    switch ( getBC_key )
    {
    case 0 :
        tschabl1.rsz ();
        tschabl1.sz (500);  // 5 s Reaktionszeit
        zeichen_count = 0;
        dataRead = 0;
        getBC_key = 1;
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
                        //checkpoint
                        // Fehler aufgetreten
                        bc_err = 1; // HW Fehler
                        bcprognummer = 0xffff;
                        getBC_key=0;
                        return 2;
                    }
                    else
                    {
                        // OK Zeichen da
                        /* EasyCODE - */
                        //getBC_key=2;
                    }
                }
                else
                {
                    zeichen_vorhanden = FALSE;
                    if ( tschabl1.tz() )
                    {
                        bc_err = 3; // Time Out
                        bcprognummer = 0xffff;
                        getBC_key=0;
                        statScanOff();
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
                        bc_err = 1; // HW Fehler
                        bcprognummer = 0xffff;
                        getBC_key=0;
                        return 2;
                    }
                    else
                    {
                        // OK Zeichen da
                        /* EasyCODE - */
                        //getBC_key=2;
                    }
                }
                else
                {
                    zeichen_vorhanden = FALSE;
                    if ( tschabl1.tz() )
                    {
                        bc_err = 3; // Time Out
                        bcprognummer = 0xffff;
                        getBC_key=0;
                        statScanOff();
                        return 2;
                    }
                    else
                    {
                        // Warte und 
                        // abgeben
                    }
                }
            #endif 
            if ( zeichen_vorhanden )
            {
                if ( recdata.byte.low == 0x0A )
                {
                    // end of data
                    dataRead = 0;
                    if ( zeichen_count == 4 )
                    {
                        //in rcvbuffer kopieren
                        /* EasyCODE_STRUCTURED_COMMENT_START */
                        /*
                        for ( int j = 0; j < zeichen_count;j++ )
                        {
                            rcvbuffer[j] = tmpbuff[j];
                        }
                        rcvbuffer[zeichen_count] = 0x00; //Nullterminator
                        for ( j = 0; j < zeichen_count;j++ )
                        {
                            asciihex_to_char(tmpbuff[j], &prognummerstrloc[j]);
                        }
                        */
                        /* EasyCODE_STRUCTURED_COMMENT_END */
                        //checkpoint
                        //prognummerstrloc[j] = 0x00; // Nullterminator
                        bcprognummer = atoi(tmpbuff);
                        //bcprognummer = atoi(prognummerstrloc);
                        /* EasyCODE - */
                        getBC_key = 3;   // kompletter code ist da
                        break;
                    }
                    else
                    {
                        if ( zeichen_count > 4 )
                        {
                            getBC_key = 1;  // keine gÅltigen Daten
                            zeichen_count = 0;
                        }
                        else
                        {
                            getBC_key = 1;  // keine gÅltigen Daten
                            //zeichen_count = 0;
                        }
                    }
                }
                if ( dataRead )
                {
                    if ( recdata.byte.low != 0x02
                         && recdata.byte.low != 0x0A
                         && recdata.byte.low != 0x0D
                         && recdata.byte.low != 0x18 )
                    {
                        tmpbuff[zeichen_count] = recdata.byte.low;
                        zeichen_count++;
                    }
                    else
                    {
                        if ( recdata.byte.low != 0x02 )
                        {
                        }
                        else
                        {
                            zeichen_count=0;
                        }
                    }
                }
                else
                {
                    if ( recdata.byte.low == 0x02 )
                    {
                        //start of data
                        dataRead = 1;
                        zeichen_count=0;
                    }
                }
            }
        }
        break;
    case 3 :
        // Barcode received
        getBC_key=0;
        return 0;
    default:
        break;
    }
    return 1;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseBC::sendbc_cmd(void) */
/* EasyCODE F */
void KlasseBC::sendbc_cmd(unsigned char * cmd_ptr)
{
    #if !defined(remote)
        SendChar (COM1,*cmd_ptr);
    #elif defined(com3)
        SendChar (COM3,*cmd_ptr);
    #endif 
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseBC::statScanOn(void) */
/* EasyCODE F */
void KlasseBC::statScanOn(void)
{
    unsigned char *tmpcmd;
    *tmpcmd = STAT_SCANON;
    sendbc_cmd(tmpcmd);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseBC::statScanOff(void) */
/* EasyCODE F */
void KlasseBC::statScanOff(void)
{
    unsigned char *tmpcmd ;
    *tmpcmd = STAT_SCANOFF;
    sendbc_cmd(tmpcmd);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseBC::initStatBC(void) */
/* EasyCODE F */
unsigned char KlasseBC::initStatBC(void)
{
    //int done = FALSE;
    /*
    RÅckgabe
    0 = fertig;
    1 = fehler;
    2 = noch nicht fertig;
    */
    //handshake
    
    //konfiguration senden
    
    //handshake
    switch ( initBC_key )
    {
    case 0 :
        /* EasyCODE ( 0 
        Konfiguration anstossen */
        //Senden: 1b 5b 43
        send_mode2(BC_STARTCOMM);
        initBC_key = 1;
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        Antwort abwarten */
        switch ( receivebc_res(BC_STARTCOMM_RES) )
        {
        case 0 :
            /* EasyCODE ( 0 
            Antwort erhalten */
            initBC_key = 2;
            break;
            /* EasyCODE ) */
        case 1 :
            //Noch nicht fertig
            break;
        default:
            //Fehler
            /* EasyCODE - */
            initBC_key = 100;
            break;
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        Konfiguration anstossen */
        //Senden: 1b 5b 43
        send_mode2(BC_STARTCONF);
        initBC_key = 3;
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        Antwort abwarten */
        //Warten auf: 1B 48 0D 0A
        switch ( receivebc_res(BC_STARTCONF_RES) )
        {
        case 0 :
            /* EasyCODE ( 0 
            Antwort erhalten */
            initBC_key = 4;
            break;
            /* EasyCODE ) */
        case 1 :
            //Noch nicht fertig
            break;
        default:
            //Fehler
            /* EasyCODE - */
            initBC_key = 100;
            break;
        }
        break;
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        Konfiguration senden */
        send_mode2(BC_CONFIGDATA);
        /* EasyCODE - */
        initBC_key = 5;
        break;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        Antwort abwarten */
        //Warten auf: 1B 4B 0D 0A
        switch ( receivebc_res(BC_KONFDAT_RES) )
        {
        case 0 :
            /* EasyCODE [ 
            Antwort erhalten */
            initBC_key = 6;
            /* EasyCODE ] */
            break;
        case 1 :
            //Noch nicht fertig
            break;
        default:
            //Fehler
            /* EasyCODE - */
            //initBC_key = 100;
            break;
        }
        break;
        /* EasyCODE ) */
    case 6 :
        /* EasyCODE ( 0 
        Konfiguration abschliessen */
        //Senden: 1B 5B 41
        send_mode2(BC_ENDCONF);
        initBC_key = 7;
        break;
        /* EasyCODE ) */
    case 7 :
        /* EasyCODE ( 0 
        Antwort abwarten */
        //Warten auf: 1B 58 0D 0A
        //done = true;
        switch ( receivebc_res(BC_ENDCONF_RES) )
        {
        case 0 :
            /* EasyCODE [ 
            Antwort erhalten, konfiguration abgeschlossen */
            initBC_key = 0;
            return 0;
            /* EasyCODE ] */
            break;
        case 1 :
            //Noch nicht fertig
            break;
        default:
            //Fehler
            /* EasyCODE - */
            initBC_key = 100;
            break;
        }
        break;
        /* EasyCODE ) */
    case 100 :
        /* EasyCODE ( 0 
        Fehler */
        initBC_key = 0;
        return 1;
        break;
        /* EasyCODE ) */
    default:
        break;
    }
    return 2;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseBC::send_mode1(void) */
/* EasyCODE F */
void KlasseBC::send_mode1(unsigned char * cmd_ptr)
{
    /*
    Der Frame f¸r das ARYGON Reader Protokoll im Mode 1 
    (Industrieprotokoll mit Checksumme) wird aufgesetzt und ¸ber
    den Com Port ausgegeben. 
    
    Input:
    Pointer welcher auf den zu sendenden Befehl zeigt (nullterminierende Nutzdaten)
    
    Output:
    nichts
    
    */
    /* EasyCODE - */
    unsigned char sndbufferloc[500];
    unsigned char *sndbuffptr;
    unsigned char i;
    unsigned char anz;
    unsigned char checksum;
    /* EasyCODE - */
    //checksum = 0;
    //sndbufferloc[0] = 0x31;   // Mode Select '1' = Netzwerkprotokoll
    /* EasyCODE_STRUCTURED_COMMENT_START */
    /*
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
    */
    /* EasyCODE_STRUCTURED_COMMENT_END */
    //checksum = sndbufferloc[1];
    // sndbufferloc[2]  = ?              // laenge, noch unbekannt
    /* EasyCODE - */
    i = 0;
    while ( (*cmd_ptr) !=  0 )
    {
        sndbufferloc[3 + i] = *cmd_ptr;
        //checksum += *cmd_ptr;
        cmd_ptr++;
        i++;
    }
    //sndbufferloc[2] = i;  // laenge ist jetzt bekannt
    //checksum += i;
    //sndbufferloc[3 + i] =  ((unsigned char)-checksum);  
    //i++;
    //sndbufferloc[3 + i ] = 0x00;  // Nullterminator
    anz = i ;
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
KlasseBC::send_mode2(void) */
/* EasyCODE F */
void KlasseBC::send_mode2(unsigned char * cmd_ptr)
{
    /*
    Der Frame f¸r das ARYGON Reader Protokoll im Mode 1 
    (Industrieprotokoll mit Checksumme) wird aufgesetzt und ¸ber
    den Com Port ausgegeben. 
    
    Input:
    Pointer welcher auf den zu sendenden Befehl zeigt (nullterminierende Nutzdaten)
    
    Output:
    nichts
    
    */
    /* EasyCODE - */
    unsigned char sndbufferloc[2000];
    unsigned char sndbuffercmd[500];
    unsigned char *sndbuffptr;
    unsigned int i;
    unsigned int anz;
    unsigned char checksum;
    /* EasyCODE - */
    //checksum = 0;
    //sndbufferloc[0] = 0x31;   // Mode Select '1' = Netzwerkprotokoll
    /* EasyCODE_STRUCTURED_COMMENT_START */
    /*
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
    */
    /* EasyCODE_STRUCTURED_COMMENT_END */
    //checksum = sndbufferloc[1];
    // sndbufferloc[2]  = ?              // laenge, noch unbekannt
    /* EasyCODE - */
    i = 0;
    while ( (*cmd_ptr) !=  0 )
    {
        sndbufferloc[i] = *cmd_ptr;
        //checksum += *cmd_ptr;
        cmd_ptr++;
        i++;
    }
    //sndbufferloc[2] = i;  // laenge ist jetzt bekannt
    //checksum += i;
    //sndbufferloc[3 + i] =  ((unsigned char)-checksum);  
    //i++;
    sndbufferloc[i ] = 0x00;  // Nullterminator
    anz = i ;
    sndbuffptr = &sndbufferloc[0];
    int k=0;
    for ( int j=0; j < anz-1; j=j+2 )
    {
        asciihex_to_char(sndbufferloc[j],sndbufferloc[j+1],&sndbuffercmd[k]);
        k++;
    }
    sndbufferloc[k] = 0x00;  // Nullterminator
    sndbuffptr = &sndbuffercmd[0];
    for ( i=0; i < k; i++ )
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
void KlasseBC::asciihex_to_char(unsigned char asciiHigh, unsigned char asciiLow, unsigned char* bytechar) */
/* EasyCODE F */
void KlasseBC::asciihex_to_char(unsigned char asciiHigh, unsigned char asciiLow, unsigned char* bytechar)
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
        //Buchstabe
        asciiHigh -= 0x37;
    }
    else
    {
        //Zahl
        asciiHigh -= 0x30;
    }
    if ( asciiLow > 0x39 )
    {
        //Buchstabe
        asciiLow -= 0x37;
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
/* EasyCODE ) */
