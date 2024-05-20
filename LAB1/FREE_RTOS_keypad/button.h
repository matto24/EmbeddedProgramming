/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: button.h
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
* 090215  MoH   Module created.
*
*****************************************************************************/

#ifndef BUTTON_H_
#define BUTTON_H_

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

// Shared Event Buffers.
// ---------------------
#define SEB_BUTTON_EVENT 40
#define SEB_BUTTON2_EVENT 41
#define SEB_TO_BUTTON2 43 // Time Out


// Software Timers.
// ----------------
#define ST_BUTTON2   4
/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

void init_buttons();

INT8U button_pushed();

void check_button();
/*****************************************************************************
*   Input    : -
*   Output   : Button Event
*   Function : Test function
******************************************************************************/

void check_button2();
/*****************************************************************************
*   Input    : -
*   Output   : Button2 Event
*   Function : Test function
******************************************************************************/


/****************************** End Of Module *******************************/
#endif /*BUTTON_H_*/
