// Mathias Eriksen 
// mterikse@ucsc.edu

#include <stdio.h>
#include "stack.h"
#include "rpn.h"
#include "string.h"

#define MAX_STRING_LENGTH 255

int RPN_Evaluate(char * rpn_string, double * result)
{
    int i; // counter variable
    
    struct Stack rpnstack; // create the stack
    StackInit(&rpnstack); // initiate the stack 
    
    double val1, val2, pushval; // operands and result variable
      
    char*token; // initiate pointer token
    
    int stringlength;
    stringlength = strlen(rpn_string);
    // Check if string is too large, if so, invalid error
    if (stringlength > MAX_STRING_LENGTH) {
        return RPN_ERROR_INVALID_TOKEN; // Error code
    }
    
    // take apart input string, splitting into smaller 
    // strings at spaces and newlines
    token = strtok(rpn_string, " \n"); 
    
    while (token != NULL){
    // when token = NULL, done with all inputs
        if (strcmp(token, "+") == 0){
            if (StackPop(&rpnstack, &val1) == STANDARD_ERROR){
                // if statement Pops first operand from stack
                return RPN_ERROR_STACK_UNDERFLOW;
            } else if (StackPop(&rpnstack, &val2) == STANDARD_ERROR) {
                // if statement Pops second operand from stack
                return RPN_ERROR_STACK_UNDERFLOW;
            } else {
                *result = val1 + val2;
                // pointer to result
                if (StackPush(&rpnstack, *result) == STANDARD_ERROR) {
                    // push result to stack
                    return RPN_ERROR_STACK_OVERFLOW;
                 }
            }
            
        // same premise as add
        } else if (strcmp(token, "-") == 0){
             if (StackPop(&rpnstack, &val1) == STANDARD_ERROR){
                return RPN_ERROR_STACK_UNDERFLOW;
            } else if (StackPop(&rpnstack, &val2) == STANDARD_ERROR) {
                return RPN_ERROR_STACK_UNDERFLOW;
            } else {
                *result = val1 - val2;
                if (StackPush(&rpnstack, *result) == STANDARD_ERROR) {
                     return RPN_ERROR_STACK_OVERFLOW;
                 }
            }
             
        // same premise as add     
        } else if (strcmp(token, "*") == 0){
            if (StackPop(&rpnstack, &val1) == STANDARD_ERROR){
                return RPN_ERROR_STACK_UNDERFLOW;
            } else if (StackPop(&rpnstack, &val2) == STANDARD_ERROR) {
                return RPN_ERROR_STACK_UNDERFLOW;
            } else {
                *result = val1 * val2;
                 if (StackPush(&rpnstack, *result) == STANDARD_ERROR) {
                     return RPN_ERROR_STACK_OVERFLOW;
                 }
            }
          
        // same premise as add    
        } else if (strcmp(token, "/") == 0) {
            if (StackPop(&rpnstack, &val1) == STANDARD_ERROR){
                return RPN_ERROR_STACK_UNDERFLOW;
            } 
            
            else if (StackPop(&rpnstack, &val2) == STANDARD_ERROR) {
                return RPN_ERROR_STACK_UNDERFLOW;
            } else if (val2 == 0) {
                // check for divide by zero error
                return RPN_ERROR_DIVIDE_BY_ZERO;
            } else {
                *result = val1/val2;
                if (StackPush(&rpnstack, *result) == STANDARD_ERROR) {
                     return RPN_ERROR_STACK_OVERFLOW;
                 }
            }
            
        // this section checks non operator strings by their ASCII codes
        } else {
            
            int decimal = 0; // decimal counter
            int neg = 0; // negative sign counter
            for (i=0 ; i < strlen(token); i++) {                
                // iterate through the whole input string
                // because the whole thing needs to be numbers, with one
                // or zero decimal points or negative signs
                if (('0' > token[i]) || ('9' < token[i])) {
                    // Checks if character is inside of numerical
                    // ASCII range
                    if ((token[i] == '.') && (decimal == 0)){
                        // we are allowed only one decimal, if more than 
                        // one, invalid token
                        decimal += 1;
                        continue; // does not hit invalid token if only one
                    } if ((token[i] == '-') && (neg == 0)) {
                        // we are allowed one negative sign
                        neg += 1;
                        continue; // does not hit invalid token if only one
                    }                                    
                    return RPN_ERROR_INVALID_TOKEN;
                }
            }
            // Cannot push value if stack is full
            if (StackIsFull(&rpnstack) == TRUE){
                return RPN_ERROR_STACK_OVERFLOW;
            } else {
            // if it got here, it is a usable number
            pushval = atof(token);
            // turn token string into double
            StackPush(&rpnstack, pushval);
            // push the number to the stack!
            }
        }       
    token = strtok(NULL, " \n"); 
    // pass a NULL pointer to get next token
    }  
    
    // Check that stack has one and only one item left on it
    if (StackPop(&rpnstack, result) == STANDARD_ERROR){
        return RPN_ERROR_TOO_FEW_ITEMS_REMAIN;
    } else if (StackIsEmpty(&rpnstack) != TRUE){
        return RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
    }
    return RPN_NO_ERROR;
}




int ProcessBackspaces(char *rpn_sentence)
{
    int i = 0, j = 0; // counter variables
    int length = strlen(rpn_sentence); // take length of input string
    
    while (i < length){ // check the whole string 
        while (rpn_sentence[i] == '\b'){ // while we have backspaces 
            i++; // move forward in original string counter
            j--; // move backward in new string counter
            // once its not a backspace, counter will be at the value to replace
            // in i
        }
        rpn_sentence[j] = rpn_sentence[i];
        // take the value at i, and put it in the new string at j
        i++;
        j++;
        // iterate through the whole string
    } 
    rpn_sentence[j] = 0;
    // set end of string = 0 
    return strlen(rpn_sentence);
    // have function return length 
}
