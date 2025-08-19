.AUTODEPEND

#     *Translator Definitions*
CC = bcc +FTEXTE.CFG
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
 ftexte.obj

#     *Explicit Rules*
ftexte.exe: ftexte.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c/P-/L$(LIBPATH) @&&|
c0h.obj+
ftexte.obj
ftexte
      # no map file
emu.lib+
mathh.lib+
ch.lib
|


#     *Individual File Dependencies*
ftexte.obj: ftexte.cfg ftexte.c

#     *Compiler Configuration File*
ftexte.cfg: ftexte.mak
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
| ftexte.cfg


