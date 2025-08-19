/* EasyCODE V8 */
/* EasyCODE ( 0 
proguppo */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
//*************************************
// Datei:        proguppo.c
// Klasse:       Kl3590
// Ersteller:    Halfmann EES 2856

#include "portal.h"
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::nafupos */
/* EasyCODE F */
// Das Up positioniert die Nadel und den Fuss. Im Nahtbildprogrammieren
// erfolgt die Positionierung abhaengig vom Programmzustandsblock und
// ob das Oberteil angehoben oder abgesenkt ist.
//
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:  TRUE   -  fertig ohne Fehler
//                      FALSE  -  Fehler oder Stopanforderung

unsigned char KlasseProgUp::nafupos (void)
{
    /* EasyCODE < */
    unsigned char retwert=TRUE;
    /* EasyCODE > */
    #ifdef buerotest
        return retwert;
    #else 
        if ( (eing.geteingkey()==E_PROG) && eing.prog.pzbl.faden && e50u2.getZustand() )
        {
        }
        else
        {
            while ( TRUE )
            {
                switch ( nup.napos () )
                {
                case 0 :
                    continue;
                case 1 :
                    koor.eing_err = 41;
                    eing.errpar = nminf.fehler;
                    retwert = FALSE;
                    break;
                default:
                    hfuss.auf();           // Fuss auf
                    y8.resAuftrag();       // Niederhalter auf
                    y4.setAuftrag();       // Klemme zu
                    y5.resAuftrag();
                    while ( !hfuss_auf.fertig() || !y8.fertig() || !y4.fertig() || !y5.fertig() )
                    {
                        if ( koor.eingstop() )
                        {
                            retwert = FALSE;
                            break;
                        }
                    }
                    break;
                }
                break;
            }
        }
        return retwert;
    #endif 
}

/* EasyCODE ) */
/* EasyCODE_STRUCTURED_COMMENT_START */
/*
EasyCODE_COMMENT_START EasyCODE ( 0 
KlasseProgUp::carmov EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
// Das Up verfaehrt den Schlitten nach koorsoll [0]. Up wird nur von smove und find benutzt.

void KlasseProgUp::carmov(void)
{
    int hilfsvar1, hilfsvar2;
    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
    smx.Fkt2_on();  // Softstepping SM-Endst. X
    smy.Fkt2_on();  // Softstepping SM-Endst. Y
    EasyCODE_COMMENT_START EasyCODE - EasyCODE_COMMENT_END
    hilfsvar1 = nup.koorsoll[0].motorx - nup.workbuf.koorist.motorx;
    if ( hilfsvar1 < 0 )
    {
        smx.status = PAWRAMPCV | SMXMINUS;
        hilfsvar1 = -hilfsvar1;
    }
    else
    {
        smx.status = PAWRAMPCV | SMXPLUS;
    }
    hilfsvar2 = nup.koorsoll[0].motory - nup.workbuf.koorist.motory;
    if ( hilfsvar2 < 0 )
    {
        smy.status = SMYMINUS;
        hilfsvar2 = -hilfsvar2;
    }
    else
    {
        smy.status = SMYPLUS;
    }
    switch ( nae.vfindex )
    {
    case 0 :
        smx.vort = saf->xvorteiler1;
        smy.status |= EFR_CLK_128;
        break;
    case 1 :
        smx.vort = saf->xvorteiler2;
        smy.status |= EFR_CLK_256;
        break;
    case 2 :
        smx.vort = saf->xvorteiler3;
        smy.status |= EFR_CLK_128;
        break;
    default:
        smx.vort = saf->xvorteiler4;
        smy.status |= EFR_CLK_64;
        break;
    }
    if ( hilfsvar1 != 0 )
    {
        smx.init();                    // SmX initialisieren
        smx.rampadr = 1;
        smx.gespls = hilfsvar1;
        smx.start_Pulse();             // Rampe fahren ohne Startverzoegerung
    }
    if ( hilfsvar2 != 0 )
    {
        smy.init();                    // SmY initialisieren
        smy.rampadr = smy_ststadr;
        smy.gespls = hilfsvar2;
        smy.start_Pulse();             // Rampe fahren ohne Startverzoegerung
    }
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
*/
/* EasyCODE_STRUCTURED_COMMENT_END */
/* EasyCODE ( 0 
KlasseProgUp::set_pzbl */
/* EasyCODE F */
// Das Up bearbeitet den Programmzustandsblock vom Programmanfang bis ein-
// schliesslich dem uebergebenen Abschnitt.
//
// Eingangs-Parameter:   Zeiger auf Abschnitt
// Rueckmeldung:         keine
void KlasseProgUp::set_pzbl (struct p_abschstruct * ptr)
{
    /* EasyCODE < */
    struct p_abschstruct * abptr;
    
    // Programmzustandsblock ruecksetzen
    eing.prog.pzbl.faden = FALSE;
    eing.prog.pzbl.pout1 = FALSE;
    eing.prog.pzbl.pout2 = FALSE;
    eing.prog.pzbl.pout3 = FALSE;
    eing.prog.pzbl.pout4 = FALSE;
    eing.prog.pzbl.pout6 = FALSE;
    eing.prog.pzbl.zusp = FALSE;
    eing.prog.pzbl.redgsw = FALSE;
    eing.prog.pzbl.kurve = FALSE;
    eing.prog.pzbl.stichl = 0;
    eing.prog.pzbl.posunten = saf->md.posunten.pos;
    eing.prog.pzbl.hub = saf->md.hub;
    eing.prog.pzbl.flag1= FALSE;
    
    abptr = (struct p_abschstruct *) (ndatanf + GPROGNR);
    /* EasyCODE > */
    while ( abptr <= ptr )
    {
        set_pzbl_abschnitt (abptr);
        abptr++;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::set_pzbl_abschnitt */
/* EasyCODE F */
// Das Up bearbeitet den Programmzustandsblock fuer den uebergebenen Abschnitt.
//
// Eingangs-Parameter:  Zeiger auf Abschnitt
// Rueckmeldung:        keine
void KlasseProgUp::set_pzbl_abschnitt (struct p_abschstruct * ptr)
{
    switch ( ptr->art )
    {
    case AB_ANNAEH :
        eing.prog.pzbl.faden = TRUE;
        break;
    case AB_SCHNEID :
        eing.prog.pzbl.faden = FALSE;
        break;
    case AB_NAEHAUS :
        eing.prog.pzbl.faden = FALSE;
        break;
    case AB_OUTSET :
        /* EasyCODE ( 0 
        Ausgang setzen */
        switch ( ptr->par2 )
        {
        case 1 :
            eing.prog.pzbl.pout1 = TRUE;
            break;
        case 2 :
            eing.prog.pzbl.pout2 = TRUE;
            break;
        case 3 :
            eing.prog.pzbl.pout3 = TRUE;
            break;
        case 4 :
            eing.prog.pzbl.pout4 = TRUE;
            break;
        case 6 :
            eing.prog.pzbl.pout6 = TRUE;
            break;
        default:
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_OUTRES :
        /* EasyCODE ( 0 
        Ausgang ruecksetzen */
        switch ( ptr->par2 )
        {
        case 1 :
            eing.prog.pzbl.pout1 = FALSE;
            break;
        case 2 :
            eing.prog.pzbl.pout2 = FALSE;
            break;
        case 3 :
            eing.prog.pzbl.pout3 = FALSE;
            break;
        case 4 :
            eing.prog.pzbl.pout4 = FALSE;
            break;
        case 6 :
            eing.prog.pzbl.pout6 = FALSE;
            break;
        default:
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_FUSSNIVEAU :
        eing.prog.pzbl.posunten = ptr->par2;
        hfuss.akt_posunten.pos = ptr->par2;
        break;
    case AB_FUSSHUB :
        eing.prog.pzbl.hub = ptr->par2;
        hfuss.akt_hub = ptr->par2;
        break;
    case AB_ZUSPGEIN :
        eing.prog.pzbl.zusp = TRUE;
        break;
    case AB_ZUSPGAUS :
        eing.prog.pzbl.zusp = FALSE;
        break;
    case AB_REDGSWEIN :
        eing.prog.pzbl.redgsw = TRUE;
        break;
    case AB_REDGSWAUS :
        eing.prog.pzbl.redgsw = FALSE;
        break;
    case AB_KURVSTP :
        eing.prog.pzbl.kurve = TRUE;
        break;
    case AB_KURVENDP :
        eing.prog.pzbl.kurve = FALSE;
        break;
    case AB_STDST :
        /* EasyCODE = */
    case AB_STICHL :
        eing.prog.pzbl.stichl = ptr->par1;
        break;
    case AB_FLAGSET :
        eing.prog.pzbl.flag1 = TRUE;
        break;
    case AB_FLAGRES :
        eing.prog.pzbl.flag1 = FALSE;
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::pstatuszeile */
/* EasyCODE F */
// Das Up zeigt die Statuszeile beim Programmieren an.
void KlasseProgUp::pstatuszeile(void)
{
    if ( (lastabsch[0] != 0) && ((eing.prog.key == 2) || (eing.prog.key == 6)) )
    {
        sprintf (eing.varstring1, pikdel, lastabsch);
        eing_botsch1.senden(eing.varstring1);
        if ( (strcmp(lastabsch,PFUSS) == 0) && !dauertakten )
        {
            eing_botsch1.senden (fusspikto);
        }
        else
        {
            if ( (strcmp(lastabsch,PENDE) == 0) && !dauertakten )
            {
                eing_botsch1.senden (endepikto);
            }
            else
            {
                if ( (strcmp(lastabsch,PSTOP) == 0) && dauertakten )
                {
                    eing_botsch1.senden (stoppikto);
                }
            }
        }
    }
    switch ( ptr_in_block(eing.prog.ppointer) )
    {
    case 2 :
    case 3 :
    case 0xff :
        eing_botsch1.senden(ION);
        break;
    default:
        break;
    }
    switch ( eing.prog.ppointer->art )
    {
    case AB_ANNAEH :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, annaehstr);
        sprintf (lastabsch, PANNAEHEN);
        break;
    case AB_SCHNEID :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, schneidstr);
        sprintf (lastabsch, PFADENSCHNEIDEN);
        break;
    case AB_ANFRIEGEL :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, anfriegelstr);
        sprintf (lastabsch, PANFANGSRIEGEL);
        break;
    case AB_ENDRIEGEL :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, endriegelstr);
        sprintf (lastabsch, PENDRIEGEL);
        break;
    case AB_NAEHAUS :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, naehausstr);
        sprintf (lastabsch, PNAEHENAUS);
        break;
    case AB_PRENDE :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, progendestr);
        sprintf (lastabsch, PEND);
        break;
    case AB_HIND :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, hindstr);
        sprintf (lastabsch, PHINDERNIS);
        break;
    case AB_STDST :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, stdststr);
        sprintf (lastabsch, PSTANDARDSTICHL);
        break;
    case AB_STICHL :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, stichlstr);
        sprintf (lastabsch, PSTI);
        break;
    case AB_STICHB :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, stichbstr);
        sprintf (lastabsch, PSTICHBREITE);
        break;
    case AB_PNR :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, pnrstr);
        sprintf (lastabsch, PPNR);
        break;
    case AB_KLCODE :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, klcodestr);
        sprintf (lastabsch, PCODE);
        break;
    case AB_AP :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, apstr);
        sprintf (lastabsch, PANFANGSPUNKT);
        break;
    case AB_EIL :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, eilstr);
        sprintf (lastabsch, PEILGANG);
        break;
    case AB_STICH :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, stichstr);
        sprintf (lastabsch, PSTICH);
        break;
    case AB_GERADE :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, geradestr);
        sprintf (lastabsch, PGERADE);
        break;
    case AB_BOGENSTP :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, bogenstr1);
        sprintf (lastabsch, PBOGENSTP1);
        break;
    case AB_BOGENENDP :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, bogenstr2);
        sprintf (lastabsch, PBOGENSTP2);
        break;
    case AB_KREISSTP :
        if ( (eing.prog.ppointer-1)->art == AB_KREISSTP )
        {
            sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, kreisstr2);
            sprintf (lastabsch, PKREISSTP2);
        }
        else
        {
            sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, kreisstr1);
            sprintf (lastabsch, PKREISSTP1);
        }
        break;
    case AB_KREISENDP :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, kreisstr3);
        sprintf (lastabsch, PKREISSTP3);
        break;
    case AB_KURVSTP :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, kurvstpstr);
        sprintf (lastabsch, PKURVENPUNKT);
        break;
    case AB_KURVENDP :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, kurvendstr);
        sprintf (lastabsch, PKURVENENDE);
        break;
    case AB_UFWSTOP :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, ufwstopstr);
        sprintf (lastabsch, PUFWSTOP);
        break;
    case AB_PSTOP :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, stopstr);
        sprintf (lastabsch, PSTOP);
        break;
    case AB_REDGSWEIN :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, redgsweinstr);
        sprintf (lastabsch, PREDGSWEIN);
        break;
    case AB_REDGSWAUS :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, redgswausstr);
        sprintf (lastabsch, PREDGSWAUS);
        break;
    case AB_ZUSPGEIN :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, zuspgeinstr);
        sprintf (lastabsch, PZUSSPGEIN);
        break;
    case AB_ZUSPGAUS :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, zuspgausstr);
        sprintf (lastabsch, PZUSSPGAUS);
        break;
    case AB_OUTSET :
        switch ( eing.prog.ppointer->par2 )
        {
        case 6 :
            sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, oberthebeneinstr);
            sprintf (lastabsch, POBERTEILHEBENEIN);
            break;
        default:
            sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, outstr);
            sprintf (lastabsch, POUT);
            break;
        }
        break;
    case AB_OUTRES :
        switch ( eing.prog.ppointer->par2 )
        {
        case 6 :
            sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, oberthebenausstr);
            sprintf (lastabsch, POBERTEILHEBENAUS);
            break;
        default:
            sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, outstr);
            sprintf (lastabsch, POUT);
            break;
        }
        break;
    case AB_FLAGSET :
        switch ( eing.prog.ppointer->par2 )
        {
        case 1 :
            sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, nadeldrehenausstr);
            sprintf (lastabsch, PNADELDREHEN_AUS);
            break;
        default:
            break;
        }
        break;
    case AB_FLAGRES :
        switch ( eing.prog.ppointer->par2 )
        {
        case 1 :
            sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, nadeldreheneinstr);
            sprintf (lastabsch, PNADELDREHEN_EIN);
            break;
        default:
            break;
        }
        break;
    case AB_FUSSNIVEAUSTUFE :
    case AB_FUSSNIVEAU :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, fussniveaustr);
        sprintf (lastabsch, PHFLEVEL);
        break;
    case AB_FUSSHUBSTUFE :
    case AB_FUSSHUB :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, fusshubstr);
        sprintf (lastabsch, PFUSS);
        break;
    case AB_WHIGH :
    case AB_WLOW :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, instr);
        sprintf (lastabsch, PIN);
        break;
    case AB_WZEIT :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, zeitstr);
        sprintf (lastabsch, P_TIME);
        break;
    case AB_DREHZ :
        sprintf (eing.varstring, statuszeilenstr, eing.prog.abschnr, drehzstr);
        sprintf (lastabsch, PDREHZAHL);
        break;
    default:
        break;
    }
    eing_botsch1.senden(eing.varstring);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::abschparanz */
/* EasyCODE F */
// Das Up zeigt die Parameter der Abschnitte an.
void KlasseProgUp::abschparanz(void)
{
    eing_botsch1.senden (delparastr);
    switch ( eing.prog.ppointer->art )
    {
    case AB_STDST :
    case AB_STICHL :
        /* EasyCODE ( 0 
        Standardstichlaenge und Stichlaenge */
        sprintf(eing.varstring,parastr1,((float)eing.prog.ppointer->par1/100));
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    case AB_STICHB :
        /* EasyCODE ( 0 
        Stichbreite */
        sprintf(eing.varstring,parastr1,((float)eing.prog.ppointer->par1/10));
        if ( eing.prog.ppointer->par1==0 )
        {
        }
        else
        {
            switch ( eing.prog.ppointer->par2 )
            {
            case 0 :
                sprintf(eing.varstring1, parastr2, "1. " CHRCODE "\x1b");
                break;
            case 1 :
                sprintf(eing.varstring1, parastr2, "1. " CHRCODE "\x1a");
                break;
            case 2 :
                sprintf(eing.varstring1, parastr2, CHRCODE "\x1b");
                break;
            case 3 :
                sprintf(eing.varstring1, parastr2, CHRCODE "\x1a");
                break;
            default:
                break;
            }
            strcat (eing.varstring, eing.varstring1);
        }
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    case AB_PNR :
        /* EasyCODE ( 0 
        Programmnummer, Nadelanzahl */
        if ( *(ndatanf+KPROGNR)==4 )
        {
            sprintf(eing.varstring, parastr11, eing.prog.ppointer->par1, P2NADEL);
        }
        else
        {
            sprintf(eing.varstring, parastr11, eing.prog.ppointer->par1, P1NADEL);
        }
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    case AB_KLCODE :
        /* EasyCODE ( 0 
        Klammercode */
        if ( eing.prog.ppointer->par1==-1 )
        {
            sprintf(eing.varstring, parastr4, "--");
        }
        else
        {
            sprintf(eing.varstring, parastr3, eing.prog.ppointer->par1);
        }
        eing_botsch1.senden(eing.varstring);
        /* EasyCODE ) */
        break;
    case AB_UFWSTOP :
    case AB_PSTOP :
    case AB_REDGSWEIN :
    case AB_REDGSWAUS :
    case AB_ZUSPGEIN :
    case AB_ZUSPGAUS :
        /* EasyCODE ( 0 
        progr. Stop, red. Geschw. ein / aus,
        Zusatzspannung ein / aus */
        if ( eing.prog.ppointer->par1 != 0 )
        {
            sprintf(eing.varstring, parastr5, eing.prog.ppointer->par1);
            eing_botsch1.senden(eing.varstring);
        }
        break;
        /* EasyCODE ) */
    case AB_FUSSNIVEAU :
        /* EasyCODE ( 0 
        Niveau Huepferfuss (Wert) */
        sprintf(eing.varstring, parastr10 , (float)eing.prog.ppointer->par2/10);
        if ( eing.prog.ppointer->par1 != 0 )
        {
            sprintf(eing.varstring1, parastr5 , eing.prog.ppointer->par1);
            strcat (eing.varstring, eing.varstring1);
        }
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    case AB_FUSSHUB :
        /* EasyCODE ( 0 
        Hub Huepferfuss (Wert) */
        sprintf(eing.varstring, parastr10 , (float)eing.prog.ppointer->par2/10);
        if ( eing.prog.ppointer->par1 != 0 )
        {
            sprintf(eing.varstring1, parastr5 , eing.prog.ppointer->par1);
            strcat (eing.varstring, eing.varstring1);
        }
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    case AB_FUSSNIVEAUSTUFE :
        /* EasyCODE ( 0 
        Niveau Huepferfuss (Stufe) */
        sprintf(eing.varstring, parastr12 , eing.prog.ppointer->par2);
        if ( eing.prog.ppointer->par1 != 0 )
        {
            sprintf(eing.varstring1, parastr5 , eing.prog.ppointer->par1);
            strcat (eing.varstring, eing.varstring1);
        }
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    case AB_FUSSHUBSTUFE :
        /* EasyCODE ( 0 
        Hub Huepferfuss (Stufe) */
        sprintf(eing.varstring, parastr12 , eing.prog.ppointer->par2);
        if ( eing.prog.ppointer->par1 != 0 )
        {
            sprintf(eing.varstring1, parastr5 , eing.prog.ppointer->par1);
            strcat (eing.varstring, eing.varstring1);
        }
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    case AB_OUTSET :
        /* EasyCODE ( 0 
        Ausgang setzen */
        switch ( eing.prog.ppointer->par2 )
        {
        case 5 :
            sprintf(eing.varstring, parastr6 , "LEVEL2");
            if ( eing.prog.ppointer->par1 != 0 )
            {
                sprintf(eing.varstring1, parastr5 , eing.prog.ppointer->par1);
                strcat (eing.varstring, eing.varstring1);
            }
            eing_botsch1.senden(eing.varstring);
            break;
        case 6 :
            /* EasyCODE < */
            //Oberteil heben beim Eilgang ein
            /* EasyCODE > */
            if ( eing.prog.ppointer->par1 != 0 )
            {
                sprintf(eing.varstring, parastr5 , eing.prog.ppointer->par1);
                eing_botsch1.senden(eing.varstring);
            }
            break;
        default:
            sprintf(eing.varstring, parastr7, eing.prog.ppointer->par2, "SET");
            if ( eing.prog.ppointer->par1 != 0 )
            {
                sprintf(eing.varstring1, parastr5 , eing.prog.ppointer->par1);
                strcat (eing.varstring, eing.varstring1);
            }
            eing_botsch1.senden(eing.varstring);
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_OUTRES :
        /* EasyCODE ( 0 
        Ausgang ruecksetzen */
        switch ( eing.prog.ppointer->par2 )
        {
        case 5 :
            sprintf(eing.varstring, parastr6 , "LEVEL1");
            if ( eing.prog.ppointer->par1 != 0 )
            {
                sprintf(eing.varstring1, parastr5 , eing.prog.ppointer->par1);
                strcat (eing.varstring, eing.varstring1);
            }
            eing_botsch1.senden(eing.varstring);
            break;
        case 6 :
            /* EasyCODE < */
            //Oberteil heben beim Eilgang aus
            /* EasyCODE > */
            if ( eing.prog.ppointer->par1 != 0 )
            {
                sprintf(eing.varstring, parastr5 , eing.prog.ppointer->par1);
                eing_botsch1.senden(eing.varstring);
            }
            break;
        default:
            sprintf(eing.varstring, parastr7, eing.prog.ppointer->par2, "RES");
            if ( eing.prog.ppointer->par1 != 0 )
            {
                sprintf(eing.varstring1, parastr5 , eing.prog.ppointer->par1);
                strcat (eing.varstring, eing.varstring1);
            }
            eing_botsch1.senden(eing.varstring);
            break;
        }
        break;
        /* EasyCODE ) */
    case AB_WHIGH :
        /* EasyCODE ( 0 
        Warten auf Eingang High */
        sprintf(eing.varstring, parastr7, eing.prog.ppointer->par2, "HIGH");
        if ( eing.prog.ppointer->par1 != 0 )
        {
            sprintf(eing.varstring1, parastr5, eing.prog.ppointer->par1);
            strcat (eing.varstring, eing.varstring1);
        }
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    case AB_WLOW :
        /* EasyCODE ( 0 
        Warten auf Eingang Low */
        sprintf(eing.varstring, parastr7, eing.prog.ppointer->par2, "LOW");
        if ( eing.prog.ppointer->par1 != 0 )
        {
            sprintf(eing.varstring1, parastr5, eing.prog.ppointer->par1);
            strcat (eing.varstring, eing.varstring1);
        }
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    case AB_WZEIT :
        /* EasyCODE ( 0 
        Warten auf Zeit */
        sprintf(eing.varstring, parastr8, (float)eing.prog.ppointer->par2/100);
        if ( eing.prog.ppointer->par1 != 0 )
        {
            sprintf(eing.varstring1, parastr5, eing.prog.ppointer->par1);
            strcat (eing.varstring, eing.varstring1);
        }
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    case AB_DREHZ :
        /* EasyCODE ( 0 
        Drehzahl */
        sprintf(eing.varstring, parastr3, eing.prog.ppointer->par2);
        if ( eing.prog.ppointer->par1 != 0 )
        {
            sprintf(eing.varstring1, parastr5, eing.prog.ppointer->par1);
            strcat (eing.varstring, eing.varstring1);
        }
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    case AB_ANNAEH :
        /* EasyCODE ( 0 
        Annaehen */
        if ( (*((unsigned char *)eing.prog.ppointer + 1)!=0) || (*((unsigned char *)eing.prog.ppointer + 3)!=0) )
        {
            sprintf(eing.varstring,parastr9,*((unsigned char *)eing.prog.ppointer+1), *((unsigned char *)eing.prog.ppointer+2),
                                             *((unsigned char *)eing.prog.ppointer+3), *((unsigned char *)eing.prog.ppointer+4));
            eing_botsch1.senden(eing.varstring);
        }
        break;
        /* EasyCODE ) */
    case AB_ANFRIEGEL :
        /* EasyCODE ( 0 
        Anfangsriegel */
        sprintf(eing.varstring,parastr9,*((unsigned char *)eing.prog.ppointer+1), *((unsigned char *)eing.prog.ppointer+2),
                                         *((unsigned char *)eing.prog.ppointer+3), *((unsigned char *)eing.prog.ppointer+4));
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    case AB_SCHNEID :
        /* EasyCODE ( 0 
        Schneiden */
        if ( (*((unsigned char *)eing.prog.ppointer + 1)!=0) || (*((unsigned char *)eing.prog.ppointer + 3)!=0) )
        {
            sprintf(eing.varstring, parastr9, *((unsigned char *)eing.prog.ppointer+1), *((unsigned char *)eing.prog.ppointer+2),
                                              *((unsigned char *)eing.prog.ppointer+3), *((unsigned char *)eing.prog.ppointer+4));
            eing_botsch1.senden(eing.varstring);
        }
        break;
        /* EasyCODE ) */
    case AB_ENDRIEGEL :
        /* EasyCODE ( 0 
        Endriegel */
        sprintf(eing.varstring, parastr9, *((unsigned char *)eing.prog.ppointer+1), *((unsigned char *)eing.prog.ppointer+2),
                                          *((unsigned char *)eing.prog.ppointer+3), *((unsigned char *)eing.prog.ppointer+4));
        eing_botsch1.senden(eing.varstring);
        break;
        /* EasyCODE ) */
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::kooranz */
/* EasyCODE F */
// Das Up zeigt die aktuellen Koordinaten,
// bezogen auf den Referenzpunkt, an.
void KlasseProgUp::kooranz(void)
{
    if ( eing.prog.koorptr.xbild == 15000 )
    {
        eing_botsch1.senden(koordinatenstr2);
    }
    else
    {
        sprintf(eing.varstring1, koordinatenstr1, eing.prog.koorptr.xbild - eing.prog.koorref.x, eing.prog.koorptr.ybild - eing.prog.koorref.y);
        eing_botsch1.senden(eing.varstring1);
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::mark_check */
/* EasyCODE F */
// Das Up ueberprueft, ob ein Block ordnungsgemaess markiert ist.
//
// Ausgangs-Parameter:
//     0  -   Block nicht ordnungsgemaess markiert
//     1  -   blockend <= blockanf
//    0xff - Block ist ordnungsgemaess markiert
unsigned char KlasseProgUp::mark_check (void)
{
    if ( (eing.prog.blockanf == NULL) || (eing.prog.blockend == NULL) )
    {
        return 0;
    }
    else
    {
        if ( eing.prog.blockanf >= eing.prog.blockend )
        {
            return 1;
        }
        else
        {
            return 0xff;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::mark_enable_test */
/* EasyCODE F */
// Das Up ueberprueft, ob ein uebergebener Zeiger auf einen markierbaren Abschnitt zeigt.
//
// Ausgangs-Parameter:  TRUE  -  Abschnitt ist markierbar
//                      FALSE -  Abschnitt ist nicht markierbar

unsigned char KlasseProgUp::mark_enable_test (struct p_abschstruct *ptr)
{
    unsigned char retwert;
    switch ( ptr->art )
    {
    case AB_KURVSTP :
        /* EasyCODE = */
    case AB_KREISSTP :
        /* EasyCODE = */
    case AB_BOGENSTP :
        /* EasyCODE = */
    case AB_PNR :
        /* EasyCODE = */
    case AB_KLCODE :
        /* EasyCODE = */
    case AB_HIND :
        /* EasyCODE = */
    case AB_MNP :
        /* EasyCODE = */
    case AB_PRENDE :
        retwert = FALSE;
        break;
    default:
        retwert = TRUE;
        break;
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::ptr_in_block */
/* EasyCODE F */
// Das Up ueberprueft, ob ein uebergebener Zeiger vor, innerhalb oder nach einem
// ordnungsgemaess markierten Block liegt.
//
// Ausgangs-Parameter:
//      0  -   Block nicht ordnungsgemaess markiert
//      1  -   blockend <= blockanf
//      2  -   Zeiger = Blockanfang
//      3  -   Zeiger = Blockende
//      4  -   Zeiger < Blockanfang
//      5  -   Zeiger > Blockende
//    0xff - Zeiger liegt innerhalb eines Blocks

unsigned char KlasseProgUp::ptr_in_block (struct p_abschstruct *ptr)
{
    switch ( mark_check () )
    {
    case 0 :
        return 0;
    case 1 :
        return 1;
    default:
        if ( ptr == eing.prog.blockanf )
        {
            return 2;
        }
        else
        {
            if ( ptr == eing.prog.blockend )
            {
                return 3;
            }
            else
            {
                if ( ptr < eing.prog.blockanf )
                {
                    return 4;
                }
                else
                {
                    if ( ptr > eing.prog.blockend )
                    {
                        return 5;
                    }
                    else
                    {
                        return 0xff;
                    }
                }
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::prog_exit */
/* EasyCODE F */
unsigned char KlasseProgUp::prog_exit (void)
{
    /* EasyCODE < */
    unsigned char loopflag, i;
    struct p_abschstruct *hlpptr;
    
    nae.homereq = TRUE;
    eing_botsch1.senden (BV030);
    eing_botsch1.senden (tz->progtext1);
    loopflag = TRUE;
    /* EasyCODE > */
    while ( loopflag )
    {
        switch ( getkey () )
        {
        case TESC :
            // alt
            //eing.prog.konverr = 0;
            //loopflag = FALSE;
            /* EasyCODE ( 0 
            SW014  Versuch */
            eing.auftrag = 3;   // Wenn ein Fehler kommt, z.B.: Programm nicht vorhanden,
                           // dann automatisch eine PNR Wahl antriggern.
            return TRUE;
            /* EasyCODE ) */
            /* EasyCODE_STRUCTURED_COMMENT_START */
            /*
            break;
            */
            /* EasyCODE_STRUCTURED_COMMENT_END */
        case TENTER :
            /* EasyCODE ( 0 
            Enter */
            eing_botsch1.senden(Z3 DELLINE);
            if ( (hlpptr = find_absch(AB_STDST,(struct p_abschstruct *)(ndatanf+GPROGNR))) == NULL )
            {
                eing.prog.konverr =
                   konv.konvert(ndatanf,0,MAXPROGLEN,&eing.prog.absch_fehler);
            }
            else
            {
                eing.prog.konverr =
                   konv.konvert(ndatanf,hlpptr->par1,MAXPROGLEN,&eing.prog.absch_fehler);
            }
            loopflag = FALSE;
            break;
            /* EasyCODE ) */
        case STOPANF :
            // Stopanforderung
            error_res ();
            break;
        default:
            eing_botsch1.senden(BELLBELL);
            break;
        }
    }
    if ( eing.prog.konverr != 0 )
    {
        return FALSE;
    }
    else
    {
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
        return TRUE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::find_absch */
/* EasyCODE C */
/* EasyCODE < */
// Das Up sucht den naechsten Abschnitt mit der vorgegebenen Abschnittsart.
// Dabei wird der Datensatz vorwaerts zum Programmende hin ab dem folgenden
// Abschnitt durchsucht, d.h. der Abschnitt auf den der uebergebene Zeiger
// zeigt, wird noch nicht durchsucht.
//
// Eingangs-Parameter:   Abschnittsart
//                       Zeiger auf Abschnitt
//                       (Suche beginnt ab naechstem Abschnitt)
// Rueckmeldung:         Zeiger auf gefundenen Abschnitt, falls gefunden
//                       NULL, falls nicht gefunden
struct p_abschstruct * KlasseProgUp::find_absch (unsigned char abschart, struct p_abschstruct * ptr)
/* EasyCODE > */
{
        if ( ptr->art == AB_PRENDE )
        {
        }
        else
        {
            do{
                ptr++;
                if ( ptr->art == abschart )
                {
                    return ptr;
                }
            }while ( (ptr->art != AB_PRENDE) && (FP_OFF(ptr) < MAXPROGLEN) );
        }
        return NULL;
}
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::rfind_absch */
/* EasyCODE C */
/* EasyCODE < */
// Das Up sucht den naechsten Abschnitt mit der vorgegebenen Abschnittsart.
// Dabei wird der Datensatz rueckwaerts zum Programmanfang hin ab dem voran-
// gehenden Abschnitt durchsucht, d.h. der Abschnitt auf den der uebergebene
// Zeiger zeigt, wird noch nicht durchsucht.
//
// Eingangs-Parameter:   Abschnittsart
//                       Zeiger auf Abschnitt
//                       (Suche beginnt ab vorangehenden Abschnitt)
// Rueckmeldung:         Zeiger auf gefundenen Abschnitt, falls gefunden
//                       NULL, falls nicht gefunden

struct p_abschstruct * KlasseProgUp::rfind_absch (unsigned char abschart, struct p_abschstruct * ptr)
/* EasyCODE > */
{
        if ( ptr->art == AB_PNR )
        {
        }
        else
        {
            do{
                ptr--;
                if ( ptr->art == abschart )
                {
                    return ptr;
                }
            }while ( ptr->art != AB_PNR );
        }
        return NULL;
}
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::pfoot */
/* EasyCODE F */
// Das Up schaltet den Fuss auf und ab. Ev. wird vorher die Nadel positioniert.
//
// Eingangs-Parameter: keine
// Ausgangs-Parameter: TRUE    -  fertig ohne Fehler
//                     FALSE   -  Fehler oder Stopanforderung

unsigned char KlasseProgUp::pfoot (void)
{
    unsigned char loopflag;
    while ( !hfuss.ready() || !y8.fertig() )
    {
        if ( koor.eingstop() )
        {
            return FALSE;
        }
    }
    if ( hfuss.getpos().pos == saf->md.posoben.pos )
    {
        // Huepferfuss oben
        if ( y8.getZustand() )
        {
        }
        else
        {
            y8.setAuftrag();
            while ( !y8.fertig() )
            {
                if ( koor.eingstop() )
                {
                    return FALSE;
                }
            }
        }
        hfuss.ab();       // Huepferfuss ab
        //y5.resAuftrag();  // Fadenspannung zu
        //y4.resAuftrag();  // Klemme auf
    }
    else
    {
        // Huepferfuss unten
        loopflag = TRUE;
        while ( loopflag )
        {
            switch ( nup.napos() )
            {
            case 0 :
                break;
            case 1 :
                koor.nae_err = 1;
                nae.errpar = nminf.fehler;
                return FALSE;
            default:
                hfuss.auf();           // Fuss auf
                y8.resAuftrag();       // Niederhalter auf
                //y4.setAuftrag();       // Klemme zu
                //y5.resAuftrag();       // Fadenspannung zu
                loopflag = FALSE;
                break;
            }
        }
    }
    while ( !hfuss.ready() || !y8.fertig() || !hfuss_auf.fertig() )
    {
        if ( koor.eingstop() )
        {
            return FALSE;
        }
    }
    return TRUE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::absch_koord */
/* EasyCODE F */
// Das Up prueft, ob der uebergebene Abschnitt ein Abschnitt mit Koordinaten
// ausser den Referenzpunkten ist.
//
// Eingangs-Parameter:   Zeiger auf Abschnitt
// Rueckmeldung:         TRUE ..... Abschnitt mit Koordinaten
//                       FALSE .. Abschnitt ohne Koordinaten

unsigned char KlasseProgUp::absch_koord (struct p_abschstruct * ptr)
{
    switch ( ptr->art )
    {
    case AB_AP :
        /* EasyCODE = */
    case AB_EIL :
        /* EasyCODE = */
    case AB_STICH :
        /* EasyCODE = */
    case AB_GERADE :
        /* EasyCODE = */
    case AB_BOGENSTP :
        /* EasyCODE = */
    case AB_BOGENENDP :
        /* EasyCODE = */
    case AB_KREISSTP :
        /* EasyCODE = */
    case AB_KREISENDP :
        /* EasyCODE = */
    case AB_KURVSTP :
        /* EasyCODE = */
    case AB_KURVENDP :
        return TRUE;
    default:
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::pout_exe */
/* EasyCODE F */
// Das Up schaltet alle Ausgaenge entsprechend dem Programmzustandsblock.
//
// Eingangs-Parameter:   keine
// Rueckmeldung:         keine

void KlasseProgUp::pout_exe (void)
{
    if ( eing.prog.pzbl.pout1 )
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
    if ( eing.prog.pzbl.pout2 )
    {
        out2.setAuftrag();
    }
    else
    {
        out2.resAuftrag();
    }
    if ( eing.prog.pzbl.pout3 )
    {
        out3.setAuftrag();
    }
    else
    {
        out3.resAuftrag();
    }
    if ( eing.prog.pzbl.pout4 )
    {
        out4.setAuftrag();
    }
    else
    {
        out4.resAuftrag();
    }
    if ( eing.prog.pzbl.zusp )
    {
        y6.setAuftrag();
    }
    else
    {
        y6.resAuftrag();
    }
    if ( hfuss.getpos().pos != saf->md.posoben.pos )
    {
        hfuss.ab();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::find */
/* EasyCODE F */
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:
//      0  -  fertig ohne Fehler
//      1  -  Fehler oder Stopanforderung
//      2  -  koorflag sitzt, Koordinaten wurden abgeschnitten

unsigned char KlasseProgUp::find (void)
{
    /* EasyCODE ( 0 
    Definition bildkoordinate, bildkoordinate_min */
    /* EasyCODE C */
    /* EasyCODE < */
    struct
    /* EasyCODE > */
    {
            int x;
            int y;
    }bildkoordinate, bildkoordinate_min;
    /* EasyCODE E */
    /* EasyCODE ) */
    /* EasyCODE < */
    float abstand, abstand_min;
    struct p_abschstruct *datenzeiger, *datenzeiger_min;
    unsigned char retwert, moveflag;
    unsigned char loopflag = TRUE;
    unsigned char findkey = 0;
    unsigned char taste;
    unsigned char tastenmerk;
    /* EasyCODE > */
    while ( loopflag )
    {
        switch ( findkey )
        {
        case 0 :
            /* EasyCODE ( 0 
            findkey = 0,   Bildschirmvorlage anzeigen, Oberteil heben */
            eing_botsch1.senden (BV031);
            eing_botsch1.senden (findpiktos);
            sprintf (eing.varstring, findkoords, nup.workbuf.koorist.xbild-eing.prog.koorref.x,
                     nup.workbuf.koorist.ybild-eing.prog.koorref.y);
            eing_botsch1.senden (eing.varstring);
            y50u1.setAuftrag();   // Oberteil heben
            findkey = 1;
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            findkey = 1,   nachdem Oberteil oben - weiter */
            if ( koor.eingstop() )
            {
                retwert = 1;
                loopflag = FALSE;
            }
            else
            {
                if ( y50u1.fertig() && (keyb.taste() == 0xff) )
                {
                    findkey = 2;
                }
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            findkey = 2,   Tasten abfragen ---> verzweigen */
            if ( koor.eingstop() )
            {
                retwert = 1;
                loopflag = FALSE;
            }
            else
            {
                switch ( keyb.taste() )
                {
                case TLINKS :
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild - 1;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                    moveflag = TRUE;
                    break;
                case TRECHTS :
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild + 1;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                    moveflag = TRUE;
                    break;
                case TAB :
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild - 1;
                    moveflag = TRUE;
                    break;
                case TAUF :
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild + 1;
                    moveflag = TRUE;
                    break;
                case TESC :
                    if ( movlastp () )
                    {
                        retwert = 0;
                    }
                    else
                    {
                        retwert = 1;
                    }
                    moveflag = FALSE;
                    loopflag = FALSE;
                    break;
                case TENTER :
                    moveflag = FALSE;
                    findkey = 7;
                    break;
                default:
                    moveflag = FALSE;
                    break;
                }
                if ( moveflag )
                {
                    nae.vfindex = 0;
                    teing1.rsz();
                    teing1.sz(saf->prodelzt);
                    if ( nup.poscheck(&nup.koorsoll[0]) )
                    {
                        nup.bitomo (&nup.koorsoll[0]);
                        nup.carmov(STANDARD);
                        findkey = 3;
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            findkey = 3,   Warten bis Einzelpuls fertig */
            if ( smx.ready() && smy.ready() )
            {
                nup.workbuf.koorist = nup.koorsoll [0];
                sprintf (eing.varstring, findkoords, nup.workbuf.koorist.xbild-eing.prog.koorref.x,
                         nup.workbuf.koorist.ybild-eing.prog.koorref.y);
                eing_botsch1.senden (eing.varstring);
                findkey = 4;
            }
            break;
            /* EasyCODE ) */
        case 4 :
            /* EasyCODE ( 0 
            findkey = 4,   nach Zeit langsames Dauerfahren starten */
            if ( koor.eingstop() )
            {
                retwert = 1;
                loopflag = FALSE;
            }
            else
            {
                if ( teing1.tz() )
                {
                    teing1.rsz();
                    teing1.sz(5);
                    nae.vfindex = 1;
                    nup.koormerker = nup.koorsoll [0];
                    findkey = 5;
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
                        findkey = 2;
                        break;
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 5 :
            /* EasyCODE ( 0 
            findkey = 5,   Dauerfahren solange Taste gedrueckt */
            if ( koor.eingstop() )
            {
                smx.dynbef = 2;  // Stop 
                smy.dynbef = 2;  // Stop 
                
                findkey = 6;
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
                        findkey = 6;
                        break;
                    }
                    /* EasyCODE ) */
                    if ( teing1.tz() && !eing_botsch1.busycheck() )
                    {
                        nup.motobi (&nup.koorsoll[0]);
                        nup.workbuf.koorist = nup.koorsoll[0]; 
                        sprintf (eing.varstring, findkoords, nup.workbuf.koorist.xbild-eing.prog.koorref.x,
                        nup.workbuf.koorist.ybild-eing.prog.koorref.y);
                        eing_botsch1.senden (eing.varstring);
                        teing1.rsz();
                        teing1.sz(5);
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 6 :
            /* EasyCODE ( 0 
            findkey = 6,   warten bis Sm fertig nachdem
                           keine Taste gedr_ckt war oder Stopanforderung */
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
                
                
                sprintf (eing.varstring, findkoords, nup.workbuf.koorist.xbild-eing.prog.koorref.x,
                         nup.workbuf.koorist.ybild-eing.prog.koorref.y);
                eing_botsch1.senden (eing.varstring);
                findkey = 2;
            }
            break;
            /* EasyCODE ) */
        case 7 :
            /* EasyCODE ( 0 
            findkey = 7,   Punkt des Bilds mit geringstem Abstand suchen */
            /* EasyCODE < */
            // Abstand im Quadrat AP<->koorist berechnen
            bildkoordinate.x = ((struct p_abschstruct *)(ndatanf + GAP))-> par1;
            bildkoordinate.y = ((struct p_abschstruct *)(ndatanf + GAP))-> par2;
            bildkoordinate_min = bildkoordinate;
            abstand_min = (float)(bildkoordinate.x-nup.workbuf.koorist.xbild)*(bildkoordinate.x-nup.workbuf.koorist.xbild) +
                          (float)(bildkoordinate.y-nup.workbuf.koorist.ybild)*(bildkoordinate.y-nup.workbuf.koorist.ybild);
            datenzeiger = datenzeiger_min = (struct p_abschstruct *)(ndatanf + GAP);
            /* EasyCODE > */
            while ( (++datenzeiger)->art != AB_PRENDE )
            {
                if ( absch_koord(datenzeiger) )
                {
                    bildkoordinate.x = datenzeiger->par1;
                    bildkoordinate.y = datenzeiger->par2;
                    abstand = (float)(bildkoordinate.x-nup.workbuf.koorist.xbild)*(bildkoordinate.x-nup.workbuf.koorist.xbild) +
                              (float)(bildkoordinate.y-nup.workbuf.koorist.ybild)*(bildkoordinate.y-nup.workbuf.koorist.ybild);
                    if ( abstand < abstand_min )
                    {
                        abstand_min = abstand;
                        datenzeiger_min = datenzeiger;
                        bildkoordinate_min = bildkoordinate;
                    }
                }
            }
            eing.prog.ppointer = datenzeiger_min;
            set_pzbl (eing.prog.ppointer);
            eing.prog.abschnr = (FP_OFF(eing.prog.ppointer)-GAP)/5 + 5;
            nup.koorsoll[0].xbild = bildkoordinate_min.x;
            nup.koorsoll[0].ybild = bildkoordinate_min.y;
            if ( nup.poscheck(&nup.koorsoll [0]) )
            {
                koorflag = FALSE;
            }
            else
            {
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
                koorflag = TRUE;
            }
            nup.bitomo (&nup.koorsoll [0]);
            if ( (nup.koorsoll[0].xbild != nup.workbuf.koorist.xbild) ||
                 (nup.koorsoll[0].ybild != nup.workbuf.koorist.ybild) )
            {
                nup.carmov (STANDARD);
            }
            findkey = 8;
            break;
            /* EasyCODE ) */
        case 8 :
            /* EasyCODE ( 0 
            findkey = 8,   auf Punkt des Bilds fahren */
            while ( !smx.ready() || !smy.ready() )
            {
                if ( !ver.pmove() )
                {
                    smx.stop ();   // verriegelt
                    smy.stop ();
                    nae.homereq = TRUE;
                    saf->carposok = FALSE;
                    retwert = 1;
                    loopflag = FALSE;
                }
            }
            nup.workbuf.koorist = nup.koorsoll [0];
            eing.prog.koorptr = nup.koorsoll [0];
            findkey = 9;
            break;
            /* EasyCODE ) */
        case 9 :
            /* EasyCODE ( 0 
            findkey = 9,   Oberteil absenken */
            if ( !nafupos() )
            {
                retwert = 1;
                loopflag = FALSE;
            }
            else
            {
                // Oberteil senken
                y50u2.setAuftrag();
                findkey = 10;
            }
            break;
            /* EasyCODE ) */
        case 10 :
            /* EasyCODE ( 0 
            findkey = 10,   warten bis Oberteil absenken fertig */
            if ( koor.eingstop() )
            {
                retwert = 1;
                loopflag = FALSE;
            }
            else
            {
                if ( y50u2.fertig() )
                {
                    if ( koorflag )
                    {
                        retwert = 2;
                        loopflag = FALSE;
                    }
                    else
                    {
                        retwert = 0;
                        loopflag = FALSE;
                    }
                }
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
/* EasyCODE_STRUCTURED_COMMENT_START */
/*
EasyCODE_COMMENT_START EasyCODE ( 0 
KlasseProgUp::inskoord EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:
//      0  -  fertig ohne Fehler, gueltige Koordinaten in workbuf.koorist
//      1  -  Fehler oder Stopanforderung
//      2  -  Abbruch, Schlitten nicht verfahren

unsigned char KlasseProgUp::inskoord (unsigned char *piktostr)
{
    unsigned char retwert,moveflag;
    unsigned char loopflag = TRUE;
    unsigned char ikkey = 0;
    while ( loopflag )
    {
        switch ( ikkey )
        {
        case 0 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            ikkey = 0,   Bildschirmvorlage anzeigen EasyCODE_COMMENT_END
            eing_botsch1.senden (BV031);
            if ( eing.prog.key != 9 )
            {
                pstatuszeile();
                abschparanz();
            }
            kooranz();
            sprintf (eing.varstring, ikpiktos, piktostr);
            eing_botsch1.senden (eing.varstring);
            sprintf (eing.varstring1, findkoords, nup.workbuf.koorist.xbild-eing.prog.koorref.x,
                     nup.workbuf.koorist.ybild-eing.prog.koorref.y);
            eing_botsch1.senden (eing.varstring1);
            ikkey = 1;
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 1 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            ikkey = 1,   Tasten abfragen ---> verzweigen EasyCODE_COMMENT_END
            if ( koor.eingstop() )
            {
                retwert = 1;
                loopflag = FALSE;
            }
            else
            {
                switch ( getkey() )
                {
                case TLINKS :
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild - 1;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                    moveflag = TRUE;
                    break;
                case TRECHTS :
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild + 1;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                    moveflag = TRUE;
                    break;
                case TAB :
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild - 1;
                    moveflag = TRUE;
                    break;
                case TAUF :
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild + 1;
                    moveflag = TRUE;
                    break;
                case TFUSS :
                    EasyCODE_COMMENT_START EasyCODE ( 0 
                    Taste Fuss EasyCODE_COMMENT_END
                    if ( !eing.pup.pfoot () )
                    {
                        error_res ();
                        ikkey = 0;
                    }
                    moveflag = FALSE;
                    break;
                    EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                case TESC :
                    if ( movlastp () )
                    {
                        retwert = 2;
                    }
                    else
                    {
                        retwert = 1;
                    }
                    moveflag = FALSE;
                    loopflag = FALSE;
                    break;
                case TENTER :
                    moveflag = FALSE;
                    retwert = 0;
                    loopflag = FALSE;
                    break;
                default:
                    moveflag = FALSE;
                    break;
                }
                if ( moveflag )
                {
                    EasyCODE_COMMENT_START EasyCODE ( 0 
                    Oberteil heben / senken EasyCODE_COMMENT_END
                    if ( eing.prog.pzbl.faden )
                    {
                        // Oberteil ab
                        y50u2.setAuftrag();
                    }
                    else
                    {
                        if ( (eing.prog.ppointer->art==AB_AP) && e50u1.getZustand() )
                        {
                        }
                        else
                        {
                            if ( eing.prog.pzbl.pout6 )
                            {
                                // Oberteil auf
                                y50u1.setAuftrag();
                            }
                            else
                            {
                                // Oberteil ab
                                y50u2.setAuftrag();
                            }
                        }
                    }
                    while ( !y50u1.fertig() || !y50u2.fertig() )
                    {
                        if ( koor.eingstop() )
                        {
                            retwert = 1;
                            loopflag = FALSE;
                            break;
                        }
                    }
                    EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                    if ( loopflag )
                    {
                        nae.vfindex = 0;
                        teing1.rsz();
                        teing1.sz(saf->prodelzt);
                        if ( nup.poscheck(&nup.koorsoll[0]) )
                        {
                            nup.bitomo (&nup.koorsoll[0]);
                            carmov();
                            ikkey = 2;
                        }
                    }
                }
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 2 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            ikkey = 2,   Warten bis Einzelpuls fertig EasyCODE_COMMENT_END
            if ( smx.ready() && smy.ready() )
            {
                nup.workbuf.koorist = nup.koorsoll [0];
                sprintf (eing.varstring, findkoords, nup.workbuf.koorist.xbild-eing.prog.koorref.x,
                         nup.workbuf.koorist.ybild-eing.prog.koorref.y);
                eing_botsch1.senden (eing.varstring);
                ikkey = 3;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 3 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            ikkey = 3,   nach Zeit langsames Dauerfahren starten EasyCODE_COMMENT_END
            if ( koor.eingstop() )
            {
                retwert = 1;
                loopflag = FALSE;
            }
            else
            {
                if ( teing1.tz() )
                {
                    teing1.rsz();
                    teing1.sz(saf->profastzt);
                    teing2.lz();
                    nae.vfindex = 1;
                    ikkey = 4;
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
                        ikkey = 1;
                        break;
                    }
                }
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 4 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            ikkey = 4,   Dauerfahren solange Taste gedrueckt EasyCODE_COMMENT_END
            if ( koor.eingstop() )
            {
                ikkey = 5;
            }
            else
            {
                if ( teing1.tz() )
                {
                    if ( nae.vfindex == 1 )
                    {
                        nae.vfindex =2;
                        teing1.rsz();
                        teing1.sz(500);
                    }
                    else
                    {
                        nae.vfindex = 3;
                    }
                }
                if ( smx.ready() && smy.ready() && !eing_botsch1.busycheck() )
                {
                    EasyCODE_COMMENT_START EasyCODE ( 0 
                    Tasten abfragen, Verfahrweg bestimmen EasyCODE_COMMENT_END
                    switch ( keyb.taste() )
                    {
                    case TLINKS :
                        switch ( nae.vfindex )
                        {
                        case 0 :
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild - 1;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                            break;
                        case 1 :
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild - 5;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                            break;
                        case 2 :
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild - 50;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                            break;
                        default:
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild - 100;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                            break;
                        }
                        break;
                    case TRECHTS :
                        switch ( nae.vfindex )
                        {
                        case 0 :
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild + 1;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                            break;
                        case 1 :
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild + 5;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                            break;
                        case 2 :
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild + 50;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                            break;
                        default:
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild + 100;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                            break;
                        }
                        break;
                    case TAB :
                        switch ( nae.vfindex )
                        {
                        case 0 :
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild - 1;
                            break;
                        case 1 :
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild - 5;
                            break;
                        case 2 :
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild - 30;
                            break;
                        default:
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild - 100;
                            break;
                        }
                        break;
                    case TAUF :
                        switch ( nae.vfindex )
                        {
                        case 0 :
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild + 1;
                            break;
                        case 1 :
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild + 5;
                            break;
                        case 2 :
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild + 30;
                            break;
                        default:
                            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild + 100;
                            break;
                        }
                        break;
                    default:
                        ikkey = 5;
                        break;
                    }
                    EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
                    if ( ikkey == 4 )
                    {
                        if ( nup.poscheck(&nup.koorsoll[0]) )
                        {
                            nup.bitomo (&nup.koorsoll[0]);
                            carmov();
                            nup.workbuf.koorist = nup.koorsoll[0];
                            sprintf (eing.varstring, findkoords, nup.workbuf.koorist.xbild-eing.prog.koorref.x,
                                     nup.workbuf.koorist.ybild-eing.prog.koorref.y);
                            eing_botsch1.senden (eing.varstring);
                        }
                        else
                        {
                            if ( nae.vfindex>=2 )
                            {
                                nae.vfindex=1;
                            }
                            else
                            {
                                nae.vfindex=0;
                            }
                            teing1.rsz();
                        }
                    }
                }
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        case 5 :
            EasyCODE_COMMENT_START EasyCODE ( 0 
            ikkey = 5,   warten bis Sm fertig nachdem
                         keine Taste gedr_ckt war oder Stopanforderung EasyCODE_COMMENT_END
            if ( smx.ready() && smy.ready() )
            {
                sprintf (eing.varstring, findkoords, nup.workbuf.koorist.xbild-eing.prog.koorref.x,
                         nup.workbuf.koorist.ybild-eing.prog.koorref.y);
                eing_botsch1.senden (eing.varstring);
                ikkey = 1;
            }
            break;
            EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
        default:
            break;
        }
    }
    return retwert;
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
*/
/* EasyCODE_STRUCTURED_COMMENT_END */
/* EasyCODE ( 0 
KlasseProgUp::inskoord */
/* EasyCODE F */
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:
//      0  -  fertig ohne Fehler, gueltige Koordinaten in workbuf.koorist
//      1  -  Fehler oder Stopanforderung
//      2  -  Abbruch, Schlitten nicht verfahren

unsigned char KlasseProgUp::inskoord (unsigned char *piktostr)
{
    unsigned char retwert,moveflag;
    unsigned char loopflag = TRUE;
    unsigned char ikkey = 0;
    unsigned char taste;
    unsigned char tastenmerk;
    while ( loopflag )
    {
        switch ( ikkey )
        {
        case 0 :
            /* EasyCODE ( 0 
            ikkey = 0,   Bildschirmvorlage anzeigen */
            eing_botsch1.senden (BV031);
            if ( eing.prog.key != 9 )
            {
                pstatuszeile();
                abschparanz();
            }
            kooranz();
            sprintf (eing.varstring, ikpiktos, piktostr);
            eing_botsch1.senden (eing.varstring);
            sprintf (eing.varstring1, findkoords, nup.workbuf.koorist.xbild-eing.prog.koorref.x,
                     nup.workbuf.koorist.ybild-eing.prog.koorref.y);
            eing_botsch1.senden (eing.varstring1);
            ikkey = 1;
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            ikkey = 1,   Tasten abfragen ---> verzweigen */
            if ( koor.eingstop() )
            {
                retwert = 1;
                loopflag = FALSE;
            }
            else
            {
                switch ( getkey() )
                {
                case TLINKS :
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild - 1;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                    moveflag = TRUE;
                    break;
                case TRECHTS :
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild + 1;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild;
                    moveflag = TRUE;
                    break;
                case TAB :
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild - 1;
                    moveflag = TRUE;
                    break;
                case TAUF :
                    nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild;
                    nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild + 1;
                    moveflag = TRUE;
                    break;
                case TFUSS :
                    /* EasyCODE ( 0 
                    Taste Fuss */
                    if ( !eing.pup.pfoot () )
                    {
                        error_res ();
                        ikkey = 0;
                    }
                    moveflag = FALSE;
                    break;
                    /* EasyCODE ) */
                case TESC :
                    if ( movlastp () )
                    {
                        retwert = 2;
                    }
                    else
                    {
                        retwert = 1;
                    }
                    moveflag = FALSE;
                    loopflag = FALSE;
                    break;
                case TENTER :
                    moveflag = FALSE;
                    retwert = 0;
                    loopflag = FALSE;
                    break;
                default:
                    moveflag = FALSE;
                    break;
                }
                if ( moveflag )
                {
                    /* EasyCODE ( 0 
                    Oberteil heben / senken */
                    if ( eing.prog.pzbl.faden )
                    {
                        // Oberteil ab
                        y50u2.setAuftrag();
                    }
                    else
                    {
                        if ( (eing.prog.ppointer->art==AB_AP) && e50u1.getZustand() )
                        {
                        }
                        else
                        {
                            if ( eing.prog.pzbl.pout6 )
                            {
                                // Oberteil auf
                                y50u1.setAuftrag();
                            }
                            else
                            {
                                // Oberteil ab
                                y50u2.setAuftrag();
                            }
                        }
                    }
                    while ( !y50u1.fertig() || !y50u2.fertig() )
                    {
                        if ( koor.eingstop() )
                        {
                            retwert = 1;
                            loopflag = FALSE;
                            break;
                        }
                    }
                    /* EasyCODE ) */
                    if ( loopflag )
                    {
                        nae.vfindex = 0;
                        teing1.rsz();
                        teing1.sz(saf->prodelzt);
                        if ( nup.poscheck(&nup.koorsoll[0]) )
                        {
                            nup.bitomo (&nup.koorsoll[0]);
                            nup.carmov(STANDARD);
                            ikkey = 2;
                        }
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            ikkey = 2,   Warten bis Einzelpuls fertig */
            if ( smx.ready() && smy.ready() )
            {
                nup.workbuf.koorist = nup.koorsoll [0];
                sprintf (eing.varstring, findkoords, nup.workbuf.koorist.xbild-eing.prog.koorref.x,
                         nup.workbuf.koorist.ybild-eing.prog.koorref.y);
                eing_botsch1.senden (eing.varstring);
                ikkey = 3;
            }
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            ikkey = 3,   nach Zeit langsames Dauerfahren starten */
            if ( koor.eingstop() )
            {
                retwert = 1;
                loopflag = FALSE;
            }
            else
            {
                if ( teing1.tz() )
                {
                    nae.vfindex = 1;
                    nup.koormerker = nup.koorsoll [0];
                    teing1.rsz();
                    teing1.sz(5);  // Anzeige Zyklus
                    ikkey = 4;
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
                        ikkey = 1;
                        break;
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 4 :
            /* EasyCODE ( 0 
            ikkey = 4,   Dauerfahren solange Taste gedrueckt */
            if ( koor.eingstop() )
            {
                smx.dynbef = 2;  // Stop 
                smy.dynbef = 2;  // Stop 
                
                ikkey = 5;
            }
            else
            {
                if ( ((smx.ready() && smy.ready()) || (nae.vfindex > 1)) )
                {
                    // Einzeltakt vom vorgaengercase ist fertig, oder wir sind im Dauertakten des dyn. Pulsabstands
                    /* EasyCODE ( 0 
                    Tasten abfragen, Verfahrweg bestimmen */
                    taste = keyb.taste();
                    switch ( taste )
                    {
                    case TLINKS :
                        switch ( nae.vfindex )
                        {
                        case 1 :
                            nup.carmovdyn(SMOTX, SMXMINUS, SLOW);
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
                            nup.carmovdyn(SMOTX, SMXPLUS, SLOW);
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
                            nup.carmovdyn(SMOTY, SMYMINUS, SLOW);
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
                            nup.carmovdyn(SMOTY, SMYPLUS, SLOW);
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
                        ikkey = 5;
                        break;
                    }
                    /* EasyCODE ) */
                    if ( teing1.tz() && !eing_botsch1.busycheck() )
                    {
                        nup.motobi (&nup.koorsoll[0]);
                        nup.workbuf.koorist = nup.koorsoll[0]; 
                        sprintf (eing.varstring, findkoords, nup.workbuf.koorist.xbild-eing.prog.koorref.x,
                        nup.workbuf.koorist.ybild-eing.prog.koorref.y);
                        eing_botsch1.senden (eing.varstring);
                        teing1.rsz();
                        teing1.sz(5);
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 5 :
            /* EasyCODE ( 0 
            ikkey = 5,   warten bis Sm fertig nachdem
                         keine Taste gedr_ckt war oder Stopanforderung */
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
                
                
                sprintf (eing.varstring, findkoords, nup.workbuf.koorist.xbild-eing.prog.koorref.x,
                         nup.workbuf.koorist.ybild-eing.prog.koorref.y);
                eing_botsch1.senden (eing.varstring);
                ikkey = 1;
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
KlasseProgUp::movlastp */
/* EasyCODE F */
// Das Up faert den Schlitten nach Druecken der Taste Enter in den letzten definierten Punkt.
//
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:  TRUE  -  fertig ohne Fehler
//                      FALSE -  Fehler oder Stopanf.

unsigned char KlasseProgUp::movlastp (void)
{
    unsigned char retwert = TRUE;
    unsigned char loopflag = TRUE;
    if ( (nup.workbuf.koorist.xbild==eing.prog.koorptr.xbild) &&
         (nup.workbuf.koorist.ybild==eing.prog.koorptr.ybild) )
    {
    }
    else
    {
        while ( loopflag )
        {
            switch ( nup.napos () )
            {
            case 0 :
                break;
            case 1 :
                koor.nae_err = 1;
                nae.errpar = nminf.fehler;
                retwert = FALSE;
                loopflag = FALSE;
                break;
            default:
                hfuss.auf();           // Fuss auf
                y4.setAuftrag();       // Klemme zu
                y5.resAuftrag();       // Fadenspannung zu
                if ( (eing.prog.ppointer->art==AB_AP) && e50u1.getZustand() )
                {
                }
                else
                {
                    if ( eing.prog.pzbl.pout6 )
                    {
                        // Oberteil heben
                        y50u1.setAuftrag();
                    }
                    else
                    {
                        // Oberteil senken
                        y50u2.setAuftrag();
                    }
                }
                while ( !hfuss_auf.fertig() || !y50u1.fertig() || !y50u2.fertig() )
                {
                    if ( koor.eingstop() )
                    {
                        retwert = FALSE;
                        break;
                    }
                }
                if ( retwert )
                {
                    nup.koorsoll [0] = eing.prog.koorptr;
                    /* EasyCODE ( 0 
                    ev. Koordinaten begrenzen */
                    if ( nup.poscheck (&nup.koorsoll [0]) )
                    {
                        koorflag = FALSE;
                    }
                    else
                    {
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
                        koorflag = TRUE;
                        nup.bitomo (&nup.koorsoll [0]);
                        eing.prog.koorptr = nup.koorsoll [0];
                    }
                    /* EasyCODE ) */
                    nup.carmov (STANDARD);
                    /* EasyCODE ( 0 
                    Warten bis Sm fertig */
                    while ( !smx.ready() || !smy.ready() )
                    {
                        if ( !ver.pmove () )
                        {
                            smx.stop();              // verriegelt
                            smy.stop();
                            nae.homereq = TRUE;
                            saf->carposok = FALSE;
                            retwert = FALSE;
                            break;
                        }
                    }
                    /* EasyCODE ) */
                    if ( retwert )
                    {
                        nup.workbuf.koorist = nup.koorsoll [0];
                        if ( koorflag )
                        {
                            // Koordinaten wurden abgeschnitten
                            koor.eing_err = 27;
                            error_res ();
                        }
                    }
                }
                loopflag = FALSE;
                break;
            }
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::absch_del */
/* EasyCODE F */
// Das Up loescht n Abschnitte in den Programmdaten ab der Position ptr.

void KlasseProgUp::absch_del (struct p_abschstruct *ptr, unsigned int n)
{
    memmove (ptr, ptr+n, FP_OFF ((unsigned char *)(find_absch (AB_PRENDE, ptr)+1)) - FP_OFF ((unsigned char *)(ptr+n)));
    *(unsigned long *)(ndatanf + KPROGLNG) -= n*5;
    *(unsigned long *)(ndatanf + KSTICHDAT) -= n*5;
    switch ( ptr_in_block (ptr) )
    {
    case 2 :
        /* EasyCODE = */
    case 3 :
        /* EasyCODE = */
    case 0xff :
        eing.prog.blockend -= n;
        break;
    case 4 :
        eing.prog.blockanf -= n;
        eing.prog.blockend -= n;
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::ins_curvend */
/* EasyCODE F */
// Falls die Kurveneingabe eroeffnet ist, wird der letzte Kurvenstuetzpunkt ab ppointer+1
// zum Kurvenendpunkt gemacht.

void KlasseProgUp::ins_curvend (void)
{
    /* EasyCODE < */
    struct p_abschstruct *hlpptr;
    /* EasyCODE > */
    if ( eing.prog.pzbl.kurve )
    {
        if ( (hlpptr = rfind_absch (AB_KURVSTP, eing.prog.ppointer + 1)) != NULL )
        {
            if ( hlpptr > rfind_absch(AB_KURVENDP,eing.prog.ppointer+1) )
            {
                hlpptr->art = AB_KURVENDP;
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::curvend */
/* EasyCODE F */
// Falls die Kurveneingabe eroeffnet ist und keine weiteren Kurvenstuetzpunkte folgen,
// wird Kurvenende eingefuegt.

void KlasseProgUp::curvend (void)
{
    /* EasyCODE < */
    struct p_abschstruct *hlpptr;
    /* EasyCODE > */
    if ( eing.prog.pzbl.kurve )
    {
        for ( hlpptr = eing.prog.ppointer + 1;
               (hlpptr->art != AB_KURVSTP) && (hlpptr->art != AB_KURVENDP); hlpptr++ )
        {
            if ( (hlpptr->art==AB_ANNAEH) || (hlpptr->art==AB_SCHNEID) || (hlpptr->art==AB_NAEHAUS) ||
                 (hlpptr->art==AB_PRENDE) || (hlpptr->art==AB_STDST) || (hlpptr->art==AB_STICHL) ||
                 (hlpptr->art==AB_STICHB) || (hlpptr->art==AB_EIL) || (hlpptr->art==AB_STICH) ||
                 (hlpptr->art==AB_GERADE) || (hlpptr->art==AB_BOGENSTP) || (hlpptr->art==AB_KREISSTP) )
            {
                ins_curvend ();
                break;
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::absch_einf */
/* EasyCODE F */
// Das Up fuegt n Abschnitte in die Programmdaten ab der Position ptr + 1 ein.
//
// Zulässige Werte fuer n:
//     1 - Einfuegen eines Abschnitts
//     2 - Einfuegen zweier Abschnitte (Kreisbogen)
//     3 - Einfuegen dreier Abschnitte (Kreis)
// Ausgangs-Parameter:
//      0  -  fertig, Abschnitt eingefuegt
//      1  -  Programm > MAXPROGLEN, Abschnitt nicht eingefuegt

unsigned char KlasseProgUp::absch_einf (struct p_abschstruct *ptr, unsigned char n)
{
    /* EasyCODE < */
    struct p_abschstruct *hlpptr;
    unsigned char i;
    /* EasyCODE > */
    if ( FP_OFF ((unsigned char *)(hlpptr = find_absch (AB_PRENDE, ptr))) > (MAXPROGLEN - (n+1)*5) )
    {
        return 1;
    }
    else
    {
        memmove (ptr+n+1, ptr+1, FP_OFF ((unsigned char *)hlpptr) - FP_OFF ((unsigned char *)ptr));
        for ( i = 0; i < n; i++ )
        {
            (ptr+1+i)->art = eing.prog.abschnitt [i].art;
            (ptr+1+i)->par1 = eing.prog.abschnitt [i].par1;
            (ptr+1+i)->par2 = eing.prog.abschnitt [i].par2;
        }
        *(unsigned long *)(ndatanf + KPROGLNG) += n*5;
        *(unsigned long *)(ndatanf + KSTICHDAT) += n*5;
        switch ( ptr_in_block (ptr) )
        {
        case 2 :
            /* EasyCODE = */
        case 0xff :
            eing.prog.blockend += n;
            break;
        case 4 :
            eing.prog.blockanf += n;
            eing.prog.blockend += n;
            break;
        default:
            break;
        }
        switch ( n )
        {
        case 1 :
            if ( (eing.prog.abschnitt[0].art != AB_EIL) && (eing.prog.abschnitt[0].art != AB_GERADE) &&
                 (eing.prog.abschnitt[0].art != AB_STICH) && (eing.prog.abschnitt[0].art != AB_KURVSTP) )
            {
            }
            else
            {
                konv.verschiebe_abschnitt (FP_OFF(ptr+n), *(ikoordtyp *)(&nup.workbuf.koorist));
            }
            break;
        case 2 :
            konv.verschiebe_abschnitt (FP_OFF(ptr+n), *(ikoordtyp *)(&nup.workbuf.koorist));
            break;
        default:
            break;
        }
        return 0;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::car_to_blockanf */
/* EasyCODE F */
// Das Up faehrt zum Blockanfang (Programmierzeiger, Schlitten, ...).
//
// Ausgangs-Parameter:
//   0  -  fertig ohne Fehler
//   1  -  Fehler (nae_err, out_err oder Stopanforderung)
//   2  -  zum Blockanfang fahren abgebrochen;  zurueck in Grundzustand
//   3  -  Progr. beenden, Ziel Eingabe
//   4  -  Progr. beenden, Ziel Produktion

unsigned char KlasseProgUp::car_to_blockanf (void)
{
    unsigned char retwert = 0xff;
    unsigned char loopflag;
    unsigned char rueckm;
    if ( eing.prog.ppointer==eing.prog.blockanf )
    {
        retwert = 0;
    }
    else
    {
        eing_botsch1.senden (moveblockapconfirm);
        loopflag = TRUE;
        while ( loopflag )
        {
            switch ( getkey() )
            {
            case TENTER :
                loopflag = FALSE;
                break;
            case TESC :
                retwert = 2;
                loopflag = FALSE;
                break;
            case TEINGABE :
                retwert = 3;
                loopflag = FALSE;
                break;
            case TENDE :
                retwert = 4;
                loopflag = FALSE;
                break;
            case STOPANF :
                error_res ();
                retwert = 2;
                loopflag = FALSE;
                break;
            default:
                break;
            }
        }
        if ( retwert == 0xff )
        {
            eing_botsch1.senden (stopmoveblockap);
            pstatuszeile ();
            abschparanz();
            kooranz();
            loopflag = TRUE;
            while ( loopflag )
            {
                if ( eing.prog.ppointer < eing.prog.blockanf )
                {
                    rueckm = absch_fwd();
                }
                else
                {
                    rueckm = absch_back();
                }
                switch ( rueckm )
                {
                case 0 :
                    retwert = 1;
                    loopflag = FALSE;
                    break;
                case 1 :
                    retwert = 2;
                    loopflag = FALSE;
                    break;
                case 2 :
                    /* EasyCODE = */
                case 3 :
                    if ( eing.prog.ppointer == eing.prog.blockanf )
                    {
                        retwert = 0;
                        loopflag = FALSE;
                    }
                    else
                    {
                        if ( keyb.taste() == TSTOP )
                        {
                            retwert = 2;
                            loopflag = FALSE;
                        }
                    }
                    break;
                case 4 :
                    koor.eing_err = 30;
                    eing.errpar = eing.prog.konverr;
                    eing.errpar2 = eing.prog.absch_fehler;
                    error_res ();
                    retwert = 2;
                    loopflag = FALSE;
                    break;
                }
            }
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Abschnittsweise vorwaerts Takten */
/* EasyCODE ( 0 
KlasseProgUp::pfwd */
/* EasyCODE F */
// Abschnittsweise vorwaerts durchs Nahtbild takten
//
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:
//      0  -  Takten beendet ohne Fehler
//      1  -  Fehler oder Stopanforderung
//      2  -  konverr sitzt
//      3  -  AP erreicht, Oberteil angehoben

unsigned char KlasseProgUp::pfwd (void)
{
    unsigned char retwert = 0xff;;
    
    teing1.rsz();
    teing1.sz (saf->tadelzt);
    koorflag = FALSE;
    dauertakten = FALSE;
    dauercount = 0;
    stopmerker = FALSE;
    while ( TRUE )
    {
        switch ( absch_fwd () )
        {
        case 0 :
            // Fehler
            retwert = 1;
            break;
        case 1 :
            // Programm-Ende oder Stop
            stopmerker = FALSE;
            retwert = 0;
            break;
        case 2 :
            /* EasyCODE < */
            // fertig, Abschnitt ausgefuehrt
            /* EasyCODE > */
            if ( !next_abschfwd () )
            {
                retwert = 0;
            }
            break;
        case 3 :
            // Koordinaten wurden abgeschnitten
            koor.eing_err = 27;
            error_res ();
            eing_botsch1.senden (BV028);
            pstatuszeile ();
            abschparanz ();
            kooranz();
            retwert = 0;
            break;
        case 4 :
            // Konvertierungsfehler, konverr sitzt
            retwert = 2;
            break;
        case 5 :
            // AP erreicht, Oberteil noch angehoben
            retwert = 3;
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
KlasseProgUp::next_abschfwd */
/* EasyCODE F */
// In diesem UP wird geprueft, ob die Bedingungen zum Durchtakten eines neuen
// Abschnitts vorliegen.
//
// Eingangs-Parameter:   keine
// Ausgangs-Parameter:   TRUE  -  naechsten Abschnitt ausfuehren
//                       FALSE -  Ende, keinen naechsten Abschnitt takten

unsigned char KlasseProgUp::next_abschfwd (void)
{
    do{
        if ( !dauertakten )
        {
            if ( keyb.taste() == TFWD )
            {
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            if ( (keyb.taste() == TSTOP) ||
                 tlinks_stop.getZustand() ||
                 trechts_stop.getZustand() )
            {
                return FALSE;
            }
        }
    }while ( !teing1.tz() );
    if ( !dauertakten )
    {
        if ( ++dauercount > 25 )
        {
            dauertakten = TRUE;
            eing_botsch1.senden (dauertaktstr);
        }
    }
    teing1.rsz();
    teing1.sz(saf->taktzt);
    return TRUE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::absch_fwd */
/* EasyCODE F */
// Abschnittsweise vorwaerts durch den geometrischen Datensatz takten.
//
// Eingangs-Parameter:   keine
// Ausgangs-Parameter:
//     0  -  Fehler oder Stopanforderung
//     1  -  fertig, Programm-Ende erreicht
//     2  -  fertig, Abschnitt ausgefuehrt
//     3  -  fertig, Koordinaten wurden abgeschnitten
//     4  -  Konvertierungsfehler, konverr sitzt
//     5  -  Abschnitt AP erreicht, Oberteil ist noch angehoben

unsigned char KlasseProgUp::absch_fwd (void)
{
    /* EasyCODE < */
    unsigned char j, loopflag;
    unsigned char retwert = 0xff;
    unsigned int abschnr_saf;
    struct p_abschstruct *ppointer_saf;
    /* EasyCODE > */
    if ( eing.prog.ppointer->art==AB_PRENDE )
    {
        retwert = 1;
    }
    else
    {
        ppointer_saf = eing.prog.ppointer;
        abschnr_saf = eing.prog.abschnr;
        /* EasyCODE ( 0 
        Kennung, MNP und Hindernisse ueberspringen */
        for ( eing.prog.ppointer++;
               (eing.prog.ppointer->art==AB_MKENN) || (eing.prog.ppointer->art==AB_MNP) || (eing.prog.ppointer->art==AB_HIND); eing.prog.ppointer++ )
        {
            if ( eing.prog.ppointer->art!=AB_MKENN )
            {
                eing.prog.abschnr++;
            }
        }
        /* EasyCODE ) */
        eing.prog.abschnr ++;
        pstatuszeile ();
        abschparanz ();
        if ( eing.prog.ppointer->art == AB_PRENDE )
        {
            retwert = 1;
        }
        else
        {
            if ( (eing.prog.ppointer->art==AB_EIL) ||
                 (eing.prog.ppointer->art==AB_STICH) ||
                 (eing.prog.ppointer->art==AB_AP) )
            {
                /* EasyCODE ( 0 
                Eilgang, Einzelstich oder Anfangspunkt */
                if ( !nafupos() )
                {
                    eing.prog.ppointer = ppointer_saf;
                    eing.prog.abschnr = abschnr_saf;
                    retwert = 0;
                }
                else
                {
                    if ( eing.prog.pzbl.faden )
                    {
                        // Oberteil senken
                        y50u2.setAuftrag();
                    }
                    else
                    {
                        if ( eing.prog.pzbl.pout6 )
                        {
                            // Oberteil heben
                            y50u1.setAuftrag();
                        }
                        else
                        {
                            if ( eing.prog.ppointer->art != AB_AP )
                            {
                                // Oberteil senken
                                y50u2.setAuftrag();
                            }
                        }
                    }
                    while ( !y50u1.fertig() || !y50u2.fertig() )
                    {
                        if ( koor.eingstop() )
                        {
                            eing.prog.ppointer = ppointer_saf;
                            eing.prog.abschnr = abschnr_saf;
                            retwert = 0;
                            break;
                        }
                    }
                    if ( retwert == 0xff )
                    {
                        nup.carlp = FALSE;
                        nup.koorsoll [0].xbild = eing.prog.ppointer->par1;
                        nup.koorsoll [0].ybild = eing.prog.ppointer->par2;
                        if ( nup.poscheck(&nup.koorsoll[0]) )
                        {
                            koorflag = FALSE;
                        }
                        else
                        {
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
                            koorflag = TRUE;
                        }
                        nup.bitomo (&nup.koorsoll [0]);
                        if ( (nup.koorsoll[0].xbild != nup.workbuf.koorist.xbild) ||
                             (nup.koorsoll[0].ybild != nup.workbuf.koorist.ybild) )
                        {
                            if ( ((eing.prog.ppointer->art==AB_AP) && !ver.move_sonst()) ||
                                 ((eing.prog.ppointer->art!=AB_AP) && !ver.pmove()) )
                            {
                                eing.prog.ppointer = ppointer_saf;
                                eing.prog.abschnr = abschnr_saf;
                                retwert = 0;
                            }
                            else
                            {
                                nup.carmov (STANDARD);
                                while ( !smx.ready() || !smy.ready() )
                                {
                                    if ( ((eing.prog.ppointer->art==AB_AP) && !ver.move_sonst()) ||
                                         ((eing.prog.ppointer->art!=AB_AP) && !ver.pmove()) )
                                    {
                                        smx.stop ();   // verriegelt
                                        smy.stop ();
                                        eing.prog.ppointer = ppointer_saf;
                                        eing.prog.abschnr = abschnr_saf;
                                        nae.homereq = TRUE;
                                        saf->carposok = FALSE;
                                        retwert = 0;
                                    }
                                    else
                                    {
                                        if ( dauertakten &&
                                             ((keyb.taste() == TSTOP) ||
                                               tlinks_stop.getZustand() ||
                                               trechts_stop.getZustand()) )
                                        {
                                            stopmerker = TRUE;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            /* EasyCODE < */
                            //Sollpos. =
                            //Istpos.
                            /* EasyCODE > */
                        }
                        if ( retwert == 0xff )
                        {
                            nup.workbuf.koorist = nup.koorsoll [0];
                            eing.prog.koorptr = nup.koorsoll [0];
                            kooranz ();
                            if ( (eing.prog.ppointer->art==AB_AP) && e50u1.getZustand() )
                            {
                                eing_botsch1.senden (BV030);
                                pstatuszeile();
                                kooranz();
                                abschparanz();
                                eing_botsch1.senden (apconfirm);
                                loopflag = TRUE;
                                while ( loopflag )
                                {
                                    switch ( getkey() )
                                    {
                                    case TENTER :
                                        eing_botsch1.senden (BV028);
                                        pstatuszeile ();
                                        abschparanz ();
                                        kooranz();
                                        loopflag = FALSE;
                                        break;
                                    case TESC :
                                        retwert = 5;
                                        loopflag = FALSE;
                                        break;
                                    default:
                                        eing_botsch1.senden(BELLBELL);
                                        break;
                                    }
                                }
                                if ( retwert == 0xff )
                                {
                                    /* EasyCODE < */
                                    // Oberteil senken
                                    /* EasyCODE > */
                                    y50u2.setAuftrag();
                                    while ( !y50u2.fertig() )
                                    {
                                        if ( koor.eingstop() )
                                        {
                                            retwert = 0;
                                            break;
                                        }
                                    }
                                    if ( retwert == 0xff )
                                    {
                                        if ( koorflag )
                                        {
                                            retwert = 3;
                                        }
                                        else
                                        {
                                            retwert = 2;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if ( koorflag )
                                {
                                    retwert = 3;
                                }
                                else
                                {
                                    if ( stopmerker )
                                    {
                                        retwert = 1;
                                    }
                                    else
                                    {
                                        retwert = 2;
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
                if ( absch_koord(eing.prog.ppointer) )
                {
                    /* EasyCODE ( 0 
                    Koordinaten */
                    if ( !nafupos() )
                    {
                        eing.prog.ppointer = ppointer_saf;
                        eing.prog.abschnr = abschnr_saf;
                        retwert = 0;
                    }
                    else
                    {
                        if ( eing.prog.pzbl.faden )
                        {
                            // Oberteil senken
                            y50u2.setAuftrag();
                        }
                        else
                        {
                            if ( eing.prog.pzbl.pout6 )
                            {
                                // Oberteil heben
                                y50u1.setAuftrag();
                            }
                            else
                            {
                                if ( eing.prog.ppointer->art != AB_AP )
                                {
                                    // Oberteil senken
                                    y50u2.setAuftrag();
                                }
                            }
                        }
                        while ( !y50u1.fertig() || !y50u2.fertig() )
                        {
                            if ( koor.eingstop() )
                            {
                                eing.prog.ppointer = ppointer_saf;
                                eing.prog.abschnr = abschnr_saf;
                                retwert = 0;
                                break;
                            }
                        }
                        if ( retwert == 0xff )
                        {
                            eing.prog.masch_adr = FP_OFF(find_absch(AB_PRENDE,eing.prog.ppointer)+1);
                            switch ( (j=konv.absch_gen(ndatanf,300,MAXPROGLEN,
                                     FP_OFF(eing.prog.ppointer),&eing.prog.masch_adr)) )
                            {
                            case 0 :
                                break;
                            case 14 :
                                koorflag = TRUE;
                                break;
                            default:
                                eing.prog.konverr = j;
                                eing.prog.ppointer = ppointer_saf;
                                eing.prog.abschnr = abschnr_saf;
                                retwert = 4;
                                break;
                            }
                            if ( retwert == 0xff )
                            {
                                nup.carlp = FALSE;
                                if ( !movstich_fwd() )
                                {
                                    eing.prog.ppointer = ppointer_saf;
                                    eing.prog.abschnr = abschnr_saf;
                                    retwert = 0;
                                }
                                else
                                {
                                    kooranz();
                                    set_pzbl_abschnitt (eing.prog.ppointer);
                                    if ( koorflag )
                                    {
                                        retwert = 3;
                                    }
                                    else
                                    {
                                        if ( stopmerker )
                                        {
                                            retwert = 1;
                                        }
                                        else
                                        {
                                            retwert = 2;
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
                    set_pzbl_abschnitt(eing.prog.ppointer);
                    pout_exe ();
                    retwert = 2;
                }
            }
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::movstich_fwd */
/* EasyCODE F */
// Einen Abschnitt stichweise durchfahren.
//
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:  TRUE  -  fertig ohne Fehler
//                      FALSE -  Fehler, nae_err sitzt

unsigned char KlasseProgUp::movstich_fwd (void)
{
    unsigned char * hlpptr;
    unsigned char retwert = 0xff;
    
    stopmerker = FALSE;
    hlpptr = (unsigned char *)(find_absch (AB_PRENDE, eing.prog.ppointer) +1);
    while ( FP_OFF (hlpptr) != eing.prog.masch_adr )
    {
        /* EasyCODE ( 0 
        Sollkoordinaten berechnen */
        if ( *hlpptr >= 0x80 )
        {
            nup.koorsoll[0].xbild = nup.workbuf.koorist.xbild - (*hlpptr & 0x7f);
        }
        else
        {
            nup.koorsoll[0].xbild = nup.workbuf.koorist.xbild + *hlpptr;
        }
        if ( *(hlpptr + 1) >= 0x80 )
        {
            nup.koorsoll[0].ybild = nup.workbuf.koorist.ybild - (*(hlpptr+1) & 0x7f);
        }
        else
        {
            nup.koorsoll[0].ybild = nup.workbuf.koorist.ybild + *(hlpptr+1);
        }
        /* EasyCODE ) */
        if ( !ver.pmove () )
        {
            retwert = FALSE;
            break;
        }
        else
        {
            nup.bitomo (&nup.koorsoll [0]);
            nup.carmov (TAKTEN);
            while ( !smx.ready() || !smy.ready() )
            {
                /* EasyCODE ( 0 
                Verriegelungen pruefen */
                if ( !ver.pmove () )
                {
                    // verriegelt
                    smx.stop();
                    smy.stop();
                    nae.homereq = TRUE;
                    saf->carposok = FALSE;
                    retwert = FALSE;
                    break;
                }
                /* EasyCODE ) */
            }
            if ( retwert == 0xff )
            {
                nup.workbuf.koorist = nup.koorsoll [0];
                hlpptr += 2;
                if ( keyb.taste() == TSTOP )
                {
                    stopmerker = TRUE;
                }
            }
            else
            {
                break;
            }
        }
    }
    if ( retwert == 0xff )
    {
        eing.prog.koorptr = nup.workbuf.koorist;
        retwert = TRUE;
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
Abschnittsweise rueckwaerts Takten */
/* EasyCODE ( 0 
KlasseProgUp::pback */
/* EasyCODE F */
// Abschnittsweise rueckwaerts durchs Nahtbild takten
//
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:
//     0  -  Takten beendet ohne Fehler
//     1  -  Fehler oder Stopanforderung
//     2  -  konverr sitzt

unsigned char KlasseProgUp::pback (void)
{
    unsigned char retwert = 0xff;;
    
    teing1.rsz();
    teing1.sz (saf->tadelzt);
    koorflag = FALSE;
    dauertakten = FALSE;
    dauercount = 0;
    stopmerker = FALSE;
    while ( TRUE )
    {
        switch ( absch_back () )
        {
        case 0 :
            // Fehler
            retwert = 1;
            break;
        case 1 :
            // Datensatz Programmnummer erreicht
            stopmerker = FALSE;
            retwert = 0;
            break;
        case 2 :
            /* EasyCODE < */
            // fertig, Abschnitt ausgefuehrt
            /* EasyCODE > */
            if ( !next_abschback () )
            {
                retwert = 0;
            }
            break;
        case 3 :
            // Koordinaten wurden abgeschnitten
            koor.eing_err = 27;
            error_res ();
            eing_botsch1.senden (BV028);
            pstatuszeile ();
            abschparanz ();
            kooranz();
            retwert = 0;
            break;
        case 4 :
            // Konvertierungsfehler, konverr sitzt
            retwert = 2;
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
KlasseProgUp::next_abschback */
/* EasyCODE F */
// In diesem UP wird geprueft, ob die Bedingungen zum Durchtakten eines neuen
// Abschnitts vorliegen.
//
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:  TRUE  -  naechsten Abschnitt ausfuehren
//                      FALSE -  Ende, keinen naechsten Abschnitt takten

unsigned char KlasseProgUp::next_abschback (void)
{
    do{
        if ( !dauertakten )
        {
            if ( keyb.taste() == TBACK )
            {
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            if ( (keyb.taste() == TSTOP) ||
                 tlinks_stop.getZustand() ||
                 trechts_stop.getZustand() )
            {
                return FALSE;
            }
        }
    }while ( !teing1.tz() );
    if ( !dauertakten )
    {
        if ( ++dauercount > 25 )
        {
            dauertakten = TRUE;
            eing_botsch1.senden (dauertaktstr);
        }
    }
    teing1.rsz();
    teing1.sz(saf->taktzt);
    return TRUE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::absch_back */
/* EasyCODE F */
// Abschnittsweise rueckwaerts durch den geometrischen Datensatz takten.
//
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:
//     0  -  Fehler oder Stopanforderung
//     1  -  fertig, Datensatz Programmnummer erreicht
//     2  -  fertig, Abschnitt ausgefuehrt
//     3  -  fertig, Koordinaten wurden abgeschnitten
//     4  -  Konvertierungsfehler, konverr sitzt

unsigned char KlasseProgUp::absch_back (void)
{
    unsigned char j;
    unsigned char retwert = 0xff;
    if ( eing.prog.ppointer->art==AB_PNR )
    {
        retwert = 1;
    }
    else
    {
        if ( (eing.prog.ppointer->art==AB_EIL) || (eing.prog.ppointer->art==AB_STICH) || (eing.prog.ppointer->art==AB_AP) )
        {
            /* EasyCODE ( 0 
            Eilgang, Einzelstich oder AP */
            koorflag = FALSE;
            if ( eing.prog.ppointer->art!=AB_AP )
            {
                if ( !nafupos () )
                {
                    retwert = 0;
                }
                else
                {
                    if ( eing.prog.pzbl.faden )
                    {
                        // Oberteil senken
                        y50u2.setAuftrag();
                    }
                    else
                    {
                        if ( eing.prog.pzbl.pout6 )
                        {
                            // Oberteil heben
                            y50u1.setAuftrag();
                        }
                        else
                        {
                            // Oberteil senken
                            y50u2.setAuftrag();
                        }
                    }
                    while ( !y50u1.fertig() || !y50u2.fertig() )
                    {
                        if ( koor.eingstop() )
                        {
                            retwert = 0;
                            break;
                        }
                    }
                    if ( retwert == 0xff )
                    {
                        konv.suchekoord(FP_OFF(eing.prog.ppointer),(ikoordtyp *)&nup.koorsoll[0]);
                        if ( !nup.poscheck(&nup.koorsoll[0]) )
                        {
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
                            koorflag = TRUE;
                        }
                        nup.bitomo (&nup.koorsoll [0]);
                        if ( !ver.pmove () )
                        {
                            retwert = 0;
                        }
                        else
                        {
                            nup.carlp = FALSE;
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
                                    retwert = 0;
                                    break;
                                }
                                else
                                {
                                    if ( dauertakten &&
                                         ((keyb.taste() == TSTOP) ||
                                           tlinks_stop.getZustand() ||
                                           trechts_stop.getZustand()) )
                                    {
                                        stopmerker = TRUE;
                                    }
                                }
                            }
                            if ( retwert == 0xff )
                            {
                                nup.workbuf.koorist = nup.koorsoll [0];
                                eing.prog.koorptr = nup.koorsoll [0];
                            }
                        }
                    }
                }
            }
            if ( retwert == 0xff )
            {
                for ( eing.prog.ppointer--; (eing.prog.ppointer->art==AB_MNP) || (eing.prog.ppointer->art==AB_HIND) ||
                      (eing.prog.ppointer->art==AB_MKENN); eing.prog.ppointer -- )
                {
                    if ( eing.prog.ppointer->art!=AB_MKENN )
                    {
                        eing.prog.abschnr --;
                    }
                }
                eing.prog.abschnr --;
                pstatuszeile ();
                abschparanz ();
                kooranz();
                if ( koorflag )
                {
                    retwert = 3;
                }
                else
                {
                    if ( stopmerker )
                    {
                        retwert = 1;
                    }
                    else
                    {
                        retwert = 2;
                    }
                }
            }
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE ( 0 
            sonst */
            if ( absch_koord (eing.prog.ppointer) )
            {
                /* EasyCODE ( 0 
                Koordinaten */
                if ( !nafupos () )
                {
                    retwert = 0;
                }
                else
                {
                    if ( eing.prog.pzbl.faden )
                    {
                        // Oberteil senken
                        y50u2.setAuftrag();
                    }
                    else
                    {
                        if ( eing.prog.pzbl.pout6 )
                        {
                            // Oberteil heben
                            y50u1.setAuftrag();
                        }
                        else
                        {
                            // Oberteil senken
                            y50u2.setAuftrag();
                        }
                    }
                    while ( !y50u1.fertig() || !y50u2.fertig() )
                    {
                        if ( koor.eingstop() )
                        {
                            retwert = 0;
                            break;
                        }
                    }
                    if ( retwert == 0xff )
                    {
                        eing.prog.masch_adr = FP_OFF(find_absch(AB_PRENDE,eing.prog.ppointer)+1);
                        switch ( (j=konv.absch_gen(ndatanf,300,MAXPROGLEN,
                                 FP_OFF(eing.prog.ppointer),&eing.prog.masch_adr)) )
                        {
                        case 0 :
                            break;
                        case 14 :
                            koorflag = TRUE;
                            break;
                        default:
                            eing.prog.konverr = j;
                            eing.prog.absch_fehler = eing.prog.abschnr;
                            retwert = 4;
                            break;
                        }
                        if ( retwert == 0xff )
                        {
                            nup.carlp = FALSE;
                            if ( !movstich_back() )
                            {
                                retwert = 0;
                            }
                            else
                            {
                                eing.prog.ppointer --;
                                eing.prog.abschnr --;
                                pstatuszeile ();
                                abschparanz();
                                kooranz();
                                set_pzbl (eing.prog.ppointer);
                                if ( koorflag )
                                {
                                    retwert = 3;
                                }
                                else
                                {
                                    if ( stopmerker )
                                    {
                                        retwert = 1;
                                    }
                                    else
                                    {
                                        retwert = 2;
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
                for ( eing.prog.ppointer --;
                      (eing.prog.ppointer->art==AB_MNP) ||
                      (eing.prog.ppointer->art==AB_HIND) ||
                      (eing.prog.ppointer->art==AB_MKENN);
                      eing.prog.ppointer -- )
                {
                    if ( eing.prog.ppointer->art!=AB_MKENN )
                    {
                        eing.prog.abschnr --;
                    }
                }
                eing.prog.abschnr --;
                set_pzbl (eing.prog.ppointer);
                pout_exe ();
                pstatuszeile ();
                abschparanz ();
                kooranz();
                retwert = 2;
            }
            /* EasyCODE ) */
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseProgUp::movstich_back */
/* EasyCODE F */
// Einen Abschnitt stichweise rueckwaerts durchfahren.
//
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:  TRUE  -  fertig ohne Fehler
//                      FALSE -  Fehler, nae_err sitzt

unsigned char KlasseProgUp::movstich_back (void)
{
    unsigned char * hlpptr;
    unsigned char retwert = 0xff;
    
    stopmerker = FALSE;
    for ( hlpptr = ndatanf + eing.prog.masch_adr;
           hlpptr != (unsigned char *)(find_absch(AB_PRENDE, eing.prog.ppointer) + 1); hlpptr -= 2 )
    {
        /* EasyCODE ( 0 
        Sollkoordinaten berechnen */
        if ( *(hlpptr- 2) >= 0x80 )
        {
            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild + (*(hlpptr-2) & 0x7f);
        }
        else
        {
            nup.koorsoll [0].xbild = nup.workbuf.koorist.xbild - *(hlpptr-2);
        }
        if ( *(hlpptr - 1) >= 0x80 )
        {
            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild + (*(hlpptr - 1) & 0x7f);
        }
        else
        {
            nup.koorsoll [0].ybild = nup.workbuf.koorist.ybild - *(hlpptr - 1);
        }
        /* EasyCODE ) */
        if ( !ver.pmove () )
        {
            retwert = FALSE;
            break;
        }
        else
        {
            nup.bitomo (&nup.koorsoll [0]);
            nup.carmov (TAKTEN);
            /* EasyCODE ( 0 
            Verriegelungen pruefen */
            while ( !smx.ready() || !smy.ready() )
            {
                if ( !ver.pmove () )
                {
                    // verriegelt
                    smx.stop();
                    smy.stop();
                    nae.homereq = TRUE;
                    saf->carposok = FALSE;
                    retwert = FALSE;
                    break;
                }
            }
            /* EasyCODE ) */
            if ( retwert == 0xff )
            {
                nup.workbuf.koorist = nup.koorsoll [0];
                if ( keyb.taste() == TSTOP )
                {
                    stopmerker = TRUE;
                }
            }
            else
            {
                break;
            }
        }
    }
    if ( retwert == 0xff )
    {
        eing.prog.koorptr = nup.workbuf.koorist;
        retwert = TRUE;
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ) */
