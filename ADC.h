/* 
 * File: adc.h
 * Author: Sergio Boch 
 * Comments: Prototipo de funciones para configuración de inicialización, 
 * conversión y lectura del ADC (PIC16F887)
 * Revision history: 
 */
 
#ifndef ADC_H
#define	ADC_H

#include <xc.h>

void adc_init(uint8_t adc_cs, uint8_t vref_plus, uint8_t vref_minus);

void adc_start(uint8_t channel);

uint16_t adc_read(void);

#endif	/* ADC_H */