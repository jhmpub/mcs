rem build with mingw or cygwin unless enviornment variable MSVC is defined

if not defined MSVC goto CYGWIN_OR_MINGW

set CMN=..\..\common
set INC=/I %CMN%
set CLOPT=/nologo /c /ZI /EHsc
set LNKOPT=/nologo /opt:noref /opt:noicf /nodefaultlib:libcmtd
set LIBS=ws2_32.lib
set OBJ=msg.obj socket.obj

cl %CLOPT% %INC% msg.cpp
cl %CLOPT% %INC% %CMN%\socket.cpp
link %LNKOPT% %LIBS% %OBJ%

goto DONE

:CYGWIN_OR_MINGW

set CMN=../../common
set INC=-I%CMN%
set BIN=%PROGRAM_DRIVE%\pkg\win32\mingw\bin
set LIBS=-lstdc++ -lws2_32
set OBJ=msg.o socket.o

%BIN%\gcc -c -g %INC% msg.cpp
%BIN%\gcc -c -g %INC% %CMN%\socket.cpp
%BIN%\gcc -static -o msg %OBJ% %LIBS%

:DONE

del /q /f *.o *.obj *.pdb *.idb 2>nul 1>nul
