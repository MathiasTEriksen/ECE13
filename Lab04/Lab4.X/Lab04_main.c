// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// User libraries
#include "rpn.h"

// **** Set macros and preprocessor directives ****
// The lab calls for 60 characters of user input
#define MAX_INPUT_LENGTH 60

int main() {
    BOARD_Init();

    //What follows is starter code.  You will need to modify it!

    char rpn_sentence[MAX_INPUT_LENGTH + 2];
    double result;

    printf("Welcome to mterikse'S RPN calculator.  Compiled on %s %s\n", __DATE__, __TIME__);
    while (1) {

        printf("Enter floats and + - / * in RPN format:\n");

        fgets(rpn_sentence, sizeof (rpn_sentence), stdin);
        
        ProcessBackspaces(rpn_sentence);
        int error = RPN_Evaluate(rpn_sentence, &result);
        
        if (error == 0) {
        printf("Result = %f\n", result);
        } else if (error == RPN_ERROR_STACK_OVERFLOW) {
            printf("ERROR: Too many items on stack.\n");
        } else if (error == RPN_ERROR_STACK_UNDERFLOW) {
            printf("ERROR: Not enough operands before operator\n");
        } else if (error == RPN_ERROR_INVALID_TOKEN) {
            printf("ERROR: Invalid character in rpn string.\n");
        } else if (error == RPN_ERROR_DIVIDE_BY_ZERO) {
            printf("ERROR: Divide by zero error.\n");
        } else if (error == RPN_ERROR_TOO_FEW_ITEMS_REMAIN) {
            printf("ERROR: Too few items left on stack\n");
        } else if (error == RPN_ERROR_TOO_MANY_ITEMS_REMAIN) {
            printf("ERROR: Too many items left on stack.\n");
        }                  
    }
    while (1);
}
