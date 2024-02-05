#include <msp.h>
#include <stdint.h>

#include "rtc.h"

void configureRTC(void)
{
    I2cPort->SEL0 |= (SCLPIN | SDAPIN);
    I2cPort->SEL1 &= ~(SCLPIN | SDAPIN);

    RXDataPointer = 0;
    TXDataPointer = 0;

    EUSCI_B3->CTLW0 |= EUSCI_A_CTLW0_SWRST;
    EUSCI_B3->CTLW0 = EUSCI_A_CTLW0_SWRST |
                      EUSCI_B_CTLW0_MODE_3 |
                      EUSCI_B_CTLW0_MST |
                      EUSCI_B_CTLW0_SYNC |
                      EUSCI_B_CTLW0_TR |
                      EUSCI_B_CTLW0_SSEL__SMCLK;

    EUSCI_B3->BRW = 480;
    EUSCI_B3->I2CSA = rtc_address;
    EUSCI_B3->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
}
