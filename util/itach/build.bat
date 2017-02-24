rem built with cygwin gcc - http://www.cygwin.com
gcc -g -c -DNO_MCS -DCONSOLE_MODE ..\..\common\common.cpp
gcc -g -c -DNO_MCS ..\..\common\socket.cpp
gcc -g -c demo.cpp
gcc -o demo demo.o socket.o common.o -l ws2_32 -lstdc++
rm.exe *.o
