#include "maindefs.h"
#include <stdio.h>
#include "uart_thread.h"

// This is a "logical" thread that processes messages from the UART
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

char buffertest;

int uart_lthread(uart_thread_struct *uptr, int msgtype, int length, unsigned char *msgbuffer) {
    if (msgtype == MSGT_OVERRUN) {
    }
    else if (msgtype == MSGT_SEND_SENSOR_DATA_TO_ARM)
    {
        
            for(char i = 0; i < length; i++)
            {
                buffertest = msgbuffer[i];
                while(BusyUSART() ) ;
                WriteUSART(msgbuffer[i]);

            }
            while(BusyUSART() ) ;
            WriteUSART(STOP_BYTE_CODE);
        
    }
}

