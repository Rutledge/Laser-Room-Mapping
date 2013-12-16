//#define ADC_V1
#define __USE18F45K22 1



#include "maindefs.h"
#include <stdio.h>
#ifndef __XC8
#include <usart.h>
#include <i2c.h>
#include <timers.h>
#else
#include <plib/usart.h>
#include <plib/i2c.h>
#include <plib/timers.h>
#include <plib/adc.h>
#endif
#include "interrupts.h"
#include "messages.h"
#include "my_uart.h"
#include "my_i2c.h"
#include "uart_thread.h"
#include "timer1_thread.h"
#include "timer0_thread.h"
#include "motor_control_thread.h"
#define _XTAL_FREQ 1200000

#ifdef __USE18F45K22
// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on SWDTEN bit))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow disabled)
#ifndef __XC8
// Have to turn this off because I don't see how to enable this in the checkboxes for XC8 in this IDE
#pragma config XINST = ON       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#else
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#endif

// CONFIG1H
#pragma config CP0 = OFF        // Code Protection bit (Program memory is not code-protected)

// CONFIG2L
//#pragma config FOSC = HSPLL     // Oscillator Selection bits (HS oscillator, PLL enabled and under software control)
//#pragma config FOSC2 = ON       // Default/Reset System Clock Select bit (Clock selected by FOSC as system clock is enabled when OSCCON<1:0> = 00)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
//#pragma config CCP2MX = DEFAULT // CCP2 MUX bit (CCP2 is multiplexed with RC1)

#else
#ifdef __USE18F2680
#pragma config OSC = IRCIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bits (1K words (2K bytes) Boot Block)
#ifndef __XC8
// Have to turn this off because I don't see how to enable this in the checkboxes for XC8 in this IDE
#pragma config XINST = ON       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#endif
#else
//Something is messed up
#endif
#endif



void main(void) {
    char c;
    char I2C_READ_OCCURING = 0;
    signed char length;
    unsigned char msgtype;
    int every_other = 0;
    int test_var = 0;
    unsigned char last_reg_recvd;
    uart_comm uc;
    i2c_comm ic;
    unsigned char msgbuffer[MSGLEN + 1];
    unsigned char i;
    uart_thread_struct uthread_data; // info for uart_lthread
    timer1_thread_struct t1thread_data; // info for timer1_lthread
    timer0_thread_struct t0thread_data; // info for timer0_lthread
    motor_control_thread_struct motorthread_data; //infof or the motor thread

#ifdef __USE18F2680
    OSCCON = 0xFC; // see datasheet
    // We have enough room below the Max Freq to enable the PLL for this chip
    OSCTUNEbits.PLLEN = 1; // 4x the clock speed in the previous line
#else
    OSCCON = 0x82; // see datasheeet
    OSCTUNEbits.PLLEN = 0; // Makes the clock exceed the PIC's rated speed if the PLL is on
#endif

    // initialize my uart  handling code
   init_uart_recv(&uc);

    // initialize the i2c code
   init_i2c(&ic);

   i2c_configure_master();

    // init the timer1 lthread
   init_timer1_lthread(&t1thread_data);

    // initialize message queues before enabling any interrupts
    init_queues();

    // set direction for PORTB to output
    TRISB = 0x0;
    LATB = 0x0;

    // how to set up PORTA for input (for the V4 board with the PIC2680)
    /*
            PORTA = 0x0;	// clear the port
            LATA = 0x0;		// clear the output latch
            ADCON1 = 0x0F;	// turn off the A2D function on these pins
            // Only for 40-pin version of this chip CMCON = 0x07;	// turn the comparator off
            TRISA = 0x0F;	// set RA3-RA0 to inputs
     */

    // initialize Timers
    
#ifdef    ENABLE_TIMER1
     OpenTimer1(TIMER_INT_ON & T1_PS_1_8 & T1_16BIT_RW & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF & T1_SOURCE_FOSC, 
             TIMER_GATE_ON & TIMER_GATE_POL_HI & TIMER_GATE_TOGGLE_ON & TIMER_GATE_1SHOT_ON & TIMER_GATE_SRC_T1GPIN &  TIMER_GATE_INT_ON );
   //  OpenI2C2(MASTER, SLEW_OFF);

#endif
 //    WriteTimer1(65086);

     



    // Peripheral interrupts can have their priority set to high or low
    // enable high-priority interrupts and low-priority interrupts
    enable_interrupts();
#if (ENABLE_UART_RX || ENABLE_UART_TX)
    OpenUSART( USART_TX_INT_OFF & USART_RX_INT_ON &  USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX  & USART_BRGH_HIGH & USART_ADDEN_OFF, 38);
#endif
    //38 gives us baud rate of approx 19230

    // Decide on the priority of the enabled peripheral interrupts
    // 0 is low, 1 is high
    
#if ENABLE_TIMER1
    // Timer1 interrupt
    IPR1bits.TMR1IP = 0;
#endif
#if ENABLE_UART_RX
    // USART RX interrupt
    IPR1bits.RCIP = 1;
#endif
#if   ENABLE_UART_TX
    // USART TX interrupt
    IPR1bits.TXIP = 0;
#endif
#if ENABLE_I2C
    // I2C interrupt
   IPR1bits.SSPIP = 1;
#endif
    //timer 0 interrupt priority
   INTCON2bits.TMR0IP = 1;
    




#if ENABLE_UART_RX
     //enable rx interrupts
  PIE1bits.RCIE = 1;
#endif
#if    ENABLE_UART_TX
    PIE1bits.TXIE = 0; //disable send interrupt until we have something in mesage queue
#endif
#if ENABLE_I2C
    // must specifically enable the I2C interrupts
    PIE1bits.SSPIE = 1;
#endif

#if ENABLE_GPIO
    TRISA = 0;
#endif

    

    


  //  OpenADC( ADC_FOSC_16 & ADC_RIGHT_JUST & ADC_4_TAD, ADC_CH0 & ADC_INT_ON
  //              & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, ADC_0ANA);

    // configure the hardware i2c device as a slave (0x9E -> 0x4F) or (0x9A -> 0x4D)
#if 1
    // Note that the temperature sensor Address bits (A0, A1, A2) are also the
    // least significant bits of LATB -- take care when changing them
    // They *are* changed in the timer interrupt handlers if those timers are
    //   enabled.  They are just there to make the lights blink and can be
    //   disabled.
  //  i2c_configure_slave(0x9E);
#else
    // If I want to test the temperature sensor from the ARM, I just make
    // sure this PIC does not have the same address and configure the
    // temperature sensor address bits and then just stay in an infinite loop
    i2c_configure_slave(0x9A);
    LATBbits.LATB1 = 1;
    LATBbits.LATB0 = 1;
    LATBbits.LATB2 = 1;
    for (;;);
#endif

    

     

    // configure the hardware USART device
   // OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT &
     //       USART_CONT_RX & USART_BRGH_LOW, 0x19);

    /* Junk to force an I2C interrupt in the simulator (if you wanted to)
    PIR1bits.SSPIF = 1;
    _asm
    goto 0x08
    _endasm;
     */

    // printf() is available, but is not advisable.  It goes to the UART pin
    // on the PIC and then you must hook something up to that to view it.
    // It is also slow and is blocking, so it will perturb your code's operation
    // Here is how it looks: printf("Hello\r\n");


    // loop forever
    // This loop is responsible for "handing off" messages to the subroutines
    // that should get them.  Although the subroutines are not threads, but
    // they can be equated with the tasks in your task diagram if you
    // structure them properly.

    #if ENABLE_GPIO
        MAIN_THREAD = 1;
        I2C_THREAD = 0;
        UART_RECEIVE_THREAD = 0;
        UART_TRANSMIT_THREAD = 0;
        MOTOR_CONTROL_THREAD = 0;
    #endif

        char testbuf[3];
        
        testbuf[0] = 'G';
        testbuf[1] = 0x74;
        testbuf[2] = 0x72;  //72 = continous
      
  

    char testbuf2[2];
    
    testbuf2[0] = 'r';
    testbuf2[1] = 0x08;

    char testbuf3[1];
   testbuf3[0]= 'C';
   

    


 // i2c_master_send(1,testbuf3, 0x42); //cal enter
 
       // __delay_ms(200);

 // for(int i = 0; i < 300; i ++)
  //      _delay(197120);
     //    testbuf3[0]= 'E';
     //   i2c_master_send(1,testbuf3, 0x42);
//    i2c_master_send(3, testbuf, 0x42);
//    i2c_master_send(2, testbuf2, 0x42);
 //   i2c_master_recv(2, 0x42);
 //   i2c_master_recv(2, 0x42);
   


    while (1) {
        // Call a routine that blocks until either on the incoming
        // messages queues has a message (this may put the processor into
        // an idle mode)
        
        
        block_on_To_msgqueues();

        // At this point, one or both of the queues has a message.  It
        // makes sense to check the high-priority messages first -- in fact,
        // you may only want to check the low-priority messages when there
        // is not a high priority message.  That is a design decision and
        // I haven't done it here.
        
        length = ToMainHigh_recvmsg(MSGLEN, &msgtype, (void *) msgbuffer);
        if (length < 0) {
            // no message, check the error code to see if it is concern
            if (length != MSGQUEUE_EMPTY) {
                // This case be handled by your code.
            }
        } else {
            switch (msgtype) {
                case MSGT_I2C_MASTER_SEND:
                {
                     i2c_master_send(length,msgbuffer, MOTOR_PIC_ADDRESS);
                     break;
                }
                default:
                {
                    // Your code should handle this error
                    break;
                };
            };
        }
        
        // Check the low priority queue
        length = ToMainLow_recvmsg(MSGLEN, &msgtype, (void *) msgbuffer);
        if (length < 0) {
            // no message, check the error code to see if it is concern
            if (length != MSGQUEUE_EMPTY) {
                // Your code should handle this situation
            }
        } else {
            switch (msgtype) {
                case MSGT_TIMER1:
                {
                    timer1_lthread(&t1thread_data, msgtype, length, msgbuffer);
                    break;
                };
                case MSGT_MOTOR_CONTROLS_FROM_ARM:
                {
                  motor_control_thread(&motorthread_data, msgtype, length, msgbuffer);
                    break;

                }
                case MSGT_OVERRUN:
                case MSGT_UART_DATA:
                {
                   
                    break;
                };
                case MSGT_I2C_MASTER_RECV:
                {
                  i2c_master_recv(2, 0x42);
                  i2c_master_recv(7, SENSOR_PIC_ADDRESS);
                    
                    break;

                };
                case MSGT_I2C_DATA:
                case MSGT_I2C_DBG:
                {
                    // Here is where you could handle debugging, if you wanted
                    // keep track of the first byte received for later use (if desired)
                    if(length == 2)
                    {
                    putIntoUARTBuffer(msgbuffer, 2);

                    }
                    else if (length == 7)
                    {
                    putIntoUARTBuffer(msgbuffer, 7);
                    PIE1bits.TXIE = 1;
                    }

                 


                    
                   
                    break;
                };
                default:
                {
                    // Your code should handle this error
                    break;
                };
            };
        }
    }

}