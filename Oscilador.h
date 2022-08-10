/* 
 * File: Oscilador.h
 * Author: Sergio Boch 
 * Comments: Prototipo de función para configuración de oscilador interno 
 * (PIC16F887)
 * Revision history: 
 */
 
#ifndef OSCILADOR_H
#define	OSCILADOR_H

#include <xc.h>
#include <stdint.h> 

void int_osc_MHz (uint8_t freq);

#endif	/* OSCILADOR_H */