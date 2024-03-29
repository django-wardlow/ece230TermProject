/*
* 
* Author: Justin Zimmerman, Django Wardlow
*
*Our project is a device that tracks time participating in an activity, similar to a timeclock, 
*using RFID recognition. Users will be able to swipe their card to see the duration they were clocked 
*in for and the total time accumulated using a built-in LCD screen. Cards are detected by querying the 
*RFID module every 100ms using an interrupt from timer A0. Users will be notified of a successful scan 
*with a confirmation beep, generated by the CCR0 module of timer A2, and a green LED. 
*
*/

#include <rfid_wrapper.h>
#include "msp.h"
#include "driverlib.h"
// #include "rfid.h"
#include "gpio.h"
#include "csHFXT.h"
#include "csLFXT.h"
#include <stdio.h>
#include "lcd8bits.h"
#include "flash.h"
#include "rtc.h"
#include "TermProject.h"
#include "feedback.h"

int display_card = 0;
uint64_t current_time = 0;
uint32_t card_uid = 0;

// function called when card is read by RFID
void card_read(uint32_t uid)
{
    positive_feedback();

    card_uid = uid;
    current_time = get_rtc_time();

    display_card = 1;

}

/// @brief displays the data associated with a card (uid, time) on the lcd
void display_card_data(){
    lcd_clear();

    // buffer for prints
    char DataBuffer[16];

    // print uuid onto display
    sprintf(DataBuffer, "uid: %d", card_uid);

    lcd_puts(DataBuffer);

    lcd_SetLineNumber(SecondLine);

    struct CardData card;

    // gets a blank card if it does not exist
    card = get_card_from_mem(card_uid);

    // init if card is blank
    if (card.uid == 0)
    {
        card.uid = card_uid;
        card.clock_in_time = 0;
        card.clocked_in = 0;
        card.accumulated_time = 0;
    }

    // if clocked in
    if (card.clocked_in == 1)
    {
        sprintf(DataBuffer, "clocked out");
        lcd_puts(DataBuffer);

        // clock card out
        card.clocked_in = 0;

        // calculate time clocked out and update accumulated time
        uint64_t time = current_time - card.clock_in_time;
        uint64_t accumulated_time = card.accumulated_time;
        accumulated_time += time;
        card.accumulated_time = accumulated_time;

        // wright new card data to mem
        update_card_in_memory(card);

        // diaply clock out info on display

        delay(DISP_HOLD / 2);

        // display time clocked in
        lcd_clear();

        sprintf(DataBuffer, "clocked in for:");
        lcd_puts(DataBuffer);

        convert_to_readable(time);

        lcd_SetLineNumber(SecondLine);

        sprintf(DataBuffer, "%d:%d:%d", second_convert_data[2], second_convert_data[1], second_convert_data[0]);
        lcd_puts(DataBuffer);

        delay(DISP_HOLD);

        // display accumulated time

        lcd_clear();

        sprintf(DataBuffer, "accumulated:");
        lcd_puts(DataBuffer);

        convert_to_readable(accumulated_time);

        lcd_SetLineNumber(SecondLine);

        sprintf(DataBuffer, "%d:%d:%d", second_convert_data[2], second_convert_data[1], second_convert_data[0]);
        lcd_puts(DataBuffer);
    }
    else
    {
        card.clocked_in = 1;

        card.clock_in_time = current_time;

        sprintf(DataBuffer, "clocked in");
        lcd_puts(DataBuffer);

        update_card_in_memory(card);
    }

    delay(DISP_HOLD);

    // return ready message
    lcd_clear();
    lcd_puts(ReadyMessage);
}


void main(void)
{
    WDT_A_holdTimer();

    // configure clocks
    configLFXT();

    configHFXT();

    configure_buzzer();

    init_feedback();

    configure_rtc();

    init_flash_memory();

    lcd8bits_init();

    // display ready message
    lcd_SetLineNumber(FirstLine);

    lcd_puts(ReadyMessage);

    rfid_init();

    // make card scan call card_read()
    rfid_set_card_read_function(card_read);

    // configure clear flash button pins

    GPIO_setAsInputPinWithPullUpResistor(BUTTON_PORT, BUTTON_PIN);

    // spin
    while (1)
    {

        // PCM_gotoLPM0();

        //if a card was read, display the info
        if(display_card){
            //prevent cards from being read while info is displayed
            disable_reading();

            display_card_data();

            enable_reading();

            display_card = 0;
        }

        // clears the flash memmory when S2 is pressed, used for debugging
        if (GPIO_getInputPinValue(BUTTON_PORT, BUTTON_PIN) == GPIO_INPUT_PIN_LOW)
        {
            flash_reset();
        }
    }
}
