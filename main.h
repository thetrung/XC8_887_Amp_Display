/*
 * 20190306.002
 * OLED 128x32
 *
 + File: main.h
 * Processor: PIC16F1825
 * Author: wizlab.it
 */

#ifndef MAIN_H
#define	MAIN_H

#include "init.h"

void loop(void);
void blink(void);
void display_adc(void);
void ADC_Init(void);
int ADC_Read(int channel);

#endif