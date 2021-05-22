#ifndef TIMINGS_H_
#define TIMINGS_H_

// *******************************************************
//
// timings.h
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
//
// *******************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/timer.h"


//*****************************************************************************
// Function declarations
//*****************************************************************************
void initTimer(void);
uint64_t getCurTime(void);
uint64_t getElapsedTime(uint64_t pastTime);
bool shouldBeRun(uint64_t lastRun, uint32_t rate);
uint64_t getTimeDiff(uint64_t pastTime, uint64_t current);


#endif /* TIMINGS_H_ */
