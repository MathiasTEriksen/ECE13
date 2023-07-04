/*====================\
    Mathias Eriksen    
    mterikse@ucsc.edu
    ECE13 
    Lab07                  
\=====================*/

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CSE13E Support Library
#include "BOARD.h"
#include "Oled.h"
#include "Adc.h"
#include "OledDriver.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Leds.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>



// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)

// **** Set any local typedefs here ****
// enumeration for oven states, includes blinking state TIME_UP which was not provided
typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING, TIME_UP
} OvenState;

// enumeration for the 3 modes the oven can be in
typedef enum {
    BAKE, TOAST, BROIL
} OvenMode;

// enumeration for the two oven settings that can be changed
typedef enum {
    TEMP, TIME
} OvenSetting;

// structure holding all applicable oven data to be used in code
typedef struct {
    OvenState state;                                // state oven is in for state machine
    OvenMode mode;                                  // mode oven is in using OvenMode typedef
    OvenSetting setting;                            // selector setting for oven
    uint16_t TIME_LEFT;                             // dynamic time that counts down during operation
    uint16_t INITIAL_TIME;                          // initial/total cook time, saves during cook period
    uint16_t TEMPERATURE;                           // temperature for oven
} OvenData;

// structure for rasing ADC events, as well as storing ADC read voltage
struct AdcResult {
    uint8_t event;                                  // can raise events
    int16_t voltage;                                // actual voltage stored here
};

// **** Define any module-level, global, or external variables here ****
#define OVEN_TOP_ON "|\x01\x01\x01\x01\x01|"        // following desired look, using special chars
#define OVEN_TOP_OFF "|\x02\x02\x02\x02\x02|"       // from Ascii.h   
#define OVEN_BOTTOM_ON "|\x03\x03\x03\x03\x03|"     // have to redefine the #defines from Ascii.h
#define OVEN_BOTTOM_OFF "|\x04\x04\x04\x04\x04|"    // here to make them fit my usage
#define OVEN_SPACE "|     |"                        // allows an open space for graphic oven
#define OVEN_TRAY "|-----|"                         // allows tray for graphic oven
#define DEGREE_SYMBOL "\xF8"                        // #define for degree symbol to avoid magic numbers

#define MIN_TEMP 300                                // minimum temp is 300 degrees
#define LONG_PRESS 5                                // long press is 5 ticks of 5 Hz timer 
#define BROIL_TEMP 500                              // constant broil mode temp is 500 degrees
#define BAKE_TEMP 350                               // default bake temp is 350 degrees
#define MIN_TIME 1                                  // minimum time setting is 1 second,0:01
#define SECOND 5                                    // used for counting down time, 1 second is 5 ticks of 5 Hz
#define TWOHZ 2                                     // 2Hz for blink
#define SIXTYSEC 60                                 // sixty seconds for min/sec conversion

static OvenData ovenData;                           // initialize static ovenData structure
static struct AdcResult AdcResult;                  // initialize static ADC structure

static uint8_t TIMER_TICK;                          // used to raise time events
static uint16_t FREE_TIME;                          // used to store free running time
static uint8_t BUTTON_EVENT = BUTTON_EVENT_NONE;    // holds button events raised by button interrupt

static uint16_t STORE_TIME;                         // stores time to check for selector or mode change
static uint16_t STORE_COOK_TIME;                    // stores time to check for a reset in cooking

static int DISP_MIN;                                // stores minutes for display for initial time
static int DISP_SEC;                                // stores seconds for display for initial time

static int DISP_MIN_LEFT;                           // stores minutes for display for cooking time left
static int DISP_SEC_LEFT;                           // stores minutes for display for cooking time left

static int i;                                       // counter for countdown logic

static int TIME_CHUNK;                              // stores 1/8 of total time for LED logic
static int j = 1;                                   // counter for LED logic, starts at 1

static uint8_t LED_PATTERN = 0xff;                  // LED pattern, starting with all on

static int k;                                       // counter for blink logic
static uint8_t TOGGLE = 0x01;                       // toggle bit for blink logic



// **** Put any helper functions here ****
void updateOvenLED(OvenData ovenData) {             // helper function for LED logic

    TIME_CHUNK = (ovenData.INITIAL_TIME) / 8;       // set time chunk equal to 1/8 of total cooking time
    
    // logic for shifting led each time the time left passes 1/8 of the total time
    if ((ovenData.INITIAL_TIME - (j * TIME_CHUNK)) >= ovenData.TIME_LEFT) {
                                                    // executes each time another eighth of total time has passed
        LED_PATTERN = (LED_PATTERN << 1);           // shift LEDS, turning off LED1 first
        LEDS_SET(LED_PATTERN);                      // set LEDS to new pattern
        j++;                                        // counter iterates each time, so the next 1/8th will
                                                    // be subtracted from the total time
    }
}

void resetOvenLED(void) {                           // helper function to reset LED logic
    LED_PATTERN = 0xff;                             // resets pattern to all on
    LEDS_SET(LED_PATTERN);                          // sets the LEDS all on
    j = 1;                                          // resets counter for LED logic
}

void OvenLEDOFF(void) {                             // helper function to turn LEDS off
    LEDS_SET(0x00);                                 // simply turns off LEDS
}



/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData) {

    char buffer[100];                               // buffer to store compiled string, does not need to 
                                                    // be static since it will reset every call

    DISP_MIN = ovenData.INITIAL_TIME / 60;          // displayed minutes will be rounded down integer of total
                                                    // time divided by 60 seconds
    DISP_SEC = ovenData.INITIAL_TIME % 60;          // displayed seconds will be remainder of total seconds
                                                    // divided by 60

    DISP_MIN_LEFT = ovenData.TIME_LEFT / 60;        // same as above
    DISP_SEC_LEFT = ovenData.TIME_LEFT % 60;        // used for dynamic cooking time

    // display to be used in the below states is all same, most importantly what time should
    // be displayed, in these states it is initial time
    if ((ovenData.state == SETUP) || (ovenData.state == SELECTOR_CHANGE_PENDING) ||
        (ovenData.state == TIME_UP)) {
                                                    // executes in these three states
        if (ovenData.mode == BAKE) {                // in the bake case
            // statement provides correct format for Bake mode and TEMP setting
            if (ovenData.setting == TEMP) {         // we have a selector symbol
                sprintf(buffer, "%s MODE: BAKE\n"
                        "%s  TIME: %.01d:%.02d\n"
                        "%s >TEMP: %d%sF\n"
                        "%s",
                        OVEN_TOP_OFF, OVEN_SPACE, DISP_MIN, DISP_SEC, OVEN_TRAY,
                        ovenData.TEMPERATURE, DEGREE_SYMBOL, OVEN_BOTTOM_OFF);                                   
            }
             // statement provides correct format for Bake mode and TIME setting
            else if (ovenData.setting == TIME) {
                sprintf(buffer, "%s MODE: BAKE\n"
                        "%s >TIME: %.01d:%.02d\n"
                        "%s  TEMP: %d%sF\n"
                        "%s",
                        OVEN_TOP_OFF, OVEN_SPACE, DISP_MIN, DISP_SEC, OVEN_TRAY,
                        ovenData.TEMPERATURE, DEGREE_SYMBOL, OVEN_BOTTOM_OFF);

            }
        }
        // statement provides correct format for TOAST mode, only one since there is no selector
        else if (ovenData.mode == TOAST) {
            sprintf(buffer, "%s MODE: TOAST\n"
                    "%s  TIME: %.01d:%.02d\n"
                    "%s\n"                          // no TEMP in TOAST mode
                    "%s",
                    OVEN_TOP_OFF, OVEN_SPACE, DISP_MIN, DISP_SEC,
                    OVEN_TRAY, OVEN_BOTTOM_OFF);
        }
        // statement provides correct format for BROIL mode, only one since temp is always 500
        else if (ovenData.mode == BROIL) {
            sprintf(buffer, "%s MODE: BROIL\n"
                    "%s  TIME:  %.01d:%.02d\n"
                    "%s  TEMP: 500%sF\n"
                    "%s",
                    OVEN_TOP_OFF, OVEN_SPACE, DISP_MIN, DISP_SEC, OVEN_TRAY,
                    DEGREE_SYMBOL, OVEN_BOTTOM_OFF);
        }      
        OledClear(OLED_COLOR_BLACK);                // clear display for new string
        OledDrawString(buffer);                     // draw string on OLED using OLED library
        OledUpdate();                               // update the OLED, populating the screen

    // in these states, we need to display the time that is counting down
    } else if ((ovenData.state == COOKING) || (ovenData.state == RESET_PENDING)) {
        // in the bake case, no selector while we are cooking so only one case
        if (ovenData.mode == BAKE) {           
                sprintf(buffer, "%s MODE: BAKE\n"
                        "%s  TIME: %.01d:%.02d\n"
                        "%s  TEMP: %d%sF\n"
                        "%s",
                        OVEN_TOP_ON, OVEN_SPACE, DISP_MIN_LEFT, DISP_SEC_LEFT,
                        OVEN_TRAY, ovenData.TEMPERATURE, DEGREE_SYMBOL, OVEN_BOTTOM_ON);    
        }
        // correct format for TOAST mode with time counting down
        else if (ovenData.mode == TOAST) {
            sprintf(buffer, "%s MODE: TOAST\n"
                    "%s  TIME: %.01d:%.02d\n"
                    "%s\n"
                    "%s",
                    OVEN_TOP_ON, OVEN_SPACE, DISP_MIN_LEFT, DISP_SEC_LEFT,
                    OVEN_TRAY, OVEN_BOTTOM_ON);
        }
        // correct format for BROIL mode with time counting down
        else if (ovenData.mode == BROIL) {
            sprintf(buffer, "%s MODE: BROIL\n"
                    "%s  TIME:  %.01d:%.02d\n"
                    "%s  TEMP: 500%sF\n"
                    "%s",
                    OVEN_TOP_ON, OVEN_SPACE, DISP_MIN_LEFT, DISP_SEC_LEFT,
                    OVEN_TRAY, DEGREE_SYMBOL, OVEN_BOTTOM_ON);
        }
        OledClear(OLED_COLOR_BLACK);                // clear OLED
        OledDrawString(buffer);                     // draw sting to OLED
        OledUpdate();                               // populates the OLED
    }
}



/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void) {   
    // switch statement that executes based on oven states
    switch (ovenData.state) {
        
        // SETUP case, we will be defaulting to this state    
        case SETUP:
            // ADC events mean an adjustment to time and temp settings based on following
            // logic, time is saved in total/initial time in ovenData
            if (AdcResult.event == TRUE) {          // when we have an ADC event
                AdcResult.event = FALSE;            // reset flag
                if (ovenData.mode == BAKE) {        // in BAKE mode
                    if (ovenData.setting == TEMP) { // if we are in TEMP setting
                        ovenData.TEMPERATURE = (AdcResult.voltage >> 2) + MIN_TEMP;
                        updateOvenOLED(ovenData);   // temperature will be adjusted
                                                    // update the OLED with new temp value
                    }                               // if we are in time setting adjust the time
                    else if (ovenData.setting == TIME) {
                        ovenData.INITIAL_TIME = (AdcResult.voltage >> 2) + 1;
                        updateOvenOLED(ovenData);   // populate OLED with new time value
                                                    // both TIME and TEMP calculations from lab doc
                    }                   
                } else {                            // in TOAST and BROIL, we always change the time, since
                                                    // temp is omitted and fixed respectively
                    ovenData.INITIAL_TIME = (AdcResult.voltage >> 2) + 1;
                    updateOvenOLED(ovenData);       // populate the OLED using time calculation
                }
            }
            // case for selector change, switches current state to SELECTOR_CHANGE_PENDING state
            // uses button 3 initial down event for control
            if ((BUTTON_EVENT & BUTTON_EVENT_3DOWN) == BUTTON_EVENT_3DOWN) {
                STORE_TIME = FREE_TIME;             // store current running time to check for long press
                ovenData.state = SELECTOR_CHANGE_PENDING;                
            }
            // case for cook start, switches current state to COOKING state
            // uses button 4 initial down event for control
            if ((BUTTON_EVENT & BUTTON_EVENT_4DOWN) == BUTTON_EVENT_4DOWN) {
                ovenData.TIME_LEFT = ovenData.INITIAL_TIME;
                                                    // set the initial dynamic cooking time to current initial
                                                    // time
                resetOvenLED();                     // reset the LED logic using helper function, turns them all on
                ovenData.state = COOKING;           // switch state
            }
            break;     
            
        // SELECTOR change state, will be in this state until button 3 is no longer down
        case SELECTOR_CHANGE_PENDING:
            // execute based on time of press once press is over (button 3 up)
            if ((BUTTON_EVENT & BUTTON_EVENT_3UP) == BUTTON_EVENT_3UP) {
                // if the difference between free and stored time is less than 1 sec
                // we are changing cooking mode
                if ((FREE_TIME - STORE_TIME) < LONG_PRESS) {
                    if (ovenData.mode == BAKE) {    // if we are in BAKE mode
                        ovenData.mode = TOAST;      // rotate over to toast mode
                        updateOvenOLED(ovenData);   // update OLED with new selector setting
                        ovenData.state = SETUP;     // go back to setup state
                    } else if (ovenData.mode == TOAST) {
                        ovenData.mode = BROIL;      // if we are in TOAST mode, rotate to BROIL mode
                        ovenData.TEMPERATURE = BROIL_TEMP;
                                                    // always use the fixed BROIL temp
                        updateOvenOLED(ovenData);   // update OLED with new settings
                        ovenData.state = SETUP;     // go back to setup state
                    } else if (ovenData.mode == BROIL) {
                        ovenData.mode = BAKE;       // if we are in BROIL mode, rotate to BAKE mode
                        ovenData.TEMPERATURE = BAKE_TEMP;
                                                    // use the default BAKE temperature
                        ovenData.setting = TIME;    // selector should always default to time (lab doc)
                        updateOvenOLED(ovenData);   // populate OLED with new mode and data
                        ovenData.state = SETUP;     // go back to setup state
                    }
                // if the press was a long press (not a short press) > 1s, change selector
                // will only affect BAKE state since the others do not have a selector
                // but this is taken case of in the ADC part of SETUp
                } else {    
                    // if the current selector setting is temp
                    if (ovenData.setting == TEMP) {
                        ovenData.setting = TIME;    // switch to time setting
                        updateOvenOLED(ovenData);   // update OLED with new selector
                        ovenData.state = SETUP;     // go back to setup state
                    // if the current selector setting is time
                    } else if (ovenData.setting == TIME) {
                        ovenData.setting = TEMP;    // switch to temp setting
                        updateOvenOLED(ovenData);   // update settings
                        ovenData.state = SETUP;     // go back to setup  
                    }
                }
            }
            break;
            
        // COOKING state, will be here as soon as button 4 is pressed from SETUP state
        case COOKING:
            // if there is a tick on 5 Hz timer from interrupt
            if (TIMER_TICK == TRUE) {
                TIMER_TICK = FALSE;                 // reset flag
                i++;                                // iterate a counter
                if (ovenData.TIME_LEFT == 0) {      // if we tick and there is no time left
                    OvenLEDOFF();                   // turn off LED
                    ovenData.state = TIME_UP;       // move to blinking times up state
                } else if (i == SECOND) {           // if a second has passed (based on counter iterating at 5 Hz)
                    ovenData.TIME_LEFT--;           // decrement one second from time left
                    updateOvenOLED(ovenData);       // update OLED with new time
                    updateOvenLED(ovenData);        // update the LED array using the helper function
                    i = 0;                          // reset the counter so we can count another second
                }
            }
            // if we press button 4 while cooking, we can exit cooking state and go back to setup
            if ((BUTTON_EVENT & BUTTON_EVENT_4DOWN) == BUTTON_EVENT_4DOWN) {
                STORE_COOK_TIME = FREE_TIME;        // store time to check for a long press
                ovenData.state = RESET_PENDING;     // go to reset pending state
            }
            break;
            
        // RESET PENDING state, can only be entered when button 4 is pressed from cooking state
        case RESET_PENDING:
            // just makes sure we still count down the timer while button events are happening
            // same logic as in cooking state
            if (TIMER_TICK == TRUE) {
                TIMER_TICK = FALSE;
                i++;
                if (ovenData.TIME_LEFT == 0) {  
                    OvenLEDOFF();                   // turn off LED
                    ovenData.state = TIME_UP;
                } else if (i == SECOND) {
                    ovenData.TIME_LEFT--;
                    updateOvenOLED(ovenData);
                    updateOvenLED(ovenData);
                    i = 0;
                }
            }
            // when the button is no longer down
            if ((BUTTON_EVENT & BUTTON_EVENT_4UP) == BUTTON_EVENT_4UP) {
                // if the press was not long press < 5 seconds
                if ((FREE_TIME - STORE_COOK_TIME) < LONG_PRESS) {                    
                    ovenData.state = COOKING;       // go back to cooking state, not a reset press
                } else {                            // if it was a long press
                    OvenLEDOFF();                   // turn off the LEDS like they should be in setup state
                    ovenData.state = SETUP;         // go to setup state, was a reset press
                    updateOvenOLED(ovenData);       // update the OLED with setup data
                }
            }
            break;
            
        // blinking TIME_UP case, when time has fully passed and cooking is done
        // will invert display at 2 HZ when cooking is done
        case TIME_UP:
            // if we have a 5 Hz timer event
            if (TIMER_TICK == TRUE) {
                TIMER_TICK = FALSE;                 // reset event flag
                k++;                                // iterate a counter
                // use two counts (2Hz) and toggle bit to activate statement
                if ((k == TWOHZ) && TOGGLE) {       
                    updateOvenOLED(ovenData);       // update the OLED
                    OledSetDisplayInverted();       // invert the display
                    TOGGLE = TOGGLE ^ 0x01;         // switch toggle bit
                    k = 0;                          // reset counter
                // case where toggle is not true
                } else if ((k == TWOHZ) && !TOGGLE) {
                    updateOvenOLED(ovenData);       // update OLED
                    OledSetDisplayNormal();         // set display back to normal
                    TOGGLE = TOGGLE ^ 0x01;         // switch toggle bit
                    k = 0;                          // reset counter
                }
            }
            // if button 4 has been pressed and un pressed
            // allows to go back to setup state without another cook starting
            if ((BUTTON_EVENT & BUTTON_EVENT_4UP) == BUTTON_EVENT_4UP) {
                updateOvenOLED(ovenData);           // update OLED 
                OledSetDisplayNormal();             // make sure display is not inverted anymore
                ovenData.state = SETUP;             // go back to setup
            }
            break;
        
        // make sure oven cannot get stuck in an illegal state    
        default: 
            
            ovenData.state = SETUP;                 // just go back to setup if an illegal state is entered     
            break;
    }
}



// Main loop
int main() {
    // initialize everything we will be using in this lab
    BOARD_Init();
    OledInit();
    AdcInit();
    ButtonsInit();
    LEDS_INIT();

    //initialize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>

    printf("Welcome to mterikse's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    //initialize state machine (and anything else you need to init) here
    
    // give the ovenData known values to start with
    ovenData.state = SETUP;
    ovenData.setting = TIME;
    ovenData.mode = BAKE;
    ovenData.INITIAL_TIME = MIN_TIME;
    ovenData.TEMPERATURE = BAKE_TEMP;
    
    // populate the OLED once to start, every other call to this function is 
    // event driven from state machine, as per lab doc
    updateOvenOLED(ovenData);

    while (1) {
        runOvenSM();                                // simply run state machine in infinite loop
    }
}

/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;

    TIMER_TICK = TRUE;                              // raise timer event flag with 5Hz clock
    FREE_TIME++;                                    // keep a free running timer
}

/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    
    BUTTON_EVENT = ButtonsCheckEvents();            // use provided library to check for button events
    
    if (AdcChanged() == TRUE) {                     // if the ADC value has changed
        AdcResult.event = TRUE;                     // raise an ADC event flag
        AdcResult.voltage = AdcRead();              // put the ADC voltage in the ADC structure
    }
}