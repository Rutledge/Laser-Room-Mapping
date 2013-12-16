/* 
 * File:   my_UART.h
 * Author: Rutledge
 *
 * Created on November 24, 2013, 8:03 PM
 */

#ifndef MY_UART_H
#define	MY_UART_H

#define MAXUARTBUF 125

typedef struct {
    char buffer_in[MAXUARTBUF];
    char buffer_in_read_ind;
    char buffer_in_write_ind;
    char buffer_in_len;
    char buffer_in_len_tmp;

    char buffer_out[MAXUARTBUF];
    char buffer_out_ind;
    char buffer_out_len;
} UART_DATA;

void UART1_Init(UART_DATA *data);
void UART2_Init(UART_DATA *data);

void UART1_Write(char* c, int length);
void UART2_Write(char* c, int length);

int UART1_Read(char* c);
int UART2_Read(char* c);
#endif	/* MY_UART_H */

