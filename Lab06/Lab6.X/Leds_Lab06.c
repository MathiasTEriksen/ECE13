/*====================\
    Mathias Eriksen    
    mterikse@ucsc.edu
    ECE13 
    Lab06                  
\=====================*/

// **** Include libraries here ****

#include "Leds_Lab06.h"
#include "BOARD.h"
#include <stdio.h>

// Initiate the LEDS
void LEDS_INIT(void)
{
    TRISE = 0x00 ;                      // initialize LEDS
    LATE = 0x00 ;                       // set all LEDS to 0
}

// Set LEDS to new pattern
void LEDS_SET(char newPattern)
{
    LATE = newPattern ;                 // Set LEDS to pattern
}

// Get current state of LEDS
char LEDS_GET(void)
{
    return LATE;                        // return current LED value
}
