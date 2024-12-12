/*
 * ErrorHandling.c
 *
 *  Created on: Nov 6, 2024
 *      Author: poofy
 */

#include "ErrorHandling.h"

void APPLICATION_ASSERT(bool x){
	if (!x){
		while (1){
		}
	}
}
