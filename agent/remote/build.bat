rem build with mingw or cygwin unless environnment variable MSVC is defined
rem see the media_control_system.pdf for compiler setup
rem 
rem Notes:
rem    1. targets must be compiled 32 bit for library compatiblity
rem    2. the cygwin compiler is used for console debugging using posix shells 
rem       like bash
rem    3. the mingw compiler is used to build a toolchain independent executable
rem       use cygwin "ldd <exe>" to review DLL dependencies
rem    4. the rcAgent synthesizes keyboard input so running as a service is not
rem       possible


if not defined MSVC goto CYGWIN_OR_MINGW

set CMN=..\..\common
set RTI=%CMN%\rti
set INC=/I %CMN% /I %RTI%\include /I %RTI%
set CLOPT=/nologo /c /ZI /EHsc
set LNKOPT=/nologo /opt:noref /opt:noicf /nodefaultlib:libcmtd /libpath:%RTI%
set LNKCON=%LNKOPT% /out:rcAgentConsole.exe
set LNKWIN=%LNKOPT% /subsystem:windows /entry:mainCRTStartup
set LIBS=ws2_32.lib user32.lib psapi.lib rtilibinit.lib rtilib.lib
set OBJ=rcAgent.obj socket.obj common.obj info.obj zrc.obj zrcc.obj

rc /nologo /fo info.obj rcAgent.rc
cl %CLOPT% %INC% rcAgent.cpp
cl %CLOPT% %INC% %RTI%\zrc.cpp
cl %CLOPT% %INC% %RTI%\zrcc.cpp
cl %CLOPT% %INC% /DMCS %CMN%\socket.cpp
cl %CLOPT% %INC% /DMCS /DCONSOLE_MODE %CMN%\common.cpp
link %LNKCON% %LIBS% %OBJ%

cl %CLOPT% %INCS% /DMCS %CMN%\common.cpp >nul
link %LNKWIN% %LIBS% %OBJ% >nul

goto DONE

:CYGWIN_OR_MINGW

set CMN=../../common
set RTI=%CMN%/rti
set INC=-I%CMN% -I%RTI%\include -I%RTI%
set LIBS=-lstdc++ -lws2_32 -lpsapi
set OBJ=rcAgent.o socket.o common.o info.o zrc.o zrcc.o %RTI%/rtilibinit.lib %RTI%/rtilib.lib 

set BIN=%PROGRAM_DRIVE%\pkg\win32\mingw\bin
%BIN%\windres -i rcAgent.rc -o info.o
%BIN%\gcc -c -g %INC% rcAgent.cpp
%BIN%\gcc -c -g %INC% %RTI%\zrc.cpp
%BIN%\gcc -c -g %INC% %RTI%\zrcc.cpp
%BIN%\gcc -c -g -DMCS %CMN%\socket.cpp
%BIN%\gcc -c -g -DMCS %CMN%\common.cpp
%BIN%\gcc -static -mwindows -o rcAgent %OBJ% %LIBS%

set BIN=%PROGRAM_DRIVE%\pkg\win32\cygwin\bin
%BIN%\windres -i rcAgent.rc -o info.o
%BIN%\gcc -c -g %INC% rcAgent.cpp
%BIN%\gcc -c -g %INC% %RTI%\zrc.cpp
%BIN%\gcc -c -g %INC% %RTI%\zrcc.cpp
%BIN%\gcc -c -g -DMCS %CMN%\socket.cpp
%BIN%\gcc -c -g -DMCS -DCONSOLE_MODE %CMN%\common.cpp
%BIN%\gcc -o rcAgentConsole %OBJ% %LIBS%

:DONE

del /q /f *.o *.obj *.pdb *.idb 2>nul 1>nul

