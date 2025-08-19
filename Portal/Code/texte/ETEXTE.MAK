.AUTODEPEND

#     *Translator Definitions*
CC = bcc +ETEXTE.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = C:\BC\LIB
INCLUDEPATH = C:\BC\INCLUDE


#     *Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#     *List Macros*


EXE_dependencies =  \
 etexte.obj

#     *Explicit Rules*
etexte.exe: etexte.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c/P-/L$(LIBPATH) @&&|
c0h.obj+
etexte.obj
etexte
      # no map file
emu.lib+
mathh.lib+
ch.lib
|


#     *Individual File Dependencies*
etexte.obj: etexte.cfg etexte.c

#     *Compiler Configuration File*
etexte.cfg: etexte.mak
  copy &&|
-mh
-3
-a
-O
-Oe
-Ob
-Z
-k-
-h
-vi-
-wpro
-weas
-wpre
-I$(INCLUDEPATH)
-L$(LIBPATH)
-P-.C
| etexte.cfg


