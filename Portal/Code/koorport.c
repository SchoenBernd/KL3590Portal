/* EasyCODE V8 */
/* EasyCODE ( 0 
koorport */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
// Datei: koorport.c
// Klasse: 3590 Portal
// Ersteller: Halfmann EWE 2856

// Die Hauptaufgaben des Koordinator sind:
//   - Aufbereitung des Eingangsabbildes
//   - Notstopueberwachung
//   - Realisierung der Hauptbetriebszustaende
//   - Durchf+hrung der Power-on-Aktionen
//   - Fehlerbearbeitung, inklusive Ausschaltfehler
//   - Koordination der Abl+ufe und Tasks ueber Stopanforderungen und
//       Stopbestaetigungen


#include "portal.h"

/* intern benoetigte Strings */
unsigned char * kennunglesens  = {CLRHOME"KENNUNG LESEN"ESCS"i"};                // Kennung des BDF-T1 lesen
unsigned char * delkennungs  = {CLRHOME"KENNUNG LOESCHEN"ESCS"IXXXXXXXX"};        // Kennung des BDF-T1 loeschen
unsigned char * kennungschreibens  = {CLRHOME"KENNUNG SCHREIBEN"ESCS"I"KENNUNG}; // Kennung des BDF-T1 schreiben
unsigned char * errs = {CLRHOME Z3 FONT1 "ERROR: %2u"};
unsigned char * ausschalts = {Z6 "SWITCH MACHINE OFF BRIEFLY"};

/* Datendefinitionen */
int prog_handle [PNRINDEX_MAX+1];
char *ndatanf;
unsigned char c167_cnt;
KlasseKoordinator koor (tdini [KoorIndex]);
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::koortask */
/* EasyCODE F */
void KlasseKoordinator::koortask (void)
{
    in_image_up();
    switch ( key )
    {
    case 0 :
        poweron();
        break;
    case 1 :
        eingabe();
        break;
    case 2 :
        produktion();
        break;
    case 3 :
        fehler();
        break;
    case 4 :
        ausschaltfehler();
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::poweron */
/* EasyCODE F */
// Hauptaktionen:
//     - Power on fuer Bedienfeld
//     - Bearbeitung Fatal-Fehler (faterror)
//     - verschiedene Selbsttests
//     - Power on fuer Eingabe
//     - Power on fuer Ablauf

void KlasseKoordinator::poweron (void)
{
    unsigned int i;
    unsigned char *bootptr;
    switch ( poweronkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        poweronkey = 0,   Wartezeit nach Break starten */
        zeit.set_btrcounten();
        RTKDelay (1);
        com2breakoff ();
        tkoor1.rsz ();
        tkoor1.sz (30);
        poweronkey = 30;
        break;
        /* EasyCODE ) */
    case 30 :
        /* EasyCODE ( 0 
        poweronkey = 30,   Baudrate Bedienfeld auf 115200 Baud umstellen */
        if ( !koor_botsch1.busycheck() && tkoor1.tz() &&
             ((ModemStatus(COM2)&CTS) == CTS) )
        {
            bdfbreakrdy = TRUE;
            /* EasyCODE - */
            /* EasyCODE_STRUCTURED_COMMENT_START */
            /*
            koor_botsch1.senden (ESCS "Xc8");
            poweronkey = 31;
            EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
            */
            /* EasyCODE_STRUCTURED_COMMENT_END */
            bdfbaudrdy = TRUE;
            keyb.bed_version_gueltig = FALSE;
            koor_botsch1.senden(VERSION);
            tkoor1.rsz();
            tkoor1.sz(100);
            poweronkey = 16;
        }
        break;
        /* EasyCODE ) */
    case 31 :
        /* EasyCODE ( 0 
        poweronkey = 31,   Warten bis Befehl gesendet, Reaktionsdelay starten */
        if ( !koor_botsch1.busycheck() )
        {
            tkoor1.rsz ();
            tkoor1.sz (200);
            poweronkey = 32;
        }
        break;
        /* EasyCODE ) */
    case 32 :
        /* EasyCODE ( 0 
        poweronkey = 32,   Baudrate COM2 auf 115200 Baud umstellen */
        if ( !koor_botsch1.busycheck() && tkoor1.tz() )
        {
            InitPort (COM2,115200,PARITY_ODD,1,8);
            SetProtocol (COM2,RTSCTS,2,1);  // HW Handshake enable
            EnableFIFO (COM2,0);            // 0 = FIFO buffer disabled.
            AllocateCOMBuffers (COM2,RWLAENGE,RWLAENGE); // Allocate receive and send buffers for the port
            EnableCOMInterrupt (COM2,1);
            bdfbaudrdy = TRUE;
            if ( bdf_load )
            {
                poweronkey = 1;
            }
            else
            {
                poweronkey = 5;
            }
        }
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        poweronkey = 1,    Piktogramme laden */
        if ( !koor_botsch1.busycheck() )
        {
            if ( bdf_load )
            {
                bdf_load = FALSE;
                koor_botsch1.senden (delkennungs);
            }
            else
            {
                screen.download (PIKTOLOAD);
                poweronkey = 2;
            }
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        poweronkey = 2,   Piktogramme laden Forts. / Bildschirmvorlagen laden */
        switch ( screen.download_check() )
        {
        case 1 :
            screen.download (VORLAGENLOAD);
            poweronkey = 3;
            break;
        case 2 :
            bdferror = 1;   // Fehler beim Downladen
            poweronkey = 20;
            break;
        case 3 :
            bdferror = 2;   // kein Ack vom Touch
            poweronkey = 20;
            break;
        default:
            break;
        }
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        poweronkey = 3,   Bildschirmvorlagen laden Forts. */
        switch ( screen.download_check() )
        {
        case 1 :
            poweronkey = 4;
            break;
        case 2 :
            bdferror = 1;   // Fehler beim Downladen
            poweronkey = 20;
            break;
        case 3 :
            bdferror = 2;   // kein Ack vom Touch
            poweronkey = 20;
            break;
        default:
            break;
        }
        break;
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        poweronkey = 4,   Bedienfeld-Kennung schreiben */
        if ( !koor_botsch1.busycheck () )
        {
            koor_botsch1.senden (kennungschreibens);
            poweronkey = 5;
        }
        break;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        poweronkey = 5,   Bedienfeld-Kennung lesen */
        if ( !koor_botsch1.busycheck() )
        {
            keyb.bed_kennung_gueltig = FALSE;
            koor_botsch1.senden (kennunglesens);
            poweronkey = 6;
        }
        break;
        /* EasyCODE ) */
    case 6 :
        /* EasyCODE ( 0 
        poweronkey = 6,   Bedienfeld-Kennung pruefen */
        if ( keyb.bed_kennung_gueltig && !koor_botsch1.busycheck() )
        {
            if ( strnicmp (KENNUNG, keyb.bed_kennung, 8)==0 )
            {
                /* EasyCODE_STRUCTURED_COMMENT_START */
                /*
                keyb.bed_version_gueltig = FALSE;
                koor_botsch1.senden(VERSION);
                tkoor1.rsz();
                tkoor1.sz(100);
                poweronkey = 16;
                EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                */
                /* EasyCODE_STRUCTURED_COMMENT_END */
                poweronkey = 7;
            }
            else
            {
                bdf_load = TRUE;
                poweronkey=1;
            }
        }
        break;
        /* EasyCODE ) */
    case 16 :
        /* EasyCODE ( 0 
        poweronkey = 16,   Bedienfeldtyp bestimmen */
        if ( keyb.bed_version_gueltig && !koor_botsch1.busycheck() )
        {
            bdfbaudrdy = FALSE;
            if ( keyb.bed_version[6] == '5' )
            {
                if ( keyb.bed_version[7] != '2' )
                {
                    bdf = PANEL;
                    //koor_botsch1.senden (CLRHOME);
                    //poweronkey=7;
                }
                else
                {
                    bdf = PANELPC;
                    //koor_botsch1.senden (CLRHOME);
                    //poweronkey=7;
                }
            }
            else
            {
                bdf = TOUCH;
                koor_botsch1.senden (CLRHOME);
                //poweronkey=7;
            }
            koor_botsch1.senden (CLRHOME ESCS "Xc8");
            poweronkey = 31;
        }
        else
        {
            if ( tkoor1.tz() && !koor_botsch1.busycheck() )
            {
                /* EasyCODE_STRUCTURED_COMMENT_START */
                /*
                bdf = TOUCH;
                koor_botsch1.senden (CLRHOME);
                poweronkey=7;
                EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                */
                /* EasyCODE_STRUCTURED_COMMENT_END */
                poweronkey = 30;
            }
        }
        break;
        /* EasyCODE ) */
    case 7 :
        /* EasyCODE ( 0 
        poweronkey = 7,   anstehender textload_err oder faterror ? */
        if ( textload_err!=0 )
        {
            poweronkey = 20;
        }
        else
        {
            if ( saf->faterror==0 )
            {
                poweronkey = 21;
            }
            else
            {
                freskey = 0;
                poncontkey = 21;
                poweronkey = 8;
            }
        }
        break;
        /* EasyCODE ) */
    case 8 :
        /* EasyCODE ( 0 
        poweronkey = 8,   Bearbeitung anstehender faterror */
        if ( faterror_res() )
        {
            poweronkey=poncontkey;
        }
        break;
        /* EasyCODE ) */
    case 9 :
        /* EasyCODE ( 0 
        poweronkey = 9,   Init Hauptantriebs-Interface, CAN-Einstellungen, CAN-Reset,
                          Wartezeit CAN starten, Enable SM-Endst., Wartezeit SM-Endst. starten */
        nminf.winit ();
        CAN->maxknotennr=ANZ_KNOTEN;
        for ( i=0; i<ANZ_KNOTEN; i++ )
        {
            CAN->choppermask[i] = saf->chopmask[i];
            CAN->startzt[i] = saf->chopstartzt;
            CAN->auszt[i] = saf->chopauszt;
            CAN->einzt[i] = saf->chopeinzt;
        }
        CAN->reset=0xaa;
        tkoor2.rsz();
        tkoor2.sz(200);
        smx.Fkt1_on();  // Enable SM-Endst.
        smy.Fkt1_on();  // Enable SM-Endst.
        smz.Fkt1_off();  // Enable (Strom ein) SM-Endst. Z
        iostopflag = FALSE;
        tkoor1.rsz();
        tkoor1.sz(saf->sm_ontime);
        poweronkey=10;
        break;
        /* EasyCODE ) */
    case 10 :
        /* EasyCODE ( 0 
        poweronkey = 10,   Tests: CAN-Reset, Anzahl Knoten, Batterie */
        if ( CAN->reset!=0x55 )
        {
            if ( tkoor2.tz() )
            {
                saf->faterror=10;
                poweronkey=20;
            }
        }
        else
        {
            #if !defined(buerotest)
                if ( tkoor2.tz() )
                {
                    switch ( CAN->anzknoten )
                    {
                    case ANZ_KNOTEN :
                        break;
                    default:
                        saf->faterror=11;
                        poweronkey=20;
                        return ;
                    }
                }
                else
                {
                    return ;
                }
            #endif 
            /* EasyCODE < */
            CAN->akterror = 0xff;
            CAN->error = 0xff;
            CAN->errorstat = 0;
            CAN->chmakt=0;
            /* EasyCODE > */
            if ( battok() )
            {
                poweronkey=11;
            }
            else
            {
                saf->faterror=8;
                poncontkey = 11;
                poweronkey=8;
            }
        }
        break;
        /* EasyCODE ) */
    case 11 :
        /* EasyCODE ( 0 
        poweronkey = 11,   Power On Eingabe */
        if ( eing.ponready() )
        {
            eingstopflag=TRUE;
            poweronkey=12;
        }
        else
        {
            eingstopflag=FALSE;
        }
        break;
        /* EasyCODE ) */
    case 12 :
        /* EasyCODE ( 0 
        poweronkey = 12,   Hauptantrieb */
        #ifdef buerotest
            poweronkey=13;
        #else 
            if ( nminf.get_ponerr() )
            {
                saf->faterror=12;
                poweronkey=20;
            }
            else
            {
                if ( nminf.get_ponrdy() )
                {
                    poweronkey=13;
                }
            }
        #endif 
        break;
        /* EasyCODE ) */
    case 13 :
        /* EasyCODE ( 0 
        poweronkey = 13,   Warten bis Schrittmotoren bereit */
        if ( tkoor1.tz() ||
             (!(smx.Eing()&EING1MSK) && !(smy.Eing()&EING1MSK) && !(smz.Eing()&EING1MSK)) )
        {
            poweronkey=14;
        }
        break;
        /* EasyCODE ) */
    case 14 :
        /* EasyCODE ( 0 
        poweronkey = 14,   Power on der Ablaeufe */
        iostopflag=FALSE;
        if ( !(notstop_ok() && error_ok()) )
        {
            errkey = 0;
            key = 3;
        }
        else
        {
            if ( nae.ponready() )
            {
                naestopflag = TRUE;
                if ( nae.stop() )
                {
                    /* EasyCODE < */
                    ponrdy = TRUE;
                    eing.auftrag = 1;
                    key = 1;  // Eingabe
                    /* EasyCODE > */
                }
            }
            else
            {
                if ( eing.auftrag == 0 )
                {
                    naestopflag = FALSE;
                }
                else
                {
                    naestopflag = TRUE;
                    poweronkey = 15;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 15 :
        /* EasyCODE ( 0 
        poweronkey = 15,   Wechsel in die Eingabe waehrend Power On */
        if ( !(notstop_ok() && error_ok()) )
        {
            /* EasyCODE < */
            poweronkey = 14;
            errkey = 0;
            key = 3; // Fehler
            /* EasyCODE > */
        }
        else
        {
            if ( nae.stop() )
            {
                /* EasyCODE < */
                poweronkey = 14;
                eingkey = 0;
                key = 1;  // Eingabe
                /* EasyCODE > */
            }
        }
        break;
        /* EasyCODE ) */
    case 20 :
        /* EasyCODE ( 0 
        poweronkey = 20,   fataler Ausschaltfehler */
        if ( !koor_botsch1.busycheck() && !koor_botsch2.busycheck() )
        {
            if ( bdferror != 0 )
            {
                sprintf (varstr1, errs, 500+bdferror);
                koor_botsch2.senden (varstr1);
            }
            else
            {
                koor_botsch1.senden (BV001);
                if ( textload_err!=0 )
                {
                    sprintf (varstr1, errs, 400+textload_err);
                    koor_botsch2.senden (varstr1);
                }
                else
                {
                    sprintf (varstr1, tz->errs, saf->faterror);
                    koor_botsch2.senden (varstr1);
                    saf->faterror=0;
                }
            }
            auskey = 0;
            key = 4;
        }
        break;
        /* EasyCODE ) */
    case 21 :
        /* EasyCODE ( 0 
        poweronkey = 21,   EMS-Speicher allokieren */
        #ifdef remote
            /* EasyCODE < */
            // Beim Debuggen vorsichtshalber handles freigeben, da bei
            // der Funktion Programm Reset die handles nicht automatisch
            // freigegeben werden.
            // Es werden hier einfach die handles 1 bis PNRINDEX_MAX+1
            // freigegeben, ohne die eigentlich erforderliche Pruefung usw.
            /* EasyCODE > */
            for ( i=1; i<=PNRINDEX_MAX+1; i++ )
            {
                ems_free(i);
            }
        #endif remote
        for ( i=0; i<=PNRINDEX_MAX; i++ )
        {
            if ( (prog_handle[i] = ems_alloc(4)) == EMS_ERR )
            {
                saf->faterror = 3;
                poweronkey=20;
            }
            else
            {
                ndatanf = (char *) MK_FP (emsseg,0);
                poweronkey=22;
            }
        }
        break;
        /* EasyCODE ) */
    case 22 :
        /* EasyCODE ( 0 
        poweronkey = 22,   C167 starten */
        if ( !koor_botsch1.busycheck() && !koor_botsch2.busycheck() )
        {
            koor_botsch1.senden (powerons);
            sprintf (koor.varstr1, poweron2s, softversion);
            koor_botsch2.senden (koor.varstr1);
            if ( saf->md.matdicke == DUENN )
            {
                bootptr = bs3version;
            }
            else
            {
                bootptr = bs3version_schwer;
            }
            switch ( start167(bootptr) )
            {
            case 0 :
                // richtige Version im Flash
                poweronkey = 9;
                break;
            case 1 :
                // falsche Version im Flash
                poweronkey=23;
                break;
            case 2 :
                // C167 reagiert nicht
                saf->faterror = 4;
                poweronkey=20;
                break;
            case 3 :
                // Bootdatei kann nicht geoeffnet werden
                saf->faterror = 5;
                poweronkey=20;
                break;
            }
        }
        break;
        /* EasyCODE ) */
    case 23 :
        /* EasyCODE ( 0 
        poweronkey = 23,   Anzeige: BV001 Firmware Download */
        if ( !koor_botsch1.busycheck() && !koor_botsch2.busycheck() )
        {
            koor_botsch1.senden (BV002);
            koor_botsch2.senden (tz->koor2);
            poweronkey = 24;
        }
        break;
        /* EasyCODE ) */
    case 24 :
        /* EasyCODE ( 0 
        poweronkey = 24,   Anzeige: Sanduhr starten */
        if ( !koor_botsch1.busycheck() && !koor_botsch2.busycheck() )
        {
            RTKSetPriority (screen.handle, KOOR_PRIOR+1);
            screen.sanduhrreq_set(160, 140);
            poweronkey = 25;
        }
        break;
        /* EasyCODE ) */
    case 25 :
        /* EasyCODE ( 0 
        poweronkey = 25,   Bs3-Firmware downladen */
        if ( !koor_botsch1.busycheck() && !koor_botsch2.busycheck() )
        {
            switch ( progflash() )
            {
            case 0 :
                // ok
                RTKSetPriority (screen.handle, SCREEN_PRIOR);
                saf->bs3download=FALSE;
                poweronkey=22;
                break;
            case 1 :
                /* EasyCODE = */
            case 2 :
                // Fehler beim Flash-Programmieren
                RTKSetPriority (screen.handle, SCREEN_PRIOR);
                saf->faterror=6;
                poweronkey=20;
                break;
            case 3 :
                /* EasyCODE = */
            case 4 :
                /* EasyCODE = */
            case 5 :
                // Fehler beim Oeffnen einer Datei
                RTKSetPriority (screen.handle, SCREEN_PRIOR);
                saf->faterror=7;
                poweronkey=20;
                break;
            case 6 :
                // C167 reagiert nicht
                RTKSetPriority (screen.handle, SCREEN_PRIOR);
                saf->faterror=4;
                poweronkey=20;
                break;
            }
        }
        break;
        /* EasyCODE ) */
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::eingabe */
/* EasyCODE F */
// Hauptaktionen:
//    - Notstop- und Fehlerpr+fung
//    - Handshake mit Eingabe und "Ausg+nge schalten"
//    - Betriebsartenwechsel

void KlasseKoordinator::eingabe (void)
{
    if ( !(notstop_ok() && error_ok()) )
    {
        errkey = 0;
        key=3;
    }
    else
    {
        switch ( eingkey )
        {
        case 0 :
            hbtartb = EINGABE;
            iostopflag=TRUE;
            eingkey = 1;
            break;
        case 1 :
            /* EasyCODE < */
            // Warten bis io steht
            // Eingabe laufen lassen
            /* EasyCODE > */
            if ( io.stop() )
            {
                eingstopflag=FALSE;
                eingkey=2;
            }
            break;
        case 2 :
            // Eingabe fertig ?
            // io3590 stoppen bzw. laufen lassen
            if ( eing.auftrag == 0 )
            {
                eingstopflag=TRUE;
                eingkey=3;
            }
            else
            {
                if ( eing.ioact )
                {
                    iostopflag=FALSE;
                }
                else
                {
                    iostopflag=TRUE;
                }
            }
            break;
        case 3 :
            /* EasyCODE < */
            // Warten bis Eingabe steht
            /* EasyCODE > */
            if ( eing.stop() && !koor_botsch1.busycheck() )
            {
                if ( ponrdy )
                {
                    hbtartb = PRODUKTION;
                    prodkey = 0;
                    key = 2;
                }
                else
                {
                    koor_botsch1.senden (CLRHOME);
                    hbtartb = POWER_ON;
                    key=0;
                }
            }
            break;
        default:
            break;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::produktion */
/* EasyCODE F */
// Hauptaktionen:
//     - Notstop- und Fehlerpruefung
//     - Handshake mit Ablaeufen und "Ausgaenge schalten"
//     - Betriebsartenwechsel

void KlasseKoordinator::produktion (void)
{
    if ( !(notstop_ok() && error_ok()) )
    {
        errkey=0;
        key = 3;
    }
    else
    {
        switch ( prodkey )
        {
        case 0 :
            // ioportal laufen lassen
            hbtartb = PRODUKTION;
            iostopflag = FALSE;
            naestopflag = FALSE;
            prodkey = 1;
            break;
        case 1 :
            if ( eing.auftrag != 0 )
            {
                naestopflag = TRUE;
                prodkey = 2;
            }
            break;
        case 2 :
            /* EasyCODE < */
            // Betriebsartenwechsel in Eingabe
            /* EasyCODE > */
            if ( nae.stop() )
            {
                eingkey=0;
                key=1;
            }
            break;
        default:
            break;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::fehler */
/* EasyCODE F */
// Hauptaktionen:
//     - Notstop- und Fehlerpr+fung
//     - Fehleranzeige, Display bearbeiten
//     - Entst¥raktionen
//     - Handshake mit Abl+ufen und "Ausg+nge schalten"
//     - Betriebsartenwechsel

void KlasseKoordinator::fehler (void)
{
    switch ( errkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        errkey = 0,   Ablaeufe stoppen, io3590 laufen lassen bzw. stoppen */
        notstop_ok();
        naestopflag=TRUE;
        eingstopflag=TRUE;
        if ( nae.stop() && eing.stop() )
        {
            iostopflag=TRUE;
            errkey=1;
        }
        else
        {
            iostopflag=FALSE;
        }
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        errkey = 1,   BV001 anzeigen, Verzweigen auf Grund des Fehlers */
        notstop_ok();
        if ( io.stop() && !koor_botsch1.busycheck() && !koor_botsch2.busycheck() )
        {
            koor_botsch1.senden (BV001);
            koor_botsch2.senden (eings);
            if ( koor_err!=0 )
            {
                errkey=2;
            }
            else
            {
                if ( nae_err != 0 )
                {
                    errkey=3;
                }
                else
                {
                    if ( io_err!=0 )
                    {
                        errkey=6;
                    }
                    else
                    {
                        if ( eing_err!=0 )
                        {
                            errkey=7;
                        }
                    }
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        errkey = 2,   Fehlerbearbeitung Koordinatorfehler koor_err */
        if ( !koor_botsch1.busycheck() )
        {
            if ( koor_err ==2 )
            {
                /* EasyCODE ( 0 
                CAN */
                sprintf (varstr1, tz->errs, 100+koor_err);
                sprintf (varstr2, errpar3s, CAN->error, CAN->errorstat);
                strcat (varstr1, varstr2);
                koor_botsch1.senden (varstr1);
                auskey = 0;
                key = 4;   // Ausschaltfehler
                koor_err = 0;
                CAN->error = 0xff;
                /* EasyCODE ) */
            }
            else
            {
                if ( koor_err < 50 )
                {
                    /* EasyCODE ( 0 
                    C167, SmX, SmY, Druckluft */
                    sprintf (varstr1, tz->errs, 100+koor_err);
                    koor_botsch1.senden (varstr1);
                    if ( koor_err == 1 )
                    {
                        // C167
                        auskey = 0;
                        key = 4;   // Ausschaltfehler
                        koor_err = 0;
                    }
                    else
                    {
                        koor_err = 0;
                        errkey = 8;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    koor_err >= 50 */
                    koor_botsch1.senden (tz->koorerrs[koor_err-50]);
                    auskey = 0;
                    key = 4;   // Ausschaltfehler
                    koor_err = 0;
                    /* EasyCODE ) */
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        errkey = 3,   Fehlerbearbeitung Naehen nae_err */
        if ( !koor_botsch1.busycheck() && !koor_botsch2.busycheck() )
        {
            if ( nae_err <= 10 )
            {
                /* EasyCODE ( 0 
                Fehler mit
                1 Parameter */
                if ( (nae_err == 1) || (nae_err == 10) )
                {
                    sprintf (varstr1, tz->errs, 200+nae_err);
                    sprintf (varstr2, errpar2s, nae.errpar, nminf.lastbef);
                    strcat (varstr1, varstr2);
                    koor_botsch1.senden (varstr1);
                    nminf.fehler = 0;
                }
                else
                {
                    sprintf (varstr1, tz->errs, 200+nae_err);
                    sprintf (varstr2, errpars, nae.errpar);
                    strcat (varstr1, varstr2);
                    koor_botsch1.senden (varstr1);
                    schabl.rf.rfid_err = 0;
                }
                /* EasyCODE ) */
            }
            else
            {
                if ( nae_err < 40 )
                {
                    /* EasyCODE ( 0 
                    Fehler ohne
                    Parameter */
                    if ( nae_err == 22 )
                    {
                        sprintf (varstr1, errpiktos, PBARCODE);
                    }
                    else
                    {
                        sprintf (varstr1, tz->errs, 200+nae_err);
                        if ( nae_err == 17 )
                        {
                            sprintf (varstr2, errpiktos, PNOCLAMP);
                            strcat (varstr1, varstr2);
                        }
                    }
                    koor_botsch1.senden (varstr1);
                    /* EasyCODE ) */
                }
                else
                {
                    if ( nae_err < 50 )
                    {
                        /* EasyCODE ( 0 
                        Verriegelungs-Fehler
                        Ursache wird angezeigt */
                        sprintf (varstr1, tz->errs, 200+nae_err);
                        koor_botsch1.senden (varstr1);
                        secerranz (nae.errpar);
                        /* EasyCODE ) */
                    }
                    else
                    {
                        /* EasyCODE ( 0 
                        Fehler die als Text
                        angezeigt werden */
                        koor_botsch1.senden (tz->naeerrs[nae_err-50]);
                        /* EasyCODE ) */
                    }
                }
            }
            nae_err=0;
            errkey = 8;
        }
        break;
        /* EasyCODE ) */
    case 6 :
        /* EasyCODE ( 0 
        errkey = 6,   Fehlerbearbeitung Ausgaenge schalten io_err */
        if ( !koor_botsch1.busycheck() && !koor_botsch2.busycheck() )
        {
            sprintf (varstr1,tz->ioerr_urs, ioerrs[io_err-1]);
            koor_botsch1.senden (varstr1);
            secerranz (io_secerr);
            io_err=0;
            io_secerr=0;
            errkey = 8;
        }
        break;
        /* EasyCODE ) */
    case 7 :
        /* EasyCODE ( 0 
        errkey = 7,   Fehlerbearbeitung Eingabe eing_err */
        if ( !koor_botsch1.busycheck() )
        {
            if ( eing_err < 40 )
            {
                if ( eing_err != 30 )
                {
                    /* EasyCODE ( 0 
                    Fehler ohne
                    Parameter */
                    sprintf (varstr1, tz->errs, 300+eing_err);
                    koor_botsch1.senden (varstr1);
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Fehler mit
                    2 Parametern */
                    sprintf (varstr1, tz->errs, 300+eing_err);
                    sprintf (varstr2, errpar2s, eing.errpar, eing.errpar2);
                    strcat (varstr1, varstr2);
                    koor_botsch1.senden (varstr1);
                    /* EasyCODE ) */
                }
            }
            else
            {
                if ( eing_err < 50 )
                {
                    /* EasyCODE ( 0 
                    Fehler mit
                    1 Parameter */
                    sprintf (varstr1, tz->errs, 300+eing_err);
                    sprintf (varstr2, errpars, eing.errpar);
                    strcat (varstr1, varstr2);
                    koor_botsch1.senden (varstr1);
                    if ( eing_err == 41 )
                    {
                        nminf.fehler = 0;
                    }
                    else
                    {
                        schabl.rf.rfid_err = 0;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Fehler die als Text
                    angezeigt werden */
                    switch ( eing_err )
                    {
                    case 50 :
                        sprintf (varstr1, tz->eingerrs[eing_err-50], eing.errpar);
                        koor_botsch1.senden (varstr1);
                        break;
                    case 51 :
                        koor_botsch1.senden (tz->eingerrs[eing_err-50]);
                        auskey = 0;
                        key = 4;   // Ausschaltfehler
                        koor_err = 0;
                        break;
                    default:
                        koor_botsch1.senden (tz->eingerrs[eing_err-50]);
                        break;
                    }
                    /* EasyCODE ) */
                }
            }
            eing_err = 0;
            errkey = 8;
        }
        break;
        /* EasyCODE ) */
    case 8 :
        /* EasyCODE ( 0 
        errkey = 8,   Warten bis keine Taste betaetigt */
        if ( keyb.taste() == 0xff )
        {
            errkey = 9;
        }
        break;
        /* EasyCODE ) */
    case 9 :
        /* EasyCODE ( 0 
        errkey = 9,   Entstoeraktion, weiterer Fehler?,  Betriebsartenwechsel */
        if ( !koor_botsch1.busycheck() )
        {
            if ( keyb.taste() == TERROR )
            {
                notstop_ok();
                if ( error_ok() )
                {
                    switch ( hbtart() )
                    {
                    case EINGABE :
                        eingkey = 0;
                        key = 1;
                        break;
                    case POWER_ON :
                        koor_botsch1.senden (CLRHOME);
                        key = 0;
                        break;
                    case PRODUKTION :
                        prodkey = 0;
                        key = 2;
                        break;
                    }
                    notstopflag=FALSE;
                }
                else
                {
                    errkey=1;
                }
            }
            else
            {
                if ( keyb.taste() == TEINGABE )
                {
                    notstop_ok();
                    if ( error_ok() )
                    {
                        notstopflag=FALSE;
                        eing.auftrag = 2;
                        eing.ioact = FALSE;
                        eingkey = 0;
                        key = 1;
                    }
                    else
                    {
                        errkey=1;
                    }
                }
            }
        }
        break;
        /* EasyCODE ) */
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::ausschaltfehler */
/* EasyCODE F */
// Nach einem schweren Fehler mu+ die Maschine kurz ausgeschaltet werden.
// - entsprechende Meldung in der letzten Zeile
// - bei Entst¥rversuchen wird gehupt und die Meldung kurz invers angezeigt

void KlasseKoordinator::ausschaltfehler (void)
{
    switch ( auskey )
    {
    case 0 :
        if ( !koor_botsch1.busycheck() )
        {
            if ( (textload_err!=0) || (bdferror != 0) )
            {
                koor_botsch1.senden (ausschalts);
            }
            else
            {
                koor_botsch1.senden (tz->ausschalts);
            }
            auskey = 1;
        }
        break;
    case 1 :
        if ( keyb.taste() == 0xff )
        {
            auskey = 2;
        }
        break;
    case 2 :
        if ( !koor_botsch1.busycheck() && !koor_botsch2.busycheck() && (keyb.taste() == TERROR) )
        {
            koor_botsch1.senden (ION);
            if ( (textload_err!=0) || (bdferror != 0) )
            {
                koor_botsch1.senden (ausschalts);
            }
            else
            {
                koor_botsch2.senden (tz->ausschalts);
            }
            tkoor1.rsz ();
            tkoor1.sz (25);
            auskey = 3;
        }
        break;
    case 3 :
        if ( !koor_botsch1.busycheck() && tkoor1.tz() )
        {
            koor_botsch1.senden (IOFF);
            auskey = 0;
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::in_image_up */
/* EasyCODE F */
void KlasseKoordinator::in_image_up (void)
{
    unsigned char i;
    for ( i=0; i<ANZ_KNOTEN; i++ )
    {
        in_image[i] = CAN->in[i] ^ saf->exormatrix[i];
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::notstop_ok */
/* EasyCODE F */
/********** notstop_ok - +berpr+ft Notstopbedingungen **********/
// +berpr+ft werden:
//     -
//     -
//     -
//     -
//     - BS3 (watchdog und Fehler)
// Meldung:
//     TRUE ...... kein Fehler
//     FALSE ..... Fehler (koor_err entsprechend gesetzt)

unsigned char KlasseKoordinator::notstop_ok (void)
{
    if ( C167_ERR!=0 )
    {
        koor_err=1;
    }
    else
    {
        if ( (c167_cnt==0) && (C167_WATCH!=0xff) )
        {
            koor_err=1;
            if ( (inportb(STATIN) & RESET_MSK)!=0 )
            {
                outportb (RES167OUT,RESDATA1);
                outportb (RES167OUT,RESDATA2);
                outportb (RES167OUT,RESDATA3);
            }
        }
        else
        {
            /* EasyCODE ( 0 
            watchdog ruecksetzen */
            if ( c167_cnt==0 )
            {
                /* EasyCODE < */
                c167_cnt=9;
                /* EasyCODE > */
                #if !defined(remote)
                    /* EasyCODE < */
                    C167_WATCH=0;
                    /* EasyCODE > */
                #endif 
            }
            else
            {
                c167_cnt--;
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Can, Schrittmotoren, Druckluft */
            #if defined(buerotest)
                return TRUE;
            #else 
                if ( CAN->error!=0xff )
                {
                    koor_err=2;
                }
                else
                {
                    if ( smx.Eing() & EING1MSK )
                    {
                        koor_err=3;
                    }
                    else
                    {
                        if ( smy.Eing() & EING1MSK )
                        {
                            koor_err=4;
                        }
                        else
                        {
                            if ( smz.Eing() & EING1MSK )
                            {
                                koor_err=6;
                            }
                            else
                            {
                                if ( !press.getZustand() && !iostopflag )
                                {
                                    // Fehler Druckluft
                                    koor_err = 5;
                                }
                                else
                                {
                                    return TRUE;
                                }
                            }
                        }
                    }
                }
            #endif 
            /* EasyCODE ) */
        }
    }
    notstopflag=TRUE;
    naestopflag=TRUE;
    iostopflag=TRUE;
    eingstopflag=TRUE;
    return FALSE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::error_ok */
/* EasyCODE F */
// Meldung:
//    TRUE ..... kein Fehler (alle gepr+ften Fehlerbytes = 0)
//    FALSE .... Fehler (mindestens ein gepr+ftes Fehlerbyte >< 0)

unsigned char KlasseKoordinator::error_ok (void)
{
    if ( (koor_err | nae_err | io_err | eing_err) == 0 )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::secerranz */
/* EasyCODE F */
// Eingangsparameter:
//      Unterfehlernummer
//       falls Unterfehlernummer=0, wird keine Anzeige durchgef+hrt
// Der sprachabh+ngige, feste Text "URSACHE: " wird entsprechend der Unterfehler-
// nummer sprachunabh+ngig erg+nzt und +ber die Botschaft koor_botsch2 gesendet.
// Die Pr+fung, dass koor_botsch2 frei ist, muss vom Aufrufer erledigt werden.
// Der variable String wird in varstr2 aufgebaut.

void KlasseKoordinator::secerranz (unsigned char secerr)
{
    if ( secerr!=0 )
    {
        sprintf (varstr2,tz->secerr_urs, secerrs[secerr-1]);
        koor_botsch2.senden (varstr2);
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::faterror_res */
/* EasyCODE F */
/********** faterror_res - Bearbeitung behebbarer Fatal-Fehler **********/
// - entsprechende Fehlermeldung auf dem Display
// - faterror ruecksetzen
// - nach Entstoeren wird Display komplett geloescht
// Meldung:
//     TRUE ..... fertig
//     FALSE ... noch nicht fertig

unsigned char KlasseKoordinator::faterror_res (void)
{
    switch ( freskey )
    {
    case 0 :
        if ( !koor_botsch1.busycheck() && !koor_botsch2.busycheck() )
        {
            koor_botsch1.senden (BV001);
            if ( saf->faterror < 50 )
            {
                sprintf (varstr1, tz->errs, saf->faterror);
                koor_botsch2.senden (varstr1);
            }
            else
            {
                koor_botsch2.senden (tz->faterrors[saf->faterror-50]);
            }
            saf->faterror=0;
            freskey=1;
        }
        break;
    case 1 :
        if ( keyb.taste() == 0xff )
        {
            freskey=2;
        }
        break;
    case 2 :
        if ( !koor_botsch1.busycheck() && (keyb.taste() == TERROR) )
        {
            koor_botsch1.senden (CLRHOME);
            return TRUE;
        }
        else
        {
            break;
        }
    default:
        break;
    }
    return FALSE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::kinit */
/* EasyCODE F */
void KlasseKoordinator::kinit (void)
{
    bdf_load=TRUE;
    saf->bs3download = TRUE;
    //saf->exormatrix[0]=0x0000; //nur zum Test xxx
    saf->exormatrix[0]=0x0800; // 0 bei Schlie+er, 1 bei Oeffner
    saf->exormatrix[1]=0x0000; // 0 bei Schlie+er, 1 bei Oeffner
    saf->chopmask[0]=0x0000; // Choppermasken Can-Knoten
    saf->chopmask[1]=0x0000; // Choppermasken Can-Knoten
                             //   (Bit gesetzt -> choppern)
    saf->chopstartzt=1000; // Start-Zeit fuer Chopper bei CAN (in ms)
    saf->chopauszt=5;      // Aus-Zeit fuer Chopper bei CAN (in ms, Achtung: nur Byte)
    saf->chopeinzt=5;      // Ein-Zeit fuer Chopper bei CAN (in ms, Achtung: nur Byte)
    saf->c167_timeout=500;   // Timeout beim Booten des C167
    saf->delflash_zt=3000;   // max. Zeit zum Loeschen des Flashs des C167
    saf->sm_ontime=100;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseKoordinator::winit */
/* EasyCODE F */
void KlasseKoordinator::winit (void)
{
    naestopflag=TRUE;
    iostopflag=TRUE;
    eingstopflag=TRUE;
}

/* EasyCODE ) */
/* EasyCODE ) */
