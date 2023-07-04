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
#include "Oled.h"
#include "OledDriver.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>                                                        

// User libraries

// **** Set macros and preprocessor directives ****

// **** Declare any data types here ****

struct AdcResult {
    uint8_t event;                      // can raise events
    int16_t voltage;                    // actual voltage stored here
};

// **** Define global, module-level, or external variables here ****
#define SIZE 5                          // size of diff from lab manual
uint16_t activeVoltage;                 // active voltage reading


// **** Declare function prototypes ****
static struct AdcResult volt = {FALSE, 0}; 
                                        // static structure so it can change
                                        // between function calls

int main(void)
{
    BOARD_Init();

    // Enable interrupts for the ADC
    IPC6bits.AD1IP = 2;
    IPC6bits.AD1IS = 0;
    IEC1bits.AD1IE = 1;

    // Set B2 to an input so AN2 can be used by the ADC.
    TRISBbits.TRISB2 = 1;

    // Configure and start the ADC
    AD1CHSbits.CH0SA = 2;               // add B2 to the mux
    AD1PCFGbits.PCFG2 = 0;              // add b2 to the ADC
    AD1CSSLbits.CSSL2 = 1;              // and add b2 to the scanner

    AD1CON1 = 0;                        // start with 0
    AD1CON1bits.SSRC = 0b111;           // auto conversion
    AD1CON1bits.ASAM = 1;               // auto sample
    
    AD1CON2 = 0;                        // start with 0
    AD1CON2bits.SMPI = 7;               // one interrupt per 8 samples

    AD1CON3 = 0;                        // start with 0
    AD1CON3bits.SAMC = 29;              // long sample time
    AD1CON3bits.ADCS = 50;              // long conversion time
    
    AD1CON1bits.ADON = 1;               // and finally turn it on

    // MY CODE    
    
    printf("Welcome to mterikse's lab6 part3 (bounce_adc).  Compiled on %s %s.\n",__TIME__,__DATE__);
    
    float percent;                        // float percentage value
    int intpercent;                       // integer percentage  for display
    char buffer[100] = "";                // buffer for print to OLED
    OledInit();                           // initiate OLED display using OLED header

    while (1){
        percent = (volt.voltage/10.23);
                                        // calculate voltage as a percentage of max
                                        // from read voltage
        intpercent = percent;           // convert to an integer value
        if (volt.event == TRUE){        // if an event is raised (change in voltage)
            volt.event = FALSE;         // reset flag
            sprintf(buffer, "Pot Reading : %d %%\nADC Voltage : %d\n",
                    intpercent, volt.voltage); 
                                        // set up buffer using sprintf() to compile
                                        // string
            
            OledClear(OLED_COLOR_BLACK);// clear screen
            OledDrawString(buffer);     // load new values
            OledUpdate();               // update the screen            
        }
        
    }
}

/**
 * This is the interrupt for the ADC1 peripheral. It will trigger whenever a new ADC reading is available
 * in the ADC buffer SFRs, ADC1BUF0 - ADC1BUF7.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_ADC_VECTOR, ipl2auto) AdcHandler(void)
{
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;

    // MY CODE    
    
    activeVoltage = ((ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 +
                     ADC1BUF5 + ADC1BUF6 + ADC1BUF7) / 8);
                                        // actual voltage as average of the eight readings, as per lab doc
    
    if (abs(activeVoltage - volt.voltage) > SIZE){
                                        // if reading and stored value are significantly different
        volt.voltage = activeVoltage;   // set new reading as stored voltage in structure
        volt.event = TRUE;              // raise an event
    }
    
    if ((volt.voltage != 0) && (activeVoltage == 0)){
                                        // handles 0 edge case, so 0 can be represented
                                        // if value stored is <5, would not properly represnt 0 otherwise
        volt.voltage = 0;               // set stored voltage to 0
        volt.event = TRUE;              // raise an event
    }
    
    if ((volt.voltage != 1023) && (activeVoltage == 1023)){
                                        // handles max edge case
        volt.voltage = 1023;            // set voltage stored to max value
        volt.event = TRUE;              // raise an event
    }
}