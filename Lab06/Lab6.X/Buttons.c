/*====================\
    Mathias Eriksen    
    mterikse@ucsc.edu
    ECE13 
    Lab06                  
\=====================*/

// **** Include libraries here ****

#include "Buttons.h"
#include <stdio.h>
#include "Leds_Lab06.h"
#include "BOARD.h"
#include <stdlib.h>

// **** Define global, module-level, or external variables here ****

int i;                                  // counter variable
static uint8_t buttonPrevious = 0x00;   // static variable for previous button values
                                        // needs to be static to keep value between function calls

// Initiate Buttons
void ButtonsInit(void)
{
    TRISD |= 0x00E0;                    // values from lab doc to initiate buttons
    TRISF |= 0x0002;  
}

uint8_t ButtonsCheckEvents(void)
{
    i++;                                // iterate counter
    uint8_t buttonsState = BUTTON_STATES();
                                        // use BUTTON_STATES() from BOARD.h to get values of buttons
                                        // is in binary for example 1000 -> button 4 down, others up
    uint8_t result = BUTTON_EVENT_NONE;
                                        // result holds to 4 bit numbers to handle two cases at once
                                        // defaults to no event, unless an event is observed

    if (i == BUTTONS_DEBOUNCE_PERIOD){  // when the de bounce period has elapsed, check for new events
        if ((buttonsState & BUTTON_STATE_1) && !(buttonPrevious & BUTTON_STATE_1)){
            result |= BUTTON_EVENT_1DOWN;
                                        // checks for case 0000 -> 0001 using bit masks from BOARD.h
                                        // specify resulting event using button event variables from buttons.h 
        }
        
        if (!(buttonsState & BUTTON_STATE_1) && (buttonPrevious & BUTTON_STATE_1)){
            result |= BUTTON_EVENT_1UP;
                                        // checks for case 0001 -> 0000 using bit masks from BOARD.h
                                        // specify resulting event using button event variables from buttons.h
        }
        
        if ((buttonsState & BUTTON_STATE_2) && !(buttonPrevious & BUTTON_STATE_2)){
            result |= BUTTON_EVENT_2DOWN;
                                        // checks for case 0000 -> 0010 using bit masks from BOARD.h
                                        // specify resulting event using button event variables from buttons.h
        }
        if (!(buttonsState & BUTTON_STATE_2) && (buttonPrevious & BUTTON_STATE_2)){
            result |= BUTTON_EVENT_2UP;
                                        // checks for case 0010 -> 0000 using bit masks from BOARD.h
                                        // specify resulting event using button event variables from buttons.h
        }
        
        if ((buttonsState & BUTTON_STATE_3) && !(buttonPrevious & BUTTON_STATE_3)){
            result |= BUTTON_EVENT_3DOWN;
                                        // checks for case 0000 -> 0100 using bit masks from BOARD.h
                                        // specify resulting event using button event variables from buttons.h
        }
        if (!(buttonsState & BUTTON_STATE_3) && (buttonPrevious & BUTTON_STATE_3)){
            result |= BUTTON_EVENT_3UP;
                                        // checks for case 0100 -> 0000 using bit masks from BOARD.h
                                        // specify resulting event using button event variables from buttons.h
        }
        
        if ((buttonsState & BUTTON_STATE_4) && !(buttonPrevious & BUTTON_STATE_4)){
            result |= BUTTON_EVENT_4DOWN;
                                        // checks for case 0000 -> 1000 using bit masks from BOARD.h
                                        // specify resulting event using button event variables from buttons.h
        }
        if (!(buttonsState & BUTTON_STATE_4) && (buttonPrevious & BUTTON_STATE_4)){
            result |= BUTTON_EVENT_4UP;
                                        // checks for case 1000 -> 0000 using bit masks from BOARD.h
                                        // specify resulting event using button event variables from buttons.h
        }
        
        buttonPrevious = buttonsState;  // set current state to previous state for use in next function call
        i = 0;                          // reset counter
    }
    return result ;                     // return resulting 8 bit event value
}
