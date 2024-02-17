/*
 * rfid_wrapper.c
 *
 *  Created on: Feb 16, 2024
 *      Author: user1
 */

#include <rfid_driver.h>
#include <rfid_wrapper.h>
#include "driverlib.h"

//timer a config
const Timer_A_UpModeConfig upModeConfig =
    {
        TIMER_A_CLOCKSOURCE_ACLK,           // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,      // ACLK/1 = 32.768khz
        TIMER_A_PERIOD,                     //set overflow period
        TIMER_A_TAIE_INTERRUPT_ENABLE,      // Enable Overflow ISR
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE, //disable ccr intrupt
        TIMER_A_DO_CLEAR                    // Clear Counter
    };

//pointer to the function to call when an rfid tag is read
void (*read_fn)(uint32_t);

///initialises the rfid module and starts timer A0 in order to 
///check for a new card every 100 ms
void rfid_init(void)
{
    //initialise rfid
    PCD_Init();

    // configure timer A0 for a periodic interrupt

    /* Configuring Continuous Mode */
    Timer_A_configureUpMode(TIMER_A0_BASE, &upModeConfig);

    /* Enabling interrupts and going to sleep */
    // Interrupt_enableSleepOnIsrExit();
    Interrupt_enableInterrupt(INT_TA0_N);

    /* Enabling MASTER interrupts */
    Interrupt_enableMaster();

    /* Starting the Timer_A0 in continuous mode */
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}


/* Timer_A0 ISR */
void TA0_N_IRQHandler(void)
{
    Timer_A_stopTimer(TIMER_A0_BASE);

    // disable intrupt
    TIMER_A0->CTL &= ~TIMER_A_TAIE_INTERRUPT_ENABLE;

    Timer_A_clearInterruptFlag(TIMER_A0_BASE);

    uint32_t uid = try_read_uid_sum();

    if (uid != 0)
    {
        read_fn(uid);
    }

    Timer_A_clearInterruptFlag(TIMER_A0_BASE);

    // enable intrupt
    TIMER_A0->CTL |= TIMER_A_TAIE_INTERRUPT_ENABLE;

    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

/// @brief sets the function that is called when a card is detected
/// @param read the function to call, should accept a uint32_t for the uid of the card
void rfid_set_card_read_function(void (*read)(uint32_t))
{
    read_fn = read;
}

/// @brief if a new card is present, read the uid and return its sum. else return 0
/// @return uid sum of new card or 0 if no new card is present
uint32_t try_read_uid_sum()
{

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle. And if present, select one.
    if (!PICC_IsNewCardPresent() || !PICC_ReadCardSerial())
    {
        return 0;
    }

    // Now a card is selected. The UID and SAK is in mfrc522.uid.

    uint32_t s = get_uid_sum(&uid);

    // printf("uid sum = %d \n", s);

    PICC_HaltA();

    return s;
}

///turn the uid from the last read card into a
///single number that can be easily understood by
///summing the bytes
uint32_t get_uid_sum(Uid *uid)
{
    uint32_t sum = 0;
    uint8_t i;
    for (i = 0; i < uid->size; i++)
    {
        // print(uid.uidByte[i] < 0x10 ? " 0" : " ");
        // print(uid.uidByte[i]);

        sum += uid->uiduint8_t[i];
    }

    return sum;
}

