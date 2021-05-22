#ifndef YAW_H_
#define YAW_H_

//*****************************************************************************
//
// yaw.h
//
// Authors:  T. Rizzi, E. Robinson, S. Meravanage
// Last modified: 21 May
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"

//*****************************************************************************
// Defines
//*****************************************************************************

// GPIO Pin Defines
#define YAW_PORT_BASE GPIO_PORTB_BASE
#define YAW_INT_PIN_A GPIO_INT_PIN_0
#define YAW_INT_PIN_B GPIO_INT_PIN_1
#define YAW_INT_BASE INT_GPIOB
#define YAW_PIN_A GPIO_PIN_0            // Quadrature A pin
#define YAW_PIN_B GPIO_PIN_1            // Quadrature B pin
#define YAW_PINS YAW_PIN_A | YAW_PIN_B  // Both quadrature pins

// Yaw-related constants
#define YAW_COMPASS_DIR -1      // Determines +ve direction of rotation. -1 for CW, +1 for CCW
#define YAW_NOTCHES_MAX 448     // Number of notches on quadrature encoder disc
#define YAW_ANGLE_MAX 360
#define YAW_ANGLE_INCREMENT 15

// Yaw reference pin
#define YAW_REF_PORT_BASE GPIO_PORTC_BASE
#define YAW_REF_PIN  GPIO_PIN_4


//*****************************************************************************
// Function declarations
//*****************************************************************************
void YawIntHandler(void);
void initYaw(void);
void updateQuadEncoder(int32_t yawPinsInput);
uint32_t getYaw(void);
bool yawCalibrated();
uint32_t getReferenceYaw();
uint32_t findReferenceYaw();

void resetYawRef();
uint32_t findReferenceYaw(uint32_t startYaw);

#endif /*YAW_H_*/

