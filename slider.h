#ifndef SLIDER_H_
#define SLIDER_H_

// *******************************************************
//
// slider.h
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
//
// *******************************************************

//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
// Constants
//*****************************************************************************
#define SW1_PIN  GPIO_PIN_7

//*****************************************************************************
// Enumeration for the slider states.
//*****************************************************************************
enum sliderStates {SLIDER_DOWN = 0, SLIDER_UP};


//*****************************************************************************
// Function declarations
//*****************************************************************************
void initSlider(void);
void updateSlider();
bool getSliderState();

#endif /*SLIDER_H_*/
