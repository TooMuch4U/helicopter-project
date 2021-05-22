#ifndef ALTITUDE_H_
#define ALTITUDE_H_

// *******************************************************
//
// altitude.h
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
//
// *******************************************************


//*****************************************************************************
// Includes
//*****************************************************************************
#include <stdint.h>

//*****************************************************************************
// Constants
//*****************************************************************************
#define BUF_SIZE 20                 // Circular buffer size
#define SAMPLE_RATE_HZ 5000         // Interrupt sampling rate
#define DISPLAY_PERCENT 0           // Display percentage mode state
#define DISPLAY_ADC 1               // Display ADC mode state
#define DISPLAY_OFF 2               // No display mode state
#define DISPLAY_MODES_COUNT 3       // Number of display modes
#define ADC_ONE_VOLT 1200           // ADC step size equivalent to 1 V
#define ALTITUDE_VOLTAGE_RANGE 1

#define ALTITUDE_MAX 100            // 100%
#define ALTITUDE_MIN 0              // 0%, etc...
#define ALTITUDE_INCREMENT 10
#define ALTITUDE_HOVER 10

//*****************************************************************************
// Functions
//*****************************************************************************

void SysTickIntHandler(void);
void ADCIntHandler(void);
void initADC (void);
void initAltitude(void);
void takeLandedSample (void);
int32_t adcToPercentage(uint32_t adcValue);
uint32_t getAltitudeADC(void);
int32_t getAltitudePercent(void);

#endif /*ALTITUDE_H_*/
