/*
 * buzzer.c
 *
 *  Created on: Feb 16, 2024
 *      Author: Django Wardlow
 */

#include "buzzer.h"

/* Timer_A PWM Configuration Parameter */
Timer_A_PWMConfig pwmConfig =
{
        TIMER_A_CLOCKSOURCE_ACLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        BUZZER_PERIOD, //total period
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        BUZZER_PERIOD/2

};

void configure_buzzer(void){
    
    Timer_A_generatePWM(BUZZER_TIMER, &pwmConfig);

    buzzer_off();
}

void buzzer_on(void){
    GPIO_setAsPeripheralModuleFunctionOutputPin(BUZZER_PORT, BUZZER_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
}

void buzzer_off(void){
    GPIO_setAsInputPin(BUZZER_PORT, BUZZER_PIN);
}
