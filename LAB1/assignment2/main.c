/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: main.c
 *
 * PROJECT....: EMP
 *
 * DESCRIPTION: Assignment 2, main module. No main.h file.
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 150215  MoH   Module created.
 *
 *****************************************************************************/
/***************************** Include files *******************************/
#include <stdint.h>
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "swtimers.h"
#include "emp_type.h"
#include "systick.h"
#include "events.h"
#include "button.h"

/*****************************    Defines    *******************************/
#define RED_LED 0x0C
#define YELLOW_LED 0x0A
#define GREEN_LED 0x06
#define RED_AND_YELLOW_LED 0x08

/*****************************   Variables   *******************************/
extern INT16S ticks;
int regular_mode_state = STOP;
int state = REGULAR;
INT8U alive_timer = TIM_500_MSEC;
INT8U night_timer = TIM_1_SEC;


void regular_mode()
{
    /*****************************************************************************
     *   Input    :
     *   Output   :
     *   Function : Changing LED like a traffic light.
     ******************************************************************************/

    while (!ticks);

    // The following will be executed every 5ms
    ticks--;

    if (!--alive_timer)
    {

        switch (regular_mode_state)
        {
        case STOP:
            regular_mode_state = WAIT_FOR_GREEN;
            // TURN ON YELLOW AND RED LED
            GPIO_PORTF_DATA_R = RED_AND_YELLOW_LED; // RED AND YELLOW
            alive_timer = TIM_500_MSEC;
            break;

        case WAIT_FOR_GREEN:

            regular_mode_state = GO;
            // TURN OFF RED LED
            // TURN OFF YELLOW LED
            // TURN ON GREEN LED
            GPIO_PORTF_DATA_R = GREEN_LED; // GREEN
            alive_timer = TIM_2_SEC;
            break;

        case WAIT_FOR_RED:
            regular_mode_state = STOP;
            GPIO_PORTF_DATA_R = RED_LED; // RED
            alive_timer = TIM_2_SEC;
            break;

        case GO:
            regular_mode_state = WAIT_FOR_RED;
            // TURN OFF RED LED
            // TURN ON YELLOW LED
            GPIO_PORTF_DATA_R = YELLOW_LED; // YELLOW
            alive_timer = TIM_500_MSEC;
            break;

        default:
            break;
        }
    }
    return 0;
}

void night_mode()
{
    /*****************************************************************************
     *   Input    :
     *   Output   :
     *   Function : Make yellow led blink
     ******************************************************************************/
    while (!ticks);

    // The following will be executed every 5ms
    ticks--;

    if (!--night_timer)
    {
        night_timer = TIM_1_SEC;

        if (GPIO_PORTF_DATA_R == 0x1A) {  // Check if the LED is currently on
               GPIO_PORTF_DATA_R |= 0x0E; // Turn off yellow LED (set bit to 1)
           } else {
               GPIO_PORTF_DATA_R = YELLOW_LED; // Turn off LEDs
           }

    }
}

int main(void)
{
    /*****************************************************************************
     *   Input    :
     *   Output   :
     *   Function : The super loop.
     ******************************************************************************/
    init_gpio();
    init_systick();

    while (1)
    {

        state = select_button(); //driver

        switch (state)
        {
        case REGULAR:
            regular_mode();
            break;

        case EMERGENCY:
            GPIO_PORTF_DATA_R = RED_LED;
            break;

        case NIGHT:
            night_mode();
            break;

        default:
            break;
        }
    }
}
/****************************** End Of Module *******************************/
