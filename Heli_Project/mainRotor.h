/*
 * mainRotor.h
 *
 *  Created on: 7/05/2024
 *      Author: jmi145, jed96
 */

#ifndef MAINROTOR_H_
#define MAINROTOR_H_


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "buttons4.h"
#include "utils/ustdlib.h"
#include "stdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"

//  PWM Hardware Details M0PWM7 (gen 3)
//  ---Main Rotor PWM: PC5, J4-05
#define PWM_MAIN_BASE        PWM0_BASE
#define PWM_MAIN_GEN         PWM_GEN_3
#define PWM_MAIN_OUTNUM      PWM_OUT_7
#define PWM_MAIN_OUTBIT      PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM  SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE   GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN    GPIO_PIN_5

#define PWM_START_RATE_HZ  250
#define PWM_RATE_STEP_HZ   50
#define PWM_RATE_MIN_HZ    50
#define PWM_RATE_MAX_HZ    400
// Duty Cycle
#define PWM_START_DUTY_PER  50
#define PWM_DUTY_STEP_PER   5
#define PWM_DUTY_MIN_PER    5
#define PWM_DUTY_MAX_PER    95
#define PWM_FIXED_DUTY     67
#define PWM_DIVIDER_CODE   SYSCTL_PWMDIV_4
#define PWM_DIVIDER        4

void initialisePWM (void);
void setPWM (uint32_t u32Freq, uint32_t u32Duty);
void controlMainGoal(void);
void initMainRotor(void);


#endif /* MAINROTOR_H_ */
