// *******************************************************
//
// controls.c
//
// Support for checking the state of controls on the Tiva,
// including the UP, DOWN, LEFT, RIGHT, RESET buttons, and
// the slider switch.
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
//
// *******************************************************

#include <buttons.h>
#include <stdbool.h>
#include <stdint.h>
#include "controls.h"
#include "slider.h"


//*****************************************************************************
// Polls the buttons and switches
//*****************************************************************************
void pollControls() {
    updateButtons();
    updateSlider();
}


//*****************************************************************************
// Initialises the button and switch controls.
//*****************************************************************************
void initControls() {
    initButtons();
    initSlider();
}


//*****************************************************************************
// Return true if the up button is pushed.
//*****************************************************************************
bool upButtonPushed() {
    uint8_t butState = checkButton (UP);
    return butState == PUSHED;
}


//*****************************************************************************
// Return true if the down button is pushed.
//*****************************************************************************
bool downButtonPushed() {
    uint8_t butState = checkButton (DOWN);
    return butState == PUSHED;
}


//*****************************************************************************
// Return true if the left button is pushed.
//*****************************************************************************
bool leftButtonPushed() {
    uint8_t butState = checkButton (LEFT);
    return butState == PUSHED;
}


//*****************************************************************************
// Return true if the right button is pushed.
//*****************************************************************************
bool rightButtonPushed() {
    uint8_t butState = checkButton (RIGHT);
    return butState == PUSHED;
}


//*****************************************************************************
// Return true if the switch is in the up state.
//*****************************************************************************
bool switchIsUp() {
    return getSliderState() == SLIDER_UP;
}

//*****************************************************************************
// Return true if the RESET switch is detected.
//*****************************************************************************
bool resetButtonPushed() {
    uint8_t butstate = checkButton(RESET);
    return butstate == PUSHED;
}

