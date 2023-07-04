/* 
 * File:   part0.c
 * Author: Mathias Eriksen
 *
 * Created on September 28, 2022, 10:41 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"

/*
 * 
 */

int main(void)
{
    BOARD_Init();
    printf("Hello World\n");
    
    BOARD_End();
    while(1);
}   

