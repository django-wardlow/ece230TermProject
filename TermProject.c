#include "msp.h"
#include "driverlib.h"
//#include "rfid.h"
#include "gpio.h"
#include "csHFXT.h"
#include "csLFXT.h"
#include <stdio.h>
#include "lcd8bits.h"



void card_read(uint32_t uid){
    printf("card with uid %d read", uid);
    lcd_off();
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

    init_rfid();

    rfid_set_card_read_function(card_read);

    char DataBuffer[16];

    sprintf(DataBuffer, "test");

//    LCD_Initializtion();    //this can be either 4-bit or 8-bit configuration
    lcd8bits_init();

    lcd_SetLineNumber(FirstLine);

    lcd_puts(DataBuffer);

    printf("starting spin");

    while(1){
        //activateRec();
        //printf("read: %d", read_uid_sum());
        /* Sleeping when not in use */

        PCM_gotoLPM0();

    }
}


