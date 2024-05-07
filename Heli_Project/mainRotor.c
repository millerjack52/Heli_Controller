/*
 * mainRotor.c
 *
 *  Created on: 7/05/2024
 *      Author: jmi145, jed96
 */

//  PWM Hardware Details M0PWM7 (gen 3)
//  ---Main Rotor PWM: PC5, J4-05
#include "mainRotor.h"
static uint32_t ui32Freq = PWM_START_RATE_HZ;
static uint32_t ui32Duty = PWM_START_DUTY_PER;


void initialisePWM (void) {
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);

    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    // Set the initial PWM parameters
    setPWM (PWM_START_RATE_HZ, PWM_FIXED_DUTY);

    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
}

void setPWM (uint32_t ui32Freq, uint32_t ui32Duty) {
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period =
        SysCtlClockGet() / PWM_DIVIDER / ui32Freq;

    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, ui32Period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM,
        ui32Period * ui32Duty / 100);
}

void initMainRotor(void) {
    // As a precaution, make sure that the peripherals used are reset
    SysCtlPeripheralReset (PWM_MAIN_PERIPH_GPIO); // Used for PWM output
    SysCtlPeripheralReset (PWM_MAIN_PERIPH_PWM);  // Main Rotor PWM
    SysCtlPeripheralReset (UP_BUT_PERIPH);        // UP button GPIO
    SysCtlPeripheralReset (DOWN_BUT_PERIPH);      // DOWN button GPIO
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);

}

void controlMainGoal (void) {
    if ((checkButton (UP) == PUSHED) && (ui32Freq < PWM_DUTY_MAX_PER))
    {
        ui32Duty += PWM_DUTY_STEP_PER;
        setPWM (ui32Freq, ui32Duty);
    }
    if ((checkButton (DOWN) == PUSHED) && (ui32Freq > PWM_DUTY_MIN_PER))
    {
        ui32Duty -= PWM_DUTY_STEP_PER;
        setPWM (ui32Freq, ui32Duty);
    }
}



