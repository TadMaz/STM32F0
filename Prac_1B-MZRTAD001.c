//********************************************************************
//*                    EEE2046F/EEE2050F C template                  *
//*==================================================================*
//* WRITTEN BY: Tadiwanashe Mazara    	                 		             *
//* DATE CREATED: 4/5/17                                                   *                                                      *
//*==================================================================*
//* PROGRAMMED IN: Eclipse Luna Service Release 1 (4.4.1)            *
//* TARGET:    PC or STM32F0?                                        *
//*==================================================================*
//* DESCRIPTION:                                                     *
//*                                                                  *
//********************************************************************
// INCLUDE FILES
//====================================================================
#include <stdio.h>
#include <math.h>
//====================================================================
// GLOBAL CONSTANTS
//====================================================================
#define TITLE "BINARY TO DECIMAL CONVERTOR"
#define AUTHOR "Tadiwanashe Mazara"
#define YEAR 2017
//====================================================================
// GLOBAL VARIABLES
//====================================================================
int number=1;

//====================================================================
// FUNCTION DECLARATIONS
int dec2bin(int value);
//====================================================================

//====================================================================
// MAIN FUNCTION
//====================================================================
/*int main()
{
        printf("*****************************\n");
        printf(" %s\n",TITLE);
        printf(" Written by: %s\n",AUTHOR);
        printf(" Date: %d\n",YEAR);
        printf("*****************************\n");
        while(number>0){

                printf("Enter a decimal number: ");
                scanf("%d",&number);
                if (number>0){
                    printf("The number you entered is %d\n",number);
                    printf("The log2 of the number is %4.2f\n",log2(number));
                    printf("The number divided by 2 is %d\n", number/2);
                    printf("The remainder is %d\n",number%2);
                    printf("The binary value is %d\n",dec2bin(number));
                }
            }
        printf("EXIT");

        return 0;
}
int dec2bin(int value){

        int i =0;
        int bin = 0;
        int remainder =0;
        int power = 1;

        while(value > 0){

            remainder = value%2;
            value /= 2;
            bin += (remainder*power);
            power *=10;
        }
        return bin ;


}
*/
int i = 5+4*6/3;
printf("%d", i);

