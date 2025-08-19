CLS
@Echo off

Echo   �����������������������������������������������������������������Ŀ
Echo   �         Erzeugen der Bin�rdateien der Texte f�r die KL3590      �
Echo   �������������������������������������������������������������������

Echo   �����������������������������������������������������������������Ŀ
Echo   �                  1. alte Dateien l�schen                        �
Echo   �������������������������������������������������������������������

call txtclean.bat
copy ..\textport.h
copy ..\touch.h

Echo   �����������������������������������������������������������������Ŀ
Echo   �      2. Texte compilieren und linken: Ergebnis *texte.exe       �
Echo   �������������������������������������������������������������������

maker -fdtexte.mak
maker -fetexte.mak
maker -fftexte.mak
maker -fstexte.mak
maker -fitexte.mak
maker -fptexte.mak

Echo   �����������������������������������������������������������������Ŀ
Echo   �                 3. textbin.exe erzeugen                         �
Echo   �������������������������������������������������������������������

maker -ftextbin.mak

Echo   �����������������������������������������������������������������Ŀ
Echo   �          4. exe-Dateien in Bin�rdateien wandeln                 �
Echo   �������������������������������������������������������������������

textbin dtexte.exe dtexte.bin
textbin etexte.exe etexte.bin
textbin ftexte.exe ftexte.bin
textbin stexte.exe stexte.bin
textbin itexte.exe itexte.bin
textbin ptexte.exe ptexte.bin

del ?texte.exe



Echo   �����������������������������������������������������������������Ŀ
Echo   �        10. Bin�rdateien Anzeigen                                �
Echo   �������������������������������������������������������������������

dir /b/s  *.bin


Echo                                                          F e r t i g




