/* EasyCODE V8 */
/* EasyCODE ( 0 
screen */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
/* Datei: screen.C        Klasse: 3590 Portal        Ersteller: Halfmann

Kurzbeschreibung:
=================
In dieser Datei befindet sich die zyklische Task screen. (Anzeige des Maschinenbedienfeldes)
Das Maschinenbedienfeld wird immer ueber die serielle Schnittstelle com2
angeschlossen.

screen:
=======
Der Anzeigenteil sendet Zeichen ueber die jeweilige ser. Schnittstelle
zum Bedienfeld. Die Strings fuer das Bedienfeld werden ueber die Anzeige-
Mailbox uebermittelt.
Dazu wird folgendes Element struct der Klasse botschaft benutzt.

botsch {
unsigned char *string;
unsigned int busy;
}; 
...

Zum Senden einer Botschaft, muss die auftragerteilende Task die Methode
senden(string) verwenden, welche automatisch das busy Flag auf True setzt.

Nach Absenden der Zeichenkette setzt die screen Task das busy Flag wieder
automatisch auf FALSE.

Die darstellbaren Zeichen und die Steuersequenzen des Bedienfeldes sind
der Beschreibung des Bedienfeldes zu entnehmen.

*/


#include "portal.h"

// Eine Initialisierung im Header ist nicht moeglich.
download_zeiger download_arr[2] = {
//  ESC Zx,     Pfad und Dateiname
ESCS"Z6", "C:\\pikport.hex",            
ESCS"Z7", "C:\\bvportal.hex"            
};
unsigned char * piktos_loesch = {ESCS "RD"};
unsigned char * vorlagen_loesch = {ESCS "RS"};

KlasseBotschaft *btpointer;  // Pointer auf die Klasse botschaft
KlasseBotschaft   Botsch[ANZBOTSCH];         // Objektdefinition
KlasseScreen screen (tdini [ScreenIndex]);   // Objektdefinition
/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseScreen::screentask  (RTK Task) */
/* EasyCODE F */
//  Task screen
//  Bearbeitung der Anzeige-Mailbox, 
//  sanduhr-Bearbeitung,
//  Anzeigestrings senden.

void KlasseScreen::screentask(void)
{
    unsigned int i;
    if ( start_download )
    {
        /* EasyCODE ( 0 
        Touch Download */
        switch ( download_status = touch_laden() )
        {
        case 0
                 //   0 ... Download ist noch nicht fertig.
        :
            break;
        case 1
                 //   1 ... Download fehlerfrei und erfolgreich beendet.
        :
            start_download = FALSE;
            break;
        case 2
                 //  2 ... Fehler beim Öffnen oder Lesen der Datei, Download abgebrochen.
        :
            /* EasyCODE = */
        case 3
                 // 3 ... Kein ACK vom Touch empfangen (_berwachungstimer abgelaufen).
        :
            start_download = FALSE;
            break;
        default:
            break;
        }
        /* EasyCODE ) */
    }
    else
    {
        /* EasyCODE ( 0 
        Botschaften bearbeiten */
        switch ( screen_key )
        {
        case 0 :
            if ( RTKGetCond(anzbox,&btpointer) )
            {
                // Botschaft da
                screen_key=1;
                stringzeiger = btpointer->getstring();
                if ( (stringzeiger == pMerker)  ||
                     (sanduhrkey == 0) )
                {
                    // Es handelt sich um eine 
                    // Sanduhr Botsch oder um
                    // eine normale Botsch.
                    pMerker = 0;
                }
                else
                {
                    // Es handelt sich um keine Sanduhr botsch
                    // und die Sanduhr ist am Laufen.
                    sanduhrpar.sanduhrreq = FALSE;  // Sanduhr abschalten
                    sprintf(screen_varstr2, sanddels, aktsanduhrstr);
                    screen_botsch1.senden(screen_varstr2);
                    sanduhrkey=0;
                }
            }
            else
            {
                if ( sanduhrpar.sanduhrreq )
                {
                    sanduhr();
                }
            }
            break;
        default:
            if ( btpointer->busycheck() )
            {
                while ( (*stringzeiger)!=0 )
                {
                    // Zeichen senden
                    SendChar (COM2,*stringzeiger);
                    stringzeiger++;
                }
            }
            else
            {
                SendBlock(COM2, stringzeiger, btpointer->get_busyvalue());
            }
            // alle Zeichen gesendet
            char * ptr;
            ptr = btpointer->getstring();
            delete [] ptr;//btpointer->getstring();;
            btpointer->clearbusy();
            screen_key=0;
            break;
        }
        /* EasyCODE ) */
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseScreen::sanduhr */
/* EasyCODE F */
// Up: Laufende Trennlinie in Wartepausen anzeigen.
void KlasseScreen::sanduhr(void)
{
    /* EasyCODE < */
    volatile struct botschaft *helpptr;
    unsigned int del_count;
    /* EasyCODE > */
    switch ( sanduhrkey )
    {
    case 0 :
        sanduhrcount=0;
        ttanz1.rsz();  
        ttanz1.sz(50);
        strcpy(aktsanduhrstr, PSANDUHR1);
        aktsanduhrstr[3] = 0x00;
        strcpy(delsanduhrstr, PSANDUHR1);
        delsanduhrstr[3] = 0x00;
        sanduhrkey=1;
        break;
    case 1 :
        if ( ttanz1.tz() && !screen_botsch1.busycheck() )
        {
            if ( (sanduhrcount) > 0 )
            {
                del_count = sanduhrcount-1;
            }
            else
            {
                del_count = 7;
            }
            /* EasyCODE ( 0 
            aktsanduhrstr bestimmen */
            /* EasyCODE < */
            // ESC P Piktonr,x,y,0,255   (0 = frame, 255 = keinTouchcode)
                        // Format Angabe 03 = führende Nullen, bis auf drei Stellen gesamt, ergänzen.
                        // Die Position der Sanduhr wird durch den Aufrufer bestimmt.
            /* EasyCODE > */
            switch ( sanduhrcount )
            {
            case 1 :
                strcpy(aktsanduhrstr, PSANDUHR2);
                break;
            case 2 :
                strcpy(aktsanduhrstr, PSANDUHR3);
                break;
            case 3 :
                strcpy(aktsanduhrstr, PSANDUHR4);
                break;
            case 4 :
                strcpy(aktsanduhrstr, PSANDUHR5);
                break;
            case 5 :
                strcpy(aktsanduhrstr, PSANDUHR6);
                break;
            case 6 :
                strcpy(aktsanduhrstr, PSANDUHR7);
                break;
            case 7 :
                strcpy(aktsanduhrstr, PSANDUHR8);
                break;
            default:
                // 0
                strcpy(aktsanduhrstr, PSANDUHR1);
                break;
            }
            /* EasyCODE ) */
            /* EasyCODE ( 0 
            delsanduhrstr bestimmen */
            switch ( del_count )
            {
            case 1 :
                strcpy(delsanduhrstr, PSANDUHR2);
                break;
            case 2 :
                strcpy(delsanduhrstr, PSANDUHR3);
                break;
            case 3 :
                strcpy(delsanduhrstr, PSANDUHR4);
                break;
            case 4 :
                strcpy(delsanduhrstr, PSANDUHR5);
                break;
            case 5 :
                strcpy(delsanduhrstr, PSANDUHR6);
                break;
            case 6 :
                strcpy(delsanduhrstr, PSANDUHR7);
                break;
            case 7 :
                strcpy(delsanduhrstr, PSANDUHR8);
                break;
            default:
                // 0
                strcpy(delsanduhrstr, PSANDUHR1);
                break;
            }
            /* EasyCODE ) */
            sprintf(screen_varstr, sanduhrs, delsanduhrstr,  aktsanduhrstr, sanduhrpar.pos_x, sanduhrpar.pos_y);
            pMerker = screen_botsch1.senden(screen_varstr);  
            ttanz1.rsz();
            ttanz1.sz(50);
            if ( sanduhrcount>=7 )
            {
                sanduhrcount=0;
            }
            else
            {
                sanduhrcount++;
            }
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseScreen::linerun */
/* EasyCODE F */
// Up: Laufende Linie als Wartepause anzeigen
void KlasseScreen::linerun(void)
{
    /* EasyCODE < */
    volatile struct botschaft *helpptr;
    /* EasyCODE > */
    switch ( linerunkey )
    {
    case 0 :
        if ( !screen_botsch1.busycheck() )
        {
            // Dotline auf Breite 2 setzen.
            sprintf(screen_varstr,ESCS"DL2" );
            screen_botsch1.senden(screen_varstr);
            linecount=0;
            ttanz1.rsz();
            ttanz1.sz(50);
            linerunkey=1;
        }
        break;
    case 1 :
        if ( ttanz1.tz() && !screen_botsch1.busycheck() )
        {
            // 5 Punkte der Linie invertieren (loeschen). (von x = 000  bis  x = 004).
            sprintf(screen_varstr,ESCS"DD0000410"ESCS"DD0040413" );
            screen_botsch1.senden(screen_varstr);
            ttanz1.rsz();
            ttanz1.sz(50);
            linerunkey=2;
        }
        break;
    case 2 :
        if ( ttanz1.tz() && !screen_botsch1.busycheck() )
        {
            // Von nun ab werden immer 10 Punkte der Linie invertiert. Dabei werden in einem 
            // Schritt die ersten 5 Punkte gesetzt und die nächsten 5 Punkte geloescht (invertiert).
            // Das Ergebnis ist ein laufendes Blank von 5 Punkten breite, welches nach rechts wandert.
            
            // Erst positionieren ohne zu zeichnen, dann positionieren und invertieren.
            sprintf(screen_varstr,ESCS"DD%03u0410"ESCS"DD%03u0413",(5*(0+linecount)),(5*(2+linecount)-1) ); 
            screen_botsch1.senden(screen_varstr);
            ttanz1.rsz();
            ttanz1.sz(50);
            if ( linecount>=62 )
            {
                // Das Touchfeld ist 320 Pixel breit (0...319).
                // Im case 2 wurden 62 mal 5 Punkte + die ersten 10 bearbeitet (also 320).
                linecount=0;
                linerunkey=3;
            }
            else
            {
                linecount++;
            }
        }
        break;
    case 3 :
        if ( ttanz1.tz() && !screen_botsch1.busycheck() )
        {
            /* EasyCODE < */
            // Nun muessen noch die letzten 5 Punkte gesetzt werden, um die Linie wieder
                        // komplett darzustellen.
                        
                        // Erst positionieren ohne zu zeichnen, dann positionieren und invertieren.
                        sprintf(screen_varstr,ESCS"DD%03u0410"ESCS"DD%03u0413",314,319 ); 
                        screen_botsch1.senden(screen_varstr);
                        ttanz1.rsz();
                        ttanz1.sz(50); 
                        linerunkey=1; // Wieder von vorne beginnen
            /* EasyCODE > */
        }
        break;
    default:
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseScreen::touch_laden */
/* EasyCODE F */
// Das Programm fuehrt fuer eine Datei einen Touch Download durch. 
// Es wird vorausgesetzt, daî vorher die Methode download mit einem
// gueltigen Index aufgerufen wurde. Der Downloadindex entscheidet über die ESC Zx 
// Sequenz und über die passende Datei zum Download.     
// Eingangs-Parameter:  keine
// Ausgangs-Parameter:
//   0 ... Download ist noch nicht fertig.
//   1 ... Download fehlerfrei und erfolgreich beendet.
//   2 ... Fehler beim Öffnen oder Lesen der Datei, Download abgebrochen.
//   3 ... Kein ACK vom Touch empfangen (_berwachungstimer abgelaufen).

unsigned char KlasseScreen::touch_laden(void)
{
    FILE *in;
    unsigned char bytes_senden;
    unsigned char hochk_found; 
    unsigned char read_byte;
    /* EasyCODE < */
    keyb.bed_ack = FALSE;   // Initialisierung
    /* EasyCODE > */
    /* EasyCODE < */
    // Datei zum Lesen öffnen im binaer Modus.
    /* EasyCODE > */
    if ( (in=fopen(download_arr[downloadind].path,"rb"))==NULL )
    {
        return 2;
    }
    if ( (filelength(fileno(in)))==-1 )
    {
        fclose(in);
        return 2;
    }
    switch ( downloadind )
    {
    case PIKTOLOAD :
        while ( (*piktos_loesch)!=0 )
        {
            SendChar (COM2,*piktos_loesch);
            piktos_loesch++;
        }
        while ( (*vorlagen_loesch)!=0 )
        {
            SendChar (COM2,*vorlagen_loesch);
            vorlagen_loesch++;
        }
        break;
    case VORLAGENLOAD :
        while ( (*vorlagen_loesch)!=0 )
        {
            SendChar (COM2,*vorlagen_loesch);
            vorlagen_loesch++;
        }
        break;
    default:
        break;
    }
    while ( (*download_arr[downloadind].esc_z)!=0 )
    {
        SendChar (COM2,*download_arr[downloadind].esc_z);
        download_arr[downloadind].esc_z++;
    }
    while ( !keyb.bed_ack )
    {
    }
    // OK,   (ACK empfangen).
    bytes_senden = FALSE;
    hochk_found = FALSE;
    /* EasyCODE ( 0 
    Input File wandeln und zum Touch senden */
    // Aus der Download Datei werden nur die reinen Nutzdaten herrausgeholt, welche immer zwischen zwei Hockkomma stehen.
    // Dazu wird erst alles ignoriert, bis das erste Hochkomma gefunden wird. Danach werden die Zeichen ins Touch geladen,
    // bis das naechste Hochkomma gefunden wird usw... . Wenn nach einem Hochkomma CRLF gefunden wird, dann wird der 
    // Code fuer CR LF auch gesendet. Es macht auch nichts, dass SendChar Byte fuer Byte aufgerufen wird, da der Zeitflaschenhals das
    // Touch Flash programmieren ist und nicht die Byte-uebertragung ueber die Schnittstelle ! 
    // Beim Downladen spielt somit die Baudrate eine untergeordnete Rolle.
    while ( TRUE )
    {
        if ( feof(in) )
        {
            break;
        }
        if ( ferror(in) )
        {
            fclose(in);
            return 2;
        }
        read_byte =  getc(in);
        if ( read_byte == 0x22 )
        {
            // " gefunden, Bytes_senden umschalten.
            hochk_found = TRUE;
            if ( bytes_senden == TRUE )
            {
                bytes_senden = FALSE;
            }
            else
            {
                bytes_senden = TRUE;
            }
        }
        else
        {
            if ( bytes_senden )
            {
                // fputc(read_byte,out); AK DOS test
                SendChar (COM2,read_byte);
            }
            else
            {
                if ( hochk_found  &&  (read_byte == 0x43) )
                {
                    // Zeichen C von CRLF nach einem
                    // Hockkomma gefunden.
                    // fputc(0x0D,out);  //  AK  DOS test
                    // fputc(0x0A,out);  // AK DOS test
                    SendChar (COM2,0x0D);   // CR  code
                    SendChar (COM2,0x0A);   // LF   code
                }
            }
            hochk_found = FALSE;
        }
    }
    /* EasyCODE ) */
    fclose(in);
    return 1;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseBotschaft::senden */
/* EasyCODE F */
char* KlasseBotschaft::senden(unsigned char *anzstring)
{
    while ( b.busy )
    {
        ;
    }
    KlasseBotschaft *hlpptr;
    char* pStr = new char[strlen(anzstring)+1];
    strcpy(pStr,anzstring);
    /* EasyCODE - */
    b.busy=0xffff;
    b.string=pStr;
    hlpptr = this;
    RTKPut(anzbox, &hlpptr);  // this = Pointer auf den Start des aktuellen Objekts,
                              // wird von C++ immer automatisch gesetzt.
    return pStr;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KlasseBotschaft::binsenden */
/* EasyCODE F */
// anzahl wird im Busy-Flag der Botschaft abgelegt. 
// busy = 0xffff  -->  String-Uebertragung

char* KlasseBotschaft::binsenden(unsigned char *daten, unsigned int anzahl)
{
    unsigned int i;
    while ( b.busy )
    {
        ;
    }
    KlasseBotschaft *hlpptr;
    unsigned char* pStr = new unsigned char[anzahl];
    for ( i = 0; i < anzahl; i++ )
    {
        *(pStr+i) = *(daten+i);
    }
    b.busy=anzahl;
    b.string=pStr;
    hlpptr = this;
    RTKPut(anzbox, &hlpptr);  // this = Pointer auf den Start des aktuellen Objekts,
                              // wird von C++ immer automatisch gesetzt.
    return pStr;
}

/* EasyCODE ) */
/* EasyCODE ) */
