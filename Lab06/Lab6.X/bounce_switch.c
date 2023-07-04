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

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// **** Declare any data types here ****

struct Timer {
    uint8_t event;                      // given timer structure
    int16_t timeRemaining;              // has event and time remaining variables
};

// **** Define global, module-level, or external variables here ****

unsigned char move;

#define LED01 0x01                      // 0000 0001
                                        // only need to check these two
#define LED08 0x80                      // 1000 0000

enum {
    LEFT = 0,                           // enumerate left and right values
    RIGHT = 1                           // avoids magic numbers
};          

// **** Declare function prototypes ****
static struct Timer switchtimer;        // create a timer using timer structure from

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0;                          // everything should be off
    T1CONbits.TCKPS = 1;                // 1:8 prescaler
    PR1 = 0xFFFF;                       // interrupt at max interval
    T1CONbits.ON = 1;                   // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0;                  //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4;                  // priority of  4
    IPC1bits.T1IS = 0;                  // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1;                  // turn the interrupt on
    
    // MY CODE
    
    printf("Welcome to mterikse's lab6 part2 (bounce_switch).  Compiled on %s %s.\n",__TIME__,__DATE__);
    
    LEDS_INIT();                        // initiate LEDS
    LEDS_SET(LED01);                    // start LEDS off at LED01
    move = LEFT;                        // start moving left, from LED01 -> LED08
    unsigned char state;                // LED state variable
    
	while(1){  
        state = LEDS_GET();             // use function from Leds_Lab06.h to get value of switches
        if (switchtimer.event == TRUE){ // if we have an event from interrupt (time to move!!!)
            switchtimer.event = FALSE;  // reset flag           
            if ((state == LED08) && (move == LEFT)) {
                                        // if we are at the leftmost LED and moving left
                move = RIGHT;           // move back to right LED08 -> LED01
            }   
            if ((state == LED01) && (move == RIGHT)) {
                                        // if we are at rightmost LED and moving right
                move = LEFT;            // move back left LED01 -> LED08
            }
            if (move == LEFT){          // if we are moving left
                state = state << 1;     // shift LED bit to left (i.e 0000 0001 -> 0000 0010)
                                        // moves the LED on board to left
            }
            if (move == RIGHT){         // if we are moving right
                state = state >> 1;     // shift LED bit to right (i.e 0000 0010 -> 0000 0001)
            }                           // moves LED on board to right
        }
        LEDS_SET(state);                // set the LED to moved value
    }			
}

/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 pre scaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    // MY CODE
    
    (switchtimer.timeRemaining)--;      // decrement timer in each pass
    
    if (switchtimer.timeRemaining < 0){ // if we have no time remaining
        switchtimer.event = TRUE;       // raise an event flag
        switchtimer.timeRemaining = SWITCH_STATES()+1;    
                                        // reset timer using SWITCH_STATES()+1, +1 so its never 0 from BOARD.h
                                        // takes care of speed using switch states, 
    }
}