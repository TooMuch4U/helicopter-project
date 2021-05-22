#ifndef CONTROLLERS_H_
#define CONTROLLERS_H_

// *******************************************************
//
// controllers.h
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 20/05/2021
//
// *******************************************************

#include <stdint.h>

//*****************************************************************************
// Constants
//*****************************************************************************
#define ALTITUDE_DELTA_ERROR_TOL 2
#define YAW_DELTA_ERROR_TOL 2


//*****************************************************************************
// Function declarations
//*****************************************************************************
int32_t getDeltaYawError(void);
int32_t getDeltaAltitudeError(void);
uint32_t runYawControl(uint32_t actualDegrees, uint32_t desiredDegrees, uint64_t deltaTime);
uint32_t runAltitudeControl(int32_t actualAltitude, int32_t desiredAltitude, uint64_t deltaTime);
uint32_t runControl(int64_t* accumulatedError, int32_t error, int32_t prevError, int64_t deltaTime,
                    int32_t pGain, int32_t iGain, uint32_t dGain, int32_t bias);
int32_t getAltitudeError(int32_t currentAltitude, int32_t desiredAltitude);
int32_t getYawError(int32_t currentYaw, int32_t desiredYaw);
void resetAccumulatedIntegral();
void increaseDesiredAltitude(uint32_t* desiredAltitude);
void decreaseDesiredAltitude(uint32_t* desiredAltitude);
void increaseDesiredYaw(uint32_t* desiredYaw);
void decreaseDesiredYaw(uint32_t* desiredYaw);


#endif /* CONTROLLERS_H_ */
