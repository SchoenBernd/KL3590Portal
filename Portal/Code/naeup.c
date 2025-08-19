/* EasyCODE V8 */
/* EasyCODE ( 0 
naeup, Unterprogramme Naehen */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
// Datei:          naeup
// Klasse:         Kl3590 Portal
// Ersteller:      Halfmann   EES 2856

#include "portal.h"

KlasseNaeUp nup;
/* EasyCODE ) */
/* EasyCODE ( 0 
Daten lesen */
/* EasyCODE ( 0 
KlasseNaeUp::dfetnsew
Daten lesen bei stehendem Naehmotor */
/* EasyCODE F */
void KlasseNaeUp::dfetnsew (void)
{
    unsigned char i;
    for ( i= XYVALID; i < LAST; ++i )
    {
        mbef[i] = 0;
    }
    if ( *workbuf.datpoi != 0xff )
    {
        /* EasyCODE ( 0 
        Koordinaten */
        if ( *workbuf.datpoi >= 0x80 )
        {
            // Verfahrweg im 2er.-Komplement
            deltax = - (*workbuf.datpoi & 0x7f);
        }
        else
        {
            deltax = *workbuf.datpoi;
        }
        if ( *(workbuf.datpoi+1) >= 0x80 )
        {
            // Verfahrweg im 2er.-Komplement
            deltay = - (*(workbuf.datpoi+1) & 0x7f);
        }
        else
        {
            deltay = *(workbuf.datpoi+1);
        }
        mbef[XYVALID] = TRUE;
        workbuf.datpoi += 2;
        /* EasyCODE ) */
    }
    else
    {
        if ( *(workbuf.datpoi+1) <= 0xf )
        {
            /* EasyCODE ( 0 
            M-Befehl
            ohne Operand */
            switch ( *(workbuf.datpoi+1) )
            {
            case 1 :
                mbef[ANNAEHEN] = TRUE;
                break;
            case 2 :
                mbef[SCHNEIDEN] = TRUE;
                break;
            case 3 :
                mbef[PROGSTOP] = TRUE;
                break;
            case 4 :
                mbef[NAEHENAUS] = TRUE;
                break;
            case 7 :
                mbef[REDGSWON] = TRUE;
                workbuf.redgswen = TRUE;
                break;
            case 8 :
                mbef[REDGSWOFF] = TRUE;
                workbuf.redgswen = FALSE;
                break;
            case 9 :
                mbef[UFWSTOP] = TRUE;
                break;
            case 0xf :
                mbef[PROGENDE] = TRUE;
                break;
            default:
                do{
                    workbuf.datpoi++;
                }while ( *workbuf.datpoi != 0xff );
                workbuf.datpoi++;
                return ;
            }
            workbuf.datpoi += 3;
            /* EasyCODE ) */
        }
        else
        {
            if ( *(workbuf.datpoi+1) < 0x1f )
            {
                /* EasyCODE ( 0 
                M-Befehl
                mit Operand */
                switch ( *(workbuf.datpoi+1) )
                {
                case 0x10 :
                    // Naehgeschwindigkeit
                    workbuf.naegsw = *(workbuf.datpoi+2);
                    break;
                case 0x11 :
                    /* EasyCODE ( 0 
                    Ausgang setzen */
                    switch ( *(workbuf.datpoi+2) )
                    {
                    case 1 :
                        mbef[OUT1SET] = TRUE;
                        break;
                    case 2 :
                        mbef[OUT2SET] = TRUE;
                        break;
                    case 3 :
                        mbef[OUT3SET] = TRUE;
                        break;
                    case 4 :
                        mbef[OUT4SET] = TRUE;
                        break;
                    case 6 :
                        mbef[OUT6SET] = TRUE;
                        break;
                    }
                    break;
                    /* EasyCODE ) */
                case 0x12 :
                    /* EasyCODE ( 0 
                    Ausgang ruecksetzen */
                    switch ( *(workbuf.datpoi+2) )
                    {
                    case 1 :
                        mbef[OUT1RES] = TRUE;
                        break;
                    case 2 :
                        mbef[OUT2RES] = TRUE;
                        break;
                    case 3 :
                        mbef[OUT3RES] = TRUE;
                        break;
                    case 4 :
                        mbef[OUT4RES] = TRUE;
                        break;
                    case 6 :
                        mbef[OUT6RES] = TRUE;
                        break;
                    }
                    break;
                    /* EasyCODE ) */
                case 0x13 :
                    /* EasyCODE < */
                    // Zusatzfadenspannung ein
                    /* EasyCODE > */
                    if ( *(workbuf.datpoi+2) == 1 )
                    {
                        mbef[ZUSPGON] = TRUE;
                    }
                    break;
                case 0x14 :
                    /* EasyCODE < */
                    // Zusatzfadenspannung aus
                    /* EasyCODE > */
                    if ( *(workbuf.datpoi+2) == 1 )
                    {
                        mbef[ZUSPGOFF] = TRUE;
                    }
                    break;
                case 0x15 :
                    /* EasyCODE ( 0 
                    Warten auf Eingang (high) */
                    switch ( *(workbuf.datpoi+2) )
                    {
                    case 1 :
                        mbef[INHIGH1] = TRUE;
                        break;
                    case 2 :
                        mbef[INHIGH2] = TRUE;
                        break;
                    }
                    break;
                    /* EasyCODE ) */
                case 0x16 :
                    /* EasyCODE ( 0 
                    Warten auf Eingang (low) */
                    switch ( *(workbuf.datpoi+2) )
                    {
                    case 1 :
                        mbef[INLOW1] = TRUE;
                        break;
                    case 2 :
                        mbef[INLOW2] = TRUE;
                        break;
                    }
                    break;
                    /* EasyCODE ) */
                case 0x17 :
                    // Warten auf Zeit
                    verzzt = *(workbuf.datpoi+2);
                    mbef[DELAY] = TRUE;
                    break;
                case 0x18 :
                    /* EasyCODE ( 0 
                    Huepferfussniveau (Wert) */
                    workbuf.posunten = *(workbuf.datpoi+2);
                    hfuss.akt_posunten.pos = workbuf.posunten;
                    hfuss.postomo (&hfuss.akt_posunten);
                    mbef[HFNIVEAU] = TRUE;
                    break;
                    /* EasyCODE ) */
                case 0x19 :
                    /* EasyCODE ( 0 
                    Huepferfusshub (Wert) */
                    workbuf.hub = *(workbuf.datpoi+2);
                    hfuss.akt_hub = workbuf.hub;
                    mbef[HFHUB] = TRUE;
                    break;
                    /* EasyCODE ) */
                case 0x1a :
                    /* EasyCODE ( 0 
                    Flag setzen */
                    switch ( *(workbuf.datpoi+2) )
                    {
                    case 1 :
                        mbef[FLAG1SET] = TRUE;
                        workbuf.flag |= 0x01;
                        break;
                    default:
                        break;
                    }
                    break;
                    /* EasyCODE ) */
                case 0x1b :
                    /* EasyCODE ( 0 
                    Flag ruecksetzen */
                    switch ( *(workbuf.datpoi+2) )
                    {
                    case 1 :
                        mbef[FLAG1RES] = TRUE;
                        workbuf.flag &= ~0x01;
                        break;
                    default:
                        break;
                    }
                    break;
                    /* EasyCODE ) */
                case 0x1c :
                    /* EasyCODE ( 0 
                    Huepferfussniveau (Stufe) */
                    workbuf.posunten = saf->md.hfniveau[(*(workbuf.datpoi+2))-1];
                    hfuss.akt_posunten.pos = workbuf.posunten;
                    hfuss.postomo (&hfuss.akt_posunten);
                    mbef[HFNIVEAULEV] = TRUE;
                    break;
                    /* EasyCODE ) */
                case 0x1d :
                    /* EasyCODE ( 0 
                    Huepferfusshub (Stufe) */
                    workbuf.hub = saf->md.hfhub[(*(workbuf.datpoi+2))-1];
                    hfuss.akt_hub = workbuf.hub;
                    mbef[HFHUBLEV] = TRUE;
                    break;
                    /* EasyCODE ) */
                default:
                    /* EasyCODE < */
                    // ungueltiger Befehl wird als NOP behandelt
                    /* EasyCODE > */
                    do{
                        workbuf.datpoi++;
                    }while ( *workbuf.datpoi != 0xff );
                    workbuf.datpoi++;
                    return ;
                }
                workbuf.datpoi += 4;
                /* EasyCODE ) */
            }
            else
            {
                if ( *(workbuf.datpoi+1) == 0x20 )
                {
                    /* EasyCODE ( 0 
                    Schnelles
                    Transportieren */
                    workbuf.datpoi += 2;
                    deltax = 0;
                    deltay = 0;
                    do{
                        if ( *workbuf.datpoi >= 0x80 )
                        {
                            deltax -= *workbuf.datpoi & 0x7f;
                        }
                        else
                        {
                            deltax += *workbuf.datpoi;
                        }
                        if ( *(workbuf.datpoi+1) >= 0x80 )
                        {
                            deltay -= *(workbuf.datpoi+1) & 0x7f;
                        }
                        else
                        {
                            deltay += *(workbuf.datpoi+1);
                        }
                        workbuf.datpoi += 2;
                    }while ( *workbuf.datpoi !=0xff );
                    workbuf.datpoi += 1;
                    mbef[XYVALID] = TRUE;
                    /* EasyCODE ) */
                }
                else
                {
                    do{
                        // ungueltiger Befehl wird als NOP behandelt
                        workbuf.datpoi++;
                    }while ( *workbuf.datpoi != 0xff );
                    workbuf.datpoi++;
                }
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::dfetsew
Daten lesen waehrend des Naehens */
/* EasyCODE F */
void KlasseNaeUp::dfetsew (void)
{
    unsigned char i;
    hfkoord tempkoord;
    for ( i= XYVALID; i < LAST; ++i )
    {
        mbef[i] = 0;
    }
    while ( *workbuf.datpoi == 0xff )
    {
        switch ( *(workbuf.datpoi+1) )
        {
        case 0x07 :
            /* EasyCODE ( 0 
            red. Geschw. ein */
            workbuf.redgswen = TRUE;
            workbuf.datpoi += 3;
            /* EasyCODE ) */
            break;
        case 0x08 :
            /* EasyCODE ( 0 
            red. Geschw. aus */
            workbuf.redgswen = FALSE;
            workbuf.datpoi += 3;
            /* EasyCODE ) */
            break;
        case 0x10 :
            /* EasyCODE ( 0 
            Naehgeschw. */
            workbuf.naegsw = *(workbuf.datpoi+2);
            workbuf.datpoi += 4;
            /* EasyCODE ) */
            break;
        case 0x11 :
            /* EasyCODE ( 0 
            Ausgang setzen */
            switch ( *(workbuf.datpoi+2) )
            {
            case 1 :
                mbef[OUT1SET] = TRUE;
                break;
            case 2 :
                mbef[OUT2SET] = TRUE;
                break;
            case 3 :
                mbef[OUT3SET] = TRUE;
                break;
            case 4 :
                mbef[OUT4SET] = TRUE;
                break;
            case 6 :
                mbef[OUT6SET] = TRUE;
                break;
            }
            workbuf.datpoi += 4;
            /* EasyCODE ) */
            break;
        case 0x12 :
            /* EasyCODE ( 0 
            Ausgang ruecksetzen */
            switch ( *(workbuf.datpoi+2) )
            {
            case 1 :
                mbef[OUT1RES] = TRUE;
                break;
            case 2 :
                mbef[OUT2RES] = TRUE;
                break;
            case 3 :
                mbef[OUT3RES] = TRUE;
                break;
            case 4 :
                mbef[OUT4RES] = TRUE;
                break;
            case 6 :
                mbef[OUT6RES] = TRUE;
                break;
            }
            workbuf.datpoi += 4;
            /* EasyCODE ) */
            break;
        case 0x13 :
            /* EasyCODE ( 0 
            Zusatzfadenspannung ein */
            if ( *(workbuf.datpoi+2) == 1 )
            {
                mbef[ZUSPGON] = TRUE;
            }
            workbuf.datpoi += 4;
            /* EasyCODE ) */
            break;
        case 0x14 :
            /* EasyCODE ( 0 
            Zusatzfadenspannung aus */
            if ( *(workbuf.datpoi+2) == 1 )
            {
                mbef[ZUSPGOFF] = TRUE;
            }
            workbuf.datpoi += 4;
            /* EasyCODE ) */
            break;
        case 0x18 :
            /* EasyCODE ( 0 
            Huepferfussniveau (Wert) */
            workbuf.posunten = *(workbuf.datpoi+2);
            hfuss.akt_posunten.pos = workbuf.posunten;
            hfuss.postomo (&hfuss.akt_posunten);
            HF->posunten = hfuss.akt_posunten.motor;
            tempkoord.pos = hfuss.akt_posunten.pos + hfuss.akt_hub;
            hfuss.postomo (&tempkoord);
            HF->posoben = tempkoord.motor;
            mbef[HFNIVEAU] = TRUE;
            workbuf.datpoi += 4;
            /* EasyCODE ) */
            break;
        case 0x19 :
            /* EasyCODE ( 0 
            Huepferfusshub (Wert) */
            workbuf.hub = *(workbuf.datpoi+2);
            hfuss.akt_hub = workbuf.hub;
            tempkoord.pos = hfuss.akt_posunten.pos + hfuss.akt_hub;
            hfuss.postomo (&tempkoord);
            HF->posoben = tempkoord.motor;
            mbef[HFHUB] = TRUE;
            workbuf.datpoi += 4;
            /* EasyCODE ) */
            break;
        case 0x1a :
            /* EasyCODE ( 0 
            Flag setzen */
            switch ( *(workbuf.datpoi+2) )
            {
            case 1 :
                mbef[FLAG1SET] = TRUE;
                workbuf.flag |= 0x01;
                break;
            default:
                break;
            }
            workbuf.datpoi += 4;
            /* EasyCODE ) */
            break;
        case 0x1b :
            /* EasyCODE ( 0 
            Flag ruecksetzen */
            switch ( *(workbuf.datpoi+2) )
            {
            case 1 :
                mbef[FLAG1RES] = TRUE;
                workbuf.flag &= ~0x01;
                break;
            default:
                break;
            }
            workbuf.datpoi += 4;
            /* EasyCODE ) */
            break;
        case 0x1c :
            /* EasyCODE ( 0 
            Huepferfussniveau (Stufe) */
            workbuf.posunten = saf->md.hfniveau[(*(workbuf.datpoi+2))-1];
            hfuss.akt_posunten.pos = workbuf.posunten;
            hfuss.postomo (&hfuss.akt_posunten);
            HF->posunten = hfuss.akt_posunten.motor;
            tempkoord.pos = hfuss.akt_posunten.pos + hfuss.akt_hub;
            hfuss.postomo (&tempkoord);
            HF->posoben = tempkoord.motor;
            mbef[HFNIVEAULEV] = TRUE;
            workbuf.datpoi += 4;
            /* EasyCODE ) */
            break;
        case 0x1d :
            /* EasyCODE ( 0 
            Huepferfusshub (Stufe) */
            workbuf.hub = saf->md.hfhub[(*(workbuf.datpoi+2))-1];
            hfuss.akt_hub = workbuf.hub;
            tempkoord.pos = hfuss.akt_posunten.pos + hfuss.akt_hub;
            hfuss.postomo (&tempkoord);
            HF->posoben = tempkoord.motor;
            mbef[HFHUBLEV] = TRUE;
            workbuf.datpoi += 4;
            /* EasyCODE ) */
            break;
        default:
            do{
                workbuf.datpoi++;
            }while ( *workbuf.datpoi != 0xff );
            workbuf.datpoi++;
            break;
        }
    }
    workbuf.datpoi += 2;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::sewstop
Stoppbedingungen pruefen */
/* EasyCODE F */
void KlasseNaeUp::sewstop (void)
{
    unsigned char *hlppoi;
    
    hlppoi = workbuf.datpoi;
    while ( *hlppoi == 0xff )
    {
        switch ( *(hlppoi+1) )
        {
        case 0x02 :
            mbef[SCHNEIDEN] = TRUE;
            return ;
        case 0x04 :
            mbef[NAEHENAUS] = TRUE;
            return ;
        case 0x03 :
            /* EasyCODE < */
            // programmierter Stop
            /* EasyCODE > */
        case 0x0f :
            /* EasyCODE < */
            // Programmende
            /* EasyCODE > */
        case 0x15 :
            /* EasyCODE < */
            // Warten auf Eingang (high)
            /* EasyCODE > */
        case 0x16 :
            /* EasyCODE < */
            // Warten auf Eingang (low)
            /* EasyCODE > */
        case 0x17 :
            // Warten auf Zeit
            mbef[STOP] = TRUE;
            return ;
        case 0x09 :
            // Stop bei Unterfadenstoerung
            if ( (saf->md.ufwen == 4) && nae.ufastoe )
            {
                mbef[STOP] = TRUE;
                return ;
            }
            else
            {
                hlppoi += 3;
                break;
            }
        default:
            do{
                hlppoi++;
            }while ( *hlppoi !=0xff );
            hlppoi++;
            break;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::dfetvor5
M-Befehle vorlesen waehrend des Naehens */
/* EasyCODE F */
void KlasseNaeUp::dfetvor5 (void)
{
    while ( *datpoi5 == 0xff )
    {
        switch ( *(datpoi5+1) )
        {
        case 0x02 :
            /* EasyCODE < */
            // Fadenschneiden
            /* EasyCODE > */
        case 0x03 :
            /* EasyCODE < */
            // programmierter Stop
            /* EasyCODE > */
        case 0x04 :
            /* EasyCODE < */
            // Naehen aus
            /* EasyCODE > */
        case 0x0f :
            /* EasyCODE < */
            // Programm-Ende
            /* EasyCODE > */
        case 0x15 :
            /* EasyCODE < */
            // Warten auf Eingang (high)
            /* EasyCODE > */
        case 0x16 :
            /* EasyCODE < */
            // Warten auf Eingang (low)
            /* EasyCODE > */
        case 0x17 :
            // Warten auf Zeit
            mbef[BREMSEN] = TRUE;
            return ;
        case 0x09 :
            // Stop bei Unterfadenstoerung
            if ( (saf->md.ufwen == 4) && nae.ufastoe )
            {
                mbef[BREMSEN] = TRUE;
                return ;
            }
            else
            {
                datpoi5 += 3;
                break;
            }
        default:
            do{
                datpoi5++;
            }while ( *datpoi5 !=0xff );
            datpoi5++;
        }
    }
    datpoi5 += 2;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::dfetvor10
Koordinaten vorlesen waehrend des Naehens */
/* EasyCODE F */
void KlasseNaeUp::dfetvor10 (void)
{
    unsigned char i, j;
    unsigned int erg1;
    if ( *datpoi10 >= 0x80 )
    {
        // Verfahrweg im 2er.-Komplement
        deltax = -(*datpoi10 & 0x7f);
    }
    else
    {
        deltax = *datpoi10 ;
    }
    if ( *(datpoi10+1) >= 0x80 )
    {
        deltay = -(*(datpoi10+1) & 0x7f);
    }
    else
    {
        deltay = *(datpoi10+1) ;
    }
    datpoi10 += 2;
    erg1 = sqrt(deltax*deltax + deltay*deltay);
    if ( erg1 > STLG_MAX+3 )
    {
        koor.nae_err = 12;
    }
    else
    {
        /* EasyCODE ( 0 
        Koordinaten in koorsoll */
        if ( vorind == 0 )
        {
            koorsoll[0].xbild = koorsoll[10].xbild + deltax;
            koorsoll[0].ybild = koorsoll[10].ybild + deltay;
        }
        else
        {
            koorsoll[vorind].xbild = koorsoll[vorind - 1].xbild + deltax;
            koorsoll[vorind].ybild = koorsoll[vorind - 1].ybild + deltay;
        }
        /* EasyCODE ) */
        if ( !poscheck(&koorsoll[vorind]) )
        {
            koor.nae_err = 11;
        }
        else
        {
            bitomo (&koorsoll[vorind]);
            /* EasyCODE ( 0 
            zulaessige Geschwindigkeit des vorgelesenen Stiches ermitteln */
            if ( vorind == 0 )
            {
                gswvor[0] = min(saf->koorgswtab1[(abs(koorsoll[0].motorx-koorsoll[10].motorx))>>2],
                                saf->koorgswtab2[(abs(koorsoll[0].motory-koorsoll[10].motory))>>2]);
            }
            else
            {
                gswvor[vorind] = min(saf->koorgswtab1[(abs(koorsoll[vorind].motorx-koorsoll[vorind - 1].motorx))>>2],
                                     saf->koorgswtab2[(abs(koorsoll[vorind].motory-koorsoll[vorind - 1].motory))>>2]);
            }
            gswvor[vorind] = min (gswvor[vorind], saf->stigswtab[erg1>>2]);
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            Unterspannung ? */
            if ( !ac_ok.getZustand() || unterspannung )
            {
                unterspannung = TRUE;
                if ( gswvor[vorind] > saf->gswred_uspg + 2 )
                {
                    gswvor[vorind] -= saf->gswred_uspg;
                }
                else
                {
                    gswvor[vorind] = 2;
                }
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            kleinste Geschwindigkeit der naechsten 11 Stich nach gswkoor */
            for ( i =1, gswkoor = gswvor[0]; i<11; ++i )
            {
                if ( gswkoor > gswvor[i] )
                {
                    gswkoor = gswvor[i];
                }
            }
            /* EasyCODE ) */
            if ( vorind == 10 )
            {
                vorind = 0;
            }
            else
            {
                ++vorind;
            }
            /* EasyCODE ( 0 
            datpoi10 auf naechste Koordinaten und ueberpruefen,
            ob weiter vorgelesen werden soll */
            while ( *datpoi10 == 0xff )
            {
                switch ( *(datpoi10+1) )
                {
                case 0x02 :
                    /* EasyCODE < */
                    // Fadenschneiden
                    /* EasyCODE > */
                case 0x03 :
                    /* EasyCODE < */
                    // programmierter Stop
                    /* EasyCODE > */
                case 0x04 :
                    /* EasyCODE < */
                    // Nõhen aus
                    /* EasyCODE > */
                case 0x0f :
                    /* EasyCODE < */
                    // Programm-Ende
                    /* EasyCODE > */
                case 0x15 :
                    /* EasyCODE < */
                    // Warten auf Eingang (high)
                    /* EasyCODE > */
                case 0x16 :
                    /* EasyCODE < */
                    // Warten auf Eingang (low)
                    /* EasyCODE > */
                case 0x17 :
                    // Warten auf Zeit
                    dfv10en = FALSE;
                    return ;
                default:
                    do{
                        datpoi10++;
                    }while ( *datpoi10 !=0xff );
                    datpoi10++;
                    break;
                }
            }
            /* EasyCODE ) */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::get_biggeststitch 
groessten Stich eines Programms ermitteln */
/* EasyCODE F */
// Datensatz wird vom aktuellen Datenzeiger (zeigt auf
// 1. Koordinaten nach Annaehen) bis zum Fadenschneiden durchsucht. 
// Der groesste Vorschub wird zurueck gemeldet.

unsigned char KlasseNaeUp::get_biggeststitch (void)
{
    unsigned int erg1;
    unsigned char biggeststitch = 0;
    unsigned char *ptr;
    
    ptr = workbuf.datpoi;
    while ( (*ptr != 0xff) || (*(ptr+1) != 0x02) )
    {
        if ( *ptr == 0xff )
        {
            /* EasyCODE ( 0 
            M-Befehl */
            // M-Befehl ueberspringen
            do{
                ptr++;
            }while ( *ptr != 0xff );
            ptr++;
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE ( 0 
            Koordinaten */
            // Stich berechnen
            deltax = *ptr & 0x7f;
            deltay = *(ptr+1) & 0x7f;
            erg1 = sqrt(deltax*deltax + deltay*deltay);
            if ( (unsigned char)erg1 > biggeststitch )
            {
                biggeststitch = erg1;
            }
            ptr += 2;
            /* EasyCODE ) */
        }
    }
    return biggeststitch;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::get_hubgsw
die dem groessten Huepferhub eines Nahtabschnitts zugeordnete
Geschwindigkeit ermitteln (bis zum naechsten Fadenschneiden) */
/* EasyCODE F */
// Datensatz wird vom aktuellen Datenzeiger (zeigt auf
// 1. Koordinaten nach Annaehen) bis zum Fadenschneiden durchsucht. 
// Die dem groessten Hub zugeordnte Geschwindigkeit wird zurueck gemeldet.

unsigned char KlasseNaeUp::get_hubgsw (void)
{
    unsigned int biggesthub;
    unsigned int hub;
    unsigned int niveau;
    hfkoord hubkoord;
    int deltaniveau;
    unsigned char *ptr;
    
    deltaniveau = 0;
    biggesthub = hub = hfuss.akt_hub;
    niveau = hfuss.akt_posunten.pos;
    ptr = workbuf.datpoi;
    while ( (*ptr != 0xff) || (*(ptr+1) != 0x02) )
    {
        if ( *ptr == 0xff )
        {
            /* EasyCODE ( 0 
            M-Befehl */
            if ( *(ptr+1) == 0x18 )
            {
                /* EasyCODE ( 0 
                Niveau Huepferfuss
                (Wert) */
                deltaniveau = *(ptr+2) - niveau;
                if ( *(ptr+2) > niveau )
                {
                    if ( deltaniveau + hub > biggesthub )
                    {
                        biggesthub = deltaniveau + hub;
                    }
                }
                niveau = *(ptr+2);
                ptr += 4;
                /* EasyCODE ) */
            }
            else
            {
                if ( *(ptr+1) == 0x19 )
                {
                    /* EasyCODE ( 0 
                    Hub Huepferfuss
                    (Wert) */
                    if ( *(ptr+2) + deltaniveau > biggesthub )
                    {
                        biggesthub = *(ptr+2) + deltaniveau;
                    }
                    hub = *(ptr+2);
                    ptr += 4;
                    /* EasyCODE ) */
                }
                else
                {
                    if ( *(ptr+1) == 0x1C )
                    {
                        /* EasyCODE ( 0 
                        Niveau Huepferfuss
                        (Stufe) */
                        deltaniveau = saf->md.hfniveau[(*(ptr+2))-1] - niveau;
                        if ( saf->md.hfniveau[(*(ptr+2))-1] > niveau )
                        {
                            if ( deltaniveau + hub > biggesthub )
                            {
                                biggesthub = deltaniveau + hub;
                            }
                        }
                        niveau = saf->md.hfniveau[(*(ptr+2))-1];
                        ptr += 4;
                        /* EasyCODE ) */
                    }
                    else
                    {
                        if ( *(ptr+1) == 0x1D )
                        {
                            /* EasyCODE ( 0 
                            Hub Huepferfuss
                            (Stufe) */
                            if ( saf->md.hfhub[(*(ptr+2))-1] + deltaniveau > biggesthub )
                            {
                                biggesthub = saf->md.hfhub[(*(ptr+2))-1] + deltaniveau;
                            }
                            hub = saf->md.hfhub[(*(ptr+2))-1];
                            ptr += 4;
                            /* EasyCODE ) */
                        }
                        else
                        {
                            // M-Befehl ueberspringen
                            do{
                                ptr++;
                            }while ( *ptr != 0xff );
                            ptr++;
                        }
                    }
                }
            }
            /* EasyCODE ) */
        }
        else
        {
            // Koordinaten
            ptr += 2;
            deltaniveau = 0;
        }
    }
    hubkoord.pos = biggesthub;
    hfuss.postomo (&hubkoord);
    return hubgswtab[hubkoord.motor>>1];
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
Controller Naehbetriebsart */
/* EasyCODE ( 0 
KlasseNaeUp::nacont90 */
/* EasyCODE F */
void KlasseNaeUp::nacont90 (void)
{
    switch ( workbuf.nbtart )
    {
    case 0 :
        transport();
        break;
    case 1 :
        nae_stop();
        break;
    case 2 :
        naeakt();
        break;
    case 3 :
        bremsen();
        break;
    case 4 :
        ende();
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::transport */
/* EasyCODE F */
void KlasseNaeUp::transport(void)
{
    dfetnsew();
    if ( mbef[PROGENDE] )
    {
        workbuf.nbtart = 4;
    }
    else
    {
        if ( mbef[ANNAEHEN] )
        {
            workbuf.nbtart = 1;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::nae_stop */
/* EasyCODE F */
void KlasseNaeUp::nae_stop (void)
{
    unsigned char i, j;
    unsigned int erg1, erg2;
    
    dfetnsew();
    if ( mbef[SCHNEIDEN] || mbef[NAEHENAUS] )
    {
        workbuf.nbtart = 0;
    }
    else
    {
        if ( mbef[XYVALID] )
        {
            /* EasyCODE ( 0 
            Koordinaten */
            erg1 = sqrt(deltax*deltax + deltay*deltay);
            if ( erg1 > STLG_MAX + 3 )
            {
                koor.nae_err = 12;
            }
            else
            {
                koorsoll[0].xbild = workbuf.koorist.xbild + deltax;
                koorsoll[0].ybild = workbuf.koorist.ybild + deltay;
                if ( !poscheck(&koorsoll[0]) )
                {
                    koor.nae_err = 11;
                }
                else
                {
                    bitomo (&koorsoll[0]);
                    /* EasyCODE ( 0 
                    zulaessige Geschwindigkeit des vorgelesenen Stiches ermitteln */
                    for ( i =0; i<11; ++i )
                    {
                        gswvor[i] = GSWMAX;
                    }
                    gswvor[0] = min(saf->koorgswtab1[(abs(koorsoll[0].motorx-workbuf.koorist.motorx))>>2],
                                    saf->koorgswtab2[(abs(koorsoll[0].motory-workbuf.koorist.motory))>>2]);
                    gswvor[0] = min (gswvor[0], saf->stigswtab[erg1>>2]);
                    /* EasyCODE ( 0 
                    Unterspannung ? */
                    if ( ac_ok.getZustand() )
                    {
                        unterspannung = FALSE;
                    }
                    else
                    {
                        unterspannung = TRUE;
                        if ( gswvor[0] > saf->gswred_uspg +2 )
                        {
                            gswvor[0] -= saf->gswred_uspg;
                        }
                        else
                        {
                            gswvor[0] = 2;
                        }
                    }
                    /* EasyCODE ) */
                    /* EasyCODE ) */
                    sewstop();
                    if ( (mbef[SCHNEIDEN] || mbef[NAEHENAUS] || mbef[STOP]) )
                    {
                        vorind = 0;
                        gswkoor = gswvor[0];
                    }
                    else
                    {
                        /* EasyCODE ( 0 
                        Vorlesen */
                        datpoi5 = workbuf.datpoi;
                        vorind = 1;
                        while ( *datpoi5 == 0xff )
                        {
                            do{
                                ++datpoi5;
                            }while ( *datpoi5 != 0xff );
                            ++datpoi5;
                        }
                        datpoi10 = datpoi5;
                        dfv10en = TRUE;
                        do{
                            if ( dfv10en )
                            {
                                dfetvor10();
                            }
                            dfetvor5();
                            if ( koor.nae_err !=0 )
                            {
                                return ;
                            }
                            else
                            {
                                if ( mbef[BREMSEN] )
                                {
                                    vorind = 0;
                                    workbuf.nbtart = 3;
                                    break;
                                }
                            }
                        }while ( vorind != 9 );
                        if ( !mbef[BREMSEN] )
                        {
                            workbuf.nbtart = 2;
                            if ( dfv10en )
                            {
                                do{
                                    dfetvor10();
                                    if ( koor.nae_err !=0 )
                                    {
                                        return ;
                                    }
                                }while ( dfv10en && (vorind !=0) );
                            }
                            vorind = 0;
                        }
                        /* EasyCODE ) */
                    }
                    smx.rampadr = abs(koorsoll[0].motorx - workbuf.koorist.motorx);
                    if ( koorsoll[0].motorx < workbuf.koorist.motorx )
                    {
                        smx.status = SMXMINUS;
                    }
                    else
                    {
                        smx.status = SMXPLUS;
                    }
                    smy.rampadr = abs(koorsoll[0].motory - workbuf.koorist.motory);
                    if ( koorsoll[0].motory < workbuf.koorist.motory )
                    {
                        smy.status = SMYMINUS;
                    }
                    else
                    {
                        smy.status = SMYPLUS;
                    }
                    nmtrb.nmmodus = 0;
                }
            }
            /* EasyCODE ) */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::naeakt */
/* EasyCODE F */
void KlasseNaeUp::naeakt (void)
{
    dfetsew();
    if ( dfv10en )
    {
        dfetvor10();
    }
    else
    {
        if ( vorind != 10 )
        {
            vorind++;
        }
        else
        {
            vorind = 0;
        }
    }
    if ( koor.nae_err == 0 )
    {
        dfetvor5();
        if ( mbef [BREMSEN] )
        {
            nmtrb.bremsrdy = FALSE;
            nmtrb.bremscount = 0;
            nmtrb.bremsverzcount = 2;
            nmtrb.nmmodus = 3;
            nup.workbuf.nbtart = 3;
        }
        else
        {
            if ( koor.naestop() || (keyb.taste() == TSTOP) ||
                 tlinks_stop.getZustand() || trechts_stop.getZustand() ||
                 nae.ofastoe || (nae.ufastoe && (saf->md.ufwen == 3) && !nae.uf_contnae) )
            {
                nmtrb.bremsrdy = FALSE;
                nmtrb.bremsverzcount = 2;
                nmtrb.nmmodus = 4;
                workbuf.nbtart = 3;
                if ( (keyb.taste() == TSTOP) ||
                     tlinks_stop.getZustand() || trechts_stop.getZustand() )
                {
                    nae.naestop = TRUE;
                }
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::bremsen */
/* EasyCODE F */
void KlasseNaeUp::bremsen (void)
{
    if ( vorind != 10 )
    {
        vorind++;
    }
    else
    {
        vorind = 0;
    }
    dfetsew();
    sewstop();
    if ( (keyb.taste() == TSTOP) ||
         tlinks_stop.getZustand() || trechts_stop.getZustand() )
    {
        nae.naestop = TRUE;
    }
    if ( mbef[SCHNEIDEN] || mbef[NAEHENAUS] )
    {
        nmtrb.nmmodus = 7;
        workbuf.nbtart = 1;
        y7.resAuftrag();
        y32.resAuftrag();
    }
    else
    {
        if ( mbef[STOP] )
        {
            nmtrb.nmmodus = 5;
            workbuf.nbtart = 1;
        }
        else
        {
            if ( nmtrb.bremsrdy )
            {
                if ( nae.ofastoe )
                {
                    nmtrb.nmmodus = 7;
                    workbuf.nbtart = 1;
                    y7.resAuftrag();
                    y32.resAuftrag();
                }
                else
                {
                    nmtrb.nmmodus = 5;
                    workbuf.nbtart = 1;
                }
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::ende */
/* EasyCODE F */
void KlasseNaeUp::ende(void)
{
    ;
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
Schlitten positionieren */
/* EasyCODE ( 0 
KlasseNaeUp::movinit */
/* EasyCODE F */
unsigned char KlasseNaeUp::movinit (void)
{
    unsigned char smxready;
    unsigned char smyready;
    #ifdef buerotest
        workbuf.koorist = saf->mnp;
        saf->carposok = TRUE;
        return TRUE;
    #endif 
    switch ( initkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        Softstepping einschalten */
        smx.Fkt2_on();  // Softstepping SM-Endst. X
        smy.Fkt2_on();  // Softstepping SM-Endst. Y
        /* EasyCODE ) */
        xinit_key = 0;
        yinit_key = 0;
        carlp = FALSE;
        saf->carposok = FALSE;
        initkey = 1;
        return FALSE;
    case 1 :
        smxready = xinit();
        smyready = yinit();
        if ( smxready && smyready )
        {
            workbuf.koorist.xbild = 15000;
            workbuf.koorist.ybild = 15000;
            workbuf.koorist.motorx = 0;
            workbuf.koorist.motory = 0;
            koorsoll [0].motorx = saf->xkor;
            koorsoll [0].motory = saf->ykor;
            carmov (STANDARD);
            initkey =2;
        }
        return FALSE;
    case 2 :
        if ( smx.ready() && smy.ready() )
        {
            // fertig
            workbuf.koorist = saf->mnp;
            saf->carposok = TRUE;
            initkey = 3;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    case 3 :
        return TRUE;
    default:
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::xinit */
/* EasyCODE F */
unsigned char KlasseNaeUp::xinit (void)
{
    switch ( xinit_key )
    {
    case 0 :
        /* EasyCODE ( 0 
        xinit_key = 0,  auf x-Initiator fahren */
        smx.status = PAWRAMPCV;
        smx.rampadr = 1;
        if ( e78.getZustand() )
        {
            smx.vort = saf->stst_vort*4;
            smx.init ();
            smx.status |= SMXMINUS;
            smx.start_Freq ();
            xinit_key = 1;
        }
        else
        {
            smx.vort = saf->stst_vort;
            smx.init ();
            smx.status |= SMXPLUS;
            smx.start_Freq ();
            xinit_key = 2;
        }
        return FALSE;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        xinit_key = 1,  x-Initiator frei ? */
        if ( e78.getZustand() )
        {
        }
        else
        {
            smx.stop();
            smx.status = (smx.status & ~RICHTUNG) | SMXPLUS;
            smx.start_Freq ();
            xinit_key = 2;
        }
        return FALSE;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        xinit_key = 2,  x-Initiator betaetigt ? */
        if ( e78.getZustand() )
        {
            smx.stop ();
            tnae2.rsz();
            tnae2.sz (saf->smdelayzt);
            xinit_key = 3;
        }
        return FALSE;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        xinit_key = 3,  langsam von x-Initiator fahren */
        if ( tnae2.tz() )
        {
            if ( e78.getZustand() )
            {
                smx.vort = saf->slow_vort;
                smx.init ();
                smx.rampadr = 1;
                smx.status = (smx.status & ~RICHTUNG) | SMXMINUS;
                smx.start_Freq ();
                xinit_key = 4;
            }
            else
            {
                xinit_key = 0;
            }
        }
        return FALSE;
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        xinit_key = 4,  x-Initiator frei ? */
        if ( e78.getZustand() )
        {
            return FALSE;
        }
        else
        {
            smx.stop ();
            xinit_key = 5;
            return TRUE;
        }
        /* EasyCODE ) */
    case 5 :
        return TRUE;
    default:
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::yinit */
/* EasyCODE F */
unsigned char KlasseNaeUp::yinit (void)
{
    switch ( yinit_key )
    {
    case 0 :
        if ( e77.getZustand() )
        {
            smy.status = SMYMINUS;
            yinit_key = 1;
            smy.status |= EFR_CLK_32;
            smy.init ();
            smy.rampadr = smy_ststadr;
            smy.start_Freq ();
        }
        else
        {
            smy.status = SMYPLUS;
            yinit_key = 2;
            smy.status |= EFR_CLK_32;
            smy.init ();
            smy.rampadr = smy_ststadr;
            smy.start_Freq ();
        }
        return FALSE;
    case 1 :
        if ( e77.getZustand() )
        {
        }
        else
        {
            smy.stop();
            yinit_key = 0;
        }
        return FALSE;
    case 2 :
        if ( e77.getZustand() )
        {
            smy.stop ();
            tnae3.rsz();
            tnae3.sz (saf->smdelayzt);
            yinit_key = 3;
        }
        return FALSE;
    case 3 :
        if ( tnae3.tz() )
        {
            if ( e77.getZustand() )
            {
                smy.status |= EFR_CLK_1024;
                smy.init ();
                smy.rampadr = smy_ststadr;
                smy.status = (smy.status & ~RICHTUNG) | SMYMINUS;
                smy.start_Freq ();
                yinit_key = 4;
            }
            else
            {
                yinit_key = 0;
            }
        }
        return FALSE;
    case 4 :
        if ( e77.getZustand() )
        {
            return FALSE;
        }
        else
        {
            smy.stop ();
            yinit_key = 5;
            return TRUE;
        }
    case 5 :
        return TRUE;
    default:
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::bitomo */
// Diese Programm berechnet aus den Bildkoordinaten die Motorkoordinaten.
// Dazu wird ein Zeiger uebergeben, der auf folgende Struktur zeigt:

//                                ---------------------
//                 pointer ---->  |X  Bildkoor-       |
//                                |   dinaten         |
//                                |-------------------|
//                                |Y  Bildkoor-       |
//                                |   dinaten         |
//                                |-------------------|
//                                |MX Motor-          |
//                                |  koordinaten      |
//                                |-------------------|
//                                |MY Motor-          |
//                                |  koordinaten      |
//                                ---------------------
// Die Bildkoordinaten werden in 0,1mm, relativ zum Nadelnullpunkt angegeben.
//
// Folgende Formel ist zu berechnen:
//
// INKRX = FAKTOR * X        Motorkoordinaten fuer Motor X
// INKRY = FAKTOR * Y        Motorkoordinaten fuer Motor Y

/* Definition der Getriebekonstanten */
# define ZZ 20           /* Zaehnezahl des Ritzels */
# define TEIL 50         /* Teilung des Zahnriemens (in 0.1mm) */
# define STEPS 1000      /* Motorschritte pro Umdrehung */
# define FAKTOR 1        /* FAKTOR = STEPS / (ZZ * TEIL) */
                         /* !! da FAKTOR=1, bei Formel nur im Kommentar !! */
/* EasyCODE ( 0 
KlasseNaeUp::bitomo */
/* EasyCODE F */
void KlasseNaeUp::bitomo (koord *koord)
{
    float tmp, tmp2;
    
    tmp = faktorx * koord->xbild;
    if ( tmp >= 0 )
    {
        koord->motorx = tmp+0.5; // runden
    }
    else
    {
        koord->motorx = tmp-0.5; // runden
    }
    tmp2 = ((float)koord->xbild/6000)*saf->schabl_kor;
    tmp = faktory * koord->ybild - tmp2;
    if ( tmp >= 0 )
    {
        koord->motory = tmp+0.5; // runden
    }
    else
    {
        koord->motory = tmp-0.5; // runden
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::motobi */
// Dieses Programm berechnet aus den Motorkoordinaten die Bildkoordinaten.
// Dazu wird ein Zeiger uebergeben, der auf folgende Struktur zeigt:
//
//                                ---------------------
//                                |X  Bildkoor-       |
//                                |   dinaten         |
//                                |-------------------|
//                                |Y  Bildkoor-       |
//                                |   dinaten         |
//                                |-------------------|
//                pointer ---->   |MX Motor-          |
//                                |  koordinaten      |
//                                |-------------------|
//                                |MY Motor-          |
//                                |  koordinaten      |
//                                ---------------------
// Die Motorkoordinaten werden in Pulse, relativ zum Nadelnullpunkt angegeben.
//
// Folgende Formel ist zu berechnen:
//
// INKRX = FAKTOR * X        Motorkoordinaten fuer Motor X
// INKRY = FAKTOR * Y        Motorkoordinaten fuer Motor Y

/* Definition der Getriebekonstanten */
/* EasyCODE ( 0 
KlasseNaeUp::motobi */
/* EasyCODE F */
void KlasseNaeUp::motobi (koord *koord)
{
    float tmp;
    
    // X-Achse
    
    tmp = (float)koord->motorx / faktorx;
    if ( tmp >= 0 )
    {
        koord->xbild = tmp+0.5; // runden
    }
    else
    {
        koord->xbild = tmp-0.5; // runden
    }
    // Y-Achse
    
    tmp = ( (float)koord->motory + ((float)koord->xbild/6000 * saf->schabl_kor) ) / faktory;
    if ( tmp >= 0 )
    {
        koord->ybild = tmp+0.5; // runden
    }
    else
    {
        koord->ybild = tmp-0.5; // runden
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::bitomo_init */
/* EasyCODE F */
void KlasseNaeUp::bitomo_init (void)
{
    faktorx = 1 + (float)saf->inkr_korx/10000;
    faktory = 1 + (float)saf->inkr_kory/10000;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::carmov */
/* EasyCODE F */
// Das Up verfaehrt den Schlitten nach koorsoll [0].
// Mode: 0 - Standard Geschw.,   1 - langsam,   2 - schnell

void KlasseNaeUp::carmov (unsigned char mode)
{
    int hilfsvar1, hilfsvar2;
    unsigned int taktvort1;
    float dummy;
    /* EasyCODE ( 0 
    Softstepping einschalten */
    smx.Fkt2_on();  // Softstepping SM-Endst. X
    smy.Fkt2_on();  // Softstepping SM-Endst. Y
    /* EasyCODE ) */
    /* EasyCODE ( 0 
    Richtungen festlegen */
    hilfsvar1 = koorsoll [0].motorx - workbuf.koorist.motorx;
    if ( hilfsvar1 < 0 )
    {
        smx.status = PAWRAMPCV | SMXMINUS;
    }
    else
    {
        smx.status = PAWRAMPCV | SMXPLUS;
    }
    hilfsvar2 = koorsoll [0].motory - workbuf.koorist.motory;
    if ( hilfsvar2 < 0 )
    {
        smy.status = SMYMINUS;
    }
    else
    {
        smy.status = SMYPLUS;
    }
    /* EasyCODE ) */
    hilfsvar1 = abs (hilfsvar1);
    hilfsvar2 = abs (hilfsvar2);
    if ( hilfsvar1 > MAXINK )
    {
        /* EasyCODE ( 0 
        weite Strecke
        fahren */
        /* EasyCODE ( 0 
        Vorteiler berechnen */
        if ( koor.hbtart() == EINGABE )
        {
            smx.vort = saf->move_vort*2;
        }
        else
        {
            if ( mode == SLOW )
            {
                smx.vort = saf->move_vort*5;
            }
            else
            {
                if ( mode == FAST )
                {
                    smx.vort = saf->move3_vort;
                }
                else
                {
                    smx.vort = saf->move_vort;
                }
            }
        }
        /* EasyCODE ) */
        smx.init(); // SmX initialisieren
        smx.gespls = hilfsvar1;
        if ( mode == FAST )
        {
            smx.rampadr = saf->move3_adr;
            smx.dachpls = hilfsvar1 - saf->move3_abramp;
        }
        else
        {
            smx.rampadr = saf->move_adr;
            smx.dachpls = hilfsvar1 - saf->move_abramp;
        }
        smx.start_Rampe_Dach(); // Rampe mit Dachverlaengerung ohne Startverzoegerung
        /* EasyCODE ) */
    }
    else
    {
        /* EasyCODE ( 0 
        sonst */
        if ( hilfsvar1 != 0 )
        {
            if ( mode == TAKTEN )
            {
                smx.vort = saf->takt_vort2;
                smx.init();
                smx.rampadr = hilfsvar1;
                smx.start_Rampe();
            }
            else
            {
                if ( mode == SLOW )
                {
                    smx.vort = saf->takt_vort*5;
                }
                else
                {
                    smx.vort = saf->takt_vort;
                }
                smx.init();                    // SmX initialisieren
                smx.rampadr = 1;
                smx.gespls = hilfsvar1;
                smx.start_Pulse();             // Rampe fahren ohne Startverzoegerung
            }
        }
        /* EasyCODE ) */
    }
    if ( hilfsvar2 > MAXINK )
    {
        /* EasyCODE ( 0 
        weite Strecke
        fahren */
        /* EasyCODE ( 0 
        Vorteiler berechnen */
        if ( koor.hbtart() == EINGABE )
        {
            smy.status |= EFR_CLK_256;
        }
        else
        {
            if ( mode == SLOW )
            {
                smy.status |= EFR_CLK_512;
            }
            else
            {
                if ( mode == FAST )
                {
                    smy.status |= EFR_CLK_64;
                }
                else
                {
                    smy.status |= EFR_CLK_128;
                }
            }
        }
        /* EasyCODE ) */
        smy.init(); // SmY initialisieren
        smy.gespls = hilfsvar2;
        if ( mode == FAST )
        {
            smy.rampadr = saf->move3_adr;
            smy.dachpls = hilfsvar2 - saf->move3_abramp;
        }
        else
        {
            smy.rampadr = saf->move_adr;
            smy.dachpls = hilfsvar2 - saf->move_abramp;
        }
        smy.start_Rampe_Dach(); // Rampe mit Dachverlaengerung ohne Startverzoegerung
        /* EasyCODE ) */
    }
    else
    {
        /* EasyCODE ( 0 
        sonst */
        if ( hilfsvar2 != 0 )
        {
            if ( mode == TAKTEN )
            {
                smy.status |= EFR_CLK_256;
                smy.init();
                smy.rampadr = hilfsvar2;
                smy.start_Rampe();
            }
            else
            {
                if ( mode == SLOW )
                {
                    smy.status |= EFR_CLK_512;
                }
                else
                {
                    smy.status |= EFR_CLK_128;
                }
                smy.init();                    // SmY initialisieren
                smy.rampadr = smy_ststadr;
                smy.gespls = hilfsvar2;
                smy.start_Pulse();             // Rampe fahren ohne Startverzoegerung
            }
        }
        /* EasyCODE ) */
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::carmovdyn */
/* EasyCODE F */
// Das Up verfaehrt den Schlitten nach koorsoll [0]. Up wird nur von smove und find benutzt.

// Eingangs-Parameter:   achse:  SMOTX SMOTY SMOTZ,  richtung:  SMXPLUS SMXMINUS SMYPLUS SMYMINUS
// Rueckmeldung:         keine

void KlasseNaeUp::carmovdyn(unsigned char achse, unsigned char richtung, unsigned char speed)
{
    int hilfsvar1, hilfsvar2;
    /* EasyCODE - */
    smx.Fkt2_on();  // Softstepping SM-Endst. X
    smy.Fkt2_on();  // Softstepping SM-Endst. Y
    switch ( achse )
    {
    case SMOTX :
        smx.status = PAWRAMPCV | richtung;
        smx.vort = saf->ramp_vort;   // = 64;
        smx.init();
        /* EasyCODE ( 0 
        Parameter abhaengig von speed */
        switch ( speed )
        {
        case STANDARD :
            smx.nmotstart = 5;       // Motordrehzahl fuer konst. fahren   in 0,01 Umdr./s
            smx.delay_ms = 2000;     // Zeit konstant fahren
            smx.a_beschl = 6,3;      // (2 Pi n / t), n = Umdr./s = 3, t = Zeit zum Beschleunigen = 3s
            smx.a_brems  = 100 ;      
            smx.nmot     = 300;      // n in 0,01 Umdr./s   entspricht 3 kHz Pulsfrequenz bei 1000 Pulsen/Umdr.
            break;
        case SLOW :
            smx.nmotstart = 5;       // Motordrehzahl fuer konst. fahren   in 0,01 Umdr./s
            smx.delay_ms = 2000;     // Zeit konstant fahren
            smx.a_beschl = 3,1;      // (2 Pi n / t), n = Umdr./s = 2, t = Zeit zum Beschleunigen = 4s
            smx.a_brems  = 100 ;      
            smx.nmot     = 200;      // n in 0,01 Umdr./s   entspricht 2 kHz Pulsfrequenz bei 1000 Pulsen/Umdr.
            break;
        default:
            // FAST
            smx.nmotstart = 5;       // Motordrehzahl fuer konst. fahren   in 0,01 Umdr./s
            smx.delay_ms = 1000;     // Zeit konstant fahren
            smx.a_beschl = 15,7;     // (2 Pi n / t), n = Umdr./s = 5, t = Zeit zum Beschleunigen = 2s
            smx.a_brems  = 157;      
            smx.nmot     = 500;      // n in 0,01 Umdr./s   entspricht 5 kHz Pulsfrequenz bei 1000 Pulsen/Umdr.
            break;
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Aus den Naehfeldgrenzen (Bildkoor) die max. erlaubte Motorpulse ermitteln */
        if ( richtung == SMXPLUS )
        {
            // Ist-Daten aller Achsen in ein Temporaeres Rechen koor speichern
            nup.koorsoll[10] = nup.workbuf.koorist;
            
            // Zum Rechnen den worst case extrem Grenzwert annehmen der vorkommen kann
            nup.koorsoll[10].xbild = saf->xnaehfeld_max;
            
            nup.bitomo (&nup.koorsoll[10]);    
            
            // Diesen worst case Extremwert wuerden wir aber mit der aktuellen 
            // y Motorposition anfahren. Zum Rechencheck ueberschreiben wir nun
            // die theoretisch noch gueltige y Motor Positionen im Extremfall mit der aktuellen 
            // y Position.
            nup.koorsoll[10].motory = nup.workbuf.koorist.motory;  
            
            
            // Poscheck prueft aber nur Bildkoordinaten, darum nun die Bildkoor erzeugen
            nup.motobi (&nup.koorsoll[10]);  // passende Bildkoor für y ermitteln
            if ( nup.poscheck(&nup.koorsoll[10]) )
            {
                smx.plse = nup.koorsoll[10].motorx - nup.workbuf.koorist.motorx;
            }
            else
            {
                // kein
                // Dauerfahren
                // erlauben
                smx.plse = 0;
            }
        }
        else
        {
            nup.koorsoll[10] = nup.workbuf.koorist;
            nup.koorsoll[10].xbild = saf->xnaehfeld_min;
            nup.bitomo (&nup.koorsoll[10]);
            nup.koorsoll[10].motory = nup.workbuf.koorist.motory;  
            nup.motobi (&nup.koorsoll[10]);
            if ( nup.poscheck(&nup.koorsoll[10]) )
            {
                smx.plse = nup.workbuf.koorist.motorx - nup.koorsoll[10].motorx;
            }
            else
            {
                smx.plse = 0;
            }
        }
        /* EasyCODE ) */
        smx.dynbef = 1;   // Start
        break;
    case SMOTY :
        smy.status = richtung;
        smy.status |= EFR_CLK_64;
        smy.init();
        /* EasyCODE ( 0 
        Parameter abhaengig von speed */
        switch ( speed )
        {
        case STANDARD :
            smy.nmotstart = 5;       // Motordrehzahl fuer konst. fahren   in 0,01 Umdr./s
            smy.delay_ms = 2000;     // Zeit konstant fahren
            smy.a_beschl = 6,3;      // (2 Pi n / t), n = Umdr./s = 3, t = Zeit zum Beschleunigen = 3s
            smy.a_brems  = 100;      
            smy.nmot     = 300;      // n in 0,01 Umdr./s   entspricht 3 kHz Pulsfrequenz bei 1000 Pulsen/Umdr.
            break;
        case SLOW :
            smy.nmotstart = 5;       // Motordrehzahl fuer konst. fahren   in 0,01 Umdr./s
            smy.delay_ms = 2000;     // Zeit konstant fahren
            smy.a_beschl = 3,1;      // (2 Pi n / t), n = Umdr./s = 2, t = Zeit zum Beschleunigen = 4s
            smy.a_brems  = 100;      
            smy.nmot     = 200;      // n in 0,01 Umdr./s   entspricht 2 kHz Pulsfrequenz bei 1000 Pulsen/Umdr.
            break;
        default:
            smy.nmotstart = 5;       // Motordrehzahl fuer konst. fahren   in 0,01 Umdr./s
            smy.delay_ms = 1000;     // Zeit konstant fahren
            smy.a_beschl = 15,7  ;   // (2 Pi n / t), n = Umdr./s = 5, t = Zeit zum Beschleunigen = 2s
            smy.a_brems  = 157 ;      
            smy.nmot     = 500;      // n in 0,01 Umdr./s   entspricht 5 kHz Pulsfrequenz bei 1000 Pulsen/Umdr.
            break;
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Aus den Naehfeldgrenzen (Bildkoor) die max. erlaubte Motorpulse ermitteln */
        if ( richtung == SMYPLUS )
        {
            //smy.plse = saf->ynaehfeld_max - nup.workbuf.koorist.ybild;
            
            nup.koorsoll[10] = nup.workbuf.koorist;
            nup.koorsoll[10].ybild = saf->ynaehfeld_max;
            nup.bitomo (&nup.koorsoll[10]);
            smy.plse = nup.koorsoll[10].motory - nup.workbuf.koorist.motory;
        }
        else
        {
            //smy.plse = nup.workbuf.koorist.ybild - saf->ynaehfeld_min;
            
            nup.koorsoll[10] = nup.workbuf.koorist;
            nup.koorsoll[10].ybild = saf->ynaehfeld_min;
            nup.bitomo (&nup.koorsoll[10]);
            smy.plse = nup.workbuf.koorist.motory - nup.koorsoll[10].motory;
        }
        /* EasyCODE ) */
        smy.dynbef = 1;   // Start
        break;
    case SMOTZ :
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::clampstart */
/* EasyCODE F */
// Klammer schalten und Startbedingungen pruefen.

// Rueckmeldung:  0    .... kein Start, kein Fehler
//                1    .... Fehler, nae_err sitzt
//                2    .... Faden nicht geschnitten
//                3    .... Naehstart
//                4    .... Vorwaerts Takten
//                5    .... Rueckwaerts Takten
//                6    .... Einzelschritt (Step)
//                7    .... Klammer schliessen, obwohl keine Klammer eingelegt 

unsigned char KlasseNaeUp::clampstart (void)
{
    unsigned char taste;
    if ( !hfuss.ready() || !y51u1.fertig() || !y51u2.fertig()  || !saf->carposok || !hfuss_auf.fertig() )
    {
        return 0;
    }
    else
    {
        taste = keyb.taste();
        klammer_edge();
        if ( saf->variante == 0 )
        {
            /* EasyCODE ( 0 
            Standard */
            if ( carlp && saf->md.autoen )
            {
                // SW002DIS
                if ( saf->md.automatic )
                {
                    if ( in4.getZustand() )
                    {
                        y51u1.setAuftrag(); // Klammer auf
                        nup.klammerAufMerker = TRUE;
                    }
                    else
                    {
                        if ( !e51u2_1.getZustand() && 
                             !e51u2_2.getZustand() &&
                             nup.klammerAufMerker )
                        {
                            y63.setAuftrag(); 
                            nup.klammerAufMerker = FALSE;
                        }
                    }
                }
                // SW002DIS
                /* EasyCODE ( 0 
                Automatik-Start */
                if ( e51u2_1.getZustand() && e51u2_2.getZustand() )
                {
                    /* EasyCODE ( 0 
                    Klammer geschlossen */
                    if ( autostart_en )
                    {
                        if ( tnae1.tz() )
                        {
                            if ( saf->md.automatic  &&  !y61.getZustand() // SW001DIS
                               )
                            {
                                y61.setAuftrag();
                            }
                            if ( saf->jigctrf == 2 )
                            {
                                // autostart_en=FALSE;  // SWb32 //SW005-1
                                /* EasyCODE_STRUCTURED_COMMENT_START */
                                /*
                                return 0;
                                */
                                /* EasyCODE_STRUCTURED_COMMENT_END */
                                // SWb32
                                return 3;
                            }
                            else
                            {
                                // SW001DIS
                                if ( (saf->md.automatic  &&  in3.getZustand())
                                                         || 
                                                (!saf->md.automatic) )
                                {
                                    // Start
                                    return 3;
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
                    }
                    else
                    {
                        if ( TKLAMMER_merker )
                        {
                            switch ( nup.napos() )
                            {
                            case 0 :
                                return 0;
                            case 1 :
                                koor.nae_err = 1;
                                nae.errpar = nminf.fehler;
                                return 1;
                            default:
                                TKLAMMER_merker = FALSE;
                                hfuss.auf();         // Fuss auf
                                y4.setAuftrag();     // Klemme zu
                                y51u1.setAuftrag();  // Schablonenverriegelung auf
                                nae.res_barcode();
                                schabl.bc.res_barcode();
                                schabl.rf.delete_rfidprognummer();
                                barcode_lesen = TRUE;
                                if ( saf->md.jigctren == 2 )
                                {
                                    abl.set_anzauftrag(2);
                                }
                                return 0;
                            }
                        }
                        else
                        {
                            if ( TKLAMMER_edge )
                            {
                                TKLAMMER_edge = FALSE;
                                TKLAMMER_merker = TRUE;
                                return 0;
                            }
                            else
                            {
                                /* EasyCODE ( 0 
                                Tasten abfragen */
                                switch ( taste )
                                {
                                case TSTART :
                                    if ( barcode_lesen )
                                    {
                                        nae.set_barcode();
                                    }
                                    return 3;
                                case TFWD :
                                    if ( barcode_lesen )
                                    {
                                        nae.set_barcode();
                                    }
                                    return 4;
                                case TBACK :
                                    if ( barcode_lesen )
                                    {
                                        nae.set_barcode();
                                    }
                                    return 5;
                                default:
                                    if ( (schabl.bc.get_barcode() != barcode_alt) &&
                                         barcode_lesen )
                                    {
                                        nae.set_barcode();
                                        barcode_alt = schabl.bc.get_barcode();
                                        if ( saf->md.jigctren == 2 )
                                        {
                                            abl.set_anzauftrag(2);
                                        }
                                    }
                                    return 0;
                                }
                                /* EasyCODE ) */
                            }
                        }
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Klammer offen */
                    if ( //SW001DIS
                         (
                         (saf->md.automatic  &&  in2.getZustand())
                                             || 
                                    (!saf->md.automatic)
                         )
                         
                                           &&
                         
                         (e79.getZustand() && e80.getZustand()) )
                    {
                        /* EasyCODE ( 0 
                        Klammer eingelegt */
                        if ( autostart_en )
                        {
                            if ( autoclampverz )
                            {
                                if ( tnae1.tz() )
                                {
                                    y51u2.setAuftrag();  // Schalonenverriegelung zu
                                    /* EasyCODE - */
                                    /* EasyCODE_STRUCTURED_COMMENT_START */
                                    /*
                                    nae.res_barcode();
                                    schabl.bc.res_barcode();
                                    barcode_lesen = TRUE;
                                    schabl.rf.set_rfid_read();
                                    schabl.bc.set_bc_read(); //SW005-1
                                    */
                                    /* EasyCODE_STRUCTURED_COMMENT_END */
                                    if ( (saf->md.jigctren == 2) && (saf->jigctrf == BARCODE) )
                                    {
                                        /* SWb32   das If is neu
                                        hier nichts machen, also nicht den
                                        vor Klammer zu gelesenen Barcode loeschen
                                        */
                                    }
                                    else
                                    {
                                        nae.res_barcode();
                                        schabl.bc.res_barcode();
                                        barcode_lesen = TRUE;
                                        schabl.rf.set_rfid_read();
                                        schabl.bc.set_bc_read(); //SW005-1
                                    }
                                    tnae1.rsz();
                                    tnae1.sz(saf->startverz);
                                    if ( saf->md.jigctren == 2 )
                                    {
                                        abl.set_anzauftrag(2);
                                    }
                                }
                            }
                            else
                            {
                                autoclampverz = TRUE;
                                tnae1.rsz();
                                //tnae1.sz(saf->autoclampverz_zt);
                                tnae1.sz(30);  // SW013
                            }
                        }
                        else
                        {
                            if ( TKLAMMER_edge )
                            {
                                TKLAMMER_edge = FALSE;
                                y51u2.setAuftrag();  // Schalonenverriegelung zu
                                nae.res_barcode();
                                schabl.bc.res_barcode();
                                barcode_lesen = TRUE;
                                schabl.rf.set_rfid_read();
                                schabl.bc.set_bc_read(); //SW005-1
                                if ( saf->md.jigctren == 2 )
                                {
                                    abl.set_anzauftrag(2);
                                }
                            }
                        }
                        return 0;
                        /* EasyCODE ) */
                    }
                    else
                    {
                        // SWb32 begin
                        if ( (schabl.bc.get_barcode() != barcode_alt) &&
                             barcode_lesen )
                        {
                            nae.set_barcode();
                            barcode_alt = schabl.bc.get_barcode();
                            if ( saf->md.jigctren == 2 )
                            {
                                abl.set_anzauftrag(2);
                            }
                        }
                        // SWb32 end
                        if ( TKLAMMER_edge )
                        {
                            TKLAMMER_edge = FALSE;
                            autostart_en = FALSE;
                            return 7;
                        }
                        else
                        {
                            autoclampverz = FALSE;
                            autostart_en = TRUE;
                            return 0;
                        }
                    }
                    /* EasyCODE ) */
                }
                /* EasyCODE ) */
            }
            else
            {
                if ( saf->md.ffmode )
                {
                    /* EasyCODE ( 0 
                    Flip-Flop-Modus */
                    #ifdef buerotest
                        /* EasyCODE ( 0 
                        Klammer geschlossen */
                        if ( !TKLAMMER_merker )
                        {
                            if ( fs1_merker )
                            {
                                if ( tnae1.tz() )
                                {
                                }
                                else
                                {
                                    if ( fs2_edge )
                                    {
                                        return 3;
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                if ( fs1_edge )
                                {
                                    fs1_edge = FALSE;
                                    fs1_merker = TRUE;
                                    tnae1.rsz();
                                    tnae1.sz(saf->fsverz);
                                    return 0;
                                }
                                else
                                {
                                    if ( fs2_edge )
                                    {
                                        if ( barcode_lesen )
                                        {
                                            nae.set_barcode();
                                        }
                                        return 3;
                                    }
                                    else
                                    {
                                        if ( TKLAMMER_edge )
                                        {
                                            TKLAMMER_merker = TRUE;
                                            TKLAMMER_edge = FALSE;
                                            return 0;
                                        }
                                        else
                                        {
                                            /* EasyCODE ( 0 
                                            Tasten */
                                            switch ( taste )
                                            {
                                            case TSTART :
                                                if ( barcode_lesen )
                                                {
                                                    nae.set_barcode();
                                                }
                                                return 3;
                                            case TFWD :
                                                if ( barcode_lesen )
                                                {
                                                    nae.set_barcode();
                                                }
                                                return 4;
                                            case TBACK :
                                                if ( barcode_lesen )
                                                {
                                                    nae.set_barcode();
                                                }
                                                return 5;
                                            default:
                                                if ( (schabl.bc.get_barcode() != barcode_alt) &&
                                                     barcode_lesen )
                                                {
                                                    nae.set_barcode();
                                                    barcode_alt = schabl.bc.get_barcode();
                                                    if ( saf->md.jigctren == 2 )
                                                    {
                                                        abl.set_anzauftrag(2);
                                                    }
                                                }
                                                return 0;
                                            }
                                            /* EasyCODE ) */
                                        }
                                    }
                                }
                            }
                        }
                        if ( !saf->cutready )
                        {
                            return 2;
                        }
                        else
                        {
                            /* EasyCODE ( 0 
                            Nadel positionieren,
                            Fuss auf, Klemme zu, Klammer auf */
                            switch ( nup.napos() )
                            {
                            case 0 :
                                return 0;
                            case 1 :
                                koor.nae_err = 1;
                                nae.errpar = nminf.fehler;
                                return 1;
                            default:
                                TKLAMMER_merker = FALSE;
                                fs1_merker = FALSE;
                                hfuss.auf();         // Fuss auf
                                y4.setAuftrag();     // Klemme zu
                                y51u1.setAuftrag();  // Schablonenverriegelung auf
                                nae.res_barcode();
                                schabl.bc.res_barcode();
                                schabl.rf.delete_rfidprognummer();
                                barcode_lesen = TRUE;
                                if ( saf->md.jigctren == 2 )
                                {
                                    abl.set_anzauftrag(2);
                                }
                                return 0;
                            }
                            /* EasyCODE ) */
                        }
                        /* EasyCODE ) */
                    #else 
                        fsedge ();
                        if ( e51u2_1.getZustand() && e51u2_2.getZustand() )
                        {
                            /* EasyCODE ( 0 
                            Klammer geschlossen */
                            if ( !TKLAMMER_merker )
                            {
                                if ( fs1_merker )
                                {
                                    if ( tnae1.tz() )
                                    {
                                    }
                                    else
                                    {
                                        if ( fs2_edge )
                                        {
                                            return 3;
                                        }
                                        else
                                        {
                                            return 0;
                                        }
                                    }
                                }
                                else
                                {
                                    if ( fs1_edge )
                                    {
                                        fs1_edge = FALSE;
                                        fs1_merker = TRUE;
                                        tnae1.rsz();
                                        tnae1.sz(saf->fsverz);
                                        return 0;
                                    }
                                    else
                                    {
                                        if ( fs2_edge )
                                        {
                                            if ( barcode_lesen )
                                            {
                                                nae.set_barcode();
                                            }
                                            return 3;
                                        }
                                        else
                                        {
                                            if ( TKLAMMER_edge )
                                            {
                                                TKLAMMER_merker = TRUE;
                                                TKLAMMER_edge = FALSE;
                                                return 0;
                                            }
                                            else
                                            {
                                                /* EasyCODE ( 0 
                                                Tasten */
                                                switch ( taste )
                                                {
                                                case TSTART :
                                                    if ( barcode_lesen )
                                                    {
                                                        nae.set_barcode();
                                                    }
                                                    return 3;
                                                case TFWD :
                                                    if ( barcode_lesen )
                                                    {
                                                        nae.set_barcode();
                                                    }
                                                    return 4;
                                                case TBACK :
                                                    if ( barcode_lesen )
                                                    {
                                                        nae.set_barcode();
                                                    }
                                                    return 5;
                                                default:
                                                    if ( (schabl.bc.get_barcode() != barcode_alt) &&
                                                         barcode_lesen )
                                                    {
                                                        nae.set_barcode();
                                                        barcode_alt = schabl.bc.get_barcode();
                                                        if ( saf->md.jigctren == 2 )
                                                        {
                                                            abl.set_anzauftrag(2);
                                                        }
                                                    }
                                                    return 0;
                                                }
                                                /* EasyCODE ) */
                                            }
                                        }
                                    }
                                }
                            }
                            if ( !saf->cutready )
                            {
                                return 2;
                            }
                            else
                            {
                                /* EasyCODE ( 0 
                                Nadel positionieren,
                                Fuss auf, Klemme zu, Klammer auf */
                                switch ( nup.napos() )
                                {
                                case 0 :
                                    return 0;
                                case 1 :
                                    koor.nae_err = 1;
                                    nae.errpar = nminf.fehler;
                                    return 1;
                                default:
                                    TKLAMMER_merker = FALSE;
                                    fs1_merker = FALSE;
                                    hfuss.auf();         // Fuss auf
                                    y4.setAuftrag();     // Klemme zu
                                    y51u1.setAuftrag();  // Schablonenverriegelung auf
                                    nae.res_barcode();
                                    schabl.bc.res_barcode();
                                    schabl.rf.delete_rfidprognummer();
                                    barcode_lesen = TRUE;
                                    if ( saf->md.jigctren == 2 )
                                    {
                                        abl.set_anzauftrag(2);
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
                            Klammer offen */
                            if ( fs1_edge )
                            {
                                /* EasyCODE ( 0 
                                Fussschalter
                                1. Stufe */
                                if ( e79.getZustand() && e80.getZustand() )
                                {
                                }
                                else
                                {
                                    koor.nae_err = 14;
                                    return 1;
                                }
                                /* EasyCODE ) */
                            }
                            else
                            {
                                if ( TKLAMMER_edge )
                                {
                                    /* EasyCODE ( 0 
                                    Taste Klammer */
                                    if ( e79.getZustand() && e80.getZustand() )
                                    {
                                    }
                                    else
                                    {
                                        return 7;
                                    }
                                    /* EasyCODE ) */
                                }
                                else
                                {
                                    if ( (taste == TSTART)||
                                         (taste == TFWD) ||
                                         (taste == TBACK) )
                                    {
                                        if ( e79.getZustand() && e80.getZustand() )
                                        {
                                        }
                                        else
                                        {
                                            koor.nae_err = 14;
                                            return 1;
                                        }
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            fs1_edge = FALSE;
                            TKLAMMER_edge = FALSE;
                            y51u2.setAuftrag();  // Schablonenverriegelung zu
                            nae.res_barcode();
                            schabl.bc.res_barcode();
                            barcode_lesen = TRUE;
                            schabl.rf.set_rfid_read();
                            schabl.bc.set_bc_read(); //SW005-1
                            if ( saf->md.jigctren == 2 )
                            {
                                abl.set_anzauftrag(2);
                            }
                            return 0;
                            /* EasyCODE ) */
                        }
                    #endif 
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Pegel-Modus */
                    #ifdef buerotest
                        /* EasyCODE ( 0 
                        Klammer geschlossen */
                        if ( !TKLAMMER_merker )
                        {
                            if ( foot1.getZustand() )
                            {
                                clopendi = FALSE;
                            }
                            else
                            {
                                if ( TKLAMMER_edge )
                                {
                                    TKLAMMER_edge = FALSE;
                                    TKLAMMER_merker = TRUE;
                                    return 0;
                                }
                                else
                                {
                                    if ( !clopendi )
                                    {
                                        if ( !saf->cutready )
                                        {
                                            return 2;
                                        }
                                        else
                                        {
                                            /* EasyCODE ( 0 
                                            Nadel positionieren,
                                            Fuss auf, Klemme zu, Klammer auf */
                                            switch ( nup.napos() )
                                            {
                                            case 0 :
                                                return 0;
                                            case 1 :
                                                koor.nae_err = 1;
                                                nae.errpar = nminf.fehler;
                                                return 1;
                                            default:
                                                TKLAMMER_merker = FALSE;
                                                fs1_merker = FALSE;
                                                hfuss.auf();         // Fuss auf
                                                y4.setAuftrag();     // Klemme zu
                                                y51u1.setAuftrag();  // Schablonenverriegelung auf
                                                nae.res_barcode();
                                                schabl.bc.res_barcode();
                                                schabl.rf.delete_rfidprognummer();
                                                barcode_lesen = TRUE;
                                                if ( saf->md.jigctren == 2 )
                                                {
                                                    abl.set_anzauftrag(2);
                                                }
                                                return 0;
                                            }
                                            /* EasyCODE ) */
                                        }
                                    }
                                }
                            }
                            if ( foot2.getZustand() )
                            {
                                return 3;
                            }
                            else
                            {
                                /* EasyCODE ( 0 
                                Tasten */
                                switch ( taste )
                                {
                                case TSTART :
                                    if ( barcode_lesen )
                                    {
                                        nae.set_barcode();
                                    }
                                    return 3;
                                case TFWD :
                                    if ( barcode_lesen )
                                    {
                                        nae.set_barcode();
                                    }
                                    return 4;
                                case TBACK :
                                    if ( barcode_lesen )
                                    {
                                        nae.set_barcode();
                                    }
                                    return 5;
                                default:
                                    if ( (schabl.bc.get_barcode() != barcode_alt) &&
                                         barcode_lesen )
                                    {
                                        nae.set_barcode();
                                        barcode_alt = schabl.bc.get_barcode();
                                        if ( saf->md.jigctren == 2 )
                                        {
                                            abl.set_anzauftrag(2);
                                        }
                                    }
                                    return 0;
                                }
                                /* EasyCODE ) */
                            }
                        }
                        else
                        {
                            /* EasyCODE ( 0 
                            Taste Klammer
                            war gedrueckt */
                            if ( !saf->cutready )
                            {
                                return 2;
                            }
                            else
                            {
                                switch ( nup.napos() )
                                {
                                case 0 :
                                    return 0;
                                case 1 :
                                    koor.nae_err = 1;
                                    nae.errpar = nminf.fehler;
                                    return 1;
                                default:
                                    TKLAMMER_merker = FALSE;
                                    fs1_merker = FALSE;
                                    hfuss.auf();        // Fuss auf
                                    y4.setAuftrag();    // Klemme zu
                                    y51u1.setAuftrag(); // Klammer auf
                                    nae.res_barcode();
                                    schabl.bc.res_barcode();
                                    schabl.rf.delete_rfidprognummer();
                                    barcode_lesen = TRUE;
                                    if ( saf->md.jigctren == 2 )
                                    {
                                        abl.set_anzauftrag(2);
                                    }
                                    return 0;
                                }
                            }
                            /* EasyCODE ) */
                        }
                        /* EasyCODE ) */
                    #else 
                        if ( e51u2_1.getZustand() && e51u2_2.getZustand() )
                        {
                            /* EasyCODE ( 0 
                            Klammer geschlossen */
                            if ( !TKLAMMER_merker )
                            {
                                if ( foot1.getZustand() )
                                {
                                    clopendi = FALSE;
                                }
                                else
                                {
                                    if ( TKLAMMER_edge )
                                    {
                                        TKLAMMER_edge = FALSE;
                                        TKLAMMER_merker = TRUE;
                                        return 0;
                                    }
                                    else
                                    {
                                        if ( !clopendi )
                                        {
                                            if ( !saf->cutready )
                                            {
                                                return 2;
                                            }
                                            else
                                            {
                                                /* EasyCODE ( 0 
                                                Nadel positionieren,
                                                Fuss auf, Klemme zu, Klammer auf */
                                                switch ( nup.napos() )
                                                {
                                                case 0 :
                                                    return 0;
                                                case 1 :
                                                    koor.nae_err = 1;
                                                    nae.errpar = nminf.fehler;
                                                    return 1;
                                                default:
                                                    TKLAMMER_merker = FALSE;
                                                    fs1_merker = FALSE;
                                                    hfuss.auf();         // Fuss auf
                                                    y4.setAuftrag();     // Klemme zu
                                                    y51u1.setAuftrag();  // Schablonenverriegelung auf
                                                    nae.res_barcode();
                                                    schabl.bc.res_barcode();
                                                    schabl.rf.delete_rfidprognummer();
                                                    barcode_lesen = TRUE;
                                                    if ( saf->md.jigctren == 2 )
                                                    {
                                                        abl.set_anzauftrag(2);
                                                    }
                                                    return 0;
                                                }
                                                /* EasyCODE ) */
                                            }
                                        }
                                    }
                                }
                                if ( foot2.getZustand() )
                                {
                                    return 3;
                                }
                                else
                                {
                                    /* EasyCODE ( 0 
                                    Tasten */
                                    switch ( taste )
                                    {
                                    case TSTART :
                                        if ( barcode_lesen )
                                        {
                                            nae.set_barcode();
                                        }
                                        return 3;
                                    case TFWD :
                                        if ( barcode_lesen )
                                        {
                                            nae.set_barcode();
                                        }
                                        return 4;
                                    case TBACK :
                                        if ( barcode_lesen )
                                        {
                                            nae.set_barcode();
                                        }
                                        return 5;
                                    default:
                                        if ( (schabl.bc.get_barcode() != barcode_alt) &&
                                             barcode_lesen )
                                        {
                                            nae.set_barcode();
                                            barcode_alt = schabl.bc.get_barcode();
                                            if ( saf->md.jigctren == 2 )
                                            {
                                                abl.set_anzauftrag(2);
                                            }
                                        }
                                        return 0;
                                    }
                                    /* EasyCODE ) */
                                }
                            }
                            else
                            {
                                /* EasyCODE ( 0 
                                Taste Klammer
                                war gedrueckt */
                                if ( !saf->cutready )
                                {
                                    return 2;
                                }
                                else
                                {
                                    switch ( nup.napos() )
                                    {
                                    case 0 :
                                        return 0;
                                    case 1 :
                                        koor.nae_err = 1;
                                        nae.errpar = nminf.fehler;
                                        return 1;
                                    default:
                                        TKLAMMER_merker = FALSE;
                                        fs1_merker = FALSE;
                                        hfuss.auf();        // Fuss auf
                                        y4.setAuftrag();    // Klemme zu
                                        y51u1.setAuftrag(); // Klammer auf
                                        nae.res_barcode();
                                        schabl.bc.res_barcode();
                                        schabl.rf.delete_rfidprognummer();
                                        barcode_lesen = TRUE;
                                        if ( saf->md.jigctren == 2 )
                                        {
                                            abl.set_anzauftrag(2);
                                        }
                                        return 0;
                                    }
                                }
                                /* EasyCODE ) */
                            }
                            /* EasyCODE ) */
                        }
                        else
                        {
                            /* EasyCODE ( 0 
                            Klammer offen */
                            if ( foot1.getZustand() )
                            {
                                /* EasyCODE ( 0 
                                Fussschalter
                                1. Stufe */
                                if ( e79.getZustand() && e80.getZustand() )
                                {
                                    // Klammer richtig eingelegt
                                    clopendi = FALSE;
                                    TKLAMMER_edge = FALSE;
                                    y51u2.setAuftrag();  // Klammer zu
                                    nae.res_barcode();
                                    schabl.bc.res_barcode();
                                    barcode_lesen = TRUE;
                                    schabl.rf.set_rfid_read();
                                    schabl.bc.set_bc_read(); //SW005-1
                                    if ( saf->md.jigctren == 2 )
                                    {
                                        abl.set_anzauftrag(2);
                                    }
                                    return 0;
                                }
                                else
                                {
                                    // Klammer falsch
                                    //  eingelegt
                                    koor.nae_err = 14;
                                    return 1;
                                }
                                /* EasyCODE ) */
                            }
                            else
                            {
                                if ( TKLAMMER_edge )
                                {
                                    /* EasyCODE ( 0 
                                    Taste Klammer */
                                    if ( e79.getZustand() && e80.getZustand() )
                                    {
                                        clopendi = TRUE;
                                        TKLAMMER_edge = FALSE;
                                        y51u2.setAuftrag();  // Klammer zu
                                        nae.res_barcode();
                                        schabl.bc.res_barcode();
                                        barcode_lesen = TRUE;
                                        schabl.rf.set_rfid_read();
                                        schabl.bc.set_bc_read(); //SW005-1
                                        if ( saf->md.jigctren == 2 )
                                        {
                                            abl.set_anzauftrag(2);
                                        }
                                        return 0;
                                    }
                                    else
                                    {
                                        return 7;
                                    }
                                    /* EasyCODE ) */
                                }
                                else
                                {
                                    if ( (taste == TSTART)||
                                         (taste == TFWD) ||
                                         (taste == TBACK) )
                                    {
                                        /* EasyCODE ( 0 
                                        Taste Start oder
                                        Vorwaerts Takten oder
                                        Rueckwaerts Takten */
                                        if ( e79.getZustand() && e80.getZustand() )
                                        {
                                            // Klammer richtig eingelegt
                                            clopendi = TRUE;
                                            TKLAMMER_edge = FALSE;
                                            y51u2.setAuftrag();  // Klammer zu
                                            nae.res_barcode();
                                            schabl.bc.res_barcode();
                                            barcode_lesen = TRUE;
                                            schabl.rf.set_rfid_read();
                                            schabl.bc.set_bc_read(); //SW005-1
                                            if ( saf->md.jigctren == 2 )
                                            {
                                                abl.set_anzauftrag(2);
                                            }
                                            return 0;
                                        }
                                        else
                                        {
                                            // Klammer falsch
                                            //  eingelegt
                                            koor.nae_err = 14;
                                            return 7;
                                        }
                                        /* EasyCODE ) */
                                    }
                                    else
                                    {
                                        return 0;
                                    }
                                }
                            }
                            /* EasyCODE ) */
                        }
                    #endif 
                    /* EasyCODE ) */
                }
            }
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE ( 0 
            120x170 mit Band
            einlegen, Start nur über
            externe Tasten */
            #ifdef buerotest
                /* EasyCODE ( 0 
                Klammer geschlossen */
                if ( !TKLAMMER_merker )
                {
                    if ( fs1_merker )
                    {
                        if ( tnae1.tz() )
                        {
                        }
                        else
                        {
                            if ( fs2_edge )
                            {
                                return 3;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                    else
                    {
                        if ( fs1_edge )
                        {
                            fs1_edge = FALSE;
                            fs1_merker = TRUE;
                            tnae1.rsz();
                            tnae1.sz(saf->fsverz);
                            return 0;
                        }
                        else
                        {
                            if ( fs2_edge )
                            {
                                if ( barcode_lesen )
                                {
                                    nae.set_barcode();
                                }
                                return 3;
                            }
                            else
                            {
                                if ( TKLAMMER_edge )
                                {
                                    TKLAMMER_merker = TRUE;
                                    TKLAMMER_edge = FALSE;
                                    return 0;
                                }
                                else
                                {
                                    /* EasyCODE ( 0 
                                    Tasten */
                                    switch ( taste )
                                    {
                                    case TSTART :
                                        if ( barcode_lesen )
                                        {
                                            nae.set_barcode();
                                        }
                                        return 3;
                                    case TFWD :
                                        if ( barcode_lesen )
                                        {
                                            nae.set_barcode();
                                        }
                                        return 4;
                                    case TBACK :
                                        if ( barcode_lesen )
                                        {
                                            nae.set_barcode();
                                        }
                                        return 5;
                                    default:
                                        if ( (schabl.bc.get_barcode() != barcode_alt) &&
                                             barcode_lesen )
                                        {
                                            nae.set_barcode();
                                            barcode_alt = schabl.bc.get_barcode();
                                            if ( saf->md.jigctren == 2 )
                                            {
                                                abl.set_anzauftrag(2);
                                            }
                                        }
                                        return 0;
                                    }
                                    /* EasyCODE ) */
                                }
                            }
                        }
                    }
                }
                if ( !saf->cutready )
                {
                    return 2;
                }
                else
                {
                    /* EasyCODE ( 0 
                    Nadel positionieren,
                    Fuss auf, Klemme zu, Klammer auf */
                    switch ( nup.napos() )
                    {
                    case 0 :
                        return 0;
                    case 1 :
                        koor.nae_err = 1;
                        nae.errpar = nminf.fehler;
                        return 1;
                    default:
                        TKLAMMER_merker = FALSE;
                        fs1_merker = FALSE;
                        hfuss.auf();         // Fuss auf
                        y4.setAuftrag();     // Klemme zu
                        y51u1.setAuftrag();  // Schablonenverriegelung auf
                        nae.res_barcode();
                        schabl.bc.res_barcode();
                        schabl.rf.delete_rfidprognummer();
                        barcode_lesen = TRUE;
                        if ( saf->md.jigctren == 2 )
                        {
                            abl.set_anzauftrag(2);
                        }
                        return 0;
                    }
                    /* EasyCODE ) */
                }
                /* EasyCODE ) */
            #else 
                fsedge ();
                if ( e51u2_1.getZustand() && e51u2_2.getZustand() )
                {
                    /* EasyCODE ( 0 
                    Klammer geschlossen */
                    if ( !TKLAMMER_merker )
                    {
                        if ( TKLAMMER_edge )
                        {
                            TKLAMMER_merker = TRUE;
                            TKLAMMER_edge = FALSE;
                            return 0;
                        }
                        else
                        {
                            /* EasyCODE ( 0 
                            Tasten */
                            if ( sonderstart () )
                            {
                                if ( barcode_lesen )
                                {
                                    nae.set_barcode();
                                }
                                return 3;
                            }
                            else
                            {
                                switch ( taste )
                                {
                                case TFWD :
                                    if ( barcode_lesen )
                                    {
                                        nae.set_barcode();
                                    }
                                    return 4;
                                case TBACK :
                                    if ( barcode_lesen )
                                    {
                                        nae.set_barcode();
                                    }
                                    return 5;
                                default:
                                    if ( (schabl.bc.get_barcode() != barcode_alt) &&
                                         barcode_lesen )
                                    {
                                        nae.set_barcode();
                                        barcode_alt = schabl.bc.get_barcode();
                                        if ( saf->md.jigctren == 2 )
                                        {
                                            abl.set_anzauftrag(2);
                                        }
                                    }
                                    return 0;
                                }
                            }
                            /* EasyCODE ) */
                        }
                    }
                    else
                    {
                        if ( !saf->cutready )
                        {
                            return 2;
                        }
                        else
                        {
                            /* EasyCODE ( 0 
                            Nadel positionieren,
                            Fuss auf, Klemme zu, Klammer auf */
                            switch ( nup.napos() )
                            {
                            case 0 :
                                return 0;
                            case 1 :
                                koor.nae_err = 1;
                                nae.errpar = nminf.fehler;
                                return 1;
                            default:
                                TKLAMMER_merker = FALSE;
                                fs1_merker = FALSE;
                                hfuss.auf();         // Fuss auf
                                y4.setAuftrag();     // Klemme zu
                                y51u1.setAuftrag();  // Schablonenverriegelung auf
                                nae.res_barcode();
                                schabl.bc.res_barcode();
                                schabl.rf.delete_rfidprognummer();
                                barcode_lesen = TRUE;
                                if ( saf->md.jigctren == 2 )
                                {
                                    abl.set_anzauftrag(2);
                                }
                                return 0;
                            }
                            /* EasyCODE ) */
                        }
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Klammer offen */
                    if ( TKLAMMER_edge )
                    {
                        /* EasyCODE ( 0 
                        Taste Klammer */
                        if ( e79.getZustand() && e80.getZustand() )
                        {
                        }
                        else
                        {
                            return 7;
                        }
                        /* EasyCODE ) */
                    }
                    else
                    {
                        if ( (taste == TFWD) ||
                             (taste == TBACK) )
                        {
                            if ( e79.getZustand() && e80.getZustand() )
                            {
                            }
                            else
                            {
                                koor.nae_err = 14;
                                return 1;
                            }
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    fs1_edge = FALSE;
                    TKLAMMER_edge = FALSE;
                    y51u2.setAuftrag();  // Schablonenverriegelung zu
                    nae.res_barcode();
                    schabl.bc.res_barcode();
                    barcode_lesen = TRUE;
                    schabl.rf.set_rfid_read();
                    schabl.bc.set_bc_read(); //SW005-1
                    if ( saf->md.jigctren == 2 )
                    {
                        abl.set_anzauftrag(2);
                    }
                    return 0;
                    /* EasyCODE ) */
                }
            #endif 
            /* EasyCODE ) */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::sonderstart */
/* EasyCODE F */
unsigned char KlasseNaeUp::sonderstart (void)
{
    unsigned char retwert = FALSE;
    if ( find_UFWSTOP () )
    {
        // Band einlegen mit Zwischenstopp
        if ( out4.getZustand() )
        {
            /* EasyCODE ( 0 
            links */
            if ( tlinksm )
            {
                if ( !tlinks_stop.getZustand() )
                {
                    retwert = TRUE;
                    tlinksm = FALSE;
                }
            }
            else
            {
                if ( tlinks_stop.getZustand() )
                {
                    tlinksm = TRUE;
                }
            }
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE ( 0 
            rechts */
            if ( trechtsm )
            {
                if ( !trechts_stop.getZustand() )
                {
                    retwert = TRUE;
                    trechtsm = FALSE;
                }
            }
            else
            {
                if ( trechts_stop.getZustand() )
                {
                    trechtsm = TRUE;
                }
            }
            /* EasyCODE ) */
        }
    }
    else
    {
        // kein Zwischenstopp
        if ( trechts_stop.getZustand() ||
             tlinks_stop.getZustand() )
        {
            retwert = TRUE;
        }
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::schneidablauf */
// Rueckmeldungen:   0 - nicht fertig
//                   1 - fertig, Faden geschnittten
//                   2 - fertig, Faden manuell geschnitten
//                   3 - Abbruch, Taste Eingabe gedrueckt
//                   4 - Abbruch, Stopanforderung
//                   5 - Abbruch, Stop-Taste oder Fehler
/* EasyCODE F */
unsigned char KlasseNaeUp::schneidablauf (void)
{
    unsigned char retwert = 0;
    int aktpos;
    switch ( schneidkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        schneidkey = 0,   Fadenschneiden einleiten */
        if ( saf->carposok && (saf->buf.nbtart == 1) && hfuss.posok )
        {
            /* EasyCODE ( 0 
            Fadenschneiden einleiten */
            hfuss.ab();          // Fuss ab
            y8.setAuftrag();     // Niederhalter ab
            y4.resAuftrag();     // Oberfadenklemme auf
            y30.resAuftrag();    // Schneiden aus
            y5.resAuftrag();     // Fadenspannung zu
            y33.setAuftrag();    // Stoffgegendruecker auf
            schneidkey = 2;
            /* EasyCODE ) */
        }
        else
        {
            nae_botsch1.senden(BV024);
            nae_botsch2.senden(hometext1);
            schneidkey = 1;
        }
        break;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        schneidkey = 1,   Abfrage "Automatisch Fadenschneiden" */
        if ( koor.naestop() )
        {
            retwert = 4;
        }
        else
        {
            if ( !nae_botsch1.busycheck() )
            {
                switch ( keyb.taste() )
                {
                case TENTER :
                    nae_botsch1.senden(BV023); 
                    y8.setAuftrag();     // Niederhalter ab  SW002  
                    y4.resAuftrag();     // Oberfadenklemme auf
                    y5.resAuftrag();     // Fadenspannung zu
                    y30.resAuftrag();    // Schneiden aus
                    y33.setAuftrag();    // Stoffgegendruecker auf
                    hfuss.posinitkey = 0;
                    if ( hfuss.posok )
                    {
                        hfuss.ab();  // Hfuss ab
                        schneidkey = 2;
                    }
                    else
                    {
                        tnae1.rsz();
                        tnae1.sz(saf->initzt);
                        schneidkey = 10;
                    }
                    break;
                case TESC :
                    nae_botsch1.senden(BV023);
                    saf->cutready = TRUE;
                    nae.cutact = FALSE;
                    nae.cutreq = FALSE;
                    retwert = 2;
                    break;
                case TEINGABE :
                    eing.auftrag = 2;
                    retwert = 3;
                    break;
                default:
                    break;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        schneidkey = 2,   Fadenschneiden starten */
        if ( koor.naestop() )
        {
            retwert = 4;
        }
        else
        {
            if ( y4.fertig() && y5.fertig() && y30.fertig() && y33.fertig() &&
                 y8.fertig() && hfuss.ready() && (nminf.befehl == 0) &&
                 // (hfuss.getpos().pos == saf->md.posunten.pos)    //   SW010
                    (hfuss.getpos().pos == hfuss.akt_posunten.pos)  //   SW011
               )
            {
                if ( nminf.fehler != 0 )
                {
                    koor.nae_err = 1;
                    nae.errpar = nminf.fehler;
                    retwert = 4;
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
                    tnae1.rsz();
                    tnae1.sz(saf->niszeit);
                    schneidenaus_flag = FALSE;
                    spgauf_flag = FALSE;
                    schneidkey = 3;
                }
            }
            else
            {
                if ( y8.fertig() &&
                     //(hfuss.getpos().pos != saf->md.posunten.pos)    // SW010
                       (hfuss.getpos().pos != hfuss.akt_posunten.pos)  // SW011
                   )
                {
                    // Jetzt nochmal, da wegen
                    // y8.fertig der aufruf
                    // in Case 1
                    // ohne wirkung sein
                    // kann  
                    // siehe 
                    // KlasseHuepferfuss::ab
                    hfuss.ab();
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        schneidkey = 3,   Warten bis Fadenschneiden ein */
        if ( koor.notstop() )
        {
            outportb (INTMASK, REF1DI | REF2DI);
            nminf.befehl = 8;
            retwert = 4;
        }
        else
        {
            if ( y30.getZustand() )
            {
                schneidenaus_flag = FALSE;
                spgauf_flag = FALSE;
                schneidkey = 4;
            }
            else
            {
                if ( tnae1.tz() )
                {
                    koor.nae_err = 21;
                    outportb (INTMASK, REF1DI | REF2DI);
                    nminf.befehl = 8;
                    retwert = 4;
                }
            }
        }
        break;
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        schneidkey = 4,   Fadenschneid-Ventil aus und
                          Fadenspannung auf beim Schneiden */
        if ( koor.notstop() )
        {
            outportb (INTMASK, REF1DI | REF2DI);
            nminf.befehl = 8;
            retwert = 4;
        }
        else
        {
            /* EasyCODE ( 0 
            Schneidventil aus,
            Fadenspannung auf */
            if ( nminf.befehl != 0 )
            {
                aktpos = nminf.nadel_pos() - saf->nadelp;
                if ( aktpos < 0 )
                {
                    aktpos += PULSE_PRO_UMDR;
                }
                aktpos = (((long)aktpos * 360)/PULSE_PRO_UMDR);
                if ( !schneidenaus_flag )
                {
                    if ( saf->md.y30offvograd >= saf->md.ref2 )
                    {
                        /* EasyCODE ( 0 
                        Pos. fuer Schneiden aus liegt zwischen Ref2 und NOT */
                        if ( aktpos >= saf->md.y30offvograd )
                        {
                            // Istpos. zwischen Fadenspannungspos. und NOT
                            y30.resAuftrag();  // Schneiden aus
                            schneidenaus_flag = TRUE;
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
                        }
                        else
                        {
                            if ( aktpos < saf->md.y30offvograd )
                            {
                                /* EasyCODE < */
                                // Istpos. zwischen NOT und Fadenspannungspos.
                                /* EasyCODE > */
                            }
                            else
                            {
                                y30.resAuftrag();  // Schneiden aus
                                schneidenaus_flag = TRUE;
                            }
                        }
                        /* EasyCODE ) */
                    }
                }
                if ( !spgauf_flag )
                {
                    if ( eing.fadsppos >= saf->md.ref2 )
                    {
                        /* EasyCODE ( 0 
                        Fadenspannungspos. liegt zwischen Ref2 und NOT */
                        if ( aktpos >= eing.fadsppos )
                        {
                            // Istpos. zwischen Fadenspannungspos. und NOT
                            y5.setAuftrag();     // Fadenspannung auf  
                            y6.resAuftrag();  // Zusatzfadenspannung auf
                            spgauf_flag = TRUE;
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
                        }
                        else
                        {
                            if ( aktpos < eing.fadsppos )
                            {
                                /* EasyCODE < */
                                // Istpos. zwischen NOT und Fadenspannungspos.
                                /* EasyCODE > */
                            }
                            else
                            {
                                y5.setAuftrag(); // Fadenspannung auf  
                                
                                
                                y6.resAuftrag();  // Zusatzfadenspannung auf
                                spgauf_flag = TRUE;
                            }
                        }
                        /* EasyCODE ) */
                    }
                }
                if ( schneidenaus_flag && spgauf_flag )
                {
                    fcutkey = 0;
                    schneidkey = 5;
                }
            }
            else
            {
                y30.resAuftrag(); // Schneiden aus
                y5.setAuftrag();     // Fadenspannung auf  
                y6.resAuftrag();  // Zusatzfadenspannung auf
                fcutkey = 0;
                schneidkey = 5;
            }
            /* EasyCODE ) */
        }
        break;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        schneidkey = 4,   Fadenschneiden */
        if ( fcutportal () )
        {
            retwert = 1;
        }
        else
        {
            if ( koor.naestop() || (koor.nae_err != 0) )
            {
                outportb (INTMASK, REF1DI | REF2DI);
                nminf.befehl = 8;
                retwert = 4;
            }
        }
        break;
        /* EasyCODE ) */
    case 10 :
        /* EasyCODE ( 0 
        schneidkey = 10,   Huepferfuss positionieren */
        if ( hfuss.posinit() )
        {
            hfuss.ab();
            schneidkey = 2;
        }
        else
        {
            if ( tnae1.tz() )
            {
                koor.nae_err = 24;
                retwert = 4;
            }
        }
        break;
        /* EasyCODE ) */
    default:
        break;
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::fcutportal */
/* EasyCODE F */
unsigned char KlasseNaeUp::fcutportal (void)
{
    switch ( fcutkey )
    {
    case 0 :
        if ( nminf.befehl == 0 )
        {
            if ( nminf.fehler != 0 )
            {
                koor.nae_err = 1;
                nae.errpar = nminf.fehler;
            }
            else
            {
                if ( nminf.motor_steht )
                {
                    outportb (INTMASK, REF2DI);
                    y4.setAuftrag();  // Klemme zu
                    y5.resAuftrag();  // Fadenspannung zu
                    y33.resAuftrag(); // Stoffgegendruecker ab
                    tnae2.rsz();
                    tnae2.sz (20);
                    fcutkey = 1;
                }
            }
        }
        return FALSE;
    case 1 :
        if ( y4.fertig() && y33.fertig() && tnae2.tz() )
        {
            nminf.befehl = 5;
            nminf.posart = KURZ;
            nminf.position = saf->nadelp>>1;
            fcutkey = 2;
            tnae1.rsz();
            tnae1.sz(200);
        }
        return FALSE;
    case 2 :
        if ( nminf.befehl == 0 )
        {
            if ( nminf.fehler == 0 )
            {
                if ( hfuss.ready() )
                {
                    saf->cutready = TRUE;
                    nae.uf_contnae = FALSE;
                    hfuss.set_istpos (hfuss.akt_posunten);
                    if ( saf->md.wedler )
                    {
                        if ( saf->md.wedelnfussoben )
                        {
                            tnae1.rsz();
                            tnae1.sz(200);
                            HF->enable = FALSE;
                            hfuss.hfmove(FCUT_MOTOR);
                            fcutkey = 6;
                        }
                        else
                        {
                            HF->enable = FALSE;
                            fcutkey = 3;
                            if ( hfuss.akt_posunten.pos < 40 )
                            {
                                y2fkt.setAuftrag(); // Fadenwedeln
                            }
                            else
                            {
                                //Achtung hier kein Fadenwedeln
                                //ohne Warnung
                            }
                        }
                    }
                    else
                    {
                        HF->enable = FALSE;
                        fcutkey = 3;
                    }
                }
                else
                {
                    if ( tnae1.tz() )
                    {
                        koor.nae_err = 25;
                    }
                }
            }
            else
            {
                koor.nae_err = 1;
                nae.errpar = nminf.fehler;
            }
        }
        return FALSE;
    case 3 :
        if ( y2fkt.fertig() )
        {
            saf->cutready = TRUE;
            hfuss.auf();
            y8.resAuftrag(); // Niederhalter auf
            fcutkey = 4;
        }
        return FALSE;
    case 4 :
        if ( hfuss_auf.fertig() && y8.fertig() )
        {
            fcutkey = 5;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    case 5 :
        return TRUE;
    case 6 :
        if ( hfuss.ready() )
        {
            hfuss.istpos.pos = FCUT_POS;
            hfuss.istpos.motor = FCUT_MOTOR;
            y2fkt.setAuftrag(); // Fadenwedeln
            fcutkey = 3;
        }
        else
        {
            if ( tnae1.tz() )
            {
                koor.nae_err = 25;
            }
        }
        return FALSE;
    default:
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::find_out1 */
/* EasyCODE F */
unsigned char KlasseNaeUp::find_out1 (void)
{
    /* EasyCODE < */
    struct p_abschstruct *workptr;
    
    workptr = (struct p_abschstruct *) (ndatanf + GAP);
    /* EasyCODE > */
    do{
        workptr++;
        if ( (workptr->art == AB_OUTSET) || (workptr->art == AB_OUTRES) )
        {
            if ( workptr->par2 == 1 )
            {
                return TRUE;
            }
        }
    }while ( workptr->art != AB_PRENDE );
    return FALSE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::find_UFWSTOP */
/* EasyCODE F */
unsigned char KlasseNaeUp::find_UFWSTOP (void)
{
    /* EasyCODE < */
    struct p_abschstruct *workptr;
    
    workptr = (struct p_abschstruct *) (ndatanf + GAP);
    /* EasyCODE > */
    do{
        workptr++;
        if ( workptr->art == AB_UFWSTOP )
        {
            return TRUE;
        }
    }while ( workptr->art != AB_PRENDE );
    return FALSE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::foot */
/* EasyCODE F */
// Das Up berbeitet die Taste Presserfuss auf/ab. Vor Presserfuss auf, wird die
// Nadel positioniert. Parallel zum Fuss wird die Fadenspannung und die
// Fadenklemme geschaltet.

void KlasseNaeUp::foot (void)
{
    if ( hfuss.ready() && y4.fertig() && y5.fertig() &&
         y8.fertig() && hfuss_auf.fertig() )
    {
        if ( !naposact )
        {
            /* EasyCODE ( 0 
            Taste abfragen, ev. Fuss ab */
            if ( keyb.taste() == TFUSS )
            {
                if ( !footm )
                {
                    if ( hfuss.getpos().pos == saf->md.posoben.pos )
                    {
                        /* EasyCODE ( 0 
                        Fuss ab */
                        if ( y8.getZustand() ||
                             ((saf->variante==1) && !nup.find_UFWSTOP () && !y8.getZustand()) )
                        {
                            footm = TRUE;
                            hfuss.ab();         // Fuss ab
                            y4.resAuftrag();    // Klemme auf
                            if ( nup.carlp && e50u1.getZustand() )
                            {
                                // Schlitten in Grundstellung
                                // und Oberteil angehoben
                                y5.setAuftrag();  // Fadenspannung auf
                                y6.resAuftrag();  // Zusatzfadenspannung auf  SW016
                            }
                            else
                            {
                                // Fadenspannung zu
                                y5.resAuftrag();
                            }
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
                        /* EasyCODE ) */
                    }
                    else
                    {
                        footm = TRUE;
                        /* EasyCODE - */
                        naposact = TRUE;
                    }
                }
            }
            else
            {
                footm = FALSE;
            }
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE ( 0 
            Nadel positionieren, Fuss auf, Klemme zu, Fadenspannung auf
            Niederhalter auf */
            switch ( nup.napos() )
            {
            case 0 :
                naposact = TRUE;
                break;
            case 1 :
                naposact = FALSE;
                koor.nae_err = 1;
                nae.errpar = nminf.fehler;
                break;
            default:
                naposact = FALSE;
                hfuss.auf();           // Fuss auf
                y4.setAuftrag();       // Klemme zu
                y5.resAuftrag();       // Fadenspannung zu
                y8.resAuftrag();       // Niederhalter auf
                break;
            }
            /* EasyCODE ) */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::anschlaege */
/* EasyCODE F */
// Das Up berbeitet die Tasten tlinks_stop und trechts_stop im
// LP, um die Anschlaege zu schalten.

void KlasseNaeUp::anschlaege (void)
{
    if ( carlp && y52.fertig() && y53.fertig() && (saf->variante == 0) )
    {
        /* EasyCODE ( 0 
        Taste tlinks_stop abfragen */
        if ( tlinks_stop.getZustand() )
        {
            if ( !tlinksm )
            {
                tlinksm = TRUE;
                if ( y52.getZustand() )
                {
                    y52.resAuftrag();
                }
                else
                {
                    y52.setAuftrag();
                }
            }
        }
        else
        {
            tlinksm = FALSE;
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Taste trechts_stop abfragen */
        if ( trechts_stop.getZustand() )
        {
            if ( !trechtsm )
            {
                trechtsm = TRUE;
                if ( y53.getZustand() )
                {
                    y53.resAuftrag();
                }
                else
                {
                    y53.setAuftrag();
                }
            }
        }
        else
        {
            trechtsm = FALSE;
        }
        /* EasyCODE ) */
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::fsedge */
/* EasyCODE F */
void KlasseNaeUp::fsedge (void)
{
    if ( !fs1_edge )
    {
        if ( foot1.getZustand() )
        {
            if ( !fs1_mem )
            {
                fs1_mem = TRUE;
                fs1_edge = TRUE;
            }
        }
        else
        {
            fs1_mem = FALSE;
        }
    }
    if ( !fs2_edge )
    {
        if ( foot2.getZustand() )
        {
            if ( !fs2_mem )
            {
                fs2_mem = TRUE;
                fs2_edge = TRUE;
            }
        }
        else
        {
            fs2_mem = FALSE;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::getprpar */
/* EasyCODE F */
/***************************** getprpar ***************************************/
/*                                                                            */
/* Das Up schreibt die Programmanfangsadresse nach proganfadr,  die Anfangs-  */
/* adresse der Stichdaten nach stianfadr, die erste freie Adresse nach        */
/* endadr. Ausserdem wird die Speicherzelle stigsw gesetzt.                   */
/*                                                                            */
/******************************************************************************/
void KlasseNaeUp::getprpar (void)
{
    unsigned char *zeiger, i;
    if ( saf->md.prsel == 0 )
    {
        /* EasyCODE < */
        // normales Programm
        /* EasyCODE > */
        for ( i =0; i<=3; i++ )
        {
            ems_map (prog_handle [0], i, i);
        }
    }
    else
    {
        for ( i =0; i<=3; i++ )
        {
            ems_map (prog_handle [saf->md.pnrindex], i, i);
        }
    }
    proganfadr = ndatanf;
    stianfadr = proganfadr + *((unsigned long *) (proganfadr + KSTICHDAT));
    endadr = proganfadr + *((unsigned long *) (proganfadr + KPROGLNG));
    if ( *((unsigned int *) (stianfadr + SSTICHL)) == 0 )
    {
        stigsw = GSWMAX;
    }
    else
    {
        stigsw = saf->stigswtab [(*((unsigned int *) (stianfadr + SSTICHL))/10)>>2];
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::klammer_edge */
/* EasyCODE F */
void KlasseNaeUp::klammer_edge (void)
{
    if ( !TKLAMMER_edge )
    {
        if ( keyb.taste() != TSCHABLONE )
        {
            TKLAMMER_mem = FALSE;
        }
        else
        {
            if ( !TKLAMMER_mem )
            {
                TKLAMMER_mem = TRUE;
                TKLAMMER_edge = TRUE;
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Klasse::NaeUp::jig_ctrl */
/* EasyCODE F */
// Das Up prueft, ob der gelesene Schablonencode mit dem aus dem Stichdaten-
// satz uebereinstimmt, falls eine Schablone eingelegt und die Schablonen-
// ueberwachung eingeschaltet ist.

// Rueckmeldung:   0   ... Schablonencode falsch
//                 1   ... Schablonencode ok oder Ueberwachung ausgeschaltet
//                 2  ...  Lesen des Codes mit RFID noch nicht fertig
//                 3  ...  Fehler beim Lesen des Transponders

unsigned char KlasseNaeUp::jig_ctrl (void)
{
    unsigned int j;
    unsigned char retwert = 1;
    if ( (e79.getZustand() || e80.getZustand()) && (saf->md.jigctren == 1) )
    {
        /* EasyCODE ( 0 
        Klammer eingelegt und
        Klammerueberwachung eingeschaltet */
        switch ( saf->jigctrf )
        {
        case RFID :
            /* EasyCODE ( 0 
            RFID */
            switch ( j=schabl.rf.get_rfidprognummer (RFID_MASCHINENKLASSE) )
            {
            case 0xfffd :
                // Code ungueltig
                retwert = 0;
                break;
            case 0xfffe :
                retwert = 2;
                break;
            case 0xffff :
                retwert = 3;
                break;
            default:
                if ( j == *(unsigned int *)(proganfadr + GKLCODE + 1) )
                {
                    retwert = 1;
                }
                else
                {
                    retwert = 0;
                }
                break;
            }
            break;
            /* EasyCODE ) */
        case STATBARC :
            /* EasyCODE ( 0 
            stationaerer Barcode-Scanner SW005-1 */
            switch ( j=schabl.bc.get_bcprognummer() )
            {
            case 0xfffd :
                // Code ungueltig
                retwert = 0;
                break;
            case 0xfffe :
                retwert = 2;
                break;
            case 0xffff :
                retwert = 3;
                break;
            default:
                if ( j == *(unsigned int *)(proganfadr + GKLCODE + 1) )
                {
                    retwert = 1;
                }
                else
                {
                    retwert = 0;
                }
                break;
            }
            break;
            /* EasyCODE ) */
        case BARCODE :
            /* EasyCODE ( 0 
            Handscanner */
            // Barcode-Scanner
            j = nae.get_barcode();
            if ( j == *(unsigned int *)(proganfadr + GKLCODE + 1) )
            {
                retwert = 1;
            }
            else
            {
                retwert = 0;
            }
            break;
            /* EasyCODE ) */
        default:
            retwert = 0;
            break;
        }
        /* EasyCODE ) */
    }
    else
    {
        /* EasyCODE < */
        // keine Klammer eingelegt oder
        // Klammerueberwachung ausgeschaltet
        /* EasyCODE > */
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::naeout_akt */
/* EasyCODE F */
// Das Up schaltet die Naehausgaenge auf den richtigen Wert beim rueckwaerts ins
// Bild Takten.
void KlasseNaeUp::naeout_akt (void)
{
    unsigned char *workptr, zusp;
    
    nup.workbuf.naeout = 0;
    nup.workbuf.redgswen = FALSE;
    workptr = nup.stianfadr+SNPSTART;
    while ( workptr < nup.workbuf.datpoi )
    {
        if ( *workptr != 0xff )
        {
            workptr +=2;
        }
        else
        {
            switch ( *(workptr+1) )
            {
            case 7 :
                nup.workbuf.redgswen = TRUE;
                workptr += 3;
                break;
            case 8 :
                nup.workbuf.redgswen = FALSE;
                workptr += 3;
                break;
            case 0x11 :
                /* EasyCODE ( 0 
                Ausgang setzen */
                switch ( *(workptr+2) )
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
                workptr += 4;
                /* EasyCODE ) */
                break;
            case 0x12 :
                /* EasyCODE ( 0 
                Ausgang ruecksetzen */
                switch ( *(workptr+2) )
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
                workptr += 4;
                /* EasyCODE ) */
                break;
            case 0x13 :
                zusp = TRUE;
                workptr += 4;
                break;
            case 0x14 :
                zusp = FALSE;
                workptr += 4;
                break;
            case 0x18 :
                workbuf.posunten = *(workbuf.datpoi+2);
                hfuss.akt_posunten.pos = workbuf.posunten;
                hfuss.postomo (&hfuss.akt_posunten);
                workptr += 4;
                break;
            case 0x19 :
                workbuf.hub = *(workbuf.datpoi+2);
                hfuss.akt_hub = workbuf.hub;
                workptr += 4;
                break;
            default:
                do{
                    workptr++;
                }while ( *workptr != 0xff );
                workptr++;
            }
        }
    }
    /* EasyCODE ( 0 
    Ausgaenge aktualisieren */
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
    if ( nup.workbuf.naeout & 0x02 )
    {
        out2.setAuftrag();
    }
    else
    {
        out2.resAuftrag();
    }
    if ( nup.workbuf.naeout & 0x04 )
    {
        out3.setAuftrag();
    }
    else
    {
        out3.resAuftrag();
    }
    if ( nup.workbuf.naeout & 0x08 )
    {
        out4.setAuftrag();
    }
    else
    {
        out4.resAuftrag();
    }
    if ( zusp )
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
    /* EasyCODE ) */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::hfuss_akt */
/* EasyCODE F */
// Das Up schaltet den Huepferfuss auf das richtige Niveau beim
// rueckwaerts Takten.

void KlasseNaeUp::hfuss_akt (void)
{
    unsigned char *workptr, zusp;
    
    hfuss.akt_posunten = saf->md.posunten;
    workptr = nup.stianfadr+SNPSTART;
    while ( workptr < nup.workbuf.datpoi )
    {
        if ( *workptr != 0xff )
        {
            workptr +=2;
        }
        else
        {
            switch ( *(workptr+1) )
            {
            case 0x18 :
                workbuf.posunten = *(workptr+2);
                hfuss.akt_posunten.pos = workbuf.posunten;
                hfuss.postomo (&hfuss.akt_posunten);
                workptr += 4;
                break;
            case 0x19 :
                workbuf.hub = *(workptr+2);
                hfuss.akt_hub = workbuf.hub;
                workptr += 4;
                break;
            case 0x1c :
                workbuf.posunten = saf->md.hfniveau[(*(workptr+2))-1];
                hfuss.akt_posunten.pos = workbuf.posunten;
                hfuss.postomo (&hfuss.akt_posunten);
                workptr += 4;
                break;
            case 0x1d :
                workbuf.hub = saf->md.hfhub[(*(workptr+2))-1];
                hfuss.akt_hub = workbuf.hub;
                workptr += 4;
                break;
            default:
                do{
                    workptr++;
                }while ( *workptr != 0xff );
                workptr++;
            }
        }
    }
    if ( (hfuss.getpos().pos != saf->md.posoben.pos) &&
         (hfuss.getpos().pos != hfuss.akt_posunten.pos) )
    {
        hfuss.ab();
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::napos */
/* EasyCODE < */
// Falls die Nadel nicht innerhalb eines bestimmten Bereichs steht, wird sie
// in OT positioniert.

// R_ckmeldung: 0    .... nicht fertig, kein Fehler
//              1    .... nicht fertig, Fehler (nminf.fehler != 0)
//              0xff .... fertig, Nadel steht im g_ltigen Bereich
/* EasyCODE > */
/* EasyCODE F */
unsigned char KlasseNaeUp::napos (void)
{
    #ifdef buerotest
        return 0xff;
    #else 
        if ( nminf.befehl != 0 )
        {
            return 0;
        }
        else
        {
            if ( nminf.fehler != 0 )
            {
                // Notstop
                nminf.befehl = 8;
                return 1;
            }
            else
            {
                if ( saf->cutready || (nae.naekey == 17) )
                {
                    /* EasyCODE ( 0 
                    Faden geschnitten --->
                    in Nadel OT positionieren */
                    if ( nminf.nadel_ot () )
                    {
                        return 0xff;
                    }
                    else
                    {
                        nminf.position = saf->nadelp>>1;
                        nminf.posart = KURZ;
                        nminf.befehl = 5;
                        return 0;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Faden nicht geschnitten --->
                    in Nadel Fadenleger OT positionieren */
                    if ( nminf.nadel_bereich (saf->ot_bereich) )
                    {
                        return 0xff;
                    }
                    else
                    {
                        nminf.position = (saf->nadelp >> 1) + ((float)saf->md.fadlot*MAXPOS/360);
                        if ( nminf.position >= MAXPOS )
                        {
                            nminf.position -= MAXPOS;
                        }
                        nminf.posart = KURZ;
                        nminf.befehl = 5;
                        return 0;
                    }
                    /* EasyCODE ) */
                }
            }
        }
    #endif 
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::nasere */
/* EasyCODE F */
void KlasseNaeUp::nasere (void)
{
    if ( mbef [OUT1SET]                  // programmierbarer Ausgang 1
       )
    {
        out1.setAuftrag();
        workbuf.naeout |= 0x01;
        if ( (saf->md.drehmodus == 2) &&
             (saf->md.nadelanzahl == 1) )
        {
            // Nadeldreheinrichtung ein
            // Unterfadenschieber ein
            y7.setAuftrag();
            y32.setAuftrag();
        }
    }
    if ( mbef [OUT1RES]                   // programmierbarer Ausgang 1
       )
    {
        out1.resAuftrag();
        workbuf.naeout &= ~0x01;
        if ( saf->md.drehmodus == 2 )
        {
            // Nadeldreheinrichtung aus
            // Unterfadenschieber aus
            y7.resAuftrag();
            y32.resAuftrag();
        }
    }
    if ( mbef [OUT2SET]                   // programmierbarer Ausgang 2
       )
    {
        out2.setAuftrag();
        workbuf.naeout |= 0x02;
    }
    if ( mbef [OUT2RES]                   // programmierbarer Ausgang 2
       )
    {
        out2.resAuftrag();
        workbuf.naeout &= ~0x02;
    }
    if ( mbef [OUT3SET]                   // programmierbarer Ausgang 3
       )
    {
        out3.setAuftrag();
        workbuf.naeout |= 0x04;
    }
    if ( mbef [OUT3RES]                   // programmierbarer Ausgang 3
       )
    {
        out3.resAuftrag();
        workbuf.naeout &= ~0x04;
    }
    if ( mbef [OUT4SET]                   // programmierbarer Ausgang 4
       )
    {
        out4.setAuftrag();
        workbuf.naeout |= 0x08;
    }
    if ( mbef [OUT4RES]                   // programmierbarer Ausgang 4
       )
    {
        out4.resAuftrag();
        workbuf.naeout &= ~0x08;
    }
    if ( mbef [OUT6SET] )
    {
        // Oberteil heben beim Eilgang
        workbuf.naeout |= 0x20;
    }
    if ( mbef [OUT6RES] )
    {
        // oberteil nicht heben beim Eilgang
        workbuf.naeout &= ~0x20;
    }
    if ( mbef [ZUSPGON]                   // Zusatz-Fadenspannung
       )
    {
        y6.setAuftrag();
        workbuf.zuspgon = TRUE;
    }
    if ( mbef [ZUSPGOFF]                  // Zustaz-Fadenspannung
       )
    {
        y6.resAuftrag();
        workbuf.zuspgon = FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::nasere_back */
/* EasyCODE F */
void KlasseNaeUp::nasere_back (void)
{
    if ( nup.mbef[OUT1RES] )
    {
        /* EasyCODE ( 0 
        progr.
        Ausgang 1 */
        out1.setAuftrag();
        nup.workbuf.naeout |= 0x01;
        if ( (saf->md.drehmodus == 2) &&
             (saf->md.nadelanzahl == 1) )
        {
            // Nadeldreheinrichtung ein
            // Unterfadenschieber ein
            y7.setAuftrag();
            y32.setAuftrag();
        }
        /* EasyCODE ) */
    }
    else
    {
        if ( nup.mbef[OUT1SET] )
        {
            /* EasyCODE ( 0 
            progr.
            Ausgang 1 */
            out1.resAusgang();
            nup.workbuf.naeout &= ~0x01;
            if ( saf->md.drehmodus == 2 )
            {
                // Nadeldreheinrichtung aus
                // Unterfadenschieber aus
                y7.resAuftrag();
                y32.resAuftrag();
            }
            /* EasyCODE ) */
        }
        else
        {
            if ( mbef [OUT2RES] )
            {
                /* EasyCODE ( 0 
                progr.
                Ausgang 2 */
                out2.setAuftrag();
                nup.workbuf.naeout |= 0x02;
                /* EasyCODE ) */
            }
            else
            {
                if ( mbef [OUT2SET] )
                {
                    /* EasyCODE ( 0 
                    progr.
                    Ausgang 2 */
                    out2.resAusgang();
                    nup.workbuf.naeout &= ~0x02;
                    /* EasyCODE ) */
                }
                else
                {
                    if ( nup.mbef[OUT3RES] )
                    {
                        /* EasyCODE ( 0 
                        progr.
                        Ausgang 3 */
                        out3.setAuftrag();
                        nup.workbuf.naeout |= 0x04;
                        /* EasyCODE ) */
                    }
                    else
                    {
                        if ( nup.mbef[OUT3SET] )
                        {
                            /* EasyCODE ( 0 
                            progr.
                            Ausgang 3 */
                            out3.resAusgang();
                            nup.workbuf.naeout &= ~0x04;
                            /* EasyCODE ) */
                        }
                        else
                        {
                            if ( nup.mbef[OUT4RES] )
                            {
                                /* EasyCODE ( 0 
                                progr.
                                Ausgang 4 */
                                out4.setAuftrag();
                                nup.workbuf.naeout |= 0x08;
                                /* EasyCODE ) */
                            }
                            else
                            {
                                if ( nup.mbef[OUT4SET] )
                                {
                                    /* EasyCODE ( 0 
                                    progr.
                                    Ausgang 4 */
                                    out4.resAusgang();
                                    nup.workbuf.naeout &= ~0x08;
                                    /* EasyCODE ) */
                                }
                                else
                                {
                                    if ( mbef [OUT6RES] )
                                    {
                                        /* EasyCODE ( 0 
                                        Oberteil heben
                                        beim Verfahren */
                                        nup.workbuf.naeout |= 0x20;
                                        /* EasyCODE ) */
                                    }
                                    else
                                    {
                                        if ( mbef [OUT6SET] )
                                        {
                                            /* EasyCODE ( 0 
                                            Oberteil heben
                                            beim Verfahren */
                                            nup.workbuf.naeout &= ~0x20;
                                            /* EasyCODE ) */
                                        }
                                        else
                                        {
                                            if ( mbef [ZUSPGOFF] )
                                            {
                                                /* EasyCODE ( 0 
                                                Zusatz-
                                                Fadenspannung */
                                                y6.setAuftrag();
                                                nup.workbuf.zuspgon = TRUE;
                                                /* EasyCODE ) */
                                            }
                                            else
                                            {
                                                /* EasyCODE ( 0 
                                                Zusatz-
                                                Fadenspannung */
                                                if ( nup.mbef[ZUSPGON] )
                                                {
                                                    y6.resAuftrag();
                                                    nup.workbuf.zuspgon = FALSE;
                                                }
                                                /* EasyCODE ) */
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::poscheck */
/* EasyCODE < */
// Das Up pr_ft, ob die _bergebenen Bildkoordinaten innerhalb des N"hfeldes liegen.

// Eingangs-Parameter:   koord ... Zeiger auf die Koordinaten
// R_ckmeldung:          TRUE  ... ok, Koordinaten innerhalb des N"hfeldes
//                       FALSE ... Fehler, Koordinaten ausserhalb des N"hfeldes
/* EasyCODE > */
/* EasyCODE F */
unsigned char KlasseNaeUp::poscheck (koord *koord)
{
    if ( (koord->xbild < saf->xnaehfeld_min) || (koord->xbild > saf->xnaehfeld_max) ||
         (koord->ybild < saf->ynaehfeld_min) || (koord->ybild > saf->ynaehfeld_max) )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::progstat */
/* EasyCODE F */
void KlasseNaeUp::progstat (void)
{
    unsigned char index;
    if ( abl.anzauftrag_ready() )
    {
        if ( !TPSTAT_edge )
        {
            /* EasyCODE ( 0 
            Tasten abfragen */
            switch ( keyb.taste () )
            {
            case TPSTAT0 :
                index = 0;
                break;
            case TPSTAT1 :
                index = 1;
                break;
            case TPSTAT2 :
                index = 2;
                break;
            case TPSTAT3 :
                index = 3;
                break;
            case TPSTAT4 :
                index = 4;
                break;
            case TPSTAT5 :
                index = 5;
                break;
            case TPSTAT6 :
                index = 6;
                break;
            case TPSTAT7 :
                index = 7;
                break;
            default:
                index = 0xff;
                break;
            }
            if ( index != 0xff )
            {
                TPSTAT_edge = TRUE;
                saf->md.next_pnrindex = index;
            }
            /* EasyCODE ) */
        }
        else
        {
            if ( keyb.taste () == 0xff )
            {
                TPSTAT_edge = FALSE;
                if ( nup.carlp )
                {
                    if ( saf->md.pnrindex == saf->md.next_pnrindex )
                    {
                    }
                    else
                    {
                        saf->md.pnrindex = saf->md.next_pnrindex;
                        getprpar();
                        workbuf.datpoi = stianfadr + SNPSTART;
                        saf->buf = workbuf;
                        abl.set_anzauftrag(7);
                    }
                }
                else
                {
                    abl.set_anzauftrag(6);
                }
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::read_jigpnr */
/* EasyCODE F */
// Das Up liest bei eingeschalteter automatischer Programmnummerwahl
// die Programmnummer von der Schablone. Die Programmnummer saf->md.pnr
// wird entsprechend gesetzt.

// Rueckmeldung: 0  ...  gelesene Programmnummer gueltig, saf->md.pnr gesetzt
//               1  ...  keine Klammer eingelegt, Programmnummer konnte nicht gelesen werden,
//               2  ...  Klammer eingelegt, Programmnummer ungueltig
//               3  ...  Lesen der Progr.nr. mit RFID noch nicht fertig
//               4  ...  Fehler beim Lesen des Transponders

unsigned char KlasseNaeUp::read_jigpnr (void)
{
    unsigned int u;
    unsigned char retwert = 2;
    if ( ((!e79.getZustand() && !e80.getZustand()) || !e51u2_1.getZustand() || !e51u2_2.getZustand())
                                   &&
         !((saf->md.jigctren == 2) && (saf->jigctrf == BARCODE))   // SWb32 //SW005-1
       )
    {
        // keine Klammer eingelegt
        retwert = 1;
    }
    else
    {
        switch ( saf->jigctrf )
        {
        case RFID :
            /* EasyCODE ( 0 
            RFID */
            switch ( u=schabl.rf.get_rfidprognummer (RFID_MASCHINENKLASSE) )
            {
            case 0xfffd :
                // Programmnummer ungueltig
                retwert = 2;
                break;
            case 0xfffe :
                retwert = 3;
                break;
            case 0xffff :
                retwert = 4;
                break;
            default:
                saf->md.pnr = u;
                retwert = 0;
                break;
            }
            break;
            /* EasyCODE ) */
        case STATBARC :
            /* EasyCODE ( 0 
            stationaerer Barcode-Scanner  SW005-1 */
            switch ( u=schabl.bc.get_bcprognummer () )
            {
            case 0xfffd :
                // Programmnummer ungueltig
                retwert = 2;
                break;
            case 0xfffe :
                retwert = 3;
                break;
            case 0xffff :
                retwert = 4;
                break;
            default:
                saf->md.pnr = u;
                retwert = 0;
                break;
            }
            break;
            /* EasyCODE ) */
        case BARCODE :
            /* EasyCODE ( 0 
            Handscanner */
            if ( nae.get_barcode() == 0xffff )
            {
                retwert = 2;
            }
            else
            {
                saf->md.pnr = nae.get_barcode();
                retwert = 0;
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
KlasseNaeUp::workbuf_res */
/* EasyCODE F */
void KlasseNaeUp::workbuf_res (void)
{
    workbuf.datpoi = stianfadr + SNPSTART;
    workbuf.nbtart = 0;
    workbuf.naeout = 0;
    workbuf.flag = 0;
    workbuf.naegsw = GSWMAX;
    workbuf.redgswen = FALSE;
    workbuf.zuspgon = FALSE;
    workbuf.posunten = saf->md.posunten.pos;
    workbuf.hub = saf->md.hub;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::nadeldreh */
/* EasyCODE F */
/* Achim Kretschmann
   Es wurde nur Quadrant 4 betrachtet, doch es muß Quadrant 1 und 4 betrachtet werden was nun korrigiert ist.

  Hilfsprog zur Berechnung ob Nadeldreheinrichtung geschaltet werden soll oder nicht.

   Rueckmeldung:  FALSE .... nicht schalten, so lassen wie es ist. 
                  TRUE  .... Einschalten bzw. Ausschalten oder wenn nicht im betreffenden Quadrant bei CHKAUS

                  Im Falle cmd == CHKQUA wird nur geprueft ob wir in den betreffenden Quadranten sind (return TRUE) 
                  oder nicht (return FALSE)                 
                  
*/

unsigned char KlasseNaeUp::nadeldreh (unsigned char cmd, int xbild2, int ybild2, int xbild1, int ybild1)
{
    unsigned char ret = FALSE;
    float x;
    double ax;
    if ( ((xbild2 - xbild1) > 0) )
    {
        // Im Quadrant 1 oder 4  (2 oder 3 beim Ruecktakten)
        if ( cmd == CHKQUA )
        {
            return TRUE;
        }
        //x = (float)(nup.koorsoll[nup.vorind].xbild - nup.workbuf.koorist.xbild) /
        //           (-nup.koorsoll[nup.vorind].ybild + nup.workbuf.koorist.ybild);
        // Tangens geht ja wohl anders rum Herr Halfmann und zwar so :
        
        x = ((float)ybild2 - (float)ybild1) / ((float)xbild2 - (float)xbild1);
        x = fabs(x);           
        ax = (atan(x));
        if ( ((ybild2 - ybild1) < 0) )
        {
            // Quadrant 4  (1 beim Ruecktakten)
            switch ( cmd )
            {
            case CHKEIN :
                if ( (ax <= saf->fadschein_winkel2) )
                {
                    ret = TRUE;
                }
                break;
            case CHKAUS :
                if ( (ax >= saf->fadschaus_winkel2) )
                {
                    ret = TRUE;
                }
                break;
            default:
                break;
            }
        }
        else
        {
            // Quadrant 1  (3 beim Ruecktakten)
            switch ( cmd )
            {
            case CHKEIN :
                if ( (ax <= saf->fadschein_winkel1) )
                {
                    ret = TRUE;
                }
                break;
            case CHKAUS :
                if ( (ax >= saf->fadschaus_winkel1) )
                {
                    ret = TRUE;
                }
                break;
            default:
                break;
            }
        }
    }
    else
    {
        //Quadrant 2 oder 3
        //(1 oder 4 beim Ruecktakten)
        switch ( cmd )
        {
        case CHKQUA :
            ret = FALSE;
            break;
        case CHKAUS :
            ret = TRUE;
            break;
        default:
            break;
        }
        //
    }
    return ret;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseNaeUp::Prog_vervollst */
/* EasyCODE F */
unsigned char KlasseNaeUp::Prog_vervollst (void)
{
    struct p_abschstruct *hlpptr;
    if ( (hlpptr = eing.pup.find_absch(AB_STDST,(struct p_abschstruct *)(ndatanf+GPROGNR))) == NULL )
    {
        eing.errpar =
           konv.konvert(ndatanf,0,MAXPROGLEN,&eing.errpar2);
    }
    else
    {
        eing.errpar =
           konv.konvert(ndatanf,hlpptr->par1,MAXPROGLEN,&eing.errpar2);
    }
    if ( eing.errpar != 0 )
    {
        koor.eing_err = 30;
        return 1;
    }
    else
    {
        switch ( di.storeprog () )
        {
        case 0 :
            return 0;
        case 1 :
            koor.eing_err = 25;
            return 1;
        case 2 :
            koor.eing_err = 26;
            return 1;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
