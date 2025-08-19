/* EasyCODE V8 */
/* EasyCODE ( 0 
cmdport */
/* EasyCODE ( 0 
Beschreibung */
/* Datei: cmdport.c       Klasse: 3590 Portal       Ersteller: Halfmann EWE 2856

In dieser Datei befinden sich die Programmverwaltung der Betriebsart Eingabe.

*/
/* EasyCODE ) */
#include "portal.h"
/* EasyCODE ( 0 
unsigned char KlasseProgverw::grundzustand(void) */
/* EasyCODE F */
unsigned char KlasseProgverw::grundzustand(void)
{
    /* EasyCODE < */
    unsigned char index, loop, loop1, loop2, tastmem;
    unsigned int i;
    struct dfree free;
    long avail;
    
    dirkey = 0;
    sprintf (arbeitsverzeichnis, "\\Portal\r");
    physdrive = DRIVE_SD;
    sourceSD();
    /* EasyCODE > */
    while ( TRUE )
    {
        /* EasyCODE ( 0 
        Bildschirmvorlage anzeigen */
        if ( taste_los() == 0xff )
        {
            return E_RUHE;
        }
        else
        {
            sprintf (eing.varstring, BV007);
            if ( physdrive == DRIVE_SD )
            {
                sprintf (eing.varstring1, laufwerk, PSDCARDKLEIN);
                strcat (eing.varstring, eing.varstring1);
                strcat (eing.varstring, lwwechsel2s);
            }
            else
            {
                strcat (eing.varstring, lans);
                strcat (eing.varstring, lwwechsel1s);
            }
        }
        strcat (eing.varstring, eing.varstring1);
        eing_botsch1.senden(eing.varstring);
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        mdportal erzeugen */
        creatmdat();
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Puffer der directories vorbelegen und laden */
        sdcarderror = 0;
        dirpnrload();
        dirfileload();
        /* EasyCODE ) */
        aktdrive = LINKS;
        loop2 = TRUE;
        while ( loop2 )
        {
            /* EasyCODE ( 0 
            Bildschirmvorlage anzeigen
            Path anzeigen
            ggf. Taste MDAT inv.
            Laufw. auf Flash setzen und inv. an */
            if ( taste_los() == 0xff )
            {
                return E_RUHE;
            }
            else
            {
                sprintf (eing.varstring, BV007);
                /* EasyCODE ( 0 
                SD-Karte */
                if ( physdrive == DRIVE_SD )
                {
                    sprintf (eing.varstring1, laufwerk, PSDCARDKLEIN);
                    strcat (eing.varstring, eing.varstring1);
                    strcat (eing.varstring, lwwechsel2s);
                }
                else
                {
                    strcat (eing.varstring, lans);
                    strcat (eing.varstring, lwwechsel1s);
                }
                eing_botsch1.senden(eing.varstring);
                switch ( sdcarderror )
                {
                case 0 :
                    break;
                case KEINEKARTE :
                    if ( (koor.bdf == PANEL) ||
                         (koor.bdf == PANELPC) )
                    {
                        sprintf(eing.varstring1, TEXTROT);
                        strcat (eing.varstring1, nocarderr);
                        strcat (eing.varstring1, TEXTSCHWARZ);
                    }
                    else
                    {
                        sprintf(eing.varstring1, nocarderr);
                    }
                    eing_botsch1.senden(eing.varstring1);
                    break;
                case FALSCHEKARTE :
                    if ( (koor.bdf == PANEL) ||
                         (koor.bdf == PANELPC) )
                    {
                        sprintf(eing.varstring1, TEXTROT);
                        strcat (eing.varstring1, wrongcarderr);
                        strcat (eing.varstring1, TEXTSCHWARZ);
                    }
                    else
                    {
                        sprintf(eing.varstring1, wrongcarderr);
                    }
                    eing_botsch1.senden(eing.varstring1);
                    break;
                case KARTESCHREIBSCHUTZ :
                    if ( (koor.bdf == PANEL) ||
                         (koor.bdf == PANELPC) )
                    {
                        sprintf(eing.varstring1, TEXTROT);
                        strcat (eing.varstring1, writeprotecterr);
                        strcat (eing.varstring1, TEXTSCHWARZ);
                    }
                    else
                    {
                        sprintf(eing.varstring1, writeprotecterr);
                    }
                    eing_botsch1.senden(eing.varstring1);
                    break;
                default:
                    if ( (koor.bdf == PANEL) ||
                         (koor.bdf == PANELPC) )
                    {
                        sprintf(eing.varstring1, TEXTROT);
                        sprintf (eing.varstring, sdcarderr, sdcarderror);
                        strcat (eing.varstring1, eing.varstring);
                        strcat (eing.varstring1, TEXTSCHWARZ);
                    }
                    else
                    {
                        sprintf (eing.varstring1, sdcarderr, sdcarderror);
                    }
                    eing_botsch1.senden(eing.varstring1);
                    break;
                }
                /* EasyCODE ) */
                switch ( dirkey )
                {
                case 1 :
                    sprintf(eing.varstring1,tinvs ,TMDAT);
                    strcat(eing.varstring1, drives[1]);
                    if ( physdrive == DRIVE_SD )
                    {
                        strcat(eing.varstring1, drives[2]);
                    }
                    else
                    {
                        strcat(eing.varstring1, drives[3]);
                    }
                    eing_botsch1.senden(eing.varstring1);
                    break;
                default:
                    sprintf(eing.varstring1,drives[0]);
                    if ( physdrive == DRIVE_SD )
                    {
                        strcat(eing.varstring1, drives[2]);
                    }
                    else
                    {
                        strcat(eing.varstring1, drives[3]);
                    }
                    eing_botsch1.senden(eing.varstring1);
                    break;
                }
                if ( aktdrive == LINKS )
                {
                    sprintf(eing.varstring,tinvs ,TSPEICHER);
                }
                else
                {
                    sprintf(eing.varstring,tinvs ,TDISKETTE);
                }
                eing_botsch1.senden(eing.varstring);
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            freie Bytes ermitteln und anzeigen */
            /* EasyCODE ( 0 
            freien Speicherplatz Flash ermitteln */
            getdfree(DRIVE_C,&free);
            while ( eing_botsch1.busycheck() )
            {
            }
            if ( free.df_sclus == 0xff )
            {
                eing_botsch1.senden(ESCS "=020188""Fehler");
            }
            else
            {
                avail = (long) free.df_avail
                            * (long) free.df_bsec
                            * (long) free.df_sclus;
                sprintf(eing.varstring1,ESCS "F001" ESCS "=005189"" %7.0f Bytes free" ESCS "F1",(float)avail);
                eing_botsch1.senden(eing.varstring1);
            }
            /* EasyCODE ) */
            while ( eing_botsch1.busycheck() )
            {
            }
            /* EasyCODE ) */
            loop = TRUE;
            while ( loop )
            {
                /* EasyCODE ( 0 
                Initial. fuer index etc. */
                for ( i = 0; i < 2; i++ )
                {
                    zindex[i] = 0;
                    dirindex[i] = 0;
                    anzreq[i] = TRUE;
                    anzkommreq[i] = TRUE;
                    block[i] = FALSE;
                    sterninv[i] = FALSE;
                }
                /* EasyCODE ) */
                key = 0;
                loop1 = TRUE;
                while ( loop1 )
                {
                    /* EasyCODE ( 0 
                    Dir anzeigen */
                    switch ( aktdrive )
                    {
                    case LINKS :
                        diraktanz(LINKS);
                        if ( sdcarderror == 0 )
                        {
                            diranz (RECHTS);
                        }
                        break;
                    case RECHTS :
                        diraktanz(RECHTS);
                        diranz(LINKS);
                    default:
                        break;
                    }
                    /* EasyCODE ) */
                    /* EasyCODE ( 0 
                    Tastenauswertung */
                    switch ( key )
                    {
                    case 0 :
                        /* EasyCODE ( 0 
                        Tastenauswertung */
                        switch ( getkey() )
                        {
                        case STOPANF :
                            return E_RUHE;
                        case TENDE :
                            return E_EXIT;
                        case TEINGABE :
                            return E_GST;
                        case TCOPY :
                            /* EasyCODE ( 0 
                            Kopieren */
                            if ( dirkey == 0 )
                            {
                                if ( dirindexmax[aktdrive] == 0 )
                                {
                                    eing_botsch1.senden(BELLBELL);
                                    break;
                                }
                            }
                            else
                            {
                                if ( fileindexmax[aktdrive] == 0 )
                                {
                                    eing_botsch1.senden(BELLBELL);
                                    break;
                                }
                            }
                            if ( taste_los() == 0xff )
                            {
                                return E_RUHE;
                            }
                            else
                            {
                                /* EasyCODE ( 0 
                                SD-Card pruefen und pnr directory laden */
                                if ( sdcardcheck() )
                                {
                                    // Richtige SD-Karte gesteckt
                                    eing_botsch1.senden(dialogfenster);
                                    eing_botsch1.senden(dialogstatus2);
                                    if ( pnrdir[RECHTS][0] == 0xffff )
                                    {
                                        sddirload();
                                    }
                                    if ( filedir[RECHTS][0] == 0xff )
                                    {
                                        sddirfileload();
                                    }
                                    copyall = FALSE;
                                    sdcopy();
                                }
                                else
                                {
                                    aktdrive = LINKS;
                                }
                                /* EasyCODE ) */
                                loop = FALSE;
                                loop1 = FALSE;
                            }
                            /* EasyCODE ) */
                            break;
                        case TDEL :
                            /* EasyCODE ( 0 
                            Loeschen */
                            if ( dirkey == 0 )
                            {
                                if ( dirindexmax[aktdrive] == 0 )
                                {
                                    eing_botsch1.senden(BELLBELL);
                                    break;
                                }
                            }
                            else
                            {
                                if ( aktdrive == LINKS )
                                {
                                    eing_botsch1.senden(BELLBELL);
                                    break;
                                }
                                else
                                {
                                    if ( fileindexmax[aktdrive] == 0 )
                                    {
                                        eing_botsch1.senden(BELLBELL);
                                        break;
                                    }
                                }
                            }
                            if ( taste_los() == 0xff )
                            {
                                return E_RUHE;
                            }
                            else
                            {
                                /* EasyCODE ( 0 
                                File loeschen */
                                if ( aktdrive == RECHTS )
                                {
                                    /* EasyCODE ( 0 
                                    SD-Card */
                                    if ( sdcardcheck() )
                                    {
                                        /* EasyCODE < */
                                        // Richtige SD-Karte gesteckt
                                        /* EasyCODE > */
                                        if ( pnrdir[RECHTS][0] == 0xffff )
                                        {
                                            sddirload();
                                        }
                                        if ( filedir[RECHTS][0] == 0xff )
                                        {
                                            sddirfileload();
                                        }
                                        del();
                                    }
                                    else
                                    {
                                        aktdrive = LINKS;
                                    }
                                    /* EasyCODE ) */
                                }
                                else
                                {
                                    del();
                                }
                                /* EasyCODE ) */
                                loop = FALSE;
                                loop1 = FALSE;
                            }
                            /* EasyCODE ) */
                            break;
                        case TSCRUP :
                            /* EasyCODE ( 0 
                            FlashFile */
                            if ( aktdrive == LINKS )
                            {
                                scrup(LINKS);
                                anzkommreq[LINKS] = TRUE;
                                if ( dirkey == 0 )
                                {
                                    teing1.rsz();
                                    teing1.sz(saf->slowdelay);  // Nach dem ersten Tastendruck warten
                                    key = 1;
                                }
                            }
                            /* EasyCODE ) */
                            break;
                        case TSCRDOWN :
                            /* EasyCODE ( 0 
                            FlashFile */
                            if ( aktdrive == LINKS )
                            {
                                scrdown(LINKS);
                                anzkommreq[LINKS] = TRUE;
                                if ( dirkey == 0 )
                                {
                                    teing1.rsz();
                                    teing1.sz(saf->slowdelay);  // Nach dem ersten Tastendruck warten
                                    key = 1;
                                }
                            }
                            /* EasyCODE ) */
                            break;
                        case TSTERN :
                            /* EasyCODE ( 0 
                            FlashFile Markierung */
                            if ( aktdrive == LINKS )
                            {
                                if ( dirkey == 0 )
                                {
                                    if ( taste_los() == 0xff )
                                    {
                                        return E_RUHE;
                                    }
                                    else
                                    {
                                        if ( block[LINKS] )
                                        {
                                            anzreq[LINKS] = TRUE;
                                            anzkommreq[LINKS] = TRUE;
                                            block[LINKS] = FALSE;
                                            sterninv[LINKS] = FALSE;
                                        }
                                        else
                                        {
                                            block[LINKS] = TRUE;
                                            sterninv[LINKS] = TRUE;
                                        }
                                        sprintf(eing.varstring,tinvs ,TSTERN);
                                        eing_botsch1.senden(eing.varstring);
                                    }
                                }
                            }
                            /* EasyCODE ) */
                            break;
                        case TSCRUP1 :
                            /* EasyCODE ( 0 
                            DiskFile */
                            if ( aktdrive == RECHTS )
                            {
                                if ( dirkey == 0 )
                                {
                                    tastmem = TSCRUP1;
                                    teing1.rsz();
                                    teing1.sz(saf->slowdelay);  // Nach dem ersten Tastendruck warten
                                    key = 1;
                                }
                                else
                                {
                                    scrup(RECHTS);
                                }
                            }
                            else
                            {
                                eing_botsch1.senden(BELLBELL);
                            }
                            /* EasyCODE ) */
                            break;
                        case TSCRDOWN1 :
                            /* EasyCODE ( 0 
                            Diskfile */
                            if ( aktdrive == RECHTS )
                            {
                                if ( dirkey == 0 )
                                {
                                    tastmem = TSCRDOWN1;
                                    teing1.rsz();
                                    teing1.sz(saf->slowdelay);  // Nach dem ersten Tastendruck warten
                                    key = 1;
                                }
                                else
                                {
                                    scrdown(RECHTS);
                                }
                            }
                            else
                            {
                                eing_botsch1.senden(BELLBELL);
                            }
                            /* EasyCODE ) */
                            break;
                        case TSTERN1 :
                            /* EasyCODE ( 0 
                            DiskFile Markierung */
                            if ( aktdrive == RECHTS )
                            {
                                if ( dirkey == 0 )
                                {
                                    if ( taste_los() == 0xff )
                                    {
                                        return E_RUHE;
                                    }
                                    else
                                    {
                                        if ( block[RECHTS] )
                                        {
                                            anzreq[RECHTS] = TRUE;
                                            anzkommreq[RECHTS] = TRUE;
                                            block[RECHTS] = FALSE;
                                            sterninv[RECHTS] = FALSE;
                                        }
                                        else
                                        {
                                            block[RECHTS] = TRUE;
                                            sterninv[RECHTS] = TRUE;
                                        }
                                        sprintf(eing.varstring,tinvs ,TSTERN1);
                                        eing_botsch1.senden(eing.varstring);
                                    }
                                }
                            }
                            else
                            {
                                eing_botsch1.senden(BELLBELL);
                            }
                            /* EasyCODE ) */
                            break;
                        case TSPEICHER :
                            /* EasyCODE ( 0 
                            neu lesen */
                            taste_los();
                            if ( aktdrive == RECHTS )
                            {
                                if ( sdcardcheck() )
                                {
                                    if ( pnrdir[RECHTS][0] == 0xffff )
                                    {
                                        sddirload();
                                    }
                                    if ( filedir[RECHTS][0] == 0xff )
                                    {
                                        sddirfileload();
                                    }
                                }
                                aktdrive = LINKS;
                                /* EasyCODE - */
                                loop = FALSE;
                                loop1 = FALSE;
                            }
                            /* EasyCODE ) */
                            break;
                        case TDISKETTE :
                            /* EasyCODE ( 0 
                            Diskette */
                            taste_los();
                            if ( aktdrive == LINKS )
                            {
                                /* EasyCODE ( 0 
                                aktives Laufwerk umschalten auf SD-Karte/LAN */
                                if ( sdcardcheck() )
                                {
                                    if ( pnrdir[RECHTS][0] == 0xffff )
                                    {
                                        sddirload();
                                    }
                                    if ( filedir[RECHTS][0] == 0xff )
                                    {
                                        sddirfileload();
                                    }
                                    aktdrive = RECHTS;
                                }
                                else
                                {
                                    eing_botsch1.senden(BELLBELL);
                                }
                                /* EasyCODE ) */
                                loop = FALSE;
                                loop1 = FALSE;
                            }
                            /* EasyCODE ) */
                            break;
                        case TMDAT :
                            /* EasyCODE ( 0 
                            Dir Umschaltung */
                            taste_los();
                            if ( sdcardcheck() )
                            {
                                /* EasyCODE < */
                                // Richtige SD-Karte gesteckt
                                /* EasyCODE > */
                                if ( pnrdir[RECHTS][0] == 0xffff )
                                {
                                    sddirload();
                                }
                                if ( filedir[RECHTS][0] == 0xff )
                                {
                                    sddirfileload();
                                }
                            }
                            else
                            {
                                aktdrive = LINKS;
                            }
                            if ( dirkey == 0 )
                            {
                                dirkey = 1;
                            }
                            else
                            {
                                dirkey = 0;
                            }
                            loop = FALSE;
                            loop1 = FALSE;
                            /* EasyCODE ) */
                            break;
                        case TNEULESEN :
                            /* EasyCODE ( 0 
                            neu laden */
                            loop = FALSE;
                            loop1 = FALSE;
                            loop2 = FALSE;
                            /* EasyCODE ) */
                            break;
                        case TLWWECHSEL :
                            /* EasyCODE ( 0 
                            rechtes Laufwerk wechseln,
                            SD <--> LAN */
                            taste_los();
                            if ( physdrive == DRIVE_SD )
                            {
                                if ( sourceNet() == 0 )
                                {
                                    physdrive = DRIVE_LAN;
                                }
                                else
                                {
                                    koor.eing_err = 36;
                                    error_res();
                                }
                            }
                            else
                            {
                                if ( sourceSD() == 0 )
                                {
                                    physdrive = DRIVE_SD;
                                }
                                else
                                {
                                    koor.eing_err = 37;
                                    error_res();
                                }
                            }
                            /* EasyCODE ( 0 
                            Dir-Puffer loeschen */
                            for ( i = 0; i <= MAX_PROGANZAHL; i++ )
                            {
                                pnrdir[RECHTS][i] = 0xffff;
                            }
                            /* EasyCODE ) */
                            /* EasyCODE ( 0 
                            fileDir-Puffer loeschen */
                            for ( i = 0; i <DIRZ_MAX; i++ )
                            {
                                filedir[RECHTS][i] = 0xff;
                            }
                            /* EasyCODE ) */
                            if ( sdcardcheck() )
                            {
                                sddirload();
                                /* EasyCODE - */
                                sddirfileload();
                            }
                            else
                            {
                                loop2 = FALSE;
                            }
                            loop = FALSE;
                            loop1 = FALSE;
                            /* EasyCODE ) */
                            break;
                        case TFORMAT :
                            /* EasyCODE ( 0 
                            Diskette/SD-Karte formatieren */
                            if ( taste_los() == 0xff )
                            {
                                return E_RUHE;
                            }
                            loop = FALSE;
                            loop1 = FALSE;
                            switch ( sdcardformat (arbeitsverzeichnis) )
                            {
                            case 0 // ok
                            :
                                loop2 = FALSE;
                                break;
                            case 2 :
                                // Karte nicht gesteckt
                                sdcarderror = KEINEKARTE;
                                /* EasyCODE ( 0 
                                Dir-Puffer loeschen */
                                for ( i = 0; i <= MAX_PROGANZAHL; i++ )
                                {
                                    pnrdir[RECHTS][i] = 0xffff;
                                }
                                /* EasyCODE ) */
                                /* EasyCODE ( 0 
                                fileDir-Puffer loeschen */
                                for ( i = 0; i <DIRZ_MAX; i++ )
                                {
                                    filedir[RECHTS][i] = 0xff;
                                }
                                /* EasyCODE ) */
                                break;
                            case 3 :
                                // Karte nicht richtig gesteckt
                                sdcarderror = KARTENSTECKFEHLER;
                                /* EasyCODE ( 0 
                                Dir-Puffer loeschen */
                                for ( i = 0; i <= MAX_PROGANZAHL; i++ )
                                {
                                    pnrdir[RECHTS][i] = 0xffff;
                                }
                                /* EasyCODE ) */
                                /* EasyCODE ( 0 
                                fileDir-Puffer loeschen */
                                for ( i = 0; i <DIRZ_MAX; i++ )
                                {
                                    filedir[RECHTS][i] = 0xff;
                                }
                                /* EasyCODE ) */
                                break;
                            case 4 :
                                // Karte schreibgeschuetzt
                                sdcarderror = KARTESCHREIBSCHUTZ;
                                break;
                            case 6 :
                                /* EasyCODE < */
                                // Formatieren abgebrochen
                                /* EasyCODE > */
                                break;
                            default:
                                // Fehler beim Formatieren
                                sdcarderror = FORMATKARTE;
                                break;
                            }
                            /* EasyCODE ) */
                            break;
                        default:
                            eing_botsch1.senden(BELLBELL);
                            break;
                        }
                        /* EasyCODE ) */
                        break;
                    case 1 :
                        /* EasyCODE ( 0 
                        SCR-Taste gehalten, slow delay */
                        if ( koor.eingstop() )
                        {
                            return E_RUHE;
                        }
                        else
                        {
                            switch ( keyb.taste() )
                            {
                            case TSCRUP :
                                if ( teing1.tz() )
                                {
                                    // Slowdelay abgelaufen
                                    teing1.rsz();
                                    teing1.sz(saf->fastdelay);
                                    scrup(LINKS);
                                    teing2.rsz();
                                    teing2.sz(saf->slowtime);
                                    key = 2;
                                }
                                break;
                            case TSCRDOWN :
                                if ( teing1.tz() )
                                {
                                    // Slowdelay abgelaufen
                                    teing1.rsz();
                                    teing1.sz(saf->fastdelay);
                                    scrdown(LINKS);
                                    teing2.rsz();
                                    teing2.sz(saf->slowtime);
                                    key = 2;
                                }
                                break;
                            case TSCRUP1 :
                                if ( teing1.tz() )
                                {
                                    // Slowdelay abgelaufen
                                    teing1.rsz();
                                    teing1.sz(saf->fastdelay);
                                    scrup(RECHTS);
                                    teing2.rsz();
                                    teing2.sz(saf->slowtime);
                                    key = 2;
                                }
                                break;
                            case TSCRDOWN1 :
                                if ( teing1.tz() )
                                {
                                    // Slowdelay abgelaufen
                                    teing1.rsz();
                                    teing1.sz(saf->fastdelay);
                                    scrdown(RECHTS);
                                    teing2.rsz();
                                    teing2.sz(saf->slowtime);
                                    key = 2;
                                }
                                break;
                            default:
                                // Taste wurde los gelassen
                                if ( aktdrive == RECHTS )
                                {
                                    if ( tastmem == TSCRUP1 )
                                    {
                                        scrup(RECHTS);
                                    }
                                    else
                                    {
                                        scrdown(RECHTS);
                                    }
                                    /* EasyCODE_STRUCTURED_COMMENT_START */
                                    /*
                                    anzkommreq[RECHTS] = TRUE;
                                    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
                                    */
                                    /* EasyCODE_STRUCTURED_COMMENT_END */
                                    key = 4;
                                }
                                else
                                {
                                    key = 0;
                                }
                                break;
                            }
                        }
                        /* EasyCODE ) */
                        break;
                    case 2 :
                        /* EasyCODE ( 0 
                        SCR-Taste gehalten, langsam Rollen und fast delay */
                        if ( koor.eingstop() )
                        {
                            return E_RUHE;
                        }
                        switch ( keyb.taste() )
                        {
                        case TSCRUP :
                            if ( teing2.tz() )
                            {
                                if ( teing1.tz() )
                                {
                                    // schnell Rollen
                                    key = 3;
                                }
                                else
                                {
                                    scrup(LINKS);
                                    /* EasyCODE - */
                                    teing2.rsz();
                                    teing2.sz(saf->slowtime);
                                }
                            }
                            break;
                        case TSCRDOWN :
                            if ( teing2.tz() )
                            {
                                if ( teing1.tz() )
                                {
                                    // schnell Rollen
                                    key = 3;
                                }
                                else
                                {
                                    scrdown(LINKS);
                                    /* EasyCODE - */
                                    teing2.rsz();
                                    teing2.sz(saf->slowtime);
                                }
                            }
                            break;
                        case TSCRUP1 :
                            if ( teing2.tz() )
                            {
                                if ( teing1.tz() )
                                {
                                    // schnell Rollen
                                    key = 3;
                                }
                                else
                                {
                                    scrup(RECHTS);
                                    teing2.rsz();
                                    teing2.sz(saf->slowtime);
                                }
                            }
                            break;
                        case TSCRDOWN1 :
                            if ( teing2.tz() )
                            {
                                if ( teing1.tz() )
                                {
                                    // schnell Rollen
                                    key = 3;
                                }
                                else
                                {
                                    scrdown(RECHTS);
                                    teing2.rsz();
                                    teing2.sz(saf->slowtime);
                                }
                            }
                            break;
                        default:
                            key = 0; // Taste wurde losgelassen
                            anzreq [aktdrive] = TRUE;
                            anzkommreq [aktdrive] = TRUE;
                            teing2.rsz();
                            teing2.sz(50);
                            while ( !teing2.tz() )
                            {
                                if ( keyb.taste() != 0xff )
                                {
                                    teing2.rsz();
                                    teing2.sz(50);
                                }
                            }
                            break;
                        }
                        /* EasyCODE ) */
                        break;
                    case 3 :
                        /* EasyCODE ( 0 
                        SCR-Taste gehalten, schnell Rollen */
                        if ( koor.eingstop() )
                        {
                            return E_RUHE;
                        }
                        switch ( keyb.taste() )
                        {
                        case TSCRUP :
                            if ( teing2.tz() && !eing_botsch1.busycheck() )
                            {
                                scrup(LINKS);
                                scrup(LINKS);
                                scrup(LINKS);
                                scrup(LINKS);
                                scrup(LINKS);
                                /* EasyCODE - */
                                teing2.rsz();
                                teing2.sz(20);
                            }
                            break;
                        case TSCRDOWN :
                            if ( teing2.tz() && !eing_botsch1.busycheck() )
                            {
                                scrdown(LINKS);
                                scrdown(LINKS);
                                scrdown(LINKS);
                                scrdown(LINKS);
                                scrdown(LINKS);
                                /* EasyCODE - */
                                teing2.rsz();
                                teing2.sz(20);
                            }
                            break;
                        case TSCRUP1 :
                            if ( teing2.tz() && !eing_botsch1.busycheck() )
                            {
                                scrup(RECHTS);
                                scrup(RECHTS);
                                scrup(RECHTS);
                                scrup(RECHTS);
                                scrup(RECHTS);
                                /* EasyCODE - */
                                teing2.rsz();
                                teing2.sz(20);
                            }
                            break;
                        case TSCRDOWN1 :
                            if ( teing2.tz() && !eing_botsch1.busycheck() )
                            {
                                scrdown(RECHTS);
                                scrdown(RECHTS);
                                scrdown(RECHTS);
                                scrdown(RECHTS);
                                scrdown(RECHTS);
                                /* EasyCODE - */
                                teing2.rsz();
                                teing2.sz(20);
                            }
                            break;
                        default:
                            key = 0; // Taste wurde losgelassen
                            anzreq [aktdrive] = TRUE;
                            anzkommreq [aktdrive] = TRUE;
                            teing2.rsz();
                            teing2.sz(50);
                            while ( !teing2.tz() )
                            {
                                if ( keyb.taste() != 0xff )
                                {
                                    teing2.rsz();
                                    teing2.sz(50);
                                }
                            }
                            break;
                        }
                        /* EasyCODE ) */
                        break;
                    case 4 :
                        /* EasyCODE ( 0 
                        Taste los gelassen waehrend slow delay */
                        teing2.rsz();
                        teing2.sz(100);
                        tastmem = 0xff;
                        while ( !teing2.tz() && (tastmem == 0xff) )
                        {
                            switch ( keyb.taste() )
                            {
                            case TSCRUP1 :
                                tastmem = TSCRUP1;
                                teing1.rsz();
                                teing1.sz(saf->slowdelay);  // Nach dem ersten Tastendruck warten
                                key = 1;
                                break;
                            case TSCRDOWN1 :
                                tastmem = TSCRDOWN1;
                                teing1.rsz();
                                teing1.sz(saf->slowdelay);  // Nach dem ersten Tastendruck warten
                                key = 1;
                                break;
                            default:
                                break;
                            }
                        }
                        if ( tastmem == 0xff )
                        {
                            anzkommreq[RECHTS] = TRUE;
                            anzreq[RECHTS] = TRUE;
                            teing2.rsz();
                            teing2.sz(100);
                            key = 5;
                        }
                        /* EasyCODE ) */
                        break;
                    case 5 :
                        if ( teing2.tz() && (keyb.taste() == 0xff) )
                        {
                            key = 0;
                        }
                        break;
                    default:
                        key = 0;
                        break;
                    }
                    /* EasyCODE ) */
                }
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
scrup, srcdown */
/* EasyCODE ( 0 
void KlasseProgverw::scrup(unsigned char drive) */
/* EasyCODE F */
void KlasseProgverw::scrup(unsigned char drive)
{
    if ( zindex[drive] > 0 )
    {
        zindex[drive] = zindex[drive]-1;
    }
    else
    {
        if ( block[drive] && (dirindex[drive] > 0) && (dirindexblock[drive] >= dirindex[drive]) )
        {
            if ( zindexblock[drive] < DIRZ_MAX-1 )
            {
                zindexblock[drive] = zindexblock[drive]+1;
            }
        }
    }
    if ( dirindex[drive] > 0 )
    {
        dirindex[drive] = dirindex[drive]-1;
        anzreq[drive] = TRUE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::scrdown(unsigned char drive) */
/* EasyCODE F */
void KlasseProgverw::scrdown(unsigned char drive)
{
    switch ( dirkey )
    {
    case 1 :
        if ( (zindex[drive] < DIRZ_MAX-1) && ((zindex[drive] +1) < fileindexmax[drive]) )
        {
            zindex[drive] = zindex[drive]+1;
        }
        else
        {
            if ( block[drive] && (dirindex[drive] < fileindexmax[drive]) && (dirindexblock[drive] <= dirindex[drive]) )
            {
                if ( zindexblock[drive] > 0 )
                {
                    zindexblock[drive] = zindexblock[drive]-1;
                }
            }
        }
        if ( filedir[drive][dirindex[drive]+1] < 100 )
        {
            dirindex[drive] = dirindex[drive]+1;
            anzreq[drive] = TRUE;
        }
        break;
    default:
        if ( (zindex[drive] < DIRZ_MAX-1)  && ((zindex[drive] +1) < dirindexmax[drive]) )
        {
            zindex[drive] = zindex[drive]+1;
        }
        else
        {
            if ( block[drive] && (dirindex[drive] < dirindexmax[drive]) && (dirindexblock[drive] <= dirindex[drive]) )
            {
                if ( zindexblock[drive] > 0 )
                {
                    zindexblock[drive] = zindexblock[drive]-1;
                }
            }
        }
        if ( (pnrdir[drive][dirindex[drive]+1] <= PNRMAX) &&
             (dirindex[drive] < MAX_PROGANZAHL) )
        {
            dirindex[drive] = dirindex[drive]+1;
            anzreq[drive] = TRUE;
        }
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::dirpnrload(void) */
/* EasyCODE F */
void KlasseProgverw::dirpnrload(void)
{
    unsigned int i, j, k;
    /* EasyCODE - */
    eing_botsch1.senden(dialogfenster);
    eing_botsch1.senden(dialogstatus0);
    /* EasyCODE ( 0 
    Sanduhr starten */
    while ( eing_botsch1.busycheck() )
    {
    }
    /* EasyCODE < */
    screen.sanduhrreq_set(160,160); // sanduhrtest einschalten AK
    /* EasyCODE > */
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    pnr directory vom FLASH laden */
    /* EasyCODE ( 0 
    Dir-Puffer loeschen */
    for ( i = 0; i <= MAX_PROGANZAHL; i++ )
    {
        pnrdir[LINKS][i] = 0xffff;
    }
    /* EasyCODE ) */
    flashdirload();
    /* EasyCODE ) */
    eing_botsch1.senden(dialogstatus2);
    /* EasyCODE ( 0 
    Sanduhr starten */
    while ( eing_botsch1.busycheck() )
    {
    }
    /* EasyCODE < */
    screen.sanduhrreq_set(160,160); // sanduhrtest einschalten AK
    /* EasyCODE > */
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    SD-Card pruefen und pnr directory laden */
    if ( sdcardcheck() )
    {
        // Richtige SD-Karte gesteckt
        sddirload();
    }
    /* EasyCODE ) */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::dirfileload(void) */
/* EasyCODE F */
void KlasseProgverw::dirfileload(void)
{
    unsigned char i;
    /* EasyCODE - */
    eing_botsch1.senden(dialogfenster);
    eing_botsch1.senden(dialogstatus0);
    /* EasyCODE ( 0 
    Flash file directory laden */
    for ( i = 0; i < DIRZ_MAX; i++ )
    {
        filedir[LINKS][i] = 0xff;
    }
    if ( findfirst("C:\\KFPORTAL.*",&ffblk,0) == 0 )
    {
        filedir[LINKS][0] = 0;
        fileindexmax[LINKS] = 1;
    }
    else
    {
        fileindexmax[LINKS] = 0;
    }
    if ( findfirst("C:\\MDPORTAL.*",&ffblk,0) == 0 )
    {
        if ( filedir[LINKS][0] == 0xff )
        {
            filedir[LINKS][0] = 1;
            fileindexmax[LINKS] = 1;
        }
        else
        {
            filedir[LINKS][1] = 1;
            fileindexmax[LINKS] = 2;
        }
    }
    /* EasyCODE ) */
    eing_botsch1.senden(dialogstatus2);
    /* EasyCODE ( 0 
    SD-Card pruefen und file directory laden */
    if ( sdcardcheck() )
    {
        // Richtige SD-Karte gesteckt
        sddirfileload();
    }
    /* EasyCODE ) */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::diraktanz(unsigned char drive) */
/* EasyCODE F */
void KlasseProgverw::diraktanz(unsigned char drive)
{
    unsigned int i,j;
    if ( anzreq[drive] )
    {
        anzreq[drive] = FALSE;
        switch ( dirkey )
        {
        case 1 :
            /* EasyCODE ( 0 
            DIR fuer MDAT und KONF
            in varstrings vorbereiten */
            for ( i = 0; i < DIRZ_MAX; i++ )
            {
                switch ( filedir[drive][i] )
                {
                case 0 :
                    sprintf(dirvarstring[i],dir1s,
                       dirposs[drive][i],
                       "KFPORTAL.BIN");
                    break;
                case 1 :
                    sprintf(dirvarstring[i],dir1s,
                       dirposs[drive][i],
                       "MDPORTAL");
                    break;
                default:
                    sprintf(dirvarstring[i],dirloeschs,
                       dirposs[drive][i]);
                    break;
                }
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Dir ausgeben */
            if ( block[drive] )
            {
            }
            else
            {
                dirindexblock[drive] = dirindex[drive];
                zindexblock[drive] = zindex[drive];
            }
            if ( fileindexmax[drive] == 0 )
            {
                for ( i = 0; i < DIRZ_MAX; i++ )
                {
                    eing_botsch1.senden(dirvarstring[i]);
                }
                eing_botsch1.senden(ESCS "F001" ION);
                sprintf(eing.varstring,dirstatus[drive],
                   0,
                   fileindexmax[drive]);
            }
            else
            {
                /* EasyCODE ( 0 
                Block markieren */
                for ( i = 0; i < DIRZ_MAX; i++ )
                {
                    if ( i == zindex[drive] )
                    {
                        if ( (koor.bdf == PANEL) ||
                             (koor.bdf == PANELPC) )
                        {
                            eing_botsch1.senden(ION);
                            eing_botsch1.senden( TEXTBLAU);
                        }
                        else
                        {
                            eing_botsch1.senden(ION);
                        }
                    }
                    eing_botsch1.senden(dirvarstring[i]);
                    if ( i == zindex[drive] )
                    {
                        if ( (koor.bdf == PANEL) ||
                             (koor.bdf == PANELPC) )
                        {
                            eing_botsch1.senden(TEXTSCHWARZ);
                            eing_botsch1.senden(IOFF);
                        }
                        else
                        {
                            eing_botsch1.senden(IOFF);
                        }
                    }
                }
                /* EasyCODE ) */
                eing_botsch1.senden(ESCS "F001" ION);
                sprintf(eing.varstring,dirstatus[drive],
                   (dirindex[drive]-dirindexblock[drive]+1),
                   fileindexmax[drive]);
            }
            eing_botsch1.senden(eing.varstring);
            eing_botsch1.senden(IOFF ESCS "F1");
            /* EasyCODE ) */
            break;
        default:
            /* EasyCODE ( 0 
            DIR fuer Programme
            in varstrings vorbereiten */
            if ( anzkommreq[drive] )
            {
                /* EasyCODE ( 0 
                Kommentar anzeigen */
                anzkommreq[drive] = FALSE;
                if ( drive == LINKS )
                {
                    /* EasyCODE ( 0 
                    Kommentar lesen */
                    for ( i=0; i<DIRZ_MAX; i++ )
                    {
                        switch ( di.parprog (pnrdir[LINKS][i+(dirindex[drive]-zindex[drive])],&cmdppar,LINKS) )
                        {
                        case 3 :
                            // Programm nicht fuer diese Maschine
                        case 0 :
                            /* EasyCODE < */
                            // nicht vorhanden
                            /* EasyCODE > */
                            break;
                        default:
                            /* EasyCODE ( 0 
                            Kommentar */
                            for ( j = 0; j < 10; j++ )
                            {
                                if ( cmdppar.kommentar[j] == 0x00 )
                                {
                                    /* EasyCODE ( 0 
                                    Ende */
                                    eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][j] = 0x00;
                                    break;
                                    /* EasyCODE ) */
                                }
                                else
                                {
                                    /* EasyCODE ( 0 
                                    eintragen */
                                    switch ( j )
                                    {
                                    case 8 :
                                        if ( cmdppar.kommentar[j+1] != 0x00
                                                                          &&
                                             cmdppar.kommentar[j+2] != 0x00 )
                                        {
                                            eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][j] = '.';
                                        }
                                        else
                                        {
                                            eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][j] = cmdppar.kommentar[j];
                                        }
                                        break;
                                    case 9 :
                                        if ( cmdppar.kommentar[j+1] != 0x00 )
                                        {
                                            eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][j] = '.';
                                        }
                                        else
                                        {
                                            eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][j] = cmdppar.kommentar[j];
                                        }
                                        eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][10] = 0x00;
                                        break;
                                    default:
                                        eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][j] = cmdppar.kommentar[j];
                                        break;
                                    }
                                    /* EasyCODE ) */
                                }
                            }
                            /* EasyCODE ) */
                            break;
                        }
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Kommentar eintragen */
                    for ( i=0; i<DIRZ_MAX; i++ )
                    {
                        sprintf(quellpath,"%04u",pnrdir[RECHTS][i+(dirindex[drive]-zindex[drive])]);
                        ReadCharFromSd(quellpath, eing.progverw.komentardir[RECHTS][i+(dirindex[drive]-zindex[drive])], 11, KKOMMENTAR);
                        /* EasyCODE ( 0 
                        .. eintragen */
                        for ( j=0; j<10; j++ )
                        {
                            if ( eing.progverw.komentardir[RECHTS][i+(dirindex[drive]-zindex[drive])][j] == 0x00 )
                            {
                                break;
                            }
                            else
                            {
                                if ( j == 9 )
                                {
                                    if ( eing.progverw.komentardir[RECHTS][i+(dirindex[drive]-zindex[drive])][10] == 0x00 )
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        eing.progverw.komentardir[RECHTS][i+(dirindex[drive]-zindex[drive])][8] = '.';
                                        eing.progverw.komentardir[RECHTS][i+(dirindex[drive]-zindex[drive])][9] = '.';
                                        eing.progverw.komentardir[RECHTS][i+(dirindex[drive]-zindex[drive])][10] = 0x00;
                                    }
                                }
                                else
                                {
                                    continue;
                                }
                            }
                        }
                        /* EasyCODE ) */
                    }
                    /* EasyCODE ) */
                }
                for ( i = 0; i < DIRZ_MAX; i++ )
                {
                    if ( pnrdir[drive][i+(dirindex[drive]-zindex[drive])] <= PNRMAX )
                    {
                        sprintf(dirvarstring[i],dirs,
                                            dirposs[drive][i],
                                            pnrdir[drive][i+(dirindex[drive]-zindex[drive])],
                                            komentardir[drive][i+(dirindex[drive]-zindex[drive])]);
                    }
                    else
                    {
                        sprintf(dirvarstring[i],dirloeschs,
                                            dirposs[drive][i]);
                    }
                }
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                Kommentar nicht anzeigen */
                for ( i = 0; i < DIRZ_MAX; i++ )
                {
                    if ( pnrdir[drive][i+(dirindex[drive]-zindex[drive])] <= PNRMAX )
                    {
                        sprintf(dirvarstring[i],dirs,
                                            dirposs[drive][i],
                                            pnrdir[drive][i+(dirindex[drive]-zindex[drive])],
                                            "          ");
                    }
                    else
                    {
                        sprintf(dirvarstring[i],dirloeschs,
                                            dirposs[drive][i]);
                    }
                }
                /* EasyCODE ) */
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Dir ausgeben */
            if ( block[drive] )
            {
            }
            else
            {
                dirindexblock[drive] = dirindex[drive];
                zindexblock[drive] = zindex[drive];
            }
            if ( dirindexmax[drive] == 0 )
            {
                for ( i = 0, eing.varstring[0]=0; i < DIRZ_MAX; i++ )
                {
                    strcat(eing.varstring, dirvarstring[i]);
                }
                eing_botsch1.senden(eing.varstring);
                eing_botsch1.senden(ESCS "F001" ION);
                sprintf(eing.varstring,dirstatus[drive],
                   0,
                   dirindexmax[drive]);
            }
            else
            {
                if ( dirindex[drive] == dirindexblock[drive] )
                {
                    /* EasyCODE ( 0 
                    Block markieren */
                    for ( i = 0, eing.varstring[0] = 0; i < DIRZ_MAX; i++ )
                    {
                        if ( i == zindex[drive] )
                        {
                            if ( (koor.bdf == PANEL) ||
                                 (koor.bdf == PANELPC) )
                            {
                                strcat(eing.varstring, ION);
                                strcat(eing.varstring, TEXTBLAU);
                            }
                            else
                            {
                                strcat(eing.varstring, ION);
                            }
                        }
                        strcat(eing.varstring, dirvarstring[i]);
                        if ( i == zindex[drive] )
                        {
                            if ( (koor.bdf == PANEL) ||
                                 (koor.bdf == PANELPC) )
                            {
                                strcat(eing.varstring,TEXTSCHWARZ);
                                strcat(eing.varstring, IOFF);
                            }
                            else
                            {
                                strcat(eing.varstring, IOFF);
                            }
                        }
                    }
                    /* EasyCODE ) */
                    eing_botsch1.senden (eing.varstring);
                    eing_botsch1.senden(ESCS "F001" ION);
                    sprintf(eing.varstring,dirstatus[drive],
                       (dirindex[drive]-dirindexblock[drive]+1),
                       dirindexmax[drive]);
                }
                else
                {
                    if ( dirindex[drive] > dirindexblock[drive] )
                    {
                        /* EasyCODE ( 0 
                        Block aufwaerts markieren */
                        for ( i = 0, eing.varstring[0] = 0; i < DIRZ_MAX; i++ )
                        {
                            if ( i == zindexblock[drive] )
                            {
                                if ( (koor.bdf == PANEL) ||
                                     (koor.bdf == PANELPC) )
                                {
                                    strcat(eing.varstring, ION);
                                    strcat(eing.varstring, TEXTBLAU);
                                }
                                else
                                {
                                    strcat(eing.varstring, ION);
                                }
                            }
                            strcat(eing.varstring, dirvarstring[i]);
                            if ( i == zindex[drive] )
                            {
                                if ( (koor.bdf == PANEL) ||
                                     (koor.bdf == PANELPC) )
                                {
                                    strcat(eing.varstring, TEXTSCHWARZ);
                                    strcat(eing.varstring, IOFF);
                                }
                                else
                                {
                                    strcat(eing.varstring, IOFF);
                                }
                            }
                        }
                        /* EasyCODE ) */
                        eing_botsch1.senden(eing.varstring);
                        eing_botsch1.senden(ESCS "F001" ION);
                        sprintf(eing.varstring,dirstatus[drive],
                           (dirindex[drive]-dirindexblock[drive]+1),
                           dirindexmax[drive]);
                    }
                    else
                    {
                        /* EasyCODE ( 0 
                        Block abwaerts markieren */
                        for ( i = 0, eing.varstring[0] = 0; i < DIRZ_MAX; i++ )
                        {
                            if ( i == zindex[drive] )
                            {
                                if ( (koor.bdf == PANEL) ||
                                     (koor.bdf == PANELPC) )
                                {
                                    strcat(eing.varstring, ION);
                                    strcat(eing.varstring, TEXTBLAU);
                                }
                                else
                                {
                                    strcat(eing.varstring, ION);
                                }
                            }
                            strcat(eing.varstring, dirvarstring[i]);
                            if ( i == zindexblock[drive] )
                            {
                                if ( (koor.bdf == PANEL) ||
                                     (koor.bdf == PANELPC) )
                                {
                                    strcat(eing.varstring, TEXTSCHWARZ);
                                    strcat(eing.varstring, IOFF);
                                }
                                else
                                {
                                    strcat(eing.varstring, IOFF);
                                }
                            }
                        }
                        /* EasyCODE ) */
                        eing_botsch1.senden(eing.varstring);
                        eing_botsch1.senden(ESCS "F001" ION);
                        sprintf(eing.varstring,dirstatus[drive],
                           (dirindexblock[drive]-dirindex[drive]+1),
                           dirindexmax[drive]);
                    }
                }
            }
            eing_botsch1.senden(eing.varstring);
            eing_botsch1.senden(IOFF ESCS "F1" );
            /* EasyCODE ) */
            break;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::diranz(unsigned char drive) */
/* EasyCODE F */
void KlasseProgverw::diranz(unsigned char drive)
{
    unsigned int i, j;
    if ( anzreq[drive] )
    {
        anzreq[drive] = FALSE;
        switch ( dirkey )
        {
        case 1 :
            /* EasyCODE ( 0 
            DIR fuer MDAT und KONF
            in varstrings vorbereiten */
            for ( i = 0; i < DIRZ_MAX; i++ )
            {
                switch ( filedir[drive][i] )
                {
                case 0 :
                    sprintf(dirvarstring[i],dir1s,
                       dirposs[drive][i],
                       "KFPORTAL.BIN");
                    break;
                case 1 :
                    sprintf(dirvarstring[i],dir1s,
                       dirposs[drive][i],
                       "MDPORTAL");
                    break;
                default:
                    sprintf(dirvarstring[i],dirloeschs,
                       dirposs[drive][i]);
                    break;
                }
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Dir ausgeben */
            for ( i = 0; i < DIRZ_MAX; i++ )
            {
                eing_botsch1.senden(dirvarstring[i]);
            }
            eing_botsch1.senden(ESCS "F001");
            sprintf(eing.varstring,dirstatus[drive],0,fileindexmax[drive]);
            eing_botsch1.senden(eing.varstring);
            eing_botsch1.senden(ESCS "F1");
            /* EasyCODE ) */
            break;
        default:
            /* EasyCODE ( 0 
            DIR fuer Programme
            in varstrings vorbereiten */
            if ( anzkommreq[drive] )
            {
                /* EasyCODE ( 0 
                Kommentar anzeigen */
                anzkommreq[drive] = FALSE;
                if ( drive == LINKS )
                {
                    /* EasyCODE ( 0 
                    Kommentar lesen */
                    for ( i=0; i<DIRZ_MAX; i++ )
                    {
                        switch ( di.parprog (pnrdir[LINKS][i+(dirindex[drive]-zindex[drive])],&cmdppar,DRIVE_C) )
                        {
                        case 3 :
                            // Programm nicht fuer diese Maschine
                        case 0 :
                            /* EasyCODE < */
                            // nicht vorhanden
                            /* EasyCODE > */
                            break;
                        default:
                            /* EasyCODE ( 0 
                            Kommentar */
                            for ( j = 0; j < 10; j++ )
                            {
                                if ( cmdppar.kommentar[j] == 0x00 )
                                {
                                    /* EasyCODE ( 0 
                                    Ende */
                                    eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][j] = 0x00;
                                    break;
                                    /* EasyCODE ) */
                                }
                                else
                                {
                                    /* EasyCODE ( 0 
                                    eintragen */
                                    switch ( j )
                                    {
                                    case 8 :
                                        if ( cmdppar.kommentar[j+1] != 0x00
                                                                          &&
                                             cmdppar.kommentar[j+2] != 0x00 )
                                        {
                                            eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][j] = '.';
                                        }
                                        else
                                        {
                                            eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][j] = cmdppar.kommentar[j];
                                        }
                                        break;
                                    case 9 :
                                        if ( cmdppar.kommentar[j+1] != 0x00 )
                                        {
                                            eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][j] = '.';
                                        }
                                        else
                                        {
                                            eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][j] = cmdppar.kommentar[j];
                                        }
                                        eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][10] = 0x00;
                                        break;
                                    default:
                                        eing.progverw.komentardir[LINKS][i+(dirindex[drive]-zindex[drive])][j] = cmdppar.kommentar[j];
                                        break;
                                    }
                                    /* EasyCODE ) */
                                }
                            }
                            /* EasyCODE ) */
                            break;
                        }
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Kommentar eintragen */
                    for ( i=0; i<DIRZ_MAX; i++ )
                    {
                        sprintf(quellpath,"%04u",pnrdir[RECHTS][i+(dirindex[drive]-zindex[drive])]);
                        ReadCharFromSd(quellpath, eing.progverw.komentardir[RECHTS][i+(dirindex[drive]-zindex[drive])], 11, KKOMMENTAR);
                        /* EasyCODE ( 0 
                        .. eintragen */
                        for ( j=0; j<10; j++ )
                        {
                            if ( eing.progverw.komentardir[RECHTS][i+(dirindex[drive]-zindex[drive])][j] == 0x00 )
                            {
                                break;
                            }
                            else
                            {
                                if ( j == 9 )
                                {
                                    if ( eing.progverw.komentardir[RECHTS][i+(dirindex[drive]-zindex[drive])][10] == 0x00 )
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        eing.progverw.komentardir[RECHTS][i+(dirindex[drive]-zindex[drive])][8] = '.';
                                        eing.progverw.komentardir[RECHTS][i+(dirindex[drive]-zindex[drive])][9] = '.';
                                        eing.progverw.komentardir[RECHTS][i+(dirindex[drive]-zindex[drive])][10] = 0x00;
                                    }
                                }
                                else
                                {
                                    continue;
                                }
                            }
                        }
                        /* EasyCODE ) */
                    }
                    /* EasyCODE ) */
                }
                for ( i = 0; i < DIRZ_MAX; i++ )
                {
                    if ( pnrdir[drive][i+(dirindex[drive]-zindex[drive])] <= PNRMAX )
                    {
                        sprintf(dirvarstring[i],dirs,
                                            dirposs[drive][i],
                                            pnrdir[drive][i+(dirindex[drive]-zindex[drive])],
                                            komentardir[drive][i+(dirindex[drive]-zindex[drive])]);
                    }
                    else
                    {
                        sprintf(dirvarstring[i],dirloeschs,
                                            dirposs[drive][i]);
                    }
                }
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                Kommentar nicht anzeigen */
                for ( i = 0; i < DIRZ_MAX; i++ )
                {
                    if ( pnrdir[drive][i+(dirindex[drive]-zindex[drive])] <= PNRMAX )
                    {
                        sprintf(dirvarstring[i],dirs,
                                            dirposs[drive][i],
                                            pnrdir[drive][i+(dirindex[drive]-zindex[drive])],
                                            "          ");
                    }
                    else
                    {
                        sprintf(dirvarstring[i],dirloeschs,
                                            dirposs[drive][i]);
                    }
                }
                /* EasyCODE ) */
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Dir ausgeben */
            for ( i = 0; i < DIRZ_MAX; i++ )
            {
                eing_botsch1.senden(dirvarstring[i]);
            }
            eing_botsch1.senden(ESCS "F001");
            sprintf(eing.varstring,dirstatus[drive],0,dirindexmax[drive]);
            eing_botsch1.senden(eing.varstring);
            eing_botsch1.senden(ESCS "F1");
            if ( sterninv[drive] )
            {
                while ( eing_botsch1.busycheck() )
                {
                }
                if ( drive == LINKS )
                {
                    sprintf(eing.varstring,tinvs ,TSTERN);
                }
                else
                {
                    sprintf(eing.varstring,tinvs ,TSTERN1);
                }
                eing_botsch1.senden(eing.varstring);
                sterninv[drive] = FALSE;
            }
            /* EasyCODE ) */
            break;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgverw::flashdirload */
/* EasyCODE F */
/********** flashdirload **********/
// Dieses UP laedt die Programmnummern des Flashs und legt diese in ei-
// nem Speicher sortiert ab.


void KlasseProgverw::flashdirload(void)
{
    int i;
    unsigned int index, j, v, pnrtemp;
    unsigned char * file = {"%s****"};
    unsigned char filevarstring[20];
    struct ffblk ffblk;
    sprintf (filevarstring, file, datdir);
    for ( i = 0; i <= MAX_PROGANZAHL; i++ )
    {
        pnrdir[LINKS][i] = 0xffff;
    }
    for ( index = 0; index<MAX_PROGANZAHL; )
    {
        if ( index == 0 )
        {
            if ( findfirst(filevarstring,&ffblk,0) == 0 )
            {
            }
            else
            {
                break;
            }
        }
        else
        {
            if ( findnext(&ffblk) == 0 )
            {
            }
            else
            {
                break;
            }
        }
        pnrtemp = (ffblk.ff_name[0]&0x0f)*1000 + (ffblk.ff_name[1]&0x0f)*100 +
                  (ffblk.ff_name[2]&0x0f)*10 + (ffblk.ff_name[3]&0x0f);
        for ( v=0; v< index; v++ )
        {
            if ( pnrdir[LINKS][v] > pnrtemp )
            {
                break;
            }
            else
            {
                continue;
            }
        }
        if ( pnrtemp>PNRMAX )
        {
        }
        else
        {
            if ( v == index )
            {
                pnrdir[LINKS][index] = pnrtemp;
            }
            else
            {
                for ( j=index; j > v; j-- )
                {
                    pnrdir[LINKS][j] = pnrdir[LINKS][j-1];
                }
                pnrdir[LINKS][v] = pnrtemp;
            }
            index++;
        }
    }
    eing.progverw.dirindexmax[LINKS] = index;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::sdcardcheck */
/* EasyCODE F */
// Dieses UP prueft ob die richtige SD-Card steckt
//
// Rueckmeldung:
//   TRUE  ...... richtige SD-Card steckt
//   FALSE ...... Karte steckt nicht oder falsche SD-Karte
//                sdcarderror sitzt

unsigned char KlasseProgverw::sdcardcheck(void)
{
    unsigned char retwert = FALSE;
    unsigned int i;
    sdcarderror = 0;
    switch ( sd.SdInitMedia () )
    {
    case 0 :
        /* EasyCODE < */
        // fertig
        /* EasyCODE > */
        switch ( sd.SdChangeDir (arbeitsverzeichnis) )
        {
        case 0 :
            // Richtige SD-Karte gesteckt
            retwert = TRUE;
            break;
        case 1 :
            // Falsche SD-Karte gesteckt
            // (Arbeitsverzeichnis nicht da);
            sdcarderror = FALSCHEKARTE;
            /* EasyCODE ( 0 
            Dir-Puffer loeschen */
            for ( i = 0; i <= MAX_PROGANZAHL; i++ )
            {
                pnrdir[RECHTS][i] = 0xffff;
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            fileDir-Puffer loeschen */
            for ( i = 0; i <DIRZ_MAX; i++ )
            {
                filedir[RECHTS][i] = 0xff;
            }
            /* EasyCODE ) */
            break;
        default:
            // Fehler
            sdcarderror = UEBERTRAGUNGSFEHLERKARTE;
            /* EasyCODE ( 0 
            Dir-Puffer loeschen */
            for ( i = 0; i <= MAX_PROGANZAHL; i++ )
            {
                pnrdir[RECHTS][i] = 0xffff;
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            fileDir-Puffer loeschen */
            for ( i = 0; i <DIRZ_MAX; i++ )
            {
                filedir[RECHTS][i] = 0xff;
            }
            /* EasyCODE ) */
            break;
        }
        break;
    case 1 :
        // keine SD-Karte gesteckt
        sdcarderror = KEINEKARTE;
        /* EasyCODE ( 0 
        Dir-Puffer loeschen */
        for ( i = 0; i <= MAX_PROGANZAHL; i++ )
        {
            pnrdir[RECHTS][i] = 0xffff;
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        fileDir-Puffer loeschen */
        for ( i = 0; i <DIRZ_MAX; i++ )
        {
            filedir[RECHTS][i] = 0xff;
        }
        /* EasyCODE ) */
        break;
    default:
        // Uebertragungsfehler
        sdcarderror = UEBERTRAGUNGSFEHLERKARTE;
        /* EasyCODE ( 0 
        Dir-Puffer loeschen */
        for ( i = 0; i <= MAX_PROGANZAHL; i++ )
        {
            pnrdir[RECHTS][i] = 0xffff;
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        fileDir-Puffer loeschen */
        for ( i = 0; i <DIRZ_MAX; i++ )
        {
            filedir[RECHTS][i] = 0xff;
        }
        /* EasyCODE ) */
        break;
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::sddirload */
/* EasyCODE F */
void KlasseProgverw::sddirload(void)
{
    unsigned int i;
    /* EasyCODE ( 0 
    Dir-Puffer loeschen */
    for ( i = 0; i <= MAX_PROGANZAHL; i++ )
    {
        pnrdir[RECHTS][i] = 0xffff;
    }
    /* EasyCODE ) */
    dirindexmax[RECHTS] = direintrag(RECHTS,0,0);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::sddirfileload */
/* EasyCODE F */
/********** diskdirfileload **********/
// Dieses UP laedt die Konf und Mdat einer SD-Karte und legt diese in ei-
// nem Dir-Speicher sortiert ab.

void KlasseProgverw::sddirfileload(void)
{
    unsigned char i;
    /* EasyCODE ( 0 
    fileDir-Puffer loeschen */
    for ( i = 0; i <DIRZ_MAX; i++ )
    {
        filedir[RECHTS][i] = 0xff;
    }
    /* EasyCODE ) */
    if ( sd.SdFileOpen("KFPORTAL.BIN\r", READ) == 0 )
    {
        filedir[RECHTS][0] = 0;
        fileindexmax[RECHTS] = 1;
    }
    else
    {
        fileindexmax[RECHTS] = 0;
    }
    sd.SdFileClose();
    if ( sd.SdFileOpen("MDPORTAL\r", READ) == 0 )
    {
        if ( filedir[RECHTS][0] == 0xff )
        {
            filedir[RECHTS][0] = 1;
            fileindexmax[RECHTS] = 1;
        }
        else
        {
            filedir[RECHTS][1] = 1;
            fileindexmax[RECHTS] = 2;
        }
    }
    sd.SdFileClose();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned int KlasseProgverw::direintrag */
/* EasyCODE F */
/********** direintrag **********/
// Dieses UP legt die Programmparameter in den entsprechenden Puffern ab.

unsigned int KlasseProgverw::direintrag(unsigned char drive, unsigned int pnr, unsigned int index)
{
    unsigned int i,j,u,v;
    unsigned int indextemp, pnrtemp;
    unsigned char loop;
    if ( drive == RECHTS )
    {
        /* EasyCODE ( 0 
        SDCARD oder LAN */
        switch ( sd.SdReadDir ("\\Portal", datenstring) )
        {
        case 0 :
            /* EasyCODE ( 0 
            ok */
            indextemp=0;
            u=0;
            loop = TRUE;
            while ( loop && (indextemp < MAX_PROGANZAHL) )
            {
                if ( datenstring[u] != 0 )
                {
                    for ( i=0, pnrtemp=0; i<=4; i++ )
                    {
                        if ( (datenstring[u+i]&0x0f) <= 9 )
                        {
                            if ( i == 4 )
                            {
                                /* EasyCODE ( 0 
                                Fehler: 5. Zeichen
                                muss 'r' sein */
                                // Uebertragungsfehler
                                sdcarderror = UEBERTRAGUNGSFEHLERKARTE;
                                loop = FALSE;
                                /* EasyCODE ) */
                            }
                        }
                        else
                        {
                            if ( datenstring[u+i] == '\r' )
                            {
                                /* EasyCODE ( 0 
                                Trennungszeichen
                                erkannt */
                                if ( i == 4 )
                                {
                                    pnrtemp = (datenstring[u]&0x0f)*1000 + (datenstring[u+1]&0x0f)*100 +
                                              (datenstring[u+2]&0x0f)*10 + (datenstring[u+3]&0x0f);
                                    for ( v=0; v< indextemp; v++ )
                                    {
                                        if ( pnrdir[RECHTS][v] > pnrtemp )
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            continue;
                                        }
                                    }
                                    if ( pnrtemp>PNRMAX )
                                    {
                                    }
                                    else
                                    {
                                        if ( v == indextemp )
                                        {
                                            pnrdir[RECHTS][indextemp] = pnrtemp;
                                        }
                                        else
                                        {
                                            for ( j=indextemp; j > v; j-- )
                                            {
                                                pnrdir[RECHTS][j] = pnrdir[RECHTS][j-1];
                                            }
                                            pnrdir[RECHTS][v] = pnrtemp;
                                        }
                                        indextemp++;
                                    }
                                }
                                u += i+1;
                                break;
                                /* EasyCODE ) */
                            }
                            else
                            {
                                /* EasyCODE ( 0 
                                Fehler:
                                falsches Zeichen */
                                // Uebertragungsfehler
                                sdcarderror = UEBERTRAGUNGSFEHLERKARTE;
                                loop = FALSE;
                                break;
                                /* EasyCODE ) */
                            }
                        }
                    }
                }
                else
                {
                    loop = FALSE;
                }
            }
            index = indextemp;
            break;
            /* EasyCODE ) */
        default:
            // Uebertragungsfehler
            sdcarderror = UEBERTRAGUNGSFEHLERKARTE;
            break;
        }
        /* EasyCODE ) */
    }
    else
    {
        /* EasyCODE ( 0 
        Flash oder Diskette */
        switch ( di.parprog (pnr,&cmdppar,drive) )
        {
        case 3 :
            // Programm nicht fuer diese Maschine
        case 0 :
            /* EasyCODE < */
            // nicht vorhanden
            /* EasyCODE > */
            break;
        default:
            eing.progverw.pnrdir[drive][index] = pnr;
            /* EasyCODE ( 0 
            Kommentar */
            for ( j = 0; j < 10; j++ )
            {
                if ( cmdppar.kommentar[j] == 0x00 )
                {
                    /* EasyCODE ( 0 
                    Ende */
                    eing.progverw.komentardir[drive][index][j] = 0x00;
                    break;
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    eintragen */
                    switch ( j )
                    {
                    case 8 :
                        if ( cmdppar.kommentar[j+1] != 0x00
                                                          &&
                             cmdppar.kommentar[j+2] != 0x00 )
                        {
                            eing.progverw.komentardir[drive][index][j] = '.';
                        }
                        else
                        {
                            eing.progverw.komentardir[drive][index][j] = cmdppar.kommentar[j];
                        }
                        break;
                    case 9 :
                        if ( cmdppar.kommentar[j+1] != 0x00 )
                        {
                            eing.progverw.komentardir[drive][index][j] = '.';
                        }
                        else
                        {
                            eing.progverw.komentardir[drive][index][j] = cmdppar.kommentar[j];
                        }
                        eing.progverw.komentardir[drive][index][10] = 0x00;
                        break;
                    default:
                        eing.progverw.komentardir[drive][index][j] = cmdppar.kommentar[j];
                        break;
                    }
                    /* EasyCODE ) */
                }
            }
            /* EasyCODE ) */
            index++;
            break;
        }
        /* EasyCODE ) */
    }
    return index;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::copydiraktuallisieren */
/* EasyCODE F */
// Dieses UP legt die kopierten Programmparameter in den entsprechenden Puffern ab.

void KlasseProgverw::copydiraktuallisieren(unsigned char drive, unsigned int index)
{
    unsigned int i, j, k;
    switch ( dirkey )
    {
    case 1 :
        switch ( filedir[drive][0] )
        {
        case 0 :
            if ( filedir[aktdrive][index] == 1 )
            {
                filedir[drive][1] = 1;
                fileindexmax[drive] = 2;
            }
            break;
        case 1 :
            if ( filedir[aktdrive][index] == 0 )
            {
                filedir[drive][1] = 1;
                filedir[drive][0] = 0;
                fileindexmax[drive] = 2;
            }
            break;
        default:
            filedir[drive][0] = filedir[aktdrive][index];
            fileindexmax[drive] = 1;
            break;
        }
        break;
    default:
        /* EasyCODE ( 0 
        pnr und Kommentar eintragen */
        /* EasyCODE ( 0 
        k f+r n+chste freie Speichestelle
        festlegen */
        for ( k = 0; k <= MAX_PROGANZAHL; k++ )
        {
            if ( pnrdir[drive][k] == 0xffff )
            {
                break;
            }
        }
        /* EasyCODE ) */
        if ( pnrdir[aktdrive][index] > pnrdir[drive][k-1] )
        {
            /* EasyCODE ( 0 
            Neue Programmnummer
            groesser als bereits einge-
            tragene Programmnummern */
            pnrdir[drive][k] = pnrdir[aktdrive][index];
            for ( j = 0; j < 11; j++ )
            {
                komentardir[drive][k][j] = komentardir[aktdrive][index][j];
            }
            /* EasyCODE ) */
            dirindexmax[drive] = dirindexmax[drive] + 1;
        }
        else
        {
            for ( i = 0; i <= MAX_PROGANZAHL; i++ )
            {
                if ( pnrdir[drive][i] >= pnrdir[aktdrive][index] )
                {
                    if ( pnrdir[drive][i] == pnrdir[aktdrive][index] )
                    {
                        /* EasyCODE ( 0 
                        Programmnummer
                        bereits vorhanden,
                        nur Kommentar er-
                        setzen. */
                        for ( j = 0; j < 11; j++ )
                        {
                            komentardir[drive][i][j] = komentardir[aktdrive][index][j];
                        }
                        /* EasyCODE ) */
                    }
                    else
                    {
                        /* EasyCODE ( 0 
                        Programmnummer
                        neu, bestehendes Dir
                        verschieben und neue
                        Programmnummer einf+gen */
                        for ( ; k > i; k-- )
                        {
                            pnrdir[drive][k] = pnrdir[drive][k-1];
                            for ( j = 0; j < 11; j++ )
                            {
                                komentardir[drive][k][j] = komentardir[drive][k-1][j];
                            }
                        }
                        pnrdir[drive][i] = pnrdir[aktdrive][index];
                        for ( j = 0; j < 11; j++ )
                        {
                            komentardir[drive][i][j] = komentardir[aktdrive][index][j];
                        }
                        /* EasyCODE ) */
                        dirindexmax[drive] = dirindexmax[drive] + 1;
                    }
                    break;
                }
            }
        }
        /* EasyCODE ) */
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned char KlasseProgverw::sourceNet(void) */
/* EasyCODE F */
/* schaltet die Programmquelle auf Netzwerk
*/
unsigned char KlasseProgverw::sourceNet(void)
{
    unsigned char retwert=0;
    switch ( sd.ChangeToNET() )
    {
    case 0 :
        retwert = 0;
        break;
    case 1 :
        retwert = 1;
        break;
    default:
        // Uebertragungsfehler
        sdcarderror = UEBERTRAGUNGSFEHLERKARTE;
        /* EasyCODE ( 0 
        Dir-Puffer loeschen */
        for ( int i = 0; i <= MAX_PROGANZAHL; i++ )
        {
            pnrdir[RECHTS][i] = 0xffff;
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        fileDir-Puffer loeschen */
        for ( i = 0; i <DIRZ_MAX; i++ )
        {
            filedir[RECHTS][i] = 0xff;
        }
        /* EasyCODE ) */
        break;
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned char KlasseProgverw::sourceSD(void) */
/* EasyCODE F */
/* schaltet die Programmquelle auf SD-Karte
*/
unsigned char KlasseProgverw::sourceSD(void)
{
    unsigned char retwert=0;
    switch ( sd.ChangeToSD() )
    {
    case 0 :
        retwert = 0;
        break;
    case 1 :
        retwert = 1;
        break;
    default:
        // Uebertragungsfehler
        sdcarderror = UEBERTRAGUNGSFEHLERKARTE;
        /* EasyCODE ( 0 
        Dir-Puffer loeschen */
        for ( int i = 0; i <= MAX_PROGANZAHL; i++ )
        {
            pnrdir[RECHTS][i] = 0xffff;
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        fileDir-Puffer loeschen */
        for ( i = 0; i <DIRZ_MAX; i++ )
        {
            filedir[RECHTS][i] = 0xff;
        }
        /* EasyCODE ) */
        break;
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned char KlasseProgverw::sdcopy(void) */
/* EasyCODE F */
/* Dieses UP managed das Kopieren von Files
   von oder auf die SD-Karte
   Uebergabeparameter:
      keine

   Rueckmeldung
      0 - kein Fehler
      1 - Abbruch, weil Funktion verworfen oder Fehler
*/

unsigned char KlasseProgverw::sdcopy(void)
{
    unsigned int i, j, k;
    unsigned int maxind;
    /* EasyCODE ( 0 
    Laufvariablen festlegen
      j = Schleifenbegrenzung
      i = Schleifenzaehler
      k = index fuer pnrarray
      maxind = max. Index des zu schreibenden Verz. */
    switch ( dirkey )
    {
    case 1 :
        j = 1;
        k = dirindex[aktdrive];
        maxind = 1;
        break;
    default:
        if ( aktdrive == LINKS )
        {
            maxind = dirindexmax[RECHTS];
        }
        else
        {
            maxind = dirindexmax[LINKS];
        }
        if ( dirindex[aktdrive] == dirindexblock[aktdrive] )
        {
            /* EasyCODE ( 0 
            nur 1 Datei markiert */
            j = (dirindex[aktdrive]-dirindexblock[aktdrive]+1);
            k = dirindex[aktdrive];
            /* EasyCODE ) */
        }
        else
        {
            if ( dirindex[aktdrive] > dirindexblock[aktdrive] )
            {
                /* EasyCODE ( 0 
                Block markiert,
                Anfang = Blockindex */
                j = (dirindex[aktdrive]-dirindexblock[aktdrive]+1);
                k = dirindexblock[aktdrive];
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                Block markiert,
                Anfang = dirindex */
                j = (dirindexblock[aktdrive]-dirindex[aktdrive]+1);
                k = dirindex[aktdrive];
                /* EasyCODE ) */
            }
        }
        break;
    }
    i = 0;
    /* EasyCODE ) */
    eing_botsch1.senden(dialogfenster);
    do{
        if ( aktdrive == LINKS )
        {
            /* EasyCODE ( 0 
            von Flash nach Sd-Karte */
            switch ( dirkey )
            {
            case 1 :
                /* EasyCODE ( 0 
                quellpath festlegen */
                switch ( filedir[LINKS][k] )
                {
                case 0 // KONF
                :
                    sprintf(quellpath,"C:KFPORTAL.BIN");
                    sprintf(zielpath,"KFPORTAL.BIN");
                    break;
                case 1 // MDAT
                :
                    // MDAT kopieren
                    // erzeugeMDAT();
                    sprintf(quellpath,"C:MDPORTAL");
                    sprintf(zielpath,"MDPORTAL");
                    break;
                default:
                    sprintf(quellpath,"C:NIX");
                    sprintf(zielpath,"NIX");
                    break;
                }
                /* EasyCODE ) */
                break;
            default:
                /* EasyCODE ( 0 
                quellpath und zielpath festlegen */
                sprintf(quellpath,"%s%04u",datdir,pnrdir[aktdrive][k]);
                sprintf(zielpath,"%04u",pnrdir[aktdrive][k]);
                /* EasyCODE ) */
                break;
            }
            /* EasyCODE ( 0 
            File kopieren */
            switch ( WriteFileToSd(quellpath, zielpath) )
            {
            case 1 // File nicht auf Quelle
            :
                koor.eing_err = 10;
                error_res();
                break;
            case 2 // Quelle-Lesefehler, kann nicht geoeffnet werden
            :
                koor.eing_err = 11;
                error_res();
                break;
            case 3 // Ziel-Schreibfehler, kann nicht geoeffnet werden
            :
                koor.eing_err = 12;
                error_res();
                break;
            case 4 // Quelle-Lesefehler
            :
                koor.eing_err = 13;
                error_res();
                break;
            case 5 // Ziel-Schreibfehler
            :
                koor.eing_err = 14;
                error_res();
                break;
            case 6 // Kopieren verworfen
            :
                return 1;
            case 7 // Kopieren verworfen, naechstes ...
            :
                break;
            default:
                copydiraktuallisieren(RECHTS,k);
                break;
            }
            /* EasyCODE ) */
            k++;
            i++;
            maxind = dirindexmax[RECHTS];
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE ( 0 
            von Sd-Karte ins Flash */
            switch ( dirkey )
            {
            case 1 :
                /* EasyCODE ( 0 
                quellpath festlegen */
                switch ( filedir[RECHTS][k] )
                {
                case 0 // KONF
                :
                    sprintf(quellpath,"KFPORTAL.BIN");
                    sprintf(zielpath,"C:KFPORTAL.BIN");
                    break;
                case 1 // MDAT
                :
                    // MDAT kopieren
                    // erzeugeMDAT();
                    sprintf(quellpath,"MDPORTAL");
                    sprintf(zielpath,"C:MDPORTAL");
                    nae.homereq = TRUE;
                    break;
                default:
                    sprintf(quellpath,"NIX");
                    sprintf(zielpath,"C:NIX");
                    break;
                }
                /* EasyCODE ) */
                break;
            default:
                /* EasyCODE ( 0 
                quellpath und zielpath festlegen */
                sprintf(quellpath,"%04u",pnrdir[aktdrive][k]);
                sprintf(zielpath,"%s%04u",datdir,pnrdir[aktdrive][k]);
                /* EasyCODE ) */
                break;
            }
            /* EasyCODE ( 0 
            File kopieren */
            switch ( ReadFileFromSd(quellpath, zielpath) )
            {
            case 1 // File nicht auf Quelle
            :
                koor.eing_err = 10;
                error_res();
                break;
            case 2 // Quelle-Lesefehler, kann nicht geoeffnet werden
            :
                koor.eing_err = 11;
                error_res();
                break;
            case 3 // Ziel-Schreibfehler, kann nicht geoeffnet werden
            :
                koor.eing_err = 12;
                error_res();
                break;
            case 4 // Quelle-Lesefehler
            :
                koor.eing_err = 13;
                error_res();
                break;
            case 5 // Ziel-Schreibfehler
            :
                koor.eing_err = 14;
                error_res();
                break;
            case 6 // Kopieren verworfen
            :
                return 1;
            case 7 // Kopieren verworfen, naechstes ...
            :
                break;
            default:
                copydiraktuallisieren(LINKS,k);
                break;
            }
            /* EasyCODE ) */
            k++;
            i++;
            maxind = dirindexmax[LINKS];
            /* EasyCODE ) */
        }
    }while ( (i < j) && (maxind < MAX_PROGANZAHL) );
    return 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned char KlasseProgverw::filecopy(unsigned char * quelle, unsigned char * ziel) */
/* EasyCODE F */
unsigned char KlasseProgverw::filecopy(unsigned char * quelle, unsigned char * ziel)
/* Dieses UP Kopiert ein File von Quelle nach Ziel, dabei werden verschiedene
   Pr+fungen durchgef+hrt.
   _bergabeparameter:
      Laufwerk,Pfad und Dateiname als String in Quelle und Ziel z.B. "A:\\KONF3590.BIN"

   R+ckmeldung
      0 - kein Fehler
      1 - Programm nicht auf Quelle
      2 - Quelle-Lesefehler, kann nicht ge+ffnet werden
      3 - Ziel-Schreibfehler, kann nicht ge+ffnet werden
      4 - Quelle-Lesefehler
      5 - Ziel-Schreibfehler
      6 - Abbruch weil Funktion verworfen
      7 - Abbruch, n+chstes Programm
*/
{
    unsigned char loop;
    unsigned char dummy[50];
    
    long i;
    if ( findfirst(quelle,&ffblk,0)!=0 )
    {
        /* EasyCODE < */
        //Programm nicht auf Quelle
        /* EasyCODE > */
        return 1;
    }
    //eing_botsch1.senden(dialogfenster);
    sprintf(eing.varstring,dialogcopy0,quelle,ziel);
    eing_botsch1.senden(eing.varstring);
    /* EasyCODE - */
    quellfilesize=ffblk.ff_fsize;
    if ( findfirst(ziel,&ffblk,0) == 0 )
    {
        /* EasyCODE ( 0 
        Datei existiert bereits
        ueberschreiben ? */
        if ( !copyall )
        {
            sprintf(eing.varstring,tz->eing_cmd1,ziel);
            eing_botsch1.senden(eing.varstring);
            eing_botsch1.senden(dialogtasten0);
            eing_botsch1.senden(dialogtasten1);
            /* EasyCODE - */
            loop = TRUE;
            while ( loop )
            {
                switch ( getkey() )
                {
                case TESC :
                    if ( taste_los() == 0xff )
                    {
                        return E_RUHE;
                    }
                    eing_botsch1.senden(dialogcopy2);
                    eing_botsch1.senden(dialogtasten2);
                    return 6;
                case TNEXT :
                    if ( taste_los() == 0xff )
                    {
                        return E_RUHE;
                    }
                    eing_botsch1.senden(dialogcopy2);
                    eing_botsch1.senden(dialogtasten2);
                    return 7;
                case TALL :
                    copyall = TRUE;
                case TENTER :
                    loop = FALSE;
                    if ( taste_los() == 0xff )
                    {
                        return E_RUHE;
                    }
                    eing_botsch1.senden(dialogcopy2);
                    eing_botsch1.senden(dialogtasten2);
                    /* EasyCODE ( 0 
                    Sanduhr starten */
                    while ( eing_botsch1.busycheck() )
                    {
                    }
                    /* EasyCODE < */
                    screen.sanduhrreq_set(160,160); // sanduhrtest einschalten AK
                    /* EasyCODE > */
                    /* EasyCODE ) */
                    break;
                default:
                    eing_botsch1.senden(BELLBELL);
                    break;
                }
            }
        }
        /* EasyCODE ) */
    }
    else
    {
        /* EasyCODE < */
        // Datei existiert nicht
        // neu anlegen
        /* EasyCODE > */
    }
    if ( (in=fopen(quelle,"rb"))==NULL )
    {
        // Quelle-Lesefehler
        fclose(in);
        return 2;
    }
    if ( (out=fopen(ziel,"wb"))==NULL )
    {
        //Disketten-Schreibfehler
        fclose(in);
        fclose(out);
        remove(ziel);
        return 3;
    }
    for ( i=0; i<quellfilesize; i++ )
    {
        if ( (ineof=fgetc(in))==EOF )
        {
            //Lesefehler
            sprintf(dummy,"Lesefehler");
            break;
        }
        if ( fputc(ineof,out)==EOF )
        {
            //Schreibfehler
            sprintf(dummy,"Schreibfehler");
            break;
        }
    }
    fclose(in);
    if ( (fclose(out)!=0) || (i!=quellfilesize) )
    {
        if ( ineof==EOF )
        {
            /* EasyCODE < */
            //Lesefehler
            /* EasyCODE > */
            remove(ziel);
            return 4;
        }
        else
        {
            /* EasyCODE < */
            //Schreibfehler
            /* EasyCODE > */
            remove(ziel);
            return 5;
        }
    }
    return 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned char KlasseProgverw::WriteFileToSd(unsigned char *quelle, unsigned char *ziel) */
/* EasyCODE F */
/* Dieses UP Kopiert ein File vom Flash auf die SD-Karte, dabei werden verschiedene
   Pruefungen durchgefuehrt.
   Uebergabeparameter:
      Laufwerk,Pfad und Dateiname als String in Quelle
   Rueckmeldung
      0 - kein Fehler
      1 - Programm nicht auf Quelle
      2 - Quelle-Lesefehler, kann nicht geoeffnet werden
      3 - Ziel-Datei kann nicht geoeffnet werden
      4 - Quelle-Lesefehler
      5 - Ziel-Schreibfehler
      6 - Abbruch weil Funktion verworfen
      7 - Abbruch, naechstes Programm
 */

unsigned char KlasseProgverw::WriteFileToSd(unsigned char *quelle, unsigned char *ziel)
{
    unsigned char loop;
    unsigned char retwert=0;
    unsigned int u, i;
    if ( findfirst(quelle,&ffblk,0)!=0 )
    {
        //Programm nicht auf Quelle
        retwert = 1;
    }
    else
    {
        //sprintf(eing.varstring, dialogfenster);
        sprintf(eing.varstring,dialogcopy1,quelle);
        //strcat (eing.varstring, eing.varstring1);
        eing_botsch1.senden(eing.varstring);
        /* EasyCODE - */
        quellfilesize=ffblk.ff_fsize;
        if ( sd.SdFileOpen(ziel, READ) == 0 )
        {
            /* EasyCODE ( 0 
            Datei existiert bereits,
            ueberschreiben ? */
            if ( !copyall )
            {
                sprintf(eing.varstring,tz->eing_cmd1,ziel);
                eing_botsch1.senden(eing.varstring);
                eing_botsch1.senden(dialogtasten0);
                eing_botsch1.senden(dialogtasten1);
                loop = TRUE;
                while ( loop )
                {
                    switch ( getkey() )
                    {
                    case TESC :
                        taste_los();
                        eing_botsch1.senden(dialogcopy2);
                        eing_botsch1.senden(dialogtasten2);
                        loop = FALSE;
                        retwert = 6;
                        break;
                    case TNEXT :
                        taste_los();
                        eing_botsch1.senden(dialogcopy2);
                        eing_botsch1.senden(dialogtasten2);
                        loop = FALSE;
                        retwert = 7;
                        break;
                    case TALL :
                        copyall = TRUE;
                    case TENTER :
                        loop = FALSE;
                        taste_los();
                        eing_botsch1.senden(dialogcopy2);
                        eing_botsch1.senden(dialogtasten2);
                        /* EasyCODE ( 0 
                        Sanduhr starten */
                        while ( eing_botsch1.busycheck() )
                        {
                        }
                        /* EasyCODE < */
                        screen.sanduhrreq_set(160,160); // sanduhrtest einschalten AK
                        /* EasyCODE > */
                        /* EasyCODE ) */
                        break;
                    default:
                        eing_botsch1.senden(BELLBELL);
                        break;
                    }
                }
            }
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE < */
            // Datei existiert nicht
            // neu anlegen
            /* EasyCODE > */
            if ( sd.SdFileCreate(ziel) == 0 )
            {
            }
            else
            {
                retwert = 3;
            }
        }
        sd.SdFileClose();
        if ( retwert == 0 )
        {
            if ( (in=fopen(quelle,"rb"))==NULL )
            {
                // Quelle-Lesefehler
                fclose(in);
                retwert = 2;
            }
            else
            {
                if ( sd.SdFileOpen(ziel,WRITE)==0 )
                {
                    /* EasyCODE ( 0 
                    Daten kopieren */
                    for ( u=0; u<quellfilesize; u+=RWLAENGE )
                    {
                        for ( i=0; (i<RWLAENGE) && ((u+i)<quellfilesize); i++ )
                        {
                            daten[i]=getc(in);
                        }
                        if ( feof(in) || ferror(in) )
                        {
                            retwert = 4;
                            break;
                        }
                        else
                        {
                            if ( sd.SdFileWrite(daten, i) == 0 )
                            {
                            }
                            else
                            {
                                retwert = 5;
                                break;
                            }
                        }
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    retwert = 3;
                }
                fclose(in);
                sd.SdFileClose();
                if ( retwert != 0 )
                {
                    sd.SdFileDelete(ziel);
                }
            }
        }
        else
        {
            //Disketten-Schreibfehler
            fclose(in);
            sd.SdFileClose();
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned char KlasseProgverw::ReadFileFromSd(unsigned char *quelle, unsigned char *ziel) */
/* EasyCODE F */
/* Dieses UP Kopiert ein File vom Flash auf die SD-Karte, dabei werden verschiedene
   Pruefungen durchgefuehrt.
   Uebergabeparameter:
      Laufwerk,Pfad und Dateiname als String in Quelle
   Rueckmeldung
      0 - kein Fehler
      1 - Programm nicht auf Quelle
      2 - Quelle-Lesefehler, kann nicht geoeffnet werden
      3 - Ziel-Datei kann nicht geoeffnet werden
      4 - Quelle-Lesefehler
      5 - Ziel-Schreibfehler
      6 - Abbruch weil Funktion verworfen
      7 - Abbruch, naechstes Programm
 */

unsigned char KlasseProgverw::ReadFileFromSd(unsigned char *quelle, unsigned char *ziel)
{
    unsigned char loop;
    unsigned char retwert=0;
    unsigned long length;
    unsigned int u, i, j;
    if ( sd.GetFileSize(quelle, &length) != 0 )
    {
        //Programm nicht auf Quelle
        retwert = 1;
    }
    else
    {
        //sprintf(eing.varstring, dialogfenster);
        sprintf(eing.varstring,dialogcopy3,quelle);
        //strcat (eing.varstring, eing.varstring1);
        eing_botsch1.senden(eing.varstring);
        if ( findfirst(ziel,&ffblk,0)== 0 )
        {
            /* EasyCODE ( 0 
            Datei existiert bereits,
            ueberschreiben ? */
            if ( !copyall )
            {
                sprintf(eing.varstring,tz->eing_cmd1,ziel);
                eing_botsch1.senden(eing.varstring);
                eing_botsch1.senden(dialogtasten0);
                eing_botsch1.senden(dialogtasten1);
                loop = TRUE;
                while ( loop )
                {
                    switch ( getkey() )
                    {
                    case TESC :
                        taste_los();
                        eing_botsch1.senden(dialogcopy2);
                        eing_botsch1.senden(dialogtasten2);
                        loop = FALSE;
                        retwert = 6;
                        break;
                    case TNEXT :
                        taste_los();
                        eing_botsch1.senden(dialogcopy2);
                        eing_botsch1.senden(dialogtasten2);
                        loop = FALSE;
                        retwert = 7;
                        break;
                    case TALL :
                        copyall = TRUE;
                    case TENTER :
                        nae.homereq = TRUE;
                        loop = FALSE;
                        taste_los();
                        eing_botsch1.senden(dialogcopy2);
                        eing_botsch1.senden(dialogtasten2);
                        /* EasyCODE ( 0 
                        Sanduhr starten */
                        while ( eing_botsch1.busycheck() )
                        {
                        }
                        /* EasyCODE < */
                        screen.sanduhrreq_set(160,160); // sanduhrtest einschalten AK
                        /* EasyCODE > */
                        /* EasyCODE ) */
                        break;
                    default:
                        eing_botsch1.senden(BELLBELL);
                        break;
                    }
                }
            }
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE < */
            // Datei existiert nicht
            // neu anlegen
            /* EasyCODE > */
        }
        if ( retwert == 0 )
        {
            if ( (out=fopen(ziel,"wb"))==NULL )
            {
                // Zieldatei kann nicht
                // geoeffnet werden
                fclose(out);
                sd.SdFileClose();
                retwert = 3;
            }
            else
            {
                if ( sd.SdFileOpen(quelle, READ) == 0 )
                {
                    /* EasyCODE ( 0 
                    Daten kopieren */
                    for ( u=0; u<length; u+=RWLAENGE )
                    {
                        if ( u+RWLAENGE>length )
                        {
                            i = length - u;
                        }
                        else
                        {
                            i = RWLAENGE;
                        }
                        if ( sd.SdFileRead(daten, i) == 0 )
                        {
                            for ( j=0; j<i; j++ )
                            {
                                fputc(daten[j], out);
                                if ( feof(out) || ferror(out) )
                                {
                                    retwert = 5;
                                    break;
                                }
                            }
                            if ( retwert != 0 )
                            {
                                break;
                            }
                        }
                        else
                        {
                            retwert = 4;
                            break;
                        }
                    }
                    /* EasyCODE ) */
                    fclose(out);
                    sd.SdFileClose();
                    if ( retwert != 0 )
                    {
                        remove(ziel);
                    }
                }
                else
                {
                    // Quelldatei kann nicht
                    // geoeffnet werden
                    fclose(out);
                    sd.SdFileClose();
                    retwert = 2;
                }
            }
        }
        else
        {
            //Abbruch
            sd.SdFileClose();
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned char KlasseProgverw::ReadCharFromSd (unsigned char *quelle, unsigned char *ziel,
                                              unsigned int anzahl, unsigned int position) */
/* EasyCODE F */
/* Dieses UP Kopiert eine bestimmte Anzahl von Zeichen aus einer Datei auf der SD-Karte nach Ziel.
   Uebergabeparameter:
      Laufwerk,Pfad und Dateiname als String in Quelle, Zeiger auf Ziel, Anzahl der zu 
      lesenden Zeichen, Leseposition in der Datei;
   Rueckmeldung
      0 - kein Fehler
      1 - Quelle-Lesefehler, kann nicht geoeffnet werden
      2 - Quelle-Lesefehler
 */

unsigned char KlasseProgverw::ReadCharFromSd(unsigned char *quelle, unsigned char *ziel,
                                             unsigned int anzahl, unsigned int position)
{
    unsigned char loop;
    unsigned char retwert=0;
    unsigned int u, i, j;
    if ( sd.SdFileOpen(quelle, READ) == 0 )
    {
        while ( eing_botsch1.busycheck() )
        {
        }
        sprintf (eing.varstring, "%s%u\r%s", SEEK_P1, position, M_SEEK_SET);
        eing_botsch1.senden (eing.varstring);
        while ( !keyb.KartenmodusMerker() )
        {
        }
        keyb.KartenmodusMerker_res();
        teing1.rsz();
        teing1.sz(T1SEK);
        i = 0;
        loop = TRUE;
        while ( loop )
        {
            if ( sdbuf.readbyte() == 0 )
            {
                if ( i == 0 )
                {
                    if ( sdbuf.data() == FILE_REF_OK )
                    {
                        // Cursor kann 
                        // positioniert werden
                        // (Handle OK)
                        i++;
                    }
                    else
                    {
                        // Fehler
                        retwert = 2;
                        loop = FALSE;
                    }
                }
                else
                {
                    if ( sdbuf.data() == SEEK_OK )
                    {
                        // Cursor ist 
                        // positioniert
                        retwert = 0;
                        loop = FALSE;
                    }
                    else
                    {
                        // Fehler
                        retwert = 2;
                        loop = FALSE;
                    }
                }
            }
            else
            {
                // kein
                // Zeichen
                // da
                if ( teing1.tz() )
                {
                    // Fehler
                    retwert = 2;
                    loop = FALSE;
                }
            }
        }
        if ( retwert == 0 )
        {
            if ( sd.SdFileRead(ziel, anzahl) == 0 )
            {
            }
            else
            {
                retwert = 2;
            }
        }
        sd.SdFileClose();
    }
    else
    {
        // Quelldatei kann nicht
        // geoeffnet werden
        sd.SdFileClose();
        retwert = 1;
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned char KlasseProgverw::del(void) */
/* EasyCODE F */
/* Dieses UP managed das Loeschen von Files
   Uebergabeparameter:
      keine

   R+ckmeldung
      0 - kein Fehler
      1 - Abbruch, weil Funktion verworfen oder Fehler
*/

unsigned char KlasseProgverw::del(void)
{
    unsigned int i, j, k;
    unsigned char loop;
    unsigned char delall = FALSE;
    /* EasyCODE ( 0 
    Laufvariablen festlegen
      j = Schleifenbegrenzung
      i = Schleifenz+hler
      k = index f+r pnrarray */
    switch ( dirkey )
    {
    case 1 :
        j = 1;
        k = dirindex[aktdrive];
        break;
    default:
        if ( dirindex[aktdrive] == dirindexblock[aktdrive] )
        {
            /* EasyCODE ( 0 
            nur 1 Datei markiert */
            j = (dirindex[aktdrive]-dirindexblock[aktdrive]+1);
            k = dirindex[aktdrive];
            /* EasyCODE ) */
        }
        else
        {
            if ( dirindex[aktdrive] > dirindexblock[aktdrive] )
            {
                /* EasyCODE ( 0 
                Block markiert,
                Anfang = Blockindex */
                j = (dirindex[aktdrive]-dirindexblock[aktdrive]+1);
                k = dirindexblock[aktdrive];
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                Block markiert,
                Anfang = dirindex */
                j = (dirindexblock[aktdrive]-dirindex[aktdrive]+1);
                k = dirindex[aktdrive];
                /* EasyCODE ) */
            }
        }
        break;
    }
    i = 0;
    /* EasyCODE ) */
    eing_botsch1.senden(dialogfenster);
    sprintf (eing.varstring, dialogtasten0);
    strcat (eing.varstring, dialogtasten1);
    eing_botsch1.senden(eing.varstring);
    do{
        switch ( dirkey )
        {
        case 1 :
            /* EasyCODE ( 0 
            quellpath festlegen */
            if ( aktdrive == LINKS )
            {
                /* EasyCODE ( 0 
                Flashdisk */
                switch ( filedir[LINKS][dirindex[LINKS]] )
                {
                case 0 // KONF
                :
                    sprintf(quellpath,"C:KFPORTAL.BIN");
                    break;
                case 1 // MDAT
                :
                    sprintf(quellpath,"C:MDPORTAL");
                    break;
                default:
                    sprintf(quellpath,"C:NIX");
                    break;
                }
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                SD-Karte */
                switch ( filedir[RECHTS][dirindex[RECHTS]] )
                {
                case 0 // KONF
                :
                    sprintf(quellpath,"KFPORTAL.BIN");
                    break;
                case 1 // MDAT
                :
                    sprintf(quellpath,"MDPORTAL");
                    break;
                default:
                    sprintf(quellpath,"NIX");
                    break;
                }
                /* EasyCODE ) */
            }
            /* EasyCODE ) */
            break;
        default:
            /* EasyCODE ( 0 
            quellpath festlegen */
            if ( aktdrive == LINKS )
            {
                /* EasyCODE ( 0 
                Flashdisk */
                sprintf(quellpath,"%s%04u",datdir,(unsigned int)pnrdir[aktdrive][k]);
                /* EasyCODE ) */
            }
            else
            {
                /* EasyCODE ( 0 
                SD-Karte */
                sprintf(quellpath,"%04u",(unsigned int)pnrdir[aktdrive][k]);
                /* EasyCODE ) */
            }
            /* EasyCODE ) */
            break;
        }
        /* EasyCODE ( 0 
        File loeschen */
        if ( delall )
        {
            /* EasyCODE ( 0 
            File loeschen */
            if ( (aktdrive == RECHTS) && ((physdrive == DRIVE_SD) || (physdrive == DRIVE_LAN)) )
            {
                sprintf(eing.varstring,dialogdel0,quellpath);
                eing_botsch1.senden(eing.varstring);
                if ( sd.SdFileDelete(quellpath) != 0 )
                {
                    // File kann nicht geloescht werden
                    // ev. Schreibschutz
                    koor.eing_err = 14;
                    error_res();
                }
                else
                {
                    deldiraktuallisieren(k);
                }
            }
            else
            {
                switch ( filedel(quellpath) )
                {
                case 1 // File kann nicht geoeffnet werden
                :
                    koor.eing_err = 12;
                    error_res();
                    break;
                case 2 // File kann nicht geloescht werden
                       // ev. Schreibschutz
                :
                    koor.eing_err = 14;
                    error_res();
                    break;
                default:
                    deldiraktuallisieren(k);
                    break;
                }
            }
            /* EasyCODE ) */
        }
        else
        {
            //eing_botsch1.senden(dialogfenster);
            sprintf(eing.varstring,tz->proghin2,quellpath);
            eing_botsch1.senden(eing.varstring);
            loop = TRUE;
            while ( loop )
            {
                switch ( getkey() )
                {
                case TESC :
                    taste_los();
                    eing_botsch1.senden(dialogcopy2);
                    eing_botsch1.senden(dialogtasten2);
                    return 1;
                case TNEXT :
                    taste_los();
                    eing_botsch1.senden(dialogcopy2);
                    k++;
                    loop = FALSE;
                    break;
                case TALL :
                    taste_los();
                    delall = TRUE;
                    eing_botsch1.senden(dialogtasten2);
                case TENTER :
                    taste_los();
                    eing_botsch1.senden(dialogcopy2);
                    loop = FALSE;
                    /* EasyCODE ( 0 
                    File loeschen */
                    if ( (aktdrive == RECHTS) && ((physdrive == DRIVE_SD) || (physdrive == DRIVE_LAN)) )
                    {
                        if ( sd.SdFileDelete(quellpath) != 0 )
                        {
                            // File kann nicht geloescht werden
                            // ev. Schreibschutz
                            koor.eing_err = 14;
                            error_res();
                        }
                        else
                        {
                            deldiraktuallisieren(k);
                        }
                    }
                    else
                    {
                        switch ( filedel(quellpath) )
                        {
                        case 1 // File kann nicht geoeffnet werden
                        :
                            koor.eing_err = 12;
                            error_res();
                            break;
                        case 2 // File kann nicht geloescht werden
                                                                                                                                   // ev. Schreibschutz
                        :
                            koor.eing_err = 14;
                            error_res();
                            break;
                        default:
                            deldiraktuallisieren(k);
                            break;
                        }
                    }
                    /* EasyCODE ) */
                    break;
                default:
                    eing_botsch1.senden(BELLBELL);
                    break;
                }
            }
        }
        /* EasyCODE ) */
        //k++;
        //i++;
        j--;
    }while ( i < j );
    return 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned char KlasseProgverw::filedel(unsigned char * quelle) */
/* EasyCODE F */
unsigned char KlasseProgverw::filedel(unsigned char * quelle)
/* Dieses UP Kopiert ein File von Quelle nach Ziel, dabei werden verschiedene
   Pr+fungen durchgef+hrt.
   _bergabeparameter:
      Laufwerk,Pfad und Dateiname als String in Quelle z.B. "A:\\KONF3590.BIN"

   R+ckmeldung
      0 - kein Fehler
      1 - Programm nicht auf Quelle
      2 - Zugriff verweigert (Quelle-Schreibfehler)
*/
{
    sprintf(eing.varstring,dialogdel0,quelle);
    eing_botsch1.senden(eing.varstring);
    if ( remove(quelle) )
    {
        if ( errno==ENOENT )
        {
            /* EasyCODE < */
            // File nicht im gefunden
            /* EasyCODE > */
            return 1;
        }
        else
        {
            /* EasyCODE < */
            // Zugriff verweigert (Schreibfehler)
            /* EasyCODE > */
            return 2;
        }
    }
    else
    {
        return 0;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::deldiraktuallisieren */
/* EasyCODE F */
// Dieses UP loescht die Programmparameter in dem aktuellen Drivepuffer und zieht die nachfolgenden vor.

void KlasseProgverw::deldiraktuallisieren(unsigned int index)
{
    unsigned int j, k;
    switch ( dirkey )
    {
    case 1 :
        for ( k = index; k < DIRZ_MAX; k++ )
        {
            if ( (k < 2) && (filedir[aktdrive][k+1] != 0xff) )
            {
                filedir[aktdrive][k] = filedir[aktdrive][k+1];
            }
            else
            {
                filedir[aktdrive][k] = 0xff;
            }
        }
        fileindexmax[aktdrive] = fileindexmax[aktdrive] - 1;
        break;
    default:
        for ( k = index; k <= MAX_PROGANZAHL; k++ )
        {
            if ( (k < MAX_PROGANZAHL) && (pnrdir[aktdrive][k+1] != 0xffff) )
            {
                pnrdir[aktdrive][k] = pnrdir[aktdrive][k+1];
                for ( j = 0; j < 11; j++ )
                {
                    komentardir[aktdrive][k][j] = komentardir[aktdrive][k+1][j];
                }
            }
            else
            {
                pnrdir[aktdrive][k] = 0xffff;
                for ( j = 0; j < 11; j++ )
                {
                    komentardir[aktdrive][k][j] = 0;
                }
            }
        }
        dirindexmax[aktdrive] = dirindexmax[aktdrive] - 1;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned char KlasseProgverw::sdcardformat (char *verzeichnis) */
/* EasyCODE F */
/*
Aufgabe:     SD-Karte formatieren und Unterverzeichnis der Maschine anlegen.
             Ist Unterverzeichnis schon da, dann alle Dateien loeschen.
             Ist Unterverzeichnis nichtda, anlegen.
             Kann Unterverzeichnis nicht angelegt werden, dann formatieren.

Uebergabewert:   char * verzeichnis der Maschine
Returnwert:      0 - Formatieren fertig
                 1 - Formatieren gescheitert
                 2 - keine Karte gesteckt
                 3 - Karte nicht richtig gesteckt
                 4 - Karte ist schreibgeschuetzt
                 5 - Fehler
                 6 - Formatieren abgebrochen
 */
unsigned char KlasseProgverw::sdcardformat (char *verzeichnis)
{
    int Rueckgabe = 0;
    int i;
    char prstring[20];
    unsigned char loopflag;
    /* EasyCODE ( 0 
    Aktion anzeigen */
    eing_botsch1.senden(dialogfenster);
    eing_botsch1.senden(dialogformat1);
    eing_botsch1.senden(tz->eing_cmd3);
    eing_botsch1.senden(dialogformat2);
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Bestaetigung */
    loopflag = TRUE;
    while ( loopflag )
    {
        switch ( getkey() )
        {
        case TESC :
            loopflag = FALSE;
            Rueckgabe = 6;
            break;
        case TENTER :
            loopflag = FALSE;
            eing_botsch1.senden(dialogformat3);
            /* EasyCODE ( 0 
            Sanduhr starten */
            while ( eing_botsch1.busycheck() )
            {
            }
            /* EasyCODE < */
            screen.sanduhrreq_set(160,160); // sanduhrtest einschalten AK
            /* EasyCODE > */
            /* EasyCODE ) */
            break;
        default:
            eing_botsch1.senden(BELLBELL);
            break;
        }
    }
    /* EasyCODE ) */
    if ( Rueckgabe == 0 )
    {
        switch ( sd.SdInitMedia() )
        {
        case 0  // fertig
        :
            /* EasyCODE ( 0 
            ok, weiter mit Statusabfrage */
            switch ( sd.SdCardStatus() )
            {
            case 0 // SD-Karte kann geschrieben werden
            :
                switch ( sd.SdChangeDir (verzeichnis) )
                {
                case 0 :
                    /* EasyCODE ( 0 
                    Richtige SD-Karte gesteckt, --> alle Programme loeschen */
                    if ( sd.SdFileDelete ("*.*") != 2 )
                    {
                    }
                    else
                    {
                        /* EasyCODE < */
                        // Fehler beim loeschen
                        // --> Karte formatieren
                        /* EasyCODE > */
                        switch ( sd.SdFormatCard () )
                        {
                        case 0 // Fertig
                        :
                            break;
                        case 1 // Formatieren gescheitert
                        :
                            Rueckgabe = 1;
                            break;
                        default:
                            // Fehler
                            Rueckgabe = 5;
                            break;
                        }
                    }
                    /* EasyCODE ) */
                    break;
                case 1 :
                    /* EasyCODE ( 0 
                    Directory nicht da, --> Directory anlegen */
                    switch ( sd.SdMakeDir (verzeichnis) )
                    {
                    case 0 // fertig
                    :
                        break;
                    default:
                        /* EasyCODE < */
                        // Fehler beim Directory anlegen
                        // --> Karte formatieren
                        /* EasyCODE > */
                        switch ( sd.SdFormatCard () )
                        {
                        case 0 // Fertig
                        :
                            break;
                        case 1 // Formatieren gescheitert
                        :
                            Rueckgabe = 1;
                            break;
                        default:
                            // Fehler
                            Rueckgabe = 5;
                            break;
                        }
                        break;
                    }
                    /* EasyCODE ) */
                    break;
                default:
                    // Fehler
                    Rueckgabe = 5;
                    break;
                }
                break;
            case 1 // Karte ist schreibgeschuetzt
            :
                Rueckgabe = 4;
                break;
            case 2 // Karte nicht richtig gesteckt
            :
                Rueckgabe = 3;
                break;
            case 3 // keine Karte gesteckt
            :
                Rueckgabe = 2;
                break;
            default:
                // Fehler
                Rueckgabe = 5;
                break;
            }
            /* EasyCODE ) */
            break;
        case 1  // keine SD-Karte gesteckt
        :
            Rueckgabe = 2;
            break;
        default:
            // Fehler
            Rueckgabe = 5;
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::creatmdat(void) */
/* EasyCODE F */
// Dieses UP legt das File MDPORTAL an und speichert die Daten aus saf->md
// in diesem File.
//
// Rueckmeldung:
//    keine, wenn o.k. ist das File MDPORTAL auf Flash, sonst nicht.

void KlasseProgverw::creatmdat(void)
{
    unsigned int i;
    unsigned char * mdptr;
    /* EasyCODE ( 0 
    Aktion anzeigen */
    eing_botsch1.senden(dialogfenster);
    /* EasyCODE - */
    eing_botsch1.senden(ESCS "F001" ESCS "=010129""CREATE FILE MDPORTAL");
    /* EasyCODE ) */
    if ( (f1=fopen("C:MDPORTAL","wb"))==0 )
    {
        fclose (f1);
        remove("C:MDPORTAL");
        koor.eing_err = 16;
        error_res();
    }
    else
    {
        mdptr=(unsigned char *)&saf->md;
        putc (MASCHDAT_KENNUNG,f1);
        for ( i=0; i<sizeof(saf->md); i++ )
        {
            putc(*mdptr,f1);
            mdptr++;
        }
        if ( ferror(f1) || (fclose(f1)!=0) )
        {
            fclose (f1);
            remove("C:MDPORTAL");
            koor.eing_err = 17;
            error_res();
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::loadmdat(void) */
/* EasyCODE F */
// Dieses UP liest das File MDPORTAL und speichert die Daten in saf->md.
//
// Rueckmeldung:
//    keine.

void KlasseProgverw::loadmdat(void)
{
    /* EasyCODE < */
    unsigned int i, maschdat_kennung, mdanzahl;
    unsigned char * mdptr;
    struct masch_daten mdat;
    /* EasyCODE > */
    if ( (f1=fopen("C:MDPORTAL","rb"))==NULL )
    {
        fclose (f1);
        koor.eing_err = 16;
        error_res();
    }
    else
    {
        maschdat_kennung=getc(f1);
        switch ( maschdat_kennung )
        {
        case MASCHDAT_KENNUNG :
            mdanzahl=sizeof(saf->md);
            break;
        default:
            fclose(f1);
            koor.eing_err = 18;
            error_res();
            return ;
        }
        mdptr=(unsigned char *)&mdat;
        for ( i=0; i<mdanzahl; i++ )
        {
            *mdptr=getc(f1);
            mdptr++;
        }
        switch ( maschdat_kennung )
        {
        case MASCHDAT_KENNUNG :
            break;
        }
        if ( feof(f1) || ferror(f1) //|| (getc(f1)!=EOF)
           )
        {
            fclose(f1);
            koor.eing_err = 19;
            error_res();
        }
        else
        {
            fclose(f1);
            RTKDisableInterrupts();
            saf->md=mdat;
            RTKEnableInterrupts();
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseProgverw::removemdat(void) */
/* EasyCODE F */
// Dieses UP loescht das File MDPORTAL.
//
// Rueckmeldung:
//    keine.

void KlasseProgverw::removemdat(void)
{
    remove("C:MDPORTAL");
}

/* EasyCODE ) */
/* EasyCODE ) */
