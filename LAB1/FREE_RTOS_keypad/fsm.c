#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "fsm.h"


typedef enum 
{
    ENTER_AMOUNT,
    ENTER_PIN,
    CHOOSE_WITHDRAW_AMOUNT,
    CHOOSE_BANKNOTE,
    WITHDRAWAL
} state;


//boolean isValidAmount

// void suspendAllTasks
INT16U amount = 0;
BOOLEAN isFull = 0;

void fsm_task(void*pvParameters)    // task manager
{
    state current_state = ENTER_AMOUNT;

    //suspendAllTasks
    while(1)
    {
        switch (current_state)
        {
            case ENTER_AMOUNT:
            // vTaskSuspend;
            // vTaskResume;
            
            if(isFull){
                current_state = ENTER_PIN;
            }

                //if ( sw1 )
                //    current_state = ENTER_PIN ?????

            break;

            case ENTER_PIN:
                GPIO_PORTF_DATA_R |= 0x04;
            break;

            case CHOOSE_WITHDRAW_AMOUNT:
            break;
            
            case CHOOSE_BANKNOTE:
            break;
            
            case WITHDRAWAL:
            break;
            
            default:
            break;
        }
    }
}
