#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "fsm.h"
#include "lcd.h"
#include "key.h"
#include "withdraw.h"

state current_state = ENTER_AMOUNT;

SemaphoreHandle_t xStateMutex;
SemaphoreHandle_t xAmountMutex;
SemaphoreHandle_t xWithdrawAmountMutex;
SemaphoreHandle_t xRotaryCompleteMutex;
extern QueueHandle_t xQueueLCD;

// void suspendAllTasks
INT16U amount = 0;
INT16U withdraw_amount = 0;

BOOLEAN amount_set = 0;
BOOLEAN pin_correct = 0;
INT8U rotary_complete = 0;
INT8U rotary_test = 0;

state getState()
{
    state outp;
    if (xSemaphoreTake(xStateMutex, portMAX_DELAY) == pdTRUE)
    {
        outp = current_state;
        xSemaphoreGive(xStateMutex);
    }
    return outp;
}

void setState(state value)
{
    if (xSemaphoreTake(xStateMutex, portMAX_DELAY) == pdTRUE)
    {
        current_state = value;
        xSemaphoreGive(xStateMutex);
    }
}

INT16U getAmount()
{
    INT16U value;
    if (xSemaphoreTake(xAmountMutex, portMAX_DELAY) == pdTRUE)
    {

        value = amount;
        xSemaphoreGive(xAmountMutex);
    }
    return value;
}

void setAmount(INT16U value)
{
    if (xSemaphoreTake(xAmountMutex, portMAX_DELAY) == pdTRUE)
    {

        amount = value;
        xSemaphoreGive(xAmountMutex);
    }
}

INT16U getWithdrawAmount()
{
    INT16U value;
    if (xSemaphoreTake(xWithdrawAmountMutex, portMAX_DELAY) == pdTRUE)
    {
        value = amount;
        xSemaphoreGive(xWithdrawAmountMutex);
    }
    return value;
}

void setWithdrawAmount(INT16U value)
{
    if (xSemaphoreTake(xWithdrawAmountMutex, portMAX_DELAY) == pdTRUE)
    {

        withdraw_amount = value;
        xSemaphoreGive(xWithdrawAmountMutex);
    }
}

void set_rotary_complete()
{
    if (xSemaphoreTake(xRotaryCompleteMutex, portMAX_DELAY) == pdTRUE)
    {
        rotary_complete = 1;
        xSemaphoreGive(xRotaryCompleteMutex);
    }
}

INT8U get_rotary_complete()
{
    INT8U outp;
    if (xSemaphoreTake(xRotaryCompleteMutex, portMAX_DELAY) == pdTRUE)
    {
        outp = rotary_complete;
        xSemaphoreGive(xRotaryCompleteMutex);
    }
    return outp;
}

void init_fsm(void)
{
    xStateMutex = xSemaphoreCreateMutex();
    xAmountMutex = xSemaphoreCreateMutex();
    xWithdrawAmountMutex = xSemaphoreCreateMutex();
    xRotaryCompleteMutex = xSemaphoreCreateMutex();
}

void fsm_task(void *pvParameters) // task manager
{
    // vTaskSuspend(withdraw_task);

    while (1)
    {
        switch (current_state)
        {
        case ENTER_AMOUNT:

            if (amount_set)
            {
                setState(ENTER_PIN);
            }

            break;

        case ENTER_PIN:
            // clr_LCD();

            if (pin_correct)
            {
                setState(CHOOSE_WITHDRAW_AMOUNT);
            }

            break;

        case CHOOSE_WITHDRAW_AMOUNT:
            if (withdraw_chosen)
            {
                setState(CHOOSE_BANKNOTE);
            }
            break;

        case CHOOSE_BANKNOTE:
            // rotary_test = rotary_complete + 48;
            // xQueueSend(xQueueLCD, &rotary_test, portMAX_DELAY);
            vTaskDelay(50 / portTICK_RATE_MS);
            if (get_rotary_complete())
            {
                setState(WITHDRAWAL);
            }

            break;

        case WITHDRAWAL:
            // clr_LCD();
            break;

        default:
            break;
        }
    }
}
