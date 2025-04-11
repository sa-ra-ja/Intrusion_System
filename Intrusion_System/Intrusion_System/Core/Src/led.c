/*
 * led.c
 *
 *  Created on: Apr 11, 2025
 *      Author: srj
 */

#include "led.h"
#include "main.h"

volatile int flag =0;

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


void extint_init(void)
{
	// 1. Enable clock for GPIOA
	RCC->AHB1ENR |= BV(GPIOA_CLK_EN);

	// 2. configure switch pin as input
	SWITCH_GPIO->MODER &= ~(BV(0) | BV(1)); // input mode
	SWITCH_GPIO->PUPDR &= ~(BV(0) | BV(1)); // no pull-up and no pull-down resistor

	// 3. configure external interrupt in system(stm32f407g)
	SYSCFG->EXTICR[0] &= ~(BV(0) | BV(1) | BV(2) | BV(3));

	// 4. configure external interrupt in external interrupt controller
	EXTI->IMR |= BV(0);
	EXTI->RTSR |= BV(0); // rising edge intr
	EXTI->FTSR |= BV(0); // falling edge intr (switch release intr is more reliable for push button switches)

	// 5. configure external interrupt in NVIC
	NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void)
{
	// clear the interrupt
	EXTI->PR |= BV(0);
	// take action on interrupt
	flag = 1; // update the flag in RAM
						// led_on(LED_RED);
						// DelayMs(1000);
						// led_off(LED_RED);
}


