// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "stack.h"

int main()
{
    BOARD_Init();

    printf("\n###### Beginning mterikse's stack test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!
    int temp = 0;
    int count = 0;
    
    // Initiate test (Only one)
    struct Stack stack = {};
    StackInit(&stack);
    if (stack.currentItemIndex == -1) {
        printf("PASSED 1/1: StackInit()\n");
        count += 1;
    } else {
        printf("FAILED 0/1: StackInit()\n");
    }
    
    // Push tests
    StackPush(&stack, 10);
    if (stack.currentItemIndex == 0){
        temp += 1;
        count += 1;
    }    
    StackPush(&stack, 20);
    if (stack.currentItemIndex == 1){
        temp += 1;
        count += 1;
    }    
    if (temp == 2){
        printf("PASSED 2/2: StackPush()\n");
    } else if (temp == 1){
        printf("FAILED 1/2: StackPush()\n");
    } else {
        printf("FAILED 0/2: StackPush()\n");
    }
    
    // Pop tests
    temp = 0;

    double popval;
    StackPop(&stack,&popval);
    if (popval == 20){
        temp += 1;
        count += 1;
    }    
    StackPop(&stack, &popval);
    if (popval == 10){
        temp += 1;
        count += 1;
    }    
    if (temp == 2){
        printf("PASSED 2/2: StackPop()\n");
    } else if (temp == 1){
        printf("FAILED 1/2: StackPop()\n");
    } else {
        printf("FAILED 0/2: StackPop()\n");
    }
    
    
    // Empty test
    temp = 0;
    
    if (StackIsEmpty(&stack) == TRUE){
        temp += 1;
        count += 1;
    }
   
    StackPush(&stack, 10);
    if (StackIsEmpty(&stack) == FALSE){
        temp += 1;
        count += 1;
    }
    
    if (temp == 2){
        printf("PASSED 2/2: StackIsEmpty()\n");
    } else if (temp == 1){
        printf("FAILED 1/2: StackIsEmpty()\n");
    } else {
        printf("FAILED 0/2: StackIsEmpty()\n");
    }
    
    // Full test
    temp = 0;
    // empty stack
    StackPop(&stack, &popval);
    
    int i;
    for (i=0;i<20;i++)
    {
        StackPush(&stack, i);
    }    
    
    if (StackIsFull(&stack) == TRUE){
        temp += 1;
        count += 1;
    }
    
    StackPop(&stack,&popval);
    if (StackIsFull(&stack) == FALSE){
        temp += 1;
        count += 1;
    }         
    
    if (temp == 2){
        printf("PASSED 2/2: StackIsFull()\n");
    } else if (temp == 1){
        printf("FAILED 1/2: StackIsFull()\n");
    } else {
        printf("FAILED 0/2: StackIsFull()\n");
    }
    

    // Stack size test
    temp = 0;
    if (StackGetSize(&stack) == 19){
        temp += 1;
        count += 1;
    }
    
    StackPush(&stack, 10);
    
    if (StackGetSize(&stack) == 20){
        temp += 1;
        count += 1;
    }
    
    if (temp == 2){
        printf("PASSED 2/2: StackSize()\n");
    } else if (temp == 1){
        printf("FAILED 1/2: StackSize()\n");
    } else {
        printf("FAILED 0/2: StackSize()\n");
    }
    
    // Check total passes
    printf("-------------------------\n");
    printf("%i out of 11 functions passed\n", count);
    
    BOARD_End();
    //test stackInit:
    while (1);
    return 0;
}


