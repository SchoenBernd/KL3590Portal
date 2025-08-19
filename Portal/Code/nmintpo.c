/* EasyCODE V8 */
/* EasyCODE ( 0 
nmintpo.c */
// Datei:          nmintpo
// Klasse:         Kl3590 Portal
// Ersteller:      Halfmann   EWE 2856

#include "portal.h"
KlasseNis nis (tdini [NisIndex]);
KlasseRef2 ref2 (tdini [Ref2Index]);
/* EasyCODE ( 0 
KlasseNis::nistask */
/* EasyCODE F */
void KlasseNis::nistask (void)
{
    float x;
    unsigned char hlpind0, hlpind1, hlpind2, hlpind3, hlpind4, cont, i;
    /* EasyCODE - */
    tnae1.rsz();
    if ( nae.cutreq )
    {
        /* EasyCODE ( 0 
        Schneiden oder Naehen aus */
        if ( nup.mbef[NAEHENAUS] && (nae.key == 3) && !nae.ofastoe )
        {
            // Naehen aus
            // Schneiddrehzahl und Stop in Fadenleger OT
            nmtrb.nmmodus = 6;
            nmtrb.grundzustand ();
        }
        else
        {
            // Schneiden
            nmtrb.gswakt = saf->md.cutgsw/10;
            nminf.drehzahl = saf->md.cutgsw*10;
            nminf.befehl = 7;
            nminf.gsw_fastout ();
            #ifdef trace
                if ( nmtrb.gswind < 100 )
                {
                    // Trace
                    nmtrb.gsw_out [nmtrb.gswind] = saf->md.cutgsw/10;
                    nmtrb.gswind ++;
                }
            #endif 
            // REF2 loeschen und freigeben
            outportb (INTCLR, REF2_SETCLR);
            outportb (INTCLR, REF2_RESCLR);
            outportb (INTMASK, REF2EI);
        }
        /* EasyCODE ) */
    }
    else
    {
        if ( koor.nae_err != 0 )
        {
            niserr ();
        }
        else
        {
/* EasyCODE_STRUCTURED_COMMENT_START */
            /*
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Unterfadenschieber setzen/ruecksetzen EasyCODE_COMMENT_END
            if ( nae.cutact )
            {
                // Nadeldreheinrichtung aus
                // Unterfadenschieber aus
                y7.resAuftrag();
                y32.resAuftrag();
            }
            else
            {
                switch ( saf->md.drehmodus )
                {
                case 0 :
                    // Modus: ausgeschaltet
                    break;
                case 1 :
                    if ( (nup.workbuf.flag & 0x01) == 0 )
                    {
                        if ( !y7.getZustand() && !y32.getZustand() )
                        {
                            EasyCODE_COMMENT_START EasyCODE ( 0 
                            Unterfadenschieber
                            ist ausgeschaltet,
                            Einschalten ? Die
                            Einschaltbedingung muss
                            fuer die naechsten 3 Stiche gelten. EasyCODE_COMMENT_END
                            if ( ((nup.koorsoll[nup.vorind].xbild - nup.workbuf.koorist.xbild) > 0) &&
                                 ((nup.koorsoll[nup.vorind].ybild - nup.workbuf.koorist.ybild) < 0) &&
                                 (saf->md.nadelanzahl == 1) )
                            {
                                x = (float)(nup.koorsoll[nup.vorind].xbild - nup.workbuf.koorist.xbild) /
                                           (-nup.koorsoll[nup.vorind].ybild + nup.workbuf.koorist.ybild);
                                if ( (atan(x) >= saf->fadschein_winkel1) &&
                                     (atan(x) <= saf->fadschein_winkel2) )
                                {
                                    hlpind1 = nup.vorind;
                                    if ( hlpind1 == 10 )
                                    {
                                        hlpind1 = 0;
                                    }
                                    else
                                    {
                                        hlpind1++;
                                    }
                                    if ( ((nup.koorsoll[hlpind1].xbild - nup.koorsoll[nup.vorind].xbild) > 0) &&
                                         ((nup.koorsoll[hlpind1].ybild - nup.koorsoll[nup.vorind].ybild) < 0) )
                                    {
                                        x = (float)(nup.koorsoll[hlpind1].xbild - nup.koorsoll[nup.vorind].xbild) /
                                                   (-nup.koorsoll[hlpind1].ybild + nup.koorsoll[nup.vorind].ybild);
                                        if ( (atan(x) >= saf->fadschein_winkel1) &&
                                             (atan(x) <= saf->fadschein_winkel2) )
                                        {
                                            hlpind2 = hlpind1;
                                            if ( hlpind2 == 10 )
                                            {
                                                hlpind2 = 0;
                                            }
                                            else
                                            {
                                                hlpind2++;
                                            }
                                            if ( ((nup.koorsoll[hlpind2].xbild - nup.koorsoll[hlpind1].xbild) > 0) &&
                                                 ((nup.koorsoll[hlpind2].ybild - nup.koorsoll[hlpind1].ybild) < 0) )
                                            {
                                                x = (float)(nup.koorsoll[hlpind2].xbild - nup.koorsoll[hlpind1].xbild) /
                                                           (-nup.koorsoll[hlpind2].ybild + nup.koorsoll[hlpind1].ybild);
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
                            }
                            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                        }
                        else
                        {
                            EasyCODE_COMMENT_START EasyCODE ( 0 
                            Unterfadenschieber
                            ist eingeschaltet,
                            Ausschalten ? Die
                            Ausschaltbedingung muss
                            fuer die naechsten 3 Stiche gelten. EasyCODE_COMMENT_END
                            cont = TRUE;
                            if ( ((nup.koorsoll[nup.vorind].xbild - nup.workbuf.koorist.xbild) > 0) &&
                                 ((nup.koorsoll[nup.vorind].ybild - nup.workbuf.koorist.ybild) < 0) )
                            {
                                x = (float)(nup.koorsoll[nup.vorind].xbild - nup.workbuf.koorist.xbild) /
                                           (-nup.koorsoll[nup.vorind].ybild + nup.workbuf.koorist.ybild);
                                if ( (atan(x) > saf->fadschaus_winkel1) &&
                                     (atan(x) < saf->fadschaus_winkel2) )
                                {
                                    cont = FALSE;
                                }
                            }
                            if ( cont )
                            {
                                hlpind1 = nup.vorind;
                                if ( hlpind1 == 10 )
                                {
                                    hlpind1 = 0;
                                }
                                else
                                {
                                    hlpind1++;
                                }
                                if ( ((nup.koorsoll[hlpind1].xbild - nup.koorsoll[nup.vorind].xbild) > 0) &&
                                     ((nup.koorsoll[hlpind1].ybild - nup.koorsoll[nup.vorind].ybild) < 0) )
                                {
                                    x = (float)(nup.koorsoll[hlpind1].xbild - nup.koorsoll[nup.vorind].xbild) /
                                               (-nup.koorsoll[hlpind1].ybild + nup.koorsoll[nup.vorind].ybild);
                                    if ( (atan(x) > saf->fadschaus_winkel1) &&
                                         (atan(x) < saf->fadschaus_winkel2) )
                                    {
                                        cont = FALSE;
                                    }
                                }
                                if ( cont )
                                {
                                    hlpind2 = hlpind1;
                                    if ( hlpind2 == 10 )
                                    {
                                        hlpind2 = 0;
                                    }
                                    else
                                    {
                                        hlpind2++;
                                    }
                                    if ( ((nup.koorsoll[hlpind2].xbild - nup.koorsoll[hlpind1].xbild) > 0) &&
                                         ((nup.koorsoll[hlpind2].ybild - nup.koorsoll[hlpind1].ybild) < 0) )
                                    {
                                        x = (float)(nup.koorsoll[hlpind2].xbild - nup.koorsoll[hlpind1].xbild) /
                                                   (-nup.koorsoll[hlpind2].ybild + nup.koorsoll[hlpind1].ybild);
                                        if ( (atan(x) > saf->fadschaus_winkel1) &&
                                             (atan(x) < saf->fadschaus_winkel2) )
                                        {
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
                            }
                            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                        }
                    }
                    break;
                case 2 :
                    // Modus: Manuell
                    break;
                default:
                    EasyCODE_COMMENT_START EasyCODE ( 0 
                    Modus: Automatik und Manuell
                    Unterfadenschieber setzen/ruecksetzen EasyCODE_COMMENT_END
                    if ( !y7.getZustand() && !y32.getZustand() )
                    {
                        EasyCODE_COMMENT_START EasyCODE ( 0 
                        Unterfadenschieber
                        ist ausgeschaltet,
                        Einschalten ? Die
                        Einschaltbedingung muss
                        fuer die naechsten 3 Stiche gelten. EasyCODE_COMMENT_END
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
                                    if ( ((nup.koorsoll[nup.vorind].xbild - nup.workbuf.koorist.xbild) > 0) &&
                                         ((nup.koorsoll[nup.vorind].ybild - nup.workbuf.koorist.ybild) < 0) )
                                    {
                                        x = (float)(nup.koorsoll[nup.vorind].xbild - nup.workbuf.koorist.xbild) /
                                                   (-nup.koorsoll[nup.vorind].ybild + nup.workbuf.koorist.ybild);
                                        if ( (atan(x) >= saf->fadschein_winkel1) &&
                                             (atan(x) <= saf->fadschein_winkel2) )
                                        {
                                            hlpind1 = nup.vorind;
                                            if ( hlpind1 == 10 )
                                            {
                                                hlpind1 = 0;
                                            }
                                            else
                                            {
                                                hlpind1++;
                                            }
                                            if ( ((nup.koorsoll[hlpind1].xbild - nup.koorsoll[nup.vorind].xbild) > 0) &&
                                                 ((nup.koorsoll[hlpind1].ybild - nup.koorsoll[nup.vorind].ybild) < 0) )
                                            {
                                                x = (float)(nup.koorsoll[hlpind1].xbild - nup.koorsoll[nup.vorind].xbild) /
                                                           (-nup.koorsoll[hlpind1].ybild + nup.koorsoll[nup.vorind].ybild);
                                                if ( (atan(x) >= saf->fadschein_winkel1) &&
                                                     (atan(x) <= saf->fadschein_winkel2) )
                                                {
                                                    hlpind2 = hlpind1;
                                                    if ( hlpind2 == 10 )
                                                    {
                                                        hlpind2 = 0;
                                                    }
                                                    else
                                                    {
                                                        hlpind2++;
                                                    }
                                                    if ( ((nup.koorsoll[hlpind2].xbild - nup.koorsoll[hlpind1].xbild) > 0) &&
                                                         ((nup.koorsoll[hlpind2].ybild - nup.koorsoll[hlpind1].ybild) < 0) )
                                                    {
                                                        x = (float)(nup.koorsoll[hlpind2].xbild - nup.koorsoll[hlpind1].xbild) /
                                                                   (-nup.koorsoll[hlpind2].ybild + nup.koorsoll[hlpind1].ybild);
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
                                    }
                                }
                            }
                        }
                        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                    }
                    else
                    {
                        EasyCODE_COMMENT_START EasyCODE ( 0 
                        Unterfadenschieber
                        ist eingeschaltet,
                        Ausschalten ? Die
                        Ausschaltbedingung muss
                        fuer die naechsten 3 Stiche gelten. EasyCODE_COMMENT_END
                        if ( out1.getZustand() )
                        {
                        }
                        else
                        {
                            if ( (nup.workbuf.flag & 0x01) == 0 )
                            {
                                cont = TRUE;
                                if ( ((nup.koorsoll[nup.vorind].xbild - nup.workbuf.koorist.xbild) > 0) &&
                                     ((nup.koorsoll[nup.vorind].ybild - nup.workbuf.koorist.ybild) < 0) )
                                {
                                    x = (float)(nup.koorsoll[nup.vorind].xbild - nup.workbuf.koorist.xbild) /
                                               (-nup.koorsoll[nup.vorind].ybild + nup.workbuf.koorist.ybild);
                                    if ( (atan(x) > saf->fadschaus_winkel1) &&
                                         (atan(x) < saf->fadschaus_winkel2) )
                                    {
                                        cont = FALSE;
                                    }
                                }
                                if ( cont )
                                {
                                    hlpind1 = nup.vorind;
                                    if ( hlpind1 == 10 )
                                    {
                                        hlpind1 = 0;
                                    }
                                    else
                                    {
                                        hlpind1++;
                                    }
                                    if ( ((nup.koorsoll[hlpind1].xbild - nup.koorsoll[nup.vorind].xbild) > 0) &&
                                         ((nup.koorsoll[hlpind1].ybild - nup.koorsoll[nup.vorind].ybild) < 0) )
                                    {
                                        x = (float)(nup.koorsoll[hlpind1].xbild - nup.koorsoll[nup.vorind].xbild) /
                                                   (-nup.koorsoll[hlpind1].ybild + nup.koorsoll[nup.vorind].ybild);
                                        if ( (atan(x) > saf->fadschaus_winkel1) &&
                                             (atan(x) < saf->fadschaus_winkel2) )
                                        {
                                            cont = FALSE;
                                        }
                                    }
                                    if ( cont )
                                    {
                                        hlpind2 = hlpind1;
                                        if ( hlpind2 == 10 )
                                        {
                                            hlpind2 = 0;
                                        }
                                        else
                                        {
                                            hlpind2++;
                                        }
                                        if ( ((nup.koorsoll[hlpind2].xbild - nup.koorsoll[hlpind1].xbild) > 0) &&
                                             ((nup.koorsoll[hlpind2].ybild - nup.koorsoll[hlpind1].ybild) < 0) )
                                        {
                                            x = (float)(nup.koorsoll[hlpind2].xbild - nup.koorsoll[hlpind1].xbild) /
                                                       (-nup.koorsoll[hlpind2].ybild + nup.koorsoll[hlpind1].ybild);
                                            if ( (atan(x) > saf->fadschaus_winkel1) &&
                                                 (atan(x) < saf->fadschaus_winkel2) )
                                            {
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
                    break;
                }
            }
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
            */
            /* EasyCODE_STRUCTURED_COMMENT_END */
            /* EasyCODE ( 0 
            Unterfadenschieber setzen/ruecksetzen  SW014 */
            if ( nae.cutact )
            {
                // Nadeldreheinrichtung aus
                // Unterfadenschieber aus
                y7.resAuftrag();
                y32.resAuftrag();
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
                    if ( (nup.workbuf.flag & 0x01) == 0 )
                    {
                        if ( !y7.getZustand() && !y32.getZustand() )
                        {
                            EasyCODE_COMMENT_START EasyCODE ( 0 
                            Unterfadenschieber
                            ist ausgeschaltet,
                            Einschalten ? Die
                            Einschaltbedingung muss
                            fuer die naechsten 3 Stiche gelten. EasyCODE_COMMENT_END
                            if ( (saf->md.nadelanzahl == 1) )
                            {
                                if ( nup.nadeldreh (CHKEIN, nup.koorsoll[nup.vorind].xbild, nup.koorsoll[nup.vorind].ybild, 
                                                        nup.workbuf.koorist.xbild ,nup.workbuf.koorist.ybild ) )
                                {
                                    hlpind1 = nup.vorind;
                                    if ( hlpind1 == 10 )
                                    {
                                        hlpind1 = 0;
                                    }
                                    else
                                    {
                                        hlpind1++;
                                    }
                                    if ( nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind1].xbild, nup.koorsoll[hlpind1].ybild, 
                                                            nup.koorsoll[nup.vorind].xbild ,nup.koorsoll[nup.vorind].ybild ) )
                                    {
                                        hlpind2 = hlpind1;
                                        if ( hlpind2 == 10 )
                                        {
                                            hlpind2 = 0;
                                        }
                                        else
                                        {
                                            hlpind2++;
                                        }
                                        if ( nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind2].xbild, nup.koorsoll[hlpind2].ybild, 
                                                                nup.koorsoll[hlpind1].xbild ,nup.koorsoll[hlpind1].ybild ) )
                                        {
                                            hlpind3 = hlpind2;
                                            if ( hlpind3 == 10 )
                                            {
                                                hlpind3 = 0;
                                            }
                                            else
                                            {
                                                hlpind3++;
                                            }
                                            if ( nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind3].xbild, nup.koorsoll[hlpind3].ybild, 
                                                                    nup.koorsoll[hlpind2].xbild ,nup.koorsoll[hlpind2].ybild)
                                                                     &&   (nup.workbuf.nbtart != 3) )
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
                            Unterfadenschieber
                            ist eingeschaltet,
                            Ausschalten ? Die
                            Ausschaltbedingung muss
                            fuer die naechsten 3 Stiche gelten. EasyCODE_COMMENT_END
                            cont = TRUE;
                            if ( nup.nadeldreh (CHKAUS, nup.koorsoll[nup.vorind].xbild, nup.koorsoll[nup.vorind].ybild, 
                                                    nup.workbuf.koorist.xbild ,nup.workbuf.koorist.ybild ) )
                            {
                                // Ausschalten
                            }
                            else
                            {
                                cont = FALSE;
                            }
                            if ( cont )
                            {
                                hlpind1 = nup.vorind;
                                if ( hlpind1 == 10 )
                                {
                                    hlpind1 = 0;
                                }
                                else
                                {
                                    hlpind1++;
                                }
                                if ( nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind1].xbild, nup.koorsoll[hlpind1].ybild, 
                                                        nup.koorsoll[nup.vorind].xbild ,nup.koorsoll[nup.vorind].ybild ) )
                                {
                                    // Ausschalten
                                }
                                else
                                {
                                    cont = FALSE;
                                }
                                if ( cont )
                                {
                                    hlpind2 = hlpind1;
                                    if ( hlpind2 == 10 )
                                    {
                                        hlpind2 = 0;
                                    }
                                    else
                                    {
                                        hlpind2++;
                                    }
                                    if ( nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind2].xbild, nup.koorsoll[hlpind2].ybild, 
                                                            nup.koorsoll[hlpind1].xbild ,nup.koorsoll[hlpind1].ybild ) )
                                    {
                                        // Ausschalten
                                    }
                                    else
                                    {
                                        cont = FALSE;
                                    }
                                    if ( cont )
                                    {
                                        hlpind3 = hlpind2;
                                        if ( hlpind3 == 10 )
                                        {
                                            hlpind3 = 0;
                                        }
                                        else
                                        {
                                            hlpind3++;
                                        }
                                        if ( nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind3].xbild, nup.koorsoll[hlpind3].ybild, 
                                                                nup.koorsoll[hlpind2].xbild ,nup.koorsoll[hlpind2].ybild )
                                                                       &&   (nup.workbuf.nbtart != 3) )
                                        {
                                            // Ausschalten
                                            EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                                            // Nadeldreheinrichtung aus
                                            // Unterfadenschieber aus
                                            y7.resAuftrag();
                                            y32.resAuftrag();
                                        }
                                        else
                                        {
                                            cont = FALSE;
                                        }
                                    }
                                }
                            }
                            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                        }
                    }
                    */
                    /* EasyCODE_STRUCTURED_COMMENT_END */
                    if ( (nup.workbuf.flag & 0x01) == 0 )
                    {
                        if ( !y7.getZustand() && !y32.getZustand() )
                        {
                            /* EasyCODE ( 0 
                            Unterfadenschieber
                            ist ausgeschaltet,
                            Einschalten ? Die
                            Einschaltbedingung muss
                            fuer die naechsten 4 Stiche gelten + Vorhaltestiche. */
                            if ( (saf->md.nadelanzahl == 1) )
                            {
                                if ( nup.nadeldreh (CHKEIN, nup.koorsoll[nup.vorind].xbild, nup.koorsoll[nup.vorind].ybild, 
                                                        nup.workbuf.koorist.xbild ,nup.workbuf.koorist.ybild )  
                                                         ||    (saf->md.VorhStcheDreh > 0) )
                                {
                                    hlpind1 = nup.vorind;
                                    for ( i = 0; i <= saf->md.VorhStcheDreh; i++ )
                                    {
                                        hlpind0 = hlpind1;
                                        if ( hlpind1 == 10 )
                                        {
                                            hlpind1 = 0;
                                        }
                                        else
                                        {
                                            hlpind1++;
                                        }
                                        if ( saf->md.VorhStcheDreh == 1 )
                                        {
                                            // Bei 0 und 1 nur einmal durchlaufen
                                            break;
                                        }
                                    }
                                    if ( nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind1].xbild, nup.koorsoll[hlpind1].ybild, 
                                                            nup.koorsoll[hlpind0].xbild ,nup.koorsoll[hlpind0].ybild ) )
                                    {
                                        hlpind2 = hlpind1;
                                        if ( hlpind2 == 10 )
                                        {
                                            hlpind2 = 0;
                                        }
                                        else
                                        {
                                            hlpind2++;
                                        }
                                        if ( nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind2].xbild, nup.koorsoll[hlpind2].ybild, 
                                                                nup.koorsoll[hlpind1].xbild ,nup.koorsoll[hlpind1].ybild ) )
                                        {
                                            hlpind3 = hlpind2;
                                            if ( hlpind3 == 10 )
                                            {
                                                hlpind3 = 0;
                                            }
                                            else
                                            {
                                                hlpind3++;
                                            }
                                            if ( nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind3].xbild, nup.koorsoll[hlpind3].ybild, 
                                                                    nup.koorsoll[hlpind2].xbild ,nup.koorsoll[hlpind2].ybild)
                                                                     // &&   (nup.workbuf.nbtart != 3)
                                               )
                                            {
                                                hlpind4 = hlpind3;
                                                if ( hlpind4 == 10 )
                                                {
                                                    hlpind4 = 0;
                                                }
                                                else
                                                {
                                                    hlpind4++;
                                                }
                                                if ( (nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind4].xbild, nup.koorsoll[hlpind4].ybild, 
                                                                        nup.koorsoll[hlpind3].xbild ,nup.koorsoll[hlpind3].ybild) 
                                                                        ||  (saf->md.VorhStcheDreh == 0) )
                                                                                     &&   (nup.workbuf.nbtart != 3) )
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
                            }
                            /* EasyCODE ) */
                        }
                        else
                        {
                            /* EasyCODE ( 0 
                            Unterfadenschieber
                            ist eingeschaltet,
                            Ausschalten ? Die
                            Ausschaltbedingung muss
                            fuer die naechsten 4 Stiche gelten. */
                            cont = TRUE;
                            if ( nup.nadeldreh (CHKAUS, nup.koorsoll[nup.vorind].xbild, nup.koorsoll[nup.vorind].ybild, 
                                                    nup.workbuf.koorist.xbild ,nup.workbuf.koorist.ybild )  ||   (saf->md.VorhStcheDreh > 0) )
                            {
                                // Ausschalten
                            }
                            else
                            {
                                cont = FALSE;
                            }
                            if ( cont )
                            {
                                hlpind1 = nup.vorind;
                                for ( i = 0; i <= saf->md.VorhStcheDreh; i++ )
                                {
                                    hlpind0 = hlpind1;
                                    if ( hlpind1 == 10 )
                                    {
                                        hlpind1 = 0;
                                    }
                                    else
                                    {
                                        hlpind1++;
                                    }
                                    if ( saf->md.VorhStcheDreh == 1 )
                                    {
                                        // Bei 0 und 1 nur einmal durchlaufen
                                        break;
                                    }
                                }
                                if ( nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind1].xbild, nup.koorsoll[hlpind1].ybild, 
                                                        nup.koorsoll[hlpind0].xbild ,nup.koorsoll[hlpind0].ybild ) )
                                {
                                    // Ausschalten
                                }
                                else
                                {
                                    cont = FALSE;
                                }
                                if ( cont )
                                {
                                    hlpind2 = hlpind1;
                                    if ( hlpind2 == 10 )
                                    {
                                        hlpind2 = 0;
                                    }
                                    else
                                    {
                                        hlpind2++;
                                    }
                                    if ( nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind2].xbild, nup.koorsoll[hlpind2].ybild, 
                                                            nup.koorsoll[hlpind1].xbild ,nup.koorsoll[hlpind1].ybild ) )
                                    {
                                        // Ausschalten
                                    }
                                    else
                                    {
                                        cont = FALSE;
                                    }
                                    if ( cont )
                                    {
                                        hlpind3 = hlpind2;
                                        if ( hlpind3 == 10 )
                                        {
                                            hlpind3 = 0;
                                        }
                                        else
                                        {
                                            hlpind3++;
                                        }
                                        if ( nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind3].xbild, nup.koorsoll[hlpind3].ybild, 
                                                                nup.koorsoll[hlpind2].xbild ,nup.koorsoll[hlpind2].ybild )
                                                                      // &&   (nup.workbuf.nbtart != 3)
                                           )
                                        {
                                            // Ausschalten
                                        }
                                        else
                                        {
                                            cont = FALSE;
                                        }
                                        if ( cont )
                                        {
                                            hlpind4 = hlpind3;
                                            if ( hlpind4 == 10 )
                                            {
                                                hlpind4 = 0;
                                            }
                                            else
                                            {
                                                hlpind4++;
                                            }
                                            if ( (nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind4].xbild, nup.koorsoll[hlpind4].ybild, 
                                                                    nup.koorsoll[hlpind3].xbild ,nup.koorsoll[hlpind3].ybild ) 
                                                                            || (saf->md.VorhStcheDreh == 0) )
                                                                           &&   (nup.workbuf.nbtart != 3) )
                                            {
                                                // Ausschalten
                                                // Nadeldreheinrichtung aus
                                                // Unterfadenschieber aus
                                                y7.resAuftrag();
                                                y32.resAuftrag();
                                            }
                                        }
                                    }
                                }
                            }
                            /* EasyCODE ) */
                        }
                    }
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
                        Unterfadenschieber
                        ist ausgeschaltet,
                        Einschalten ? Die
                        Einschaltbedingung muss
                        fuer die naechsten 3 Stiche gelten. EasyCODE_COMMENT_END
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
                                    if ( nup.nadeldreh (CHKEIN, nup.koorsoll[nup.vorind].xbild, nup.koorsoll[nup.vorind].ybild, 
                                                            nup.workbuf.koorist.xbild ,nup.workbuf.koorist.ybild ) )
                                    {
                                        hlpind1 = nup.vorind;
                                        if ( hlpind1 == 10 )
                                        {
                                            hlpind1 = 0;
                                        }
                                        else
                                        {
                                            hlpind1++;
                                        }
                                        if ( nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind1].xbild, nup.koorsoll[hlpind1].ybild, 
                                                                nup.koorsoll[nup.vorind].xbild ,nup.koorsoll[nup.vorind].ybild ) )
                                        {
                                            hlpind2 = hlpind1;
                                            if ( hlpind2 == 10 )
                                            {
                                                hlpind2 = 0;
                                            }
                                            else
                                            {
                                                hlpind2++;
                                            }
                                            if ( nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind2].xbild, nup.koorsoll[hlpind2].ybild, 
                                                                    nup.koorsoll[hlpind1].xbild ,nup.koorsoll[hlpind1].ybild ) )
                                            {
                                                hlpind3 = hlpind2;
                                                if ( hlpind3 == 10 )
                                                {
                                                    hlpind3 = 0;
                                                }
                                                else
                                                {
                                                    hlpind3++;
                                                }
                                                if ( nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind3].xbild, nup.koorsoll[hlpind3].ybild, 
                                                                        nup.koorsoll[hlpind2].xbild ,nup.koorsoll[hlpind2].ybild )
                                                                              &&   (nup.workbuf.nbtart != 3) )
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
                            }
                        }
                        EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                    }
                    else
                    {
                        EasyCODE_COMMENT_START EasyCODE ( 0 
                        Unterfadenschieber
                        ist eingeschaltet,
                        Ausschalten ? Die
                        Ausschaltbedingung muss
                        fuer die naechsten 3 Stiche gelten. EasyCODE_COMMENT_END
                        if ( out1.getZustand() )
                        {
                        }
                        else
                        {
                            if ( (nup.workbuf.flag & 0x01) == 0 )
                            {
                                cont = TRUE;
                                if ( nup.nadeldreh (CHKAUS, nup.koorsoll[nup.vorind].xbild, nup.koorsoll[nup.vorind].ybild, 
                                                        nup.workbuf.koorist.xbild ,nup.workbuf.koorist.ybild ) )
                                {
                                    // Ausschalten
                                }
                                else
                                {
                                    cont = FALSE;
                                }
                                if ( cont )
                                {
                                    hlpind1 = nup.vorind;
                                    if ( hlpind1 == 10 )
                                    {
                                        hlpind1 = 0;
                                    }
                                    else
                                    {
                                        hlpind1++;
                                    }
                                    if ( nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind1].xbild, nup.koorsoll[hlpind1].ybild, 
                                                            nup.koorsoll[nup.vorind].xbild ,nup.koorsoll[nup.vorind].ybild ) )
                                    {
                                        // Ausschalten
                                    }
                                    else
                                    {
                                        cont = FALSE;
                                    }
                                    if ( cont )
                                    {
                                        hlpind2 = hlpind1;
                                        if ( hlpind2 == 10 )
                                        {
                                            hlpind2 = 0;
                                        }
                                        else
                                        {
                                            hlpind2++;
                                        }
                                        if ( nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind2].xbild, nup.koorsoll[hlpind2].ybild, 
                                                                nup.koorsoll[hlpind1].xbild ,nup.koorsoll[hlpind1].ybild ) )
                                        {
                                            // Ausschalten
                                        }
                                        else
                                        {
                                            cont = FALSE;
                                        }
                                        if ( cont )
                                        {
                                            hlpind3 = hlpind2;
                                            if ( hlpind3 == 10 )
                                            {
                                                hlpind3 = 0;
                                            }
                                            else
                                            {
                                                hlpind3++;
                                            }
                                            if ( nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind3].xbild, nup.koorsoll[hlpind3].ybild, 
                                                                    nup.koorsoll[hlpind2].xbild ,nup.koorsoll[hlpind2].ybild )
                                                                     &&   (nup.workbuf.nbtart != 3) )
                                            {
                                                // Ausschalten
                                                EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                                                // Nadeldreheinrichtung aus
                                                // Unterfadenschieber aus
                                                y7.resAuftrag();
                                                y32.resAuftrag();
                                            }
                                            else
                                            {
                                                cont = FALSE;
                                            }
                                        }
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
                        Unterfadenschieber
                        ist ausgeschaltet,
                        Einschalten ? Die
                        Einschaltbedingung muss
                        fuer die naechsten 3 Stiche gelten. */
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
                                    //
                                    if ( nup.nadeldreh (CHKEIN, nup.koorsoll[nup.vorind].xbild, nup.koorsoll[nup.vorind].ybild, 
                                                            nup.workbuf.koorist.xbild ,nup.workbuf.koorist.ybild )  
                                                             ||    (saf->md.VorhStcheDreh > 0) )
                                    {
                                        hlpind1 = nup.vorind;
                                        for ( i = 0; i <= saf->md.VorhStcheDreh; i++ )
                                        {
                                            hlpind0 = hlpind1;
                                            if ( hlpind1 == 10 )
                                            {
                                                hlpind1 = 0;
                                            }
                                            else
                                            {
                                                hlpind1++;
                                            }
                                            if ( saf->md.VorhStcheDreh == 1 )
                                            {
                                                // Bei 0 und 1 nur einmal durchlaufen
                                                break;
                                            }
                                        }
                                        if ( nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind1].xbild, nup.koorsoll[hlpind1].ybild, 
                                                                nup.koorsoll[hlpind0].xbild ,nup.koorsoll[hlpind0].ybild ) )
                                        {
                                            hlpind2 = hlpind1;
                                            if ( hlpind2 == 10 )
                                            {
                                                hlpind2 = 0;
                                            }
                                            else
                                            {
                                                hlpind2++;
                                            }
                                            if ( nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind2].xbild, nup.koorsoll[hlpind2].ybild, 
                                                                    nup.koorsoll[hlpind1].xbild ,nup.koorsoll[hlpind1].ybild ) )
                                            {
                                                hlpind3 = hlpind2;
                                                if ( hlpind3 == 10 )
                                                {
                                                    hlpind3 = 0;
                                                }
                                                else
                                                {
                                                    hlpind3++;
                                                }
                                                if ( nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind3].xbild, nup.koorsoll[hlpind3].ybild, 
                                                                        nup.koorsoll[hlpind2].xbild ,nup.koorsoll[hlpind2].ybild)
                                                                         // &&   (nup.workbuf.nbtart != 3)
                                                   )
                                                {
                                                    hlpind4 = hlpind3;
                                                    if ( hlpind4 == 10 )
                                                    {
                                                        hlpind4 = 0;
                                                    }
                                                    else
                                                    {
                                                        hlpind4++;
                                                    }
                                                    if ( (nup.nadeldreh (CHKEIN, nup.koorsoll[hlpind4].xbild, nup.koorsoll[hlpind4].ybild, 
                                                                            nup.koorsoll[hlpind3].xbild ,nup.koorsoll[hlpind3].ybild) 
                                                                            ||  (saf->md.VorhStcheDreh == 0) )
                                                                                         &&   (nup.workbuf.nbtart != 3) )
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
                                }
                            }
                        }
                        /* EasyCODE ) */
                    }
                    else
                    {
                        /* EasyCODE ( 0 
                        Unterfadenschieber
                        ist eingeschaltet,
                        Ausschalten ? Die
                        Ausschaltbedingung muss
                        fuer die naechsten 3 Stiche gelten. */
                        if ( out1.getZustand() )
                        {
                        }
                        else
                        {
                            if ( (nup.workbuf.flag & 0x01) == 0 )
                            {
                                cont = TRUE;
                                if ( nup.nadeldreh (CHKAUS, nup.koorsoll[nup.vorind].xbild, nup.koorsoll[nup.vorind].ybild, 
                                                        nup.workbuf.koorist.xbild ,nup.workbuf.koorist.ybild )  ||   (saf->md.VorhStcheDreh > 0) )
                                {
                                    // Ausschalten
                                }
                                else
                                {
                                    cont = FALSE;
                                }
                                if ( cont )
                                {
                                    hlpind1 = nup.vorind;
                                    for ( i = 0; i <= saf->md.VorhStcheDreh; i++ )
                                    {
                                        hlpind0 = hlpind1;
                                        if ( hlpind1 == 10 )
                                        {
                                            hlpind1 = 0;
                                        }
                                        else
                                        {
                                            hlpind1++;
                                        }
                                        if ( saf->md.VorhStcheDreh == 1 )
                                        {
                                            // Bei 0 und 1 nur einmal durchlaufen
                                            break;
                                        }
                                    }
                                    if ( nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind1].xbild, nup.koorsoll[hlpind1].ybild, 
                                                            nup.koorsoll[hlpind0].xbild ,nup.koorsoll[hlpind0].ybild ) )
                                    {
                                        // Ausschalten
                                    }
                                    else
                                    {
                                        cont = FALSE;
                                    }
                                    if ( cont )
                                    {
                                        hlpind2 = hlpind1;
                                        if ( hlpind2 == 10 )
                                        {
                                            hlpind2 = 0;
                                        }
                                        else
                                        {
                                            hlpind2++;
                                        }
                                        if ( nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind2].xbild, nup.koorsoll[hlpind2].ybild, 
                                                                nup.koorsoll[hlpind1].xbild ,nup.koorsoll[hlpind1].ybild ) )
                                        {
                                            // Ausschalten
                                        }
                                        else
                                        {
                                            cont = FALSE;
                                        }
                                        if ( cont )
                                        {
                                            hlpind3 = hlpind2;
                                            if ( hlpind3 == 10 )
                                            {
                                                hlpind3 = 0;
                                            }
                                            else
                                            {
                                                hlpind3++;
                                            }
                                            if ( nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind3].xbild, nup.koorsoll[hlpind3].ybild, 
                                                                    nup.koorsoll[hlpind2].xbild ,nup.koorsoll[hlpind2].ybild )
                                                                          // &&   (nup.workbuf.nbtart != 3)
                                               )
                                            {
                                                // Ausschalten
                                            }
                                            else
                                            {
                                                cont = FALSE;
                                            }
                                            if ( cont )
                                            {
                                                hlpind4 = hlpind3;
                                                if ( hlpind4 == 10 )
                                                {
                                                    hlpind4 = 0;
                                                }
                                                else
                                                {
                                                    hlpind4++;
                                                }
                                                if ( (nup.nadeldreh (CHKAUS, nup.koorsoll[hlpind4].xbild, nup.koorsoll[hlpind4].ybild, 
                                                                        nup.koorsoll[hlpind3].xbild ,nup.koorsoll[hlpind3].ybild ) 
                                                                                || (saf->md.VorhStcheDreh == 0) )
                                                                               &&   (nup.workbuf.nbtart != 3) )
                                                {
                                                    // Ausschalten
                                                    // Nadeldreheinrichtung aus
                                                    // Unterfadenschieber aus
                                                    y7.resAuftrag();
                                                    y32.resAuftrag();
                                                }
                                            }
                                        }
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
                    }
                    /* EasyCODE ) */
                    break;
                }
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            naechsten Stich vorbereiten */
            nup.workbuf.koorist = nup.koorsoll [nup.vorind];
            nup.nasere ();
            saf->buf = nup.workbuf;
            if ( saf->md.ufwen==1 )
            {
                if ( saf->bobcnt != 0xffff )
                {
                    saf->bobcnt ++;
                    //nae.anzboreq = TRUE;
                }
            }
            if ( nae.cutact )
            {
                /* EasyCODE ( 0 
                Schneidstich */
                // REF1 loeschen und freigeben
                outportb (INTCLR, REF1_SETCLR);
                outportb (INTCLR, REF1_RESCLR);
                outportb (INTMASK, REF1EI);
                /* EasyCODE ) */
            }
            else
            {
                if ( !nmtrb.nmstop )
                {
                    /* EasyCODE ( 0 
                    naechsten Stich */
                    nup.nacont90 ();
                    if ( koor.nae_err != 0 )
                    {
                        niserr ();
                    }
                    else
                    {
                        smx.status &= ~RICHTUNG;
                        if ( (nup.koorsoll [nup.vorind].motorx - nup.workbuf.koorist.motorx) < 0 )
                        {
                            smx.status |= SMXMINUS;
                        }
                        else
                        {
                            smx.status |= SMXPLUS;
                        }
                        smy.status &= ~RICHTUNG;
                        if ( (nup.koorsoll [nup.vorind].motory - nup.workbuf.koorist.motory) < 0 )
                        {
                            smy.status |= SMYMINUS;
                        }
                        else
                        {
                            smy.status |= SMYPLUS;
                        }
                        smx.rampadr = abs (nup.koorsoll [nup.vorind].motorx - nup.workbuf.koorist.motorx);
                        smy.rampadr = abs (nup.koorsoll [nup.vorind].motory - nup.workbuf.koorist.motory);
                        outportb (INTCLR, REF1_SETCLR);     // REF1 loeschen und freigeben
                        outportb (INTCLR, REF1_RESCLR);
                        outportb (INTMASK, REF1EI);
                    }
                    /* EasyCODE ) */
                }
            }
            /* EasyCODE ) */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNis::start */
/* EasyCODE F */
void KlasseNis::start (void)
{
    nmtrb.grundzustand ();
    if ( koor.nae_err == 0 )
    {
        if ( !smx.start_Rampe_Verz_Errorcheck () || !smy.start_Rampe_Verz_Errorcheck () )
        {
            koor.nae_err = 15;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNis::niserr */
/* EasyCODE F */
void KlasseNis::niserr (void)
{
    smx.stop ();       // SmX stoppen
    smy.stop ();       // SmY stoppen
    nminf.befehl = 8;  // Notstop Nm
    nmtrb.nmstop = TRUE;
    saf->carposok = FALSE;
    nae.homereq = TRUE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRef2::ref2task */
/* EasyCODE F */
void KlasseRef2::ref2task (void)
{
    y30.setAusgang();   // Fadenschneiden
    y31.resAuftrag();   // Unterfadenklemme zu
    y2u1.resAusgang();  // Fadenwedler in Grundstellung
    y2u2.resAusgang();
    y2fkt_drucklos.clearAuftrag();
    nmtrb.nmmodus = 6;
    nmtrb.grundzustand ();
    nminf.pos = nminf.nadel_pos();
}

/* EasyCODE ) */
/* EasyCODE ) */
