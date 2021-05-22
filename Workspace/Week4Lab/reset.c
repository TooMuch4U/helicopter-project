// *******************************************************
//
// reset.c
//
// Support for interfacing with the helicopters main and tail rotors.
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: YEET
//
// *******************************************************


//*****************************************************************************
// Includes.
//*****************************************************************************
#include "reset.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "slider.h"


//*****************************************************************************
// Defines
//*****************************************************************************
#define RESET_PERIPH  SYSCTL_PERIPH_GPIOA
#define RESET_PORT_BASE  GPIO_PORTA_BASE
#define RESET_PIN  GPIO_PIN_6
#define RESET_INT_PIN GPIO_INT_PIN_6
#define RESET_INT_BASE INT_GPIOA


//*****************************************************************************
// Functions
//*****************************************************************************

//*****************************************************************************
// Initialises reset interrupt
//*****************************************************************************
void initReset(void)
{
    // Initialise the GPIO pin for reading reset signal
    SysCtlPeripheralEnable(RESET_PERIPH);

    // disable interrupts
    GPIOIntDisable(RESET_PORT_BASE, RESET_INT_PIN);
    GPIOIntDisable(RESET_PORT_BASE, GPIO_INT_PIN_7);

    // set it up as an input
    GPIOPinTypeGPIOInput(RESET_PORT_BASE, RESET_PIN);

    // configure it to be a weak pull up
    GPIOPadConfigSet(RESET_PORT_BASE, RESET_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // configure the interrupt to be falling edge only (active low)
    GPIOIntTypeSet(RESET_PORT_BASE, RESET_PIN, GPIO_RISING_EDGE);

    // register the interrupt handler
    //GPIOIntRegisterPin(RESET_PORT_BASE, RESET_INT_PIN, &resetIntHandler);
    GPIOIntRegister(RESET_PORT_BASE, resetIntHandler);

    // enable interrupts on this pin
    GPIOIntEnable(RESET_PORT_BASE, RESET_INT_PIN);
}


//*****************************************************************************
// Interrupt handler for soft reset
//*****************************************************************************
void resetIntHandler(void)
{
    // Clear the interrupt
    GPIOIntClear(RESET_PORT_BASE, RESET_INT_PIN);
    SysCtlReset();

}




