/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: key.h
*
* PROJECT....: EMP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150321  MoH   Module created.
*
*****************************************************************************/

#ifndef _KEY_H
  #define _KEY_H




void init_keypad(void);
BOOLEAN get_keyboard( INT8U* );
void keypad_task(void *pvParameters);
void enter_amount_task(void *pvParameters);
void enter_pin_task(void *pvParameters);

#endif
