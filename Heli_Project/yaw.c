/*
 * yaw.c
 *
 *  Created on: 23/04/2024
 *      Author: jmi145, jed96
 */

#include "yaw.h"


void quadratureIntHandler(void) {
    int32_t encoder_A = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
    int32_t encoder_B = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);

    GPIOIntClear(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1);

    //Get phase for quad phase machine
    prev_phase = cur_phase;
    if (encoder_A == 0 && encoder_B == 0) {
        cur_phase = 1;
    } else if(encoder_A == 0 && encoder_B == 1) {
        cur_phase = 2;
    } else if(encoder_A == 1 && encoder_B == 1) {
        cur_phase = 3;
    } else if(encoder_A == 1 && encoder_B == 0) {
        cur_phase = 4;
    }
    calculateYaw(prev_phase, cur_phase);

}

void calculateYaw(int32_t prev_phase, int32_t cur_phase) {
    // Increase of decrease yaw based on phase
    if ( prev_phase == 1 && cur_phase == 4) {
        yaw --;
    } else if (prev_phase == 4 && cur_phase == 1) {
        yaw ++;
    } else if (prev_phase < cur_phase) {
        yaw ++;
    } else if (prev_phase > cur_phase) {
        yaw --;
    }
}

void refYawIntHandler(void) {
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4);
    yawCalibrated = true;
    yaw = 0;
}

void initYaw(void) {
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntRegister(GPIO_PORTB_BASE, quadratureIntHandler);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    IntEnable(INT_GPIOB);

    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntRegister(GPIO_PORTC_BASE, refYawIntHandler);
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4);
    IntEnable(INT_GPIOC);
}

int32_t calcYawDegrees() {
    int32_t degrees;
    degrees = degConversion * yaw; // (/100)
    return degrees;
}







