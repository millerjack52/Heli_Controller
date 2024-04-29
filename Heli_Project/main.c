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
//
// The interrupt handler for the for SysTick interrupt.
//
//*****************************************************************************

//*****************************************************************************
// Global variables
//*****************************************************************************
static circBuf_t g_inBuffer;        // Buffer of size BUF_SIZE integers (sample values)
static uint32_t g_ulSampCnt;    // Counter for the interrupts
static int32_t g_heliLandedAlt = 1240; //Value for the reading at heli landed alitutude

//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt.
//
//*****************************************************************************
void
SysTickIntHandler(void)
{

    ADCProcessorTrigger(ADC0_BASE, 3);
    g_ulSampCnt++;

}

void
ADCIntHandler(void)
{
    uint32_t ulValue;

    //
    // Get the single sample from ADC0.  ADC_BASE is defined in
    // inc/hw_memmap.h
    ADCSequenceDataGet(ADC0_BASE, 3, &ulValue);
    //
    // Place it in the circular buffer (advancing write index)
    writeCircBuf (&g_inBuffer, ulValue);
    //
    // Clean up, clearing the interrupt
    ADCIntClear(ADC0_BASE, 3);
}


void
initADC (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntRegister (ADC0_BASE, 3, ADCIntHandler);
    ADCIntEnable(ADC0_BASE, 3);
}


//*****************************************************************************
// Initialisation functions for the clock (incl. SysTick), ADC, display
//*****************************************************************************
void
initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    //
    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister(SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable();
    SysTickEnable();
}

void
initDisplay (void)
{
    // intialise the Orbit OLED display
    OLEDInitialise ();
}

void display(int32_t percentVal, int32_t degrees) {
    char string[MAX_STR_LEN];
    //Extract integer and decimal parts from the degrees calculation, and then display the altitude percentage and the yaw value.
    int32_t integerPart = degrees / 100;
    int32_t decimalPart = abs(degrees % 100);

    usnprintf (string, sizeof(string), "Altitude = %4d%%", percentVal);
    OLEDStringDraw (string, 0, 1);

    if (decimalPart < 10) {
        usnprintf (string, sizeof(string), "Yaw = %3d.0%1d deg", integerPart, decimalPart);
        OLEDStringDraw (string, 0, 2);
    } else {
        usnprintf (string, sizeof(string), "Yaw = %3d.%2d deg", integerPart, decimalPart);
        OLEDStringDraw (string, 0, 2);
    }


}

void
groundSet(void)
{
    int16_t i;
    int32_t sum;
    sum = 0;
    for (i = 0; i < BUF_SIZE; i++)
        sum = sum + readCircBuf (&g_inBuffer);
    // Calculate and display the rounded mean of the buffer contents
    g_heliLandedAlt = (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;
}

int calculateAltitudePercentage(int32_t adcValue, int32_t landedAltitude) {

    int32_t altitudePercentage = (((g_heliLandedAlt - adcValue) * 10) / 124);
    return altitudePercentage;
}



int main(void) {

    int16_t i;
    int32_t sum;
    int32_t meanVal;
    int32_t percentAlt;
    int32_t degrees;

    SysCtlPeripheralReset (UP_BUT_PERIPH);        // UP button GPIO
    SysCtlPeripheralReset (LEFT_BUT_PERIPH);        // LEFT button GPIO

    initYaw();
    initClock ();
    initADC ();
    initDisplay ();
    initCircBuf (&g_inBuffer, BUF_SIZE);
    initButtons ();
    IntMasterEnable();
    SysCtlDelay (SysCtlClockGet() / 6);
    groundSet();
    percentAlt = 0;
    display(percentAlt, 0);
    while (1)

    {
        updateButtons();

        if ((checkButton (LEFT) == PUSHED)) {
            groundSet();
        }
        sum = 0;
        for (i = 0; i < BUF_SIZE; i++)
            sum = sum + readCircBuf (&g_inBuffer);
        // Calculate and display the rounded mean of the buffer contents
        meanVal = (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;

        percentAlt = calculateAltitudePercentage(meanVal, g_heliLandedAlt);
        degrees = calcYawDegrees();

        display(percentAlt, degrees);
    }
}



