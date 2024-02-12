/*
 * flash.c
 *
 *  Created on: Feb 8, 2024
 *      Author: Django Wardlow
 * referanced msp driverlib flash_program_memmory.c
 */

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdio.h>

#include "flash.h"

struct CardData cards[CARD_COUNT];

//clear flash back to 0
// int main(){
//
//     init_flash_memory();
//
//     memset(cards, 0, CARD_BYTES);
//
//     wright_cards_to_mem();
//
//
//     while(1){
//
//     }
//
//
// }

void init_flash_memory(void){

    //enable wrighting when clock speed is high
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    //unprotect memmory sector
    MAP_FlashCtl_unprotectSector(FLASH_INFO_MEMORY_SPACE_BANK1,FLCTL_A_BANK1_INFO_WEPROT_PROT0);

    /* Initializing known cards to 0 */
    memset(cards, 0, CARD_BYTES);

    //read cards from memmory into ram
    read_cards_from_mem();

}

void wright_cards_to_mem(void){
    /* Trying to erase the sector. Within this function, the API will
    automatically try to erase the maximum number of tries. If it fails,
    trap in an infinite loop */
    if(!MAP_FlashCtl_eraseSector(DATA_START))
        while(1);


    /* Trying to program the memory. Within this function, the API will 
    automatically try to program the maximum number of tries. If it fails,
    trap inside an infinite loop */
    if(!MAP_FlashCtl_programMemory(cards,
            (void*) DATA_START, CARD_BYTES))
                while(1);
    
}

void read_cards_from_mem(){
    //make c array pointing into flash
    struct CardData *mem_cards = (struct CardData*) DATA_START;

    //copy data out of flash to ram array
    int i = 0;
    for(i = 0; i<CARD_COUNT; i++){
        cards[i] = mem_cards[i];
    }

}

//gets a card with a particular uid from memmory
struct CardData get_card_from_mem(int uid){
    struct CardData out;
    out.uid = 0;

    int i = 0;
    while(1){
        struct CardData c = cards[i];

        if(c.uid == 0){
            break;
        }
        else if(c.uid == uid){
            out = c;
            break;
        }
        else{
            i++;
        }
    }

    return out;
}

//updates a cards data in memmory
//if the card does not exist then it is added
void update_card_in_memmory(struct CardData new_card){

    //loop through cards until the corect one is found or a invalid card is found
    int i = 0;
    while(1){
        struct CardData c = cards[i];

        if(c.uid == 0){
            break;
        }
        else if(c.uid == new_card.uid){
            break;
        }
        else{
            i++;
        }
    }

    //updatee card data in ram
    cards[i] = new_card;

    //wright data in ram to flash
    wright_cards_to_mem();
}


