// This is where the "user" interrupts handlers should go
// The *must* be declared in "user_interrupts.h"

#include "maindefs.h"
#ifndef __XC8
#include <timers.h>
#else
#include <plib/timers.h>
#endif
#include "user_interrupts.h"
#include "messages.h"
#include "my_i2c.h"

unsigned char msgbuffer[MSGLEN + 1];
unsigned char msgtype;

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer0 interrupt

void timer0_int_handler() {
    
    clearBusBusy();
}

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer1 interrupt
int message3 = 0;
int first_time = 1;
char data1[3];
char count = 0;
void timer1_int_handler() {
    //unsigned int result;

   

    //message3++;

    //if (message3 == 10)
    //{
    #if ENABLE_UART_TX
       // message3 = 0;
        //PIE1bits.TXIE = 1;
    #endif
    #if ENABLE_I2C
    //if(first_time == 1)
    //{
        //unsigned char data1[4];
     //   data1[0] = 0x32; //laser type = 0x32 .. infrared = 0x42
       // data1[1] = 0x55+count;
       // data1[2] = 0x56+count;
      //  count++;

        //if (count == 2)
        //{
       // i2c_master_recv(7, SENSOR_PIC_ADDRESS);
       // i2c_master_send(3,data1, MOTOR_PIC_ADDRESS);
       ToMainLow_sendmsg(0,MSGT_I2C_MASTER_RECV, (void *)data1);
      //  ToMainHigh_sendmsg(3,MSGT_I2C_MASTER_SEND, data1);
       
     //   count =0;
       // }

        
        

           // first_time = 0;
        //}
    #endif
    //}


    //WriteTimer1(65086);

  
}

int ADCValue = 0;
//int x[300];
int counter2 = 0;
char testbuffer[50];



void adc_int_handler() {

    ADCValue = ReadADC();





//    x[counter2] = ADCValue;

    if(counter2 < 299)
    counter2++;
    else
    {
        counter2 = 0;
        CloseTimer1();
//        ToMainHigh_sendmsg()
    }

}