/*====================\
    Mathias Eriksen    
    mterikse@ucsc.edu
    ECE13 
    Lab09                  
\=====================*/

#include "Field.h"
#include "BOARD.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define FIELD_BOAT_SIZE_ZERO 0
#define FIELD_DIRS 1

void FieldPrint_UART(Field *own_field, Field * opp_field){
    
    int i;
    int j;
       
    printf("         Own Field \n\n");
    printf("    0 1 2 3 4 5 6 7 8 9\n");
    printf("   ---------------------\n");
    
    for (i=0 ; i < FIELD_ROWS ; i++){
        printf("%d [ ", i);
        for (j=0 ; j < FIELD_COLS; j++){
            switch(own_field->grid[i][j]){
                case (FIELD_SQUARE_EMPTY):
                    printf(". ");
                    break;
                case (FIELD_SQUARE_SMALL_BOAT):
                    printf("3 ");
                    break;
                case (FIELD_SQUARE_MEDIUM_BOAT):
                    printf("4 ");
                    break;
                case (FIELD_SQUARE_LARGE_BOAT):
                    printf("5 ");
                    break;
                case (FIELD_SQUARE_HUGE_BOAT):
                    printf("6 ");
                    break;
                case (FIELD_SQUARE_HIT):
                    printf("X ");
                    break;
                case (FIELD_SQUARE_MISS):
                    printf("  ");
                    break;
            }
            
        }
        printf("]\n");
    }
    
    i=0;
    j=0;
    
    printf("         Opp Field \n\n");
    printf("    0 1 2 3 4 5 6 7 8 9\n");
    printf("   ---------------------\n");
    
    for (i=0 ; i < FIELD_ROWS ; i++){
        printf("%d [ ", i);
        for (j=0 ; j < FIELD_COLS; j++){
            switch (opp_field->grid[i][j]){
                case (FIELD_SQUARE_UNKNOWN):
                    printf(". ");
                    break;
                case (FIELD_SQUARE_HIT):
                    printf("X ");
                    break;
                case (FIELD_SQUARE_MISS):
                    printf("  ");
                    break;
            }
        }
        printf("]\n");
    }
}

void FieldInit(Field *own_field, Field * opp_field){
    
    int i;
    int j;
    // initializes empty squares for own grid
    for (j=0 ; j < FIELD_COLS ; j++){
        for (i=0 ; i < FIELD_ROWS; i++){
            own_field->grid[i][j] = FIELD_SQUARE_EMPTY;
        }
    }
    
    i = 0;
    j = 0;
    // initializes unknown squares for opp grid
    for (j=0 ; j < FIELD_COLS; j++){
        for (i=0 ; i < FIELD_ROWS; i++){
            opp_field->grid[i][j] = FIELD_SQUARE_UNKNOWN;
        }
    }    
    
    // set lives
    own_field->smallBoatLives = FIELD_BOAT_SIZE_ZERO;
    own_field->mediumBoatLives = FIELD_BOAT_SIZE_ZERO;
    own_field->largeBoatLives = FIELD_BOAT_SIZE_ZERO;
    own_field->hugeBoatLives = FIELD_BOAT_SIZE_ZERO;
    
    opp_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    opp_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    opp_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    opp_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
}

SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col){
    
    return f->grid[row][col];
            
}

SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p){
    
    // save and return old value
    uint8_t old_value = f->grid[row][col];   
    f->grid[row][col] = p;    
    return old_value;
    
}

uint8_t FieldAddBoat(Field *own_field, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type) {
    
    int i = 0;
    // test variables
    uint8_t col_test = col;
    uint8_t row_test = row;
    
    // switch based on type of boat
    switch (boat_type){
        
        // check empty spaces, place boat
        case (FIELD_BOAT_TYPE_SMALL):
            if (dir == FIELD_DIR_EAST){ 
                if ((col + FIELD_BOAT_SIZE_SMALL) > (FIELD_COLS)){
                    return STANDARD_ERROR;
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_SMALL) ; i++){
                    if (own_field->grid[row][col_test++] != FIELD_SQUARE_EMPTY){
                        return STANDARD_ERROR;
                    }
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_SMALL) ; i++) {
                    own_field->grid[row][(col++)] = FIELD_SQUARE_SMALL_BOAT;
                }
                          
            }
            else if (dir == FIELD_DIR_SOUTH){
                if ((row + FIELD_BOAT_SIZE_SMALL) > (FIELD_ROWS)){
                    return STANDARD_ERROR;
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_SMALL) ; i++){
                    if (own_field->grid[row_test++][col] != FIELD_SQUARE_EMPTY){
                        return STANDARD_ERROR;
                    }
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_SMALL) ; i++) {
                    own_field->grid[row++][(col)] = FIELD_SQUARE_SMALL_BOAT;
                }             
            }
            own_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
            break;
        // check empty spaces, place boat
        case (FIELD_BOAT_TYPE_MEDIUM):
            if (dir == FIELD_DIR_EAST){ 
                if ((col + FIELD_BOAT_SIZE_MEDIUM) > (FIELD_COLS)){
                    return STANDARD_ERROR;
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_MEDIUM) ; i++){
                    if (own_field->grid[row][col_test++] != FIELD_SQUARE_EMPTY){
                        return STANDARD_ERROR;
                    }
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_MEDIUM) ; i++) {
                    own_field->grid[row][(col++)] = FIELD_SQUARE_MEDIUM_BOAT;
                }
                          
            }
            else if (dir == FIELD_DIR_SOUTH){
                if ((row + FIELD_BOAT_SIZE_MEDIUM) > (FIELD_ROWS)){
                    return STANDARD_ERROR;
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_MEDIUM) ; i++){
                    if (own_field->grid[row_test++][col] != FIELD_SQUARE_EMPTY){
                        return STANDARD_ERROR;
                    }
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_MEDIUM) ; i++) {
                    own_field->grid[row++][(col)] = FIELD_SQUARE_MEDIUM_BOAT;
                }             
            }
            own_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
            break;
        // check empty spaces, place boat
        case (FIELD_BOAT_TYPE_LARGE):
            if (dir == FIELD_DIR_EAST){ 
                if ((col + FIELD_BOAT_SIZE_LARGE) > (FIELD_COLS)){
                    return STANDARD_ERROR;
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_LARGE) ; i++){
                    if (own_field->grid[row][col_test++] != FIELD_SQUARE_EMPTY){
                        return STANDARD_ERROR;
                    }
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_LARGE) ; i++) {
                    own_field->grid[row][(col++)] = FIELD_SQUARE_LARGE_BOAT;
                }
                          
            }
            else if (dir == FIELD_DIR_SOUTH){
                if ((row + FIELD_BOAT_SIZE_LARGE) > (FIELD_ROWS)){
                    return STANDARD_ERROR;
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_LARGE) ; i++){
                    if (own_field->grid[row_test++][col] != FIELD_SQUARE_EMPTY){
                        return STANDARD_ERROR;
                    }
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_LARGE) ; i++) {
                    own_field->grid[row++][(col)] = FIELD_SQUARE_LARGE_BOAT;
                }             
            }
            own_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
            break;
        // check empty spaces, place boat
        case (FIELD_BOAT_TYPE_HUGE):
            if (dir == FIELD_DIR_EAST){ 
                if ((col + FIELD_BOAT_SIZE_HUGE) > (FIELD_COLS)){
                    return STANDARD_ERROR;
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_HUGE) ; i++){
                    if (own_field->grid[row][col_test++] != FIELD_SQUARE_EMPTY){
                        return STANDARD_ERROR;
                    }
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_HUGE) ; i++) {
                    own_field->grid[row][(col++)] = FIELD_SQUARE_HUGE_BOAT;
                }
                          
            }
            else if (dir == FIELD_DIR_SOUTH){
                if ((row + FIELD_BOAT_SIZE_HUGE) > (FIELD_ROWS)){
                    return STANDARD_ERROR;
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_HUGE) ; i++){
                    if (own_field->grid[row_test++][col] != FIELD_SQUARE_EMPTY){
                        return STANDARD_ERROR;
                    }
                }
                for (i = 0; i < (FIELD_BOAT_SIZE_HUGE) ; i++) {
                    own_field->grid[row++][(col)] = FIELD_SQUARE_HUGE_BOAT;
                }             
            }
            own_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
            break;
    } 
    return SUCCESS;
    
}

SquareStatus FieldRegisterEnemyAttack(Field *own_field, GuessData *opp_guess){
    
    SquareStatus Result = own_field->grid[opp_guess->row][opp_guess->col];
    // switch based on square attack was on
    switch (own_field->grid[opp_guess->row][opp_guess->col]){
        // if its empty, register a miss
        case (FIELD_SQUARE_EMPTY):
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_MISS;
            opp_guess->result = RESULT_MISS;
            break;
        // occupied, check for sink, and register hit
        case (FIELD_SQUARE_SMALL_BOAT):
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            (own_field->smallBoatLives)--;
            if (own_field->smallBoatLives == 0){
                opp_guess->result = RESULT_SMALL_BOAT_SUNK;
            } else {
                opp_guess->result = RESULT_HIT;
            }
            break;
        // occupied, check for sink, and register hit
        case (FIELD_SQUARE_MEDIUM_BOAT):
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            (own_field->mediumBoatLives)--;
            if (own_field->mediumBoatLives == 0){
                opp_guess->result = RESULT_MEDIUM_BOAT_SUNK;
            } else {
                opp_guess->result = RESULT_HIT;
            }
            break;
        // occupied, check for sink, and register hit   
        case (FIELD_SQUARE_LARGE_BOAT):
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            (own_field->largeBoatLives)--;
            if (own_field->largeBoatLives == 0){
                opp_guess->result = RESULT_LARGE_BOAT_SUNK;
            } else {
                opp_guess->result = RESULT_HIT;
            }
            break;
        // occupied, check for sink, and register hit
        case (FIELD_SQUARE_HUGE_BOAT):
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            (own_field->hugeBoatLives)--;
            if (own_field->hugeBoatLives == 0){
                opp_guess->result = RESULT_HUGE_BOAT_SUNK;
            } else {
                opp_guess->result = RESULT_HIT;
            }      
            break;
    }
    
    return Result;
        
}

SquareStatus FieldUpdateKnowledge(Field *opp_field, const GuessData *own_guess){
    
    switch (own_guess->result){
        // update based on result of guess
        case (RESULT_SMALL_BOAT_SUNK):
           opp_field->smallBoatLives = FIELD_BOAT_SIZE_ZERO;
           opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
           break;
        case (RESULT_MEDIUM_BOAT_SUNK):
           opp_field->mediumBoatLives = FIELD_BOAT_SIZE_ZERO;
           opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
           break;
        case (RESULT_LARGE_BOAT_SUNK):
           opp_field->largeBoatLives = FIELD_BOAT_SIZE_ZERO;
           opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
           break;  
        case (RESULT_HUGE_BOAT_SUNK):
           opp_field->hugeBoatLives = FIELD_BOAT_SIZE_ZERO;
           opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
           break;
        case (RESULT_HIT):
           opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
           break;
        case (RESULT_MISS):
           opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_MISS;
           break;       
    }    
}

uint8_t FieldGetBoatStates(const Field *f){
    // grab states of boats based on bit placement
    uint8_t ALIVE_STATE = 0x00 ;
    
    if (f->smallBoatLives != 0){
        ALIVE_STATE =  ALIVE_STATE | FIELD_BOAT_STATUS_SMALL;        
    }
    
    if (f->mediumBoatLives != 0){
        ALIVE_STATE =  ALIVE_STATE | FIELD_BOAT_STATUS_MEDIUM;        
    }
    
    if (f->largeBoatLives != 0){
        ALIVE_STATE =  ALIVE_STATE | FIELD_BOAT_STATUS_LARGE;        
    }
    
    if (f->hugeBoatLives != 0){
        ALIVE_STATE =  ALIVE_STATE | FIELD_BOAT_STATUS_HUGE;        
    }
    
    return ALIVE_STATE;
}


uint8_t FieldAIPlaceAllBoats(Field *own_field){
    
    int RAND_ROW;
    int RAND_COL;
    BoatDirection RAND_DIR;
    
    BoatType PLACE_BOAT = FIELD_BOAT_TYPE_SMALL;
    
    while (1){
        // get random  vals
        RAND_ROW = (rand() % FIELD_ROWS);
        RAND_COL = (rand() % FIELD_COLS);
        RAND_DIR = (rand() & FIELD_DIRS);
        
        switch (PLACE_BOAT){
            // check in each case if boat can be placed 
            case (FIELD_BOAT_TYPE_SMALL):                
                if ((FieldAddBoat(own_field, RAND_ROW, RAND_COL, RAND_DIR, PLACE_BOAT)) == SUCCESS){
                    PLACE_BOAT = FIELD_BOAT_TYPE_MEDIUM;
                    break;                    
                }
            
            case (FIELD_BOAT_TYPE_MEDIUM):
                if ((FieldAddBoat(own_field, RAND_ROW, RAND_COL, RAND_DIR, PLACE_BOAT)) == SUCCESS){
                    PLACE_BOAT = FIELD_BOAT_TYPE_LARGE;
                    break;                    
                }
                
            case (FIELD_BOAT_TYPE_LARGE):
                if ((FieldAddBoat(own_field, RAND_ROW, RAND_COL, RAND_DIR, PLACE_BOAT)) == SUCCESS){
                    PLACE_BOAT = FIELD_BOAT_TYPE_HUGE;
                    break;                    
                }
            
            case (FIELD_BOAT_TYPE_HUGE):
                if ((FieldAddBoat(own_field, RAND_ROW, RAND_COL, RAND_DIR, PLACE_BOAT)) == SUCCESS){
                    PLACE_BOAT = FIELD_BOAT_TYPE_SMALL;
                    return SUCCESS;                 
                }

                
        }        
    }   
}

GuessData FieldAIDecideGuess(const Field *opp_field) {
    
    GuessData AI_GUESS;
    
    while (1){
        
        AI_GUESS.row = (rand() % FIELD_ROWS);
        AI_GUESS.col = (rand() % FIELD_COLS);
        // make sure AI doesn't guess somewhere it already has
        if ((FieldGetSquareStatus(opp_field, AI_GUESS.row, AI_GUESS.col) != FIELD_SQUARE_HIT) &&
             (FieldGetSquareStatus(opp_field, AI_GUESS.row, AI_GUESS.col) != FIELD_SQUARE_MISS)   ){
            break;
        }              
    }
    
    return AI_GUESS;
    
}