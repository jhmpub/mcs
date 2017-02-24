rem built with cygwin gcc - http://www.cygwin.com
gcc -c -DCONSOLE_MODE -DNO_MCS ..\..\common\common.cpp
gcc -c -DNO_MCS  ..\..\common\socket.cpp
gcc -c demo.cpp
gcc -o demo demo.o common.o socket.o -lws2_32 -lstdc++
rm.exe *.o
