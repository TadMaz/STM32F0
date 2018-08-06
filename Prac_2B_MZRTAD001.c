//********************************************************************
//*                    EEE2046F C template                           *
//*==================================================================*
//* WRITTEN BY:    	                 		             *
//* DATE CREATED:                                                    *
//* MODIFIED:                                                        *
//*==================================================================*
//* PROGRAMMED IN: Eclipse Luna Service Release 1 (4.4.1)            *
//* DEV. BOARD:    UCT STM32 Development Board                       *
//* TARGET:	   STMicroelectronics STM32F051C6                    *
//*==================================================================*
//* DESCRIPTION:                                                     *
//*                                                                  *
//********************************************************************
// INCLUDE FILES
//====================================================================
#include "lcd_stm32f0.h"
#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
//====================================================================
// SYMBOLIC CONSTANTS
//====================================================================

//====================================================================
// GLOBAL VARIABLES
//====================================================================
#define SW0 GPIO_IDR_0
#define SW1 GPIO_IDR_1
#define SW2 GPIO_IDR_2


//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
void InitPorts(void);
char CountUp(char value);
char CountDown(char value);
void Delay(int Delay1,int Delay2);
//====================================================================
// MAIN FUNCTION
//====================================================================
void main (void)
{
	//init_LCD();
	InitPorts();// Initialise lcd
	/*lcd_putstring("EEE2046F PRAC 2B");		// Display string on line 1
	lcd_command(LINE_TWO);				// Move cursor to line 2
	lcd_putstring("*TAD MAZ*");	*/		// Display string on line 2
	char value=0;

	while(1){
	if (~(GPIOA->IDR)&SW0 ){		//if switch is pressed, start counting up
		while(1){
			CountUp(value);
			value++;
			Delay(2550000,6553500);

			if(~(GPIOA->IDR)&SW2){					//if switch 2 is pressed, start counting down
				while(1){		//CountDown while switch 1 is not pressed or while value is greater than zero
					CountDown(value);
					value--;
					Delay(2550000,6553500);
					if ((value <0)||(~(GPIOA->IDR)&SW1)){ //Stop counting down if SW1 is pressed or value gets to 0.
							break;
					}
				}
			}

		}
	}

	}

}							// End of main

//====================================================================
// FUNCTION DEFINITIONS
//====================================================================
void InitPorts(void){

//--------------------------------------------------------------------
//GPIOA - Pushbuttons
//--------------------------------------------------------------------
	// enable clock for push-buttons
	 RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	// set pins A0-A3 to GPIO inputs, GPIOA_MODERx = 00
	GPIOA->MODER &= ~(GPIO_MODER_MODER0|
	GPIO_MODER_MODER1|
	GPIO_MODER_MODER2|
	GPIO_MODER_MODER3);
	// enable pull-up resistors, GPIOA_PUPDRx = 01
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR0_0|
	GPIO_PUPDR_PUPDR1_0|
	GPIO_PUPDR_PUPDR2_0|
	GPIO_PUPDR_PUPDR3_0);
//--------------------------------------------------------------------
	 //GPIOB - LEDS
 //--------------------------------------------------------------------
	 // enable clock for push-buttons
	 RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	 // set pins B0-B7, B10-B11 to GPIO outputs, GPIOB_MODERx = 01
	GPIOB->MODER |= 0x5555;   /// 0x5555 = 0101010101....pattern

}
char CountUp(char value){
		GPIOB ->ODR = value;
		value = value+1;

		return value;

}
char CountDown(char value){
		GPIOB ->ODR = value;
		value = value+1;
		return value;
}

void Delay(int Delay1,int Delay2){
	for(int i =0;i<Delay1;i++){
		for (int j;j<Delay2;j++){}
	}
}
//********************************************************************
// END OF PROGRAM
//********************************************************************
