#include "lcd.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include <stdint.h>


void delayMs(int n) {
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 80000; j++) {}  /* do nothing for 1 ms */
}


void ClearDisplay(){
    //Enable read/write
    //Set PD2=0 (RS = 0)
    //Set PD3=1 (E = 1)
    GPIO_PORTD_DATA_R &= 0x0F;
}

void SetCursor(int row, int col){
    int row_offsets[] = {0x00, 0x40};  // Address offsets for each row
    int address = col + row_offsets[row]; // Compute address
    // Set DDRAM address command
    GPIO_PORTC_DATA_R = 0x80 | address;
    // RS low for command mode
    GPIO_PORTD_DATA_R &= ~0x04;
    // EN high to enable command
    GPIO_PORTD_DATA_R |= 0x08;
    // Add delay for the command execution (consult LCD datasheet)
    delayMs(1);


    // Disable command
    GPIO_PORTD_DATA_R &= ~0x08;
    // Add further delay (consult LCD datasheet)
}


void writeH(){
    // Set RS to 0
    //Step 2 - function set
    GPIO_PORTD_DATA_R &= ~0x04; // RS = 0
    GPIO_PORTC_DATA_R |= 0x20;

    //Step 3 - function set
    GPIO_PORTC_DATA_R &= 0x00;

    //Step 4 - display on/off control
    GPIO_PORTC_DATA_R |= 0xD0;

    //Step 5 - entry mode set
    // 5.0
    GPIO_PORTD_DATA_R  &= ~0x04; // RS = 0
    GPIO_PORTC_DATA_R &= ~0xF0;

    // 5.1
    GPIO_PORTD_DATA_R  &= ~0x04; // RS = 0
    GPIO_PORTC_DATA_R &= ~0x60;


//6.0
    GPIO_PORTD_DATA_R  |= 0x04; // RS = 1
    GPIO_PORTC_DATA_R = 0x40; // PC6 = 1


    // 6.1
    GPIO_PORTD_DATA_R  |= 0x04; // RS = 1
    GPIO_PORTD_DATA_R = 0x80; // PC7 = 1


}
void WriteCharacter(char c){

    // Set RS to 1 for data register
    GPIO_PORTD_DATA_R |= 0x04; // RS = 1

    // Set the relevant bits
    GPIO_PORTD_DATA_R = (GPIO_PORTD_DATA_R & 0x0F) | (c & 0xF0);

    // Togg le the enable pin to write the data
    GPIO_PORTD_DATA_R |= 0x08; // E = 1
    GPIO_PORTD_DATA_R &= ~0x08; // E = 0

    // Write the lower 4 bits
    GPIO_PORTD_DATA_R = (GPIO_PORTD_DATA_R & 0x0F) | ((c & 0x0F) << 4);

    // Toggle the enable pin again
    GPIO_PORTD_DATA_R |= 0x08; // E = 1
    GPIO_PORTD_DATA_R &= ~0x08; // E = 0
}




void PrintString(char* str){
while(*str){
        WriteCharacter(*str++);
    }
}
