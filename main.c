// *******************************************************
//
// main.c
//
// Main loop for the program.
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: YEET
//
// *******************************************************

#include "controls.h"
#include "altitude.h"
#include "controllers.h"
#include "yaw.h"
#include "display.h"
#include "rotors.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "serial.h"
#include "kernel.h"
#include "timings.h"
#include "reset.h"
#include "flightStates.h"


//*****************************************************************************
// Constants
//*****************************************************************************
#define NUM_TASKS 4


//*****************************************************************************
// Globals
//*****************************************************************************
uint64_t prevControlTime = 0;
int32_t altitude;
uint32_t yaw;
uint32_t mainDuty;
uint32_t tailDuty;
uint32_t desiredAltitude;
uint32_t desiredYaw;
uint8_t flightState = LANDED_LOCK;
uint32_t searchYawStart;


//*****************************************************************************
// Function declarations
//*****************************************************************************
void initClock (void);
void init(void);
void runController();
void refreshDisplay();
void checkControls();
void sendSerialData();


//*****************************************************************************
// Initialisation functions for the clock (incl. SysTick), ADC, display
//*****************************************************************************
void initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    // Set PWM clock
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);
}


//*****************************************************************************
// Initialisation of all systems needed.
//*****************************************************************************
void init(void) {
   initClock();
   initAltitude();
   initDisplay();
   initControls();
   initYaw();
   initMainPWM();
   initTailPWM();
   initSerial();
   initTimer();
   initReset();

   // Enable interrupts to the processor.
   IntMasterEnable();

   // Take landed sample
   SysCtlDelay (SysCtlClockGet() / 6);
   takeLandedSample();
}


//*****************************************************************************
// Task function to updated the heli values, and run control systems.
//*****************************************************************************
void runController() {
    // Get the altitude and yaw
    altitude = getAltitudePercent();
    yaw = getYaw();

    // Calculate time
    uint64_t currentTime = getCurTime();
    uint64_t deltaTime = getTimeDiff(prevControlTime, currentTime);

    // Run PI controll
    mainDuty = runAltitudeControl(altitude, desiredAltitude, deltaTime);
    tailDuty = runYawControl(yaw, desiredYaw, deltaTime);
    setMainPWM(mainDuty);
    setTailPWM(tailDuty);

    // Updated prev control time
    prevControlTime = currentTime;

    // Check if the rotors should be off
    if (flightState == LANDED || flightState == LANDED_LOCK) {
        mainDuty = 0;
        tailDuty = 0;
    }
}


//*****************************************************************************
// Task for refreshing the display.
//*****************************************************************************
void refreshDisplay() {
    updateDisplay(altitude, yaw, mainDuty, tailDuty);
}


//*****************************************************************************
// Task for polling controls and returning desired altitude and yaw as required
//*****************************************************************************
void checkControls()
{
    // Poll controls
    pollControls();

    // Check what to do based on the flight state
    switch (flightState)
    {
        case LANDED_LOCK:
            desiredAltitude = ALTITUDE_MIN;
            desiredYaw = 0;
            if (!switchIsUp()) { flightState = LANDED; }
            break;

        case LAUNCHING:
            // Hover at 2% and keep yaw at zero
            desiredAltitude = ALTITUDE_HOVER;

            // Check next state
            if (altitude == desiredAltitude) {
                flightState = SEEKING;
            }
            break;

        case SEEKING:
            // Find reference yaw
            desiredAltitude = ALTITUDE_HOVER;
            desiredYaw = findReferenceYaw(searchYawStart);

            if (yawCalibrated()) { flightState = SETTING; }
            break;

        case SETTING:
            if (yaw == desiredYaw) {
                desiredAltitude = ALTITUDE_HOVER;   // TODO: remove magic number
                flightState = FLYING;
            }
            break;

        case FLYING:
            pollControls();

            // Check state of each button, and actuate
            if (upButtonPushed()) {
                increaseDesiredAltitude(&desiredAltitude);
            }
            if (downButtonPushed()) {
                decreaseDesiredAltitude(&desiredAltitude);
            }
            if (rightButtonPushed()) {
                increaseDesiredYaw(&desiredYaw);
            }
            if (leftButtonPushed()) {
                decreaseDesiredYaw(&desiredYaw);
            }

            // Check if it should move to next state
            if (!switchIsUp()) { flightState = LANDING_TURN; }
            break;

        case LANDING_TURN:
            // Move to reference yaw
            desiredYaw = getReferenceYaw();
            if (yaw == desiredYaw) {
                desiredAltitude = ALTITUDE_MIN;
                flightState = LANDING; }
            break;

        case LANDING:
            // Move to landing position and ignore controls
            if (altitude == ALTITUDE_MIN && yaw < ((2 + desiredYaw)% 360)) {
                flightState = LANDED;
                disablePWM();
                SysCtlReset();

            }
            break;

        case LANDED:
            // Set altitude to min
            disablePWM();

            // Check if next state
            if (switchIsUp()) {
                flightState = LAUNCHING;
                resetAccumulatedIntegral();
                desiredYaw = yaw;
                searchYawStart = yaw;
                enablePWM();

            }
            break;
    }
}


//*****************************************************************************
// Task for sending serial data.
//*****************************************************************************
void sendSerialData() {
    sendData(altitude, desiredAltitude, yaw, desiredYaw, mainDuty, tailDuty, flightState);
}


//*****************************************************************************
// Main function.
//*****************************************************************************
int main(void)
{
    // Initialise all systems
    init();

    // Main process
    Process processes[NUM_TASKS] = {
         {*runController, KERNEL_MAX_RATE},
         {*refreshDisplay, 4},
         {*checkControls, 100},
         {*sendSerialData, 5}
    };
    runKernel(processes, NUM_TASKS);
}
