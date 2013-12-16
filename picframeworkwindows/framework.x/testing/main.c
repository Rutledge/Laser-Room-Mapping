/*
 * File:   main.c
 * Author: Rutledge
 *
 * Created on November 22, 2013, 2:02 PM
 */


// PIC18F45K22 Configuration Bit Settings
#include <xc.h>
#include <delays.h>
#include "defines.h"
#include "my_UART.h"
#include "operations.h"

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1H
#pragma config FOSC = HSMP	// Oscillator Selection bits (EC oscillator (high power, >16 MHz))
#pragma config PLLCFG = ON	// 4X PLL Enable (Oscillator used directly)
#pragma config PRICLKEN = ON	// Primary clock enable bit (Primary clock is always enabled)
#pragma config FCMEN = OFF	// Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF	// Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF	// Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = SBORDIS	// Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 190	// Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF	// Watchdog Timer Enable bits (WDT is always enabled. SWDTEN bit has no effect)
#pragma config WDTPS = 32768	// Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC1	// CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF	// PORTB A/D Enable bit (PORTB<5:0> pins are configured as analog input channels on Reset)
#pragma config CCP3MX = PORTB5	// P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config HFOFST = ON	// HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0	// Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTD2	// ECCP2 B output mux bit (P2B is on RD2)
#pragma config MCLRE = EXTMCLR	// MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON	// Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON		// Single-Supply ICSP Enable bit (Single-Supply ICSP enabled if MCLRE is also 1)
#pragma config XINST = OFF	// Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF	// Code Protection Block 0 (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF	// Code Protection Block 1 (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF	// Code Protection Block 2 (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF	// Code Protection Block 3 (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF	// Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF	// Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF	// Write Protection Block 0 (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF	// Write Protection Block 1 (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF	// Write Protection Block 2 (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF	// Write Protection Block 3 (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF	// Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF	// Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF	// Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF	// Table Read Protection Block 0 (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF	// Table Read Protection Block 1 (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF	// Table Read Protection Block 2 (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF	// Table Read Protection Block 3 (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF	// Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

/*
 *
 */
int main(void)
{
    ANSELA = 0x0;		// Set to Digital Function
    ANSELB = 0x0;
    ANSELC = 0x0;
    ANSELD = 0x0;
    ANSELE = 0x0;

    UART_DATA uart_data;
    UART1_Init(&uart_data);
    UART2_Init(&uart_data);

    TRISAbits.TRISA0 = 0;	// Set PIN A as output
    TRISAbits.TRISA1 = 0;

    LATAbits.LATA1 = 0;		//Set PIN for Stepper motor low



    laser_init();
    
//    while(1){
//        wait_onesec();
//        unsigned long temp= laser_read();
//        wifi_write(temp);
//        move_motor();
//        move_motor();
//
//
//    }
//}

    unsigned long lastdist;
    unsigned long arcarea;
    unsigned long currdist = laser_read();
//    wifi_write(currdist);
    unsigned long avgdist;
    unsigned long total = 0;
    
    for (int n = 0; n < 100; n++) {
        move_motor();
        move_motor();
        lastdist = currdist;
        currdist = laser_read();
 //       wifi_write(currdist);
        avgdist = (currdist + lastdist);
        avgdist = (avgdist / 2);
        arcarea = avgdist*avgdist;
        arcarea = (arcarea / 10000);
        arcarea = arcarea * 314;
        total += arcarea;
        wifi_write(total);
    }
    while (1);
}


//}
//}
//        char achar[]= "character array"; // Creates character array achar with 'character array'
//    while(1){
//      char c;
//      if (UART2_Read(&c) == 0) { // Write back what you read
//          UART1_Write(&c, 1);
//        }
//      if (UART1_Read(&c) == 0) { // Write back what you read
//          UART2_Write(&c, 1);
//        }
//
//    }
//        wait();


//        char uonly[]= "U"; // Creates character array achar with U
//        UART1_Write(&uonly, 1);
//        char ronly[]= "R"; // Creates character array achar with U
//        UART1_Write(&ronly, 1);


//        UART1_Write(&uonly, 1);
//        char vonly[]= "VAV"; // Creates character array achar with U
//        UART2_Write(&vonly, 3);
//         __delay_ms(50);
