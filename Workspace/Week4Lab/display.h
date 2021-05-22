#ifndef DISPLAY_H_
#define DISPLAY_H_

// ****************************************************************************
//
// display.h
//
// Authors:  T. Rizzi, E. Robinson, S. Meravanage
// Last modified: 21 May 2021
//
// ****************************************************************************

//*****************************************************************************
// Constants
//*****************************************************************************
#define DISPLAY_ALTITUDE_LINE 0
#define DISPLAY_YAW_LINE 1
#define DISPLAY_MAIN_DUTY_LINE 2
#define DISPLAY_TAIL_DUTY_LINE 3

//*****************************************************************************
// Functions
//*****************************************************************************
void displayClear(void);
void displayTitle(void);
void displayAltitude(int32_t altitude);
void displayYaw(uint32_t yaw);
void displayMainDuty(uint32_t mainDuty);
void displayTailDuty(uint32_t tailDuty);
void updateDisplay(int32_t altitude, uint32_t yaw, uint32_t mainDuty, uint32_t tailDuty);
void initDisplay(void);

#endif /* DISPLAY_H_ */
