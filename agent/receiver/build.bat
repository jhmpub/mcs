rem built with cygwin and mingw gcc - see media_control_system.pdf for compiler setup
rem Notes:
rem    1. targets must be compiled 32 bit to be compatible with the 32 bit tira2.dll
rem    2. cygwin compiler is used for termio console debugging
rem    3. the mingw compiler is used to create a toolchain independent production exe
rem       use ldd to verify

set BIN=%PROGRAM_DRIVE%\pkg\win32\cygwin\bin
%BIN%\windres -i arAgent.rc -o fileinfo.o
%BIN%\gcc -c -g arAgent.cpp
%BIN%\gcc -c -g ..\..\common\socket.cpp
%BIN%\gcc -c -g -DCONSOLE_MODE ..\..\common\common.cpp
%BIN%\gcc -o arAgentConsole arAgent.o socket.o common.o fileinfo.o -lws2_32 -lstdc++
rm.exe *.o

set BIN=%PROGRAM_DRIVE%\pkg\win32\mingw\bin
%BIN%\windres -i arAgent.rc -o fileinfo.o
%BIN%\gcc -c -g arAgent.cpp
%BIN%\gcc -c -g ..\..\common\socket.cpp
%BIN%\gcc -c -g ..\..\common\common.cpp
%BIN%\gcc -static -mwindows -o arAgent arAgent.o socket.o common.o fileinfo.o -lws2_32 -lstdc++
rm.exe *.o

