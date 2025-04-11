/*
 * timer.c
 *
 *  Created on: Apr 4, 2025
 *      Author: srj
 */

#include "timer.h"
#include "main.h"

  int duty;

void TimerPWMinit(void){
	//configuring PC6 as TIM8 CH1
	//enabling PC clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	//setting PC6 mode as Alternate Fn (10)
	GPIOC->MODER |= (1<<(2*6+1));
	GPIOC->MODER &= ~(1<<(2*6));
	//disabling pull-up and pull-down
	GPIOC->PUPDR &= ~((1<<(2*6))|(1<<(2*6+1)));
	//set alt fn "3" as TIM8
	GPIOC->AFR[0] |= (3<<(6*4));

	//PWM config

	//Enabling TIM8 clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
	//seting PWM clock (10KHz to 100 KHz). Program the period (ARR) and the duty cycle (CCR)
	// PCLK = 16Mhz, PSC = 16 --> TCLK = 1mhz
	//ARR=100       PWM Output freq = TCLK/ARR = 1mhz/10khz
	// CCR =50
	TIM8->PSC=TIM_PR-1;
	TIM8->ARR=100-1;
	TIM8->CCR1=0;
	//Configutr output pin
	// SELECT output mode by writing CCS bits (00 on CCMRx register.)
	// The PWM can be selected individually (one PWM per OCx output) by writing 110 (PWM mode 1) or 11 (mode 2) in the TIMx_CCRx register.
	TIM8->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; //PWM
	//Setting preload bit in CCMRx regi and ARPE bit in the CR1 regi
	TIM8->CCMR1 |= TIM_CCMR1_OC1PE;
	TIM8->CR1 |= TIM_CR1_ARPE;
	//Select the counting mode in CR:
	//if 00- edge alingned  else  centre aligned
	TIM8->CR1 &= ~(TIM_CR1_CMS_0 | TIM_CR1_CMS_1);
	//ENABLE CAPTURE COMPARE
	TIM8->CCER |= TIM_CCER_CC1E;
	//Enable main output ,ENALBLE COUNTER
	TIM8->BDTR |= TIM_BDTR_MOE;
	TIM8->CR1 |= TIM_CR1_CEN;
	TIM8->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM8_CC_IRQn);
	TIM8->CR1 |= TIM_CR1_CEN;

}

void TIM8_CC_IRQHandler(void){

	if(TIM8->SR & TIM_SR_UIF){
		TIM8->SR &= ~TIM_SR_UIF;
		for(duty=0; duty<100; duty++) {
			  				TIM8->CCR1 = duty;
			  				HAL_Delay(40);
			  			}
			  			for(duty=99; duty>=0; duty--) {
			  				TIM8->CCR1 = duty;
			  				HAL_Delay(40);
			  			}
			  }
	}
