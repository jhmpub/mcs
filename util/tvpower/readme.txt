Abstract: power on/off an LG TV and a receiver amplifier using a single
desktop shortcut icon

n.b. steps to configure
1. wperl.exe must be in the search path
2. update TV_POWER_PATH variable below for the local installation
3. run the schtask_acClientToggle.bat file on the htpc
4. configure ssh between the remote host and the htpc.
   see sshcmd.pl for details
5. update site specific variables in %TV_POWER_PATH%\acClientToggle.pl
6. create a shortcut icon that runs
   wperl.exe %TV_POWER_PATH%\sshcmd.pl <htpc_host> "schtasks /run /tn acClientToggle"
