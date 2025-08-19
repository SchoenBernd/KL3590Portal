.AUTODEPEND

#     *Translator Definitions*
CC = bcc +STEXTE.CFG
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
 stexte.obj

#     *Explicit Rules*
stexte.exe: stexte.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c/P-/L$(LIBPATH) @&&|
c0h.obj+
stexte.obj
stexte
      # no map file
emu.lib+
mathh.lib+
ch.lib
|


#     *Individual File Dependencies*
stexte.obj: stexte.cfg stexte.c

#     *Compiler Configuration File*
stexte.cfg: stexte.mak
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
| stexte.cfg


