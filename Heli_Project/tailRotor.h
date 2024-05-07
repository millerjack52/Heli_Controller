/*
 * tailRotor.h
 *
 *  Created on: 7/05/2024
 *      Author: jmi145
 */

#ifndef TAILROTOR_H_
#define TAILROTOR_H_


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

//  ---Tail Rotor PWM: PF1
#define PWM_TAIL_BASE        PWM1_BASE
#define PWM_TAIL_GEN         PWM_GEN_2
#define PWM_TAIL_OUTNUM      PWM_OUT_5
#define PWM_TAIL_OUTBIT      PWM_OUT_5_BIT
#define PWM_TAIL_PERIPH_PWM  SYSCTL_PERIPH_PWM1
#define PWM_TAIL_PERIPH_GPIO SYSCTL_PERIPH_GPIOF
#define PWM_TAIL_GPIO_BASE   GPIO_PORTF_BASE
#define PWM_TAIL_GPIO_CONFIG GPIO_PF1_M1PWM5
#define PWM_TAIL_GPIO_PIN    GPIO_PIN_1

#define PWM_TAIL_START_RATE_HZ 200
#define PWM_RATE_STEP_HZ   50
#define PWM_RATE_MIN_HZ    50
#define PWM_RATE_MAX_HZ    400
// Duty Cycle
#define PWM_TAIL_START_DUTY_PER 10
#define PWM_DUTY_STEP_PER   5
#define PWM_DUTY_MIN_PER    5
#define PWM_DUTY_MAX_PER    95
#define PWM_FIXED_DUTY     67
#define PWM_DIVIDER_CODE   SYSCTL_PWMDIV_4
#define PWM_DIVIDER        4

void initialisePWM2 (void);
void setPWM2 (uint32_t u32Freq, uint32_t u32Duty);
void controlTailGoal (void);
void initTail(void);

#endif /* TAILROTOR_H_ */
