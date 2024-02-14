#include <msp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "rtc.h"

float time_stamp;
uint8_t received_bytes[7], status;

void configureRTC(void)
{
    I2Cport_EUSCI_B0->SEL0 |= (SCLPIN_EUSCI_B0 | SDAPIN_EUSCI_B0);
    I2Cport_EUSCI_B0->SEL1 &= ~(SCLPIN_EUSCI_B0 | SDAPIN_EUSCI_B0);

    EUSCI_B0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
    EUSCI_B0->CTLW0 = EUSCI_A_CTLW0_SWRST |
                      EUSCI_B_CTLW0_MODE_3 |
                      EUSCI_B_CTLW0_MST |
                      EUSCI_B_CTLW0_SYNC |
                      EUSCI_B_CTLW0_TR |
                      EUSCI_B_CTLW0_SSEL__SMCLK;

    EUSCI_B0->BRW = 480;
    EUSCI_B0->I2CSA = rtc_address;
    EUSCI_B0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
}

void set_rtc_read_address(void)
{
    status = I2C_Start_Write_EUSCI_B0();
    if (status == 1)
        printf("\r\nStart write Error status=%d", status);
    status = I2C_Write_Byte_EUSCI_B0(time_address);
    if (status == 1)
        printf("\r\nWrite register address Error status=%d", status);
    status = I2C_send_NACK_Bit();
    if (status == 1)
        printf("\r\nSending NACK bit Error status=%d", status);
    status = I2C_send_Stop_Bit();
    if (status == 1)
        printf("\r\nSending STOP bit Error status=%d", status);
}

void get_rtc(void)
{

    configureRTC();
    set_rtc_read_address();
    status = I2C_Start_Read_EUSCI_B0();

    if (status == 1)
        printf("\r\nStart Reading Error status=%d", status);
    received_bytes[0] = I2C_Read_Byte_EUSCI_B0();

    received_bytes[1] = I2C_Read_Byte_EUSCI_B0();

    received_bytes[2] = I2C_Read_Byte_EUSCI_B0();

    received_bytes[3] = I2C_Read_Byte_EUSCI_B0();

    received_bytes[4] = I2C_Read_Byte_EUSCI_B0();

    received_bytes[5] = I2C_Read_Byte_EUSCI_B0();

    received_bytes[6] = I2C_Read_Byte_EUSCI_B0();

    status = I2C_send_NACK_Bit();
    if (status == 1)
        printf("\r\nSending NACK bit Error status=%d", status);
    status = I2C_send_Stop_Bit();
    if (status == 1)
        printf("\r\nSending STOP bit Error status=%d", status);

    create_time_stamp();
}

uint64_t create_time_stamp(void)
{
    enum
    {
        AM,
        PM
    } time_mode;
    uint8_t sec = 10 * ((received_bytes[0] >> 4) & 0x7) + received_bytes[0] & 0xF;
    uint8_t min = 10 * (received_bytes[1] >> 4) + received_bytes[1] & 0xF;
    uint8_t hours;
    if (received_bytes[2] & BIT6 == 1)
    {
        if (received_bytes[2] & BIT5 == 1)
        {
            time_mode = PM;
        }
        else
        {
            time_mode = AM;
        }
        hours = (received_bytes[2] & BIT4) * 10 + received_bytes[2] & 0xF;
    }
    else
    {
        hours = (received_bytes[2] & (BIT5 | BIT4)) * 10 + received_bytes[2] & 0xF;
    }
    uint8_t date = 10 * (received_bytes[4] & (BIT5 | BIT4)) + received_bytes[4] & 0xF;
    uint8_t month = 10 * (received_bytes[5] & BIT4) + received_bytes[5] & 0xF;
    uint8_t year = 10 * (received_bytes[6] >> 4) + received_bytes[6] & 0xF;

    //    printf(" hours: %d", hours);
    //    printf(received_bytes[0]);
    //    printf(" min: %d", min);
    printf(" sec: %d\n", sec);
    //    printf(" date: %d", date);
    //    printf(" month: %d", month);
    //    printf(" year: %d\n", year);

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

void I2C_Configure_EUSCI_B0(uint8_t Device_Address, uint16_t Baud_Count)
{

    /* Configure IC2 pins */
    I2Cport_EUSCI_B0->SEL0 |= SCLPIN_EUSCI_B0 | SDAPIN_EUSCI_B0; // set I2C pins as secondary function
    I2Cport_EUSCI_B0->SEL1 &= ~(SCLPIN_EUSCI_B0 | SDAPIN_EUSCI_B0);

    /* Configure eUSCI_B0 for I2C mode
     *  I2C master mode, synchronous, 7-bit address, SMCLK clock source,
     *  transmit mode, with automatic STOP condition generation
     */
    EUSCI_B0->CTLW0 |= EUSCI_A_CTLW0_SWRST;      // Software reset enabled
    EUSCI_B0->CTLW0 = EUSCI_A_CTLW0_SWRST |      // Remain eUSCI in reset mode
                      EUSCI_B_CTLW0_MODE_3 |     // I2C mode
                      EUSCI_B_CTLW0_MST |        // Master mode
                      EUSCI_B_CTLW0_SYNC |       // Sync mode
                      EUSCI_B_CTLW0_TR |         // Transmitter mode
                      EUSCI_B_CTLW0_SSEL__SMCLK; // SMCLK at 3MHz default clock

    EUSCI_B0->BRW = Baud_Count; // UCBRx Baud rate value

    /* Configure I2C to communicate with GY-521 */
    EUSCI_B0->I2CSA = Device_Address;        // I2C peripheral address
    EUSCI_B0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Release eUSCI from reset

    // interrupts on transmit, receive and acknowledgment
    //    EUSCI_B0->IE |= EUSCI_A_IE_RXIE |       // Enable receive interrupt
    //                EUSCI_A_IE_TXIE |               // Enable transmit interrupt
    //                EUSCI_B_IE_NACKIE;              // Enable NACK interrupt

} // end I2C_Configure_EUSCI_B0()

//=================================================
// Send first byte with address and write command
// Return acknowledgment flag NACKIFG=0 if acknowledgment is receive
//=================================================

uint8_t I2C_Start_Write_EUSCI_B0(void)
{
    /*  format for Read operation
     *  ______________________________
     *  |       |          |         |
     *  | Start |  Addr  W |  | Ack  |
     *  |_______|__________|_________|
     */
    // Wait for STOP condition to be complete
    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP)
        ;
    // switch to receive mode to set bit 0 to read R/W=1
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR; // set transmit mode
    // #define EUSCI_B_CTLW0_TXSTT  ((uint16_t)0x0002)  /*!< Transmit START condition in master mode */
    //  Start sending I2C start condition and address frame with receive (or read)
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
    // wait for address frame is transmitted completely
    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT)
        ;
    // wait for ACK from receiver
    while (EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG)
        ;
    // Return ACknowledgment flag from receiver
    return (EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG);
}

//=================================================
// Send 8-it data
// Return acknowledgment flag NACKIFG=1 if acknowledgment is receive
//=================================================

uint8_t I2C_Write_Byte_EUSCI_B0(uint8_t data)
{
    /*  format for Write operation
     *  ____________________
     *  |       |          |
     *  | data  |  | Ack   |
     *  |_______|__________|
     */
    // Wait if bus is busy
    //    while (EUSCI_B0->STATW & EUSCI_B_STATW_BBUSY);
    // wait for TX buffer to be ready
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0))
        ;
    EUSCI_B0->TXBUF = data;
    // wait for data frame is transmitted completely
    // IFG0 is set when TXBUF is empty
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0))
        ;
    // Return ACknowledgment flag from receiver
    //     NACKIFG=0 if acknowledgment is received and NACKIFG=1 for no ACK
    // wait for ACK from receiver
    while (EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG)
        ;
    return (EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG);
}

//=================================================
// Send first byte with address and read command
// Return acknowledgment flag NACKIFG=1 if acknowledgment is receive
//=================================================

uint8_t I2C_Start_Read_EUSCI_B0(void)
{
    /*  format for Read operation
     *  ______________________________
     *  |       |          |         |
     *  | Start |  Addr  R |  | Ack  |
     *  |_______|__________|_________|
     */
    // Wait for STOP condition to be complete
    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP)
        ;
    // switch to receive mode to set bit 0 to read R/W=1
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_TR; // set receiver mode
    // #define EUSCI_B_CTLW0_TXSTT  ((uint16_t)0x0002)  /*!< Transmit START condition in master mode */
    //  Start sending I2C start condition and address frame with receive (or read)
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
    // wait for address frame is transmitted completely
    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT)
        ;

    // Return ACknowledgment flag from receiver
    return (EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG);
}

//=================================================
// read a byte from the slave
// Acknowledgment bit is sent to the slave automatically
//=================================================

uint8_t I2C_Read_Byte_EUSCI_B0(void)
{
    /*  format for read operation. ACK is sent automatically
     *  _____________________________
     *  |                |          |
     *  | Receiver data  |  | Ack   |
     *  |________________|__________|
     */
    uint8_t data;
    // Wait if bus is busy
    //      while (EUSCI_B0->STATW & EUSCI_B_STATW_BBUSY);
    // wait for receiver buffer to be full
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0))
        ;
    // reading the buffer clears the flag EUSCI_B_IFG_RXIFG0
    data = EUSCI_B0->RXBUF;
    return data;
}

// Send I2C NACK condition to tell slave not to expect acknowledgment
/*!< Transmit NACK condition in master mode */
// In master receiver mode, send NACK before sending STOP condition.
// TXNACK is automatically cleared after STOP is generated
uint8_t I2C_send_NACK_Bit(void)
{
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXNACK;
    // Ensure NACK condition got sent. this will block this function
    //    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXNACK);
    return EUSCI_B_CTLW0_TXNACK; // cleared after NACK is generated
}

// Send I2C stop condition
/*!< Transmit STOP condition in master mode */
// In master receiver mode, the STOP condition is preceded by a NACK.
// UCTXSTP is automatically cleared after STOP is generated.
uint8_t I2C_send_Stop_Bit(void)
{
    // Send I2C stop condition
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
    // Ensure stop condition got sent
    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP)
        ;
    return EUSCI_B_CTLW0_TXSTP; // cleared after STOP is generated
}
