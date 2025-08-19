/* EasyCODE V8 */
/* EasyCODE ( 0 
progport */
#include "portal.h"
/* EasyCODE ( 0 
KlasseProg::grundzustand */
/* EasyCODE F */
unsigned char KlasseProg::grundzustand(void)
{
    unsigned char retwert= 0xff;
    unsigned char loopflag, loopflag2;
    while ( TRUE )
    {
        switch ( key )
        {
        case 0 :
            /* EasyCODE ( 0 
            key = 0,   Einstieg ins Programmieren, Maschinen-Grundstellung pruefen */
            #ifdef buerotest
                saf->mnp_gueltig = 0x55;
                nup.carlp = TRUE;
                saf->carposok = TRUE;
                nup.workbuf.koorist = saf->mnp;
            #endif 
            /* EasyCODE < */
            konv.AnfRiegelStatus.fertig = TRUE;
            /* EasyCODE > */
            if ( saf->mnp_gueltig!=0x55 )
            {
                koor.eing_err = 1;
                error_res();
                retwert = E_GST;
            }
            else
            {
                if ( !nup.carlp || nae.faedelreq || !saf->carposok )
                {
                    // Fehler: Maschine nicht
                    // in Grundstellung
                    koor.eing_err = 8;
                    error_res();
                    retwert = E_GST;
                }
                else
                {
                    if ( taste_los() == 0xff )
                    {
                        retwert = E_RUHE;
                    }
                    else
                    {
                        eing_botsch1.senden(BV029);
                        eing_botsch1.senden(klammers);
                        #ifdef buerotest
                            // Klammer geschlossen
                            eing_botsch1.senden(showenters);
                        #else 
                            if ( e51u2_1.getZustand() && e51u2_2.getZustand() )
                            {
                                // Klammer geschlossen
                                eing_botsch1.senden(showenters);
                            }
                            else
                            {
                                // Klammer offen
                            }
                        #endif 
                        eing.ioact = TRUE;
                        key = 1;
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            key = 1,   Klammer einlegen, Programmnummer waehlen */
            switch ( keyb.taste() )
            {
            case TEINGABE :
                eing.ioact = FALSE;
                retwert = E_GST;
                break;
            case TENDE :
                eing.ioact = FALSE;
                retwert = E_EXIT;
                break;
            case TENTER :
                if ( taste_los() == 0xff )
                {
                    key = 10;
                }
                else
                {
                    eing.ioact = FALSE;
                    /* EasyCODE ( 0 
                    ausgewaehlte Programmnummer bestimmen */
                    switch ( saf->md.prsel )
                    {
                    case 0 :
                        /* EasyCODE = */
                    default:
                        // einfaches Programm
                        pnr=saf->md.pnr;
                        break;
                    case 1 // Sequenz
                    :
                        pnr=saf->md.seq_array[saf->md.seq][saf->md.pnrindex];
                        break;
                    case 2 // verbundene Programme
                    :
                        pnr=saf->md.vpnr_array[saf->md.vpnr][saf->md.pnrindex];
                        break;
                    }
                    /* EasyCODE ) */
                    /* EasyCODE ( 0 
                    Programmnummer waehlen */
                    pargetval.auftrag = 3;       // pnr wahl
                    pargetval.altwert = pnr;
                    pargetval.string = pnrs;
                    pargetval.pikto = pnrps;
                    pargetval.maxwert = PNRMAX;
                    pargetval.minwert = PNRMIN;
                    pargetval.kommastellen = 0;
                    pargetval.sonder2string = pnrkomms;
                    switch ( getvalue_eing() )
                    {
                    case 0 :
                        /* EasyCODE ( 0 
                        ENTER */
                        pnr = pargetval.wert;
                        switch ( di.loadprog (pnr) )
                        {
                        case 0 :
                            // ok
                            if ( *(ndatanf+KPROGNR)==2 )
                            {
                                /* EasyCODE_STRUCTURED_COMMENT_START */
                                /*
                                koor.eing_err = 48;
                                eing.errpar = pnr;
                                error_res();
                                retwert = E_GST;
                                EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                                */
                                /* EasyCODE_STRUCTURED_COMMENT_END */
                                // Brother-Progr.
                                // positionieren
                                key = 9;
                            }
                            else
                            {
                                if ( *(ndatanf + GKENNUNG + 1) != PORTAL )
                                {
                                    koor.eing_err = 20;
                                    error_res();
                                    retwert = E_GST;
                                }
                                else
                                {
                                    /* EasyCODE < */
                                    *(unsigned long *)(ndatanf + KPROGLNG) = *(unsigned long *)(ndatanf + KSTICHDAT);
                                    *(ndatanf + GKENNUNG + 2) = 0xff;
                                    konv.prog_anf = ndatanf;
                                    ppointer =(struct p_abschstruct *)(ndatanf + GPROGNR);
                                    abschnr = 1;
                                    koorref.x = koorref.y = 0;
                                    blockanf = blockend = NULL;
                                    koorptr = nup.workbuf.koorist;
                                    eing.pup.set_pzbl (ppointer);
                                    eing.pup.lastabsch[0] = 0;
                                    eing.ioact = TRUE;
                                    key = 2;
                                    /* EasyCODE > */
                                }
                            }
                            break;
                        case 1 //Prog > MAXPROGLEN
                        :
                            koor.eing_err = 43;
                            eing.errpar = pnr;
                            error_res();
                            retwert = E_GST;
                            break;
                        default:
                            /* EasyCODE < */
                            // Programm nicht vorhanden oder Lesefehler
                            memmove (ndatanf, urprog, *(unsigned long *)(urprog + KPROGLNG));
                            *(unsigned int *) (ndatanf + GPROGNR + 1) = pnr;
                            konv.prog_anf = ndatanf;
                            ppointer =(struct p_abschstruct *)(ndatanf + GPROGNR);
                            abschnr = 1;
                            koorref.x = koorref.y = 0;
                            blockanf = blockend = NULL;
                            koorptr = nup.workbuf.koorist;
                            eing.pup.set_pzbl (ppointer);
                            eing.pup.lastabsch[0] = 0;
                            eing.ioact = TRUE;
                            key = 2;
                            /* EasyCODE > */
                            break;
                        }
                        break;
                        /* EasyCODE ) */
                    case 1 //ESC
                    :
                        retwert = E_GST;
                        break;
                    default:
                        // Stopanf
                        retwert = E_RUHE;
                        break;
                    }
                    /* EasyCODE ) */
                }
                break;
            case TSCHABLONE :
                #ifdef buerotest
                #else 
                    if ( e51u2_1.getZustand() && e51u2_2.getZustand() )
                    {
                        // Klammer geschlossen
                        y51u1.setAuftrag(); // Klammer auf
                        eing_botsch1.senden(delenters);
                    }
                    else
                    {
                        // Klammer offen
                        y51u2.setAuftrag(); // Klammer schliessen
                        eing_botsch1.senden(showenters);
                    }
                    while ( !y51u1.fertig() || !y51u2.fertig() )
                    {
                        if ( koor.eingstop() )
                        {
                            key = 10;
                            break;
                        }
                    }
                #endif 
                if ( taste_los() == 0xff )
                {
                    key = 10;
                }
                break;
            default:
                if ( taste_los() == 0xff )
                {
                    key = 10;
                }
                break;
            case 0xff :
                break;
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            key = 2,  Grundzustand */
            eing_botsch1.senden (BV028);
            eing.pup.pstatuszeile ();
            eing.pup.abschparanz ();
            eing.pup.kooranz();
            loopflag = TRUE;
            while ( loopflag )
            {
                if ( eing.pup.dauertakten )
                {
                    eing_botsch1.senden(stopdauertaktstr);
                    eing.pup.dauertakten = FALSE;
                }
                switch ( getkey () )
                {
                case TINS :
                    /* EasyCODE ( 0 
                    Taste Einfuegen */
                    if ( (abschnr >= 5) && (ppointer->art != AB_KREISSTP) && (ppointer->art != AB_BOGENSTP) && (ppointer->art != AB_PRENDE) )
                    {
                        key = 3;
                    }
                    else
                    {
                        eing_botsch1.senden (BELLBELL);
                    }
                    loopflag = FALSE;
                    break;
                    /* EasyCODE ) */
                case TFWD :
                    /* EasyCODE ( 0 
                    Taste Vorwaerts Takten */
                    switch ( eing.pup.pfwd () )
                    {
                    case 0 :
                        break;
                    case 1 :
                        error_res ();
                        if ( !saf->carposok )
                        {
                            retwert = E_GST;
                            eing.ioact = FALSE;
                        }
                        loopflag = FALSE;
                        break;
                    case 2 :
                        koor.eing_err = 30;
                        eing.errpar = konverr;
                        eing.errpar2 = absch_fehler;
                        error_res ();
                        loopflag = FALSE;
                        break;
                    case 3 :
                        loopflag2 = TRUE;
                        while ( loopflag2 )
                        {
                            switch ( eing.pup.inskoord (PANFANGSPUNKT) )
                            {
                            case 0 :
                                ppointer->par1 = nup.workbuf.koorist.xbild;
                                ppointer->par2 = nup.workbuf.koorist.ybild;
                                koorptr = nup.workbuf.koorist;
                                y50u2.setAuftrag();
                                while ( !y50u2.fertig() )
                                {
                                    if ( koor.eingstop() )
                                    {
                                        error_res();
                                        break;
                                    }
                                }
                                if ( y50u2.fertig() )
                                {
                                    loopflag2 = FALSE;
                                    loopflag = FALSE;
                                }
                                break;
                            case 1 :
                                error_res();
                                if ( saf->carposok )
                                {
                                }
                                else
                                {
                                    retwert = E_GST;
                                    loopflag2 = FALSE;
                                    loopflag = FALSE;
                                }
                                break;
                            case 2 :
                                eing_botsch1.senden (BELLBELL);
                                break;
                            default:
                                break;
                            }
                        }
                        break;
                    }
                    break;
                    /* EasyCODE ) */
                case TBACK :
                    /* EasyCODE ( 0 
                    Taste Rueckwaerts Takten */
                    switch ( eing.pup.pback () )
                    {
                    case 0 :
                        break;
                    case 1 :
                        error_res ();
                        if ( !saf->carposok )
                        {
                            retwert = E_GST;
                            eing.ioact = FALSE;
                        }
                        loopflag = FALSE;
                        break;
                    case 2 :
                        koor.eing_err = 30;
                        eing.errpar = konverr;
                        eing.errpar2 = absch_fehler;
                        error_res ();
                        loopflag = FALSE;
                        break;
                    }
                    break;
                    /* EasyCODE ) */
                case TFUSS :
                    /* EasyCODE ( 0 
                    Taste Fuss */
                    if ( !eing.pup.pfoot () )
                    {
                        error_res ();
                        loopflag = FALSE;
                    }
                    break;
                    /* EasyCODE ) */
                case TFIND :
                    /* EasyCODE ( 0 
                    Taste Find (Suchen) */
                    if ( abschnr >= 5 )
                    {
                        switch ( eing.pup.find () )
                        {
                        case 0 :
                            break;
                        case 1 :
                            error_res ();
                            if ( !saf->carposok )
                            {
                                retwert = E_GST;
                                eing.ioact = FALSE;
                            }
                            break;
                        case 2
                                                                                                                                                                                                                                                                                                                                         // 
                             //Koordinaten wurden abgeschnitten
                        :
                            koor.eing_err = 27;
                            error_res ();
                            break;
                        }
                    }
                    else
                    {
                        eing_botsch1.senden (BELLBELL);
                    }
                    loopflag = FALSE;
                    break;
                    /* EasyCODE ) */
                case TKOMMENTAR :
                    /* EasyCODE ( 0 
                    Taste Kommentar */
                    pargetstring.altstring = ndatanf + KKOMMENTAR;
                    pargetstring.string = progpikto;
                    pargetstring.anz_char = KPROGLNG - KKOMMENTAR - 1;
                    switch ( getstring() )
                    {
                    case 0 :
                        strcpy(ndatanf + KKOMMENTAR,(char*)pargetstring.neustring);
                        loopflag = FALSE;
                        break;
                    case 1  // ESC
                    :
                        loopflag = FALSE;
                        break;
                    default:
                        // Stoppanf
                        error_res ();
                        loopflag = FALSE;
                        break;
                    }
                    break;
                    /* EasyCODE ) */
                case TBLOCKANF :
                    /* EasyCODE ( 0 
                    Taste Blockanfang */
                    if ( eing.pup.mark_enable_test (ppointer) )
                    {
                        blockanf = ppointer;
                        loopflag = FALSE;
                    }
                    else
                    {
                        eing_botsch1.senden (BELLBELL);
                    }
                    break;
                    /* EasyCODE ) */
                case TBLOCKEND :
                    /* EasyCODE ( 0 
                    Taste Blockende */
                    if ( eing.pup.mark_enable_test (ppointer) )
                    {
                        blockend = ppointer;
                        loopflag = FALSE;
                    }
                    else
                    {
                        eing_botsch1.senden (BELLBELL);
                    }
                    break;
                    /* EasyCODE ) */
                case TBLOCK :
                    /* EasyCODE ( 0 
                    Taste Block */
                    if ( eing.pup.mark_check() == 0xff )
                    {
                        // Block richtig markiert
                        key = 6;
                        loopflag = FALSE;
                    }
                    else
                    {
                        // Block nicht oder
                        // nicht richtig markiert
                        eing_botsch1.senden (BELLBELL);
                    }
                    break;
                    /* EasyCODE ) */
                case TBILD :
                    /* EasyCODE ( 0 
                    Taste Bild */
                    key = 7;
                    loopflag = FALSE;
                    break;
                    /* EasyCODE ) */
                case TREF :
                    /* EasyCODE ( 0 
                    Taste Referenzposition */
                    koorref.x = nup.workbuf.koorist.xbild;
                    koorref.y = nup.workbuf.koorist.ybild;
                    eing.pup.kooranz();
                    break;
                    /* EasyCODE ) */
                case TDEL :
                    /* EasyCODE ( 0 
                    Taste Loeschen */
                    key = 4;
                    loopflag = FALSE;
                    break;
                    /* EasyCODE ) */
                case TPROG :
                    /* EasyCODE ( 0 
                    Taste Aendern */
                    key = 5;
                    loopflag = FALSE;
                    break;
                    /* EasyCODE ) */
                case STOPANF :
                    /* EasyCODE ( 0 
                    Stopanforderung */
                    error_res ();
                    loopflag = FALSE;
                    break;
                    /* EasyCODE ) */
                case TEINGABE :
                    /* EasyCODE ( 0 
                    Taste Eingabe */
                    if ( taste_los() == 0xff )
                    {
                        key = 10;
                    }
                    else
                    {
                        if ( eing.pup.prog_exit () )
                        {
                            retwert = E_GST;
                        }
                        else
                        {
                            koor.eing_err = 30;
                            eing.errpar = konverr;
                            eing.errpar2 = absch_fehler;
                            error_res ();
                        }
                    }
                    loopflag = FALSE;
                    break;
                    /* EasyCODE ) */
                case TENDE :
                    /* EasyCODE ( 0 
                    Taste Ende */
                    if ( eing.pup.prog_exit () )
                    {
                        retwert = E_EXIT;
                    }
                    else
                    {
                        koor.eing_err = 30;
                        eing.errpar = konverr;
                        eing.errpar2 = absch_fehler;
                        error_res ();
                    }
                    loopflag = FALSE;
                    break;
                    /* EasyCODE ) */
                default:
                    break;
                }
            }
            break;
            /* EasyCODE ) */
        case 3 :
            // Einfuegen
            retwert = einf ();
            break;
        case 4 :
            // Loeschen
            retwert = del ();
            break;
        case 5 :
            // Aendern
            retwert = aender ();
            break;
        case 6 :
            // Block
            retwert = block ();
            break;
        case 7 :
            // Bild
            retwert = bild ();
            break;
        case 9 :
            // Brother-Bild positionieren
            retwert = brotherpos ();
            break;
        case 10 :
            /* EasyCODE ( 0 
            key = 10,   error_res */
            error_res();
            if ( taste_los() == 0xff )
            {
                retwert = E_RUHE;
            }
            else
            {
                key = 0;
            }
            break;
            /* EasyCODE ) */
        default:
            break;
        }
        if ( retwert != 0xff )
        {
            break;
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProg::einf */
/* EasyCODE F */
unsigned char KlasseProg::einf(void)
{
    /* EasyCODE < */
    unsigned char retwert = 0xff;
    unsigned char einfkey = 0;
    unsigned char loopflag = TRUE;
    unsigned char loopflag2;
    int vpartemp = 0;
    ikoordtyp diff;
    struct p_abschstruct *hlpptr, *workptr;
    unsigned int blocklng, i;
    /* EasyCODE > */
    while ( loopflag )
    {
        switch ( einfkey )
        {
        case 0 :
            /* EasyCODE ( 0 
            einfkey = 0,   Anzeige, Tasten abfragen */
            eing_botsch1.senden (BV032);
            if ( pzbl.faden )
            {
                eing_botsch1.senden (pschneidstr);
            }
            else
            {
                eing_botsch1.senden (pannaehstr);
            }
            if ( *(ndatanf+KPROGNR) == 3 )
            {
                // 1-Nadel
                if ( pzbl.flag1 )
                {
                    eing_botsch1.senden (pnadeldrehenein);
                }
                else
                {
                    eing_botsch1.senden (pnadeldrehenaus);
                }
            }
            eing.pup.pstatuszeile ();
            eing.pup.abschparanz ();
            eing.pup.kooranz();
            switch ( getkey () )
            {
            case TPROG :
                /* EasyCODE ( 0 
                Taste Programmieren */
                key = 2;
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TENDE :
                /* EasyCODE ( 0 
                Taste Ende */
                if ( eing.pup.prog_exit () )
                {
                    retwert = E_EXIT;
                }
                else
                {
                    koor.eing_err = 30;
                    eing.errpar = konverr;
                    eing.errpar2 = absch_fehler;
                    error_res ();
                }
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TEINGABE :
                /* EasyCODE ( 0 
                Taste Eingabe */
                if ( taste_los() == 0xff )
                {
                    key = 10;
                }
                else
                {
                    if ( eing.pup.prog_exit () )
                    {
                        retwert = E_GST;
                    }
                    else
                    {
                        koor.eing_err = 30;
                        eing.errpar = konverr;
                        eing.errpar2 = absch_fehler;
                        error_res ();
                    }
                }
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TBLOCK :
                /* EasyCODE ( 0 
                Taste Block */
                einfkey = 1;
                break;
                /* EasyCODE ) */
            case TGERADE :
                /* EasyCODE ( 0 
                Taste Gerade */
                if ( pzbl.stichl == 0 )
                {
                    eing_botsch1.senden (nostichl);
                    switch ( getkey() )
                    {
                    case TERROR :
                        break;
                    case STOPANF :
                        error_res();
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    einfkey = 2;
                }
                break;
                /* EasyCODE ) */
            case TSTICH :
                /* EasyCODE ( 0 
                Taste Einzelstich */
                einfkey = 3;
                break;
                /* EasyCODE ) */
            case TEILGANG :
                /* EasyCODE ( 0 
                Taste Eilgang */
                if ( pzbl.faden )
                {
                    eing_botsch1.senden (BELLBELL);
                }
                else
                {
                    einfkey = 4;
                }
                break;
                /* EasyCODE ) */
            case TKURVENPUNKT :
                /* EasyCODE ( 0 
                Taste Kurve */
                if ( pzbl.stichl == 0 )
                {
                    eing_botsch1.senden (nostichl);
                    switch ( getkey() )
                    {
                    case TERROR :
                        break;
                    case STOPANF :
                        error_res();
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    einfkey = 5;
                }
                break;
                /* EasyCODE ) */
            case TKURVENENDE :
                /* EasyCODE ( 0 
                Taste Kurvenende */
                eing.pup.ins_curvend ();
                eing.pup.set_pzbl (ppointer);
                break;
                /* EasyCODE ) */
            case TKREIS :
                /* EasyCODE ( 0 
                Taste Kreis */
                if ( pzbl.stichl == 0 )
                {
                    eing_botsch1.senden (nostichl);
                    switch ( getkey() )
                    {
                    case TERROR :
                        break;
                    case STOPANF :
                        error_res();
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    einfkey = 6;
                }
                break;
                /* EasyCODE ) */
            case TBOGEN :
                /* EasyCODE ( 0 
                Taste Bogen */
                if ( pzbl.stichl == 0 )
                {
                    eing_botsch1.senden (nostichl);
                    switch ( getkey() )
                    {
                    case TERROR :
                        break;
                    case STOPANF :
                        error_res();
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    einfkey = 8;
                }
                break;
                /* EasyCODE ) */
            case TANNAEHEN :
                /* EasyCODE ( 0 
                Taste Annaehen */
                einfkey = 10;
                break;
                /* EasyCODE ) */
            case TANFANGSRIEGEL :
                /* EasyCODE ( 0 
                Taste Anfangsriegel */
                einfkey = 11;
                break;
                /* EasyCODE ) */
            case TFADENSCHNEIDEN :
                /* EasyCODE ( 0 
                Taste Fadenschneiden */
                einfkey = 12;
                break;
                /* EasyCODE ) */
            case TENDRIEGEL :
                /* EasyCODE ( 0 
                Taste Endriegel */
                einfkey = 13;
                break;
                /* EasyCODE ) */
            case TNAEHENAUS :
                /* EasyCODE ( 0 
                Taste Naehen aus */
                einfkey = 14;
                break;
                /* EasyCODE ) */
            case TNADELDREHEN :
                /* EasyCODE ( 0 
                Taste Nadeldrehen */
                einfkey = 20;
                break;
                /* EasyCODE ) */
            case TSTANDARDSTICHL :
                /* EasyCODE ( 0 
                Taste Standardstichlaenge */
                einfkey = 15;
                break;
                /* EasyCODE ) */
            case TSTI :
                /* EasyCODE ( 0 
                Taste Stichlaenge */
                einfkey = 16;
                break;
                /* EasyCODE ) */
            case TSTICHBREITE :
                /* EasyCODE ( 0 
                Taste Stichbreite */
                einfkey = 17;
                break;
                /* EasyCODE ) */
            case TMFUNKTIONEN :
                /* EasyCODE ( 0 
                Taste M-Funktionen */
                einfkey = 18;
                break;
                /* EasyCODE ) */
            case STOPANF :
                /* EasyCODE ( 0 
                Stopanforderung */
                error_res ();
                break;
                /* EasyCODE ) */
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            einfkey = 1,   Block einfuegen */
            switch ( eing.pup.mark_check () )
            {
            case 0xff :
                eing_botsch1.senden (blockconfirm);
                loopflag2 = TRUE;
                while ( loopflag2 )
                {
                    switch ( getkey () )
                    {
                    case TENTER :
                        /* EasyCODE ( 0 
                        Block einfuegen */
                        if ( eing.pup.absch_koord (blockanf) )
                        {
                            diff.x = koorptr.xbild - blockanf->par1;
                            diff.y = koorptr.ybild - blockanf->par2;
                        }
                        else
                        {
                            konv.suchekoord (FP_OFF (blockanf), (ikoordtyp *) &nup.koorsoll[0]);
                            diff.x = koorptr.xbild - nup.koorsoll [0].xbild;
                            diff.y = koorptr.ybild - nup.koorsoll [0].ybild;
                        }
                        hlpptr = eing.pup.find_absch (AB_PRENDE, ppointer);
                        blocklng = FP_OFF (blockend) - FP_OFF (blockanf);
                        switch ( eing.pup.ptr_in_block (ppointer) )
                        {
                        case 2 :
                            /* EasyCODE < */
                            // auf Blockanfang
                            /* EasyCODE > */
                        case 0xff :
                            /* EasyCODE < */
                            // in Block
                            /* EasyCODE > */
                            if ( FP_OFF (hlpptr + 2*blocklng/5 + 1) > MAXPROGLEN )
                            {
                                koor.eing_err = 29;
                                error_res ();
                                einfkey = 0;
                                loopflag2 = FALSE;
                            }
                            else
                            {
                                // Block nach hinten
                                memmove (hlpptr+1+blocklng/5, blockanf+1, blocklng);
                                // Platz schaffen fuer Block
                                memmove (ppointer+1+blocklng/5, ppointer+1, FP_OFF(hlpptr) - FP_OFF(ppointer));
                                // Block in freien Platz
                                memmove (ppointer+1, hlpptr+1+blocklng/5, blocklng);
                            }
                            break;
                        default:
                            if ( FP_OFF (hlpptr + 1 + blocklng/5) > MAXPROGLEN )
                            {
                                koor.eing_err = 29;
                                error_res ();
                                einfkey = 0;
                                loopflag2 = FALSE;
                            }
                            else
                            {
                                // Platz schaffen fuer Block
                                memmove (ppointer+1+blocklng/5, ppointer+1, FP_OFF(hlpptr) - FP_OFF(ppointer));
                                if ( eing.pup.ptr_in_block (ppointer) == 4 )
                                {
                                    blockanf += blocklng/5;
                                    blockend += blocklng/5;
                                }
                                /* EasyCODE < */
                                memmove (ppointer+1, blockanf+1, blocklng); // Block in freien Platz
                                /* EasyCODE > */
                            }
                            break;
                        }
                        if ( loopflag2 )
                        {
                            blockanf = ppointer;
                            blockend = ppointer + blocklng/5;
                            for ( workptr = blockanf + 1;  workptr <= blockend;  workptr++ )
                            {
                                if ( eing.pup.absch_koord (workptr) )
                                {
                                    workptr->par1 += diff.x;
                                    workptr->par2 += diff.y;
                                }
                            }
                            if ( eing.pup.absch_koord (blockend) )
                            {
                                diff.x = blockend->par1;
                                diff.y = blockend->par2;
                            }
                            else
                            {
                                konv.suchekoord (FP_OFF(blockend), &diff);
                            }
                            konv.verschiebe_abschnitt (FP_OFF (blockend), diff);
                            *(unsigned long *)(ndatanf + KPROGLNG) += blocklng;
                            *(unsigned long *)(ndatanf + KSTICHDAT) += blocklng;
                            einfkey = 0;
                        }
                        break;
                        /* EasyCODE ) */
                    case TESC :
                        einfkey = 0;
                        loopflag2 = FALSE;
                        break;
                    case TENDE :
                        if ( eing.pup.prog_exit () )
                        {
                            retwert = E_EXIT;
                            loopflag = FALSE;
                        }
                        else
                        {
                            koor.eing_err = 30;
                            eing.errpar = konverr;
                            eing.errpar2 = absch_fehler;
                            error_res ();
                            einfkey = 0;
                        }
                        loopflag2 = FALSE;
                        break;
                    case TEINGABE :
                        if ( eing.pup.prog_exit () )
                        {
                            retwert = E_GST;
                            loopflag = FALSE;
                        }
                        else
                        {
                            koor.eing_err = 30;
                            eing.errpar = konverr;
                            eing.errpar2 = absch_fehler;
                            error_res ();
                            einfkey = 0;
                        }
                        loopflag2 = FALSE;
                        break;
                    case STOPANF :
                        error_res ();
                        einfkey = 0;
                        loopflag2 = FALSE;
                        break;
                    default:
                        break;
                    }
                }
                break;
            default:
                // Block nicht oder nicht richtig markiert
                koor.eing_err = 28;
                error_res ();
                einfkey = 0;
                break;
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            einfkey = 2,   Gerade einfuegen */
            switch ( eing.pup.inskoord (PGERADE) )
            {
            case 0 :
                abschnitt [0].art = AB_GERADE;
                abschnitt [0].par1 = nup.workbuf.koorist.xbild;
                abschnitt [0].par2 = nup.workbuf.koorist.ybild;
                eing.pup.ins_curvend ();
                switch ( eing.pup.absch_einf (ppointer,1) )
                {
                case 0 :
                    abschnr += 1;
                    ppointer += 1;
                    koorptr = nup.workbuf.koorist;
                    eing.pup.set_pzbl (ppointer);
                    if ( taste_los() == 0xff )
                    {
                        error_res();
                        key = 2;
                        loopflag = FALSE;
                    }
                    break;
                case 1 :
                    // Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                    koor.eing_err = 29;
                    error_res();
                    key = 2;
                    loopflag = FALSE;
                    break;
                }
                break;
            case 1 :
                error_res();
                if ( saf->carposok )
                {
                    key = 2;
                }
                else
                {
                    retwert = E_GST;
                }
                loopflag = FALSE;
                break;
            case 2 :
                einfkey = 0;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            einfkey = 3,   Einzelstich einfuegen */
            switch ( eing.pup.inskoord (PSTICH) )
            {
            case 0 :
                if ( (int)(hypot(nup.workbuf.koorist.xbild - koorptr.xbild,
                     nup.workbuf.koorist.ybild - koorptr.ybild))
                      > STLG_MAX+3 )
                {
                    koor.eing_err = 31;
                    error_res();
                }
                else
                {
                    abschnitt [0].art = AB_STICH;
                    abschnitt [0].par1 = nup.workbuf.koorist.xbild;
                    abschnitt [0].par2 = nup.workbuf.koorist.ybild;
                    eing.pup.ins_curvend ();
                    switch ( eing.pup.absch_einf (ppointer,1) )
                    {
                    case 0
                                                                                                                                                                                                                                                                         // 
                         //ok
                    :
                        abschnr += 1;
                        ppointer += 1;
                        koorptr = nup.workbuf.koorist;
                        eing.pup.set_pzbl (ppointer);
                        einfkey = 0;
                        break;
                    case 1
                                                                                                                                                                                                                                                                         // 
                         //Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                    :
                        koor.eing_err = 29;
                        error_res();
                        key = 2;
                        loopflag = FALSE;
                        break;
                    }
                }
                break;
            case 1 :
                error_res();
                if ( saf->carposok )
                {
                    key = 2;
                }
                else
                {
                    retwert = E_GST;
                }
                loopflag = FALSE;
                break;
            case 2 :
                einfkey = 0;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 4 :
            /* EasyCODE ( 0 
            einfkey = 4,   Eilgang einfuegen */
            switch ( eing.pup.inskoord (PEILGANG) )
            {
            case 0 :
                abschnitt [0].art = AB_EIL;
                abschnitt [0].par1 = nup.workbuf.koorist.xbild;
                abschnitt [0].par2 = nup.workbuf.koorist.ybild;
                eing.pup.ins_curvend ();
                switch ( eing.pup.absch_einf (ppointer,1) )
                {
                case 0
                                                                                                                                                                                                         // ok
                :
                    abschnr += 1;
                    ppointer += 1;
                    koorptr = nup.workbuf.koorist;
                    eing.pup.set_pzbl (ppointer);
                    einfkey = 0;
                    break;
                case 1
                                                                                                                                                                                                         // 
                     //Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                :
                    koor.eing_err = 29;
                    error_res();
                    key = 2;
                    loopflag = FALSE;
                    break;
                }
                break;
            case 1 :
                error_res();
                if ( saf->carposok )
                {
                    key = 2;
                }
                else
                {
                    retwert = E_GST;
                }
                loopflag = FALSE;
                break;
            case 2 :
                einfkey = 0;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 5 :
            /* EasyCODE ( 0 
            einfkey = 5,   Kurvenpunkt einfuegen */
            switch ( eing.pup.inskoord (PKURVENPUNKT) )
            {
            case 0 :
                if ( (koorptr.xbild==nup.workbuf.koorist.xbild) &&
                     (koorptr.ybild==nup.workbuf.koorist.ybild) )
                {
                    koor.eing_err = 32;
                    error_res();
                }
                else
                {
                    abschnitt [0].art = AB_KURVSTP;
                    abschnitt [0].par1 = nup.workbuf.koorist.xbild;
                    abschnitt [0].par2 = nup.workbuf.koorist.ybild;
                    switch ( eing.pup.absch_einf (ppointer,1) )
                    {
                    case 0  //ok
                    :
                        abschnr += 1;
                        ppointer += 1;
                        koorptr = nup.workbuf.koorist;
                        eing.pup.set_pzbl (ppointer);
                        if ( taste_los() == 0xff )
                        {
                            error_res();
                            key = 2;
                            loopflag = FALSE;
                        }
                        break;
                    case 1  //Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                    :
                        koor.eing_err = 29;
                        error_res();
                        key = 2;
                        loopflag = FALSE;
                        break;
                    }
                }
                break;
            case 1 :
                error_res();
                if ( saf->carposok )
                {
                    key = 2;
                    eing.pup.ins_curvend();
                }
                else
                {
                    retwert = E_GST;
                }
                loopflag = FALSE;
                break;
            case 2 :
                einfkey = 0;
                eing.pup.ins_curvend();
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 6 :
            /* EasyCODE ( 0 
            einfkey = 6,   Kreisstuetzpunkt 1 eingeben */
            switch ( eing.pup.inskoord (PKREISSTP1) )
            {
            case 0 :
                if ( (koorptr.xbild==nup.workbuf.koorist.xbild) &&
                     (koorptr.ybild==nup.workbuf.koorist.ybild) )
                {
                    koor.eing_err = 32;
                    error_res();
                }
                else
                {
                    abschnitt [0].art = AB_KREISSTP;
                    abschnitt [0].par1 = nup.workbuf.koorist.xbild;
                    abschnitt [0].par2 = nup.workbuf.koorist.ybild;
                    einfkey = 7;
                    if ( taste_los() == 0xff )
                    {
                        error_res();
                        key = 2;
                        loopflag = FALSE;
                    }
                }
                break;
            case 1 :
                error_res();
                if ( saf->carposok )
                {
                    key = 2;
                }
                else
                {
                    retwert = E_GST;
                }
                loopflag = FALSE;
                break;
            case 2 :
                einfkey = 0;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 7 :
            /* EasyCODE ( 0 
            einfkey = 7,   Kreisstuetzpunkt 2 eingeben.
                           Kreis einfuegen */
            switch ( eing.pup.inskoord (PKREISSTP2) )
            {
            case 0 :
                if ( ((nup.workbuf.koorist.xbild==koorptr.xbild) &&
                      (nup.workbuf.koorist.ybild==koorptr.ybild)) ||
                     ((nup.workbuf.koorist.xbild==abschnitt[0].par1) &&
                      (nup.workbuf.koorist.ybild==abschnitt[0].par2)) )
                {
                    koor.eing_err = 32;
                    error_res();
                }
                else
                {
                    abschnitt [1].art = AB_KREISSTP;
                    abschnitt [1].par1 = nup.workbuf.koorist.xbild;
                    abschnitt [1].par2 = nup.workbuf.koorist.ybild;
                    abschnitt [2].art = AB_KREISENDP;
                    abschnitt [2].par1 = koorptr.xbild;
                    abschnitt [2].par2 = koorptr.ybild;
                    eing.pup.ins_curvend ();
                    switch ( eing.pup.absch_einf (ppointer,3) )
                    {
                    case 0 //ok
                    :
                        abschnr += 3;
                        ppointer += 3;
                        eing.pup.set_pzbl (ppointer);
                        einfkey = 0;
                        while ( !eing.pup.movlastp() )
                        {
                            error_res();
                            if ( !saf->carposok )
                            {
                                retwert = E_GST;
                                loopflag = FALSE;
                                break;
                            }
                        }
                        break;
                    case 1 //Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                    :
                        koor.eing_err = 29;
                        error_res();
                        key = 2;
                        loopflag = FALSE;
                        break;
                    }
                }
                break;
            case 1 :
                error_res();
                if ( saf->carposok )
                {
                    key = 2;
                }
                else
                {
                    retwert = E_GST;
                }
                loopflag = FALSE;
                break;
            case 2 :
                einfkey = 0;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 8 :
            /* EasyCODE ( 0 
            einfkey = 8,   Bogenstuetzpunkt 1 eingeben */
            switch ( eing.pup.inskoord (PBOGENSTP1) )
            {
            case 0 :
                if ( (koorptr.xbild==nup.workbuf.koorist.xbild) &&
                     (koorptr.ybild==nup.workbuf.koorist.ybild) )
                {
                    koor.eing_err = 32;
                    error_res();
                }
                else
                {
                    abschnitt [0].art = AB_BOGENSTP;
                    abschnitt [0].par1 = nup.workbuf.koorist.xbild;
                    abschnitt [0].par2 = nup.workbuf.koorist.ybild;
                    einfkey = 9;
                    if ( taste_los() == 0xff )
                    {
                        error_res();
                        key = 2;
                        loopflag = FALSE;
                    }
                }
                break;
            case 1 :
                error_res();
                if ( saf->carposok )
                {
                    key = 2;
                }
                else
                {
                    retwert = E_GST;
                }
                loopflag = FALSE;
                break;
            case 2 :
                einfkey = 0;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 9 :
            /* EasyCODE ( 0 
            einfkey = 9,   Bogenstuetzpunkt 2 eingeben.
                           Bogen einfuegen */
            switch ( eing.pup.inskoord (PBOGENSTP2) )
            {
            case 0 :
                if ( ((nup.workbuf.koorist.xbild==koorptr.xbild) &&
                      (nup.workbuf.koorist.ybild==koorptr.ybild)) ||
                     ((nup.workbuf.koorist.xbild==abschnitt[0].par1) &&
                      (nup.workbuf.koorist.ybild==abschnitt[0].par2)) )
                {
                    koor.eing_err = 32;
                    error_res();
                }
                else
                {
                    abschnitt [1].art = AB_BOGENENDP;
                    abschnitt [1].par1 = nup.workbuf.koorist.xbild;
                    abschnitt [1].par2 = nup.workbuf.koorist.ybild;
                    eing.pup.ins_curvend ();
                    switch ( eing.pup.absch_einf (ppointer,2) )
                    {
                    case 0
                                                                                                                                                                                                                                                                         // 
                         //ok
                    :
                        abschnr += 2;
                        ppointer += 2;
                        koorptr = nup.workbuf.koorist;
                        eing.pup.set_pzbl (ppointer);
                        einfkey = 0;
                        break;
                    case 1
                                                                                                                                                                                                                                                                         // 
                         //Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                    :
                        koor.eing_err = 29;
                        error_res();
                        key = 2;
                        loopflag = FALSE;
                        break;
                    }
                }
                break;
            case 1 :
                error_res();
                if ( saf->carposok )
                {
                    key = 2;
                }
                else
                {
                    retwert = E_GST;
                }
                loopflag = FALSE;
                break;
            case 2 :
                einfkey = 0;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 10 :
            /* EasyCODE ( 0 
            einfkey = 10,   Annaehen einfuegen */
            abschnitt [0].art = AB_ANNAEH;
            abschnitt [0].par1 = 0x0100; // 1 Teilst., 0 Vorw.St.
            abschnitt [0].par2 = 0x0100;
            switch ( eing.pup.absch_einf (ppointer,1) )
            {
            case 0 // ok
            :
                abschnr += 1;
                ppointer += 1;
                eing.pup.ins_curvend();
                eing.pup.set_pzbl (ppointer);
                einfkey = 0;
                break;
            case 1 // Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
            :
                koor.eing_err = 29;
                error_res();
                key = 2;
                loopflag = FALSE;
                break;
            }
            break;
            /* EasyCODE ) */
        case 11 :
            /* EasyCODE ( 0 
            einfkey = 11,   Annaehen + Anfangsriegel einfuegen */
            /* EasyCODE < */
            abschnitt [0].art = AB_ANNAEH;
            abschnitt [0].par1 = 0x0102; // 1 Teilstich, 2 Vorw.Stiche
            abschnitt [0].par2 = 0x0102; // 1 Teilstich, 2 Rueckw.Stiche
            /* EasyCODE > */
            if ( (hlpptr = eing.pup.find_absch (AB_ANNAEH, (struct p_abschstruct *) (ndatanf + GPROGNR))) == NULL )
            {
            }
            else
            {
                if ( (*((unsigned char *)hlpptr + 1) != 0) ||
                     (*((unsigned char *)hlpptr + 3) != 0) )
                {
                    abschnitt [0].par1 = hlpptr->par1;
                    abschnitt [0].par2 = hlpptr->par2;
                }
            }
            switch ( eing.pup.absch_einf (ppointer,1) )
            {
            case 0
                                                                                                                                         // ok
            :
                abschnr += 1;
                ppointer += 1;
                eing.pup.ins_curvend();
                eing.pup.set_pzbl (ppointer);
                einfkey = 0;
                break;
            case 1
                                                                                                                                         // Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
            :
                koor.eing_err = 29;
                error_res();
                key = 2;
                loopflag = FALSE;
                break;
            }
            break;
            /* EasyCODE ) */
        case 12 :
            /* EasyCODE ( 0 
            einfkey = 12,   Fadenschneiden einfuegen */
            for ( i=0, loopflag2=TRUE;  loopflag2;  i++ )
            {
                if ( (ppointer - i)->art == AB_ANNAEH )
                {
                    eing_botsch1.senden(BELLBELL);
                    loopflag2 = FALSE;
                    einfkey = 0;
                }
                else
                {
                    if ( eing.pup.absch_koord (ppointer - i) )
                    {
                        /* EasyCODE < */
                        loopflag2 = FALSE;
                        abschnitt [0].art = AB_SCHNEID;
                        abschnitt [0].par1 = 0x0100; // 1Teilst., 0 Vorw.St.
                        abschnitt [0].par2 = 0x0100; // 1Teilst., 0 Rueckw.St.
                        /* EasyCODE > */
                        switch ( eing.pup.absch_einf (ppointer,1) )
                        {
                        case 0
                                                                                                                                                                                                                                                                                                                                         // 
                             //ok
                        :
                            abschnr += 1;
                            ppointer += 1;
                            eing.pup.ins_curvend();
                            eing.pup.set_pzbl (ppointer);
                            einfkey = 0;
                            break;
                        case 1
                                                                                                                                                                                                                                                                                                                                         // 
                             //Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                        :
                            koor.eing_err = 29;
                            error_res();
                            key = 2;
                            loopflag = FALSE;
                            break;
                        }
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 13 :
            /* EasyCODE ( 0 
            einfkey = 13,   Schneiden + Endriegel einfuegen */
            for ( i=0, loopflag2=TRUE;  loopflag2;  i++ )
            {
                if ( (ppointer - i)->art == AB_ANNAEH )
                {
                    eing_botsch1.senden(BELLBELL);
                    loopflag2 = FALSE;
                    einfkey = 0;
                }
                else
                {
                    if ( eing.pup.absch_koord (ppointer - i) )
                    {
                        loopflag2 = FALSE;
                        abschnitt [0].art = AB_SCHNEID;
                        // 1 Teilst., 2 Vorw.St.
                        abschnitt [0].par1 = 0x0102;
                        // 1 Teilst., 2 Rückw.St.
                        abschnitt [0].par2 = 0x0102;
                        if ( (hlpptr = eing.pup.find_absch (AB_SCHNEID, (struct p_abschstruct *) (ndatanf + GPROGNR))) == NULL )
                        {
                        }
                        else
                        {
                            if ( (*((unsigned char *)hlpptr + 1) != 0) ||
                                 (*((unsigned char *)hlpptr + 3) != 0) )
                            {
                                abschnitt [0].par1 = hlpptr->par1;
                                abschnitt [0].par2 = hlpptr->par2;
                            }
                        }
                        switch ( eing.pup.absch_einf (ppointer,1) )
                        {
                        case 0
                                                                                                                                                                                                                                                                                                                                         // 
                             //ok
                        :
                            abschnr += 1;
                            ppointer += 1;
                            eing.pup.ins_curvend();
                            eing.pup.set_pzbl (ppointer);
                            einfkey = 0;
                            break;
                        case 1
                                                                                                                                                                                                                                                                                                                                         // 
                             //Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                        :
                            koor.eing_err = 29;
                            error_res();
                            key = 2;
                            loopflag = FALSE;
                            break;
                        }
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 14 :
            /* EasyCODE ( 0 
            einfkey = 14,   Naehen aus einfuegen */
            for ( i=0, loopflag2=TRUE;  loopflag2;  i++ )
            {
                if ( (ppointer - i)->art == AB_ANNAEH )
                {
                    eing_botsch1.senden(BELLBELL);
                    loopflag2 = FALSE;
                    einfkey = 0;
                }
                else
                {
                    if ( eing.pup.absch_koord (ppointer - i) )
                    {
                        loopflag2 = FALSE;
                        abschnitt [0].art = AB_NAEHAUS;
                        abschnitt [0].par1 = 0;
                        abschnitt [0].par2 = 0;
                        switch ( eing.pup.absch_einf (ppointer,1) )
                        {
                        case 0 //ok
                        :
                            abschnr += 1;
                            ppointer += 1;
                            eing.pup.ins_curvend();
                            eing.pup.set_pzbl (ppointer);
                            einfkey = 0;
                            break;
                        case 1 //Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                        :
                            koor.eing_err = 29;
                            error_res();
                            key = 2;
                            loopflag = FALSE;
                            break;
                        }
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 15 :
            /* EasyCODE ( 0 
            einfkey = 15,   Standardstichlaenge einfuegen */
            if ( (hlpptr = eing.pup.find_absch (AB_STDST, (struct p_abschstruct *) (ndatanf + GPROGNR))) == NULL )
            {
                pargetval.minwert = STLG_MIN*10;
                pargetval.maxwert = STLG_MAX*10;
                pargetval.auftrag = 0;
                pargetval.altwert = 300;
                pargetval.pikto = ststips;
                pargetval.kommastellen = 2;
                pargetval.string = floatstr2;
                switch ( getvalue_eing() )
                {
                case 0 :
                    // ENTER
                    abschnitt [0].par1 = pargetval.wert;
                    break;
                case 1 :
                    // ESC
                    einfkey = 0;
                    break;
                default:
                    // Stoppanf
                    error_res();
                    einfkey = 0;
                    break;
                }
            }
            else
            {
                abschnitt [0].par1 = hlpptr->par1;
            }
            if ( einfkey == 15 )
            {
                abschnitt [0].art = AB_STDST;
                abschnitt [0].par2 = 0;
                switch ( eing.pup.absch_einf (ppointer,1) )
                {
                case 0 :
                    // ok
                    abschnr += 1;
                    ppointer += 1;
                    eing.pup.ins_curvend();
                    eing.pup.set_pzbl (ppointer);
                    einfkey = 0;
                    break;
                case 1 :
                    // Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                    koor.eing_err = 29;
                    error_res();
                    key = 2;
                    loopflag = FALSE;
                    break;
                }
            }
            break;
            /* EasyCODE ) */
        case 16 :
            /* EasyCODE ( 0 
            einfkey = 16,   Stichlaenge einfuegen */
            if ( pzbl.stichl == 0 )
            {
                pargetval.altwert = 300;
            }
            else
            {
                pargetval.altwert = pzbl.stichl;
            }
            pargetval.minwert = STLG_MIN*10;
            pargetval.maxwert = STLG_MAX*10;
            pargetval.auftrag = 0;
            pargetval.pikto = stips;
            pargetval.kommastellen = 2;
            pargetval.string = floatstr2;
            switch ( getvalue_eing() )
            {
            case 0
                                                                                                                                         // ENTER
            :
                abschnitt [0].par1 = pargetval.wert;
                abschnitt [0].art = AB_STICHL;
                abschnitt [0].par2 = 0;
                switch ( eing.pup.absch_einf (ppointer,1) )
                {
                case 0
                                                                                                                                                                                                         // ok
                :
                    abschnr += 1;
                    ppointer += 1;
                    eing.pup.ins_curvend();
                    eing.pup.set_pzbl (ppointer);
                    einfkey = 0;
                    break;
                case 1
                                                                                                                                                                                                         // 
                     //Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                :
                    koor.eing_err = 29;
                    error_res();
                    key = 2;
                    loopflag = FALSE;
                    break;
                }
                break;
            case 1
                                                                                                                                         // ESC
            :
                einfkey = 0;
                break;
            default:
                // Stoppanf
                error_res();
                einfkey = 0;
                break;
            }
            break;
            /* EasyCODE ) */
        case 17 :
            /* EasyCODE ( 0 
            einfkey = 17,   Stichbreite einfuegen */
            pargetval.minwert = 0;
            pargetval.maxwert = STLG_MAX;
            pargetval.altwert = 30;
            pargetval.auftrag = 0;
            pargetval.pikto = stibreiteps;
            pargetval.kommastellen = 1;
            pargetval.string = floatstr;
            switch ( getvalue_eing() )
            {
            case 0 :
                // ENTER
                abschnitt [0].par1 = pargetval.wert;
                abschnitt [0].art = AB_STICHB;
                if ( pargetval.wert == 0 )
                {
                    abschnitt [0].par2 = 0;
                }
                else
                {
                    loopflag2 = TRUE;
                    while ( loopflag2 )
                    {
                        eing_botsch1.senden (BV035);
                        switch ( getkey () )
                        {
                        case TZZ1LINKS :
                            abschnitt [0].par2 = 0;
                            loopflag2 = FALSE;
                            break;
                        case TZZ1RECHTS :
                            abschnitt [0].par2 = 1;
                            loopflag2 = FALSE;
                            break;
                        case TZZLINKS :
                            abschnitt [0].par2 = 2;
                            loopflag2 = FALSE;
                            break;
                        case TZZRECHTS :
                            abschnitt [0].par2 = 3;
                            loopflag2 = FALSE;
                            break;
                        case TESC :
                            einfkey = 0;
                            loopflag2 = FALSE;
                            break;
                        case STOPANF :
                            /* EasyCODE ( 0 
                            Stopanforderung */
                            error_res ();
                            break;
                            /* EasyCODE ) */
                        default:
                            break;
                        }
                    }
                }
                if ( einfkey == 17 )
                {
                    switch ( eing.pup.absch_einf (ppointer,1) )
                    {
                    case 0 :
                        // ok
                        abschnr += 1;
                        ppointer += 1;
                        eing.pup.ins_curvend();
                        eing.pup.set_pzbl (ppointer);
                        einfkey = 0;
                        break;
                    case 1 :
                        // Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                        koor.eing_err = 29;
                        error_res();
                        key = 2;
                        loopflag = FALSE;
                        break;
                    }
                }
                break;
            case 1 :
                // ESC
                einfkey = 0;
                break;
            default:
                // Stoppanf
                error_res();
                einfkey = 0;
                break;
            }
            break;
            /* EasyCODE ) */
        case 18 :
            /* EasyCODE ( 0 
            einfkey = 18,   M-Funktionen, Bildschirm aufbauen */
            eing_botsch1.senden (BV033);
            eing.pup.pstatuszeile ();
            eing.pup.abschparanz ();
            eing.pup.kooranz();
            if ( pzbl.pout1 )
            {
                sprintf (eing.varstring, out1setres, PRES);
            }
            else
            {
                sprintf (eing.varstring, out1setres, PSET);
            }
            if ( pzbl.pout2 )
            {
                sprintf (eing.varstring1, out2setres, PRES);
            }
            else
            {
                sprintf (eing.varstring1, out2setres, PSET);
            }
            strcat( eing.varstring, eing.varstring1);
            if ( pzbl.pout3 )
            {
                sprintf (eing.varstring1, out3setres, PRES);
            }
            else
            {
                sprintf (eing.varstring1, out3setres, PSET);
            }
            strcat( eing.varstring, eing.varstring1);
            if ( pzbl.pout4 )
            {
                sprintf (eing.varstring1, out4setres, PRES);
            }
            else
            {
                sprintf (eing.varstring1, out4setres, PSET);
            }
            strcat( eing.varstring, eing.varstring1);
            sprintf (eing.varstring1, fusshubs, (float)pzbl.hub/10);
            strcat( eing.varstring, eing.varstring1);
            sprintf (eing.varstring1, fusslevels, (float)pzbl.posunten/10);
            strcat( eing.varstring, eing.varstring1);
            strcat( eing.varstring, fusshublevels);
            strcat( eing.varstring, fusslevellevels);
            if ( pzbl.pout6 )
            {
                sprintf (eing.varstring1, othebenpiktos, POBERTEILHEBENAUS);
            }
            else
            {
                sprintf (eing.varstring1, othebenpiktos, POBERTEILHEBENEIN);
            }
            strcat( eing.varstring, eing.varstring1);
            if ( pzbl.zusp )
            {
                sprintf (eing.varstring1, zuspgpiktos, PZUSSPGAUS);
            }
            else
            {
                sprintf (eing.varstring1, zuspgpiktos, PZUSSPGEIN);
            }
            strcat( eing.varstring, eing.varstring1);
            if ( pzbl.redgsw )
            {
                sprintf (eing.varstring1, redgswpiktos, PREDGSWAUS);
            }
            else
            {
                sprintf (eing.varstring1, redgswpiktos, PREDGSWEIN);
            }
            strcat (eing.varstring, eing.varstring1);
            sprintf (eing.varstring1, vpars, vpartemp);
            strcat (eing.varstring, eing.varstring1);
            eing_botsch1.senden (eing.varstring);
            einfkey = 19;
            break;
            /* EasyCODE ) */
        case 19 :
            /* EasyCODE ( 0 
            einfkey = 19,   M-Funktionen einfuegen */
            switch ( getkey () )
            {
            case TPROG :
                /* EasyCODE ( 0 
                Taste Programmieren */
                key = 2;
                einfkey = 0;
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TENDE :
                /* EasyCODE ( 0 
                Taste Ende */
                if ( eing.pup.prog_exit () )
                {
                    retwert = E_EXIT;
                }
                else
                {
                    koor.eing_err = 30;
                    eing.errpar = konverr;
                    eing.errpar2 = absch_fehler;
                    error_res ();
                }
                einfkey = 18;
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TEINGABE :
                /* EasyCODE ( 0 
                Taste Eingabe */
                if ( taste_los() == 0xff )
                {
                    key = 10;
                }
                else
                {
                    if ( eing.pup.prog_exit () )
                    {
                        retwert = E_GST;
                    }
                    else
                    {
                        koor.eing_err = 30;
                        eing.errpar = konverr;
                        eing.errpar2 = absch_fehler;
                        error_res ();
                    }
                    einfkey = 18;
                }
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TOUT1 :
                /* EasyCODE ( 0 
                Ausgang 1 */
                if ( pzbl.pout1 )
                {
                    abschnitt [0].art = AB_OUTRES;
                }
                else
                {
                    abschnitt [0].art = AB_OUTSET;
                }
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 1;
                break;
                /* EasyCODE ) */
            case TOUT2 :
                /* EasyCODE ( 0 
                Ausgang 2 */
                if ( pzbl.pout2 )
                {
                    abschnitt [0].art = AB_OUTRES;
                }
                else
                {
                    abschnitt [0].art = AB_OUTSET;
                }
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 2;
                break;
                /* EasyCODE ) */
            case TOUT3 :
                /* EasyCODE ( 0 
                Ausgang 3 */
                if ( pzbl.pout3 )
                {
                    abschnitt [0].art = AB_OUTRES;
                }
                else
                {
                    abschnitt [0].art = AB_OUTSET;
                }
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 3;
                break;
                /* EasyCODE ) */
            case TOUT4 :
                /* EasyCODE ( 0 
                Ausgang 4 */
                if ( pzbl.pout4 )
                {
                    abschnitt [0].art = AB_OUTRES;
                }
                else
                {
                    abschnitt [0].art = AB_OUTSET;
                }
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 4;
                break;
                /* EasyCODE ) */
            case THFHUB :
                /* EasyCODE ( 0 
                Huepferfuss-Hub (Wert) */
                pargetval.altwert = pzbl.hub;
                pargetval.minwert = HFLEVEL_HUB_MIN;
                pargetval.maxwert = HFLEVEL_HUB_MAX;
                pargetval.auftrag = 0;
                pargetval.pikto = hfhubps;
                pargetval.kommastellen = 1;
                pargetval.string = floatstr;
                switch ( getvalue_eing() )
                {
                case 0 // ENTER
                :
                    abschnitt [0].par2 = pargetval.wert;
                    abschnitt [0].art = AB_FUSSHUB;
                    abschnitt [0].par1 = vpartemp;
                    hfuss.akt_hub = pargetval.wert;
                    break;
                case 1 // ESC
                :
                    einfkey = 0;
                    break;
                default:
                    // Stoppanf
                    error_res();
                    einfkey = 0;
                    break;
                }
                break;
                /* EasyCODE ) */
            case THFLEVEL :
                /* EasyCODE ( 0 
                Niveau Huepferfuss (Wert) */
                pargetval.altwert = pzbl.posunten;
                pargetval.minwert = HFLEVEL_HUB_MIN;
                pargetval.maxwert = HFLEVEL_MAX;
                pargetval.auftrag = 0;
                pargetval.pikto = hflevelps;
                pargetval.kommastellen = 1;
                pargetval.string = floatstr;
                switch ( getvalue_eing() )
                {
                case 0 // ENTER
                :
                    abschnitt [0].par2 = pargetval.wert;
                    abschnitt [0].art = AB_FUSSNIVEAU;
                    abschnitt [0].par1 = vpartemp;
                    hfuss.akt_posunten.pos = pargetval.wert;
                    break;
                case 1 // ESC
                :
                    einfkey = 0;
                    break;
                default:
                    // Stoppanf
                    error_res();
                    einfkey = 0;
                    break;
                }
                break;
                /* EasyCODE ) */
            case THFHUBSTEP :
                /* EasyCODE ( 0 
                Huepferfuss-Hub (Stufe) */
                pargetval.altwert = HF_STUFE_MIN;
                pargetval.minwert = HF_STUFE_MIN;
                pargetval.maxwert = HF_STUFE_MAX;
                pargetval.auftrag = 0;
                pargetval.pikto = hfhubps;
                pargetval.kommastellen = 0;
                pargetval.string = intstr;
                switch ( getvalue_eing() )
                {
                case 0 // ENTER
                :
                    abschnitt [0].par2 = pargetval.wert;
                    abschnitt [0].art = AB_FUSSHUBSTUFE;
                    abschnitt [0].par1 = vpartemp;
                    break;
                case 1 // ESC
                :
                    einfkey = 0;
                    break;
                default:
                    // Stoppanf
                    error_res();
                    einfkey = 0;
                    break;
                }
                break;
                /* EasyCODE ) */
            case THFLEVELSTEP :
                /* EasyCODE ( 0 
                Niveau Huepferfuss (Stufe) */
                pargetval.altwert = HF_STUFE_MIN;
                pargetval.minwert = HF_STUFE_MIN;
                pargetval.maxwert = HF_STUFE_MAX;
                pargetval.auftrag = 0;
                pargetval.pikto = hflevelps;
                pargetval.kommastellen = 0;
                pargetval.string = intstr;
                switch ( getvalue_eing() )
                {
                case 0 // ENTER
                :
                    abschnitt [0].par2 = pargetval.wert;
                    abschnitt [0].art = AB_FUSSNIVEAUSTUFE;
                    abschnitt [0].par1 = vpartemp;
                    break;
                case 1 // ESC
                :
                    einfkey = 0;
                    break;
                default:
                    // Stoppanf
                    error_res();
                    einfkey = 0;
                    break;
                }
                break;
                /* EasyCODE ) */
            case TOBERTEIL :
                /* EasyCODE ( 0 
                Oberteil heben beim Eilgang (Ausgang 6) */
                if ( pzbl.pout6 )
                {
                    abschnitt [0].art = AB_OUTRES;
                }
                else
                {
                    abschnitt [0].art = AB_OUTSET;
                }
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 6;
                break;
                /* EasyCODE ) */
            case TZUSPG :
                /* EasyCODE ( 0 
                Zusatzfadenspannung */
                if ( pzbl.zusp )
                {
                    abschnitt [0].art = AB_ZUSPGAUS;
                }
                else
                {
                    abschnitt [0].art = AB_ZUSPGEIN;
                }
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 1;
                break;
                /* EasyCODE ) */
            case TIN1HIGH :
                /* EasyCODE ( 0 
                Warten auf Eingang 1 High */
                abschnitt [0].art = AB_WHIGH;
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 1;
                break;
                /* EasyCODE ) */
            case TIN1LOW :
                /* EasyCODE ( 0 
                Warten auf Eingang 1 Low */
                abschnitt [0].art = AB_WLOW;
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 1;
                break;
                /* EasyCODE ) */
            case TIN2HIGH :
                /* EasyCODE ( 0 
                Warten auf Eingang 2 High */
                abschnitt [0].art = AB_WHIGH;
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 2;
                break;
                /* EasyCODE ) */
            case TIN2LOW :
                /* EasyCODE ( 0 
                Warten auf Eingang 2 Low */
                abschnitt [0].art = AB_WLOW;
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 2;
                break;
                /* EasyCODE ) */
            case TSTOP :
                /* EasyCODE ( 0 
                Stop */
                abschnitt [0].art = AB_PSTOP;
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 0;
                break;
                /* EasyCODE ) */
            case TUFWSTOP :
                /* EasyCODE ( 0 
                Stop bei Unterfadenstoerung */
                abschnitt [0].art = AB_UFWSTOP;
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 0;
                break;
                /* EasyCODE ) */
            case TREDGSW :
                /* EasyCODE ( 0 
                Reduzierte Drehzahl */
                if ( pzbl.redgsw )
                {
                    abschnitt [0].art = AB_REDGSWAUS;
                }
                else
                {
                    abschnitt [0].art = AB_REDGSWEIN;
                }
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 0;
                break;
                /* EasyCODE ) */
            case TZEIT :
                /* EasyCODE ( 0 
                Warten auf Zeit */
                abschnitt [0].art = AB_WZEIT;
                abschnitt [0].par1 = vpartemp;
                abschnitt [0].par2 = 100;
                pargetval.altwert = 100;
                pargetval.minwert = WZEIT_MIN;
                pargetval.maxwert = WZEIT_MAX;
                pargetval.auftrag = 0;
                pargetval.pikto = zeitps;
                pargetval.kommastellen = 2;
                pargetval.string = zeitstr2;
                switch ( getvalue_eing() )
                {
                case 0 :
                    // ENTER
                    abschnitt [0].par2 = pargetval.wert;
                    switch ( eing.pup.absch_einf (ppointer,1) )
                    {
                    case 0 :
                        // ok
                        abschnr += 1;
                        ppointer += 1;
                        einfkey = 0;
                        break;
                    case 1 :
                        // Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                        koor.eing_err = 29;
                        error_res();
                        key = 2;
                        loopflag = FALSE;
                        break;
                    }
                    break;
                case 1 :
                    // ESC
                    einfkey = 0;
                    break;
                default:
                    // Stoppanf
                    error_res();
                    einfkey = 0;
                    break;
                }
                break;
                /* EasyCODE ) */
            case TDREHZAHL :
                /* EasyCODE ( 0 
                Drehzahl */
                pargetval.minwert = GSWMIN;
                pargetval.maxwert = GSWMAX;
                pargetval.auftrag = 0;
                pargetval.altwert = saf->md.maxgsw;
                pargetval.pikto = gswps;
                pargetval.kommastellen = 0;
                pargetval.string = gswstr;
                switch ( getvalue_eing() )
                {
                case 0
                                                                                                                                                                                                         // ENTER
                :
                    abschnitt [0].art = AB_DREHZ;
                    abschnitt [0].par1 = 0;
                    abschnitt [0].par2 = pargetval.wert * 100;
                    switch ( eing.pup.absch_einf (ppointer,1) )
                    {
                    case 0 :
                        // ok
                        abschnr += 1;
                        ppointer += 1;
                        einfkey = 0;
                        break;
                    case 1 :
                        // Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                        koor.eing_err = 29;
                        error_res();
                        key = 2;
                        loopflag = FALSE;
                        break;
                    }
                    break;
                case 1 :
                    // ESC
                    einfkey = 0;
                    break;
                default:
                    // Stoppanf
                    error_res();
                    einfkey = 0;
                    break;
                }
                break;
                /* EasyCODE ) */
            case TVPAR :
                /* EasyCODE ( 0 
                Verschiebe-Parameter */
                pargetval.minwert = VPAR_MIN;
                pargetval.maxwert = VPAR_MAX;
                pargetval.auftrag = 0;
                pargetval.altwert = vpartemp;
                pargetval.pikto = vparps;
                pargetval.kommastellen = 0;
                pargetval.string = intstr;
                switch ( getvalue_eing() )
                {
                case 0 // ENTER
                :
                    vpartemp = pargetval.wert;
                    einfkey = 18;
                    break;
                case 1 // ESC
                :
                    einfkey = 0;
                    break;
                default:
                    // Stoppanf
                    error_res();
                    einfkey = 0;
                    break;
                }
                break;
                /* EasyCODE ) */
            case TESC :
                einfkey = 0;
                break;
            case STOPANF :
                /* EasyCODE ( 0 
                Stopanforderung */
                error_res ();
                einfkey = 18;
                break;
                /* EasyCODE ) */
            default:
                einfkey = 18;
                break;
            }
            if ( einfkey == 19 )
            {
                switch ( eing.pup.absch_einf (ppointer,1) )
                {
                case 0 :
                    // ok
                    abschnr += 1;
                    ppointer += 1;
                    eing.pup.set_pzbl_abschnitt (ppointer);
                    eing.pup.pout_exe();
                    einfkey = 0;
                    break;
                case 1 :
                    // Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                    koor.eing_err = 29;
                    error_res();
                    key = 2;
                    loopflag = FALSE;
                    break;
                }
            }
            break;
            /* EasyCODE ) */
        case 20 :
            /* EasyCODE ( 0 
            einfkey = 20,   Nadel drehen ein/aus einfuegen */
            if ( pzbl.flag1 )
            {
                abschnitt [0].art = AB_FLAGRES;
            }
            else
            {
                abschnitt [0].art = AB_FLAGSET;
            }
            abschnitt [0].par1 = vpartemp;
            abschnitt [0].par2 = 1;
            switch ( eing.pup.absch_einf (ppointer,1) )
            {
            case 0 :
                // ok
                abschnr += 1;
                ppointer += 1;
                eing.pup.set_pzbl_abschnitt (ppointer);
                einfkey = 0;
                break;
            case 1 :
                // Prog > MAXPROGLEN, Abschnitt nicht eingefuegt
                koor.eing_err = 29;
                error_res();
                key = 2;
                loopflag = FALSE;
                break;
            }
            break;
            /* EasyCODE ) */
        default:
            break;
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProg::del */
/* EasyCODE F */
unsigned char KlasseProg::del (void)
{
    unsigned char j;
    unsigned char retwert = 0xff;
    switch ( ppointer->art )
    {
    case AB_PNR :
        /* EasyCODE = */
    case AB_KLCODE :
        /* EasyCODE = */
    case AB_HIND :
        /* EasyCODE = */
    case AB_MNP :
        /* EasyCODE = */
    case AB_AP :
        /* EasyCODE = */
    case AB_PRENDE
         // Aendern nicht erlaubt
    :
        eing_botsch1.senden (BELLBELL);
        /* EasyCODE - */
        key = 2;
        break;
    case AB_EIL :
        /* EasyCODE = */
    case AB_STICH :
        /* EasyCODE ( 0 
        Eilgang oder Einzelstich */
        if ( !eing.pup.nafupos () )
        {
            error_res ();
            key = 2;
        }
        else
        {
            konv.suchekoord(FP_OFF(eing.prog.ppointer),(ikoordtyp *)&nup.koorsoll[0]);
            if ( nup.poscheck (&nup.koorsoll [0]) )
            {
                eing.pup.koorflag = FALSE;
            }
            else
            {
                /* EasyCODE ( 0 
                Koordinaten begrenzen */
                if ( nup.koorsoll [0].xbild < saf->xnaehfeld_min )
                {
                    nup.koorsoll [0].xbild = saf->xnaehfeld_min;
                }
                if ( nup.koorsoll [0].xbild > saf->xnaehfeld_max )
                {
                    nup.koorsoll [0].xbild = saf->xnaehfeld_max;
                }
                if ( nup.koorsoll [0].ybild < saf->ynaehfeld_min )
                {
                    nup.koorsoll [0].ybild = saf->ynaehfeld_min;
                }
                if ( nup.koorsoll [0].ybild > saf->ynaehfeld_max )
                {
                    nup.koorsoll [0].ybild = saf->ynaehfeld_max;
                }
                eing.pup.koorflag = TRUE;
                /* EasyCODE ) */
            }
            if ( !ver.pmove () )
            {
                // verriegelt
                error_res ();
                key = 2;
            }
            else
            {
                nup.bitomo (&nup.koorsoll[0]);
                nup.carmov (STANDARD);
                while ( !smx.ready() || !smy.ready() )
                {
                    if ( !ver.pmove () )
                    {
                        // verriegelt
                        smx.stop ();
                        smy.stop ();
                        nae.homereq = TRUE;
                        saf->carposok = FALSE;
                        error_res ();
                        retwert = E_RUHE;
                        eing.ioact = FALSE;
                        break;
                    }
                }
                if ( retwert == 0xff )
                {
                    nup.workbuf.koorist = nup.koorsoll [0];
                    koorptr = nup.koorsoll [0];
                    if ( eing.pup.koorflag )
                    {
                        // Koordinaten wurden abgeschnitten
                        koor.eing_err = 27;
                        error_res ();
                    }
                    konv.verschiebe_abschnitt (FP_OFF(ppointer), *(ikoordtyp *)(&nup.workbuf.koorist));
                    eing.pup.absch_del (ppointer, 1);
                    ppointer --;
                    abschnr --;
                    key = 2;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case AB_GERADE :
        /* EasyCODE = */
    case AB_KURVSTP :
        /* EasyCODE = */
    case AB_KURVENDP :
        /* EasyCODE ( 0 
        Gerade, Kurvenstuetzpunkt oder Kurvenendpunkt */
        if ( !eing.pup.nafupos () )
        {
            error_res ();
            key = 2;
        }
        else
        {
            masch_adr = FP_OFF(eing.pup.find_absch(AB_PRENDE,ppointer)+1);
            switch ( (j=konv.absch_gen(ndatanf,300,MAXPROGLEN,FP_OFF(ppointer),&masch_adr)) )
            {
            case 0 :
                eing.pup.koorflag = FALSE;
                break;
            case 14 :
                eing.pup.koorflag = TRUE;
                break;
            default:
                koor.eing_err = 30;
                eing.errpar = j;
                eing.errpar2 = abschnr;
                error_res ();
                key = 2;
                break;
            }
            if ( key == 4 )
            {
                if ( !eing.pup.movstich_back () )
                {
                    error_res ();
                    if ( saf->carposok )
                    {
                        key = 2;
                    }
                    else
                    {
                        retwert = E_GST;
                        eing.ioact = FALSE;
                    }
                }
                else
                {
                    if ( eing.pup.koorflag )
                    {
                        // Koordinaten wurden abgeschnitten
                        koor.eing_err = 27;
                        error_res ();
                    }
                    konv.verschiebe_abschnitt (FP_OFF(ppointer), *(ikoordtyp *)(&nup.workbuf.koorist));
                    eing.pup.absch_del (ppointer, 1);
                    ppointer --;
                    abschnr --;
                    eing.pup.set_pzbl (ppointer);
                    eing.pup.curvend ();
                    key = 2;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case AB_BOGENSTP :
        /* EasyCODE = */
    case AB_BOGENENDP :
        /* EasyCODE = */
    case AB_KREISSTP :
        /* EasyCODE = */
    case AB_KREISENDP :
        /* EasyCODE ( 0 
        Bogenstuetzpunkt, Bogenendpunkt,
        Kreisstuetzpunkt, Kreisendpunkt */
        if ( !eing.pup.nafupos () )
        {
            error_res ();
            key = 2;
        }
        else
        {
            eing.pup.koorflag = FALSE;
            do{
                masch_adr = FP_OFF(eing.pup.find_absch(AB_PRENDE,ppointer)+1);
                switch ( (j=konv.absch_gen(ndatanf,300,MAXPROGLEN,FP_OFF(ppointer),&masch_adr)) )
                {
                case 0 :
                    break;
                case 14 :
                    eing.pup.koorflag = TRUE;
                    break;
                default:
                    koor.eing_err = 30;
                    eing.errpar = j;
                    eing.errpar2 = abschnr;
                    error_res ();
                    key = 2;
                    break;
                }
                if ( key == 4 )
                {
                    if ( !eing.pup.movstich_back () )
                    {
                        error_res ();
                        if ( saf->carposok )
                        {
                            key = 2;
                        }
                        else
                        {
                            retwert = E_GST;
                            eing.ioact = FALSE;
                        }
                        break;
                    }
                    else
                    {
                        ppointer --;
                        abschnr --;
                        eing.pup.pstatuszeile ();
                        eing.pup.abschparanz ();
                        eing.pup.kooranz ();
                    }
                }
                else
                {
                    break;
                }
            }while ( (ppointer->art == AB_BOGENSTP) || (ppointer->art == AB_KREISSTP) );
            if ( (key == 4) && (retwert == 0xff) )
            {
                if ( eing.pup.koorflag )
                {
                    // Koordinaten wurden abgeschnitten
                    koor.eing_err = 27;
                    error_res ();
                }
                konv.verschiebe_abschnitt (FP_OFF(ppointer), *(ikoordtyp *)(&nup.workbuf.koorist));
                if ( (ppointer+1)->art == AB_BOGENSTP )
                {
                    konv.verschiebe_abschnitt (FP_OFF(ppointer+2), *(ikoordtyp *)&nup.workbuf.koorist);
                    eing.pup.absch_del (ppointer+1, 2);
                }
                else
                {
                    eing.pup.absch_del (ppointer+1, 3);
                }
                key = 2;
            }
        }
        break;
        /* EasyCODE ) */
    default:
        eing.pup.absch_del (ppointer, 1);
        /* EasyCODE - */
        ppointer --;
        /* EasyCODE - */
        abschnr --;
        /* EasyCODE - */
        eing.pup.set_pzbl (ppointer);
        /* EasyCODE - */
        eing.pup.pout_exe ();
        /* EasyCODE - */
        key = 2;
        break;
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProg::aender */
/* EasyCODE F */
unsigned char KlasseProg::aender(void)
{
    /* EasyCODE < */
    unsigned char retwert = 0xff;
    unsigned char loopflag;
    unsigned char *stringptr;
    struct p_abschstruct *hlpptr;
    /* EasyCODE > */
    switch ( ppointer->art )
    {
    case AB_PNR :
        /* EasyCODE ( 0 
        Programmnummer aendern */
        pargetval.auftrag = 1;
        pargetval.altwert = ppointer->par1;
        if ( *(ndatanf+KPROGNR)==4 )
        {
            pargetval.string = prog_pnr2s;
        }
        else
        {
            pargetval.string = prog_pnr1s;
        }
        pargetval.sonderstring = prog_pnrp2s;
        pargetval.pikto = pnrps;
        pargetval.maxwert = PNRMAX;
        pargetval.minwert = PNRMIN;
        pargetval.kommastellen = 0;
        switch ( getvalue_eing() )
        {
        case 0 // ENTER
        :
            /* EasyCODE ( 0 
            Taste Enter */
            if ( (di.exprog (DRIVE_C, pargetval.wert) != 0) && (pargetval.wert != ppointer->par1) )
            {
                eing_botsch1.senden(BV030);
                sprintf(eing.varstring, tz->proghin1, pargetval.wert);
                eing_botsch1.senden(eing.varstring);
                loopflag = TRUE;
                while ( loopflag )
                {
                    switch ( getkey () )
                    {
                    case TESC :
                        key = 2;
                        loopflag = FALSE;
                        break;
                    case TENTER :
                        ppointer->par1 = pargetval.wert;
                        key = 2;
                        loopflag = FALSE;
                        break;
                    case TEINGABE :
                        /* EasyCODE ( 0 
                        Taste Eingabe */
                        if ( taste_los() == 0xff )
                        {
                            key = 10;
                        }
                        else
                        {
                            if ( eing.pup.prog_exit () )
                            {
                                retwert = E_GST;
                            }
                            else
                            {
                                koor.eing_err = 30;
                                eing.errpar = konverr;
                                eing.errpar2 = absch_fehler;
                                error_res ();
                                key = 2;
                            }
                        }
                        loopflag = FALSE;
                        break;
                        /* EasyCODE ) */
                    case TENDE :
                        /* EasyCODE ( 0 
                        Taste Ende */
                        if ( eing.pup.prog_exit () )
                        {
                            retwert = E_EXIT;
                        }
                        else
                        {
                            koor.eing_err = 30;
                            eing.errpar = konverr;
                            eing.errpar2 = absch_fehler;
                            error_res ();
                            key = 2;
                        }
                        loopflag = FALSE;
                        break;
                        /* EasyCODE ) */
                    default:
                        break;
                    }
                }
            }
            else
            {
                ppointer->par1 = pargetval.wert;
                key = 2;
            }
            break;
            /* EasyCODE ) */
        case 1 // ESC
        :
            key = 2;
            break;
        case 3 // Sondertaste
        :
            switch ( pargetval.taste )
            {
            case T1NADEL :
                *(ndatanf+KPROGNR)=3;
                break;
            case T2NADEL :
                *(ndatanf+KPROGNR)=4;
                break;
            default:
                break;
            }
            break;
        default:
            // Stopanf
            key = 2;
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_KLCODE :
        /* EasyCODE ( 0 
        Klammercode aendern */
        pargetval.auftrag = 0;
        pargetval.altwert = ppointer->par1;
        pargetval.string = intstr;
        pargetval.pikto = klcodeps;
        pargetval.kommastellen = 0;
        pargetval.minwert = -1;
        pargetval.maxwert = SCHABLCODE_MAX;
        switch ( getvalue_eing() )
        {
        case 0  // ENTER
        :
            ppointer->par1 = pargetval.wert;
            key = 2;
            break;
        case 1  // ESC
        :
            key = 2;
            break;
        default:
            // Stopanforderung
            key = 2;
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_ANNAEH :
    case AB_ANFRIEGEL :
    case AB_SCHNEID :
    case AB_ENDRIEGEL :
        /* EasyCODE ( 0 
        Riegel aendern */
        if ( *((unsigned char *)ppointer+2)==0 )
        {
            *((unsigned char *)ppointer+2)=1;
        }
        if ( *((unsigned char *)ppointer+4)==0 )
        {
            *((unsigned char *)ppointer+4)=1;
        }
        eing_botsch1.senden (BV036);
        if ( (ppointer->art == AB_ANNAEH) ||
             (ppointer->art == AB_ANFRIEGEL) )
        {
            sprintf (eing.varstring, riegelpiktos, PANFANGSRIEGEL);
        }
        else
        {
            sprintf (eing.varstring, riegelpiktos, PENDRIEGEL);
        }
        sprintf (eing.varstring1, riegelwertes, *((unsigned char *)ppointer+1), *((unsigned char *)ppointer+2),
                                                *((unsigned char *)ppointer+3), *((unsigned char *)ppointer+4));
        strcat(eing.varstring, eing.varstring1);
        eing_botsch1.senden (eing.varstring);
        while ( loopflag )
        {
            switch ( getkey() )
            {
            case TFWDSTINKR :
                /* EasyCODE ( 0 
                Vorwaertsstiche inkrementieren */
                if ( *((unsigned char *)ppointer+1) <
                     *((unsigned char *)ppointer+3) * *((unsigned char *)ppointer+2) / *((unsigned char *)ppointer+4) )
                {
                    *((unsigned char *)ppointer+1) += 1;
                }
                else
                {
                    eing_botsch1.senden (BELLBELL);
                }
                break;
                /* EasyCODE ) */
            case TFWDSTDEKR :
                /* EasyCODE ( 0 
                Vorwaertsstiche dekrementieren */
                if ( *((unsigned char *)ppointer+1) > RSTVORW_MIN )
                {
                    *((unsigned char *)ppointer+1) -= 1;
                }
                else
                {
                    eing_botsch1.senden (BELLBELL);
                }
                break;
                /* EasyCODE ) */
            case TFWDTEILSTINKR :
                /* EasyCODE ( 0 
                Vorwaerts-Teilstiche inkrementieren */
                if ( *((unsigned char *)ppointer+4) == 3 )
                {
                    if ( *((unsigned char *)ppointer+2) < 3 )
                    {
                        *((unsigned char *)ppointer+2) += 1;
                    }
                    else
                    {
                        eing_botsch1.senden (BELLBELL);
                    }
                }
                else
                {
                    if ( *((unsigned char *)ppointer+2) < RTEILSTICHE_MAX )
                    {
                        *((unsigned char *)ppointer+2) += 1;
                    }
                    else
                    {
                        eing_botsch1.senden (BELLBELL);
                    }
                }
                if ( (*((unsigned char *)ppointer+4) == 2) && (*((unsigned char *)ppointer+2) == 3) )
                {
                    *((unsigned char *)ppointer+2) = 4;
                }
                break;
                /* EasyCODE ) */
            case TFWDTEILSTDEKR :
                /* EasyCODE ( 0 
                Vorwaerts-Teilstiche dekrementieren */
                if ( *((unsigned char *)ppointer+4) == 3 )
                {
                    if ( *((unsigned char *)ppointer+2) > 3 )
                    {
                        *((unsigned char *)ppointer+2) -= 1;
                    }
                    else
                    {
                        eing_botsch1.senden (BELLBELL);
                    }
                }
                else
                {
                    if ( *((unsigned char *)ppointer+2) > *((unsigned char *)ppointer+4) )
                    {
                        *((unsigned char *)ppointer+2) -= 1;
                    }
                    else
                    {
                        eing_botsch1.senden (BELLBELL);
                    }
                }
                if ( (*((unsigned char *)ppointer+4) == 2) && (*((unsigned char *)ppointer+2) == 3) )
                {
                    *((unsigned char *)ppointer+2) = 2;
                }
                if ( *((unsigned char *)ppointer+1) > *((unsigned char *)ppointer+3) * *((unsigned char *)ppointer+2) / *((unsigned char *)ppointer+4) )
                {
                    *((unsigned char *)ppointer+1) = *((unsigned char *)ppointer+3) * *((unsigned char *)ppointer+2) / *((unsigned char *)ppointer+4);
                }
                break;
                /* EasyCODE ) */
            case TBACKSTINKR :
                /* EasyCODE ( 0 
                Rueckwaertsstiche inkrementieren */
                if ( *((unsigned char *)ppointer+3) < RSTRUECKW_MAX )
                {
                    *((unsigned char *)ppointer+3) += 1;
                }
                else
                {
                    eing_botsch1.senden (BELLBELL);
                }
                break;
                /* EasyCODE ) */
            case TBACKSTDEKR :
                /* EasyCODE ( 0 
                Rueckwaertsstiche dekrementieren */
                if ( *((unsigned char *)ppointer+3) > RSTRUECKW_MIN )
                {
                    *((unsigned char *)ppointer+3) -= 1;
                }
                else
                {
                    eing_botsch1.senden (BELLBELL);
                }
                if ( *((unsigned char *)ppointer+1) > *((unsigned char *)ppointer+3) * *((unsigned char *)ppointer+2) / *((unsigned char *)ppointer+4) )
                {
                    *((unsigned char *)ppointer+1) = *((unsigned char *)ppointer+3) * *((unsigned char *)ppointer+2) / *((unsigned char *)ppointer+4);
                }
                break;
                /* EasyCODE ) */
            case TBACKTEILSTINKR :
                /* EasyCODE ( 0 
                Rueckwaerts-Teilstiche inkrementieren */
                if ( *((unsigned char *)ppointer+4) < RTEILSTICHE_MAX )
                {
                    *((unsigned char *)ppointer+4) += 1;
                }
                else
                {
                    eing_botsch1.senden (BELLBELL);
                }
                switch ( *((unsigned char *)ppointer+4) )
                {
                case 2 :
                    if ( (*((unsigned char *)ppointer+2) ==1) || (*((unsigned char *)ppointer+2) ==3) )
                    {
                        *((unsigned char *)ppointer+2) = 2;
                    }
                    break;
                case 3 :
                    /* EasyCODE = */
                case 4 :
                    *((unsigned char *)ppointer+2) = *((unsigned char *)ppointer+4);
                    break;
                default:
                    break;
                }
                if ( *((unsigned char *)ppointer+1) > *((unsigned char *)ppointer+3) * *((unsigned char *)ppointer+2) / *((unsigned char *)ppointer+4) )
                {
                    *((unsigned char *)ppointer+1) = *((unsigned char *)ppointer+3) * *((unsigned char *)ppointer+2) / *((unsigned char *)ppointer+4);
                }
                break;
                /* EasyCODE ) */
            case TBACKTEILSTDEKR :
                /* EasyCODE ( 0 
                Rueckwaerts-Teilstiche dekrementieren */
                if ( *((unsigned char *)ppointer+4) > RTEILSTICHE_MIN )
                {
                    *((unsigned char *)ppointer+4) -= 1;
                }
                else
                {
                    eing_botsch1.senden (BELLBELL);
                }
                switch ( *((unsigned char *)ppointer+4) )
                {
                case 2 :
                    if ( (*((unsigned char *)ppointer+2) ==1) || (*((unsigned char *)ppointer+2) ==3) )
                    {
                        *((unsigned char *)ppointer+2) = 2;
                    }
                    break;
                case 3 :
                    /* EasyCODE = */
                case 4 :
                    *((unsigned char *)ppointer+2) = *((unsigned char *)ppointer+4);
                    break;
                default:
                    break;
                }
                if ( *((unsigned char *)ppointer+1) > *((unsigned char *)ppointer+3) * *((unsigned char *)ppointer+2) / *((unsigned char *)ppointer+4) )
                {
                    *((unsigned char *)ppointer+1) = *((unsigned char *)ppointer+3) * *((unsigned char *)ppointer+2) / *((unsigned char *)ppointer+4);
                }
                break;
                /* EasyCODE ) */
            case TENTER :
                /* EasyCODE ( 0 
                Taste Enter */
                loopflag = FALSE;
                key = 2;
                break;
                /* EasyCODE ) */
            case TPROG :
                /* EasyCODE ( 0 
                Taste Programmieren */
                loopflag = FALSE;
                key = 2;
                break;
                /* EasyCODE ) */
            case TEINGABE :
                /* EasyCODE ( 0 
                Taste Eingabe */
                if ( taste_los() == 0xff )
                {
                    key = 10;
                }
                else
                {
                    if ( eing.pup.prog_exit () )
                    {
                        retwert = E_GST;
                    }
                    else
                    {
                        koor.eing_err = 30;
                        eing.errpar = konverr;
                        eing.errpar2 = absch_fehler;
                        error_res ();
                        key = 2;
                    }
                }
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TENDE :
                /* EasyCODE ( 0 
                Taste Ende */
                if ( eing.pup.prog_exit () )
                {
                    retwert = E_EXIT;
                }
                else
                {
                    koor.eing_err = 30;
                    eing.errpar = konverr;
                    eing.errpar2 = absch_fehler;
                    error_res ();
                    key = 2;
                }
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            default:
                continue;
            }
            sprintf (eing.varstring1, riegelwertes, *((unsigned char *)ppointer+1), *((unsigned char *)ppointer+2),
                                                    *((unsigned char *)ppointer+3), *((unsigned char *)ppointer+4));
            eing_botsch1.senden (eing.varstring1);
        }
        break;
        /* EasyCODE ) */
    case AB_WZEIT :
        /* EasyCODE ( 0 
        Warten auf Zeit aendern */
        pargetval.altwert = ppointer->par2;
        pargetval.minwert = WZEIT_MIN;
        pargetval.maxwert = WZEIT_MAX;
        pargetval.auftrag = 0;
        pargetval.pikto = zeitps;
        pargetval.kommastellen = 2;
        pargetval.string = zeitstr2;
        switch ( getvalue_eing() )
        {
        case 0 :
            // ENTER
            ppointer->par2 = pargetval.wert;
            /* EasyCODE ( 0 
            Verschiebe-Parameter aendern */
            pargetval.minwert = VPAR_MIN;
            pargetval.maxwert = VPAR_MAX;
            pargetval.auftrag = 0;
            pargetval.altwert = ppointer->par1;
            pargetval.pikto = vparps;
            pargetval.kommastellen = 0;
            pargetval.string = intstr;
            switch ( getvalue_eing() )
            {
            case 0 :
                // ENTER
                ppointer->par1 = pargetval.wert;
                key = 2;
                break;
            case 1 :
                // ESC
                key = 2;
                break;
            default:
                // Stoppanf
                error_res();
                key = 2;
                break;
            }
            /* EasyCODE ) */
            break;
        case 1 :
            // ESC
            key = 2;
            break;
        default:
            // Stoppanf
            error_res();
            key = 2;
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_DREHZ :
        /* EasyCODE ( 0 
        Drehzahl aendern */
        pargetval.minwert = GSWMIN;
        pargetval.maxwert = GSWMAX;
        pargetval.auftrag = 0;
        pargetval.altwert = ppointer->par2/100;
        pargetval.pikto = gswps;
        pargetval.kommastellen = 0;
        pargetval.string = gswstr;
        switch ( getvalue_eing() )
        {
        case 0 :
            // ENTER
            ppointer->par2 = pargetval.wert * 100;
            /* EasyCODE ( 0 
            Verschiebe-Parameter aendern */
            pargetval.minwert = VPAR_MIN;
            pargetval.maxwert = VPAR_MAX;
            pargetval.auftrag = 0;
            pargetval.altwert = ppointer->par1;
            pargetval.pikto = vparps;
            pargetval.kommastellen = 0;
            pargetval.string = intstr;
            switch ( getvalue_eing() )
            {
            case 0 :
                // ENTER
                ppointer->par1 = pargetval.wert;
                key = 2;
                break;
            case 1 :
                // ESC
                key = 2;
                break;
            default:
                // Stoppanf
                error_res();
                key = 2;
                break;
            }
            /* EasyCODE ) */
            break;
        case 1 :
            // ESC
            key = 2;
            break;
        default:
            // Stoppanf
            error_res();
            key = 2;
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_STDST :
        /* EasyCODE ( 0 
        Standardstichlaenge aendern */
        pargetval.minwert = STLG_MIN*10;
        pargetval.maxwert = STLG_MAX*10;
        pargetval.auftrag = 0;
        pargetval.altwert = ppointer->par1;
        pargetval.pikto = ststips;
        pargetval.kommastellen = 2;
        pargetval.string = floatstr2;
        switch ( getvalue_eing() )
        {
        case 0 :
            /* EasyCODE < */
            // ENTER
            ppointer->par1 = pargetval.wert;
            hlpptr = (struct p_abschstruct *) (ndatanf + GPROGNR);
            /* EasyCODE > */
            while ( (hlpptr = eing.pup.find_absch (AB_STDST, hlpptr)) != NULL )
            {
                hlpptr->par1 = ppointer->par1;
            }
            eing.pup.set_pzbl_abschnitt (ppointer);
            key = 2;
            break;
        case 1 :
            // ESC
            key = 2;
            break;
        default:
            // Stoppanf
            error_res();
            key = 2;
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_STICHL :
        /* EasyCODE ( 0 
        Stichlaenge aendern */
        pargetval.altwert = ppointer->par1;
        pargetval.minwert = STLG_MIN*10;
        pargetval.maxwert = STLG_MAX*10;
        pargetval.auftrag = 0;
        pargetval.pikto = stips;
        pargetval.kommastellen = 2;
        pargetval.string = floatstr2;
        switch ( getvalue_eing() )
        {
        case 0 :
            // ENTER
            ppointer->par1 = pargetval.wert;
            key = 2;
            break;
        case 1 :
            // ESC
            key = 2;
            break;
        default:
            // Stoppanf
            error_res();
            key = 2;
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_STICHB :
        /* EasyCODE ( 0 
        Stichbreite aendern */
        pargetval.minwert = 0;
        pargetval.maxwert = STLG_MAX;
        pargetval.altwert = ppointer->par1;
        pargetval.auftrag = 0;
        pargetval.pikto = stibreiteps;
        pargetval.kommastellen = 1;
        pargetval.string = floatstr;
        switch ( getvalue_eing() )
        {
        case 0 :
            // ENTER
            ppointer->par1 = pargetval.wert;
            if ( pargetval.wert == 0 )
            {
                ppointer->par2 = 0;
            }
            else
            {
                loopflag = TRUE;
                while ( loopflag )
                {
                    eing_botsch1.senden (BV035);
                    switch ( getkey () )
                    {
                    case TZZ1LINKS :
                        ppointer->par2 = 0;
                        loopflag = FALSE;
                        break;
                    case TZZ1RECHTS :
                        ppointer->par2 = 1;
                        loopflag = FALSE;
                        break;
                    case TZZLINKS :
                        ppointer->par2 = 2;
                        loopflag = FALSE;
                        break;
                    case TZZRECHTS :
                        ppointer->par2 = 3;
                        loopflag = FALSE;
                        break;
                    case TESC :
                        key = 2;
                        loopflag = FALSE;
                        break;
                    case STOPANF :
                        /* EasyCODE ( 0 
                        Stopanforderung */
                        error_res ();
                        break;
                        /* EasyCODE ) */
                    default:
                        break;
                    }
                }
            }
            key = 2;
            break;
        case 1 :
            // ESC
            key = 2;
            break;
        default:
            // Stoppanf
            error_res();
            key = 2;
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_FUSSHUB :
        /* EasyCODE ( 0 
        Huepferfuss Hub (Wert) */
        pargetval.altwert = ppointer->par2;
        pargetval.minwert = HFLEVEL_HUB_MIN;
        pargetval.maxwert = HFLEVEL_HUB_MAX;
        pargetval.auftrag = 0;
        pargetval.pikto = hfhubps;
        pargetval.kommastellen = 1;
        pargetval.string = floatstr;
        switch ( getvalue_eing() )
        {
        case 0 :
            // ENTER
            ppointer->par2 = pargetval.wert;
            hfuss.akt_hub = pargetval.wert;
            pzbl.hub = pargetval.wert;
            /* EasyCODE ( 0 
            Verschiebe-Parameter aendern */
            pargetval.minwert = VPAR_MIN;
            pargetval.maxwert = VPAR_MAX;
            pargetval.auftrag = 0;
            pargetval.altwert = ppointer->par1;
            pargetval.pikto = vparps;
            pargetval.kommastellen = 0;
            pargetval.string = intstr;
            switch ( getvalue_eing() )
            {
            case 0 :
                // ENTER
                ppointer->par1 = pargetval.wert;
                key = 2;
                break;
            case 1 :
                // ESC
                key = 2;
                break;
            default:
                // Stoppanf
                error_res();
                key = 2;
                break;
            }
            /* EasyCODE ) */
            break;
        case 1 :
            // ESC
            key = 2;
            break;
        default:
            // Stoppanf
            error_res();
            key = 2;
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_FUSSNIVEAU :
        /* EasyCODE ( 0 
        Huepferfuss Niveau (Wert) */
        pargetval.altwert = ppointer->par2;
        pargetval.minwert = HFLEVEL_HUB_MIN;
        pargetval.maxwert = HFLEVEL_MAX;
        pargetval.auftrag = 0;
        pargetval.pikto = hflevelps;
        pargetval.kommastellen = 1;
        pargetval.string = floatstr;
        switch ( getvalue_eing() )
        {
        case 0 :
            // ENTER
            ppointer->par2 = pargetval.wert;
            hfuss.akt_posunten.pos = pargetval.wert;
            pzbl.posunten = pargetval.wert;
            /* EasyCODE ( 0 
            Verschiebe-Parameter aendern */
            pargetval.minwert = VPAR_MIN;
            pargetval.maxwert = VPAR_MAX;
            pargetval.auftrag = 0;
            pargetval.altwert = ppointer->par1;
            pargetval.pikto = vparps;
            pargetval.kommastellen = 0;
            pargetval.string = intstr;
            switch ( getvalue_eing() )
            {
            case 0 :
                // ENTER
                ppointer->par1 = pargetval.wert;
                key = 2;
                break;
            case 1 :
                // ESC
                key = 2;
                break;
            default:
                // Stoppanf
                error_res();
                key = 2;
                break;
            }
            /* EasyCODE ) */
            break;
        case 1 :
            // ESC
            key = 2;
            break;
        default:
            // Stoppanf
            error_res();
            key = 2;
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_FUSSHUBSTUFE :
        /* EasyCODE ( 0 
        Huepferfuss Hub (Stufe) */
        pargetval.altwert = ppointer->par2;
        pargetval.minwert = HF_STUFE_MIN;
        pargetval.maxwert = HF_STUFE_MAX;
        pargetval.auftrag = 0;
        pargetval.pikto = hfhubps;
        pargetval.kommastellen = 0;
        pargetval.string = intstr;
        switch ( getvalue_eing() )
        {
        case 0 // ENTER
        :
            //abschnitt [0].par2 = pargetval.wert;
            ppointer->par2 = pargetval.wert;  // SW014
            /* EasyCODE ( 0 
            Verschiebe-Parameter aendern */
            pargetval.minwert = VPAR_MIN;
            pargetval.maxwert = VPAR_MAX;
            pargetval.auftrag = 0;
            pargetval.altwert = ppointer->par1;
            pargetval.pikto = vparps;
            pargetval.kommastellen = 0;
            pargetval.string = intstr;
            switch ( getvalue_eing() )
            {
            case 0 :
                // ENTER
                ppointer->par1 = pargetval.wert;
                key = 2;
                break;
            case 1 :
                // ESC
                key = 2;
                break;
            default:
                // Stoppanf
                error_res();
                key = 2;
                break;
            }
            /* EasyCODE ) */
            break;
        case 1 // ESC
        :
            key = 2;
            break;
        default:
            // Stoppanf
            error_res();
            key = 2;
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_FUSSNIVEAUSTUFE :
        /* EasyCODE ( 0 
        Huepferfuss Niveau (Stufe) */
        pargetval.altwert = ppointer->par2;
        pargetval.minwert = HF_STUFE_MIN;
        pargetval.maxwert = HF_STUFE_MAX;
        pargetval.auftrag = 0;
        pargetval.pikto = hflevelps;
        pargetval.kommastellen = 0;
        pargetval.string = intstr;
        switch ( getvalue_eing() )
        {
        case 0 // ENTER
        :
            //abschnitt [0].par2 = pargetval.wert;
            ppointer->par2 = pargetval.wert;  // SW014
            /* EasyCODE ( 0 
            Verschiebe-Parameter aendern */
            pargetval.minwert = VPAR_MIN;
            pargetval.maxwert = VPAR_MAX;
            pargetval.auftrag = 0;
            pargetval.altwert = ppointer->par1;
            pargetval.pikto = vparps;
            pargetval.kommastellen = 0;
            pargetval.string = intstr;
            switch ( getvalue_eing() )
            {
            case 0 :
                // ENTER
                ppointer->par1 = pargetval.wert;
                key = 2;
                break;
            case 1 :
                // ESC
                key = 2;
                break;
            default:
                // Stoppanf
                error_res();
                key = 2;
                break;
            }
            /* EasyCODE ) */
            break;
        case 1 // ESC
        :
            key = 2;
            break;
        default:
            // Stoppanf
            error_res();
            key = 2;
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_PSTOP :
    case AB_UFWSTOP :
    case AB_ZUSPGEIN :
    case AB_ZUSPGAUS :
    case AB_REDGSWEIN :
    case AB_REDGSWAUS :
    case AB_OUTSET :
    case AB_OUTRES :
    case AB_FLAGSET :
    case AB_FLAGRES :
    case AB_WHIGH :
    case AB_WLOW :
        /* EasyCODE ( 0 
        Verschiebe-Parameter aendern */
        pargetval.minwert = VPAR_MIN;
        pargetval.maxwert = VPAR_MAX;
        pargetval.auftrag = 0;
        pargetval.altwert = ppointer->par1;
        pargetval.pikto = vparps;
        pargetval.kommastellen = 0;
        pargetval.string = intstr;
        switch ( getvalue_eing() )
        {
        case 0 :
            // ENTER
            ppointer->par1 = pargetval.wert;
            key = 2;
            break;
        case 1 :
            // ESC
            key = 2;
            break;
        default:
            // Stoppanf
            error_res();
            key = 2;
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_GERADE :
    case AB_STICH :
    case AB_KURVSTP :
    case AB_KURVENDP :
    case AB_KREISSTP :
    case AB_KREISENDP :
    case AB_BOGENSTP :
    case AB_BOGENENDP :
    case AB_EIL :
    case AB_AP :
        /* EasyCODE ( 0 
        Koordinaten aendern */
        switch ( ppointer->art )
        {
        case AB_GERADE :
            stringptr = PGERADE;
            break;
        case AB_STICH :
            stringptr = PSTICH;
            break;
        case AB_KURVSTP :
            stringptr = PKURVENPUNKT;
            break;
        case AB_KURVENDP :
            stringptr = PKURVENENDE;
            break;
        case AB_KREISSTP :
            if ( (ppointer-1)->art == AB_KREISSTP )
            {
                stringptr = PKREISSTP2;
            }
            else
            {
                stringptr = PKREISSTP1;
            }
            break;
        case AB_KREISENDP :
            stringptr = PKREISSTP3;
            break;
        case AB_BOGENSTP :
            stringptr = PBOGENSTP1;
            break;
        case AB_BOGENENDP :
            stringptr = PBOGENSTP2;
            break;
        case AB_EIL :
            stringptr = PEILGANG;
            break;
        default:
            // AB_AP
            stringptr = PANFANGSPUNKT;
            break;
        }
        switch ( eing.pup.inskoord (stringptr) )
        {
        case 0 :
            ppointer->par1 = nup.workbuf.koorist.xbild;
            ppointer->par2 = nup.workbuf.koorist.ybild;
            koorptr = nup.workbuf.koorist;
            key = 2;
            break;
        case 1 :
            error_res();
            if ( saf->carposok )
            {
                key = 2;
            }
            else
            {
                retwert = E_GST;
            }
            break;
        case 2 :
            key = 2;
            break;
        default:
            break;
        }
        break;
        /* EasyCODE ) */
    default:
        key = 2;
        break;
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProg::block */
/* EasyCODE F */
unsigned char KlasseProg::block(void)
{
    /* EasyCODE < */
    unsigned char retwert = 0xff;
    unsigned char loopflag = TRUE;
    unsigned char loopflag2;
    unsigned char blockey = 0;
    unsigned int scalex = 100;
    unsigned int scaley = 100;
    unsigned int drehwinkel = 0;
    unsigned char spiegeln = FALSE;
    struct p_abschstruct * workptr;
    ikoordtyp diff, zielk;
    /* EasyCODE > */
    while ( loopflag )
    {
        switch ( blockey )
        {
        case 0 :
            /* EasyCODE ( 0 
            blockey = 0, auf Blockanfang positionieren */
            switch ( eing.pup.car_to_blockanf() )
            {
            case 0 :
                blockey = 1;
                break;
            case 1 :
                error_res();
                if ( !saf->carposok )
                {
                    if ( eing.pup.prog_exit () )
                    {
                        eing.ioact = FALSE;
                        retwert = E_GST;
                    }
                    else
                    {
                        koor.eing_err = 30;
                        eing.errpar = konverr;
                        eing.errpar2 = absch_fehler;
                        error_res ();
                        key = 2;
                    }
                }
                else
                {
                    key = 2;
                }
                loopflag = FALSE;
                break;
            case 2 :
                key = 2;
                loopflag = FALSE;
                break;
            case 3 :
                if ( taste_los() == 0xff )
                {
                    key = 10;
                }
                else
                {
                    if ( eing.pup.prog_exit () )
                    {
                        retwert = E_GST;
                    }
                    else
                    {
                        koor.eing_err = 30;
                        eing.errpar = konverr;
                        eing.errpar2 = absch_fehler;
                        error_res ();
                        key = 2;
                    }
                }
                loopflag = FALSE;
                break;
            case 4 :
                if ( eing.pup.prog_exit () )
                {
                    retwert = E_EXIT;
                }
                else
                {
                    koor.eing_err = 30;
                    eing.errpar = konverr;
                    eing.errpar2 = absch_fehler;
                    error_res ();
                    key = 2;
                }
                loopflag = FALSE;
                break;
            }
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            blockey = 1, Anzeige, Tasten abfragen */
            eing_botsch1.senden (BV037);
            sprintf (eing.varstring, blockmanipwertes, scalex, scaley, drehwinkel);
            if ( spiegeln )
            {
                sprintf(eing.varstring1,tinvs ,TSPIEGELN);
                strcat (eing.varstring, eing.varstring1);
            }
            eing_botsch1.senden (eing.varstring);
            switch ( getkey() )
            {
            case TSCALEX :
                /* EasyCODE ( 0 
                Scale X */
                pargetval.minwert = SCALEX_MIN;
                pargetval.maxwert = SCALEX_MAX;
                pargetval.auftrag = 0;
                pargetval.altwert = scalex;
                pargetval.pikto = scalexps;
                pargetval.kommastellen = 0;
                pargetval.string = scalestr;
                switch ( getvalue_eing() )
                {
                case 0 :
                    // ENTER
                    scalex = pargetval.wert;
                    break;
                case 1 :
                    /* EasyCODE < */
                    // ESC
                    /* EasyCODE > */
                    break;
                default:
                    // Stoppanf
                    error_res();
                    break;
                }
                break;
                /* EasyCODE ) */
            case TSCALEY :
                /* EasyCODE ( 0 
                Scale Y */
                pargetval.minwert = SCALEY_MIN;
                pargetval.maxwert = SCALEY_MAX;
                pargetval.auftrag = 0;
                pargetval.altwert = scaley;
                pargetval.pikto = scaleyps;
                pargetval.kommastellen = 0;
                pargetval.string = scalestr;
                switch ( getvalue_eing() )
                {
                case 0 :
                    // ENTER
                    scaley = pargetval.wert;
                    break;
                case 1 :
                    /* EasyCODE < */
                    // ESC
                    /* EasyCODE > */
                    break;
                default:
                    // Stoppanf
                    error_res();
                    break;
                }
                break;
                /* EasyCODE ) */
            case TDREHEN :
                /* EasyCODE ( 0 
                Drehen */
                pargetval.minwert = DREHWINKEL_MIN;
                pargetval.maxwert = DREHWINKEL_MAX;
                pargetval.auftrag = 0;
                pargetval.altwert = drehwinkel;
                pargetval.pikto = drehwinkelps;
                pargetval.kommastellen = 0;
                pargetval.string = drehwinkelstr;
                switch ( getvalue_eing() )
                {
                case 0 :
                    // ENTER
                    drehwinkel = pargetval.wert;
                    break;
                case 1 :
                    /* EasyCODE < */
                    // ESC
                    /* EasyCODE > */
                    break;
                default:
                    // Stoppanf
                    error_res();
                    break;
                }
                break;
                /* EasyCODE ) */
            case TSPIEGELN :
                /* EasyCODE ( 0 
                Spiegeln */
                if ( spiegeln )
                {
                    spiegeln = FALSE;
                }
                else
                {
                    spiegeln = TRUE;
                }
                break;
                /* EasyCODE ) */
            case TENTER :
                /* EasyCODE ( 0 
                Taste Enter */
                konv.block_abb (FP_OFF(blockanf), FP_OFF(blockend), *(ikoordtyp*)(&nup.workbuf.koorist),
                                (float)scalex/100, (float)scaley/100,(float)drehwinkel, spiegeln);
                if ( eing.pup.absch_koord(blockend) )
                {
                    nup.koorsoll [0].xbild = blockend->par1;
                    nup.koorsoll [0].ybild = blockend->par2;
                }
                else
                {
                    konv.suchekoord(FP_OFF(blockend),(ikoordtyp*)&nup.koorsoll[0]);
                }
                konv.verschiebe_abschnitt (FP_OFF(blockend), *(ikoordtyp*)nup.koorsoll );
                key = 2;
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TDEL :
                /* EasyCODE ( 0 
                Taste delete */
                workptr = blockend;
                do{
                    if ( eing.pup.absch_koord(workptr) )
                    {
                        konv.verschiebe_abschnitt ( FP_OFF(workptr), *(ikoordtyp*)&koorptr );
                        break;
                    }
                    else
                    {
                        workptr--;
                    }
                }while ( workptr != blockanf );
                eing.pup.absch_del ( blockanf+1, (FP_OFF(blockend)-FP_OFF(blockanf))/5 );
                blockanf = NULL;
                blockend = NULL;
                key = 2;
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TBLOCKMOVE :
                /* EasyCODE ( 0 
                Taste Block schieben */
                switch ( eing.pup.inskoord (PSCHLITTEN) )
                {
                case 0 :
                    konv.suchekoord (FP_OFF (blockanf+1), (ikoordtyp *) &nup.koorsoll[0]);
                    diff.x = nup.workbuf.koorist.xbild - nup.koorsoll[0].xbild;
                    diff.y = nup.workbuf.koorist.ybild - nup.koorsoll[0].ybild;
                    zielk.x = nup.workbuf.koorist.xbild;
                    zielk.y = nup.workbuf.koorist.ybild;
                    for ( workptr=blockanf, loopflag2 = TRUE; loopflag2; zielk.x = workptr->par1+diff.x, zielk.y = workptr->par2+diff.y )
                    {
                        konv.verschiebe_abschnitt ( FP_OFF(workptr), zielk );
                        do{
                            workptr++;
                            if ( workptr > blockend )
                            {
                                loopflag2 = FALSE;
                                break;
                            }
                        }while ( !eing.pup.absch_koord(workptr) || (workptr->art == AB_KREISENDP) );
                    }
                    koorptr = nup.workbuf.koorist;
                    loopflag = FALSE;
                    key = 2;
                    break;
                case 1 :
                    error_res();
                    if ( saf->carposok )
                    {
                        key = 2;
                    }
                    else
                    {
                        retwert = E_GST;
                    }
                    loopflag = FALSE;
                    break;
                case 2 :
                    loopflag = FALSE;
                    key = 2;
                    break;
                default:
                    break;
                }
                break;
                /* EasyCODE ) */
            case TPROG :
                /* EasyCODE ( 0 
                Taste Programmieren */
                key = 2;
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TENDE :
                /* EasyCODE ( 0 
                Taste Ende */
                if ( eing.pup.prog_exit () )
                {
                    retwert = E_EXIT;
                }
                else
                {
                    koor.eing_err = 30;
                    eing.errpar = konverr;
                    eing.errpar2 = absch_fehler;
                    error_res ();
                    key = 2;
                }
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TEINGABE :
                /* EasyCODE ( 0 
                Taste Eingabe */
                if ( taste_los() == 0xff )
                {
                    key = 10;
                }
                else
                {
                    if ( eing.pup.prog_exit () )
                    {
                        retwert = E_GST;
                    }
                    else
                    {
                        koor.eing_err = 30;
                        eing.errpar = konverr;
                        eing.errpar2 = absch_fehler;
                        error_res ();
                        key = 2;
                    }
                }
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case STOPANF :
                /* EasyCODE ( 0 
                Stopanforderung */
                error_res ();
                break;
                /* EasyCODE ) */
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        default:
            break;
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProg::bild */
/* EasyCODE F */
unsigned char KlasseProg::bild(void)
{
    /* EasyCODE < */
    unsigned char retwert = 0xff;
    unsigned char loopflag = TRUE;
    unsigned char loopflag2;
    unsigned char bildkey = 0;
    unsigned int scalex = 100;
    unsigned int scaley = 100;
    unsigned int drehwinkel = 0;
    unsigned char spiegeln = FALSE;
    unsigned char sympunkt;
    struct p_abschstruct * workptr;
    struct p_abschstruct * blockanf_sav;
    struct p_abschstruct * blockend_sav;
    ikoordtyp diff, zielk, sym;
    /* EasyCODE > */
    while ( loopflag )
    {
        switch ( bildkey )
        {
        case 0 :
            /* EasyCODE ( 0 
            bildkey = 0,  Anzeige, Tasten abfragen */
            eing_botsch1.senden (BV038);
            sprintf (eing.varstring, blockmanipwertes, scalex, scaley, drehwinkel);
            if ( spiegeln )
            {
                sprintf(eing.varstring1,tinvs ,TSPIEGELN);
                strcat (eing.varstring, eing.varstring1);
            }
            eing_botsch1.senden (eing.varstring);
            switch ( getkey() )
            {
            case TSCALEX :
                /* EasyCODE ( 0 
                Scale X */
                pargetval.minwert = SCALEX_MIN;
                pargetval.maxwert = SCALEX_MAX;
                pargetval.auftrag = 0;
                pargetval.altwert = scalex;
                pargetval.pikto = scalexps;
                pargetval.kommastellen = 0;
                pargetval.string = scalestr;
                switch ( getvalue_eing() )
                {
                case 0 :
                    // ENTER
                    scalex = pargetval.wert;
                    break;
                case 1 :
                    /* EasyCODE < */
                    // ESC
                    /* EasyCODE > */
                    break;
                default:
                    // Stoppanf
                    error_res();
                    break;
                }
                break;
                /* EasyCODE ) */
            case TSCALEY :
                /* EasyCODE ( 0 
                Scale Y */
                pargetval.minwert = SCALEY_MIN;
                pargetval.maxwert = SCALEY_MAX;
                pargetval.auftrag = 0;
                pargetval.altwert = scaley;
                pargetval.pikto = scaleyps;
                pargetval.kommastellen = 0;
                pargetval.string = scalestr;
                switch ( getvalue_eing() )
                {
                case 0 :
                    // ENTER
                    scaley = pargetval.wert;
                    break;
                case 1 :
                    /* EasyCODE < */
                    // ESC
                    /* EasyCODE > */
                    break;
                default:
                    // Stoppanf
                    error_res();
                    break;
                }
                break;
                /* EasyCODE ) */
            case TDREHEN :
                /* EasyCODE ( 0 
                Drehen */
                pargetval.minwert = DREHWINKEL_MIN;
                pargetval.maxwert = DREHWINKEL_MAX;
                pargetval.auftrag = 0;
                pargetval.altwert = drehwinkel;
                pargetval.pikto = drehwinkelps;
                pargetval.kommastellen = 0;
                pargetval.string = drehwinkelstr;
                switch ( getvalue_eing() )
                {
                case 0 :
                    // ENTER
                    drehwinkel = pargetval.wert;
                    break;
                case 1 :
                    /* EasyCODE < */
                    // ESC
                    /* EasyCODE > */
                    break;
                default:
                    // Stoppanf
                    error_res();
                    break;
                }
                break;
                /* EasyCODE ) */
            case TSPIEGELN :
                /* EasyCODE ( 0 
                Spiegeln */
                if ( spiegeln )
                {
                    spiegeln = FALSE;
                }
                else
                {
                    spiegeln = TRUE;
                }
                break;
                /* EasyCODE ) */
            case TENTERZIFFER :
                /* EasyCODE ( 0 
                Taste Enterziffer */
                sym.x = nup.workbuf.koorist.xbild;
                sym.y = nup.workbuf.koorist.ybild;
                sympunkt = SYMX;
                bildkey = 1;
                break;
                /* EasyCODE ) */
            case TENTERSCHLITTEN :
                /* EasyCODE ( 0 
                Taste Enterschlitten */
                if ( FP_OFF(ppointer) < FP_OFF(ndatanf + GAP) )
                {
                    eing_botsch1.senden (BELLBELL);
                }
                else
                {
                    bildkey = 2;
                }
                break;
                /* EasyCODE ) */
            case TBILDMOVE :
                /* EasyCODE ( 0 
                Taste Bild schieben */
                if ( eing.pup.mark_enable_test (ppointer) )
                {
                    blockanf_sav = blockanf;
                    blockend_sav = blockend;
                    blockanf = ppointer;
                    blockend = eing.pup.find_absch(AB_PRENDE, ppointer) -1;
                    switch ( eing.pup.inskoord (PSCHLITTEN) )
                    {
                    case 0 :
                        /* EasyCODE ( 0 
                        Bild verschieben */
                        konv.suchekoord (FP_OFF (blockanf+1), (ikoordtyp *) &nup.koorsoll[0]);
                        diff.x = nup.workbuf.koorist.xbild - nup.koorsoll[0].xbild;
                        diff.y = nup.workbuf.koorist.ybild - nup.koorsoll[0].ybild;
                        zielk.x = nup.workbuf.koorist.xbild;
                        zielk.y = nup.workbuf.koorist.ybild;
                        for ( workptr=blockanf, loopflag2 = TRUE; loopflag2; zielk.x = workptr->par1+diff.x, zielk.y = workptr->par2+diff.y )
                        {
                            konv.verschiebe_abschnitt ( FP_OFF(workptr), zielk );
                            do{
                                workptr++;
                                if ( workptr > blockend )
                                {
                                    loopflag2 = FALSE;
                                    break;
                                }
                            }while ( !eing.pup.absch_koord(workptr) || (workptr->art == AB_KREISENDP) );
                        }
                        koorptr = nup.workbuf.koorist;
                        blockanf = blockanf_sav;
                        blockend = blockend_sav;
                        loopflag = FALSE;
                        key = 2;
                        break;
                        /* EasyCODE ) */
                    case 1 :
                        /* EasyCODE ( 0 
                        Fehler oder Stopanforderung */
                        error_res();
                        if ( saf->carposok )
                        {
                            key = 2;
                        }
                        else
                        {
                            retwert = E_GST;
                        }
                        loopflag = FALSE;
                        break;
                        /* EasyCODE ) */
                    case 2 :
                        /* EasyCODE ( 0 
                        Abbruch */
                        loopflag = FALSE;
                        key = 2;
                        break;
                        /* EasyCODE ) */
                    default:
                        break;
                    }
                }
                else
                {
                    eing_botsch1.senden (BELLBELL);
                }
                break;
                /* EasyCODE ) */
            case TPROG :
                /* EasyCODE ( 0 
                Taste Programmieren */
                key = 2;
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TENDE :
                /* EasyCODE ( 0 
                Taste Ende */
                if ( eing.pup.prog_exit () )
                {
                    retwert = E_EXIT;
                }
                else
                {
                    koor.eing_err = 30;
                    eing.errpar = konverr;
                    eing.errpar2 = absch_fehler;
                    error_res ();
                    key = 2;
                }
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case TEINGABE :
                /* EasyCODE ( 0 
                Taste Eingabe */
                if ( taste_los() == 0xff )
                {
                    key = 10;
                }
                else
                {
                    if ( eing.pup.prog_exit () )
                    {
                        retwert = E_GST;
                    }
                    else
                    {
                        koor.eing_err = 30;
                        eing.errpar = konverr;
                        eing.errpar2 = absch_fehler;
                        error_res ();
                        key = 2;
                    }
                }
                loopflag = FALSE;
                break;
                /* EasyCODE ) */
            case STOPANF :
                /* EasyCODE ( 0 
                Stopanforderung */
                error_res ();
                break;
                /* EasyCODE ) */
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            bildkey = 1,   Symmetriepunkt-Eingabe mit Ziffern */
            pargetval.auftrag = 1;
            pargetval.string = koordstr;
            pargetval.kommastellen = 0;
            if ( sympunkt == SYMX )
            {
                pargetval.altwert = sym.x;
                pargetval.maxwert = saf->xnaehfeld_max;
                pargetval.minwert = saf->xnaehfeld_min;
                pargetval.pikto = symxps;
                pargetval.sonderstring = symyp2s;
            }
            else
            {
                pargetval.altwert = sym.y;
                pargetval.maxwert = saf->ynaehfeld_max;
                pargetval.minwert = saf->ynaehfeld_min;
                pargetval.pikto = symyps;
                pargetval.sonderstring = symxp2s;
            }
            switch ( getvalue_eing() )
            {
            case 0 :
                /* EasyCODE ( 0 
                ENTER */
                if ( sympunkt == SYMX )
                {
                    sym.x = pargetval.wert;
                }
                else
                {
                    sym.y = pargetval.wert;
                }
                bildkey = 3;
                break;
                /* EasyCODE ) */
            case 1 :
                // ESC
                bildkey = 0;
                break;
            case 2 :
                // Stopanf
                error_res ();
                break;
            case 3 :
                /* EasyCODE < */
                // Sondertaste
                /* EasyCODE > */
                if ( sympunkt == SYMX )
                {
                    sympunkt = SYMY;
                }
                else
                {
                    sympunkt = SYMX;
                }
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            bildkey = 2,   Symmetriepunkt-Eingabe mit Verfahrtasten */
            switch ( eing.pup.inskoord (PSCHLITTEN) )
            {
            case 0 :
                sym.x = nup.workbuf.koorist.xbild;
                sym.y = nup.workbuf.koorist.ybild;
                bildkey = 3;
                break;
            case 1 :
                error_res();
                if ( saf->carposok )
                {
                    key = 2;
                }
                else
                {
                    retwert = E_GST;
                }
                loopflag = FALSE;
                break;
            case 2 :
                loopflag = FALSE;
                key = 2;
                break;
            default:
                break;
            }
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            bildkey = 3,   Bild manipulieren */
            /* EasyCODE < */
            konv.block_abb (FP_OFF(ndatanf+GPROGNR), FP_OFF(eing.pup.find_absch(AB_PRENDE,(struct p_abschstruct *)(ndatanf+GPROGNR))),
                       sym, (float)scalex/100, (float)scaley/100, (float)drehwinkel, spiegeln);
            koorptr.xbild = ((struct p_abschstruct *)(ndatanf + GAP))->par1;
            koorptr.ybild = ((struct p_abschstruct *)(ndatanf + GAP))->par2;
            nup.bitomo (&koorptr);
            /* EasyCODE > */
            while ( !eing.pup.movlastp() )
            {
                error_res();
                if ( !saf->carposok )
                {
                    retwert = E_GST;
                    break;
                }
            }
            if ( retwert == 0xff )
            {
                /* EasyCODE < */
                ppointer = (struct p_abschstruct *) (ndatanf + GAP);
                abschnr = 5;
                eing.pup.set_pzbl (ppointer);
                key = 2;
                /* EasyCODE > */
            }
            loopflag = FALSE;
            break;
            /* EasyCODE ) */
        default:
            break;
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProg::brotherpos */
/* EasyCODE F */
unsigned char KlasseProg::brotherpos(void)
{
    unsigned char retwert = 0xff;
    unsigned char loopflag;
    
    eing_botsch1.senden (BV029);
    sprintf (eing.varstring, fusss);
    strcat (eing.varstring, fwds);
    eing_botsch1.senden (eing.varstring);
    loopflag = TRUE;
    while ( loopflag )
    {
        switch ( getkey () )
        {
        case TFWD :
            /* EasyCODE ( 0 
            auf AP fahren, Oberteil senken */
            nae.homereq = TRUE;
            nup.carlp = FALSE;
            eing.ioact = TRUE;
            nup.koorsoll [0].xbild = *(int *)(nup.stianfadr+SAP_X);
            nup.koorsoll [0].ybild = *(int *)(nup.stianfadr+SAP_Y);
            if ( nup.poscheck(&nup.koorsoll[0]) )
            {
            }
            else
            {
                /* EasyCODE ( 0 
                Koordinaten begrenzen */
                if ( nup.koorsoll [0].xbild < saf->xnaehfeld_min )
                {
                    nup.koorsoll [0].xbild = saf->xnaehfeld_min;
                }
                if ( nup.koorsoll [0].xbild > saf->xnaehfeld_max )
                {
                    nup.koorsoll [0].xbild = saf->xnaehfeld_max;
                }
                if ( nup.koorsoll [0].ybild < saf->ynaehfeld_min )
                {
                    nup.koorsoll [0].ybild = saf->ynaehfeld_min;
                }
                if ( nup.koorsoll [0].ybild > saf->ynaehfeld_max )
                {
                    nup.koorsoll [0].ybild = saf->ynaehfeld_max;
                }
                /* EasyCODE ) */
            }
            nup.bitomo (&nup.koorsoll [0]);
            if ( !ver.move_sonst() )
            {
                error_res ();
                eing_botsch1.senden (BV029);
                sprintf (eing.varstring, fusss);
                strcat (eing.varstring, fwds);
                eing_botsch1.senden (eing.varstring);
            }
            else
            {
                nup.carmov (STANDARD);
                while ( !smx.ready() || !smy.ready() )
                {
                    if ( !ver.move_sonst() )
                    {
                        smx.stop ();   // verriegelt
                        smy.stop ();
                        saf->carposok = FALSE;
                        retwert = E_EXIT;
                    }
                }
                if ( retwert == 0xff )
                {
                    nup.workbuf.koorist = nup.koorsoll [0];
                    y50u2.setAuftrag();
                    while ( !y50u2.fertig() )
                    {
                        if ( koor.eingstop() )
                        {
                            error_res();
                            eing_botsch1.senden (BV029);
                            sprintf (eing.varstring, fusss);
                            strcat (eing.varstring, fwds);
                            eing_botsch1.senden (eing.varstring);
                            break;
                        }
                    }
                }
                loopflag = FALSE;
            }
            break;
            /* EasyCODE ) */
        case TFUSS :
            /* EasyCODE ( 0 
            Taste Fuss */
            eing.ioact = TRUE;
            if ( !eing.pup.pfoot () )
            {
                error_res ();
                eing_botsch1.senden (BV029);
                sprintf (eing.varstring, fusss);
                strcat (eing.varstring, fwds);
                eing_botsch1.senden (eing.varstring);
            }
            break;
            /* EasyCODE ) */
        case STOPANF :
            /* EasyCODE ( 0 
            Stopanforderung */
            retwert = E_GST;
            loopflag = FALSE;
            break;
            /* EasyCODE ) */
        case TEINGABE :
            /* EasyCODE ( 0 
            Taste Eingabe */
            taste_los();
            retwert = E_GST;
            loopflag = FALSE;
            break;
            /* EasyCODE ) */
        case TENDE :
            /* EasyCODE ( 0 
            Taste Ende */
            taste_los ();
            retwert = E_EXIT;
            loopflag = FALSE;
            break;
            /* EasyCODE ) */
        default:
            break;
        }
    }
    if ( retwert == 0xff )
    {
        koorptr = nup.workbuf.koorist;
        switch ( eing.pup.inskoord (PANFANGSPUNKT) )
        {
        case 0 :
            *(int *)(nup.stianfadr+SAP_X) = nup.workbuf.koorist.xbild ;
            *(int *)(nup.stianfadr+SAP_Y) = nup.workbuf.koorist.ybild ;
            switch ( di.storeprog () )
            {
            case 0 :
                break;
            case 1 :
                koor.eing_err = 25;
                error_res ();
                break;
            case 2 :
                koor.eing_err = 26;
                error_res ();
                break;
            }
            eing.ioact = FALSE;
            retwert = E_GST;
            break;
        case 1 :
            error_res();
            retwert = E_GST;
            break;
        case 2 :
            retwert = E_GST;
            break;
        default:
            break;
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ) */
