/*
 * buzzer.h
 *
 *  Created on: Feb 16, 2024
 *      Author: Django Wardlow
 */


#include "driverlib.h"

#ifndef BUZZER_H_
#define BUZZER_H_

//period in terms of ACLK ticks
#define BUZZER_PERIOD 12

#define BUZZER_TIMER TIMER_A2_BASE
#define BUZZER_PORT GPIO_PORT_P5
#define BUZZER_PIN GPIO_PIN6

void configure_buzzer(void);

void buzzer_on(void);

void buzzer_off(void);

#endif /* BUZZER_H_ */
