#include "msp.h"
#include "driverlib.h"
//#include "rfid.h"
#include "gpio.h"
#include "csHFXT.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A_holdTimer();

    configHFXT();

    init_rfid();

    void rfid_set_card_read_function(card_read);

    while(1){
        //activateRec();
        printf("read: %d", read_uid_sum());
    }
}


void card_read(uint8_t uid){
    printf("card with uid %d read", uid);
}
