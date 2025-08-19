/* EasyCODE V8 */
/* EasyCODE ( 0 
Utilities KL3590 Portal */
/* EasyCODE ( 0 
Header, Definitionen, Prototypen */
// Datei:          utlport.c
// Klasse:         Kl3590 Portal
// Ersteller:      Halfmann   EWE 2856

// Folgende Unterprogramme stehen zur VerfÅgung:

// ems_alloc .......... ems-Speicher reservieren
// ems_free ........... ems-Speicher freigeben
// ems_map ............ allokierten Speicher der ems-page zuordnen
// safdat_check ....... Checksumme der netzausfallsicheren Daten bilden
// axtoi .............. atoi fÅr Hexziffern
// textload ........... sprachabhÑngiges Text-Binaerfile in Heap laden

#include "portal.h"

volatile unsigned char textload_err;
/* EasyCODE ) */
/* EasyCODE ( 0 
ems_alloc */
/* EasyCODE F */
/* EasyCODE < */
//*************** ems_alloc ***************
// Funktion: Allokiert die angegebene Zahl von Pages und liefert ein Handle
//                   zum Zugriff auf diese Pages zurÅck.
// Eing.-Par.: pages - die Anzahl der zu allokierenden Pages (jeweils 16 kByte)
// Return-Wert: EMS_ERR bei einem Fehler, sonst das EMS-Handle

int ems_alloc (int pages)
/* EasyCODE > */
{
    /* EasyCODE < */
    union REGS regs;
    
    regs.h.ah = 0x43;                   // Funktionsnummer: Pages allokieren
    regs.x.bx = pages;
    int86 (EMS_INT, &regs, &regs);
    /* EasyCODE > */
    if ( regs.h.ah )
    {
        /* EasyCODE < */
        // Fehler
        /* EasyCODE > */
        return EMS_ERR;
    }
    else
    {
        /* EasyCODE < */
        // Handle zurÅckliefern
        /* EasyCODE > */
        return regs.x.dx;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ems_free */
/* EasyCODE F */
/* EasyCODE < */
//*************** ems_free ***************
// Funktion: Gibt die Pages eines Handles  frei
// Eing.-Par.: Handle
// Return-Wert: 0 .......... o.k.
//                         sonst .. Fehler

unsigned char ems_free (int handle)
/* EasyCODE > */
{
    /* EasyCODE < */
    union REGS regs;
    
    regs.h.ah = 0x45;                   // Funktionsnummer: Pages freigeben
    regs.x.dx = handle;
    int86 (EMS_INT, &regs, &regs);
    /* EasyCODE > */
    return regs.h.ah;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ems_map */
/* EasyCODE F */
/* EasyCODE < */
//*************** ems_map ***************
// Funktion: Bildet eine der unter dem angegebenen Handle allokierten Seiten
//                   auf eine physikalische Seite im Page-Frame ab.
// Eing.-Par.: handle - das bei ems_alloc zurÅckgelieferte Handle
//                    logp - logische Page (0 bis n-1)
//                    physp - physikalische Page (0 bis 3)
// Return-Wert: FALSE bei einem Fehler, sonst TRUE

unsigned char ems_map (int handle, int logp, unsigned char physp)
/* EasyCODE > */
{
    /* EasyCODE < */
    union REGS regs;
    
    regs.h.ah = 0x44;                   // Funktionsnummer: Mapping setzen
    regs.h.al = physp;
    regs.x.bx = logp;
    regs.x.dx = handle;
    int86 (EMS_INT, &regs, &regs);
    /* EasyCODE > */
    /* EasyCODE < */
    // Handle zurÅckliefern
    /* EasyCODE > */
    return (!(regs.h.ah));
}

/* EasyCODE ) */
/* EasyCODE ( 0 
safdat_check */
/* EasyCODE F */
/* EasyCODE < */
//*************** safdat_check ***************
// Funktion: Es wird die Word-Checksumme der netzausfallsicheren Daten
//           gebildet und in saf->safdatsum abgespeichert. Anschliessend
//           wird saf->safdatsum_gueltig auf TRUE gesetzt.

void safdat_check (void)
/* EasyCODE > */
{
    unsigned int i;
    
    saf->safdatsum_gueltig = FALSE;
    saf->safdatsum = 0;
    for ( i=0; i < (sizeof(struct safdat_struct)+1)/2; i++ )
    {
        saf->safdatsum += *(((unsigned int *) saf) +i);
    }
    saf->safdatsum_gueltig = TRUE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
axtoi */
/* EasyCODE F */
int axtoi(char *arg)
{
    unsigned int i = 0;
    int adres = 0;
    while ( arg && *arg && isxdigit(*arg) )
    {
        i = *arg++ - '0';
        if ( i >= 9 )
        {
            i -= 7;
        }
        adres <<= 4;
        adres |= (i & 0x0f);
    }
    return (adres);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
textload */
/* EasyCODE F */
/* EasyCODE < */
//*************** textload ***************
// Funktion: Liest die durch sprache ausgewÑhlte Textdatei in den Heap.
//           Bei einem Fehler wird textload_err entsprechend gesetzt.

void textload (void)
/* EasyCODE > */
{
    int handle;
    long length;
    unsigned int i;
    unsigned char * (*ptr);
    switch ( saf->md.sprache )
    {
    case 0 :
        handle = open ("c:\\dtexte.bin", O_RDONLY | O_BINARY);
        break;
    case 1 :
        handle = open ("c:\\etexte.bin", O_RDONLY | O_BINARY);
        break;
    case 2 :
        handle = open ("c:\\ftexte.bin", O_RDONLY | O_BINARY);
        break;
    case 3 :
        handle = open ("c:\\stexte.bin", O_RDONLY | O_BINARY);
        break;
    case 4 :
        handle = open ("c:\\itexte.bin", O_RDONLY | O_BINARY);
        break;
    }
    if ( handle == -1 )
    {
        close (handle);
        // Textdatei l‰sst sich nicht ˆffnen
        textload_err = 1;
    }
    else
    {
        length = filelength (handle);
        if ( (unsigned int)read (handle, tz, length) != length )
        {
            close (handle);
            // Fehler beim Lesen des Bin‰rfiles
            textload_err = 2;
        }
        else
        {
            ptr = (unsigned char * *) tz;
            for ( i=0; i < (sizeof (struct text)  / sizeof (tz->textanf)); i++ )
            {
                *ptr = FP_OFF(*ptr) + (unsigned char *) tz;
                ptr ++;
            }
            close (handle);
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
com1_com3_init */
/* EasyCODE F */
void com1_com3_init (void)
{
    if ( saf->jigctrf==NOCODE )
    {
        #ifdef com3
            InitPort (COM3,9600,PARITY_ODD,1,8);
            SetProtocol (COM3,RTSCTS,2,1);
            AllocateCOMBuffers (COM3,80,10);	// Foko,24.09.09
            EnableFIFO (COM3,0);
            //SetIRQ (COM3,12); // Mobs mit Com Port Karte  
            SetIRQ (COM3,11);   // Mobs mit 4 Com Ports intern
        #endif 
    }
    else
    {
        if ( saf->jigctrf==BARCODE )
        {
            #if !defined(remote)
                InitPort (COM1,9600,PARITY_ODD,1,8);
                SetProtocol (COM1,RTSCTS,2,1);
                AllocateCOMBuffers (COM1,80,10);	
                EnableFIFO (COM1,0);
            #elif defined(com3)
                InitPort (COM3,9600,PARITY_ODD,1,8);
                SetProtocol (COM3,RTSCTS,2,1);
                AllocateCOMBuffers (COM3,80,10);	// Foko,24.09.09
                EnableFIFO (COM3,0);
                SetIRQ (COM3,12); // Mobs mit Com Port Karte  
                //SetIRQ (COM3,11);   // Mobs mit 4 Com Ports intern
            #endif 
            schabl.rfiden = FALSE;
        }
        else
        {
            if ( saf->jigctrf==STATBARC )
            {
                //SW005-1
                #if !defined(remote)
                    InitPort (COM1,9600,PARITY_NONE,1,8); //Datalogic DS2210, stat. Barcodescanner
                    SetProtocol (COM1,NoProtocol,2,1);
                    AllocateCOMBuffers (COM1,80,10);		// Allocate receive and send buffers for the port
                    EnableFIFO (COM1,8);				// 0 = FIFO buffer disabled.
                    RTKClearMailbox (SendBuffer[COM1]);
                    RTKClearMailbox (ReceiveBuffer[COM1]);
                    EnableCOMInterrupt (COM1,1);                 // wegen AllocateCOMBuffers ist der 2. Parameter ohne Bedeutung
                #elif defined(com3)
                    InitPort (COM3,9600,PARITY_NONE,1,8);  // Test, folgende Zeile gehˆrt hier eigentlich hin
                    //InitPort (COM3,9600,PARITY_ODD,1,8);
                    SetProtocol (COM3,NoProtocol,2,1);
                    AllocateCOMBuffers (COM3,80,10);	// Foko,24.09.09
                    EnableFIFO (COM3,8);
                    //EnableFIFO (COM3,0);
                    //SetIRQ (COM3,12); // Mobs mit Com Port Karte  
                    SetIRQ (COM3,11);   // Mobs mit 4 Com Ports intern
                    
                    //folgende zeilen sollten wieder weg
                    RTKClearMailbox (SendBuffer[COM3]);
                    RTKClearMailbox (ReceiveBuffer[COM3]);
                    EnableCOMInterrupt (COM3,1);
                #endif 
            }
            else
            {
                if ( !schabl.rfiden )
                {
                    #if !defined(remote)
                        InitPort (COM1,9600,PARITY_NONE,1,8);  // Default f¸r ARYGON RS232 Reader
                        SetProtocol (COM1,RTSCTS,2,1);	      // HW Handshake disable
                        AllocateCOMBuffers (COM1,80,10);		// Allocate receive and send buffers for the port
                        EnableFIFO (COM1,8);				// 0 = FIFO buffer disabled.
                        RTKClearMailbox (SendBuffer[COM1]);
                        RTKClearMailbox (ReceiveBuffer[COM1]);
                        EnableCOMInterrupt (COM1,1);                 // wegen AllocateCOMBuffers ist der 2. Parameter ohne Bedeutung
                    #elif defined(com3)
                        InitPort (COM3,9600,PARITY_NONE,1,8);  // Default f¸r ARYGON RS232 Reader
                        SetProtocol (COM3,RTSCTS,2,1);	      // HW Handshake disable
                        AllocateCOMBuffers (COM3,80,10);		// Allocate receive and send buffers for the port
                        EnableFIFO (COM3,8);				// 0 = FIFO buffer disabled.
                        
                        SetIRQ (COM3,12); // Mobs mit Com Port Karte  
                        //SetIRQ (COM3,11);   // Mobs mit 4 Com Ports intern
                        
                        RTKClearMailbox (SendBuffer[COM3]);
                        RTKClearMailbox (ReceiveBuffer[COM3]);
                        EnableCOMInterrupt (COM3,1);                 // wegen AllocateCOMBuffers ist der 2. Parameter ohne Bedeutung
                    #endif 
                    schabl.rfiden = TRUE;
                }
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
