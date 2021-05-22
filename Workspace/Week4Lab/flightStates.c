// *******************************************************
//
// flightStates.c
//
// Enumeration to define the flight states, and related
// functions.
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
//
// *******************************************************


//*****************************************************************************
// Includes
//*****************************************************************************
#include <stdint.h>
#include "flightStates.h"


//*****************************************************************************
// Defines
//*****************************************************************************
#define MAX_STR_LEN 20;


//********************************************************
// Gets a string representation of the flight state
//********************************************************
char* getStateStr(uint32_t state)
{
    switch (state)
    {
        case LANDED_LOCK:
            return "LANDED";
            break;

        case LAUNCHING:
            return "TAKEOFF";
            break;

        case SEEKING:
            return "TAKEOFF";
            break;

        case SETTING:
            return "TAKEOFF";
            break;

        case FLYING:
            return "FLIGHT";
            break;

        case LANDING_TURN:
            return "LANDING";
            break;

        case LANDING:
            return "LANDING";
            break;

        case LANDED:
            return "LANDED";
            break;
    }
}
