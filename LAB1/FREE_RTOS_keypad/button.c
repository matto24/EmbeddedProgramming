/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: button.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "events.h"
#include "button.h"
#include "swtimers.h"
#include "messages.h"
/*****************************    Defines    *******************************/
#define BS_IDLE           0
#define BS_FIRST_PUSH     1
#define BS_FIRST_RELEASE  2
#define BS_SECOND_PUSH    3
#define BS_LONG_PUSH      4

/*****************************   Constants   *******************************/


/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/


void init_buttons(){
    init_swtimers();
}

INT8U button_pushed()
{
    return( !(GPIO_PORTF_DATA_R & 0x10) );                                // SW1 at PF4
}
INT8U button2_pushed()
{

  return( !(GPIO_PORTF_DATA_R & 0x01) );                                // SW2 at PF0
}

void check_button2()

/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  swt_ctrl();
  static INT8U  button2_state = BS_IDLE;
         INT8U  time_out;

  time_out = get_msg_event( SEB_TO_BUTTON2 );

  switch( button2_state )
  {
    case BS_IDLE:
        if( button2_pushed( ))                                          // if button pushed
        {
            button2_state = BS_FIRST_PUSH;                               // we go from the idle state 
      	}

        break;
    case BS_FIRST_PUSH:
        if( !button2_pushed() )                                         // if button released before the timer runs out it was a normal push
        {
            button2_state = BS_FIRST_RELEASE;
            start_swtimer( ST_BUTTON2, SEB_TO_BUTTON2, MILLISEC(50) );   // we set a new timer to see if the button is pressed again shortly after (double push)
        }

        break;
    case BS_FIRST_RELEASE:
        if( time_out )                                                  // if the timer runs out without the button being pushed again it was a single push
        {
            button2_state = BS_IDLE;
            put_msg_event( SEB_BUTTON2_EVENT, BE_SINGLE_PUSH );
        }
        else
        {
        if( button2_pushed() )                                          // if button is pressed again before the timer runs out it was a double push
        {
            button2_state = BS_SECOND_PUSH;
        }
        }
        break;
    case BS_SECOND_PUSH:
            button2_state = BS_IDLE;
            put_msg_event( SEB_BUTTON2_EVENT, BE_DOUBBLE_PUSH );
        break;
    default:
      break;
  }
}

void check_button()

/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  swt_ctrl();
  static INT8U  button_state = BS_IDLE;

  switch( button_state )
  {
    case BS_IDLE:
        if( button_pushed( ))                                           // if button pushed
        {
            button_state = BS_FIRST_PUSH;                               // we go from the idle state to first push
        }

        break;
    case BS_FIRST_PUSH:
        
        if( !button_pushed() )                                          // if button released before the timer runs out it was a normal push
        {
            button_state = BS_FIRST_RELEASE;
        }
        break;
        
    case BS_FIRST_RELEASE:
            button_state = BS_IDLE;
            put_msg_event( SEB_BUTTON_EVENT, BE_SINGLE_PUSH );
    default:
      break;
  }
}

/****************************** End Of Module *******************************/












