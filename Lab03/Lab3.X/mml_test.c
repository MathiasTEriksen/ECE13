// Mathias Eriksen 
// mterikse@ucsc.edu
// File : mml_test.c

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h> // for fabs()

//CMPE13 Support Library
#include "BOARD.h"

// User libraries:
#include "MatrixMath.h"

// Module-level variables:
float idmat[3][3] = {{1,0,0},{0,1,0},{0,0,1}}; // ID Matrix
float mat1[3][3] = {{1,2,3},{1,2,3},{1,2,3}}; // First Matrix
float mat2[3][3] = {{4,5,-6},{4,-5,6},{-4,5,6}}; // Second Matrix
float mat3[3][3] = {{1,-7,-9},{4.0,-1,2},{10.00,-1,2}}; // Third Matrix

// Expected values for add
float addid1[3][3] = {{2,2,3},{1,3,3},{1,2,4}}; // Add idmat to mat1
float add12[3][3] = {{5,7,-3},{5,-3,9},{-3,7,9}}; // Add mat1 to mat2

// Expected values for multiply
float multid1[3][3] = {{1,0,0},{0,2,0},{0,0,3}}; // Multiply idmat and mat1
float mult23[3][3] = {{4,-35,54},{16,5,12},{-40,-5,12}}; // Multiply mat2 and mat3

// Expected Values for Scalar Add
float saddid[3][3] = {{11,10,10},{10,11,10},{10,10,11}}; // Scalar add 10 to idmat
float sadd3[3][3] = {{-4,-12,-14},{-1,-6,-3},{5,-6,-3}}; // Scalar add -5 to mat3

// Expected Scalar Multiply
float smultid[3][3] = {{3,0,0},{0,3,0},{0,0,3}}; // Scalar multiply id mat by 3 
float smult2[3][3] = {{-8,-10,12},{-8,10,-12},{8,-10,-12}}; // Scalar multiply mat2 by -2

// Expected Transpose Values
float trans2[3][3] = {{4,4,-4},{5,-5,5},{-6,6,6}}; // Transpose mat2
float trans3[3][3] = {{1,4,10},{-7,-1,-1},{-9,2,2}}; // Transpose mat3

// Expected Submatrix Values
float sub00[2][2] = {{1,0},{0,1}}; // Remove row, column 0 from idmat
float sub12[2][2] = {{1,0},{0,0}}; // Remove row 1, column 2, from idmat

// Expected Trace Values
float trace; // Float for use with expected trace values

// Empty Result Matrices
float result[3][3] = {{},{},{}}; // 3x3
float smallresult[2][2] = {{},{}}; // 2x2

// Expected Determinant Values
float det, expdet; // Float expected values (Ensure correct formats)

// Expected Inverse Values
float inverse2[3][3] = {{1.0/8.0, 1.0/8.0, 0.0}, // Inverse of mat2 from symbolab
                        {1.0/10.0, 0.0, 1.0/10.0},
                        {0.0, 1.0/12.0, 1.0/12.0}};
// Floating point fractions only, 1/8 is 0, do 1.0/8.0 to get float out

float inverse3[3][3] = {{0, -1.0/6.0, 1.0/6.0}, // Inverse of mat3 from symbolab
                        {-2.0/23.0, -2.0/3.0, 19.0/69.0},
                        {-1.0/23.0, 1.0/2.0,-9.0/46.0}};

// Initiate counter test variables
int count, tempcount;

int main()
{
    BOARD_Init();

    printf("Beginning mterikse's mml test harness, compiled on %s %s\n\n", __DATE__, __TIME__);
    
    // Equals Check
    count = 0 ;
    
    if ((MatrixEquals(mat1,mat1)) && (MatrixEquals(mat1,mat2) == 0)) {
        printf("PASSED (2/2): MatrixEquals()\n");
        count = count + 2;
    } else if ((MatrixEquals(mat1,mat1) == 0) && (MatrixEquals(mat1,mat2) == 1)) {
        printf("FAILED (0/2): MatrixEquals()\n");
    } else {
        printf("FAILED (1/2): MatrixEquals()\n");
        count = count + 1;
    }
    // Tests if equals function works by checking output from function
    // Truth statements in if statement
    // Iterate counter for correct tests
    
    // Add Check
    tempcount = 0;
    
    MatrixAdd(idmat,mat1,result);   
    if (MatrixEquals(result,addid1)) {
        tempcount = tempcount + 1;
    }
    MatrixAdd(mat1,mat2,result);
    if (MatrixEquals(result,add12)){
        tempcount = tempcount + 1;
    }
    
    if (tempcount == 2) {
        printf("PASSED (2/2): MatrixAdd()\n");
        count = count + 2;
    } else if (tempcount == 0) {
        printf("FAILED (0/2): MatrixAdd()\n");
    } else {
        printf("FAILED (1/2): MatrixAdd()\n");
        count = count + 1;
    }
    // Tests by using add function, passing matrices we want to add
    // and uses equals check to check result against expected
    // matrix
    // Uses temp counter to check whether the two tests were successful
    
    // Multiply Check
    tempcount = 0;
    
    MatrixMultiply(idmat,mat1,result);   
    if (MatrixEquals(result,multid1)) {
        tempcount = tempcount + 1;
    }
    MatrixMultiply(mat2,mat3,result);
    if (MatrixEquals(result,mult23)){
        tempcount = tempcount + 1;
    }
    
    if (tempcount == 2) {
        printf("PASSED (2/2): MatrixMultiply()\n");
        count = count + 2;
    } else if (tempcount == 0) {
        printf("FAILED (0/2): MatrixMultiply()\n");
    } else {
        printf("FAILED (1/2): MatrixMultiply()\n");
        count = count + 1;
    }
    // Same as add, but using multiply function
    
    // Scalar Add Check
    tempcount = 0;
    
    MatrixScalarAdd(10,idmat,result);   
    if (MatrixEquals(result,saddid)) {
        tempcount = tempcount + 1;
    }
    MatrixScalarAdd(-5,mat3,result);
    if (MatrixEquals(result,sadd3)){
        tempcount = tempcount + 1;
    }
    
    if (tempcount == 2) {
        printf("PASSED (2/2): MatrixScalarAdd()\n");
        count = count + 2;
    } else if (tempcount == 0) {
        printf("FAILED (0/2): MatrixScalarAdd()\n");
    } else {
        printf("FAILED (1/2): MatrixScalarAdd()\n");
        count = count + 1;
    }
    // Uses scalar add function to test it, checks against expected 
    // matrix as previous
    
    // Scalar Multiply Check
    tempcount = 0;
    
    MatrixScalarMultiply(3,idmat,result);   
    if (MatrixEquals(result,smultid)) {
        tempcount = tempcount + 1;
    }
    MatrixScalarMultiply(-2,mat2,result);
    if (MatrixEquals(result,smult2)){
        tempcount = tempcount + 1;
    }
    
    if (tempcount == 2) {
        printf("PASSED (2/2): MatrixScalarMultiply()\n");
        count = count + 2;
    } else if (tempcount == 0) {
        printf("FAILED (0/2): MatrixScalarMultiply()\n");
    } else {
        printf("FAILED (1/2): MatrixScalarMultiply()\n");
        count = count + 1;
    }
    // Same as scalar add with multiply
    
    // Trace Check
    tempcount = 0;
    
    trace = MatrixTrace(idmat);   
    if ((3 - FP_DELTA) < trace < (3 + FP_DELTA)) {
        tempcount = tempcount + 1;
    }
    trace = MatrixTrace(mat1);   
    if ((6 - FP_DELTA) < trace < (6 + FP_DELTA)) {
        tempcount = tempcount + 1;
    }
    
    if (tempcount == 2) {
        printf("PASSED (2/2): MatrixTrace()\n");
        count = count + 2;
    } else if (tempcount == 0) {
        printf("FAILED (0/2): MatrixTrace()\n");
    } else {
        printf("FAILED (1/2): MatrixTrace()\n");
        count = count + 1;
    }
    // Checks trace by returning a trace value for a matrix,
    // and checking if output value in trace is equivalent to 
    // what we expect
    
    // Transpose Check
    tempcount = 0;
    
    MatrixTranspose(mat2,result);   
    if (MatrixEquals(result,trans2)) {
        tempcount = tempcount + 1;
    }
    MatrixTranspose(mat3,result);
    if (MatrixEquals(result,trans3)){
        tempcount = tempcount + 1;
    }
    
    if (tempcount == 2) {
        printf("PASSED (2/2): MatrixTranspose()\n");
        count = count + 2;
    } else if (tempcount == 0) {
        printf("FAILED (0/2): MatrixTranspose()\n");
    } else {
        printf("FAILED (1/2): MatrixTranspose()\n");
        count = count + 1;
    }
    // Uses transpose function in order to check value, takes result ands 
    // checks equality against expected output
    
    // SubMatrix Check    
    tempcount = 0;
    
    MatrixSubmatrix(0,0,idmat,smallresult);   
    if (MatrixSmallEquals(smallresult,sub00)) {
        tempcount = tempcount + 1;
    }
    MatrixSubmatrix(1,2,idmat,smallresult);
    if (MatrixSmallEquals(smallresult,sub12)){
        tempcount = tempcount + 1;
    }
    
    if (tempcount == 2) {
        printf("PASSED (2/2): MatrixSubmatrix()\n");
        count = count + 2;
    } else if (tempcount == 0) {
        printf("FAILED (0/2): MatrixSubmatrix()\n");
    } else {
        printf("FAILED (1/2): MatrixSubmatrix()\n");
        count = count + 1;
    }
    // Wrote a small equality check function that works for a 2x2 matrix
    // called 'MatrixSmallEquals', in order to check for proper output
    // from submatrix function
    
    // Determinant Check
    tempcount = 0;
    
    expdet = 1;
    det = MatrixDeterminant(idmat);   
    if ((fabs(det - expdet)) < FP_DELTA) {
        tempcount = tempcount + 1;
    }
    
    expdet = -138; 
    det = MatrixDeterminant(mat3);   
    if ((fabs(det - expdet)) < FP_DELTA) {
        tempcount = tempcount + 1;
    }
    
    if (tempcount == 2) {
        printf("PASSED (2/2): MatrixDeterminant()\n");
        count = count + 2;
    } else if (tempcount == 0) {
        printf("FAILED (0/2): MatrixDeterminant()\n");
    } else {
        printf("FAILED (1/2): MatrixDeterminant()\n");
        count = count + 1;
    }
    // Use determinant function in order to take determinant of a matrix
    // checks for equality within delta of the expected value of each
    // determinant
   
    
    // Inverse Check
    tempcount = 0;
    
    MatrixInverse(mat2,result);   
    if (MatrixEquals(result,inverse2)) {
        tempcount = tempcount + 1;
    }
    MatrixInverse(mat3,result);
    if (MatrixEquals(result,inverse3)){
        tempcount = tempcount + 1;
    }
    
    if (tempcount == 2) {
        printf("PASSED (2/2): MatrixInverse()\n");
        count = count + 2;
    } else if (tempcount == 0) {
        printf("FAILED (0/2): MatrixInverse()\n");
    } else {
        printf("FAILED (1/2): MatrixInverse()\n");
        count = count + 1;
    }
    // Invert matrices and test against the values calculated using 
    // an online inverse matrix calculator
    
    printf("------------------------------\n");
    printf("%d out of 20 tests passed\n\n", count);
    // Print how many of total tests were passed using count variable
    
    printf("Output of MatrixPrint() for a Matrix:\n");
    MatrixPrint(mat3);
    
    printf("Output of MatrixPrint() for Inverse of Prior Matrix:\n");
    MatrixInverse(mat3,result);
    MatrixPrint(result);
    // Print out two matrices in order to check proper format of 
    // the matrix print function. Output looks good, and works for
    // lots of significant figures
    
    // Not sure what length was necessary, but had to limit length in
    // order to ensure proper alignment under all conditions
    // Inverted matrix is a good example of the print capabilities
    BOARD_End();
    while (1);
}

