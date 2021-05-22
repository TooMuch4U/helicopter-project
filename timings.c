// *******************************************************
//
// timings.c
//
// Support functions used for timing
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
//
// *******************************************************


//*****************************************************************************
// Includes
//*****************************************************************************
#include "timings.h"

//*****************************************************************************
// Defines
//*****************************************************************************
#define TIMING_BASE WTIMER5_BASE
#define TIMING_MODE TIMER_CFG_PERIODIC
#define TIMING_PERIPH SYSCTL_PERIPH_WTIMER5
#define TIMING_TIMER TIMER_BOTH
#define TIMING_MAX_64 18446744073709551615


//*****************************************************************************
// Globals to module
//*****************************************************************************
static clockRate;

//*****************************************************************************
// Sets up the timer module.
// Modified from Lecture 16, Page 10
//*****************************************************************************
void initTimer(void)
{
    clockRate = SysCtlClockGet();

    // Timer initialisation
    SysCtlPeripheralReset(TIMING_PERIPH);
    SysCtlPeripheralEnable(TIMING_PERIPH);

    // Configure timer A for input edge time capture
    TimerDisable(TIMING_BASE, TIMING_TIMER); // Disable timer for setup
    TimerConfigure(TIMING_BASE, TIMING_MODE);
    TimerLoadSet64(TIMING_BASE, TIMING_MAX_64);
    TimerEnable(TIMING_BASE, TIMING_TIMER); // Enable timer
}


//*****************************************************************************
// Gets the current time in clock ticks.
//*****************************************************************************
uint64_t getCurTime(void)
{
    return TimerValueGet64(TIMING_BASE);
}


//*****************************************************************************
// Returns the elapsed time since a past time.
//*****************************************************************************
uint64_t getElapsedTime(uint64_t pastTime)
{
    // Timer counts down so calculation seems unintuative.
    uint64_t current = getCurTime();
    return (pastTime - current);
}

//*****************************************************************************
// Returns the difference (in ticks) between two reference times.
//*****************************************************************************
uint64_t getTimeDiff(uint64_t pastTime, uint64_t current)
{
    // Timer counts down so calculation seems unintuative.
    return (pastTime - current);
}


//*****************************************************************************
// Schedules a function at a desired rate in HZ.
// If it is time for the function to run, true value will be returned.
//*****************************************************************************
bool shouldBeRun(uint64_t lastRun, uint32_t rate)
{
    // If is time to run it
    return (getElapsedTime(lastRun) > (clockRate / rate));
}



