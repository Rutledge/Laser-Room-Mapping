#include <xc.h>
#include "my_UART.h"
#include "defines.h"



static UART_DATA *uart_1_data_p;

void UART1_Init(UART_DATA *data) {
    uart_1_data_p = data;

    UART1_TX_TRIS = 0; // Tx pin set to output
    UART1_RX_TRIS = 1; // Rx pin set to input

    BAUDCON1bits.BRG16 = 0; // 8-bit baud rate generator
    SPBRG1 = 77; // Set to 25 for UART speed to 115200 baud, 77 for 38400
    TXSTA1bits.BRGH = 1; // High speed mode
    TXSTA1bits.SYNC = 0; // Async mode
    RCSTA1bits.SPEN = 1; // Serial port enable
    TXSTA1bits.TX9 = 0; // 8 bit transmission
    RCSTA1bits.RX9 = 0; // 8 bit reception
    RCSTA1bits.CREN = 1; // Continuous receive mode

    TXSTA1bits.TXEN = 1; // Enable transmission


    // Initialize the buffer that holds UART messages
    uart_1_data_p->buffer_in_read_ind = 0;
    uart_1_data_p->buffer_in_write_ind = 0;
    uart_1_data_p->buffer_in_len = 0;
    uart_1_data_p->buffer_in_len_tmp = 0;
}

static UART_DATA *uart_2_data_p;

void UART2_Init(UART_DATA *data) {
    uart_2_data_p = data;

    UART2_RX_TRIS=1;
    UART2_TX_TRIS=0; // Set up UART 2

    BAUDCON2bits.BRG16 = 0; // 8-bit baud rate generator
    SPBRG2 = 77; // Set to 25 for UART speed to 115200 baud, 77 for 38400
    TXSTA2bits.BRGH = 1; // High speed mode
    TXSTA2bits.SYNC = 0; // Async mode
    RCSTA2bits.SPEN = 1; // Serial port enable
    TXSTA2bits.TX9 = 0; // 8 bit transmission
    RCSTA2bits.RX9 = 0; // 8 bit reception
    RCSTA2bits.CREN = 1; // Continuous receive mode

    TXSTA2bits.TXEN = 1; // Enable transmission


    // Initialize the buffer that holds UART messages
    uart_2_data_p->buffer_in_read_ind = 0;
    uart_2_data_p->buffer_in_write_ind = 0;
    uart_2_data_p->buffer_in_len = 0;
    uart_2_data_p->buffer_in_len_tmp = 0;
}

void UART1_Write(char* c, int length){
    for (int i=length-1; i>=0; i--){
    TXREG1 = c[i];
    Nop();
    while (!PIR1bits.TX1IF);
    }
}

void UART2_Write(char* c, int length){
    for (int i=0; i<length; i++){
    TXREG2 = c[i];
    Nop();
    while (!PIR3bits.TX2IF);
    }
}


int UART1_Read(char *c){// This function reads in values from the UART recieve bus and returns a reference to the location of the character
    if (PIR1bits.RC1IF) { // Check if incoming data flag is high
        *c = RCREG1;  //If flag is high read that character on recieve register
        return 0;
    }
        else
            return -1;
}

int UART2_Read(char *c){// This function reads in values from the UART recieve bus and returns a reference to the location of the character
    if (PIR3bits.RC2IF) { // Check if incoming data flag is high
        *c = RCREG2;  //If flag is high read that character on recieve register
        return 0;
    }
    else
        return -1;
}