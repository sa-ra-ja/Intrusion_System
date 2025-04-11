/*
 * rtc.h
 *
 *  Created on: Apr 7, 2025
 *      Author: admin
 */

#ifndef RTC_H_
#define RTC_H_

#include "stm32f4xx.h"

#define RTC_ASYNC_PR	128
#define RTC_SYNC_PR		250

typedef struct RtcDate {
	uint16_t date;
	uint16_t month;
	uint16_t year;
	uint16_t weekday;
}RtcDate_t;

typedef struct RtcTime {
	uint16_t hr;
	uint16_t min;
	uint16_t sec;
}RtcTime_t;

void RtcInit(RtcDate_t *dt, RtcTime_t *tm);
void RtcSetDate(RtcDate_t *dt);
void RtcSetTime(RtcTime_t *tm);
void RtcGetDate(RtcDate_t *dt);
void RtcGetTime(RtcTime_t *tm);

#endif /* RTC_H_ */
