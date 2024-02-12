#include <msp.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define rtc_address 0x68
#define time_address 0x00

#ifndef I2C_DRIVER_FUNCTIONS_H_
#define I2C_DRIVER_FUNCTIONS_H_

#define I2Cport_EUSCI_B3 P6
#define SCLPIN_EUSCI_B3 BIT7
#define SDAPIN_EUSCI_B3 BIT6

void configureRTC(void);

void I2C_Configure_EUSCI_B3(uint8_t Device_Address, uint16_t Baud_Count);

void I2C_write_Data(unsigned char write_data);

void I2C_receive_Data(unsigned char *data);

void I2C_wait_for_device_ready(unsigned char control_byte);

void I2C_acknowledge(void);

uint8_t I2C_Start_Write_EUSCI_B3(void);

uint8_t I2C_Start_Read_EUSCI_B3(void);

uint8_t I2C_Write_Byte_EUSCI_B3(uint8_t data);

uint8_t I2C_Read_Byte_EUSCI_B3(void);

uint8_t I2C_send_NACK_Bit(void);

uint8_t I2C_send_Stop_Bit(void);

#endif /* I2C_DRIVER_FUNCTIONS_H_ */
