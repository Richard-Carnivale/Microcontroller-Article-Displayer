// multimod_dac.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for DAC functions

/************************************Includes***************************************/

#include "../multimod_dac.h"
#include "../multimod_PCA9555.h"

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"
#include "driverlib/ssi.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/adc.h"

/************************************Includes***************************************/

#define AUDIO_PCA9555_ADDR 0x22

/********************************Public Functions***********************************/

// MutimodDAC_Init
// Initializes ports & modules for the DAC
// Return: void
void MutimodDAC_Init(void) {
	// stuff


    // Register the interrupt handler
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);
}

// MutimodDAC_Write
// Performs a write to a DAC register
// Return: void
void MutimodDAC_Write(uint32_t reg_address, uint32_t data) {


}

// MutimodDAC_Read
// Performs a read from a DAC register
// Return: void
uint32_t MutimodDAC_Read(uint32_t reg_address) {




    //return data & 0xFFFF; // extract the 16-bit data
}

/********************************Public Functions***********************************/
