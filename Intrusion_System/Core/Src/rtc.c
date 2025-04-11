/*
 * rtc.c
 *
 *  Created on: Apr 7, 2025
 *      Author: admin
 */

#include "rtc.h"

// 00 <= binVal <= 99
uint32_t Bin2BCD(uint32_t binVal) {
  // get unit place digit
  uint32_t unit = binVal % 10;
  // get tens place digit
  uint32_t tens = binVal / 10;
  // combine tens place digit bin with unit place digit bin
  uint32_t result = (tens << 4) | unit;
  return result;
}

// 00 <= bcdVal <= 99
uint32_t BCD2Bin(uint32_t bcdVal) {
  // get unit place digit
  uint32_t unit = bcdVal & 0x0F;
  // get tens place digit
  uint32_t tens = (bcdVal & 0xF0) >> 4;
  // combine tens place digit bin with unit place digit bin
  uint32_t result = (tens * 10) + unit;
  return result;
}

void RtcInit(RtcDate_t *dt, RtcTime_t *tm) {
	//- enable peripheral clock power
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	//- enable access to the RTC registers in PWR control (stm32 manual 5.4)
	PWR->CR |= PWR_CR_DBP;
	//- enable LSI in RCC CSR (stm32 manual 7.3.21)
	RCC->CSR |= RCC_CSR_LSION;
	//- wait for LSI ready flag
	while(!(RCC->CSR & RCC_CSR_LSIRDY));
	//- select LSI (32 KHz) and enable RTC in RCC (BDCR) (stm32 manual 7.3.20)
	RCC->BDCR |= RCC_BDCR_RTCSEL_1;
	RCC->BDCR &= ~RCC_BDCR_RTCSEL_0;
	RCC->BDCR |= RCC_BDCR_RTCEN;
	//- enter key to unlock write protection. key sequence must be 0xCA and then 0x53 (stm32 manual 26.3.5)
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
	//- enable rtc setup in RTC ISR (i.e. register init) -- initialization mode
	RTC->ISR |= RTC_ISR_INIT;
	//- wait until register update is allowed
	while(!(RTC->ISR & RTC_ISR_INITF));
	//- set sync & async prescalar in RTC PRER
	RTC->PRER = ((RTC_ASYNC_PR - 1) << RTC_PRER_PREDIV_A_Pos) |
					((RTC_SYNC_PR - 1) << RTC_PRER_PREDIV_S_Pos);
	//- set initial date and time
	RtcSetDate(dt);
	RtcSetTime(tm);
	//- rtc date/time should be read from calendar registers (not shadow registers) (RTC CR)
	RTC->CR |= RTC_CR_BYPSHAD;
	//- start rtc in free running mode (RTC ISR)
	RTC->ISR &= ~RTC_ISR_INIT;
	//- disable access to RTC registers (write protection) in PWR control
	PWR->CR &= ~PWR_CR_DBP;
}

void RtcSetDate(RtcDate_t *dt) {
	//- Convert binary data into BCD
	uint32_t date = Bin2BCD(dt->date);
	uint32_t month = Bin2BCD(dt->month);
	uint32_t year = Bin2BCD(dt->year);
	uint32_t weekday = Bin2BCD(dt->weekday);
	//- Set day, month, year, and day of week into a RTC Date Register DR
	uint32_t dr = (date << RTC_DR_DU_Pos) |
			 	 	 (month << RTC_DR_MU_Pos) |
					 (weekday << RTC_DR_WDU_Pos) |
					 (year << RTC_DR_YU_Pos);
	RTC->DR = dr;
}

void RtcSetTime(RtcTime_t *tm) {
	//- Convert binary data into BCD
	uint32_t hr = Bin2BCD(tm->hr);
	uint32_t min = Bin2BCD(tm->min);
	uint32_t sec = Bin2BCD(tm->sec);
	//- Set hours, minutes, and seconds into a RTC Time Register TR. Also set AM/PM mode
	uint32_t tr = (sec << RTC_TR_SU_Pos) |
					(min << RTC_TR_MNU_Pos) |
					(hr << RTC_TR_HU_Pos);
	RTC->TR = tr;
}

void RtcGetDate(RtcDate_t *dt) {
	//- Read RTC Date register
	uint32_t dr = RTC->DR;
	//- Retrieve day, month, year, and day of week from DR and convert from BCD to binary
	uint32_t date = (dr & (RTC_DR_DT_Msk | RTC_DR_DU_Msk)) >> RTC_DR_DU_Pos;
	dt->date = BCD2Bin(date);
	uint32_t month = (dr & (RTC_DR_MT_Msk | RTC_DR_MU_Msk)) >> RTC_DR_MU_Pos;
	dt->month = BCD2Bin(month);
	uint32_t weekday = (dr & (RTC_DR_WDU_Msk)) >> RTC_DR_WDU_Pos;
	dt->weekday = BCD2Bin(weekday);
	uint32_t year = (dr & (RTC_DR_YT_Msk | RTC_DR_YU_Msk)) >> RTC_DR_YU_Pos;
	dt->year = BCD2Bin(year);
}

void RtcGetTime(RtcTime_t *tm) {
	//- Read RTC Time register
	uint32_t tr = RTC->TR;
	//- Retrieve hours, minutes, and seconds from TR and convert from BCD to binary
	uint32_t sec = (tr & (RTC_TR_ST_Msk | RTC_TR_SU_Msk)) >> RTC_TR_SU_Pos;
	tm->sec = BCD2Bin(sec);
	uint32_t min= (tr & (RTC_TR_MNT_Msk | RTC_TR_MNU_Msk)) >> RTC_TR_MNU_Pos;
	tm->min = BCD2Bin(min);
	uint32_t hr = (tr & (RTC_TR_HT_Msk | RTC_TR_HU_Msk)) >> RTC_TR_HU_Pos;
	tm->hr = BCD2Bin(hr);
}
