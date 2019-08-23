.SILENT :

!IF "$(CFG)" != "i386" && "$(CFG)" != "amd64"
CFG=i386
!ENDIF

!IF "$(CFG)" == "i386"
OUTPUT=..\Release\i386
CFLAGS=/c /GF /GS- /MD /O1 /W3
RCCFG=/d "_M_IX86"
LDCFG=/VERSION:5.0 /SUBSYSTEM:WINDOWS,5.0 /OSVERSION:5.0 /MACHINE:IX86 /NXCOMPAT /DYNAMICBASE
LIBS2=msvcrt_win2000.obj
!ELSE
OUTPUT=..\Release\amd64
CFLAGS=/c /GF /GS- /MD /O1 /W3
RCCFG=/d "_M_AMD64"
LDCFG=/VERSION:5.2 /SUBSYSTEM:WINDOWS,5.2 /OSVERSION:5.2 /MACHINE:AMD64 /NXCOMPAT /DYNAMICBASE
LIBS2=msvcrt_win2003.obj
!ENDIF

CC=cl /nologo
RC=rc
LD=link /nologo

SRC=..\src
RES=..\res
EXE=$(OUTPUT)\metapath.exe

CDEF=/D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D "_UNICODE" /D "UNICODE"
CINC=
RCFLAGS=/l 0x0409 /d "NDEBUG"
LDFLAGS=/INCREMENTAL:NO /RELEASE /OPT:REF /OPT:ICF /MERGE:.rdata=.text
LIBS= \
  kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib shlwapi.lib comdlg32.lib \
  comctl32.lib winspool.lib imm32.lib ole32.lib oleaut32.lib psapi.lib

BUILD : PREBUILD $(EXE) POSTBUILD

PREBUILD :
  echo Building...
  -md "$(OUTPUT)" >nul 2>&1

POSTBUILD :
  echo Success.

CLEAN :
  echo Cleaning...
  -del /s "$(OUTPUT)\*.exe" "$(OUTPUT)\*.obj" "$(OUTPUT)\*.res" "$(OUTPUT)\*.pdb" "$(OUTPUT)\*.idb" >nul 2>&1

OBJ= \
  $(OUTPUT)\Dialogs.obj \
  $(OUTPUT)\Dlapi.obj \
  $(OUTPUT)\Dropsource.obj \
  $(OUTPUT)\Helpers.obj \
  $(OUTPUT)\metapath.obj \
  $(OUTPUT)\metapath.res

{$(SRC)\}.c{$(OUTPUT)}.obj ::
  $(CC) /Fo$(OUTPUT)\ $(CINC) $(CDEF) $(CFLAGS) $<

{$(SRC)\}.cpp{$(OUTPUT)}.obj ::
  $(CC) /Fo$(OUTPUT)\ $(CINC) $(CDEF) $(CFLAGS) $<

$(OUTPUT)\metapath.res : $(SRC)\metapath.rc
  echo $(**F)
  $(RC) $(RCFLAGS) $(RCCFG) /fo$@ $** >nul

$(EXE) : $(OBJ)
  $(LD) /OUT:"$(EXE)" $(LDFLAGS) $(LDCFG) $(LIBS) $** $(LIBS2)

$(OUTPUT)\Dialogs.obj : \
  $(SRC)\Dialogs.c \
  $(SRC)\metapath.h \
  $(SRC)\Dlapi.h \
  $(SRC)\Helpers.h \
  $(SRC)\Dialogs.h \
  $(SRC)\resource.h \
  $(SRC)\version.h

$(OUTPUT)\Dlapi.obj : \
$(SRC)\Dlapi.c \
$(SRC)\Dlapi.h

$(OUTPUT)\Helpers.obj : \
  $(SRC)\Helpers.c \
  $(SRC)\Dlapi.h \
  $(SRC)\Helpers.h \
  $(SRC)\resource.h

$(OUTPUT)\metapath.obj : \
  $(SRC)\metapath.c \
  $(SRC)\Helpers.h \
  $(SRC)\Dlapi.h \
  $(SRC)\Dialogs.h \
  $(SRC)\metapath.h \
  $(SRC)\resource.h

$(OUTPUT)\Dropsource.obj : \
  $(SRC)\Dropsource.cpp \
  $(SRC)\Dropsource.h

$(SRC)\metapath.rc : \
  $(SRC)\resource.h \
  $(SRC)\version.h \
  $(SRC)\metapath.ver \
  $(RES)\Open.bmp \
  $(RES)\Toolbar.bmp \
  $(RES)\crosshair.cur \
  $(RES)\cross1.ico \
  $(RES)\cross2.ico \
  $(RES)\Folder.ico \
  $(RES)\Goto.ico \
  $(RES)\Items.ico \
  $(RES)\metapath.ico \
  $(RES)\Options.ico \
  $(RES)\Options2.ico \
  $(RES)\Progs.ico \
  $(RES)\Run.ico \
  $(RES)\metapath.exe.manifest
