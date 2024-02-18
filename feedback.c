/*
*Author: Justin Zimmerman, Django Wardlow
* Created: 2/17/24
*
* Adapted from MSPware examples MSP432 Timer_A - Variable PWM and 
* Timer_A UpMode GPIO Toggle by Timothy Logan
* 
*/

#include <msp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "driverlib.h"

#include "feedback.h"

void init_feedback(void)
{
    unsigned int delay; // time to turn on and off each LED to make sure they work
    LED_PORT->SEL0 &= ~(all_pins);
    LED_PORT->SEL1 &= ~(all_pins);
    LED_PORT->DIR |= (all_pins);

    LED_PORT->OUT |= (all_pins);
    for (delay = 0; delay < ALL_LEDs_On_Time; delay++)
        ;
    // turn off all LEDs
    LED_PORT->OUT &= ~(green_pin);

    configure_buzzer();
}

void toggle_LED()
{
    LED_PORT->OUT ^= (all_pins);
}

//500 ms delay
const Timer_A_UpModeConfig upConfig =
    {
        TIMER_A_CLOCKSOURCE_ACLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_32,
        TIMER_PERIOD,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
        TIMER_A_DO_CLEAR
    };

//buzzer tone
Timer_A_PWMConfig pwmConfig =
{
        TIMER_A_CLOCKSOURCE_ACLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        BUZZER_PERIOD, //total period
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        BUZZER_PERIOD/2

};

/// @brief switches the led to green and plays a tone for 500 ms
void positive_feedback(void)
{
    Timer_A_configureUpMode(DELAY_TIMER, &upConfig);

    Interrupt_enableInterrupt(INT_TA1_0);

    Interrupt_setPriority(TA1_0_IRQn, 0);

    Interrupt_enableMaster();

    Timer_A_startCounter(DELAY_TIMER, TIMER_A_UP_MODE);

    buzzer_on();

    toggle_LED();
}

//called after a 500 ms delay
void TA1_0_IRQHandler(void)
{
    toggle_LED();
    buzzer_off();
    Timer_A_clearInterruptFlag(DELAY_TIMER);
    Timer_A_clearCaptureCompareInterrupt(DELAY_TIMER, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_stopTimer(DELAY_TIMER);
}

//configures the buzzer to play a tone
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