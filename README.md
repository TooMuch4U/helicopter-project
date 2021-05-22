# ENCE361 'Helicopter' Project README
# Group 'HeliFriAMGroup2'
# Authors: Tom Rizzi, Euan Robinson, Satwik Meravanage

Main file is 'main.c'. Support modules include:
altitude.c - Support for returning altitude metrics
buttons.c - Support for button-input controls of the helicopter
circBufT.c - Support for storing altitude ADC readings into a circular buffer
controls.c - Support for buttons and slider switch
display.c - Support for the Orbit OLED display, and displaying helicopter metrics
flightStates.c - Support for a finite state machine for different helicopter flight mides
kernel.c - Support for round-robin execution of the program
rotors.c - Support for main and tail rotor PWM control
serial.c - Support for returning helicopter metrics via a serial comm
slider.c - Support for Tiva slider switch
timings.c - Support for a timer
yaw.c - Support for returning yaw metrics
tm4c123gh6pm_startup_ccs.c - TivaWare for Launchpad system
ustdlib.c - Standard Texas Instruments function library

All source files are located in `Workspace/Week4Lab/`.
Designated CCS workspace is `Workspace`.

## Checklist
A shortened version of the project specification to be used as a checklist.

1. [x] The 2-channel quadrature signal for the helicopter yaw.
2. [x] helicopter altitude should be displayed as a percentage on the Orbit OLED board.
3. [x] Two output PWM signals should be generated, one for the main rotor and one for the tail rotor.
4. [x] The SW1 slider switch on the Orbit board should control the mode of the helicopter.
    - [x] The helicopter should land smoothly, facing the reference orientation.
    - [x] At take off the helicopter should start to hover.
    - [x] With the main rotor near the hover point, the helicopter should rotate to a known
    reference position.
    - [x] While the helicopter is landing any change to the switch position should be
    ignored until landing is complete.
    - [x] When the program starts the helicopter should be in landed mode with the motors off,
    regardless of the slider position.
5. [x] The four user-programmable buttons of the TIVA-Orbit board should be
programmed to interactively control the altitude and yaw of the helicopter.
    - [x] Each operation of the UP button on the Orbit board should cause the helicopter
    to increase its altitude by 10% of its total vertical range.
    - [x] Each operation of the DOWN button on the Orbit board should cause the
    helicopter to decrease its altitude by 10% of its total vertical range.
    - [ ] Neither the UP nor the DOWN button should affect the yaw of the helicopter.
    - [x] Each operation of the CCW (left) button should cause the helicopter to rotate 15º counterclockwise.
    - [x] Each operation of the CW (right) button should cause the helicopter to rotate 15º clockwise.
    - [ ] Neither the CCW nor the CW button should affect the height of the helicopter.
6. [x] The program should respond to pushbutton signals that are asserted for 200 ms for each “push”;
only one button can be operated at a time.
7. [x] Operation of the virtual RESET button (active LOW) invokes a call to the API function
SysCtlReset() (prototype in driverlib/sysctl.h).
8. [x] The program should have a real-time foreground/background kernel operating on a
round-robin basis for background tasks. Robust behaviour should be maintained at
all times.
9. [x] A PI(D) controller with a constant offset should be implemented to achieve controlled “flight”.
10. [x] Information on the status of the helicopter should be transmitted via a serial link.
Updates should be transmitted at regular intervals (no fewer than 4 updates per second).

