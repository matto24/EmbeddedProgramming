#include <stdint.h>
#include "tm4c123gh6pm.h"

/**
 * main.c
 */
#define SW1 1
#define SW2 2

int correctCount = 0;
int wrongCount = 0;
int correctSequence[] = {1,2,2,1,2};
int correctSequenceLength = sizeof(correctSequence)/sizeof(correctSequence[0]);


void GPIOF_Init(void)
{
    /**************************************************************************************
    * Input: None
    * Output: None
    * Function: Initializes GPIO Port F for button and LED
    **************************************************************************************/
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // Enable clock to GPIO Port F
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0){}; // Wait for clock to stabilize

    GPIO_PORTF_DIR_R &= ~0x11; // Set PF4 (SW1) and PF0(SW1) as input
    GPIO_PORTF_DIR_R |= 0x0E;  // Set PF1, PF2, and PF3 (LEDs) as outputs
    GPIO_PORTF_DEN_R |= 0x1F;  // Enable digital function on PF0, PF1, PF2, PF3 and PF4
    GPIO_PORTF_PUR_R |= 0x11;  // Enable pull-up on PF4 and PF0
    GPIO_PORTF_IS_R &= ~0x11;  // PF4 and PF0 is edge-sensitive not high/low
    GPIO_PORTF_IBE_R &= ~0x11; // PF4 and PF0 is not both edges
    GPIO_PORTF_IEV_R &= ~0x11; // PF4 and PF0 falling edge event (release) press??????
    GPIO_PORTF_ICR_R |= 0x11;  // Clear the interrupt flag for PF4 and PF0
    GPIO_PORTF_IM_R |= 0x11;   // Enable interrupt on PF4 (SW1) and PF0 (SW2)
    NVIC_EN0_R = 1 << 30;      // Enable interrupt 30 in NVIC
}


void GPIOF_Handler(void)
{
    /**************************************************************************************
     * Input: None
     * Output: None
     * Function: Interrupt handler for GPIO Port F
     ***************************************************************************************/

    if (GPIO_PORTF_RIS_R & 0x10) // Check if interrupt was caused by PF4
    {
        if(correctSequence[correctCount+wrongCount] == SW1) {
            correctCount++;
        } else {
            wrongCount++;
        }
        while(!(GPIO_PORTF_DATA_R & 0x10))  // wait here until the switch is released

        GPIO_PORTF_ICR_R |= 0x10; // Clear the interrupt flag for PF4
    }

    if (GPIO_PORTF_RIS_R & 0x01) // Check if interrupt was caused by PF0
        {
            if(correctSequence[correctCount] == SW2) {
                correctCount++;
            } else {
                wrongCount++;
            }
            while(!(GPIO_PORTF_DATA_R & 0x01))// wait here until the switch is released

            GPIO_PORTF_ICR_R |= 0x01; // Clear the interrupt flag for PF0
        }


    if(correctCount == correctSequenceLength) {
        GPIO_PORTF_DATA_R |= 0x08;
        correctCount = 0;
        wrongCount = 0;
    }

    if((correctCount+wrongCount) == correctSequenceLength)
     {
        GPIO_PORTF_DATA_R |= 0x02;
        correctCount = 0;
        wrongCount = 0;
    }

}


int main(void)
{

    GPIOF_Init();

    while(1) {

    }
	return 0;
}
