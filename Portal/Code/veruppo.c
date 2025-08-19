/* EasyCODE V8 */
/* EasyCODE ( 0 
Verriegelungs-Unterprogramme KL3590 */
/* EasyCODE ( 0 
Header, Prototypen, Definitionen */
// Datei:        veruppo.c
// Klasse:       Kl3590 Potal
// Ersteller:    Wilfried Halfmann   EES 2856


#include "portal.h"

KlasseVerUp ver;
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseVerUp::move_sonst,   Fahren ausserhalb des Naehprogramms */
/* EasyCODE F */
unsigned char KlasseVerUp::move_sonst (void)
{
    #if defined(buerotest)
        return TRUE;
    #else 
        if ( !e81.getZustand() )
        {
            // Tisch
            // abgeklappt
            nae.errpar = E81_UF1;
        }
        else
        {
            if ( !e50u1.getZustand() )
            {
                // Oberteil angehoben
                nae.errpar = E50U1_UF1;
            }
            else
            {
                if ( !e75.getZustand() )
                {
                    // Spulenabdeckung
                    // geschlossen
                    nae.errpar = E75_UF1;
                }
                else
                {
                    if ( ((e80.getZustand() || e79.getZustand()) &&  nae.deckelueberwachung) &&
                          !e76.getZustand() )
                    {
                        // Schablonendeckel
                        // geschlossen
                        nae.errpar = E76_UF1;
                    }
                    else
                    {
                        if ( !e33.getZustand() )
                        {
                            // Stoffgegendruecker
                            // unten
                            nae.errpar = E33_UF1;
                        }
                        else
                        {
                            if ( hfuss.getpos().pos != saf->md.posoben.pos )
                            {
                                // Huepferfuss oben
                                nae.errpar = HUEPFER_OBEN;
                            }
                            else
                            {
                                if ( y8.getZustand() )
                                {
                                    // Niederhalter oben
                                    nae.errpar = Y8_UF0;
                                }
                                else
                                {
                                    if ( !nminf.nadel_ot() )
                                    {
                                        // Nadelposition
                                        nae.errpar = NEEDLEPOS_UF1;
                                    }
                                    else
                                    {
                                        return TRUE;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        koor.nae_err = 40;
        return FALSE;
    #endif 
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseVerUp::move_nae,   Fahren im Naehprogramm */
/* EasyCODE F */
unsigned char KlasseVerUp::move_nae (void)
{
    if ( !e81.getZustand() )
    {
        // Tisch
        // abgeklappt
        nae.errpar = E81_UF1;
    }
    else
    {
        if ( hfuss.getpos().pos!=saf->md.posoben.pos )
        {
            // Huepferfuss oben
            nae.errpar = HUEPFER_OBEN;
        }
        else
        {
            if ( y8.getZustand() )
            {
                // Niederhalter oben
                nae.errpar = Y8_UF0;
            }
            else
            {
                if ( ((e80.getZustand() || e79.getZustand()) &&  nae.deckelueberwachung) &&
                      !e76.getZustand() )
                {
                    // Schablonendeckel
                    // geschlossen
                    nae.errpar = E76_UF1;
                }
                else
                {
                    if ( saf->cutready && !nminf.nadel_ot() )
                    {
                        // Nadel im OT-Bereich
                        nae.errpar = NEEDLEPOS_UF1;
                    }
                    else
                    {
                        if ( !saf->cutready &&
                             !nminf.nadel_bereich(saf->ot_bereich) )
                        {
                            // Nadel im OT-Bereich
                            nae.errpar = NEEDLEPOS_UF1;
                        }
                        else
                        {
                            if ( !e33.getZustand() )
                            {
                                // Stoffgegendruecker
                                // nicht abgesenkt
                                nae.errpar = E33_UF1;
                            }
                            else
                            {
                                return TRUE;
                            }
                        }
                    }
                }
            }
        }
    }
    koor.nae_err = 45;
    return FALSE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseVerUp::takt,   Takten im Naehbereich */
/* EasyCODE F */
unsigned char KlasseVerUp::takt (void)
{
    if ( !e81.getZustand() )
    {
        // Tisch
        // abgeklappt
        nae.errpar = E81_UF1;
    }
    else
    {
        if ( !saf->cutready )
        {
            // Faden geschnitten
            nae.errpar = CUTREADY_UF1;
        }
        else
        {
            if ( !nminf.nadel_bereich(saf->takt_bereich) )
            {
                // Nadel im zulaessigen Bereich
                nae.errpar = NEEDLEPOS_UF1;
            }
            else
            {
                return TRUE;
            }
        }
    }
    koor.nae_err = 46;
    return FALSE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseVerUp::pmove,   Transportieren waehrend Programmieren */
/* EasyCODE F */
unsigned char KlasseVerUp::pmove (void)
{
    #if defined(buerotest)
        return TRUE;
    #endif 
    if ( !eing.prog.pzbl.faden )
    {
        /* EasyCODE ( 0 
        zu fahrender Bereich */
        if ( !e81.getZustand() )
        {
            // Tisch
            // abgeklappt
            koor.nae_err = 45;
            nae.errpar = E81_UF1;
            return FALSE;
        }
        else
        {
            if ( hfuss.getpos().pos!=saf->md.posoben.pos )
            {
                // Huepferfuss oben
                koor.nae_err = 45;
                nae.errpar = HUEPFER_OBEN;
                return FALSE;
            }
            else
            {
                if ( y8.getZustand() )
                {
                    // Niederhalter
                    // oben
                    koor.nae_err = 45;
                    nae.errpar = Y8_UF0;;
                    return FALSE;
                }
                else
                {
                    if ( !e33.getZustand() )
                    {
                        // Stoffgegenfruecker
                        // unten
                        koor.nae_err = 45;
                        nae.errpar = E33_UF1;
                        return FALSE;
                    }
                    else
                    {
                        if ( !nminf.nadel_bereich(saf->ot_bereich) )
                        {
                            // Nadelposition
                            koor.nae_err = 45;
                            nae.errpar = NEEDLEPOS_UF1;
                            return FALSE;
                        }
                        else
                        {
                            return TRUE;
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
        zu naehender Bereich */
        if ( !e81.getZustand() )
        {
            koor.nae_err = 45;
            // Tisch
            // abgeklappt
            nae.errpar = E81_UF1;
            return FALSE;
        }
        else
        {
            if ( !nminf.nadel_bereich(saf->takt_bereich) )
            {
                /* EasyCODE < */
                koor.nae_err = 45;
                nae.errpar = NEEDLEPOS_UF1; // Nadelposition
                /* EasyCODE > */
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
        /* EasyCODE ) */
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
