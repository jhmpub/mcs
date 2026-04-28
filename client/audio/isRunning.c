// Calling this utility with an application's window title
// returns TRUE if found or FALSE if not found 
//
// compiles with cygwin gcc - http://cygwin.com
// gcc -mno-cygwin -mwindows -o isRunning isRunning.c
//
// or debug mode:
// gcc -o isRunning isRunning.c
// 

#include <stdio.h>
#include <windows.h>

int main(int argc, char ** argv) {
   
   if (argc!=2)
       printf("usage: %s <window title>\n", argv[0]);
       
   HWND hWnd = FindWindow(NULL, argv[1]);
   
   if (hWnd==NULL) 
       exit(0);
   
   exit(1);
} 
