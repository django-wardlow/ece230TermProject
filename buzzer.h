/*
 * buzzer.h
 *
 *  Created on: Feb 16, 2024
 *      Author: user1
 */


#include "driverlib.h"

#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZER_PORT GPIO_PORT_P5
#define BUZZER_PIN GPIO_PIN6

void configure_buzzer(void);

void buzzer_on(void);

void buzzer_off(void);

#endif /* BUZZER_H_ */
