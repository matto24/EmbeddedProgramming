#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

/**
 * main.c
 */


// Timer0 A interrupt service routine
void Timer0A_Handler(void) {
    // Clear the timer interrupt
    TIMER0_ICR_R = TIMER_ICR_TATOCINT;

}

void GPIOF_Handler(void)
{
    // Clear the interrupt flag
    GPIO_PORTF_ICR_R = 0x10;

    // Handle the interrupt (toggle an LED, for example)
    GPIO_PORTF_DATA_R ^= 0x02;
}


int main(void)
{
    int dummy;

    //Enable the GPIO port that is used for the on-board LEDs and switches
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

    //Do a dummy read to insert a few cycles after enabling the peripheral
    dummy = SYSCTL_RCGC2_R;

    //Set the direction as output (PF1-PF3)
    GPIO_PORTF_DIR_R = 0x0E;

    //Enable the GPIO pins for digital function (PF1-PF4)
    GPIO_PORTF_DEN_R = 0x1E;

    //Enable internal pull-up (PF4)
    //Done to ensure the input pin is pulled to a high level when the button is not pressed
    GPIO_PORTF_PUR_R = 0x10;



//-------------------------------------\|/ TIMER \|/-----------------------------------------------------------


    // Enable timer peripheral clock
    SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;

    // Disable timer
    TIMER0_CTL_R &= ~(TIMER_CTL_TAEN);
    // Enable 32-bit configuration
    TIMER0_CFG_R = 0x00000000;
    // Set to Periodic Timer mode, first clear then set
    TIMER0_TAMR_R &= ~(TIMER_TAMR_TAMR_M);
    TIMER0_TAMR_R |= TIMER_TAMR_TAMR_PERIOD;
    // Set Reload value. timeout = 1sek @ F_CPU = 16 Mhz.
    TIMER0_TAILR_R = 16000000;
    // Enable timer 0 interrupt
    TIMER0_IMR_R |= TIMER_IMR_TATOIM;


//-------------------------------------\|/ INTERRUPT \|/-----------------------------------------------------------
    // Vector number 35, Interrupt Number = 19
    // Clear pending interrupt
    //NVIC_UNPEND0_R |= NVIC_UNPEND0_INT19;
    // Set Priority to 0x10, first clear then set.
    NVIC_PRI4_R &= ~(NVIC_PRI4_INT19_M);
    NVIC_PRI4_R |= (NVIC_PRI4_INT19_M & (0x10<<NVIC_PRI4_INT19_S));
    // Enable NVIC interrupt
    //NVIC_EN0_R |= NVIC_EN0_INT19;
    // Enable and start timer
    TIMER0_CTL_R |= TIMER_CTL_TAEN;


// Configure the interrupt to trigger on falling edge (button press)
GPIO_PORTF_IS_R &= ~0x10;     // PF4 is edge-sensitive
GPIO_PORTF_IBE_R &= ~0x10;    // PF4 is not both edges
GPIO_PORTF_IEV_R &= ~0x10;    // PF4 falling edge event
GPIO_PORTF_IM_R |= 0x10;      // Arm interrupt on PF4

// Enable GPIO port F interrupt in the NVIC
NVIC_EN0_R |= 0x40000000;

// Enable interrupts globally.
__enable_irq();


//-------------------------------------------------------------------------------------------------------------

    int count = 0;
    int color[] = {0x00,0x08,0x04,0x0C,0x02,0x0A,0x06,0x0E};
    bool countsUp = true;
    bool buttonPressed = false;



    while(1) {
        //Start timer



        //Check if button is not pressed.
        if (GPIO_PORTF_DATA_R & 0x10) {
            if(buttonPressed){


                //Button is pressed
                buttonPressed = false;
                            if(countsUp) {
                                if(count<7) {
                                    count = count+1;
                                } else {
                                    count = 0;
                                }
                            } else {
                                if(count>1) {
                                    count = count-1;
                                } else {
                                    count = 7;
                                }
                            }
            //Delay for debouncing
            int i=0;
            for(i =0; i< 1000; i++);

            }
             //Turns the LED ON.
            GPIO_PORTF_DATA_R = color[count];

        } else  {
            buttonPressed = true;

            //Wait

        }

    }
    return(0);
}

