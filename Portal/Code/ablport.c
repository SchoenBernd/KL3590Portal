/* EasyCODE V8 */
/* EasyCODE ( 0 
ablport, Ablauf */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
// Datei:          ablport
// Klasse:         Kl3590 Portal
// Ersteller:      Halfmann   EWE 2856

#include "portal.h"

KlasseAblauf abl (tdini [AblIndex]);
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseAblauf::abltask */
/* EasyCODE F */
void KlasseAblauf::abltask (void)
{
    ablanz();
    nae.verw();
    smx.dynamic();
    smy.dynamic();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseAblauf::ablanz */
/* EasyCODE F */
void KlasseAblauf::ablanz (void)
{
    /* EasyCODE < */
    unsigned char helpstr1[100];
    unsigned char kommstr[11];
    unsigned int i;
    struct progpar parptr;
    /* EasyCODE > */
    if ( !abl_botsch1.busycheck() && !abl_botsch2.busycheck() &&
         !abl_botsch3.busycheck() && !abl_botsch4.busycheck() &&
         (koor.hbtart() == PRODUKTION) )
    {
        next_anzauftrag = 0;
        switch ( anzauftrag )
        {
        case 0 :
            break;
        case 1 :
            /* EasyCODE ( 0 
            anzauftrag = 1,  komplette Bildschirmseite Produktion aufbauen */
            abl_botsch1.senden (BV020);
            /* EasyCODE ( 0 
            Statuszeile,   abl_botsch1, varstr1 */
            if ( saf->md.jigctren == 2 )
            {
                /* EasyCODE ( 0 
                autom. Programmnummerwahl */
                sprintf (varstr1, prodstatpiktopnrs, PPAUTO);
                switch ( nup.read_jigpnr() )
                {
                case 0 :
                    // Programmnummer konnte 
                    // gelesen werden
                    /* EasyCODE ( 0 
                    Programmnummer */
                    sprintf (helpstr1, prodstatpnrs[1], saf->md.pnr);
                    strcat (varstr1, helpstr1);
                    /* EasyCODE ) */
                    /* EasyCODE ( 0 
                    Stichlaenge */
                    if ( di.parprog(saf->md.pnr, &parptr, DRIVE_C) == 2 )
                    {
                        if ( parptr.stichl != 0 )
                        {
                            sprintf (helpstr1, prodstatstls[1], (float)parptr.stichl/100);
                            strcat (varstr1, helpstr1);
                        }
                        else
                        {
                            sprintf (helpstr1, prodstatstls[0]);
                            strcat (varstr1, helpstr1);
                        }
                        /* EasyCODE ( 0 
                        Kommentar */
                        for ( i=0; i < 6; i++ )
                        {
                            kommstr[i] = parptr.kommentar[i];
                        }
                        kommstr[6] = 0;
                        sprintf(helpstr1, prodstatkomm1s, kommstr);
                        strcat (varstr1, helpstr1);
                        for ( i=0; i < 6; i++ )
                        {
                            kommstr[i] = parptr.kommentar[i+6];
                        }
                        kommstr[6] = 0;
                        sprintf(helpstr1, prodstatkomm2s, kommstr);
                        strcat (varstr1, helpstr1);
                        for ( i=0; i < 6; i++ )
                        {
                            kommstr[i] = parptr.kommentar[i+12];
                        }
                        kommstr[6] = 0;
                        sprintf(helpstr1, prodstatkomm3s, kommstr);
                        strcat (varstr1, helpstr1);
                        /* EasyCODE ) */
                    }
                    else
                    {
                        sprintf (helpstr1, prodstatstls[0]);
                        strcat (varstr1, helpstr1);
                    }
                    /* EasyCODE ) */
                    break;
                case 3 :
                    next_anzauftrag = 12;
                    break;
                default:
                    // Programmnummer konnte 
                    // nicht gelesen werden
                    /* EasyCODE ( 0 
                    Programmnummer = ---- */
                    sprintf (helpstr1, prodstatpnrs[0]);
                    strcat (varstr1, helpstr1);
                    /* EasyCODE ) */
                    /* EasyCODE ( 0 
                    Stichlaenge = --- */
                    sprintf (helpstr1, prodstatstls[0]);
                    strcat (varstr1, helpstr1);
                    /* EasyCODE ) */
                    break;
                }
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                gewaehlte Programmnummer */
                sprintf (varstr1, prodstatpiktopnrs, PPNR);
                switch ( saf->md.prsel )
                {
                case PNR :
                    /* EasyCODE ( 0 
                    PNR */
                    sprintf (helpstr1, prodstatpnrs[1], saf->md.pnr);
                    strcat (varstr1, helpstr1);
                    di.parprog(saf->md.pnr, &parptr, DRIVE_C);
                    break;
                    /* EasyCODE ) */
                case SEQ :
                    /* EasyCODE ( 0 
                    SEQ */
                    sprintf (helpstr1, prodstatpnrs[1], saf->md.seq_array[saf->md.seq][saf->md.pnrindex]);
                    strcat (varstr1, helpstr1);
                    sprintf (helpstr1, prodstatpiktokleins, PSEQKLEIN);
                    strcat (varstr1, helpstr1);
                    if ( saf->md.autoseq )
                    {
                        sprintf (helpstr1, piktoinv, PSEQKLEIN);
                        strcat (varstr1, helpstr1);
                    }
                    sprintf (helpstr1, prodstatseqvpnrs, saf->md.seq);
                    strcat (varstr1, helpstr1);
                    di.parprog(saf->md.seq_array[saf->md.seq][saf->md.pnrindex], &parptr, DRIVE_C);
                    break;
                    /* EasyCODE ) */
                default:
                    /* EasyCODE ( 0 
                    VPNR */
                    sprintf (helpstr1, prodstatpnrs[1], saf->md.vpnr_array[saf->md.vpnr][saf->md.pnrindex]);
                    strcat (varstr1, helpstr1);
                    sprintf (helpstr1, prodstatpiktokleins, PVPNRKLEIN);
                    strcat (varstr1, helpstr1);
                    sprintf (helpstr1, prodstatseqvpnrs, saf->md.vpnr);
                    strcat (varstr1, helpstr1);
                    di.parprog(saf->md.vpnr_array[saf->md.vpnr][saf->md.pnrindex], &parptr, DRIVE_C);
                    break;
                    /* EasyCODE ) */
                }
                /* EasyCODE ( 0 
                Stichlaenge */
                if ( parptr.stichl != 0 )
                {
                    sprintf (helpstr1, prodstatstls[1], (float)parptr.stichl/100);
                    strcat (varstr1, helpstr1);
                }
                else
                {
                    sprintf (helpstr1, prodstatstls[0]);
                    strcat (varstr1, helpstr1);
                }
                /* EasyCODE ) */
                if ( (saf->md.jigctren == 1) && (saf->jigctrf == BARCODE) )
                {
                    // SWb32 //SW005-1
                }
                else
                {
                    /* EasyCODE ( 0 
                    Kommentar */
                    for ( i=0; i < 6; i++ )
                    {
                        kommstr[i] = parptr.kommentar[i];
                    }
                    kommstr[6] = 0;
                    sprintf(helpstr1, prodstatkomm1s, kommstr);
                    strcat (varstr1, helpstr1);
                    for ( i=0; i < 6; i++ )
                    {
                        kommstr[i] = parptr.kommentar[i+6];
                    }
                    kommstr[6] = 0;
                    sprintf(helpstr1, prodstatkomm2s, kommstr);
                    strcat (varstr1, helpstr1);
                    for ( i=0; i < 6; i++ )
                    {
                        kommstr[i] = parptr.kommentar[i+12];
                    }
                    kommstr[6] = 0;
                    sprintf(helpstr1, prodstatkomm3s, kommstr);
                    strcat (varstr1, helpstr1);
                    /* EasyCODE ) */
                }
                /* EasyCODE ) */
            }
            /* EasyCODE ( 0 
            Drehzahl */
            sprintf (helpstr1, prodstatgsws, saf->md.maxgsw*100);
            strcat (varstr1, helpstr1);
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Barcode */
            if ( (saf->md.jigctren==1) && (saf->jigctrf==BARCODE) )
            {
                strcat (varstr1, bcpiktoklein);
                if ( schabl.bc.get_barcode() != 0xffff )
                {
                    sprintf(helpstr1, bctext1, schabl.bc.get_barcode());
                    strcat (varstr1, helpstr1);
                }
                else
                {
                    strcat (varstr1, bctext2);
                }
            }
            abl_botsch2.senden(varstr1);  //***WH
            varstr1[0] = 0;
            /* EasyCODE ) */
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Tastenblock und Text,   abl_botsch2, varstr1 */
            /* EasyCODE ( 0 
            Find */
            if ( !nup.carlp )
            {
                sprintf (helpstr1, prodtbfinds);
                strcat (varstr1, helpstr1);
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Autostart/ manueller Start */
            if ( saf->md.autoen )
            {
                sprintf (helpstr1, prodtbautomans, TAUTOS, PAUTO);
                strcat (varstr1, helpstr1);
                sprintf(helpstr1,touchinv ,TAUTOS);
            }
            else
            {
                sprintf (helpstr1, prodtbautomans, TMANS, PMAN);
                strcat (varstr1, helpstr1);
                sprintf(helpstr1,touchinv ,TMANS);
            }
            strcat (varstr1, helpstr1);
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Unterfadenzaehler */
            if ( nae.ofastoe )
            {
                strcat (varstr1, prodtbfastoes);
            }
            else
            {
                if ( nae.ufastoe )
                {
                    strcat (varstr1, prodtbspulenwechsels);
                }
                else
                {
                    if ( saf->md.ufwen == 1 )
                    {
                        sprintf (helpstr1, prodtbufcounts, saf->bobcnt, saf->md.bobcnt_wert);
                        strcat (varstr1, helpstr1);
                    }
                    /* EasyCODE ( 0 
                    aktuelle Huepferfusswerte */
                    sprintf (helpstr1, akthuepferwerte, (float)hfuss.akt_posunten.pos/10, (float)hfuss.akt_hub/10);
                    strcat (varstr1, helpstr1);
                    /* EasyCODE ) */
                }
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Uebertemperatur */
            if ( temp.getZustand() )
            {
                nae.uebertemp = FALSE;
            }
            else
            {
                nae.uebertemp = TRUE;
                strcat (varstr1, uebertemps);
            }
            /* EasyCODE ) */
            abl_botsch2.senden(varstr1);
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Sequenzen, verbundene Programme,   abl_botsch3, varstr3 */
            switch ( saf->md.prsel )
            {
            case 1 :
                for ( i=PNRINDEX_MIN, varstr3[0]=0; i<=PNRINDEX_MAX; i++ )
                {
                    if ( saf->md.seq_array[saf->md.seq][i]<=PNRMAX )
                    {
                        sprintf (helpstr1, pstats[i], saf->md.seq_array[saf->md.seq][i]);
                        strcat (varstr3, helpstr1);
                        if ( (i == saf->md.next_pnrindex)// && saf->caratsew &&
                             //(saf->md.autoseq || (saf->md.next_pnrindex != saf->md.pnrindex))
                           )
                        {
                            sprintf (helpstr1, touchinv, pstattasts[i]);
                            strcat (varstr3, helpstr1);
                        }
                    }
                }
                break;
            case 2 :
                for ( i=PNRINDEX_MIN, varstr3[0]=0; i<=PNRINDEX_MAX; i++ )
                {
                    if ( saf->md.vpnr_array[saf->md.vpnr][i]<=PNRMAX )
                    {
                        sprintf (helpstr1, pstats[i], saf->md.vpnr_array[saf->md.vpnr][i]);
                        strcat (varstr3, helpstr1);
                        if ( (i == saf->md.next_pnrindex)// && saf->caratsew
                           )
                        {
                            sprintf (helpstr1, touchinv, pstattasts[i]);
                            strcat (varstr3, helpstr1);
                        }
                    }
                }
                break;
            default:
                varstr3[0] = 0;
                break;
            }
            abl_botsch3.senden(varstr3);
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Tastenleiste,   abl_botsch4, varstr2 */
            sprintf (varstr2, starts);
            strcat (varstr2, homes);
            strcat (varstr2, fwds);
            strcat (varstr2, backs);
            
            
            /*
            
            sprintf (helpstr1, ESCS "=002070" "W1 " "%3u ", saf->md.Nadeldreh1ein);
            strcat (varstr2, helpstr1);
            sprintf (helpstr1, ESCS "=002084" "W2 " "%3u ", saf->md.Nadeldreh2ein);
            strcat (varstr2, helpstr1);
            
            sprintf (helpstr1, ESCS "=002098" "R1 " "%-3.6f ", saf->fadschein_winkel1);
            strcat (varstr2, helpstr1);
            sprintf (helpstr1, ESCS "=002112" "R2 " "%-3.6f ", saf->fadschein_winkel2);
            strcat (varstr2, helpstr1);
            
            */
            
            
            
            
            abl_botsch4.senden(varstr2);
            /* EasyCODE ) */
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            anzauftrag = 2,   Statuszeile anzeigen */
            if ( saf->md.jigctren == 2 )
            {
                /* EasyCODE ( 0 
                autom. Programmnummerwahl */
                if ( y51u1.fertig() && y51u2.fertig() )
                {
                    sprintf (varstr1, prodstatpiktopnrs, PPAUTO);
                    switch ( nup.read_jigpnr() )
                    {
                    case 0 :
                        // Programmnummer konnte 
                        // gelesen werden
                        /* EasyCODE ( 0 
                        Programmnummer */
                        sprintf (helpstr1, prodstatpnrs[1], saf->md.pnr);
                        strcat (varstr1, helpstr1);
                        /* EasyCODE ) */
                        /* EasyCODE ( 0 
                        Stichlaenge */
                        if ( di.parprog(saf->md.pnr, &parptr, DRIVE_C) == 2 )
                        {
                            if ( parptr.stichl != 0 )
                            {
                                sprintf (helpstr1, prodstatstls[1], (float)parptr.stichl/100);
                                strcat (varstr1, helpstr1);
                            }
                            else
                            {
                                sprintf (helpstr1, prodstatstls[0]);
                                strcat (varstr1, helpstr1);
                            }
                            /* EasyCODE ( 0 
                            Kommentar */
                            for ( i=0; i < 6; i++ )
                            {
                                kommstr[i] = parptr.kommentar[i];
                            }
                            kommstr[6] = 0;
                            sprintf(helpstr1, prodstatkomm1s, kommstr);
                            strcat (varstr1, helpstr1);
                            for ( i=0; i < 6; i++ )
                            {
                                kommstr[i] = parptr.kommentar[i+6];
                            }
                            kommstr[6] = 0;
                            sprintf(helpstr1, prodstatkomm2s, kommstr);
                            strcat (varstr1, helpstr1);
                            for ( i=0; i < 6; i++ )
                            {
                                kommstr[i] = parptr.kommentar[i+12];
                            }
                            kommstr[6] = 0;
                            sprintf(helpstr1, prodstatkomm3s, kommstr);
                            strcat (varstr1, helpstr1);
                            /* EasyCODE ) */
                        }
                        else
                        {
                            sprintf (helpstr1, prodstatstls[0]);
                            strcat (varstr1, helpstr1);
                        }
                        /* EasyCODE ) */
                        break;
                    case 3 :
                        next_anzauftrag = 12;
                        break;
                    default:
                        // Programmnummer konnte 
                        // nicht gelesen werden
                        /* EasyCODE ( 0 
                        Programmnummer = ---- */
                        sprintf (helpstr1, prodstatpnrs[0]);
                        strcat (varstr1, helpstr1);
                        /* EasyCODE ) */
                        /* EasyCODE ( 0 
                        Stichlaenge = --- */
                        sprintf (helpstr1, prodstatstls[0]);
                        strcat (varstr1, helpstr1);
                        /* EasyCODE ) */
                        break;
                    }
                }
                else
                {
                    return ;
                }
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                gewaehlte Programmnummer */
                sprintf (varstr1, prodstatpiktopnrs, PPNR);
                switch ( saf->md.prsel )
                {
                case PNR :
                    /* EasyCODE ( 0 
                    PNR */
                    sprintf (helpstr1, prodstatpnrs[1], saf->md.pnr);
                    strcat (varstr1, helpstr1);
                    di.parprog(saf->md.pnr, &parptr, DRIVE_C);
                    break;
                    /* EasyCODE ) */
                case SEQ :
                    /* EasyCODE ( 0 
                    SEQ */
                    sprintf (helpstr1, prodstatpnrs[1], saf->md.seq_array[saf->md.seq][saf->md.pnrindex]);
                    strcat (varstr1, helpstr1);
                    sprintf (helpstr1, prodstatpiktokleins, PSEQKLEIN);
                    strcat (varstr1, helpstr1);
                    if ( saf->md.autoseq )
                    {
                        sprintf (helpstr1, piktoinv, PSEQKLEIN);
                        strcat (varstr1, helpstr1);
                    }
                    sprintf (helpstr1, prodstatseqvpnrs, saf->md.seq);
                    strcat (varstr1, helpstr1);
                    di.parprog(saf->md.seq_array[saf->md.seq][saf->md.pnrindex], &parptr, DRIVE_C);
                    break;
                    /* EasyCODE ) */
                default:
                    /* EasyCODE ( 0 
                    VPNR */
                    sprintf (helpstr1, prodstatpnrs[1], saf->md.vpnr_array[saf->md.vpnr][saf->md.pnrindex]);
                    strcat (varstr1, helpstr1);
                    sprintf (helpstr1, prodstatpiktokleins, PVPNRKLEIN);
                    strcat (varstr1, helpstr1);
                    sprintf (helpstr1, prodstatseqvpnrs, saf->md.vpnr);
                    strcat (varstr1, helpstr1);
                    di.parprog(saf->md.vpnr_array[saf->md.vpnr][saf->md.pnrindex], &parptr, DRIVE_C);
                    break;
                    /* EasyCODE ) */
                }
                /* EasyCODE ( 0 
                Stichlaenge */
                if ( parptr.stichl != 0 )
                {
                    sprintf (helpstr1, prodstatstls[1], (float)parptr.stichl/100);
                    strcat (varstr1, helpstr1);
                }
                else
                {
                    sprintf (helpstr1, prodstatstls[0]);
                    strcat (varstr1, helpstr1);
                }
                /* EasyCODE ) */
                if ( (saf->md.jigctren == 1) && (saf->jigctrf == BARCODE) )
                {
                    // SWb32 //SW005-1
                }
                else
                {
                    /* EasyCODE ( 0 
                    Kommentar */
                    for ( i=0; i < 6; i++ )
                    {
                        kommstr[i] = parptr.kommentar[i];
                    }
                    kommstr[6] = 0;
                    sprintf(helpstr1, prodstatkomm1s, kommstr);
                    strcat (varstr1, helpstr1);
                    for ( i=0; i < 6; i++ )
                    {
                        kommstr[i] = parptr.kommentar[i+6];
                    }
                    kommstr[6] = 0;
                    sprintf(helpstr1, prodstatkomm2s, kommstr);
                    strcat (varstr1, helpstr1);
                    for ( i=0; i < 6; i++ )
                    {
                        kommstr[i] = parptr.kommentar[i+12];
                    }
                    kommstr[6] = 0;
                    sprintf(helpstr1, prodstatkomm3s, kommstr);
                    strcat (varstr1, helpstr1);
                    /* EasyCODE ) */
                }
                /* EasyCODE ) */
            }
            /* EasyCODE ( 0 
            Drehzahl */
            sprintf (helpstr1, prodstatgsws, saf->md.maxgsw*100);
            strcat (varstr1, helpstr1);
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Barcode */
            if ( (saf->md.jigctren==1) && (saf->jigctrf==BARCODE) )
            {
                strcat (varstr1, bcpiktoklein);
                if ( schabl.bc.get_barcode() != 0xffff )
                {
                    sprintf(helpstr1, bctext1, schabl.bc.get_barcode());
                    strcat (varstr1, helpstr1);
                }
                else
                {
                    strcat (varstr1, bctext2);
                }
            }
            /* EasyCODE ) */
            abl_botsch1.senden(varstr1);
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            anzauftrag = 3,   laufende Maschine */
            if ( (nae.key != 5) && (nae.key != 6) )
            {
                // kein Takten
                //nae.ufastoe = FALSE;
                nae.ofastoe = FALSE;
                nae.kofastoe = FALSE;   // SW003DIS
                
                abl_botsch2.senden (tastlaufend2);
            }
            else
            {
                abl_botsch2.senden (tastlaufend);
            }
            break;
            /* EasyCODE ) */
        case 4 :
            /* EasyCODE ( 0 
            anzauftrag = 4,   Abfrage Klammer einlegen */
            abl_botsch1.senden (BV021);
            break;
            /* EasyCODE ) */
        case 5 :
            /* EasyCODE ( 0 
            anzauftrag = 5,   Taste Auto/Man */
            sprintf (varstr1, prodtbautomandels);
            if ( saf->md.autoen )
            {
                sprintf (helpstr1, prodtbautomans, TAUTOS, PAUTO);
                strcat (varstr1, helpstr1);
                sprintf(helpstr1,touchinv ,TAUTOS);
            }
            else
            {
                sprintf (helpstr1, prodtbautomans, TMANS, PMAN);
                strcat (varstr1, helpstr1);
                sprintf(helpstr1,touchinv ,TMANS);
            }
            strcat (varstr1, helpstr1);
            abl_botsch1.senden (varstr1);
            break;
            /* EasyCODE ) */
        case 6 :
            /* EasyCODE ( 0 
            anzauftrag = 6,   Stationstasten anzeigen */
            switch ( saf->md.prsel )
            {
            case 1 :
                for ( i=PNRINDEX_MIN, varstr3[0]=0; i<=PNRINDEX_MAX; i++ )
                {
                    if ( saf->md.seq_array[saf->md.seq][i]<=PNRMAX )
                    {
                        sprintf (varstr2, touchdel, pstattasts[i]);
                        sprintf (helpstr1, pstats[i], saf->md.seq_array[saf->md.seq][i]);
                        strcat (varstr2, helpstr1);
                        if ( (i == saf->md.next_pnrindex)// && saf->caratsew &&
                             //(saf->md.autoseq || (saf->md.next_pnrindex != saf->md.pnrindex))
                           )
                        {
                            sprintf (helpstr1, touchinv, pstattasts[i]);
                            strcat (varstr2, helpstr1);
                        }
                        strcat (varstr3, varstr2);
                    }
                }
                break;
            case 2 :
                for ( i=PNRINDEX_MIN, varstr3[0]=0; i<=PNRINDEX_MAX; i++ )
                {
                    if ( saf->md.vpnr_array[saf->md.vpnr][i]<=PNRMAX )
                    {
                        sprintf (varstr2, touchdel, pstattasts[i]);
                        sprintf (helpstr1, pstats[i], saf->md.vpnr_array[saf->md.vpnr][i]);
                        strcat (varstr2, helpstr1);
                        if ( (i == saf->md.next_pnrindex)// && saf->caratsew
                           )
                        {
                            sprintf (helpstr1, touchinv, pstattasts[i]);
                            strcat (varstr2, helpstr1);
                        }
                        strcat (varstr3, varstr2);
                    }
                }
                break;
            default:
                varstr3[0] = 0;
                break;
            }
            abl_botsch3.senden(varstr3);
            break;
            /* EasyCODE ) */
        case 7 :
            /* EasyCODE ( 0 
            anzauftrag = 7,   Progr.nr, Stichlaenge und Stationstasten anzeigen */
            if ( keyb.taste() == 0xff )
            {
                if ( saf->md.prsel == SEQ )
                {
                    /* EasyCODE ( 0 
                    SEQ */
                    sprintf (varstr1, prodstatpnrs[1], saf->md.seq_array[saf->md.seq][saf->md.pnrindex]);
                    di.parprog(saf->md.seq_array[saf->md.seq][saf->md.pnrindex], &parptr, DRIVE_C);
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    VPNR */
                    sprintf (varstr1, prodstatpnrs[1], saf->md.vpnr_array[saf->md.vpnr][saf->md.pnrindex]);
                    di.parprog(saf->md.vpnr_array[saf->md.vpnr][saf->md.pnrindex], &parptr, DRIVE_C);
                    /* EasyCODE ) */
                }
                /* EasyCODE ( 0 
                Stichlaenge */
                if ( parptr.stichl != 0 )
                {
                    sprintf (helpstr1, prodstatstls[1], (float)parptr.stichl/100);
                    strcat (varstr1, helpstr1);
                }
                else
                {
                    sprintf (helpstr1, prodstatstls[0]);
                    strcat (varstr1, helpstr1);
                }
                /* EasyCODE ) */
                if ( (saf->md.jigctren == 1) && (saf->jigctrf == BARCODE) )
                {
                    // SWb32 //SW005-1
                }
                else
                {
                    /* EasyCODE ( 0 
                    Kommentar */
                    for ( i=0; i < 6; i++ )
                    {
                        kommstr[i] = parptr.kommentar[i];
                    }
                    kommstr[6] = 0;
                    sprintf(helpstr1, prodstatkomm1s, kommstr);
                    strcat (varstr1, helpstr1);
                    for ( i=0; i < 6; i++ )
                    {
                        kommstr[i] = parptr.kommentar[i+6];
                    }
                    kommstr[6] = 0;
                    sprintf(helpstr1, prodstatkomm2s, kommstr);
                    strcat (varstr1, helpstr1);
                    for ( i=0; i < 6; i++ )
                    {
                        kommstr[i] = parptr.kommentar[i+12];
                    }
                    kommstr[6] = 0;
                    sprintf(helpstr1, prodstatkomm3s, kommstr);
                    strcat (varstr1, helpstr1);
                    /* EasyCODE - */
                    abl_botsch1.senden(varstr1);
                    /* EasyCODE ) */
                }
                /* EasyCODE ( 0 
                Stationstasten */
                switch ( saf->md.prsel )
                {
                case 1 :
                    for ( i=PNRINDEX_MIN, varstr3[0]=0; i<=PNRINDEX_MAX; i++ )
                    {
                        if ( saf->md.seq_array[saf->md.seq][i]<=PNRMAX )
                        {
                            sprintf (varstr2, touchdel, pstattasts[i]);
                            sprintf (helpstr1, pstats[i], saf->md.seq_array[saf->md.seq][i]);
                            strcat (varstr2, helpstr1);
                            if ( (i == saf->md.next_pnrindex)// && saf->caratsew &&
                                 //(saf->md.autoseq || (saf->md.next_pnrindex != saf->md.pnrindex))
                               )
                            {
                                sprintf (helpstr1, touchinv, pstattasts[i]);
                                strcat (varstr2, helpstr1);
                            }
                            strcat (varstr3, varstr2);
                        }
                    }
                    break;
                case 2 :
                    for ( i=PNRINDEX_MIN, varstr3[0]=0; i<=PNRINDEX_MAX; i++ )
                    {
                        if ( saf->md.vpnr_array[saf->md.vpnr][i]<=PNRMAX )
                        {
                            sprintf (varstr2, touchdel, pstattasts[i]);
                            sprintf (helpstr1, pstats[i], saf->md.vpnr_array[saf->md.vpnr][i]);
                            strcat (varstr2, helpstr1);
                            if ( (i == saf->md.next_pnrindex)// && saf->caratsew
                               )
                            {
                                sprintf (helpstr1, touchinv, pstattasts[i]);
                                strcat (varstr2, helpstr1);
                            }
                            strcat (varstr3, varstr2);
                        }
                    }
                    break;
                default:
                    varstr3[0] = 0;
                    break;
                }
                abl_botsch3.senden(varstr3);
                /* EasyCODE ) */
            }
            else
            {
                next_anzauftrag = 7;
            }
            break;
            /* EasyCODE ) */
        case 8 :
            /* EasyCODE ( 0 
            anzauftrag = 8,   Pikto IN anzeigen */
            sprintf (varstr1, mbefs, PIN); 
            abl_botsch1.senden(varstr1);
            break;
            /* EasyCODE ) */
        case 9 :
            /* EasyCODE ( 0 
            anzauftrag = 9,   Pikto TIME anzeigen */
            sprintf (varstr1, mbefs, P_TIME); 
            abl_botsch1.senden(varstr1);
            break;
            /* EasyCODE ) */
        case 10 :
            /* EasyCODE ( 0 
            anzauftrag = 10,   Uebertemperatur */
            if ( temp.getZustand() )
            {
                nae.uebertemp = FALSE;
                sprintf (varstr1, pikdel, PTEMP); 
                abl_botsch1.senden(varstr1);
            }
            else
            {
                nae.uebertemp = TRUE;
                if ( nae.key != 3 )
                {
                    // Naehablauf nicht aktiv
                    abl_botsch1.senden(uebertemps);
                }
                else
                {
                    // Naehablauf aktiv
                    abl_botsch1.senden(uebertemp2s);
                }
            }
            break;
            /* EasyCODE ) */
        case 11 :
            /* EasyCODE ( 0 
            anzauftrag = 11,   Unterspannung */
            abl_botsch1.senden(unterspannungs);
            acanz = TRUE;
            break;
            /* EasyCODE ) */
        case 12 :
            /* EasyCODE ( 0 
            anzauftrag = 12,   Progr. nr. mit RFID */
            switch ( nup.read_jigpnr() )
            {
            case 0 :
                // Programmnummer konnte 
                // gelesen werden
                /* EasyCODE ( 0 
                Programmnummer */
                sprintf (varstr1, prodstatpnrs[1], saf->md.pnr);
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Stichlaenge */
                if ( di.parprog(saf->md.pnr, &parptr, DRIVE_C) == 2 )
                {
                    if ( parptr.stichl != 0 )
                    {
                        sprintf (helpstr1, prodstatstls[1], (float)parptr.stichl/100);
                        strcat (varstr1, helpstr1);
                    }
                    else
                    {
                        sprintf (helpstr1, prodstatstls[0]);
                        strcat (varstr1, helpstr1);
                    }
                }
                else
                {
                    sprintf (helpstr1, prodstatstls[0]);
                    strcat (varstr1, helpstr1);
                }
                /* EasyCODE ) */
                abl_botsch1.senden(varstr1);
                break;
            case 3 :
                next_anzauftrag = 12;
                break;
            default:
                // Programmnummer konnte 
                // nicht gelesen werden
                /* EasyCODE ( 0 
                Programmnummer = ---- */
                sprintf (helpstr1, prodstatpnrs[0]);
                strcat (varstr1, helpstr1);
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Stichlaenge = --- */
                sprintf (helpstr1, prodstatstls[0]);
                strcat (varstr1, helpstr1);
                /* EasyCODE ) */
                abl_botsch1.senden(varstr1);
                break;
            }
            break;
            /* EasyCODE ) */
        case 13 :
            /* EasyCODE ( 0 
            anzauftrag = 13,   Unterfadenzaehler */
            sprintf (varstr1, bobcounts, saf->bobcnt);
            abl_botsch1.senden(varstr1);
            nae.anzboreq = FALSE;
            break;
            /* EasyCODE ) */
        case 14 :
            /* EasyCODE ( 0 
            anzauftrag = 14,   Pikto IN loeschen */
            sprintf (varstr1, pikdel, PIN); 
            abl_botsch1.senden(varstr1);
            break;
            /* EasyCODE ) */
        case 15 :
            /* EasyCODE ( 0 
            anzauftrag = 15,   Pikto TIME loeschen */
            sprintf (varstr1, pikdel, P_TIME); 
            abl_botsch1.senden(varstr1);
            break;
            /* EasyCODE ) */
        case 16 :
            /* EasyCODE ( 0 
            anzauftrag = 16,   Tastenleiste, Taste Find 
                               und Huepferfusswerte aktualisieren */
            if ( !nup.carlp )
            {
                sprintf (varstr1, prodtbfinds);
                abl_botsch1.senden(varstr1);
            }
            abl_botsch2.senden(prodploeschs);
            //sprintf(varstr2, prodploeschs);
            sprintf (varstr2, starts);
            strcat (varstr2, homes);
            strcat (varstr2, fwds);
            strcat (varstr2, backs);
            sprintf (helpstr1, akthuepferwerte, (float)hfuss.akt_posunten.pos/10, (float)hfuss.akt_hub/10);
            strcat (varstr2, helpstr1);
            abl_botsch3.senden(varstr2);
            break;
            /* EasyCODE ) */
        case 17 :
            /* EasyCODE ( 0 
            anzauftrag = 17,   Huepferfusswerte aktualisieren */
            sprintf (varstr1, akthuepferwerte, (float)hfuss.akt_posunten.pos/10, (float)hfuss.akt_hub/10);
            abl_botsch2.senden(varstr1);
            break;
            /* EasyCODE ) */
        case 30 :
            /* EasyCODE ( 0 
            anzauftrag = 30,   Istdrehzahl zu Trace */
            sprintf (varstr1, ESCS "S12" ESCS "=002070" "IstDrehz: " "%4u" ESCS "S11", (nmtrb.gswanz [0] [2] * 100) );
            sprintf (helpstr1, ESCS "=002094" "W1 " "%3u ", saf->md.Nadeldreh1ein);
            strcat (varstr1, helpstr1);
            sprintf (helpstr1, ESCS "=002108" "W2 " "%3u ", saf->md.Nadeldreh2ein);
            strcat (varstr1, helpstr1);
            abl_botsch1.senden(varstr1);
            //resauftrag = TRUE;
            
            
            /*
            (nminf.get_drehzahl()+50) / 100; // Trace
            
            nmtrb.gswanz [0] [2] = (nminf.get_drehzahl()+50) / 100; // Trace
            unsigned char * prodistdrezahls = {ESCS "S12" ESCS "=001170" "Ausw:" "%4u" " Ist:" "%4u" " Feld:" "%2u" ESCS "S11"};  // nur bei Debug trace
            sprintf (helpstr1, ESCS "=002070" "W1 " "%3u ", saf->md.Nadeldreh1ein);
            strcat (varstr2, helpstr1);
            sprintf (helpstr1, ESCS "=002084" "W2 " "%3u ", saf->md.Nadeldreh2ein);
            strcat (varstr2, helpstr1);
            
            sprintf (helpstr1, ESCS "=002098" "R1 " "%-3.6f ", saf->fadschein_winkel1);
            strcat (varstr2, helpstr1);
            sprintf (helpstr1, ESCS "=002112" "R2 " "%-3.6f ", saf->fadschein_winkel2);
            strcat (varstr2, helpstr1);
            */
            break;
            /* EasyCODE ) */
        default:
            break;
        }
        anzauftrag = next_anzauftrag;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseAblauf::kinit */
/* EasyCODE F */
void KlasseAblauf::kinit (void)
{
    nae.kinit();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseAblauf::winit */
/* EasyCODE F */
void KlasseAblauf::winit (void)
{
    nae.winit();
}

/* EasyCODE ) */
/* EasyCODE ) */
