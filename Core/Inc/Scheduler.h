/*
 * Scheduler.h
 *
 *  Created on: Sep 10, 2024
 *      Author: poofy
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "stm32f4xx_hal.h"


#define APP_DELAY_FLAG_EVENT     	(1 << 0)
#define GENERATE_GAME_EVENT			(1 << 1)
#define GENERATE_TETRIMINO_EVENT	(1 << 2)
#define ROTATE_CW_EVENT				(1 << 3)
#define MOVE_TETRIMINO_DOWN_EVENT	(1 << 4)
#define END_GAME_EVENT				(1 << 5)
#define UPDATE_POSITION_EVENT		(1 << 6)




uint32_t getScheduledEvents(void);									//return scheduled events
void addSchedulerEvent(uint32_t toBeSched);							//adds an event to be scheduled
void removeSchedulerEvent(uint32_t toBeRem);						//removes event to be scheduled

#endif /* SCHEDULER_H_ */
