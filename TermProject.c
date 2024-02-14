#include "msp.h"
#include "driverlib.h"
//#include "rfid.h"
#include "gpio.h"
#include "csHFXT.h"
#include "csLFXT.h"
#include <stdio.h>
#include "lcd8bits.h"
#include "flash.h"
#include "rtc.h"


uint64_t curTime = 0;

void card_read(uint32_t uid){
    lcd_clear();

    char DataBuffer[16];

    sprintf(DataBuffer, "%d t:%d", uid, (int)curTime);

    lcd_puts(DataBuffer);

    lcd_SetLineNumber(SecondLine);

    //gets a blank card if it does not exist
    struct CardData card;

    card = get_card_from_mem(uid);

    //init if card is blank
    if(card.uid == 0){
        card.uid = uid;
        card.clock_in_time = 0;
        card.clocked_in = 0;
        card.accumulated_time = 0;
    }

    if(card.clocked_in == 1){
        card.clocked_in = 0;
        uint64_t time = curTime - card.clock_in_time;
        uint64_t acum_time = card.accumulated_time;
        acum_time += time;
        sprintf(DataBuffer, "ci:%d, t:%d", time, acum_time);
        card.accumulated_time = acum_time;
        lcd_puts(DataBuffer);

        update_card_in_memmory(card);
    }
    else{
        card.clocked_in = 1;

        card.clock_in_time = curTime;

        sprintf(DataBuffer, "clocked in");
        lcd_puts(DataBuffer);

        update_card_in_memmory(card);
        
    }


//    lcd_off();



}

/**
 * main.c
 */
void main(void)
{
	WDT_A_holdTimer();

	printf("start");

    configLFXT();
    
    configHFXT();
    while(1){
        get_rtc();
    }

//    init_flash_memory();
//
//    char DataBuffer[16];
//
//    sprintf(DataBuffer, "on");
//
//    lcd8bits_init();
//
//    lcd_SetLineNumber(FirstLine);
//
//    //lcd_putch('r');
//
//    lcd_puts(DataBuffer);
//
//    printf("starting spin");
//
//    init_rfid();
//
//    rfid_set_card_read_function(card_read);
//
//    curTime = 0;
//
//    while(1){
//        //activateRec();
//        //printf("read: %d", read_uid_sum());
//        /* Sleeping when not in use */
//
//        // PCM_gotoLPM0();
//
//        curTime++;
//        DelayMs(1000);
////
////        lcd_clear();
////
////        sprintf(DataBuffer, "%d", curTime);
////        lcd_puts(DataBuffer);

//    }
}


