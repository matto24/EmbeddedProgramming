#include <stdint.h>
#include <math.h>
#include "tm4c123gh6pm.h"
#include "systick.h"
#include "emp_type.h"

#define TIM_1_SEC 200


extern int ticks;

int main(void)
{
    //A)
    INT16U var1 = 0x1111; // X - any number you enter
    var1 &= ~(0x60);

    //B)
    INT16U var2 = 0x0FF1; // X – any number you enter

    //Clear bit[10-5]
        //~(0x07E0)
    var2 &= ~(0x07E0);
    //Set 4 bits to 1.
    var2 |= (0x03C0);

    //C
    INT16U var3 = 0x67; //0b0110 0111


    INT16U iterator = 0x00;
    INT16U counter = 0x00;


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

        int alive_timer = TIM_1_SEC;

        init_systick();

        while(1) {

            while( !ticks );

            ticks--;

            if(! --alive_timer) {

                iterator = pow(2,counter);

                counter++;
                if(counter>7) {
                    counter = 0;
                }

                if(iterator & var3) {
                    GPIO_PORTF_DATA_R |= 0x04;
                } else {
                    GPIO_PORTF_DATA_R &= ~(0x04);
                }

                alive_timer = TIM_1_SEC;
            }



        }



    return 0;
}
