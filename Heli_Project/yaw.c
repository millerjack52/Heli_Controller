/*
 * yaw.c
 *
 *  Created on: 23/04/2024
 *      Author: jmi145, jed96
 */
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

void quadratureIntHandler(void) {
    int32_t encoder_A = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
    int32_t encoder_B = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);

    GPIOIntClear(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1);

    //get phase for quad phase machine
    prev_phase = cur_phase;
    if (encoder_A == 0 && encoder_B == 0) {
        cur_phase = 1
    } else if(encoder_A == 0 && encoder_B == 1) {
        cur_phase = 2
    } else if(encoder_A == 1 && encoder_B == 1) {
        cur_phase = 3
    } else if(encoder_A == 1 && encoder_B == 0) {
        cur_phase = 4
    }
    yaw = calculateYaw(prev_phase, cur_phase)




}




