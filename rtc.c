#include <msp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "rtc.h"
#include "driverlib.h"

float time_stamp;
uint8_t received_bytes[7], status;

/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        48000000,                                // SMCLK = 48MHz
        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

void configure_rtc(void)
{
    //set pins for i2c
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
            GPIO_PIN4 + GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);

    //configure i2c perfiral
    I2C_initMaster(EUSCI_B1_BASE, &i2cConfig);

    /* Specify slave address */
    I2C_setSlaveAddress(EUSCI_B1_BASE, rtc_address);

    /* Set Master in transmit mode */
    I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);

    /* Enable I2C Module to start operations */
    I2C_enableModule(EUSCI_B1_BASE);

    I2C_clearInterruptFlag(EUSCI_B1_BASE,
            EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_NAK_INTERRUPT);
}

uint64_t get_rtc_time(void)
{
    I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);

    while (MAP_I2C_masterIsStopSent(EUSCI_B1_BASE) == EUSCI_B_I2C_SENDING_STOP);

    //start send
    I2C_masterSendMultiByteStart(EUSCI_B1_BASE, time_address);

    //wait for send to finish
    wait_for_status(EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_MODE);

    I2C_masterReceiveStart(EUSCI_B1_BASE);

    wait_for_status(EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    received_bytes[0] = I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);

    wait_for_status(EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    received_bytes[1] = I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);

    wait_for_status(EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    received_bytes[2] = I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);

    wait_for_status(EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    received_bytes[3] = I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);
    
    wait_for_status(EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    received_bytes[4] = I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);

    wait_for_status(EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    received_bytes[5] = I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);

    wait_for_status(EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    I2C_masterReceiveMultiByteStop(EUSCI_B1_BASE);

    received_bytes[6] = I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);

    return create_time_stamp();
}

void wait_for_status(uint_fast16_t goal){
    uint_fast16_t status;

    status = I2C_getInterruptStatus(EUSCI_B1_BASE, goal);

    while (status == 0){
        status = I2C_getInterruptStatus(EUSCI_B1_BASE, goal);
    }

    I2C_clearInterruptFlag(EUSCI_B1_BASE, goal);
}

uint64_t create_time_stamp(void)
{
    enum
    {
        AM,
        PM
    } time_mode;

    uint8_t sec_byte = received_bytes[0];
    uint8_t min_byte = received_bytes[1];
    uint8_t hour_byte = received_bytes[2];
    uint8_t day_byte = received_bytes[3];
    uint8_t date_byte = received_bytes[4];
    uint8_t month_byte = received_bytes[5];
    uint8_t year_byte = received_bytes[6];

    uint8_t s1 = ((sec_byte >> 4) & 0x7);
    uint8_t s2 = sec_byte & 0xF;
    uint8_t sec = 10 * s1 + s2;

    uint8_t min = (10 * (min_byte >> 4)) + (min_byte & 0xF);

    uint8_t hours;
    if (hour_byte & BIT6 > 1)
    {
        if (hour_byte & BIT5 > 1)
        {
            time_mode = PM;
        }
        else
        {
            time_mode = AM;
        }
        hours = ((hour_byte & BIT4)>>4) * 10 + (hour_byte & 0xF);
    }
    else
    {
        uint8_t m = (BIT5 | BIT4);
        uint8_t h1 = (hour_byte & m)>>4;
        uint8_t h2 = (hour_byte & 0xF);
        hours = h1 * 10 + h2;
    }

    uint8_t d1 = (date_byte & (BIT5 | BIT4))>>4;
    uint8_t d2 = (date_byte & 0xF);
    uint8_t date = (10 * d1) + d2;

    uint8_t month = 10 * ((month_byte & BIT4)>>4) + (month_byte & 0xF);

    uint8_t year = 10 * (year_byte >> 4) + (year_byte & 0xF);

    uint64_t time = (31540000) * (uint64_t)year + (3600) * (uint64_t)hours + (60) * (uint64_t)min + (uint64_t)sec + lookup_day(date, month, year);
    return time;
}

uint64_t lookup_day(uint8_t day, uint8_t month, uint8_t year)
{
    if (month == 1)
    {
        return (uint64_t)day * 86400;
    }
    else if (month == 2)
    {
        return (uint64_t)(day + 31) * 86400;
    }
    if (year % 4 == 0)
    {
        day += 1;
    }
    else if (month == 3)
    {
        return (uint64_t)(day + 31 + 28) * 86400;
    }
    else if (month == 4)
    {
        return (uint64_t)(day + 31 + 28 + 31) * 86400;
    }
    else if (month == 5)
    {
        return (uint64_t)(day + 31 + 28 + 31 + 30) * 86400;
    }
    else if (month == 6)
    {
        return (uint64_t)(day + 31 + 28 + 31 + 30 + 31) * 86400;
    }
    else if (month == 7)
    {
        return (uint64_t)(day + 31 + 28 + 31 + 30 + 31 + 30) * 86400;
    }
    else if (month == 8)
    {
        return (uint64_t)(day + 31 + 28 + 31 + 30 + 31 + 30 + 31) * 86400;
    }
    else if (month == 9)
    {
        return (uint64_t)(day + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31) * 86400;
    }
    else if (month == 10)
    {
        return (uint64_t)(day + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30) * 86400;
    }
    else if (month == 11)
    {
        return (uint64_t)(day + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) * 86400;
    }
    else if (month == 12)
    {
        return (uint64_t)(day + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) * 86400;
    }
}
