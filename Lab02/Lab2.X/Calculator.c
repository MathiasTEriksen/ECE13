// Mathias Eriksen , mterikse@ucsc.edu
//
//
// Standard libraries
#include <stdio.h>
#include <math.h>


//User libraries:
#include "BOARD.h"

#ifdef PIC32
// Microchip libraries
#include <xc.h>
#endif


// **** Declare function prototypes ****
double Add(double operand1, double operand2);
double Subtract(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double Divide(double operand1, double operand2);
double Average(double operand1, double operand2);
double AbsoluteValue(double operand);
double FahrenheitToCelsius(double operand);
double CelsiusToFahrenheit(double operand);
double Tangent(double operand);
double Round(double operand);
//add more prototypes here

char c1;
double op1;
double op2;
double Val;

void CalculatorRun(void)
{
    printf("\n\nWelcome to Mathias' calculator program! Compiled at %s %s\n",__DATE__, __TIME__);
    
    // Infinite loop with while(1)
    
    while(1) { 
        printf("\nEnter a mathematical operation to perform (*,/,+,-,m,a,c,f,t,r): ");
        scanf(" %c", &c1);
        
        if ((c1 != '*') && (c1 != '/') && (c1 != '+') && (c1 != '-') && (c1 != 'm') &&
            (c1 != 'a') && (c1 != 'c') && (c1 != 'f') && (c1 != 't') && (c1 != 'r') &&
            (c1 != 'q')){
            c1 = '0';
            printf("\n  Error, not a valid operator\n");
            
            // Invalid operator check
            
        } else if (c1 == 'q'){
            break; 
            
            // Check for quit input
            
        } else {
            printf("\n    Enter the first operand: ");
            scanf(" %lf", &op1);              
            
            // Get first operand
            
            if (c1 == '+'){
                printf("\n    Enter the second operand: ");
                scanf(" %lf", &op2);
                Val = Add(op1, op2);
                printf("\n    Result of (%lf + %lf): %lf\n", op1, op2, Val);
            }
            
            // Addition Statement
            
            if (c1 == '-'){
                printf("\n    Enter the second operand: ");
                scanf(" %lf", &op2);
                Val = Subtract(op1, op2);
                printf("\n    Result of (%lf - %lf): %lf\n", op1, op2, Val);        
            }
            
            // Subtraction Statement
            
            if (c1 == '*'){
                printf("\n    Enter the second operand: ");
                scanf(" %lf", &op2);
                Val = Multiply(op1, op2);
                printf("\n    Result of (%lf * %lf): %lf\n", op1, op2, Val);        
            }
            
            // Multiplication Statement
            
            if (c1 == '/'){
                printf("\n    Enter the second operand: ");
                scanf(" %lf", &op2);
                if (op2 == 0) {
                    printf("\n    Divide by zero error\n");
                } else {
                Val = Divide(op1, op2);
                printf("\n    Result of (%lf / %lf): %lf\n", op1, op2, Val); 
                }
            }           
            
            // Division Statement
            
            if (c1 == 'm'){
                printf("\n    Enter the second operand: ");
                scanf(" %lf", &op2);
                Val = Average(op1, op2);
                printf("\n    Average of (%lf & %lf): %lf\n", op1, op2, Val);        
            }
            
            // Average Statement
            
            if (c1 == 'a'){
                Val = AbsoluteValue(op1);
                printf("\n    Result of |%lf|: %lf\n", op1, Val);
            }
            
            // Absolute Value Statement
            
             if (c1 == 'f'){
                Val = CelsiusToFahrenheit(op1);
                printf("\n    Result of (%lf C -> F): %lf\n", op1, Val);
            }
            
            // Celsius to Fahrenheit with f
            
             if (c1 == 'c'){
                Val = FahrenheitToCelsius(op1);
                printf("\n    Result of (%lf F -> C): %lf\n", op1, Val);
            }
            
             if (c1 == 't'){
                Val = Tangent(op1);
                printf("\n    Result of (tan(%lf)): %lf\n", op1, Val);
            }
            
             if (c1 == 'r'){
                Val = Round(op1);
                printf("\n    Result of (round(%lf)): %lf\n", op1, Val);
            }
        
            // Round Statement
            
        }
    }

    // Your code here
    BOARD_End();
    while (1);
}

/********************************************************************************
 * Define the Add function here.
 ********************************************************************************/
double Add(double operand1, double operand2)
{
    return (operand1+operand2);
}

/********************************************************************************
 * Define the Subtract function here.
 ********************************************************************************/
double Subtract(double operand1, double operand2)
{
	return (operand1-operand2);
}

/********************************************************************************
 * Define the Multiply function here.
 ********************************************************************************/
double Multiply(double operand1, double operand2)
{
	return (operand1*operand2);
}

/********************************************************************************
 * Define the Divide function here.
 ********************************************************************************/
double Divide(double operand1, double operand2)
{
	return (operand1/operand2);
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double operand)
{
	return (fabs(operand));
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double operand)
{
	return ((5.0 / 9.0)*(operand - 32.0));
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double operand)
{
	return ((9.0 / 5.0)*(operand) + 32.0);
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double operand1, double operand2)
{
	return ((operand1+operand2)/2);
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees.
 ********************************************************************************/
double Tangent(double operand)
{
	return (tan(operand));
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand)
{
    return(round(operand));
}
