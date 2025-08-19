/* EasyCODE V8 */
/* EasyCODE ( 0 
hfportal.c, Unterprogramme Huepferfuss */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
// Datei:          hfuss
// Klasse:         Kl3590 Portal
// Ersteller:      Halfmann   EWE 2856

#include "portal.h"

KlasseHuepferfuss hfuss;
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseHuepferfuss::auf */
/* EasyCODE F */
void KlasseHuepferfuss::auf ()
{
    if ( istpos.pos != saf->md.posoben.pos )
    {
        if ( saf->md.hfsicher )
        {
            auf_initkey = 0;
            hfuss_auf.setAuftrag();
        }
        else
        {
            hfmove (saf->md.posoben.motor);
            istpos = saf->md.posoben;
            hfuss_auf.setAuftrag();
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseHuepferfuss::auf_init */
/* EasyCODE F */
// Huepferfuss wird zunaechst auf dem Initiator positioniert
// und anschliessend auf die obere Position gefahren.
// Position muss ok sein.
// Rueckgabe: TRUE  - fertig
//            FALSE - nicht fertig

unsigned char KlasseHuepferfuss::auf_init ()
{
    unsigned char retwert = FALSE;
    switch ( auf_initkey )
    {
    case 0 :
        hfmove (saf->md.posoben.motor + 3);
        auf_initkey = 1;
        break;
    case 1 :
        if ( ready() )
        {
            tnae1.rsz();
            //tnae1.sz(50);
            tnae1.sz(400);  // SW009
            if ( e29.getZustand() )
            {
                smz.vort = saf->startstopvorteiler*15;
                smz.status = PAWRAMPCV | SMZMINUS;
                smz.init ();
                smz.rampadr = 1;
                smz.start_Freq ();
                auf_initkey = 3;
            }
            else
            {
                smz.vort = saf->startstopvorteiler*15;
                smz.status = PAWRAMPCV | SMZPLUS;
                smz.init ();
                smz.rampadr = 1;
                smz.start_Freq ();
                auf_initkey = 2;
            }
        }
        break;
    case 2 :
        if ( e29.getZustand() )
        {
            smz.stop();
            auf_initkey = 1;
        }
        else
        {
            if ( tnae1.tz() )
            {
                smz.stop();
                hfuss.posok = FALSE;
                koor.nae_err = 24;
                auf_initkey = 10;
            }
        }
        break;
    case 3 :
        if ( !e29.getZustand() )
        {
            smz.stop();
            auf_initkey = 4;
            if ( saf->hfkor != 0 )
            {
                if ( saf->hfkor < 0 )
                {
                    smz.status = PAWRAMPCV | SMZMINUS;
                }
                else
                {
                    smz.status = PAWRAMPCV | SMZPLUS;
                }
                smz.vort = saf->movevorteiler;
                smz.init();
                smz.rampadr = 1;
                smz.gespls = abs(saf->hfkor);
                smz.start_Pulse ();        // Pulszahl mit fester Frequenz
            }
        }
        else
        {
            if ( tnae1.tz() )
            {
                smz.stop();
                hfuss.posok = FALSE;
                koor.nae_err = 24; 
                auf_initkey = 10;
            }
        }
        break;
    case 4 :
        if ( smz.ready() )
        {
            // fertig
            istpos = saf->md.posoben;
            posok = TRUE;
            retwert = TRUE;
            auf_initkey = 5;
        }
        break;
    case 5 :
        retwert = TRUE;
        break;
    default:
        break;
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseHuepferfuss::ab */
/* EasyCODE F */
void KlasseHuepferfuss::ab (void)
{
    hfkoord tempkoord;
    if ( y8.fertig() )
    {
        if ( y8.getZustand() || ((saf->variante==1) && !nup.find_UFWSTOP ()) )
        {
            tempkoord = akt_posunten;
            hfmove (tempkoord.motor);
            istpos = tempkoord;
        }
        else
        {
            // Niederhalter unten
            nae.errpar = Y8_UF1;
            koor.nae_err = 42;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseHuepferfuss::hfmove (int pos) */
/* EasyCODE F */
void KlasseHuepferfuss::hfmove (int pos)
{
    int hilfsvar;
    
    hilfsvar = pos - istpos.motor;
    if ( hilfsvar != 0 )
    {
        if ( hilfsvar < 0 )
        {
            smz.status = PAWRAMPCV | SMZMINUS;
        }
        else
        {
            smz.status = PAWRAMPCV | SMZPLUS;
        }
        smz.vort = saf->movevorteiler;
        smz.init();
        hilfsvar = abs (hilfsvar);
        smz.rampadr = 1;
        smz.gespls = hilfsvar;
        smz.start_Pulse ();        // Pulszahl mit fester Frequenz
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseHuepferfuss::postomo */
/* EasyCODE F */
// postomo berechnet die Motorkoordinaten der
// uebergebenen Position
// Formel: Motorkoord. = Position[in 0,1mm] * 0,424
// Der Faktor 0,424 ergibt sich aus 400 / (Pi * D[in 0,1mm])
// D = Wirkdurchmesser des Ritzels = 30mm
// 400 = Anzahl der Schrittmotor-Pulse pro Umdrehung ( 2er Sinus)

void KlasseHuepferfuss::postomo (hfkoord *pos)
{
    float tmp;
    
    tmp = 0.424 * pos->pos;
    if ( tmp >= 0 )
    {
        pos->motor = tmp+0.5; // runden
    }
    else
    {
        pos->motor = tmp-0.5; // runden
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseHuepferfuss::posinit */
/* EasyCODE F */
unsigned char KlasseHuepferfuss::posinit (void)
{
    #ifdef buerotest
        istpos.pos = INITIATOR_POS;
        istpos.motor = INITIATOR_MOTOR;
        posok = TRUE;
        return TRUE;
    #endif 
    switch ( posinitkey )
    {
    case 0 :
        /* EasyCODE ( 0 
        posinitkey = 0,  auf Initiator fahren */
        smz.status = PAWRAMPCV;
        smz.rampadr = 1;
        if ( e29.getZustand() )
        {
            smz.vort = saf->startstopvorteiler;
            smz.init ();
            smz.status |= SMZMINUS;
            smz.start_Freq ();
            posinitkey = 1;
        }
        else
        {
            smz.vort = saf->startstopvorteiler;
            smz.init ();
            smz.status |= SMZPLUS;
            smz.start_Freq ();
            posinitkey = 2;
        }
        return FALSE;
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        posinitkey = 1,  Initiator frei ? */
        if ( e29.getZustand() )
        {
        }
        else
        {
            smz.stop();
            smz.status = (smz.status & ~RICHTUNG) | SMZPLUS;
            smz.start_Freq ();
            posinitkey = 2;
        }
        return FALSE;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        posinitkey = 2,  Initiator betaetigt ? */
        if ( e29.getZustand() )
        {
            smz.stop ();
            tnae2.rsz();
            tnae2.sz (saf->smdelayzt);
            posinitkey = 3;
        }
        return FALSE;
        /* EasyCODE ) */
    case 3 :
        /* EasyCODE ( 0 
        posinitkey = 3,  langsam vom Initiator fahren */
        if ( tnae2.tz() )
        {
            if ( e29.getZustand() )
            {
                smz.vort = saf->startstopvorteiler*15;
                smz.init ();
                smz.rampadr = 1;
                smz.status = (smz.status & ~RICHTUNG) | SMZMINUS;
                smz.start_Freq ();
                posinitkey = 4;
            }
            else
            {
                posinitkey = 0;
            }
        }
        return FALSE;
        /* EasyCODE ) */
    case 4 :
        /* EasyCODE ( 0 
        posinitkey = 4,  Initiator frei ?
                         ja -> Korrektur fahren */
        if ( e29.getZustand() )
        {
        }
        else
        {
            smz.stop ();
            posinitkey = 5;
            if ( saf->hfkor != 0 )
            {
                if ( saf->hfkor < 0 )
                {
                    smz.status = PAWRAMPCV | SMZMINUS;
                }
                else
                {
                    smz.status = PAWRAMPCV | SMZPLUS;
                }
                smz.vort = saf->movevorteiler;
                smz.init();
                smz.rampadr = 1;
                smz.gespls = abs(saf->hfkor);
                smz.start_Pulse ();        // Pulszahl mit fester Frequenz
            }
        }
        return FALSE;
        /* EasyCODE ) */
    case 5 :
        /* EasyCODE ( 0 
        posinitkey = 5,   Korrektur fahren fertig ? */
        if ( smz.ready() )
        {
            // fertig
            istpos.pos = INITIATOR_POS;
            istpos.motor = INITIATOR_MOTOR;
            posok = TRUE;
            hfuss_auf.clearAuftrag(); // SW009
            posinitkey = 6;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
        /* EasyCODE ) */
    case 6 :
        return TRUE;
    default:
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseHuepferfuss::kaltinit */
/* EasyCODE F */
void KlasseHuepferfuss::kaltinit (void)
{
    saf->movevorteiler = 2000;
    saf->startstopvorteiler = 2000;
    saf->servvorteiler = 5000;
    saf->md.posoben.pos = INITIATOR_POS;
    saf->md.posoben.motor = INITIATOR_MOTOR;
    saf->md.posunten.pos = POSUNTEN_POS;
    saf->md.posunten.motor = POSUNTEN_MOTOR;
    saf->md.hub = 30;
    saf->startauf = 135; //145;   // Inkr.
    saf->startab = 249;  //239;    // Inkr.
}

/* EasyCODE ) */
/* EasyCODE ) */
