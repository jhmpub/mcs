struct charToIrTx {
   const char c;
   struct irTx * irTx;
} keymap[] = {
   {'d',              irTxCmd[ITACH_CHANNEL_DOWN].remote},
   {'u',              irTxCmd[ITACH_CHANNEL_UP].remote},
   {'[',              irTxCmd[ITACH_CHAPTER_BACK].remote},
   {']',              irTxCmd[ITACH_CHAPTER_NEXT].remote},
   {'s',              irTxCmd[ITACH_DISPLAY].remote},
   {'{',              irTxCmd[ITACH_HDMI_SOURCE_SET_TOP].remote},   
   {'}',              irTxCmd[ITACH_HDMI_SOURCE_DVR].remote},   
   {'t',              irTxCmd[ITACH_HDMI_SOURCE_TOGGLE].remote},   
   {'e',              irTxCmd[ITACH_EJECT].remote},
   {'f',              irTxCmd[ITACH_FAST_FORWARD].remote},
   {'m',              irTxCmd[ITACH_MENU].remote},
   {'z',              irTxCmd[ITACH_MUTE].remote},
   {'0',              irTxCmd[ITACH_N0].remote},
   {'1',              irTxCmd[ITACH_N1].remote},
   {'2',              irTxCmd[ITACH_N2].remote},
   {'3',              irTxCmd[ITACH_N3].remote},
   {'4',              irTxCmd[ITACH_N4].remote},
   {'5',              irTxCmd[ITACH_N5].remote},
   {'6',              irTxCmd[ITACH_N6].remote},
   {'7',              irTxCmd[ITACH_N7].remote},
   {'8',              irTxCmd[ITACH_N8].remote},
   {'9',              irTxCmd[ITACH_N9].remote},
   {'o',              irTxCmd[ITACH_OK].remote},
   {' ',              irTxCmd[ITACH_PAUSE].remote},
   {'g',              irTxCmd[ITACH_PLAY].remote},
   {'p',              irTxCmd[ITACH_POWER].remote},
   {'c',              irTxCmd[ITACH_RECALL].remote},
   {'r',              irTxCmd[ITACH_RETURN].remote},
   {'b',              irTxCmd[ITACH_REWIND].remote},
   {'x',              irTxCmd[ITACH_STOP].remote},
   {'v',              irTxCmd[ITACH_VIDEO].remote},
   {'-',              irTxCmd[ITACH_VOLUME_DOWN].remote},
   {'+',              irTxCmd[ITACH_VOLUME_UP].remote},
   {CHAR_ARROW_UP,    irTxCmd[ITACH_MENU_UP].remote},
   {CHAR_ARROW_DOWN,  irTxCmd[ITACH_MENU_DOWN].remote},
   {CHAR_ARROW_RIGHT, irTxCmd[ITACH_MENU_RIGHT].remote},
   {CHAR_ARROW_LEFT,  irTxCmd[ITACH_MENU_LEFT].remote},
};   

void menu(void);
void sendMsg(struct irTx *);
struct irTx * getIrTx(char);


