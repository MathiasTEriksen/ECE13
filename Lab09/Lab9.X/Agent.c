#include <stdint.h>
#include "Message.h"
#include "BattleBoats.h"
#include "BOARD.h"
#include "Agent.h"
#include "BattleBoats.h"
#include "Message.h"
#include "Negotiation.h"
#include "Field.h"
#include "Oled.h"
#include "BOARD.h"
#include "Ascii.h"


/**
 * Defines the various states used within the agent state machines. All states should be used
 * within a valid agent implementation. Additionally there is no need for states outside of
 * those defined here.
 */
/*typedef enum {
    AGENT_STATE_START, //0
    AGENT_STATE_CHALLENGING, //1
    AGENT_STATE_ACCEPTING, //2
    AGENT_STATE_ATTACKING, //3
    AGENT_STATE_DEFENDING, //4
    AGENT_STATE_WAITING_TO_SEND, //5
    AGENT_STATE_END_SCREEN, //6

    //If implementing a human agent, you will need a state to setup boats:
    AGENT_STATE_SETUP_BOATS, //7
} AgentState;*/

typedef struct {
    AgentState state;
    uint8_t turncounter;
    Field ownField;
    Field oppField;
    NegotiationData A;
    NegotiationData B;
    NegotiationData hashA;
    NegotiationData hashB;
    Message M;
    GuessData attack;
    GuessData Defend;

}agentData;

static agentData AD;

#define Ships_Sunk 0x0F

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts.  At a minimum, this requires:
 *   -setting the start state of the Agent SM.
 *   -setting turn counter to 0
 * If you are using any other persistent data in Agent SM, that should be reset as well.
 * 
 * It is not advised to call srand() inside of AgentInit.  
 *  */
void AgentInit(void){
    AD.state = AGENT_STATE_START;
    AD.turncounter = 0;
}

/**
 * AgentRun evolves the Agent state machine in response to an event.
 * 
 * @param  The most recently detected event
 * @return Message, a Message struct to send to the opponent. 
 * 
 * If the returned Message struct is a valid message
 * (that is, not of type MESSAGE_NONE), then it will be
 * passed to the transmission module and sent via UART.
 * This is handled at the top level! AgentRun is ONLY responsible 
 * for generating the Message struct, not for encoding or sending it.
 */
Message AgentRun(BB_Event event){
    switch(AD.state){
        case AGENT_STATE_START:
            if(event.type == BB_EVENT_START_BUTTON){
               AD.A = rand();//get 
               AD.hashA = NegotiationHash(AD.A);
               FieldInit(&AD.ownField, &AD.oppField);//initialize field
               AD.state = AGENT_STATE_CHALLENGING;//change state to challenge
               FieldAIPlaceAllBoats(&AD.ownField);//populate own field
               AD.M.type = MESSAGE_CHA;
               AD.M.param0 = AD.hashA;
               return AD.M;
            }
            else if(event.type == BB_EVENT_CHA_RECEIVED){
                AD.B = rand();
                FieldInit(&AD.ownField, &AD.oppField);//initialize field
                AD.state = AGENT_STATE_ACCEPTING;//change state to accepting
                FieldAIPlaceAllBoats(&AD.ownField);//populate own field
                AD.M.type = MESSAGE_ACC;//send ACC
                return AD.M;
            }
           
        case AGENT_STATE_CHALLENGING:
            if(event.type == BB_EVENT_ACC_RECEIVED){
                if(NegotiateCoinFlip(AD.A, event.param0) == HEADS){//when 
                    AD.state = AGENT_STATE_WAITING_TO_SEND;
                }
                else if(NegotiateCoinFlip(AD.A, event.param1) == TAILS){
                    AD.state = AGENT_STATE_DEFENDING;
                }
                AD.M.type = MESSAGE_REV;//send REV
                return AD.M;    
            }
  
        case AGENT_STATE_ACCEPTING:
            if(event.type == BB_EVENT_REV_RECEIVED){
                if(NegotiateCoinFlip(AD.B, event.param0) == HEADS){//when 
                    AD.state = AGENT_STATE_DEFENDING;
                }
                else if(NegotiateCoinFlip(AD.B, event.param1) == TAILS){//when 
                    FieldAIDecideGuess(&AD.oppField);//decide guess
                    AD.M.type = MESSAGE_SHO;//send SHO
                    return AD.M;
                    AD.state = AGENT_STATE_ATTACKING;
                }
                else if(NegotiationVerify(AD.A, AD.B) == FALSE){//when it detected cheating
                    
                    OledDrawString("The System has detected Cheating!\n");
                    OledUpdate();
                    AD.state = AGENT_STATE_END_SCREEN;
                }
            return AD.M;    
            }
            
        case AGENT_STATE_WAITING_TO_SEND:
            if(event.type == BB_EVENT_MESSAGE_SENT){
                AD.turncounter ++;
                
                AD.attack = FieldAIDecideGuess(&AD.oppField);//decide guess
                AD.M.type = MESSAGE_SHO;//send SHO
                AD.M.param0 = AD.attack.row;
                AD.M.param1 =AD.attack.col;
                AD.state = AGENT_STATE_ATTACKING;
                return AD.M;
            }
            
        case AGENT_STATE_ATTACKING:
            if(event.type == BB_EVENT_RES_RECEIVED){
                FieldUpdateKnowledge(&AD.oppField, &AD.attack);//update record of enemy field
                if((FieldGetBoatStates(&AD.oppField) & Ships_Sunk) == 0){
                 
                    OledDrawString("Event Result: Victory!");
                    OledUpdate();
                    AD.state = AGENT_STATE_END_SCREEN;
                
                }
                else{
                    AD.state = AGENT_STATE_DEFENDING;
                }
                
            return AD.M;    
            }
            
        case AGENT_STATE_DEFENDING:
            if(event.type == BB_EVENT_SHO_RECEIVED){
                AD.Defend.row = event.param0;
                AD.Defend.col = event.param1;
                FieldRegisterEnemyAttack(&AD.ownField, &AD.Defend);
             
                if((FieldGetBoatStates(&AD.ownField) & Ships_Sunk) == 0){
                   
                   OledDrawString("Event Result: Defeat.");
                   OledUpdate();
                   AD.state = AGENT_STATE_END_SCREEN; 
                }
                else{
                   AD.state = AGENT_STATE_WAITING_TO_SEND;
                }
               
                AD.M.type = MESSAGE_RES;//send RES
                AD.M.param0 = event.param0;
                AD.M.param1 = event.param1;
                AD.M.param2 = AD.Defend.result;
                return AD.M; 
            }
            
        case AGENT_STATE_END_SCREEN:
            AD.M.type = MESSAGE_NONE;
            return AD.M;
            
        case AGENT_STATE_SETUP_BOATS:
            AD.M.type = MESSAGE_NONE;
            return AD.M;
    
        }
    return AD.M;
    }

/** * 
 * @return Returns the current state that AgentGetState is in.  
 * 
 * This function is very useful for testing AgentRun.
 */
AgentState AgentGetState(void){
    return AD.state;
}

/** * 
 * @param Force the agent into the state given by AgentState
 * 
 * This function is very useful for testing AgentRun.
 */
void AgentSetState(AgentState newState){
    AD.state = newState;
}
