/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: messages.h
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

#ifndef _ECP_H
  #define _ECP_H

/***************************** Include files *******************************/
#include "emp_type.h"
/*****************************    Defines    *******************************/
  
/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
INT8U get_msg_event( INT8U );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/
void put_msg_event( INT8U, INT8U );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/
INT8U get_msg_state( INT8U );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/
void put_msg_state( INT8U, INT8U );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

/****************************** End Of Module *******************************/
#endif
