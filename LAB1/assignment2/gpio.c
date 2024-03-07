/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: gpio.c
 *
 * PROJECT....: EMP
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 150215  MoH   Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "gpio.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_gpio(void)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : The super loop.
 ******************************************************************************/
{

  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // Enable clock to GPIO Port F
  while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0)
  {
  }; // Wait for clock to stabilize

  // Set the direction as output (PF1, PF2 and PF3).
  GPIO_PORTF_DIR_R = 0x0E;

  // Enable the GPIO pins for digital function (PF1, PF2, PF3, PF4).
  GPIO_PORTF_DEN_R = 0x1E;

  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R = 0x10;
}
/****************************** End Of Module *******************************/
