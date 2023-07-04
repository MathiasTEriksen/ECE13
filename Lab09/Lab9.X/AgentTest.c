//#ifdef
// **** Include libraries here ****
// Standard libraries 
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

//CE13 standard libraries:
#include "Buttons.h"
#include "Uart1.h"
#include "Oled.h"

// Battleboats Libraries:
#include "BattleBoats.h"
#include "Agent.h"
#include "Negotiation.h"
#include "Message.h"
#include "Field.h"


int main(){
    
    BB_Event Test;
    AgentInit();
    
    Test.type = BB_EVENT_START_BUTTON;
    
    Message MSG = AgentRun(Test);
    
    printf("Testing mterikse's AgentTest harness...\n\n");
    
    
    printf("\nTesting START state\n");
    
    if (MSG.type == MESSAGE_CHA){
        printf("SUCCESS\n");
    } else {
        printf("FAILED\n");
    }
    
    printf("%u\n", MSG.param0);
    
    printf("\nTesting CHA state\n");
    Test.type = BB_EVENT_ACC_RECEIVED;
    
    MSG = AgentRun(Test);
    
    if (MSG.type == MESSAGE_REV){
        printf("SUCCESS\n");
    } else {
        printf("FAILED\n");
    }
    
    
    printf("\nTesting Next state (Attack/Defend)...\n");
    
    AgentState GetState = AgentGetState();
    
    if (GetState == AGENT_STATE_WAITING_TO_SEND){
        
        printf("Attacking State\n");
        
        Test.type = BB_EVENT_RES_RECEIVED;
        
        MSG = AgentRun(Test);
        
        printf("Guess Row: %d, Guess Col: %d\n", MSG.param0, MSG.param1);        
        
        
    } else {
        
        printf("Defending State\n");
        
        Test.type = BB_EVENT_SHO_RECEIVED;
        
        MSG = AgentRun(Test);
        
        printf("Shot Row: %d, Shot Col: %d\n", MSG.param0, MSG.param1);
        
         printf("Attacking State\n");
         
        Test.type = BB_EVENT_MESSAGE_SENT;
        
        MSG = AgentRun(Test);
        
        printf("Guess Row: %d, Guess Col: %d\n", MSG.param0, MSG.param1); 
         
        Test.type = BB_EVENT_RES_RECEIVED;
        
    }
    
}
