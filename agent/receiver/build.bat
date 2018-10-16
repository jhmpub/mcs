rem build with mingw or cygwin unless environnment variable MSVC is defined
rem see the media_control_system.pdf for compiler setup
rem 
rem Notes:
rem    1. targets must be compiled 32 bit for library compatiblity
rem    2. the cygwin compiler is used for console debugging using posix shells like bash
rem    3. the mingw compiler is used to build a toolchain independent executable
rem       use cygwin "ldd <exe>" to review DLL dependencies

if not defined MSVC goto CYGWIN_OR_MINGW

set CMN=..\..\common
set RTI=%CMN%\rti
set INC=/I %CMN% /I %RTI%\include /I %RTI%
set CLOPT=/nologo /c /ZI /EHsc
set LNKOPT=/nologo /opt:noref /opt:noicf /nodefaultlib:libcmtd /libpath:%RTI%
set LNKCON=%LNKOPT% /out:arAgentConsole.exe
set LNKWIN=%LNKOPT% /subsystem:windows /entry:mainCRTStartup
set LIBS=ws2_32.lib user32.lib
set OBJ=arAgent.obj socket.obj common.obj info.obj

rc /nologo /fo info.obj arAgent.rc
cl %CLOPT% %INC% arAgent.cpp
cl %CLOPT% %INC% /DMCS %CMN%\socket.cpp
cl %CLOPT% %INC% /DMCS /DCONSOLE_MODE %CMN%\common.cpp
link %LNKCON% %LIBS% %OBJ%

cl %CLOPT% %INCS% /DMCS %CMN%\common.cpp >nul
link %LNKWIN% %LIBS% %OBJ%

goto DONE

:CYGWIN_OR_MINGW

set CMN=../../common
set RTI=%CMN%/rti
set INC=-I%CMN% -I%RTI%\include -I%RTI%
set LIBS=-lws2_32 -lstdc++
set OBJ=arAgent.o socket.o common.o info.o

set BIN=%PROGRAM_DRIVE%\pkg\win32\mingw\bin
%BIN%\windres -i arAgent.rc -o info.o
%BIN%\gcc -c -g %INC% arAgent.cpp
%BIN%\gcc -c -g -DMCS %CMN%\socket.cpp
%BIN%\gcc -c -g -DMCS %CMN%\common.cpp
%BIN%\gcc -static -mwindows -o arAgent %OBJ% %LIBS%

set BIN=%PROGRAM_DRIVE%\pkg\win32\cygwin\bin
%BIN%\windres -i arAgent.rc -o info.o
%BIN%\gcc -c -g %INC% arAgent.cpp
%BIN%\gcc -c -g -DMCS %CMN%\socket.cpp
%BIN%\gcc -c -g -DMCS -DCONSOLE_MODE %CMN%\common.cpp
%BIN%\gcc -o arAgentConsole %OBJ% %LIBS%

:DONE

del /q /f *.o *.obj *.pdb *.idb 2>nul 1>nul

