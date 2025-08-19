/* EasyCODE V8 */
/* EasyCODE ( 0 
Portalvers.c,   Software-Teilenummer */
// Definition eines Strings fuer die Softwareteilenummer der KL3590-Portal
// mit Bs3 und Bedienfeld P1
/*

Basis war Version 007  bzw. 001

20.11.2013
SW    Aenderungen markiert mit SW002

- Nach Spulenwechsel und Start gab es keinen Zwischenstop zum Bandeinlegen nach dem Kopf senken.

- Korrektur von Wilfries Aenderungen bez. Bandanschlag noetig
Zur Info
alt ohne Bandanschlag            neu mit Bandanschlag Ventil wurde umverwendet:
------------------------------------------------------------------------------
Y9 Oberfaden rückziehen          zu Bandzufuehrung links rechts
                                 Prog Out4 (Can 2 out16)
------------------------------------------------------------------------------
Y7 Nadeldreheinrichtung ein      zu Bandanschlag (Krokodilklemme oeffner)
                                 Y10 (Can 1 out16)
------------------------------------------------------------------------------
Y8 Niederhalter ab               Neuer Niederhalter mit Rolle ab
                                 prog. Out3 (Can 2 out15).
                                 prog. Ausgang, damit man ihn vor Ende der Naht 
                                 heben kann.

- Nach Notaus im Naehen gab es einen Fehler im Fadenschneiden und die Hupferfusspositionen waren geloescht.


14.03.2014
Version SW009 

- Fehler 224 Huepferfussinitiator nicht gefunden im Fall wenn der Hüpfer Schritte verliert hat
  und dann beim Schneiden die Nadelstange berührte.
  Die Zeit von 500 ms und sogar 2000ms reichen dann nicht aus für die Referenzfahrt. 
  Diese ist jetzt auf 4000ms gesetzt.
- Nach dem Fehler 224 blieb immer der Auftrag für Hüpferfuss hoch gesetzt, was dazu führte, dass
  Ablauftasten wie Start und Takten unendlich lange gewartet haben bis der Auftrag zurückgesetzt wird.
  Das Rücksetzen ist aber nie passiert und es schien als ob das BDFP1 hängt.
  Also Lösung wird jetzt in der Referenzfahrt der Auftrag immer gelöscht und die Tasten gehen wieder.
 
24.03 ... 06.06.2014
Version SW010   
- Nach Software Update mit Wunsch, Rampen für schweres Material laden, kommt nach dem Softwareupdate und
  dem anschliessendem kopieren von MDAT mit schweres Material, kein Hinweis eines Ausschaltfehlers und es 
  wird die Rampe fuer leichtes Material geladen. 
  Loesung: Nach dem kopieren von MDAT wird nun geprueft ob sich md.matdicke veraendert hat und es wird
  entsprechend darauf reagiert.
- Parameter 401 ... 406 Rollgrenze max. von 1000 (10s) auf 256 (2,56 s) geaendert so wie in Listen.doc.
- Im Falle, mechanisch gekoppelter Stoffgegendrücker (201 = 0) und programmierter Stopp ohne Schneiden 
  im Nahtbild und danach Eilgang zum Faden legen, dann blieben wir ohne Fehlermeldung stehen.
  Als Lösung wird die Eingangsabfrage e33 getZustand abhaengig vom Parameter 201 gemacht und erst in 
  der spaeteren Verriegelungspruefung geprüft, damit eine Fehlermeldung durch kommt. 
- Im Schneidablauf wurde der Niederhalter und Huepferantrieb in etwa zeitgleich runtergeschaltet.  
  Jedoch im Huepferantriebsablauf wird geprueft ob der Niederhalter fertig ist. Wenn dies nicht der
  Fall ist, faehrt der Huepferantrieb nicht und die Schrittmotorbewegung ist sofort fertig.
  Jedoch die Position ist falsch. 
  Als Loesung wird nun im "Fertig Check" auch nach die aktuelle Position geprüft. Wenn falsch,
  dann wird der Hüpferantrieb nochmal runter gefahren.
- Im Fall programmierter Stopp ohne Schneiden im Nahtbild und danach Eilgang zum Faden legen, ging die 
  Fadenspannung nicht auf was falsch ist.

Version SW011   23.07.2014
- Stop im Nahtbild und anschliessend Home fuehrt zum endlos haengen wenn im Nahtbild die Huepferfussdaten,
  Niveau und Hub, aus den Parametern 150 und 151 verwendet werden. Dies ist eine Folge der Korrektur von
  SW010.
  Die Lösung in SW010 bezueglich dem neuen Fertig Check im Schneidablauf war schon richtig, jedoch 
  gibt es zwei Positionen für unten (saf->md.posunten.pos und hfuss.akt_posunten.pos) jenachdem ob die
  Daten aus den Huepferfussparameter 150 und 151 gerade aktiv sind oder nicht.
  In hfuss.akt_posunten.pos steht immer der richtige Wert, welcher nun verwendet wird um das Problem zu
  loesen.

Version SW012   17.06.2016
- Im Service Huepferfuss Nullpunkteinstellen war die Werte Anzeige falsch im Falle Rollen 
  von -10 auf -9. Das 0 Zeichen der -10 wurde nicht gelöscht und es erschien -90.
  Anzeige ist nun korrigiert.

Version SW013   27.09.2016
- Wenn einer der Schablonenverriegelungen Sensoren nicht betaetigt wurde wegen schief einlegen der Schablone,
  dann ist es nicht mehr moeglich die Schablonenverriegelung zu oeffnen. Fehler wurde richtig angezeigt, doch
  der Ausgangablauf wurde nie fertig (Totverriegelung). Als Loesung oeffnet nun die Verriegelung sofort.

- Wenn man im Autostart die Schablone sehr langsam einlegt, dann kommt es vor wegen der falschen Position der
  "Schablone eingelegt" Initiatoren, dass die Schablone erkannt wird obwohl noch gar nicht ganz eingeschoben !
  Bei der 3587 und allen 3590 sind die Initiatoren für Schablone eingelegt hinten was richtig ist. 
  Nur bei der Portal sind diese vorne angeordnet und erkennen schon die Alukante der Schablonenbefestigung als 
  gültig eingelegt was falsch ist.
  Per Software ist das Problem nicht zu lösen. Als Software Work around wird das Auftreten des Problems 
  etwas abgeschwaecht durch folgende zwei Massnahmen:  
  a) Verzögerungstimer zwischen Schablone eingelegt und schliessen der Schablonenverriegelung.
  b) Nach dem Fahren auf Anfangspunkt wird nochmal geprüft ob die Schablone immer noch da ist, falls nicht dann gibt es 
     einen Nähabbruch mit Fehler.

Version SW001 Aenderungen markiert mit SW001DIS  Naehfeld 1200x700 05.01.2017 Decoupage Industriel Sarl   79-001 104 57/xxx
- Anbindung an die SPS wegen der Kunden Zu- und Abfuehrung (Karusell).  
 
Version SW002 Aenderungen markiert mit SW002DIS
- Bei Homefahrt immer auf Initiatoren fahren 
- In Ablauf Warten bei Clampstart, Wenn der Befehl zum Klammer auf kommt, dann auch im warten Klammer auf.

Version SW003 Aenderungen markiert mit SW003DIS
- Garnkonenueberwachung: Wenn CAN2 IN12 (1) oder CAN2 IN13 (2) Low werden und der neuer Parameter 153 auf ein steht dann 
  fuehrt dies zu einer einer Garkonenstoerung.
  
Version SW003 Aenderungen markiert mit SW004DIS
- RFID Test, neuer Reader Sangoma.
  
Version SW003 Aenderungen markiert mit SW005DIS
- Mit der Aenderung SW013 wird nicht angezeigt, welcher Initiator nicht schaltet bei Schablone schief eingelegt.
- Default Wert von Parameter 201 Stoffgegendrücker ändern von 1 auf 0.

Version SW004 Aenderungen markiert mit SW006DIS
- Nur im automatic Betrieb für Decoupage Industriel Sarl nach jedem Naehen Referenzfahrt.
- Wenn 109 Schablonenüberwachung = 0, dann nicht RFID lesen starten beim Start, sonst lesen wir waehrend der Fahrt und
  der RFID Ablauf geht auf Fehler 7, was in diesem Fall unnoetig ist.

20.01.2018 ... 20.10.2018  und  alle Funktionen der Fernsteuerung und Garnkonenueberwachung auch mit drin.
Version SW014
- Wenn Oberteil gehoben (an der Maschine programmiert) soll Faden- und Zusatzfadenspannung auf sein!   Wunsch Herr Mo China
- Default Wert der Nadelanzahl von 2 auf 1 geaendert, da die Einnadelmaschine unueblicher ist.     
- Wenn man im Programmieren ein falsches Prog erstellt und dann auf ESC drueckt, dann soll
  man nicht ausschalten muessen, sondern im eing Exit Konvert zur Prognummerwahl aufgefordert werden. 
- Schlingstichformeln zum Schalten der Nadeldreheinrichtung und Unterfadenschieber korrigiert.
  Die Formel muß gelten fuer Quadrant 1 und 4 und nicht nur fuer Quadrant 4.  
  Anzahl Stiche ohne Schalten der Dreheinrichtung von >= 3 auf >= 4 erhoeht.   gefunden won KB und MI fuer Draexlmeier
- Im Service Menue Huepferfuss Hub und Niveau einstellen gab es einen Anzeigefehler mit der Einheit mm. Es wurde manchmal mmm angezeigt.
- Im freien programmieren konnte mittels der Aendern Taste nicht mehr die Stufe vom Huepferfuss Hub oder Niveau veraendern werden.
  Ein neuer Wert wurde nicht uebernommen.

07.02.2019
Version SW015
- Schlingstichwinkel als Parameter eingebbar gemacht. Neue Parameter 905 Quadrant 1 ein und 906 Quadrant 4 ein hinzugefuegt.
- Zusaetzlich zur Schlingstichwinkeleingabe ist auch eine Anzahl Vorhaltestiche fur die Nadeldreheinrichtung noetig geworden bei 
  hoeheren Drehzahlen. Neuer Parameter eigefuegt: 154 Vorhaltestiche Nadel Dreheinrichtung.
- Wenn Par. 109 Schablonenueberwachung auf 2 = autom. Prog. Nr. Wahl steht und man kopiert ein von PSP erstelltes Programm 
  in die Maschine, dann wird bei der Start Ausloesung durch das Einlegen des Rahmens ein Fehler 342 "Prog unvollstaendig" 
  angezeigt. (Loesung synchronisiert von der 3590 Serie).
- In dem Fall von oben wurde keine Stichlaenge angezeigt, da diese erst nach dem Programm vervollstaendigen da ist was erst 
  nach dem Start/Takten passiert.
  Es wird nun die Anzeige aktualisiert, was aber erst nach druecken der Start oder Takten Taste moeglich ist.
  Also die Stichlaenge wird nun etwas verzoegert angezeigt.
- Wenn man Programme, welche Teil von Sequenz- oder Verbundprogrammen sind, von PSP in die Maschine kopiert, dann wurde nur
  das erste Programm vervollstaendigt. Nun werden alle Programme einer Sequenz oder Verbund durchgegangen und wenn noetig 
  automatisch vervollstaendigt.
- Fadenklemme soll bei OFW Stoerung immer auf gehen, damit ein Einfaedeln, auch ohne Fahrt zur Servicepos. moeglich ist.
- Maxgeschwindigkeit von 1500 auf 2000 Upm erhoeht, da laut PFAFF Portal Prospekt 2000 Upm moeglich sein soll.
- Geheimfunktion Istdrehzahl Anzeige frei geschaltet. Zum Test nur für PFAFF Mitarbeiter mittels 901 = 2.


12.11.2019
Version SW016
- Nach dem Schneiden und Verfahren die Fadenspannung zu lassen.
  In der Korrektur von SW014 Herr Mo China, wurden vom Kunden die Luftschlaeuche fuer Fadenspg und Zusatzfadenspg verdreht.
  Somit war die Korrektur falsch, da wir alle verwirrt wurden, also Rueckgaengig machen.
- Spannung nur oeffnen, wenn: 
   am Nahtende kein Schneiden programmiert ist.
   143 Fadenwedeln Hüpferfuß oben auf ein steht (Dann aber Fadenklemme nach Parameter 306 schliessen).
   Zusatzfadenspannung in den Faellen auch oeffnen.

24.03.2021  
- durch ein Missverstaendnis (Roman) das Naehfeld NF120x200 (1200x2000) aus Sonder-Magna eingebaut.
  Wird bisher nicht gebraucht. (dennoch mal Software drin gelassen)
- Korrektur Anzeige Info fuer 1200x1700 (dadurck klportal.bat und portal.mak angepasst)

2022-03-25: SW005 fuer Firma Strecker:
-1: stationaeren Barcode-Scanner eingebaut
    Anmerkung zu SWb32: kommt aus dem 3590-Projekt: SW032 26.03.2020  markiert mit  SWb32 
    Aenderung fuer einen Autostart mit Handscanner, nur unter bestimmten Bedingungen!

2023-06-26:
- unsigned char tfaedelmem auskommentiert, weil wohl nicht verwendet
- SW18-1: Automatischer Spulenwechsel fuer Draexlmaier 
  jetzt erst bei Fadenschneiden nicht schon bei Naht Ende (z.B. Fadenziehen)
  Dazu muss der Parameter 101 (saf->md.ufwen) auf 5 gestellt sein - neu.

2023-01-10:
- SW18-2: fuer alle Naehfelder!: einmal Schneidzylinder betaetigen in firstHome (soll Haengen verhindern)

2024-12-05: Stand 1200x700 019 fuer GB wegen Schneidzylinder betätigen
          
*/
#ifdef NF120x70
    // Naehfeld 1200x700
    unsigned char * softversion = {"79-001 104-12/019"}; // Achtung KENNUNG des BDF muss mit 
    unsigned char * bs3version = {"79-001 104-13/004*"}; // * fuer Terminator
    unsigned char * bs3version_schwer = {"79-001 104-29/001*"}; // * fuer Terminator, für schwere Schablonen
#endif 
#ifdef NF120x170
    // Naehfeld 1200x1700
    unsigned char * softversion = {"79-001 104-30/c13"}; // Achtung !!! KENNUNG des BDF muss mit 
                                                         // geaendert werden
    unsigned char * bs3version = {"79-001 104-13/004*"}; // * fuer Terminator
    unsigned char * bs3version_schwer = {"79-001 104-29/001*"}; // * fuer Terminator, für schwere Schablonen
#endif 
#ifdef NF100x50
#endif 
#ifdef NF120x200
    // Naehfeld 1200x2000
    unsigned char * softversion = {"79-001 104-94/a01"}; // Achtung !!! KENNUNG des BDF muss mit 
                                                         // geaendert werden
    unsigned char * bs3version =        {"79-001 104-13/004*"}; // * fuer Terminator
    unsigned char * bs3version_schwer = {"79-001 104-29/001*"}; // * fuer Terminator, für schwere Schablonen
#endif //
/* EasyCODE ) */
