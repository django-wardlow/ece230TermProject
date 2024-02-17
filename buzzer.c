/*
 * buzzer.c
 *
 *  Created on: Feb 16, 2024
 *      Author: user1
 */

#include "buzzer.h"

/* Timer_A PWM Configuration Parameter */
Timer_A_PWMConfig pwmConfig =
{
        TIMER_A_CLOCKSOURCE_ACLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        100, //total period
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        50
};

void configure_buzzer(void){
    GPIO_setAsPeripheralModuleFunctionOutputPin(BUZZER_PORT, BUZZER_PIN, GPIO_PRIMARY_MODULE_FUNCTION);

    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
}

void buzzer_on(void){

}

void buzzer_off(void);