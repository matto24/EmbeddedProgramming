#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "gpio.h"
#include "systick.h"
#include "tmodel.h"
#include "systick.h"
#include "rtc.h"
#include "lcd.h"
#include "ui.h"
#include "file.h"
#include "key.h"



int main(void)
/*****************************************************************************
*   Input    : NONE
*   Output   : Returns 0
*   Function : Init hardware and then loop forever
******************************************************************************/
{
  init_gpio();

  init_files();
  init_rtcs();

  open_queue( Q_LCD );
  open_queue( Q_KEY );

  start_task( TASK_RTC, rtc_task );
  start_task( TASK_DISPLAY_RTC, display_rtc_task );
  start_task( TASK_LCD, lcd_task );
  start_task( TASK_KEY, key_task );
  start_task( TASK_UI_KEY, ui_key_task );

  schedule();
  return( 0 );
}
