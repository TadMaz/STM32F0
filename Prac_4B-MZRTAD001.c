//********************************************************************
//*                    EEE2046F C template                           *
//*==================================================================*
//* WRITTEN BY:   Tadiwanashe Mazara 	                 		     *
//* DATE CREATED:   22 June 2017                                     *
//* MODIFIED:        23.06                                           *
//*==================================================================*
//* PROGRAMMED IN: Eclipse Luna Service Release 1 (4.4.1)            *
//* DEV. BOARD:    UCT STM32 Development Board                       *
//* TARGET:	   STMicroelectronics STM32F051C6                        *
//*==================================================================*
//* DESCRIPTION:                                                     *
//*                                                                  *
//********************************************************************
// INCLUDE FILES
//====================================================================
#include "lcd_stm32f0.h"
#include "stm32f0xx.h"
#include<stdio.h>
#include <math.h>
//====================================================================
// SYMBOLIC CONSTANTS
//====================================================================

//====================================================================
// GLOBAL VARIABLES
//====================================================================
#define SW0 ((GPIOA->IDR & GPIO_IDR_0)==0)
#define SW1 ((GPIOA->IDR & GPIO_IDR_1)==0)
#define SW2 ((GPIOA->IDR & GPIO_IDR_2)==0)
#define SW3 ((GPIOA->IDR & GPIO_IDR_3)==0)
int bcd[4] = {0,0,0,0};
int ascii[4] ={0,0,0,0};
int rainfall = 0;
uint8_t ADC_value =0;
#define Vsp 24.0
//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
void initPorts(void);
void ConverttoBCD(int count);
void init_ADC(void);
void init_NVIC(void);
void init_EXTI(void) ;
void EXTI0_1_IRQHandler(void);
void display(void);
void check_battery(void);
void show_voltage(double voltage);
//====================================================================
// MAIN FUNCTION
//====================================================================
void main (void)
{
	init_LCD();					// Initialise lcd
	lcd_putstring("EEE2046F Prac 4B");		// Display string on line 1
	lcd_command(LINE_TWO);				// Move cursor to line 2
	lcd_putstring("Tadiwa Mazara");			// Display string on line 2
	initPorts();
	init_EXTI();
	init_NVIC();
	init_ADC();
	display();



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
	//Set pins A5 as ananlogue inputs
		GPIOA->MODER |= GPIO_MODER_MODER5;
	//set PortB0-7 and PB10 and PB11 as outputs
		GPIOB->MODER |= 0b00000000010100010101010101010101; ///set PortB0-Port B1 as outputs
	//set POT0 as analogue input

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
void init_ADC(void){
	//enable ADC clock
		RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
	//Stop the ADC
		ADC1->CR &=~ADC_CR_ADSTART;
	//set Parameters 8 bit resolution,right align, single shot mode
		ADC1-> CFGR1 |= ADC_CFGR1_RES_1;
		ADC1 -> CFGR1 &= ~ADC_CFGR1_ALIGN;
		ADC1 -> CFGR1 &= ~ADC_CFGR1_CONT;
	//select channel(in this case channel 5)
		ADC1->CHSELR |=  ADC_CHSELR_CHSEL5;

}


void init_NVIC(void){
	//enable interrupt from EXTI1 in NVIC 
	NVIC_EnableIRQ(EXTI0_1_IRQn);
}

void init_EXTI(void) {

	//enable clock for sys cfg controller
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	// map PA1 to EXTI1
	SYSCFG->EXTICR[1]= SYSCFG_EXTICR1_EXTI1_PA;
	//unmask external interrupt
	EXTI->IMR|=EXTI_IMR_MR1;
	//trigger on falling edge(when the switch is pressed)
	EXTI->FTSR|= EXTI_FTSR_TR1;
	


}

void EXTI0_1_IRQHandler(void){
	//clear set by program counter flag(interrupt pending bit)
	EXTI -> PR |= EXTI_PR_PR1;
	//wait until switch is released 
	while (SW1){}
	//increment the rainfall
	rainfall +=2;
											/*GPIOB->ODR = 1;  DEBUG*/ 
}

void display(void){
	//wait for a button to be pressed
	while(1){
		// display welcome messsage
		if(SW0){
			lcd_command(CLEAR);
			lcd_putstring("Weather Station");
			lcd_command(LINE_TWO);
			lcd_putstring("Press S2");
		//display rain bucket tip
		}else if (SW1){
			lcd_command(CLEAR);
			lcd_putstring("Rain Bucket tip");
		//display the current rainfall		
		}else if(SW2){
			lcd_command(CLEAR);
			lcd_putstring("Rainfall:");
			lcd_command(LINE_TWO);
			ConverttoBCD(rainfall); 		//convert rainfall to bcd for display
			lcd_command(CLEAR);
			lcd_putstring("Rainfall:");
			lcd_command(LINE_TWO);
			lcd_putchar(48);
		//put first 3 digits
			for(int a =0; a<3;a++){
				lcd_putchar(ascii[a]);
			}
		//insert comma and zero
			lcd_putchar(46);
			lcd_putchar(ascii[3]);
			lcd_putchar(109);		//insert mm
			lcd_putchar(109);
			
			
		//display voltage
		}else if(SW3){
			check_battery();
		

		}

	}
}
void check_battery(void){

	//activate ADC to be ready
			ADC1 -> CR |= ADC_CR_ADEN;
	//ADC does not start immediately(we have to wait); check status
			while((ADC1->ISR & ADC_ISR_ADRDY) ==0){}
	//once ADC is ready, we start conversion
			ADC1 -> CR |= ADC_CR_ADSTART;
	//There is a conversion time; wait until conversion is complete
			while((ADC1->ISR & ADC_ISR_EOC) == 0){}
	//Read value from ADC1 Data register 16 bit
			ADC_value = ADC1->DR;
	//convert ADC value to analogue voltage and display on screen
			double ADC_voltage = (Vsp *ADC_value)/((pow(2,8))-1);
			show_voltage(ADC_voltage);             
	//switch on LED D9 if voltage is less than 14V(has ADC value of 148.75)
			if(ADC_value < 148.75){
				GPIOB->ODR |=0b0000110000000000; //D9
			}
}
void show_voltage(double voltage){
	/** This function takes in a 8 bit ADC voltage and displays it on the lcd correct to 3 dp places*/
			char buffer[16];							//array to store formatted characters.
			unsigned char a = (int)voltage;					//type cast as int to obtain value after comma and store as char
			unsigned char b = (int)((voltage - a)*1000) ;  //b obtains decimal part;since we want 3 dp, multiply this by 1000
			sprintf(buffer,"%3d.%3.3d V",a,b);
			lcd_command(CLEAR);
			lcd_putstring("Battery:");
			lcd_command(LINE_TWO);
			lcd_putstring(buffer);
}

//********************************************************************
// END OF PROGRAM
//********************************************************************
