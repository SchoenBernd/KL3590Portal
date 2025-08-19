/* EasyCODE V8 */
/* EasyCODE ( 0 
ringbuf.c */
/* Datei: ringbuf.c
   Klasse: 3590 Portal
   Ersteller: Halfmann EWE 2856

   Kurzbeschreibung:
   =================
   Das Programm organisiert den Zugriff auf einen Ringpuffer. Der Ringpuffer
   besteht aus Verwaltungsteil (6 Byte) und Datenteil (variabel). 

   Beachte !   Die Anzahl der beschreibbaren Bytes = laenge - 1.
               Die maximale laenge = 255 - rest ( rest mu+ kleiner
               als die laenge sein).
               In read_offset und write_offset werden nur die offsets
               zur Anfangsadresse abgelegt.
*/

#include "portal.h"

unsigned char sddaten[SDLAENGE];
KlasseRBuf sdbuf(SDLAENGE, 3, sddaten);   // laenge = SDLAENGE, rest = 3
/* EasyCODE ( 0 
Konstruktor
KlasseRBuf (unsigned char inilaenge, unsigned char inirest, unsigned char *ptr) */
/* EasyCODE F */
KlasseRBuf::KlasseRBuf (unsigned char inilaenge, unsigned char inirest, unsigned char *ptr)
{
    laenge = inilaenge;
    rest = inirest;
    datenptr = ptr;
    read_offset = 0;
    write_offset = 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRBuf::loeschen */
/* EasyCODE F */
void KlasseRBuf::loeschen(void)
{
    read_offset = 0;
    write_offset = 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRBuf::status */
/* EasyCODE F */
//Rueckmeldung:
//     0 - Puffer in normalem Zustand
//     1 - Warnung, Puffer gleich voll
//         (max. Rest frei)
//     2 - Warnung, Ueberlauf beim naechten Schreibzugriff
//     3 - Puffer leer


unsigned char KlasseRBuf::status(void)
{
    if ( leercheck() )
    {
        return 3;
    }
    else
    {
        if ( ueberlaufcheck() )
        {
            return 2;
        }
        else
        {
            if ( warnungscheck() )
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRBuf::readbyte */
/* EasyCODE F */
// Rueckmeldung:
//      0 - Byte ordnungsgemaess gelesen
//          read_data = gelesenes Datenbyte
//      1 - Puffer leer

unsigned char KlasseRBuf::readbyte(void)
{
    if ( leercheck() )
    {
        return 1;
    }
    else
    {
        read_data = datenptr[read_offset];
        if ( ++read_offset==laenge )
        {
            read_offset=0;
        }
        return 0;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRBuf::writebyte */
/* EasyCODE F */
// Rueckmeldung:
//      0 - Byte ordnungsgemaess geschrieben
//      1 - Warnung, Puffer gleich voll (max. Rest frei)
//      2 - Puffer voll, Byte nicht geschrieben

unsigned char KlasseRBuf::writebyte(unsigned char data)
{
    if ( ueberlaufcheck() )
    {
        return 2;
    }
    else
    {
        write_data = data;
        datenptr[write_offset] = write_data;
        if ( ++write_offset==laenge )
        {
            write_offset=0;
        }
        if ( warnungscheck() )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRBuf::leercheck */
/* EasyCODE F */
unsigned char KlasseRBuf::leercheck(void)
{
    if ( read_offset==write_offset )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRBuf::ueberlaufcheck */
/* EasyCODE F */
unsigned char KlasseRBuf::ueberlaufcheck(void)
{
    if ( write_offset+1 < laenge )
    {
        if ( write_offset+1 == read_offset )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if ( (write_offset+1 - laenge) == read_offset )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseRBuf::warnungscheck */
/* EasyCODE F */
unsigned char KlasseRBuf::warnungscheck(void)
{
    if ( write_offset < read_offset )
    {
        if ( write_offset+1+rest >= read_offset )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if ( (write_offset+1+rest >= laenge) &&
             ((write_offset+1+rest-laenge) >= read_offset) )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
