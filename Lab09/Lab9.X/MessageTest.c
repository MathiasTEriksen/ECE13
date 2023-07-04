#include "BOARD.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Message.h"
#include "BattleBoats.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
    
    printf("Testing rmikami's MessageTest harness...\n\n");
    
    printf("Testing Message_Encode() 1/3...\n");
    
    char string1[MESSAGE_MAX_LEN];
    char string2[MESSAGE_MAX_LEN];
    
    Message MessageTest;
    
    MessageTest.type = MESSAGE_CHA;
    MessageTest.param0 = 1;
    
    printf("Expected: $CHA,1*57\n");
    Message_Encode(string1, MessageTest);
    
    printf("Output: %s\n", string1);
    
    printf("Testing Message_Encode() 2/3...\n");
    
    MessageTest.type = MESSAGE_RES;
    MessageTest.param0 = 1;
    MessageTest.param1 = 5;
    MessageTest.param2 = 8;
    
    printf("Expected: $RES,1,5,8*54\n");
    Message_Encode(string2, MessageTest);
    
    printf("Output: %s\n", string2);
    
    printf("Testing Message_ParseMessage() 3/3...\n");
    
     MessageTest.type = MESSAGE_RES;
    MessageTest.param0 = 1;
    MessageTest.param1 = 0;
    MessageTest.param2 = 3;
    
    printf("Expected: $RES,1,0,3*5A\n");
    Message_Encode(string2, MessageTest);
    
    printf("Output: %s\n", string2);
    
    printf("Testing Message_ParseMessage() 1/2...\n");
    
    
    BB_Event TestEvent1;
    
    Message_ParseMessage("RES,1,0,3", "5A", &TestEvent1);
    
    if (TestEvent1.type == BB_EVENT_RES_RECEIVED){
        printf("PASSED: Correct Type\n");
    } else {
        printf("FAILED: Incorrect Type\n");
    }
      
    if (TestEvent1.param0 == 1){
        printf("PASSED: Correct param0\n");
    } else {
        printf("FAILED: Incorrect param0\n");
    }
    
    if (TestEvent1.param1 == 0){
        printf("PASSED: Correct param1\n");
    } else {
        printf("FAILED: Incorrect param1\n");
    }
    
    if (TestEvent1.param2 == 3){
        printf("PASSED: Correct param2\n");
    } else {
        printf("FAILED: Incorrect param2\n");
    }
    
    printf("\nTesting Message_ParseMessage() 2/2...\n");
    
    BB_Event TestEvent2;
    
    Message_ParseMessage("CHA,1", "57", &TestEvent2);
    
    if (TestEvent2.type == BB_EVENT_CHA_RECEIVED){
        printf("PASSED: Correct Type\n");
    } else {
        printf("FAILED: Incorrect Type\n");
    }
      
    if (TestEvent2.param0 == 1){
        printf("PASSED: Correct param0\n");
    } else {
        printf("FAILED: Incorrect param0\n");
    }
    
    printf("\nTesting Message_Decode() 1/2...\n");
    
    int i = 0;
    BB_Event DCtest1;
    char DecodeTest1[] = "$CHA,1*57\n";
    
    for (i=0; i<(strlen(DecodeTest1)); i++){
        if ((Message_Decode(DecodeTest1[i], &DCtest1)) == STANDARD_ERROR){
            printf("FAILED: STANDARD_ERROR\n");
        } 
    }
    
    if (DCtest1.type == BB_EVENT_CHA_RECEIVED){
        printf("PASSED: Correct Type\n");
    } else {
        printf("FAILED: Incorrect Type\n");
    }
      
    if (DCtest1.param0 == 1){
        printf("PASSED: Correct param0\n");
    } else {
        printf("FAILED: Incorrect param0\n");
    }   
    
    printf("\nTesting Message_Decode() 2/2...\n");
    
    i = 0;
    BB_Event DCtest2;
    char DecodeTest2[] = "$RES,1,0,3*5A\n";
    
    for (i=0; i<(strlen(DecodeTest2)); i++){
        if ((Message_Decode(DecodeTest2[i], &DCtest2)) == STANDARD_ERROR){
            printf("FAILED: STANDARD_ERROR\n");
        } 
    }
    
    if (DCtest2.type == BB_EVENT_RES_RECEIVED){
        printf("PASSED: Correct Type\n");
    } else {
        printf("FAILED: Incorrect Type\n");
    }
      
    if (DCtest2.param0 == 1){
        printf("PASSED: Correct param0\n");
    } else {
        printf("FAILED: Incorrect param0\n");
    }
    
      if (DCtest2.param1 == 0){
        printf("PASSED: Correct param1\n");
    } else {
        printf("FAILED: Incorrect param1\n");
    }
    
    if (DCtest2.param2 == 3){
        printf("PASSED: Correct param2\n");
    } else {
        printf("FAILED: Incorrect param2\n");
    }
    
    
    
    
    
}