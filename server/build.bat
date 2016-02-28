rem built with cygwin gcc - http://www.cygwin.com
rem
windres -i acServer.rc -o fileinfo.o
gcc -c acServer.c
gcc -c ..\common\common.c
gcc -o acServerConsole acServer.o common.o fileinfo.o -lws2_32
gcc -mno-cygwin -mwindows -c acServer.c
gcc -mno-cygwin -mwindows -c ..\common\common.c
gcc -mno-cygwin -mwindows -o acServer acServer.o common.o fileinfo.o -lws2_32
rm.exe *.o
