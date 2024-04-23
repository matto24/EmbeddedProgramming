/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: rtc.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090926  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "emp_type.h"
#include "tmodel.h"
#include "lcd.h"
#include "file.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U sec, min, hour;



/*****************************   Functions   *******************************/
INT8U get_hour()
{
  return( hour );
}
INT8U get_min()
{
  return( min );
}
INT8U get_sec()
{
  return( sec );
}

void set_hour( INT8U new_hour )
{
  hour = new_hour;
}
void set_min( INT8U new_min )
{
  min = new_min;
}
void set_sec( INT8U new_sec )
{
  sec = new_sec;
}

void rtc_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Disable global interrupt
******************************************************************************/
{
  switch( my_state )
  {
    case 0:
      set_state( 1 );
      wait( 100 );
      break;
    case 1:
      sec++;
      if( sec >= 60 )
      {
        min++;
        if( min >= 60 )
        {
       	  hour++;
          if( hour >= 24 )
            hour = 0;
          min = 0;
        }
        sec = 0;
      }
      wait( 200 );
      signal( SEM_RTC_UPDATED );
      break;
  }
}

void display_rtc_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
{
  if( sec & 0x01 )
    gfprintf( COM2, "%c%c%02d %02d %02d", 0x1B, 0x84, hour, min, sec );
  else
    gfprintf( COM2, "%c%c%02d:%02d:%02d", 0x1B, 0x84, hour, min, sec );
  wait_sem( SEM_RTC_UPDATED, WAIT_FOREVER );
}



/****************************** End Of Module *******************************/












