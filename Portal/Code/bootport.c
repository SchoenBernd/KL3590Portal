/* EasyCODE V8 */
/* EasyCODE ( 0 
bootport */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
// Datei:        bootport.c, Unterprogramme zum Booten der C167-Firmware
// Klasse:       3590 Portal
// Ersteller:    Halfmann EWE 2856

// folgende Unterprogramme stehen zur VerfÅgung:

// start167
//    Schreibt die Bootsequenz ins DPR, startet den C167 und
//    ueberprueft die Firmwareversion, die der C167 ins DPR
//    geschrieben hat.
// progflash
//    Liest nacheinander die Daten der 3 Dateien bs3.bin,
//    rampen.bin und version.txt von Diskette, schreibt sie
//    ins DPR und wickelt den Handshake mit dem C167 zum
//    Programmieren des Flashs ab.

// Includes
#include "portal.h"


// Variablen
FILE *in;
/* EasyCODE ) */
/* EasyCODE ( 0 
start167 */
/* EasyCODE F */
// start167
// Eingangs-Parameter:  aktvers ... Zeiger auf Teilenummer der aktuellen Version
// Ausgangs-Parameter:
//   0 ... C167 lÑuft, richtige Version im Flash
//   1 ... C167 lÑuft, falsche Version im Flash
//   2 ... C167 reagiert nicht
//   3 ... Bootdatei kann nicht geˆffnet werden

unsigned char start167 (unsigned char *aktvers)
{
    unsigned char *ptr;
    unsigned int i, length;
    if ( (inportb(STATIN) & RESET_MSK)!=0 )
    {
        outportb (RES167OUT,RESDATA1);
        outportb (RES167OUT,RESDATA2);
        outportb (RES167OUT,RESDATA3);
    }
    C167_ERR=0;
    C167_READY=0;
    ptr=DPRBASE;
    if ( (in=fopen("c:c167boot.bin","rb"))==NULL )
    {
        return 3;
    }
    else
    {
        length=filelength (fileno(in));
        for ( i=0, ptr=DPRBASE; i<length; i++ )
        {
            *(ptr+i)=fgetc(in);
        }
        fclose(in);
        outportb (RES167OUT,RESDATA1);
        outportb (RES167OUT,RESDATA2);
        outportb (RES167OUT,RESDATA3);
        tkoor1.rsz();
        tkoor1.sz (saf->c167_timeout);
        while ( !tkoor1.tz() )
        {
            if ( C167_READY==0x55 )
            {
                break;
            }
        }
        if ( tkoor1.tz() )
        {
            return 2;
        }
        else
        {
            if ( saf->bs3download )
            {
                return 1;
            }
            else
            {
                for ( i=0; i<17; i++ )
                {
                    if ( *(DPRVersion+i) != *(aktvers+i) )
                    {
                        return 1;
                    }
                }
                C167_READY=0;
                tkoor1.rsz();
                tkoor1.sz(saf->c167_timeout);
                while ( !tkoor1.tz() )
                {
                    if ( C167_READY==0xff )
                    {
                        break;
                    }
                }
                if ( tkoor1.tz() )
                {
                    return 2;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
progflash */
/* EasyCODE F */
// progflash
// Das Programm wickelt im Dialog mit der Bootsoftware im DPR das
// Programmieren des Flash ab. Es wird vorausgesetzt, da· die richtige
// Diskette eingelegt ist.
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:
//   0 ... fertig, Flash wurde programmiert, Reset auf C167 ausgegeben
//   1 ... Fehler beim Flash lîschen
//   2 ... Fehler beim Flash programmieren
//   3 ... Fehler beim ôffnen oder Lesen der Datei bs3.bin
//   4 ... Fehler beim ôffnen oder Lesen der Datei rampen.bin
//   5 ... Fehler beim ôffnen oder Lesen der Datei version.txt
//   6 ... C167 reagiert nicht

unsigned char progflash (void)
{
    unsigned int i, j;
    long length;
    long v;
    if ( (in=fopen("c:bs3.bin","rb"))==NULL )
    {
        fclose(in);
        return 3;
    }
    else
    {
        if ( (length=filelength(fileno(in)))==-1 )
        {
            fclose(in);
            return 3;
        }
        else
        {
            // Flash lîschen
            C167_READY=0xaa;
            tkoor1.rsz();
            tkoor1.sz(saf->delflash_zt);
            while ( !tkoor1.tz() )
            {
                if ( C167_READY==0x55 )
                {
                    if ( C167_ERR!=0 )
                    {
                        fclose(in);
                        return 1;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if ( tkoor1.tz() )
            {
                fclose(in);
                return 6;
            }
            else
            {
                /* EasyCODE ( 0 
                Firmware ins Flash schreiben */
                for ( v=0; 2*v<length; v+=100 )
                {
                    for ( i=0; (i<100) && (2*(v+i)<length); i++ )
                    {
                        FLASH->data[i]=getw(in);
                    }
                    if ( feof(in) || ferror(in) )
                    {
                        fclose(in);
                        return 3;
                    }
                    else
                    {
                        FLASH->anz=i;
                        FLASH->adr=0x100000 + 2*v;
                        C167_READY=0xaa;
                        tkoor1.rsz();
                        tkoor1.sz(saf->c167_timeout);
                        while ( !tkoor1.tz() )
                        {
                            if ( C167_READY==0x55 )
                            {
                                if ( C167_ERR!=0 )
                                {
                                    fclose(in);
                                    return 2;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                        if ( tkoor1.tz() )
                        {
                            fclose(in);
                            return 6;
                        }
                    }
                }
                fclose(in);
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Rampen ins Flash schreiben */
                if ( saf->md.matdicke == DUENN )
                {
                    if ( (in=fopen("c:rampen.bin","rb"))==NULL )
                    {
                        return 4;
                    }
                }
                else
                {
                    if ( (in=fopen("c:rampen_schwer.bin","rb"))==NULL )
                    {
                        return 4;
                    }
                }
                if ( (length=filelength(fileno(in)))==-1 )
                {
                    fclose(in);
                    return 4;
                }
                else
                {
                    for ( v=0; 2*v<length; v+=100 )
                    {
                        for ( i=0; (i<100) && (2*(v+i)<length); i++ )
                        {
                            FLASH->data[i]=getw(in);
                        }
                        if ( feof(in) || ferror(in) )
                        {
                            fclose(in);
                            return 4;
                        }
                        else
                        {
                            FLASH->anz=i;
                            FLASH->adr=RAMPENSTARTADR + 2*v;
                            C167_READY=0xaa;
                            tkoor1.rsz();
                            tkoor1.sz(saf->c167_timeout);
                            while ( !tkoor1.tz() )
                            {
                                if ( C167_READY==0x55 )
                                {
                                    if ( C167_ERR!=0 )
                                    {
                                        fclose(in);
                                        return 2;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                            }
                            if ( tkoor1.tz() )
                            {
                                fclose(in);
                                return 6;
                            }
                        }
                    }
                    fclose(in);
                }
                /* EasyCODE ) */
                /* EasyCODE ( 0 
                Firmware-Version ins Flash schreiben */
                if ( saf->md.matdicke == DUENN )
                {
                    if ( (in=fopen("c:version.txt","rb"))==NULL )
                    {
                        return 5;
                    }
                }
                else
                {
                    if ( (in=fopen("c:version_schwer.txt","rb"))==NULL )
                    {
                        return 5;
                    }
                }
                for ( i=0; i<9; i++ )
                {
                    FLASH->data[i]=getw(in);
                }
                /* EasyCODE < */
                *(((unsigned char *)FLASH->data)+17) = 0; // Terminator einfuegen
                /* EasyCODE > */
                if ( feof(in) || ferror(in) )
                {
                    fclose(in);
                    return 5;
                }
                else
                {
                    FLASH->anz=9;
                    FLASH->adr=VERSIONADR;
                    C167_READY=0xaa;
                    tkoor1.rsz();
                    tkoor1.sz(saf->c167_timeout);
                    while ( !tkoor1.tz() )
                    {
                        if ( C167_READY==0x55 )
                        {
                            if ( C167_ERR!=0 )
                            {
                                fclose(in);
                                return 2;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    fclose(in);
                    if ( tkoor1.tz() )
                    {
                        return 6;
                    }
                    else
                    {
                        outportb (RES167OUT,RESDATA1);
                        outportb (RES167OUT,RESDATA2);
                        outportb (RES167OUT,RESDATA3);
                        return 0;
                    }
                }
                /* EasyCODE ) */
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
