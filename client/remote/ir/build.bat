rem built with cygwin gcc - http://www.cygwin.com
rem
windres -i rcClient.rc -o fileinfo.o
gcc -DCONSOLE_MODE -c rcClient.c
gcc -c ..\..\..\common\common.c
gcc -o rcClientConsole rcClient.o common.o fileinfo.o -lws2_32
gcc -mno-cygwin -mwindows -c rcClient.c
gcc -mno-cygwin -mwindows -c ..\..\..\common\common.c
gcc -mno-cygwin -mwindows -o rcClient rcClient.o common.o fileinfo.o -lws2_32
rm.exe *.o
