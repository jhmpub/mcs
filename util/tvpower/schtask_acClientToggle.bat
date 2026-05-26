rem Abstract: power on/off an LG TV and a receiver amplifier using a single
rem desktop shortcut icon.
rem 
rem please see the readme.txt file in this file's directory for configuration 
rem steps
rem
rem 2026 May 09 jhm original creation

rem update TV_POWER_PATH for the local installation
set TV_POWER_PATH="%data_drive%\home\jhm\src\mcs\util\tvpower"
schtasks /create /tn acClientToggle /rl HIGHEST /tr "wperl.exe %TV_POWER_PATH%\acClientToggle.pl" /sc once /st 00:00
