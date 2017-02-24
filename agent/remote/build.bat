rem built with cygwin and mingw gcc - see media_control_system.pdf for compiler setup
rem Notes:
rem    1. targets must be compiled 32 bit to be compatible with the 32 bit tira2.dll
rem    2. cygwin compiler is used for termio console debugging
rem    3. the mingw compiler is used to create a toolchain independent production exe
rem       use ldd to verify
rem    4. rcagent synthesizes keyboard input so it can't be run as a service

set BIN=%PROGRAM_DRIVE%\pkg\win32\cygwin\bin
%BIN%\windres -i rcAgent.rc -o fileinfo.o
%BIN%\gcc -g -c rcAgent.cpp
%BIN%\gcc -g -c ..\..\common\socket.cpp
%BIN%\gcc -g -c -DCONSOLE_MODE ..\..\common\common.cpp
%BIN%\gcc -o rcAgentConsole rcAgent.o socket.o common.o fileinfo.o -lpsapi -lws2_32 -lstdc++
rm.exe *.o

set BIN=%PROGRAM_DRIVE%\pkg\win32\mingw\bin
%BIN%\windres -i rcAgent.rc -o fileinfo.o
%BIN%\gcc -g -c rcAgent.cpp
%BIN%\gcc -g -c ..\..\common\socket.cpp
%BIN%\gcc -g -c ..\..\common\common.cpp
%BIN%\gcc -static -mwindows -o rcAgent rcAgent.o socket.o common.o fileinfo.o -lpsapi -lws2_32 -lstdc++
rm.exe *.o
