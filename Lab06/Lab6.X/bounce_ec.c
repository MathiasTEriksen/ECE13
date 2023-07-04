/*====================\
    Mathias Eriksen    
    mterikse@ucsc.edu
    ECE13 
    Lab06                  
\=====================*/

// this extra credit file implements the behavior specified in the lab doc,
// not in the included extra credit example. The example video does not
// reflect the behavior specified in the lab doc accurately

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Buttons.h"
#include "Leds_Lab06.h"
#include "Oled.h"
#include "OledDriver.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

struct AdcResult {
    uint8_t event;                      // can raise events
    int16_t voltage;                    // actual voltage stored here
};

struct Timer {
    uint8_t event;                      // given timer structure
    int16_t timeRemaining;              // has event and time remaining variables
};

// **** Define global, module-level, or external variables here ****

#define LED01 0x01                      // 0000 0001
                                        // only need to check these two
#define LED08 0x80                      // 1000 0000

#define SIZE 5                          // size of diff from lab manual
uint16_t activeVoltage;                 // active voltage reading

enum {
    LEFT = 0,                           // enumerate left and right values
    RIGHT = 1                           // avoids magic numbers
};  

unsigned char move;

static uint8_t Check;                   // static variable for button check
static uint8_t toggle = 0x01;           // static variable for button check

// **** Declare function prototypes ****

static struct Timer switchtimer;        // create a timer using timer structure from

static struct AdcResult volt = {FALSE, 0}; 
                                        // static structure so it can change
                                        // between function calls

int main(void)
{
    BOARD_Init();

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

    AD1CON1 = 0; // start with 0
    AD1CON1bits.SSRC = 0b111;           // auto conversion
    AD1CON1bits.ASAM = 1;               // auto sample
    
    AD1CON2 = 0; // start with 0
    AD1CON2bits.SMPI = 7;               // one interrupt per 8 samples

    AD1CON3 = 0; // start with 0
    AD1CON3bits.SAMC = 29;              // long sample time
    AD1CON3bits.ADCS = 50;              // long conversion time

    AD1CON1bits.ADON = 1;               // and finally turn it on

    // MY CODE
    
    printf("Welcome to mterikse's lab6 part6 (Extra Credit).  Compiled on %s %s.\n",__TIME__,__DATE__);

    LEDS_INIT();                        // initiate LEDS
    LEDS_SET(LED01);                    // start LEDS off at LED01
    move = LEFT;                        // start moving left, from LED01 -> LED08
    unsigned char state;                // LED state variable
    
    float percent;
    int intpercent;                     // integer percentage  for display
    char buffer[100] = "";              // buffer for print to OLED
    OledInit();                         // Initiate OLED

        while(1){  
            state = LEDS_GET();         // use function from Leds_Lab06.h to get value of switches
            
            uint8_t switchesState = SWITCH_STATES();
                                        // need switched for button behavior
            Check = ButtonsCheckEvents();
                                        // check for button events
                    
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
                
                if ((move == LEFT) && toggle){ 
                                            // if we are moving left, and button toggle is on
                    state = state << 1;     // shift LED bit to left (i.e 0000 0001 -> 0000 0010)
                                            // moves the LED on board to left
                }
                if ((move == RIGHT) && toggle){         
                                            // if we are moving right, and button toggle is on
                    state = state >> 1;     // shift LED bit to right (i.e 0000 0010 -> 0000 0001)
                }   
            }
            
            LEDS_SET(state);                // set the LED to moved value
            
            if (!(switchesState & SWITCH_STATE_SW1)){
                                            // if switch is down, will execute using bit mask from BOARD.h
                    if ((Check & BUTTON_EVENT_1DOWN) ==  BUTTON_EVENT_1DOWN){
                        toggle = toggle ^ 0x01;                        
                    }
                    if ((Check & BUTTON_EVENT_2DOWN) ==  BUTTON_EVENT_2DOWN){                        
                        toggle = toggle ^ 0x01;                                                  
                    }
                    if ((Check & BUTTON_EVENT_3DOWN) ==  BUTTON_EVENT_3DOWN){                      
                        toggle = toggle ^ 0x01;
                    }
                    if ((Check & BUTTON_EVENT_4DOWN) ==  BUTTON_EVENT_4DOWN){                        
                        toggle = toggle ^ 0x01;                                              
                    }
                                            // uses bitwise XOR to toggle a control bit on and off
                                            // as specified
                }
                
                if ((switchesState & SWITCH_STATE_SW1)){
                                            // if switch is up, will execute using bit mask from BOARD.h
                    if ((Check & BUTTON_EVENT_1UP) ==  BUTTON_EVENT_1UP){                        
                        toggle = toggle ^ 0x01;                      
                    }
                    if ((Check & BUTTON_EVENT_2UP) ==  BUTTON_EVENT_2UP){                        
                        toggle = toggle ^ 0x01;                              
                    }
                    if ((Check & BUTTON_EVENT_3UP) ==  BUTTON_EVENT_3UP){                        
                        toggle = toggle ^ 0x01;

                    }
                    if ((Check & BUTTON_EVENT_4UP) ==  BUTTON_EVENT_4UP){                       
                        toggle = toggle ^ 0x01;
                    }
                                            // uses bitwise XOR to toggle a control bit on and off
                                            // as specified
                }
            
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
 * This is the interrupt for the Timer1 peripheral.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;
    
    (switchtimer.timeRemaining)--;      // decrement timer in each pass
    
    if (switchtimer.timeRemaining < 0){ // if we have no time remaining
        switchtimer.event = TRUE;       // raise an event flag
        switchtimer.timeRemaining = (100 - (volt.voltage / 10.23));    
                                        // reset timer using voltage from BOARD.h
                                        // takes care of speed using switch states, 
    }
}

/**
 * This is the ISR for the ADC1 peripheral.
 */
void __ISR(_ADC_VECTOR, IPL2AUTO) AdcHandler(void)
{
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;
    
    activeVoltage = ((ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 +
                     ADC1BUF5 + ADC1BUF6 + ADC1BUF7) / 8);
    
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