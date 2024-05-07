/*
 * tailRotor.c
 *
 *  Created on: 7/05/2024
 *      Author: jmi145
 */
#include "tailRotor.h"
static uint32_t ui32Freq = PWM_TAIL_START_RATE_HZ;
static uint32_t ui32Duty = PWM_TAIL_START_DUTY_PER;
void
initialisePWM2 (void) //Make tail rotor pwm
{
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);

    GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);

    PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    // Set the initial PWM parameters
    setPWM2 (PWM_TAIL_START_RATE_HZ, PWM_TAIL_START_DUTY_PER);

    PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);
}

void
setPWM2 (uint32_t ui32Freq, uint32_t ui32Duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period =
        SysCtlClockGet() / PWM_DIVIDER / ui32Freq;

    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, ui32Period);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM,
        ui32Period * ui32Duty / 100);
}

void initTail (void) {
    // As a precaution, make sure that the peripherals used are reset
    SysCtlPeripheralReset (PWM_TAIL_PERIPH_GPIO);
    SysCtlPeripheralReset (PWM_TAIL_PERIPH_PWM);
    SysCtlPeripheralReset (LEFT_BUT_PERIPH);      // LEFT button GPIO
    SysCtlPeripheralReset (RIGHT_BUT_PERIPH);     // RIGHT button GPIO
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);
}

void controlTailGoal (void) {
    //Background task: Up and down buttons alter duty cycle by 5% respectively
    if ((checkButton (RIGHT) == PUSHED) && (ui32Duty < PWM_DUTY_MAX_PER))
    {
        ui32Duty += PWM_DUTY_STEP_PER;
        setPWM2 (ui32Freq, ui32Duty);
    }
    if ((checkButton (LEFT) == PUSHED) && (ui32Duty > PWM_DUTY_MIN_PER))
    {
        ui32Duty -= PWM_DUTY_STEP_PER;
        setPWM2 (ui32Freq, ui32Duty);

    }
}


