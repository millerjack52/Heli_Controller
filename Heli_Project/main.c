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


//********************************************************
// Constants
//********************************************************
#define SYSTICK_RATE_HZ 100
#define SLOWTICK_RATE_HZ 4
#define MAX_STR_LEN 16
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
static uint32_t g_heliLandedAlt = 1240; //Value for the reading at heli landed alitutude
static uint32_t g_heliMaxHeightAlt = 0; //Value for the reading at heli max alitutude

//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt.
//
//*****************************************************************************
void
SysTickIntHandler(void)
{
    //
    // Initiate a conversion
    //

    ADCProcessorTrigger(ADC0_BASE, 3);
    g_ulSampCnt++;

}

//*****************************************************************************
//
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//
//*****************************************************************************
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
initADC (void)
{
    //
    // The ADC0 peripheral must be enabled for configuration and use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    ADCSequenceEnable(ADC0_BASE, 3);

    //
    // Register the interrupt handler
    ADCIntRegister (ADC0_BASE, 3, ADCIntHandler);

    //
    // Enable interrupts for ADC0 sequence 3 (clears any outstanding interrupts)
    ADCIntEnable(ADC0_BASE, 3);
}

void
initDisplay (void)
{
    // intialise the Orbit OLED display
    OLEDInitialise ();
}

//*****************************************************************************
//
// Function to display the mean ADC value (10-bit value, note) and sample count.
//
//*****************************************************************************

dispBlank()
{
    OLEDStringDraw ("                 ", 0, 0);
    OLEDStringDraw ("                 ", 0, 1);
}

void
displayPercentAlt(uint32_t percentVal)
{
    dispBlank();
    char string[17];  // 16 characters across the display

    OLEDStringDraw ("Percent Alt", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.

    usnprintf (string, sizeof(string), "Percent = %3d", percentVal);
    OLEDStringDraw (string, 0, 1);

}

void
displayMeanAdc(uint32_t meanVal)
{
    dispBlank();
    char string[17];  // 16 characters across the display

    OLEDStringDraw ("Mean ADC val", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.

    usnprintf (string, sizeof(string), "ADC: %4d", meanVal);
    OLEDStringDraw (string, 0, 1);

}

void
groundSet(void)
{
    uint16_t i;
    int32_t sum;
    sum = 0;
    for (i = 0; i < BUF_SIZE; i++)
        sum = sum + readCircBuf (&g_inBuffer);
    // Calculate and display the rounded mean of the buffer contents
    g_heliLandedAlt = (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;
    g_heliMaxHeightAlt = g_heliLandedAlt - 1240;


}

int calculateAltitudePercentage(int adcValue, int landedAltitude) {

    int altitudePercentage = (((g_heliLandedAlt - adcValue) * 10) / 124);


    return altitudePercentage;
}

int
main(void)
{
    uint16_t i;
    int32_t sum;
    uint16_t meanVal;
    uint8_t percentAlt;

    int8_t dispMode;

    SysCtlPeripheralReset (UP_BUT_PERIPH);        // UP button GPIO
    SysCtlPeripheralReset (LEFT_BUT_PERIPH);        // LEFT button GPIO


    initClock ();
    initADC ();
    initDisplay ();
    initCircBuf (&g_inBuffer, BUF_SIZE);
    initButtons ();  // Initialises 4 pushbuttons (UP, DOWN, LEFT, RIGHT)
    //
    // Enable interrupts to the processor.
    IntMasterEnable();

    groundSet();

    displayPercentAlt (percentAlt);
    while (1)
    {
        //
        // Background task: calculate the (approximate) mean of the values in the
        // circular buffer and display it, together with the sample number.
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


        if ((checkButton (UP) == PUSHED)) {
            dispMode += 1;
        }
        if (dispMode == 0) {
            displayPercentAlt (percentAlt);
        } else if (dispMode == 1) {
            displayMeanAdc(meanVal);
        } else if (dispMode == 2) {
            dispBlank();
        } else {
            dispMode = 0;
            displayPercentAlt (percentAlt);
        }
        //SysCtlDelay (SysCtlClockGet() / 6);  // Update display at ~ 2 Hz
    }
}



