rem build with mingw or cygwin unless enviornment variable MSVC is defined

if not defined MSVC goto CYGWIN_OR_MINGW

set CMN=..\..\common
set RTI=%CMN%\rti
set OBJ=demo.obj common.obj zrc.obj zrcc.obj
set INC=/I %CMN% /I %RTI%\include /I %RTI%
set CLOPT=/nologo /c /ZI /EHsc
set LNKOPT=/nologo /opt:noref /opt:noicf /nodefaultlib:libcmtd /libpath:%RTI%
set LIBS=rtilibinit.lib rtilib.lib user32.lib

cl %CLOPT% %INC% demo.cpp
cl %CLOPT% %INC% %RTI%\zrc.cpp
cl %CLOPT% %INC% %RTI%\zrcc.cpp
cl %CLOPT% %INC% /DCONSOLE_MODE %CMN%\common.cpp
link %LNKOPT% %LIBS% %OBJ%

goto DONE

:CYGWIN_OR_MINGW

set CMN=../../common
set RTI=%CMN%/rti
set INC=-I%CMN% -I%RTI%\include -I%RTI%
set LIBS=-lstdc++
set OBJ=demo.o common.o zrc.o zrcc.o %RTI%/rtilibinit.lib %RTI%/rtilib.lib 
set BIN=%PROGRAM_DRIVE%\pkg\win32\mingw\bin

%BIN%\gcc -c -g %INC% demo.cpp
%BIN%\gcc -c -g %INC% %RTI%\zrc.cpp
%BIN%\gcc -c -g %INC% %RTI%\zrcc.cpp
%BIN%\gcc -c -g -DCONSOLE_MODE %CMN%\common.cpp
%BIN%\gcc -static -o demo %OBJ% %LIBS%

:DONE

del /q /f *.o *.obj *.pdb *.idb 2>nul 1>nul

