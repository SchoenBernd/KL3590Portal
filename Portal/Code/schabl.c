/* EasyCODE V8 */
/* EasyCODE ( 0 
schabl, Schablonenerkennung */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
// Datei:          schabl
// Klasse:         Kl3590 Portal
// Ersteller:      Halfmann   EWE 2856

#include "portal.h"

KlasseSchabl schabl (tdini [SchablIndex]);
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSchabl::schabltask */
/* EasyCODE F */
void KlasseSchabl::schabltask (void)
{
    switch ( schablkey )
    {
    case 0 :
        //SW005-1
        schablkey = 1;
        mode = saf->jigctrf;
        break;
    case 1 :
        //SW005-1
        switch ( mode )
        {
        case NOCODE :
            break;
        case RFID :
            if ( rfiden )
            {
                rf.rfid();
            }
            break;
        case BARCODE :
            bc.ablauf();
            break;
        case STATBARC :
            bc.statBC(); // SW005-1
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
