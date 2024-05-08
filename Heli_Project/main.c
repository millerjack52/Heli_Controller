//********************************************************
//
// main.c - main project code for ENCE361 helicopter project
//
// Link with modules:  buttons2, OrbitOLEDInterface, circBufT
//
// Author:  P.J. Bones  UCECE
// Last modified:   16.4.2018
//

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
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "buttons4.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "circBufT.h"
#include "yaw.h"
#include "rotors.h"
//#include "altitude.h"


//********************************************************
// Constants
//********************************************************
#define SYSTICK_RATE_HZ 100
#define SLOWTICK_RATE_HZ 4
#define MAX_STR_LEN 17
//---USB Serial comms: UART0, Rx:PA0 , Tx:PA1
#define BAUD_RATE 9600
#define UART_USB_BASE           UART0_BASE
#define UART_USB_PERIPH_UART    SYSCTL_PERIPH_UART0
#define UART_USB_PERIPH_GPIO    SYSCTL_PERIPH_GPIOA
#define UART_USB_GPIO_BASE      GPIO_PORTA_BASE
#define UART_USB_GPIO_PIN_RX    GPIO_PIN_0
#define UART_USB_GPIO_PIN_TX    GPIO_PIN_1
#define UART_USB_GPIO_PINS      UART_USB_GPIO_PIN_RX | UART_USB_GPIO_PIN_TX
#define BUF_SIZE 12
#define SAMPLE_RATE_HZ 48


//*****************************************************************************
// Global variables
//*****************************************************************************
static circBuf_t g_inBuffer;        // Buffer of size BUF_SIZE integers (sample values)
static uint32_t g_ulSampCnt;    // Counter for the interrupts
static int32_t g_heliLandedAlt = 1240; //Value for the reading at heli landed alitutude

// The interrupt handler for the for SysTick interrupt.
void SysTickIntHandler(void) {
    ADCProcessorTrigger(ADC0_BASE, 3);
    g_ulSampCnt++;
}

void ADCIntHandler(void) {
    // Get the single sample from ADC0, place it in a circular buffer and clear the interupt.
    uint32_t ulValue;
    ADCSequenceDataGet(ADC0_BASE, 3, &ulValue);
    writeCircBuf (&g_inBuffer, ulValue);
    ADCIntClear(ADC0_BASE, 3);
}

void initADC (void) {
    //Initialize the ADC
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntRegister (ADC0_BASE, 3, ADCIntHandler);
    ADCIntEnable(ADC0_BASE, 3);
}

void initClock (void) {
    // Set the clock rate to 20 MHz, set up the period for the timer and enable the service and interupt
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    SysTickIntRegister(SysTickIntHandler);
    SysTickIntEnable();
    SysTickEnable();
}

void initDisplay (void) {
    OLEDInitialise ();
}

void display(int32_t percentVal, int32_t degrees, int32_t AltTarget, int32_t degTarget) {
    char string[MAX_STR_LEN];
    //Extract integer and decimal parts from the degrees calculation, and then display the altitude percentage and the yaw value.
    int32_t integerPart = degrees / 100;
    int32_t decimalPart = abs(degrees % 100);

    // Display the altitude percentage
    usnprintf (string, sizeof(string), "Altitude = %4d%%", percentVal);
    OLEDStringDraw (string, 0, 0);



    /* Displays the yaw value in degrees, and checks to see if the decimal part 
    is less than 10 (0.1 in actual terms) and if it is, adds a 0 in front of it, so it is displayed correctly. */
    if (decimalPart < 10) {
        usnprintf (string, sizeof(string), "Yaw = %3d.0%1d deg", integerPart, decimalPart);
        OLEDStringDraw (string, 0, 1);
    } else {
        usnprintf (string, sizeof(string), "Yaw = %3d.%2d deg", integerPart, decimalPart);
        OLEDStringDraw (string, 0, 1);
    }

    usnprintf (string, sizeof(string), "Tgt Alt = %3d%%", AltTarget);
    OLEDStringDraw (string, 0, 2);

    usnprintf (string, sizeof(string), "Tgt Deg = %3d deg", degTarget);
    OLEDStringDraw (string, 0, 3);
}

void groundSet(void) {
    // resets the ground value for the altitude reading so the current height is read as 0%.
    int16_t i;
    int32_t sum;
    sum = 0;
    for (i = 0; i < BUF_SIZE; i++)
        sum = sum + readCircBuf (&g_inBuffer);
    // Calculate and display the rounded mean of the buffer contents
    g_heliLandedAlt = (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;
}

int calculateAltitudePercentage(int32_t adcValue, int32_t landedAltitude) {
    // Calculates the percentage of altitude the heli is currently at.
    int32_t altitudePercentage = (((g_heliLandedAlt - adcValue) * 10) / 124);
    return altitudePercentage;
}

void updateLandedState(float deltaTime) {
    mainDuty = MainRotorControlUpdate(targetAlt, percentAlt, deltaTime);
    tailDuty = TailRotorControlUpdate(targetYawDeg, degrees, deltaTime);
    mainPWM();
    tailPWM();

    // Check for the rising edge of SWITCH1
    if (checkButton(SWITCH1) == PUSHED) {
        currentState = TAKEOFF;
        stateChange = true;
    }
}

void updateTakeoffState(float deltaTime) {
    if (stateChange) {
        stateChange = false;
        yawCalibrated = false;
    }

    if (!yawCalibrated) {
        targetAlt = 10;
        DisableRefYawInt(false); // enable ref yaw interrupt
        targetYawDeg = yaw + 5;
        mainDuty = controlMain(targetAlt, percentAlt, deltaTime);
        tailDuty = controlTail(targetYawDeg, degrees, deltaTime);
        SetMainPWM(ui32DutyMain);
        SetTailPWM(ui32DutyTail);
    } else {
        atHoverLevel = true;
        DisableRefYawInt(true); // disable ref yaw interrupt
        currentState = FLYING;
        targetYawDeg = 0;
        stateChange = true;
    }

    // Check for the falling edge of SWITCH1
    if (checkButton(SWITCH1) == RELEASED) {
        currentState = LANDING;
        stateChange = true;
    }
}

void updateFlyingState() {
    atHoverLevel = false;

    if (checkButton(UP) == PUSHED) {
        targetAlt = (targetAlt < 90) ? targetAlt + 10 : 100;
    }

    if (checkButton(DOWN) == PUSHED) {
        targetAlt = (targetAlt > 10) ? targetAlt - 10 : 10;
        if (targetAlt == 10) {
            currentState = LANDING;
            stateChange = true;
        }
    }

    if (checkButton(LEFT) == PUSHED) {
        targetYawDeg -= 10;
        if (targetYawDeg < -179) {
            targetYawDeg += 360;
        }
    }

    if (checkButton(RIGHT) == PUSHED) {
        targetYawDeg += 10;
        if (targetYawDeg > 180) {
            targetYawDeg -= 360;
        }
    }

    // Check for the falling edge of SWITCH1
    if (checkButton(SWITCH1) == RELEASED) {
        currentState = LANDING;
        targetAlt = 10;
        stateChange = true;
    }

    ui32DutyMain = mainPWM(targetAlt, percentAlt, deltaTime);
    ui32DutyTail = tailPWM(targetYawDeg, degrees, deltaTime);
    SetMainPWM(ui32DutyMain);
    SetTailPWM(ui32DutyTail);
}

void updateLandingState() {
    if (targetYawDeg < 0) {
        targetYawDeg = degrees + 1;
    } else if (targetYawDeg > 0) {
        targetYawDeg = degrees - 1;
    }

    if (currentYaw == 0 && altPercentage == 10) {
        atHoverLevel = true;
    }

    if (atHoverLevel) {
        targetAlt = altPercentage - 1;
    }

    if (currentYaw == 0 && currentAltitudePercentage == 0) {
        currentState = LANDED;
    }

    mainDuty = MainRotorControlUpdate(targetAlt, percentAlt, deltaTime);
    tailDuty = TailRotorControlUpdate(targetYawDeg, degrees, deltaTime);
    SetMainPWM(mainDuty);
    SetTailPWM(tailDuty);
}

int main(void) {
    //Defines vars to be used in main.

    int16_t i;
    int32_t sum;
    int32_t meanVal;
    int32_t percentAlt;
    int32_t degrees;
    int32_t targetAlt;
    int32_t targetYawDeg;
    

    SysCtlPeripheralReset (UP_BUT_PERIPH);        // UP button GPIO
    SysCtlPeripheralReset (DOWN_BUT_PERIPH);      // DOWN button GPIO
    SysCtlPeripheralReset (LEFT_BUT_PERIPH);      // LEFT button GPIO
    SysCtlPeripheralReset (RIGHT_BUT_PERIPH);     // RIGHT button GPIO
    SysCtlPeripheralReset (SWITCH1_BUT_PERIPH);   // SWITCH1 switch GPIO

    initYaw();
    initClock ();
    initADC ();
    initDisplay ();
    initCircBuf (&g_inBuffer, BUF_SIZE);

    initialisePWM ();
    initialisePWM2();
    initRotors();


    initButtons ();
    IntMasterEnable();
    SysCtlDelay (SysCtlClockGet() / 6);

    enum States {
        LANDED = 0,
        TAKEOFF,
        FLYING,
        LANDING
    }

    enum States currentState = LANDED;
    bool atHoverLevel;





    groundSet();
    percentAlt = 0;
    display(percentAlt, 0, 0, 0);
    while (1) {

    updateButtons();
    targetYawDeg = controlTailGoal(targetYawDeg);
    targetAlt = controlMainGoal(targetAlt);

    switch (currentState) {
        case LANDED:
            updateLandedState(deltaTime);
            break;
        case TAKEOFF:
            updateTakeoffState(deltaTime);
            break;
        case FLYING:
            updateFlyingState();
            break;
        case LANDING:
            updateLandingState();
            break;
}

        // Check for the falling edge of SWITCH

        //Calculates initial avg ADC value.
        sum = 0;
        for (i = 0; i < BUF_SIZE; i++)
            sum = sum + readCircBuf (&g_inBuffer);
        // Calculate and display the rounded mean of the buffer contents
        meanVal = (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;

        percentAlt = calculateAltitudePercentage(meanVal, g_heliLandedAlt);
        degrees = calcYawDegrees();

        display(percentAlt, degrees, targetAlt, targetYawDeg);
    }



}



