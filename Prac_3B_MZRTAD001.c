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
//====================================================================
// SYMBOLIC CONSTANTS
//====================================================================

//====================================================================
// GLOBAL VARIABLES
//====================================================================
#define SW0 GPIO_IDR_0
#define SW1 GPIO_IDR_1
#define SW2 GPIO_IDR_2
int bcd[4] = {0,0,0,0};
int ascii[4] ={0,0,0,0};
//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
void initPorts(void);
void ConverttoBCD(int count);
//====================================================================
// MAIN FUNCTION
//====================================================================
void main (void)
{
	init_LCD();					// Initialise lcd
	lcd_putstring("EEE2046F Prac 3B");		// Display string on line 1
	lcd_command(LINE_TWO);				// Move cursor to line 2
	lcd_putstring("Tadiwa Mazara");			// Display string on line 2
	initPorts();
	int count = 0;

	while(1){
		//loop forever; wait for SW1 to be pressed
		if (~(GPIOA->IDR)&SW0 ){
			lcd_command(CLEAR);
			lcd_putstring("Weather Station");
			lcd_command(LINE_TWO);
			lcd_putstring("Press SW2");
		//loop forever; wait for SW2 to be pressed
			while(1){
				if(~(GPIOA->IDR)&SW2){
					while(~(GPIOA->IDR)&SW2){}
					lcd_command(CLEAR);
					lcd_putstring("Rain bucket tip");

					while(1){
							if(~(GPIOA->IDR)&SW1){
								while(~(GPIOA->IDR)&SW1){}  //loop forever while SW1 is pressed;
								count+=2;
								GPIOB->ODR= count;
							}
							//display count on lcd
							//convert to bcd the ascii
							//iterate through ascii array and putstrings on lcds
							if(~(GPIOA->IDR)&SW2){
									ConverttoBCD(count);
									lcd_command(CLEAR);
									lcd_putstring("Rainfall:");
									lcd_command(LINE_TWO);
									lcd_putchar(48);
									//put first 3 digits
									for(int a =0; a<3;a++){
										lcd_putchar(ascii[a]);
									}
									//insert comma and last digit
									lcd_putchar(46);
									lcd_putchar(ascii[3]);
									lcd_putchar(109);           //insert char"m"
									lcd_putchar(109);           //insert char"m"





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
void initPorts(void){
	//Enable clock
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//enable Pull up resistors
		GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR0_0|
						 GPIO_PUPDR_PUPDR1_0|
						 GPIO_PUPDR_PUPDR2_0|
						 GPIO_PUPDR_PUPDR3_0);
	//Set pins A0, A1, A2 as inputs
		GPIOA->MODER &= ~(GPIO_MODER_MODER0|
						  GPIO_MODER_MODER1|
						  GPIO_MODER_MODER2|
						  GPIO_MODER_MODER3);
	//set PortB0-7 as outputs
		GPIOB->MODER |= 0b00000000000000010101010101010101; //set PortB0-Port B1 as outputs
	}
void ConverttoBCD(int count){
		//obtain count
		int number = count;
		// assign each number a to an index in array
		for(int i =3;i>-1;i--){
			int rem = number %10;
			bcd[i]= rem;
			number /=10;
		}
		//add 48 to each number to convert to ascii
		for(int j =0;j<4;j++){
			ascii[j]= (bcd[j]+48);
		}

}
//********************************************************************
// END OF PROGRAM
//********************************************************************
