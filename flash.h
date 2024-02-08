/*
 * flash.h
 *
 *  Created on: Feb 8, 2024
 *      Author: user1
 */

#ifndef FLASH_H_
#define FLASH_H_

//bank 1 sector 0 of information memory
#define DATA_START 0x00204000
#define DATA_END 0x00204FFF

//24 bytes of data
//a card with a uid of 0 is invalid and indicates that the card does not exist
struct CardData {
    uint32_t uid;
    uint32_t clocked_in;
    uint64_t clock_in_time;
    uint64_t accumulated_time;
};

#define CARD_DATA_BYTES 24
// 4k(sector size)/24 gives a max of 166 cards in a flash sector at a time
#define CARD_COUNT 166

#define CARD_BYTES (CARD_DATA_BYTES*CARD_COUNT)

void init_flash_memory(void);
void wright_cards_to_mem(void);
void read_cards_from_mem();



#endif /* FLASH_H_ */