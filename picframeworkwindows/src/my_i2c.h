#ifndef __my_i2c_h
#define __my_i2c_h

#include "messages.h"

#define MAXI2CBUF MSGLEN
typedef struct __i2c_comm {
	unsigned char buffer[MAXI2CBUF];
	unsigned char	buflen;
        unsigned char bufind;
	unsigned char	event_count;
	unsigned char	status;
	unsigned char	error_code;
	unsigned char	error_count;
	unsigned char outbuffer[MAXI2CBUF];
	unsigned char outbuflen;
	unsigned char outbufind;
	unsigned char slave_addr;
        unsigned char busBusy;

} i2c_comm;

#define I2C_IDLE 0x5
#define I2C_STARTED 0x6
#define	I2C_RCV_DATA 0x7
#define I2C_SLAVE_SEND 0x8
#define I2C_I2C_MASTER_SEND_ADDRESS_FOR_SEND_DATA 0x9
#define I2C_MASTER_SEND_DATA 0x10
#define I2C_MASTER_RX_ADDR 0x11
#define I2C_MASTER_SEND_ADDRESS_FOR_RECEIVE_DATA 0x12
#define I2C_MASTER_RECEIVE_DATA 0x13
#define I2C_MASTER_DATA_READ 0x14
#define I2C_STOP_IDLE 0x15

#define I2C_ERR_THRESHOLD 1
#define I2C_ERR_OVERRUN 0x4
#define I2C_ERR_NOADDR 0x5
#define I2C_ERR_NODATA 0x6
#define I2C_ERR_MSGTOOLONG 0x7
#define I2C_ERR_MSG_TRUNC 0x8
#define I2C_ERROR_NO_ACK 0x13

void init_i2c(i2c_comm *);
void i2c_int_handler(void);
int i2c_master_handler(void);
void start_i2c_slave_reply(unsigned char,unsigned char *);
void i2c_configure_slave(unsigned char);
void i2c_configure_master();
unsigned char i2c_master_send(unsigned char,unsigned char *, unsigned char slave_addr);
unsigned char i2c_master_recv(unsigned char, unsigned char slave_addr);
void  clearBusBusy();

#endif