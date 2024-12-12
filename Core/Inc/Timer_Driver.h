/*
 * Timer_Driver.h
 *
 *  Created on: Oct 10, 2024
 *      Author: poofy
 */

#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

#include "InterruptControl.h"

void Timer_Init(void);					//initialize the specified timer,
void StartTimer(void);
void stopTimer(void);
uint32_t GetElapsedTime(void);
void IncrimentTime(void);



#endif /* TIMER_DRIVER_H_ */
