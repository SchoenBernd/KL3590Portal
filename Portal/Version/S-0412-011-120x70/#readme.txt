3590 Portal   79-001 104-30/xxx und  79-001 104-12/xxx
===========

�nderungen:

20.11.2013
SW002 / SW008
- Nach Spulenwechsel und Start gab es keinen Zwischenstop zum Bandeinlegen nach dem Kopf senken.
- Korrektur von Wilfries Aenderungen bez. Bandanschlag noetig:
- Es gab auch eine Umverdrahtung zur Erfuellung der Bandanschlag Funktion:

alt ohne Bandanschlag            neu mit Bandanschlag Ventil wurde umverwendet:
------------------------------------------------------------------------------
Y9 Oberfaden r�ckziehen          zu Bandzufuehrung links rechts
                                 Prog Out4 (Can 2 out16)
------------------------------------------------------------------------------
Y7 Nadeldreheinrichtung ein      zu Bandanschlag (Krokodilklemme oeffner)
                                 Y10 (Can 1 out16)
------------------------------------------------------------------------------
Y8 Niederhalter ab               Neuer Niederhalter mit Rolle ab
                                 prog. Out3 (Can 2 out15).
                                 prog. Ausgang, damit man ihn vor Ende der Naht 
                                 heben kann.

- Nach Notaus, Power On und auf unterbrochenen Punkt weiter Naehen gab es einen Fehler im Fadenschneiden 
  und die Hupferfusspositionen waren geloescht.
  
  
  
  
  
  
------------------------------------------------------------------------------
  
14.03.2014
Version SW009 

- Fehler 224 Huepferfussinitiator nicht gefunden im Fall wenn der H�pfer Schritte 
  verliert hat und dann beim Schneiden die Nadelstange ber�hrte.
  Die Zeit von 500 ms und sogar 2000ms reichen dann nicht aus f�r die Referenzfahrt. 
  Diese ist jetzt auf 4000ms gesetzt.
- Nach dem Fehler 224 blieb immer der Auftrag f�r H�pferfuss hoch gesetzt, was dazu 
  f�hrte, dass Ablauftasten wie Start und Takten unendlich lange gewartet haben bis der 
  Auftrag zur�ckgesetzt wird.
  Das R�cksetzen ist aber nie passiert und es schien als ob das BDFP1 h�ngt.
  Also L�sung wird jetzt in der Referenzfahrt der Auftrag immer gel�scht und die 
  Tasten gehen wieder.

------------------------------------------------------------------------------
 
12.06.2014
Version 79-001 104-12 / 010   bzw.  79-001 104-30 / 003

- Nach Software Update mit Wunsch, Rampen f�r schweres Material laden, kommt nach dem 
  Softwareupdate und dem anschliessendem kopieren von MDAT mit schweres Material, kein 
  Hinweis eines Ausschaltfehlers und es wird die Rampe fuer leichtes Material geladen. 
  Loesung: Nach dem kopieren von MDAT wird nun geprueft ob sich md.matdicke veraendert hat 
  und es wird entsprechend darauf reagiert.
- Parameter 401 ... 406 Rollgrenze max. von 1000 (10s) auf 256 (2,56 s) geaendert so wie 
  in Listen.doc.
- Im Falle, mechanisch gekoppelter Stoffgegendr�cker (201 = 0) und programmierter Stopp ohne 
  Schneiden im Nahtbild und danach Eilgang zum Faden legen, dann blieben wir ohne Fehlermeldung 
  stehen.
  Als L�sung wird die Eingangsabfrage e33 getZustand abhaengig vom Parameter 201 gemacht und 
  erst in der spaeteren Verriegelungspruefung gepr�ft, damit eine Fehlermeldung durch kommt. 
- Im Schneidablauf wurde der Niederhalter und Huepferantrieb in etwa zeitgleich runtergeschaltet.  
  Jedoch im Huepferantriebablauf wird geprueft ob der Niederhalter fertig ist. Wenn dies nicht 
  der Fall ist, faehrt der Huepferantrieb nicht und die Schrittmotorbewegung ist sofort fertig.
  Jedoch die Position ist falsch. 
  Als Loesung wird nun im "Fertig Check" auch nach die aktuelle Position gepr�ft. Wenn falsch,
  dann wird der H�pferantrieb nochmal runter gefahren.
- Im Fall programmierter Stopp ohne Schneiden im Nahtbild und danach Eilgang zum Faden legen, ging die 
  Fadenspannung nicht auf, dies war falsch.
  
------------------------------------------------------------------------------

23.07.2014
Version 79-001 104-12 / 011   bzw.  79-001 104-30 / 004
- Stop im Nahtbild und anschliessend Home fuehrt zum endlos haengen wenn im Nahtbild die Huepferfussdaten,
  Niveau und Hub, aus den Parametern 150 und 151 verwendet werden. Dies ist eine Folge der Korrektur von
  SW010.
  Die L�sung in SW010 bezueglich dem neuen Fertig Check im Schneidablauf war schon richtig, jedoch 
  gibt es zwei Positionen f�r unten (saf->md.posunten.pos und hfuss.akt_posunten.pos) jenachdem ob die
  Daten aus den Huepferfussparameter 150 und 151 gerade aktiv sind oder nicht.
  In hfuss.akt_posunten.pos steht immer der richtige Wert, welcher nun verwendet wird um das Problem zu
  loesen.
