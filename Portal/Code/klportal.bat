CLS
@Echo off
Echo       �������������������������������������������������������Ŀ
Echo       �Erzeugen der Portal-Software f�r die BS3                �
Echo       �portal.bat erzeugt portal.exe                          �
Echo       �ein 2. Flag definiert die N�hfeldgr��e:                �
Echo       �120x70, 120X170, 100x50, 120x200,                      �
Echo       �Ist kein 2. Flag definiert wir die Software fuer 120x70�
Echo       �erzeugt.                                               �
Echo       ���������������������������������������������������������


If "%1"=="?" goto Hilfe
If "%1"=="TST" goto Buero
If "%1"=="tst" goto Buero
If "%1"=="Tst" goto Buero
If "%1"=="REM" goto Remote
If "%1"=="rem" goto Remote
If "%1"=="Rem" goto Remote
If "%1"=="TRACE" goto Trace
If "%1"=="trace" goto Trace
If "%1"=="Trace" goto Trace
If "%1"=="EXE" goto EXE
If "%1"=="exe" goto EXE
If "%1"=="Exe" goto EXE
If "%1"=="COM3" goto Com3
If "%1"=="Com3" goto Com3
If "%1"=="com3" goto Com3
If "%1"=="com3b" goto Com3b
If "%1"=="NET" goto Net
If "%1"=="Net" goto Net
If "%1"=="net" goto Net
goto Hilfe



:EXE
Echo 
Echo       �����������������������������������������������������������������Ŀ
Echo       �         Erzeugen der Portal-Software fuer die BS3               �
Echo       �         portal.bat erzeugt portal.exe                           �
Echo       �������������������������������������������������������������������
call clean
If "%2"=="100x50" goto EXE2
If "%2"=="120x170" goto EXE3
If "%2"=="120x200" goto EXE4
Echo N�hfeld 1200 x 700
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=0 -Dtrace=0 -Dcom3=0 -D120x70=1
goto Ende
:EXE2
Echo  N�hfeld 1000 x 500
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=0 -Dtrace=0 -Dcom3=0 -D100x50=1
goto Ende
:EXE3
Echo  N�hfeld 1200 x 1700
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=0 -Dtrace=0 -Dcom3=0 -D120x170=1
goto Ende
:EXE4
Echo  N�hfeld 1200 x 2000
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=0 -Dtrace=0 -Dcom3=0 -D120x200=1
goto Ende

:Buero
Echo 
Echo       �����������������������������������������������������������������Ŀ
Echo       �         Erzeugen der Portal-Software f�r die BS3                �
Echo       �         portal.bat tst erzeugt portal.exe und definiert das     �
Echo       �         Flag remote zum Remote-debuggen �ber Com1 und das       �
Echo       �         Flag buerotest zur Inbetriebnahme ohne Maschine         �
Echo       �������������������������������������������������������������������
call clean
If "%2"=="100x50" goto Buero2
If "%2"=="120x170" goto Buero3
If "%2"=="120x200" goto Buero4
Echo N�hfeld 1200 x 700
Maker -fportal.mak -K+  -Dbuerotest=1 -Dremote=1 -Dtrace=0 -Dcom3=0 -D120x70=1
goto Ende
:Buero2
Echo N�hfeld 1000 x 500
Maker -fportal.mak -K+  -Dbuerotest=1 -Dremote=1 -Dtrace=0 -Dcom3=0 -D100x50=1
goto Ende
:Buero3
Echo N�hfeld 1200 x 1700
Maker -fportal.mak -K+  -Dbuerotest=1 -Dremote=1 -Dtrace=0 -Dcom3=0 -D120x170=1
goto Ende
:Buero4
Echo N�hfeld 1200 x 2000
Maker -fportal.mak -K+  -Dbuerotest=1 -Dremote=1 -Dtrace=0 -Dcom3=0 -D120x200=1
goto Ende

:Remote
Echo 
Echo       �����������������������������������������������������������������Ŀ
Echo       �         Erzeugen der Portal-Software f�r die BS3                �
Echo       �         portal.bat rem erzeugt portal.exe und definiert das     �
Echo       �         Flag remote zum Remote-debuggen �ber Com1               �
Echo       �������������������������������������������������������������������
call clean
If "%2"=="100x50" goto Remote2
If "%2"=="120x170" goto Remote3
If "%2"=="120x200" goto Remote4
Echo N�hfeld 1200 x 700
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=1 -Dtrace=0 -Dcom3=0 -D120x70=1
goto Ende
:Remote2
Echo N�hfeld 1000 x 500
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=1 -Dtrace=0 -Dcom3=0 -D100x50=1
goto Ende
:Remote3
Echo N�hfeld 1200 x 1700
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=1 -Dtrace=0 -Dcom3=0 -D120x170=1
goto Ende
:Remote4
Echo N�hfeld 1200 x 2000
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=1 -Dtrace=0 -Dcom3=0 -D120x2000=1
goto Ende

:Trace
Echo 
Echo       �����������������������������������������������������������������Ŀ
Echo       �         Erzeugen der Portal-Software f�r die BS3                �
Echo       �         portal.bat trace erzeugt portal.exe und definiert das   �
Echo       �         Flag remote zum Remote-debuggen �ber Com1 und das Flag  �
Echo       �         trace zum Aufzeichnen der Drehzahlen, Rampenoffsets und �
Echo       �         Verz�gerungen.                                          �
Echo       �������������������������������������������������������������������
call clean
If "%2"=="100x50" goto Trace2
If "%2"=="120x170" goto Trace3
If "%2"=="120x200" goto Trace4
Echo N�hfeld 1200 x 700
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=1 -Dtrace=1 -Dcom3=0 -D120x70=1
goto Ende
:Trace2
Echo N�hfeld 1000 x 500
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=1 -Dtrace=1 -Dcom3=0 -D100x50=1
goto Ende
:Trace3
Echo N�hfeld 1200 x 1700
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=1 -Dtrace=1 -Dcom3=0 -D120x170=1
goto Ende
:Trace4
Echo N�hfeld 1200 x 2000
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=1 -Dtrace=1 -Dcom3=0 -D120x200=1
goto Ende

:Com3
Echo 
Echo       �����������������������������������������������������������������Ŀ
Echo       �         Erzeugen der Portal-Software fuer die BS3               �
Echo       �         portal.bat com3 erzeugt portal.exe und definiert das    �
Echo       �         Flag remote zum Remote-debuggen ueber Com1 und das Flag �
Echo       �         com3 zum Testen der seriellen Schnittstelle (PC-Kommuni-�
Echo       �         kation) ueber Com3 und das Flag buerotest zur Intrieb-  �
Echo       �         nahme ohne Maschine.                                    �
Echo       �������������������������������������������������������������������
call clean
If "%2"=="100x50" goto Com32
If "%2"=="120x170" goto Com33
If "%2"=="120x200" goto Com34
Echo N�hfeld 1200 x 700
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=1 -Dtrace=1 -Dcom3=1 -D120x70=1
goto Ende
:Com32
Echo N�hfeld 1000 x 500
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=1 -Dtrace=1 -Dcom3=1 -D100x50=1
goto Ende
:Com33
Echo N�hfeld 1200 x 1700
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=1 -Dtrace=1 -Dcom3=1 -D120x170=1
goto Ende
:Com34
Echo N�hfeld 1200 x 2000
Maker -fportal.mak -K+  -Dbuerotest=0 -Dremote=1 -Dtrace=1 -Dcom3=1 -D120x200=1
goto Ende

:Com3b
Echo 
Echo       �����������������������������������������������������������������Ŀ
Echo       �         Erzeugen der Portal-Software fuer die BS3               �
Echo       �         portal.bat com3 erzeugt portal.exe und definiert das    �
Echo       �         Flag remote zum Remote-debuggen ueber Com1 und das Flag �
Echo       �         com3 zum Testen der seriellen Schnittstelle (PC-Kommuni-�
Echo       �         kation) ueber Com3 und das Flag buerotest zur Intrieb-  �
Echo       �         nahme ohne Maschine.                                    �
Echo       �������������������������������������������������������������������
call clean
If "%2"=="100x50" goto Com3b2
If "%2"=="120x170" goto Com3b3
If "%2"=="120x200" goto Com3b4
Echo N�hfeld 1200 x 700
Maker -fportal.mak -K+  -Dbuerotest=1 -Dremote=1 -Dtrace=0 -Dcom3=1 -D120x70=1
goto Ende
:Com3b2
Echo N�hfeld 1000 x 500
Maker -fportal.mak -K+  -Dbuerotest=1 -Dremote=1 -Dtrace=0 -Dcom3=1 -D100x50=1
goto Ende
:Com3b3
Echo N�hfeld 1200 x 1700
Maker -fportal.mak -K+  -Dbuerotest=1 -Dremote=1 -Dtrace=0 -Dcom3=1 -D120x170=1
goto Ende
:Com3b4
Echo N�hfeld 1200 x 2000
Maker -fportal.mak -K+  -Dbuerotest=1 -Dremote=1 -Dtrace=0 -Dcom3=1 -D120x200=1
goto Ende


:Hilfe
Echo 
Echo       �����������������������������������������������������������������Ŀ
Echo       �         Erzeugen der Portal-Software fuer die BS3               �
Echo       �         Aufruf:                                                 �
Echo       �         portal.bat exe erzeugt portal.exe ohne Debug-Infos      �
Echo       �         portal.bat rem erzeugt portal.exe mit Debug-Infos und   �
Echo       �                        definiert das Flag remote zum            �
Echo       �                        Remote-debuggen ueber Com1               �
Echo       �         portal.bat tst erzeugt portal.exe und definiert das     �
Echo       �                        Flag remote zum Remote-debuggen ueber    �
Echo       �                        Com1 und das Flag buerotest zur Inbe-    �
Echo       �                        triebnahme ohne Maschine                 �
Echo       �         portal.bat trace                                        �
Echo       �                        erzeugt portal.exe und definiert das     �
Echo       �                        Flag remote zum Remote-debuggen ueber    �
Echo       �                        Com1 und das Flag trace zum Aufzeichnen  �
Echo       �                        der Drehzahlen, Rampenoffsets und Ver-   �
Echo       �                        zoegerungen.                             �
Echo       �         portal.bat com3                                         �
Echo       �                        erzeugt portal.exe und definiert das     �
Echo       �                        Flag remote zum Remote-debuggen ueber    �
Echo       �                        Com1 und das Flag com3 zum Testen der    �
Echo       �                        seriellen Schnittstelle (PC-Kommunikat.) �
Echo       �                        ueber Com3                               �
Echo       �         portal.bat com3b                                        �
Echo       �                        erzeugt portal.exe und definiert das     �
Echo       �                        Flag remote zum Remote-debuggen ueber    �
Echo       �                        Com1 und das Flag com3 zum Testen der    �
Echo       �                        seriellen Schnittstelle (PC-Kommunikat.) �
Echo       �                        ueber Com3 und das Flag buerotest zur    �
Echo       �                        Intriebnahme ohne Maschine               �
Echo       �������������������������������������������������������������������
Pause
:Ende
