// *******************************************************
//
// rotors.c
//
// Support for interfacing with the helicopters main and tail rotors
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
//
// *******************************************************


//*****************************************************************************
// Includes
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h" // Needed for pin configuration
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "buttons.h"
#include "rotors.h"


//*******************************************
// Local prototypes
//*******************************************
void initialiseMainPWM (void);
void initialiseTailPWM (void);
void setMainPWM (uint32_t u32Duty);
void setTailPWM (uint32_t u32Duty);


//*****************************************************************************
// Globals to module
//*****************************************************************************
static uint32_t g_mainDuty = PWM_MAIN_DUTY_LOW; // Initial duty cycle for main rotor
static uint32_t g_tailDuty = PWM_TAIL_DUTY_LOW; // Initial duty cycle for tail rotor


//*********************************************************
// Initialises main rotor motor
// M0PWM7 (J4-05, PC5) is used for the main rotor motor
//*********************************************************
void initMainPWM (void)
{
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);

    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    // Set the initial PWM parameters
    setMainPWM (PWM_DUTY_ZERO);

    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
}


//*********************************************************
// Initialises tail rotor motor
// M1PWM5 (J3-10, PF1) is used for the tail rotor motor
//*********************************************************
void initTailPWM (void) {
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);

    GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);

    PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    // Set the initial PWM parameters
    setTailPWM(PWM_DUTY_ZERO);

    PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);
}



//********************************************************
// Function to set the freq, duty cycle of M0PWM7
//********************************************************
void
setMainPWM (uint32_t ui32Duty) {
    g_mainDuty = ui32Duty;
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period =
        SysCtlClockGet() / PWM_DIVIDER / PWM_RATE_HZ;

    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, ui32Period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM,
        ui32Period * ui32Duty / 100);
}


//********************************************************
// Function to set the freq, duty cycle of M1PWM5
//********************************************************
void
setTailPWM (uint32_t ui32Duty)
{
    g_tailDuty = ui32Duty;

    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period =
        SysCtlClockGet() / PWM_DIVIDER / PWM_RATE_HZ;

    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, ui32Period);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM,
        ui32Period * ui32Duty / 100);
}


//********************************************************
// Function to enable PWM output
//********************************************************
void enablePWM (void)
{
    // Initialisation is complete, so turn on the output.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);
}


//********************************************************
// Function to disable PWM output
//********************************************************
void disablePWM (void)
{
    // Flight is complete, so turn off the output.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);
}

//********************************************************
// Returns duty cycle of main rotor
//********************************************************
uint32_t getMainDuty (void)
{
    return g_mainDuty;
}


//********************************************************
// Returns duty cycle of tail rotor
//********************************************************
uint32_t getTailDuty (void)
{
    return g_tailDuty;
}
