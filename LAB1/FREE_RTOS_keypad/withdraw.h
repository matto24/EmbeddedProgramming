/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: withdraw.h
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
* 100408  KHA    Module created.
*
*****************************************************************************/

#ifndef WITHDRAW_H_
    #define WITHDRAW_H_

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U done;
//INT16U withdraw_amount;


/*****************************   Functions   *******************************/



void withdraw_task(void *pvParameters);


/****************************** End Of Module *******************************/
#endif /* WITHDRAW_H_ */
