// Mathias Eriksen 
// mterikse@ucsc.edu
#include "stack.h"

void StackInit(struct Stack *stack)
{
    stack -> currentItemIndex = -1;
    // Initiate current index to -1
    stack -> initialized = TRUE;
    // set initialized state to true
}

int StackPush(struct Stack *stack, double value)
{
    if ((stack -> initialized == FALSE) || 
            (stack -> currentItemIndex == (STACK_SIZE - 1)))
    {
        return STANDARD_ERROR;
        // returns an error if stack is not initialized, or if stack is full
        // we cannot push to a full or uninitialized stack
    }
    
    stack -> currentItemIndex += 1;
    // iterate current index, starts at zero, which is why index is initialized
    // to  -1
    stack -> stackItems[stack -> currentItemIndex] = value;
    // push the value to the stack in the current index
    
    return SUCCESS;
}

int StackPop(struct Stack *stack, double *value)
{
    if ((stack -> initialized == FALSE) || (stack -> currentItemIndex == -1))
        // cannot pop if stack is uninitialized or empty 
    {
        return STANDARD_ERROR;
    }
    
    *value = stack -> stackItems[stack -> currentItemIndex];
    // value is equal to the item in the stack at the current index
    stack -> currentItemIndex -= 1;
    // decrement the item index 
    return SUCCESS;
}

int StackIsEmpty(const struct Stack *stack)
{
    if ((stack -> currentItemIndex) == -1)
    // if current index is a -1, the stack is empty
    {
        return TRUE;
    } else {
        return FALSE;
    }
}

int StackIsFull(const struct Stack *stack)
{
    if (stack -> currentItemIndex == (STACK_SIZE - 1))
    // if current index is at 19, it is full (20 items) since index starts at 0
    {
        return TRUE;
    } else {
        return FALSE;
    }
}

int StackGetSize(const struct Stack *stack)
{
    if (stack -> initialized == FALSE)
    // error for uninitialized stacks
    {
        return SIZE_ERROR;
    }
    
    return (stack -> currentItemIndex) + 1;
    // size will be current index + 1, since indexes from 0
}
