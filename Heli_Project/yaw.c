/*
 * yaw.c
 *
 *  Created on: 23/04/2024
 *      Author: jmi145, jed96
 */

#include "yaw.h"


void quadratureIntHandler(void) {
    // Initialize the input sensors and clears any pending interupts on the pins to prevent false interupts
    int32_t encoder_A = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
    int32_t encoder_B = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);
    GPIOIntClear(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1);

    //Gets the current phase of the state machine depending on the states of the 2 sensors (encoder A and encoder B)
    prev_phase = cur_phase;
    if (!encoder_A && !encoder_B) {
        cur_phase = 1;
    } else if(!encoder_A && encoder_B) {
        cur_phase = 2;
    } else if(encoder_A && encoder_B) {
        cur_phase = 3;
    } else if(encoder_A && !encoder_B) {
        cur_phase = 4;
    }
    //Alter yaw based on the current and previous yaw
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


void initYaw(void) {
    //Initialize the GPIO peripherals used for sensing yaw as well as the interupt handlers.
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(YAW_REF_PERIPH);

    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntRegister(GPIO_PORTB_BASE, quadratureIntHandler);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //Initialise the reference yaw interrupt

    GPIOPadConfigSet (YAW_REF_PORT_BASE, YAW_REF_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntRegister(YAW_REF_PORT_BASE, RefYawIntHandler);
    GPIOIntTypeSet(YAW_REF_PORT_BASE, YAW_REF_PIN, GPIO_RISING_EDGE);
    GPIOIntEnable(YAW_REF_PORT_BASE, YAW_REF_PIN);
    IntEnable(INT_GPIOC);
}

int32_t calcYawDegrees() {
    /* Calculate yaw in degrees - This is done by multiplying the yaw value by 321. There are 112 teeth, so 321*112 gives approximately 36,000.
     By multiplying the number number of teeth that have been passed by 321, we get a number in degrees which can then be split up into integer and 
     decimal parts which gives an accurate reading of the yaw in degrees */
    int32_t degrees;
    degrees = degConversion * yaw;
    if (yaw >= 224) {
        yaw -= 448;
    } else if (yaw < -224 ) {
        yaw += 448;
    }
    return degrees / 4;
}
