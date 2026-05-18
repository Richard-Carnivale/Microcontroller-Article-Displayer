// threads.c
// Date Updated: 2025-12-05
// Defines for thread functions.

/************************************Includes***************************************/

#include "./threads.h"

#include "./MultimodDrivers/multimod.h"
#include "./MultimodDrivers/multimod_buttons.h"
#include "./MultimodDrivers/multimod_joystick.h"
#include "./MultimodDrivers/SPI_string.h"

#include <utils/ustdlib.c>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <driverlib/gpio.h>
#include <inc/hw_memmap.h>
#include <inc/tm4c123gh6pm.h>
#include <driverlib/interrupt.h>


#include "./G8RTOS/G8RTOS_IPC.h"
#include "./G8RTOS/G8RTOS_Scheduler.h"

/************************************Includes***************************************/

/*************************************Defines***************************************/
#define MAX_ARTICLES 10
#define MAX_LEN 256
/*************************************Defines***************************************/

/*********************************Global Variables**********************************/
uint8_t buffer[20] = {0};
uint8_t counter1 = 0;
uint8_t arrow_position = 1;
char title_string[64];

/*char uart_line[128];
uint8_t uart_index = 0;
uint8_t article_y = 210;*/
char articles[MAX_ARTICLES][MAX_LEN]; // storage for 5 headlines
uint8_t article_count = 0;            // how many received
int16_t current_index = -2;            // which one is displayed
char uart_line[MAX_LEN];
uint8_t uart_index = 0;
volatile bool button_pressed = false;
volatile bool joystick_pressed = false;
volatile bool showing_description = false;
uint8_t kill = 0;
bool once = false;
/*********************************Global Variables**********************************/

/*************************************Threads***************************************/

void Idle_Thread(void){
    while(1);
}


void Beginning_Screen_Thread(void){
    IntDisable(INT_GPIOE);

    G8RTOS_WaitSemaphore(&sem_SPIA);
    //ST7789_Fill(ST7789_BLACK);
    //display_fillGradient(64, 0, 64, 0, 0, 0);
    //display_fillGradient(128, 0, 0, 255, 128, 0);
    display_fillGradient(0, 128, 128, 0, 0, 128);


    usnprintf(title_string, sizeof(title_string), "%s", "HELLO! I am an article displayer");
    ST7789_DrawStringStatic(title_string, ST7789_WHITE, 7, 230);

    usnprintf(title_string, sizeof(title_string), "%s", "Select a category below.");
    ST7789_DrawStringStatic(title_string, ST7789_WHITE, 7, 210);

    usnprintf(title_string, sizeof(title_string), "%s", "GENERAL");
    ST7789_DrawStringStatic(title_string, ST7789_GREEN, 10, 190);

    usnprintf(title_string, sizeof(title_string), "%s", "TECHNOLOGY");
    ST7789_DrawStringStatic(title_string, ST7789_YELLOW, 10, 170);

    usnprintf(title_string, sizeof(title_string), "%s", "SCIENCE");
    ST7789_DrawStringStatic(title_string, ST7789_GREEN, 10, 150);

    usnprintf(title_string, sizeof(title_string), "%s", "BUSINESS");
    ST7789_DrawStringStatic(title_string, ST7789_YELLOW, 10, 130);

    usnprintf(title_string, sizeof(title_string), "%s", "ENTERTAINMENT");
    ST7789_DrawStringStatic(title_string, ST7789_GREEN, 10, 110);

    usnprintf(title_string, sizeof(title_string), "%s", "SPORTS");
    ST7789_DrawStringStatic(title_string, ST7789_YELLOW, 10, 90);

    usnprintf(title_string, sizeof(title_string), "%s", "Use joystick to scroll and");
    ST7789_DrawStringStatic(title_string, ST7789_WHITE, 10, 50);
    usnprintf(title_string, sizeof(title_string), "%s", "joystick press to select.");
    ST7789_DrawStringStatic(title_string, ST7789_WHITE, 10, 30);

    usnprintf(title_string, sizeof(title_string), "%s", "<--");
    ST7789_DrawStringStatic(title_string, ST7789_BLACK, 70, 130);
    ST7789_DrawStringStatic(title_string, ST7789_BLACK, 105, 110);
    ST7789_DrawStringStatic(title_string, ST7789_BLACK, 55, 90);
    ST7789_DrawStringStatic(title_string, ST7789_BLACK, 85, 170);
    ST7789_DrawStringStatic(title_string, ST7789_BLACK, 65, 190);
    ST7789_DrawStringStatic(title_string, ST7789_BLACK, 65, 150);

    G8RTOS_SignalSemaphore(&sem_SPIA);

    while(1){
        uint16_t x = G8RTOS_ReadFIFO(JOYSTICK_FIFO);

        /*usnprintf(title_string, sizeof(title_string), "%s", "<--");*/

        if(x < 1500){
            if(arrow_position == 5){
                arrow_position = 0;
            } else {
                arrow_position++;
            }
        }
        if(x > 2550){
            if(arrow_position == 0){
                arrow_position = 5;
            } else {
                arrow_position--;
            }
        }

        G8RTOS_WaitSemaphore(&sem_SPIA);
        if(arrow_position == 0){
            ST7789_DrawStringStatic(title_string, ST7789_BLACK, 55, 90);
            ST7789_DrawStringStatic(title_string, ST7789_BLACK, 85, 170);
            ST7789_DrawStringStatic(title_string, ST7789_WHITE, 65, 190); // arrow to general
        } else if (arrow_position == 1){
            ST7789_DrawStringStatic(title_string, ST7789_BLACK, 65, 190);
            ST7789_DrawStringStatic(title_string, ST7789_BLACK, 65, 150);
            ST7789_DrawStringStatic(title_string, ST7789_WHITE, 85, 170); // arrow to technology
        } else if (arrow_position == 2){
            ST7789_DrawStringStatic(title_string, ST7789_BLACK, 85, 170);
            ST7789_DrawStringStatic(title_string, ST7789_BLACK, 70, 130);
            ST7789_DrawStringStatic(title_string, ST7789_WHITE, 65, 150); // arrow to science
        } else if (arrow_position == 3){
            ST7789_DrawStringStatic(title_string, ST7789_BLACK, 65, 150);
            ST7789_DrawStringStatic(title_string, ST7789_BLACK, 105, 110);
            ST7789_DrawStringStatic(title_string, ST7789_WHITE, 70, 130); // arrow to business
        } else if (arrow_position == 4){
            ST7789_DrawStringStatic(title_string, ST7789_BLACK, 70, 130);
            ST7789_DrawStringStatic(title_string, ST7789_BLACK, 55, 90);
            ST7789_DrawStringStatic(title_string, ST7789_WHITE, 105, 110); // arrow to entertainment
        } else if (arrow_position == 5){
            ST7789_DrawStringStatic(title_string, ST7789_BLACK, 65, 190);
            ST7789_DrawStringStatic(title_string, ST7789_BLACK, 105, 110);
            ST7789_DrawStringStatic(title_string, ST7789_WHITE, 55, 90); // arrow to sports
        }
        G8RTOS_SignalSemaphore(&sem_SPIA);

        //UARTprintf("%d", arrow_position);

        if(JOYSTICK_GetPress()){
            G8RTOS_WaitSemaphore(&sem_UART);
            if (arrow_position == 0) {
                // arrow to general
                char word[] = "general";
                for (uint8_t i = 0; i < 7; i++) {
                    UARTCharPut(UART4_BASE, word[i]);
                }
            } else if (arrow_position == 1) {
                // arrow to technology
                char word[] = "technology";
                for (uint8_t i = 0; i < 10; i++) {
                    UARTCharPut(UART4_BASE, word[i]);
                }
            } else if (arrow_position == 2) {
                // arrow to science
                char word[] = "science";
                for (uint8_t i = 0; i < 7; i++) {
                    UARTCharPut(UART4_BASE, word[i]);
                }
            } else if (arrow_position == 3) {
                // arrow to business
                char word[] = "business";
                for (uint8_t i = 0; i < 8; i++) {
                    UARTCharPut(UART4_BASE, word[i]);
                }
            } else if (arrow_position == 4) {
                // arrow to entertainment
                char word[] = "entertainment";
                for (uint8_t i = 0; i < 13; i++) {
                    UARTCharPut(UART4_BASE, word[i]);
                }
            } else if (arrow_position == 5) {
                // arrow to sports
                char word[] = "sports";
                for (uint8_t i = 0; i < 6; i++) {
                    UARTCharPut(UART4_BASE, word[i]);
                }
            }
            G8RTOS_SignalSemaphore(&sem_UART);
            while(JOYSTICK_GetPress()) sleep(3);
            G8RTOS_AddThread(Query_Screen_Thread, 253, "query\0");
            G8RTOS_KillSelf();
        }
        sleep(10); // small delay
    }
}

void Query_Screen_Thread(void){
    G8RTOS_WaitSemaphore(&sem_SPIA);
    ST7789_Fill(0x9200);
    usnprintf(title_string, sizeof(title_string), "%s", "Do you want to search for any");
    ST7789_DrawStringStatic(title_string, ST7789_WHITE, 7, 220);
    usnprintf(title_string, sizeof(title_string), "%s", "specific keyword? Use a connected UART terminal on CCS or putty.");
    ST7789_DrawStringStatic(title_string, ST7789_WHITE, 7, 200);
    usnprintf(title_string, sizeof(title_string), "%s", "If not, press joystick.");
    ST7789_DrawStringStatic(title_string, ST7789_WHITE, 7, 160);
    usnprintf(title_string, sizeof(title_string), "%s", "Keyword: ");
    ST7789_DrawStringStatic(title_string, ST7789_WHITE, 7, 100);
    G8RTOS_SignalSemaphore(&sem_SPIA);

    G8RTOS_WaitSemaphore(&sem_UART);
    UARTprintf("Enter Keyword: ");
    G8RTOS_SignalSemaphore(&sem_UART);

    while(1){
        G8RTOS_WaitSemaphore(&sem_UART);
        if (UARTCharsAvail(UART0_BASE)) {
            uint8_t b = UARTCharGet(UART0_BASE);

            if(b == 8 || b == 127){
                if (counter1 > 0) {
                    UARTprintf("\b \b");
                    counter1--;
                    buffer[counter1] = 0;
                    G8RTOS_WaitSemaphore(&sem_SPIA);
                    ST7789_DrawRectangle(65, 92, 150, 12, 0x9200);
                    G8RTOS_SignalSemaphore(&sem_SPIA);
                }
            } else {
                if (b == 13 || b == 10) {
                    UARTprintf("\r\n");
                    buffer[counter1] = '\0';
                    for(uint8_t i = 0; i < counter1; i++){
                        UARTCharPut(UART4_BASE, buffer[i]);
                        buffer[i] = 0;
                    }
                    if(counter1 < 1){
                        UARTCharPut(UART4_BASE, 'n');
                    }
                    counter1 = 0;
                    G8RTOS_AddThread(Choose_Article_Thread, 254, "choose\0");
                    G8RTOS_SignalSemaphore(&sem_UART);
                    G8RTOS_KillSelf();
                } else {
                    if (counter1 < sizeof(buffer) - 1) {   // prevent overflow
                        UARTprintf("%c", b);
                        buffer[counter1++] = b;
                    }
                }
            }

            G8RTOS_WaitSemaphore(&sem_SPIA);
            usnprintf(title_string, sizeof(title_string), "%s", buffer);
            ST7789_DrawStringStatic(title_string, 0x057F, 65, 100);
            G8RTOS_SignalSemaphore(&sem_SPIA);

        } else if (JOYSTICK_GetPress()){
            UARTCharPut(UART4_BASE, 'n');
            while(JOYSTICK_GetPress()) sleep(3);
            counter1 = 0;
            G8RTOS_AddThread(Choose_Article_Thread, 254, "choose\0");
            G8RTOS_SignalSemaphore(&sem_UART);
            G8RTOS_KillSelf();
        }
        G8RTOS_SignalSemaphore(&sem_UART);
        sleep(3);
    }
}

void Choose_Article_Thread(void) {
    IntEnable(INT_GPIOE);

    G8RTOS_WaitSemaphore(&sem_SPIA);
    ST7789_Fill(0x9200);
    usnprintf(title_string, sizeof(title_string), "%s", "Articles Loading. Please wait a minimum of 2 seconds.");
    ST7789_DrawStringStatic(title_string, 0x057F, 7, 250);
    usnprintf(title_string, sizeof(title_string), "%s", "Please use the first two buttonson the mutlimod to move");
    ST7789_DrawStringStatic(title_string, ST7789_WHITE, 7, 210);
    usnprintf(title_string, sizeof(title_string), "%s", "between articles and use the    joystick to read ");
    ST7789_DrawStringStatic(title_string, ST7789_WHITE, 7, 170);
    usnprintf(title_string, sizeof(title_string), "%s", "more about the article you are  currently looking at.");
    ST7789_DrawStringStatic(title_string, ST7789_WHITE, 7, 130);
    usnprintf(title_string, sizeof(title_string), "%s", "Now press SW1.");
    ST7789_DrawStringStatic(title_string, ST7789_WHITE, 7, 75);
    G8RTOS_SignalSemaphore(&sem_SPIA);

    while (1) {
        G8RTOS_WaitSemaphore(&sem_UART);
        while (UARTCharsAvail(UART4_BASE)) {
            uint8_t b = UARTCharGet(UART4_BASE);
            if (b == 0xE2) {
                UARTCharGet(UART4_BASE); // consume 0x80
                UARTCharGet(UART4_BASE); // consume 0x99
                b = 0x27; // replace
            }

            //char c = (char)b;

            if (b == '\n' || b == '\r') {
                if (uart_index > 0 && article_count < MAX_ARTICLES) {
                    uart_line[uart_index] = '\0'; // terminate string
                    strncpy(articles[article_count], uart_line, MAX_LEN);
                    articles[article_count][MAX_LEN-1] = '\0'; // safety
                    article_count++;
                    uart_index = 0;
                }
            } else if (uart_index < MAX_LEN - 1) {
                uart_line[uart_index++] = (char)b;
            } else {
                uart_line[uart_index] = '\0';
                uart_index = 0;
            }
            //UARTprintf("%c", b);
        }
        G8RTOS_SignalSemaphore(&sem_UART);


        if (button_pressed && article_count > 0 && !showing_description) {
            button_pressed = false; // clear flag
            //current_index = (current_index + 1) % article_count;
            //UARTprintf("button press handled\n");


            G8RTOS_WaitSemaphore(&sem_SPIA);
            //ST7789_Fill(0x9200); // clear screen
            if(!once){
                ST7789_DrawRectangle(0, 220, 240, 60, 0x9200);
                once = true;
            }
            usnprintf(title_string, sizeof(title_string), "%s", "Articles returned. Press joystick to read full description.");
            ST7789_DrawStringStatic(title_string, 0x057F, 10, 250);
            ST7789_DrawRectangle(0, 65, 240, 150, 0x0000);
            ST7789_DrawStringStatic(articles[current_index], ST7789_WHITE, 7, 210);
            usnprintf(title_string, sizeof(title_string), "%s", "  To search for another topic,");
            ST7789_DrawStringStatic(title_string, 0x057F, 10, 60);
            usnprintf(title_string, sizeof(title_string), "%s", "  press SW4.");
            ST7789_DrawStringStatic(title_string, 0x057F, 10, 40);
            usnprintf(title_string, sizeof(title_string), "%s", "  Wait 2-3 seconds to reset.");
            ST7789_DrawStringStatic(title_string, 0x057F, 10, 20);

            G8RTOS_SignalSemaphore(&sem_SPIA);
        }

        if(JOYSTICK_GetPress() && article_count > 0){

            if (!showing_description) {
                // move to description
                if (current_index + 1 < article_count) {
                    current_index++;
                    showing_description = true;
                }
            } else {
                // currently showing description advance to next title
                current_index--;
                if (current_index <= 0) {
                    current_index = 0;
                }
                showing_description = false;
            }

            // redraw
            G8RTOS_WaitSemaphore(&sem_SPIA);
            //ST7789_Fill(0x9200);
            //usnprintf(title_string, sizeof(title_string), "%s", "Articles returned. Press joystick to read full description.");
            //ST7789_DrawStringStatic(title_string, 0x057F, 10, 250);
            ST7789_DrawRectangle(0, 65, 240, 150, 0x0000);
            ST7789_DrawStringStatic(articles[current_index], ST7789_WHITE, 7, 210);
            //usnprintf(title_string, sizeof(title_string), "%s", "  To search for another topic,");
            //ST7789_DrawStringStatic(title_string, 0x057F, 10, 60);
            //usnprintf(title_string, sizeof(title_string), "%s", "  press SW4.");
            //ST7789_DrawStringStatic(title_string, 0x057F, 10, 40);
            //usnprintf(title_string, sizeof(title_string), "%s", "  Wait 2-3 seconds to reset.");
            //ST7789_DrawStringStatic(title_string, 0x057F, 10, 20);
            G8RTOS_SignalSemaphore(&sem_SPIA);

            while (JOYSTICK_GetPress()) sleep(3);

            /*G8RTOS_WaitSemaphore(&sem_UART);


            G8RTOS_SignalSemaphore(&sem_UART);
            while(JOYSTICK_GetPress()) sleep(3);
            G8RTOS_AddThread(Query_Screen_Thread, 253, "query\0");
            G8RTOS_KillSelf();*/
        }

        if(kill){
            // reset globals
            article_count = 0;
            current_index = -2;
            uart_index = 0;
            button_pressed = false;
            joystick_pressed = false;
            showing_description = false;
            kill = 0;
            once = false;

            // optionally clear articles
            for (int i = 0; i < MAX_ARTICLES; i++) {
                articles[i][0] = '\0';
            }
            G8RTOS_WaitSemaphore(&sem_SPIA);
            ST7789_Fill(0x9200);
            G8RTOS_SignalSemaphore(&sem_SPIA);

            G8RTOS_AddThread(Beginning_Screen_Thread, 253, "query\0");
            G8RTOS_KillSelf();
        }

    }
}

void Read_Buttons(void) {
    while (1) {
        G8RTOS_WaitSemaphore(&sem_PCA9555_Debounce);
        sleep(50);  // debounce

        uint8_t button_data = MultimodButtons_Get();

        if (!showing_description) {
            if (button_data == 253) { // next

                button_pressed = true;

                //UARTprintf("button pressed\n");
                //showing_description = false;
                current_index = (current_index + 2) % article_count;
                while (MultimodButtons_Get() == 253) sleep(3);
            }
            else if (button_data == 251) { // prev
                button_pressed = true;
                current_index = (current_index + article_count - 2) % article_count;
                //showing_description = false;
                while (MultimodButtons_Get() == 251) sleep(3);
            }
        }

        if(button_data == 239){
            kill = 1;
            while (MultimodButtons_Get() == 239) sleep(3);
        }

        GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_4);
        IntEnable(INT_GPIOE);
    }
}

/*************************************Threads***************************************/

/********************************Periodic Threads***********************************/


void Get_Joystick(void) {
    uint16_t x = JOYSTICK_GetX(); // upper 16 bits = X, lower = Y
    G8RTOS_WriteFIFO(JOYSTICK_FIFO, x);
}

/********************************Periodic Threads***********************************/


/*******************************Aperiodic Threads***********************************/

void GPIOE_Handler() {
    G8RTOS_SignalSemaphore(&sem_PCA9555_Debounce);
    IntDisable(INT_GPIOE);
}

void GPIOD_Handler() {
    G8RTOS_SignalSemaphore(&sem_Joystick_Debounce);
    GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_2);
}

/*******************************Aperiodic Threads***********************************/
