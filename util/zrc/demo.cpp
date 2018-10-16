//
//   This software was written to pair a Texas Instruments CC2531 
//   USB dongle with any Comcast set top box that supports an XR11
//   remote control.  After pairing, this software provides remote 
//   control functionality from a console interface:
//
//   Windows PC CC2531 USB Dongle <-> Comcast RF4CE Set Top Box
//
//   The RF4CE API library files are from Texas Instruments Remoti
//   software v1.3.1, a freely available RF4CE Zigbee Remote 
//   Control compliant protocol stack.  
//   http://www.ti.com/tool/REMOTI
//
//   Build using the cygwin, mingw, or microsoft c compiler
//   See mcs/doc/media_control_system.pdf for compiler setup information
//   Run batch.bat to compile
//
//   2018 Jun 12 jhm original creation 

// direct zrcc.h to not compile this module as a controller or a target
#define HOST_CMN TRUE  
#include "zrcc.h"

int main(int argc, char **argv) {

   // load the Remoti Network Processor library and connect to the device
   const char * comPort = argc==2 ? argv[1] : DEFAULT_ZRC_COM_PORT;
   initZigbeeRemoteControl(comPort);
   
   zrcMenuLoop();
   
}






