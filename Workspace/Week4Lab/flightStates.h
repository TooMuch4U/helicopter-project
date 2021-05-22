#ifndef FLIGHTSTATES_H_
#define FLIGHTSTATES_H_

// *******************************************************
//
// flightStates.h
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
//
// *******************************************************

//*****************************************************************************
// Enumeration of flight states
//*****************************************************************************
enum flightStates {
    LANDED = 0,         // Landed and ready to takeoff.
    LANDING = 1,        // In the process of descending.
    LAUNCHING = 2,      // Ascending to the hover height.
    SEEKING = 3,        // Looking for the yaw reference direction.
    SETTING = 4,        // Yaw reference has been found and is now turning to face it.
    FLYING = 5,         // Standard flying with button controls.
    LANDING_TURN = 6,   // Turning to face the reference position before descending.
    LANDED_LOCK = 7     // When the heli is reset or turned on with the switch in the up position.
};

char* getStateStr(uint32_t state);


#endif /* FLIGHTSTATES_H_ */
