/* EasyCODE V8 */
/* EasyCODE ( 0 
eingport */
/* EasyCODE ( 0 
Beschreibung */
/* Datei: eingport.c       Klasse: 3590  Portal     Ersteller: Halfmann  EWE 2856

In dieser Datei befinden sich folgende Teile der Betriebsart Eingabe :

- Verwaltungsteil mit :
   - Power on
   - Ruhezustand / Anfang
   - Exit
   - Eingabe  Grundzustand
   - Programmnummern - Wahl

- Direktfunktionen :
   - Schlitten links/rechts
   - Schlitten vor/zurueck
   - N"hmotor drehen

- Men_funktionen :
   - Info
   - Programmverwaltung
   - Parameter-Eingabe
   - Programmieren
     (eigene Datei PROGPORT.C)
   - Laender-Einstellungen
   - Funktionen sperren/freigeben
   - Service

- Initialisierungen :
   - Kaltinitialisierung
   - Warminitialisierung

*/
/* EasyCODE ) */
#include "portal.h"

FILE *f1;
KlasseEing eing (tdini [EingIndex]);        // Objektdefinition
/* EasyCODE ( 0 
parameter structure array */
// Parameter specifications
// Max group Nr. = 9
 //st_parameter* parameter;
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseEing */
/* EasyCODE ( 0 
void KlasseEing::eingtask(void) */
/* EasyCODE F */
void KlasseEing::eingtask(void)
{
    unsigned int i;
    int parametertemp;
    unsigned char matdicke_changed_loc;
    switch ( key )
    {
    case E_PON :
        key = poweron();
        break;
    case E_RUHE :
        key = ruhe();
        break;
    case E_EXIT :
        key = exit();
        break;
    case E_GST :
        key = grundzustand();
        break;
    case E_PNRW :
        key = pnrwahl();
        break;
    case E_VPNR :
        /* EasyCODE = */
    case E_SEQ :
        key = seqwahl();
        break;
    case E_PROG :
        prog.key = 0;
        key = prog.grundzustand();
        break;
    case E_PROGVER :
        matdicke_changed_loc = saf->md.matdicke;  // SW010
        key=progverw.grundzustand();
        progverw.loadmdat();
        progverw.removemdat();
        saf->konfig_gueltig = 0;
        saf->mnp_gueltig = 0;
        saf->inkrkor_gueltig = 0;
        saf->nadelp_gueltig = 0;
        konfinit();
        com1_com3_init();
        if ( saf->md.matdicke != matdicke_changed_loc )
        {
            // SW010
            nae.naetab_boot();
            koor.eing_err = 51;
        }
        if ( (saf->md.Nadeldreh1ein <= 1)  &&  (saf->md.Nadeldreh2ein <= 1)  /* SW015  */ )
        {
            // Bei der MDAT Version 4 stand 1  1 drin, darum <= 1.
            saf->md.Nadeldreh1ein = 3;   // Notfall Daten bei alter SD Karte
            saf->md.Nadeldreh2ein = 47;  // Notfall Daten bei alter SD Karte
        }
        break;
    case E_PAR :
        key = par.grundzustand();
        /* EasyCODE ( 0 
        Parameteraenderungen, die weitere
        Aktionen zur Folge haben, behandeln */
        /* EasyCODE ( 0 
        Parameter 201 bis 208, Konfiguration */
        if ( par.IsChanged (202) )
        {
            com1_com3_init();
        }
        if ( par.IsChanged (206) || par.IsChanged (207) )
        {
            nup.bitomo_init();
        }
        if ( par.IsChanged(201) || par.IsChanged(202) ||
             par.IsChanged(203) || par.IsChanged(204) ||
             par.IsChanged(205) || par.IsChanged(206) ||
             par.IsChanged(207) || par.IsChanged(208) )
        {
            saf->konfig_gueltig=0x55;
            konfinit();
            par.DelChangedInfo(201);
            par.DelChangedInfo(202);
            par.DelChangedInfo(203);
            par.DelChangedInfo(204);
            par.DelChangedInfo(205);
            par.DelChangedInfo(206);
            par.DelChangedInfo(207);
            par.DelChangedInfo(208);
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Parameter 109, Klammerueberwachung */
        if ( par.IsChanged(109) )
        {
            par.DelChangedInfo(109);
            nae.homereq = TRUE;
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Parameter 105, Fw-Empfindlichkeit */
        if ( par.IsChanged(105) )
        {
            nminf.pawini();
            par.DelChangedInfo(105);
            teing1.rsz();
            teing1.sz(saf->quick_delayzt);
        }
        else
        {
            teing1.lz();
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Parameter 131, Materialdicke/Gewicht */
        if ( par.IsChanged(131) )
        {
            par.DelChangedInfo(131);
            nae.naetab_boot();
            koor.eing_err = 51;
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Parameter 142, Tastenpieps */
        if ( par.IsChanged(142) )
        {
            par.DelChangedInfo(142);
            if ( saf->md.tastenpiep )
            {
                eing_botsch1.senden(ESCS "RR3");
            }
            else
            {
                eing_botsch1.senden(ESCS "RR1");
            }
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Parameter 145, verkuerzter Anfangsfaden */
        if ( par.IsChanged(145) )
        {
            par.DelChangedInfo(145);
            if ( saf->md.kurzerAnffad )
            {
                saf->md.wedelnfussoben = FALSE;
            }
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Parameter 301, NIS */
        if ( par.IsChanged(301) )
        {
            while ( nminf.befehl!=0 )
            {
                if ( nminf.fehler!=0 )
                {
                    break;
                }
            }
            if ( nminf.fehler!=0 )
            {
                koor.eing_err = 41;
                eing.errpar = nminf.fehler;
                key = E_RUHE;
                break;
            }
            else
            {
                while ( !teing1.tz() )
                {
                }
                parametertemp=(saf->nadelp >> 1) + ((long)saf->md.nisvograd*MAXPOS/360);
                if ( parametertemp >= MAXPOS )
                {
                    parametertemp -= MAXPOS;
                }
                nminf.parameter = parametertemp;
                nminf.befehl=3;
                while ( nminf.befehl!=0 )
                {
                    if ( nminf.fehler!=0 )
                    {
                        break;
                    }
                }
                if ( nminf.fehler!=0 )
                {
                    koor.eing_err = 41;
                    eing.errpar = nminf.fehler;
                    key = E_RUHE;
                    break;
                }
                else
                {
                    par.DelChangedInfo(301);
                    teing1.rsz();
                    teing1.sz(saf->quick_delayzt);
                }
            }
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Parameter 303, REF2 */
        if ( par.IsChanged(303) )
        {
            while ( nminf.befehl!=0 )
            {
                if ( nminf.fehler!=0 )
                {
                    break;
                }
            }
            if ( nminf.fehler!=0 )
            {
                koor.eing_err = 41;
                eing.errpar = nminf.fehler;
                key = E_RUHE;
                break;
            }
            else
            {
                while ( !teing1.tz() )
                {
                }
                parametertemp = (saf->nadelp >> 1) + ((long)saf->md.ref2*MAXPOS/360);
                if ( parametertemp >= MAXPOS )
                {
                    parametertemp -= MAXPOS;
                }
                nminf.parameter = parametertemp;
                nminf.befehl=4;
                while ( nminf.befehl!=0 )
                {
                    if ( nminf.fehler!=0 )
                    {
                        break;
                    }
                }
                if ( nminf.fehler!=0 )
                {
                    koor.eing_err = 41;
                    eing.errpar = nminf.fehler;
                    key = E_RUHE;
                    break;
                }
                else
                {
                    par.DelChangedInfo(303);
                    teing1.rsz();
                    teing1.sz(saf->quick_delayzt);
                }
            }
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Parameter 502, Schneiddrehzahl */
        if ( par.IsChanged(502) )
        {
            while ( nminf.befehl!=0 )
            {
                if ( nminf.fehler!=0 )
                {
                    break;
                }
            }
            if ( nminf.fehler!=0 )
            {
                koor.eing_err = 41;
                eing.errpar = nminf.fehler;
                key = E_RUHE;
                break;
            }
            else
            {
                while ( !teing1.tz() )
                {
                }
                nminf.parameter = saf->md.cutgsw;
                nminf.befehl = 17;
                while ( nminf.befehl!=0 )
                {
                    if ( nminf.fehler!=0 )
                    {
                        break;
                    }
                }
                if ( nminf.fehler!=0 )
                {
                    koor.eing_err = 41;
                    eing.errpar = nminf.fehler;
                    key = E_RUHE;
                    break;
                }
                else
                {
                    par.DelChangedInfo(502);
                    teing1.rsz();
                    teing1.sz(saf->quick_delayzt);
                }
            }
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Parametergruppe 700, NM-Parameter */
        for ( i=1; i<= 19; i++ )
        {
            if ( par.IsChanged(700+i) )
            {
                par.DelChangedInfo(700+i);
                i = 0xffff;
                break;
            }
        }
        if ( (i==0xffff) || par.IsChanged(725) )
        {
            for ( i=0; i<QP_ANZAHL; i++ )
            {
                while ( nminf.befehl!=0 )
                {
                    if ( nminf.fehler!=0 )
                    {
                        break;
                    }
                }
                if ( nminf.fehler!=0 )
                {
                    koor.eing_err = 41;
                    eing.errpar = nminf.fehler;
                    key = E_RUHE;
                    break;
                }
                else
                {
                    while ( !teing1.tz() )
                    {
                    }
                    nminf.parameternr = quick_paramnr [i];
                    nminf.parameter = saf->md.quick_paramwert [i];
                    nminf.befehl = 10;
                    while ( nminf.befehl!=0 )
                    {
                        if ( nminf.fehler!=0 )
                        {
                            break;
                        }
                    }
                    if ( nminf.fehler!=0 )
                    {
                        koor.eing_err = 41;
                        eing.errpar = nminf.fehler;
                        key = E_RUHE;
                        break;
                    }
                    else
                    {
                        teing1.rsz();
                        teing1.sz(saf->quick_delayzt);
                    }
                }
            }
        }
        /* EasyCODE ) */
        /* EasyCODE ) */
        break;
    case E_INFO :
        key=info.grundzustand();
        break;
    case E_LAENDER :
        key=laender.grundzustand();
        break;
    case E_FUNKT :
        key=funkt.grundzustand();
        break;
    case E_SM :
        key=direkt.movesm();
        break;
    case E_REF :
        key=direkt.smref(TRUE);
        break;
    case E_REF2 :
        key=direkt.smref(FALSE);
        break;
    case E_NM :
        key=direkt.nm();
        break;
    case E_SERV :
        serv.key = S_START;
        /* EasyCODE - */
        key=serv.grundzustand();
        break;
    case E_STI :
        key = stichl();
        break;
    case E_GSW :
        key = drehzahl();
        break;
    case E_UFW :
        key = ufw();
        break;
    case E_RFID :
        key=direkt.rfid();
        break;
    case E_BARCODE :
        //key=direkt.bctest();
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseEing::poweron(void) */
/* EasyCODE F */
unsigned char KlasseEing::poweron(void)
{
    if ( koor.eingstop() )
    {
        stopb=TRUE;
        return E_PON;
    }
    else
    {
        stopb=FALSE;
        ems_map (prog_handle[0],0,0);
        /* EasyCODE ( 0 
        Tastenreaktion und spez. Zeichen */
        while ( eing_botsch1.busycheck() )
        {
        }
        if ( saf->md.tastenpiep )
        {
            eing_botsch1.senden(ESCS "RR3");
        }
        else
        {
            eing_botsch1.senden(ESCS "RR1");
        }
        // schoenere 0
        //eing_botsch1.senden(ESCS "C13000003C666666666666663C000000");
        /* EasyCODE ) */
        ponrdy=TRUE;
        while ( !koor.eingstop() )
        {
        }
        return E_RUHE  // Ruhe, Anfang
       ;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseEing::ruhe(void) */
/* EasyCODE F */
unsigned char KlasseEing::ruhe(void)
{
    if ( koor.eingstop() )
    {
        stopb=TRUE;
    }
    else
    {
        stopb=FALSE;
        ioact=FALSE;
        switch ( auftrag )
        {
        case 0 :
            /* EasyCODE = */
        case 1 :
            /* EasyCODE ( 0 
            Exit */
            return E_EXIT  // Exit nach PowerOn
           ;
            /* EasyCODE ) */
        case 5 :
            /* EasyCODE = */
        case 2 :
            /* EasyCODE ( 0 
            PIN -> Grundzustand Eingabe */
            switch ( fkeytst(3) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                return E_GST // frei -> Grundzustand
               ;
            case 2 // gesperrt
            :
                return E_EXIT // gesperrt -> Exit
               ;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            PIN -> PNR - Wahl */
            switch ( fkeytst(0) )
            {
            case 0 :
                /* EasyCODE = */
            case 1 :
                return E_PNRW // frei -> PNR - Wahl
               ;
            case 2 :
                return E_EXIT // gesperrt -> Exit
               ;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case 4 :
            /* EasyCODE ( 0 
            PIN -> Programmieren */
            switch ( fkeytst(4) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                return E_PROG // frei -> Programmieren
               ;
            case 2 // gesperrt
            :
                return E_EXIT // gesperrt -> Exit
               ;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case 6 :
            /* EasyCODE ( 0 
            PIN -> Stichlaengen - Wahl */
            switch ( fkeytst(1) )
            {
            case 0 :
                /* EasyCODE = */
            case 1 :
                return E_STI // frei -> Stichlaengen - Wahl
               ;
            case 2 :
                return E_EXIT // gesperrt -> Exit
               ;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case 7 :
            /* EasyCODE ( 0 
            PIN -> Drehzahl - Wahl */
            switch ( fkeytst(2) )
            {
            case 0 :
                /* EasyCODE = */
            case 1 :
                return E_GSW // frei -> Drehzahl - Wahl
               ;
            case 2 :
                return E_EXIT // gesperrt -> Exit
               ;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case 8 :
            /* EasyCODE ( 0 
            PIN -> UFW-Unterfadenzaehler ruecksetzen */
            switch ( fkeytst(18) )
            {
            case 0 :
                /* EasyCODE = */
            case 1 :
                return E_UFW // frei -> Drehzahl - Wahl
               ;
            case 2 :
                return E_EXIT // gesperrt -> Exit
               ;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        default:
            break;
        }
    }
    return E_RUHE // sonst -> Ruhezustand
   ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseEing::exit(void) */
/* EasyCODE F */
unsigned char KlasseEing::exit(void)
{
    /* EasyCODE < */
    struct p_abschstruct *hlpptr;
    unsigned char loopflag;
    /* EasyCODE > */
    switch ( auftrag )
    {
    case 5 :
        /* EasyCODE < */
        // keine exit Bearbeitung, Koordinator im PowerOn
        /* EasyCODE > */
        break;
    default:
        /* EasyCODE ( 0 
        Konfig und Nadel OT pruefen */
        if ( saf->konfig_gueltig!=0x55 )
        {
            koor.eing_err = 5;
            error_res();
            return E_GST;
        }
        else
        {
            if ( saf->nadelp_gueltig!=0x55 )
            {
                koor.eing_err = 6;
                error_res();
                return E_GST;
            }
        }
        /* EasyCODE ) */
        if ( nae.firsthome )
        {
        }
        else
        {
            /* EasyCODE ( 0 
            Nullpunkte pruefen */
            if ( saf->mnp_gueltig!=0x55 )
            {
                koor.eing_err = 1;
                error_res();
                return E_GST;
            }
            else
            {
                if ( saf->hfp_gueltig!=0x55 )
                {
                    koor.eing_err = 34;
                    error_res();
                    return E_GST;
                }
            }
            /* EasyCODE ) */
        }
        /* EasyCODE ( 0 
        Programme laden */
        loopflag = TRUE;
        while ( loopflag  /* SW015 */ )
        {
            /* EasyCODE ( 0 
            Programme laden */
            loopflag = FALSE;  // SW015
            if ( saf->md.jigctren != 2 )
            {
                switch ( di.readprog_to_ems () )
                {
                case 6 :
                    // nur Stichdaten
                case 0 :
                    /* EasyCODE < */
                    // ok
                    /* EasyCODE > */
                    break;
                case 1 :
                    /* EasyCODE ( 0 
                    Programm unvollstaendig */
                    if ( /* EasyCODE < */
                    (hlpptr = eing.pup.find_absch(AB_STDST,(struct p_abschstruct *)(ndatanf+GPROGNR))) == NULL
                    /* EasyCODE > */
                     )
                    {
                        errpar =
                           konv.konvert(ndatanf,0,MAXPROGLEN,&errpar2);
                    }
                    else
                    {
                        errpar =
                           konv.konvert(ndatanf,hlpptr->par1,MAXPROGLEN,&errpar2);
                    }
                    if ( errpar != 0 )
                    {
                        koor.eing_err = 30;
                        error_res();
                        if ( auftrag == 2 )
                        {
                            return E_GST;
                        }
                        else
                        {
                            return E_PNRW;
                        }
                    }
                    else
                    {
                        switch ( di.storeprog () )
                        {
                        case 0 :
                            loopflag = TRUE;  // SW015
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
                        break;
                    }
                    /* EasyCODE ) */
                case 2 :
                    // Programm zu gross
                    koor.eing_err = 43;
                    auftrag = 3;
                    error_res();
                    if ( auftrag == 2 )
                    {
                        return E_GST;
                    }
                    else
                    {
                        return E_PNRW;
                    }
                case 3 :
                    // Programm nicht vorhanden
                    koor.eing_err = 44;
                    auftrag = 3;
                    error_res();
                    if ( auftrag == 2 )
                    {
                        return E_GST;
                    }
                    else
                    {
                        return E_PNRW;
                    }
                case 4 :
                    // Flash-Lesefehler oder Prog. fehlerhaft
                    koor.eing_err = 45;
                    auftrag = 3;
                    error_res();
                    if ( auftrag == 2 )
                    {
                        return E_GST;
                    }
                    else
                    {
                        return E_PNRW;
                    }
                case 5 :
                    // Programm nicht fr diese Maschine
                    koor.eing_err = 47;
                    auftrag = 3;
                    error_res();
                    if ( auftrag == 2 )
                    {
                        return E_GST;
                    }
                    else
                    {
                        return E_PNRW;
                    }
                default:
                    return E_PNRW;
                }
            }
            /* EasyCODE ) */
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Schlingstichwinkel ausrechnen  SW015 */
        saf->fadschein_winkel1 = ((float) (saf->md.Nadeldreh1ein)/180)*M_PI;  // Bogenmass Quadrant 1   
        saf->fadschaus_winkel1 = ((float) (saf->md.Nadeldreh1ein + 4)/180)*M_PI;  // Bogenmass Quadrant 1   
        
        saf->fadschein_winkel2 = ((float) (saf->md.Nadeldreh2ein)/180)*M_PI;  // Bogenmass Quadrant 4   
        saf->fadschaus_winkel2 = ((float) (saf->md.Nadeldreh2ein + 5)/180)*M_PI;  // Bogenmass Quadrant 4 
        //eing_botsch1.senden(BELLBELL);
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Nadelanzahl pruefen */
        if ( ((*(ndatanf+KPROGNR)==3)&&(saf->md.nadelanzahl==2)) ||
             ((*(ndatanf+KPROGNR)==4)&&(saf->md.nadelanzahl==1)) )
        {
            // Programm zu gross
            koor.eing_err = 35;
            auftrag = 3;
            error_res();
            if ( auftrag == 2 )
            {
                return E_GST;
            }
            else
            {
                return E_PNRW;
            }
        }
        else
        {
            break;
        }
        /* EasyCODE ) */
    }
    auftrag = 0;
    while ( !koor.eingstop() )
    {
    }
    return E_RUHE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseEing::grundzustand(void) */
/* EasyCODE F */
unsigned char KlasseEing::grundzustand(void)
{
    /* EasyCODE ( 0 
    Bilschirmvorlage anzeigen */
    if ( taste_los() == 0xff )
    {
        return E_RUHE;
    }
    else
    {
        while ( eing_botsch1.busycheck() )
        {
            ;
        }
        eing_botsch1.senden(BV003);
        if ( saf->jigctrf != 1 )
        {
            sprintf (varstring, touchdel, TRFIDS);
            sprintf (varstring1, pikdel, PRFID);
            strcat (varstring, varstring1);
            eing_botsch1.senden(varstring);
            /* EasyCODE - */
            //Sw005-1
            /* EasyCODE - */
            //bctest() blieb bei der Portal haengen derzeit ohne Anzeige BV039!
            /* EasyCODE_STRUCTURED_COMMENT_START */
            /*
            if ( saf->jigctrf == STATBARC )
            {
                eing_botsch1.senden(bctaste2);
            }
            */
            /* EasyCODE_STRUCTURED_COMMENT_END */
}
    }
    /* EasyCODE ) */
    while ( TRUE )
    {
        switch ( getkey() )
        {
        case STOPANF :
            return E_RUHE;
        case TENDE :
            return E_EXIT;
        case TPROG :
            /* EasyCODE ( 0 
            Pin - Abfrage -> Programmieren */
            switch ( fkeytst(4) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                return E_PROG;
            case 2 // gesperrt
            :
                return E_GST;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case TPROGVERW :
            /* EasyCODE ( 0 
            Pin - Abfrage -> Progverwaltung */
            switch ( fkeytst(5) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                mdloadflag = FALSE;
                /* EasyCODE - */
                konfloadflag = FALSE;
                return E_PROGVER;
            case 2 // gesperrt
            :
                return E_GST;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case TPAR :
            /* EasyCODE ( 0 
            Pin - Abfrage -> Parameter */
            switch ( fkeytst(6) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                par.key = 1;
                return E_PAR;
            case 2 // gesperrt
            :
                return E_GST;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case TINFO :
            return E_INFO;
        case TLAENDEREINST :
            /* EasyCODE ( 0 
            Pin - Abfrage -> Laendereinstellungen */
            switch ( fkeytst(21) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                laender.key = 0;
                return E_LAENDER;
            case 2 // gesperrt
            :
                return E_GST;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case TFKTSPERREN :
            /* EasyCODE ( 0 
            Pin - Abfrage -> Funktionen sperren/freigeben */
            switch ( fkeytst(22) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                funkt.key = 0;
                return E_FUNKT;
            case 2 // gesperrt
            :
                return E_GST;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case TSCHLITTEN :
            /* EasyCODE ( 0 
            Pin - Abfrage -> Schrittmotor verfahren */
            switch ( fkeytst(25) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                return E_SM;
            case 2 // gesperrt
            :
                return E_GST;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case TREF :
            /* EasyCODE ( 0 
            Pin - Abfrage -> Schlittentest */
            switch ( fkeytst(28) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                return E_REF;
            case 2 // gesperrt
            :
                return E_GST;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case TRFID :
            /* EasyCODE ( 0 
            Pin - Abfrage -> RFID */
            switch ( fkeytst(31) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                return E_RFID;
            case 2 // gesperrt
            :
                return E_GST;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case TBARCODE :
            /* EasyCODE ( 0 
            Pin - Abfrage -> Barcode */
            switch ( fkeytst(31) )
            {
            case 0 // frei
            :
            case 1 // frei (nach PIN eingabe)
            :
                return E_BARCODE;
            case 2 // gesperrt
            :
                return E_GST;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case TNAEHMOTOR :
            /* EasyCODE ( 0 
            Pin - Abfrage -> Naehmotor */
            switch ( fkeytst(29) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                return E_NM;
            case 2 // gesperrt
            :
                return E_GST;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        case TSERVICE :
            /* EasyCODE ( 0 
            Pin - Abfrage -> Service */
            switch ( fkeytst(23) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                serv.key = 0;
                return E_SERV // frei -> Service
               ;
            case 2 // gesperrt
            :
                return E_GST;
            default:
                return E_RUHE // Stopanforderung -> Ruhezustand
               ;
            }
            /* EasyCODE ) */
        default:
            /* EasyCODE ( 0 
            noch nicht fertig */
            while ( eing_botsch1.busycheck() )
            {
                ;
            }
            eing_botsch1.senden(BELLBELL);
            /* EasyCODE ) */
            break;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseEing::pnrwahl(void) */
/* EasyCODE F */
unsigned char KlasseEing::pnrwahl(void)
{
    while ( TRUE )
    {
        pargetval.auftrag = 4;
        pargetval.altwert = saf->md.pnr;
        pargetval.string = pnrs;
        pargetval.sonderstring = pnrp2s;
        pargetval.pikto = pnrps;
        pargetval.maxwert = PNRMAX;
        pargetval.minwert = PNRMIN;
        pargetval.kommastellen = 0;
        pargetval.sonder2string = pnrkomms;
        switch ( getvalue_eing() )
        {
        case 0 // ENTER
        :
            saf->md.pnr = pargetval.wert;
            saf->md.prsel = PNR;
            nae.homereq = TRUE;
            if ( saf->md.jigctren == 2 )
            {
                saf->md.jigctren = 1;
            }
            return E_EXIT;
        case 1 // ESC
        :
            return E_EXIT;
        case 2 // Stopanf
        :
            return E_RUHE;
        case 3 // Sondertaste
        :
            switch ( pargetval.taste )
            {
            case TSEQ :
                return E_SEQ;
            case TVPNR :
                return E_VPNR;
            case TPAUTO :
                /* EasyCODE ( 0 
                automatische Prog.nr.wahl */
                if ( saf->jigctrf )
                {
                    saf->md.jigctren = 2;
                    saf->md.prsel = PNR;
                    nae.homereq = TRUE;
                    return E_EXIT;
                }
                else
                {
                    eing_botsch1.senden(BELLBELL);
                    break;
                }
                /* EasyCODE ) */
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseEing::seqwahl(void) */
/* EasyCODE F */
unsigned char KlasseEing::seqwahl(void)
{
    unsigned char loopflag, loopflag1;
    unsigned char string[6];
    unsigned char seqarrs[PNRINDEX_MAX+1][6];
    unsigned char seqcursorarrs[PNRINDEX_MAX+1][6];
    unsigned char tempindex, i, j, ziffer, firstziffer, secondziffer, thirdziffer;
    unsigned int temppnr, dummy;
    unsigned int temparray[SEQVPNR_MAX+1][PNRINDEX_MAX+1];
    while ( TRUE )
    {
        while ( eing_botsch1.busycheck() )
        {
            ;
        }
        if ( taste_los() == 0xff )
        {
            return E_RUHE;
        }
        else
        {
            eing_botsch1.senden(pnrloeschs);
            eing_botsch1.senden(seqprogtastes);
            eing_botsch1.senden(zulloesch);
            sprintf(eing.varstring1,zulis,(long)SEQVPNR_MIN,(long)SEQVPNR_MAX);
            eing_botsch1.senden(eing.varstring1);
            if ( key == E_SEQ )
            {
                eing_botsch1.senden(seqps);
                temppnr = saf->md.seq;
                for ( j = SEQVPNR_MIN; j <=SEQVPNR_MAX; j++ )
                {
                    for ( i = PNRINDEX_MIN; i <= PNRINDEX_MAX; i++ )
                    {
                        temparray[j][i] = saf->md.seq_array[j][i];
                    }
                }
            }
            else
            {
                eing_botsch1.senden(vpnrps);
                temppnr = saf->md.vpnr;
                for ( j = SEQVPNR_MIN; j <=SEQVPNR_MAX; j++ )
                {
                    for ( i = PNRINDEX_MIN; i <= PNRINDEX_MAX; i++ )
                    {
                        temparray[j][i] = saf->md.vpnr_array[j][i];
                    }
                }
            }
            loopflag = TRUE;
            while ( loopflag )
            {
                /* EasyCODE ( 0 
                Anzeige Gruppenprogramme */
                while ( eing_botsch1.busycheck() )
                {
                    ;
                }
                sprintf(eing.varstring,pnrs,temppnr);
                eing_botsch1.senden(eing.varstring);
                for ( i = 0; i <= PNRINDEX_MAX; i++ )
                {
                    if ( temparray[temppnr][i] == 0xffff )
                    {
                        sprintf(string,seqprogs[0],"    ");
                    }
                    else
                    {
                        sprintf(string,seqprogs[1],temparray[temppnr][i]);
                    }
                    sprintf(seqarrs[i],string);
                }
                while ( eing_botsch1.busycheck() )
                {
                    ;
                }
                sprintf(eing.varstring,seqpnrvars,seqarrs[0],seqarrs[1],seqarrs[2],seqarrs[3],seqarrs[4],
                seqarrs[5],seqarrs[6],seqarrs[7]);
                eing_botsch1.senden(eing.varstring);
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Tasten abfragen */
                switch ( getkey() )
                {
                case STOPANF :
                    return E_RUHE;
                case TESC :
                    return E_EXIT;
                case TSEQ //eigentlich TPNR
                :
                    saf->md.pnr = 0;
                    return E_PNRW;
                case TVPNR :
                    if ( key == E_SEQ )
                    {
                        return E_VPNR;
                    }
                    else
                    {
                        return E_SEQ;
                    }
                case TPAUTO :
                    /* EasyCODE ( 0 
                    automatische Prog.nr.wahl */
                    if ( saf->jigctrf )
                    {
                        saf->md.jigctren = 2;
                        saf->md.prsel = PNR;
                        nae.homereq = TRUE;
                        return E_EXIT;
                    }
                    else
                    {
                        eing_botsch1.senden(BELLBELL);
                        break;
                    }
                    /* EasyCODE ) */
                case TENTER :
                    if ( temparray[temppnr][0] == 0xffff )
                    {
                        // Sequenz noch nicht programmiert
                        loopflag = FALSE;
                        break;
                    }
                    else
                    {
                        if ( key == E_SEQ )
                        {
                            // Sequenz bereits programmiert
                            saf->md.prsel = SEQ;
                            saf->md.seq = temppnr;
                        }
                        else
                        {
                            // Sequenz bereits programmiert
                            saf->md.prsel = VPNR;
                            saf->md.vpnr = temppnr;
                        }
                        saf->md.pnrindex = 0;
                        saf->md.next_pnrindex = 0;
                        nae.homereq = TRUE;
                        if ( saf->md.jigctren == 2 )
                        {
                            saf->md.jigctren = 0;
                        }
                        return E_EXIT;
                    }
                case TSEQPROG :
                    while ( keyb.taste() != 0xff )
                    {
                    }
                    loopflag = FALSE;
                    break;
                case TNULL :
                    temppnr = 0;
                    break;
                case T1 :
                    temppnr = 1;
                    break;
                case T2 :
                    temppnr = 2;
                    break;
                case T3 :
                    temppnr = 3;
                    break;
                case T4 :
                    temppnr = 4;
                    break;
                case T5 :
                    temppnr = 5;
                    break;
                case T6 :
                    temppnr = 6;
                    break;
                case T7 :
                    temppnr = 7;
                    break;
                case T8 :
                    temppnr = 8;
                    break;
                case T9 :
                    temppnr = 9;
                    break;
                case TSCRUP :
                    if ( temppnr < SEQVPNR_MAX )
                    {
                        temppnr = temppnr + 1;
                        while ( eing_botsch1.busycheck() )
                        {
                            ;
                        }
                        sprintf(eing.varstring,pnrs,temppnr);
                        eing_botsch1.senden(eing.varstring);
                        /* EasyCODE ( 0 
                        Anzeige Gruppenprogramme */
                        while ( eing_botsch1.busycheck() )
                        {
                            ;
                        }
                        sprintf(eing.varstring,pnrs,temppnr);
                        eing_botsch1.senden(eing.varstring);
                        for ( i = 0; i <= PNRINDEX_MAX; i++ )
                        {
                            if ( temparray[temppnr][i] == 0xffff )
                            {
                                sprintf(string,seqprogs[0],"    ");
                            }
                            else
                            {
                                sprintf(string,seqprogs[1],temparray[temppnr][i]);
                            }
                            sprintf(seqarrs[i],string);
                        }
                        while ( eing_botsch1.busycheck() )
                        {
                            ;
                        }
                        sprintf(eing.varstring,seqpnrvars,seqarrs[0],seqarrs[1],seqarrs[2],seqarrs[3],seqarrs[4],
                        seqarrs[5],seqarrs[6],seqarrs[7]);
                        eing_botsch1.senden(eing.varstring);
                        /* EasyCODE ) */
                        /* EasyCODE ( 0 
                        Rollen */
                        teing1.rsz();
                        teing1.sz(saf->slowdelay);
                        while ( !teing1.tz() )
                        {
                            if ( koor.eingstop() || (keyb.taste()!=TSCRUP) )
                            {
                                break;
                            }
                        }
                        if ( teing1.tz() )
                        {
                            while ( keyb.taste()==TSCRUP )
                            {
                                if ( koor.eingstop() )
                                {
                                    break;
                                }
                                else
                                {
                                    if ( temppnr < SEQVPNR_MAX )
                                    {
                                        temppnr = temppnr +1;
                                    }
                                    while ( eing_botsch1.busycheck() )
                                    {
                                        ;
                                    }
                                    sprintf(eing.varstring,pnrs,temppnr);
                                    eing_botsch1.senden(eing.varstring);
                                    /* EasyCODE ( 0 
                                    Anzeige Gruppenprogramme */
                                    while ( eing_botsch1.busycheck() )
                                    {
                                        ;
                                    }
                                    sprintf(eing.varstring,pnrs,temppnr);
                                    eing_botsch1.senden(eing.varstring);
                                    for ( i = 0; i <= PNRINDEX_MAX; i++ )
                                    {
                                        if ( temparray[temppnr][i] == 0xffff )
                                        {
                                            sprintf(string,seqprogs[0],"    ");
                                        }
                                        else
                                        {
                                            sprintf(string,seqprogs[1],temparray[temppnr][i]);
                                        }
                                        sprintf(seqarrs[i],string);
                                    }
                                    while ( eing_botsch1.busycheck() )
                                    {
                                        ;
                                    }
                                    sprintf(eing.varstring,seqpnrvars,seqarrs[0],seqarrs[1],seqarrs[2],seqarrs[3],seqarrs[4],
                                    seqarrs[5],seqarrs[6],seqarrs[7]);
                                    eing_botsch1.senden(eing.varstring);
                                    /* EasyCODE ) */
                                    teing1.rsz();
                                    teing1.sz(saf->slowtime);
                                    do{
                                        if ( koor.eingstop() || (keyb.taste()!=TSCRUP) )
                                        {
                                            break;
                                        }
                                    }while ( !teing1.tz() );
                                }
                            }
                        }
                        /* EasyCODE ) */
                    }
                    break;
                case TSCRDOWN :
                    if ( temppnr > SEQVPNR_MIN )
                    {
                        temppnr = temppnr -1;
                        while ( eing_botsch1.busycheck() )
                        {
                            ;
                        }
                        sprintf(eing.varstring,pnrs,temppnr);
                        eing_botsch1.senden(eing.varstring);
                        /* EasyCODE ( 0 
                        Anzeige Gruppenprogramme */
                        while ( eing_botsch1.busycheck() )
                        {
                            ;
                        }
                        sprintf(eing.varstring,pnrs,temppnr);
                        eing_botsch1.senden(eing.varstring);
                        for ( i = 0; i <= PNRINDEX_MAX; i++ )
                        {
                            if ( temparray[temppnr][i] == 0xffff )
                            {
                                sprintf(string,seqprogs[0],"    ");
                            }
                            else
                            {
                                sprintf(string,seqprogs[1],temparray[temppnr][i]);
                            }
                            sprintf(seqarrs[i],string);
                        }
                        while ( eing_botsch1.busycheck() )
                        {
                            ;
                        }
                        sprintf(eing.varstring,seqpnrvars,seqarrs[0],seqarrs[1],seqarrs[2],seqarrs[3],seqarrs[4],
                        seqarrs[5],seqarrs[6],seqarrs[7]);
                        eing_botsch1.senden(eing.varstring);
                        /* EasyCODE ) */
                        /* EasyCODE ( 0 
                        Rollen */
                        teing1.rsz();
                        teing1.sz(saf->slowdelay);
                        while ( !teing1.tz() )
                        {
                            if ( koor.eingstop() || (keyb.taste()!=TSCRDOWN) )
                            {
                                break;
                            }
                        }
                        if ( teing1.tz() )
                        {
                            while ( keyb.taste()==TSCRDOWN )
                            {
                                if ( koor.eingstop() )
                                {
                                    break;
                                }
                                else
                                {
                                    if ( temppnr > SEQVPNR_MIN )
                                    {
                                        temppnr = temppnr -1 ;
                                    }
                                    while ( eing_botsch1.busycheck() )
                                    {
                                        ;
                                    }
                                    sprintf(eing.varstring,pnrs,temppnr);
                                    eing_botsch1.senden(eing.varstring);
                                    /* EasyCODE ( 0 
                                    Anzeige Gruppenprogramme */
                                    while ( eing_botsch1.busycheck() )
                                    {
                                        ;
                                    }
                                    sprintf(eing.varstring,pnrs,temppnr);
                                    eing_botsch1.senden(eing.varstring);
                                    for ( i = 0; i <= PNRINDEX_MAX; i++ )
                                    {
                                        if ( temparray[temppnr][i] == 0xffff )
                                        {
                                            sprintf(string,seqprogs[0],"    ");
                                        }
                                        else
                                        {
                                            sprintf(string,seqprogs[1],temparray[temppnr][i]);
                                        }
                                        sprintf(seqarrs[i],string);
                                    }
                                    while ( eing_botsch1.busycheck() )
                                    {
                                        ;
                                    }
                                    sprintf(eing.varstring,seqpnrvars,seqarrs[0],seqarrs[1],seqarrs[2],seqarrs[3],seqarrs[4],
                                    seqarrs[5],seqarrs[6],seqarrs[7]);
                                    eing_botsch1.senden(eing.varstring);
                                    /* EasyCODE ) */
                                    teing1.rsz();
                                    teing1.sz(saf->slowtime);
                                    do{
                                        if ( koor.eingstop() || (keyb.taste()!=TSCRDOWN) )
                                        {
                                            break;
                                        }
                                    }while ( !teing1.tz() );
                                }
                            }
                        }
                        /* EasyCODE ) */
                    }
                    break;
                case TCLEAR :
                    temppnr = 0;
                    break;
                default:
                    while ( eing_botsch1.busycheck() )
                    {
                        ;
                    }
                    eing_botsch1.senden(BELLBELL);
                    break;
                }
                /* EasyCODE ) */
            }
            /* EasyCODE ( 0 
            Sequenz programmieren */
            /* EasyCODE ( 0 
            Anzeige */
            tempindex = 0;
            if ( temparray[temppnr][tempindex] == 0xffff )
            {
                temparray[temppnr][tempindex] = 0;
            }
            if ( key == E_SEQ )
            {
                eing_botsch1.senden(seqpikleistes[0]);
            }
            else
            {
                eing_botsch1.senden(seqpikleistes[2]);
            }
            eing_botsch1.senden(seqpikleistes[1]);
            eing_botsch1.senden(zulloesch);
            sprintf(eing.varstring1,zulis,(long)PNRMIN,(long)PNRMAX);
            eing_botsch1.senden(eing.varstring1);
            loopflag = TRUE;
            firstziffer = TRUE;
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Sequenzprogramme eingeben */
            while ( loopflag )
            {
                for ( i = 0; i <= PNRINDEX_MAX; i++ )
                {
                    if ( temparray[temppnr][i] == 0xffff )
                    {
                        sprintf(string,seqprogs[0],"    ");
                    }
                    else
                    {
                        sprintf(string,seqprogs[1],temparray[temppnr][i]);
                    }
                    sprintf(seqarrs[i],string);
                }
                while ( eing_botsch1.busycheck() )
                {
                    ;
                }
                sprintf(eing.varstring,seqpnrvars,seqarrs[0],seqarrs[1],seqarrs[2],seqarrs[3],seqarrs[4],
                seqarrs[5],seqarrs[6],seqarrs[7]);
                eing_botsch1.senden(eing.varstring);
                for ( i = 0; i <= PNRINDEX_MAX; i++ )
                {
                    if ( tempindex == i )
                    {
                        sprintf(string,seqprogs[0],"<");
                    }
                    else
                    {
                        sprintf(string,seqprogs[0]," ");
                    }
                    sprintf(seqcursorarrs[i],string);
                }
                while ( eing_botsch1.busycheck() )
                {
                    ;
                }
                sprintf(eing.varstring1,seqcursorvars,seqcursorarrs[0],seqcursorarrs[1],seqcursorarrs[2],seqcursorarrs[3],seqcursorarrs[4],
                seqcursorarrs[5],seqcursorarrs[6],seqcursorarrs[7]);
                eing_botsch1.senden(eing.varstring1);
                loopflag1 = TRUE;
                dummy = temparray[temppnr][tempindex];
                /* EasyCODE ( 0 
                Sequenz eingeben */
                while ( loopflag1 )
                {
                    switch ( ziffer = getkey() )
                    {
                    case STOPANF :
                        return E_RUHE;
                    case TENDE :
                        /* EasyCODE ( 0 
                        TENDE */
                        if ( dummy <= PNRMAX )
                        {
                            temparray[temppnr][tempindex] = dummy;
                            saf->md.pnrindex = 0;
                            saf->md.next_pnrindex = 0;
                            if ( key == E_SEQ )
                            {
                                saf->md.seq = temppnr;
                                saf->md.prsel = SEQ;
                                for ( j = SEQVPNR_MIN; j <=SEQVPNR_MAX; j++ )
                                {
                                    for ( i = PNRINDEX_MIN; i <= PNRINDEX_MAX; i++ )
                                    {
                                        saf->md.seq_array[j][i] = temparray[j][i];
                                    }
                                }
                            }
                            else
                            {
                                saf->md.vpnr = temppnr;
                                saf->md.prsel = VPNR;
                                for ( j = SEQVPNR_MIN; j <=SEQVPNR_MAX; j++ )
                                {
                                    for ( i = PNRINDEX_MIN; i <= PNRINDEX_MAX; i++ )
                                    {
                                        saf->md.vpnr_array[j][i] = temparray[j][i];
                                    }
                                }
                            }
                            nae.homereq = TRUE;
                            if ( saf->md.jigctren == 2 )
                            {
                                saf->md.jigctren = 0;
                            }
                            return E_EXIT;
                        }
                        else
                        {
                            while ( eing_botsch1.busycheck() )
                            {
                                ;
                            }
                            eing_botsch1.senden(BELLBELL);
                            break;
                        }
                        /* EasyCODE ) */
                    case TESC :
                        /* EasyCODE ( 0 
                        TESC */
                        if ( taste_los() == 0xff )
                        {
                            return E_RUHE;
                        }
                        else
                        {
                            firstziffer = TRUE;
                            loopflag1 = FALSE;
                            break;
                        }
                        /* EasyCODE ) */
                    case TLEFT :
                        /* EasyCODE ( 0 
                        TSCRLEFT */
                        if ( taste_los() == 0xff )
                        {
                            return E_RUHE;
                        }
                        else
                        {
                            if ( dummy <= PNRMAX )
                            {
                                temparray[temppnr][tempindex] = dummy;
                                if ( tempindex > 0 )
                                {
                                    tempindex = tempindex -1;
                                    firstziffer = TRUE;
                                }
                                loopflag1 = FALSE;
                            }
                            else
                            {
                                while ( eing_botsch1.busycheck() )
                                {
                                    ;
                                }
                                eing_botsch1.senden(BELLBELL);
                            }
                            break;
                        }
                        /* EasyCODE ) */
                    case TRIGHT :
                        /* EasyCODE ( 0 
                        TSCRRIGHT */
                        if ( taste_los() == 0xff )
                        {
                            return E_RUHE;
                        }
                        else
                        {
                            if ( dummy <= PNRMAX )
                            {
                                temparray[temppnr][tempindex] = dummy;
                                if ( tempindex < PNRINDEX_MAX )
                                {
                                    tempindex = tempindex +1;
                                    firstziffer = TRUE;
                                    if ( temparray[temppnr][tempindex] == 0xffff )
                                    {
                                        temparray[temppnr][tempindex] = 0;
                                    }
                                }
                                loopflag1 = FALSE;
                            }
                            else
                            {
                                while ( eing_botsch1.busycheck() )
                                {
                                    ;
                                }
                                eing_botsch1.senden(BELLBELL);
                            }
                            break;
                        }
                        /* EasyCODE ) */
                    case TINS :
                        /* EasyCODE ( 0 
                        TINS */
                        if ( taste_los() == 0xff )
                        {
                            return E_RUHE;
                        }
                        else
                        {
                            if ( dummy <= PNRMAX )
                            {
                                temparray[temppnr][tempindex] = dummy;
                                if ( temparray[temppnr][PNRINDEX_MAX] == 0xffff )
                                {
                                    for ( i = tempindex; temparray[temppnr][i] != 0xffff; i++ )
                                    {
                                    }
                                    for ( ; i > tempindex; i-- )
                                    {
                                        temparray[temppnr][i] = temparray[temppnr][i-1];
                                    }
                                    firstziffer = TRUE;
                                    temparray[temppnr][tempindex] = 0;
                                }
                                loopflag1 = FALSE;
                            }
                            else
                            {
                                while ( eing_botsch1.busycheck() )
                                {
                                    ;
                                }
                                eing_botsch1.senden(BELLBELL);
                            }
                            break;
                        }
                        /* EasyCODE ) */
                    case TDEL :
                        /* EasyCODE ( 0 
                        TDEL */
                        if ( taste_los() == 0xff )
                        {
                            return E_RUHE;
                        }
                        else
                        {
                            if ( tempindex == PNRINDEX_MAX )
                            {
                                temparray[temppnr][tempindex] = 0xffff;
                                tempindex = tempindex -1;
                            }
                            else
                            {
                                if ( temparray[temppnr][tempindex+1] == 0xffff )
                                {
                                    temparray[temppnr][tempindex] = 0xffff;
                                    if ( tempindex == 0 )
                                    {
                                        saf->md.pnr = 0;
                                        return E_PNRW;
                                    }
                                    else
                                    {
                                        tempindex = tempindex -1;
                                    }
                                }
                                else
                                {
                                    for ( i = tempindex; i < PNRINDEX_MAX; i++ )
                                    {
                                        temparray[temppnr][i] = temparray[temppnr][i+1];
                                    }
                                    temparray[temppnr][i] = 0xffff;
                                }
                            }
                            firstziffer = TRUE;
                            loopflag1 = FALSE;
                            break;
                        }
                        /* EasyCODE ) */
                    case TNULL :
                        /* EasyCODE = */
                    case T1 :
                        /* EasyCODE = */
                    case T2 :
                        /* EasyCODE = */
                    case T3 :
                        /* EasyCODE = */
                    case T4 :
                        /* EasyCODE = */
                    case T5 :
                        /* EasyCODE = */
                    case T6 :
                        /* EasyCODE = */
                    case T7 :
                        /* EasyCODE = */
                    case T8 :
                        /* EasyCODE = */
                    case T9 :
                        switch ( ziffer )
                        {
                        case TNULL :
                            ziffer  = 0;
                            break;
                        case T1 :
                            ziffer  = 1;
                            break;
                        case T2 :
                            ziffer  = 2;
                            break;
                        case T3 :
                            ziffer  = 3;
                            break;
                        case T4 :
                            ziffer  = 4;
                            break;
                        case T5 :
                            ziffer  = 5;
                            break;
                        case T6 :
                            ziffer  = 6;
                            break;
                        case T7 :
                            ziffer  = 7;
                            break;
                        case T8 :
                            ziffer  = 8;
                            break;
                        default:
                            // T9
                            ziffer  = 9;
                            break;
                        }
                        if ( firstziffer )
                        {
                            dummy= ziffer;
                            firstziffer = FALSE;
                            secondziffer = TRUE;
                        }
                        else
                        {
                            if ( secondziffer )
                            {
                                dummy = dummy * 10 + ziffer;
                                secondziffer = FALSE;
                                thirdziffer = TRUE;
                            }
                            else
                            {
                                if ( thirdziffer )
                                {
                                    dummy = dummy * 10 + ziffer;
                                    thirdziffer = FALSE;
                                }
                                else
                                {
                                    dummy = dummy * 10 + ziffer;
                                    firstziffer = TRUE;
                                }
                            }
                        }
                        while ( eing_botsch1.busycheck() )
                        {
                            ;
                        }
                        sprintf(eing.varstring1,seqpnrs[tempindex],dummy);
                        /* EasyCODE - */
                        eing_botsch1.senden(eing.varstring1);
                        break;
                    case TSCRUP :
                        if ( dummy < PNRMAX )
                        {
                            dummy = dummy + 1;
                            firstziffer = TRUE;
                            while ( eing_botsch1.busycheck() )
                            {
                                ;
                            }
                            sprintf(eing.varstring1,seqpnrs[tempindex],dummy);
                            eing_botsch1.senden(eing.varstring1);
                        }
                        break;
                    case TSCRDOWN :
                        if ( dummy > PNRMIN )
                        {
                            dummy = dummy - 1;
                            firstziffer = TRUE;
                            while ( eing_botsch1.busycheck() )
                            {
                                ;
                            }
                            sprintf(eing.varstring1,seqpnrs[tempindex],dummy);
                            eing_botsch1.senden(eing.varstring1);
                        }
                        break;
                    case TCLEAR :
                        dummy = 0;
                        /* EasyCODE - */
                        firstziffer = TRUE;
                        while ( eing_botsch1.busycheck() )
                        {
                            ;
                        }
                        sprintf(eing.varstring1,seqpnrs[tempindex],dummy);
                        /* EasyCODE - */
                        eing_botsch1.senden(eing.varstring1);
                        break;
                    default:
                        while ( eing_botsch1.busycheck() )
                        {
                            ;
                        }
                        eing_botsch1.senden(BELLBELL);
                        break;
                    }
                }
                /* EasyCODE ) */
            }
            /* EasyCODE ) */
            /* EasyCODE ) */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseEing::stichl(void) */
/* EasyCODE F */
unsigned char KlasseEing::stichl(void)
{
    /* EasyCODE < */
    struct progpar parptr;
    unsigned int stineu;
    unsigned char rueckm;
    /* EasyCODE > */
    switch ( saf->md.prsel )
    {
    case PNR :
        /* EasyCODE ( 0 
        PNR */
        rueckm = di.parprog(saf->md.pnr, &parptr, DRIVE_C);
        break;
        /* EasyCODE ) */
    case SEQ :
        /* EasyCODE ( 0 
        SEQ */
        rueckm = di.parprog(saf->md.seq_array[saf->md.seq][saf->md.pnrindex], &parptr, DRIVE_C);
        break;
        /* EasyCODE ) */
    default:
        /* EasyCODE ( 0 
        VPNR */
        rueckm = di.parprog(saf->md.vpnr_array[saf->md.vpnr][saf->md.pnrindex], &parptr, DRIVE_C);
        break;
        /* EasyCODE ) */
    }
    if ( rueckm == 3 )
    {
        koor.eing_err = 20;
        error_res();
        return E_EXIT;
    }
    else
    {
        if ( parptr.stichl == 0 )
        {
            eing_botsch1.senden(BELLBELL);
            return E_EXIT;
        }
        else
        {
            pargetval.minwert = STLG_MIN;
            pargetval.maxwert = STLG_MAX;
            pargetval.auftrag = 0;
            pargetval.altwert = parptr.stichl/10;
            pargetval.pikto = stips;
            pargetval.kommastellen = 1;
            pargetval.string = floatstr;
            switch ( getvalue_eing() )
            {
            case 0 // ENTER
            :
                stineu = pargetval.wert;
                if ( (errpar=konv.konvert(ndatanf,stineu*10,MAXPROGLEN,&errpar2)) != 0 )
                {
                    koor.eing_err = 30;
                    error_res();
                    return E_EXIT;
                }
                else
                {
                    switch ( di.storeprog () )
                    {
                    case 0 :
                        // o.k.
                        nae.homereq = TRUE;
                        return E_EXIT;
                    case 1 :
                        // Speicherueberlauf
                        koor.eing_err = 25;
                        error_res();
                        return E_EXIT;
                    default:
                        // Flash Schreibfehler
                        koor.eing_err = 26;
                        /* EasyCODE - */
                        error_res();
                        return E_EXIT;
                    }
                }
            case 1 :
                /* EasyCODE < */
                // ESC
                /* EasyCODE > */
                return E_EXIT;
            default:
                /* EasyCODE < */
                // Stoppanf
                /* EasyCODE > */
                return E_RUHE;
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseEing::drehzahl(void) */
/* EasyCODE F */
unsigned char KlasseEing::drehzahl(void)
{
    pargetval.minwert = GSWMIN;
    pargetval.maxwert = GSWMAX;
    pargetval.auftrag = 0;
    pargetval.altwert = saf->md.maxgsw;
    pargetval.pikto = gswps;
    pargetval.kommastellen = 0;
    pargetval.string = gswstr;
    switch ( getvalue_eing() )
    {
    case 0 // ENTER
    :
        saf->md.maxgsw = pargetval.wert;
        return E_EXIT;
    case 1 // ESC
    :
        return E_EXIT;
    default:
        /* EasyCODE < */
        // Stoppanf
        /* EasyCODE > */
        return E_RUHE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseEing::ufw(void) */
/* EasyCODE F */
unsigned char KlasseEing::ufw(void)
{
    pargetval.minwert = UFWVOR_MIN;
    pargetval.maxwert = UFWVOR_MAX;
    pargetval.auftrag = 1;
    pargetval.altwert = saf->md.bobcnt_wert;
    pargetval.pikto = ufwps;
    pargetval.sonderstring = ufwp2s;
    pargetval.kommastellen = 0;
    pargetval.string = ufwstr;
    switch ( getvalue_eing() )
    {
    case 0
         // ENTER
    :
        saf->md.bobcnt_wert = pargetval.wert;
        return E_EXIT;
    case 1
         // ESC
    :
        return E_EXIT;
    case 3
         // CLEAR
    :
        saf->bobcnt = 0;
        return E_EXIT;
    default:
        /* EasyCODE < */
        // Stoppanf
        /* EasyCODE > */
        return E_RUHE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseEing::kinit(void) */
/* EasyCODE F */
void KlasseEing::kinit(void)
{
    unsigned char i, j;
    
    saf->slowtime=20;
    saf->fasttime=6;
    saf->slowdelay=50;
    saf->fastdelay=200;
    saf->md.prsel = PNR;
    saf->md.pnr = 0;
    saf->md.seq = 0;
    saf->md.vpnr = 0;
    saf->md.pnrindex = PNRINDEX_MIN;
    saf->md.next_pnrindex = PNRINDEX_MIN;
    saf->md.sprache = 0;
    #ifdef buerotest
        saf->md.tastenpiep = FALSE;
    #else 
        saf->md.tastenpiep = TRUE;
    #endif 
    saf->md.kontrast = 100;
    saf->md.autoen = FALSE;
    saf->md.maxgsw = 15;
    saf->md.bobcnt_wert = 10000;
    saf->bobcnt = 0;
    par.parameterinit();
    par.KaltStart();
    saf->bcinit_flag = TRUE; //SW005-1
    /* EasyCODE ( 0 
    seq_array[ ][ ] */
    for ( j = SEQVPNR_MIN; j <=SEQVPNR_MAX; j++ )
    {
        for ( i = PNRINDEX_MIN; i <= PNRINDEX_MAX; i++ )
        {
            saf->md.seq_array[j][i] = 0xffff;
        }
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    vpnr_array[ ][ ] */
    for ( j = SEQVPNR_MIN; j <=SEQVPNR_MAX; j++ )
    {
        for ( i = PNRINDEX_MIN; i <= PNRINDEX_MAX; i++ )
        {
            saf->md.vpnr_array[j][i] = 0xffff;
        }
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    codenr_arr[ ]
    fkey_array[ ][ ] */
    saf->md.codenr_arr[0] = 0001;
    saf->md.codenr_arr[1] = 0002;
    saf->md.codenr_arr[2] = 0003;
    for ( j=0; j<FKEY_USER_MAX; j++ )
    {
        for ( i = 0; i < FKEY_FUNK_MAX; i++ )
        {
            saf->md.fkey_array[j][i] = fkey_ur[j][i];
        }
    }
    /* EasyCODE ) */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseEing::winit(void) */
/* EasyCODE F */
void KlasseEing::winit(void)
{
    par.parameterinit();
    serv.Init(&io);
    konfinit();
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseEing::konfinit(void) */
/* EasyCODE F */
void KlasseEing::konfinit(void)
{
    // Da konfinit auch nach Verlassen der Konfiguration aufgerufen werden
    // kann, loht es sich daraus eine eigene Methode zu machen.
    
    unsigned int i;
    /* EasyCODE ( 0 
    Konfigurations - Datei oeffnen und
    ggf. initialisieren */
    if ( (f1=fopen("c:kfportal.bin","r+b"))==NULL )
    {
        f1=fopen("c:kfportal.bin","w+b");
        for ( i=0; i<=FF_CHAR_MAX; i++ )
        {
            fputc(0,f1);
        }
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Konfiguration */
    if ( saf->konfig_gueltig==0x55 )
    {
        /* EasyCODE ( 0 
        RAM-Inhalt  gueltig,
        RAM-Inhalt ins FLASH schreiben */
        // RAM-Konfiguration gueltig
        // RAM-Konfiguration ins FLASH schreiben
        fseek (f1,FF_KONFIG_GUE,SEEK_SET);
        fputc (0,f1);   // ungueltig
        fputc (saf->stoffgegendrueckerf,f1);
        fputc (saf->jigctrf,f1);
        fputc (saf->variante,f1);
        fseek (f1,FF_KONFIG_GUE,SEEK_SET);
        fputc (0x55,f1);  // gueltig
        /* EasyCODE ) */
    }
    else
    {
        /* EasyCODE ( 0 
        RAM-Inhalt ungueltig,
        wenn FLASH-Inhalt gueltig, RAM-Inhalt aus FLASH holen
        sonst RAM-Inhalt setzen */
        // RAM-Konfiguration ungueltig
        // geg. RAM-Konfiguration aus FLASH holen
        fseek (f1,FF_KONFIG_GUE,SEEK_SET);
        if ( fgetc(f1)==0x55 )
        {
            // Konfiguration im FLASH gueltig
            saf->stoffgegendrueckerf=fgetc(f1);
            saf->jigctrf=fgetc(f1);
            saf->variante=fgetc(f1);
            saf->konfig_gueltig=0x55;
        }
        else
        {
            /* EasyCODE < */
            // Konfiguration im FLASH ungueltig
            saf->stoffgegendrueckerf=1;
            saf->jigctrf=0;
            saf->variante = 0;
            // Produktionsdaten loeschen
            memset (prod,0,sizeof(struct prodstruct));
            /* EasyCODE > */
        }
        /* EasyCODE ) */
    }
    /* EasyCODE ( 0 
    Schlittenposition (MNP) */
    if ( saf->mnp_gueltig==0x55 )
    {
        /* EasyCODE ( 0 
        Schlittenpos. im
        Ram gueltig */
        if ( (saf->xkor < -100) || (saf->xkor > 100) ||
             (saf->ykor < -100) || (saf->ykor > 100) )
        {
            saf->mnp_gueltig = 0;
            saf->xkor=0;
            saf->ykor=0;
        }
        else
        {
            // RAM-Nadelpos. ins FLASH schreiben
            fseek (f1,FF_MNP_GUE,SEEK_SET);
            fputc (0,f1);  // ungueltig
            fputc (saf->xkor,f1);
            fputc (saf->ykor,f1);
            fseek (f1,FF_MNP_GUE,SEEK_SET);
            fputc (0x55,f1);  // gueltig
        }
        /* EasyCODE ) */
    }
    else
    {
        fseek (f1,FF_MNP_GUE,SEEK_SET);
        if ( fgetc(f1)==0x55 )
        {
            /* EasyCODE ( 0 
            Schlittenpos. im Flash gueltig */
            // Nadelpos. im FLASH gueltig
            saf->xkor=fgetc(f1);
            saf->ykor=fgetc(f1);
            if ( (saf->xkor < -100) || (saf->xkor > 100) ||
                 (saf->ykor < -100) || (saf->ykor > 100) )
            {
                saf->xkor=0;
                saf->ykor=0;
            }
            else
            {
                saf->mnp_gueltig=0x55;
            }
            /* EasyCODE ) */
        }
        else
        {
            // Schlittenpos. im FLASH
            //   ungueltig
            // Schlittenpos. setzen
            saf->xkor=0;
            saf->ykor=0;
        }
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Inkrement-Korrektur */
    if ( saf->inkrkor_gueltig==0x55 )
    {
        /* EasyCODE ( 0 
        RAM-Inkr.-Korrektur gueltig */
        if ( (saf->inkr_korx < INKRKOR_MIN) ||
             (saf->inkr_korx > INKRKOR_MAX) ||
             (saf->inkr_kory < INKRKOR_MIN) ||
             (saf->inkr_kory > INKRKOR_MAX) )
        {
            saf->inkr_korx=10;
            saf->inkr_korx=10;
        }
        if ( (saf->schabl_kor < SCHABLKOR_MIN) ||
             (saf->schabl_kor > SCHABLKOR_MAX) )
        {
            saf->schabl_kor=0;
        }
        // RAM-Inkr.-Korrektur ins FLASH
        //   schreiben
        fseek (f1,FF_INKRKOR_GUE,SEEK_SET);
        fputc (0,f1);  // ungueltig
        fputc (saf->inkr_korx,f1);
        fputc (saf->inkr_kory,f1);
        fputc (saf->schabl_kor,f1);
        fseek (f1,FF_INKRKOR_GUE,SEEK_SET);
        fputc (0x55,f1);  // gueltig
        /* EasyCODE ) */
    }
    else
    {
        // RAM-Inkr.-Korrektur ungueltig
        // geg. RAM-Inkr.-Korrektur aus FLASH holen
        fseek (f1,FF_INKRKOR_GUE,SEEK_SET);
        if ( fgetc(f1)==0x55 )
        {
            /* EasyCODE ( 0 
            Inkr.-Korrektur im FLASH gueltig */
            saf->inkr_korx=fgetc(f1);
            saf->inkr_kory=fgetc(f1);
            if ( (saf->inkr_korx < INKRKOR_MIN) ||
                 (saf->inkr_korx > INKRKOR_MAX) ||
                 (saf->inkr_kory < INKRKOR_MIN) ||
                 (saf->inkr_kory > INKRKOR_MAX) )
            {
                saf->inkr_korx=10;
                saf->inkr_kory=10;
            }
            saf->schabl_kor=fgetc(f1);
            if ( (saf->schabl_kor < SCHABLKOR_MIN) ||
                 (saf->schabl_kor > SCHABLKOR_MAX) )
            {
                saf->schabl_kor=0;
            }
            saf->inkrkor_gueltig=0x55;
            /* EasyCODE ) */
        }
        else
        {
            // Inkr.-Korrektur im FLASH ungueltig
            // Inkr.-Korrektur setzen
            saf->inkr_korx=10;
            saf->inkr_kory=10;
            saf->schabl_kor = 0;
            saf->inkrkor_gueltig=0x55;
        }
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Nadelposition (Oberteil) */
    if ( saf->nadelp_gueltig==0x55 )
    {
        /* EasyCODE ( 0 
        RAM-Inhalt  gueltig,
        RAM-Inhalt ins FLASH schreiben */
        /* EasyCODE < */
        fseek (f1,FF_NADELPOS_GUE,SEEK_SET);
        fputc (0,f1);  // ungueltig
        putw (saf->nadelp,f1);
        fseek (f1,FF_NADELPOS_GUE,SEEK_SET);
        fputc (0x55,f1);  // gueltig
        /* EasyCODE > */
        /* EasyCODE ) */
    }
    else
    {
        /* EasyCODE ( 0 
        RAM-Inhalt ungueltig,
        wenn FLASH-Inhalt g_ltig, RAM-Inhalt aus FLASH holen
        sonst RAM-Inhalt setzen */
        fseek (f1,FF_NADELPOS_GUE,SEEK_SET);
        if ( fgetc(f1)==0x55 )
        {
            // Nadelpos. im FLASH gueltig
            saf->nadelp=getw(f1);
            saf->nadelp_gueltig=0x55;
        }
        else
        {
            // Nadelpos. im FLASH ungueltig
            // Nadelpos. setzen
            saf->nadelp=0;
        }
        /* EasyCODE ) */
    }
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Huepferfuss-Position */
    if ( saf->hfp_gueltig==0x55 )
    {
        /* EasyCODE ( 0 
        RAM-Inhalt  gueltig,
        RAM-Inhalt ins FLASH schreiben */
        fseek (f1,FF_HF_GUE,SEEK_SET);
        fputc (0,f1);  // ungueltig
        putw (saf->hfkor,f1);
        fseek (f1,FF_HF_GUE,SEEK_SET);
        fputc (0x55,f1);  // gueltig
        /* EasyCODE ) */
    }
    else
    {
        /* EasyCODE ( 0 
        RAM-Inhalt ungueltig,
        wenn FLASH-Inhalt gueltig, RAM-Inhalt aus FLASH holen
        sonst RAM-Inhalt setzen */
        fseek (f1,FF_HF_GUE,SEEK_SET);
        if ( fgetc(f1)==0x55 )
        {
            // Nadelpos. im FLASH gueltig
            saf->hfkor=getw(f1);
            saf->hfp_gueltig=0x55;
        }
        else
        {
            // Nadelpos. im FLASH ungueltig
            // Nadelpos. setzen
            saf->hfkor=0;
        }
        /* EasyCODE ) */
    }
    /* EasyCODE ) */
    fclose (f1);
    /* EasyCODE ) */
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseInfo */
/* EasyCODE F */
unsigned char KlasseInfo::grundzustand(void)
{
    unsigned char pc104software [20]; // var. String
    unsigned char fwversion [20]; // var. String
    unsigned char display [20]; // var. String
    unsigned char nmotor [20]; // var. String
    unsigned char i;
    /* EasyCODE - */
    key = 0;
    while ( TRUE )
    {
        switch ( key )
        {
        case 0 :
            /* EasyCODE ( 0 
            Bildschirmvorlage und SW Staende holen und anzeigen */
            if ( taste_los() == 0xff )
            {
                return E_RUHE;
            }
            else
            {
                while ( nminf.befehl != 0 )
                {
                }
                nminf.softvers[0] = 0;
                if ( nminf.fehler == 0 )
                {
                    // Softwareversion lesen
                    nminf.befehl = 20;
                }
                eing_botsch1.senden(BV004);
                strncpy(fwversion, DPRVersion, 17);
                fwversion[17] = 0x00;   //  Nullterminator
                strncpy(pc104software, softversion, 17);
                pc104software[17] = 0x00; //  Nullterminator
                /* EasyCODE ( 0 
                Version Bedienfeld */
                keyb.bed_version_gueltig = FALSE;
                eing_botsch1.senden(VERSION);
                teing1.rsz();
                teing1.sz(100);
                while ( !teing1.tz() )
                {
                    if ( keyb.bed_version_gueltig )
                    {
                        break;
                    }
                }
                if ( teing1.tz() )
                {
                    /* EasyCODE < */
                    //  Keine Version vom Touch empfangen
                    /* EasyCODE > */
                    for ( i = 0; i <= 7; i++ )
                    {
                        /* EasyCODE < */
                        display[i] = 0x2D;  // ---- anzeigen
                        /* EasyCODE > */
                    }
                }
                else
                {
                    // OK,  Version empfangen
                    
                    strncpy(display, keyb.bed_version, 8);
                }
                display[8] = 0x00;  // Nullterminator
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                IP-Adresse */
                keyb.ipadr_gueltig = FALSE;
                eing_botsch1.senden(ESCS "Ni");
                teing1.rsz();
                teing1.sz(100);
                while ( !teing1.tz() )
                {
                    if ( keyb.ipadr_gueltig )
                    {
                        break;
                    }
                }
                if ( teing1.tz() )
                {
                    //  Keine IP-Adresse empfangen
                    keyb.ipadr[0] = 0;
                }
                else
                {
                    // OK,  IP-Adresse empfangen
                }
                /* EasyCODE ) */
                while ( nminf.befehl != 0 )
                {
                }
                sprintf(eing.varstring, swversions, pc104software, fwversion, display, nminf.version, nminf.softvers, saf->piececounter);
                if ( vncrunning )
                {
                    strcat (eing.varstring, vncstops);
                }
                else
                {
                    strcat (eing.varstring, vncstarts);
                }
                eing_botsch1.senden(eing.varstring);
                sprintf(eing.varstring1, stundens,(float)prod->betriebs_h/100, (float)prod->produkt_h/100);
                eing_botsch1.senden(eing.varstring1);
                sprintf (eing.varstring, ipadrs, keyb.ipadr);
                eing_botsch1.senden(eing.varstring);
                teing1.rsz();
                teing1.sz(30); //  300ms
                key = 1;
                break;
            }
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            Tasten abfragen */
            if ( koor.eingstop() )
            {
                return E_RUHE;
            }
            else
            {
                if ( teing1.tz() && !eing_botsch1.busycheck() )
                {
                    /* EasyCODE < */
                    sprintf(eing.varstring, stundens, (float)prod->betriebs_h/100, (float)prod->produkt_h/100);
                    eing_botsch1.senden(eing.varstring);
                    teing1.rsz();
                    teing1.sz(30); //  300ms
                    /* EasyCODE > */
                }
                switch ( keyb.taste() )
                {
                case TENDE :
                    return E_EXIT;
                case TEINGABE :
                    return E_GST;
                case TCLEAR1 :
                    /* EasyCODE ( 0 
                    Betriebs Stundenzaehler loeschen */
                    switch ( fkeytst(19) )
                    {
                    case 0 //frei
                    :
                        /* EasyCODE = */
                    case 1 //frei (nach PIN Eingabe)
                    :
                        if ( taste_los() == 0xff )
                        {
                            return E_RUHE;
                        }
                        else
                        {
                            prod->betriebs_h = 0;
                            key = 0;
                            break;
                        }
                    case 2 //gesperrt
                    :
                        key = 0;
                        break;
                    default:
                        return E_RUHE // Stopanforderung -> Ruhezustand
                       ;
                    }
                    break;
                    /* EasyCODE ) */
                case TCLEAR2 :
                    /* EasyCODE ( 0 
                    Produktions Stundenzaehler loeschen */
                    switch ( fkeytst(20) )
                    {
                    case 0 //frei
                    :
                        /* EasyCODE = */
                    case 1 //frei (nach PIN Eingabe)
                    :
                        if ( taste_los() == 0xff )
                        {
                            return E_RUHE;
                        }
                        else
                        {
                            prod->produkt_h = 0;
                            key = 0;
                            break;
                        }
                    case 2 //gesperrt
                    :
                        key = 0;
                        break;
                    default:
                        return E_RUHE // Stopanforderung -> Ruhezustand
                       ;
                    }
                    break;
                    /* EasyCODE ) */
                case TCLEAR3 :
                    /* EasyCODE ( 0 
                    Tagesst_ckzaehler ruecksetzen */
                    switch ( fkeytst(17) )
                    {
                    case 0 //frei
                    :
                        /* EasyCODE = */
                    case 1 // frei (nach PIN Eingabe)
                    :
                        if ( taste_los() == 0xff )
                        {
                            return E_RUHE;
                        }
                        else
                        {
                            saf->piececounter = 0;
                            key = 0;
                            break;
                        }
                    case 2 //gesperrt
                    :
                        key = 0;
                        break;
                    default:
                        return E_RUHE // Stopanforderung -> Ruhezustand
                       ;
                    }
                    break;
                    /* EasyCODE ) */
                case TVNC :
                    vncrunning = !vncrunning;
                    if ( koor.bdf == PANEL )
                    {
                        /* EasyCODE ( 0 
                        panel */
                        if ( vncrunning )
                        {
                            eing_botsch1.senden(vncruns);
                        }
                        else
                        {
                            eing_botsch1.senden(vncnotruns);
                        }
                        /* EasyCODE ) */
                    }
                    else
                    {
                        /* EasyCODE ( 0 
                        PC */
                        if ( vncrunning )
                        {
                            eing_botsch1.senden(vncPCruns);
                        }
                        else
                        {
                            eing_botsch1.senden(vncPCnotruns);
                        }
                        /* EasyCODE ) */
                    }
                    key = 0;
                    break;
                default:
                    break;
                }
            }
            break;
            /* EasyCODE ) */
        default:
            break;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseLaender */
/* EasyCODE ( 0 
KlasseLaender::grundzustand */
/* EasyCODE F */
unsigned char KlasseLaender::grundzustand(void)
{
    unsigned char sprachmerker;
    if ( taste_los() == 0xff )
    {
        return E_RUHE;
    }
    else
    {
        eing_botsch1.senden(BV005);
        eing_botsch1.senden(anzsprache);
        inv_sprachtaste();
        sprachmerker = saf->md.sprache;
    }
    while ( TRUE )
    {
        switch ( getkey() )
        {
        case STOPANF :
            textload();
            return E_RUHE;
        case TENDE :
            textload();
            return E_EXIT;
        case TEINGABE :
            textload();
            return E_GST;
        case TDEUTSCH :
            sprachmerker = DEUTSCH;
            break;
        case TENGL :
            sprachmerker = ENGL;
            break;
        case TFRANZ :
            sprachmerker = FRANZ;
            break;
        case TSPAN :
            sprachmerker = SPAN;
            break;
        case TITAL :
            sprachmerker = ITAL;
            break;
        default:
            break;
        }
        if ( sprachmerker != saf->md.sprache )
        {
            inv_sprachtaste(); // aktuelle Sprachtaste aus
            saf->md.sprache = sprachmerker;
            if ( taste_los() == 0xff )
            {
                return E_RUHE;
            }
            else
            {
                /* EasyCODE < */
                inv_sprachtaste(); // aktuelle Sprachtaste an
                /* EasyCODE > */
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseLaender::inv_sprachtaste */
/* EasyCODE F */
void KlasseLaender::inv_sprachtaste(void)
{
    while ( eing_botsch1.busycheck() )
    {
    }
    switch ( saf->md.sprache )
    {
    case ENGL :
        sprintf(eing.varstring,tinvs ,TENGL);
        break;
    case FRANZ :
        sprintf(eing.varstring,tinvs ,TFRANZ);
        break;
    case SPAN :
        sprintf(eing.varstring,tinvs ,TSPAN);
        break;
    case ITAL :
        sprintf(eing.varstring,tinvs ,TITAL);
        break;
    default:
        sprintf(eing.varstring,tinvs ,TDEUTSCH);
        break;
    }
    eing_botsch1.senden(eing.varstring);
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseFunkt */
/* EasyCODE ( 0 
KlasseFunkt::grundzustand */
/* EasyCODE F */
unsigned char KlasseFunkt::grundzustand(void)
{
    unsigned char userindex, fktindex, loopflag, loopflag1;
    userindex = 0xff;
    fktindex = 0;
    while ( TRUE )
    {
        /* EasyCODE ( 0 
        Bildschirmvorlage anzeigen */
        if ( taste_los() == 0xff )
        {
            return E_RUHE;
        }
        eing_botsch1.senden(BV006);
        eing_botsch1.senden(users);
        /* EasyCODE ) */
        loopflag1 = TRUE;
        while ( loopflag1 )
        {
            /* EasyCODE ( 0 
            Fkt - Zustand anzeigen */
            eing_botsch1.senden(schlossloeschs);
            sprintf(eing.varstring,fktnrzusts,fktindex, pschloss1s[saf->md.fkey_array[0][fktindex]],
                                                        pschloss2s[saf->md.fkey_array[1][fktindex]],
                                                        pschloss3s[saf->md.fkey_array[2][fktindex]]);
            eing_botsch1.senden(eing.varstring);
            /* EasyCODE ) */
            switch ( getkey() )
            {
            case STOPANF :
                return E_RUHE;
            case TENDE :
                return E_EXIT;
            case TEINGABE :
                return E_GST;
            case TUSER1 :
                userindex = 0;
                break;
            case TUSER2 :
                userindex = 1;
                break;
            case TUSER3 :
                userindex = 2;
                break;
            case TUSER1ZUST :
                /* EasyCODE ( 0 */
                if ( taste_los() == 0xff )
                {
                    return E_RUHE;
                }
                else
                {
                    if ( (fktindex>=7) && (fktindex<=15) )
                    {
                        eing_botsch1.senden(BELLBELL);
                    }
                    else
                    {
                        if ( saf->md.fkey_array[0][fktindex] == 0 )
                        {
                            saf->md.fkey_array[0][fktindex] = 1;
                        }
                        else
                        {
                            saf->md.fkey_array[0][fktindex] = 0;
                        }
                    }
                }
                break;
                /* EasyCODE ) */
            case TUSER2ZUST :
                /* EasyCODE ( 0 */
                if ( taste_los() == 0xff )
                {
                    return E_RUHE;
                }
                else
                {
                    if ( (fktindex>=7) && (fktindex<=15) )
                    {
                        eing_botsch1.senden(BELLBELL);
                    }
                    else
                    {
                        if ( saf->md.fkey_array[1][fktindex] == 0 )
                        {
                            saf->md.fkey_array[1][fktindex] = 1;
                        }
                        else
                        {
                            saf->md.fkey_array[1][fktindex] = 0;
                        }
                    }
                }
                break;
                /* EasyCODE ) */
            case TUSER3ZUST :
                /* EasyCODE ( 0 */
                if ( taste_los() == 0xff )
                {
                    return E_RUHE;
                }
                else
                {
                    if ( (fktindex>=7) && (fktindex<=15) )
                    {
                        eing_botsch1.senden(BELLBELL);
                    }
                    else
                    {
                        if ( saf->md.fkey_array[2][fktindex] == 0 )
                        {
                            saf->md.fkey_array[2][fktindex] = 1;
                        }
                        else
                        {
                            saf->md.fkey_array[2][fktindex] = 0;
                        }
                    }
                }
                break;
                /* EasyCODE ) */
            case TFKTNR :
                /* EasyCODE ( 0 */
                pargetval.maxwert = FKEY_FUNK_MAX - 1;
                pargetval.minwert = 0;
                pargetval.kommastellen = 0;
                pargetval.string = val_ints;
                sprintf(eing.varstring, val_pik_einhs, "\x1b=020008\x1bS12F : \x1bS11", " ");
                pargetval.altwert = fktindex;
                pargetval.pikto = eing.varstring;
                pargetval.auftrag = 0;
                switch ( getvalue_eing() )
                {
                case 0
                                                                                                                                                                                                                                                                                 // 
                     //ENTER
                :
                    fktindex = pargetval.wert;
                case 1
                                                                                                                                                                                                                                                                                 // 
                     //ESC
                :
                    loopflag1 = FALSE;
                    break;
                case 2
                                                                                                                                                                                                                                                                                 // 
                     //Stopanf
                :
                    return E_RUHE;
                default:
                    loopflag1 = FALSE;
                    break;
                }
                break;
                /* EasyCODE ) */
            case TFKTNRINKR :
                /* EasyCODE ( 0 */
                loopflag = TRUE;
                teing1.rsz();
                teing1.sz(saf->slowdelay);
                /* EasyCODE ( 0 
                Fkt index inkr. */
                if ( fktindex < FKEY_FUNK_MAX -1 )
                {
                    ++fktindex ;
                }
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Fkt - Zustand anzeigen */
                eing_botsch1.senden(schlossloeschs);
                sprintf(eing.varstring,fktnrzusts,fktindex,pschloss1s[saf->md.fkey_array[0][fktindex]],
                                                           pschloss2s[saf->md.fkey_array[1][fktindex]],
                                                           pschloss3s[saf->md.fkey_array[2][fktindex]]);
                eing_botsch1.senden(eing.varstring);
                /* EasyCODE ) */
                do{
                    if ( keyb.taste()!=TFKTNRINKR )
                    {
                        loopflag = FALSE;
                        break;
                    }
                    if ( koor.eingstop() )
                    {
                        return E_RUHE;
                    }
                }while ( !teing1.tz() );
                if ( loopflag )
                {
                }
                else
                {
                    break;
                }
                do{
                    teing2.rsz();
                    teing2.sz(saf->slowtime);
                    /* EasyCODE ( 0 
                    Fkt index inkr. */
                    if ( fktindex < FKEY_FUNK_MAX -1 )
                    {
                        ++fktindex ;
                    }
                    /* EasyCODE ) */
                    /* EasyCODE ( 0 
                    Fkt - Zustand anzeigen */
                    eing_botsch1.senden(schlossloeschs);
                    sprintf(eing.varstring,fktnrzusts,fktindex, pschloss1s[saf->md.fkey_array[0][fktindex]],
                                                                pschloss2s[saf->md.fkey_array[1][fktindex]],
                                                                pschloss3s[saf->md.fkey_array[2][fktindex]]);
                    eing_botsch1.senden(eing.varstring);
                    /* EasyCODE ) */
                    do{
                        if ( keyb.taste()!=TFKTNRINKR )
                        {
                            loopflag = FALSE;
                            break;
                        }
                        if ( koor.eingstop() )
                        {
                            return E_RUHE;
                        }
                    }while ( !teing2.tz() );
                }while ( loopflag );
                break;
                /* EasyCODE ) */
            case TFKTNRDEKR :
                /* EasyCODE ( 0 */
                loopflag = TRUE;
                teing1.rsz();
                teing1.sz(saf->slowdelay);
                if ( fktindex > 0 )
                {
                    --fktindex;
                }
                eing_botsch1.senden(schlossloeschs);
                sprintf(eing.varstring,fktnrzusts,fktindex, pschloss1s[saf->md.fkey_array[0][fktindex]],
                                                            pschloss2s[saf->md.fkey_array[1][fktindex]],
                                                            pschloss3s[saf->md.fkey_array[2][fktindex]]);
                eing_botsch1.senden(eing.varstring);
                do{
                    if ( keyb.taste()!=TFKTNRDEKR )
                    {
                        loopflag = FALSE;
                        break;
                    }
                    if ( koor.eingstop() )
                    {
                        return E_RUHE;
                    }
                }while ( !teing1.tz() );
                if ( loopflag )
                {
                }
                else
                {
                    break;
                }
                do{
                    teing2.rsz();
                    teing2.sz(saf->slowtime);
                    if ( fktindex > 0 )
                    {
                        --fktindex;
                    }
                    eing_botsch1.senden(schlossloeschs);
                    sprintf(eing.varstring,fktnrzusts,fktindex, pschloss1s[saf->md.fkey_array[0][fktindex]],
                                                                pschloss2s[saf->md.fkey_array[1][fktindex]],
                                                                pschloss3s[saf->md.fkey_array[2][fktindex]]);
                    eing_botsch1.senden(eing.varstring);
                    do{
                        if ( keyb.taste()!=TFKTNRDEKR )
                        {
                            loopflag = FALSE;
                            break;
                        }
                        if ( koor.eingstop() )
                        {
                            return E_RUHE;
                        }
                    }while ( !teing2.tz() );
                }while ( loopflag );
                break;
                /* EasyCODE ) */
            default:
                /* EasyCODE ( 0 */
                while ( eing_botsch1.busycheck() )
                {
                    ;
                }
                eing_botsch1.senden(BELLBELL);
                break;
                /* EasyCODE ) */
            }
            if ( userindex == 0xff )
            {
            }
            else
            {
                /* EasyCODE ( 0 
                Userpin eingeben */
                pargetval.maxwert = 9999;
                pargetval.minwert = 0;
                pargetval.kommastellen = 0;
                pargetval.string = val_int4s;
                sprintf(eing.varstring, val_pik_einhs, ppins, " ");
                pargetval.altwert = saf->md.codenr_arr[userindex];
                pargetval.pikto = eing.varstring;
                pargetval.auftrag = 0;
                switch ( getvalue_eing() )
                {
                case 0
                                                                                                                                                                                                                                                                                 // 
                     //ENTER
                :
                    saf->md.codenr_arr[userindex] = pargetval.wert;
                case 1
                                                                                                                                                                                                                                                                                 // 
                     //ESC
                :
                    loopflag1 = FALSE;
                    break;
                case 2
                                                                                                                                                                                                                                                                                 // 
                     //Stopanf
                :
                    return E_RUHE;
                default:
                    loopflag1 = FALSE;
                    break;
                }
                /* EasyCODE ) */
                userindex = 0xff;
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseDirekt */
/* EasyCODE ( 0 
KlasseDirekt::movesm */
/* EasyCODE F */
unsigned char KlasseDirekt::movesm(void)
{
    unsigned char loopflag, retwert, taste;
    /* EasyCODE ( 0 
    Bildschirmvorlage anzeigen */
    if ( taste_los() == 0xff )
    {
        return E_RUHE;
    }
    eing_botsch1.senden(BV010);
    /* EasyCODE ) */
    loopflag = TRUE;
    while ( loopflag )
    {
        switch ( getkey() )
        {
        case STOPANF :
            retwert = E_RUHE;
            loopflag = FALSE;
            break;
        case TENDE :
            retwert = E_EXIT;
            loopflag = FALSE;
            break;
        case TEINGABE :
            retwert = E_GST;
            loopflag = FALSE;
            break;
        case TSCHLITTENVOR :
            /* EasyCODE ( 0 
            Schlitten vor, ^^ */
            /* EasyCODE ( 0 
            Softstepping einschalten */
            smx.Fkt2_on();  // Softstepping SM-Endst. X
            smy.Fkt2_on();  // Softstepping SM-Endst. Y
            /* EasyCODE ) */
            saf->carposok = FALSE;
            nae.homereq = TRUE;
            taste = TSCHLITTENVOR;
            smy.status = SMYMINUS;
            smy.status |= EFR_CLK_64;
            smy.init();
            smy.rampadr = smy_ststadr;
            smy.start_Freq ();
            /* EasyCODE ) */
            break;
        case TSCHLITTENZURUECK :
            /* EasyCODE ( 0 
            Schlitten zurueck */
            /* EasyCODE ( 0 
            Softstepping einschalten */
            smx.Fkt2_on();  // Softstepping SM-Endst. X
            smy.Fkt2_on();  // Softstepping SM-Endst. Y
            /* EasyCODE ) */
            saf->carposok = FALSE;
            nae.homereq = TRUE;
            taste = TSCHLITTENZURUECK;
            smy.status = SMYPLUS;
            smy.status |= EFR_CLK_64;
            smy.init();
            smy.rampadr = smy_ststadr;
            smy.start_Freq ();
            /* EasyCODE ) */
            break;
        case TSCHLITTENRECHTS :
            /* EasyCODE ( 0 
            Schlitten rechts */
            /* EasyCODE ( 0 
            Softstepping einschalten */
            smx.Fkt2_on();  // Softstepping SM-Endst. X
            smy.Fkt2_on();  // Softstepping SM-Endst. Y
            /* EasyCODE ) */
            saf->carposok = FALSE;
            nae.homereq = TRUE;
            taste = TSCHLITTENRECHTS;
            smx.status = PAWRAMPCV | SMXPLUS;
            smx.vort = saf->vorserv;
            smx.init();
            smx.rampadr = 1;
            smx.start_Freq ();
            /* EasyCODE ) */
            break;
        case TSCHLITTENLINKS :
            /* EasyCODE ( 0 
            Schlitten links */
            /* EasyCODE ( 0 
            Softstepping einschalten */
            smx.Fkt2_on();  // Softstepping SM-Endst. X
            smy.Fkt2_on();  // Softstepping SM-Endst. Y
            /* EasyCODE ) */
            saf->carposok = FALSE;
            nae.homereq = TRUE;
            taste = TSCHLITTENLINKS;
            smx.status = PAWRAMPCV | SMXMINUS;
            smx.vort = saf->vorserv;
            smx.init();
            smx.rampadr = 1;
            smx.start_Freq ();
            /* EasyCODE ) */
            break;
        default:
            eing_botsch1.senden(BELLBELL);
            taste = 0xfe;
            break;
        }
        if ( loopflag && (taste != 0xfe) )
        {
            while ( TRUE )
            {
                if ( keyb.taste() != taste )
                {
                    smx.stop();
                    smy.stop();
                    teing1.rsz();
                    teing1.sz(50);
                    while ( !teing1.tz() )
                    {
                        if ( keyb.taste() != 0xff )
                        {
                            teing1.rsz();
                            teing1.sz(50);
                        }
                    }
                    break;
                }
            }
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseDirekt::smref */
/* EasyCODE F */
unsigned char KlasseDirekt::smref(unsigned char carposit)
{
    unsigned char smkey;
    unsigned char refindex, loopflag;
    unsigned char tastenmem;
    int parametertemp;
    unsigned int i;
    if ( carposit )
    {
        smkey = 0;
    }
    else
    {
        eing.ioact = TRUE;
        refindex = 0;
        eing_botsch1.senden(BV013);
        nup.koorsoll[0].xbild=saf->refpkt[0].x;
        nup.koorsoll[0].ybild=saf->refpkt[0].y;
        nup.bitomo(&nup.koorsoll[0]);
        nup.workbuf.koorist=nup.koorsoll[0];
        nup.carlp=FALSE;
        nae.homereq=TRUE;
        sprintf(eing.varstring,reftexts, 1,
                saf->refpkt[0].x,
                saf->refpkt[0].y);
        eing_botsch1.senden(eing.varstring);
        eing_botsch1.senden(refs);
        smkey = 3;
    }
    while ( TRUE )
    {
        switch ( smkey )
        {
        case 0 :
            #ifndef buerotest
                /* EasyCODE ( 0 
                Nullpunkte gueltig ? */
                if ( saf->mnp_gueltig!=0x55 )
                {
                    koor.eing_err = 1;
                    return E_RUHE;
                }
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Maschine in Grundstellung ? */
                if ( nup.carlp && !nae.faedelreq && saf->carposok )
                {
                }
                else
                {
                    koor.eing_err = 8;
                    return E_RUHE;
                }
                /* EasyCODE ) */
            #endif 
            /* EasyCODE ( 0 
            BV013 anzeigen */
            if ( taste_los() == 0xff )
            {
                return E_RUHE;
            }
            else
            {
                eing_botsch1.senden(BV013);
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
                smkey = 1;
                break;
            }
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            Klammer einlegen */
            switch ( keyb.taste() )
            {
            case TEINGABE :
                eing.ioact = FALSE;
                return E_GST;
            case TENDE :
                eing.ioact = FALSE;
                return E_EXIT;
            case TENTER :
                smkey = 2;
                /* EasyCODE - */
                nup.initkey = 0;
                tnae1.rsz();
                tnae1.sz(saf->initzt);
                nup.carlp=FALSE;
                nae.homereq=TRUE;
                smkey = 5;
                if ( taste_los() == 0xff )
                {
                    return E_RUHE;
                }
                else
                {
                    break;
                }
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
                            smkey = 10;
                            break;
                        }
                    }
                #endif 
                if ( taste_los() == 0xff )
                {
                    return E_RUHE;
                }
                else
                {
                    break;
                }
            default:
                if ( taste_los() == 0xff )
                {
                    return E_RUHE;
                }
                else
                {
                    break;
                }
            case 0xff :
                break;
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            auf 1. Punkt fahren, Oberteil absenken,
            Piktoleiste anzeigen */
            if ( !eing.pup.nafupos() )
            {
                eing.ioact = FALSE;
                smkey = 10;
            }
            else
            {
                if ( !ver.move_sonst() )
                {
                    eing.ioact = FALSE;
                    smkey = 10;
                }
                else
                {
                    nup.koorsoll[0].xbild=saf->refpkt[0].x;
                    nup.koorsoll[0].ybild=saf->refpkt[0].y;
                    nup.bitomo(&nup.koorsoll[0]);
                    nup.carlp=FALSE;
                    nae.homereq=TRUE;
                    nup.carmov(FAST);
                    while ( !smx.ready() || !smy.ready() )
                    {
                        if ( koor.notstop() )
                        {
                            smx.stop();
                            smy.stop();
                            saf->carposok=FALSE;
                            eing.ioact = FALSE;
                            smkey = 10;
                            return E_REF;
                        }
                    }
                    nup.workbuf.koorist=nup.koorsoll[0];
                    refindex = 0;
                    y50u2.setAuftrag();
                    while ( !y50u2.fertig() )
                    {
                        if ( koor.eingstop() )
                        {
                            eing.ioact = FALSE;
                            smkey = 10;
                            return E_REF;
                        }
                    }
                    sprintf(eing.varstring,reftexts, 1,
                            saf->refpkt[0].x,
                            saf->refpkt[0].y);
                    eing_botsch1.senden(eing.varstring);
                    eing_botsch1.senden(refs);
                    smkey = 3;
                }
            }
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            mit den Tasten FWD/BACK
            zu den einzelnen Referenzpunkten */
            switch ( (tastenmem=keyb.taste()) )
            {
            case TEINGABE :
                eing.ioact = FALSE;
                return E_GST;
            case TENDE :
                eing.ioact = FALSE;
                return E_EXIT;
            case TPAR :
                /* EasyCODE ( 0 
                Parameter aendern */
                eing.par.key = 1;
                eing.par.grundzustand();
                /* EasyCODE ( 0 
                Parameteraenderungen, die weitere
                Aktionen zur Folge haben, behandeln */
                /* EasyCODE ( 0 
                Parameter 201 bis 208, Konfiguration */
                if ( eing.par.IsChanged (202) )
                {
                    com1_com3_init();
                }
                if ( eing.par.IsChanged (206) || eing.par.IsChanged (207) )
                {
                    nup.bitomo_init();
                }
                if ( eing.par.IsChanged(201) || eing.par.IsChanged(202) ||
                     eing.par.IsChanged(203) || eing.par.IsChanged(204) ||
                     eing.par.IsChanged(205) || eing.par.IsChanged(206) ||
                     eing.par.IsChanged(207) || eing.par.IsChanged(208) )
                {
                    saf->konfig_gueltig=0x55;
                    eing.konfinit();
                    eing.par.DelChangedInfo(201);
                    eing.par.DelChangedInfo(202);
                    eing.par.DelChangedInfo(203);
                    eing.par.DelChangedInfo(204);
                    eing.par.DelChangedInfo(205);
                    eing.par.DelChangedInfo(206);
                    eing.par.DelChangedInfo(207);
                    eing.par.DelChangedInfo(208);
                }
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Parameter 109, Klammerueberwachung */
                if ( eing.par.IsChanged(109) )
                {
                    eing.par.DelChangedInfo(109);
                    nae.homereq = TRUE;
                }
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Parameter 105, Fw-Empfindlichkeit */
                if ( eing.par.IsChanged(105) )
                {
                    nminf.pawini();
                    eing.par.DelChangedInfo(105);
                    teing1.rsz();
                    teing1.sz(saf->quick_delayzt);
                }
                else
                {
                    teing1.lz();
                }
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Parameter 131, Materialdicke */
                if ( eing.par.IsChanged(131) )
                {
                    eing.par.DelChangedInfo(131);
                    nae.naetab_boot();
                    nae.homereq = TRUE;
                }
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Parameter 142, Tastenpieps */
                if ( eing.par.IsChanged(142) )
                {
                    eing.par.DelChangedInfo(142);
                    if ( saf->md.tastenpiep )
                    {
                        eing_botsch1.senden(ESCS "RR3");
                    }
                    else
                    {
                        eing_botsch1.senden(ESCS "RR1");
                    }
                }
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Parameter 145, verkuerzter Anfangsfaden */
                if ( eing.par.IsChanged(145) )
                {
                    eing.par.DelChangedInfo(145);
                    if ( saf->md.kurzerAnffad )
                    {
                        saf->md.wedelnfussoben = FALSE;
                    }
                }
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Parameter 301, NIS */
                if ( eing.par.IsChanged(301) )
                {
                    while ( nminf.befehl!=0 )
                    {
                        if ( nminf.fehler!=0 )
                        {
                            break;
                        }
                    }
                    if ( nminf.fehler!=0 )
                    {
                        koor.eing_err = 41;
                        eing.errpar = nminf.fehler;
                        break;
                    }
                    else
                    {
                        while ( !teing1.tz() )
                        {
                        }
                        parametertemp=(saf->nadelp >> 1) + ((long)saf->md.nisvograd*MAXPOS/360);
                        if ( parametertemp >= MAXPOS )
                        {
                            parametertemp -= MAXPOS;
                        }
                        nminf.parameter = parametertemp;
                        nminf.befehl=3;
                        while ( nminf.befehl!=0 )
                        {
                            if ( nminf.fehler!=0 )
                            {
                                break;
                            }
                        }
                        if ( nminf.fehler!=0 )
                        {
                            koor.eing_err = 41;
                            eing.errpar = nminf.fehler;
                            break;
                        }
                        else
                        {
                            eing.par.DelChangedInfo(301);
                            teing1.rsz();
                            teing1.sz(saf->quick_delayzt);
                        }
                    }
                }
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Parameter 303, REF2 */
                if ( eing.par.IsChanged(303) )
                {
                    while ( nminf.befehl!=0 )
                    {
                        if ( nminf.fehler!=0 )
                        {
                            break;
                        }
                    }
                    if ( nminf.fehler!=0 )
                    {
                        koor.eing_err = 41;
                        eing.errpar = nminf.fehler;
                        break;
                    }
                    else
                    {
                        while ( !teing1.tz() )
                        {
                        }
                        parametertemp = (saf->nadelp >> 1) + ((long)saf->md.ref2*MAXPOS/360);
                        if ( parametertemp >= MAXPOS )
                        {
                            parametertemp -= MAXPOS;
                        }
                        nminf.parameter = parametertemp;
                        nminf.befehl=4;
                        while ( nminf.befehl!=0 )
                        {
                            if ( nminf.fehler!=0 )
                            {
                                break;
                            }
                        }
                        if ( nminf.fehler!=0 )
                        {
                            koor.eing_err = 41;
                            eing.errpar = nminf.fehler;
                            break;
                        }
                        else
                        {
                            eing.par.DelChangedInfo(303);
                            teing1.rsz();
                            teing1.sz(saf->quick_delayzt);
                        }
                    }
                }
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Parameter 502, Schneiddrehzahl */
                if ( eing.par.IsChanged(502) )
                {
                    while ( nminf.befehl!=0 )
                    {
                        if ( nminf.fehler!=0 )
                        {
                            break;
                        }
                    }
                    if ( nminf.fehler!=0 )
                    {
                        koor.eing_err = 41;
                        eing.errpar = nminf.fehler;
                        break;
                    }
                    else
                    {
                        while ( !teing1.tz() )
                        {
                        }
                        nminf.parameter = saf->md.cutgsw;
                        nminf.befehl = 17;
                        while ( nminf.befehl!=0 )
                        {
                            if ( nminf.fehler!=0 )
                            {
                                break;
                            }
                        }
                        if ( nminf.fehler!=0 )
                        {
                            koor.eing_err = 41;
                            eing.errpar = nminf.fehler;
                            break;
                        }
                        else
                        {
                            eing.par.DelChangedInfo(502);
                            teing1.rsz();
                            teing1.sz(saf->quick_delayzt);
                        }
                    }
                }
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Parametergruppe 700, NM-Parameter */
                for ( i=1; i<= 19; i++ )
                {
                    if ( eing.par.IsChanged(700+i) )
                    {
                        eing.par.DelChangedInfo(700+i);
                        i = 0xffff;
                        break;
                    }
                }
                if ( (i==0xffff) || eing.par.IsChanged(725) )
                {
                    for ( i=0; i<QP_ANZAHL; i++ )
                    {
                        while ( nminf.befehl!=0 )
                        {
                            if ( nminf.fehler!=0 )
                            {
                                break;
                            }
                        }
                        if ( nminf.fehler!=0 )
                        {
                            koor.eing_err = 41;
                            eing.errpar = nminf.fehler;
                            break;
                        }
                        else
                        {
                            while ( !teing1.tz() )
                            {
                            }
                            nminf.parameternr = quick_paramnr [i];
                            nminf.parameter = saf->md.quick_paramwert [i];
                            nminf.befehl = 10;
                            while ( nminf.befehl!=0 )
                            {
                                if ( nminf.fehler!=0 )
                                {
                                    break;
                                }
                            }
                            if ( nminf.fehler!=0 )
                            {
                                koor.eing_err = 41;
                                eing.errpar = nminf.fehler;
                                break;
                            }
                            else
                            {
                                teing1.rsz();
                                teing1.sz(saf->quick_delayzt);
                            }
                        }
                    }
                }
                /* EasyCODE ) */
                /* EasyCODE ) */
                nup.koorsoll[0].xbild=saf->refpkt[refindex].x;
                nup.koorsoll[0].ybild=saf->refpkt[refindex].y;
                nup.bitomo(&nup.koorsoll[0]);
                nup.carmov(FAST);
                while ( !smx.ready() || !smy.ready() )
                {
                    if ( koor.notstop() )
                    {
                        smx.stop();
                        smy.stop();
                        saf->carposok=FALSE;
                        eing.ioact = FALSE;
                        smkey = 10;
                        return E_REF;
                    }
                }
                nup.workbuf.koorist=nup.koorsoll[0];
                eing_botsch1.senden(BV013);
                sprintf(eing.varstring,reftexts, refindex+1,
                   saf->refpkt[refindex].x,
                   saf->refpkt[refindex].y);
                eing_botsch1.senden(eing.varstring);
                eing_botsch1.senden(refs);
                break;
                /* EasyCODE ) */
            case TFWD :
                /* EasyCODE = */
            case TBACK :
                /* EasyCODE ( 0 
                naechsten/vorigen Punkt anfahren */
                if ( !eing.pup.nafupos() )
                {
                    eing.ioact = FALSE;
                    smkey = 10;
                }
                else
                {
                    if ( tastenmem == TFWD )
                    {
                        if ( refindex >= 6 )
                        {
                            refindex = 0;
                        }
                        else
                        {
                            refindex++;
                        }
                    }
                    else
                    {
                        if ( refindex == 0 )
                        {
                            refindex = 6;
                        }
                        else
                        {
                            refindex--;
                        }
                    }
                    nup.koorsoll[0].xbild=saf->refpkt[refindex].x;
                    nup.koorsoll[0].ybild=saf->refpkt[refindex].y;
                    nup.bitomo(&nup.koorsoll[0]);
                    nup.carmov(FAST);
                    while ( !smx.ready() || !smy.ready() )
                    {
                        if ( koor.notstop() )
                        {
                            smx.stop();
                            smy.stop();
                            saf->carposok=FALSE;
                            eing.ioact = FALSE;
                            smkey = 10;
                            return E_REF;
                        }
                    }
                    nup.workbuf.koorist=nup.koorsoll[0];
                    sprintf(eing.varstring,reftexts, refindex+1,
                       saf->refpkt[refindex].x,
                       saf->refpkt[refindex].y);
                    eing_botsch1.senden(eing.varstring);
                    if ( taste_los() == 0xff )
                    {
                        eing.ioact = FALSE;
                        smkey = 10;
                    }
                }
                break;
                /* EasyCODE ) */
            case TFUSS :
                /* EasyCODE ( 0 
                Taste Fuss */
                while ( !hfuss.ready() || !y8.fertig() || !y4.fertig() )
                {
                    if ( koor.eingstop() )
                    {
                        eing.ioact = FALSE;
                        smkey = 10;
                        return E_REF;
                    }
                }
                if ( hfuss.getpos().pos == saf->md.posoben.pos )
                {
                    // Huepferfuss oben
                    hfuss.ab();         // Huepferfuss ab
                    y8.setAuftrag();    // Niederhalter ab
                    y5.setAuftrag();    // Fadenspannung auf
                    y4.resAuftrag();    // Klemme auf
                    y6.resAuftrag();  // Zusatzfadenspannung auf  SW016
                }
                else
                {
                    // Huepferfuss unten
                    loopflag = TRUE;
                    while ( loopflag )
                    {
                        switch ( nup.napos () )
                        {
                        case 0 :
                            break;
                        case 1 :
                            koor.eing_err = 41;
                            eing.errpar = nminf.fehler;
                            eing.ioact = FALSE;
                            smkey = 10;
                            return E_REF;
                        default:
                            hfuss.auf();        // Huepferfuss auf
                            y8.resAuftrag();    // Niederhalter auf
                            y5.resAuftrag();    // Fadenspannung zu
                            y4.setAuftrag();    // Klemme zu
                            break;
                        }
                    }
                }
                while ( !hfuss_auf.fertig() || !y8.fertig() || !y4.fertig() )
                {
                    if ( koor.eingstop() )
                    {
                        eing.ioact = FALSE;
                        smkey = 10;
                    }
                }
                if ( taste_los() == 0xff )
                {
                    eing.ioact = FALSE;
                    smkey = 10;
                }
                break;
                /* EasyCODE ) */
            default:
                if ( taste_los() == 0xff )
                {
                    return E_RUHE;
                }
                else
                {
                    break;
                }
            case 0xff :
                break;
            }
            break;
            /* EasyCODE ) */
        case 5 :
            /* EasyCODE ( 0 
            Schlitten positionieren */
            if ( nup.movinit() )
            {
                // nicht verriegelt
                smkey = 2;
            }
            else
            {
                if ( tnae1.tz() )
                {
                    koor.nae_err = 13;
                    smx.stop();
                    smy.stop();
                    eing.ioact = FALSE;
                    smkey = 10;
                }
                else
                {
                    if ( !ver.move_sonst() )
                    {
                        smx.stop();
                        smy.stop();
                        eing.ioact = FALSE;
                        smkey = 10;
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 10 :
            /* EasyCODE ( 0 
            error_res */
            error_res();
            if ( taste_los() == 0xff )
            {
                return E_RUHE;
            }
            else
            {
                smkey = 0;
            }
            break;
            /* EasyCODE ) */
        default:
            break;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseDirekt::nm */
/* EasyCODE F */
unsigned char KlasseDirekt::nm(void)
{
    unsigned int servgsw = GSWMIN;
    unsigned int servgswmax;
    unsigned char nmkey = 0;
    unsigned char anzcount;
    unsigned long gswbuf;
    unsigned char loopflag;
    unsigned char nmcont;
    int parametertemp;
    unsigned int altpos = 0xffff;
    hfkoord tempkoord;
    
    
    
    
    tempkoord.pos = hfuss.akt_hub;
    hfuss.postomo (&tempkoord);
    servgswmax = hubgswtab[tempkoord.motor>>1];
    while ( TRUE )
    {
        switch ( nmkey )
        {
        case 0 :
            /* EasyCODE ( 0 
            Bildschirmvorlage anzeigen */
            if ( taste_los() == 0xff )
            {
                nmkey = 10;
            }
            else
            {
                eing_botsch1.senden(BV012);
                sprintf (eing.varstring, nmgeschws, servgsw);
                eing_botsch1.senden(eing.varstring);
                sprintf(eing.varstring1, nmistgeschws, 0);
                eing_botsch1.senden (eing.varstring1);
                altpos = 0xffff;
                nmkey = 1;
            }
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            Geschw. eingeben, Tasten abfragen */
            switch ( keyb.taste() )
            {
            case TEINGABE :
                eing.ioact = FALSE;
                return E_GST;
            case TENDE :
                eing.ioact = FALSE;
                return E_EXIT;
            case TSTART :
                eing.ioact = TRUE;
                eing_botsch1.senden (nmistposdels);
                nmcont = 2;
                nae.homereq = TRUE;
                if ( e75.getZustand() && e50u2.getZustand() )
                {
                    // Spulenabdeckung geschlossen
                    // und Oberteil unten
                    nmkey = 14;
                }
                else
                {
                    nmkey = 11;
                }
                break;
            case TNMGSW :
                /* EasyCODE ( 0 
                Geschwindigkeit mit Ziffernfeld eingeben */
                pargetval.minwert = GSWMIN*100;
                pargetval.maxwert = servgswmax*100;
                pargetval.auftrag = 0;
                pargetval.altwert = servgsw*100;
                pargetval.pikto = nmps;
                pargetval.kommastellen = 0;
                pargetval.string = val_ints;
                switch ( getvalue_eing() )
                {
                case 0 //ENTER
                :
                    servgsw = pargetval.wert/100;
                    nmkey = 0;
                    break;
                case 1 //ESC
                :
                    nmkey = 0;
                    break;
                default:
                    /* EasyCODE < */
                    // Stopanf
                    /* EasyCODE > */
                    return E_RUHE;
                }
                break;
                /* EasyCODE ) */
            case TGSW_UP :
                /* EasyCODE = */
            case TGSW_DOWN :
                /* EasyCODE ( 0 
                Geschwindigkeit rollen */
                pargetval.tastencode = keyb.taste();
                pargetval.altwert = servgsw;
                pargetval.maxwert = servgswmax;
                pargetval.minwert = GSWMIN;
                pargetval.kommastellen = 0;
                pargetval.string = nmgeschws;
                getvalkey = 0;  // Wichtig Ini von getvalue scroll
                loopflag = TRUE;
                while ( loopflag )
                {
                    if ( koor.eingstop() )
                    {
                        return E_RUHE;
                    }
                    else
                    {
                        switch ( getvalue_scr () )
                        {
                        case 0 //fertig
                        :
                            loopflag = FALSE;
                            break;
                        default:
                            /* EasyCODE < */
                            // noch nicht fertig
                            /* EasyCODE > */
                            break;
                        }
                        servgsw = pargetval.wert;
                    }
                }
                break;
                /* EasyCODE ) */
            case TPOS :
                /* EasyCODE ( 0 
                aktuelle Nadelposition als Nadel OT uebernehmen */
                switch ( fkeytst(7) )
                {
                case 0 // frei
                :
                case 1 // frei (nach PIN eingabe)
                :
                    saf->nadelp_gueltig = 0;
                    saf->nadelp = nminf.nadel_pos();
                    saf->nadelp_gueltig = 0x55;
                    // Nadelpos. ins Konf-File schreiben
                    f1 = fopen ("c:kfportal.bin", "r+b");
                    fseek (f1,FF_NADELPOS_GUE,SEEK_SET);
                    fputc (0,f1);  // ungueltig
                    putw (saf->nadelp,f1);
                    fseek (f1,FF_NADELPOS_GUE,SEEK_SET);
                    fputc (0x55,f1);  // gueltig
                    fclose (f1);
                    while ( nminf.befehl!=0 )
                    {
                        if ( nminf.fehler!=0 )
                        {
                            break;
                        }
                    }
                    if ( nminf.fehler!=0 )
                    {
                        koor.eing_err = 41;
                        eing.errpar = nminf.fehler;
                        nmkey = 10;
                    }
                    else
                    {
                        /* EasyCODE ( 0 
                        Ref1 und Ref2 programmieren */
                        parametertemp=(saf->nadelp >> 1) + ((long)saf->md.nisvograd*MAXPOS/360);
                        if ( parametertemp >= MAXPOS )
                        {
                            parametertemp -= MAXPOS;
                        }
                        nminf.parameter = parametertemp;
                        nminf.befehl=3;
                        while ( nminf.befehl!=0 )
                        {
                            if ( nminf.fehler!=0 )
                            {
                                break;
                            }
                        }
                        if ( nminf.fehler!=0 )
                        {
                            koor.eing_err = 41;
                            eing.errpar = nminf.fehler;
                            nmkey = 10;
                        }
                        else
                        {
                            parametertemp = (saf->nadelp >> 1) + ((long)saf->md.ref2*MAXPOS/360);
                            if ( parametertemp >= MAXPOS )
                            {
                                parametertemp -= MAXPOS;
                            }
                            nminf.parameter = parametertemp;
                            nminf.befehl=4;
                            while ( nminf.befehl!=0 )
                            {
                                if ( nminf.fehler!=0 )
                                {
                                    break;
                                }
                            }
                            if ( nminf.fehler!=0 )
                            {
                                koor.eing_err = 41;
                                eing.errpar = nminf.fehler;
                                nmkey = 10;
                            }
                            else
                            {
                                if ( taste_los() == 0xff )
                                {
                                    nmkey = 10;
                                }
                                else
                                {
                                    nmkey = 0;
                                }
                            }
                        }
                        /* EasyCODE ) */
                    }
                    break;
                case 2 // gesperrt
                :
                    eing.ioact = FALSE;
                    return E_GST;
                default:
                    eing.ioact = FALSE;
                    return E_RUHE // Stopanforderung -> Ruhezustand
                   ;
                }
                break;
                /* EasyCODE ) */
            case TFADENSCHNEIDEN :
                /* EasyCODE ( 0 
                Fadenschneid-Ablauf starten */
                eing.ioact = TRUE;
                eing_botsch1.senden (nmistposdels);
                nmcont = 4;
                nae.homereq = TRUE;
                if ( e75.getZustand() && e50u2.getZustand() )
                {
                    nmkey = 14;
                }
                else
                {
                    nmkey = 11;
                }
                break;
                /* EasyCODE ) */
            case 0xff :
                /* EasyCODE ( 0 
                keine Taste gedrueckt, Position anzeigen */
                if ( nminf.nadel_pos() <= PULSE_PRO_UMDR )
                {
                    if ( nminf.nadel_pos() != altpos )
                    {
                        altpos = nminf.nadel_pos();
                        sprintf(eing.varstring, nmistposs, altpos);
                        eing_botsch1.senden (eing.varstring);
                    }
                }
                /* EasyCODE ) */
                break;
            default:
                eing_botsch1.senden(BELLBELL);
                break;
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            Nm starten */
            if ( koor.eingstop() )
            {
                // Stopanforderung
                nmkey = 10;
            }
            else
            {
                if ( (nminf.befehl != 0) || !y4.fertig() || !y5.fertig() || !y7.fertig() ||
                     !hfuss.ready() || !y30.fertig() || !y32.fertig() )
                {
                }
                else
                {
                    if ( nminf.fehler != 0 )
                    {
                        koor.eing_err = 41;
                        eing.errpar = nminf.fehler;
                        nmkey = 10;
                    }
                    else
                    {
                        if ( taste_los() == 0xff )
                        {
                            nmkey = 10;
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
                            nminf.drehzahl = servgsw*100;
                            nminf.befehl = 7;
                            teing3.rsz();
                            teing3.sz(10);
                            anzcount = 0;
                            gswbuf = 0;
                            nmkey = 3;
                        }
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            Nm dreht, Tasten abfragen */
            if ( koor.eingstop() )
            {
                // Stopanforderung
                nminf.position = saf->nadelp >> 1;
                nminf.posart = VORWAERTS;
                nminf.befehl=5;
                nmkey = 10;
            }
            else
            {
                switch ( keyb.taste() )
                {
                case TEINGABE :
                    nminf.position = saf->nadelp >> 1;
                    nminf.posart = VORWAERTS;
                    nminf.befehl=5;
                    eing.ioact = FALSE;
                    return E_GST;
                case TENDE :
                    nminf.position = saf->nadelp >> 1;
                    nminf.posart = VORWAERTS;
                    nminf.befehl=5;
                    eing.ioact = FALSE;
                    return E_EXIT;
                case TSTOP :
                    nminf.position = saf->nadelp >> 1;
                    nminf.posart = VORWAERTS;
                    nminf.befehl=5;
                    sprintf(eing.varstring, nmistgeschws, 0);
                    eing_botsch1.senden (eing.varstring);
                    if ( taste_los() == 0xff )
                    {
                        nmkey = 10;
                    }
                    else
                    {
                        nmkey = 1;
                    }
                    break;
                case TGSW_UP :
                    /* EasyCODE = */
                case TGSW_DOWN :
                    /* EasyCODE ( 0 
                    Geschwindigkeit rollen */
                    pargetval.tastencode = keyb.taste();
                    pargetval.altwert = servgsw;
                    pargetval.maxwert = servgswmax;
                    pargetval.minwert = GSWMIN;
                    pargetval.kommastellen = 0;
                    pargetval.string = nmgeschws;
                    getvalkey = 0;  // Wichtig Ini von getvalue scroll
                    loopflag = TRUE;
                    while ( loopflag )
                    {
                        if ( koor.eingstop() )
                        {
                            nminf.position = saf->nadelp >> 1;
                            nminf.posart = VORWAERTS;
                            nminf.befehl=5;
                            nmkey = 10;
                            loopflag = FALSE;
                        }
                        else
                        {
                            switch ( getvalue_scr () )
                            {
                            case 0 //fertig
                            :
                                loopflag = FALSE;
                                break;
                            default:
                                /* EasyCODE < */
                                // noch nicht fertig
                                /* EasyCODE > */
                                break;
                            }
                            servgsw = pargetval.wert;
                            if ( nminf.befehl != 0 )
                            {
                            }
                            else
                            {
                                if ( nminf.fehler != 0 )
                                {
                                    nminf.position = saf->nadelp >> 1;
                                    nminf.posart = VORWAERTS;
                                    nminf.befehl=5;
                                    koor.eing_err = 41;
                                    eing.errpar = nminf.fehler;
                                    nmkey = 10;
                                    loopflag = FALSE;
                                }
                                else
                                {
                                    nminf.drehzahl = servgsw*100;
                                    nminf.befehl = 7;
                                }
                            }
                            if ( teing3.tz() )
                            {
                                gswbuf += nminf.get_drehzahl();
                                if ( ++anzcount == 10 )
                                {
                                    sprintf(eing.varstring, nmistgeschws, (gswbuf/100)*10);
                                    eing_botsch1.senden (eing.varstring);
                                    gswbuf = 0;
                                    anzcount = 0;
                                }
                                teing3.rsz();
                                teing3.sz(10);
                            }
                        }
                    }
                    break;
                    /* EasyCODE ) */
                case 0xff :
                    /* EasyCODE ( 0 
                    keine Taste gedrueckt */
                    if ( teing3.tz() )
                    {
                        gswbuf += nminf.get_drehzahl();
                        if ( ++anzcount == 10 )
                        {
                            sprintf(eing.varstring, nmistgeschws, (gswbuf/100)*10);
                            eing_botsch1.senden (eing.varstring);
                            gswbuf = 0;
                            anzcount = 0;
                        }
                        teing3.rsz();
                        teing3.sz(10);
                    }
                    break;
                    /* EasyCODE ) */
                default:
                    eing_botsch1.senden(BELLBELL);
                    break;
                }
            }
            break;
            /* EasyCODE ) */
        case 4 :
            /* EasyCODE ( 0 
            Fadenschneid-Ablauf starten */
            if ( koor.eingstop() )
            {
                // Stopanforderung
                nmkey = 10;
            }
            else
            {
                if ( (nminf.befehl != 0) || !y4.fertig() || !y5.fertig() || !y7.fertig() ||
                     !hfuss.ready() || !y30.fertig() || !y32.fertig() )
                {
                }
                else
                {
                    if ( nminf.fehler != 0 )
                    {
                        koor.eing_err = 41;
                        eing.errpar = nminf.fehler;
                        nmkey = 10;
                    }
                    else
                    {
                        nmtrb.gswakt = 0;
                        nmtrb.gswinten = FALSE;
                        outportb (INTCLR, REF1_SETCLR);
                        outportb (INTCLR, REF1_RESCLR);
                        outportb (INTMASK, REF1EI);
                        nmtrb.nmmodus = 8;
                        nmtrb.grundzustand ();
                        nup.fcutkey = 0;
                        teing1.rsz();
                        teing1.sz(saf->niszeit);
                        nmkey = 5;
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 5 :
            /* EasyCODE ( 0 
            Warten bis Fadenschneiden ein geschaltet, Position
            Fadenschneiden aus abfragen ---> Fadenschneiden aus */
            if ( y30.getZustand() )
            {
                if ( nminf.befehl != 0 )
                {
                    /* EasyCODE ( 0 
                    Fadensschneidposition pruefen */
                    aktpos = nminf.nadel_pos() - saf->nadelp;
                    if ( aktpos < 0 )
                    {
                        aktpos += PULSE_PRO_UMDR;
                    }
                    aktpos = (((long)aktpos * 360)/PULSE_PRO_UMDR);
                    if ( saf->md.y30offvograd >= saf->md.ref2 )
                    {
                        /* EasyCODE ( 0 
                        Pos. fuer Schneiden aus liegt zwischen Ref2 und NOT */
                        if ( aktpos >= saf->md.y30offvograd )
                        {
                            /* EasyCODE < */
                            // Istpos. zwischen Fadenspannungspos. und NOT
                            /* EasyCODE > */
                        }
                        else
                        {
                            break;
                        }
                        /* EasyCODE ) */
                    }
                    else
                    {
                        /* EasyCODE ( 0 
                        Pos. fuer Schneiden aus
                        liegt nach NOT */
                        if ( aktpos >= saf->md.ref2 )
                        {
                            /* EasyCODE < */
                            // Istpos. vor NOT
                            /* EasyCODE > */
                            break;
                        }
                        else
                        {
                            if ( aktpos < saf->md.y30offvograd )
                            {
                                /* EasyCODE < */
                                // Istpos. zwischen NOT und Fadenspannungspos.
                                /* EasyCODE > */
                                break;
                            }
                        }
                        /* EasyCODE ) */
                    }
                    /* EasyCODE ) */
                }
                y30.resAusgang();  // Fadenschneiden aus
                nmkey = 6;
            }
            else
            {
                if ( teing1.tz() )
                {
                    outportb (INTMASK, REF1DI | REF2DI);
                    nminf.befehl = 8;
                    koor.eing_err = 24;
                    nmkey = 10;
                }
            }
            break;
            /* EasyCODE ) */
        case 6 :
            /* EasyCODE ( 0 
            Fadenspannung auf beim Schneidstich */
            if ( nminf.befehl != 0 )
            {
                /* EasyCODE ( 0 
                Fadenspannungsposition pruefen */
                aktpos = nminf.nadel_pos() - saf->nadelp;
                if ( aktpos < 0 )
                {
                    aktpos += PULSE_PRO_UMDR;
                }
                aktpos = (((long)aktpos * 360)/PULSE_PRO_UMDR);
                if ( eing.fadsppos >= saf->md.ref2 )
                {
                    /* EasyCODE ( 0 
                    Fadenspannungspos. liegt zwischen Ref2 und NOT */
                    if ( aktpos >= eing.fadsppos )
                    {
                        /* EasyCODE < */
                        // Istpos. zwischen Fadenspannungspos. und NOT
                        /* EasyCODE > */
                    }
                    else
                    {
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Fadenspannungspos. liegt nach NOT */
                    if ( aktpos >= saf->md.ref2 )
                    {
                        /* EasyCODE < */
                        // Istpos. vor NOT
                        /* EasyCODE > */
                        break;
                    }
                    else
                    {
                        if ( aktpos < eing.fadsppos )
                        {
                            /* EasyCODE < */
                            // Istpos. zwischen NOT und Fadenspannungspos.
                            /* EasyCODE > */
                            break;
                        }
                    }
                    /* EasyCODE ) */
                }
                /* EasyCODE ) */
            }
            y5.setAuftrag();  // Fadenspannung auf
            y6.resAuftrag();  // Zusatzfadenspannung auf
            nup.fcutkey = 0;
            nmkey = 7;
            break;
            /* EasyCODE ) */
        case 7 :
            /* EasyCODE ( 0 
            Fadenschneiden */
            if ( nup.fcutportal () )
            {
                nmkey = 1;
            }
            else
            {
                if ( koor.eingstop() || (koor.nae_err != 0) )
                {
                    outportb (INTMASK, REF1DI | REF2DI);
                    nminf.befehl = 8;
                    nmkey = 10;
                }
            }
            break;
            /* EasyCODE ) */
        case 10 :
            /* EasyCODE ( 0 
            error_res */
            error_res();
            if ( taste_los() == 0xff )
            {
            }
            else
            {
                nmkey = 0;
            }
            break;
            /* EasyCODE ) */
        case 11 :
            /* EasyCODE ( 0 
            nmkey = 11, Vorbereitung zum Drehen,
                        Fuss auf, Fadensp. auf, Fadenklemme zu */
            switch ( nup.napos() )
            {
            case 0 :
                break;
            case 1 :
                koor.eing_err = 41;
                eing.errpar = nminf.fehler;
                nmkey = 10;
                break;
            default:
                hfuss.auf();     // Fuss auf
                y8.resAuftrag(); // Niederhalter auf
                y4.setAuftrag(); // Klemme zu
                y5.resAuftrag(); // Fadenspannung zu
                y31.resAuftrag(); // Unterfadenklemme zu
                nmkey=12;
                break;
            }
            /* EasyCODE ) */
        case 12 :
            /* EasyCODE ( 0 
            nmkey = 12, Vorbereitung zum Drehen,
                        Spulenabdeckung zu */
            if ( koor.eingstop() )
            {
                nmkey = 10;
            }
            else
            {
                if ( hfuss_auf.fertig()   // Fuss
                   )
                {
                    if ( e75.getZustand() )
                    {
                        // Spulenabdeckung zu
                        nmkey=13;
                    }
                    else
                    {
                        koor.eing_err = 7;
                        nmkey = 10;
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 13 :
            /* EasyCODE ( 0 
            nmkey = 13, Vorbereitung zum Drehen,
                        Oberteil senken */
            if ( koor.eingstop() )
            {
                nmkey = 10;
            }
            else
            {
                y50u2.setAuftrag(); // Oberteil senken
                nmkey=14;
            }
            break;
            /* EasyCODE ) */
        case 14 :
            /* EasyCODE ( 0 
            nmkey = 14, Vorbereitung zum Drehen,
                        Fuss ab, Fadenspannung zu, Fadenlemme auf,
                        Fadenschneiden aus, Unterfadenschieber aus */
            if ( koor.eingstop() )
            {
                nmkey = 10;
            }
            else
            {
                if ( y50u2.fertig() )
                {
                    if ( y8.fertig () )
                    {
                        if ( y8.getZustand() )
                        {
                            hfuss.ab();        // Fuss ab
                            y2fkt_aus.setAuftrag(); // Fadenwedler in Grundstellung
                            y5.resAuftrag();   // Fadenspannung zu
                            y4.setAuftrag();   // Fadenklemme zu
                            y7.resAuftrag();   // Nadeldreheinrichtung aus
                            y30.resAuftrag();  // Fadenschneiden aus
                            y32.resAuftrag();  // Unterfadenschieber aus
                            y33.setAuftrag();  // Stoffgegendruecker auf
                            nmkey=nmcont;
                        }
                        else
                        {
                            y8.setAuftrag();   // Niederhalter ab
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
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseDirekt::rfid */
/* EasyCODE F */
unsigned char KlasseDirekt::rfid(void)
{
    unsigned char rfkey = 0;
    unsigned int u;
    unsigned char loopflag=TRUE;
    unsigned char tastenmem;
    unsigned char retwert;
    while ( loopflag )
    {
        switch ( rfkey )
        {
        case 0 :
            #ifndef buerotest
                /* EasyCODE ( 0 
                Nullpunkte gueltig uns Maschine in Grundstellung ? */
                if ( saf->mnp_gueltig!=0x55 )
                {
                    koor.eing_err = 1;
                    retwert = E_RUHE;
                    loopflag = FALSE;
                    break;
                }
                else
                {
                    /* EasyCODE ( 0 
                    Maschine in Grundstellung ? */
                    if ( nup.carlp && !nae.faedelreq && saf->carposok )
                    {
                    }
                    else
                    {
                        koor.eing_err = 8;
                        retwert = E_RUHE;
                        loopflag = FALSE;
                        break;
                    }
                    /* EasyCODE ) */
                }
                /* EasyCODE ) */
            #endif //
            /* EasyCODE ( 0 
            BV017 anzeigen */
            if ( taste_los() == 0xff )
            {
                retwert = E_RUHE;
                loopflag = FALSE;
                break;
            }
            else
            {
                eing_botsch1.senden(BV017);
                eing_botsch1.senden(klammers);
                #ifdef buerotest
                    // Klammer geschlossen
                    eing_botsch1.senden(showcodes);
                #else 
                    if ( e51u2_1.getZustand() && e51u2_2.getZustand() )
                    {
                        // Klammer geschlossen
                        eing_botsch1.senden(showcodes);
                    }
                    else
                    {
                        // Klammer offen
                    }
                #endif 
                eing.ioact = TRUE;
                rfkey = 3;
                break;
            }
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            Klammer einlegen */
            switch ( keyb.taste() )
            {
            case TEINGABE :
                eing.ioact = FALSE;
                retwert = E_GST;
                loopflag = FALSE;
                break;
            case TENDE :
                eing.ioact = FALSE;
                retwert = E_EXIT;
                loopflag = FALSE;
                break;
            case TENTER :
                rfkey = 2;
                if ( taste_los() == 0xff )
                {
                    retwert = E_RUHE;
                    loopflag = FALSE;
                }
                break;
            case TSCHABLONE :
                #ifdef buerotest
                    schabl.rf.set_rfid_read();
                    eing_botsch1.senden(showcodes);
                    rfkey = 3;
                #else //
                    if ( e51u2_1.getZustand() && e51u2_2.getZustand() )
                    {
                        // Klammer geschlossen
                        y51u1.setAuftrag(); // Klammer auf
                        schabl.rf.delete_rfidprognummer();
                        eing_botsch1.senden(delcodes);
                    }
                    else
                    {
                        // Klammer offen
                        y51u2.setAuftrag(); // Klammer schliessen
                        schabl.rf.set_rfid_read();
                        eing_botsch1.senden(showcodes);
                    }
                    rfkey = 3;
                    while ( !y51u1.fertig() || !y51u2.fertig() )
                    {
                        if ( koor.eingstop() )
                        {
                            rfkey = 10;
                            break;
                        }
                    }
                #endif //
                if ( taste_los() == 0xff )
                {
                    retwert = E_RUHE;
                    loopflag = FALSE;
                }
                break;
            default:
                if ( taste_los() == 0xff )
                {
                    retwert = E_RUHE;
                    loopflag = FALSE;
                }
                break;
            case 0xff :
                break;
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            Neuen Code eingeben und schreiben */
            pargetval.minwert = PNRMIN;
            pargetval.maxwert = PNRMAX;
            pargetval.auftrag = 0;
            pargetval.altwert = 0;
            pargetval.pikto = rfidps;
            pargetval.kommastellen = 0;
            pargetval.string = rfidstr;
            switch ( getvalue_eing() )
            {
            case 0 // ENTER
            :
                eing_botsch1.senden(BV017);
                eing_botsch1.senden(klammers);
                #ifdef buerotest
                    // Klammer geschlossen
                    eing_botsch1.senden(showcodes);
                #else 
                    if ( e51u2_1.getZustand() && e51u2_2.getZustand() )
                    {
                        // Klammer geschlossen
                        eing_botsch1.senden(showcodes);
                    }
                    else
                    {
                        // Klammer offen
                    }
                #endif 
                schabl.rf.set_rfidprognummer (pargetval.wert, RFID_MASCHINENKLASSE);
                rfkey = 4;
                break;
            case 1 // ESC
            :
                rfkey = 0;
                break;
            default:
                // Stoppanf
                rfkey = 10;
                break;
            }
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            Code anzeigen */
            #ifdef buerotest
                switch ( u=schabl.rf.get_rfidprognummer (RFID_MASCHINENKLASSE) )
                {
                case 0xfffd :
                    // Programmnummer ungueltig
                    eing_botsch1.senden(rfidtext2);
                    rfkey = 1;
                    break;
                case 0xfffe :
                    // Lesen der Programmnummer nicht fertig
                    break;
                case 0xffff :
                    koor.eing_err = 46;
                    eing.errpar = schabl.rf.rfid_err;
                    rfkey = 10;
                    break;
                default:
                    sprintf (eing.varstring, rfidtext1, u);
                    eing_botsch1.senden(eing.varstring);
                    rfkey = 1;
                    break;
                }
            #else //
                if ( (!e79.getZustand() && !e80.getZustand()) || !e51u2_1.getZustand() || !e51u2_2.getZustand() )
                {
                    switch ( u=schabl.rf.get_rfidprognummer (RFID_MASCHINENKLASSE) )
                    {
                    case 0xfffd :
                        // Programmnummer ungueltig
                        eing_botsch1.senden(rfidtext2);
                        rfkey = 1;
                        break;
                    case 0xfffe :
                        // Lesen der Programmnummer nicht fertig
                        break;
                    default:
                        schabl.rf.rfid_err = 0;
                        eing_botsch1.senden(rfidtext2);
                        rfkey = 1;
                        break;
                    }
                }
                else
                {
                    switch ( u=schabl.rf.get_rfidprognummer (RFID_MASCHINENKLASSE) )
                    {
                    case 0xfffd :
                        // Programmnummer ungueltig
                        eing_botsch1.senden(rfidtext2);
                        rfkey = 1;
                        break;
                    case 0xfffe :
                        // Lesen der Programmnummer nicht fertig
                        break;
                    case 0xffff :
                        koor.eing_err = 46;
                        eing.errpar = schabl.rf.rfid_err;
                        rfkey = 10;
                        break;
                    default:
                        sprintf (eing.varstring, rfidtext1, u);
                        eing_botsch1.senden(eing.varstring);
                        rfkey = 1;
                        break;
                    }
                }
            #endif //
            break;
            /* EasyCODE ) */
        case 4 :
            /* EasyCODE ( 0 
            Code schreiben fertig ? */
            switch ( schabl.rf.check_rfidprognummer_write_rdy () )
            {
            case 0 :
                // fertig
                schabl.rf.set_rfid_read();
                rfkey = 3;
                break;
            case 0xff :
                koor.eing_err = 46;
                eing.errpar = schabl.rf.rfid_err;
                rfkey = 10;
                break;
            default:
                // nicht fertig
                break;
            }
            break;
            /* EasyCODE ) */
        case 10 :
            /* EasyCODE ( 0 
            error_res */
            error_res();
            if ( taste_los() == 0xff )
            {
                retwert = E_RUHE;
                loopflag = FALSE;
            }
            else
            {
                eing_botsch1.senden(BV017);
                eing_botsch1.senden(klammers);
                #ifdef buerotest
                    // Klammer geschlossen
                    eing_botsch1.senden(showcodes);
                #else 
                    if ( e51u2_1.getZustand() && e51u2_2.getZustand() )
                    {
                        // Klammer geschlossen
                        eing_botsch1.senden(showcodes);
                    }
                    else
                    {
                        // Klammer offen
                    }
                #endif 
                eing_botsch1.senden(rfidtext2);
                rfkey = 1;
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
//bctest() blieb bei der Portal haengen derzeit ohne Anzeige BV039!
//ggf. ist bctest() auch nicht für stat. BC geeignet
/* EasyCODE_STRUCTURED_COMMENT_START */
/*
EasyCODE_COMMENT_START EasyCODE ( 0 
KlasseDirekt::bctest EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
unsigned char KlasseDirekt::bctest(void)
{
    unsigned char bckey = 0;
    unsigned int u;
    while ( TRUE )
    {
        switch ( bckey )
        {
        case 0 :
            if ( EasyCODE_COMMENT_START nup.carmnp EasyCODE_COMMENT_END nup.carlp && !nae.faedelreq && saf->carposok )
            {
                bckey = 1;
                break;
            }
            else
            {
                koor.eing_err = 8;
                return E_RUHE;
            }
        case 1 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            BV039 anzeigen EasyCODE_COMMENT_END
            if ( taste_los() == 0xff )
            {
                return E_RUHE;
            }
            else
            {
                eing_botsch1.senden(BV039);
                eing_botsch1.senden(klammers);
                #ifdef buerotest
                    // Klammer geschlossen
                    schabl.bc.set_bc_read();
                    eing_botsch1.senden(bctaste);
                    bckey = 2;
                #else 
                    if ( EasyCODE_COMMENT_START y11u2.getZustand() EasyCODE_COMMENT_END
                         e51u2_2.getZustand() )
                    {
                        // Klammer geschlossen
                        schabl.bc.set_bc_read();
                        eing_botsch1.senden(bctaste);
                        bckey = 2;
                    }
                    else
                    {
                        // Klammer offen
                        eing_botsch1.senden(bcanztext2);
                        bckey = 3;
                    }
                #endif 
                eing.ioact = TRUE;
                break;
            }
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 2 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Barcode lesen EasyCODE_COMMENT_END
            switch ( u=schabl.bc.get_bcprognummer () )
            {
            case 0xfffd :
                // Programmnummer ungueltig
                eing_botsch1.senden(bcanztext2);
                bckey = 3;
                break;
            case 0xfffe :
                // Lesen der Programmnummer nicht fertig
                break;
            case 0xffff :
                koor.eing_err = 49;
                eing.errpar = schabl.bc.bc_err;
                bckey = 10;
                break;
            default:
                sprintf (eing.varstring, bcanztext1, u);
                eing_botsch1.senden(eing.varstring);
                bckey = 3;
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 3 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            Tasten abfragen EasyCODE_COMMENT_END
            switch ( keyb.taste() )
            {
            case TEINGABE :
                nae.homereq = TRUE;
                eing.ioact = FALSE;
                return E_GST;
            case TENDE :
                nae.homereq = TRUE;
                eing.ioact = FALSE;
                return E_EXIT;
            case TBARCODE :
                //Scanner an
                schabl.bc.set_bc_read();
                eing_botsch1.senden(bcanztext2);
                bckey=2;
                if ( taste_los() == 0xff )
                {
                    return E_RUHE;
                }
                else
                {
                    break;
                }
            case TSCHABLONE :
                #ifdef buerotest
                #else 
                    if ( EasyCODE_COMMENT_START y11u2.getZustand() EasyCODE_COMMENT_END
                         e51u2_2.getZustand() )
                    {
                        // Klammer geschlossen
                        //y11u1.setAuftrag(); // Klammer auf
                        y51u1.setAuftrag(); // Klammer auf
                        eing_botsch1.senden(delbctaste);
                        eing_botsch1.senden(bcanztext2);
                    }
                    else
                    {
                        // Klammer offen
                        //y11u2.setAuftrag(); // Klammer schliessen
                        y51u2.setAuftrag(); // Klammer schliessen
                        schabl.bc.set_bc_read();
                        eing_botsch1.senden(bctaste);
                        bckey = 2;
                    }
                    while ( //!y11u1.fertig() || !y11u2.fertig()
                            !y51u1.fertig() || !y51u2.fertig() )
                    {
                        if ( koor.eingstop() )
                        {
                            bckey = 10;
                            break;
                        }
                    }
                #endif 
                if ( taste_los() == 0xff )
                {
                    return E_RUHE;
                }
                else
                {
                    break;
                }
            default:
                if ( taste_los() == 0xff )
                {
                    return E_RUHE;
                }
                else
                {
                    break;
                }
            case 0xff :
                break;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 10 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            error_res EasyCODE_COMMENT_END
            error_res();
            if ( taste_los() == 0xff )
            {
                return E_RUHE;
            }
            else
            {
                eing_botsch1.senden(BV039);
                eing_botsch1.senden(klammers);
                eing_botsch1.senden(bcanztext2);
                bckey = 3;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        default:
            break;
        }
    }
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
*/
/* EasyCODE_STRUCTURED_COMMENT_END */
/* EasyCODE ) */
/* EasyCODE ) */
