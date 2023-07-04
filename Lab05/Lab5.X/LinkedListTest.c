// Mathias Eriksen
// mterikse@ucsc.edu

#include <stdio.h>
#include <string.h>

#include "BOARD.h"
#include "LinkedList.h"



int main(){
    
    printf("\n#### Beginning mterikse's LinkedList test harness: ####\n\n");
    
    // Simply execute each function and use the print function 
    // to compare outputs to desired ones, remember functions
    // accept addresses to list items as arguments
    
    printf("Testing LinkedListNew(): (1/1)\n");
    ListItem *test1 = LinkedListNew("string");
    printf("Expected: [string, ]\n");
    printf("Output:   ");
    LinkedListPrint(test1);
    printf("\n");
    
    printf("Testing LinkedListCreateAfter(): (1/3)\n");
    ListItem *test2 = LinkedListCreateAfter(test1, "b");
    printf("Expected: [string, b, ]\n");
    printf("Output:   ");
    LinkedListPrint(test2);
    printf("\n");
    
    printf("Testing LinkedListCreateAfter(): (2/3)\n");
    ListItem *test3 = LinkedListCreateAfter(test1, "between");
    printf("Expected: [string, between, b, ]\n");
    printf("Output:   ");
    LinkedListPrint(test3);
    printf("\n");
    
    printf("Testing LinkedListCreateAfter(): (3/3)\n");
    ListItem *test4 = LinkedListCreateAfter(test2, NULL);
    printf("Expected: [string, between, b, (null), ]\n");
    printf("Output:   ");
    LinkedListPrint(test4);
    printf("\n");
           
    printf("Testing LinkedListRemove(): (1/1)\n");
    LinkedListRemove(test2); 
    printf("Expected: [string, between, (null), ]\n");
    printf("Output:   ");
    LinkedListPrint(test1);
    printf("\n");
    
    printf("Testing LinkedListSize(): (1/1)\n");
    int val = LinkedListSize(test1); 
    printf("Expected: 3\n");
    printf("Output:   %d\n", val);
    printf("\n");
    
    printf("Testing LinkedListGetFirst(): (1/1)\n");
    ListItem *first = LinkedListGetFirst(test4); 
    printf("Expected: string\n");
    printf("Output:   %s\n", first -> data);
    printf("\n");
    
    printf("Testing LinkedListGetLast(): (1/1)\n");
    ListItem *last = LinkedListGetLast(test4); 
    printf("Expected: (null)\n");
    printf("Output:   %s\n", last -> data);
    printf("\n");
    
    printf("Testing LinkedListSwapData(): (1/2)\n");
    LinkedListSwapData(test1, test4);
    printf("Expected: [(null), between, string, ]\n");
    printf("Output:   ");
    LinkedListPrint(test1);
    printf("\n");
    
    printf("Testing LinkedListSwapData(): (2/2)\n");
    LinkedListSwapData(test4, test3);
    printf("Expected: [(null), string, between, ]\n");
    printf("Output:   ");
    LinkedListPrint(test1);
    printf("\n");
    
    
    int a = 7;

    int b = 3;

    char n = 0x30;

    switch (a | b) {

    case 0: case 1: case 2: case 3: case 4:

          n += 2;

    case 5: case 6: case 7: case 8: case 9:

          n += 3;

    }

    printf("%d\n", n);
    
    struct Packed {
   unsigned int a: 2;
   unsigned int b: 2;

   unsigned :4;
   signed int c: 8;

    };
    float x = sizeof(struct Packed);
    printf("packed: %f", x);
    
    enum u {A, B};
    
    printf("A: %d, B: %d",A,B);

  
    printf("\n#########################################################\n\n");
    
}
