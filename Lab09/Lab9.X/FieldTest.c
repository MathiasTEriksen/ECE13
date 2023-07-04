/*====================\
    Mathias Eriksen    
    mterikse@ucsc.edu
    ECE13 
    Lab09                  
\=====================*/

#include "Field.h"
#include "FieldOled.h"
#include "BOARD.h"
#include "Oled.h"
#include "OledDriver.h"


#include <stdlib.h>
#include <stdio.h>


int main(){
    
    printf("Testing rmikami's FieldTest harness...\n\n");
    
    int i;
    
    Field own_field;
    Field opp_field;
    
    GuessData Guess;
    
    FieldInit(&own_field, &opp_field);    
    FieldAIPlaceAllBoats(&own_field);
    FieldPrint_UART(&own_field, &opp_field);
    
    for (i=0; i<16; i++){
        Guess = FieldAIDecideGuess(&own_field);
        FieldRegisterEnemyAttack(&own_field, &Guess);
    }

    FieldPrint_UART(&own_field, &opp_field);
    
    for (i=0; i<16; i++){
        Guess = FieldAIDecideGuess(&own_field);
        FieldRegisterEnemyAttack(&own_field, &Guess);
    }

    FieldPrint_UART(&own_field, &opp_field);
    
}