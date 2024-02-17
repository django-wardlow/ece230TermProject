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

#define all_pins (green_pin | red_pin)
#define ALL_LEDs_On_Time 10

#define TIMER_PERIOD 1000

void InitializeRGBLEDs(void);

void toggle_LED(void);

void Init_LED_timer(void);
