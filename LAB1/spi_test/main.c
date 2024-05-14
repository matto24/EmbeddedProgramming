/* Example code to transmit data with SPI1 module of TM4C123 */
/* Transmits character A and B with a delay of one second */
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

/* function prototype of SPI and Delay */
void SPI1_init(void);
void SPI1_Write(unsigned int data);
void Delay_ms(int time_ms);

/* Main routine of code */
int main(void)
{
    unsigned int val2 = 0x01;
    unsigned int val1 = 0;
    SPI1_init();


    //SPI1_Write(0);
    //Delay_ms(1000);
    while(1)
        {

        //if ((GPIO_PORTF_DATA_R & (1 << 4)) == 0) {


                SPI1_Write(val2);
                //val2++;
                Delay_ms(300);

            //val2++;
        //}
            //Delay_ms(1000);


            //SPI1_Write(val1);
            //Delay_ms(1000);
    //val2++;


        }
}

void SPI1_Write(unsigned int data)
{
    GPIO_PORTF_DATA_R &= ~(1<<2);       /* Make PF2 Selection line (SS) low */
    while((SSI1_SR_R & 2) == 0); /* wait untill Tx FIFO is not full */
    SSI1_DR_R = data;            /* transmit byte over SSI1Tx line */
    while(SSI1_SR_R & 0x10);     /* wait until transmit complete */
    GPIO_PORTF_DATA_R |= 0x04;        /* keep selection line (PF2) high in idle condition */
}

void SPI1_Read(){

}

void SPI1_init(void)
{
    /* Enable clock to SPI1, GPIOD and GPIOF */

    SYSCTL_RCGCSSI_R |= (1<<1);   /*set clock enabling bit for SPI1 */
    SYSCTL_RCGCGPIO_R |= (1<<3); /* enable clock to GPIOD for SPI1 */
    SYSCTL_RCGCGPIO_R |= (1<<5); /* enable clock to GPIOF for slave select */

    /*Initialize PD3 and PD0 for SPI1 alternate function*/

    GPIO_PORTD_AMSEL_R &= ~0x09;      /* disable analog functionality RD0 and RD3 */
    GPIO_PORTD_DEN_R |= 0x09;         /* Set RD0 and RD3 as digital pin */
    GPIO_PORTD_AFSEL_R |= 0x09;       /* enable alternate function of RD0 and RD3*/
    GPIO_PORTD_PCTL_R &= ~0x0000F00F; /* assign RD0 and RD3 pins to SPI1 */
    GPIO_PORTD_PCTL_R |= 0x00002002;  /* assign RD0 and RD3 pins to SPI1  */

    /* Initialize PF2 as a digital output as a slave select pin */

    GPIO_PORTF_DEN_R |= (1<<2);         /* set PF2 pin digital */
    GPIO_PORTF_DIR_R |= (1<<2);         /* set PF2 pin output */
    GPIO_PORTF_DATA_R |= (1<<2);        /* keep SS idle high */

    /* Select SPI1 as a Master, POL = 0, PHA = 0, clock = 4 MHz, 8 bit data */

    SSI1_CR1_R = 0;          /* disable SPI1 and configure it as a Master */
    SSI1_CC_R = 0;           /* Enable System clock Option */
    SSI1_CPSR_R = 0;         /* Select prescaler value of 4 .i.e 16MHz/4 = 4MHz */
    SSI1_CR0_R  = 0x0017;     /* 8-bit format og TI frame format
    SPI mode 2, 4 bit data */
    SSI1_CR1_R  |= 2;         /* enable SPI1 */

    // Enable the GPIO pins for digital function (PF4)
      //GPIO_PORTF_DEN_R |= (1 << 4);

      // Enable internal pull-up (PF4).
      //GPIO_PORTF_PUR_R |= (1 << 4);
}

/* This function generates delay in ms */
/* calculations are based on 16MHz system clock frequency */

void Delay_ms(int time_ms)
{
    int i, j;
    for(i = 0 ; i < time_ms; i++)
        for(j = 0; j < 3180; j++)
            {}  /* excute NOP for 1ms */
}
