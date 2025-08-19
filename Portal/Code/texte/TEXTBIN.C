/* EasyCODE V6.5
 */
/* EasyCODE ( 1 */
// textbin
// Programm zum Erzeugen eines BinÑrfiles fÅr die Texte einer Landessprache
// aus dem zugehîrigen EXE-File
// Das BinÑrfile enthÑlt am Anfang die relativen Pointer auf alle Strings
// (siehe auch struct text) und danach die Strings selbst.
// Aufruf: textbin EXE-File BinÑrfile
//   (Beispiel: textbin dtxt.exe dtxt.bin)

#include <stdio.h>
#include <io.h>
#include <string.h>
#include <mem.h>
#include "textport.h"

#pragma warn -par
#define MAXLENGTH 60000

unsigned char daten [MAXLENGTH];
/* EasyCODE ( 2
main */
/* EasyCODE F */
void main(int argc, char *argv[])
{
    FILE *in, *out;
    unsigned long length;
    unsigned int i, anf_index, end_index;
    unsigned char *anf_string = "@@@ANF";
    unsigned char *end_string = "@@@END";
    if( (in = fopen(argv[1], "rb")) == NULL )
    {
        printf ("\n Fehler: EXE-File kann nicht geîffnet werden \n");
        fcloseall();
        return ;
    }
    length = filelength ( fileno(in) );
    if( (length > MAXLENGTH) || (length < (strlen(anf_string)+strlen(end_string))) )
    {
        printf ("\n Fehler: Grî·e des EXE-Files \n");
        fcloseall();
        return ;
    }
    if( (unsigned int) read ( fileno(in), daten, length ) != length )
    {
        printf ("\n Fehler beim Einlesen des EXE-Files \n");
        fcloseall();
        return ;
    }
    anf_index = 0xffff;
    for( i=0; i < length-strlen(anf_string); i++ )
    {
        if( memcmp ( &daten[i], anf_string, strlen(anf_string) ) == 0 )
        {
            anf_index = i;
            break;
        }
    }
    if( anf_index == 0xffff )
    {
        printf ("\n Fehler: keine Anfangkennung im EXE-File \n");
        fcloseall();
        return ;
    }
    end_index = 0xffff;
    for( ; i < length-strlen(end_string); i++ )
    {
        if( memcmp ( &daten[i], end_string, strlen(end_string) ) == 0 )
        {
            end_index = i + strlen(end_string);
            break;
        }
    }
    if( end_index == 0xffff )
    {
        printf ("\n Fehler: keine Endekennung im EXE-File \n");
        fcloseall();
        return ;
    }
    if( sizeof(struct text) > anf_index )
    {
        printf ("\n Fehler: Anzahl der Stringpointer im EXE-File \n");
        fcloseall();
        return ;
    }
    anf_index = anf_index - sizeof(struct text);
    i = end_index - anf_index + 1;
    if( i > TEXTLENGTH )
    {
        printf ("\n Fehler: TextlÑnge zu gro· (LÑnge: %u, [max.: %u]) \n",
              i, TEXTLENGTH);
        fcloseall();
        return ;
    }
    if( (out = fopen(argv[2], "wb")) == NULL )
    {
        printf ("\n Fehler: BinÑrfile kann nicht geîffnet werden \n");
        fcloseall();
        return ;
    }
    if( write ( fileno(out), &daten[anf_index], i ) != i )
    {
        printf ("\n Fehler beim Schreiben des BinÑrfiles \n");
        fcloseall();
        remove (argv[2]);
        return ;
    }
    if( fcloseall() == EOF )
    {
        printf ("\n Fehler beim Schlie·en der Files \n");
        remove (argv[2]);
        return ;
    }
    printf ("\n BinÑrfile %s wurde erstellt (LÑnge: %u, [max.: %u]) \n",
             argv[2], i, TEXTLENGTH);

}
/* EasyCODE ) */
/* EasyCODE ) */
