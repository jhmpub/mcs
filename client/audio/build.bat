@echo off
rem acclient built with Sun java SE - http://java.sun.com
rem setforegroundwindow built with mingw gcc - http://mingw.org
if "%1" == "-all" (
   set BIN=%PROGRAM_DRIVE%\pkg\win32\mingw\bin
   %BIN%\gcc -g -static -mwindows -o setForegroundWindow setForegroundWindow.c
)   
javac.exe acclient.java
jar.exe cvmf manifest AcClient.jar *.class
del *.class >nul