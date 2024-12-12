/*
 * Scheduler.c
 *
 *  Created on: Sep 10, 2024
 *      Author: poofy
 */
#include "Scheduler.h"

static uint32_t scheduledEvents = 0;


void addSchedulerEvent(uint32_t eventID){    				//adds an event to be scheduled
	scheduledEvents |= eventID;		////SET bit (input arg) in scheduledEvents

}

void removeSchedulerEvent(uint32_t eventID){				//removes event to be scheduled
	scheduledEvents &= ~(eventID);							//clear bit (input arg) in scheduledEvents
}

uint32_t getScheduledEvents(void){							//return scheduled events
	return scheduledEvents;
}


