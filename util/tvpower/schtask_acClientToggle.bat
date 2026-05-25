rem Abstract: power on/off an LG TV and a receiver amplifier using a single
rem desktop shortcut icon
rem 
rem n.b. the following prerequisites: 
rem 1. wperl.exe must be in the search path
rem 2. update TV_POWER_PATH variable below for the local installation
rem 3. run this batch file on the htpc
rem 4. configure ssh between the remote host and the htpc.
rem    see sshcmd.pl for details
rem 5. update site specific variables in %TV_POWER_PATH%\acClientToggle.pl
rem 6. create a shortcut icon that runs
rem    wperl.exe %TV_POWER_PATH%\sshcmd.pl <htpc_host> "schtasks /run /tn acClientToggle"
rem
rem 2026 May 09 jhm original creation

set TV_POWER_PATH="%data_drive%\home\jhm\src\mcs\util\tvpower"
schtasks /create /tn acClientToggle /rl HIGHEST /tr "wperl.exe %TV_POWER_PATH%\acClientToggle.pl" /sc once /st 00:00
