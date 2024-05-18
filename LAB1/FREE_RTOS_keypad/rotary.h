/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: rotary.h
*
* PROJECT....: Final Assignment
*
* DESCRIPTION: Rotary encoder stuff.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090215  MoH   Module created.
*
*****************************************************************************/

#ifndef ROTARY_
#define ROTARY_

/***************************** Include files *******************************/
#include "FreeRTOS.h"
#include "semphr.h"

/*****************************    Defines    *******************************/

/****************************Shared resources*******************************/
extern volatile INT8U rotary_pressed_flag;

//extern volatile INT8U rotaryValue;
extern SemaphoreHandle_t xRotaryValueMutex;

extern SemaphoreHandle_t xRotaryDirectionSemaphore;


extern SemaphoreHandle_t xRotaryPressSemaphore;


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void wr_banknote_str(INT8U);


void init_rotary(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function :
******************************************************************************/


void GPIOA_Handler(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function :
******************************************************************************/




INT8U getRotaryValue(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function :
******************************************************************************/


void rotary_task(void *pvParameters);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function :
******************************************************************************/

void rotary_pressed_task(void *pvParameters);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function :
******************************************************************************/



/****************************** End Of Module *******************************/
#endif /*ROTARY_*/
