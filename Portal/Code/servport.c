/* EasyCODE V8 */
/* EasyCODE ( 0 
servport */
// Datei: servport.c   Klasse: 3590 Portal       Ersteller: Halfmann EWE 2856

#include "portal.h"
/* EasyCODE ( 0 
Defines */
/* EasyCODE < */
#define DISP_OUT        0x01
#define DISP_IN         0x02
#define DISP_MASK       0x04
#define DISP_TITEL      0x08
#define DISP_PFEIL      0x10
#define DISP_BV         0x20
#define DISP_ALL        0xFF
#define DISP_MASK_OUT   (DISP_MASK | DISP_OUT)
#define DISP_MOF        (DISP_MASK | DISP_OUT | DISP_PFEIL)
#define DISP_MOFT       (DISP_MASK | DISP_OUT | DISP_PFEIL | DISP_TITEL)
#define DISP_MOFTI      (DISP_MASK | DISP_OUT | DISP_PFEIL | DISP_TITEL | DISP_IN)
#define DISP_MOIF       (DISP_MASK | DISP_OUT | DISP_PFEIL | DISP_IN)
#define CAN_EING_PERIODE 30
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseService::grundzustand */
/* EasyCODE F */
unsigned char KlasseService::grundzustand(void)
{
    unsigned char pressed;
    unsigned char validateMask = 0;
    while ( 1 )
    {
        /* EasyCODE < */
        pressed = keyb.taste();// gedrukte Taste merken
        /* EasyCODE > */
        switch ( pressed )
        {
        case TENDE :
            /* EasyCODE ( 0 
            Ende */
            eing.ioact = FALSE;
            key = S_START;
            return E_EXIT;
            /* EasyCODE ) */
        case TEINGABE :
            /* EasyCODE ( 0 
            Eingabe */
            eing.ioact = FALSE;
            key = S_START;
            return E_GST;
            /* EasyCODE ) */
        case TKALTSTART :
            /* EasyCODE ( 0 
            Kaltstart */
            eing.ioact = FALSE;
            switch ( fkeytst(24) )
            {
            case 0
                         // frei
            :
                /* EasyCODE = */
            case 1
                         // frei (nach PIN eingabe)
            :
                key = S_START;
                return kaltstart();
            case 2
                         // gesperrt
            :
                key = S_START;
                break;
            default:
                key = S_FEHLER;
                break;
            }
            break;
            /* EasyCODE ) */
        case TNULLP :
            /* EasyCODE ( 0 
            Schlitten-Nullpunkt einstellen */
            eing.ioact = FALSE;
            switch ( fkeytst(30) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                key = S_START;
                return nullpunkte();
            case 2 // gesperrt
            :
                key = S_START;
                break;
            default:
                key = S_FEHLER;
                break;
            }
            break;
            /* EasyCODE ) */
        case THFNULLP :
            /* EasyCODE ( 0 
            Huepferfuss-Nullpunkt einstellen */
            eing.ioact = FALSE;
            switch ( fkeytst(27) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                key = S_START;
                return hfussnullpunkt();
            case 2 // gesperrt
            :
                key = S_START;
                break;
            default:
                key = S_FEHLER;
                break;
            }
            break;
            /* EasyCODE ) */
        case THFUSS :
            /* EasyCODE ( 0 
            Huepferfuss */
            eing.ioact = FALSE;
            switch ( fkeytst(26) )
            {
            case 0 // frei
            :
                /* EasyCODE = */
            case 1 // frei (nach PIN eingabe)
            :
                key = S_START;
                return movehf();
            case 2 // gesperrt
            :
                key = S_START;
                break;
            default:
                key = S_FEHLER;
                break;
            }
            break;
            /* EasyCODE ) */
        default:
            if ( koor.eingstop() )
            {
                eing.ioact = FALSE;
                key = S_FEHLER;
            }
            /* EasyCODE ( 0 
            ServTanz */
            /* EasyCODE < */
            // eing.ioact
            /* EasyCODE > */
            switch ( key )
            {
            case S_START :
                /* EasyCODE ( 0 
                erste Aufruf (paint Display) */
                if ( CheckPage() )
                {
                    // Der erste sichbare Parameter ist
                    // gefunden und eingesetzt.Also weiter
                    Invalidate(DISP_ALL);
                    switch ( spage )
                    {
                    case 1 :
                        /* EasyCODE < */
                        sprintf(str,touchinv,TCAN1S); // can 1 invert
                        /* EasyCODE > */
                        break;
                    case 2 :
                        /* EasyCODE < */
                        sprintf(str,touchinv,TCAN2S); // can 2 invert
                        /* EasyCODE > */
                        break;
                    case 4 :
                        sprintf(str,touchinv,TSER_SONDERS);
                        break;
                    }
                    eing_botsch1.senden(str);
                    lastKey = pressed;
                    // reset repeater timers
                    teing1.lz(); 
                    teing2.lz();
                    // Zeitabstand die Eingaenge zu aktualisieren
                    teing3.lz(); 
                    key = S_IO;
                    teing3.rsz();
                    teing3.sz(20);
                }
                else
                {
                    /* EasyCODE < */
                    // keine gultige 
                    // Parameter gefunden
                    /* EasyCODE > */
                    return E_GST;
                }
                /* EasyCODE ) */
                break;
            case S_IO :
                /* EasyCODE ( 0 */
                switch ( tastKey )
                {
                case 1 :
                    tastKey = 5;
                    break;
                case 5 :
                    /* EasyCODE ( 0 
                    is a new key pressed ? */
                    if ( lastKey == pressed )
                    {
                        /* EasyCODE ( 0 
                        key repeat or no key */
                        if ( pressed==0xff )
                        {
                            /* EasyCODE < */
                            // released (Taste los)
                            /* EasyCODE > */
                            if ( teing3.tz() )
                            {
                                teing3.rsz();
                                teing3.sz(CAN_EING_PERIODE);
                                switch ( spage )
                                {
                                case 1 :
                                    /* EasyCODE = */
                                case 2 :
                                    validateMask |= DISP_IN;
                                    break;
                                case 4 :
                                    validateMask |= DISP_OUT;
                                    break;
                                default:
                                    ;
                                }
                            }
                        }
                        else
                        {
                            if ( (pressed == TLEFT) || (pressed == TRIGHT) )
                            {
                                // go check first delay time
                                tastKey = 10;
                            }
                        }
                        /* EasyCODE ) */
                    }
                    else
                    {
                        /* EasyCODE ( 0 
                        New key */
                        /* EasyCODE < */
                        lastKey = pressed;
                        // Invalidate(Operate(pressed));
                        validateMask = Operate(pressed);
                        teing1.rsz();
                        teing1.sz(saf->slowdelay); // delay for 1.st repeat
                        teing2.lz(); // stop repeater
                        /* EasyCODE > */
                        /* EasyCODE ) */
                    }
                    /* EasyCODE ) */
                    break;
                case 10 :
                    /* EasyCODE ( 0 
                    is first delay time over ? */
                    if ( lastKey == pressed )
                    {
                        if ( teing1.tz() )
                        {
                            teing1.rsz();
                            // the last key will be slow repeated
                            // during this time 
                            //teing1.sz(saf->fastdelay);
                            //            teing2.rsz();
                            // repeater rate
                            teing1.sz(saf->slowtime);
                            // Invalidate(Operate(pressed));
                            validateMask = Operate(pressed);
                            tastKey = 15; // start slow repeat and wait to fast
                        }
                    }
                    else
                    {
                        // Key is released before timeout
                        // or any other key is pressed
                        tastKey = 5;
                        // zinvoll nur wenn andere Tastencode
                        // bevor 0xFF entdeckt wurde.
                        // Invalidate(Operate(pressed));
                        validateMask = Operate(pressed);
                    }
                    /* EasyCODE ) */
                    break;
                case 15 :
                    /* EasyCODE ( 0 
                    repeat key slow */
                    if ( lastKey == pressed )
                    {
                        if ( teing1.tz() )
                        {
                            // repeat slow
                            teing1.rsz();
                            teing1.sz(saf->slowtime);
                            //Invalidate(Operate(pressed));
                            validateMask = Operate(pressed);
                        }
                    }
                    else
                    {
                        // key is released
                        tastKey = 5;
                        // Invalidate(Operate(pressed));
                        validateMask = Operate(pressed);
                    }
                    /* EasyCODE ) */
                    break;
                case 0 :
                    tastKey = 1;
                    break;
                }
                /* EasyCODE ) */
                break;
            case S_AUS :
                /* EasyCODE ( 0 */
                switch ( spage )
                {
                case 1 :
                    /* EasyCODE = */
                case 2 :
                    /* EasyCODE ( 0 
                    Normal outputs */
                    if ( CheckOut(spage,auftragID[spage]) )
                    {
                        // fertig
                        auftragID[spage] = 0;
                        key = S_IO;
                        validateMask |= DISP_MASK_OUT;
                    }
                    /* EasyCODE ) */
                    break;
                case 4 :
                    /* EasyCODE ( 0 
                    Sonder Auftraege */
                    if ( pIO->GetSonder(auftragID[spage])->fertig() )
                    {
                        // fertig
                        auftragID[spage] = 0;
                        key = S_IO;
                        validateMask |= DISP_OUT;
                    }
                    /* EasyCODE ) */
                    break;
                default:
                    ;
                }
                /* EasyCODE ) */
                break;
            case S_PIEPS :
                key = S_IO;
                break;
            case S_FEHLER :
                eing.ioact = FALSE;
                error_res();
                key = S_START;
                break;
            case S_EXIT :
                break;
            }
            /* EasyCODE ( 0 
            Display refresh */
            if ( validateMask )
            {
                // refresh display
                Invalidate(validateMask);
                validateMask = 0;
            }
            /* EasyCODE ) */
            /* EasyCODE ) */
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseService::kaltstart */
/* EasyCODE F */
unsigned char KlasseService::kaltstart(void)
{
    if ( taste_los() == 0xff )
    {
        return E_RUHE;
    }
    else
    {
        eing_botsch1.senden(BV014);
        eing_botsch1.senden(pkaltstarts);
        eing_botsch1.senden(tz->eingkaltstart);
        switch ( getkey() )
        {
        case TENTER :
            saf->kaltstart = TRUE;
            if ( taste_los() == 0xff )
            {
                return E_RUHE;
            }
            else
            {
                eing_botsch1.senden(BV001);
                eing_botsch1.senden(tz->ausschalts);
                while ( TRUE )
                {
                }
            }
        case TSERVICE :
            /* EasyCODE = */
        case TESC :
            return E_SERV;
        case TEINGABE :
            return E_GST;
        case TENDE :
            return E_EXIT;
        case STOPANF :
            /* EasyCODE = */
        default:
            return E_RUHE;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseService::nullpunkte */
/* EasyCODE F */
unsigned char KlasseService::nullpunkte(void)
{
    unsigned char nullkey = 0;
    unsigned char servtemp;
    while ( TRUE )
    {
        switch ( nullkey )
        {
        case 0 :
            if ( nup.carlp && !nae.faedelreq && saf->carposok )
            {
                nullkey = 1;
                break;
            }
            else
            {
                koor.eing_err = 8;
                return E_RUHE;
            }
        case 1 :
            /* EasyCODE ( 0 
            BV016 anzeigen */
            if ( taste_los() == 0xff )
            {
                return E_RUHE;
            }
            else
            {
                eing_botsch1.senden(BV016);
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
                nullkey = 2;
                break;
            }
            /* EasyCODE ) */
        case 2 :
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
            case TSERVICE :
                eing.ioact = FALSE;
                return E_SERV;
            case TENTER :
                nullkey = 3;
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
                            nullkey = 10;
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
        case 3 :
            /* EasyCODE ( 0 
            BV016 anzeigen */
            if ( taste_los() == 0xff )
            {
                return E_RUHE;
            }
            else
            {
                eing_botsch1.senden(BV016);
                eing.ioact = TRUE;
                /* EasyCODE ( 0 
                Oberteil heben */
                y50u1.setAuftrag();
                while ( !y50u1.fertig() )
                {
                    if ( koor.eingstop() )
                    {
                        eing.ioact = FALSE;
                        nullkey = 10;
                        break;
                    }
                }
                /* EasyCODE ) */
                if ( nullkey == 10 )
                {
                }
                else
                {
                    if ( !eing.pup.nafupos() )
                    {
                        eing.ioact = FALSE;
                        nullkey = 10;
                    }
                    else
                    {
                        if ( !ver.move_sonst() )
                        {
                            eing.ioact = FALSE;
                            nullkey = 10;
                        }
                        else
                        {
                            nup.initkey = 0;
                            tnae1.rsz();
                            tnae1.sz(saf->initzt);
                            nae.homereq=TRUE;
                            nullkey = 4;
                        }
                    }
                }
                break;
            }
            /* EasyCODE ) */
        case 4 :
            /* EasyCODE ( 0 
            Schlitten positionieren */
            if ( nup.movinit() )
            {
                // nicht verriegelt
                nup.koorsoll [0].xbild = 0;
                nup.koorsoll [0].ybild = 0;
                nup.bitomo (&nup.koorsoll [0]);
                nup.carmov (FAST);
                nup.carlp = FALSE;
                nullkey = 6;
            }
            else
            {
                if ( tnae1.tz() )
                {
                    koor.nae_err = 13;
                    smx.stop();
                    smy.stop();
                    eing.ioact = FALSE;
                    nullkey = 10;
                }
                else
                {
                    if ( !ver.move_sonst() )
                    {
                        smx.stop();
                        smy.stop();
                        eing.ioact = FALSE;
                        nullkey = 10;
                    }
                }
            }
            break;
            /* EasyCODE ) */
        case 5 :
            /* EasyCODE ( 0 
            Position eingeben */
            if ( (servtemp=schlitten()) != 0 )
            {
                return servtemp;
            }
            else
            {
                nullkey = 3;
                break;
            }
            /* EasyCODE ) */
        case 6 :
            /* EasyCODE ( 0 
            auf Nadelnullpunkt fahren (0, 0) */
            if ( smx.ready() && smy.ready() )
            {
                // SmX und SmY fertig
                nup.workbuf.koorist = nup.koorsoll [0];
                saf->buf = nup.workbuf;
                nullkey = 5;
                /* EasyCODE ( 0 
                Oberteil absenken */
                eing.ioact = TRUE;
                y50u2.setAuftrag();
                while ( !y50u2.fertig() )
                {
                    if ( koor.eingstop() )
                    {
                        eing.ioact = FALSE;
                        nullkey = 10;
                        break;
                    }
                }
                /* EasyCODE ) */
            }
            else
            {
                if ( koor.notstop() )
                {
                    smx.stop();
                    smy.stop();
                    nae.homereq = TRUE;
                    saf->carposok = FALSE;
                    eing.ioact = FALSE;
                    nullkey = 10;
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
                nullkey = 1;
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
KlasseService::schlitten */
/* EasyCODE F */
unsigned char KlasseService::schlitten(void)
{
    unsigned char loopflag, tastmem;
    char xkor_temp, ykor_temp, deltax, deltay;
    FILE *f1;
    
    eing_botsch1.senden(BV019);
    /* EasyCODE ( 0 
    Schlittenposition eingeben */
    xkor_temp = saf->xkor;
    ykor_temp = saf->ykor;
    while ( TRUE )
    {
        while ( eing_botsch1.busycheck() )
        {
            ;
        }
        sprintf (eing.varstring, nullpkoord2s, xkor_temp, ykor_temp);
        eing_botsch1.senden (eing.varstring);
        loopflag = TRUE;
        while ( loopflag )
        {
            switch ( getkey() )
            {
            case TLINKS :
                loopflag = FALSE;
                if ( xkor_temp <= -100 )
                {
                    deltax = 0;
                    deltay = 0;
                }
                else
                {
                    xkor_temp --;
                    deltax = -1;
                    deltay = 0;
                }
                break;
            case TRECHTS :
                loopflag = FALSE;
                if ( xkor_temp >= 100 )
                {
                    deltax = 0;
                    deltay = 0;
                }
                else
                {
                    xkor_temp ++;
                    deltax = 1;
                    deltay = 0;
                }
                break;
            case TAUF :
                loopflag = FALSE;
                if ( ykor_temp >= 100 )
                {
                    deltax = 0;
                    deltay = 0;
                }
                else
                {
                    ykor_temp ++;
                    deltay = 1;
                    deltax = 0;
                }
                break;
            case TAB :
                loopflag = FALSE;
                if ( ykor_temp <= -100 )
                {
                    deltax = 0;
                    deltay = 0;
                }
                else
                {
                    ykor_temp --;
                    deltay = -1;
                    deltax = 0;
                }
                break;
            case TEINGABE :
            case TSERVICE :
            case TNULLP :
            case TENDE :
            case TREF :
            case TENTER :
                /* EasyCODE ( 0 
                Enter */
                tastmem = keyb.taste();
                saf->mnp_gueltig = 0;
                saf->xkor = xkor_temp;
                saf->ykor = ykor_temp;
                saf->mnp_gueltig = 0x55;
                // RAM-Nadelpos. ins FLASH schreiben
                f1 = fopen ("c:konf3590.bin", "r+b");
                fseek (f1,FF_MNP_GUE,SEEK_SET);
                fputc (0,f1);  // ungueltig
                fputc (saf->xkor,f1);
                fputc (saf->ykor,f1);
                fseek (f1,FF_MNP_GUE,SEEK_SET);
                fputc (0x55,f1);  // gueltig
                fclose (f1);
                nae.homereq = TRUE;
                saf->carposok = FALSE;
                switch ( tastmem )
                {
                case TEINGABE :
                    eing.ioact = FALSE;
                    return E_GST;
                case TSERVICE :
                    eing.ioact = FALSE;
                    return E_SERV;
                case TENDE :
                    eing.ioact = FALSE;
                    return E_EXIT;
                case TENTER :
                    eing.ioact = FALSE;
                    return E_SERV;
                case TREF :
                    eing.ioact = FALSE;
                    return E_REF2;
                default:
                    return 0;
                }
                /* EasyCODE ) */
            case STOPANF :
                eing.ioact = FALSE;
                /* EasyCODE - */
                error_res();
                return 0;
            case TESC :
                eing.ioact = FALSE;
                return E_SERV;
            default:
                break;
            }
        }
        nup.koorsoll[0].motorx = nup.workbuf.koorist.motorx + deltax;
        nup.koorsoll[0].motory = nup.workbuf.koorist.motory + deltay;
        nup.carmov(STANDARD);
        while ( !smx.ready() || !smy.ready() )
        {
            ;
        }
        nup.workbuf.koorist.motorx = nup.koorsoll[0].motorx;
        nup.workbuf.koorist.motory = nup.koorsoll[0].motory;
    }
    /* EasyCODE ) */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseService::hfussnullpunkt */
/* EasyCODE F */
unsigned char KlasseService::hfussnullpunkt(void)
{
    unsigned char nullkey = 0;
    hfkoord temppos;
    unsigned char retwert=E_RUHE;
    unsigned char loop = TRUE;
    while ( loop )
    {
        switch ( nullkey )
        {
        case 0 :
            /* EasyCODE ( 0 
            Grundstellung pruefen, Anzeige */
            if ( nup.carlp && !nae.faedelreq && saf->carposok )
            {
                if ( hfuss.posok )
                {
                    if ( saf->cutready )
                    {
                        eing_botsch1.senden(BV018);
                        hfuss.posinitkey = 0;
                        tnae1.rsz();
                        tnae1.sz(saf->initzt);
                        nae.homereq = TRUE;
                        nullkey = 1;
                    }
                    else
                    {
                        koor.eing_err = 33;
                        loop = FALSE;
                    }
                }
                else
                {
                    koor.eing_err = 34;
                    loop = FALSE;
                }
            }
            else
            {
                koor.eing_err = 8;
                loop = FALSE;
            }
            break;
            /* EasyCODE ) */
        case 1 :
            /* EasyCODE ( 0 
            Huepferfuss auf Initiator positionieren, Oberteil ab */
            switch ( nup.napos () )
            {
            case 0 :
                break;
            case 1 :
                koor.eing_err = 41;
                eing.errpar = nminf.fehler;
                eing.ioact = FALSE;
                nullkey = 10;
                break;
            default:
                if ( hfuss.posinit() )
                {
                    eing.ioact = TRUE;
                    y8.resAuftrag(); // Niederhalter auf
                    while ( !y8.fertig() )
                    {
                        if ( koor.eingstop() )
                        {
                            eing.ioact = FALSE;
                            nullkey = 10;
                            break;
                        }
                    }
                    if ( nullkey != 10 )
                    {
                        y50u2.setAuftrag();
                        nullkey = 2;
                        while ( !y50u2.fertig() )
                        {
                            if ( koor.eingstop() )
                            {
                                eing.ioact = FALSE;
                                nullkey = 10;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if ( tnae1.tz() )
                    {
                        koor.nae_err = 24;
                        nullkey = 10;
                    }
                }
                break;
            }
            break;
            /* EasyCODE ) */
        case 2 :
            /* EasyCODE ( 0 
            Niederhalter ab */
            y8.setAuftrag();
            nullkey = 3;
            while ( !y8.fertig() )
            {
                if ( koor.eingstop() )
                {
                    eing.ioact = FALSE;
                    nullkey = 10;
                    break;
                }
            }
            break;
            /* EasyCODE ) */
        case 3 :
            /* EasyCODE ( 0 
            auf Einstellposition fahren */
            temppos.pos = HFEINSTELL_POS;
            temppos.motor = HFEINSTELL_MOTOR;
            hfuss.hfmove (temppos.motor);
            y33.setAuftrag();
            /* EasyCODE - */
            // Stoffgegendruecker auf
            while ( !hfuss.ready() && !y33.fertig() )
            {
                if ( koor.eingstop() )
                {
                    eing.ioact = FALSE;
                    nullkey = 10;
                    break;
                }
            }
            if ( nullkey != 10 )
            {
                hfuss.set_istpos(temppos);
                nullkey = 4;
            }
            break;
            /* EasyCODE ) */
        case 4 :
            /* EasyCODE ( 0 
            Huepferfussposition einstellen */
            if ( (retwert=hfusseinstellen()) != 0 )
            {
                loop = FALSE;
            }
            else
            {
                retwert = E_RUHE;
                nullkey = 0;
            }
            break;
            /* EasyCODE ) */
        case 10 :
            /* EasyCODE ( 0 
            error_res */
            error_res();
            if ( taste_los() == 0xff )
            {
                loop = FALSE;
            }
            else
            {
                nullkey = 0;
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
KlasseService::hfusseinstellen */
/* EasyCODE F */
unsigned char KlasseService::hfusseinstellen(void)
{
    unsigned char loopflag, tastmem, retwert;
    unsigned char mainloop = TRUE;
    char kor_temp, delta;
    hfkoord tempkoord;
    FILE *f1;
    kor_temp = saf->hfkor;
    while ( mainloop )
    {
        while ( eing_botsch1.busycheck() )
        {
            ;
        }
        sprintf (eing.varstring, nullphfs, kor_temp);
        eing_botsch1.senden (eing.varstring);
        loopflag = TRUE;
        while ( loopflag )
        {
            switch ( getkey() )
            {
            case TAUF :
                loopflag = FALSE;
                if ( kor_temp >= 100 )
                {
                    delta = 0;
                }
                else
                {
                    nae.homereq = TRUE;
                    hfuss.posok = FALSE;
                    kor_temp ++;
                    delta = 1;
                }
                break;
            case TAB :
                loopflag = FALSE;
                if ( kor_temp <= -100 )
                {
                    delta = 0;
                }
                else
                {
                    nae.homereq = TRUE;
                    hfuss.posok = FALSE;
                    kor_temp --;
                    delta = -1;
                }
                break;
            case TEINGABE :
                /* EasyCODE = */
            case TSERVICE :
                /* EasyCODE = */
            case TENDE :
                /* EasyCODE = */
            case TENTER :
                /* EasyCODE ( 0 
                Enter */
                tastmem = keyb.taste();
                saf->hfp_gueltig = 0;
                saf->hfkor = kor_temp;
                saf->hfp_gueltig = 0x55;
                // Huepferfuss-Korrektur ins FLASH schreiben
                f1 = fopen ("c:kfportal.bin", "r+b");
                fseek (f1,FF_HF_GUE,SEEK_SET);
                fputc (0,f1);  // ungueltig
                fputc (saf->hfkor,f1);
                fseek (f1,FF_HF_GUE,SEEK_SET);
                fputc (0x55,f1);  // gueltig
                fclose (f1);
                mainloop = FALSE;
                loopflag = FALSE;
                switch ( tastmem )
                {
                case TEINGABE :
                    eing.ioact = FALSE;
                    retwert = E_GST;
                    break;
                case TSERVICE :
                    eing.ioact = FALSE;
                    retwert = E_SERV;
                    break;
                case TENDE :
                    eing.ioact = FALSE;
                    retwert = E_EXIT;
                    break;
                case TENTER :
                    eing.ioact = FALSE;
                    retwert = E_SERV;
                    break;
                default:
                    retwert = 0;
                    break;
                }
                break;
                /* EasyCODE ) */
            case STOPANF :
                eing.ioact = FALSE;
                error_res();
                loopflag = FALSE;
                mainloop = FALSE;
                retwert = 0;
                break;
            case TESC :
                eing.ioact = FALSE;
                loopflag = FALSE;
                mainloop = FALSE;
                retwert = E_SERV;
                break;
            default:
                break;
            }
        }
        if ( mainloop )
        {
            hfuss.hfmove(tempkoord.motor = hfuss.getpos().motor + delta);
            while ( !hfuss.ready() )
            {
            }
        }
    }
    y33.resAuftrag();
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseService::movehf */
/* EasyCODE F */
unsigned char KlasseService::movehf(void)
{
    unsigned char retwert,moveflag, vfindex;
    unsigned char loopflag = TRUE;
    unsigned char movehfkey = 0;
    hfkoord sollpos;
    if ( saf->hfp_gueltig == 0x55 )
    {
        while ( loopflag )
        {
            switch ( movehfkey )
            {
            case 0 :
                /* EasyCODE ( 0 
                movehfkey = 0,   Bildschirmvorlage anzeigen */
                if ( taste_los() == 0xff )
                {
                    retwert = E_RUHE;
                    loopflag = FALSE;
                }
                else
                {
                    if ( hfuss.posok )
                    {
                        eing_botsch1.senden(BV015);
                        sprintf (eing.varstring, hfusshubs, (float)saf->md.hub/10);
                        sprintf (eing.varstring1, hfussniveaus, (float)saf->md.posunten.pos/10);
                        strcat (eing.varstring, eing.varstring1);
                        sprintf (eing.varstring1, hfussposs, (float)hfuss.getpos().pos/10);
                        strcat (eing.varstring, eing.varstring1);
                        eing_botsch1.senden (eing.varstring);
                        eing.ioact = TRUE;
                        y33.setAuftrag();  // Stoffgegendruecker auf
                        y8.setAuftrag();   // Niederhalter ab
                        movehfkey = 1;
                    }
                    else
                    {
                        koor.eing_err = 34;
                        retwert = E_RUHE;
                        loopflag = FALSE;
                    }
                }
                break;
                /* EasyCODE ) */
            case 1 :
                /* EasyCODE ( 0 
                movehfkey = 1,   Tasten abfragen ---> verzweigen */
                if ( koor.eingstop() )
                {
                    retwert = E_RUHE;
                    loopflag = FALSE;
                }
                else
                {
                    if ( y8.fertig() && y33.fertig() )
                    {
                        switch ( getkey() )
                        {
                        case THFAB :
                            /* EasyCODE ( 0 
                            Huepferfuss ab, Einzelpuls */
                            if ( hfuss.getpos().pos > 0 )
                            {
                                sollpos.pos = hfuss.getpos().pos - 1;
                                moveflag = TRUE;
                            }
                            else
                            {
                                moveflag = FALSE;
                            }
                            break;
                            /* EasyCODE ) */
                        case THFAUF :
                            /* EasyCODE ( 0 
                            Huepferfuss auf, Einzelpuls */
                            if ( hfuss.getpos().pos < MAX_POS )
                            {
                                sollpos.pos = hfuss.getpos().pos + 1;
                                moveflag = TRUE;
                            }
                            else
                            {
                                moveflag = FALSE;
                            }
                            break;
                            /* EasyCODE ) */
                        case THFHUB :
                            /* EasyCODE ( 0 
                            auf Hub oben - Position fahren */
                            sollpos.pos = saf->md.posunten.pos + saf->md.hub;
                            hfuss.postomo (&sollpos);
                            hfuss.hfmove (sollpos.motor);
                            moveflag = FALSE;
                            movehfkey = 6;
                            break;
                            /* EasyCODE ) */
                        case THFNIVEAU :
                            /* EasyCODE ( 0 
                            auf unteres Huepferfussniveau fahren */
                            sollpos = saf->md.posunten;
                            hfuss.hfmove (sollpos.motor);
                            moveflag = FALSE;
                            movehfkey = 6;
                            break;
                            /* EasyCODE ) */
                        case TENTER1 :
                            /* EasyCODE ( 0 
                            Hub uebernehmen */
                            moveflag = FALSE;
                            if ( hfuss.getpos().pos >= saf->md.posunten.pos )
                            {
                                saf->md.hub = hfuss.getpos().pos - saf->md.posunten.pos;
                                hfuss.akt_hub = saf->md.hub;
                                sprintf (eing.varstring, hfusshubs, (float)saf->md.hub/10);
                                eing_botsch1.senden (eing.varstring);
                            }
                            else
                            {
                                eing_botsch1.senden(BELLBELL);
                            }
                            break;
                            /* EasyCODE ) */
                        case TENTER2 :
                            /* EasyCODE ( 0 
                            Niveau uebernehmen */
                            if ( hfuss.getpos().pos <= HFLEVEL_MAX )
                            {
                                moveflag = FALSE;
                                saf->md.posunten = hfuss.getpos();
                                hfuss.akt_posunten = saf->md.posunten;
                                sprintf (eing.varstring, hfussniveaus, (float)saf->md.posunten.pos/10);
                                eing_botsch1.senden (eing.varstring);
                            }
                            else
                            {
                                eing_botsch1.senden(BELLBELL);
                            }
                            break;
                            /* EasyCODE ) */
                        case TEINGABE :
                            /* EasyCODE ( 0 
                            Taste Eingabe */
                            nae.homereq = TRUE;
                            retwert = E_RUHE;
                            moveflag = FALSE;
                            loopflag = FALSE;
                            break;
                            /* EasyCODE ) */
                        case TENDE :
                            /* EasyCODE ( 0 
                            Taste Ende */
                            nae.homereq = TRUE;
                            retwert = E_EXIT;
                            moveflag = FALSE;
                            loopflag = FALSE;
                            break;
                            /* EasyCODE ) */
                        default:
                            moveflag = FALSE;
                            break;
                        }
                    }
                    else
                    {
                        moveflag = FALSE;
                    }
                    if ( moveflag )
                    {
                        if ( loopflag )
                        {
                            /* EasyCODE ( 0 
                            Puls ausfuehren */
                            vfindex = 0;
                            teing1.rsz();
                            teing1.sz(saf->prodelzt);
                            hfuss.postomo (&sollpos);
                            hfuss.hfmove(sollpos.motor);
                            movehfkey = 2;
                            /* EasyCODE ) */
                        }
                    }
                }
                break;
                /* EasyCODE ) */
            case 2 :
                /* EasyCODE ( 0 
                movehfkey = 2,   Warten bis Einzelpuls fertig */
                if ( hfuss.ready() )
                {
                    hfuss.set_istpos(sollpos);
                    sprintf (eing.varstring, hfussposs, (float)hfuss.getpos().pos/10);
                    eing_botsch1.senden (eing.varstring);
                    movehfkey = 3;
                }
                break;
                /* EasyCODE ) */
            case 3 :
                /* EasyCODE ( 0 
                movehfkey = 3,   nach Zeit langsames Dauerfahren starten */
                if ( koor.eingstop() )
                {
                    retwert = E_RUHE;
                    loopflag = FALSE;
                }
                else
                {
                    if ( teing1.tz() )
                    {
                        teing1.rsz();
                        teing1.sz(saf->profastzt);
                        teing2.rsz();
                        teing2.sz(5);
                        vfindex = 1;
                        movehfkey = 4;
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
                            movehfkey = 1;
                            break;
                        }
                    }
                }
                break;
                /* EasyCODE ) */
            case 4 :
                /* EasyCODE ( 0 
                movehfkey = 4,   Dauerfahren solange Taste gedrueckt */
                if ( koor.eingstop() )
                {
                    movehfkey = 5;
                }
                else
                {
                    if ( teing1.tz() && (vfindex==1) )
                    {
                        vfindex = 2;
                    }
                    if ( hfuss.ready() && teing2.tz() && !eing_botsch1.busycheck() )
                    {
                        /* EasyCODE ( 0 
                        Tasten abfragen, Verfahrweg bestimmen */
                        switch ( keyb.taste() )
                        {
                        case THFAB :
                            switch ( vfindex )
                            {
                            case 0 :
                                sollpos.pos = hfuss.getpos().pos - 1;
                                break;
                            case 1 :
                                sollpos.pos = hfuss.getpos().pos - 1;
                                break;
                            default:
                                sollpos.pos = hfuss.getpos().pos - 5;
                                break;
                            }
                            if ( sollpos.pos < 0 )
                            {
                                sollpos.pos = 0;
                            }
                            break;
                        case THFAUF :
                            switch ( vfindex )
                            {
                            case 0 :
                                sollpos.pos = hfuss.getpos().pos + 1;
                                break;
                            case 1 :
                                sollpos.pos = hfuss.getpos().pos + 1;
                                break;
                            default:
                                sollpos.pos = hfuss.getpos().pos + 5;
                                break;
                            }
                            if ( sollpos.pos > MAX_POS )
                            {
                                sollpos.pos = MAX_POS;
                            }
                            break;
                        default:
                            movehfkey = 5;
                            break;
                        }
                        /* EasyCODE ) */
                        if ( movehfkey == 4 )
                        {
                            hfuss.postomo (&sollpos);
                            hfuss.hfmove(sollpos.motor);
                            hfuss.set_istpos (sollpos);
                            sprintf (eing.varstring, hfussposs, (float)hfuss.getpos().pos/10);
                            eing_botsch1.senden (eing.varstring);
                            teing2.rsz();
                            teing2.sz(5);
                        }
                    }
                }
                break;
                /* EasyCODE ) */
            case 5 :
                /* EasyCODE ( 0 
                movehfkey = 5,   warten bis Huepferfuss fertig nachdem
                                 keine Taste gedrueckt war oder Stopanforderung */
                if ( hfuss.ready() )
                {
                    sprintf (eing.varstring, hfussposs, (float)hfuss.getpos().pos/10);
                    eing_botsch1.senden (eing.varstring);
                    movehfkey = 1;
                }
                break;
                /* EasyCODE ) */
            case 6 :
                /* EasyCODE ( 0 
                movehfkey = 6,   Warten bis Position erreicht */
                if ( hfuss.ready() )
                {
                    hfuss.set_istpos(sollpos);
                    sprintf (eing.varstring, hfussposs, (float)hfuss.getpos().pos/10);
                    eing_botsch1.senden (eing.varstring);
                    movehfkey = 1;
                }
                break;
                /* EasyCODE ) */
            default:
                break;
            }
        }
    }
    else
    {
        koor.eing_err = 34;
        retwert = E_RUHE;
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
void KlasseService::Init(KlasseIO* const pio) */
/* EasyCODE F */
void KlasseService::Init(KlasseIO* const pio)
{
    pIO = pio;
    spage = 1;
    sonder = 1;
    maske = 0x0001;
    dispMode = MODE_NORMAL;
    lastKey = 0xFF;
    setmem(auftragID,sizeof (auftragID),0);
    index = 0; // 0..15
    tastKey = 1;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Invalidate(char refresh) */
/* EasyCODE F */
char KlasseService::Invalidate(char refresh)
{
    // Diese Funktion aktualisiert die LCD
    // fuer jedes Bildschirmteil wurde in
    // 'refresh' ein bit vorgeseh.Das heisst,nur
    // die Teilen von Bildschirm werden aktualisiert, 
    // die entsprechnden Bits in 'refresh' gesetzt sind.
    char fstr[50];
    switch ( dispMode )
    {
    case MODE_NORMAL :
        /* EasyCODE ( 0 
        Background (BV) */
        if ( refresh & DISP_BV )
        {
            // Display the main service display
            eing_botsch1.senden(BV009);
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Diplay titel */
        if ( refresh & DISP_TITEL )
        {
            // Die NumerierungZeile 
            GetTitleStr(fstr);
            sprintf(str,ESCS"=002100%s",fstr);
            eing_botsch1.senden(str);
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Display inputs */
        if ( refresh & DISP_IN )
        {
        }
        // Die Eingaenge
        GetInputStr(spage,fstr);
        sprintf(str,ESCS"=018115%s",fstr);
        eing_botsch1.senden(str);
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Display outputs */
        if ( refresh & DISP_OUT )
        {
            // Die Ausgaenge
            GetOutputStr(spage,fstr);
            sprintf(str,ESCS"=018130%s",fstr);
            eing_botsch1.senden(str);
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Display Pfeil position */
        if ( refresh & DISP_PFEIL )
        {
            // Die Pfeil
            GetPfeilStr(fstr,index);
            sprintf(str,ESCS"=066145%s",fstr);
            eing_botsch1.senden(str);
        }
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Display type/Mask line */
        if ( refresh & DISP_MASK )
        {
            // Aus dieser Zeile ist zu erkennen:
            // ob ein Ausgang ein Sonderausgang ist
            // ob ein Auftrag fuer Ausgang besteht bzw. bearbeitet wurde.
            GetSRStr(spage,fstr);
            sprintf(str,ESCS"=002160%s",fstr);
            eing_botsch1.senden(str);
        }
        /* EasyCODE ) */
        break;
    case MODE_SONDER :
        break;
    default:
        break;
    }
    return refresh;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
CheckPage() */
/* EasyCODE F */
char KlasseService::CheckPage()
{
    // ob canNo und maske gultig sind.
    // wenn nicht, dann reset to first page
    unsigned char ix;
    switch ( spage )
    {
    case 1 :
        /* EasyCODE < */
        // Can1
        /* EasyCODE > */
    case 2 :
        /* EasyCODE < */
        // Can2
        /* EasyCODE > */
    case 3 :
        /* EasyCODE ( 0 
        eventuelle sonder positionen ueberspringt */
        /* EasyCODE < */
        // Can3
        ix = GetNextIndex();// einmal vor und einmal zurueck
        /* EasyCODE > */
        if ( index == ix )
        {
        }
        else
        {
            /* EasyCODE < */
            index = ix;
            index = GetPrevIndex();// eventuelle sonder positionen ueberspringt
            /* EasyCODE > */
        }
        /* EasyCODE ) */
        break;
    case 4 :
        /* EasyCODE ( 0 
        ueberpruefen ob index-Zeiger an falsche Stelle steht */
        if ( index >= GetSonderAnz() )
        {
            index = GetSonderAnz()-1;
        }
        /* EasyCODE ) */
        break;
    default:
        ;
    }
    return TRUE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetInputStr(unsigned char can,char* const inStr) */
/* EasyCODE F */
void KlasseService::GetInputStr(unsigned char can,char* const inStr)
{
    // can is the Can Module number
    // inStr is the address of string to be filled
    // inStr must >=18 bytes 
    int anz = GetInAnz();
    unsigned int maskCtrl = 0xFFFF;
    unsigned int inMask;
    unsigned char count = 0;
    unsigned int result = 0;
    char fstr[50];
    switch ( spage )
    {
    case 1 :
        /* EasyCODE < */
        // Can1
        /* EasyCODE > */
    case 2 :
        /* EasyCODE < */
        // Can2
        /* EasyCODE > */
    case 3 :
        /* EasyCODE ( 0 
        CAN Pages */
        // CAN 1..3
        anz = GetOutAnz();
        strcpy(inStr," IN  :");
        for ( int i = 0 ; i < anz ; i++ )
        {
            if ( pIO->GetEingang(i)->GetCanNr() == can )
            {
                inMask = pIO->GetEingang(i)->GetMaske();
                if ( inMask & maskCtrl )
                {
                    /* EasyCODE < */
                    maskCtrl |= inMask;// reset this mask bit
                    /* EasyCODE > */
                    if ( pIO->GetEingang(i)->getZustand() )
                    {
                        result |= inMask;
                    }
                    else
                    {
                        /* EasyCODE < */
                        // input is low
                        /* EasyCODE > */
                    }
                    count++;
                }
                else
                {
                    // Error: mask repeat
                    strcpy(inStr,"* MASK PROBLEM *");
                    return ;
                }
            }
            if ( count == 16 )
            {
                break;
            }
        }
        FormatStr(result,fstr);
        strcat(inStr,fstr);
        /* EasyCODE ) */
        break;
    case 4 :
        /* EasyCODE < */
        strcpy(inStr,"                          "); // 26 spaces
        /* EasyCODE > */
        break;
    default:
        ;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetTitleStr(char* const inStr) */
/* EasyCODE F */
void KlasseService::GetTitleStr(char* const fstr)
{
    /* EasyCODE < */
    // inStr is the address of string to be filled
    // inStr must >=30 bytes
    /* EasyCODE > */
    switch ( spage )
    {
    case 1 :
        /* EasyCODE < */
        // CAN 1
        /* EasyCODE > */
    case 2 :
        /* EasyCODE < */
        // CAN 2
        /* EasyCODE > */
    case 3
         // CAN 1..3
    :
        sprintf(fstr,"DIGITAL 1......8 9......16");
        break;
    case 4 :
        /* EasyCODE < */
        // Sonder Auftraege
        /* EasyCODE > */
        /* EasyCODE ( 0 
        Sonder Auftraege */
        int anz = ANZAHL_SONDERAUFTRAEGE;
        char* head = " SONDER ";
        char tempstr[5];
        strcpy(fstr,head);
        for ( int i = 1 ; i <= anz ;i++ )
        {
            switch ( i )
            {
            case 1 :
                strcat(fstr,"1");
                break;
            case 8 :
                strcat(fstr,"8 ");
                break;
            case 16 :
                strcat(fstr,"16");
                break;
            case ANZAHL_SONDERAUFTRAEGE :
                sprintf(tempstr,"%u",anz);
                strcat(fstr,tempstr);
                break;
            default:
                strcat(fstr,".");
            }
        }
        for ( i = anz ; i <= 17 ; i++ )
        {
            /* EasyCODE < */
            strcat(fstr," ");    // blank extended
            /* EasyCODE > */
        }
        /* EasyCODE ) */
        break;
    default:
        sprintf(fstr,ESCS"                            ");
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetOutputStr(unsigned char can,char* const outStr) */
/* EasyCODE F */
void KlasseService::GetOutputStr(unsigned char page,char* const outStr)
{
    // can is the Can Module number
    // outStr is the address of string to be filled
    // inStr must >= 18 bytes 
    int anz;
    unsigned int maskCtrl = 0xFFFF;
    unsigned int outMask;
    unsigned char count = 0;
    unsigned int level = 0;
    unsigned int freiMask = 0xFFFF;
    unsigned int sonderMask = 0;
    int i;
    char fstr[50];
    switch ( spage )
    {
    case 1 :
        /* EasyCODE < */
        // Can1
        /* EasyCODE > */
    case 2 :
        /* EasyCODE < */
        // Can2
        /* EasyCODE > */
    case 3 :
        /* EasyCODE ( 0 
        CAN pages */
        anz = GetOutAnz();
        strcpy(outStr,"OUT  :");
        for ( i = 0 ; i < anz ; i++ )
        {
            if ( pIO->GetAusgang(i)->GetCanNr() == page )
            {
                outMask = pIO->GetAusgang(i)->GetMaske();
                if ( outMask & maskCtrl )
                {
                    /* EasyCODE ( 0 
                    read output */
                    /* EasyCODE < */
                    maskCtrl &= ~outMask;// reset this mask bit
                    /* EasyCODE > */
                    if ( pIO->GetAusgang(i)->IsFrei() )
                    {
                    }
                    else
                    {
                        // Not free. Reset free mask bit
                        freiMask &= ~outMask;
                        if ( pIO->GetAusgang(i)->IsSonder() )
                        {
                            sonderMask |= outMask;
                        }
                    }
                    if ( pIO->GetAusgang(i)->getZustand() )
                    {
                        // output is high
                        level |= outMask;
                    }
                    else
                    {
                        /* EasyCODE < */
                        // input is low
                        /* EasyCODE > */
                    }
                    count++;
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    MASK Error */
                    // Error: mask repeat
                    strcpy(outStr,"* MASK PROBLEM *");
                    return ;
                    /* EasyCODE ) */
                }
            }
            if ( count == 16 )
            {
                break;
            }
        }
        /* EasyCODE < */
        FormatStr(level,fstr);//,freiMask,'X');
        strcat(outStr,fstr); // make string
        /* EasyCODE > */
        /* EasyCODE ) */
        break;
    case 4 :
        /* EasyCODE ( 0 
        Sonder page */
        // Sonderauftraege
        anz = GetSonderAnz();
        strcpy(outStr," S   :");
        for ( i = 0; i < anz ; i++ )
        {
            if ( pIO->GetSonder(i) )
            {
                if ( pIO->GetSonder(i)->fertig() )
                {
                    strcat(outStr,"0");
                }
                else
                {
                    strcat(outStr,"1");
                }
                if ( i==7 )
                {
                    /* EasyCODE < */
                    strcat(outStr," ");// space after 8.th
                    /* EasyCODE > */
                }
            }
            else
            {
                /* EasyCODE < */
                // auftrag not exist
                /* EasyCODE > */
            }
        }
        for ( i = anz ; i <= 16 ; i++ )
        {
            /* EasyCODE < */
            strcat(outStr," ");    // blank extended
            /* EasyCODE > */
        }
        /* EasyCODE ) */
        break;
    default:
        ;
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetSRStr(unsigned char can,char* const curStr,pos) */
/* EasyCODE F */
void KlasseService::GetSRStr(unsigned char can,char* curStr)
{
    // can is the Can Module number
    // outStr is the address of string to be filled
    // inStr must >= 30 bytes 
    int anz = GetOutAnz();
    unsigned int maskCtrl = 0xFFFF;
    unsigned int outMask;
    unsigned char count = 0;
    //unsigned int level = 0;
    //unsigned int freiMask = 0xFFFF;
    unsigned int sonderMask = 0;
    unsigned int setMask = 0;
    unsigned int resetMask = 0;
    switch ( spage )
    {
    case 1 :
        /* EasyCODE < */
        // CAN 1
        /* EasyCODE > */
    case 2 :
        /* EasyCODE < */
        // CAN 2
        /* EasyCODE > */
    case 3 :
        /* EasyCODE ( 0 
        Can pages */
        // CAN 1..3
        strcpy(curStr,"SET/RES:");
        for ( int i = 0 ; i < anz ; i++ )
        {
            if ( pIO->GetAusgang(i)->GetCanNr() == can )
            {
                outMask = pIO->GetAusgang(i)->GetMaske();
                if ( outMask & maskCtrl )
                {
                    /* EasyCODE ( 0 
                    set, reset, sonder ? */
                    /* EasyCODE < */
                    maskCtrl &= ~outMask;// reset this mask bit
                    /* EasyCODE > */
                    if ( pIO->GetAusgang(i)->IsSonder() )
                    {
                        sonderMask |= outMask;
                    }
                    else
                    {
                        switch ( pIO->GetAusgang(i)->getAuftrag() )
                        {
                        case SET :
                            setMask |= outMask;
                            break;
                        case RES :
                            resetMask |= outMask;
                            break;
                        case READY :
                            break;
                        default:
                            ;
                        }
                    }
                    count++;
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    Mask repeat */
                    // Error: mask repeat
                    strcpy(curStr,"* MASK PROBLEM *");
                    return ;
                    /* EasyCODE ) */
                }
            }
            if ( count == 16 )
            {
                break;
            }
        }
        unsigned int roller = 0x0001;
        /* EasyCODE ( 0 
        Make string */
        for ( i = 0; i < 17 ; i++ )
        {
            if ( i==8 )
            {
                /* EasyCODE < */
                strcat(curStr," ");// space between two bytes
                /* EasyCODE > */
            }
            else
            {
                /* EasyCODE ( 0 
                Auswertung */
                if ( (roller & sonderMask) == roller )
                {
                    strcat(curStr,"S");
                }
                else
                {
                    if ( (setMask & roller) == roller )
                    {
                        strcat(curStr,"1");
                    }
                    else
                    {
                        if ( (resetMask & roller) == roller )
                        {
                            strcat(curStr,"0");
                        }
                        else
                        {
                            strcat(curStr,"-");
                        }
                    }
                }
                /* EasyCODE ) */
                roller*=2;
            }
        }
        /* EasyCODE ) */
        /* EasyCODE ) */
        break;
    case 4 :
        /* EasyCODE < */
        strcpy(curStr,"                          "); // 26 spaces
        /* EasyCODE > */
        break;
    default:
        ;
    }
    return ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Auftrag(unsigned char can,unsigned char n) */
/* EasyCODE F */
char KlasseService::Auftrag(unsigned char can,unsigned char num,char set)
{
    // can is the Can Module number
    // num is the output number 0..15 (mask bit number)
    // if set == TRUE -> Auftrag setzen
    // if set == FALSE -> Auftrag ruecksetzen
    int anz = GetOutAnz();
    unsigned int outMask = 0x0001;
    switch ( spage )
    {
    case 1 :
        /* EasyCODE < */
        // CAN 1
        /* EasyCODE > */
    case 2 :
        /* EasyCODE < */
        // CAN 2
        /* EasyCODE > */
    case 3 :
        /* EasyCODE ( 0 
        Auftrag set/reset */
        /* EasyCODE < */
        // CAN 1..3
        /* EasyCODE > */
        if ( num > 15 )
        {
            /* EasyCODE < */
            // output number not acceptable
            /* EasyCODE > */
            return FALSE;
        }
        else
        {
            // prepare the right mask bit
            outMask <<= num;
        }
        for ( int i = 0 ; i < anz ; i++ )
        {
            if ( pIO->GetAusgang(i)->GetCanNr() == can )
            {
                if ( outMask == pIO->GetAusgang(i)->GetMaske() )
                {
                    /* EasyCODE ( 0 
                    Auftrag bearbeiten */
                    if ( set )
                    {
                        pIO->GetAusgang(i)->setAuftrag();
                    }
                    else
                    {
                        pIO->GetAusgang(i)->resAuftrag();
                    }
                    /* EasyCODE ) */
                    auftragID[spage] = outMask;
                    return TRUE;
                }
            }
            else
            {
                /* EasyCODE < */
                // Not in this CAN
                /* EasyCODE > */
            }
        }
        /* EasyCODE ) */
        break;
    case 4 :
        /* EasyCODE ( 0 
        Set Sonderauftrag */
        if ( set )
        {
            pIO->GetSonder(num)->setAuftrag();
            auftragID[spage] = num;
        }
        else
        {
            pIO->GetSonder(num)->resAuftrag();
            auftragID[spage] = 0;
        }
        return TRUE;
        /* EasyCODE ) */
    default:
        return FALSE;
    }
    return FALSE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
CheckOut(unsigned char can,unsigned char index) */
/* EasyCODE F */
char KlasseService::CheckOut(unsigned char page,unsigned int outMask)
{
    // page is the Can/page number
    // outMask is the output mask (only one bit could be set)
    // This function resets the coresponding flag if Auftrag is fertig
    // and return TRUE.
    int anz ;
    /* EasyCODE - */
    anz = GetOutAnz();
    for ( int i = 0 ; i < anz ; i++ )
    {
        if ( pIO->GetAusgang(i)->GetCanNr() == page )
        {
            if ( outMask == pIO->GetAusgang(i)->GetMaske() )
            {
                if ( pIO->GetAusgang(i)->fertig() )
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
        }
    }
    return FALSE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetPfeilStr(char* fstr,unsigned char pos) */
/* EasyCODE F */
void KlasseService::GetPfeilStr(char* fstr,unsigned char pos)
{
    // fills the fstr with Pfeil string. fstr >= 30 bytes.
    // pos is the pfeil position
    unsigned int roller = 0x0001;
    for ( int i = 0; i < 17 ; i++ )
    {
        if ( i==8 )
        {
            // space between two bytes
            *fstr = ' ';
            pos++;
        }
        else
        {
            if ( i == pos )
            {
                // Pfeil up char
                *fstr = 1;
                fstr++;
                *fstr = 0x18;
            }
            else
            {
                *fstr = ' ';
            }
            roller*=2;
        }
        fstr++;
    }
    /* EasyCODE < */
    *fstr = 0; // Null terminator
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
FormatStr(unsigned char mask,char* str) */
/* EasyCODE F */
void KlasseService::FormatStr(unsigned int levelMask,char* fstr,
unsigned int freiMask,char sp )
{
    // convert mask to string format and load to given str pointer
    // example:       mask = 0x8841 
    //            freiMask = 0x4802   -->  str = "1x00x000 010000x1" 
    
    // fstr size must >= 30 bytes
    unsigned int roller = 0x0001;
    for ( int i = 0; i < 17 ; i++ )
    {
        if ( i==8 )
        {
            /* EasyCODE < */
            *fstr = ' ';// space between two bytes
            /* EasyCODE > */
        }
        else
        {
            /* EasyCODE ( 0 
            Auswertung */
            if ( (roller & freiMask) == roller )
            {
                *fstr = sp;
            }
            else
            {
                if ( (roller & levelMask) == roller )
                {
                    *fstr = '1';
                }
                else
                {
                    *fstr = '0';
                }
            }
            /* EasyCODE ) */
            roller*=2;
        }
        fstr++;
    }
    /* EasyCODE < */
    *fstr = 0; // Null terminated
    /* EasyCODE > */
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Operate(command) */
/* EasyCODE F */
unsigned char KlasseService::Operate(unsigned char command)
{
    // process the given command (as pressed key)
    unsigned char result = FALSE;
    switch ( command )
    {
    case TCAN1 :
        /* EasyCODE ( 0 
        Page: Can1 */
        switch ( spage )
        {
        case 1 :
            break;
        case 2 :
            sprintf(str,touchinv,TCAN2S); 
            eing_botsch1.senden(str);
            sprintf(str,touchinv,TCAN1S);
            eing_botsch1.senden(str);
            result |= DISP_MOFTI;
            break;
        case 4 :
            sprintf(str,touchinv,TSER_SONDERS);
            eing_botsch1.senden(str);
            sprintf(str,touchinv,TCAN1S);
            eing_botsch1.senden(str);
            result |= DISP_MOFTI;
            break;
        }
        spage = 1;
        CheckPage();
        /* EasyCODE ) */
        break;
    case TCAN2 :
        /* EasyCODE ( 0 
        Page: Can2 */
        switch ( spage )
        {
        case 1 :
            sprintf(str,touchinv,TCAN1S); 
            eing_botsch1.senden(str);
            sprintf(str,touchinv,TCAN2S);
            eing_botsch1.senden(str);
            result |= DISP_MOFTI;
            break;
        case 2 :
            break;
        case 4 :
            sprintf(str,touchinv,TSER_SONDERS);
            eing_botsch1.senden(str);
            sprintf(str,touchinv,TCAN2S);
            eing_botsch1.senden(str);
            result |= DISP_MOFTI;
            break;
        }
        spage = 2;
        CheckPage();
        /* EasyCODE ) */
        break;
    case TSER_SONDER :
        /* EasyCODE ( 0 
        Page: sonder */
        if ( GetSonderAnz() )
        {
            switch ( spage )
            {
            case 1 :
                sprintf(str,touchinv,TCAN1S); 
                eing_botsch1.senden(str);
                sprintf(str,touchinv,TSER_SONDERS);
                eing_botsch1.senden(str);
                result |= DISP_MOFTI;
                break;
            case 2 :
                sprintf(str,touchinv,TCAN2S); 
                eing_botsch1.senden(str);
                sprintf(str,touchinv,TSER_SONDERS);
                eing_botsch1.senden(str);
                result |= DISP_MOFTI;
                break;
            case 4 :
                break;
            }
            spage = 4;
            CheckPage();
        }
        /* EasyCODE ) */
        break;
    case TPLUS :
        /* EasyCODE < */
        // Auftrag setzen
        /* EasyCODE > */
        if ( Auftrag(spage,index,TRUE) )
        {
            result |= DISP_MASK_OUT; // disp
            key = S_AUS;
            eing.ioact = TRUE;
        }
        break;
    case TMINUS :
        /* EasyCODE < */
        // Auftrag ruecksetzen
        /* EasyCODE > */
        if ( Auftrag(spage,index,FALSE) )
        {
            result |= DISP_MASK_OUT;
            key = S_AUS;
            eing.ioact = TRUE;
        }
        break;
    case TLEFT :
        index = GetPrevIndex();
        /* EasyCODE - */
        result |= DISP_PFEIL;
        break;
    case TRIGHT :
        index = GetNextIndex();
        /* EasyCODE - */
        result |= DISP_PFEIL;
        break;
    case TKALTSTART :
        break;
    default:
        result = FALSE;
    }
    return result;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetInAnz()const */
/* EasyCODE F */
unsigned char KlasseService::GetInAnz()const
{
    /* EasyCODE < */
    // return: then number of inputs
    /* EasyCODE > */
    if ( pIO )
    {
        return pIO->GetInAnz();
    }
    else
    {
        return 0;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetOutAnz()const */
/* EasyCODE F */
unsigned char KlasseService::GetOutAnz()const
{
    /* EasyCODE < */
    // return: the number of outputs
    /* EasyCODE > */
    if ( pIO )
    {
        return pIO->GetOutAnz();
    }
    else
    {
        return 0;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetSonderAnz()const */
/* EasyCODE F */
unsigned char KlasseService::GetSonderAnz()const
{
    /* EasyCODE < */
    // return: the number of sonder outputs
    /* EasyCODE > */
    if ( pIO )
    {
        return pIO->GetSonderAnz();
    }
    else
    {
        return 0;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned char KlasseService::GetNextIndex() */
/* EasyCODE F */
unsigned char KlasseService::GetNextIndex()
{
    // finds next Pfeil position (next output)
    // and updates the 'index' member variable
    // Die Sonderausgaenge werden uebersprungen.
    int anz;
    unsigned int outMask;
    unsigned char ix = 0;
    unsigned int nextMask = 0x0001;
    switch ( spage )
    {
    case 1 :
        /* EasyCODE < */
        // CAN 1
        /* EasyCODE > */
    case 2 :
        /* EasyCODE < */
        // CAN 2
        /* EasyCODE > */
    case 3 :
        /* EasyCODE ( 0 
        Normal Auftraege */
        // CAN 1..3
        nextMask <<= index;
        anz = GetOutAnz();
        if ( nextMask == 0x8000 )
        {
            /* EasyCODE < */
            // last position
            /* EasyCODE > */
        }
        else
        {
            /* EasyCODE < */
            nextMask *= 2; // shift left
            /* EasyCODE > */
            for ( int i = 0 ; i < anz ; i++ )
            {
                if ( pIO->GetAusgang(i)->GetCanNr() == spage )
                {
                    outMask = pIO->GetAusgang(i)->GetMaske();
                    if ( (outMask & nextMask) == nextMask )
                    {
                        if ( pIO->GetAusgang(i)->IsSonder() )
                        {
                            if ( nextMask == 0x8000 )
                            {
                                /* EasyCODE < */
                                // last position
                                /* EasyCODE > */
                                break;
                            }
                            else
                            {
                                // shift left
                                nextMask *= 2;
                                i = 0;
                            }
                        }
                        else
                        {
                            /* EasyCODE < */
                            // found
                            /* EasyCODE > */
                            while ( nextMask )
                            {
                                // shift right
                                nextMask/=2; 
                                ix++;
                            }
                            return ix-1;
                        }
                    }
                }
            }
        }
        /* EasyCODE ) */
        break;
    case 4 :
        /* EasyCODE ( 0 
        Sonder Auftraege */
        /* EasyCODE < */
        anz = GetSonderAnz(); // max sonderauftraege = 16
        /* EasyCODE > */
        if ( index > (anz-1) )
        {
            return anz-1;
        }
        else
        {
            if ( index == (anz-1) )
            {
            }
            else
            {
                return index+1;
            }
        }
        /* EasyCODE ) */
        break;
    default:
        return index;
    }
    return index;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
unsigned char KlasseService::GetPrevIndex() */
/* EasyCODE F */
unsigned char KlasseService::GetPrevIndex()
{
    // finds prev. Pfeil position (prev. output)
    // and updates the 'index' member variable
    // Die Sonderausgaenge werden uebersprungen.
    int anz ;
    unsigned int outMask;
    unsigned char ix = 0;
    unsigned int prevMask = 0x0001;
    switch ( spage )
    {
    case 1 :
        /* EasyCODE < */
        // CAN 1
        /* EasyCODE > */
    case 2 :
        /* EasyCODE < */
        // CAN 2
        /* EasyCODE > */
    case 3 :
        /* EasyCODE ( 0 
        Normal Auftraege */
        // CAN 3
        anz = GetOutAnz();
        prevMask <<= index;
        if ( prevMask == 0x0001 )
        {
            /* EasyCODE < */
            // last position
            /* EasyCODE > */
        }
        else
        {
            /* EasyCODE < */
            prevMask /= 2; // shift right
            /* EasyCODE > */
            for ( int i = 0 ; i < anz ; i++ )
            {
                if ( pIO->GetAusgang(i)->GetCanNr() == spage )
                {
                    outMask = pIO->GetAusgang(i)->GetMaske();
                    if ( (outMask & prevMask) == prevMask )
                    {
                        if ( pIO->GetAusgang(i)->IsSonder() )
                        {
                            if ( prevMask == 0x0001 )
                            {
                                /* EasyCODE < */
                                // first position
                                /* EasyCODE > */
                                break;
                            }
                            else
                            {
                                // shift right
                                prevMask /= 2;
                                
                                i = 0;
                            }
                        }
                        else
                        {
                            /* EasyCODE < */
                            // found
                            /* EasyCODE > */
                            while ( prevMask )
                            {
                                // shift right
                                prevMask /= 2; 
                                ix++;
                            }
                            return ix-1;
                        }
                    }
                }
            }
        }
        /* EasyCODE ) */
        break;
    case 4 :
        /* EasyCODE ( 0 
        Sonder Auftraege */
        if ( index )
        {
            return index-1;
        }
        /* EasyCODE ) */
        break;
    default:
        return index;
    }
    return index;
}

/* EasyCODE ) */
/* EasyCODE ) */
