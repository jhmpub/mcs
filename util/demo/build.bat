rem built with cygwin gcc - http://www.cygwin.com
gcc -c ..\..\common\common.c
gcc -c demo.c
gcc -o demo demo.o common.o -lws2_32
rm.exe *.o
