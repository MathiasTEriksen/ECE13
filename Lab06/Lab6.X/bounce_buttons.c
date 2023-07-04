/*====================\
    Mathias Eriksen    
    mterikse@ucsc.edu
    ECE13 
    Lab06                  
\=====================*/

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Leds_Lab06.h"
#include "Buttons.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries

// **** Set macros and preprocessor directives ****

// **** Declare any data types here ****

// **** Define global, module-level, or external variables here ****

#define LED01 0x01                      // 0000 0001
#define LED02 0x02                      // 0000 0010
#define LED03 0x04                      // 0000 0100
#define LED04 0x08                      // 0000 1000

#define LED05 0x10                      // 0001 0000
#define LED06 0x20                      // 0010 0001
#define LED07 0x40                      // 0100 0000
#define LED08 0x80                      // 1000 0000

static uint8_t Check;                   // static variable for button check

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();
    LEDS_INIT();                        // initiate LEDS

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0;                          // everything should be off
    T1CONbits.TCKPS = 1;                // 1:8 pre scaler
    PR1 = 0xFFFF;                       // interrupt at max interval
    T1CONbits.ON = 1;                   // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0;                  //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4;                  // priority of  4
    IPC1bits.T1IS = 0;                  // sub priority of 0 arbitrarily 
    IEC0bits.T1IE = 1;                  // turn the interrupt on

    // MY CODE
    
    printf("Welcome to mterikse's lab6 part5 (bounce_buttons).  Compiled on %s %s.\n", __TIME__, __DATE__);

    while (1){
       uint8_t switchesState = SWITCH_STATES();
                                        // get current state of switches using function from BOARD.h
       Check = ButtonsCheckEvents();    // use function from buttons.c to check for button events
       
       if (switchesState & SWITCH_STATE_SW1){
                                        // if switch is up, will execute using bit mask from BOARD.h
           if ((Check & BUTTON_EVENT_1UP) ==  BUTTON_EVENT_1UP){
                LEDS_SET(LEDS_GET() ^ (LED01 | LED02));
                                        // turn on LEDS 01 and 02 if button 1 up event is raised
            }
            if ((Check & BUTTON_EVENT_2UP) ==  BUTTON_EVENT_2UP){
                LEDS_SET(LEDS_GET() ^ (LED03 | LED04));
                                        // turn on LEDS 03 and 04 if button 2 up event is raised
            }
            if ((Check & BUTTON_EVENT_3UP) ==  BUTTON_EVENT_3UP){
                LEDS_SET(LEDS_GET() ^ (LED05 | LED06));
                                        // turn on LEDS 05 and 06 if button 3 up event is raised
            }
            if ((Check & BUTTON_EVENT_4UP) ==  BUTTON_EVENT_4UP){
                LEDS_SET(LEDS_GET() ^ (LED07 | LED08));
                                        // turn on LEDS 07 and 08 if button 4 up event is raised
            }
       }
       
       if (!(switchesState & SWITCH_STATE_SW1)){
                                        // if switch is down, will execute using bit mask from BOARD.h and NOT
           if ((Check & BUTTON_EVENT_1DOWN) ==  BUTTON_EVENT_1DOWN){
                LEDS_SET(LEDS_GET() ^ (LED01 | LED02));
                                        // turn on LEDS 01 and 02 if button 1 down event is raised
            }
            if ((Check & BUTTON_EVENT_2DOWN) ==  BUTTON_EVENT_2DOWN){
                LEDS_SET(LEDS_GET() ^ (LED03 | LED04));
                                        // turn on LEDS 03 and 04 if button 2 down event is raised
            }
            if ((Check & BUTTON_EVENT_3DOWN) ==  BUTTON_EVENT_3DOWN){
                LEDS_SET(LEDS_GET() ^ (LED05 | LED06));
                                        // turn on LEDS 05 and 06 if button 3 down event is raised
            }
            if ((Check & BUTTON_EVENT_4DOWN) ==  BUTTON_EVENT_4DOWN){
                LEDS_SET(LEDS_GET() ^ (LED07 | LED08));
                                        // turn on LEDS 07 and 08 if button 4 down event is raised
            }
       }       
    }
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;
}