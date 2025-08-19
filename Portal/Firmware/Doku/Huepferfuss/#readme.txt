RTA CSD 92 SM Endstufe für Portal Anschluß an die BS3 bzw. Rampemmeßplatz
===========================================================================




Annahme: 4 Kg Last = ca. 40 N = 0,4 NM bei unserem Ritzel von r = 10mm.
Wegen Getriebeverluste wird benötigt ca. 0,8 NM.

Motor Type 103H6703-0440 Serie     1,6  Ohm/Phase DC 2A unipolar, 1,4A (= 2A / Wurzel2) dauer bipolar

Angabe RTA Homepage Parallel bipolar bei 400 rpm 0,38 NM soll weniger Drehmoment haben als 
Serie Bipolar 400 rpm 0,43 NM ist falsch da entgegen der Physik.

Angabe direkt bei Sanyo Denki ist viel seriöser, da Drehmomentangabe über 400 rpm sowieso unsinnig sind: 
Angabe Sanyo Denki: 100 rpm 0,2 NM, 400 rpm  0 Nm bei 24V Power.

Fazit: Aktueller Motor hat nur ca. 1/4 so viel Kraft wie gewünscht.



Motor Type 103H7123-1740 (etwas größere Bauform) Aufschrift DC 4A    Parallel Wicklung.

Angabe RTA Homepage: 400 rpm 0,75 NM und weit bis 3000 rpm (unseriös). 

Angabe Sanyo Denki: 100 rpm 0,4 NM, 400 rpm,  0 Nm bei 24V Power.


Fazit: Auf jeden Fall ist der 103H7123-1740 doppelt so stark. Da wir zusätzlich 48V verwenden,
       liegen wir auf der sicheren Seite, jedoch die 4 kg werden wir wohl damit auch nicht erreichen
       vielleicht 2...3 kg.
       
Auf der Sanyo Denki Homepage ist nur von max. 3A die Rede. Man müßte dann mit Nicastro klären,
ob 4 A dauerhaft erlaubt sind oder doch die kleine Endstufe mit 2,4 A probieren oder die 
CSD 94 mit 4,4A.









Nanotec hat solch eine Größe nicht !


Motor Type 103H6703-0740 Parallel  0,83 Ohm/Phase DC 2A unipolar (Angabe DC 3A, (2A * Wurzel2 = 2,838A bipolar)
3 Monate Lieferzeit!






Verschaltung 2 Phase Bipolar in Serienschaltung:

===>  Phase 1  Orange Blau   (Weis isolieren)
===>  Phase 2  Rot Gelb      (Schwarz isolieren)

Motor 1,4 A max. Ergibt an der CSD 92:

===> Dip Switch CSD 92:
1 ON      1,2 A spitze    (OFF 1,7 A  wäre zu testen wegen zu hoher Motor Temperatur)
2 OFF     1,2 A spitze    (ON  1,7 A  wäre zu testen wegen zu hoher Motor Temperatur)
3 OFF      Dumping (Geräuschreduktion = aber Drehmomentreduktion) aus
4 OFF     NA
5 OFF     400 Pulse/Undr = Halbschritt  (1000 Pulse/Undr gibts nicht)
6 OFF     400 Pulse/Undr = Halbschritt  (1000 Pulse/Undr gibts nicht)

BS3 hat als letzte Ausgangsstufe einen ULN2003A.
5V TTL open collector darlington mit 500mA Output.
Jedoch gehen die Protection Dioden dann wahrscheinlich zu den 5V Vcc.

Darum
===> Jumper auf Pull Up stellen

===> Dip Switch CSD 94:
1 OFF     4,4 A  erlaubt wenn Pausen zwischen dem Nähzyklus 103H7123-1740  4A
2 OFF     4,4 A  erlaubt wenn Pausen zwischen dem Nähzyklus 103H7123-1740  4A
3 ON      Dumping (Geräuschreduktion = aber Drehmomentreduktion) an
4 OFF     NA
5 OFF     400 Pulse/Undr = Halbschritt  (1000 Pulse/Undr gibts nicht)
6 OFF     400 Pulse/Undr = Halbschritt  (1000 Pulse/Undr gibts nicht)

===> CSD 92 / 94  C1 connector Belegung
PIN 
9     Power +24V/+48V               																					
10    Power GND                																				
11    Chassis GND    = Gehäuse 	PE																			
12    Orange         = A       																				
13    Blau           = A\      																				
14    Rot            = B\      																				
15    Gelb           = B       																				
16    Kabel Schirm   Auflegen beim Portal !! wegen Störunterdrückung         																				

===> CSD 92 / 94   C2 connector  Belegung
RTA BS3 
Pin Pin
1   15  rtbl     GND         Signal/Power GND
2   11  rs       C Off in    High = active      !!!  FKT1 !!!,  ja wird benötigt !!!!
3   9   br       Step in     High low transition 
4   10  ge       dir in
5                x4 in       Pull up  also  (open oder high = disabled, low = enabled)
6                NA
7   8   gr/rs    fault out  open Collector out:  GND = normal,  open = fault. BS3 bekommt Low = kein fault.
8                GND        Signal/Power


=====================================================================
=====================================================================

===> A-NDC CN2 Belegung
RTA BS3 
Pin Pin
1          shield Signal line (option)
2   11 rs  - C off in      Endstufe ist aktive wenn low.
3   1  ws  + C off in      gebrückt zu + 5V 
4   10 ge  - Dir in
5   1  ws  + Dir in        gebrückt zu + 5V 
6   9  br  - Step in       High low transition 
7   1  ws  + Step in       gebrückt zu + 5V 
8          - Driver fault output  Wenn 8 mit 9 kurzgeschlossen (geschaltet) = kein Fehler   
9          + Driver fault output    
10         shield Signal line (option)        	
	
===> A-NDC CN1 Belegung
PIN 
11    Power +24V               																					
12    Power GND                																				
13    Chassis GND    = Gehäuse 	PE																			
14    gelb           = B\    																				
15    rot            = B      																				
16    blau           = A\      																				
17    orange         = A       																				
18    Kabel Schirm       

Jumper: FC = off = offen = automatic current reduction, CO = off = offen = current off input available 

===> Dip Switch A-NDC:
1 Off    4,3 A Spitze
2 On     "
3 Off    " 
4 Off   Enhanced Smoothness ;   (  On   Enhanced Torque  )
5 ON    Enhanced Smoothness ;   ( OFF   Enhanced Torque  )
6 OFF   400 Pulse/Undr 
7 OFF   "
8 OFF   "

Problem mit dem Delay zwischen Richtung und Puls besteht nicht.
Zyklischer Einbruch des Stroms besteht auch nicht. Jedoch Stromchopper ist auch nicht synchron zur
Phasenumschaltfrequenz (macht aber erstmal nichts).
Einstellung "Enhanced Motor Torque" hat gleiches Drehmoment wie die normale CSD 94.




Rampenmessen:
0...9 mm Hublänge.


20 mm effektiven Zahnraddruchmesser ergibt (d * Pi)  628,32 1/10 mm/Umdr Vorschub.
Halbschritt = 400 Schritte/Umdr.  ergibt 1,5708 1/10 mm / Puls.

1,5708 1/10 mm  => 1 Puls.
90     1/10 mm  => 57,3
50     1/10mm   => 31 Pulse
30     1/10mm   => 19,1 Pulse

Eine Rampe fahren schnellstmöglich und testen und mit 
und ohne freeze nach unten.
Bei geometrisch ohne freeze, bei zeitlich mit freeze.

Oben sofort umkehren.



Firmware:
256 Pulse.
S:\_RuhendeProjekte\Firmware\BSx\BS3\bs3firm\firm256>   Korrektur aus firm480 übernehmen.





Nanotec SMCI-33 im Takt Richtungsmode.
=======================================

I/O X1:

1   Enable
5   Dir
6   Takt

Verlangt Push Pull, SPS kompatibel !!!
geht nicht mit BS3 da nur open Kollektor !!!!!!!

Mit Mauschlerplatine und den RS485 Einstellungen
##1i60$0D  1A
##1!4$0D   Takt Richtung Modus
##1g2$0D   Halbschritt



SMC-11 
DC bis 35V Versorgung, 1,4 A 
SMCI12 nur 24V Versorgung


SMC32 abgekündigt aber 7 Stück im PFAFF Lager
kleinste Einstellung Halbschritt 1,6A

48V Versorgung: Ergibt 2er Sinus (Halbschritt) aber 2A Spitze und nicht unsere max 1,4 A Spitze.



 wäre aber Sinus



X - Mind B4


===> Dip Switch: 
1 OFF      3,43 A   OFF 4A
2 ON       3,43 A   OFF 4A

3 OFF      Equalization  aus

4 OFF    400 steps/rec
5 OFF
6 OFF 



103H7123-1740 mit 4A erlaubt
X Mind B4 3,43 A 
31 Pulse (5mm) 65 % Stillstand   50 40 45 30 

1600 1/min   X Mind B4 mit 3,43 A

1700 1/min   X Mind B4 mit 4A

1200 1/min   CSD 92  mit 2,4A  gemessen 





11.05.2011
*******************************************************

P320 4A 2er sin Strom Werte 0, 40, 56
Rampe  65 % Stillstand  Rampenwerte 50 40 45 30 
Motor (groß 103H7123-1740  mit  4A)
Mechanik Ritzel-Lösung

1000 1/min 57 Pulse (= 9mm)  mit geringer Kraftreserve

1600 1/min 31 Pulse (= 5mm)  mit geringer Kraftreserve

2100 1/min 19 Pulse (= 3mm)  mit geringer Kraftreserve

*******************************************************

12.05.2011
*******************************************************
Wie oben:
P320 4A 2er sin Strom Werte 0, 40, 56
Rampe  65 % Stillstand  Rampenwerte 50 40 45 30 
Motor (groß 103H7123-1740  mit  4A)
Mechanik Hebel Lösung per Hand modifiziert Hebel starr gemacht ohne Feder
Hebelarm ca. 9,4 ... 10 mm, 
Haltekraft über 5kg.,
Haltekraft mit 1,4A Spitze ca. 5kg  (nur zum Test)


800  1/min 57 Pulse (= 9mm)  mit geringer Kraftreserve 
     (weniger als oben, da wir schon leicht in die Ecken fahren müssen).

1600 1/min 31 Pulse (= 5mm)  mit geringer Kraftreserve

2300 1/min 19 Pulse (= 3mm) Hebel waagerecht!  mit geringer Kraftreserve

1900 1/min 19 Pulse (= 3mm) Hebel oben oder unten!  mit geringer Kraftreserve

Probem: Im ungünstigen Fall (Hebel nicht waagerecht) entstehen große 
        Schleifreibkräfte zwischen den Hebeln was bei großen Hubhöhen
        zu Schrittverlust führt. Wenn jedoch der Hebel waagerecht ist 
        gewinnen wir an Drehzahl (Kraft).
        
        
*******************************************************
12.05.2011
*******************************************************
Wie oben jedoch mit 16,8 mm eff. Hebelarmradius

Haltekraft über 5kg.,
Haltekraft mit 1,4A Spitze ca. 3kg Haltekraft (nur zum Test)

1200  1/min 34 Pulse (= 9mm)  mit geringer Kraftreserve

1900 1/min 19 Pulse (= 5mm)  mit geringer Kraftreserve

2400 1/min 11 Pulse (= 3mm) Hebel waagerecht!  mit geringer Kraftreserve

---- 1/min 11 Pulse (= 3mm) in die Ecken kommen wir nicht mehr da der Motorhebel
     zu kurz ist für diesen Versuchsaufbau.
*******************************************************



20mm Ritzel
============

30.05.2011
*******************************************************
RTA CSD 94 3,8A Spitze 2er sin 
Rampe  65 % Stillstand  Rampenwerte 50 40 45 30 gelbe Tabelle
Motor (groß 103H7123-1740   4A)
Mechanik Ritzel-Lösung alt

1000 1/min 57 Pulse (= 9mm)  mit geringer Kraftreserve

1600 1/min 31 Pulse (= 5mm)  mit geringer Kraftreserve

2100 1/min 19 Pulse (= 3mm)  mit geringer Kraftreserve

*******************************************************

30.05.2011
*******************************************************
RTA CSD 94 4,4A Spitze 2er sin 
Rampe  65 % Stillstand  Rampenwerte 50 40 45 30 gelbe Tabelle
Motor (groß 103H7123-1740   4A)
Mechanik Ritzel-Lösung alt

1200 1/min 57 Pulse (= 9mm)  mit geringer Kraftreserve

1800 1/min 31 Pulse (= 5mm)  mit geringer Kraftreserve

2300 1/min 19 Pulse (= 3mm)  mit geringer Kraftreserve

*******************************************************
*******************************************************

30mm Ritzel
============

29.06.2011  
*******************************************************
RTA A-NDC 96 4,3A Spitze 2er sin (gemessen 4A)
Rampe  65 % Stillstand  Rampenwerte 50 40 45 30 gelbe Tabelle
Motor (groß 103H7123-1740   4A)
Mechanik Ritzel-Lösung neu 30 mm Druchmesser eff.

1000 1/min 38 Pulse (= 9mm)  mit geringer Kraftreserve smooth
1300 1/min 38 Pulse (= 9mm)  mit geringer Kraftreserve Torque

1400 1/min 21 Pulse (= 5mm)  mit geringer Kraftreserve smooth
2000 1/min 21 Pulse (= 5mm)  mit geringer Kraftreserve Torque

1800 1/min 12 Pulse (= 3mm)  mit geringer Kraftreserve smooth
2300 1/min 12 Pulse (= 3mm)  mit geringer Kraftreserve Torque
*******************************************************

*******************************************************
RTA CSD 94 4,4A Spitze 2er sin (gemessen 4,4A)
Rampe  65 % Stillstand  Rampenwerte 50 40 45 30 gelbe Tabelle
Motor (groß 103H7123-1740   4A)
Mechanik Ritzel-Lösung neu 30 mm Druchmesser eff.

1300 1/min 38 Pulse (= 9mm)  mit geringer Kraftreserve 

1800 1/min 21 Pulse (= 5mm)  mit geringer Kraftreserve 
      (Fehler der Stromeinbruchs macht sich hier besonders stark bemerkbar)

2300 1/min 12 Pulse (= 3mm)  mit geringer Kraftreserve 

*******************************************************