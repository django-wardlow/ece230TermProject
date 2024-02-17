/*
 * rfid_wrapper.h
 *
 *  Created on: Feb 16, 2024
 *      Author: user1
 */

#include "driverlib.h"

#ifndef RFID_DRIVER_H_
#define RFID_DRIVER_H_

    #define TIMER_A_PERIOD 3200 //100 ms    


    void rfid_init(void);
    
    void rfid_set_card_read_function(void (*read)(uint32_t));

#endif /* RFID_DRIVER_H_ */
