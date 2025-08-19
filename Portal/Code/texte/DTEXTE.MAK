.AUTODEPEND

#     *Translator Definitions*
CC = bcc +DTEXTE.CFG
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
 dtexte.obj

#     *Explicit Rules*
dtexte.exe: dtexte.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c/P-/L$(LIBPATH) @&&|
c0h.obj+
dtexte.obj
dtexte
      # no map file
emu.lib+
mathh.lib+
ch.lib
|


#     *Individual File Dependencies*
dtexte.obj: dtexte.cfg dtexte.c

#     *Compiler Configuration File*
dtexte.cfg: dtexte.mak
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
| dtexte.cfg


