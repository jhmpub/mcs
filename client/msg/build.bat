rem built with mingw gcc - http://mingw.org
set BIN=%PROGRAM_DRIVE%\pkg\win32\mingw\bin
%BIN%\gcc -g -DNO_COMMON -DNO_MCS -c ..\..\common\socket.cpp
%BIN%\gcc -g -c msg.cpp
%BIN%\gcc -g -static -o msg msg.o socket.o -lws2_32 -lstdc++
rm.exe *.o
