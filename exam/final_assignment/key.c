/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: key.c
 *
 * PROJECT....: EMP
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 150321  MoH   Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "lcd.h"
#include "fsm.h"
#include "button.h"

/*****************************    Queues    **********************************/

QueueHandle_t xQueueKey;

/*****************************    Variables   **********************************/
INT8U my_state = 0;
INT8U i = 0;
INT8U j = 0;
INT8U first_run = 1;
INT8U first_run_pin = 1;
INT16U pin_value;
INT16U tmp_amount = 0;

/*****************************    Defines     ***********************************/

// #define QUEUE_LENGTH 128
#define ASCII_TO_INT 48

extern QueueHandle_t xQueueLCD;

/*****************************   Functions    **********************************/

void init_keypad(void)
{
  xQueueKey = xQueueCreate(QUEUE_LENGTH, sizeof(INT8U));
}

INT8U row(INT8U y)
{
  INT8U result = 0;

  switch (y)
  {
  case 0x01:
    result = 1;
    break;
  case 0x02:
    result = 2;
    break;
  case 0x04:
    result = 3;
    break;
  case 0x08:
    result = 4;
    break;
  }
  return (result);
}

INT8U key_catch(x, y)
INT8U x, y;
{
  const INT8U matrix[3][4] = {{'*', '7', '4', '1'},
                              {'0', '8', '5', '2'},
                              {'#', '9', '6', '3'}};

  return (matrix[x - 1][y - 1]);
}

BOOLEAN get_keyboard(INT8U *pch)
{
  if (xQueueReceive(xQueueKey, pch, 0) == pdPASS)
  {
    // if (*pch == '1')
    // {
    //   GPIO_PORTF_DATA_R ^= 0x04;
    // }
    // // turn on LED
    // GPIO_PORTF_DATA_R ^= 0x02;

    return 1;
  }
  else
  {
    // turn on LED
    // GPIO_PORTF_DATA_R ^= 0x08;
    return 0;
  }
}

BOOLEAN check_column(INT8U x)
{
  INT8U y = GPIO_PORTE_DATA_R & 0x0F; // Save the values of the 4 bits for the rows
  if (y)                              // If one of them are set...
  {                                   // ...we first find the row number with the function row()
    INT8U ch = key_catch(x, row(y));  // Now that we have the row and column we look up the corresponding character using the function key_catch
    // if (ch == '#'){
    //   GPIO_PORTF_DATA_R ^= 0x04;
    // }

    if (xQueueSend(xQueueKey, &ch, portMAX_DELAY) == pdPASS)
    { // Put the character in a queue so it can be used by another task
      // turn on LED
      // GPIO_PORTF_DATA_R ^= 0x02;
    }
    return 1;
  }
  return 0;
}

void keypad_task(void *pvParameters)
{
  while (1)
  {

    // case: ENTER_AMOUNT
    // case: ENTER_PIN

    switch (my_state)
    {
    case 0:
      GPIO_PORTA_DATA_R &= 0xE3; // Clear the 3 bits for the columns
      GPIO_PORTA_DATA_R |= 0x10; // Set the bit for column 1
      if (check_column(1))       // Check all the rows for column 1, using the function check_column
      {                          // If a button press is registered we go to next state so the press is only registered once
        my_state = 1;
        break;
      }
      GPIO_PORTA_DATA_R &= 0xE3; // Repeat the above for the two other columns
      GPIO_PORTA_DATA_R |= 0x08; // Set bit for column 2
      if (check_column(2))
      {
        my_state = 1;
        break;
      }
      GPIO_PORTA_DATA_R &= 0xE3;
      GPIO_PORTA_DATA_R |= 0x04;
      if (check_column(3))
      {
        my_state = 1;
        break;
      }
      break;
    case 1:
      if (!(GPIO_PORTE_DATA_R & 0x0F)) // We stay here until the button is released so a button press is not counted more than once
      {
        my_state = 0;

        vTaskDelay(50 / portTICK_RATE_MS);

        // if (get_keyboard(&key_press))
        // {
        //   xQueueSend(xQueueLCD, &key_press, portMAX_DELAY);
        // }
      }
      break;
    }
  }
}

void enter_amount_task(void *pvParameters)
{

  while (1)
  {

    if (getState() == ENTER_AMOUNT)
    {

      if (first_run)
      {
        clr_LCD();
        INT8U enter_amount[] = "Enter amount:";
        wr_str_LCD(enter_amount);
        move_LCD(0, 1);
        vTaskDelay(100 / portTICK_RATE_MS);
        first_run = 0;
      }

      INT8U key_press = 0;

      if (get_keyboard(&key_press))
      {

        if (key_press == '#' || key_press == '*')
        {
        }
        else
        {
          if (i < 4)
          {
            xQueueSend(xQueueLCD, &key_press, portMAX_DELAY);
            switch (i)
            {
            case 0:
              tmp_amount = (key_press - ASCII_TO_INT) * 1000;
              break;
            case 1:
              tmp_amount = tmp_amount + (key_press - ASCII_TO_INT) * 100;
              break;
            case 2:
              tmp_amount = tmp_amount + (key_press - ASCII_TO_INT) * 10;
              break;
            case 3:
              tmp_amount = tmp_amount + (key_press - ASCII_TO_INT);
              setAmount(tmp_amount);
              break;
            default:
              break;
            }
          }
          i++;
        }
      }

      if (button_pushed() && i > 3)
      {
          amount_set = 1;
      }
    }
  }
}

void enter_pin_task(void *pvParameters)
{

  while (1)
  {
    if (getState() == ENTER_PIN)
    {

      if (first_run_pin)
      {
        j = 0;
        // xQueueReset(xQueueKey);
        pin_value = 0;
        clr_LCD();
        vTaskDelay(50 / portTICK_RATE_MS);
        INT8U enterPin[] = "Enter Pin:";
        wr_str_LCD(enterPin);
        move_LCD(0, 1);
        vTaskDelay(100 / portTICK_RATE_MS);
        first_run_pin = 0;
      }

      INT8U pin_key = 0;

      if (get_keyboard(&pin_key))
      {

        if (pin_key == '#' || pin_key == '*')
        {
        }
        else
        {
          if (j < 4)
          {
            xQueueSend(xQueueLCD, &pin_key, portMAX_DELAY);
          }

          
          if (j == 3)
          {

            if (!(pin_value % 8))
            {
              vTaskDelay(200 / portTICK_RATE_MS);
              pin_correct = 1;
            }
            else
            {
              vTaskDelay(500 / portTICK_RATE_MS);
              first_run_pin = 1;
              j=0;
              //clear LCD queue
              xQueueReset(xQueueLCD);
            }
          }

          switch (j)
          {
          case 0:
            pin_value = (pin_key - ASCII_TO_INT) * 1000;
            break;
          case 1:
            pin_value = pin_value + (pin_key - ASCII_TO_INT) * 100;
            break;
          case 2:
            pin_value = pin_value + (pin_key - ASCII_TO_INT) * 10;
            break;
          case 3:
            pin_value = pin_value + (pin_key - ASCII_TO_INT);

            break;
          default:
            break;
          }
          j++;
        }
      }
    }
  }
}