/*
* Reads the time from an RTC module over i2c
* Author: Justin Zimmerman, Django Wardlow
*/

#include <msp.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "driverlib.h"

#define rtc_address 0x68
#define time_address 0x00

#ifndef RTC_H_
#define RTC_H_

#define EUSCI_PERIPHERAL EUSCI_B1_BASE
#define I2Cport_EUSCI_B1 GPIO_PORT_P6
#define SCLPIN_EUSCI_B1 BIT5
#define SDAPIN_EUSCI_B1 BIT4

#define SEC_PER_MIN 60
#define SEC_PER_HOUR 3600

int second_convert_data[3];


void configure_rtc(void);

uint64_t get_rtc_time(void);

void convert_to_readable(uint64_t seconds);

#endif /* RTC_H_ */
