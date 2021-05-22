#ifndef CONTROLS_H_
#define CONTROLS_H_

// *******************************************************
//
// controls.h
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
//
// *******************************************************

#include <stdbool.h>

//*****************************************************************************
// Functions
//*****************************************************************************
void initControls();
void pollControls();
bool upButtonPushed();
bool downButtonPushed();
bool leftButtonPushed();
bool rightButtonPushed();
bool switchIsUp();

#endif /*CONTROLS_H_*/
