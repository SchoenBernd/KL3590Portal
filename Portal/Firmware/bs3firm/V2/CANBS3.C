/******************************************************************************/
/* Datei:        canbs3.c                                                     */
/* Ersteller:    Wilfried Halfmann   ESE 2856                                 */
/*                                                                            */
/* In diesem Programm werden bis zu 5 CAN-Knoten mit 16 Ein- und 16 Ausgängen */
/* behandelt. Außerdem können die Ausgänge gechoppert werden. Diese Konfigu-  */
/* ration wird bei der Kl3588 benutzt.                                        */
/******************************************************************************/

#include "dprbs3.h"
#include "mainbs3.h"

// Defines
#define low(wert) *((unsigned char *)&wert)
#define high(wert) *((unsigned char *)&wert + 1)

// Daten
unsigned char sdata CANKey, sdata CANAblKey, sdata KnotenTriggerCnt;
TCAN_Obj sdata Out16, sdata In16, sdata OutMode;

// Prototypen
void CANInit (void);
void CANTask (void);
void RequestInp (void);
void ReCheckInp (void);
void TransmitOut (void);
void SetOutMode (void);

#pragma eject
/***************************** CANInit ****************************************/
/*                                                                            */
/* Das Up initialisiert die Daten des Can-Interfaces im Dual-Port-Ram und     */
/* setzt den CANKey auf 2. Das Up wird beim Einschalten aufgerufen.           */
/*                                                                            */
/******************************************************************************/

void CANInit (void)
{
   unsigned char *workptr, i;

   for (workptr = (unsigned char *) CAN; workptr < &CAN->version; workptr++) *workptr = 0;
   CAN->version = 1;
   CAN->adr = 0;
   #ifdef FIRM480               
     CAN->maxknotennr = 3;  // war  =8 im Fall alter 480 Firmware. Machte aber kein Sinn.
   #else                           
     CAN->maxknotennr = 3;
   #endif           
   CAN->start = 0;
   CAN->chmakt = 0;
   CAN->nu_5 = 0;
   CAN->reset = 0x33;
   CAN->akterror = 0xff;
   CAN->error = 0xff;
   CAN->errorstat = 0;
   for (i=0; i<5; i++) CAN->nu_6 [i] = 0;
   for (i=0; i<8; i++) CAN->choppermask [i] = 0;
   for (i=0; i<8; i++) CAN->startzt [i] = 500;
   for (i=0; i<8; i++) CAN->auszt [i] = 10;
   for (i=0; i<8; i++) CAN->einzt [i] = 10;
   CANKey = 2;
}


/***************************** CANRes *****************************************/
/*                                                                            */
/* Das Up initialisiert die Daten des Can-Interfaces im Dual-Port-Ram, die    */
/* nicht vom PC104 initialisiert werden und setzt den CANKey auf 0. Das Up    */
/* wird bei einem Software-Reset vom PC104 aufgerufen.                        */
/*                                                                            */
/******************************************************************************/

void CANRes (void)
{
   unsigned char *workptr, i;

   for (workptr = (unsigned char *) CAN; workptr < &CAN->version; workptr++) *workptr = 0;
   CAN->version = 1;
   CAN->adr = 0;
   CAN->start = 0;
   CAN->chmakt = 0;
   CAN->nu_5 = 0;
   CAN->akterror = 0xff;
   CAN->error = 0xff;
   CAN->errorstat = 0;
   for (i=0; i<5; i++) CAN->nu_6 [i] = 0;
   CANKey = 0;
}
#pragma eject
/***************************** CANTask ****************************************/

void CANTask (void)
{
   unsigned char i,u;

   switch (CANKey)
   {
      case 0:
         CAN_vInit ();                 // CAN-Baustein und Objekte initialisieren
         CAN->reset = 0x55;
         KnotenTriggerCnt = 0;
         CANKey = 1;
         CANAblKey = 0;
         break;

      case 1:
         if (CANFehler != 0)
         {
            CAN->akterror = CAN->adr;
            if (CAN->error == 0xff)
            {
               CAN->error = CAN->adr;
               CAN->errorstat = 1;               // nicht aktiv
            }
            CANKey = 0;
         }
         else
         {
            if (CAN->reset != 0x55) CANRes();
            else
            {
               switch (CANAblKey)
               {
                  case 0:
                     if (CAN->start == 0)
                     {
                        CAN->start = 0xff;
                        KnotenTriggerCnt = 0;
                        RequestInp();
                        CANAblKey = 1;
                     }
                     else if (CAN->chmakt == 0)
                     {
                        CAN->chmakt = 0xff;
                        KnotenTriggerCnt = 0;
                        SetOutMode ();
                     }
                     else
                     {
                        for (i=0,u=0; i<CAN->maxknotennr; i++)
                        {
                           if ((CAN->status[i] & 0x01) == 0x01) u++; // Knoten aktiv ?
                        }
                        CAN->anzknoten = u;
                        if (++KnotenTriggerCnt == 50) CAN->start = 0;
                     }
                     break;

                  case 1:
                     ReCheckInp ();    // Eingangsknoten lesen und prüfen
                     CANAblKey = 2;
                     break;

                  case 2:
                     TransmitOut ();   // Ausgangsknoten senden
                     CANAblKey = 0;
                     break;
               }
            }
         }
         break;

      case 2:
         CAN_vInit ();                 // CAN-Baustein und Objekte initialisieren
         KnotenTriggerCnt = 0;
         CANKey = 3;
         CANAblKey = 0;
         break;

      case 3:
         if (CAN->reset == 0xaa)
         {
            CAN->reset = 0x55;         // PC104 hat Initialisierungswerte in
            CANKey = 1;                // DPR geschrieben ---> loslaufen
         }
         break;
   }
}
#pragma eject
/***************************** RequestInp *************************************/
/*                                                                            */
/* Das Up triggert das Senden eines Remote-Frame der Empfangsobjekte und      */
/* setzt Bit4 (Warten auf Input-Objekt) im Status des entsprechenden Knotens. */
/*                                                                            */
/******************************************************************************/

void RequestInp (void)
{
   unsigned char i;

   for (i=1; i<=CAN->maxknotennr; i++)
   {
      CAN_vTransmit(i+5);
      CAN->status[i-1] |= 0x10;
   }
}



/***************************** ReCheckInp *************************************/
/*                                                                            */
/* Das Up liest die Eingänge und die Abbilder der Ausgänge aller Knoten und   */
/* schreibt sie in das Dual-Port-Ram. Falls von einem Knoten 2 mal keine Bot- */
/* schaft empfangen wird, wird der Knoten passiv geschaltet und ein Fehler im */
/* Dual-Port-Ram gemeldet.                                                    */
/*                                                                            */
/******************************************************************************/

void ReCheckInp (void)
{
   unsigned char i, workstat;

   for (i=1; i<=CAN->maxknotennr; i++)
   {
      if (CAN_vGetMsgObj(i+5, &In16))
      {
         low(CAN->in[i-1]) = In16.ubData[1];     // Eingangs-Abbild in DPR
         high(CAN->in[i-1]) = In16.ubData[0];
         low(CAN->out_ist[i-1]) = In16.ubData[4]; // Istwerte der Ausgänge in DPR
         high(CAN->out_ist[i-1]) = In16.ubData[3];
                                         // Fehlerbit und Aktiv-Bit setzen
                                         // Receive Status und Time out rücksetzen
         workstat = CAN->status[i-1];
         workstat = ((workstat & ~0x80) | (In16.ubData[2] & 0x80)) & 0xed | 0x01;
         CAN->status[i-1] = workstat;
         if ((workstat & 0x80) != 0)     // Sitzt das Fehlerbit ?
         {
            CAN->akterror = i;
            if (CAN->error == 0xff)
            {
               CAN->error = i;
               CAN->errorstat |= 0x80;
            }
         }
      }
      else
      {
         if ((CAN->status[i-1] & 0x02) == 0x02)  // Sitzt das Time out - Bit ?
         {
            if ((CAN->status[i-1] & 0x01) == 0x01)    // Knoten aktiv ?
            {
               CAN->status[i-1] &= ~0x01;        // Knoten nicht mehr aktiv
               CAN->akterror = i;
               if (CAN->error == 0xff)
               {
                  CAN->error = i;
                  CAN->errorstat |= 0x01;
               }
            }
         }
         else CAN->status[i-1] |= 0x02;          // Time out - Bit setzen
      }
   }
}
#pragma eject
/***************************** TransmitOut ************************************/
/*                                                                            */
/* Das Up sendet die Ausgangs-Sollwerte aller Knoten über das Sendeobjekt 1.  */
/* Können die Werte eines Knotens nicht gesendet werden, so wird das          */
/* TransmitStatus-Bit im DPR-Status des entsprechenden Knotens gesetzt.       */
/*                                                                            */
/******************************************************************************/

void TransmitOut (void)
{
   unsigned char i, TimeOutCheck;

   for (i=1; i<=CAN->maxknotennr; i++)
   {
      TimeOutCheck = 0;
      Out16.ubMsgCfg = 0x28;  // 2 Datenbytes, DIR = transmit, standard messages
      Out16.ulArbitr = 0x132 + i;
      Out16.ubData[1] = low(CAN->out_soll[i-1]);
      Out16.ubData[0] = high(CAN->out_soll[i-1]);
      while ((CAN_bRequestMsgObj(i)==0) && (TimeOutCheck<10)) TimeOutCheck++;
      if (TimeOutCheck >= 10) CAN->status[i-1] |= 0x08; // Transmit-Status setzen
      else
      {
         CAN_vConfigMsgObj (i, &Out16);
         CAN_vTransmit (i);
         CAN->status[i-1] &= ~0x08;       // Transmit-Status rücksetzen
      }
   }
}



/***************************** SetOutMode *************************************/
/*                                                                            */
/* Das Up sendet die Ausgangs-Sollwerte aller Knoten über das Sendeobjekt 1.  */
/* Können die Werte eines Knotens nicht gesendet werden, so wird das          */
/* TransmitStatus-Bit im DPR-Status des entsprechenden Knotens gesetzt.       */
/*                                                                            */
/******************************************************************************/

void SetOutMode (void)
{
   unsigned char i, TimeOutCheck;

   for (i=1; i<=CAN->maxknotennr; i++)
   {
      OutMode.ubMsgCfg = 0x68;  // 6 Datenbytes, DIR = transmit, standard messages
      OutMode.ulArbitr = 0x232 + i;
      OutMode.ubData[1] = low(CAN->choppermask[i-1]);
      OutMode.ubData[0] = high(CAN->choppermask[i-1]);
      OutMode.ubData[3] = low(CAN->startzt[i-1]);
      OutMode.ubData[2] = high(CAN->startzt[i-1]);
      OutMode.ubData[4] = CAN->auszt[i-1];
      OutMode.ubData[5] = CAN->einzt[i-1];
      TimeOutCheck = 0;
      while ((CAN_bRequestMsgObj(11)==0) && (TimeOutCheck<100)) TimeOutCheck++;
      if (TimeOutCheck >= 100) CAN->status[i-1] |= 0x04; // Transmit-Status setzen
      else
      {
         CAN_vConfigMsgObj (11, &OutMode);
         CAN_vTransmit (11);
         CAN->status[i-1] &= ~0x04;       // Transmit-Status rücksetzen
      }
   }
}

