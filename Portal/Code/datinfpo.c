/* EasyCODE V8 */
/* EasyCODE ( 0 
datinfpo */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
// Datei:        datinfpo.c
// Klasse:       3590 Portal
// Ersteller:    Halfmann   EWE 2856
//
// öber das Dateninterface werden Zugriffe auf den NÑhdatenbereich (Bildspeicher) vorgenommen.
// Die NÑhdaten sind in der Flashdisk abgespeichert. Die aktuell zu bearbeitenden NÑhdaten werden
// in max. 8 Arbeitsbereichen (je 64 kByte) in den EMS-Speicher geladen. Auf diese Bereiche wird Åber
// eine Page-Frame zugegriffen, deren Startadresse bei emsseg:0 liegt. Das aufrufende
// Programm muss sicherstellen, dass die richtigen EMS-Pages gemapt sind.
//
// Folgende Funktionen stehen zur VerfÅgung:
// loadprog   ............   Programm vom Bildspeicher in den Arbeitsspeicher laden
// storeprog   ...........   Programm vom Arbeitsspeicher in den Bildspeicher speichern
// delprog   .............   Programm im Bildspeicher lîschen
// delall   ..............   Alle Programme im Bildspeicher lîschen
// exprog   ..............   Abfrage, ob Programm im Bildspeicher vorhanden
// parprog   .............   Abfrage, ob Programm im Bildspeicher vorhanden und Lesen der Programmparameter
// freememory  ...........   Abfrage, wieviel Speicherplatz noch frei ist

#pragma option -O1

#include "portal.h"
/* EasyCODE ) */
/* EasyCODE ( 0 
Klassedatinf::loadprog,   Programm laden */
/* EasyCODE F */
// Eingangsparameter:   prognr ... Programmnummer
// RÅckmeldungen:
//    0 ... ok
//    1 ... Programm > MAXPROGLEN
//    2 ... Programm nicht vorhanden
//    3 ... Fehler beim Lesen des Programms

unsigned char Klassedatinf::loadprog (unsigned int prognr)
{
    int handle;
    unsigned long length;
    unsigned char name[20];
    unsigned char retwert;
    unsigned char *workptr;
    
    workptr = (unsigned char *)MK_FP (emsseg,0);
    sprintf (name, "%s%04u",datdir,prognr);
    if ( (handle = open(name, O_RDONLY | O_BINARY)) == -1 )
    {
        retwert = 2;
    }
    else
    {
        if ( (length = filelength(handle)) > MAXPROGLEN )
        {
            retwert = 1;
        }
        else
        {
            if ( read ( handle, workptr, length ) != (int)length )
            {
                remove (name);
                retwert = 3;
            }
            else
            {
                if ( prognr != * (unsigned int *) (workptr + GPROGNR +1) )
                {
                    remove (name);
                    retwert = 3;
                }
                else
                {
                    retwert = 0;
                }
            }
        }
    }
    close (handle);
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Klassedatinf::storeprog,   Programm speichern */
/* EasyCODE F */
// Eingangsparameter:  keine
// RÅckmeldungen:
//    0 ... ok
//    1 ... SpeicherÅberlauf
//    2 ... Fehler beim Schreiben

unsigned char Klassedatinf::storeprog (void)
{
    FILE *out;
    unsigned char name[20];
    unsigned char retwert;
    unsigned char *workptr;
    int erg, length;
    
    retwert = 0;
    workptr = (unsigned char *)MK_FP (emsseg,0);
    length = (int) (* (long *) (workptr+KPROGLNG));
    sprintf (name,  "%s%04u",datdir, * (unsigned int *) (workptr +GPROGNR +1));
    if ( (out = fopen(name, "wb")) == NULL )
    {
        retwert = 2;
    }
    else
    {
        if ( (erg = write ( fileno(out), workptr, length)) != length )
        {
            if ( erg == -1 )
            {
                retwert = 2;
            }
            else
            {
                retwert = 1;
            }
        }
    }
    if ( fclose(out) != 0 )
    {
        retwert = 2;
    }
    if ( retwert != 0 )
    {
        remove(name);
    }
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Klassedatinf::delprog,   Programm lîschen */
/* EasyCODE F */
// Eingangsparameter:  Programmnummer
// RÅckmeldungen:
//    0 ... ok
//    1 ... Programm nicht im Speicher
//    2 ... Flash-Schreibfehler

unsigned char Klassedatinf::delprog (unsigned int prognr)
{
    unsigned char name[20];
    
    sprintf (name, "%s%04u",datdir,prognr);
    if ( remove(name) )
    {
        if ( errno==ENOENT )
        {
            /* EasyCODE < */
            // Prog nicht im Speicher
            /* EasyCODE > */
            return 1;
        }
        else
        {
            /* EasyCODE < */
            // Flash-Schreibfehler
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
Klassedatinf::delall,   alle Programme lîschen */
/* EasyCODE F */
// RÅckmeldungen:
//    0 ... ok
//    1 ... kein Programm im Speicher
//    2 ... Flash-Schreibfehler

unsigned char Klassedatinf::delall (void)
{
    /* EasyCODE < */
    struct ffblk ffblk;
    int done;
    unsigned char name[30];
    
    sprintf (name, "%s*.*", datdir);
    done = findfirst ( name, &ffblk, FA_NORMAL);
    /* EasyCODE > */
    if ( done )
    {
        /* EasyCODE < */
        // kein Prog im Speicher
        /* EasyCODE > */
        return 1;
    }
    while ( !done )
    {
        sprintf (name, "%s%s",datdir, ffblk.ff_name);
        if ( remove(name) )
        {
            /* EasyCODE < */
            // Flash-Schreibfehler
            /* EasyCODE > */
            return 2;
        }
        done = findnext (&ffblk);
    }
    return 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Klassedatinf::exprog,   Abfrage, ob Programm existiert */
/* EasyCODE F */
// RÅckmeldungen:
//    0 ... Programm nicht vorhanden
//    1 ... Programm nicht vollstÑndig
//    2 ... Programm vorhanden und vollstÑndig

unsigned char Klassedatinf::exprog (unsigned char quelle, unsigned int prognr)
{
    FILE *in;
    unsigned char name[20];
    unsigned char retwert;
    if ( quelle == DRIVE_A )
    {
        sprintf (name, "A:\%04u",prognr);
    }
    else
    {
        sprintf (name, "%s%04u",datdir,prognr);
    }
    if ( (in = fopen(name, "rb")) == NULL )
    {
        retwert = 0;
    }
    else
    {
        fseek (in, GKENNUNG + 2, SEEK_SET);
        if ( fgetc(in) == 0xff )
        {
            retwert = 1;
        }
        else
        {
            retwert = 2;
        }
    }
    fclose (in);
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Klassedatinf::parprog,  Abfrage, ob Programm existiert und Parameter lesen */
/* EasyCODE F */
/* EasyCODE < */
// RÅckmeldungen:
//    0 ... Programm nicht vorhanden
//    1 ... Programm nicht vollstÑndig
//    2 ... Programm vorhanden und vollstÑndig
//    3 ... falsche Kennung, Programm nicht fÅr diese Maschine

unsigned char Klassedatinf::parprog (unsigned int prognr, struct progpar *parptr, unsigned char drive)
/* EasyCODE > */
{
    FILE *in;
    unsigned char name[20];
    unsigned char retwert, i;
    unsigned char *workptr;
    unsigned long offset;
    if ( drive == DRIVE_A )
    {
        sprintf (name, "A:\%04u",prognr);
    }
    else
    {
        sprintf (name, "%s%04u",datdir,prognr);
    }
    if ( (in = fopen(name, "rb")) == NULL )
    {
        retwert = 0;
    }
    else
    {
        fseek (in, GKENNUNG + 1, SEEK_SET);
        if ( fgetc(in) != PORTAL )
        {
            retwert = 3;
        }
        else
        {
            if ( fgetc(in) == 0xff )
            {
                /* EasyCODE ( 0 
                Kommentar */
                fseek (in, KKOMMENTAR, SEEK_SET);
                workptr = (unsigned char *) &parptr->kommentar[0];
                for ( i=0; i<20; i++ )
                {
                    *(workptr+i) = fgetc(in);
                    *(workptr+i+1) = 0x00;  // Nullterminator setzen
                }
                /* EasyCODE ) */
                retwert = 1;
            }
            else
            {
                fseek (in, GKLCODE + 1, SEEK_SET);
                parptr->klcode = getw(in);
                fseek (in, KPROGLNG, SEEK_SET);
                workptr = (unsigned char *) &parptr->laenge;
                for ( i=0; i<4; i++ )
                {
                    *(workptr+i) = fgetc(in);
                }
                fseek (in, KSTICHDAT, SEEK_SET);
                workptr = (unsigned char *) &offset;
                for ( i=0; i<4; i++ )
                {
                    *(workptr+i) = fgetc(in);
                }
                fseek (in, offset + SSTICHL, SEEK_SET);
                parptr->stichl = getw (in);
                fseek (in, offset + SSTICHZAHL, SEEK_SET);
                parptr->anzstiche = getw (in);
                /* EasyCODE ( 0 
                Kommentar */
                fseek (in, KKOMMENTAR, SEEK_SET);
                workptr = (unsigned char *) &parptr->kommentar[0];
                for ( i=0; i<20; i++ )
                {
                    *(workptr+i) = fgetc(in);
                    *(workptr+i+1) = 0x00;  // Nullterminator setzen
                }
                /* EasyCODE ) */
                retwert = 2;
            }
        }
    }
    fclose (in);
    return retwert;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Klassedatinf::freememory,  freien Speicherplatz melden */
/* EasyCODE F */
// RÅckmeldung:    freier Speicherplatz

unsigned long Klassedatinf::freememory (void)
{
    /* EasyCODE < */
    struct dfree free;
    unsigned long avail;
    
    getdfree (3, &free);
    avail = (long) free.df_avail * (long) free.df_bsec * (long) free.df_sclus;
    /* EasyCODE > */
    return avail;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Klassedatinf::readprog_to_ems */
// Das Up liest die gewaehlte(n) Programm(e) vom Flash in den EMS-Speicher.
// Rueckmeldungen:       0 ... ok
//                       1 ... Programm unvollstaendig
//                       2 ... Programm zu gro·
//                       3 ... Programm nicht vorhanden
//                       4 ... Flash-Lesefehler oder Programm fehlerhaft
//                       5 ... falsche Kennung, Programm nicht fÅr diese Maschine
//                       6 ... nur Stichdaten
/* EasyCODE F */
unsigned char Klassedatinf::readprog_to_ems (void)
{
    unsigned char i,j;
    switch ( saf->md.prsel )
    {
    case 0 :
        // 
    default:
        /* EasyCODE ( 0 
        einfaches Programm */
        for ( i=0; i<=3; i++ )
        {
            ems_map (prog_handle[0],i,i);
        }
        switch ( loadprog(saf->md.pnr) )
        {
        case 0 :
            if ( *(ndatanf+KPROGNR)==2 )
            {
                // nur Stichdaten
                return 6;
            }
            else
            {
                if ( *(ndatanf+GKENNUNG+1)!= PORTAL )
                {
                    eing.errpar = saf->md.pnr;
                    return 5;
                }
                else
                {
                    if ( *(ndatanf+GKENNUNG+2)==-1 )
                    {
                        eing.errpar = saf->md.pnr;
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        case 1 :
            eing.errpar = saf->md.pnr;
            return 2;
        case 2 :
            eing.errpar = saf->md.pnr;
            return 3;
        default:
            eing.errpar = saf->md.pnr;
            return 4;
        }
        /* EasyCODE ) */
    case 1 :
        /* EasyCODE ( 0 
        Sequenz */
        for ( j=0; j<=PNRINDEX_MAX; j++ )
        {
            if ( saf->md.seq_array[saf->md.seq][j]==0xffff )
            {
                break;
            }
            else
            {
                for ( i=0; i<=3; i++ )
                {
                    ems_map (prog_handle[j],i,i);
                }
                switch ( loadprog(saf->md.seq_array[saf->md.seq][j]) )
                {
                case 0 :
                    if ( *(ndatanf+GKENNUNG+1)!= PORTAL )
                    {
                        eing.errpar = saf->md.seq_array[saf->md.seq][j];
                        return 5;
                    }
                    else
                    {
                        if ( *(ndatanf+GKENNUNG+2)==-1 )
                        {
                            eing.errpar = saf->md.seq_array[saf->md.seq][j];
                            return 1;
                        }
                        else
                        {
                            break;
                        }
                    }
                case 1 :
                    eing.errpar = saf->md.seq_array[saf->md.seq][j];
                    return 2;
                case 2 :
                    eing.errpar = saf->md.seq_array[saf->md.seq][j];
                    return 3;
                default:
                    eing.errpar = saf->md.seq_array[saf->md.seq][j];
                    return 4;
                }
            }
        }
        for ( i=0; i<=3; i++ )
        {
            ems_map (prog_handle[saf->md.pnrindex],i,i);
        }
        return 0;
        /* EasyCODE ) */
    case 2 :
        /* EasyCODE ( 0 
        verbundene Programme */
        for ( j=0; j<=PNRINDEX_MAX; j++ )
        {
            if ( saf->md.vpnr_array[saf->md.vpnr][j]==0xffff )
            {
                break;
            }
            else
            {
                for ( i=0; i<=3; i++ )
                {
                    ems_map (prog_handle[j],i,i);
                }
                switch ( loadprog(saf->md.vpnr_array[saf->md.vpnr][j]) )
                {
                case 0 :
                    if ( *(ndatanf+GKENNUNG+1)!= PORTAL )
                    {
                        eing.errpar = saf->md.vpnr_array[saf->md.vpnr][j];
                        return 5;
                    }
                    else
                    {
                        if ( *(ndatanf+GKENNUNG+2)==-1 )
                        {
                            eing.errpar = saf->md.vpnr_array[saf->md.vpnr][j];
                            return 1;
                        }
                        else
                        {
                            break;
                        }
                    }
                case 1 :
                    eing.errpar = saf->md.vpnr_array[saf->md.vpnr][j];
                    return 2;
                case 2 :
                    eing.errpar = saf->md.vpnr_array[saf->md.vpnr][j];
                    return 3;
                default:
                    eing.errpar = saf->md.vpnr_array[saf->md.vpnr][j];
                    return 4;
                }
            }
        }
        for ( i=0; i<=3; i++ )
        {
            ems_map (prog_handle[saf->md.pnrindex],i,i);
        }
        return 0;
        /* EasyCODE ) */
    }
}

/* EasyCODE ) */
Klassedatinf di;
/* EasyCODE ) */
