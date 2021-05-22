// *******************************************************
//
// altitude.c
//
// Support for reading the altitude of the helicopter via
// use of ADC as a percentage height.
// Code modified from ENCE361 laboratory source code.
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
//
// *******************************************************


//*****************************************************************************
// Includes
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "utils/ustdlib.h"
#include "circBufT.h"
#include "inc/hw_ints.h"
#include "stdlib.h"
#include "altitude.h"


//*****************************************************************************
// Globals to module
//*****************************************************************************
static circBuf_t g_inBuffer;            // Circular buffer of size BUF_SIZE integers (sample values)
static uint32_t g_landedSample = 0;     // Initial sample for the helicopter 'landed' altitude
static uint32_t g_ulSampCnt;        // Counter for the interrupts


//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt
//
//*****************************************************************************
void
SysTickIntHandler(void)
{
    // Initiate an ADC conversion
    ADCProcessorTrigger(ADC0_BASE, 3);
    g_ulSampCnt++;
}


//*****************************************************************************
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//*****************************************************************************
void ADCIntHandler(void)
{
    uint32_t ulValue;

    // Get the single sample from ADC0.  ADC_BASE is defined in inc/hw_memmap.h
    ADCSequenceDataGet(ADC0_BASE, 3, &ulValue);

    // Place it in the circular buffer (advancing write index)
    writeCircBuf (&g_inBuffer, ulValue);

    // Clean up, clearing the interrupt
    ADCIntClear(ADC0_BASE, 3);
}


//*****************************************************************************
// Initialises ADC-related peripherals.
//*****************************************************************************
void initADC (void)
{
    // The ADC0 peripheral must be enabled for configuration and use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);

    // Since sample sequence 3 is now configured, it must be enabled.
    ADCSequenceEnable(ADC0_BASE, 3);

    // Register the interrupt handler
    ADCIntRegister (ADC0_BASE, 3, ADCIntHandler);

    // Enable interrupts for ADC0 sequence 3 (clears any outstanding interrupts)
    ADCIntEnable(ADC0_BASE, 3);
}


//*****************************************************************************
// Initialises everything needed for the altitude module.
//*****************************************************************************
void initAltitude(void) {
    initADC();
    initCircBuf (&g_inBuffer, BUF_SIZE);

    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister(SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable();
    SysTickEnable();
}


//*****************************************************************************
// Function to take the mean ADC value and assign to the landed sample variable
//*****************************************************************************
void takeLandedSample (void)
{
    g_landedSample = getAltitudeADC();
}


//*****************************************************************************
// Function to calculate the ADC as a percentage.
//*****************************************************************************
int32_t adcToPercentage(uint32_t adcValue)
{
    // Calculate the ADC value relative to what it should be when landed
    int relativeValue = g_landedSample - adcValue;

    // Calculate the percentage
    int percent = ((relativeValue * 100) / (ADC_ONE_VOLT * ALTITUDE_VOLTAGE_RANGE));
    return percent;
}


//*****************************************************************************
// Returns the current average ADC value of the altitude
//*****************************************************************************
uint32_t getAltitudeADC(void)
{
    uint32_t sum = 0;
    uint32_t i = 0;
    for (i = 0; i < BUF_SIZE; i++) {
        sum = sum + readCircBuf (&g_inBuffer);
    }

    // Calculate and display the rounded mean of the buffer contents
    return (2 * sum + BUF_SIZE)/ 2 / BUF_SIZE;
}


//*****************************************************************************
// Returns percentage of the current average ADC value in proportion to max height
//*****************************************************************************
int32_t getAltitudePercent(void)
{
    uint32_t adc = getAltitudeADC();
    int32_t percent = adcToPercentage(adc);
    return percent;
}

