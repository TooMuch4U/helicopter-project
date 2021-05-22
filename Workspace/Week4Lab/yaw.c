//*****************************************************************************
//
// yaw.c
//
// Determines helicopter yaw using quadrature encoder GPIO
// interrupt-driven program
//
// Authors:  T. Rizzi, E. Robinson, S. Meravanage
// Last modified: 21 May
//
//*****************************************************************************


//*****************************************************************************
// Includes
//*****************************************************************************
#include "yaw.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "timings.h"


//*****************************************************************************
// Defines
//*****************************************************************************
#define YAW_REF_PERIPH  SYSCTL_PERIPH_GPIOC
#define YAW_REF_PORT_BASE  GPIO_PORTC_BASE
#define YAW_REF_PIN  GPIO_PIN_4
#define YAW_REF_INT_PIN GPIO_INT_PIN_4
#define YAW_REF_INT_BASE INT_GPIOC
#define YAW_SEEK_INTERVAL 120
#define YAW_SEARCH_RATE 20 // Degrees per second during search for reference yaw


//*****************************************************************************
// Enumeration
//*****************************************************************************
enum yawState {UNLOCKED = 0, LOCKED = 1};


//*****************************************************************************
// Globals to module
//*****************************************************************************
static uint32_t g_notches = 0;
static int g_yawPinsCur = 0;
static int g_yawPinsPrev = 0;
volatile uint32_t referenceYaw;
volatile bool yawRefFound = false;
static uint64_t yawRefTimeStart = 0;


//*****************************************************************************
// Interrupt handler for yaw GPIO pins
//*****************************************************************************
void YawIntHandler(void)
{
    GPIOIntClear(YAW_PORT_BASE, YAW_INT_PIN_A | YAW_INT_PIN_B | GPIO_INT_PIN_2 | GPIO_INT_PIN_3);
    updateQuadEncoder(GPIOPinRead(YAW_PORT_BASE, YAW_PINS));
}


//*****************************************************************************
// Interrupt handler for yaw reference
//*****************************************************************************
void yawRefIntHandler(void)
{
    // Clear the interrupt
    GPIOIntClear(YAW_REF_PORT_BASE, YAW_REF_INT_PIN );
    if (!yawRefFound) {
        referenceYaw = getYaw();
        yawRefFound = true;
    }
}


//*****************************************************************************
// Initialises yaw handling
//*****************************************************************************
void initYaw(void)
{
    // Initialise the GPIO interrupt for quad decoding
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOInput(YAW_PORT_BASE, YAW_PINS);
    GPIOPadConfigSet(YAW_PORT_BASE, YAW_PINS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntEnable(YAW_PORT_BASE, YAW_INT_PIN_A|YAW_INT_PIN_B);
    GPIOIntTypeSet(YAW_PORT_BASE, YAW_PINS, GPIO_BOTH_EDGES);
    IntPrioritySet(YAW_INT_BASE, 0);
    IntRegister(YAW_INT_BASE, YawIntHandler);
    IntEnable(YAW_INT_BASE);

    // Initialise the GPIO pin for reading yaw reference signal
    SysCtlPeripheralEnable(YAW_REF_PERIPH); // Enable the peripheral
    GPIOIntDisable(YAW_REF_PORT_BASE, YAW_REF_INT_PIN); // disable interrupts while configuring
    GPIOPinTypeGPIOInput(YAW_REF_PORT_BASE, YAW_REF_PIN); // set it up as an input
    GPIOPadConfigSet(YAW_REF_PORT_BASE, YAW_REF_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntTypeSet(YAW_REF_PORT_BASE, YAW_REF_PIN, GPIO_FALLING_EDGE); // Interrupt on falling edge
    GPIOIntRegister(YAW_REF_PORT_BASE, yawRefIntHandler); // register the interrupt handler
    GPIOIntEnable(YAW_REF_PORT_BASE, YAW_REF_INT_PIN); // enable interrupts on this pin

    // Enable all interrupts
    IntMasterEnable();
}


//*****************************************************************************
// Gets direction of change in quadrature disc notches (0, -1, +1)
//*****************************************************************************
void updateQuadEncoder(int32_t yawPinsInput)
{
    g_yawPinsPrev = g_yawPinsCur;
    g_yawPinsCur = yawPinsInput;

    // Check which direction it changed.
    int8_t left = ((g_yawPinsCur & 2) >> 1) ^ (g_yawPinsPrev & 1);
    int8_t right = -1 * (((g_yawPinsPrev & 2) >> 1) ^ (g_yawPinsCur & 1));
    int8_t dirChange = left + right;

    // Negative wrap-around case
    if (g_notches == 0 && dirChange < 0) {
        g_notches = YAW_NOTCHES_MAX;
    }
    // Positive wrap-around case
    else if (g_notches == YAW_NOTCHES_MAX && dirChange > 0) {
        g_notches = 0;
    }
    // Normal case
    else {
        g_notches = g_notches + dirChange;
    }
}


//*****************************************************************************
// Converts quadrature disc notch position to angle in degrees, returns value
//*****************************************************************************
uint32_t getYaw(void)
{
    uint32_t yaw = (g_notches * (YAW_ANGLE_MAX - 1)) / YAW_NOTCHES_MAX;
    return yaw;
}


//*****************************************************************************
// Returns the next yaw value to search at
//*****************************************************************************
uint32_t findReferenceYaw(uint32_t startYaw) // might need to pass in yaw by reference to assign to refYaw when found
{

    uint32_t outputYaw;

    // Check if the yaw reference has been found
    if (yawRefFound) {
        outputYaw = referenceYaw;
    }

    else {
        // Set the yaw reference time if its 0
        if (yawRefTimeStart == 0) {
            yawRefTimeStart = getCurTime();
        }

        outputYaw = (startYaw + ((YAW_SEARCH_RATE * getElapsedTime(yawRefTimeStart)) / SysCtlClockGet())) % YAW_ANGLE_MAX;
    }

    return outputYaw;
}


//*****************************************************************************
// Returns true if the yaw reference has been found.
//*****************************************************************************
bool yawCalibrated()
{
    return yawRefFound;
}


//*****************************************************************************
// Returns the yaw reference.
//*****************************************************************************
uint32_t getReferenceYaw()
{
    return referenceYaw;
}


//*****************************************************************************
// Resets the yaw reference.
//*****************************************************************************
void resetYawRef()
{
    yawRefFound = false;
}






