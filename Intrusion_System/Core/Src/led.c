/*
 * led.c
 *
 *  Created on: Apr 11, 2025
 *      Author: srj
 */

#include "led.h"

void InitLed(uint32_t pin){
	RCC->AHB1ENR |= RCC_AHB1RSTR_GPIODRST;
	GPIOD->MODER |= 1<<(pin*2);
}

void OnLed(uint32_t pin){
	GPIOD->BSRR |= 1<<pin;
}

void OffLed(uint32_t pin){
	GPIOD->BSRR |= 1<<(pin*2);
}

void ToggleLed(uint32_t pin){
	GPIOD->ODR ^= 1<<pin;
}

void BlinkLed(uint32_t pin,uint32_t ms){
	OnLed(pin);
	DelayMs(ms);
	OffLed(pin);
	DelayMs(ms);
}


