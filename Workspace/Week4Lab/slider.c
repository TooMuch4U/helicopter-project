// *******************************************************
//
// slider.c
//
// Functions for interfacing with the PA7 (SW1) slider switch.
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
//
// *******************************************************

//*****************************************************************************
// Includes.
//*****************************************************************************
#include <slider.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"


//*****************************************************************************
// Defines.
//*****************************************************************************
#define SW1_PERIPH  SYSCTL_PERIPH_GPIOA
#define SW1_PORT_BASE  GPIO_PORTA_BASE


//*****************************************************************************
// Globals to module
//*****************************************************************************
static bool state = SLIDER_DOWN;


//*****************************************************************************
// Initialises all stuff needed for the slider switch.
//*****************************************************************************
void initSlider(void)
{
    SysCtlPeripheralEnable (SW1_PERIPH);
    GPIOPinTypeGPIOInput (SW1_PORT_BASE, SW1_PIN);
    GPIOPadConfigSet (SW1_PORT_BASE, SW1_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
}


//*****************************************************************************
// Reads the GPIO pin associated with the slider switch.
// Updates the state variable accordingly.
//*****************************************************************************
void updateSlider()
{
    state = (GPIOPinRead (SW1_PORT_BASE, SW1_PIN) == SW1_PIN);
}


//*****************************************************************************
// Returns the current state of the slider switch.
//*****************************************************************************
bool getSliderState()
{
    return state;
}

