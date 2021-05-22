// *******************************************************
//
// controllers.c
//
// Contains PID controllers for helicopter
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 20/05/2021
//
// *******************************************************

#include "altitude.h"
#include "yaw.h"
#include "display.h"
#include "rotors.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "controllers.h"
#include "driverlib/gpio.h"
#include "timings.h"

//*****************************************************************************
// Defines
//*****************************************************************************
#define GAIN_SCALE 1000         // Scales gains to allow calculation with integers only
#define TIME_SCALE 200000       // Scales time from clock ticks so that each 1 is 0.01s

// GAINS FOR REAL HELI
#define ALTITUDE_PROPORTIONAL_GAIN 400
#define ALTITUDE_INTEGRAL_GAIN 10
#define ALTITUDE_DERIVATIVE_GAIN 0
#define ALTITUDE_CONTROL_BIAS 5

#define YAW_PROPORTIONAL_GAIN 300
#define YAW_INTEGRAL_GAIN 10
#define YAW_DERIVATIVE_GAIN 0
#define YAW_CONTROL_BIAS 0

// GAINS FOR EMULATOR
//#define ALTITUDE_PROPORTIONAL_GAIN 1000
//#define ALTITUDE_INTEGRAL_GAIN 100
//#define ALTITUDE_DERIVATIVE_GAIN 800
//#define ALTITUDE_CONTROL_BIAS 5
//
//#define YAW_PROPORTIONAL_GAIN 1100
//#define YAW_INTEGRAL_GAIN 20
//#define YAW_DERIVATIVE_GAIN 500
//#define YAW_CONTROL_BIAS 0

#define PWM_MAX_DUTY 98
#define PWM_MIN_DUTY 2


//*****************************************************************************
// Globals to module
//*****************************************************************************
static int64_t tailErrorIntegral;           // Accumulated error integral for yaw
static int64_t mainErrorIntegral;           // Accumulated error integral for main
static int32_t tailError = 0;
static int32_t mainError = 0;
static int32_t prevTailError = 0;
static int32_t prevMainError = 0;


//*****************************************************************************
// Returns change in tail error
//*****************************************************************************
int32_t getDeltaYawError(void)
{
    return tailError - prevTailError;
}


//*****************************************************************************
// Returns change in main error
//*****************************************************************************
int32_t getDeltaAltitudeError(void)
{
    return mainError - prevMainError;
}


//*****************************************************************************
// Takes the yaw in degrees, and performs an iteration of the yaw controller.
// Returns the new duty cycle to use for the tail rotor.
//*****************************************************************************
uint32_t runYawControl(uint32_t actualDegrees, uint32_t desiredDegrees, uint64_t deltaTime)
{
    tailError = getYawError(actualDegrees, desiredDegrees);
    uint32_t control = runControl(&tailErrorIntegral, tailError, prevTailError, deltaTime, YAW_PROPORTIONAL_GAIN,
                                  YAW_INTEGRAL_GAIN, YAW_DERIVATIVE_GAIN, YAW_CONTROL_BIAS);
    prevTailError = tailError;
    return control;
}


//*****************************************************************************
// Takes the height in degrees, and performs an iteration of the height controller.
// Returns the new duty cycle to use for the main rotor.
//*****************************************************************************
uint32_t runAltitudeControl(int32_t actualAltitude, int32_t desiredAltitude, uint64_t deltaTime)
{
    mainError = getAltitudeError(actualAltitude, desiredAltitude);
    uint32_t control = runControl(&mainErrorIntegral, mainError, prevMainError, deltaTime, ALTITUDE_PROPORTIONAL_GAIN,
                                 ALTITUDE_INTEGRAL_GAIN, ALTITUDE_DERIVATIVE_GAIN, ALTITUDE_CONTROL_BIAS);
    prevMainError = mainError;
    return control;
}


//*****************************************************************************
// Runs a generic PI control loop.
//*****************************************************************************
uint32_t runControl(int64_t* accumulatedError, int32_t error, int32_t prevError, int64_t deltaTime,
                    int32_t pGain, int32_t iGain, uint32_t dGain, int32_t bias)
{
    // Updated the accumulated integral error
    *accumulatedError = (*accumulatedError) + ((error * deltaTime) / TIME_SCALE);

    // Calculate the controls
    int64_t pControl = pGain * error;
    int64_t iControl = iGain * (*accumulatedError);
    int64_t dControl = dGain * (error - prevError);
    int64_t PWM = ((pControl + iControl + dControl) / GAIN_SCALE) + bias;

    // Check the control isn't going out of bounds
    if (PWM > PWM_MAX_DUTY) {
        PWM = PWM_MAX_DUTY;
    } else if (PWM < PWM_MIN_DUTY) {
        PWM = PWM_MIN_DUTY;
    }

    return PWM;
}


//*****************************************************************************
// Calculates altitude error from current yaw and desired yaw
//*****************************************************************************
int32_t getAltitudeError(int32_t currentAltitude, int32_t desiredAltitude)
{
    return (desiredAltitude - currentAltitude);
}


//*****************************************************************************
// Calculates yaw error from current yaw and desired yaw
//*****************************************************************************
int32_t getYawError(int32_t currentYaw, int32_t desiredYaw)
{
    int32_t error = 0;
    int32_t e = desiredYaw - currentYaw; // Error, uncorrected for 360-deg wrap-around

    // Wrap-around correction
    if (e > 180) {
        error = e - 360;
    } else if (e < -180) {
        error = e + 360;
    } else {
        error = e;
    }

    return error;
}


//*****************************************************************************
// Resets altitude & yaw integral
//*****************************************************************************
void resetAccumulatedIntegral()
{
    mainErrorIntegral = 0;
    tailErrorIntegral = 0;
}



//*****************************************************************************
// Increases height by increment if able
//*****************************************************************************
void increaseDesiredAltitude(uint32_t* desiredAltitude)
{
    if ((*desiredAltitude + ALTITUDE_INCREMENT) > ALTITUDE_MAX) {
        *desiredAltitude = ALTITUDE_MAX;
    } else {
        *desiredAltitude += ALTITUDE_INCREMENT;
    }
}


//*****************************************************************************
// Decreases height by increment if able
//*****************************************************************************
void decreaseDesiredAltitude(uint32_t* desiredAltitude)
{
    if (*desiredAltitude == ALTITUDE_MIN) {    // If altitude would go under zero, correct for underflow
        *desiredAltitude = ALTITUDE_MIN;
    } else {
        *desiredAltitude -= ALTITUDE_INCREMENT;
    }
}


//*****************************************************************************
// Increases yaw by increment if able
//*****************************************************************************
void increaseDesiredYaw(uint32_t* desiredYaw)
{
    if ((*desiredYaw + YAW_ANGLE_INCREMENT) > (YAW_ANGLE_MAX - 1)) {
        *desiredYaw = (*desiredYaw + YAW_ANGLE_INCREMENT) % YAW_ANGLE_MAX;
    } else {
        *desiredYaw += YAW_ANGLE_INCREMENT;
    }
}


//*****************************************************************************
// Decreases yaw by increment if able
//*****************************************************************************
void decreaseDesiredYaw(uint32_t* desiredYaw)
{
    // Check the yaw won't go below the minimum
    if ((*desiredYaw - YAW_ANGLE_INCREMENT) > YAW_ANGLE_MAX) {
        *desiredYaw = ((YAW_ANGLE_MAX - 1) - YAW_ANGLE_INCREMENT);
    } else {
        *desiredYaw -= YAW_ANGLE_INCREMENT;
    }
}


