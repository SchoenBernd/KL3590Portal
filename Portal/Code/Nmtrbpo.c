/* EasyCODE V8 */
/* EasyCODE ( 0 
N„hmotortreiber Kl3590 */
/* EasyCODE ( 0 
Header, Prototypen, Definitionen */
// Datei:        nmtrbpo.c
// Klasse:       Kl3590 Portal
// Ersteller:    Wilfried Halfmann   EWE 2856
//
// Der N„hmotortreiber ist die Schnittstelle zwischen den Abl„ufen und dem
// N„hmotorinterface. Abh„ngig von nmmodus wird der N„hmotor angesteuert.
// W„hrend dem N„hen wird die N„hgeschwindigkeit ausgegeben und die zugeord-
// neten Rampenoffsets und Verz”gerungen bestimmt.
//
//    nmmodus      Funktion
//       0                    N„hmotor einschalten
//       1                    Ann„hen
//       2                    N„hen
//       3                    Bremsen in 5 Stichen
//       4                    schnelles Bremsen
//       5                    Stop in Nadel OT
//       6                    Stop in Fadenleger OT
//       7                    kleinste Bremsdrehzahl ausgeben, cutact = TRUE,
//                             bei laufendem Motor
//       8                    Schneiden aus dem Stand


#include "portal.h"

KlasseNmTreiber nmtrb;
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::grundzustand */
/* EasyCODE F */
void KlasseNmTreiber::grundzustand (void)
{
    switch ( nmmodus )
    {
    case 0 :
        // N„hmotor einschalten
        nmon ();
        break;
    case 1 :
        // Ann„hen
        annaehen ();
        break;
    case 2 :
        // N„hen
        naeh ();
        break;
    case 3 :
        // Bremsen in 5 Stichen
        bremsen5 ();
        break;
    case 4 :
        // schnelles Bremsen
        bremsen ();
        break;
    case 5 :
        // Stop in OT
        stopot ();
        break;
    case 6 :
        // Stop in Fadenleger OT
        stopflot ();
        break;
    case 7 :
        // kleinste Bremsdrehzahl ausgeben bei laufendem
        // Motor, cutact = TRUE
        bremscut ();
        break;
    case 8 :
        // Schneiden aus dem Stand
        gswoncut ();
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::nmon,   N„hmotor einschalten */
/* EasyCODE F */
void KlasseNmTreiber::nmon (void)
{
    #ifdef trace
        /* EasyCODE < */
        gswind = 0;      // Trace
        gswanz_ind = 0;  // Trace
        smind = 0;
        /* EasyCODE > */
    #endif 
    ofwcount = saf->md.ofwpreset;
    ufwcount = saf->md.ufwpreset;
    zuspgcount = -1;
    if ( saf->cutready )
    {
        ufkcount = saf->md.ufk_verzst;
        if ( saf->md.wedelnfussoben )
        {
            fwkcount = saf->md.fwk_verzst;
            nae.kurzerAnffad = FALSE;
        }
        else
        {
            fwkcount = 0;
            if ( saf->md.kurzerAnffad )
            {
                nae.kurzerAnffad = TRUE;
            }
            else
            {
                nae.kurzerAnffad = FALSE;
            }
        }
    }
    else
    {
        fwkcount = -1;
        if ( nae.kurzerAnffad )
        {
            ufkcount = saf->md.ufk_verzst;
            zuspgcount = 3;
            nae.kurzerAnffad = FALSE;
        }
        else
        {
            ufkcount = -1;
        }
    }
    slowcount = saf->md.slowst;
    gswcount = 5;
    bremscount = 0;
    annaecount = 0;
    saf->cutready = FALSE;
    nae.cutact = FALSE;
    nae.cutreq = FALSE;
    nmstop = FALSE;
    gswakt = min (saf->md.annaetab [0] - 1, saf->md.maxgsw);
    nminf.drehzahl = ((int) gswakt) * 100;
    nminf.befehl = 7;
    #ifdef trace
        /* EasyCODE < */
        gsw_out [0] = gswakt; // Trace
        gswind = 1;           // Trace
        /* EasyCODE > */
    #endif 
    if ( nup.mbef [SCHNEIDEN] )
    {
        y7.resAuftrag();
        y32.resAuftrag();
        nmmodus = 7;
    }
    else
    {
        if ( nae.kurzerAnffad )
        {
            nmmodus = 6;
        }
        else
        {
            if ( nup.mbef [BREMSEN] )
            {
                bremscount = 0;
                bremsverzcount = 0;
                bremsrdy = FALSE;
                nmmodus = 3;
            }
            else
            {
                if ( nup.mbef [STOP] || nup.mbef[NAEHENAUS] )
                {
                    nmmodus = 5;
                }
                else
                {
                    nmmodus = 1;
                }
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::annaehen,   Ann„hen */
/* EasyCODE F */
void KlasseNmTreiber::annaehen (void)
{
    /* EasyCODE < */
    gswinten = FALSE; // kein integrierender Hochlauf
    /* EasyCODE > */
    if ( slowcount == 0 )
    {
        annaecount ++;
        if ( annaecount == 7 )
        {
            nmmodus = 2;
        }
        gswaus (saf->md.annaetab [annaecount]);
    }
    else
    {
        slowcount --;
        gswaus (saf->md.annaetab [0]);
    }
    /* EasyCODE ( 0 
    Oberfadenw„chter */
    switch ( ofwcount )
    {
    case -1 :
        break;
    case 0 :
        ofwcount = -1;
        if ( saf->md.ofwen )
        {
            ofwaktiv = TRUE;
        }
        break;
    default:
        ofwcount --;
        break;
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Unterfadenw„chter */
    switch ( ufwcount )
    {
    case -1 :
        break;
    case 0 :
        ufwcount = -1;
        if ( saf->md.ufwen > 1 )
        {
            ufwaktiv = TRUE;
        }
        break;
    default:
        ufwcount --;
        break;
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Fadenwedlerklemme */
    switch ( fwkcount )
    {
    case -1 :
        break;
    case 0 :
        fwkcount = -1;
        y2fkt_drucklos.setAuftrag();
        break;
    default:
        fwkcount --;
        break;
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Unterfadenklemme */
    switch ( ufkcount )
    {
    case -1 :
        break;
    case 0 :
        ufkcount = -1;
        y31.setAuftrag();
        break;
    default:
        ufkcount --;
        break;
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Zusatzspannung */
    switch ( zuspgcount )
    {
    case -1 :
        break;
    case 0 :
        zuspgcount = -1;
        if ( !nup.workbuf.zuspgon )
        {
            // Zusatzfadenspannung auf
            y6.resAuftrag();
        }
        break;
    default:
        zuspgcount --;
        break;
    }
    /* EasyCODE ) */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::naeh,   N„hen */
/* EasyCODE F */
void KlasseNmTreiber::naeh (void)
{
    gswaus (nup.workbuf.naegsw);
    /* EasyCODE ( 0 
    Oberfadenw„chter */
    switch ( ofwcount )
    {
    case -1 :
        break;
    case 0 :
        ofwcount = -1;
        if ( saf->md.ofwen )
        {
            ofwaktiv = TRUE;
        }
        break;
    default:
        ofwcount --;
        break;
    }
    /* EasyCODE ) */
    if ( saf->md.koueberw    // SW003DIS
       )
    {
        kofwaktiv = TRUE;
    }
    /* EasyCODE ( 0 
    Unterfadenw„chter */
    switch ( ufwcount )
    {
    case -1 :
        break;
    case 0 :
        ufwcount = -1;
        if ( saf->md.ufwen > 1 )
        {
            ufwaktiv = TRUE;
        }
        break;
    default:
        ufwcount --;
        break;
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Fadenwedlerklemme */
    switch ( fwkcount )
    {
    case -1 :
        break;
    case 0 :
        fwkcount = -1;
        y2fkt_drucklos.setAuftrag();
        break;
    default:
        fwkcount --;
        break;
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Unterfadenklemme */
    switch ( ufkcount )
    {
    case -1 :
        break;
    case 0 :
        ufkcount = -1;
        y31.setAuftrag();
        break;
    default:
        ufkcount --;
        break;
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Zusatzspannung */
    switch ( zuspgcount )
    {
    case -1 :
        break;
    case 0 :
        zuspgcount = -1;
        if ( !nup.workbuf.zuspgon )
        {
            // Zusatzfadenspannung auf
            y6.resAuftrag();
        }
        break;
    default:
        zuspgcount --;
        break;
    }
    /* EasyCODE ) */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::bremsen5,   in 5 Stichen bis zum Stop bremsen */
/* EasyCODE F */
void KlasseNmTreiber::bremsen5 (void)
{
    /* EasyCODE < */
    gswinten = FALSE;  // kein integrierendes Bremsen
    /* EasyCODE > */
    if ( slowcount == 0 )
    {
        if ( bremsverzcount == 0 )
        {
            gswaus (min (saf->bremstab [bremscount], gswakt));
        }
        else
        {
            gswaus (gswakt);
            bremsverzcount--;
            return ;
        }
    }
    else
    {
        if ( saf->md.annaetab [0] < saf->bremstab [bremscount] )
        {
            gswaus (saf->md.annaetab [0]);
        }
        else
        {
            gswaus (saf->bremstab [bremscount]);
        }
        slowcount --;
    }
    ofwaktiv = FALSE;
    ufwaktiv = FALSE;
    kofwaktiv = FALSE;  // SW003DIS
    if ( bremscount != 4 )
    {
        bremscount ++;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::bremsen,   schnellstm”glich bremsen bis zum Stop */
/* EasyCODE F */
void KlasseNmTreiber::bremsen (void)
{
    /* EasyCODE < */
    gswinten = FALSE;  // kein integrierendes Bremsen
    /* EasyCODE > */
    if ( (bremsverzcount == 0) || (gswcount == 0) )
    {
        gswaus (saf->bremstab [4]);
    }
    else
    {
        gswaus (gswakt);
        bremsverzcount --;
    }
    ofwaktiv = FALSE;
    ufwaktiv = FALSE;
    kofwaktiv = FALSE;    // SW003DIS
    if ( gswakt <= saf->bremstab [4] )
    {
        bremsrdy = TRUE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::stopot,   Stop in Nadel OT */
/* EasyCODE F */
void KlasseNmTreiber::stopot (void)
{
    if ( nminf.befehl != 0 )
    {
        koor.nae_err = 10;
        nae.errpar = 1;
    }
    else
    {
        if ( nminf.fehler != 0 )
        {
            koor.nae_err = 1;
            nae.errpar = nminf.fehler;
        }
        else
        {
            nminf.position = saf->nadelp >> 1;
            nminf.posart = 0;
            nminf.befehl = 5;
            nmstop = TRUE;
            gswaus (0);
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::stopflot,   Stop in Fadenleger OT */
/* EasyCODE F */
void KlasseNmTreiber::stopflot (void)
{
    if ( nminf.befehl != 0 )
    {
        koor.nae_err = 10;
        nae.errpar = 2;
    }
    else
    {
        if ( nminf.fehler != 0 )
        {
            koor.nae_err = 1;
            nae.errpar = nminf.fehler;
        }
        else
        {
            postemp = (saf->nadelp >> 1) + ((float)saf->md.fadlot*MAXPOS/360);
            if ( postemp >= MAXPOS )
            {
                postemp -= MAXPOS;
            }
            nminf.position = postemp;
            nminf.posart = 0;
            nminf.befehl = 5;
            nmstop = TRUE;
            gswaus (0);
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::bremscut,   kleinste Bremsdrehzahl ausgeben
                             und cutact setzen bei laufendem Motor */
/* EasyCODE F */
void KlasseNmTreiber::bremscut (void)
{
    gswinten = FALSE;
    gswaus (saf->bremstab [5]);
    nae.cutact = TRUE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::gswoncut,   Drehzahl ein zum Schneiden aus dem Stand */
/* EasyCODE F */
void KlasseNmTreiber::gswoncut (void)
{
    if ( nminf.befehl == 0 )
    {
        if ( nminf.fehler != 0 )
        {
            koor.nae_err = 1;
            nae.errpar = nminf.fehler;
        }
        else
        {
            /* EasyCODE < */
            gswakt = GSWON;
            nminf.drehzahl =  GSWON * 100;
            nminf.befehl = 7;
            nae.cutact = TRUE;
            nae.cutreq = TRUE;
            /* EasyCODE > */
            #ifdef trace
                /* EasyCODE < */
                gsw_out [0] = GSWON;  // Trace
                gswind = 1;                          // Trace
                /* EasyCODE > */
            #endif 
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::gswaus,   N„hgeschwindigkeit und N„htabellen betimmen */
/* EasyCODE F */
void KlasseNmTreiber::gswaus (unsigned char zielgsw)
{
    unsigned char sollgsw;
    char i;
    if ( (nmmodus == 5) || (nmmodus ==6) )
    {
        /* EasyCODE ( 0 
        Stop in Nadel OT */
        gswakt = GSWON;
        naetabausw (GSWON);
        /* EasyCODE ) */
    }
    else
    {
        sollgsw = min (min(zielgsw,nup.workbuf.naegsw), min(saf->md.maxgsw,nup.gswkoor));
        if ( nup.workbuf.redgswen )
        {
            sollgsw = min (min(sollgsw, nup.fussgsw), min(nup.stigsw, saf->md.redgsw));
        }
        else
        {
            sollgsw = min(nup.fussgsw, min(sollgsw, nup.stigsw));
        }
        if ( sollgsw > gswakt )
        {
            /* EasyCODE ( 0 
            Beschleunigen */
            if ( gswinten && (sollgsw <= gswakt+20) )
            {
                /* EasyCODE < */
                // integrierender Hochlauf
                /* EasyCODE > */
                for ( i=0; i<17; i++ )
                {
                    if ( saf->inttab [i] > gswakt )
                    {
                        if ( saf->inttab [i] < sollgsw )
                        {
                            sollgsw = saf->inttab [i];
                        }
                        break;
                    }
                }
            }
            else
            {
                // schneller Hochlauf
                gswinten = FALSE;
                for ( i=0; i<8; i++ )
                {
                    if ( saf->md.annaetab [i] > gswakt )
                    {
                        if ( saf->md.annaetab [i] < sollgsw )
                        {
                            sollgsw = saf->md.annaetab [i];
                        }
                        break;
                    }
                }
            }
            gswup (sollgsw);
            gswcount = 5;
            if ( (annaecount > 1)&&!gswinten )
            {
                naetabausw ((((nminf.get_drehzahl()+50) / 100) + 3*gswakt) / 4);
            }
            else
            {
                naetabausw (gswakt);
            }
            /* EasyCODE_STRUCTURED_COMMENT_START */
            /*
            naetabausw (gswakt);
            */
            /* EasyCODE_STRUCTURED_COMMENT_END */
            /* EasyCODE ) */
        }
        else
        {
            if ( sollgsw == gswakt )
            {
                /* EasyCODE ( 0 
                Sollgeschww. = Istgeschw. */
                if ( nminf.befehl != 0 )
                {
                    koor.nae_err = 10;
                    nae.errpar = 1;
                }
                else
                {
                    if ( nminf.fehler != 0 )
                    {
                        koor.nae_err = 1;
                    }
                    else
                    {
                        nminf.tot_fastout ();
                        if ( gswcount == 0 )
                        {
                            naetabausw ((nminf.get_drehzahl()+50) / 100);
                        }
                        else
                        {
                            gswcount --;
                            if ( annaecount > 1 )
                            {
                                naetabausw ((((nminf.get_drehzahl()+50) / 100) + 3*gswakt) / 4);
                            }
                            else
                            {
                                naetabausw (gswakt);
                            }
                            /* EasyCODE_STRUCTURED_COMMENT_START */
                            /*
                            naetabausw (gswakt);
                            */
                            /* EasyCODE_STRUCTURED_COMMENT_END */
}
                        gswinten = TRUE;
                    }
                }
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                Bremsen */
                if ( gswinten && (sollgsw >= gswakt-20) )
                {
                    /* EasyCODE < */
                    // integrierendes Bremsen
                    /* EasyCODE > */
                    for ( i=16; i>=0; i-- )
                    {
                        if ( saf->inttab [i] < gswakt )
                        {
                            if ( saf->inttab [i] > sollgsw )
                            {
                                sollgsw = saf->inttab [i];
                            }
                            break;
                        }
                    }
                }
                else
                {
                    // schnelles Bremsen
                    gswinten = FALSE;
                    for ( i=0; i<6; i++ )
                    {
                        if ( saf->bremstab [i] < gswakt )
                        {
                            if ( saf->bremstab [i] > sollgsw )
                            {
                                sollgsw = saf->bremstab [i];
                            }
                            break;
                        }
                    }
                }
                gswup (sollgsw);
                gswcount = 0;
                if ( bremscount > 3 )
                {
                    naetabausw (max(((((nminf.get_drehzahl()+50) / 100) + 3*gswakt) / 4), gswakt));
                }
                else
                {
                    naetabausw (max(((nminf.get_drehzahl()+50) / 100), gswakt));
                }
                /* EasyCODE ) */
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::gswup,   N„hgeschwindigkeit ausgeben */
/* EasyCODE F */
void KlasseNmTreiber::gswup (unsigned char gsw)
{
    if ( nminf.befehl != 0 )
    {
        koor.nae_err = 10;
        nae.errpar = 4;
    }
    else
    {
        if ( nminf.fehler != 0 )
        {
            koor.nae_err = 1;
            nae.errpar = nminf.fehler;
        }
        else
        {
            if ( gsw <= GSWMAX )
            {
                gswakt = gsw;
            }
            else
            {
                gswakt = GSWMAX;
            }
            nminf.drehzahl = ((int) gsw) * 100;
            nminf.befehl = 7;
            nminf.gsw_fastout ();
            #ifdef trace
                if ( gswind < 100 )
                {
                    // Trace
                    gsw_out [gswind] = gsw;
                    gswind ++;
                }
            #endif 
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNmTreiber::naetabausw */
/* EasyCODE F */
void KlasseNmTreiber::naetabausw (unsigned char gsw)
{
    if ( gsw > GSWMAX )
    {
        gsw = GSWMAX;
    }
    if ( (annaecount == 7) && (gswcount == 0) )
    {
        if ( gswakt > gsw )
        {
            gsw = gswakt;
        }
    }
    if ( gsw > nup.gswvor [nup.vorind] )
    {
        gsw = nup.gswvor [nup.vorind];
    }
    smx.verz = verzsmx [gsw][smx.rampadr >>2];
    if ( saf->md.matdicke == DUENN )
    {
        smy.verz = verzsmy [gsw][smy.rampadr >>2];
    }
    else
    {
        smy.verz = verzsmy_schwer [gsw][smy.rampadr >>2];
    }
    smx.rampadr += rampoffsmx [gsw];
    smy.rampadr += rampoffsmy [gsw];
    #if defined(trace)
        verzsm [smind][0] = smx.verz;           // Trace
        verzsm [smind][1] = smy.verz;           // Trace
        adr [smind][0] = smx.rampadr;           // Trace
        adr [smind][1] = smy.rampadr;           // Trace
        if ( smind == 99 )
        {
            /* EasyCODE < */
            smind = 0;                 // Trace
            /* EasyCODE > */
        }
        else
        {
            /* EasyCODE < */
            smind ++;                             // Trace
            /* EasyCODE > */
        }
        /* EasyCODE < */
        gswanz [gswanz_ind] [0] = gswakt;          // Trace
        gswanz [gswanz_ind] [1] = gsw;             // Trace
        gswanz [gswanz_ind] [2] = (nminf.get_drehzahl()+50) / 100; // Trace
        /* EasyCODE > */
        if ( gswanz_ind == 49 )
        {
            /* EasyCODE < */
            gswanz_ind = 0;      // Trace
            /* EasyCODE > */
        }
        else
        {
            /* EasyCODE < */
            gswanz_ind ++;                        // Trace
            /* EasyCODE > */
        }
    #endif 
}

/* EasyCODE ) */
/* EasyCODE ) */
