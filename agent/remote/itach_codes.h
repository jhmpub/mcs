////
//// itach wifi to ir codes
////
//// This file is automatically generated by mcs/util/itach/itach_codes.pl
////

enum remote {
   TV_MODE,
   DVD_MODE,
   VCR_MODE,
   REMOTE_MODES,
   UNDEFINED_MODE
};

struct irTx {
   const char * code;   // itach ir codes, NULL if n/a
   const char * sz;     // command message and debug description
};


// itach structures indexed by the remote enum

static struct irTx irTxChannelDown[REMOTE_MODES] = {
   {
      "sendir,1:3,1,36764,1,1,128,64,17,15,17,47,17,15,17,15,17,15,17,15,17"
      ",15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,1"
      "5,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15,"
      "17,15,17,15,17,15,17,47,17,15,17,47,17,15,17,47,17,47,17,15,17,15,17"
      ",47,17,15,17,47,17,15,17,47,17,47,17,15,17,47,17,3600\r",

      "tv channel down"
   },

   {
      "sendir,1:3,2,40064,1,1,341,171,21,64,21,64,21,22,21,22,21,22,21,64,2"
      "1,22,21,64,21,22,21,22,21,64,21,64,21,64,21,22,21,64,21,22,21,64,21,"
      "22,21,64,21,64,21,64,21,22,21,22,21,64,21,22,21,64,21,22,21,22,21,22"
      ",21,64,21,64,21,22,21,1024,341,171,21,64,21,64,21,22,21,22,21,22,21,"
      "64,21,22,21,64,21,22,21,22,21,64,21,64,21,64,21,22,21,64,21,22,21,64"
      ",21,22,21,64,21,64,21,64,21,22,21,22,21,64,21,22,21,64,14,4000\r",

      "dvd chapter back"
   },

   {
      "sendir,1:3,3,35919,1,1,32,31,32,31,64,31,32,62,64,62,32,31,64,31,32,"
      "31,32,31,32,62,32,4597\r",

      "vcr channel down"
   }
};

static struct irTx irTxChannelUp[REMOTE_MODES] = {
   {
      "sendir,1:3,4,36764,1,1,128,64,17,15,17,47,17,15,17,15,17,15,17,15,17"
      ",15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,1"
      "5,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15,"
      "17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,47,17,47,17,15,17,15,17"
      ",15,17,15,17,47,17,15,17,47,17,47,17,15,17,47,17,3600\r",

      "tv channel up"
   },

   {
      "sendir,1:3,5,40064,1,1,341,171,21,64,21,64,21,22,21,22,21,22,21,64,2"
      "1,22,21,64,21,22,21,22,21,64,21,64,21,64,21,22,21,64,21,22,21,22,21,"
      "22,21,64,21,64,21,64,21,22,21,22,21,64,21,64,21,64,21,22,21,22,21,22"
      ",21,64,21,64,21,22,21,1023\r",

      "dvd chapter next"
   },

   {
      "sendir,1:3,6,35919,1,1,32,31,64,31,32,31,32,62,64,62,32,31,64,31,32,"
      "31,32,31,32,31,32,4597\r",

      "vcr channel up"
   }
};

static struct irTx irTxChapterBack[REMOTE_MODES] = {
   {
      NULL,

      "tv chapter back (not defined)"
   },

   {
      "sendir,1:3,7,40064,1,1,341,171,21,64,21,64,21,22,21,22,21,22,21,64,2"
      "1,22,21,64,21,22,21,22,21,64,21,64,21,64,21,22,21,64,21,22,21,64,21,"
      "22,21,64,21,64,21,64,21,22,21,22,21,64,21,22,21,64,21,22,21,22,21,22"
      ",21,64,21,64,21,22,21,1024,341,171,21,64,21,64,21,22,21,22,21,22,21,"
      "64,21,22,21,64,21,22,21,22,21,64,21,64,21,64,21,22,21,64,21,22,21,64"
      ",21,22,21,64,21,64,21,64,21,22,21,22,21,64,21,22,21,64,14,4000\r",

      "dvd chapter back"
   },

   {
      NULL,

      "vcr chapter back (not defined)"
   }
};

static struct irTx irTxChapterNext[REMOTE_MODES] = {
   {
      NULL,

      "tv chapter next (not defined)"
   },

   {
      "sendir,1:3,8,40064,1,1,341,171,21,64,21,64,21,22,21,22,21,22,21,64,2"
      "1,22,21,64,21,22,21,22,21,64,21,64,21,64,21,22,21,64,21,22,21,22,21,"
      "22,21,64,21,64,21,64,21,22,21,22,21,64,21,64,21,64,21,22,21,22,21,22"
      ",21,64,21,64,21,22,21,1023\r",

      "dvd chapter next"
   },

   {
      NULL,

      "vcr chapter next (not defined)"
   }
};

static struct irTx irTxDisplay[REMOTE_MODES] = {
   {
      "sendir,1:3,9,37202,1,1,131,64,17,15,17,47,17,15,17,15,17,15,17,15,17"
      ",15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,1"
      "5,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15,"
      "17,15,17,15,17,15,17,47,17,15,17,15,17,47,17,47,17,47,17,15,17,15,17"
      ",47,17,15,17,15,17,47,17,47,17,47,17,15,17,47,17,3700\r",

      "tv display"
   },

   {
      "sendir,1:3,10,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,6"
      "3,22,63,22,20,22,20,22,20,22,63,22,63,22,63,22,20,22,20,22,63,22,63,"
      "22,63,22,20,22,20,22,20,22,5144\r",

      "dvd display"
   },

   {
      NULL,

      "vcr display (not defined)"
   }
};

static struct irTx irTxEject[REMOTE_MODES] = {
   {
      NULL,

      "tv eject (not defined)"
   },

   {
      "sendir,1:3,11,40064,1,1,341,171,21,64,21,64,21,22,21,22,21,22,21,64,"
      "21,22,21,64,21,22,21,22,21,64,21,64,21,64,21,22,21,64,21,22,21,64,21"
      ",22,21,22,21,64,21,64,21,22,21,22,21,64,21,22,21,64,21,64,21,22,21,2"
      "2,21,64,21,64,21,22,21,1023,341,171,21,64,21,64,21,64,21,64,21,22,21"
      ",64,21,22,21,64,21,22,21,22,21,22,21,22,21,64,21,22,21,64,21,22,21,2"
      "2,21,64,21,64,21,22,21,64,21,64,21,22,21,64,21,64,21,22,21,22,21,64,"
      "21,22,21,22,21,64,21,22,21,1023\r",

      "dvd eject"
   },

   {
      NULL,

      "vcr eject (not defined)"
   }
};

static struct irTx irTxFastForward[REMOTE_MODES] = {
   {
      NULL,

      "tv fast forward (not defined)"
   },

   {
      "sendir,1:3,12,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,20,22"
      ",63,22,20,22,63,22,63,22,20,22,20,22,63,22,63,22,20,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1191,343,171,22,63,22,63,22,20,22,20,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,2"
      "0,22,63,22,20,22,63,22,63,22,20,22,20,22,63,22,63,22,20,22,63,22,20,"
      "22,20,22,63,22,63,22,20,22,5144\r",

      "dvd fast forward"
   },

   {
      "sendir,1:3,13,35919,1,1,32,31,64,31,32,31,32,62,64,62,32,31,32,31,64"
      ",62,64,31,32,4597\r",

      "vcr fast forward"
   }
};

static struct irTx irTxHdmiSourceDvd[REMOTE_MODES] = {
   {
      "sendir,1:3,14,37764,1,1,343,171,22,21,22,63,22,21,22,21,22,21,22,21,"
      "22,21,22,21,22,63,22,21,22,63,22,63,22,63,22,63,22,63,22,63,22,21,22"
      ",63,22,21,22,63,22,63,22,21,22,63,22,63,22,63,22,21,22,63,22,21,22,2"
      "1,22,63,22,21,22,21,22,1517,343,85,22,3653,343,85,22,4833\r",

      "hdmi source dvd"
   },

   {
      "sendir,1:3,15,37764,1,1,343,171,22,21,22,63,22,21,22,21,22,21,22,21,"
      "22,21,22,21,22,63,22,21,22,63,22,63,22,63,22,63,22,63,22,63,22,21,22"
      ",63,22,21,22,63,22,63,22,21,22,63,22,63,22,63,22,21,22,63,22,21,22,2"
      "1,22,63,22,21,22,21,22,1517,343,85,22,3653,343,85,22,4833\r",

      "hdmi source dvd"
   },

   {
      "sendir,1:3,16,37764,1,1,343,171,22,21,22,63,22,21,22,21,22,21,22,21,"
      "22,21,22,21,22,63,22,21,22,63,22,63,22,63,22,63,22,63,22,63,22,21,22"
      ",63,22,21,22,63,22,63,22,21,22,63,22,63,22,63,22,21,22,63,22,21,22,2"
      "1,22,63,22,21,22,21,22,1517,343,85,22,3653,343,85,22,4833\r",

      "hdmi source dvd"
   }
};

static struct irTx irTxHdmiSourceDvr[REMOTE_MODES] = {
   {
      "sendir,1:3,17,37650,1,1,343,171,22,21,22,63,22,21,22,21,22,21,22,21,"
      "22,21,22,21,22,63,22,21,22,63,22,63,22,63,22,63,22,63,22,63,22,63,22"
      ",63,22,21,22,63,22,63,22,21,22,63,22,63,22,21,22,21,22,63,22,21,22,2"
      "1,22,63,22,21,22,21,22,1513,343,85,22,3644,343,85,22,4819\r",

      "hdmi source dvr"
   },

   {
      "sendir,1:3,18,37650,1,1,343,171,22,21,22,63,22,21,22,21,22,21,22,21,"
      "22,21,22,21,22,63,22,21,22,63,22,63,22,63,22,63,22,63,22,63,22,63,22"
      ",63,22,21,22,63,22,63,22,21,22,63,22,63,22,21,22,21,22,63,22,21,22,2"
      "1,22,63,22,21,22,21,22,1513,343,85,22,3644,343,85,22,4819\r",

      "hdmi source dvr"
   },

   {
      "sendir,1:3,19,37650,1,1,343,171,22,21,22,63,22,21,22,21,22,21,22,21,"
      "22,21,22,21,22,63,22,21,22,63,22,63,22,63,22,63,22,63,22,63,22,63,22"
      ",63,22,21,22,63,22,63,22,21,22,63,22,63,22,21,22,21,22,63,22,21,22,2"
      "1,22,63,22,21,22,21,22,1513,343,85,22,3644,343,85,22,4819\r",

      "hdmi source dvr"
   }
};

static struct irTx irTxHdmiSourceSetTop[REMOTE_MODES] = {
   {
      "sendir,1:3,20,37764,1,1,343,171,22,21,22,63,22,21,22,21,22,21,22,21,"
      "22,21,22,21,22,63,22,21,22,63,22,63,22,63,22,63,22,63,22,63,22,21,22"
      ",63,22,63,22,63,22,63,22,21,22,63,22,63,22,63,22,21,22,21,22,21,22,2"
      "1,22,63,22,21,22,21,22,1517,343,85,22,3653,343,85,22,4833\r",

      "hdmi source set top"
   },

   {
      "sendir,1:3,21,37764,1,1,343,171,22,21,22,63,22,21,22,21,22,21,22,21,"
      "22,21,22,21,22,63,22,21,22,63,22,63,22,63,22,63,22,63,22,63,22,21,22"
      ",63,22,63,22,63,22,63,22,21,22,63,22,63,22,63,22,21,22,21,22,21,22,2"
      "1,22,63,22,21,22,21,22,1517,343,85,22,3653,343,85,22,4833\r",

      "hdmi source set top"
   },

   {
      "sendir,1:3,22,37764,1,1,343,171,22,21,22,63,22,21,22,21,22,21,22,21,"
      "22,21,22,21,22,63,22,21,22,63,22,63,22,63,22,63,22,63,22,63,22,21,22"
      ",63,22,63,22,63,22,63,22,21,22,63,22,63,22,63,22,21,22,21,22,21,22,2"
      "1,22,63,22,21,22,21,22,1517,343,85,22,3653,343,85,22,4833\r",

      "hdmi source set top"
   }
};

static struct irTx irTxHdmiSourceToggle[REMOTE_MODES] = {
   {
      "sendir,1:3,23,37764,1,1,343,171,22,21,22,63,22,21,22,21,22,21,22,21,"
      "22,21,22,21,22,63,22,21,22,63,22,63,22,63,22,63,22,63,22,63,22,63,22"
      ",21,22,21,22,63,22,63,22,21,22,63,22,63,22,21,22,63,22,63,22,21,22,2"
      "1,22,63,22,21,22,21,22,1517,343,85,22,3652,343,85,22,4833\r",

      "hdmi source toggle"
   },

   {
      "sendir,1:3,24,37764,1,1,343,171,22,21,22,63,22,21,22,21,22,21,22,21,"
      "22,21,22,21,22,63,22,21,22,63,22,63,22,63,22,63,22,63,22,63,22,63,22"
      ",21,22,21,22,63,22,63,22,21,22,63,22,63,22,21,22,63,22,63,22,21,22,2"
      "1,22,63,22,21,22,21,22,1517,343,85,22,3652,343,85,22,4833\r",

      "hdmi source toggle"
   },

   {
      "sendir,1:3,25,37764,1,1,343,171,22,21,22,63,22,21,22,21,22,21,22,21,"
      "22,21,22,21,22,63,22,21,22,63,22,63,22,63,22,63,22,63,22,63,22,63,22"
      ",21,22,21,22,63,22,63,22,21,22,63,22,63,22,21,22,63,22,63,22,21,22,2"
      "1,22,63,22,21,22,21,22,1517,343,85,22,3652,343,85,22,4833\r",

      "hdmi source toggle"
   }
};

static struct irTx irTxMenu[REMOTE_MODES] = {
   {
      "sendir,1:3,26,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,47,17,15,17,47,17,15,1"
      "7,15,17,47,17,15,17,15,17,47,17,15,17,47,17,47,17,3700\r",

      "tv menu"
   },

   {
      "sendir,1:3,27,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,6"
      "3,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22,63,22,20,"
      "22,20,22,20,22,63,22,20,22,5144\r",

      "dvd menu"
   },

   {
      "sendir,1:3,28,35919,1,1,64,31,32,31,32,31,32,62,64,62,64,62,64,31,32"
      ",62,64,4597\r",

      "vcr menu"
   }
};

static struct irTx irTxMenuDown[REMOTE_MODES] = {
   {
      "sendir,1:3,29,36764,1,1,128,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,15,17,47,17,15,17,47,17,47,17,15,17,15,1"
      "7,47,17,15,17,47,17,15,17,47,17,47,17,15,17,47,17,3600\r",

      "tv menu down"
   },

   {
      "sendir,1:3,30,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,6"
      "3,22,63,22,20,22,20,22,63,22,63,22,63,22,63,22,20,22,20,22,63,22,63,"
      "22,20,22,20,22,20,22,20,22,5144\r",

      "dvd menu down"
   },

   {
      "sendir,1:3,31,35919,1,1,64,62,64,31,32,62,64,62,64,62,64,31,32,31,32"
      ",62,32,4597\r",

      "vcr menu down"
   }
};

static struct irTx irTxMenuLeft[REMOTE_MODES] = {
   {
      "sendir,1:3,32,36764,1,1,128,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,47,17,15,17,15,1"
      "7,47,17,15,17,15,17,15,17,15,17,47,17,15,17,47,17,3600\r",

      "tv menu left"
   },

   {
      "sendir,1:3,33,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,6"
      "3,22,63,22,20,22,20,22,20,22,63,22,63,22,20,22,20,22,20,22,63,22,63,"
      "22,63,22,20,22,20,22,63,22,5144\r",

      "dvd menu left"
   },

   {
      "sendir,1:3,34,35919,1,1,64,31,32,31,32,31,32,62,64,62,64,62,64,62,64"
      ",62,32,4597\r",

      "vcr menu left"
   }
};

static struct irTx irTxMenuRight[REMOTE_MODES] = {
   {
      "sendir,1:3,35,36764,1,1,128,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,47,17,15,17,47,17,3600\r",

      "tv menu right"
   },

   {
      "sendir,1:3,36,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,2"
      "0,22,20,22,63,22,20,22,20,22,63,22,63,22,20,22,63,22,63,22,20,22,63,"
      "22,63,22,20,22,20,22,63,22,5144\r",

      "dvd menu right"
   },

   {
      "sendir,1:3,37,35919,1,1,64,31,32,31,32,31,32,62,64,62,64,62,64,62,32"
      ",31,64,4597\r",

      "vcr menu right"
   }
};

static struct irTx irTxMenuUp[REMOTE_MODES] = {
   {
      "sendir,1:3,38,36764,1,1,128,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,47,17,47,17,15,17,15,1"
      "7,15,17,15,17,47,17,15,17,47,17,47,17,15,17,47,17,3600\r",

      "tv menu up"
   },

   {
      "sendir,1:3,39,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,2"
      "0,22,63,22,20,22,20,22,63,22,63,22,63,22,63,22,63,22,20,22,63,22,63,"
      "22,20,22,20,22,20,22,20,22,5144\r",

      "dvd menu up"
   },

   {
      "sendir,1:3,40,35919,1,1,64,62,64,31,32,62,64,62,64,62,64,31,32,31,32"
      ",31,32,4597\r",

      "vcr menu up"
   }
};

static struct irTx irTxMute[REMOTE_MODES] = {
   {
      "sendir,1:3,41,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,47,17,47,17,15,17,15,1"
      "7,15,17,47,17,15,17,15,17,47,17,47,17,15,17,47,17,3700\r",

      "mute"
   },

   {
      "sendir,1:3,42,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,47,17,47,17,15,17,15,1"
      "7,15,17,47,17,15,17,15,17,47,17,47,17,15,17,47,17,3700\r",

      "mute"
   },

   {
      "sendir,1:3,43,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,47,17,47,17,15,17,15,1"
      "7,15,17,47,17,15,17,15,17,47,17,47,17,15,17,47,17,3700\r",

      "mute"
   }
};

static struct irTx irTxN0[REMOTE_MODES] = {
   {
      "sendir,1:3,44,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,15,17,15,17,47,17,47,17,15,17,15,17,15,1"
      "7,47,17,15,17,15,17,47,17,47,17,15,17,15,17,47,17,3700\r",

      "tv n0"
   },

   {
      "sendir,1:3,45,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,2"
      "0,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,63,22,63,22,63,22,63,"
      "22,63,22,20,22,63,22,20,22,5144\r",

      "dvd n0"
   },

   {
      "sendir,1:3,46,35919,1,1,32,31,32,31,64,31,32,62,64,62,64,31,32,31,32"
      ",31,32,31,32,31,32,4597\r",

      "vcr n0"
   }
};

static struct irTx irTxN1[REMOTE_MODES] = {
   {
      "sendir,1:3,47,37202,1,1,131,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,47,17,15,17,15,17,47,17,3700\r",

      "tv n1"
   },

   {
      "sendir,1:3,48,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,6"
      "3,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,63,22,63,22,63,"
      "22,63,22,20,22,63,22,20,22,5144\r",

      "dvd n1"
   },

   {
      "sendir,1:3,49,35919,1,1,32,31,64,31,32,31,32,62,64,62,64,31,32,31,32"
      ",31,32,31,32,62,32,4597\r",

      "vcr n1"
   }
};

static struct irTx irTxN2[REMOTE_MODES] = {
   {
      "sendir,1:3,50,36764,1,1,128,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,47,17,15,17,15,17,15,1"
      "7,47,17,15,17,15,17,15,17,47,17,15,17,15,17,47,17,2738\r",

      "tv n2"
   },

   {
      "sendir,1:3,51,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,2"
      "0,22,63,22,20,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,63,22,63,"
      "22,63,22,20,22,63,22,20,22,5144\r",

      "dvd n2"
   },

   {
      "sendir,1:3,52,35919,1,1,32,31,32,31,64,31,32,62,64,62,64,31,32,31,32"
      ",31,32,62,64,4597\r",

      "vcr n2"
   }
};

static struct irTx irTxN3[REMOTE_MODES] = {
   {
      "sendir,1:3,53,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,47,17,15,17,15,17,15,1"
      "7,15,17,47,17,15,17,15,17,47,17,15,17,15,17,47,17,3700\r",

      "tv n3"
   },

   {
      "sendir,1:3,54,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,6"
      "3,22,63,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,20,22,63,22,63,"
      "22,63,22,20,22,63,22,20,22,5144\r",

      "dvd n3"
   },

   {
      "sendir,1:3,55,35919,1,1,32,31,32,31,64,31,32,62,64,62,64,31,32,31,32"
      ",31,32,62,32,31,32,4597\r",

      "vcr n3"
   }
};

static struct irTx irTxN4[REMOTE_MODES] = {
   {
      "sendir,1:3,56,36764,1,1,128,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,47,17,15,17,15,17,47,17,15,17,15,17,15,1"
      "7,47,17,47,17,15,17,15,17,47,17,15,17,15,17,47,17,2738\r",

      "tv n4"
   },

   {
      "sendir,1:3,57,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,2"
      "0,22,20,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,63,22,20,22,63,"
      "22,63,22,20,22,63,22,20,22,5144\r",

      "dvd n4"
   },

   {
      "sendir,1:3,58,35919,1,1,32,31,32,31,64,31,32,62,64,62,64,31,32,31,32"
      ",62,64,31,32,3500\r",

      "vcr n4"
   }
};

static struct irTx irTxN5[REMOTE_MODES] = {
   {
      "sendir,1:3,59,37202,1,1,131,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,47,17,15,17,15,17,15,1"
      "7,15,17,15,17,47,17,15,17,47,17,15,17,15,17,47,17,3700\r",

      "tv n5"
   },

   {
      "sendir,1:3,60,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,6"
      "3,22,20,22,63,22,20,22,20,22,63,22,20,22,63,22,20,22,63,22,20,22,63,"
      "22,63,22,20,22,63,22,20,22,5144\r",

      "dvd n5"
   },

   {
      "sendir,1:3,61,35919,1,1,32,31,64,31,32,31,32,62,64,62,64,31,32,31,32"
      ",62,64,62,32,4597\r",

      "vcr n5"
   }
};

static struct irTx irTxN6[REMOTE_MODES] = {
   {
      "sendir,1:3,62,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,15,17,47,17,15,17,47,17,15,17,15,17,15,1"
      "7,47,17,15,17,47,17,15,17,47,17,15,17,15,17,47,17,3700\r",

      "tv n6"
   },

   {
      "sendir,1:3,63,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,20,22,63,"
      "22,63,22,20,22,63,22,20,22,5144\r",

      "dvd n6"
   },

   {
      "sendir,1:3,64,35919,1,1,32,31,32,31,64,31,32,62,64,62,64,31,32,31,32"
      ",62,32,31,64,4597\r",

      "vcr n6"
   }
};

static struct irTx irTxN7[REMOTE_MODES] = {
   {
      "sendir,1:3,65,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,47,17,47,17,15,17,47,17,15,17,15,17,15,1"
      "7,15,17,47,17,47,17,15,17,47,17,15,17,15,17,47,17,3700\r",

      "tv n7"
   },

   {
      "sendir,1:3,66,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,6"
      "3,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,20,22,20,22,20,22,63,"
      "22,63,22,20,22,63,22,20,22,5144\r",

      "dvd n7"
   },

   {
      "sendir,1:3,67,35919,1,1,32,31,64,31,32,31,32,62,64,62,64,31,32,31,32"
      ",62,32,31,32,31,32,4597\r",

      "vcr n7"
   }
};

static struct irTx irTxN8[REMOTE_MODES] = {
   {
      "sendir,1:3,68,37202,1,1,131,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,47,17,47,17,15,17,47,17,15,17,15,17,15,1"
      "7,47,17,47,17,47,17,15,17,47,17,15,17,15,17,47,17,3700\r",

      "tv n8"
   },

   {
      "sendir,1:3,69,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,2"
      "0,22,20,22,20,22,63,22,20,22,63,22,20,22,63,22,63,22,63,22,63,22,20,"
      "22,63,22,20,22,63,22,20,22,5144\r",

      "dvd n8"
   },

   {
      "sendir,1:3,70,35919,1,1,32,31,64,31,32,31,32,62,64,62,64,31,32,62,64"
      ",31,32,31,32,4597\r",

      "vcr n8"
   }
};

static struct irTx irTxN9[REMOTE_MODES] = {
   {
      "sendir,1:3,71,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,47,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,47,17,47,17,15,17,15,17,47,17,3700\r",

      "tv n9"
   },

   {
      "sendir,1:3,72,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,6"
      "3,22,20,22,20,22,63,22,20,22,63,22,20,22,63,22,20,22,63,22,63,22,20,"
      "22,63,22,20,22,63,22,20,22,5144\r",

      "dvd n9"
   },

   {
      "sendir,1:3,73,35919,1,1,32,31,32,31,64,31,32,62,64,62,64,31,32,62,64"
      ",31,32,62,32,4597\r",

      "vcr n9"
   }
};

static struct irTx irTxOk[REMOTE_MODES] = {
   {
      "sendir,1:3,74,36764,1,1,128,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,15,17,15,17,47,17,15,17,15,17,47,17,15,1"
      "7,47,17,15,17,15,17,47,17,15,17,15,17,47,17,47,17,3600\r",

      "tv ok"
   },

   {
      "sendir,1:3,75,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,6"
      "3,22,63,22,63,22,63,22,20,22,63,22,63,22,63,22,20,22,20,22,20,22,20,"
      "22,63,22,20,22,20,22,20,22,1192,343,171,22,63,15,5144\r",

      "dvd ok"
   },

   {
      "sendir,1:3,76,35919,1,1,64,62,64,31,32,62,64,62,64,62,64,62,32,31,32"
      ",31,32,4597\r",

      "vcr ok"
   }
};

static struct irTx irTxPause[REMOTE_MODES] = {
   {
      NULL,

      "tv pause (not defined)"
   },

   {
      "sendir,1:3,77,40192,1,1,109,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",63,22,63,22,63,22,63,22,20,22,20,22,63,22,20,22,20,22,20,22,20,22,2"
      "0,22,63,22,63,22,20,22,5144\r",

      "dvd pause"
   },

   {
      "sendir,1:3,78,35919,1,1,32,31,64,31,32,31,32,62,64,62,32,31,64,62,64"
      ",31,32,62,32,4597\r",

      "vcr pause"
   }
};

static struct irTx irTxPlay[REMOTE_MODES] = {
   {
      NULL,

      "tv play (not defined)"
   },

   {
      "sendir,1:3,79,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,20,22"
      ",63,22,63,22,63,22,63,22,20,22,20,22,63,22,63,22,20,22,20,22,20,22,2"
      "0,22,63,22,63,22,20,22,1191,343,171,22,63,22,63,22,20,22,20,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,2"
      "0,22,63,22,63,22,63,22,63,22,20,22,20,22,63,22,63,22,20,22,20,22,20,"
      "22,20,22,63,22,63,22,20,22,5144\r",

      "dvd play"
   },

   {
      "sendir,1:3,80,35919,1,1,32,31,32,31,64,31,32,62,64,62,32,31,32,31,64"
      ",62,64,62,32,4597\r",

      "vcr play"
   }
};

static struct irTx irTxPower[REMOTE_MODES] = {
   {
      "sendir,1:3,81,36764,1,1,128,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,15,17,47,17,47,17,47,17,47,17,15,17,15,1"
      "7,47,17,15,17,47,17,47,17,47,17,47,17,15,17,47,17,2738\r",

      "tv power"
   },

   {
      "sendir,1:3,82,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1189,343,171,22,63,22,63,22,63,22,63,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,20,22,20,22,63,22,20,22,63,22,20,22,2"
      "0,22,20,22,63,22,63,22,63,22,63,22,20,22,63,22,63,22,63,22,20,22,20,"
      "22,20,22,20,22,63,22,20,22,5144\r",

      "dvd power"
   },

   {
      "sendir,1:3,83,35919,1,1,32,31,32,31,64,31,32,62,64,62,64,31,32,62,32"
      ",31,64,31,32,4597\r",

      "vcr power"
   }
};

static struct irTx irTxRecall[REMOTE_MODES] = {
   {
      "sendir,1:3,84,37202,1,1,131,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,47,17,47,17,15,17,47,17,47,17,15,17,15,1"
      "7,47,17,47,17,47,17,15,17,47,17,47,17,15,17,47,17,3700\r",

      "tv recall"
   },

   {
      NULL,

      "dvd recall (not defined)"
   },

   {
      NULL,

      "vcr recall (not defined)"
   }
};

static struct irTx irTxReturn[REMOTE_MODES] = {
   {
      "sendir,1:3,85,36764,1,1,128,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,47,17,15,17,47,17,47,1"
      "7,15,17,15,17,47,17,15,17,47,17,15,17,47,17,15,17,4705\r",

      "tv return"
   },

   {
      NULL,

      "dvd return (not defined)"
   },

   {
      NULL,

      "vcr return (not defined)"
   }
};

static struct irTx irTxRewind[REMOTE_MODES] = {
   {
      NULL,

      "tv rewind (not defined)"
   },

   {
      "sendir,1:3,86,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",63,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,20,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1191,343,171,22,63,22,63,22,20,22,20,22,20,22"
      ",63,22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,6"
      "3,22,63,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,20,22,63,22,20,"
      "22,20,22,63,22,63,22,20,22,5144\r",

      "dvd rewind"
   },

   {
      "sendir,1:3,87,35919,1,1,32,31,64,31,32,31,32,62,64,62,32,31,32,31,64"
      ",31,32,62,64,4597\r",

      "vcr rewind"
   }
};

static struct irTx irTxStop[REMOTE_MODES] = {
   {
      NULL,

      "tv stop (not defined)"
   },

   {
      "sendir,1:3,88,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,20,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,63,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,5144\r",

      "dvd stop"
   },

   {
      "sendir,1:3,89,35919,1,1,32,31,32,31,64,31,32,62,64,62,32,31,32,31,64"
      ",62,32,31,64,4597\r",

      "vcr stop"
   }
};

static struct irTx irTxVideo[REMOTE_MODES] = {
   {
      "sendir,1:3,90,36764,1,1,128,64,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,15,17,47,17,15,17,15,17,15,17,15,17,15,1"
      "7,47,17,15,17,47,17,15,17,15,17,15,17,15,17,47,17,2738\r",

      "tv video"
   },

   {
      "sendir,1:3,91,40192,1,1,343,171,22,63,22,63,22,20,22,20,22,20,22,63,"
      "22,20,22,63,22,20,22,20,22,63,22,63,22,63,22,20,22,63,22,20,22,63,22"
      ",20,22,20,22,63,22,63,22,20,22,20,22,63,22,20,22,63,22,63,22,20,22,2"
      "0,22,63,22,63,22,20,22,1190\r",

      "dvd video"
   },

   {
      "sendir,1:3,92,35919,1,1,32,31,64,31,32,31,32,62,64,62,32,31,32,31,32"
      ",31,32,31,32,31,64,3500\r",

      "vcr video"
   }
};

static struct irTx irTxVolumeDown[REMOTE_MODES] = {
   {
      "sendir,1:3,93,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,47,17,15,17,15,1"
      "7,47,17,15,17,15,17,15,17,15,17,47,17,15,17,47,17,3700\r",

      "volume down"
   },

   {
      "sendir,1:3,94,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,47,17,15,17,15,1"
      "7,47,17,15,17,15,17,15,17,15,17,47,17,15,17,47,17,3700\r",

      "volume down"
   },

   {
      "sendir,1:3,95,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,47,17,15,17,15,1"
      "7,47,17,15,17,15,17,15,17,15,17,47,17,15,17,47,17,3700\r",

      "volume down"
   }
};

static struct irTx irTxVolumeUp[REMOTE_MODES] = {
   {
      "sendir,1:3,96,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,47,17,15,17,47,17,3700\r",

      "volume up"
   },

   {
      "sendir,1:3,97,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,47,17,15,17,47,17,3700\r",

      "volume up"
   },

   {
      "sendir,1:3,98,37202,1,1,130,65,17,15,17,47,17,15,17,15,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,"
      "15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,17,15,17,15,17,15"
      ",17,15,17,15,17,15,17,15,17,15,17,15,17,15,17,15,17,47,17,15,17,15,1"
      "7,15,17,15,17,15,17,15,17,15,17,47,17,15,17,47,17,3700\r",

      "volume up"
   }
};

