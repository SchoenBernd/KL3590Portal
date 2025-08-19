/* EasyCODE V8 */
/* EasyCODE ( 0 
naeport, Naehen */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
// Datei:          naeport
// Klasse:         Kl3590 Portal
// Ersteller:      Halfmann   EWE 2856

#include "portal.h"

KlasseNaehen nae;
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaehen::verw */
/* EasyCODE F */
void KlasseNaehen::verw (void)
{
    switch ( key )
    {
    case 0 :
        zeit.res_prodcounten();
        stopzustand();
        break;
    case 1 :
        poweron();
        break;
    case 2 :
        zeit.res_prodcounten();
        warten();
        break;
    case 3 :
        zeit.set_prodcounten();
        nup.progstat();
        ablauf();
        break;
    case 4 :
        zeit.res_prodcounten();
        home();
        break;
    case 5 :
        zeit.res_prodcounten();
        fwd();
        break;
    case 6 :
        zeit.res_prodcounten();
        back();
        break;
    case 7 :
        zeit.res_prodcounten();
        faedel();
        break;
    case 8 :
        zeit.res_prodcounten();
        find();
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
unsigned char welcherFadenWechsel = 0;
/* EasyCODE ( 0 
KlasseNaehen::stopzustand */
/* EasyCODE F */
void KlasseNaehen::stopzustand (void)
{
    if ( (koor.hbtart() == PRODUKTION) && y1.getZustand() )
    {
        y1.resAuftrag(); // Blasluft ausschalten
    }
    if ( koor.naestop() )
    {
        stopb = TRUE;
        abl.set_anzauftrag(0);
    }
    else
    {
        stopb = FALSE;
        if ( koor.nae_err != 0 )
        {
        }
        else
        {
            if ( ponrdy )
            {
                if ( saf->md.jigctren != 2 )
                {
                    nup.getprpar();
                }
                if ( homereq )
                {
                    homekey = 0;
                    key = 4;
                }
                else
                {
                    if ( saf->mnp_gueltig!=0x55 )
                    {
                        eing.auftrag = 1;
                    }
                    else
                    {
                        if ( faedelreq )
                        {
                            faedelkey = 0;
                            key = 7;
                        }
                        else
                        {
                            if ( findreq )
                            {
                                findkey = 0;
                                key = 8;
                            }
                            else
                            {
                                wartenkey = 0;
                                key = 2;
                            }
                        }
                    }
                }
            }
            else
            {
                poweronkey = 0;
                key = 1;
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaehen::warten */
/* EasyCODE F */
void KlasseNaehen::warten (void)
{
    switch ( wartenkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        wartenkey = 0,  Einstieg ins Warten */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( (keyb.taste() == 0xff) && abl.anzauftrag_ready() &&
                 !trechts_stop.getZustand() && !tlinks_stop.getZustand() )
            {
                nup.fs1_edge = FALSE;
                nup.fs2_edge = FALSE;
                nup.fs1_merker = FALSE;
                nup.TKLAMMER_edge = FALSE;
                nup.TKLAMMER_merker = FALSE;
                nup.tlinksm = FALSE;
                nup.trechtsm = FALSE;
                nup.clopendi = TRUE;
                nup.autostart_en = FALSE;
                nup.naposact = FALSE;
                stepmerker = FALSE;
                abl.acanz = FALSE;
                abl.redanz = FALSE;
                nup.klammerAufMerker = FALSE; // SW002DIS
                abl.set_anzauftrag(1);
                wartenkey = 1;
                if ( get_barcode() == 0xffff )
                {
                    nup.barcode_lesen = TRUE;
                }
                else
                {
                    nup.barcode_lesen = FALSE;
                }
                nup.barcode_alt = 0xffff;
            }
        }
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        wartenkey = 1,  Warten */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( abl.anzauftrag_ready() )
            {
                switch ( nup.clampstart() )
                {
                case 0 :
                    /* EasyCODE ( 0 
                    Tasten abfragen */
                    switch ( keyb.taste() )
                    {
                    case TEINGABE :
                        eing.auftrag = 2;
                        wartenkey = 10;
                        break;
                    case TPNR :
                        eing.auftrag = 3;
                        wartenkey = 10;
                        break;
                    case TSTI :
                        eing.auftrag = 6;
                        wartenkey = 10;
                        break;
                    case TGSWMAX :
                        eing.auftrag = 7;
                        wartenkey = 10;
                        break;
                    case TAUTO :
                        saf->md.autoen = FALSE;
                        wartenkey = 11;
                        break;
                    case TMAN :
                        saf->md.autoen = TRUE;
                        wartenkey = 11;
                        break;
                    case TCOUNTER :
                        eing.auftrag = 8;
                        wartenkey = 10;
                        break;
                    case TFAEDEL :
                        faedelreq = TRUE;
                        nextfaedelkey = 3;
                        tfaedelstartanz = FALSE;
                        key = 0;
                        break;
                    case TFIND :
                        findreq = TRUE;
                        findkey = 0;
                        key = 0;
                        break;
                    case THOME :
                        homereq = TRUE;
                        homekey = 0;
                        thomeanz = FALSE;
                        key = 4;
                        break;
                    default:
                        break;
                    }
                    nup.foot();
                    nup.progstat();
                    nup.anschlaege();
                    break;
                    /* EasyCODE ) */
                case 1 :
                    /* EasyCODE ( 0 
                    Fehler, koor.nae_err sitzt */
                    key = 0;
                    break;
                    /* EasyCODE ) */
                case 2 :
                    /* EasyCODE ( 0 
                    Klammer kann nicht geoeffnet werden, da
                    der Faden nicht geschnitten ist */
                    koor.nae_err = 16;
                    key = 0;
                    break;
                    /* EasyCODE ) */
                case 3 :
                    /* EasyCODE ( 0 
                    Naehstart */
                    if ( e81.getZustand() )
                    {
                        if ( (saf->md.jigctren == 2) && !nup.carlp )
                        {
                            switch ( nup.read_jigpnr () )
                            {
                            case 0 :
                                if ( pnrmem != saf->md.pnr )
                                {
                                    koor.nae_err = 19;
                                    saf->md.pnr = pnrmem;
                                    key = 0;
                                }
                                else
                                {
                                    // Naehstart
                                    wartenkey = 2;
                                }
                                break;
                            case 1 :
                                // keine Klammer eingelegt,
                                // Programmnummer konnte nicht
                                // gelesen werden
                                koor.nae_err = 17;
                                key = 0;
                                break;
                            case 2 :
                                // Programmnummer ungueltig
                                koor.nae_err = 18;
                                key = 0;
                                break;
                            case 3 :
                                // RFID-Lesen nicht fertig
                                weiterinfo = 0;
                                wartenkey = 4;
                                break;
                            case 4 :
                                // RFID-Fehler
                                koor.nae_err = 2;
                                errpar = schabl.rf.rfid_err;
                                key = 0;
                                break;
                            default:
                                break;
                            }
                        }
                        else
                        {
                            if ( (saf->jigctrf==BARCODE) && (saf->md.jigctren==2) &&
                                 (get_barcode() == 0xffff) )
                            {
                                koor.nae_err = 22;
                                key = 0;
                            }
                            else
                            {
                                // Naehstart
                                if ( nup.carlp && (saf->md.jigctren > 0)  // SW006DIS
                                   )
                                {
                                    schabl.rf.set_rfid_read();
                                }
                                wartenkey = 2;
                            }
                        }
                    }
                    else
                    {
                        // Tisch abgeklappt
                        koor.nae_err = 26;
                        key = 0;
                    }
                    break;
                    /* EasyCODE ) */
                case 4 :
                    /* EasyCODE ( 0 
                    Vorwaerts Takten */
                    if ( e81.getZustand() )
                    {
                        if ( (saf->md.jigctren == 2) && !nup.carlp )
                        {
                            switch ( nup.read_jigpnr () )
                            {
                            case 0 :
                                if ( pnrmem != saf->md.pnr )
                                {
                                    koor.nae_err = 19;
                                    saf->md.pnr = pnrmem;
                                    key = 0;
                                }
                                else
                                {
                                    wartenkey = 5;
                                }
                                break;
                            case 1 :
                                // keine Klammer eingelegt,
                                // Programmnummer konnte nicht
                                // gelesen werden
                                koor.nae_err = 17;
                                key = 0;
                                break;
                            case 2 :
                                // Programmnummer ungueltig
                                koor.nae_err = 18;
                                key = 0;
                                break;
                            case 3 :
                                // RFID-Lesen nicht fertig
                                weiterinfo = 1;
                                wartenkey = 4;
                                break;
                            case 4 :
                                // RFID-Fehler
                                koor.nae_err = 2;
                                errpar = schabl.rf.rfid_err;
                                key = 0;
                                break;
                            default:
                                break;
                            }
                        }
                        else
                        {
                            if ( (saf->jigctrf==BARCODE) && (saf->md.jigctren==2) &&
                                 (get_barcode() == 0xffff) )
                            {
                                koor.nae_err = 22;
                                key = 0;
                            }
                            else
                            {
                                if ( nup.carlp && (saf->md.jigctren > 0)  // SW006DIS
                                   )
                                {
                                    schabl.rf.set_rfid_read();
                                }
                                wartenkey = 5;
                            }
                        }
                    }
                    else
                    {
                        // Tisch abgeklappt
                        koor.nae_err = 26;
                        key = 0;
                    }
                    break;
                    /* EasyCODE ) */
                case 5 :
                    /* EasyCODE ( 0 
                    Rueckwaerts Takten */
                    if ( e81.getZustand() )
                    {
                        if ( (saf->md.jigctren == 2) && !nup.carlp )
                        {
                            switch ( nup.read_jigpnr () )
                            {
                            case 0 :
                                if ( pnrmem != saf->md.pnr )
                                {
                                    koor.nae_err = 19;
                                    saf->md.pnr = pnrmem;
                                    key = 0;
                                }
                                else
                                {
                                    wartenkey = 6;
                                }
                                break;
                            case 1 :
                                // keine Klammer eingelegt,
                                // Programmnummer konnte nicht
                                // gelesen werden
                                koor.nae_err = 17;
                                key = 0;
                                break;
                            case 2 :
                                // Programmnummer ungueltig
                                koor.nae_err = 18;
                                key = 0;
                                break;
                            case 3 :
                                // RFID-Lesen nicht fertig
                                weiterinfo = 2;
                                wartenkey = 4;
                                break;
                            case 4 :
                                // RFID-Fehler
                                koor.nae_err = 2;
                                errpar = schabl.rf.rfid_err;
                                key = 0;
                                break;
                            default:
                                break;
                            }
                        }
                        else
                        {
                            if ( (saf->jigctrf==BARCODE) && (saf->md.jigctren==2) &&
                                 (get_barcode() == 0xffff) )
                            {
                                koor.nae_err = 22;
                                key = 0;
                            }
                            else
                            {
                                if ( nup.carlp && (saf->md.jigctren > 0)  // SW006DIS
                                   )
                                {
                                    schabl.rf.set_rfid_read();
                                }
                                wartenkey = 6;
                            }
                        }
                    }
                    else
                    {
                        // Tisch abgeklappt
                        koor.nae_err = 26;
                        key = 0;
                    }
                    break;
                    /* EasyCODE ) */
                case 7 :
                    /* EasyCODE ( 0 
                    Klammer schliessen, obwohl
                    keine Klammer eingelegt */
                    abl.set_anzauftrag(4);
                    wartenkey = 3;
                    break;
                    /* EasyCODE ) */
                default:
                    break;
                }
                /* EasyCODE ( 0 
                Uebertemperatur */
                if ( ((temp.getZustand() && uebertemp) ||
                     (!temp.getZustand() && !uebertemp)) &&
                     abl.anzauftrag_ready() )
                {
                    abl.set_anzauftrag(10);
                }
                /* EasyCODE ) */
            }
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        wartenkey = 2,   Einstieg ins Naehen */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( (saf->variante == 0) ||
                 (!trechts_stop.getZustand() &&
                  !tlinks_stop.getZustand()) )
            {
                if ( y52.fertig() && y53.fertig() )
                {
                    if ( !y52.getZustand() && !y53.getZustand() )
                    {
                        if ( ufastoe && (saf->md.ufwen == 3) )
                        {
                            nae.uf_contnae = TRUE;
                        }
                        else
                        {
                            nae.uf_contnae = FALSE;
                        }
                        if ( abl.anzauftrag_ready() )
                        {
                            abl.set_anzauftrag(3);
                            stepmerker = FALSE;
                            nup.workbuf = saf->buf;
                        }
                        if ( demomodus )
                        {
                            fstepkey = 0;
                            fwdkey = 0;
                            key = 5;
                        }
                        else
                        {
                            naestop = FALSE;
                            naekey = 0;
                            key = 3;
                        }
                    }
                    else
                    {
                        y52.resAuftrag();
                        y53.resAuftrag();
                    }
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        wartenkey = 3,   Klammer schliessen nach Abfrage */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            switch ( keyb.taste() )
            {
            case TENTER :
                y51u2.setAuftrag();  // Klammer zu
                nae.res_barcode();
                schabl.bc.res_barcode();
                nup.barcode_lesen = TRUE;
                //schabl.rf.set_rfid_read();
                wartenkey = 0;
                break;
            case TESC :
                wartenkey = 0;
                break;
            default:
                break;
            }
        }
        break;
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        wartenkey = 4,  Warten bis Programmnummer gelesen wurde */
        switch ( nup.read_jigpnr () )
        {
        case 0 :
            if ( pnrmem != saf->md.pnr )
            {
                koor.nae_err = 19;
                saf->md.pnr = pnrmem;
                key = 0;
            }
            else
            {
                switch ( weiterinfo )
                {
                case 0 :
                    //Naehen
                    wartenkey = 2;
                    break;
                case 1 :
                    // Vorwaerts Takten
                    fwdkey = 0;
                    nup.workbuf = saf->buf;
                    key = 5;
                    break;
                case 2 :
                    // Rueckwaerts Takten
                    backkey = 0;
                    fastcount = 0;
                    nup.workbuf = saf->buf;
                    key = 6;
                    break;
                default:
                    break;
                }
            }
            break;
        case 1 :
            // keine Klammer eingelegt,
            // Programmnummer konnte nicht
            // gelesen werden
            koor.nae_err = 17;
            key = 0;
            break;
        case 2 :
            // Programmnummer ungueltig
            koor.nae_err = 18;
            key = 0;
            break;
        case 3 :
            break;
        default:
            // RFID-Fehler
            koor.nae_err = 2;
            errpar = schabl.rf.rfid_err;
            key = 0;
            break;
        }
        break;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        wartenkey = 5,   Einstieg ins Vorwaerts Takten */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( y52.fertig() && y53.fertig() )
            {
                if ( !y52.getZustand() && !y53.getZustand() )
                {
                    // Vorwaerts Takten
                    fwdkey = 0;
                    nup.workbuf = saf->buf;
                    key = 5;
                }
                else
                {
                    y52.resAuftrag();
                    y53.resAuftrag();
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 6 :
        /* EasyCODE ( 0 
        wartenkey = 6,   Einstieg ins Rueckwaerts Takten */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( y52.fertig() && y53.fertig() )
            {
                if ( !y52.getZustand() && !y53.getZustand() )
                {
                    // Rueckwaerts Takten
                    backkey = 0;
                    fastcount = 0;
                    nup.workbuf = saf->buf;
                    key = 6;
                }
                else
                {
                    y52.resAuftrag();
                    y53.resAuftrag();
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 10 :
        if ( koor.naestop() )
        {
            key = 0;
        }
        break;
    case 11 :
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( keyb.taste() == 0xff )
            {
                abl.set_anzauftrag (5);
                wartenkey = 1;
            }
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaehen::poweron */
/* EasyCODE F */
void KlasseNaehen::poweron (void)
{
    int parametertemp;
    switch ( poweronkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        poweronkey = 0, NIS (Ref. 1) programmieren */
        #ifdef buerotest
            homereq = TRUE;
            thomeanz = TRUE;
            ponrdy = TRUE;
            key = 0;
        #else 
            if ( nminf.befehl == 0 )
            {
                if ( nminf.fehler != 0 )
                {
                    koor.nae_err = 1;
                    errpar = nminf.fehler;
                    key = 0;
                }
                else
                {
                    parametertemp=(saf->nadelp >> 1) + ((long)saf->md.nisvograd*MAXPOS/360);
                    if ( parametertemp >= MAXPOS )
                    {
                        parametertemp -= MAXPOS;
                    }
                    nminf.parameter=parametertemp;
                    nminf.befehl = 3;
                    poweronkey = 1;
                    tnae1.rsz();
                    tnae1.sz(saf->quick_delayzt);
                }
            }
        #endif 
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        poweronkey = 1,  Schneidpos. (Ref. 2) programmieren */
        if ( (nminf.befehl == 0) && tnae1.tz() )
        {
            if ( nminf.fehler != 0 )
            {
                koor.nae_err = 1;
                errpar = nminf.fehler;
                key = 0;
            }
            else
            {
                parametertemp = (saf->nadelp >> 1) + ((long)saf->md.ref2*MAXPOS/360);
                if ( parametertemp >= MAXPOS )
                {
                    parametertemp -= MAXPOS;
                }
                nminf.parameter = parametertemp;
                nminf.befehl = 4;
                poweronkey = 2;
                tnae1.rsz();
                tnae1.sz(saf->quick_delayzt);
            }
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        poweronkey = 2,  Totmannzeit programmieren */
        if ( (nminf.befehl == 0) && tnae1.tz() )
        {
            if ( nminf.fehler != 0 )
            {
                koor.nae_err = 1;
                errpar = nminf.fehler;
                key = 0;
            }
            else
            {
                #if defined(remote)
                    nminf.parameter = 0;        // Totmann-_berwachung ausschalten
                    nminf.befehl = 6;
                #else 
                    /* EasyCODE < */
                    nminf.parameter = 50;
                    /* EasyCODE > */
                #endif 
                nminf.befehl = 6;
                poweronkey = 3;
                tnae1.rsz();
                tnae1.sz(saf->quick_delayzt);
            }
        }
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        poweronkey = 3 */
        if ( nminf.befehl == 0 )
        {
            if ( nminf.fehler != 0 )
            {
                koor.nae_err = 1;
                errpar = nminf.fehler;
                key = 0;
            }
            else
            {
                qp_index = 0;
                poweronkey = 4;
            }
        }
        break;
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        poweronkey = 4,  restliche Quick-Parameter programmieren */
        if ( (nminf.befehl == 0) && tnae1.tz() )
        {
            if ( nminf.fehler != 0 )
            {
                koor.nae_err = 1;
                errpar = nminf.fehler;
                key = 0;
            }
            else
            {
                nminf.parameternr = quick_paramnr [qp_index];
                nminf.parameter = saf->md.quick_paramwert [qp_index];
                nminf.befehl = 10;
                tnae1.rsz();
                tnae1.sz(saf->quick_delayzt);
                qp_index ++;
                if ( qp_index == QP_ANZAHL )
                {
                    poweronkey = 5;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        poweronkey = 5,   warten bis nminf fertig */
        if ( (nminf.befehl == 0) && tnae1.tz() )
        {
            if ( nminf.fehler != 0 )
            {
                koor.nae_err = 1;
                errpar = nminf.fehler;
                key = 0;
            }
            else
            {
                if ( e79.getZustand() && e80.getZustand() )
                {
                    // Klammer eingelegt
                    y51u2.setAuftrag();   // Klammer zu
                }
                else
                {
                    // Klammer nicht eingelegt
                    y51u1.setAuftrag();   // Klammer auf
                }
                homereq = TRUE;
                thomeanz = TRUE;
                ponrdy = TRUE;
                key = 0;
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
KlasseNaehen::ablauf */
/* EasyCODE F */
void KlasseNaehen::ablauf (void)
{
    switch ( ablaufkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        ablaufkey = 0,   Schlitten steht in Grundstellung, carlp */
        if ( koor.naestop() || (keyb.taste() == TSTOP) )
        {
            key = 0;
        }
        else
        {
            switch ( nup.jig_ctrl () )
            {
            case 0 :
                // falscher Code
                koor.nae_err = 20;
                key = 0;
                break;
            case 1 :
                // Code ok
                tnae4.rsz();
                tnae4.sz(60000);
                ablaufkey = 10;
                break;
            case 2 :
                // Lesen noch nicht fertig
                break;
            default:
                // RFID-Fehler
                koor.nae_err = 2;
                errpar = schabl.rf.rfid_err;
                key = 0;
                break;
            }
        }
        break;
        /* EasyCODE ) */
    case 10 :
        /* EasyCODE ( 0 
        ablaufkey = 10,   auf Anfangspunkt fahren */
        if ( koor.naestop() || (keyb.taste() == TSTOP) )
        {
            /* EasyCODE ( 0 
            Stopanf. */
            key = 0;
            /* EasyCODE ) */
        }
        else
        {
            if ( abl.anzauftrag_ready()   /* SW015 */ )
            {
                if ( saf->md.jigctren == 2 )
                {
                    /* EasyCODE ( 0 
                    Programm lesen und laden bei
                    automatischer Programmnummerwahl */
                    switch ( nup.read_jigpnr () )
                    {
                    case 0 :
                        /* EasyCODE ( 0 
                        Programm lesen */
                        switch ( di.readprog_to_ems () )
                        {
                        case 1 //Programm unvollstaendig
                        :
                            /*koor.eing_err = 42;
                            key = 0;
                            return;
                            */
                            // Programm unvollstaendig (nun automatisch vervollstaendigen)
                            if ( nup.Prog_vervollst()   // SW015
                               )
                            {
                                // koor.eing_err, eing.errpar und eing.errpar2 wurden gesetzt
                                key = 0;
                                return ;
                            }
                            else
                            {
                                // Kein Fehler, weiter machen ohne break
                                abl.set_anzauftrag(2);  // SW015
                            }
                        case 6 :
                            // nur Stichdaten
                        case 0 //ok
                        :
                            pnrmem = saf->md.pnr;
                            nup.getprpar ();
                            nup.workbuf_res ();
                            saf->buf = nup.workbuf;
                            break;
                        case 2 //Programm zu gross
                        :
                            koor.eing_err = 43;
                            key = 0;
                            return ;
                        case 3 //Programm nicht vorhanden
                        :
                            koor.eing_err = 44;
                            key = 0;
                            return ;
                        case 5 :
                            // Programm nicht fÅr diese Maschine
                            koor.eing_err = 47;
                            key = 0;
                            return ;
                        default:
                            // Flash-Lesefehler oder Prog. fehlerhaft
                            koor.eing_err = 45;
                            key = 0;
                            return ;
                        }
                        /* EasyCODE ) */
                        break;
                    case 1 :
                        // keine Klammer eingelegt,
                        // Programmnummer konnte nicht
                        // gelesen werden
                        koor.nae_err = 17;
                        key = 0;
                        return ;
                    case 2 :
                        // Programmnummer ungueltig
                        koor.nae_err = 18;
                        key = 0;
                        return ;
                    case 3 :
                        // RFID-Lesen nicht fertig
                        return ;
                    default:
                        // RFID-Fehler
                        koor.nae_err = 2;
                        errpar = schabl.rf.rfid_err;
                        key = 0;
                        return ;
                    }
                    /* EasyCODE ) */
                }
                /* EasyCODE ( 0 
                Schablonenkontrolle */
                switch ( nup.jig_ctrl () )
                {
                case 0 :
                    // falscher Code
                    koor.nae_err = 20;
                    key = 0;
                    break;
                case 1 :
                    // Code ok
                    switch ( nup.napos () )
                    {
                    case 0 :
                        break;
                    case 1 :
                        koor.nae_err = 1;
                        errpar = nminf.fehler;
                        key = 0;
                        break;
                    default:
                        /* EasyCODE ( 0 
                        Fahren auf AP starten */
                        if ( !ver.move_sonst() )
                        {
                            // verriegelt
                            key = 0;
                        }
                        else
                        {
                            // nicht verriegelt
                            nup.koorsoll [0].xbild = *((int *)(nup.stianfadr + SAP_X));
                            nup.koorsoll [0].ybild = *((int *)(nup.stianfadr + SAP_Y));
                            if ( nup.poscheck (&nup.koorsoll [0]) )
                            {
                                if ( e79.getZustand() || e80.getZustand() // SW013 begin
                                   )
                                {
                                    SchablVorhanden = TRUE;
                                }
                                else
                                {
                                    SchablVorhanden = FALSE;
                                }
                                nup.bitomo (&nup.koorsoll [0]);
                                nup.carmov (FAST);
                                nup.carlp = FALSE;
                                stopmerker = FALSE;
                                ablaufkey = 11;
                            }
                            else
                            {
                                koor.nae_err = 11;
                                key = 0;
                            }
                        }
                        /* EasyCODE ) */
                    }
                    break;
                case 2 :
                    // Lesen noch nicht fertig
                    break;
                default:
                    // RFID-Fehler
                    koor.nae_err = 2;
                    errpar = schabl.rf.rfid_err;
                    key = 0;
                    break;
                }
                /* EasyCODE ) */
            }
        }
        break;
        /* EasyCODE ) */
    case 50 :
        /* EasyCODE_STRUCTURED_COMMENT_START */
        /*
        EasyCODE_COMMENT_START EasyCODE ( 0 
        ablaufkey = 50, Delay Klammer richtig eingelegt abwarten EasyCODE_COMMENT_END
        if ( koor.naestop() || (keyb.taste() == TSTOP) )
        {
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Stopanf. EasyCODE_COMMENT_END
            key = 0;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        }
        else
        {
            if ( tnae2.tz() )
            {
                if ( e79.getZustand() && e80.getZustand() )
                {
                    nup.carmov (FAST);
                    nup.carlp = FALSE;
                    stopmerker = FALSE;
                    ablaufkey = 11;
                }
                else
                {
                    koor.nae_err = 40;
                    nae.errpar = E80_UF1;
                    key = 0;
                }
            }
        }
        break;
        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        */
        /* EasyCODE_STRUCTURED_COMMENT_END */
    case 11 :
        /* EasyCODE ( 0 
        ablaufkey = 11,   Warten bis Schlitten in AP */
        if ( smx.ready() && smy.ready() && abl.anzauftrag_ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            saf->upfahren_dis = FALSE;
            ablaufkey = 15;
            if ( SchablVorhanden && (!e79.getZustand() || !e80.getZustand()) )
            {
                // SW013    SW005DIS 
                koor.nae_err = 40;
                if ( !e80.getZustand() )
                {
                    nae.errpar = E80_UF1;
                }
                else
                {
                    nae.errpar = E79_UF1;
                }
                homereq = TRUE;
                key = 0;
            }
            if ( saf->md.prsel == 1 )
            {
                /* EasyCODE ( 0 
                Sequenz */
                if ( saf->md.autoseq )
                {
                    if ( ((saf->md.next_pnrindex + 1) > PNRINDEX_MAX) ||
                         (saf->md.seq_array[saf->md.seq][saf->md.next_pnrindex+1] == 0xffff) )
                    {
                        saf->md.next_pnrindex = 0;
                    }
                    else
                    {
                        saf->md.next_pnrindex++;
                    }
                    abl.set_anzauftrag(6);
                }
                /* EasyCODE ) */
            }
            else
            {
                if ( saf->md.prsel == 2 )
                {
                    /* EasyCODE ( 0 
                    verbundene Programme */
                    if ( ((saf->md.next_pnrindex + 1) > PNRINDEX_MAX) ||
                         (saf->md.vpnr_array[saf->md.vpnr][saf->md.next_pnrindex+1] == 0xffff) )
                    {
                        saf->md.next_pnrindex = 0;
                    }
                    else
                    {
                        saf->md.next_pnrindex++;
                    }
                    abl.set_anzauftrag(6);
                    /* EasyCODE ) */
                }
            }
            if ( stopmerker )
            {
                key = 0;
            }
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
            else
            {
                if ( koor.naestop() || (keyb.taste() == TSTOP) ||
                     tlinks_stop.getZustand() ||
                     trechts_stop.getZustand() )
                {
                    stopmerker = TRUE;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 15 :
        /* EasyCODE ( 0 
        ablaufkey = 15,   Oberteil senken */
        if ( koor.naestop() || (keyb.taste() == TSTOP) )
        {
            key = 0;
        }
        else
        {
            switch ( nup.napos () )
            {
            case 0 :
                break;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                key = 0;
                break;
            default:
                if ( y4.fertig() && y5.fertig() &&
                     y8.fertig() && hfuss_auf.fertig() )
                {
                    if ( (hfuss.getpos().pos == saf->md.posoben.pos) &&
                         !y8.getZustand() )
                    {
                        /* EasyCODE ( 0 
                        Fuss oben, Oberteil senken */
                        if ( saf->md.jigctren == 2 )
                        {
                            switch ( nup.read_jigpnr () )
                            {
                            case 0 :
                                if ( pnrmem != saf->md.pnr )
                                {
                                    koor.nae_err = 19;
                                    saf->md.pnr = pnrmem;
                                    key = 0;
                                }
                                else
                                {
                                    y50u2.setAuftrag();  // Oberteil senken
                                    y1.setAuftrag();     // Blasluft Nadelkuehlung ein
                                    ablaufkey = 16;
                                }
                                break;
                            case 1 :
                                // keine Klammer eingelegt,
                                // Programmnummer konnte nicht
                                // gelesen werden
                                koor.nae_err = 17;
                                key = 0;
                                break;
                            case 2 :
                                // Programmnummer ungueltig
                                koor.nae_err = 18;
                                key = 0;
                                break;
                            case 3 :
                                // RFID-Lesen nicht fertig
                                break;
                            default:
                                // RFID-Fehler
                                koor.nae_err = 2;
                                errpar = schabl.rf.rfid_err;
                                key = 0;
                                break;
                            }
                        }
                        else
                        {
                            y50u2.setAuftrag();  // Oberteil senken
                            y1.setAuftrag();     // Blasluft Nadelkuehlung ein
                            ablaufkey = 16;
                        }
                        /* EasyCODE ) */
                    }
                    else
                    {
                        hfuss.auf();      // Fuss auf
                        y4.setAuftrag();  // Klemme zu
                        y5.resAuftrag();  // Fadenspannung zu
                        y8.resAuftrag();  // Niederhalter auf
                    }
                }
                break;
            }
        }
        break;
        /* EasyCODE ) */
    case 16 :
        /* EasyCODE ( 0 
        ablaufkey = 16,   Warten bis Oberteil senken fertig */
        if ( koor.naestop() || (keyb.taste() == TSTOP) )
        {
            key = 0;
        }
        else
        {
            if ( y50u2.fertig() )
            {
                ablaufkey = 20;
                naekey = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 20 :
        /* EasyCODE ( 0 
        ablaufkey = 20,   Naehen */
        switch ( naehen () )
        {
        case 0 :
            /* EasyCODE < */
            // aktiv
            /* EasyCODE > */
            break;
        case 1 :
            // Stop
            stopmerker = FALSE;
            naekey = 0;
            key = 0;
            break;
        case 2 :
            // Einlegestopp
            ablaufkey = 40;
            if ( koor.naestop() || (keyb.taste() == TSTOP) )
            {
                key = 0;
            }
            break;
        default:
            // Programm-Ende
            ablaufkey = 25;
            break;
        }
        break;
        /* EasyCODE ) */
    case 25 :
        /* EasyCODE ( 0 
        ablaufkey = 25,   Oberteil auf, Fuss auf, Klemme zu,
                          Fadensp. auf, Blasluft Nadelkuehlung aus */
        if ( koor.naestop() || (keyb.taste() == TSTOP) )
        {
            key = 0;
        }
        else
        {
            y50u1.setAuftrag();   // Oberteil heben
            hfuss.auf_initkey = 0;
            hfuss.auf_init();     // Fuss auf
            y8.resAuftrag();      // Niederhalter auf
            y33.resAuftrag();     // Stoffgegendruecker ab
            y4.setAuftrag();      // Klemme zu
            if ( !saf->cutready  // SW016
               )
            {
                y5.setAuftrag();  // Fadenspannung auf
                y6.resAuftrag();  // Zusatzfadenspannung auf
            }
            else
            {
                y5.resAuftrag();      // Fadenspannung zu
            }
            // y5.resAuftrag();   // Fadenspannung zu  // SW016
            
            y1.resAuftrag();      // Blasluft Nadelkuehlung aus
            ablaufkey = 26;
        }
        break;
        /* EasyCODE ) */
    case 26 :
        /* EasyCODE ( 0 
        ablaufkey = 26,  Stueckzaehler inkrementieren, ev. Sequenz bzw. verbundene Progr.nr. weiterschalten,
                         entsprechend verzweigen */
        if ( koor.naestop() || (keyb.taste() == TSTOP) )
        {
            key = 0;
        }
        else
        {
            if ( hfuss.auf_init() )
            {
                if ( y50u1.fertig() && y4.fertig() && y5.fertig()&& y8.fertig() &&
                     y33.fertig() && abl.anzauftrag_ready() )
                {
                    y32.resAuftrag();  // Unterfadenschieber aus
                    y7.resAuftrag();   // Nadeldreheinrichtung aus
                    y6.resAuftrag();   // Zusatzfadenspannung auf
                    nup.workbuf_res();
                    saf->buf = nup.workbuf;
                    hfuss.akt_posunten = saf->md.posunten;
                    hfuss.akt_hub = saf->md.hub;
                    ablaufkey = 35;
                    if ( !stepmerker )
                    {
                        if ( (saf->md.prsel == 2) && (saf->md.next_pnrindex != 0) )
                        {
                        }
                        else
                        {
                            saf->piececounter ++;
                            prod->total_piececount ++;
                        }
                    }
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 35 :
        /* EasyCODE ( 0 
        ablaufkey = 35,   auf Ladepunkt fahren */
        if ( koor.naestop() || (keyb.taste() == TSTOP) )
        {
            key = 0;
        }
        else
        {
            if ( abl.anzauftrag_ready() )
            {
                if ( (saf->md.prsel == 2) && (saf->md.next_pnrindex != 0) )
                {
                    ablaufkey = 10;
                }
                else
                {
                    /* EasyCODE ( 0 
                    auf LP fahren */
                    switch ( nup.napos () )
                    {
                    case 0 :
                        return ;
                    case 1 :
                        koor.nae_err = 1;
                        errpar = nminf.fehler;
                        key = 0;
                        return ;
                    default:
                        if ( ver.move_sonst () )
                        {
                            nup.koorsoll[0].xbild = nup.workbuf.koorist.xbild;
                            nup.koorsoll[0].motorx = nup.workbuf.koorist.motorx;
                            nup.koorsoll[0].ybild = saf->lp.ybild;
                            nup.koorsoll[0].motory = saf->lp.motory;
                            saf->upfahren_dis = TRUE;
                            ablaufkey = 36;
                            nup.carmov (FAST);
                        }
                        else
                        {
                            key = 0;
                            return ;
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( saf->md.prsel != 0 )
                {
                    saf->md.pnrindex = saf->md.next_pnrindex;
                    nup.getprpar ();
                    nup.workbuf_res ();
                    saf->buf = nup.workbuf;
                    abl.set_anzauftrag(7);
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 36 :
        /* EasyCODE ( 0 
        ablaufkey = 36,   Warten bis Schlitten im LP */
        if ( smx.ready() && smy.ready() && abl.anzauftrag_ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            nup.carlp = TRUE;
            if ( saf->md.openclampen )
            {
                // Klammer auf
                y51u1.setAuftrag();
                schabl.bc.res_barcode();
                schabl.rf.delete_rfidprognummer();
                res_barcode();
                // Statuszeile anzeigen
                abl.set_anzauftrag(2);
            }
            zykluszeit = (float)(60000 - tnae4.getwert())/100;
            if ( //SW18-1
                 ((saf->md.ufwen > 1) && ufastoe && (saf->md.ufwen != 5)) ||
                 ((saf->md.ufwen == 1) && (saf->bobcnt >= saf->md.bobcnt_wert)) )
            {
                //Spulenwechsel starten
                saf->bobcnt = 0;
                nae.faedelreq = TRUE;
                nextfaedelkey = 3;
                tfaedelstartanz = FALSE;
                if ( saf->md.automatic )
                {
                    // Auf keinen Fall
                    // Jig finished senden
                    key = 0;
                    break;
                }
            }
            else
            {
                /*  SW001DIS     */
                if ( saf->md.openclampen )
                {
                }
                else
                {
                    if ( saf->md.automatic   // SW001DIS
                       )
                    {
                        y62.setAuftrag();
                        ablaufkey = 37; // SW001DIS
                        break;
                    }
                }
            }
            ablaufkey = 0;
            key = 0;
        }
        else
        {
            if ( koor.notstop() )
            {
                // verriegelt
                smx.stop();
                smy.stop();
                nae.homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 37 :
        /* EasyCODE ( 0 
        ablaufkey = 37,     SW001DIS */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( in4.getZustand() )
            {
                y51u1.setAuftrag(); // Klammer auf
                ablaufkey = 38;     // SW001DIS
            }
            else
            {
                if ( (keyb.taste() == TSTOP) )
                {
                    ablaufkey = 0;
                    key = 0;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 38 :
        /* EasyCODE ( 0 
        ablaufkey = 38,     SW001DIS */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( !e51u2_1.getZustand() && !e51u2_2.getZustand()  //SW002DIS
               )
            {
                // Klammer ist auf
                y63.setAuftrag(); 
                ablaufkey = 0;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 40 :
        /* EasyCODE ( 0 
        ablaufkey = 40,  Einlegestopp -> Fahren auf LP */
        if ( koor.naestop() || (keyb.taste() == TSTOP) )
        {
            key = 0;
        }
        else
        {
            if ( saf->cutready )
            {
                if ( y50u1.fertig() && y50u2.fertig() )
                {
                    if ( e50u1.getZustand() )
                    {
                        // Oberteil angehoben
                        /* EasyCODE ( 0 
                        auf LP fahren */
                        switch ( nup.napos () )
                        {
                        case 0 :
                            break;
                        case 1 :
                            koor.nae_err = 1;
                            errpar = nminf.fehler;
                            key = 0;
                            break;
                        default:
                            if ( ver.move_sonst () )
                            {
                                nup.koormerker = nup.workbuf.koorist;
                                nup.koorsoll[0].xbild = nup.workbuf.koorist.xbild;
                                nup.koorsoll[0].motorx = nup.workbuf.koorist.motorx;
                                nup.koorsoll[0].ybild = saf->lp.ybild;
                                nup.koorsoll[0].motory = saf->lp.motory;
                                ablaufkey = 41;
                                nup.carmov (FAST);
                            }
                            else
                            {
                                key = 0;
                            }
                            break;
                        }
                        /* EasyCODE ) */
                    }
                    else
                    {
                        // Oberteil heben
                        y50u1.setAuftrag();
                    }
                }
            }
            else
            {
                koor.nae_err = 23;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 41 :
        /* EasyCODE ( 0 
        ablaufkey = 41,   Einlegestopp,  Warten bis Schlitten LP, carlp */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            nup.carlp = TRUE;
            ablaufkey = 42;
            key = 0;
        }
        else
        {
            if ( koor.notstop() )
            {
                // verriegelt
                smx.stop();
                smy.stop();
                nae.homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 42 :
        /* EasyCODE ( 0 
        ablaufkey = 42,  Einlegestopp -> Fahren auf Stopp-Position starten */
        if ( !ver.move_sonst() )
        {
            // verriegelt
            key = 0;
        }
        else
        {
            // nicht verriegelt
            nup.koorsoll [0] = nup.koormerker;
            nup.carmov (FAST);
            nup.carlp = FALSE;
            stopmerker = FALSE;
            ablaufkey = 43;
        }
        break;
        /* EasyCODE ) */
    case 43 :
        /* EasyCODE ( 0 
        ablaufkey = 43,  Einlegestopp -> warten bis Stopp-Position erreicht */
        if ( smx.ready() && smy.ready() )
        {
            // Smx und Smy fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            ablaufkey = 15;
            if ( stopmerker )
            {
                key = 0;
            }
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
            else
            {
                if ( koor.naestop() || (keyb.taste() == TSTOP) )
                {
                    stopmerker = TRUE;
                }
            }
        }
        break;
        /* EasyCODE ) */
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaehen::naehen */
/* EasyCODE F */
unsigned char KlasseNaehen::naehen (void)
{
    unsigned char i, tempstitchlength, y6merker;
    hfkoord tempkoord;
    switch ( naekey )
    {
    case 0 :
        /* EasyCODE ( 0 
        naekey = 0,  Stopanforderung oder nae-botsch1 = busy ? */
        if ( koor.naestop() || (keyb.taste() == TSTOP) ||
             tlinks_stop.getZustand() || trechts_stop.getZustand() )
        {
            return 1;
        }
        else
        {
            if ( !abl.anzauftrag_ready() )
            {
                return 0;
            }
        }
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        naekey = 1,  Bild-Start */
        nup.nacont90();
        if ( koor.nae_err != 0 )
        {
            return 1;
        }
        else
        {
            if ( nup.mbef [XYVALID] )
            {
                /* EasyCODE ( 0 
                Koordinaten gelesen */
                if ( nup.workbuf.nbtart == 0 )
                {
                    /* EasyCODE ( 0 
                    Transportieren */
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild + nup.deltax;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild + nup.deltay;
                    if ( !nup.poscheck (&nup.koorsoll [0]) )
                    {
                        koor.nae_err = 11;
                        return 1;
                    }
                    else
                    {
                        nup.bitomo (&nup.koorsoll [0]);
                        naekey = 2;
                        return 0;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Annaehen */
                    if ( //SW18-1
                         (((saf->md.ufwen > 1) && ufastoe && (saf->md.ufwen != 5)) ||
                          ((saf->md.ufwen == 1) && (saf->bobcnt >= saf->md.bobcnt_wert))) && 
                         saf->cutready )
                    {
                        //Spulenwechsel starten
                        saf->bobcnt = 0;
                        nae.faedelreq = TRUE;
                        nextfaedelkey = 3;
                        tfaedelstartanz = FALSE;
                        return 1;
                    }
                    else
                    {
                        if ( (saf->md.ufwen==3) && ufastoe )
                        {
                            uf_contnae = TRUE;
                        }
                        else
                        {
                            uf_contnae = FALSE;
                        }
                        if ( //SW18-1
                             (saf->md.ufwen == 5) && ufastoe )
                        {
                            ufastoe_stopNachSchneiden = TRUE; ////SW18-1 Merker fuer Fadenschneiden
                        }
                        ufastoe = FALSE;
                        if ( y10.getZustand() )
                        {
                            // Annaehen nach
                            // Band einlegen   SW002
                            y50u2.setAuftrag();  // Oberteil senken
                            y8.setAuftrag();     // Niederhalter ab  (offen neu)
                            out3.setAuftrag();   // Bandzufuehrung ab (neu)
                            hfuss.ab();          // Huepferfuss ab
                            naekey = 26;
                        }
                        else
                        {
                            if ( e50u1.getZustand() )
                            {
                                // Oberteil oben
                                naekey = 16;
                            }
                            else
                            {
                                // Oberteil unten
                                naekey = 17;
                            }
                        }
                        return 0;
                    }
                    /* EasyCODE ) */
                }
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                M-Befehl gelesen */
                if ( nup.mbef [PROGENDE] )
                {
                    /* EasyCODE ( 0 
                    Progr.Ende */
                    saf->upfahren_dis = TRUE;
                    saf->buf = nup.workbuf;
                    return 0xff;
                    /* EasyCODE ) */
                }
                else
                {
                    if ( nup.mbef [SCHNEIDEN] )
                    {
                        /* EasyCODE ( 0 
                        Schneiden */
                        saf->buf = nup.workbuf;
                        return 0;
                        /* EasyCODE ) */
                    }
                    else
                    {
                        if ( nup.mbef [PROGSTOP] )
                        {
                            /* EasyCODE ( 0 
                            progr. Stop */
                            if ( (nup.workbuf.naeout & 0x20) == 0x20 )
                            {
                                // Einlegestopp
                                saf->buf = nup.workbuf;
                                return 2;
                            }
                            else
                            {
                                // progr. Stopp
                                saf->buf = nup.workbuf;
                                return 1;
                            }
                            /* EasyCODE ) */
                        }
                        else
                        {
                            if ( nup.mbef [UFWSTOP] && (saf->variante == 1) )
                            {
                                /* EasyCODE ( 0 
                                Band einlegen */
                                if ( //SW18-1
                                     (((saf->md.ufwen > 1) && ufastoe && (saf->md.ufwen != 5)) ||
                                      ((saf->md.ufwen == 1) && (saf->bobcnt >= saf->md.bobcnt_wert))) &&
                                     saf->cutready )
                                {
                                    // SW002 neu
                                    //Spulenwechsel starten
                                    saf->bobcnt = 0;
                                    nae.faedelreq = TRUE;
                                    nextfaedelkey = 3;
                                    tfaedelstartanz = FALSE;
                                    return 1;
                                }
                                else
                                {
                                    saf->buf = nup.workbuf;
                                    y50u2.setAuftrag();  // Oberteil senken   
                                    y8.setAuftrag();     // Niederhalter ab  (offen neu)
                                    out3.setAuftrag();   // Bandzufuehrung ab (hier neu SW002)
                                    y10.setAuftrag();    // Bandanschlag ab
                                    naekey = 25;
                                    return 0;
                                }
                                /* EasyCODE ) */
                            }
                            else
                            {
                                if ( nup.mbef [REDGSWON] )
                                {
                                    /* EasyCODE ( 0 
                                    red. Geschw. ein */
                                    nup.workbuf.redgswen = TRUE;
                                    saf->buf = nup.workbuf;
                                    return 0;
                                    /* EasyCODE ) */
                                }
                                else
                                {
                                    if ( nup.mbef [REDGSWOFF] )
                                    {
                                        /* EasyCODE ( 0 
                                        red. Geschw. aus */
                                        nup.workbuf.redgswen = FALSE;
                                        saf->buf = nup.workbuf;
                                        return 0;
                                        /* EasyCODE ) */
                                    }
                                    else
                                    {
                                        /* EasyCODE ( 0 
                                        progr. Ausgaenge */
                                        for ( i=OUT1SET; i<=ZUSPGOFF; i++ )
                                        {
                                            if ( nup.mbef [i] )
                                            {
                                                nup.nasere ();
                                                saf->buf = nup.workbuf;
                                                naekey = 0;
                                                return 0;
                                            }
                                        }
                                        /* EasyCODE ) */
                                        /* EasyCODE ( 0 
                                        progr. Eingaenge */
                                        for ( i=INHIGH1; i<=INLOW2; i++ )
                                        {
                                            if ( nup.mbef [i] )
                                            {
                                                abl.set_anzauftrag(8);
                                                naekey = 11;
                                                return 0;
                                            }
                                        }
                                        /* EasyCODE ) */
                                        if ( nup.mbef [DELAY] )
                                        {
                                            /* EasyCODE ( 0 
                                            Warten auf Zeit */
                                            abl.set_anzauftrag(9);
                                            tnae1.rsz();
                                            tnae1.sz (nup.verzzt);
                                            naekey = 12;
                                            return 0;
                                            /* EasyCODE ) */
                                        }
                                        else
                                        {
                                            if ( nup.mbef[HFNIVEAU] ||
                                                 nup.mbef[HFHUB] ||
                                                 nup.mbef[HFNIVEAULEV] ||
                                                 nup.mbef[HFHUBLEV] )
                                            {
                                                /* EasyCODE ( 0 
                                                Huepferfuss-
                                                Niveau oder -Hub */
                                                if ( hfuss.getpos().pos != saf->md.posoben.pos )
                                                {
                                                    hfuss.ab();
                                                }
                                                /* EasyCODE ) */
                                            }
                                            saf->buf = nup.workbuf;
                                            return 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                /* EasyCODE ) */
            }
        }
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        naekey = 2,  Nadel positionieren, Fuss auf, Transportieren starten */
        if ( koor.naestop() || (keyb.taste() == TSTOP) ||
             tlinks_stop.getZustand() || trechts_stop.getZustand() )
        {
            return 1;
        }
        else
        {
            switch ( nup.napos () )
            {
            case 0 :
                return 0;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                return 1;
            default:
                if ( !hfuss_auf.fertig() || !y5.fertig() || !y4.fertig() || !y8.fertig()
                     || !y50u1.fertig() || !y50u2.fertig() || !y33.fertig() )
                {
                    return 0;
                }
                else
                {
                    /* EasyCODE ( 0 
                    Oberteil, Niederhalter, Fuss, Klemme, Fadenspannung, StoffgegendrÅcker */
                    if ( (nup.workbuf.naeout & 0x20) == 0x20 )
                    {
                        /* EasyCODE < */
                        // Oberteil heben
                        /* EasyCODE > */
                        if ( !e50u1.getZustand() )
                        {
                            // SW014
                            //y5.setAuftrag(); // Fadenspannung auf // SW016
                            y6.resAusgang();   // Zusatzfadenspannung auf
                            /* EasyCODE - */
                            y50u1.setAuftrag();
                            return 0;
                        }
                    }
                    else
                    {
                        /* EasyCODE < */
                        // Oberteil senken
                        /* EasyCODE > */
                        if ( !e50u2.getZustand() )
                        {
                            y50u2.setAuftrag();
                            return 0;
                        }
                    }
                    if ( (hfuss.getpos().pos == saf->md.posoben.pos) &&
                         !y8.getZustand() && (e33.getZustand() || !saf->stoffgegendrueckerf)  /* SW010 */ )
                    {
                        /* EasyCODE < */
                        // Fuss oben
                        /* EasyCODE > */
                        if ( saf->cutready )
                        {
                            if ( !y4.getZustand() )
                            {
                                y4.setAuftrag();   // Klemme zu
                                y5.resAuftrag();   // Fadenspannung zu
                                return 0;
                            }
                        }
                        else
                        {
                            if ( y4.getZustand() || !y5.getZustand()  // SW010
                               )
                            {
                                y4.resAuftrag();   // Klemme auf
                                y5.setAuftrag();   // Fadenspannung auf, 
                                                   // muss hier immer erfolgen
                                                   
                                y6.resAusgang();  // Zusatzfadenspannung auf  SW016
                                return 0;
                            }
                        }
                        if ( ver.move_nae() )
                        {
                            nup.carmov (FAST);
                            stopmerker = FALSE;
                            naekey = 3;
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        hfuss.auf();       // Fuss auf
                        y8.resAuftrag();   // Niederhalter auf
                        y33.resAuftrag();  // Stoffgegendruecker ab
                        return 0;
                    }
                    /* EasyCODE ) */
                }
            }
        }
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        naekey = 3,  Transportieren */
        if ( smx.ready() && smy.ready() )
        {
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            naekey = 0;
            if ( stopmerker )
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                return 1;
            }
            else
            {
                if ( koor.naestop() || (keyb.taste() == TSTOP) ||
                     tlinks_stop.getZustand() || trechts_stop.getZustand() )
                {
                    stopmerker = TRUE;
                }
                return 0;
            }
        }
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        naekey = 4,  Annaehen */
        if ( koor.naestop() || (keyb.taste() == TSTOP) ||
             tlinks_stop.getZustand() || trechts_stop.getZustand() )
        {
            return 1;
        }
        else
        {
            if ( !y1.fertig() || !y5.fertig() || !y4.fertig() || !y8.fertig() ||
                 !y30.fertig() || !y31.fertig() || !y33.fertig() ||
                 !hfuss.ready() || !y2fkt_aus.fertig() )
            {
                return 0;
            }
            else
            {
                /* EasyCODE ( 0 
                Huepfer starten */
                smz.pll_faktor = saf->pll;
                smz.status = PAWRAMPPLL;
                smz.init();
                HF->smplus = SMZPLUS;
                HF->smminus = SMZMINUS;
                HF->istpos = hfuss.getpos().motor;
                HF->posunten = hfuss.akt_posunten.motor;
                tempkoord.pos = hfuss.akt_posunten.pos + hfuss.akt_hub;
                hfuss.postomo (&tempkoord);
                HF->posoben = tempkoord.motor;
                if ( saf->startauf + saf->nadelp > PULSE_PRO_UMDR )
                {
                    HF->startauf = saf->startauf + saf->nadelp - PULSE_PRO_UMDR;
                }
                else
                {
                    HF->startauf = saf->startauf + saf->nadelp;
                }
                if ( saf->startab + saf->nadelp > PULSE_PRO_UMDR )
                {
                    HF->startab = saf->startab + saf->nadelp - PULSE_PRO_UMDR;
                }
                else
                {
                    HF->startab = saf->startab + saf->nadelp;
                }
                HF->enable = TRUE;
                /* EasyCODE ) */
                nup.fussgsw = nup.get_hubgsw();
                /* EasyCODE ( 0 
                Softstepping ausschalten */
                smx.Fkt2_off();  // Softstepping SM-Endst. X aus
                smy.Fkt2_off();  // Softstepping SM-Endst. Y aus
                /* EasyCODE ) */
                smx.pll_faktor = saf->pll;
                smy.pll_faktor = saf->pll;
                /* EasyCODE ( 0 
                Schlittentransportart bestimmen */
                smx.vort = saf->ramp_vort;
                smy.status |= EFR_CLK_64;
                smx.status=(smx.status & RICHTUNG) | PAWRAMPCV | PAWSVZPLL;
                smy.status=smy.status | PAWSVZPLL;
                /* EasyCODE ) */
                smx.init();
                smy.init();
                if ( nminf.befehl != 0 )
                {
                    return 0;
                }
                else
                {
                    if ( nminf.fehler != 0 )
                    {
                        koor.nae_err = 1;
                        errpar = nminf.fehler;
                        return 1;
                    }
                    else
                    {
                        bobres1.setAuftrag();
                        bobres2.setAuftrag();
                        i = saf->cutready;
                        nmtrb.grundzustand ();
                        outportb (INTCLR, REF1_SETCLR);
                        outportb (INTCLR, REF1_RESCLR);
                        outportb (INTMASK, REF1EI);
                        tnae1.rsz();
                        tnae1.sz(saf->niszeit);
                        if ( i )
                        {
                            naekey = 20;
                        }
                        else
                        {
                            naekey = 5;
                        }
                        return 0;
                    }
                }
            }
        }
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        naekey = 5,  Naehen */
        if ( nmtrb.nmstop )
        {
            /* EasyCODE ( 0 
            Stop */
            tnae1.rsz();
            tnae1.sz(saf->nmstopzt);
            if ( nup.mbef[SCHNEIDEN] || ofastoe )
            {
                // Schneidstich
                nup.schneidkey = 3;
                naekey = 15;
            }
            else
            {
                naekey = 6;
            }
            cutact = FALSE;
            cutreq = FALSE;
            /* EasyCODE ) */
        }
        else
        {
            if ( (nminf.fehler != 0) || koor.notstop() || tnae1.tz() || !e81.getZustand() )
            {
                /* EasyCODE ( 0 
                Fehler */
                outportb (INTMASK, REF1DI | REF2DI);
                nminf.befehl = 8;
                HF->enable = FALSE;
                smx.stop();
                smy.stop();
                smz.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                hfuss.posok = FALSE;
                naekey = 6;
                if ( tnae1.tz() && (koor.nae_err == 0) )
                {
                    koor.nae_err = 21;
                }
                else
                {
                    if ( !e81.getZustand() )
                    {
                        koor.nae_err = 26;
                    }
                }
                /* EasyCODE ) */
            }
            else
            {
                if ( nmtrb.ofwaktiv &&
                     (therr1.getZustand() || ((saf->md.nadelanzahl == 2) && therr2.getZustand())) )
                {
                    ofastoe = TRUE;
                }
                if ( nmtrb.ufwaktiv &&
                     (boberr1.getZustand() || ((saf->md.nadelanzahl == 2) && boberr2.getZustand())) )
                {
                    ufastoe = TRUE;
                }
                if ( nmtrb.kofwaktiv &&    // SW003DIS
                     (!kone1.getZustand() || ((saf->md.nadelanzahl == 2) && !kone2.getZustand())) )
                {
                    if ( !kone1.getZustand() )
                    {
                        kofastoe_ursache = 1;
                    }
                    else
                    {
                        kofastoe_ursache = 2;
                    }
                    kofastoe = TRUE;
                }
            }
        }
        if ( ((temp.getZustand() && uebertemp) ||
             (!temp.getZustand() && !uebertemp)) &&
             abl.anzauftrag_ready() )
        {
            // Uebertemperatur
            abl.set_anzauftrag(10);
        }
        else
        {
            if ( nup.unterspannung && !abl.acanz && abl.anzauftrag_ready() )
            {
                // Unterspannung
                abl.set_anzauftrag(11);
            }
            else
            {
                if ( anzboreq && abl.anzauftrag_ready() )
                {
                    // Unterfadenzaehler
                    abl.set_anzauftrag(13);
                }
                else
                {
                    if ( eing.par.prtName > 1 )
                    {
                        // Zum Test Istdrehzahl anzeigen  SW015
                        if ( abl.anzauftrag_ready()  &&  tnae2.tz() )
                        {
                            tnae2.rsz();
                            tnae2.sz(10); 
                            nmtrb.gswanz [0] [2] = (nminf.get_drehzahl()+50) / 100; // Trace
                            abl.set_anzauftrag(30);
                        }
                    }
                }
            }
        }
        return 0;
        /* EasyCODE ) */
    case 6 :
        /* EasyCODE ( 0 
        naekey = 6, Naehen Stop */
        if ( (nminf.befehl != 0) || !abl.anzauftrag_ready() )
        {
            return 0;
        }
        else
        {
            outportb (INTMASK, REF1DI | REF2DI);
            anzboreq = FALSE;
            if ( nminf.fehler != 0 )
            {
                koor.nae_err = 1;
                errpar = nminf.fehler;
                return 1;
            }
            else
            {
                if ( koor.nae_err != 0 )
                {
                    return 1;
                }
                else
                {
                    if ( smx.ready() && smy.ready() && smz.ready() &&
                         ((smx.Status() & FEHLERMSK) == 0) && ((smy.Status() & FEHLERMSK) == 0) )
                    {
                        HF->enable = FALSE;
                        hfuss.set_istpos (hfuss.akt_posunten);
                        if ( //SW18-1 nur stoppen wenn ufwen nicht 5
                             ufastoe && (saf->md.ufwen != 5) )
                        {
                            return 1;
                        }
                        else
                        {
                            if ( kofastoe  // SW003DIS
                               )
                            {
                                koor.nae_err = 5;
                                nae.errpar = kofastoe_ursache ;
                                return 1;
                            }
                            else
                            {
                                if ( naestop )
                                {
                                    return 1;
                                }
                                else
                                {
                                    if ( kurzerAnffad )
                                    {
                                        naekey = 18;
                                        return 0;
                                    }
                                    else
                                    {
                                        naekey = 0;
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if ( tnae1.tz() )
                        {
                            HF->enable = FALSE;
                            smx.stop();
                            smy.stop();
                            smz.stop();
                            koor.nae_err = 15;
                            homereq = TRUE;
                            saf->carposok = FALSE;
                            hfuss.posok = FALSE;
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
        /* EasyCODE ) */
    case 8 :
        /* EasyCODE ( 0 
        naekey = 8,  Ruecktakten nach Fadenstoerung */
        if ( fastcount == 0 )
        {
            y4.resAuftrag();    // Klemme auf  SW015
            return 1;
        }
        else
        {
            back ();
            return 0;
        }
        /* EasyCODE ) */
    case 11 :
        /* EasyCODE ( 0 
        naekey = 11,  Warten auf Eingang */
        if ( koor.naestop() || (keyb.taste() == TSTOP) ||
             tlinks_stop.getZustand() || trechts_stop.getZustand() )
        {
            return 1;
        }
        else
        {
            if ( abl.anzauftrag_ready() )
            {
                /* EasyCODE ( 0 
                Eingaenge abfragen */
                if ( nup.mbef[INHIGH1] )
                {
                    if ( !in1.getZustand() )
                    {
                        return 0;
                    }
                }
                else
                {
                    if ( nup.mbef[INLOW1] )
                    {
                        if ( in1.getZustand() )
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        if ( nup.mbef[INHIGH2] )
                        {
                            if ( !in2.getZustand() )
                            {
                                return 0;
                            }
                        }
                        else
                        {
                            if ( nup.mbef[INLOW2] )
                            {
                                if ( in2.getZustand() )
                                {
                                    return 0;
                                }
                            }
                        }
                    }
                }
                /* EasyCODE ) */
                saf->buf = nup.workbuf;
                naekey = 0;
                abl.set_anzauftrag(14);
            }
            return 0;
        }
        /* EasyCODE ) */
    case 12 :
        /* EasyCODE ( 0 
        naekey = 12,  Warten auf Zeit */
        if ( koor.naestop() || (keyb.taste() == TSTOP) ||
             tlinks_stop.getZustand() || trechts_stop.getZustand() )
        {
            return 1;
        }
        else
        {
            if ( abl.anzauftrag_ready() )
            {
                if ( tnae1.tz() )
                {
                    saf->buf = nup.workbuf;
                    naekey = 0;
                    abl.set_anzauftrag(15);
                }
            }
            return 0;
        }
        /* EasyCODE ) */
    case 15 :
        /* EasyCODE ( 0 
        naekey = 15,  Fadenschneiden */
        if ( (keyb.taste() == TSTOP) ||
             tlinks_stop.getZustand() || trechts_stop.getZustand() )
        {
            stopmerker = TRUE;
        }
        switch ( nup.schneidablauf () )
        {
        case 0 :
            if ( koor.naestop() || (koor.nae_err != 0) )
            {
                outportb (INTMASK, REF1DI | REF2DI);
                nminf.befehl = 8;
                homereq = TRUE;
                key = 0;
                return 1;
            }
            else
            {
                return 0;
            }
        case 1 :
            if ( ofastoe )
            {
                fastcount = saf->md.backst;
                backkey = 0;
                naekey = 8;
                return 0;
            }
            else
            {
                if ( kofastoe     // SW003DIS
                   )
                {
                    koor.nae_err = 5;
                    nae.errpar = kofastoe_ursache ;
                    return 1;
                }
                else
                {
                    if ( //SW18-1
                         (saf->md.ufwen == 5) && ufastoe_stopNachSchneiden )
                    {
                        //Spulenwechsel starten
                        saf->bobcnt = 0;
                        nae.faedelreq = TRUE;
                        nextfaedelkey = 3;
                        tfaedelstartanz = FALSE;
                        /* EasyCODE - */
                        ufastoe_stopNachSchneiden = FALSE; //Merker fuer Fadenschneiden
                        return 1;
                    }
                    else
                    {
                        if ( stopmerker )
                        {
                            return 1;
                        }
                        else
                        {
                            naekey = 0;
                            return 0;
                        }
                    }
                }
            }
        default:
            homereq = TRUE;
            key = 0;
            return 1;
        }
        /* EasyCODE ) */
    case 16 :
        /* EasyCODE ( 0 
        naekey = 16,   Oberteil senken vor Annaehen */
        if ( koor.naestop() || (keyb.taste() == TSTOP) ||
             tlinks_stop.getZustand() || trechts_stop.getZustand() )
        {
            return 1;
        }
        else
        {
            switch ( nup.napos () )
            {
            case 0 :
                return 0;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                return 1;
            default:
                if ( hfuss_auf.fertig() )
                {
                    if ( (hfuss.getpos().pos == saf->md.posoben.pos) &&
                         !y8.getZustand() )
                    {
                        // Fuss oben
                        y50u2.setAuftrag();  // Oberteil ab
                        naekey = 17;
                    }
                    else
                    {
                        hfuss.auf();     // Fuss auf
                        y8.resAuftrag(); // Niederhalter auf
                        y4.setAuftrag(); // Klemme zu
                        y5.resAuftrag(); // Fadenspannung zu
                        y31.resAuftrag(); // Unterfadenklemme zu
                    }
                }
                return 0;
            }
        }
        /* EasyCODE ) */
    case 17 :
        /* EasyCODE ( 0 
        naekey = 17, Naehausgaenge in Ordnung bringen, Fuss ab vor Annaehen */
        if ( koor.naestop() || (keyb.taste() == TSTOP) ||
             tlinks_stop.getZustand() || trechts_stop.getZustand() )
        {
            return 1;
        }
        else
        {
            if ( y50u2.fertig() )
            {
                switch ( nup.napos () )
                {
                case 0 :
                    return 0;
                case 1 :
                    koor.nae_err = 1;
                    errpar = nminf.fehler;
                    return 1;
                default:
                    if ( y8.fertig() )
                    {
                        if ( y8.getZustand() ||
                             ((saf->variante==1) && !nup.find_UFWSTOP () && !y8.getZustand()) )
                        {
                            /* EasyCODE ( 0 
                            Naehausgaenge setzen */
                            if ( (nup.workbuf.naeout & 0x01) == 0x01 )
                            {
                                out1.setAuftrag();
                                if ( (saf->md.drehmodus == 2) &&
                                     (saf->md.nadelanzahl == 1) )
                                {
                                    // Nadeldreheinrichtung ein
                                    // Unterfadenschieber ein
                                    y7.setAuftrag();
                                    y32.setAuftrag();
                                }
                            }
                            else
                            {
                                out1.resAuftrag();
                                if ( saf->md.drehmodus == 2 )
                                {
                                    // Nadeldreheinrichtung aus
                                    // Unterfadenschieber aus
                                    y7.resAuftrag();
                                    y32.resAuftrag();
                                }
                            }
                            if ( (nup.workbuf.naeout & 0x02) == 0x02 )
                            {
                                out2.setAuftrag();
                            }
                            else
                            {
                                out2.resAuftrag();
                            }
                            if ( (nup.workbuf.naeout & 0x04) == 0x04 )
                            {
                                out3.setAuftrag();
                            }
                            else
                            {
                                out3.resAuftrag();
                            }
                            if ( (nup.workbuf.naeout & 0x08) == 0x08 )
                            {
                                out4.setAuftrag();
                            }
                            else
                            {
                                out4.resAuftrag();
                            }
                            if ( nup.workbuf.zuspgon )
                            {
                                // Zusatzfadenspannung zu
                                y6.setAuftrag();
                            }
                            else
                            {
                                // Zusatzfadenspannung auf
                                y6.resAuftrag();
                            }
                            y30.resAuftrag();       // Schneiden aus
                            y1.setAuftrag();        // Blasluft Nadelkuehlung ein
                            hfuss.ab();             // Huepferfuss ab
                            y31.resAuftrag();       // Unterfadenklemme zu
                            y2fkt_aus.setAuftrag(); // Fadenwedler in Grundstellung
                            y33.setAuftrag();       // Stoffgegendruecker auf
                            bobres1.resAuftrag();   // Reset Unterfadenwaechter
                            bobres2.resAuftrag();   // Reset Unterfadenwaechter
                            naekey = 4;
                            if ( saf->cutready )
                            {
                                if ( saf->md.wedelnfussoben )
                                {
                                    y5.setAuftrag();  // Fadenspannung auf
                                    y6.resAuftrag();  // Zusatzfadenspannung auf  SW016
                                }
                                else
                                {
                                    y5.resAuftrag();  // Fadenspannung zu
                                    if ( saf->md.kurzerAnffad )
                                    {
                                        // Zusatzfadenspannung zu
                                        y6.setAuftrag();
                                    }
                                }
                                y4fkt.setAuftrag();  // Oberfadenklemme verzoegert auf
                            }
                            else
                            {
                                y5.resAuftrag();  // Fadenspannung zu
                                y4.resAuftrag();  // Oberfadenklemme auf
                                if ( saf->md.kurzerAnffad )
                                {
                                    // Zusatzfadenspannung zu
                                    y6.setAuftrag();
                                }
                            }
                            /* EasyCODE ) */
                        }
                        else
                        {
                            if ( (saf->variante==1) && !nup.find_UFWSTOP () )
                            {
                                // Niederhalter auf
                                y8.resAuftrag();
                            }
                            else
                            {
                                // Niederhalter ab
                                y8.setAuftrag();
                            }
                        }
                    }
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        /* EasyCODE ) */
    case 18 :
        /* EasyCODE ( 0 
        naekey = 18,   Fadenziehen beim Annaehen */
        nup.workbuf.nbtart = 1;
        saf->buf = nup.workbuf; 
        tempkoord.pos = hfuss.akt_posunten.pos + hfuss.akt_hub;
        hfuss.postomo (&tempkoord);
        hfuss.hfmove (tempkoord.motor);
        y5.setAuftrag();     // Fadenspannung auf
        y6.resAuftrag();  // Zusatzfadenspannung auf  SW016
        if ( y6.getZustand() )
        {
            y6merker = TRUE;
            y6.resAuftrag();
        }
        else
        {
            y6merker = FALSE;
        }
        y9fkt.setAuftrag();
        naekey = 19;
        return 0;
        /* EasyCODE ) */
    case 19 :
        /* EasyCODE ( 0 
        naekey = 19,   Warten bis Fadenziehen beim Annaehen fertig */
        if ( koor.naestop() || (keyb.taste() == TSTOP) ||
             tlinks_stop.getZustand() || trechts_stop.getZustand() )
        {
            return 1;
        }
        else
        {
            if ( y9fkt.fertig() && hfuss.ready() )
            {
                hfuss.set_istpos (tempkoord);
                naekey = 0;
                if ( y6merker )
                {
                    y6.setAuftrag();
                }
            }
            return 0;
        }
        /* EasyCODE ) */
    case 20 :
        /* EasyCODE ( 0 
        naekey = 20,   Fadenspannung zu beim Annaehen */
        if ( koor.notstop() )
        {
            outportb (INTMASK, REF1DI | REF2DI);
            nminf.befehl = 8;
            smx.stop();
            smy.stop();
            homereq = TRUE;
            saf->carposok = FALSE;
            naekey = 6;
            return 1;
        }
        else
        {
            if ( tnae1.tz() )
            {
                koor.nae_err = 21;
                outportb (INTMASK, REF1DI | REF2DI);
                nminf.befehl = 8;
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                naekey = 6;
                return 1;
            }
            else
            {
                /* EasyCODE ( 0 
                Position pruefen und
                ev. Spannung zu */
                aktpos = nminf.nadel_pos() - saf->nadelp;
                if ( aktpos < 0 )
                {
                    aktpos += PULSE_PRO_UMDR;
                }
                aktpos = (((long)aktpos * 360)/PULSE_PRO_UMDR);
                if ( aktpos >= saf->md.spgzuvograd )
                {
                    y5.resAuftrag(); // Fadenspannung zu
                    naekey = 5;
                }
                /* EasyCODE ) */
                return 0;
            }
        }
        /* EasyCODE ) */
    case 25 :
        /* EasyCODE ( 0 
        naekey = 25,   Oberteil ab, Niederhalter ab, Anschlag ab fertig ? */
        if ( y50u2.fertig() && y8.fertig() && y10.fertig() )
        {
            return 1;
        }
        else
        {
            if ( koor.naestop() )
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        /* EasyCODE ) */
    case 26 :
        /* EasyCODE ( 0 
        naekey = 26,   Bandanschlag auf */
        if ( hfuss.ready() )
        {
            y10.resAuftrag();   // Bandanschlag auf
            naekey = 27;
            return 0;
        }
        else
        {
            if ( koor.naestop() )
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        /* EasyCODE ) */
    case 27 :
        /* EasyCODE ( 0 
        naekey = 27,   Bandanschlag fertig ---> naehstart */
        if ( y10.fertig() )
        {
            naekey = 17;
            return 0;
        }
        else
        {
            if ( koor.naestop() )
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        /* EasyCODE ) */
    default:
        return 0;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaehen::home */
/* EasyCODE F */
void KlasseNaehen::home (void)
{
    #ifdef buerotest
        homereq = FALSE;
        saf->md.pnrindex = saf->md.next_pnrindex;
        firsthome = FALSE;
        nup.workbuf.koorist = saf->mnp;
        nup.carlp = TRUE;
        saf->carposok = TRUE;
        nup.workbuf_res();
        saf->buf = nup.workbuf;
        hfuss.akt_posunten = saf->md.posunten;
        hfuss.akt_hub = saf->md.hub;
        hfuss.istpos.pos = INITIATOR_POS;
        hfuss.istpos.motor = INITIATOR_MOTOR;
        hfuss.posok = TRUE;
        key = 0;
        return ;
    #endif 
    switch ( homekey )
    {
    case 0 :
        /* EasyCODE ( 0 
        homekey = 0,   Bildschirmvorlage anzeigen */
        if ( keyb.taste() == 0xff )
        {
            if ( nup.carlp && !nae.faedelreq && saf->carposok )
            {
                thomeanz = FALSE;
            }
            if ( thomeanz )
            {
                // Homefahren abfragen
                nae_botsch1.senden(BV022);
                homekey = 1;
            }
            else
            {
                // Direkt fahren, nur Stopp Anzeige
                nae_botsch1.senden(BV023);
                thomeanz = TRUE;
                homekey = 3;
            }
        }
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        homekey = 1,   Warten bis Taste Home gedrueckt */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            switch ( keyb.taste() )
            {
            case TEINGABE :
                eing.auftrag = 2;
                homekey = 2;
                break;
            case THOME :
                thomeanz = FALSE;
                homekey = 0;
                break;
            default:
                break;
            }
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        homekey = 2,   Warten auf Stopanforderung */
        if ( koor.naestop() )
        {
            key = 0;
        }
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        homekey = 3,   Spulenabdeckung zu, Unterfadenschieber aus */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                homekey = 0;
            }
            else
            {
                y50u2.resAuftrag(); // Auftrag Oberteil ab ruecks.
                y7.resAuftrag();    // Nadeldreheinrichtung aus
                y32.resAuftrag();   // Unterfadenschieber aus
                y52.resAuftrag();
                y53.resAuftrag();
                y10.resAuftrag();   // Bandanschlag auf
                homekey = 4;
            }
        }
        break;
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        homekey = 4,   Fadenschneiden einleiten bzw. Nadel positionieren */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                homekey = 0;
            }
            else
            {
                if ( y7.fertig() && y32.fertig() && y52.fertig() && y53.fertig() &&
                     !nae_botsch1.busycheck() && !nae_botsch2.busycheck() )
                {
                    if ( firsthome )
                    {
                        //  SW002
                        hfuss.akt_posunten = saf->md.posunten;
                        hfuss.akt_hub = saf->md.hub;
                    }
                    if ( saf->cutready )
                    {
                        /* EasyCODE ( 0 
                        Nadel positionieren, Fuss auf,
                        Klemme zu, Fadenspannung zu,
                        Zusatzspannung auf, Fadenschneiden aus */
                        switch ( nup.napos() )
                        {
                        case 0 :
                            break;
                        case 1 :
                            koor.nae_err = 1;
                            errpar = nminf.fehler;
                            key = 0;
                            break;
                        default:
                            y4.setAuftrag();    // Oberfadenklemme zu
                            y5.resAuftrag();    // Fadenspannung zu
                            y6.resAuftrag();    // Zusatzspannung auf
                            y8.resAuftrag();    // Niederhalter auf
                            y30.resAuftrag();   // Fadenschneiden aus
                            y33.resAuftrag();   // Stoffgegendruecker ab
                            hfuss.posinitkey = 0;
                            tnae1.rsz();
                            tnae1.sz(saf->initzt);
                            homekey = 6;
                            break;
                        }
                        /* EasyCODE ) */
                    }
                    else
                    {
                        nup.schneidkey = 0;
                        homekey = 5;
                    }
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        homekey = 5,   Fadenschneiden */
        switch ( nup.schneidablauf() )
        {
        case 0 :
            break;
        case 1 :
            hfuss.posinitkey = 0;
            tnae1.rsz();
            tnae1.sz(saf->initzt);
            homekey = 6;
            break;
        case 2 :
            // Faden manuell geschnitten
            homekey = 4;
            break;
        case 3 :
            // Taste Eingabe gedrueckt
            eing.auftrag = 2;
            homekey = 2;
            break;
        case 4 :
            // Stopanforderung oder Fehler
            key = 0;
            break;
        default:
            // Stop-Taste
            homekey = 0;
            break;
        }
        break;
        /* EasyCODE ) */
    case 6 :
        /* EasyCODE ( 0 
        homekey = 6,   Huepferfuss positionieren */
        if ( hfuss.posinit() )
        {
            //hfuss.auf();  // Fuss auf
            homekey = 8;
        }
        else
        {
            if ( tnae1.tz() )
            {
                smz.stop();
                koor.nae_err = 24;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 8 :
        /* EasyCODE ( 0 
        homekey = 8,   Oberteil heben */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                homekey = 0;
            }
            else
            {
                if ( y4.fertig() && y5.fertig() && hfuss.ready() &&
                     y8.fertig() && y33.fertig() )
                {
                    y50u1.setAuftrag();   // Oberteil auf
                    homekey = 9;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 9 :
        /* EasyCODE ( 0 
        homekey = 9,   nachdem Oberteil oben - entsprechend verzweigen */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                homekey = 0;
            }
            else
            {
                if ( y50u1.fertig() )
                {
                    nozufkey = 0;
                    if ( saf->md.automatic  // SW006DIS
                       )
                    {
                        saf->carposok = FALSE;  // SW002DIS
                    }
                    homekey = 12;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 12 :
        /* EasyCODE ( 0 
        homekey = 12,   Home-Ablauf */
        switch ( nozufkey )
        {
        case 0 :
            /* EasyCODE ( 0 
            nozufkey = 0,   Klammer schalten */
            if ( koor.naestop() )
            {
                key = 0;
            }
            else
            {
                if ( keyb.taste() == TSTOP )
                {
                    homekey = 0;
                }
                else
                {
                    if ( saf->carposok && nup.carlp &&
                         (nup.workbuf.koorist.ybild == saf->lp.ybild) )
                    {
                        nozufkey = 2;
                    }
                    else
                    {
                        if ( e51u2_1.getZustand() && e51u2_2.getZustand() )
                        {
                            /* EasyCODE ( 0 
                            Klammer geschlossen */
                            if ( saf->carposok )
                            {
                                nozufkey = 1;
                            }
                            else
                            {
                                nup.initkey = 0;
                                tnae1.rsz();
                                tnae1.sz(saf->initzt);
                                nozufkey = 3;
                            }
                            /* EasyCODE ) */
                        }
                        else
                        {
                            if ( e79.getZustand() && e80.getZustand() )
                            {
                                // Klammer zu
                                y51u2.setAuftrag();
                            }
                            else
                            {
                                /* EasyCODE ( 0 
                                Klammer offen und keine
                                Schablone eingelegt */
                                if ( saf->carposok )
                                {
                                    nozufkey = 1;
                                }
                                else
                                {
                                    nup.initkey = 0;
                                    tnae1.rsz();
                                    tnae1.sz(saf->initzt);
                                    nozufkey = 3;
                                }
                                /* EasyCODE ) */
                            }
                        }
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            nozufkey = 1,   auf LP fahren */
            if ( koor.naestop() )
            {
                key = 0;
            }
            else
            {
                if ( keyb.taste() == TSTOP )
                {
                    homekey = 0;
                }
                else
                {
                    if ( ver.move_sonst() )
                    {
                        nup.koorsoll[0].xbild = nup.workbuf.koorist.xbild;
                        nup.koorsoll[0].motorx = nup.workbuf.koorist.motorx;
                        nup.koorsoll[0].ybild = saf->lp.ybild;
                        nup.koorsoll[0].motory = saf->lp.motory;
                        nup.carmov (FAST);
                        nozufkey = 2;
                    }
                    else
                    {
                        key = 0;
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            nozufkey = 2,   nachdem LP erreicht Flags, Naehausgaenge und workbuf ruecksetzen */
            if ( smx.ready() && smy.ready() )
            {
                nup.workbuf.koorist = nup.koorsoll [0];
                nup.carlp = TRUE;
                switch ( saf->md.prsel )
                {
                case 0 :
                    break;
                case 1 :
                    saf->md.pnrindex = saf->md.next_pnrindex;
                    nup.getprpar();
                    break;
                default:
                    saf->md.pnrindex = 0;
                    saf->md.next_pnrindex = 0;
                    nup.getprpar();
                    break;
                }
                out1.resAuftrag();
                out2.resAuftrag();
                out3.resAuftrag();
                out4.resAuftrag();
                nup.workbuf_res ();
                saf->buf = nup.workbuf;
                hfuss.akt_posunten = saf->md.posunten;
                hfuss.akt_hub = saf->md.hub;
                faedelreq = FALSE;
                findreq = FALSE;
                ofastoe = FALSE;
                ufastoe = FALSE;
                kofastoe = FALSE;    // SW003DIS
                
                saf->upfahren_dis = TRUE;
                ablaufkey = 0;
                homereq = FALSE;
                key = 0;
                if ( saf->md.openclampen )
                {
                    y51u1.setAuftrag();  // Klammer auf
                    schabl.bc.res_barcode();
                    schabl.rf.delete_rfidprognummer();
                    res_barcode();
                }
                else
                {
                    if ( !e51u2_1.getZustand() || !e51u2_2.getZustand() )
                    {
                        schabl.bc.res_barcode();
                        res_barcode();
                        schabl.rf.delete_rfidprognummer();
                    }
                    else
                    {
                        if ( firsthome )
                        {
                            schabl.rf.set_rfid_read();
                            schabl.bc.set_bc_read(); //SW005-1
                        }
                    }
                }
                if ( firsthome )
                {
                    /* EasyCODE ( 0 
                    im ersten Home EINMAL Fadenschneidzylinder betaetigen SW18-2 */
                    //SW18-2
                    //Nadel im gueltigen Bereich
                    //nur im ersten Home EINMAL Fadenschneidzylinder betaetigen
                    //um Mechanik zu schmieren, sonst kommt Zylinder zu spaet
                    /* EasyCODE - */
                    eing.ioact = TRUE;
                    if ( !nminf.nadel_bereich(saf->ot_bereich) )
                    {
                        io.secerr=NEEDLEPOS_UF1;
                        io.mainerr=Y30_HF1;
                    }
                    else
                    {
                        y30.setAusgang();  // Fadenschneiden ein
                        /* EasyCODE - */
                        tio4.rsz();
                        tio4.sz(40); //400 ms warten
                        while ( tio4.tz() == 0 )
                        {
                            //warten, Zeit laeuft
                        }
                        y30.resAusgang();  // Fadenschneiden aus
                        /* EasyCODE - */
                        tio4.rsz();
                        tio4.sz(25); //250 ms warten
                        while ( tio4.tz() == 0 )
                        {
                            //warten, Zeit laeuft
                        }
                        //Fadenschneidzylinder einmal betaetigt
                    }
                    eing.ioact = FALSE;
                    /* EasyCODE ) */
                }
                firsthome = FALSE;
            }
            else
            {
                if ( koor.notstop() )
                {
                    smx.stop();
                    smy.stop();
                    saf->carposok = FALSE;
                    key = 0;
                }
            }
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            nozufkey = 3,   auf Initiatoren fahren */
            if ( nup.movinit () )
            {
                if ( firsthome )
                {
                    //  SW002
                    hfuss.akt_posunten = saf->md.posunten;
                    hfuss.akt_hub = saf->md.hub;
                    if ( carposok_mem && !saf->upfahren_dis )
                    {
                        if ( !nae_botsch1.busycheck() && !nae_botsch2.busycheck() &&
                             (keyb.taste() == 0xff) )
                        {
                            nae_botsch1.senden(BV024);
                            nae_botsch2.senden(hometext2);
                            nozufkey = 4;
                        }
                    }
                    else
                    {
                        nozufkey = 1;
                    }
                }
                else
                {
                    nozufkey = 1;
                }
            }
            else
            {
                if ( tnae1.tz() )
                {
                    koor.nae_err = 13;
                    smx.stop();
                    smy.stop();
                    saf->carposok = FALSE;
                    key = 0;
                }
                else
                {
                    if ( !ver.move_sonst() || koor.notstop() )
                    {
                        smx.stop();
                        smy.stop();
                        saf->carposok = FALSE;
                        key = 0;
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 4 :
            /* EasyCODE ( 0 
            nozufkey = 4,   "Auf unterbrochenen Punkt fahren ?" */
            if ( koor.naestop() )
            {
                key = 0;
            }
            else
            {
                if ( !nae_botsch1.busycheck() )
                {
                    switch ( keyb.taste() )
                    {
                    case TENTER :
                        nae_botsch1.senden(BV023);
                        homekey = 15;
                        break;
                    case TESC :
                        nae_botsch1.senden(BV023);
                        nozufkey = 1;
                        break;
                    case TEINGABE :
                        eing.auftrag = 2;
                        homekey = 2;
                        break;
                    default:
                        break;
                    }
                }
            }
            break;
            /* EasyCODE ) */
        default:
            break;
        }
        break;
        /* EasyCODE ) */
    case 15 :
        /* EasyCODE ( 0 
        homekey = 15,   auf unterbrochenen Punkt fahren */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                homekey = 0;
            }
            else
            {
                if ( ver.move_sonst() )
                {
                    nup.koorsoll [0] = saf->buf.koorist;
                    nup.carmov (FAST);
                    homekey = 16;
                }
                else
                {
                    key = 0;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 16 :
        /* EasyCODE ( 0 
        homekey = 16,   nachdem unterbrochener Punkt erreicht - Oberteil ab */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            if ( saf->buf.nbtart != 0 )
            {
                saf->buf.nbtart = 1;
            }
            nup.workbuf = saf->buf;
            if ( (nup.workbuf.naeout & 0x20) != 0x20 )
            {
                y50u2.setAuftrag();   // Oberteil ab
            }
            homekey = 17;
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                saf->carposok = FALSE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 17 :
        /* EasyCODE ( 0 
        homekey = 17,   Naehausgaenge richtig schalten */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                homekey = 0;
            }
            else
            {
                if ( y50u2.fertig() )
                {
                    /* EasyCODE < */
                    // Naehausgaenge setzen
                    /* EasyCODE > */
                    if ( (nup.workbuf.naeout & 0x01) == 0x01 )
                    {
                        out1.setAuftrag();
                        if ( (saf->md.drehmodus == 2) &&
                             (saf->md.nadelanzahl == 1) )
                        {
                            // Nadeldreheinrichtung ein
                            // Unterfadenschieber ein
                            y7.setAuftrag();
                            y32.setAuftrag();
                        }
                    }
                    else
                    {
                        out1.resAuftrag();
                        if ( saf->md.drehmodus == 2 )
                        {
                            // Nadeldreheinrichtung aus
                            // Unterfadenschieber aus
                            y7.resAuftrag();
                            y32.resAuftrag();
                        }
                    }
                    if ( (nup.workbuf.naeout & 0x02) == 0x02 )
                    {
                        out2.setAuftrag();
                    }
                    else
                    {
                        out2.resAuftrag();
                    }
                    if ( (nup.workbuf.naeout & 0x04) == 0x04 )
                    {
                        out3.setAuftrag();
                    }
                    else
                    {
                        out3.resAuftrag();
                    }
                    if ( (nup.workbuf.naeout & 0x08) == 0x08 )
                    {
                        out4.setAuftrag();
                    }
                    else
                    {
                        out4.resAuftrag();
                    }
                    if ( nup.workbuf.zuspgon )
                    {
                        /* EasyCODE < */
                        y6.setAuftrag();  // Zusatzfadenspannung ein
                        /* EasyCODE > */
                    }
                    else
                    {
                        /* EasyCODE < */
                        y6.resAuftrag();  // Zusatzfadenspannung aus
                        /* EasyCODE > */
                    }
                    faedelreq = FALSE;
                    findreq = FALSE;
                    ofastoe = FALSE;
                    kofastoe = FALSE;    // SW003DIS
                    
                    //ufastoe = FALSE;
                    ablaufkey = 20;
                    naekey = 0;
                    homereq = FALSE;
                    key = 0;
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
Vorwaerts Takten */
/* EasyCODE ( 0 
KlasseNaehen::fwd */
/* EasyCODE F */
void KlasseNaehen::fwd (void)
{
    switch ( fwdkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        fwdkey = 0,   Verzweigen abhaengig von ablaufkey */
        switch ( ablaufkey )
        {
        case 0 :
            /* EasyCODE = */
        case 10 :
            fwdkey = 1;
            break;
        case 15 :
            fwdkey = 3;
            break;
        case 16 :
            fwdkey = 4;
            break;
        case 20 :
            fwdkey = 5;
            break;
        case 25 :
            fwdkey = 10;
            break;
        case 26 :
            /* EasyCODE = */
        case 35 :
            fwdkey = 11;
            break;
        case 40 :
            fwdkey = 15;
            break;
        case 42 :
            fwdkey = 17;
            break;
        default:
            key = 0;
            break;
        }
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        fwdkey = 1,   Schlitten steht in Grundstellung (carlp),
                      Fahren in AP starten */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( abl.anzauftrag_ready()  /* SW015 */ )
            {
                if ( saf->md.jigctren == 2 )
                {
                    /* EasyCODE ( 0 
                    Programm lesen und laden bei
                    automatischer Programmnummerwahl */
                    switch ( nup.read_jigpnr () )
                    {
                    case 0 :
                        /* EasyCODE ( 0 
                        Programm lesen */
                        switch ( di.readprog_to_ems () )
                        {
                        case 1 //Programm unvollstaendig
                        :
                            /*
                            koor.eing_err = 42;
                            key = 0;
                            return;
                            */
                            // Programm unvollstaendig (nun automatisch vervollstaendigen)
                            if ( nup.Prog_vervollst()   // SW015
                               )
                            {
                                // koor.eing_err, eing.errpar und eing.errpar2 wurden gesetzt
                                key = 0;
                                return ;
                            }
                            else
                            {
                                // Kein Fehler, weiter machen ohne break
                                abl.set_anzauftrag(2);  // SW015
                            }
                        case 6 :
                            // nur Stichdaten
                        case 0 //ok
                        :
                            pnrmem = saf->md.pnr;
                            nup.getprpar ();
                            nup.workbuf_res ();
                            saf->buf = nup.workbuf;
                            break;
                        case 2 //Programm zu gross
                        :
                            koor.eing_err = 43;
                            key = 0;
                            return ;
                        case 3 //Programm nicht vorhanden
                        :
                            koor.eing_err = 44;
                            key = 0;
                            return ;
                        case 5 :
                            // Programm nicht fÅr diese Maschine
                            koor.eing_err = 47;
                            key = 0;
                            return ;
                        default:
                            // Flash-Lesefehler oder Prog. fehlerhaft
                            koor.eing_err = 45;
                            key = 0;
                            return ;
                        }
                        /* EasyCODE ) */
                        break;
                    case 1 :
                        // keine Klammer eingelegt,
                        // Programmnummer konnte nicht
                        // gelesen werden
                        koor.nae_err = 17;
                        key = 0;
                        return ;
                    case 2 :
                        // Programmnummer ungueltig
                        koor.nae_err = 18;
                        key = 0;
                        return ;
                    case 3 :
                        // RFID-Lesen nicht fertig
                        return ;
                    default:
                        // RFID-Fehler
                        koor.nae_err = 2;
                        errpar = schabl.rf.rfid_err;
                        key = 0;
                        return ;
                    }
                    /* EasyCODE ) */
                }
                /* EasyCODE ( 0 
                Schablonenkontrolle */
                switch ( nup.jig_ctrl () )
                {
                case 0 :
                    // falscher Code
                    koor.nae_err = 20;
                    key = 0;
                    break;
                case 1 :
                    // Code ok
                    switch ( nup.napos () )
                    {
                    case 0 :
                        break;
                    case 1 :
                        koor.nae_err = 1;
                        errpar = nminf.fehler;
                        key = 0;
                        break;
                    default:
                        /* EasyCODE ( 0 
                        Fahren auf AP starten */
                        if ( !ver.move_sonst() )
                        {
                            // verriegelt
                            key = 0;
                        }
                        else
                        {
                            // nicht verriegelt
                            nup.koorsoll [0].xbild = *((int *)(nup.stianfadr + SAP_X));
                            nup.koorsoll [0].ybild = *((int *)(nup.stianfadr + SAP_Y));
                            if ( nup.poscheck (&nup.koorsoll [0]) )
                            {
                                if ( e79.getZustand() || e80.getZustand() // SW013 begin
                                   )
                                {
                                    SchablVorhanden = TRUE;
                                }
                                else
                                {
                                    SchablVorhanden = FALSE;
                                }
                                fwdkey = 2;
                                nup.bitomo (&nup.koorsoll [0]);
                                nup.carmov (FAST);
                                nup.carlp = FALSE;
                            }
                            else
                            {
                                koor.nae_err = 11;
                                key = 0;
                            }
                        }
                        /* EasyCODE ) */
                    }
                    break;
                case 2 :
                    // Lesen noch nicht fertig
                    break;
                default:
                    // RFID-Fehler
                    koor.nae_err = 2;
                    errpar = schabl.rf.rfid_err;
                    key = 0;
                    break;
                }
                /* EasyCODE ) */
            }
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        fwdkey = 2,   Warten bis Schlitten in AP,  caratsew */
        if ( smx.ready() && smy.ready() && abl.anzauftrag_ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            saf->upfahren_dis = FALSE;
            ablaufkey = 15;
            fwdkey = 20;
            if ( SchablVorhanden && (!e79.getZustand() || !e80.getZustand()) )
            {
                // SW013    SW005DIS 
                koor.nae_err = 40;
                if ( !e80.getZustand() )
                {
                    nae.errpar = E80_UF1;
                }
                else
                {
                    nae.errpar = E79_UF1;
                }
                homereq = TRUE;
                key = 0;
            }
            if ( saf->md.prsel == 2 )
            {
                /* EasyCODE ( 0 
                verbundene Programme */
                if ( ((saf->md.next_pnrindex + 1) > PNRINDEX_MAX) ||
                     (saf->md.vpnr_array[saf->md.vpnr][saf->md.next_pnrindex+1] == 0xffff) )
                {
                    saf->md.next_pnrindex = 0;
                }
                else
                {
                    saf->md.next_pnrindex++;
                }
                abl.set_anzauftrag(6);
                /* EasyCODE ) */
            }
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        fwdkey = 3,   Oberteil absenken */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            switch ( nup.napos () )
            {
            case 0 :
                break;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                key = 0;
                break;
            default:
                if ( hfuss_auf.fertig() && y4.fertig() &&
                     y5.fertig() && y8.fertig() )
                {
                    if ( (hfuss.getpos().pos == saf->md.posoben.pos) &&
                         !y8.getZustand() )
                    {
                        /* EasyCODE ( 0 
                        Fuss oben, Niederhalter oben,
                        Oberteil senken */
                        if ( saf->md.jigctren == 2 )
                        {
                            switch ( nup.read_jigpnr () )
                            {
                            case 0 :
                                if ( pnrmem != saf->md.pnr )
                                {
                                    koor.nae_err = 19;
                                    saf->md.pnr = pnrmem;
                                    key = 0;
                                }
                                else
                                {
                                    y50u2.setAuftrag();  // Oberteil senken
                                    ablaufkey = 16;
                                    fwdkey = 4;
                                }
                                break;
                            case 1 :
                                // keine Klammer eingelegt,
                                // Programmnummer konnte nicht
                                // gelesen werden
                                koor.nae_err = 17;
                                key = 0;
                                break;
                            case 2 :
                                // Programmnummer ungueltig
                                koor.nae_err = 18;
                                key = 0;
                                break;
                            case 3 :
                                // noch nicht fertig
                                break;
                            default:
                                // RFID-Fehler
                                koor.nae_err = 2;
                                errpar = schabl.rf.rfid_err;
                                key = 0;
                                break;
                            }
                        }
                        else
                        {
                            y50u2.setAuftrag();  // Oberteil senken
                            ablaufkey = 16;
                            fwdkey = 4;
                        }
                        /* EasyCODE ) */
                    }
                    else
                    {
                        hfuss.auf();      // Fuss auf
                        y4.setAuftrag();  // Klemme zu
                        y5.resAuftrag();  // Fadenspannung zu
                        y8.resAuftrag();  // Niederhalter auf
                    }
                }
                break;
            }
        }
        break;
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        fwdkey = 4,   Warten bis Oberteil absenken fertig */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( y50u2.fertig() )
            {
                ablaufkey = 20;
                naekey = 0;
                fwdkey = 20;
            }
        }
        break;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        fwdkey = 5,   Faden geschnitten ? */
        if ( saf->cutready || (nup.workbuf.nbtart == 0) )
        {
            dauertakten = FALSE;
            dauercount = 0;
            tnae2.rsz();
            tnae2.sz(saf->tadelzt);
            fstepkey = 0;
            fwdkey = 9;
            if ( demomodus && abl.anzauftrag_ready() )
            {
                abl.set_anzauftrag(3);
            }
        }
        else
        {
            y7.resAuftrag();  // Nadeldreheinrichtung aus
            y32.resAuftrag(); // Unterfadenschieber aus
            fwdkey = 6;
        }
        break;
        /* EasyCODE ) */
    case 6 :
        /* EasyCODE ( 0 
        fwdkey = 6,  Fadenschneiden einleiten */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( y7.fertig() && y32.fertig() )
            {
                nup.schneidkey = 0;
                fwdkey = 7;
            }
        }
        break;
        /* EasyCODE ) */
    case 7 :
        /* EasyCODE ( 0 
        fwdkey = 7,   Fadenschneiden */
        switch ( nup.schneidablauf () )
        {
        case 0 :
            if ( koor.naestop() || (koor.nae_err != 0) )
            {
                outportb (INTMASK, REF1DI | REF2DI);
                nminf.befehl = 8;
                key = 0;
            }
            break;
        case 1 :
            dauertakten = FALSE;
            dauercount = 0;
            tnae2.rsz();
            tnae2.sz(saf->tadelzt);
            fstepkey = 0;
            fwdkey = 9;
            if ( demomodus && abl.anzauftrag_ready() )
            {
                abl.set_anzauftrag(3);
            }
            break;
        default:
            key = 0;
            break;
        }
        break;
        /* EasyCODE ) */
    case 8 :
        /* EasyCODE ( 0 
        fwdkey = 8,   nextstep ? */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( !dauertakten && !demomodus )
            {
                if ( keyb.taste() == TFWD )
                {
                    if ( tnae2.tz() )
                    {
                        if ( (++dauercount >= 50) && abl.anzauftrag_ready() )
                        {
                            stopmerker = FALSE;
                            dauertakten = TRUE;
                            abl.set_anzauftrag(3);
                        }
                        tnae2.rsz();
                        tnae2.sz(saf->taktzt);
                        fstepkey = 0;
                        fwdkey = 9;
                    }
                }
                else
                {
                    fwdkey = 20;
                }
            }
            else
            {
                if ( stopmerker ||
                     (keyb.taste() == TSTOP) ||
                     tlinks_stop.getZustand() ||
                     trechts_stop.getZustand() )
                {
                    stopmerker = FALSE;
                    key = 0;
                }
                else
                {
                    if ( tnae2.tz() )
                    {
                        tnae2.rsz();
                        tnae2.sz(saf->taktzt);
                        fstepkey = 0;
                        fwdkey = 9;
                    }
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 9 :
        /* EasyCODE ( 0 
        fwdkey = 9,   durchs Naehprogramm takten */
        switch ( fstep () )
        {
        case 0 :
            if ( dauertakten &&
                 ((keyb.taste() == TSTOP) ||
                   tlinks_stop.getZustand() ||
                   trechts_stop.getZustand()) )
            {
                stopmerker = TRUE;
            }
            break;
        case 1 :
            if ( demomodus )
            {
                fwdkey = 20;
            }
            else
            {
                key = 0;
            }
            break;
        case 2 :
            ablaufkey = 40;
            fwdkey = 15;
            break;
        default:
            fwdkey = 8;
            break;
        }
        break;
        /* EasyCODE ) */
    case 10 :
        /* EasyCODE ( 0 
        fwdkey = 10,   Oberteil heben */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            switch ( nup.napos() )
            {
            case 0 :
                break;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                key = 0;
                break;
            default:
                y50u1.setAuftrag();  // Oberteil heben
                hfuss.auf();         // Fuss auf
                y4.resAuftrag();     // Klemme zu
                y5.resAuftrag();     // Fadenspannung zu
                y8.resAuftrag();     // Niederhalter auf
                ablaufkey = 26;
                fwdkey = 20;
                break;
            }
        }
        break;
        /* EasyCODE ) */
    case 11 :
        /* EasyCODE ( 0 
        fwdkey = 11,   Fahren zum LP bzw. AP des naechsten Programmes bei verbundenen Programmen */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( y50u1.fertig() && y4.fertig() && y5.fertig()&& y8.fertig() &&
                 y33.fertig() && hfuss.ready() && abl.anzauftrag_ready() )
            {
                nup.workbuf_res();
                y7.resAuftrag();  // Nadeldreheinrichtung aus
                y32.resAuftrag(); // Unterfadenschieber aus
                y6.resAuftrag();  // Zusatzfadenspannung aus
                saf->buf = nup.workbuf;
                hfuss.akt_posunten = saf->md.posunten;
                hfuss.akt_hub = saf->md.hub;
                if ( saf->md.prsel == 2 )
                {
                    /* EasyCODE ( 0 
                    verbundene Programme */
                    if ( saf->md.next_pnrindex != 0 )
                    {
                        saf->md.pnrindex = saf->md.next_pnrindex;
                        nup.getprpar ();
                        nup.workbuf_res ();
                        saf->buf = nup.workbuf;
                        abl.set_anzauftrag(7);
                        fwdkey = 1;
                        ablaufkey = 10;
                    }
                    else
                    {
                        switch ( nup.napos () )
                        {
                        case 0 :
                            break;
                        case 1 :
                            koor.nae_err = 1;
                            errpar = nminf.fehler;
                            key = 0;
                            break;
                        default:
                            if ( ver.move_sonst() )
                            {
                                saf->md.pnrindex = saf->md.next_pnrindex;
                                nup.getprpar ();
                                nup.workbuf_res ();
                                saf->buf = nup.workbuf;
                                abl.set_anzauftrag(7);
                                nup.koorsoll[0].xbild = nup.workbuf.koorist.xbild;
                                nup.koorsoll[0].motorx = nup.workbuf.koorist.motorx;
                                nup.koorsoll[0].ybild = saf->lp.ybild;
                                nup.koorsoll[0].motory = saf->lp.motory;
                                saf->upfahren_dis = TRUE;
                                nup.carmov (FAST);
                                fwdkey = 12;
                            }
                            else
                            {
                                key = 0;
                            }
                            break;
                        }
                    }
                    break;
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    sonst */
                    switch ( nup.napos () )
                    {
                    case 0 :
                        break;
                    case 1 :
                        koor.nae_err = 1;
                        errpar = nminf.fehler;
                        key = 0;
                        break;
                    default:
                        if ( ver.move_sonst() )
                        {
                            if ( saf->md.prsel == 1 )
                            {
                                saf->md.pnrindex = saf->md.next_pnrindex;
                                nup.getprpar ();
                                nup.workbuf_res ();
                                saf->buf = nup.workbuf;
                                abl.set_anzauftrag(7);
                            }
                            nup.koorsoll[0].xbild = nup.workbuf.koorist.xbild;
                            nup.koorsoll[0].motorx = nup.workbuf.koorist.motorx;
                            nup.koorsoll[0].ybild = saf->lp.ybild;
                            nup.koorsoll[0].motory = saf->lp.motory;
                            saf->upfahren_dis = TRUE;
                            nup.carmov (FAST);
                            fwdkey = 12;
                        }
                        else
                        {
                            key = 0;
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 12 :
        /* EasyCODE ( 0 
        fwdkey = 12,   Warten bis LP erreicht, ev. Klammer auf */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            nup.carlp = TRUE;
            if ( saf->md.openclampen && !demomodus )
            {
                y51u1.setAuftrag();  // Klammerverriegelung auf
                schabl.bc.res_barcode();
                schabl.rf.delete_rfidprognummer();
                res_barcode();
            }
            ablaufkey = 0;
            fwdkey = 20;
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 15 :
        /* EasyCODE ( 0 
        fwdkey = 15,  Einlegestopp */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( saf->cutready )
            {
                if ( y50u1.fertig() && y50u2.fertig() )
                {
                    if ( e50u1.getZustand() )
                    {
                        // Oberteil angehoben
                        /* EasyCODE ( 0 
                        auf LP fahren */
                        switch ( nup.napos () )
                        {
                        case 0 :
                            break;
                        case 1 :
                            koor.nae_err = 1;
                            errpar = nminf.fehler;
                            key = 0;
                            break;
                        default:
                            if ( ver.move_sonst () )
                            {
                                nup.koormerker = nup.workbuf.koorist;
                                nup.koorsoll[0].xbild = nup.workbuf.koorist.xbild;
                                nup.koorsoll[0].motorx = nup.workbuf.koorist.motorx;
                                nup.koorsoll[0].ybild = saf->lp.ybild;
                                nup.koorsoll[0].motory = saf->lp.motory;
                                fwdkey = 16;
                                nup.carmov (FAST);
                            }
                            else
                            {
                                key = 0;
                            }
                            break;
                        }
                        /* EasyCODE ) */
                    }
                    else
                    {
                        // Oberteil heben
                        y50u1.setAuftrag();
                    }
                }
            }
            else
            {
                koor.nae_err = 23;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 16 :
        /* EasyCODE ( 0 
        fwdkey = 16,   Einlegestopp,  Warten bis Schlitten im LP, carlp */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            nup.carlp = TRUE;
            ablaufkey = 42;
            key = 0;
        }
        else
        {
            if ( koor.notstop() )
            {
                // verriegelt
                smx.stop();
                smy.stop();
                nae.homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 17 :
        /* EasyCODE ( 0 
        fwdkey = 17,  Einlegestopp -> Fahren auf Stopp-Position starten */
        if ( !ver.move_sonst() )
        {
            // verriegelt
            key = 0;
        }
        else
        {
            // nicht verriegelt
            nup.koorsoll [0] = nup.koormerker;
            nup.carmov (FAST);
            nup.carlp = FALSE;
            fwdkey = 18;
        }
        break;
        /* EasyCODE ) */
    case 18 :
        /* EasyCODE ( 0 
        fwdkey = 18,  Einlegestopp -> warten bis Stopp-Position erreicht */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            /* EasyCODE ( 0 
            Nadel positionieren, Fuss auf, Niederhalter auf,
            Klemme zu, Fadenspannung zu,
            Oberteil senken */
            switch ( nup.napos() )
            {
            case 0 :
                break;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                key = 0;
                break;
            default:
                if ( hfuss_auf.fertig() && y8.fertig() &&
                     y4.fertig() && y5.fertig() )
                {
                    if ( (hfuss.getpos().pos == saf->md.posoben.pos) && !y8.getZustand() )
                    {
                        // Fuss oben
                        y50u2.setAuftrag();  // Oberteil senken
                        ablaufkey = 20;
                        naekey = 0;
                        fwdkey = 20;
                    }
                    else
                    {
                        hfuss.auf();      // Fuss auf
                        y4.setAusgang();  // Klemme zu
                        y5.resAusgang();  // Fadenspannung zu
                        y8.resAuftrag();  // Niederhalter auf
                    }
                }
                else
                {
                    if ( koor.naestop() )
                    {
                        key = 0;
                    }
                }
                break;
            }
            /* EasyCODE ) */
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 20 :
        /* EasyCODE ( 0 
        fwdkey = 20,   Warten bis keine Taste gedrueckt */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( demomodus )
            {
                fwdkey = 0;
            }
            else
            {
                if ( (keyb.taste() == 0xff) && abl.anzauftrag_ready() )
                {
                    if ( (ablaufkey == 15) || (ablaufkey == 26) )
                    {
                        abl.set_anzauftrag(16);
                    }
                    else
                    {
                        abl.set_anzauftrag(17);
                    }
                    wartenkey = 1;
                    key = 2;
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
KlasseNaehen::fstep */
/* EasyCODE F */
unsigned char KlasseNaehen::fstep (void)
{
    unsigned char i;
    float x;
    switch ( fstepkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        fstepkey = 0,   Schritt starten */
        if ( abl.anzauftrag_ready() )
        {
            fwdctrl ();
            if ( nup.mbef[XYVALID] )
            {
                /* EasyCODE ( 0 
                Koordinaten */
                nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild + nup.deltax;
                nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild + nup.deltay;
                if ( !nup.poscheck (&nup.koorsoll [0]) )
                {
                    koor.nae_err = 11;
                    return 1;
                }
                else
                {
                    nup.bitomo (&nup.koorsoll [0]);
                    if ( nup.workbuf.nbtart == 0 )
                    {
                        // Transportieren
                        fstepkey = 2;
                        return 0;
                    }
                    else
                    {
                        if ( y10.getZustand() )
                        {
                            // Annaehen nach
                            // Band einlegen
                            y50u2.setAuftrag();  // Oberteil senken SW002
                            y8.setAuftrag();     // Niederhalter ab
                            hfuss.ab();          // Huepferfuss ab
                            fstepkey = 10;
                            return 0;
                        }
                        else
                        {
                            if ( (nup.deltax*nup.deltax + nup.deltay*nup.deltay) >
                                 ((STLG_MAX+3)*(STLG_MAX+3)) )
                            {
                                koor.nae_err = 12;
                                return 1;
                            }
                            else
                            {
                                if ( e50u1.getZustand() )
                                {
                                    // Oberteil oben
                                    fstepkey = 7;
                                }
                                else
                                {
                                    fstepkey = 1;
                                }
                                return 0;
                            }
                        }
                    }
                }
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                M-Befehl */
                if ( nup.mbef[PROGENDE] )
                {
                    saf->upfahren_dis = TRUE;
                    saf->buf = nup.workbuf;
                    ablaufkey = 25;
                    return 1;
                }
                else
                {
                    if ( nup.mbef[PROGSTOP] || (nup.mbef[ANNAEHEN] && !saf->cutready) )
                    {
                        saf->buf = nup.workbuf;
                        if ( nup.mbef[PROGSTOP] &&
                             ((nup.workbuf.naeout&0x20) == 0x20) )
                        {
                            return 2;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        if ( nup.mbef [UFWSTOP] && (saf->variante == 1) )
                        {
                            saf->buf = nup.workbuf;
                            y50u2.setAuftrag();  // Oberteil senken
                            y8.setAuftrag();     // Niederhalter ab
                            y10.setAuftrag();    // Bandanschlag ab
                            return 1;
                        }
                        else
                        {
                            for ( i=OUT1SET; i<=ZUSPGOFF; i++ )
                            {
                                if ( nup.mbef [i] )
                                {
                                    nup.nasere ();
                                    saf->buf = nup.workbuf;
                                    return 0xff;
                                }
                            }
                            for ( i=INHIGH1; i<=INLOW2; i++ )
                            {
                                if ( nup.mbef [i] )
                                {
                                    abl.set_anzauftrag(8);
                                    fstepkey = 5;
                                    return 0;
                                }
                            }
                            if ( nup.mbef[DELAY] )
                            {
                                abl.set_anzauftrag(9);
                                tnae1.rsz();
                                tnae1.sz(nup.verzzt);
                                fstepkey = 6;
                                return 0;
                            }
                            else
                            {
                                if ( nup.mbef[HFNIVEAU] ||
                                     nup.mbef[HFHUB] ||
                                     nup.mbef[HFNIVEAULEV] ||
                                     nup.mbef[HFHUBLEV] )
                                {
                                    /* EasyCODE ( 0 
                                    Huepferfuss-
                                    Niveau */
                                    if ( hfuss.getpos().pos != saf->md.posoben.pos )
                                    {
                                        hfuss.ab();
                                    }
                                    saf->buf = nup.workbuf;
                                    abl.set_anzauftrag(17);
                                    /* EasyCODE ) */
                                    return 0xff;
                                }
                                else
                                {
                                    saf->buf = nup.workbuf;
                                    return 0;
                                }
                            }
                        }
                    }
                }
                /* EasyCODE ) */
            }
        }
        else
        {
            return 0;
        }
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        fstepkey = 1,   1 Stich vorwaerts takten */
        if ( koor.naestop() || !ver.takt() )
        {
            return 1;
        }
        else
        {
            switch ( saf->md.drehmodus )
            {
            case 0 :
                // Modus: ausgeschaltet
                break;
            case 1 :
                /* EasyCODE ( 0 
                Modus: Automatik
                Unterfadenschieber setzen/ruecksetzen */
                /* EasyCODE_STRUCTURED_COMMENT_START */
                /*
                if ( (nup.workbuf.flag & 0x01) == 0 )
                {
                    if ( ((nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild) > 0) &&
                         ((nup.koorsoll[0].ybild - nup.workbuf.koorist.ybild) < 0) &&
                         (saf->md.nadelanzahl == 1) )
                    {
                        x = (float) (nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild)/
                            (-nup.koorsoll[0].ybild + nup.workbuf.koorist.ybild);
                        if ( !y7.getZustand() && !y32.getZustand() )
                        {
                            if ( (atan(x) >= saf->fadschein_winkel1) &&
                                 (atan(x) <= saf->fadschein_winkel2) )
                            {
                                // Nadeldreheinrichtung ein
                                // Unterfadenschieber ein
                                y7.setAuftrag();
                                y32.setAuftrag();
                            }
                        }
                        else
                        {
                            if ( (atan(x) <= saf->fadschaus_winkel1) ||
                                 (atan(x) >= saf->fadschaus_winkel2) )
                            {
                                // Nadeldreheinrichtung aus
                                // Unterfadenschieber aus
                                y7.resAuftrag();
                                y32.resAuftrag();
                            }
                        }
                    }
                    else
                    {
                        // Nadeldreheinrichtung aus
                        // Unterfadenschieber aus
                        y7.resAuftrag();
                        y32.resAuftrag();
                    }
                    //
                }
                */
                /* EasyCODE_STRUCTURED_COMMENT_END */
                //  SW014  Mit Hilfsfunktion
                if ( (nup.workbuf.flag & 0x01) == 0 )
                {
                    if ( (saf->md.nadelanzahl == 1) )
                    {
                        if ( !y7.getZustand() && !y32.getZustand() )
                        {
                            if ( nup.nadeldreh (CHKEIN, nup.koorsoll[0].xbild, nup.koorsoll[0].ybild, 
                                                        nup.workbuf.koorist.xbild, nup.workbuf.koorist.ybild ) )
                            {
                                // Nadeldreheinrichtung ein
                                // Unterfadenschieber ein
                                y7.setAuftrag();
                                y32.setAuftrag();
                            }
                        }
                        else
                        {
                            if ( nup.nadeldreh (CHKAUS, nup.koorsoll[0].xbild, nup.koorsoll[0].ybild, 
                                                        nup.workbuf.koorist.xbild ,nup.workbuf.koorist.ybild ) )
                            {
                                // Nadeldreheinrichtung aus
                                // Unterfadenschieber aus
                                y7.resAuftrag();
                                y32.resAuftrag();
                            }
                        }
                    }
                    else
                    {
                        // Nadeldreheinrichtung aus
                        // Unterfadenschieber aus
                        y7.resAuftrag();
                        y32.resAuftrag();
                    }
                }
                /* EasyCODE ) */
                break;
            case 2 :
                // Modus: Manuell
                break;
            default:
                /* EasyCODE ( 0 
                Modus: Automatik und Manuell
                Unterfadenschieber setzen/ruecksetzen */
                if ( !y7.getZustand() && !y32.getZustand() )
                {
                    /* EasyCODE ( 0 
                    Nadeldreheinrichtung und
                    Unterfadenschieber ausgeschaltet */
                    if ( saf->md.nadelanzahl == 1 )
                    {
                        if ( out1.getZustand() )
                        {
                            // Nadeldreheinrichtung ein
                            // Unterfadenschieber ein
                            y7.setAuftrag();
                            y32.setAuftrag();
                        }
                        else
                        {
/* EasyCODE_STRUCTURED_COMMENT_START */
                            /*
                            if ( (nup.workbuf.flag & 0x01) == 0 )
                            {
                                if ( ((nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild) > 0) &&
                                     ((nup.koorsoll[0].ybild - nup.workbuf.koorist.ybild) < 0) )
                                {
                                    x = (float) (nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild)/
                                        (-nup.koorsoll[0].ybild + nup.workbuf.koorist.ybild);
                                    if ( (atan(x) >= saf->fadschein_winkel1) &&
                                         (atan(x) <= saf->fadschein_winkel2) )
                                    {
                                        // Nadeldreheinrichtung ein
                                        // Unterfadenschieber ein
                                        y7.setAuftrag();
                                        y32.setAuftrag();
                                    }
                                }
                            }
                            */
                            /* EasyCODE_STRUCTURED_COMMENT_END */
                            //  SW014  Mit Hilfsfunktion
                            if ( (nup.workbuf.flag & 0x01) == 0 )
                            {
                                if ( nup.nadeldreh (CHKEIN, nup.koorsoll[0].xbild, nup.koorsoll[0].ybild, 
                                                            nup.workbuf.koorist.xbild, nup.workbuf.koorist.ybild ) )
                                {
                                    // Nadeldreheinrichtung ein
                                    // Unterfadenschieber ein
                                    y7.setAuftrag();
                                    y32.setAuftrag();
                                }
                            }
                        }
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Nadeldreheinrichtung und
                    Unterfadenschieber eingeschaltet */
                    if ( out1.getZustand() )
                    {
                    }
                    else
                    {
/* EasyCODE_STRUCTURED_COMMENT_START */
                        /*
                        if ( (nup.workbuf.flag & 0x01) == 0 )
                        {
                            if ( ((nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild) > 0) &&
                                 ((nup.koorsoll[0].ybild - nup.workbuf.koorist.ybild) < 0) )
                            {
                                x = (float) (nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild)/
                                    (-nup.koorsoll[0].ybild + nup.workbuf.koorist.ybild);
                                if ( (atan(x) <= saf->fadschaus_winkel1) ||
                                     (atan(x) >= saf->fadschaus_winkel2) )
                                {
                                    // Nadeldreheinrichtung aus
                                    // Unterfadenschieber aus
                                    y7.resAuftrag();
                                    y32.resAuftrag();
                                }
                            }
                            else
                            {
                                // Nadeldreheinrichtung aus
                                // Unterfadenschieber aus
                                y7.resAuftrag();
                                y32.resAuftrag();
                            }
                        }
                        else
                        {
                            // Nadeldreheinrichtung aus
                            // Unterfadenschieber aus
                            y7.resAuftrag();
                            y32.resAuftrag();
                        }
                        */
                        /* EasyCODE_STRUCTURED_COMMENT_END */
                        //  SW014  Mit Hilfsfunktion
                        if ( (nup.workbuf.flag & 0x01) == 0 )
                        {
                            if ( nup.nadeldreh (CHKAUS, nup.koorsoll[0].xbild, nup.koorsoll[0].ybild, 
                                                        nup.workbuf.koorist.xbild ,nup.workbuf.koorist.ybild ) )
                            {
                                // Nadeldreheinrichtung aus
                                // Unterfadenschieber aus
                                y7.resAuftrag();
                                y32.resAuftrag();
                            }
                        }
                        else
                        {
                            // Nadeldreheinrichtung aus
                            // Unterfadenschieber aus
                            y7.resAuftrag();
                            y32.resAuftrag();
                        }
                    }
                    /* EasyCODE ) */
                }
                /* EasyCODE ) */
                break;
            }
            nup.carmov (TAKTEN);
            fstepkey = 4;
            return 0;
        }
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        fstepkey = 2,   Nadel positionieren , ev. Oberteil heben, Fuss auf,
                        Fadenspannung auf, StoffgegendrÅcker ab */
        if ( koor.naestop() )
        {
            return 1;
        }
        else
        {
            switch ( nup.napos () )
            {
            case 0 :
                return 0;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                return 1;
            default:
                /* EasyCODE ( 0 
                Oberteil, Fuss, Klemme, Fadenspannung */
                if ( (nup.workbuf.naeout & 0x20) == 0x20 )
                {
                    // Oberteil heben
                    y50u1.setAuftrag();
                }
                else
                {
                    // Oberteil senken
                    y50u2.setAuftrag();
                }
                if ( saf->cutready )
                {
                    y4.setAuftrag();  // Klemme zu
                    y5.resAuftrag();  // Fadenspannung zu
                }
                else
                {
                    y4.resAuftrag();  // Klemme auf
                    y5.resAuftrag();  // Fadenspannung auf
                }
                hfuss.auf();       // Fuss auf
                y8.resAuftrag();   // Niederhalter auf
                y33.resAuftrag();  // StoffgegendrÅcker ab
                fstepkey = 3;
                return 0;
                /* EasyCODE ) */
            }
        }
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        fstepkey = 3,   Transport starten */
        if ( koor.naestop() )
        {
            return 1;
        }
        else
        {
            if ( hfuss_auf.fertig() && y4.fertig() && y5.fertig() &&
                 y8.fertig() && y50u1.fertig() && y50u2.fertig() &&
                 y33.fertig() )
            {
                if ( ver.move_nae() )
                {
                    nup.carmov(FAST);
                    fstepkey = 4;
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 0;
            }
        }
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        fstepkey = 4,   Warten bis Schritt ausgefuehrt */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            return 0xff;
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        fstepkey = 5,   Warten auf Eingang */
        if ( abl.anzauftrag_ready() )
        {
            if ( koor.naestop() )
            {
                abl.set_anzauftrag(14);
                return 1;
            }
            else
            {
                if ( (nup.mbef[INHIGH1] && in1.getZustand()) || (nup.mbef[INHIGH2] && in2.getZustand()) ||
                     (nup.mbef[INLOW1] && !in1.getZustand()) || (nup.mbef[INLOW2] && !in2.getZustand()) )
                {
                    abl.set_anzauftrag(14);
                    saf->buf = nup.workbuf;
                    return 0xff;
                }
                else
                {
                    if ( keyb.taste() == TFWD )
                    {
                        /* EasyCODE < */
                        // FWD gedrueckt
                        /* EasyCODE > */
                        return 0;
                    }
                    else
                    {
                        abl.set_anzauftrag(14);
                        return 1;
                    }
                }
            }
        }
        else
        {
            return 0;
        }
        /* EasyCODE ) */
    case 6 :
        /* EasyCODE ( 0 
        fstepkey = 6,   Warten auf Zeit */
        if ( abl.anzauftrag_ready() )
        {
            if ( koor.naestop() )
            {
                abl.set_anzauftrag(15);
                return 1;
            }
            else
            {
                if ( tnae1.tz() )
                {
                    abl.set_anzauftrag(15);
                    saf->buf = nup.workbuf;
                    return 0xff;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            return 0;
        }
        /* EasyCODE ) */
    case 7 :
        /* EasyCODE ( 0 
        fstepkey = 7,   Oberteil senken vor Stich vorwaerts takten */
        if ( koor.naestop() )
        {
            return 1;
        }
        else
        {
            switch ( nup.napos() )
            {
            case 0 :
                return 0;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                return 1;
            default:
                if ( y4.fertig() && y5.fertig() &&
                     y8.fertig() && hfuss_auf.fertig() )
                {
                    if ( (hfuss.getpos().pos == saf->md.posoben.pos) && !y8.getZustand() )
                    {
                        // Fuss oben
                        y50u2.setAuftrag();  // Oberteil senken
                        fstepkey = 8;
                    }
                    else
                    {
                        hfuss.auf();      // Fuss auf
                        y4.setAuftrag();  // Klemme zu
                        y5.resAuftrag();  // Fadenspannung zu
                        y8.resAuftrag();  // Niederhalter auf
                    }
                }
                return 0;
            }
        }
        /* EasyCODE ) */
    case 8 :
        /* EasyCODE ( 0 
        fstepkey = 8,   Fertigmeldung vor Stich vorwaerts takten */
        if ( koor.naestop() )
        {
            return 1;
        }
        else
        {
            if ( y50u2.fertig() )
            {
                fstepkey = 1;
            }
            return 0;
        }
        /* EasyCODE ) */
    case 10 :
        /* EasyCODE ( 0 
        fstepkey = 10,   Bandanschlag auf */
        if ( hfuss.ready() )
        {
            y10.resAuftrag();   // Bandanschlag auf
            fstepkey = 11;
            return 0;
        }
        else
        {
            if ( koor.naestop() )
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        /* EasyCODE ) */
    case 11 :
        /* EasyCODE ( 0 
        fstepkey = 11,   Bandanschlag fertig ---> naehstart */
        if ( y10.fertig() )
        {
            return 0xff;
        }
        else
        {
            if ( koor.naestop() )
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        /* EasyCODE ) */
    default:
        return 1;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaehen::fwdctrl */
/* EasyCODE F */
void KlasseNaehen::fwdctrl (void)
{
    switch ( nup.workbuf.nbtart )
    {
    case 0 :
        nup.dfetnsew ();
        if ( nup.mbef[PROGENDE] )
        {
            nup.workbuf.nbtart = 4;
        }
        else
        {
            if ( nup.mbef[ANNAEHEN] )
            {
                nup.workbuf.nbtart = 1;
                y33.setAuftrag();   // Stoffgegendruecker auf
            }
        }
        break;
    case 1 :
        nup.dfetnsew ();
        if ( nup.mbef[SCHNEIDEN] || nup.mbef[NAEHENAUS] )
        {
            nup.workbuf.nbtart = 0;
            y33.resAuftrag();   // Stoffgegendruecker ab
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
Rueckwaerts Takten */
/* EasyCODE ( 0 
KlasseNaehen::back */
/* EasyCODE F */
void KlasseNaehen::back (void)
{
    unsigned char *datpoi_mem;
    switch ( backkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        backey = 0,   verzweigen abhaengig von ablaufkey */
        switch ( ablaufkey )
        {
        case 0 :
            backkey = 1;
            break;
        case 15 :
            backkey = 3;
            break;
        case 16 :
            backkey = 5;
            break;
        case 20 :
            backkey = 6;
            break;
        case 25 :
            backkey = 11;
            break;
        case 26 :
            /* EasyCODE = */
        case 35 :
            backkey = 12;
            break;
        case 40 :
            dauertakten = FALSE;
            dauercount = 0;
            tnae2.rsz();
            tnae2.sz(saf->tadelzt);
            bstepkey = 0;
            backkey = 10;
            break;
        case 42 :
            backkey = 18;
            break;
        default:
            key = 0;
            break;
        }
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        backey = 1,   Schlitten steht im MNP, Fahren auf  EP starten */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( abl.anzauftrag_ready()  /* SW015 */ )
            {
                if ( saf->md.prsel == 2 )
                {
                    /* EasyCODE ( 0 
                    verbundene Programme */
                    for ( saf->md.pnrindex = PNRINDEX_MAX; saf->md.pnrindex > 0; saf->md.pnrindex-- )
                    {
                        if ( saf->md.vpnr_array[saf->md.vpnr][saf->md.pnrindex] != 0xffff )
                        {
                            break;
                        }
                    }
                    saf->md.next_pnrindex = 0;
                    nup.getprpar ();
                    nup.workbuf_res ();
                    saf->buf = nup.workbuf;
                    abl.set_anzauftrag(7);
                    /* EasyCODE ) */
                }
                else
                {
                    if ( saf->md.jigctren == 2 )
                    {
                        /* EasyCODE ( 0 
                        Programm lesen und laden bei
                        automatischer Programmnummerwahl */
                        switch ( nup.read_jigpnr () )
                        {
                        case 0 :
                            /* EasyCODE ( 0 
                            Programm lesen */
                            switch ( di.readprog_to_ems () )
                            {
                            case 1 //Programm unvollstaendig
                            :
                                /*
                                koor.eing_err = 42;
                                key = 0;
                                return;
                                */
                                // Programm unvollstaendig (nun automatisch vervollstaendigen)
                                if ( nup.Prog_vervollst()   // SW015
                                   )
                                {
                                    // koor.eing_err, eing.errpar und eing.errpar2 wurden gesetzt
                                    key = 0;
                                    return ;
                                }
                                else
                                {
                                    // Kein Fehler, weiter machen ohne break
                                    abl.set_anzauftrag(2);  // SW015
                                }
                            case 6 :
                                // nur Stichdaten
                            case 0 //ok
                            :
                                pnrmem = saf->md.pnr;
                                nup.getprpar ();
                                nup.workbuf_res ();
                                saf->buf = nup.workbuf;
                                break;
                            case 2 //Programm zu gross
                            :
                                koor.eing_err = 43;
                                key = 0;
                                return ;
                            case 3 //Programm nicht vorhanden
                            :
                                koor.eing_err = 44;
                                key = 0;
                                return ;
                            case 5 :
                                // Programm nicht fÅr diese Maschine
                                koor.eing_err = 47;
                                key = 0;
                                return ;
                            default:
                                // Flash-Lesefehler oder Prog. fehlerhaft
                                koor.eing_err = 45;
                                key = 0;
                                return ;
                            }
                            /* EasyCODE ) */
                            break;
                        case 1 :
                            // keine Klammer eingelegt,
                            // Programmnummer konnte nicht
                            // gelesen werden
                            koor.nae_err = 17;
                            key = 0;
                            return ;
                        case 2 :
                            // Programmnummer ungueltig
                            koor.nae_err = 18;
                            key = 0;
                            return ;
                        case 3 :
                            // RFID-Lesen nicht fertig
                            return ;
                        default:
                            // RFID-Fehler
                            koor.nae_err = 2;
                            errpar = schabl.rf.rfid_err;
                            key = 0;
                            return ;
                        }
                        /* EasyCODE ) */
                    }
                }
                /* EasyCODE ( 0 
                Schablonenkontrolle */
                switch ( nup.jig_ctrl () )
                {
                case 0 :
                    // falscher Code
                    koor.nae_err = 20;
                    key = 0;
                    break;
                case 1 :
                    // Code ok
                    if ( ver.move_sonst() )
                    {
                        /* EasyCODE ( 0 
                        in Endpunkt fahren */
                        nup.koorsoll[0].xbild = *((int *)(nup.stianfadr + SEP_X));
                        nup.koorsoll[0].ybild = *((int *)(nup.stianfadr + SEP_Y));
                        if ( nup.poscheck (&nup.koorsoll[0]) )
                        {
                            if ( e79.getZustand() || e80.getZustand() // SW013 begin
                               )
                            {
                                SchablVorhanden = TRUE;
                            }
                            else
                            {
                                SchablVorhanden = FALSE;
                            }
                            nup.bitomo (&nup.koorsoll [0]);
                            nup.carmov (FAST);
                            nup.carlp = FALSE;
                            backkey = 2;
                        }
                        else
                        {
                            koor.nae_err = 11;
                            key = 0;
                        }
                        /* EasyCODE ) */
                    }
                    else
                    {
                        key = 0;
                    }
                    break;
                case 2 :
                    // Lesen noch nicht fertig
                    break;
                default:
                    // RFID-Fehler
                    koor.nae_err = 2;
                    errpar = schabl.rf.rfid_err;
                    key = 0;
                    break;
                }
                /* EasyCODE ) */
            }
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        backkey = 2,   Warten bis EP erreicht */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            saf->upfahren_dis = TRUE;
            ablaufkey = 35;
            backkey = 20;
            if ( SchablVorhanden && (!e79.getZustand() || !e80.getZustand()) )
            {
                // SW013    SW005DIS 
                koor.nae_err = 40;
                if ( !e80.getZustand() )
                {
                    nae.errpar = E80_UF1;
                }
                else
                {
                    nae.errpar = E79_UF1;
                }
                homereq = TRUE;
                key = 0;
            }
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        backey = 3,   Fahren vom AP zum LP starten bzw.
                      zum EP des vorhergehenden Programms bei verbundenen Programmen */
        if ( koor.naestop() )
        {
            backkey = 20;
        }
        else
        {
            if ( y50u2.fertig() && hfuss.ready() && abl.anzauftrag_ready() )
            {
                if ( saf->md.prsel == 2 )
                {
                    /* EasyCODE ( 0 
                    verbundene Programme */
                    if ( saf->md.pnrindex == 0 )
                    {
                        saf->md.next_pnrindex = 0;
                    }
                    else
                    {
                        if ( ver.move_sonst() )
                        {
                            saf->md.next_pnrindex = saf->md.pnrindex;
                            saf->md.pnrindex--;
                        }
                        else
                        {
                            key = 0;
                            break;
                        }
                    }
                    nup.getprpar ();
                    nup.workbuf_res ();
                    saf->buf = nup.workbuf;
                    abl.set_anzauftrag(7);
                    if ( saf->md.next_pnrindex != 0 )
                    {
                        nup.koorsoll [0].xbild = *((int *)(nup.stianfadr + SEP_X));
                        nup.koorsoll [0].ybild = *((int *)(nup.stianfadr + SEP_Y));
                        if ( nup.poscheck (&nup.koorsoll [0]) )
                        {
                            nup.bitomo (&nup.koorsoll [0]);
                            nup.carmov (FAST);
                            nup.carlp = FALSE;
                            backkey = 2;
                        }
                        else
                        {
                            koor.nae_err = 11;
                            homereq = TRUE;
                            key = 0;
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( ver.move_sonst () )
                {
                    /* EasyCODE ( 0 
                    in LP fahren */
                    if ( saf->md.prsel == 1 )
                    {
                        saf->md.pnrindex = saf->md.next_pnrindex;
                        nup.getprpar();
                        nup.workbuf_res();
                        saf->buf = nup.workbuf;
                        hfuss.akt_posunten = saf->md.posunten;
                        hfuss.akt_hub = saf->md.hub;
                        abl.set_anzauftrag(7);
                    }
                    nup.koorsoll[0].xbild = nup.workbuf.koorist.xbild;
                    nup.koorsoll[0].motorx = nup.workbuf.koorist.motorx;
                    nup.koorsoll[0].ybild = saf->lp.ybild;
                    nup.koorsoll[0].motory = saf->lp.motory;
                    nup.carmov(FAST);
                    backkey = 4;
                    /* EasyCODE ) */
                }
                else
                {
                    key = 0;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        backkey = 4,   Warten bis LP erreicht */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            nup.carlp = TRUE;
            if ( saf->md.openclampen )
            {
                y51u1.setAuftrag();  // Schablonenverriegelung auf
                schabl.bc.res_barcode();
                schabl.rf.delete_rfidprognummer();
                res_barcode();
            }
            ablaufkey = 0;
            backkey = 20;
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        backkey = 5, Oberteil heben */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            switch ( nup.napos() )
            {
            case 0 :
                break;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                key = 0;
                break;
            default:
                y50u1.setAuftrag();   // Oberteil heben
                hfuss.auf();          // Fuss auf
                y4.setAuftrag();      // Klemme zu
                y5.resAuftrag();      // Fadenspannung zu
                y8.resAuftrag();      // Niederhalter auf
                ablaufkey = 15;
                backkey = 20;
                break;
            }
        }
        break;
        /* EasyCODE ) */
    case 6 :
        /* EasyCODE ( 0 
        backkey = 6,   Faden geschnitten ? */
        if ( saf->cutready || (nup.workbuf.nbtart == 0) )
        {
            if ( nup.workbuf.datpoi == nup.stianfadr + SNPSTART )
            {
                /* EasyCODE ( 0 
                AP erreicht */
                nup.workbuf_res ();
                y7.resAuftrag();  // Nadeldreheinrichtung aus
                y32.resAuftrag(); // Unterfadenschieber aus
                y4.resAuftrag();  // 2. Niveau Huepferfuss aus
                y6.resAuftrag();  // Zusatzfadenspannung aus
                saf->buf = nup.workbuf;
                backkey = 5;
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                Takten durchs Naehprogramm
                starten */
                dauertakten = FALSE;
                dauercount = 0;
                bstepkey = 0;
                backkey = 10;
                tnae2.rsz();
                if ( fastcount == 0 )
                {
                    tnae2.sz(saf->tadelzt);
                }
                else
                {
                    tnae2.sz(saf->taktzt);
                }
                /* EasyCODE ) */
            }
        }
        else
        {
            y7.resAuftrag();  // Nadeldreheinrichtung aus
            y32.resAuftrag(); // Unterfadenschieber aus
            backkey = 7;
        }
        break;
        /* EasyCODE ) */
    case 7 :
        /* EasyCODE ( 0 
        backkey = 7,  Fadenschneiden einleiten */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( y7.fertig() && y32.fertig() )
            {
                nup.schneidkey = 0;
                backkey = 8;
            }
        }
        break;
        /* EasyCODE ) */
    case 8 :
        /* EasyCODE ( 0 
        backkey = 8,  Fadenschneiden */
        switch ( nup.schneidablauf () )
        {
        case 0 :
            if ( koor.naestop() || (koor.nae_err != 0) )
            {
                outportb (INTMASK, REF1DI | REF2DI);
                nminf.befehl = 8;
                key = 0;
            }
            break;
        case 1 :
            dauertakten = FALSE;
            dauercount = 0;
            tnae2.rsz();
            tnae2.sz(saf->tadelzt);
            bstepkey = 0;
            backkey = 10;
            break;
        default:
            key = 0;
            break;
        }
        break;
        /* EasyCODE ) */
    case 9 :
        /* EasyCODE ( 0 
        backkey = 9,   nextbstep ? */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( fastcount == 0 )
            {
                /* EasyCODE ( 0 
                normales rueckwaerts takten */
                if ( !dauertakten )
                {
                    if ( keyb.taste() == TBACK )
                    {
                        if ( tnae2.tz() )
                        {
                            if ( (++dauercount >= 50) && abl.anzauftrag_ready() )
                            {
                                stopmerker = FALSE;
                                dauertakten = TRUE;
                                abl.set_anzauftrag(3);
                            }
                            tnae2.rsz();
                            tnae2.sz(saf->taktzt);
                            bstepkey = 0;
                            backkey = 10;
                        }
                    }
                    else
                    {
                        backkey = 20;
                    }
                }
                else
                {
                    if ( stopmerker ||
                         (keyb.taste() == TSTOP) ||
                         tlinks_stop.getZustand() ||
                         trechts_stop.getZustand() )
                    {
                        stopmerker = FALSE;
                        wartenkey = 0;
                        key = 2;
                    }
                    else
                    {
                        if ( tnae2.tz() )
                        {
                            tnae2.rsz();
                            tnae2.sz(saf->taktzt);
                            bstepkey = 0;
                            backkey = 10;
                        }
                    }
                }
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                rueckwaerts takten nach
                Fadenstoerung */
                if ( tnae2.tz() )
                {
                    fastcount --;
                    if ( fastcount == 0 )
                    {
                    }
                    else
                    {
                        tnae2.rsz();
                        tnae2.sz(saf->taktzt);
                        bstepkey = 0;
                        backkey = 10;
                    }
                }
                /* EasyCODE ) */
            }
        }
        break;
        /* EasyCODE ) */
    case 10 :
        /* EasyCODE ( 0 
        backey = 10,   durchs Naehprogramm takten */
        switch ( bstep () )
        {
        case 0 :
            if ( dauertakten &&
                 ((keyb.taste() == TSTOP) ||
                   tlinks_stop.getZustand() ||
                   trechts_stop.getZustand()) )
            {
                stopmerker = TRUE;
            }
            break;
        case 1 :
            key = 0;
            fastcount = 0;
            break;
        case 2 :
            backkey = 16;
            break;
        default:
            backkey = 9;
            break;
        }
        break;
        /* EasyCODE ) */
    case 11 :
        /* EasyCODE ( 0 
        backkey = 11,   Schlitten steht im EP */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            /* EasyCODE < */
            // Oberteil absenken fertig ?
            /* EasyCODE > */
            if ( y50u2.fertig() )
            {
                dauertakten = FALSE;
                dauercount = 0;
                tnae2.rsz();
                tnae2.sz(saf->tadelzt);
                bstepkey = 0;
                backkey = 10;
                ablaufkey = 20;
            }
        }
        break;
        /* EasyCODE ) */
    case 12 :
        /* EasyCODE ( 0 
        backkey = 12,   Oberteil absenken */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            nup.workbuf.datpoi = nup.endadr;
            nup.naeout_akt ();
            saf->buf = nup.workbuf;
            backkey = 15;
        }
        break;
        /* EasyCODE ) */
    case 15 :
        /* EasyCODE ( 0 
        backkey = 15,   Warten auf Naeheingaenge, Oberteil senken */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( y6.fertig() && hfuss.ready() )
            {
                if ( saf->md.jigctren == 2 )
                {
                    switch ( nup.read_jigpnr () )
                    {
                    case 0 :
                        if ( pnrmem != saf->md.pnr )
                        {
                            koor.nae_err = 19;
                            saf->md.pnr = pnrmem;
                            key = 0;
                        }
                        else
                        {
                            /* EasyCODE ( 0 
                            Nadel positionieren, Fuss auf,
                            Niederhalter auf,
                            Klemme zu, Fadenspannung zu,
                            Oberteil senken */
                            switch ( nup.napos() )
                            {
                            case 0 :
                                break;
                            case 1 :
                                koor.nae_err = 1;
                                errpar = nminf.fehler;
                                key = 0;
                                break;
                            default:
                                if ( y4.fertig() && y5.fertig() &&
                                     y8.fertig() && hfuss_auf.fertig() )
                                {
                                    if ( (hfuss.getpos().pos == saf->md.posoben.pos) &&
                                         !y8.getZustand() )
                                    {
                                        // Fuss oben
                                        y50u2.setAuftrag();  // Oberteil senken
                                        ablaufkey = 25;
                                        backkey = 20;
                                    }
                                    else
                                    {
                                        hfuss.auf();     // Fuss auf
                                        y4.setAusgang(); // Klemme zu
                                        y5.setAusgang(); // Fadenspannung auf
                                        y8.resAuftrag(); // Niederhalter auf
                                    }
                                }
                                break;
                            }
                            /* EasyCODE ) */
                        }
                        break;
                    case 1 :
                        // keine Klammer eingelegt,
                        // Programmnummer konnte nicht
                        // gelesen werden
                        koor.nae_err = 17;
                        key = 0;
                        break;
                    case 2 :
                        // Programmnummer ungueltig
                        koor.nae_err = 18;
                        key = 0;
                        break;
                    case 3 :
                        // noch nicht fertig
                        break;
                    default:
                        // RFID-Fehler
                        koor.nae_err = 2;
                        errpar = schabl.rf.rfid_err;
                        key = 0;
                        break;
                    }
                }
                else
                {
                    /* EasyCODE ( 0 
                    Nadel positionieren, Fuss auf,
                    Niederhalter auf,
                    Klemme zu, Fadenspannung zu,
                    Oberteil senken */
                    switch ( nup.napos() )
                    {
                    case 0 :
                        break;
                    case 1 :
                        koor.nae_err = 1;
                        errpar = nminf.fehler;
                        key = 0;
                        break;
                    default:
                        if ( y4.fertig() && y5.fertig() &&
                             y8.fertig() && hfuss_auf.fertig() )
                        {
                            if ( (hfuss.getpos().pos == saf->md.posoben.pos) &&
                                 !y8.getZustand() )
                            {
                                // Fuss oben
                                y50u2.setAuftrag();  // Oberteil senken
                                ablaufkey = 25;
                                backkey = 20;
                            }
                            else
                            {
                                hfuss.auf();     // Fuss auf
                                y4.setAusgang(); // Klemme zu
                                y5.setAusgang(); // Fadenspannung auf
                                y8.resAuftrag(); // Niederhalter auf
                            }
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 16 :
        /* EasyCODE ( 0 
        backkey = 16,  Einlegestopp */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( y50u1.fertig() && y50u2.fertig() )
            {
                if ( e50u1.getZustand() )
                {
                    // Oberteil angehoben
                    /* EasyCODE ( 0 
                    auf LP fahren */
                    switch ( nup.napos () )
                    {
                    case 0 :
                        break;
                    case 1 :
                        koor.nae_err = 1;
                        errpar = nminf.fehler;
                        key = 0;
                        break;
                    default:
                        if ( ver.move_sonst () )
                        {
                            nup.koormerker = nup.workbuf.koorist;
                            nup.koorsoll[0].xbild = nup.workbuf.koorist.xbild;
                            nup.koorsoll[0].motorx = nup.workbuf.koorist.motorx;
                            nup.koorsoll[0].ybild = saf->lp.ybild;
                            nup.koorsoll[0].motory = saf->lp.motory;
                            backkey = 17;
                            nup.carmov (FAST);
                        }
                        else
                        {
                            key = 0;
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    // Oberteil heben
                    y50u1.setAuftrag();
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 17 :
        /* EasyCODE ( 0 
        backkey = 17,   Einlegestopp,  Warten bis Schlitten im LP, carlp */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            nup.workbuf.datpoi += 3;
            saf->buf = nup.workbuf;
            nup.carlp = TRUE;
            ablaufkey = 42;
            key = 0;
        }
        else
        {
            if ( koor.notstop() )
            {
                // verriegelt
                smx.stop();
                smy.stop();
                nae.homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 18 :
        /* EasyCODE ( 0 
        backkey = 18,  Einlegestopp -> Fahren auf Stopp-Position starten */
        if ( !ver.move_sonst() )
        {
            // verriegelt
            key = 0;
        }
        else
        {
            // nicht verriegelt
            nup.koorsoll [0] = nup.koormerker;
            nup.carmov (FAST);
            nup.carlp = FALSE;
            backkey = 19;
        }
        break;
        /* EasyCODE ) */
    case 19 :
        /* EasyCODE ( 0 
        backkey = 19,  Einlegestopp -> warten bis Stopp-Position erreicht */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            /* EasyCODE ( 0 
            Nadel positionieren, Fuss auf,
            Klemme zu, Fadenspannung auf,
            Oberteil senken */
            switch ( nup.napos() )
            {
            case 0 :
                break;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                key = 0;
                break;
            default:
                if ( koor.naestop() )
                {
                    key = 0;
                }
                else
                {
                    if ( y4.fertig() && y5.fertig() && y6.fertig() &&  // SW016
                         y8.fertig() && hfuss_auf.fertig() )
                    {
                        if ( hfuss.getpos().pos == saf->md.posoben.pos )
                        {
                            /* EasyCODE < */
                            // Fuss oben
                            /* EasyCODE > */
                            nup.workbuf.datpoi -= 3;
                            saf->buf = nup.workbuf;
                            y50u2.setAuftrag();  // Oberteil senken
                            ablaufkey = 20;
                            naekey = 0;
                            backkey = 20;
                        }
                        else
                        {
                            hfuss.auf();      // Fuss auf
                            y4.setAusgang();  // Klemme zu
                            y5.setAusgang();  // Fadenspannung auf
                            y6.resAuftrag();  // SW016
                            y8.resAuftrag();  // Niederhalter auf
                        }
                    }
                }
                break;
            }
            /* EasyCODE ) */
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 20 :
        /* EasyCODE ( 0 
        backkey = 20,   Warten bis keine Taste gedrueckt */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( keyb.taste() == 0xff )
            {
                wartenkey = 1;
                key = 2;
            }
        }
        break;
        /* EasyCODE ) */
    default:
        key = 0;
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaehen::bstep */
/* EasyCODE F */
unsigned char KlasseNaehen::bstep (void)
{
    /* EasyCODE < */
    unsigned char i;
    float x;
    struct p_abschstruct *hlpptr;
    /* EasyCODE > */
    switch ( bstepkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        bstepkey = 0,   Schritt starten */
        if ( nup.workbuf.datpoi == (nup.stianfadr + SNPSTART) )
        {
            nup.workbuf_res ();
            y7.resAuftrag();  // Nadeldreheinrichtung aus
            y32.resAuftrag(); // Unterfadenschieber aus
            y6.resAuftrag();  // Zusatzfadenspannung aus
            saf->buf = nup.workbuf;
            return 1;
        }
        else
        {
            backctrl ();
            if ( nup.mbef[XYVALID] )
            {
                /* EasyCODE ( 0 
                Koordinaten */
                nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild - nup.deltax;
                nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild - nup.deltay;
                if ( !nup.poscheck (&nup.koorsoll [0]) )
                {
                    koor.nae_err = 11;
                    return 1;
                }
                else
                {
                    nup.bitomo (&nup.koorsoll [0]);
                    if ( nup.workbuf.nbtart == 0 )
                    {
                        // Transportieren
                        bstepkey = 2;
                        return 0;
                    }
                    else
                    {
                        if ( (nup.deltax*nup.deltax + nup.deltay*nup.deltay) >
                             ((STLG_MAX+3)*(STLG_MAX+3)) )
                        {
                            koor.nae_err = 12;
                            return 1;
                        }
                        else
                        {
                            if ( e50u1.getZustand() )
                            {
                                // Oberteil oben
                                bstepkey = 5;
                            }
                            else
                            {
                                bstepkey = 1;
                            }
                            return 0;
                        }
                    }
                }
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                M-Befehl */
                if ( nup.mbef[PROGSTOP] || (nup.mbef[UFWSTOP] && (saf->variante==1)) || (nup.mbef[NAEHENAUS] && !saf->cutready) )
                {
                    /* EasyCODE ( 0 
                    Progr. Stop,
                    Naehen aus */
                    if ( nup.mbef[PROGSTOP] &&
                         ((nup.workbuf.naeout & 0x20) == 0x20) )
                    {
                        return 2;
                    }
                    else
                    {
                        if ( nup.mbef[UFWSTOP] && (saf->variante==1) )
                        {
                        }
                        else
                        {
                            saf->buf = nup.workbuf;
                        }
                        return 1;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    if ( nup.mbef[REDGSWON] || nup.mbef[REDGSWOFF] )
                    {
                        /* EasyCODE ( 0 
                        red. Geschw. */
                        nup.workbuf.redgswen = !nup.workbuf.redgswen;
                        saf->buf = nup.workbuf;
                        return 0;
                        /* EasyCODE ) */
                    }
                    else
                    {
                        /* EasyCODE ( 0 
                        Ein- / Ausgaenge */
                        for ( i=OUT1SET; i<=ZUSPGOFF; i++ )
                        {
                            if ( nup.mbef [i] )
                            {
                                nup.nasere_back ();
                                saf->buf = nup.workbuf;
                                return 0xff;
                            }
                        }
                        /* EasyCODE ) */
                        if ( nup.mbef[HFNIVEAU] ||
                             nup.mbef[HFHUB] ||
                             nup.mbef[HFNIVEAULEV] ||
                             nup.mbef[HFHUBLEV] )
                        {
                            nup.hfuss_akt();
                            abl.set_anzauftrag(17);
                        }
                        saf->buf = nup.workbuf;
                        saf->upfahren_dis = FALSE;
                        return 0;
                    }
                }
                /* EasyCODE ) */
            }
        }
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        bstepkey = 1,   1 Stich rueckwaerts starten */
        if ( koor.naestop() || !ver.takt() )
        {
            return 1;
        }
        else
        {
            switch ( saf->md.drehmodus )
            {
            case 0 :
                // Modus: ausgeschaltet
                break;
            case 1 :
                /* EasyCODE_STRUCTURED_COMMENT_START */
                /*
                EasyCODE_COMMENT_START EasyCODE ( 0 
                Modus: Automatik
                Unterfadenschieber setzen/ruecksetzen EasyCODE_COMMENT_END
                if ( (nup.workbuf.flag & 0x01) == 0 )
                {
                    if ( ((nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild) > 0) &&
                         ((nup.koorsoll[0].ybild - nup.workbuf.koorist.ybild) < 0) &&
                         (saf->md.nadelanzahl == 1) )
                    {
                        x = (float) (nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild)/
                            (-nup.koorsoll[0].ybild + nup.workbuf.koorist.ybild);
                        if ( !y7.getZustand() && !y32.getZustand() )
                        {
                            if ( (atan(x) >= saf->fadschein_winkel1) &&
                                 (atan(x) <= saf->fadschein_winkel2) )
                            {
                                // Nadeldreheinrichtung ein
                                // Unterfadenschieber ein
                                y7.setAuftrag();
                                y32.setAuftrag();
                            }
                        }
                        else
                        {
                            if ( (atan(x) <= saf->fadschaus_winkel1) ||
                                 (atan(x) >= saf->fadschaus_winkel2) )
                            {
                                // Nadeldreheinrichtung aus
                                // Unterfadenschieber aus
                                y7.resAuftrag();
                                y32.resAuftrag();
                            }
                        }
                    }
                    else
                    {
                        // Nadeldreheinrichtung aus
                        // Unterfadenschieber aus
                        y7.resAuftrag();
                        y32.resAuftrag();
                    }
                }
                EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                */
                /* EasyCODE_STRUCTURED_COMMENT_END */
                /* EasyCODE ( 0 
                Modus: Automatik
                Unterfadenschieber setzen/ruecksetzen */
                /* EasyCODE_STRUCTURED_COMMENT_START */
                /*
                if ( (nup.workbuf.flag & 0x01) == 0 )
                {
                    if ( (-(nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild) > 0) &&
                         (-(nup.koorsoll[0].ybild - nup.workbuf.koorist.ybild) < 0) &&
                         (saf->md.nadelanzahl == 1) )
                    {
                        x = (float) -(nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild)/
                            -(-nup.koorsoll[0].ybild + nup.workbuf.koorist.ybild);
                        if ( !y7.getZustand() && !y32.getZustand() )
                        {
                            if ( (atan(x) >= saf->fadschein_winkel1) &&
                                 (atan(x) <= saf->fadschein_winkel2) )
                            {
                                // Nadeldreheinrichtung ein
                                // Unterfadenschieber ein
                                y7.setAuftrag();
                                y32.setAuftrag();
                            }
                        }
                        else
                        {
                            if ( (atan(x) <= saf->fadschaus_winkel1) ||
                                 (atan(x) >= saf->fadschaus_winkel2) )
                            {
                                // Nadeldreheinrichtung aus
                                // Unterfadenschieber aus
                                y7.resAuftrag();
                                y32.resAuftrag();
                            }
                        }
                    }
                    else
                    {
                        // Nadeldreheinrichtung aus
                        // Unterfadenschieber aus
                        y7.resAuftrag();
                        y32.resAuftrag();
                    }
                }
                */
                /* EasyCODE_STRUCTURED_COMMENT_END */
                //  SW014   Aufrufe mit neuer Hilfsfunktion
                if ( (nup.workbuf.flag & 0x01) == 0 )
                {
                    if ( (saf->md.nadelanzahl == 1) )
                    {
                        if ( !y7.getZustand() && !y32.getZustand() )
                        {
                            if ( nup.nadeldreh (CHKEIN, nup.workbuf.koorist.xbild, nup.workbuf.koorist.ybild, 
                                                        nup.koorsoll[0].xbild ,nup.koorsoll[0].ybild ) )
                            {
                                // Nadeldreheinrichtung ein
                                // Unterfadenschieber ein
                                y7.setAuftrag();
                                y32.setAuftrag();
                            }
                        }
                        else
                        {
                            if ( nup.nadeldreh (CHKAUS, nup.workbuf.koorist.xbild, nup.workbuf.koorist.ybild, 
                                                        nup.koorsoll[0].xbild ,nup.koorsoll[0].ybild ) )
                            {
                                // Nadeldreheinrichtung aus
                                // Unterfadenschieber aus
                                y7.resAuftrag();
                                y32.resAuftrag();
                            }
                        }
                    }
                    else
                    {
                        // Nadeldreheinrichtung aus
                        // Unterfadenschieber aus
                        y7.resAuftrag();
                        y32.resAuftrag();
                    }
                }
                /* EasyCODE ) */
                break;
            case 2 :
                // Modus: Manuell
                break;
            default:
/* EasyCODE_STRUCTURED_COMMENT_START */
                /*
                EasyCODE_COMMENT_START EasyCODE ( 0 
                Modus: Automatik und Manuell
                Unterfadenschieber setzen/ruecksetzen EasyCODE_COMMENT_END
                if ( !y7.getZustand() && !y32.getZustand() )
                {
                    EasyCODE_COMMENT_START EasyCODE ( 0 
                    Nadeldreheinrichtung und
                    Unterfadenschieber ausgeschaltet EasyCODE_COMMENT_END
                    if ( saf->md.nadelanzahl == 1 )
                    {
                        if ( out1.getZustand() )
                        {
                            // Nadeldreheinrichtung ein
                            // Unterfadenschieber ein
                            y7.setAuftrag();
                            y32.setAuftrag();
                        }
                        else
                        {
                            if ( (nup.workbuf.flag & 0x01) == 0 )
                            {
                                if ( ((nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild) > 0) &&
                                     ((nup.koorsoll[0].ybild - nup.workbuf.koorist.ybild) < 0) )
                                {
                                    x = (float) (nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild)/
                                        (-nup.koorsoll[0].ybild + nup.workbuf.koorist.ybild);
                                    if ( (atan(x) >= saf->fadschein_winkel1) &&
                                         (atan(x) <= saf->fadschein_winkel2) )
                                    {
                                        // Nadeldreheinrichtung ein
                                        // Unterfadenschieber ein
                                        y7.setAuftrag();
                                        y32.setAuftrag();
                                    }
                                }
                            }
                        }
                    }
                    EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                }
                else
                {
                    EasyCODE_COMMENT_START EasyCODE ( 0 
                    Nadeldreheinrichtung und
                    Unterfadenschieber eingeschaltet EasyCODE_COMMENT_END
                    if ( out1.getZustand() )
                    {
                    }
                    else
                    {
                        if ( (nup.workbuf.flag & 0x01) == 0 )
                        {
                            if ( ((nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild) > 0) &&
                                 ((nup.koorsoll[0].ybild - nup.workbuf.koorist.ybild) < 0) )
                            {
                                x = (float) (nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild)/
                                    (-nup.koorsoll[0].ybild + nup.workbuf.koorist.ybild);
                                if ( (atan(x) <= saf->fadschaus_winkel1) ||
                                     (atan(x) >= saf->fadschaus_winkel2) )
                                {
                                    // Nadeldreheinrichtung aus
                                    // Unterfadenschieber aus
                                    y7.resAuftrag();
                                    y32.resAuftrag();
                                }
                            }
                            else
                            {
                                // Nadeldreheinrichtung aus
                                // Unterfadenschieber aus
                                y7.resAuftrag();
                                y32.resAuftrag();
                            }
                        }
                        else
                        {
                            // Nadeldreheinrichtung aus
                            // Unterfadenschieber aus
                            y7.resAuftrag();
                            y32.resAuftrag();
                        }
                    }
                    EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                }
                EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                */
                /* EasyCODE_STRUCTURED_COMMENT_END */
                /* EasyCODE ( 0 
                Modus: Automatik und Manuell
                Unterfadenschieber setzen/ruecksetzen */
                if ( !y7.getZustand() && !y32.getZustand() )
                {
                    /* EasyCODE ( 0 
                    Nadeldreheinrichtung und
                    Unterfadenschieber ausgeschaltet */
                    if ( saf->md.nadelanzahl == 1 )
                    {
                        if ( out1.getZustand() )
                        {
                            // Nadeldreheinrichtung ein
                            // Unterfadenschieber ein
                            y7.setAuftrag();
                            y32.setAuftrag();
                        }
                        else
                        {
                            if ( (nup.workbuf.flag & 0x01) == 0 )
                            {
                                /* EasyCODE_STRUCTURED_COMMENT_START */
                                /*
                                if ( (-(nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild) > 0) &&
                                     (-(nup.koorsoll[0].ybild - nup.workbuf.koorist.ybild) < 0) )
                                {
                                    x = (float) -(nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild)/
                                        -(-nup.koorsoll[0].ybild + nup.workbuf.koorist.ybild);
                                    if ( (atan(x) >= saf->fadschein_winkel1) &&
                                         (atan(x) <= saf->fadschein_winkel2) )
                                    {
                                        // Nadeldreheinrichtung ein
                                        // Unterfadenschieber ein
                                        y7.setAuftrag();
                                        y32.setAuftrag();
                                    }
                                }
                                */
                                /* EasyCODE_STRUCTURED_COMMENT_END */
                                //  SW014
                                if ( nup.nadeldreh (CHKEIN, nup.workbuf.koorist.xbild, nup.workbuf.koorist.ybild, 
                                                            nup.koorsoll[0].xbild ,nup.koorsoll[0].ybild ) )
                                {
                                    // Nadeldreheinrichtung ein
                                    // Unterfadenschieber ein
                                    y7.setAuftrag();
                                    y32.setAuftrag();
                                }
                            }
                        }
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Nadeldreheinrichtung und
                    Unterfadenschieber eingeschaltet */
                    if ( out1.getZustand() )
                    {
                    }
                    else
                    {
                        if ( (nup.workbuf.flag & 0x01) == 0 )
                        {
                            /* EasyCODE_STRUCTURED_COMMENT_START */
                            /*
                            if ( ((nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild) > 0) &&
                                 ((nup.koorsoll[0].ybild - nup.workbuf.koorist.ybild) < 0) )
                            {
                                x = (float) (nup.koorsoll[0].xbild - nup.workbuf.koorist.xbild)/
                                    (-nup.koorsoll[0].ybild + nup.workbuf.koorist.ybild);
                                if ( (atan(x) <= saf->fadschaus_winkel1) ||
                                     (atan(x) >= saf->fadschaus_winkel2) )
                                {
                                    // Nadeldreheinrichtung aus
                                    // Unterfadenschieber aus
                                    y7.resAuftrag();
                                    y32.resAuftrag();
                                }
                            }
                            else
                            {
                                // Nadeldreheinrichtung aus
                                // Unterfadenschieber aus
                                y7.resAuftrag();
                                y32.resAuftrag();
                            }
                            */
                            /* EasyCODE_STRUCTURED_COMMENT_END */
                            // SW014
                            if ( nup.nadeldreh (CHKAUS, nup.workbuf.koorist.xbild, nup.workbuf.koorist.ybild, 
                                                        nup.koorsoll[0].xbild ,nup.koorsoll[0].ybild ) )
                            {
                                // Nadeldreheinrichtung aus
                                // Unterfadenschieber aus
                                y7.resAuftrag();
                                y32.resAuftrag();
                            }
                        }
                        else
                        {
                            // Nadeldreheinrichtung aus
                            // Unterfadenschieber aus
                            y7.resAuftrag();
                            y32.resAuftrag();
                        }
                    }
                    /* EasyCODE ) */
                }
                /* EasyCODE ) */
                break;
            }
            nup.carmov (TAKTEN);
            bstepkey = 4;
            return 0;
        }
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        bstepkey = 2,   Nadel positionieren , ev. Oberteil heben, Fuss auf,
                        Fadenspannung zu, Klemme zu, Stoffgegendruecker ab */
        if ( koor.naestop() )
        {
            return 1;
        }
        else
        {
            switch ( nup.napos () )
            {
            case 0 :
                return 0;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                return 1;
            default:
                /* EasyCODE ( 0 
                Oberteil, Fuss, Klemme, Fadenspannung */
                if ( (nup.workbuf.naeout & 0x20) == 0x20 )
                {
                    // Oberteil heben
                    y50u1.setAuftrag();
                }
                else
                {
                    // Oberteil senken
                    y50u2.setAuftrag();
                }
                if ( saf->cutready )
                {
                    y4.setAuftrag();  // Klemme zu
                    y5.resAuftrag();  // Fadenspannung zu
                }
                else
                {
                    y4.resAuftrag();  // Klemme auf
                    y5.setAuftrag();  // Fadenspannung auf
                    y6.resAuftrag();  // SW016
                }
                hfuss.auf();       // Fuss auf
                y8.resAuftrag();   // Niederhalter auf
                y33.resAuftrag();  // Stoffgegendruecker ab
                bstepkey = 3;
                return 0;
                /* EasyCODE ) */
            }
        }
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        bstepkey = 3,   Transport rueckwaerts starten */
        if ( koor.naestop() )
        {
            return 1;
        }
        else
        {
            if ( y4.fertig() && y5.fertig() && y50u1.fertig() && y50u2.fertig() &&
                 hfuss_auf.fertig() && y33.fertig() && y8.fertig() )
            {
                if ( ver.move_nae() )
                {
                    nup.carmov(FAST);
                    bstepkey = 4;
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 0;
            }
        }
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        bstepkey = 4,   Warten bis Schritt ausgefuehrt */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            return 0xff;
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        bstepkey = 5,   Oberteil senken vor Stich rueckwaerts takten */
        if ( koor.naestop() )
        {
            return 1;
        }
        else
        {
            switch ( nup.napos() )
            {
            case 0 :
                return 0;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                return 1;
            default:
                if ( y4.fertig() && y5.fertig() &&
                     y8.fertig() && hfuss_auf.fertig() )
                {
                    if ( (hfuss.getpos().pos == saf->md.posoben.pos) &&
                         !y8.getZustand() )
                    {
                        // Fuss oben
                        y50u2.setAuftrag();  // Oberteil senken
                        bstepkey = 6;
                    }
                    else
                    {
                        hfuss.auf();      // Fuss auf
                        y4.setAuftrag();  // Klemme zu
                        y5.resAuftrag();  // Fadenspannung zu
                        y8.resAuftrag();  // Niederhalter auf
                    }
                }
                return 0;
            }
        }
        /* EasyCODE ) */
    case 6 :
        /* EasyCODE ( 0 
        bstepkey = 6,   Fertigmeldung vor Stich rueckwaerts takten */
        if ( koor.naestop() )
        {
            return 1;
        }
        else
        {
            if ( y50u2.fertig() )
            {
                bstepkey = 1;
            }
            return 0;
        }
        /* EasyCODE ) */
    default:
        return 1;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaehen::backctrl */
/* EasyCODE F */
void KlasseNaehen::backctrl (void)
{
    unsigned char *hlppoi;
    
    nup.workbuf.datpoi --;
    if ( *nup.workbuf.datpoi == 0xff )
    {
        do{
            nup.workbuf.datpoi --;
        }while ( *nup.workbuf.datpoi != 0xff );
    }
    else
    {
        nup.workbuf.datpoi --;
    }
    hlppoi = nup.workbuf.datpoi;
    nup.dfetnsew ();
    nup.workbuf.datpoi = hlppoi;
    if ( nup.workbuf.nbtart == 1 )
    {
        if ( nup.mbef[ANNAEHEN] )
        {
            nup.workbuf.nbtart = 0;
            // Stoffgegendruecker ab
            y33.resAuftrag();
        }
    }
    else
    {
        if ( nup.mbef[SCHNEIDEN] || nup.mbef[NAEHENAUS] )
        {
            nup.workbuf.nbtart = 1;
            // Stoffgegendruecker auf
            y33.setAuftrag();
        }
        else
        {
            nup.workbuf.nbtart = 0;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaehen::faedel */
/* EasyCODE F */
void KlasseNaehen::faedel (void)
{
    switch ( faedelkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        faedelkey = 0,   Bildschirmvorlage anzeigen */
        if ( keyb.taste() == 0xff )
        {
            if ( tfaedelstartanz )
            {
                //Spulenwechsel-Vorschlag-Warten auf Starttaste
                nae_botsch1.senden(BV026);
                faedelkey = 1;
            }
            else
            {
                //waehrend Spulenwechsel nur Stop
                nae_botsch1.senden(BV025);
                tfaedelstartanz = TRUE;
                faedelkey = nextfaedelkey;
            }
        }
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        faedelkey = 1,   Warten bis Taste Start gedrueckt */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            switch ( keyb.taste() )
            {
            case TEINGABE :
                eing.auftrag = 2;
                faedelkey = 2;
                break;
            case THOME :
                homereq = TRUE;
                thomeanz = FALSE;
                key = 0;
                break;
            case TSTART :
                tfaedelstartanz = FALSE;
                faedelkey = 0;
                break;
            default:
                break;
            }
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        faedelkey = 2,   Warten auf Stopanforderung */
        if ( koor.naestop() )
        {
            key = 0;
        }
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        faedelkey = 3,   Fadenschneiden einleiten bzw. Nadel positionieren */
        if ( koor.naestop() )
        {
            nextfaedelkey = 3;
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                nextfaedelkey = 3;
                faedelkey = 0;
            }
            else
            {
                if ( saf->cutready )
                {
                    switch ( nup.napos () )
                    {
                    case 0 :
                        break;
                    case 1 :
                        koor.nae_err = 1;
                        errpar = nminf.fehler;
                        nextfaedelkey = 3;
                        key = 0;
                        break;
                    default:
                        hfuss.auf();        // Fuss auf
                        y4.setAuftrag();    // Klemme zu
                        y5.resAuftrag();    // Fadenspannung zu
                        y6.resAuftrag();    // Zusatzspannung auf
                        y8.resAuftrag();    // Niederhalter auf
                        out3.resAuftrag();
                        y30.resAuftrag();   // Fadenschneiden aus
                        y33.resAuftrag();   // Stoffgegendruecker ab
                        faedelkey = 8;
                        break;
                    }
                }
                else
                {
                    if ( saf->buf.nbtart == 0 )
                    {
                        faedelreq = FALSE;
                        key = 0;
                    }
                    else
                    {
                        y7.resAuftrag();    // Nadeldreheinrichtung aus
                        y32.resAuftrag();   // Unterfadenschieber aus
                        faedelkey = 4;
                    }
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        faedelkey = 4,   Fadenschneiden einleiten */
        if ( koor.naestop() )
        {
            nextfaedelkey = 3;
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                nextfaedelkey = 3;
                faedelkey = 0;
            }
            else
            {
                if ( y7.fertig() && y32.fertig() )
                {
                    nup.schneidkey = 0;
                    faedelkey = 5;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        faedelkey = 5,   Fadenschneiden */
        switch ( nup.schneidablauf () )
        {
        case 0 :
            if ( koor.naestop() || (koor.nae_err != 0) )
            {
                outportb (INTMASK, REF1DI | REF2DI);
                nminf.befehl = 8;
                nextfaedelkey = 3;
                key = 0;
            }
            break;
        case 1 :
            faedelkey = 8;
            break;
        default:
            key = 0;
            break;
        }
        break;
        /* EasyCODE ) */
    case 8 :
        /* EasyCODE ( 0 
        faedelkey = 8,   Oberteil heben */
        if ( koor.naestop() )
        {
            nextfaedelkey = 3;
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                nextfaedelkey = 3;
                faedelkey = 0;
            }
            else
            {
                if ( hfuss_auf.fertig() && y4.fertig() && y5.fertig() && y6.fertig() &&
                     y8.fertig() && y30.fertig() && y33.fertig() )
                {
                    if ( e80.getZustand() && e79.getZustand() )
                    {
                        // Schablonenverriegelung zu
                        y51u2.setAuftrag();
                    }
                    y50u1.setAuftrag();   // Oberteil heben
                    faedelkey = 9;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 9 :
        /* EasyCODE ( 0 
        faedelkey = 9,   nachdem Oberteil oben - in Serviceposition fahren starten */
        if ( koor.naestop() )
        {
            nextfaedelkey = 3;
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                nextfaedelkey = 3;
                faedelkey = 0;
            }
            else
            {
                if ( y50u1.fertig() &&
                     y51u2.fertig() )
                {
                    if ( ver.move_sonst() )
                    {
                        nup.koorsoll[0].xbild = saf->servicepos.xbild;
                        nup.koorsoll[0].motorx = saf->servicepos.motorx;
                        nup.koorsoll[0].ybild = saf->servicepos.ybild;
                        nup.koorsoll[0].motory = saf->servicepos.motory;
                        nup.carmov (FAST);
                        faedelkey = 10;
                    }
                    else
                    {
                        nextfaedelkey = 3;
                        key = 0;
                    }
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 10 :
        /* EasyCODE ( 0 
        faedelkey = 10,   in Serviceposition fahren */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            nup.workbuf.koorist = nup.koorsoll [0];
            sewpoint = !nup.carlp;
            nup.carlp = FALSE;
            faedelkey = 18;
        }
        else
        {
            if ( !ver.move_sonst() || koor.notstop() )
            {
                // verriegelt
                smx.stop();
                smy.stop();
                saf->carposok = FALSE;
                homereq = TRUE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 18 :
        /* EasyCODE ( 0 
        faedelkey = 18,   Fuss ab, Klemme auf, Fadenspannung zu */
        if ( koor.naestop() )
        {
            nextfaedelkey = 18;
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                nextfaedelkey = 18;
                faedelkey = 0;
            }
            else
            {
                if ( !nae_botsch1.busycheck() && !nae_botsch2.busycheck() )
                {
                    if ( y8.fertig() )
                    {
                        if ( y8.getZustand() )
                        {
                            hfuss.ab();         // Fuss ab
                            y4.resAuftrag();    // Klemme auf
                            y5.setAuftrag();    // Fadenspannung auf
                            y6.resAuftrag();  // SW016
                            nae_botsch1.senden(BV026);
                            sprintf (nae.varstring, faedelhfposs, (float)hfuss.getpos().pos/10);
                            nae_botsch2.senden (nae.varstring);
                            faedelkey = 19;
                            if ( saf->md.nadelanzahl == 1 )
                            {
                                y7.setAuftrag();  // Nadeldreheinrichtung ein
                                nae_botsch1.senden(tnadeldrehens);
                            }
                        }
                        else
                        {
                            y8.setAuftrag(); // Niederhalter ab
                            out3.setAuftrag();
                        }
                    }
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 19 :
        /* EasyCODE ( 0 
        faedelkey = 19,   Tasten abfragen */
        if ( koor.naestop() )
        {
            faedelkey = 18;
            key = 0;
        }
        else
        {
            if ( y4.fertig() && y5.fertig() && y8.fertig() && y7.fertig() &&
                 hfuss.ready() && !nae_botsch1.busycheck() &&
                 y50u1.fertig() && y50u2.fertig() && y33.fertig() )
            {
                switch ( keyb.taste() )
                {
                case TOBERTEIL :
                    if ( e50u1.getZustand() && !e50u2.getZustand() )
                    {
                        // Oberteil oben
                        y50u2.setAuftrag(); // Oberteil ab
                        y5.resAuftrag();    // Fadenspannung zu
                        y33.setAuftrag();   // Stoffgegendruecker auf
                    }
                    else
                    {
                        // Oberteil unten
                        y50u1.setAuftrag(); // Oberteil auf
                        y5.setAuftrag();    // Fadenspannung auf
                        y6.resAuftrag();  // SW016
                        y33.resAuftrag();   // Stoffgegendruecker ab
                    }
                    faedelkey = 25;
                    break;
                case TSTOFFGEGENDRUECKER :
                    if ( e33.getZustand() )
                    {
                        // Stoffgegendruecker unten
                        y33.setAuftrag(); // Stoffgegendruecker auf
                    }
                    else
                    {
                        // Stoffgegendruecker oben
                        y33.resAuftrag(); // Stoffgegendruecker ab
                    }
                    faedelkey = 25;
                    break;
                case TFADENWEDLER :
                    if ( !y2u1.getZustand() && !y2u2.getZustand() )
                    {
                        // Fadenwedler in Grundstellung
                        y2u2.setAusgang(); // Fadenwedler drucklos
                    }
                    else
                    {
                        // Fadenwedler nicht in Grundstellung
                        y2u1.resAusgang(); // Fadenwedler in Grundst.
                        y2u2.resAusgang();
                    }
                    faedelkey = 25;
                    break;
                case TNADELDREHEN :
                    if ( y7.getZustand() )
                    {
                        y7.resAuftrag(); // Nadeldreheinrichtung aus
                    }
                    else
                    {
                        y7.setAuftrag(); // Nadeldreheinrichtung ein
                    }
                    faedelkey = 25;
                    break;
                case THFAUF :
                    /* EasyCODE ( 0 
                    Huepferfuss auf, Einzelpuls */
                    if ( hfuss.getpos().pos < MAX_POS )
                    {
                        hfsollpos.pos = hfuss.getpos().pos + 1;
                        vfindex = 0;
                        tnae1.rsz();
                        tnae1.sz(saf->prodelzt);
                        hfuss.postomo (&hfsollpos);
                        hfuss.hfmove(hfsollpos.motor);
                        faedelkey = 30;
                    }
                    break;
                    /* EasyCODE ) */
                case THFAB :
                    /* EasyCODE ( 0 
                    Huepferfuss ab, Einzelpuls */
                    if ( hfuss.getpos().pos > 0 )
                    {
                        hfsollpos.pos = hfuss.getpos().pos - 1;
                        vfindex = 0;
                        tnae1.rsz();
                        tnae1.sz(saf->prodelzt);
                        hfuss.postomo (&hfsollpos);
                        hfuss.hfmove(hfsollpos.motor);
                        faedelkey = 30;
                    }
                    break;
                    /* EasyCODE ) */
                case TEINGABE :
                    eing.auftrag = 2;
                    nextfaedelkey = 18;
                    tfaedelstartanz = FALSE;
                    faedelkey = 2;
                    break;
                case THOME :
                    homereq = TRUE;
                    thomeanz = FALSE;
                    key = 0;
                    break;
                case TSTART :
                    nae_botsch1.senden(BV025);
                    faedelkey = 20;
                    break;
                default:
                    break;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 20 :
        /* EasyCODE ( 0 
        faedelkey = 20,   Nadel positionieren, Fuss auf, Klemme zu, Fadenspannung auf */
        if ( koor.naestop() )
        {
            nextfaedelkey = 20;
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                nextfaedelkey = 20;
                faedelkey = 0;
            }
            else
            {
                switch ( nup.napos() )
                {
                case 0 :
                    break;
                case 1 :
                    koor.nae_err = 1;
                    errpar = nminf.fehler;
                    nextfaedelkey = 20;
                    key = 0;
                    break;
                default:
                    y50u1.setAuftrag(); // Oberteil auf
                    hfuss.auf();        // Fuss auf
                    y4.setAuftrag();    // Klemme zu
                    y5.resAuftrag();    // Fadenspannung zu
                    y8.resAuftrag();    // Niederhalter auf
                    out3.resAuftrag();
                    y33.resAuftrag();   // Stoffgegendruecker ab
                    y7.resAuftrag();    // Nadeldreheinrichtung aus
                    faedelkey = 21;
                    break;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 21 :
        /* EasyCODE ( 0 
        faedelkey = 21,  auf unterbrochenen Punkt fahren starten */
        if ( koor.naestop() )
        {
            nextfaedelkey = 21;
            key = 0;
        }
        else
        {
            if ( keyb.taste() == TSTOP )
            {
                nextfaedelkey = 21;
                faedelkey = 0;
            }
            else
            {
                if ( hfuss_auf.fertig() && y4.fertig() && 
                     y5.fertig() && y7.fertig() && y8.fertig() &&
                     y33.fertig() && y50u1.fertig() )
                {
                    ufastoe = FALSE;
                    if ( ver.move_sonst() )
                    {
                        nup.koorsoll[0] = saf->buf.koorist;
                        nup.carmov (FAST);
                        faedelkey = 23;
                    }
                    else
                    {
                        nextfaedelkey = 21;
                        key = 0;
                    }
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 23 :
        /* EasyCODE ( 0 
        faedelkey = 23,   auf unterbrochenen Punkt fahren */
        if ( smx.ready() && smy.ready() )
        {
            // SmX und SmY fertig
            nup.workbuf = saf->buf;
            nup.carlp = !sewpoint;
            faedelreq = FALSE;
            if ( (saf->variante == 1) )
            {
                // SW002
                y50u2.setAuftrag();  // Oberteil senken   
                y8.setAuftrag();     // Niederhalter ab  (offen neu)
                out3.setAuftrag();   // Bandzufuehrung ab (hier neu SW002)
                y10.setAuftrag();
            }
            key = 0;
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                homereq = TRUE;
                saf->carposok = FALSE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 25 :
        if ( keyb.taste() == 0xff )
        {
            faedelkey = 19;
        }
        break;
    case 30 :
        /* EasyCODE ( 0 
        faedelkey = 30,   Warten bis Einzelpuls fertig */
        if ( hfuss.ready() )
        {
            hfuss.set_istpos(hfsollpos);
            sprintf (nae.varstring, faedelhfposs, (float)hfuss.getpos().pos/10);
            nae_botsch1.senden (nae.varstring);
            faedelkey = 31;
        }
        break;
        /* EasyCODE ) */
    case 31 :
        /* EasyCODE ( 0 
        faedelkey = 31,   nach Zeit langsames Dauerfahren starten */
        if ( koor.naestop() )
        {
            nextfaedelkey = 3;
            key = 0;
        }
        else
        {
            if ( tnae1.tz() )
            {
                tnae1.rsz();
                tnae1.sz(saf->profastzt);
                tnae2.rsz();
                tnae2.sz(5);
                vfindex = 1;
                faedelkey = 32;
            }
            else
            {
                switch ( keyb.taste() )
                {
                case THFAB :
                    break;
                case THFAUF :
                    break;
                default:
                    faedelkey = 19;
                    break;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 32 :
        /* EasyCODE ( 0 
        faedelkey = 32,   Dauerfahren solange Taste gedrueckt */
        if ( koor.naestop() )
        {
            faedelkey = 33;
        }
        else
        {
            if ( tnae1.tz() && (vfindex==1) )
            {
                vfindex = 2;
            }
            if ( hfuss.ready() && tnae2.tz() && !nae_botsch1.busycheck() )
            {
                /* EasyCODE ( 0 
                Tasten abfragen, Verfahrweg bestimmen */
                switch ( keyb.taste() )
                {
                case THFAB :
                    switch ( vfindex )
                    {
                    case 0 :
                        hfsollpos.pos = hfuss.getpos().pos - 1;
                        break;
                    case 1 :
                        hfsollpos.pos = hfuss.getpos().pos - 1;
                        break;
                    default:
                        hfsollpos.pos = hfuss.getpos().pos - 5;
                        break;
                    }
                    if ( hfsollpos.pos < 0 )
                    {
                        hfsollpos.pos = 0;
                    }
                    break;
                case THFAUF :
                    switch ( vfindex )
                    {
                    case 0 :
                        hfsollpos.pos = hfuss.getpos().pos + 1;
                        break;
                    case 1 :
                        hfsollpos.pos = hfuss.getpos().pos + 1;
                        break;
                    default:
                        hfsollpos.pos = hfuss.getpos().pos + 5;
                        break;
                    }
                    if ( hfsollpos.pos > MAX_POS )
                    {
                        hfsollpos.pos = MAX_POS;
                    }
                    break;
                default:
                    faedelkey = 33;
                    break;
                }
                /* EasyCODE ) */
                if ( faedelkey == 32 )
                {
                    hfuss.postomo (&hfsollpos);
                    hfuss.hfmove(hfsollpos.motor);
                    hfuss.set_istpos (hfsollpos);
                    sprintf (nae.varstring, faedelhfposs, (float)hfuss.getpos().pos/10);
                    nae_botsch1.senden (nae.varstring);
                    tnae2.rsz();
                    tnae2.sz(5);
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 33 :
        /* EasyCODE ( 0 
        faedelkey = 33,   warten bis Huepferfuss fertig nachdem
                          keine Taste gedrueckt war oder Stopanforderung */
        if ( hfuss.ready() )
        {
            sprintf (nae.varstring, faedelhfposs, (float)hfuss.getpos().pos/10);
            nae_botsch1.senden (nae.varstring);
            faedelkey = 19;
        }
        break;
        /* EasyCODE ) */
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaehen::find */
/* EasyCODE F */
void KlasseNaehen::find (void)
{
    /* EasyCODE C */
    /* EasyCODE < */
    struct
    /* EasyCODE > */
    {
            int x;
            int y;
    }bildkoordinate, bildkoordinate_min;
    /* EasyCODE E */
    float abstand, abstand_min;
    unsigned char *datenzeiger, *datenzeiger_min;
    switch ( findkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        findkey = 0,   Fadenschneiden einleiten bzw. Nadel positionieren,
                       Bildschirm-Vorlage anzeigen */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( !nae_botsch1.busycheck() )
            {
                if ( saf->cutready )
                {
                    switch ( nup.napos() )
                    {
                    case 0 :
                        break;
                    case 1 :
                        koor.nae_err = 1;
                        errpar = nminf.fehler;
                        key = 0;
                        break;
                    default:
                        hfuss.auf();        // Fuss auf
                        y4.setAuftrag();    // Klemme zu
                        y5.resAuftrag();    // Fadenspannung zu
                        y6.resAuftrag();    // Zusatzspannung auf
                        y8.resAuftrag();    // Niederhalter auf
                        y30.resAuftrag();   // Fadenschneiden aus
                        y33.resAuftrag();   // Stoffgegendruecker ab
                        nae_botsch1.senden(BV027);
                        findkey = 5;
                        break;
                    }
                }
                else
                {
                    if ( saf->buf.nbtart == 0 )
                    {
                        findreq = FALSE;
                        key = 2;
                    }
                    else
                    {
                        y7.resAuftrag();    // Nadeldreheinrichtung aus
                        y32.resAuftrag();   // Unterfadenschieber aus
                        nae_botsch1.senden(BV027);
                        findkey = 1;
                    }
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        findkey = 1,   Fadenschneiden einleiten */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( y7.fertig() && y32.fertig() )
            {
                nup.schneidkey = 0;
                findkey = 2;
            }
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        findkey = 2,   Fadenschneiden */
        switch ( nup.schneidablauf () )
        {
        case 0 :
            if ( koor.naestop() || (koor.nae_err != 0) )
            {
                outportb (INTMASK, REF1DI | REF2DI);
                nminf.befehl = 8;
                key = 0;
            }
            break;
        case 1 :
            findkey = 5;
            break;
        default:
            key = 0;
            break;
        }
        break;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        findkey = 5,   Oberteil heben */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( hfuss_auf.fertig() && y4.fertig() && y5.fertig() && y6.fertig() &&
                 y8.fertig() && y30.fertig() && y33.fertig() )
            {
                y50u1.setAuftrag();   // Oberteil heben
                findkey = 6;
            }
        }
        break;
        /* EasyCODE ) */
    case 6 :
        /* EasyCODE ( 0 
        findkey = 6,   nachdem Oberteil oben - weiter */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( y50u1.fertig() && (keyb.taste() == 0xff) )
            {
                findkey = 7;
            }
        }
        break;
        /* EasyCODE ) */
    case 7 :
        /* EasyCODE ( 0 
        findkey = 7,   Tasten abfragen ---> verzweigen */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            switch ( keyb.taste() )
            {
            case TLINKS :
                nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild - 1;
                nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                break;
            case TRECHTS :
                nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild + 1;
                nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                break;
            case TAB :
                nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild - 1;
                break;
            case TAUF :
                nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild + 1;
                break;
            case THOME :
                homereq = TRUE;
                thomeanz = FALSE;
                key = 0;
                return ;
            case TENTER :
                findkey = 12;
                return ;
            case TEINGABE :
                eing.auftrag = 2;
                key = 0;
                return ;
            default:
                return ;
            }
            vfindex = 0;
            tnae1.rsz();
            tnae1.sz(saf->prodelzt);
            if ( nup.poscheck(&nup.koorsoll[0]) )
            {
                nup.bitomo (&nup.koorsoll[0]);
                nup.carmov(STANDARD);
                findkey = 8;
            }
        }
        break;
        /* EasyCODE ) */
    case 8 :
        /* EasyCODE ( 0 
        findkey = 8,   Warten bis Einzelpuls fertig */
        if ( smx.ready() && smy.ready() && !nae_botsch1.busycheck() )
        {
            nup.workbuf.koorist = nup.koorsoll [0];
            sprintf (varstring, findkoords, nup.workbuf.koorist.xbild, nup.workbuf.koorist.ybild);
            nae_botsch1.senden (varstring);
            findkey = 9;
        }
        break;
        /* EasyCODE ) */
    case 9 :
        /* EasyCODE ( 0 
        findkey = 9,   nach Zeit langsames Dauerfahren starten */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( tnae1.tz() )
            {
                tnae1.rsz();
                tnae1.sz(5);
                vfindex = 1;
                nup.koormerker = nup.koorsoll [0];
                findkey = 10;
            }
            else
            {
                switch ( keyb.taste() )
                {
                case TLINKS :
                    break;
                case TRECHTS :
                    break;
                case TAB :
                    break;
                case TAUF :
                    break;
                default:
                    findkey = 7;
                    break;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 10 :
        /* EasyCODE ( 0 
        findkey = 10,   Dauerfahren solange Taste gedrueckt */
        if ( koor.naestop() )
        {
            smx.dynbef = 2;  // Stop 
            smy.dynbef = 2;  // Stop 
            findkey = 11;
        }
        else
        {
            if ( ((smx.ready() && smy.ready()) || (nae.vfindex > 1)) )
            {
                /* EasyCODE ( 0 
                Tasten abfragen, Verfahrweg bestimmen */
                taste = keyb.taste();
                switch ( taste )
                {
                case TLINKS :
                    switch ( nae.vfindex )
                    {
                    case 1 :
                        nup.carmovdyn(SMOTX, SMXMINUS, FAST);
                        tastenmerk = taste;
                        nae.vfindex = 2;
                        break;
                    case 2 :
                        nup.koorsoll [0].motorx = nup.koormerker.motorx - smx.Pulse();
                        break;
                    default:
                        break;
                    }
                    break;
                case TRECHTS :
                    switch ( nae.vfindex )
                    {
                    case 1 :
                        nup.carmovdyn(SMOTX, SMXPLUS, FAST);
                        tastenmerk = taste;
                        nae.vfindex = 2;
                        break;
                    case 2 :
                        nup.koorsoll [0].motorx = nup.koormerker.motorx + smx.Pulse();
                        break;
                    default:
                        break;
                    }
                    break;
                case TAB :
                    switch ( nae.vfindex )
                    {
                    case 1 :
                        nup.carmovdyn(SMOTY, SMYMINUS, FAST);
                        tastenmerk = taste;
                        nae.vfindex = 2;
                        break;
                    case 2 :
                        nup.koorsoll [0].motory = nup.koormerker.motory - smy.Pulse();
                        break;
                    default:
                        break;
                    }
                    break;
                case TAUF :
                    switch ( nae.vfindex )
                    {
                    case 1 :
                        nup.carmovdyn(SMOTY, SMYPLUS, FAST);
                        tastenmerk = taste;
                        nae.vfindex = 2;
                        break;
                    case 2 :
                        nup.koorsoll [0].motory = nup.koormerker.motory + smy.Pulse();
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    smx.dynbef = 2;  // Stop 
                    smy.dynbef = 2;  // Stop 
                    findkey = 11;
                    break;
                }
                /* EasyCODE ) */
                if ( findkey == 10 )
                {
                    if ( teing1.tz() && !eing_botsch1.busycheck() )
                    {
                        nup.motobi (&nup.koorsoll[0]);
                        nup.workbuf.koorist = nup.koorsoll[0]; 
                        sprintf (varstring, findkoords, nup.workbuf.koorist.xbild,nup.workbuf.koorist.ybild);
                        eing_botsch1.senden (varstring);
                        teing1.rsz();
                        teing1.sz(5);
                    }
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 11 :
        /* EasyCODE ( 0 
        findkey = 11,   warten bis Sm fertig nachdem
                        keine Taste gedrueckt war oder Stopanforderung */
        if ( smx.ready() && smy.ready() )
        {
            // Erst nach stehen der Achsen stimmt smx.Pulse() genau, also jetzt noch einmal
            // smx.Pulse auslesen und berechnen.
            switch ( tastenmerk )
            {
            case TLINKS :
                nup.koorsoll [0].motorx = nup.koormerker.motorx - smx.Pulse();
                break;
            case TRECHTS :
                nup.koorsoll [0].motorx = nup.koormerker.motorx + smx.Pulse();
                break;
            case TAB :
                nup.koorsoll [0].motory = nup.koormerker.motory - smy.Pulse();
                break;
            case TAUF :
                nup.koorsoll [0].motory = nup.koormerker.motory + smy.Pulse();
                break;
            default:
                break;
            }
            nup.motobi (&nup.koorsoll[0]);
            /* EasyCODE - */
            // Da nach Versuche motobi und dann bitomo leider nur zu 99,9% stimmig waren
            // wird nun die Rechenungenauigkeit korrigiert und gefahren
            nup.workbuf.koorist = nup.koorsoll[0]; 
            nup.bitomo (&nup.koorsoll[0]);
            /* EasyCODE ( 0 
            Korrekturfahrt wenn nach Rundung die Motorpulse nicht stimmen */
            if ( (nup.workbuf.koorist.motorx != nup.koorsoll[0].motorx) || 
                 (nup.workbuf.koorist.motory != nup.koorsoll[0].motory) )
            {
                eing_botsch1.senden(BELLBELL); // Debug Test indikator auf Korrekturfahrt
                nup.carmov(STANDARD);
            }
            /* EasyCODE ) */
            nup.workbuf.koorist = nup.koorsoll[0];
            sprintf (varstring, findkoords, nup.workbuf.koorist.xbild, nup.workbuf.koorist.ybild);
            nae_botsch1.senden (varstring);
            findkey = 7;
        }
        break;
        /* EasyCODE ) */
    case 12 :
        /* EasyCODE ( 0 
        findkey = 12,   Punkt des Nahtbilds mit geringstem Abstand suchen */
        // Abstand im Quadrat AP<->koorist berechnen
        bildkoordinate.x = *((int *)(nup.stianfadr + SAP_X));
        bildkoordinate.y = *((int *)(nup.stianfadr + SAP_Y));
        bildkoordinate_min = bildkoordinate;
        abstand_min = (float)(bildkoordinate.x-nup.workbuf.koorist.xbild)*(bildkoordinate.x-nup.workbuf.koorist.xbild) +
                      (float)(bildkoordinate.y-nup.workbuf.koorist.ybild)*(bildkoordinate.y-nup.workbuf.koorist.ybild);
        datenzeiger = datenzeiger_min = (unsigned char *)(nup.stianfadr+SNPSTART);
        /* EasyCODE ( 0 
        Bildkoordinate mit kleinstem Abstand suchen */
        while ( datenzeiger < nup.endadr )
        {
            if ( *datenzeiger == 0xff )
            {
                if ( *(datenzeiger+1) == 0x20 )
                {
                    /* EasyCODE ( 0 
                    Schnelles Transportieren */
                    // Schnelles Transportieren / Eilgang
                     datenzeiger += 2;
                    do{
                        if ( *datenzeiger >= 0x80 )
                        {
                            bildkoordinate.x  -= *datenzeiger & 0x7f;
                        }
                        else
                        {
                            bildkoordinate.x += *datenzeiger;
                        }
                        if ( *(datenzeiger+1) >= 0x80 )
                        {
                            bildkoordinate.y -= *(datenzeiger+1) & 0x7f;
                        }
                        else
                        {
                            bildkoordinate.y += *(datenzeiger+1);
                        }
                        datenzeiger += 2;
                    }while ( *datenzeiger !=0xff );
                    datenzeiger ++;
                    abstand = (float)(bildkoordinate.x-nup.workbuf.koorist.xbild)*(bildkoordinate.x-nup.workbuf.koorist.xbild) +
                              (float)(bildkoordinate.y-nup.workbuf.koorist.ybild)*(bildkoordinate.y-nup.workbuf.koorist.ybild);
                    if ( abstand < abstand_min )
                    {
                        abstand_min = abstand;
                        datenzeiger_min = datenzeiger;
                        bildkoordinate_min = bildkoordinate;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    M-Befehl */
                    while ( *(++datenzeiger) != 0xff )
                    {
                    }
                    datenzeiger++;
                    /* EasyCODE ) */
                }
            }
            else
            {
                /* EasyCODE ( 0 
                Koordinate */
                if ( *datenzeiger >= 0x80 )
                {
                    bildkoordinate.x -=  (*datenzeiger & 0x7f);
                }
                else
                {
                    bildkoordinate.x += *datenzeiger;
                }
                datenzeiger++;
                if ( *datenzeiger >= 0x80 )
                {
                    bildkoordinate.y -=  (*datenzeiger & 0x7f);
                }
                else
                {
                    bildkoordinate.y += *datenzeiger;
                }
                datenzeiger++;
                abstand = (float)(bildkoordinate.x-nup.workbuf.koorist.xbild)*(bildkoordinate.x-nup.workbuf.koorist.xbild) +
                          (float)(bildkoordinate.y-nup.workbuf.koorist.ybild)*(bildkoordinate.y-nup.workbuf.koorist.ybild);
                if ( abstand < abstand_min )
                {
                    abstand_min = abstand;
                    datenzeiger_min = datenzeiger;
                    bildkoordinate_min = bildkoordinate;
                }
                /* EasyCODE ) */
            }
        }
        /* EasyCODE ) */
        nup.workbuf_res();
        datenzeiger = (unsigned char *)(nup.stianfadr + SNPSTART);
        /* EasyCODE ( 0 
        workbuf aktualisieren */
        while ( datenzeiger < datenzeiger_min )
        {
            if ( *datenzeiger == 0xff )
            {
                switch ( *(datenzeiger+1) )
                {
                case 1 //Annaehen
                :
                    nup.workbuf.nbtart = 1;
                    datenzeiger += 3;
                    break;
                case 2 :
                case 4 //Fadenschneiden oder Naehen aus
                :
                    nup.workbuf.nbtart = 0;
                    /* EasyCODE - */
                    datenzeiger += 3;
                    break;
                case 7 //reduzierte Drehzahl ein
                :
                    nup.workbuf.redgswen = TRUE;
                    /* EasyCODE - */
                    datenzeiger += 3;
                    break;
                case 8 //reduzierte Drehzahl aus
                :
                    nup.workbuf.redgswen = FALSE;
                    /* EasyCODE - */
                    datenzeiger += 3;
                    break;
                case 0x10 //Naehgeschwindigkeit
                :
                    nup.workbuf.naegsw = *(datenzeiger+2);
                    /* EasyCODE - */
                    datenzeiger += 4;
                    break;
                case 0x11 :
                    /* EasyCODE < */
                    // Ausgang setzen
                    /* EasyCODE > */
                    switch ( *(datenzeiger+2) )
                    {
                    case 1 :
                        nup.workbuf.naeout |= 0x01;
                        break;
                    case 2 :
                        nup.workbuf.naeout |= 0x02;
                        break;
                    case 3 :
                        nup.workbuf.naeout |= 0x04;
                        break;
                    case 4 :
                        nup.workbuf.naeout |= 0x08;
                        break;
                    case 6 :
                        nup.workbuf.naeout |= 0x20;
                        break;
                    default:
                        break;
                    }
                    datenzeiger += 4;
                    break;
                case 0x12 :
                    /* EasyCODE < */
                    // Ausgang ruecksetzen
                    /* EasyCODE > */
                    switch ( *(datenzeiger+2) )
                    {
                    case 1 :
                        nup.workbuf.naeout &= ~0x01;
                        break;
                    case 2 :
                        nup.workbuf.naeout &= ~0x02;
                        break;
                    case 3 :
                        nup.workbuf.naeout &= ~0x04;
                        break;
                    case 4 :
                        nup.workbuf.naeout &= ~0x08;
                        break;
                    case 6 :
                        nup.workbuf.naeout &= ~0x20;
                        break;
                    default:
                        break;
                    }
                    datenzeiger += 4;
                    break;
                case 0x13 :
                    nup.workbuf.zuspgon = TRUE;
                    datenzeiger += 4;
                    break;
                case 0x14 :
                    nup.workbuf.zuspgon = FALSE;
                    datenzeiger += 4;
                    break;
                case 0x18 :
                    nup.workbuf.posunten = *(datenzeiger+2);
                    hfuss.akt_posunten.pos = nup.workbuf.posunten;
                    hfuss.postomo (&hfuss.akt_posunten);
                    datenzeiger += 4;
                    break;
                case 0x19 :
                    nup.workbuf.hub = *(datenzeiger+2);
                    hfuss.akt_hub = nup.workbuf.hub;
                    datenzeiger += 4;
                    break;
                default:
                    /* EasyCODE < */
                    // ungueltiger Befehl wird als NOP behandelt
                    /* EasyCODE > */
                    do{
                        datenzeiger++;
                    }while ( *datenzeiger != 0xff );
                    datenzeiger++;
                    break;
                }
            }
            else
            {
                // Koordinaten
                datenzeiger += 2;
            }
        }
        /* EasyCODE ) */
        nup.workbuf.datpoi = datenzeiger_min;
        nup.koorsoll[0].xbild = bildkoordinate_min.x;
        nup.koorsoll[0].ybild = bildkoordinate_min.y;
        if ( nup.poscheck(&nup.koorsoll[0]) )
        {
            nup.bitomo (&nup.koorsoll[0]);
            if ( ver.move_sonst() )
            {
                nup.carmov(FAST);
                findkey = 13;
            }
            else
            {
                key = 0;
            }
        }
        else
        {
            // Position unzulßssig
            koor.nae_err = 11;
            homereq = TRUE;
            key = 0;
        }
        break;
        /* EasyCODE ) */
    case 13 :
        /* EasyCODE ( 0 
        findkey = 13,   auf Punkt des Nahtbilds fahren */
        if ( smx.ready() && smy.ready() )
        {
            /* EasyCODE < */
            nup.workbuf.koorist = nup.koorsoll [0];
            saf->buf = nup.workbuf;
            // Naehausgaenge setzen
            /* EasyCODE > */
            if ( (nup.workbuf.naeout & 0x01) == 0x01 )
            {
                out1.setAuftrag();
                if ( (saf->md.drehmodus == 2) &&
                     (saf->md.nadelanzahl == 1) )
                {
                    // Nadeldreheinrichtung ein
                    // Unterfadenschieber ein
                    y7.setAuftrag();
                    y32.setAuftrag();
                }
            }
            else
            {
                out1.resAuftrag();
                if ( saf->md.drehmodus == 2 )
                {
                    // Nadeldreheinrichtung aus
                    // Unterfadenschieber aus
                    y7.resAuftrag();
                    y32.resAuftrag();
                }
            }
            if ( (nup.workbuf.naeout & 0x02) == 0x02 )
            {
                out2.setAuftrag();
            }
            else
            {
                out2.resAuftrag();
            }
            if ( (nup.workbuf.naeout & 0x04) == 0x04 )
            {
                out3.setAuftrag();
            }
            else
            {
                out3.resAuftrag();
            }
            if ( (nup.workbuf.naeout & 0x08) == 0x08 )
            {
                out4.setAuftrag();
            }
            else
            {
                out4.resAuftrag();
            }
            if ( nup.workbuf.zuspgon )
            {
                // Zusatzfadenspannung ein
                y6.setAuftrag();
            }
            else
            {
                // Zusatzfadenspannung aus
                y6.resAuftrag();
            }
            if ( nup.workbuf.nbtart == 0 )
            {
                findreq = FALSE;
                ablaufkey = 20;
                naekey = 0;
                key = 0;
            }
            else
            {
                findkey = 14;
            }
        }
        else
        {
            if ( koor.notstop() )
            {
                smx.stop();
                smy.stop();
                saf->carposok = FALSE;
                homereq = TRUE;
                key = 0;
            }
        }
        break;
        /* EasyCODE ) */
    case 14 :
        /* EasyCODE ( 0 
        findkey = 14,   Oberteil absenken */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            switch ( nup.napos () )
            {
            case 0 :
                break;
            case 1 :
                koor.nae_err = 1;
                errpar = nminf.fehler;
                break;
            default:
                if ( y4.fertig() && y5.fertig() &&
                     y8.fertig() && hfuss_auf.fertig() )
                {
                    if ( hfuss.getpos().pos == saf->md.posoben.pos )
                    {
                        // Fuss oben
                        y50u2.setAuftrag();  // Oberteil senken
                        findkey = 15;
                    }
                    else
                    {
                        hfuss.auf();      // Fuss auf
                        y4.setAuftrag();  // Klemme zu
                        y5.resAuftrag();  // Fadenspannung zu
                        y8.resAuftrag();  // Niederhalter auf
                    }
                }
                break;
            }
        }
        break;
        /* EasyCODE ) */
    case 15 :
        /* EasyCODE ( 0 
        findkey = 15,   warten bis Oberteil absenken fertig */
        if ( koor.naestop() )
        {
            key = 0;
        }
        else
        {
            if ( y50u2.fertig() )
            {
                findreq = FALSE;
                ablaufkey = 20;
                naekey = 0;
                key = 0;
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
Inits */
/* EasyCODE ( 0 
KlasseNaehen::kinit */
/* EasyCODE F */
void KlasseNaehen::kinit (void)
{
    // Flags
       saf->cutready = TRUE;
    /* EasyCODE ( 0 
    Ladepunkt */
    #ifdef NF120x70
        saf->lp.ybild = LP_YBILD_12070;
        saf->lp.motory = LP_MOTORY_12070;
    #endif 
    #ifdef NF120x170
        saf->lp.ybild = LP_YBILD_120170;
        saf->lp.motory = LP_MOTORY_120170;
    #endif 
    #ifdef NF100x50
        saf->lp.ybild = LP_YBILD_10050;
        saf->lp.motory = LP_MOTORY_10050;
    #endif 
    #ifdef NF120x200
        saf->lp.ybild = LP_YBILD_120200;
        saf->lp.motory = LP_MOTORY_120200;
    #endif 
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    MNP (Initiatorenpunkt) */
    #ifdef NF120x70
        saf->mnp.xbild = MNP_XBILD_12070;
        saf->mnp.ybild = MNP_YBILD_12070;
        saf->mnp.motorx = MNP_MOTORX_12070;
        saf->mnp.motory = MNP_MOTORY_12070;
    #endif 
    #ifdef NF120x170
        saf->mnp.xbild = MNP_XBILD_120170;
        saf->mnp.ybild = MNP_YBILD_120170;
        saf->mnp.motorx = MNP_MOTORX_120170;
        saf->mnp.motory = MNP_MOTORY_120170;
    #endif 
    #ifdef NF100x50
        saf->mnp.xbild = MNP_XBILD_10050;
        saf->mnp.ybild = MNP_YBILD_10050;
        saf->mnp.motorx = MNP_MOTORX_10050;
        saf->mnp.motory = MNP_MOTORY_10050;
    #endif 
    #ifdef NF120x200
        saf->mnp.xbild  = MNP_XBILD_120200;
        saf->mnp.ybild  = MNP_YBILD_120200;
        saf->mnp.motorx = MNP_MOTORX_120200;
        saf->mnp.motory = MNP_MOTORY_120200;
    #endif 
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    servicepos (Serviceposition) */
    #ifdef NF120x70
        saf->servicepos.xbild = SP_XBILD_12070;
        saf->servicepos.ybild = SP_YBILD_12070;
        saf->servicepos.motorx = SP_MOTORX_12070;
        saf->servicepos.motory = SP_MOTORY_12070;
    #endif 
    #ifdef NF120x170
        saf->servicepos.xbild = SP_XBILD_120170;
        saf->servicepos.ybild = SP_YBILD_120170;
        saf->servicepos.motorx = SP_MOTORX_120170;
        saf->servicepos.motory = SP_MOTORY_120170;
    #endif 
    #ifdef NF100x50
    #endif 
    #ifdef NF120x200
        saf->servicepos.xbild = SP_XBILD_120200;
        saf->servicepos.ybild = SP_YBILD_120200;
        saf->servicepos.motorx = SP_MOTORX_120200;
        saf->servicepos.motory = SP_MOTORY_120200;
    #endif 
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Referenzpunkte */
    // Naehfeld-Positionen
    saf->refpkt[0].x = saf->refpkt[2].x = saf->refpkt[4].x = saf->refpkt[6].x = 0;
    saf->refpkt[0].y = saf->refpkt[2].y = saf->refpkt[4].y = saf->refpkt[6].y = 0;
    saf->refpkt[1].x = 0;
    #ifdef NF120x70
        saf->refpkt[1].y = -7000;
        saf->refpkt[3].x = 6000;
        saf->refpkt[3].y = 0;
        saf->refpkt[5].x = -6000;
        saf->refpkt[5].y = 0;
    #endif 
    #ifdef NF120x170
        saf->refpkt[1].y = -7000;
        saf->refpkt[3].x = 6000;
        saf->refpkt[3].y = 0;
        saf->refpkt[5].x = -6000;
        saf->refpkt[5].y = 0;
    #endif 
    #ifdef NF100x50
        saf->refpkt[1].y = -5000;
        saf->refpkt[3].x = 5000;
        saf->refpkt[3].y = 0;
        saf->refpkt[5].x = -5000;
        saf->refpkt[5].y = 0;
    #endif 
    #ifdef NF120x200
        saf->refpkt[1].y = -15880;  // -7000;
        saf->refpkt[3].x = 6000;
        saf->refpkt[3].y = 0;
        saf->refpkt[5].x = -6000;
        saf->refpkt[5].y = 0;
    #endif 
    /* EasyCODE ) */
    // Winkel fuer Fadenschieber
    
    //saf->fadschein_winkel1 = ((float)25/180)*M_PI;  // Bogenmass 
    //saf->fadschein_winkel2 = ((float)83/180)*M_PI;  // Bogenmass 
    //saf->fadschaus_winkel1 = ((float)20/180)*M_PI;  // Bogenmass 
    //saf->fadschaus_winkel2 = ((float)88/180)*M_PI;  // Bogenmass 
    
    
    /*
    saf->fadschein_winkel1 = ((float) 2/180)*M_PI;  // Bogenmass Quadrant 1   SW014
    saf->fadschaus_winkel1 = ((float) 6/180)*M_PI;  // Bogenmass Quadrant 1   SW014
    
    saf->fadschein_winkel2 = ((float)46/180)*M_PI;  // Bogenmass Quadrant 4   SW014
    saf->fadschaus_winkel2 = ((float)51/180)*M_PI;  // Bogenmass Quadrant 4   SW014
    */
    
    
    // Zeiten
    saf->footzt = 5;                         // Zeit fuer Fuss ab
    saf->nmstopzt = 100;                     // Zeit zum Stoppen des Naehmotors
    saf->niszeit = 100;                      // Ueberwachungszeit fuer NIS
    saf->smdelayzt = 5;                      // Verzoegerungszeit vor dem genauen Positionieren der Sm
    #ifdef NF120x70
        saf->initzt = 1500;                      // Zeit um Sm in Initiatoren zu fahren
    #endif 
    #ifdef NF120x170
        saf->initzt = 3000;                      // Zeit um Sm in Initiatoren zu fahren
    #endif 
    #ifdef NF120x200
        saf->initzt = 3600;                      // Zeit um Sm in Initiatoren zu fahren
    #endif 
    saf->tadelzt = 50;                       // Verzoegerung zwischen Einzeltakt und Dauertakten
    saf->taktzt = 0;                         // Wiederholrate beim Dauertakten
    saf->prodelzt = 50;
    saf->profastzt = 200;
    saf->quick_delayzt = 10;                 // EEPROM-Programmierung bei Quick
    saf->naezeit1 = 5;                       // Zeit nach Klammer auf bis sbernahme Klammer zu
    saf->startverz = 20;                     // Verz"gerung bei Automatikstart
    saf->autoclampverz_zt = 20;              // Verz"gerung Klammer zu beim Auto-Start
    saf->fsverz = 20;                        // Verz"gerung beim Start mit Fussschalter
    /* EasyCODE - */
    // Schrittmotorwerte
    saf->pll = 10;
    saf->move_vort = 100;                    // Vorteiler Rampe mit Dachverl"ngerung
    saf->move_vort_uspg = 200;               // Vorteiler Rampe mit Dachverl"ngerung bei Unterspannung
    saf->move3_vort = 70;                    // Vorteiler Rampe mit Dachverl"ngerung
    saf->takt_vort = 2000;                   // Vorteiler Takten
    saf->takt_vort_uspg = 500;               // Vorteiler Takten bei Unterspannung
    saf->takt_vort2 = 256;                   // Vorteiler sychrones Takten
    saf->ramp_vort = 64;                     // Vorteiler beim Naehen
    saf->stst_vort = 1000;                   // Vorteiler fuer Start-Stop-Frequenz 2 kHz
    saf->slow_vort = 20000;                  // Vorteiler langsames Fahren zum genauen Positionieren (50Hz)
    saf->vorserv = 2500;                     // Vorteiler Service SM
    saf->move_adr = 161;                     // Adresse der Rampe mit Dachverlaengerung
    saf->move_abramp = 48;                   // Pulse in der Abrampe der Rampe mit Dachverlaengerung
    saf->move3_adr = 150;                    // Adresse der Rampe mit Dachverlaengerung
    saf->move3_abramp = 48;                  // Pulse in der Abrampe der Rampe mit Dachverlaengerung
    
    saf->xvorteiler1 = 2000;
    saf->xvorteiler2 = 25000;
    saf->xvorteiler3 = 6000;
    saf->xvorteiler4 = 3000;
    
    // Geschwindigkeiten
    saf->gswred_uspg = 5;                    // bei Unterspannunug werden alle Werte der Koordinaten-
                                             // Drehzahlzuordnung um diesen Wert abgesenkt
    
    // Zaehler
    saf->piececounter = 0;
    saf->bobcnt = 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaehen::winit */
/* EasyCODE F */
void KlasseNaehen::winit (void)
{
    unsigned char gswmem;
    
    carposok_mem = saf->carposok;
    saf->carposok = FALSE;
    firsthome = TRUE;
    res_barcode();
    nup.bitomo_init ();
    gswmem = saf->md.annaetab [0];
    naetab_boot ();
    saf->md.annaetab [0] = gswmem;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Klasse Naehen::naetab_boot */
/* EasyCODE F */
void KlasseNaehen::naetab_boot (void)
{
    /* EasyCODE ( 0 
    Tabellen fuer
    intermittierendes Fahren
    laden */
    memcpy (saf->md.annaetab, annaeurtab, sizeof saf->md.annaetab);
    memcpy (saf->bremstab, bremsurtab, sizeof saf->bremstab);
    memcpy (saf->inttab, inturtab, sizeof saf->inttab);
    if ( saf->md.matdicke == DUENN )
    {
        // leichte Schablone
        memcpy (saf->stigswtab, stigswurtab, sizeof saf->stigswtab);
        memcpy (saf->koorgswtab1, koorgswtab1_duenn, sizeof (saf->koorgswtab1));
        memcpy (saf->koorgswtab2, koorgswtab2_duenn, sizeof (saf->koorgswtab2));
    }
    else
    {
        // schwere Schablone
        memcpy (saf->stigswtab, stigswurtab, sizeof saf->stigswtab);
        memcpy (saf->koorgswtab1, koorgswtab1_duenn, sizeof (saf->koorgswtab1));
        memcpy (saf->koorgswtab2, koorgswtab2_duenn_schwer, sizeof (saf->koorgswtab2));
    }
    /* EasyCODE ) */
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ) */
