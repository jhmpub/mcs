// compiles with cygwin gcc
// use the scancode.com utility to derive the required scan codes

#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <winuser.h>

// key press and release scan code sequences
#define SCANCODE_B           0x000030B0   
#define SCANCODE_F           0x000021A1   
#define SCANCODE_N           0x000031B1   
#define SCANCODE_P           0x00001999   
#define SCANCODE_Z           0x00002CAC
#define SCANCODE_ALTF4       0x383EBEB8   // program exit
#define SCANCODE_ENTER       0x00001C9C   
#define SCANCODE_SPACE       0x000039B9   
#define SCANCODE_ARROW_UP    0xE048E0C8
#define SCANCODE_ARROW_DOWN  0xE050E0D0
                         
#define  SCANCODE_KEY_RELEASE_FLAG  0x80
#define  SCANCODE_ESCAPE            0xE0

void generateKey(int);
void sendKeyScanCode(int);

int main() {
   Sleep(5000);  // delay 5 seconds to switch to a "test" window
//   generateKey(SCANCODE_ARROW_DOWN);
//   generateKey(SCANCODE_ARROW_UP);
//   generateKey(SCANCODE_ALTF4);
   generateKey(SCANCODE_B);
   generateKey(SCANCODE_F);
   generateKey(SCANCODE_SPACE);
   generateKey(SCANCODE_N);
   generateKey(SCANCODE_P);
   generateKey(SCANCODE_Z);
   generateKey(SCANCODE_ENTER);
}   


void generateKey(int wScan) {
   int shift=32;
   int scancode;
   
   while (shift) {
      shift-=8;
      sendKeyScanCode(wScan>>shift);
   }
}


void sendKeyScanCode (int wScan)
{
   KEYBDINPUT  kbd;
   INPUT       input;
   
   wScan&=0xFF;
   
   if (!wScan)
      return;
  
   bzero((char *)&kbd, sizeof(KEYBDINPUT));
   bzero((char *)&input, sizeof(INPUT));
  
   kbd.dwFlags |= KEYEVENTF_SCANCODE;
   if ((wScan & SCANCODE_KEY_RELEASE_FLAG) && wScan!=SCANCODE_ESCAPE) {
      // scan codes greater than 127 other than escape indicate a key release
      kbd.dwFlags |=  KEYEVENTF_KEYUP;
   }   
   kbd.wScan = wScan;
   input.type = INPUT_KEYBOARD;
   input.ki = kbd;
  
   SendInput(1,&input,sizeof(INPUT));
}

