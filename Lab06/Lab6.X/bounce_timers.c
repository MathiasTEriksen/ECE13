/*====================\
    Mathias Eriksen    
    mterikse@ucsc.edu
    ECE13 
    Lab06                  
\=====================*/

// **** Include libraries here ****

// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"

// **** Declare any data types here ****

struct Timer {
    uint8_t event;                      // provided timer structure
    int16_t timeRemaining;              // contains event and time remaining variables
};

// **** Define global, module-level, or external variables here ****

#define FREQ 38                         // frequency from lab doc, 38 Hz
#define ATIME 2*FREQ                    // Atime = 2 seconds
#define BTIME 3*FREQ                    // Btime = 3 seconds
#define CTIME 5*FREQ                    // Btime = 5 seconds

#define LED01 0x01                      // define LED01 = 0001
#define LED02 0x02                      // define LED02 = 0010
#define LED03 0x04                      // define LED03 = 0100

// **** Declare function prototypes ****

static struct Timer TimerA = {FALSE,ATIME};
static struct Timer TimerB = {FALSE,BTIME};
static struct Timer TimerC = {FALSE,CTIME};
                                        // initialize times, and start with correct times remaining
                                        // as well as no event flags
int main(void)
{
    BOARD_Init();
    LEDS_INIT();                        // initiate the LEDS
    
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
    
    printf("Welcome to mterikse's lab6 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);
            
    while (1) {

        if (TimerA.event == TRUE) {     // if event flag for time A being up is raised
            printf("A");                // print out A
            TimerA.event = FALSE;       // reset flag
            LEDS_SET(LEDS_GET() ^ LED01);
                                        // turn on LED01, will be on for 2 seconds, then off for 2
                                        // uses toggle (^) to turn on or off based on existing value
        }
        if (TimerB.event == TRUE) {     // if event flag for time B being up is raised
            printf("B");                // print out B
            TimerB.event = FALSE;       // reset flag
            LEDS_SET(LEDS_GET() ^ LED02);
                                        // turn on LED02, will be on for 3 seconds, then off for 3
                                        // uses toggle (^) to turn on or off based on existing value
        }
        if (TimerC.event == TRUE) {     // if event flag for time B being up is raised
            printf("C");                // print out B
            TimerC.event = FALSE;       // reset flag
            LEDS_SET(LEDS_GET() ^ LED03);
                                        // turn on LED03, will be on for 5 seconds, then off for 5
                                        // uses toggle (^) to turn on or off based on existing value
        }
    }
}

/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;
    
    // MT CODE
    
    // decrement timers at frequency of peripheral clock
    (TimerA.timeRemaining)--;           //  decrement time remaining values  
    (TimerB.timeRemaining)--;
    (TimerC.timeRemaining)--;       
    
    // update TimerA when time is up
    if (TimerA.timeRemaining == 0){     // when timerA time remaining is 0
        TimerA.event = TRUE;            // raise timerA event flag
        TimerA.timeRemaining = ATIME;   // reset timer
    }
    // update TimerB when time is up
    if (TimerB.timeRemaining == 0){     // when timerB time remaining is 0
        TimerB.event = TRUE;            // raise timerB event flag
        TimerB.timeRemaining = BTIME;   // reset timer
    }   
    // update timerCss when time is up
    if (TimerC.timeRemaining == 0){     // when timerC time remaining is 0
        TimerC.event = TRUE;            // reset timerC event flag
        TimerC.timeRemaining = CTIME;   // reset timer
    }
}