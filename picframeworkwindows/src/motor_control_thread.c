#include "maindefs.h"
#include <stdio.h>
#include "motor_control_thread.h"
#include "messages.h"

// This is a "logical" thread that processes messages from the UART
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

char motor_direction;
char time;
char magnitude;
char testbuffer[50];
int counter = 0;

int motor_control_thread(motor_control_thread_struct *mptr, int msgtype, int length, unsigned char *msgbuffer) {
    
    #if ENABLE_GPIO
        MAIN_THREAD = 0;
        MOTOR_CONTROL_THREAD = 1;
    #endif
    //int i = 0;
       // motor_direction = msgbuffer[0];
      //  time = msgbuffer[1];
       // magnitude = msgbuffer[2];

    /*    testbuffer[counter] = msgbuffer[0];
        testbuffer[counter+1] = msgbuffer[1];
        testbuffer[counter+2] = msgbuffer[2];
        counter = counter + 3;
        if (counter == 45)
            counter = 0;*/

       ToMainHigh_sendmsg(3,MSGT_I2C_MASTER_SEND, msgbuffer);
    #if ENABLE_GPIO
        MAIN_THREAD = 1;
        MOTOR_CONTROL_THREAD = 0;
    #endif





    
}
   

