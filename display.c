// ****************************************************************************
//
// display.c
//
// Support for operating the Orbit OLED display, and displaying flight
// metrics of the helicopter
//
// Authors:  T. Rizzi, E. Robinson, S. Meravanage
// Last modified: 21 May 2021
//
// ****************************************************************************


//*****************************************************************************
// Includes
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "stdlib.h"
#include "display.h"


//*****************************************************************************
// Initialise the display module. Alias for OLEDInitialise
//*****************************************************************************
void initDisplay(void)
{
    OLEDInitialise();
}

//*****************************************************************************
// Clears all lines on OLED display
//*****************************************************************************
void displayClear(void)
{
    char string[17];
    usnprintf (string, sizeof(string), "                ");
    OLEDStringDraw (string, 0, 0);
    OLEDStringDraw (string, 0, 1);
    OLEDStringDraw (string, 0, 2);
    OLEDStringDraw (string, 0, 3);
}


//*****************************************************************************
// Displays helicopter altitude percentage on OLED
//*****************************************************************************
void displayAltitude(int32_t altitude)
{
    char string[17];
    usnprintf (string, sizeof(string), "Altitude:  %3d%%    ", altitude);
    OLEDStringDraw (string, 0, DISPLAY_ALTITUDE_LINE);
}


//*****************************************************************************
// Displays helicopter yaw in degrees on OLED
//*****************************************************************************
void displayYaw(uint32_t yaw)
{
    char string[17];
    usnprintf (string, sizeof(string), "Yaw (deg): %3d     ", yaw);
    OLEDStringDraw (string, 0, DISPLAY_YAW_LINE);
}


//*****************************************************************************
// Displays helicopter PWM duty cycles for main rotor
//*****************************************************************************
void displayMainDuty(uint32_t mainDuty)
{
    char string[17];
    usnprintf (string, sizeof(string), "Main Duty: %3d%%  ", mainDuty);
    OLEDStringDraw (string, 0, DISPLAY_MAIN_DUTY_LINE);
}


//*****************************************************************************
// Displays helicopter PWM duty cycles for tail rotor
//*****************************************************************************
void displayTailDuty(uint32_t tailDuty)
{
    char string[17];
    usnprintf (string, sizeof(string), "Tail Duty: %3d%%  ", tailDuty);
    OLEDStringDraw (string, 0, DISPLAY_TAIL_DUTY_LINE);
}


//*****************************************************************************
// Updates display with helicopter metrics
//*****************************************************************************
void
updateDisplay(int32_t altitude, uint32_t yaw, uint32_t mainDuty, uint32_t tailDuty)
{
    // Draw title
    displayYaw(yaw);
    displayAltitude(altitude);
    displayMainDuty(mainDuty);
    displayTailDuty(tailDuty);
}


