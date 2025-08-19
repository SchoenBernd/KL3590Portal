

(saf->md.jigctren == 1)
109  0 = aus,  1 = schabl �berw.   2 = automatisch program..
(e79.getZustand() || e80.getZustand()) && (saf->md.jigctren == 1)


e80	Schablonen eingelegt rechts
e79	Schablonen eingelegt links

y51.1	Schablonenverriegelung auf  Impulsventil     Out3 
y51.2	Schablonenverriegelung zu   Impulsventil     Out4

e51.2_1	Schablonenverriegelung geschlossen rechts
e51.2_2	Schablonenverriegelung geschlossen links


Fehler  Y51.2(1)
Ursache E51.2_1(1)



koor.nae_err = 40;
nae.errpar = E80_UF1;
key = 0;


tio9.rsz ();
tio9.sz (saf->md.fza_time);

tio9.tz()




Fadenschneiden einleiten

hfuss.ab();
tempkoord = akt_posunten;
istpos = tempkoord;

getpos liefert   istpos



hfuss.akt_posunten.pos
hfuss.akt_posunten.pos




TNAEHENAUS
AB_NAEHAUS  #define    AB_NAEHAUS         0x65      /* NAEHEN AUS */
eing.prog.pzbl.faden = FALSE;
ds

KlasseHuepferfuss::auf

hfuss_auf.setAuftrag();
void shfuss_auf (void)
hfuss.auf_init()


Der Stoffgegedr�cker ist an dieser Portal mechanisch gekoppelt,
auf den Ventil Y33 ist sind Stopfen drin.

Wenn die Nadel oben ist ist der Stoffgegendr�cker unten

Fadenleger OT von 75 auf 50 Grad ge�ndert.


Problem:
========
Bei Kundenvorf�hrung blieb die 3590 Portal sporadisch im Stop stehen
und nix ging mehr ohne Fehlermeldung.

Bedingung:
==========
Wenn mitten im Bild ein Zwischenstopp ohne Fadenschneiden programmiert ist und
danach ein Eilgang folgt, dann kommen wir sporadisch in das Problem rein.

Da das Problem schon l�nger bekannt war und es jetzt auch vor Kunden aufgetauchte,
habe ich mich der Sache in einer Sp�tschicht mal angenommen.

Analyse:
========
An dieser Portal, ist der Stoffgegendr�cker, so wie es aussieht, mechanisch gekoppelt
mit der Greiferwelle.
Einmal ist das Ventil Y33 mit Stopfen versehen und zum anderen war der Paramter
201 auf Stoffgegendr�cker auf mechanisch gekoppelt gesetzt.

Im Zwischenstopp wird die Nadel auf Fadenleger OT positioniert (nicht OT !!!)
Danach wird der Stoffgegendr�cker runtergeschaltet, was aber wegen dem 
Parameter 201 und wegen der mechanischen Kopplung nicht wirkt.
Macht aber nix, wenn der Ausgang dennoch geschalten wird, da das dann ins leer geht.
A b e r  der Initiatoreingang E33 (Stoffgegendr�cker unten) wird i m m e r 
gepr�ft vor fahren des Schlittens.
Wahrscheinlich wollte Wilfried damit sicher stellen, das niemals der Rahmen
�ber den Stoffgegendr�cker f�hrt, wenn er noch oben ist. Egal ob elektrisch 
(Ventil) oder mechanisch angetrieben.
Eine Fehlermeldung gibt es auch nicht, da ja In "Ausg�nge Schalten" der
Verriegelungsscheck abh�ngig vom Parameter 201 ausgeschaltet ist.
Wenn wir physikalisch nichts schalten, dann kann es auch keinen Fehler geben.

Im Service hat man gesehen, dass bei Fadenleger OT Position der Initiator gerade
schaltet aber meistens nicht mehr. Zum Test hatte ich mal die Fadenleger OT 
Postion auf 50 Grad anstelle 75 gesetzt und dann ging es immer.


L�sungsm�glichkeiten:
=====================
Ich habe bis jetzt noch nicht den Code g�ndert, da ich nicht weis
was die richtige L�sung w�re.

L�sungsm�glichkeiten:
a) Abh�ngig vom Parameter 201 pr�fe in diesem Fall E33 nicht mehr ab,
   das k�nnte aber irgendwann zum Stoffgegendr�cker Abriss f�hren.
   
b) Die Fadenleger OT Postition m��te kleiner gew�hlt werden, das geht
   bestimmt N�htechnisch nicht.
   
c) Im Bereich von OT bis Fadenleger OT mu� die Kurbelwelle so sein, dass 
   der Stoffgegendr�cker unten ist und der Initiator sicher bed�mpft wird.

d) Der Initator wird soweit verschoben, sodass er sicher bed�mpft ist
   im Falle OT bis max. Fadenleger OT Wert.

c) In diesem Fall Positioniere ich nicht auf Fadenleger OT sondern auf
   OT. W�re das erlaubt ?????
   
   
Was nun ?











portal.bat exe 120x170


y10 Bandanschlag ab Maschinenvariante 1
y50u2.setAuftrag();  // Oberteil senken
y8.setAuftrag();     // Niederhalter ab  (offen neu)
out3.setAuftrag();   // Banzufuehrung ab (neu)




Wir sprechen ja von der Portal gro� 120 x 170:

Die �nderung von Wilfried w�re in der Version 79-001 104-30/w02 gemacht. 
Bitte per Info auslesen ob der Softwareupdate geklappt hat.


Wenn es zum Kaltstart oder BS3 Absturz gekommen ist, dann k�nnten auch alle
Parameter gel�scht worden sein.
Dann m�ssen diese von Herberts oder von Deiner SD Karte wieder geladen werden.
(M-DAT und Konf-DAT)



Damit die �nderungen von Wilfried �berhaupt greifen mu� folgender Parameter sitzen:
Maschinenvariante" (saf->variante) Paramter 203 = 1. (default w�re 0 !)
(Prog. Stop zum Band einlegen.)


Nach SW Update und stimmigem Parameter kommt dann immer der Fehler:
Fehler 242 H�pfer ab verrriegelt Ursache Y8(1) (Niederhalter ab)






