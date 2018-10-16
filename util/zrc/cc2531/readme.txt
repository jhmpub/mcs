Texas Instruments offers an evaluation board usb dongle with a CC2531 system 
on a chip specifically designed for zigbee applications.  This implementation
was developed to support PC remote control with the following configuration:

Windows PC CC2531 USB Dongle <-> Comcast RF4CE Set Top Box

A CC2531 USB dongle must be programmed with Texas Instrument's 
RNP-CC2531F256.hex firmware file using their CC Debugger and free SmartRF 
flash programming software.  The PC host user application links to TI's API
library to interface with the Remoti Network Processor firmware:

user PC app <-> RemoTI API <-> RNP USB dongle

The CC Debugger and CC2531 usb dongle can be purchased at https://store.ti.com.
Search for parts CC-DEBUGGER and CC2531EMK.  These items may also be available 
on ebay at lower cost.  As of this writing, TI is selling these devices for $49
each and ebay has sellers offering them for around $10.

Alternatively, a second CC2531 USB dongle can be flashed with their
sniffer_fw__cc2531.hex file and used as a sniffer for engineering development 
and troubleshooting.

The sniffer software is freely available from:
http://www.ti.com/tool/packet-sniffer

Installation Steps:

1. Run Setup_SmartRF_Flash_Programmer-1.12.8.exe to install
   the flash programmer.
2. Cable the CC Debugger to the CC2531 usb dongle like the
   jpg picture in the same directory as this readme
3. Use the SmartRF Flash Programmer and CC Debugger to program
   the CC2531 usb dongle with the RNP-CC2531F256.hex file
4. Configure the RNP usb driver on the Windows PC:
   Open Device Manager by running "devmgmt.msc"
   Right click on Other devices\RemoTI CC2531 CDC and select 
   "Update Driver Software..."
   Select "Browse my computer for driver software"
   Browse to the folder containing this readme and more 
   importantly the file usb_RemoTICdcDriver.inf
   Select "Next"
   Ignore the Windows Security warning and select
   "Install this driver software anyway"
5. Run the "../demo.exe <comN>" utility to pair the CC2531
   with a Comcast set top box
 