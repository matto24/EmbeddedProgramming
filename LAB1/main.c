#include <stdint.h>
#include "tm4c123gh6pm.h"

/**
 * main.c
 */
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

    while(1) {
        //Check if button is not pressed. If button is pressed go to else
        if (GPIO_PORTF_DATA_R & 0x10)
            //Turn OFF LED
           GPIO_PORTF_DATA_R &= ~(0x08);
        else
            //Button is pressed
            //Turns the LED ON.
           GPIO_PORTF_DATA_R |= 0x08;
    }
	return(0);
}