

/**
 * main.c
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "systick_frt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"
#include "key.h"
#include "lcd.h"
#include "fsm.h"
#include "semphr.h"
#include "withdraw.h"
#include "button.h"
#include "fsm.h"
#include "rotary.h"
#include "adc.h"

#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO 1
#define MED_PRIO 2
#define HIGH_PRIO 3


SemaphoreHandle_t xLCDSemaphore;
QueueHandle_t xQueueLCD;


static void setupHardware(void)
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function :
 *****************************************************************************/
{
  // TODO: Put hardware configuration and initialisation in here

  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  init_systick();
  init_gpio();
  init_keypad();
  initLCD();
  init_buttons();
  init_fsm();
  init_rotary();
  init_adc();
}

int main(void)
{
  setupHardware();
          // BACKBONE TASKS
  xTaskCreate(keypad_task, "KEYPAD", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL);
  xTaskCreate(lcd_task, "LCD", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL);
  xTaskCreate(fsm_task, "FSM", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL);

          // State TASKS
  xTaskCreate( enter_amount_task, "ENTER_AMOUNT_TASK", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate( enter_pin_task, "ENTER PIN", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate( withdraw_task, "withdraw", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate(rotary_task, "ROTARY TASK", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL);
  xTaskCreate(rotary_pressed_task, "LED BLINKING TASK", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL);
  
          // RTOS SCHEDULER
  vTaskStartScheduler();
  return 0;
}
