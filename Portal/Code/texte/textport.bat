CLS
@Echo off

Echo   зддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
Echo   Ё         Erzeugen der Bin└rdateien der Texte f│r die KL3590      Ё
Echo   юддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддды

Echo   зддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
Echo   Ё                  1. alte Dateien l■schen                        Ё
Echo   юддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддды

call txtclean.bat
copy ..\textport.h
copy ..\touch.h

Echo   зддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
Echo   Ё      2. Texte compilieren und linken: Ergebnis *texte.exe       Ё
Echo   юддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддды

maker -fdtexte.mak
maker -fetexte.mak
maker -fftexte.mak
maker -fstexte.mak
maker -fitexte.mak
maker -fptexte.mak

Echo   зддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
Echo   Ё                 3. textbin.exe erzeugen                         Ё
Echo   юддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддды

maker -ftextbin.mak

Echo   зддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
Echo   Ё          4. exe-Dateien in Bin└rdateien wandeln                 Ё
Echo   юддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддды

textbin dtexte.exe dtexte.bin
textbin etexte.exe etexte.bin
textbin ftexte.exe ftexte.bin
textbin stexte.exe stexte.bin
textbin itexte.exe itexte.bin
textbin ptexte.exe ptexte.bin

del ?texte.exe



Echo   зддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
Echo   Ё        10. Bin└rdateien Anzeigen                                Ё
Echo   юддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддды

dir /b/s  *.bin


Echo                                                          F e r t i g




