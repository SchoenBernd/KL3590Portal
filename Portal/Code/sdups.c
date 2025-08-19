/* EasyCODE V8 */
/* EasyCODE ( 0 
sdups, Unterprogramme SD-Karte */
// Datei:          sdups
// Klasse:         Kl3590 Portal
// Ersteller:      Halfmann   EWE 2856

#include "portal.h"
KlasseSdUp sd;
/* EasyCODE ( 0 
KlasseSdUp::SdInitMedia */
/* EasyCODE F */
/* SD-Karte initialisieren

Rueckgabe:   0 - fertig
             1 - Karte nicht gesteckt
             2 - Fehler
*/
int KlasseSdUp::SdInitMedia (void)
{
    /* EasyCODE < */
    int loop;
    int Rueckgabe = 1;
    union highlow data;
    /* EasyCODE > */
    while ( eing_botsch1.busycheck() )
    {
    }
    // Befehl Init_Media
    eing_botsch1.senden (INITMEDIA_P1);
    while ( !keyb.KartenmodusMerker() )
    {
    }
    keyb.KartenmodusMerker_res();
    teing1.rsz();
    teing1.sz (T3SEK);
    loop = TRUE;
    while ( loop )
    {
        if ( teing1.tz() )
        {
            // Timeout
            loop = FALSE;
        }
        else
        {
            if ( sdbuf.readbyte() == 0 )
            {
                if ( sdbuf.data() == SD_INIT_OK )
                {
                    // Init_Media fertig
                    loop = FALSE;
                    Rueckgabe = 0;
                }
                else
                {
                    if ( sdbuf.data() == SD_INIT_FAILED )
                    {
                        // Init_Media fertig
                        loop = FALSE;
                        Rueckgabe = 1;
                    }
                    else
                    {
                        /* EasyCODE < */
                        // andere Zeichen
                        // ignorieren
                        /* EasyCODE > */
                    }
                }
            }
            else
            {
                /* EasyCODE < */
                // kein
                // Zeichen
                // da
                /* EasyCODE > */
            }
        }
    }
    teing1.rsz();
    teing1.sz (T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 2;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSdUp::SdCardStatus */
/* EasyCODE F */
/* Fragt den Status der SD-Karte ab

Uebergabe:   keiner

Rueckgabe:   0 - Karte darf beschrieben werden
             1 - Karte ist schreibgeschuetzt
             2 - Karte nicht richtig gesteckt
             3 - Karte nicht gesteckt
             4 - Fehler
*/
int KlasseSdUp::SdCardStatus (void)
{
    union highlow data;
    int Rueckgabe = 4;
    int loop;
    while ( eing_botsch1.busycheck() )
    {
    }
    // Befehl Init_Media
    eing_botsch1.senden (KARTENSTATUS_P1);
    while ( !keyb.KartenmodusMerker() )
    {
    }
    keyb.KartenmodusMerker_res();
    teing1.rsz();
    teing1.sz (T1SEK);
    loop = TRUE;
    while ( loop )
    {
        if ( teing1.tz() )
        {
            // Timeout
            loop = FALSE;
        }
        else
        {
            if ( sdbuf.readbyte() == 0 )
            {
                switch ( sdbuf.data() )
                {
                case CARD_IS_WR_ENABLE :
                    Rueckgabe = 0;
                    break;
                case CARD_IS_WR_PROTECT :
                    Rueckgabe = 1;
                    break;
                case CARD_INSERETION_FAIL :
                    Rueckgabe = 2;
                    break;
                case NO_CARD_INSERTED :
                    Rueckgabe = 3;
                    break;
                default:
                    break;
                }
                loop = FALSE;
            }
            else
            {
                /* EasyCODE < */
                // kein
                // Zeichen
                // da
                /* EasyCODE > */
            }
        }
    }
    teing1.rsz();
    teing1.sz (T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 4;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSdUp::SdFileOpen */
/* EasyCODE F */
/* Datei auf SD-Karte oeffnen
Uebergabe:   Stringzeiger auf Dateiname
             overwrite = FALSE  -  normales oeffnen
             overwrite = TRUE   -  oeffnen und zum ueberschreiben loeschen

Rueckgabe:   0 - Datei geoeffnet
             1 - Datei nicht vorhanden
             2 - Fehler
*/
int KlasseSdUp::SdFileOpen (char *dateiname, unsigned char overwrite)
{
    int Rueckgabe = 2;
    int i, loop;
    while ( eing_botsch1.busycheck() )
    {
    }
    if ( overwrite )
    {
        sprintf (eing.varstring, OPENWRITE_P1);
    }
    else
    {
        sprintf (eing.varstring, OPEN_P1);
    }
    strcat (eing.varstring, dateiname);
    strcat (eing.varstring, "\r");
    eing_botsch1.senden(eing.varstring);
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
            /* EasyCODE ( 0 
            Daten auswerten */
            if ( i == 0 )
            {
                if ( sdbuf.data() == HANDLE )
                {
                    // Bei Handle 0
                    // weitermachen
                    i++;
                }
                else
                {
                    if ( sdbuf.data() == FILE_NOT_AVAILABLE )
                    {
                        // Datei nicht vorhanden
                        loop = 0;
                        Rueckgabe = 1;
                    }
                    else
                    {
                        // Fehler
                        loop = 0;
                    }
                }
            }
            else
            {
                if ( i == 1 )
                {
                    if ( sdbuf.data() == FILE_IS_OPENED )
                    {
                        // Datei vorhanden
                        loop = 0;
                        Rueckgabe = 0;
                    }
                    else
                    {
                        if ( sdbuf.data() == FILE_OPEN_FAILED )
                        {
                            // Datei nicht vorhanden
                            loop = 0;
                            Rueckgabe = 1;
                        }
                        else
                        {
                            // Fehler
                            loop = 0;
                        }
                    }
                }
                else
                {
                    i++;
                }
            }
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE < */
            // kein
            // Zeichen
            // da
            /* EasyCODE > */
            if ( teing1.tz() )
            {
                loop= FALSE;
            }
        }
    }
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 2;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSdUp::SdFileClose */
/* EasyCODE F */
/* Datei auf SD-Karte schliessen

Uebergabe:   Stringzeiger aus Dateiname

Rueckgabe:   0 - Datei geschlossen
             2 - Fehler
*/
int KlasseSdUp::SdFileClose (void)
{
    union highlow data;
    int Rueckgabe = 2;
    while ( eing_botsch1.busycheck() )
    {
    }
    eing_botsch1.senden(CLOSE_P1);
    while ( !keyb.KartenmodusMerker() )
    {
    }
    keyb.KartenmodusMerker_res();
    teing1.rsz();
    teing1.sz(T1SEK);
    while ( TRUE )
    {
        if ( sdbuf.readbyte() == 0 )
        {
            // Datei geschlossen
            Rueckgabe = 0;
            break;
        }
        else
        {
            /* EasyCODE < */
            // kein
            // Zeichen
            // da
            /* EasyCODE > */
            if ( teing1.tz() )
            {
                break;
            }
        }
    }
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 2;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSdUp::SdFileCreate */
/* EasyCODE F */
/* Datei auf SD-Karte erzeugen
Uebergabe:   Stringzeiger auf Dateiname

Rueckgabe:   0 - Datei erzeugt
             1 - Datei nicht erzeugt
             2 - Fehler
*/
int KlasseSdUp::SdFileCreate (char *dateiname)
{
    /* EasyCODE < */
    union highlow data;
    int Rueckgabe = 2;
    int i, loop;
    /* EasyCODE > */
    while ( eing_botsch1.busycheck() )
    {
    }
    sprintf (eing.varstring, CREATE_P1);
    strcat (eing.varstring, dateiname);
    strcat (eing.varstring, "\r");
    eing_botsch1.senden(eing.varstring);
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
            /* EasyCODE ( 0 
            Daten auswerten */
            if ( i == 0 )
            {
                if ( sdbuf.data() == HANDLE )
                {
                    // Bei Handle 0
                    // weitermachen
                    i++;
                }
                else
                {
                    if ( sdbuf.data() == FILE_NOT_AVAILABLE )
                    {
                        // Datei nicht erstellt
                        loop = 0;
                        Rueckgabe = 1;
                    }
                    else
                    {
                        // Fehler
                        loop = 0;
                    }
                }
            }
            else
            {
                if ( i == 1 )
                {
                    if ( sdbuf.data() == FILE_IS_CREATED )
                    {
                        // Datei erstellt
                        loop = 0;
                        Rueckgabe = 0;
                    }
                    else
                    {
                        if ( sdbuf.data() == FILE_CREATION_FAILED )
                        {
                            // Datei nicht erstellt
                            loop = 0;
                            Rueckgabe = 1;
                        }
                        else
                        {
                            // Fehler
                            loop = 0;
                        }
                    }
                }
                else
                {
                    i++;
                }
            }
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE < */
            // kein
            // Zeichen
            // da
            /* EasyCODE > */
            if ( teing1.tz() )
            {
                loop= FALSE;
            }
        }
    }
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 2;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSdUp::SdChangeDir */
/* EasyCODE F */
/* Wechselt zum angegebenen Directory

Uebergabe:   Stringzeiger des Directorys

Rueckgabe:   0 - ok
             1 - Directory existiert nicht
             2 - Fehler
*/
int KlasseSdUp::SdChangeDir (char *dir)
{
    /* EasyCODE < */
    union highlow data;
    int Rueckgabe = 2;
    int loop = TRUE;
    /* EasyCODE > */
    while ( eing_botsch1.busycheck() )
    {
    }
    sprintf (eing.varstring, CHANGEDIRECTORY_P1);
    strcat (eing.varstring, dir);
    eing_botsch1.senden(eing.varstring);
    while ( !keyb.KartenmodusMerker() )
    {
    }
    keyb.KartenmodusMerker_res();
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( loop )
    {
        if ( sdbuf.readbyte() == 0 )
        {
            loop = FALSE;
            if ( sdbuf.data() == CHANGE_DIR_OK )
            {
                // Directory gewechselt
                Rueckgabe = 0;
            }
            else
            {
                // Directory nicht da
                Rueckgabe = 1;
            }
        }
        else
        {
            /* EasyCODE < */
            // kein
            // Zeichen
            // da
            /* EasyCODE > */
            if ( teing1.tz() )
            {
                loop= FALSE;
            }
        }
    }
    teing1.rsz();
    teing1.sz (T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 2;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSdUp::SdReadDir */
/* EasyCODE F */
/* Directory von SD-Karte lesen

Uebergabe:   Directory
             Stringzeiger f+r Directorydaten

Rueckgabe:   0 - Directory gelesen
             1 - Fehler
*/
int KlasseSdUp::SdReadDir (char *directory, char *dirstring)
{
    /* EasyCODE < */
    union highlow data;
    int Rueckgabe = 1;
    int i, loop;
    /* EasyCODE > */
    while ( eing_botsch1.busycheck() )
    {
    }
    sprintf (eing.varstring, READDIRECTORY_P1);
    strcat (eing.varstring, directory);
    strcat (eing.varstring, "\\????\r");
    eing_botsch1.senden(eing.varstring);
    while ( !keyb.KartenmodusMerker() )
    {
    }
    keyb.KartenmodusMerker_res();
    teing1.rsz();
    teing1.sz(T10SEK);
    i = 0;
    loop = TRUE;
    while ( loop && (i<3000) )
    {
        if ( sdbuf.readbyte() == 0 )
        {
            dirstring[i++] = sdbuf.data();
            if ( i > 1 )
            {
                if ( dirstring[i-1] == '\r' &&
                     dirstring[i-2] == '\r' )
                {
                    /* EasyCODE < */
                    // fertig wenn 2 * \r
                    // hintereinander empfangen
                    /* EasyCODE > */
                    if ( i == 2 )
                    {
                        dirstring[i-2] = 0;
                    }
                    else
                    {
                        dirstring[i-1] = 0;
                    }
                    loop = FALSE;
                    Rueckgabe = 0;
                }
            }
        }
        else
        {
            /* EasyCODE < */
            // kein
            // Zeichen
            // da
            /* EasyCODE > */
            if ( teing1.tz() )
            {
                loop= FALSE;
            }
        }
    }
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 1;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSdUp::SdMakeDir */
/* EasyCODE F */
/* Directory auf SD-Karte anlegen

Uebergabe:   Stringzeiger auf Directoryname (von Root)

Rueckgabe:   0 - Directory angelegt
             1 - Directory nicht angelegt
             2 - Fehler
*/
int KlasseSdUp::SdMakeDir (char *name)
{
    union highlow data;
    int loop;
    int Rueckgabe = 2;
    while ( eing_botsch1.busycheck() )
    {
    }
    sprintf (eing.varstring, MAKEDIRECTORY_P1);
    strcat (eing.varstring, name);
    eing_botsch1.senden(eing.varstring);
    while ( !keyb.KartenmodusMerker() )
    {
    }
    keyb.KartenmodusMerker_res();
    teing1.rsz();
    teing1.sz(T1SEK);
    while ( loop )
    {
        if ( sdbuf.readbyte() == 0 )
        {
            if ( sdbuf.data() == MAKE_DIR_OK )
            {
                // Directory angelegt
                loop = FALSE;
                Rueckgabe = 0;
            }
            else
            {
                if ( sdbuf.data() == MAKE_DIR_FAIL )
                {
                    // Directory nicht angelegt
                    loop = FALSE;
                    Rueckgabe = 1;
                }
                else
                {
                    /* EasyCODE < */
                    // andere Zeichen
                    // ignorieren
                    /* EasyCODE > */
                }
            }
        }
        else
        {
            /* EasyCODE < */
            // kein
            // Zeichen
            // da
            /* EasyCODE > */
            if ( teing1.tz() )
            {
                // Timeout
                loop = FALSE;
            }
        }
    }
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 2;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSdUp::SdFileWrite */
/* EasyCODE F */
/* Datei auf SD-Karte schreiben

Uebergabe:   Stringzeiger auf Programmdaten
             anzahl der Daten, 1 < anzahl <= RWLAENGE

Rueckgabe:   0 - Datei geschrieben
             1 - falsches Handle
             2 - Fehler beim Schreiben
             3 - Fehler
*/
int KlasseSdUp::SdFileWrite (char *programmdaten, unsigned int anzahl)
{
    union highlow data;
    union highlow fehler;
    int Rueckgabe = 3;
    int i, loop;
    while ( eing_botsch1.busycheck() )
    {
    }
    sprintf (eing.varstring, "%s%05d\r", WRITE_P1, anzahl);
    eing_botsch1.senden (eing.varstring);
    for ( i = 0; i < anzahl; i++ )
    {
        eing.varstring1[i] = *(programmdaten+i);
    }
    eing_botsch1.binsenden (eing.varstring1, anzahl);
    while ( !keyb.KartenmodusMerker() )
    {
    }
    keyb.KartenmodusMerker_res();
    teing1.rsz();
    teing1.sz(i);
    i = 0;
    loop = TRUE;
    /* EasyCODE ( 0 
    Antwort lesen */
    while ( loop )
    {
        if ( sdbuf.readbyte() == 0 )
        {
            if ( i == 0 )
            {
                if ( sdbuf.data() == FILE_REF_OK )
                {
                    // Datei kann 
                    // geschrieben werden
                    // (Handle OK)
                    loop = FALSE;
                    Rueckgabe = 0;
                }
                else
                {
                    if ( sdbuf.data() == FILE_NOT_AVAILABLE )
                    {
                        // Datei kann nicht
                        // geschrieben werden
                        // (falsche Handle)
                        loop = FALSE;
                        Rueckgabe = 1;
                    }
                    else
                    {
                        if ( sdbuf.data() == WRITE_ERROR )
                        {
                            // Fehler beim schreiben
                            // jetzt noch zwei Byte
                            // Fehlerposition holen
                            i++;
                        }
                        else
                        {
                            // Fehler
                            loop = FALSE;
                        }
                    }
                }
            }
            else
            {
                if ( i == 1 )
                {
                    // Lowbyte des Fehlers
                    fehler.byte.low = sdbuf.data();
                    i++;
                }
                else
                {
                    // Highbyte des Fehlers
                    fehler.byte.high = sdbuf.data();
                    loop = FALSE;
                    Rueckgabe = 2;
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
                loop = FALSE;
            }
        }
    }
    /* EasyCODE ) */
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 3;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSdUp::SdFileRead */
/* EasyCODE F */
/* Datei von SD-Karte lesen

Uebergabe:   Stringzeiger auf Programmdaten
             Anfahl der zu lesenden Daten

Rueckgabe:   0 - Datei gelesen
             1 - falsches Handle
             2 - Fehler
 */
int KlasseSdUp::SdFileRead (char *programmdaten, unsigned long laenge)
{
    union highlow data;
    int Rueckgabe = 2;
    int i, loop;
    while ( eing_botsch1.busycheck() )
    {
    }
    sprintf (eing.varstring, "%s%05d\r", READ_P1, laenge);
    eing_botsch1.senden (eing.varstring);
    while ( !keyb.KartenmodusMerker() )
    {
    }
    keyb.KartenmodusMerker_res();
    teing1.rsz();
    teing1.sz(T10SEK);
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
                    // Datei kann 
                    // gelesen werden
                    // (Handle OK)
                    i++;
                }
                else
                {
                    // Datei kann nicht
                    // gelesen werden
                    // (falsche Handle)
                    loop = FALSE;
                    Rueckgabe = 1;
                }
            }
            else
            {
                if ( i < laenge )
                {
                    // Programmdaten speichern
                    programmdaten[i-1] = sdbuf.data();
                    i++;
                }
                else
                {
                    // fertig
                    programmdaten[i-1] = sdbuf.data();
                    loop = FALSE;
                    Rueckgabe = 0;
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
                loop = FALSE;
            }
        }
    }
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 2;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSdUp::SdFileDelete */
/* EasyCODE F */
/* Datei auf SD-Karte loeschen

Uebergabe:   Stringzeiger aus Dateiname

Rueckgabe:   0 - Datei geloescht
             1 - Datei nicht geloescht
             2 - Fehler
*/
int KlasseSdUp::SdFileDelete (char *dateiname)
{
    union highlow data;
    int Rueckgabe = 2;
    int loop;
    while ( eing_botsch1.busycheck() )
    {
    }
    sprintf (eing.varstring, DELETE_P1);
    strcat (eing.varstring, dateiname);
    strcat (eing.varstring, "\r");
    eing_botsch1.senden(eing.varstring);
    while ( !keyb.KartenmodusMerker() )
    {
    }
    keyb.KartenmodusMerker_res();
    teing1.rsz();
    teing1.sz(T10SEK);
    loop = TRUE;
    while ( loop )
    {
        if ( sdbuf.readbyte() == 0 )
        {
            if ( sdbuf.data() == FILE_REMOVED )
            {
                // Datei geloescht
                loop = FALSE;
                Rueckgabe = 0;
            }
            else
            {
                if ( sdbuf.data() == FILE_NOT_REMOVED )
                {
                    // Datei nicht geloescht
                    loop = FALSE;
                    Rueckgabe = 1;
                }
                else
                {
                    /* EasyCODE < */
                    // andere Zeichen
                    // ignorieren
                    /* EasyCODE > */
                }
            }
        }
        else
        {
            /* EasyCODE < */
            // kein
            // Zeichen
            // da
            /* EasyCODE > */
            if ( teing1.tz() )
            {
                // Timeout
                loop = FALSE;
            }
        }
    }
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 2;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Klasse SdUp::SdFormatCard */
/* EasyCODE F */
/* SD-Karte formatieren
Uebergabe:   keine

Rueckgabe:   0 - Formatieren fertig
             1 - Formatieren gescheitert
             2 - Fehler
*/
int KlasseSdUp::SdFormatCard (void)
{
    union highlow data;
    int Rueckgabe = 2;
    int loop;
    while ( eing_botsch1.busycheck() )
    {
    }
    eing_botsch1.senden(FORMATIEREN_P1);
    while ( !keyb.KartenmodusMerker() )
    {
    }
    keyb.KartenmodusMerker_res();
    teing1.rsz();
    teing1.sz(T30SEK);
    while ( loop )
    {
        if ( sdbuf.readbyte() == 0 )
        {
            if ( sdbuf.data() == FORMAT_COMPLETE )
            {
                // Formatieren fertig
                loop = FALSE;
                Rueckgabe = 0;
            }
            else
            {
                if ( sdbuf.data() == FORMAT_FAILED )
                {
                    // Formatieren gescheitert
                    loop = FALSE;
                    Rueckgabe = 1;
                }
                else
                {
                    /* EasyCODE < */
                    // andere Zeichen
                    // ignorieren
                    /* EasyCODE > */
                }
            }
        }
        else
        {
            /* EasyCODE < */
            // kein
            // Zeichen
            // da
            /* EasyCODE > */
            if ( teing1.tz() )
            {
                // Timeout
                loop = FALSE;
            }
        }
    }
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 2;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseSdUp::GetFileSize */
/* EasyCODE F */
/* Datei auf SD-Karte oeffnen
Uebergabe:   Stringzeiger auf Dateiname
             Zeiger auf filesize

Rueckgabe:   0 - ok, filesize gueltig
             1 - Datei nicht vorhanden
             2 - Fehler
*/
int KlasseSdUp::GetFileSize (char *dateiname, unsigned long *filesize)
{
    int Rueckgabe = 2;
    int i, loop;
    char tmp[9];
    /* EasyCODE ( 0 
    File oeffnen */
    while ( eing_botsch1.busycheck() )
    {
    }
    sprintf (eing.varstring, OPEN_P1);
    strcat (eing.varstring, dateiname);
    strcat (eing.varstring, "\r");
    eing_botsch1.senden(eing.varstring);
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
            /* EasyCODE ( 0 
            Daten auswerten */
            if ( i == 0 )
            {
                if ( sdbuf.data() == HANDLE )
                {
                    // Bei Handle 0
                    // weitermachen
                    i++;
                }
                else
                {
                    if ( sdbuf.data() == FILE_NOT_AVAILABLE )
                    {
                        // Datei nich vorhanden
                        loop = 0;
                        Rueckgabe = 1;
                    }
                    else
                    {
                        // Fehler
                        loop = 0;
                    }
                }
            }
            else
            {
                if ( i == 1 )
                {
                    if ( sdbuf.data() == FILE_IS_OPENED )
                    {
                        // Datei vorhanden
                        loop = 0;
                        Rueckgabe = 0;
                    }
                    else
                    {
                        if ( sdbuf.data() == FILE_OPEN_FAILED )
                        {
                            // Datei nicht vorhanden
                            loop = 0;
                            Rueckgabe = 1;
                        }
                        else
                        {
                            // Fehler
                            loop = 0;
                        }
                    }
                }
                else
                {
                    i++;
                }
            }
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE < */
            // kein
            // Zeichen
            // da
            /* EasyCODE > */
            if ( teing1.tz() )
            {
                loop= FALSE;
            }
        }
    }
    /* EasyCODE ) */
    if ( Rueckgabe == 0 )
    {
        /* EasyCODE ( 0 
        Oeffnen hat funktioniert,  SEEK Dateiende */
        while ( eing_botsch1.busycheck() )
        {
        }
        sprintf (eing.varstring, "%s0\r%s", SEEK_P1, M_SEEK_END);
        eing_botsch1.senden (eing.varstring);
        while ( !keyb.KartenmodusMerker() )
        {
        }
        keyb.KartenmodusMerker_res();
        teing1.rsz();
        teing1.sz(T1SEK);
        i = 0;
        loop = TRUE;
        /* EasyCODE ( 0 
        Antwort lesen */
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
                        Rueckgabe = 2;
                        loop = FALSE;
                    }
                }
                else
                {
                    if ( sdbuf.data() == SEEK_OK )
                    {
                        // Cursor ist 
                        // positioniert
                        Rueckgabe = 0;
                        loop = FALSE;
                    }
                    else
                    {
                        // Fehler
                        Rueckgabe = 2;
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
                    Rueckgabe = 2;
                    loop = FALSE;
                }
            }
        }
        /* EasyCODE ) */
        /* EasyCODE ) */
        if ( Rueckgabe == 0 )
        {
            /* EasyCODE ( 0 
            SEEK hat funktioniert,  mit TELL Dateilaenge lesen */
            while ( eing_botsch1.busycheck() )
            {
            }
            eing_botsch1.senden (TELL_P1);
            while ( !keyb.KartenmodusMerker() )
            {
            }
            keyb.KartenmodusMerker_res();
            teing1.rsz();
            teing1.sz(T1SEK);
            i = 0;
            loop = TRUE;
            /* EasyCODE ( 0 
            Antwort lesen */
            while ( loop )
            {
                if ( sdbuf.readbyte() == 0 )
                {
                    if ( i == 0 )
                    {
                        if ( sdbuf.data() == FILE_REF_OK )
                        {
                            // Handle OK
                            i++;
                        }
                        else
                        {
                            // Fehler
                            Rueckgabe = 2;
                            loop = FALSE;
                        }
                    }
                    else
                    {
                        if ( i == 1 )
                        {
                            if ( sdbuf.data() == FTELL_OK )
                            {
                                // Cursor ist 
                                // positioniert
                                i++;
                            }
                            else
                            {
                                // Fehler
                                Rueckgabe = 2;
                                loop = FALSE;
                            }
                        }
                        else
                        {
                            tmp[i-2] = sdbuf.data();
                            if ( i==9 )
                            {
                                tmp[8]=0;
                                sscanf (tmp, "%lu", filesize);
                                Rueckgabe= 0;
                                loop = FALSE;
                            }
                            else
                            {
                                i++;
                            }
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
                        Rueckgabe = 2;
                        loop = FALSE;
                    }
                }
            }
            /* EasyCODE ) */
            /* EasyCODE ) */
        }
        /* EasyCODE ( 0 
        File schliessen */
        while ( eing_botsch1.busycheck() )
        {
        }
        eing_botsch1.senden(CLOSE_P1);
        while ( !keyb.KartenmodusMerker() )
        {
        }
        keyb.KartenmodusMerker_res();
        teing1.rsz();
        teing1.sz(T1SEK);
        while ( TRUE )
        {
            if ( sdbuf.readbyte() == 0 )
            {
                // Datei geschlossen
                break;
            }
            else
            {
                /* EasyCODE < */
                // kein
                // Zeichen
                // da
                /* EasyCODE > */
                if ( teing1.tz() )
                {
                    Rueckgabe = 2;
                    break;
                }
            }
        }
        /* EasyCODE ) */
    }
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 2;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Klasse SdUp::ChangeToSD */
/* EasyCODE F */
/* SD-Karte als Programmquelle setzen
Uebergabe:   keine

Rueckgabe:   	0 - OK
           		1 - NOK
	            2 - Fehler

*/
int KlasseSdUp::ChangeToSD (void)
{
    union highlow data;
    int Rueckgabe = 1;
    int loop;
    while ( eing_botsch1.busycheck() )
    {
    }
    eing_botsch1.senden(MODESD_P1);
    while ( !keyb.KartenmodusMerker() )
    {
    }
    //keyb.KartenmodusMerker_res();
    teing1.rsz();
    teing1.sz(T10SEK);
    while ( loop )
    {
        if ( sdbuf.readbyte() == 0 )
        {
            if ( sdbuf.data() == MODE_SD_OK )
            {
                //Erfolgreich
                loop = FALSE;
                Rueckgabe = 0;
            }
            else
            {
                if ( sdbuf.data() == MODE_SD_FAIL )
                {
                    // Formatieren gescheitert
                    loop = FALSE;
                    Rueckgabe = 1;
                }
                else
                {
                    /* EasyCODE < */
                    // andere Zeichen
                    // ignorieren
                    /* EasyCODE > */
                }
            }
        }
        else
        {
            /* EasyCODE < */
            // kein
            // Zeichen
            // da
            /* EasyCODE > */
            if ( teing1.tz() )
            {
                // Timeout
                loop = FALSE;
            }
        }
    }
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 2;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Klasse SdUp::ChangeToNET */
/* EasyCODE F */
/* Netzwerk als Programmquelle setzen
Uebergabe:   keine

Rueckgabe:   	0 - OK
             	1 - NOK
	            2 - Fehler

*/
int KlasseSdUp::ChangeToNET (void)
{
    union highlow data;
    int Rueckgabe = 1;
    int loop;
    while ( eing_botsch1.busycheck() )
    {
    }
    eing_botsch1.senden(MODENET_P1);
    while ( !keyb.KartenmodusMerker() )
    {
    }
    //keyb.KartenmodusMerker_res();
    teing1.rsz();
    teing1.sz(T10SEK);
    while ( loop )
    {
        if ( sdbuf.readbyte() == 0 )
        {
            //int tmp = sdbuf.data();
            if ( sdbuf.data() == MODE_NET_OK )
            {
                //Erfolgreich
                loop = FALSE;
                Rueckgabe = 0;
            }
            else
            {
                if ( sdbuf.data() == MODE_NET_FAIL )
                {
                    // Formatieren gescheitert
                    loop = FALSE;
                    Rueckgabe = 1;
                }
                else
                {
                    /* EasyCODE < */
                    // andere Zeichen
                    // ignorieren
                    /* EasyCODE > */
                }
            }
        }
        else
        {
            /* EasyCODE < */
            // kein
            // Zeichen
            // da
            /* EasyCODE > */
            if ( teing1.tz() )
            {
                // Timeout
                loop = FALSE;
            }
        }
    }
    teing1.rsz();
    teing1.sz(T3SEK);
    while ( keyb.Kartenmodus() )
    {
        /* EasyCODE < */
        // Warten bis fertig
        /* EasyCODE > */
        if ( teing1.tz() )
        {
            Rueckgabe = 2;
            keyb.Kartenmodus_res();
            break;
        }
    }
    return Rueckgabe;
}

/* EasyCODE ) */
/* EasyCODE ) */
