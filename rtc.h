#include <msp.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define rtc_address 0x68
#define time_address 0x00

#ifndef I2C_DRIVER_FUNCTIONS_H_
#define I2C_DRIVER_FUNCTIONS_H_

#define I2Cport_EUSCI_B1 P6
#define SCLPIN_EUSCI_B1 BIT5
#define SDAPIN_EUSCI_B1 BIT4

void configure_rtc(void);

uint64_t get_rtc_time(void);

#endif /* I2C_DRIVER_FUNCTIONS_H_ */
