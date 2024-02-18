/*
*Author: Justin Zimmerman, Django Wardlow
* Created: 2/17/24
*
*/
#include <msp.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define LED_PORT P2 // external pins
#define green_pin BIT3
#define red_pin BIT4

// #define LED_PORT P2 // onboard LED test
// #define green_pin BIT1
// #define red_pin BIT0

#define DELAY_TIMER TIMER_A1_BASE
#define all_pins (green_pin | red_pin)
#define ALL_LEDs_On_Time 10

#define TIMER_PERIOD 500

//period in terms of ACLK ticks
#define BUZZER_PERIOD 12

#define BUZZER_TIMER TIMER_A2_BASE
#define BUZZER_PORT GPIO_PORT_P5
#define BUZZER_PIN GPIO_PIN6

void init_feedback(void);

void toggle_LED(void);

void positive_feedback(void);
