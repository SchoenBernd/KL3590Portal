#-----------------------------------------------------------------------------
#  MakeFile:   portal.mak fuer die BS3
#  Version:    
#  Zieldatei:  portal.exe
#  Compiler:   BC++ V3.1
#  Linker:     TLink V5.1
#  Macros:     MODEL   memory model
#              STACK   application stack size
#
#  Bei PC104-Modulen mit Co-Prozessor fp87.lib linken.
#  Bei PC104-Modulen ohne Co-Prozessor emu.lib linken.
#
#-----------------------------------------------------------------------------

.AUTODEPEND

#     *Definitions*
LIBPATH = C:\BC\LIB
INCLUDEPATH = C:\BC\INCLUDE
STACK = 4096
MODEL = h                    # Speichermodell huge
FLOAT = 2                    # 0 - none, 1 - emulator, 2 - coprocessor


CFLAGS = -m$(MODEL) -3 -a -O1 -Z -k- -c -P -g20 -j20      # -P   C++ Compilierung immer ausfuehren   
TFLAGS = /s/c/P-/L$(LIBPATH)

!if   $(FLOAT) == 0
CFLAGS      =  $(CFLAGS) -f-
!elif   $(FLOAT) == 1
CFLAGS      =  $(CFLAGS) -f
!elif $(FLOAT) == 2
CFLAGS      =  $(CFLAGS) -f287
!else
!error   Invalid floating point option selected
!endif

# Process the selected buerotest option
!if $(buerotest) == 1
CFLAGS  = $(CFLAGS) -Dbuerotest
!endif

# Process the selected com3 option
!if $(com3) == 1
CFLAGS  = $(CFLAGS) -Dcom3
!endif


# Process the selected remote option
!if $(remote) == 1
CFLAGS  = $(CFLAGS) -Dremote
CFLAGS  = $(CFLAGS) -v           # Debugger Info ein, inline Erweiterung aus
TFLAGS  = $(TFLAGS) /v          
!else
CFLAGS  = $(CFLAGS) -v-          # Debugger Info aus, inline Erweiterung ein
!endif


# Process the selected trace option
!if $(trace) == 1
CFLAGS  = $(CFLAGS) -Dtrace
!endif

# Process the selected 120x70 option
!if $(120x70) == 1
CFLAGS  = $(CFLAGS) -DNF120x70
!endif

# Process the selected 100x50 option
!if $(100x50) == 1
CFLAGS  = $(CFLAGS) -DNF100x50
!endif


# Process the selected 120x170 option
!if $(120x170) == 1
CFLAGS  = $(CFLAGS) -DNF120x170
!endif

# Process the selected 120x200 option
!if $(120x200) == 1
CFLAGS  = $(CFLAGS) -DNF120x200
!endif


# Wegen Linker Error, Debugging information overflow, bestimmte Dateien ohne Debug nachcompilieren
CFLAGX = $(CFLAGS) -v-    # ohne Debug Info

#     *Implicit Rules*
.c.obj:
  bcc $(CFLAGS) $*.c

  

OBJ1    =      hpportal.obj
OBJ2    =      portvers.obj ablport.obj bootport.obj eingport.obj eingup.obj keybport.obj\
               koorport.obj ioportal.obj screen.obj texte.obj treiber.obj Utlport.obj naeport.obj\
               zeitport.obj datinfpo.obj cmdport.obj nminf.obj sminf.obj\
               parport.obj progport.obj nmtrbpo.obj naeup.obj nmintpo.obj naetabpo.obj proguppo.obj\
               veruppo.obj servport.obj konvport.obj sdups.obj ringbuf.obj schabl.obj rfidport.obj\
               bcportal.obj hfportal.obj




EXE_dependencies =  \
  $(OBJ1)\
  $(OBJ2)\
 {$(LIBPATH)}rtksth.lib \
 {$(LIBPATH)}rtkth.lib \
 {$(LIBPATH)}ch.lib

#     *Explicit Rules*


portal.exe: $(EXE_dependencies)

# Wegen Linker Error, Debugging information overflow, bestimmte Dateien ohne Debug nachcompilieren
# Die Zeilen mussen hier hin, sonst werden sie X mal ausgefuert
 # bcc $(CFLAGX) hpportal.c
   bcc $(CFLAGX) zeitport.c
   bcc $(CFLAGX) portvers.c


  tlink $(TFLAGS) @&&|
c0$(MODEL).obj+
$(OBJ1)+
$(OBJ2)
portal,portal
rtkst$(MODEL).lib+
rtkt$(MODEL).lib+
fp87.lib+
math$(MODEL).lib+
c$(MODEL).lib
|

