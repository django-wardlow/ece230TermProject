#include "msp.h"
#include "driverlib.h"
//#include "rfid.h"
#include "gpio.h"
#include "csHFXT.h"
#include "csLFXT.h"



void card_read(uint8_t uid){
    printf("card with uid %d read", uid);
}

/**
 * main.c
 */
void main(void)
{
	WDT_A_holdTimer();

    configLFXT();
    
    configHFXT();

    init_rfid();

    rfid_set_card_read_function(card_read);

    printf("starting spin");

    while(1){
        //activateRec();
        //printf("read: %d", read_uid_sum());
        /* Sleeping when not in use */

        //PCM_gotoLPM0();

    }
}


