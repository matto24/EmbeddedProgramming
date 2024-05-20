/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: rotary.c
 *
 * PROJECT....: Final Assignment
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 050128  KA    Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "emp_type.h"
#include "glob_def.h"
// #include "binary.h"
#include "rotary.h"
#include "adc.h"
#include "fsm.h"

/*****************************    Defines    *******************************/
#define CW 0
#define CCW 1

#define PA5 0x20
#define PA6 0x40
#define PA7 0x80

#define MAX_DELAY_MS 1000 // 1 Hz
#define MIN_DELAY_MS 100  // 10 Hz
#define MAX_ADC_VALUE 4095

/*****************************   Constants   *******************************/

INT8U bank_note[3][3] = {"100", "50 ", "10 "};
INT8U bank_note_int[3] = {100, 50, 10};

/*****************************   Variables   *******************************/

QueueHandle_t xQueueLCD;

INT8U rotary_dir = CW;

volatile INT8U rotary_value = 0;
SemaphoreHandle_t xRotaryValueMutex;

SemaphoreHandle_t xRotaryDirectionSemaphore;

SemaphoreHandle_t xRotaryPressSemaphore;

INT8U final_rot_value = 0;
INT8U first_run_banknote = 1;

INT8U is_final_selected = 0;
INT8U first_run_withdrawal = 1;

volatile INT8U rotary_pressed = 0;

/*****************************   Functions   *******************************/
void wr_banknote_str(INT8U num)
{
    INT8U i;
    for (i = 0; i < 3;)
    {
        xQueueSend(xQueueLCD, &bank_note[num][i], portMAX_DELAY);
        i++;
    }
}

void init_rotary(void)
{
    int dummy;

    // Enable the GPIO port that is used
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA | SYSCTL_RCGC2_GPIOF;

    // Do a dummy read to insert a few cycles after enabling the peripheral.
    dummy = SYSCTL_RCGC2_R;

    // Set pins PA5, PA6, PA7 as inputs
    GPIO_PORTA_DIR_R &= ~(PA5 + PA6 + PA7);

    // Enable digital functions for PA5, PA6, PA7
    GPIO_PORTA_DEN_R |= PA5 + PA6 + PA7;

    // Enable pull-up resistors for PA5, PA6, PA7
    GPIO_PORTA_PUR_R |= PA5 + PA6 + PA7;

    // Interrupt stuff
    GPIO_PORTA_IS_R &= ~PA5;       // PA5 is edge-sensitive
    GPIO_PORTA_IBE_R &= ~PA5;      // PA5 is not both edges
    GPIO_PORTA_IEV_R |= PA5;       // Trigger on rising edge for PA5
    GPIO_PORTA_ICR_R |= PA5 + PA7; // Clear any prior interrupts on PA5
    GPIO_PORTA_IM_R |= PA5 + PA7;  // Enable interrupt on PA5
    NVIC_EN0_R = 1 << 0;           // Enable IRQ for GPIO Port A

    // Shared resources
    xRotaryValueMutex = xSemaphoreCreateMutex();
    xRotaryDirectionSemaphore = xSemaphoreCreateBinary();
    xRotaryPressSemaphore = xSemaphoreCreateBinary();
}

void GPIOA_Handler(void)
{

    if (GPIO_PORTA_RIS_R & PA7) // Check if interrupt was caused by PA7 (rotary press)
    {
        xSemaphoreGiveFromISR(xRotaryPressSemaphore, NULL); 
        rotary_pressed = 1;
        set_rotary_complete();

        GPIO_PORTA_ICR_R |= PA7; // clear interrupt flag
    }
    else if (GPIO_PORTA_RIS_R & PA5 && !rotary_pressed) // Check if interrupt was caused by PA5
    {
        rotary_dir = ((GPIO_PORTA_DATA_R & (1 << 6)) != 0) ? CW : CCW; // if PA6 is high => CW, PA6 is low => CCW
        xSemaphoreGiveFromISR(xRotaryDirectionSemaphore, NULL);

        GPIO_PORTA_ICR_R |= PA5; // Clear the interrupt flag for PA5
    }
}

INT8U getRotaryValue(void)
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function :
 *****************************************************************************/
{
    INT8U outp;
    if (xSemaphoreTake(xRotaryValueMutex, portMAX_DELAY) == pdTRUE)
    {
        outp = rotary_value;
        xSemaphoreGive(xRotaryValueMutex);
    }
    return outp;
}

void rotary_task(void *pvParameters)
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function :
 *****************************************************************************/
{
    while (1)
    {
        if (getState() == CHOOSE_BANKNOTE)
        {
            if (first_run_banknote)
            {
                clr_LCD();
                set_cursor(0x0C);
                INT8U banknote_text[] = "Choose banknote:";
                wr_str_LCD(banknote_text);
                move_LCD(0, 1);
                wr_banknote_str(rotary_value);
                first_run_banknote = 0;
            }

            if (xSemaphoreTake(xRotaryDirectionSemaphore, portMAX_DELAY) == pdTRUE)
            {
                if (xSemaphoreTake(xRotaryValueMutex, portMAX_DELAY) == pdTRUE)
                {
                    if (rotary_dir == CW)
                    {
                        rotary_value = (rotary_value + 1) % 3;
                    }
                    else
                    {
                        rotary_value = (rotary_value - 1 + 3) % 3;
                    }
                    move_LCD(0, 1);
                    wr_banknote_str(rotary_value);
                    // GPIO_PORTF_DATA_R = (rotary_value+1)*2;
                    vTaskDelay(100 / portTICK_RATE_MS);

                    xSemaphoreGive(xRotaryValueMutex);
                }
            }
        }
    }
}
void rotary_pressed_task(void *pvParameters)
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function :
 *****************************************************************************/
{
    while (1)
    {
        if (getState() == WITHDRAWAL)
        {
            if (first_run_withdrawal)
            {
                clr_LCD();
                set_cursor(0x0C);
                INT8U withdraw_text[] = "Printing money";
                wr_str_LCD(withdraw_text);

                final_rot_value = getRotaryValue();
                GPIO_PORTF_DATA_R = 0x0E;

                is_final_selected = 1;
                first_run_withdrawal = 0;
            }

            // if (xSemaphoreTake(xRotaryPressSemaphore, portMAX_DELAY) == pdTRUE)
            //{
            // }
            if (is_final_selected)
            {
                switch (final_rot_value)
                {
                case 0:
                    GPIO_PORTF_DATA_R ^= 0x02;
                    break;
                case 1:
                    GPIO_PORTF_DATA_R ^= 0x04;
                    break;
                case 2:
                    GPIO_PORTF_DATA_R ^= 0x08;
                    break;
                }
                INT16U adc_value = get_adc();
                INT16U delay_ms = MAX_DELAY_MS - ((adc_value * (MAX_DELAY_MS - MIN_DELAY_MS)) / MAX_ADC_VALUE);
                vTaskDelay(delay_ms / portTICK_RATE_MS);
            }
        }
    }
}

/****************************** End Of Module *******************************/
