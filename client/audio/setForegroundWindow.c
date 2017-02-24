// Calling this utility with an application's window title
// will restore the first matching window and set it to 
// the foreground
//
// compiles with mingw gcc - http://mingw.org
// gcc -g -static -mwindows -o setForegroundWindow setForegroundWindow.c
//
// or debug mode:
// gcc -g -static -o setForegroundWindow setForegroundWindow.c

#include <stdio.h>
#include <windows.h>

int main(int argc, char ** argv) {

   if (argc!=2)
       printf("usage: %s <window title>\n", argv[0]);
       
   HWND hWnd = FindWindow(NULL, argv[1]);
   
   printf("%s handle %s\n", 
           argv[1], 
           hWnd!=NULL ? "found" : "not found");
   
   if (hWnd==NULL) 
       exit(1);
               
   ShowWindow(hWnd, SW_RESTORE);
   SetForegroundWindow(hWnd);        
   
   exit(0);
} 
