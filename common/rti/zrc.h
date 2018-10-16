#define RTI_TX_OPTIONS RTI_TX_OPTION_ACKNOWLEDGED | \
                       RTI_TX_OPTION_VENDOR_SPECIFIC | \
                       RTI_TX_OPTION_SECURITY 

#define VALIDATION_SEQUENCE_BEGIN 0
#define VALIDATION_SEQUENCE_PIN_COMPLETE 3     // 3 digit validation pin
#define VALIDATION_SEQUENCE_COMMIT_COMPLETE 3  // 3 pairing commit request msgs 
#define PAIR "pair"
#define UNPAIR "unpair"

BOOL isZrcCmdQueueEmpty(void);
void zrcCmdQueueAdd(struct cercCmd *);
void zrcCmdQueueAdd(struct zrcMsg *);
void zrcCmdQueueRemove(void);


