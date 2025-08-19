.AUTODEPEND

#     *Translator Definitions*
CC = bcc +ITEXTE.CFG
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
 itexte.obj

#     *Explicit Rules*
itexte.exe: itexte.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c/P-/L$(LIBPATH) @&&|
c0h.obj+
itexte.obj
itexte
      # no map file
emu.lib+
mathh.lib+
ch.lib
|


#     *Individual File Dependencies*
itexte.obj: itexte.cfg itexte.c

#     *Compiler Configuration File*
itexte.cfg: itexte.mak
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
| itexte.cfg


