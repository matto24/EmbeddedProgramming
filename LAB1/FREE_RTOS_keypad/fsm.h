/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: leds.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef _FSM_H
  #define _FSM_H

/***************************** Include files *******************************/
#include "FreeRTOS.h"
#include "queue.h"


/*****************************    Defines    *******************************/
// Special ASCII characters
// ------------------------

extern INT16U amount;
extern BOOLEAN isFull;

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/


void fsm_task(void *pvParameters);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/


/****************************** End Of Module *******************************/
#endif

