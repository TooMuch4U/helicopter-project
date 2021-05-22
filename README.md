# ENCE361 Helicopter Project
An embedded systems project for the The TI Tiva C Series TM4C123G.

## About
### Goal
The goal of this project was to program a remote controlled helicopter so that a “pilot” can fly a sortie, comprised of the following component tasks:
- find a direction in which to take-off,
- rise to an altitude where stable, manoeuvrable flight can be maintained,
- rotate around a fixed position and over several incremental steps,
- land back at base, “parking” in a home position.

### Contributors
- Tom Rizzi
- Euan Robinson
- Satwik Meravanage

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

