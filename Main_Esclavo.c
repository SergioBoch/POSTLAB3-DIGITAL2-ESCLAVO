/*
 * File:   Main_Esclavo.c
 * Author: Sergio Boch
 *
 * Created on 8 de agosto de 2022, 12:51 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h> 
#include <stdio.h>
#include "SPI.h"
#include "Oscilador.h"
#include "ADC.h"
#define _XTAL_FREQ 1000000
#define RS RC6
#define EN RC7
#define IN_MIN 0
#define IN_MAX 1023 
#define OUT_MIN 0
#define OUT_MAX 500

uint16_t ADRESH_1 = 0;
uint16_t ADRESLS_1 = 0;
uint16_t ADRESH_2 = 0;
uint16_t ADRESLS_2 = 0;
uint8_t ENVIO = 0;

void __interrupt() SPI_slave(void){
    if(PIR1bits.ADIF){                      
        if(ADCON0bits.CHS == 0){                
            ADRESH_1 = (adc_read()>>2) & 255; 
            ADRESLS_1 = adc_read()& 3;
        } else if(ADCON0bits.CHS == 1){
            ADRESH_2 = (adc_read()>>2) & 255;
            ADRESLS_2 = adc_read()& 3;
        }
        PIR1bits.ADIF = 0;                          
    }
}

void setup(void);

void main(void) {
    int_osc_MHz(1);
    adc_init(0, 0, 0);
    setup(); 
    while(1){
        if (ADCON0bits.GO == 0){
            if(ADCON0bits.CHS == 0){        // Interrupción por AN0
                ADCON0bits.CHS = 0b0001;    // Cambio de AN0 a AN1
            }
            else if (ADCON0bits.CHS == 1){  // Interrupción por AN1
                ADCON0bits.CHS = 0b0000;    // Cambio de AN1 a AN0         
            }
             __delay_us(40);                
            ADCON0bits.GO = 1;            
        }
    }
    return;
}

void setup(void){       

    ANSEL = 0b00000011;         
    ANSELH = 0b00000000;        // I/O digitales
    TRISAbits.TRISA5 = 1;       // SS como entrada
    TRISAbits.TRISA0 = 1;       
    TRISAbits.TRISA1 = 1;      
    PORTA = 0b00000000;         // Limpiar PORTA
 
    INTCONbits.GIE = 1;         // Habilitar interrupciones globales
    PIR1bits.SSPIF = 0;        
    PIE1bits.SSPIE = 1;         // Habilitar interrupciones de SPI
    INTCONbits.PEIE = 1;      
    return;
}