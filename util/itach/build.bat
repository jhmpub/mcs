rem build with mingw or cygwin unless enviornment variable MSVC is defined

if not defined MSVC goto CYGWIN_OR_MINGW

set CMN=..\..\common
set OBJ=demo.obj common.obj socket.obj
set INC=/I %CMN%
set CLOPT=/nologo /c /ZI /EHsc
set LNKOPT=/nologo /opt:noref /opt:noicf /nodefaultlib:libcmtd
set LIBS=user32.lib ws2_32.lib

cl %CLOPT% %INC% demo.cpp
cl %CLOPT% %INC% %CMN%\socket.cpp
cl %CLOPT% %INC% /DCONSOLE_MODE %CMN%\common.cpp
link %LNKOPT% %LIBS% %OBJ%

goto DONE

:CYGWIN_OR_MINGW

set CMN=..\..\common
set INC=-I%CMN%
set LIBS=-lstdc++ -lws2_32
set OBJ=demo.o common.o socket.o
set BIN=%PROGRAM_DRIVE%\pkg\win32\mingw\bin
%BIN%\gcc -c -g %INC% demo.cpp
%BIN%\gcc -c -g %CMN%\socket.cpp
%BIN%\gcc -c -g -DCONSOLE_MODE %CMN%\common.cpp
%BIN%\gcc -static -o demo %OBJ% %LIBS%

:DONE

del /q /f *.o *.obj *.pdb *.idb 2>nul 1>nul

