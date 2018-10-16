//
//   zrc.cpp (zigbee remote controller)
//
//   Functions in this module support a controller paired to a 
//   Comcast RF4CE set top box:
//
//   Windows PC CC2531 USB Dongle <-> Comcast RF4CE Set Top Box
//
//   2018 Jun 12 jhm original creation 


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "extern.h"
#include "zrc.h"

// direct zrcc.h to compile this module as a controller
#define HOST_CTL TRUE
#include "zrcc.h"


// validation process makes a temporary pairing permanent
static int validationState, validating;
static struct zrcMsg zrcMsg; 

// RTI_InitReq callback
// After a successful initialization, find the current pairing or 
// if unpaired, ready the validation state machine for the pairing
// process
void RTI_InitCnf(rStatus_t status) {

   // insure startup flag is set to restore rather than clear
   uint8 startupFlg = NIB_RESTORE_STATE;
   RTI_WriteItemEx(RTI_PROFILE_RTI, RTI_CP_ITEM_STARTUP_CTRL, 1, &startupFlg );
  
   if ( status == RTI_SUCCESS ) {
   
      printf("done\n");
    
      validationState = findPairingEntry() == RTI_INVALID_PAIRING_REF ?
         VALIDATION_SEQUENCE_BEGIN : 
         VALIDATION_SEQUENCE_PIN_COMPLETE + VALIDATION_SEQUENCE_COMMIT_COMPLETE;

      zrcInitialized = TRUE;
   }
   else {
      char szErrMsg[256];
      printf("failed\n");
      snprintf(szErrMsg, sizeof(szErrMsg),
               "RTI_InitCnf status 0x%02x: %s", status, statusToMsg(status));
      rfClose();
      errorExit(szErrMsg);
   }
}


// RTI_PairReq callback
// set the validation sequence state machine into validation mode 
void RTI_PairCnf( rStatus_t status, uint8 dstIndex, uint8 devType ) {

   printf("RTI_PairCnf status 0x%02x: %s\n", status, statusToMsg(status));
   
   if (status == RTI_SUCCESS) {
      pairingIndex = dstIndex;   
      validating=TRUE;   
   } 
}


// RTI_SendDataReq confirmation callback
// 
// Notes:
//    This callback may occur before the call to RTI_SendDataReq has 
//    returned
// 
//    The validation process after pairing comes was derived from
//    a bit of reverse engineering.  After pairing an XR11 comcast remote 
//    with a PC target, the XR11 consistently sends the following data:
//    0x24 0xdc 0x00 0x04
//    0x20 0x00 N times 
//    The XR11's N value is 15 but sending that message twice to the
//    comcast set top box seems to do the commit trick.  N is defined as:
//
//    VALIDATION_SEQUENCE_COMMIT_COMPLETE - 1
//
//    where VALIDATION_SEQUENCE_COMMIT_COMPLETE is the total number of 
//    messages in the commit sequence
//
//    After a successful pairing, the STB will display a request on the tv 
//    screen for a 3 digit pin code.  After entering the third digit, the 
//    validation state machine automatically sends the XR11's "magic bytes" 
//    as three separate messages:
//
//    0x24 0xdc 0x00 0x04
//    0x20 0x00  
//    0x20 0x00  
//
//    Comcast XG2 STB manufactured by Samsung test
//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    Ran this utility and pressed p to initiate pairing.  The STB prompted 
//    for a three digit pin on the TV screen.  After keying in the third 
//    digit, the three "commit request" messages were sent.  About twenty 
//    seconds later the STB asked to enable tv or audio player remote 
//    control.  Selecting "Not Now" or "Skip" completed the validation 
//    process.  A validated pairing sequence (pin # will vary) should look 
//    like:
//
//    Calling RTI_PairReq
//    RTI_PairCnf status 0x00: RTI_SUCCESS
//    5 -> CERC 0x25
//    5 -> CERC 0x25
//    7 -> CERC 0x27
//    Sending commit request: 0x24 0xdc 0x00 0x04
//    Sending commit request: 0x20 0x00
//    Sending commit request: 0x20 0x00
//    STB pairing committed   
//
//    The RTI library is not production quality.  For example, testing
//    an unpair/pair sequence required killing this running utility
//    and unplugging/plugging the USB dongle.  A successful pairing may 
//    take a couple tries.   
//
void RTI_SendDataCnf( rStatus_t status ) {

   static uint8 commitSequence1[] = {0x24,0xdc,0x00,0x04};
   static uint8 commitSequence2[] = {0x20,0x00};
   
   if (status == RTI_SUCCESS) {
      
      if (validating) { 
      
         validationState++;
            
         if (validationState == VALIDATION_SEQUENCE_PIN_COMPLETE) {
            int i;
            // set top box pin # keyed in so queue up the commit sequence
            zrcMsg.data = commitSequence1; 
            zrcMsg.len = sizeof(commitSequence1);
            zrcMsg.sz = "pairing validation phase 1";
            zrcCmdQueueAdd(&zrcMsg);
            
            zrcMsg.data = commitSequence2;
            zrcMsg.len = sizeof(commitSequence2);
            zrcMsg.sz = "pairing validation phase 2";
            for (i=0; i<VALIDATION_SEQUENCE_COMMIT_COMPLETE-1; i++)
               zrcCmdQueueAdd(&zrcMsg);
         }
         else if (validationState > VALIDATION_SEQUENCE_PIN_COMPLETE &&
                  isZrcCmdQueueEmpty()) {
            // set top box pin # keyed in AND commit sequence sent 
            validating = FALSE;
            printf("STB pairing committed\n");
         }   
      } 
         
      // either sleep the RNP UART interface or continue emptying the queue
      isZrcCmdQueueEmpty() ? RTI_EnableSleepReq() : wakeupRNP();
      
   } else {
      printf("RTI_SendDataCnf: status 0x%02x: %s\n", status, statusToMsg(status));
   } 
}


// RTI_DisableSleepReq callback to initiate pairings, unpair and send data
void RTI_DisableSleepCnf( rStatus_t status ) {

   isSleeping = FALSE;
   
   if (zrcExiting)
      return;
   
   if (status == RTI_SUCCESS && !isZrcCmdQueueEmpty()) {
   
      zrcCmdQueueRemove();
      
      if (!strcmp(zrcMsg.sz, PAIR)) {
         printf("Calling RTI_PairReq\n");
         RTI_PairReq();
      } else if (pairingIndex != RTI_INVALID_PAIRING_REF) {
         if (!strcmp(zrcMsg.sz, UNPAIR)) {
            printf("Calling RTI_UnpairReq\n");
            RTI_UnpairReq(pairingIndex);
         } else {
         
            uint8 * zrcMsgData;
            uint8 cercMsgData[2];
            
            if (validationState >= VALIDATION_SEQUENCE_PIN_COMPLETE && validating) {
               char szData[64];
               zrcMsgData = zrcMsg.data;
               printf( "Sending commit request: %s\n", 
                  uint8ArrayToSz(zrcMsg.data, zrcMsg.len, szData, sizeof(szData)) );
               Sleep(500);   
            } else {
               zrcMsgData = cercMsgData;
               zrcMsgData[0] = RTI_CERC_USER_CONTROL_PRESSED;
               zrcMsgData[1] = zrcMsg.data[0];
               printf("%s -> CERC 0x%02x\n", zrcMsg.sz, zrcMsgData[1]);
            }
                  
            RTI_SendDataReq(
               pairingIndex,
               RTI_PROFILE_RF4CE_MSO,
               RTI_VENDOR_COMCAST,
               RTI_TX_OPTIONS,
               zrcMsg.len,
               zrcMsgData);
         }
      } else {
         printf("RTI_DisableSleepCnf: controller not paired\n");
      }   
      
   } else {
      printf("\nRTI_DisableSleepCnf status 0x%02x: %s\n", status, statusToMsg(status));
   } 
}  

// RTI_UnpairReq() callback
// invalidate pairing information and ready the validation state machine for
// the pairing process
void RTI_UnpairCnf( rStatus_t status, uint8 dstIndex ) {
   printf("RTI_UnpairCnf status 0x%02x: %s\n", status, statusToMsg(status));
   if (status == RTI_SUCCESS && findPairingEntry() == RTI_INVALID_PAIRING_REF)
      validationState = VALIDATION_SEQUENCE_BEGIN;
}


// use a queue to serialize zigbee request and confirmation sequencing
static LinkedList * zrcCmdQueue;      
static volatile LONG zrcCmdQueueLock;   // insure exclusive access
BOOL isZrcCmdQueueEmpty() { 
   while (isLocked(&zrcCmdQueueLock))   
      Sleep(10);
   BOOL isEmpty = isQueueEmpty(zrcCmdQueue); 
   unlock(&zrcCmdQueueLock);
   return isEmpty;
}

// remove a zigbee remote control command from the queue
void zrcCmdQueueRemove() { 
   struct zrcMsg * qEntry;
   
   while (isLocked(&zrcCmdQueueLock))   
      Sleep(10);
      
   if (qEntry=(struct zrcMsg *) queueRemove(&zrcCmdQueue)) {
      zrcMsg.data = qEntry->data;
      zrcMsg.len = qEntry->len;
      zrcMsg.sz = qEntry->sz;
      free(qEntry);
   }   
   
   unlock(&zrcCmdQueueLock);
}

// add a zigbee remote control CERC command to the queue
void zrcCmdQueueAdd(struct cercCmd * cercCmd) {
   if (cercCmd) {
      struct zrcMsg zrcMsg;
      zrcMsg.data = &cercCmd->cerc;
      zrcMsg.len = 2;
      zrcMsg.sz = cercCmd->sz;
      zrcCmdQueueAdd(&zrcMsg);
   }
}      

// add a zigbee remote control message to the queue
void zrcCmdQueueAdd(struct zrcMsg * z) { 
   struct zrcMsg * zrcMsg = (struct zrcMsg *) malloc(sizeof(struct zrcMsg));
   
   zrcMsg->data = z->data;
   zrcMsg->len = z->len;
   zrcMsg->sz = z->sz;   
      
   while (isLocked(&zrcCmdQueueLock))   
      Sleep(10);
   queueAdd(&zrcCmdQueue, (void *) zrcMsg); 
   unlock(&zrcCmdQueueLock);
}







