#include <msp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "driverlib.h"

#include "LED.h"

void InitializeRGBLEDs(void)
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
}

void toggle_LED()
{
    LED_PORT->OUT ^= (all_pins);
}

const Timer_A_UpModeConfig upConfig =
    {
        TIMER_A_CLOCKSOURCE_ACLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_32,
        TIMER_PERIOD,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
        TIMER_A_DO_CLEAR};

void Init_LED_timer(void)
{
    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);

    Interrupt_enableInterrupt(INT_TA1_0);
    Interrupt_enableMaster();

    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);

    toggle_LED();
}

void TA1_0_IRQHandler(void)
{
    toggle_LED();
    Timer_A_clearInterruptFlag(TIMER_A1_BASE);
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_stopTimer(TIMER_A1_BASE);
}