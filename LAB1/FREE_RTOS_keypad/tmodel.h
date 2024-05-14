/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: tmodel.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Defines the elemtn of the task model..
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 101004  MoH   Module created.
*
*****************************************************************************/

#ifndef _TMODEL_H_
#define _TMODEL_H_



// Shared Event Buffers.
// ---------------------
#define SEB_BUTTON_EVENT 40
#define SEB_BUTTON2_EVENT 41
#define SEB_TO_BUTTON 42 // Time Out
#define SEB_TO_BUTTON2 43 // Time Out

#define TEST_CHANNEL 12

// Software Timers.
// ----------------
#define ST_BUTTON   1
#define ST_BUTTON2   4

#endif /* _TMODEL_H_ */
