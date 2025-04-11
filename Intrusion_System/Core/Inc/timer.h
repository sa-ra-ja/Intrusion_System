/*
 * timer.h
 *
 *  Created on: Apr 4, 2025
 *      Author: srj
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "stm32f4xx.h"

#define TIM_PCLK 16000000
#define TIM_PR
extern volatile int duty;

void TimerPWMinit(void);


#endif /* TIMER_H_ */
