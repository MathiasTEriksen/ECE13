// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"



// User libraries
#include "rpn.h"

int main()
{
    BOARD_Init();
    int error;

    printf("\n###### Beginning mterikse's rpn test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!
    
    char test0[] = "2 2 +\n";
    double result0;
    double expected0 = 4;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test0);
    error = RPN_Evaluate(test0, &result0);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n");
    } else if (result0 != expected0) {
        printf("   Failed, expected = %f , result = %f\n", expected0, result0);
    } else {
        printf("   Success %f!\n\n", result0);
    }

    
    char test1[] = "2 2g +\n";
    printf("Testing RPN_Evaluate with \"%s\"... \n", test1);
    
    double result1;
    error = RPN_Evaluate(test1, &result1);
    
    if (error == RPN_ERROR_INVALID_TOKEN){
        printf("   Success Correct Error!\n\n");
    } else {
       printf("   Failed, RPN_Evaluate produced an incorrect error\n"); 
    }

    char test2[] = "0 2 /\n";
    printf("Testing RPN_Evaluate with \"%s\"... \n", test1);
    
    double result2;
    error = RPN_Evaluate(test2, &result2);
    
    if (error == RPN_ERROR_DIVIDE_BY_ZERO){
        printf("   Success Correct Error!\n\n");
    } else {
       printf("   Failed, RPN_Evaluate produced an incorrect error\n"); 
    }
    
    char test3[] = "10 2 /\n";
    printf("Testing RPN_Evaluate with \"%s\"... \n", test1);
    
    double result3;
    error = RPN_Evaluate(test3, &result3);
    
    if (result3 == 0.2){
        printf("   Success %f!\n\n", result3);
    } else {
       printf("   Failed, RPN_Evaluate produced an incorrect result\n"); 
    }
    
    printf("Testing ProcessBackspaces:\n");
    char test_pb1[] = "123\b34\b";
    ProcessBackspaces(test_pb1);
    printf("result    : %s\n", test_pb1);
    printf("should be : 123\n");
  
        
    char buffer[100];

    strncpy(buffer, "Sample input", 100);

    buffer[8] = 0;

    int strSize = strlen(buffer);
    
    int n = (2 << 2) + 1 & 3;
    int x = 9 & 3;
    printf("%d\n", strSize);
    printf("%d\n", n);
    printf("%d\n", x);
    
    BOARD_End();
    while (1);
}


