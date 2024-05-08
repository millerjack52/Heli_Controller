/*
 * yaw.h
 *
 *  Created on: 23/04/2024
 *      Author: jmi145, jed96
 */

#ifndef YAW_H_
#define YAW_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "buttons4.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"

static int8_t prev_phase;
static int8_t cur_phase;
static int32_t yaw;
static bool yawCalibrated = false;
static int32_t degConversion = 321;

void quadratureIntHandler(void);
void calculateYaw(int32_t prev_phase, int32_t cur_phase);
void refYawIntHandler(void);
void initYaw(void);
int32_t calcYawDegrees();



#endif /* YAW_H_ */
