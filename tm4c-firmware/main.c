// Final Project, uP2 Fall 2025
// Updated: 2025-12-07
// Created by: Richard Carnivale
// Article displayer using custom RTOS implementation

/************************************Includes***************************************/

#include "G8RTOS/G8RTOS.h"
#include "./MultimodDrivers/multimod.h"

#include "./threads.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"

/************************************Includes***************************************/

/*************************************Defines***************************************/
/*************************************Defines***************************************/

/********************************Public Variables***********************************/
/********************************Public Variables***********************************/


/************************************MAIN*******************************************/

int main(void)
{
    // Sets clock speed to 80 MHz. You'll need it!
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // you might want a delay here (~10 ms) to make sure the display has powered up
    SysCtlDelay(10);

    // initialize the G8RTOS framework
    G8RTOS_Init();
    multimod_init();
    //ST7789_Fill(0xFFFF);

    // Add semaphores, threads, FIFOs here
    G8RTOS_InitSemaphore(&sem_UART,1);
    G8RTOS_InitSemaphore(&sem_I2CA,1);
    G8RTOS_InitSemaphore(&sem_SPIA,1);
    G8RTOS_InitSemaphore(&sem_PCA9555_Debounce,0);
    G8RTOS_InitSemaphore(&sem_Joystick_Debounce,1);
    
    G8RTOS_InitFIFO(JOYSTICK_FIFO);
    
    G8RTOS_AddThread(Idle_Thread, 255, "idle\0");
    G8RTOS_AddThread(Beginning_Screen_Thread, 253, "beginning\0");
    G8RTOS_AddThread(Read_Buttons, 254, "button\0");

    // add periodic and aperiodic events here
    G8RTOS_Add_PeriodicEvent (Get_Joystick, 50, 1);

    G8RTOS_Add_APeriodicEvent(GPIOE_Handler, 5, INT_GPIOE_TM4C123);
    G8RTOS_Add_APeriodicEvent(GPIOD_Handler, 5, INT_GPIOD_TM4C123);


    G8RTOS_Launch();


    while (1);
}

/************************************MAIN*******************************************/
