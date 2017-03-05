/*
 * support.h
 *
 *  Created on: 27 ���. 2016 �.
 *      Author: Gumenyak
 */
#include "stdio.h"
#ifndef SUPPORT_H_
#define SUPPORT_H_

#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1

typedef enum { false, true } bool;
void delay(uint16_t msec);
void to_num16(char * buf, uint16_t num);
void setPinMode(uint8_t iPin, uint8_t iMode);
void digitalWrite (uint8_t iPin, uint8_t val);
uint8_t digitalRead (uint8_t iPin);

#endif /* SUPPORT_H_ */
