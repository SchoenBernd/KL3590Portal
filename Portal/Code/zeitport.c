/* EasyCODE V8 */
/* EasyCODE ( 0 
zeitport, Zeitglieder */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
// Datei:          zeitport
// Klasse:         Kl3590 Portal mit Touch
// Ersteller:      Halfmann   EWE 2856

#include "portal.h"

KlasseZeit zeit (tdini[ZeitIndex]);
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseZeit::zeittask */
/* EasyCODE F */
void KlasseZeit::zeittask (void)
{
    unsigned char i;
    for ( i=0; i<ANZAHL_ZEITGL; i++ )
    {
        Zt[i].dekrz ();
    }
    if ( btrcounten )
    {
        if ( ++btrcount == 3600 )
        {
            ++prod->betriebs_h;
            btrcount = 0;
        }
    }
    if ( prodcounten )
    {
        if ( ++prodcount == 3600 )
        {
            ++prod->produkt_h;
            prodcount = 0;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
