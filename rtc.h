#define I2cPort P6
#define SCLPIN BIT7
#define SDAPIN BIT6

#define rtc_address 0x68
uint8_t RXDataPointer, TXDataPointer;

void configureRTC(void);