#ifndef __maindefs
#define __maindefs

#ifdef __XC8
#include <xc.h>
#ifdef _18F45J10
#define __USE18F45J10 1
#else
#ifdef _18F2680
#define __USE18F2680 1
#endif
#endif
#else
#ifdef __18F45J10
#define __USE18F45J10 1
#else
#ifdef __18F2680
#define __USE18F2680 1
#endif
#endif
#include <p18cxxx.h>
#endif


#define ENABLE_UART_TX 1
#define ENABLE_UART_RX 1
#define ENABLE_I2C 1
#define ENABLE_TIMER1 1
#define ENABLE_GPIO 1

// Message type definitions
#define MSGT_TIMER0 10
#define MSGT_TIMER1 11
#define MSGT_MAIN1 20
#define	MSGT_OVERRUN 30
#define MSGT_UART_DATA 31
#define MSGT_I2C_SENSOR 39
#define MSGT_I2C_DBG 41
#define	MSGT_I2C_DATA 40
#define MSGT_I2C_RQST 42
#define MSGT_I2C_MASTER_SEND_COMPLETE 43
#define MSGT_I2C_MASTER_SEND_FAILED 44
#define MSGT_I2C_MASTER_RECV_COMPLETE 45
#define MSGT_I2C_MASTER_RECV_FAILED 46
#define MSGT_MOTOR_CONTROLS_FROM_ARM 60
#define MSGT_SEND_SENSOR_DATA_TO_ARM 61
#define MSGT_I2C_MASTER_RECV 70
#define MSGT_I2C_MASTER_SEND 80

#define LASER_DATA_TO_ARM 47
#define INFRARED_DATA_TO_ARM 49
#define SENSOR_DATA_TO_ARM 48
#define STOP_BYTE_CODE 0x55

#define REQUEST_LASER_DATA 50
#define REQUEST_INFRARED_DATA_FROM_SLAVE 51

#define MAIN_THREAD LATA0
#define I2C_THREAD LATA1
#define UART_RECEIVE_THREAD LATA2
#define UART_TRANSMIT_THREAD LATA3
#define MOTOR_CONTROL_THREAD LATA5

#define SENSOR_PIC_ADDRESS 0x2A
#define MOTOR_PIC_ADDRESS 0x8A

#endif

