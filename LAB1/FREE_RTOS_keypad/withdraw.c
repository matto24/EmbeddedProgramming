/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: withdraw.c
 *
 * PROJECT....: ECP
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

#include "gpio.h"
#include "withdraw.h"
#include "swtimers.h"
#include "button.h"
#include "messages.h"
#include "events.h"
#include "tmodel.h"
#include "lcd.h"
#include "fsm.h"

/*****************************    Defines    *******************************/
#define PF0 0 // Bit 0

#define RED 2
#define YELLOW 4
#define GREEN 8

/*****************************   Constants   *******************************/
QueueHandle_t xQueueLCD;

/*****************************   Variables   *******************************/
INT8U first_run_withdraw = 1;

INT8U num = 3;
INT8U number_arr[4][3] = {"50 ", "100 ", "200", "500"};
INT16U withdraw_value[4] = {50, 100, 200, 500};

/*****************************   Functions   *******************************/

void wr_withdraw_str()
{
  INT8U i;
  for (i = 0; i < 3;)
  {

    xQueueSend(xQueueLCD, &number_arr[num][i], portMAX_DELAY);

    i++;
  }
}

void withdraw_task(void *pvParameters)
{
  //
  while (1)
  {
    if (getState() == CHOOSE_WITHDRAW_AMOUNT)
    {

      if (first_run_withdraw)
      {
        clr_LCD();
        set_cursor(0x0C);
        BOOLEAN withdraw_chosen = FALSE;
        INT8U withdraw[] = "Withdraw Amount:";
        wr_str_LCD(withdraw);
        move_LCD(0, 1);
        wr_withdraw_str();
        first_run_withdraw = 0;
      }
      //swt_ctrl();
      check_button();
      check_button2();
      vTaskDelay(10 / portTICK_RATE_MS);
      // semaphore?
      INT8U event2 = get_msg_event(SEB_BUTTON2_EVENT); // read the event buffer for the button2
      switch (event2)
      {
      case BE_SINGLE_PUSH: // if the event is a single push

        num = (num - 1 + 4) % 4;
        // GPIO_PORTF_DATA_R = ~colorArr[num];
        // change number here
        move_LCD(0, 1);
        wr_withdraw_str();

        break;
      case BE_DOUBBLE_PUSH: // if the event is a double push
        if (withdraw_value[num] < getAmount())
        {
          setWithdrawAmount(withdraw_value[num]);
          withdraw_chosen = TRUE;
        }
        else
        {
          clr_LCD();
          vTaskDelay(100 / portTICK_RATE_MS);
          move_LCD(0, 0);
          INT8U message[] = "Cant withdraw:";
          wr_str_LCD(message);
          move_LCD(0, 1);
          wr_withdraw_str();
          vTaskDelay(3000 / portTICK_RATE_MS);
          first_run_withdraw = 1;
        }

        // select current number and continue
        break;
      }

      // semaphore?
      INT8U event = get_msg_event(SEB_BUTTON_EVENT); // read the event buffer for the button
      switch (event)
      {
      case BE_SINGLE_PUSH: // if the event is a single push
        num = (num + 1) % 4;

        move_LCD(0, 1);
        wr_withdraw_str();

        break;
      }
    }
  }
}

/****************************** End Of Module *******************************/
