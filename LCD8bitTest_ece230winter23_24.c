//File Name: LCD8bitTest_ece230winter23_24.c
//Author: Jianjian Song
//Date: January 25, 2024
//ECE230-01/02 Winter 2023-2024

/******************************************************************************
 * MSP432P1111
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P41111
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST          P2.6 |<--Register Select RS
 *            |             P2.7 |<--E Clock
 *            |                  |
 *            |          Port 4  |--> Data Lines
 *            |                  |
 *            |                  |
 * Author: 
*******************************************************************************/
/* Standard Includes */
#include <msp.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "lcd8bits.h"

int demo(void)
{
    char Title[]={"LCD Spring 2023"};
    char DataBuffer[16];
    float pi;

    /* Stop Watchdog timer */
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
//write a floating point number in buffer
    pi=3.141592653589;
    sprintf(DataBuffer, " pi=%f6.4", pi);

//    LCD_Initializtion();    //this can be either 4-bit or 8-bit configuration
//    lcd4bits_init();
    lcd8bits_init();

    lcd_SetLineNumber(FirstLine);
      lcd_puts(Title);

    lcd_SetLineNumber(SecondLine);

//    lcd4bits_write(DATA_MODE, 'e');
//    lcd4bits_write(DATA_MODE, 'c');

    lcd8bits_write(DATA_MODE, 'e');
    lcd8bits_write(DATA_MODE, 'c');
    lcd_putch('e');
    lcd_putch('2');
    lcd_putch('3');
    lcd_putch('0');
    lcd_puts(DataBuffer);

    while(1) { };
}  //end main()

