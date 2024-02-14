#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "systick.h"

#define TIM_200_MS 40
int double_press_timer;
extern int ticks;
int alive_timer = TIM_200_MS;
bool automode_active;

/**
 * main.c
 */

void GPIOF_Init(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // Enable clock to GPIO Port F
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0)
    {
    }; // Wait for clock to stabilize

    GPIO_PORTF_DIR_R &= ~0x10; // Set PF4 (SW1) as input
    GPIO_PORTF_DIR_R |= 0x0E;  // Set PF1, PF2, and PF3 (LEDs) as outputs
    GPIO_PORTF_DEN_R |= 0x1E;  // Enable digital function on PF1, PF2, PF3 and PF4
    GPIO_PORTF_PUR_R |= 0x10;  // Enable pull-up on PF4
    GPIO_PORTF_IS_R &= ~0x10;  // PF4 is edge-sensitive not high/low
    GPIO_PORTF_IBE_R &= ~0x10; // PF4 is not both edges
    GPIO_PORTF_IEV_R &= ~0x10; // PF4 falling edge event (release) press??????
    GPIO_PORTF_ICR_R |= 0x10;  // Clear the interrupt flag for PF4
    GPIO_PORTF_IM_R |= 0x10;   // Enable interrupt on PF4 (SW1)
    NVIC_EN0_R = 1 << 30;      // Enable interrupt 30 in NVIC
}

int color_count = 0;
bool direction_up = true; // Decrement or increment

void GPIOF_Handler(void)
{
    if (GPIO_PORTF_RIS_R & 0x10) // Check if interrupt was caused by PF4
    {
        automode_active = false;
        // Set direction for double press
        if (double_press_timer < 300000)
        {
            direction_up = !direction_up;
        }
        else
        {
            //CALL FUNCTION TO CHANGE LED
            LED_Changer();
        }



        // Delay for debouncing
        int i = 0;
        for (i = 0; i < 400000; i++);
        double_press_timer = 0;

        GPIO_PORTF_ICR_R |= 0x10; // Clear the interrupt flag for PF4
    }
}

void LED_Changer(void)
{
    int color[] = {0x00, 0x08, 0x04, 0x0C, 0x02, 0x0A, 0x06, 0x0E};
    if (direction_up)
    {
        // count < 7 increment by 1 or set 0
        color_count = (color_count < 7) ? color_count + 1 : 0;
    }
    else
    {
        // count > 7 decrement by 1 or set 7
        color_count = (color_count > 0) ? color_count - 1 : 7;
    }
    GPIO_PORTF_DATA_R = color[color_count]; // Toggle the LEDS on PF1, PF2 and PF3
}

void auto_mode(void)
{
    //GPIO_PORTF_DATA_R &= ~(0xFF);
    while(!ticks);

    //Decrement ticks every 5ms.--;

    ticks--;



    if(! --alive_timer) {
        LED_Changer();
        alive_timer = TIM_200_MS;
    }

}


int main(void)
{
    GPIOF_Init(); // Initialize GPIO Port F for button and LED
    init_systick();
    double_press_timer = 10000; // Start at 10000 to make sure it doesn't count the first press as a double press.
    int autotimer = 0;
    automode_active = false;

    while (1)
    {
        if(automode_active){
            auto_mode();
        }
        if(GPIO_PORTF_DATA_R & 0x10){
            autotimer = 0;
        }
        else{
                    autotimer++;
                    if(autotimer > 1000000){

                        automode_active = true;
                    }


                }
        double_press_timer++;


    }
    return 0;
}
