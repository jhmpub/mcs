- abstract
  lgtv.exe is a command line utility to control an lg tv 
  lgtv_gui.exe is the same utility without a console
  LG TVs use the WebSocket Secure (WSS) interface protocol
   
  2026 Apr 7 jhm ported from
  https://github.com/klattimer/LGWebOSRemote

- pip install <module>
  pyinstaller==6.19.0
  wakeonlan==1.1.6
  ws4py==0.6.0
  requests==2.32.4
  getmac==0.9.2

- LG TV settings
  gear icon > gear icon > General > Always Ready
  Enable the function: on
  Use always ready without wallpaper: check

- lgtv.exe usage
  setup commands are a prerequisite for any general command 
  <name> refers to a unique authorization object in c:\users\<user>\.lgtv\config.json
  <host> tv hostname or ip address
  lgtv --help for a --ssl command list

  general command format
    lgtv --name <name> --ssl <cmd> <args>

  setup commands
    lgtv --ssl auth <host> <name>
    lgtv setDefault <name>

  power on/off commands
    lgtv --ssl on
    lgtv --ssl off

  e.g.
  ping lgtv
  lgtv --ssl auth lgtv lgtv
  lgtv setDefault lgtv
  lgtv --ssl on
  lgtv --ssl off

