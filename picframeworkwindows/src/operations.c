#include <xc.h>
#include "operations.h"
#include "defines.h"
#include "my_UART.h"

void wait_onesec()
{
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);

    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);

    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);

    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);

    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
}

void move_motor()
{
    LATAbits.LATA0 = 1;		//Set PIN for Stepper motor high
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    
    LATAbits.LATA0 = 0;		//Set PIN for Stepper motor low
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
}

void laser_init()
{
    char init = 'U';
    char resp;
    UART2_Write(&init, 1);
    while (1) {
	if (UART2_Read(&resp) == 0) {	// Check for response
	    if (resp == ':') {
		break;
            }
	}
    }
}

unsigned long laser_read()
{
    char read[] = "B";
    UART2_Write(&read, 1);
    unsigned long temp = 0;
    
    while(1){
        if(UART2_Read(&read) == 0)
        {
            temp |= (read[0] << 24);
            break;
        }
    }
    while(1){
        if(UART2_Read(&read) == 0)
        {
            temp |= (read[0] << 16);
            break;
        }
    }
    while(1){
        if(UART2_Read(&read) == 0)
        {
            temp |= (read[0] << 8);
            break;
        }
    }
    while(1){
        if(UART2_Read(&read) == 0)
        {
            temp |= (read[0]);
            break;
        }
    }
    while(1){
        if(UART2_Read(&read) == 0)
            break;
    }
    while(1){
        if(UART2_Read(&read) == 0)
            break;
    }
    return temp;

}

void wifi_write(unsigned long total)
{
    char write[4];
    write[3] = total >> 24;
    write[2] = total >> 16;
    write[1] = total >> 8;
    write[0] = total;

    UART1_Write(&write, 4);
    char newline = '\n';
    UART1_Write(newline, 1);

}