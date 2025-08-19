.AUTODEPEND

#     *Translator Definitions*
CC = bcc +TEXTBIN.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH =  C:\BC\LIB
INCLUDEPATH = C:\BC\INCLUDE


#     *Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#     *List Macros*


EXE_dependencies =  \
 textbin.obj

#     *Explicit Rules*
textbin.exe: textbin.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c/P-/L$(LIBPATH) @&&|
c0h.obj+
textbin.obj
textbin
      # no map file
emu.lib+
mathh.lib+
ch.lib
|


#     *Individual File Dependencies*
textbin.obj: textbin.cfg textbin.c

#     *Compiler Configuration File*
textbin.cfg: textbin.mak
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
| textbin.cfg


