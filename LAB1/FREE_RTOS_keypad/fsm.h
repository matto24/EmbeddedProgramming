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
#include "glob_def.h"
#include "semphr.h"


/*****************************    Defines    *******************************/
// Special ASCII characters
// ------------------------

extern INT16U amount;
extern BOOLEAN amount_set;
extern INT16U withdraw_amount;

extern BOOLEAN pin_correct;

typedef enum
{
    ENTER_AMOUNT,
    ENTER_PIN,
    CHOOSE_WITHDRAW_AMOUNT,
    CHOOSE_BANKNOTE,
    WITHDRAWAL
} state;

extern SemaphoreHandle_t xStateMutex;
extern SemaphoreHandle_t xAmountMutex;
extern SemaphoreHandle_t xWithdrawAmountMutex;
extern SemaphoreHandle_t xRotaryCompleteMutex;

/*****************************   Constants   *******************************/



/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/


void set_rotary_complete();

state getState();

INT16U getAmount();
void setAmount(INT16U value);

INT16U getWithdrawAmount();
void setWithdrawAmount(INT16U value);


void init_fsm(void);

void fsm_task(void *pvParameters);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/


/****************************** End Of Module *******************************/
#endif

