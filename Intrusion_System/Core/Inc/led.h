/*
 * led.h
 *
 *  Created on: Apr 11, 2025
 *      Author: srj
 */

#ifndef INC_LED_H_
#define INC_LED_H_


#include "stm32f4xx.h"

#define GREEN	12
#define ORANGE	13
#define RED	14
#define BLUE	15

void InitLed(uint32_t);
void OnLed(uint32_t);
void OffLed(uint32_t);
void ToggleLed(uint32_t);
void BlinkLed(uint32_t,uint32_t);


#endif /* INC_LED_H_ */
