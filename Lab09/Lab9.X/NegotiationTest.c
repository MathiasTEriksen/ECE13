// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

//CE13 standard libraries:
#include "Buttons.h"
#include "Uart1.h"
#include "Oled.h"

// Battleboats Libraries:
#include "BattleBoats.h"
#include "Agent.h"
#include "Negotiation.h"
#include "Message.h"
#include "Field.h"

int main(){
   
   printf("Testing mterikse's NegotiationTest harness...\n\n");

   
   printf("Testing NegotiationHash() 1/2:\n");
   int hashednum1 = NegotiationHash(3);
   if(hashednum1 == 9){
       printf("SUCCESS: Hashed\n");
   }
   else{
       printf("FAILED: Not hashed\n");
   }
   
   printf("\nTesting NegotiationHash() 2/2:\n");
   int hashednum2 = NegotiationHash(12345);
   if(hashednum2 == 43182){
       printf("SUCCESS: Hashed\n");
   }
   else{
       printf("FAILED: Not hashed\n");
   }
   
   printf("\nTesting NegotiationVerify() 1/2:\n");
   if(NegotiationVerify(3, hashednum1) == TRUE){
       printf("Verified\n");
   }
   else{
       printf("Not verified\n");
   }
   
   printf("\nTesting NegotiationVerify() 2/2:\n");
   if(NegotiationVerify(12345, hashednum2) == TRUE){
       printf("Verified\n");
   }
   else{
       printf("Not verified\n");
   }
      
   printf("\nTesting NegotiateCoinFlip() 1/2:\n");
   int ncflip = NegotiateCoinFlip(9,6);
   if(ncflip == 1){
        printf("HEADS\n");
   }
   else{
         printf("TAILS\n");
   }
   return 0;
}
