// Mathias Eriksen  
// mterikse@ucsc.edu
// File : MatrixMath.c

#include <stdio.h>
#include <math.h>

//User libraries:
#include "BOARD.h"

#define FP_DELTA 0.0001
#define DIM 3
#define DIM2 2

// Use i,j for row,column counters throughout
// In places with naming conflict, use i_,j_, or k

// Implement MatrixPrint 
// Used nested loop to iterate through and print
void MatrixPrint(float mat[DIM][DIM])
{
    int i,j; // Counters
    float printval;
    
    printf("\n----------------------------------\n");
    for(i=0; i<3; i++)
    {                           
      for(j=0; j<3; j++)
      {
        printval = mat[i][j]; // Take value
        printf("| %8.4g " , printval);  // Setup for clean output
        // Max 8 sig figs, but hope thats alright because more would look clunky
        }
      printf("|");
      printf("\n----------------------------------\n");
    }
    printf("\n");  
}



// IGNORE, MatrixPrint for 2x2 matrices used for debugging
void MatrixSmallPrint(float mat[DIM2][DIM2]) 
{
    int i,j; // Counters
    float smallprintval;
    printf("\n-------------------\n");
    for(i=0; i<2; i++)
    {                           
      for(j=0; j<2; j++)
      {
        smallprintval = mat[i][j];
        printf("| %6.4g " , smallprintval);
      }
      printf("|");
      printf("\n-------------------\n");
    }
    printf("\n"); 
}



int MatrixEquals(float mat1[DIM][DIM], float mat2[DIM][DIM])
{
   int i,j; // Counters
   float equalval1, equalval2, temp; // Equal variables
   temp = 0;
   
   for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
           equalval1 = mat1[i][j];
           equalval2 = mat2[i][j];
           if ((fabs(equalval1 - equalval2)) < FP_DELTA){ // Check proximity
            } else {
               temp++; // increment temp if elements are unequal
            }        
        }         
    }
    if (temp != 0) {
       return 0;    // Not Equal if temp has been incremented while iterating
       // through matrix
    } else {
       return 1;    // Equal if temp has not been incremented
    }
}



// Same as Equals, but for a 2x2 matrix
// Used in test harness for checking sub matrix function
int MatrixSmallEquals(float mat1[DIM2][2], float mat2[DIM2][DIM2])
{
   int i,j;
   float equalval1, equalval2, temp; // Equal variables
   temp = 0;
   
   for (i = 0; i < 2; i++){
        for (j = 0; j < 2; j++){
           equalval1 = mat1[i][j];
           equalval2 = mat2[i][j];
           if ((fabs(equalval1 - equalval2)) < FP_DELTA){ // Check proximity
            } else {
               temp++;
            }        
        }         
    }
    if (temp != 0) {
       return 0;    // Not Equal
    } else {
       return 1;    // Equal
    }
}



void MatrixAdd(float mat1[DIM][DIM], float mat2[DIM][DIM], float result[DIM][DIM])
{
    int i,j;
    float newaddval, addval1, addval2; // Add variables
    
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
           addval1 = mat1[i][j]; // take element from i,j in first matrix
           addval2 = mat2[i][j]; // take element from i,j in second matrix
           newaddval = addval1 + addval2; // add them together
           result[i][j] = newaddval; // put value in corresponding index
           // of the result matrix
        }        
    }
}



void MatrixMultiply(float mat1[DIM][DIM], float mat2[DIM][DIM], float result[DIM][DIM])
{
    int i,j;
    float newmultval, multval1, multval2; // Multiply variables
    
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
           multval1 = mat1[i][j]; // take element from i,j in first matrix
           multval2 = mat2[i][j]; // take element from i,j in seconf matrix
           newmultval = multval1 * multval2; 
           result[i][j] = newmultval; // put value in corresponding index 
           // of the result matrix
        }        
    }  
}



void MatrixScalarAdd(float x, float mat[DIM][DIM], float result[DIM][DIM])
{
    int i,j;
    float scalarval; // Scalar add variable
    scalarval = 0;
    
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            scalarval = (mat[i][j] + x); // scalar add input scalar to each
            // individual element in matrix
            result[i][j] = scalarval; // insert into result matrix
        }        
    } 
}


void MatrixScalarMultiply(float x, float mat[DIM][DIM], float result[DIM][DIM])
{
    int i,j;
    float scalarmultval; // Scalar multiply variable
    scalarmultval = 0;
    
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            scalarmultval = (mat[i][j] * x);// scalar multiply by input scalar
            // at each element in the matrix
            result[i][j] = scalarmultval; // insert into result matrix
        }        
    } 
}



float MatrixTrace(float mat[DIM][DIM])
{
    int i,j;
    float traceval; // trace variable  
    traceval = 0;
    
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            if (i == j) { // when row and column are equal,i.e diagonal values
                traceval =  traceval + mat[i][j]; // add element to trace value
            }
        }        
    } 
    return traceval;
}



void MatrixTranspose(float mat[DIM][DIM], float result[DIM][DIM])
{
    int i,j;
    
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            result[i][j] = mat[j][i]; // simply swap rows and columns from
            // original matrix
            }
        }        
} 

void MatrixSubmatrix(int i, int j, float mat[DIM][DIM], float result[DIM2][DIM2])
{  
    int i_,j_; // use i_ and j_ as counters, i and j are passed as the 
    // row/column to be deleted
    int subindexi, subindexj;
    subindexi = 0;
    subindexj = 0;
    
    for ( i_ = 0; i_ < 3; i_++){
        if (i_ == i){
            continue; // skip this row if it is the row that has been passed
            // so it will be deleted from matrix
        }
        for (j_ = 0; j_ < 3; j_++){  
            if (j_ == j){ // skip this column so it will be deleted from the \
                // the matrix
                continue;
            }
            result[subindexi][subindexj] = mat[i_][j_]; //insert element into
            // matrix if it is one of the allowed elemnts in the input matrix
            subindexj++; // increment sub matrix column counter
            if (subindexj == 2) {
                subindexj = 0;
                subindexi++;
                // when column 2 is reached, column counter will reset since the
                // 2x2 only has columns 0 and 1, and moves to the next row
            }           
            }
        }    
}



float MatrixDeterminant(float mat[DIM][DIM])
{
    float a1,a2,a3,b1,b2,b3,c1,c2,c3;
    a1 = mat[0][0];
    a2 = mat[0][1];
    a3 = mat[0][2];
    b1 = mat[1][0];
    b2 = mat[1][1];
    b3 = mat[1][2];
    c1 = mat[2][0];
    c2 = mat[2][1];
    c3 = mat[2][2];
    // assign variables for correct values from the input matrix
    
    return (a1*(b2*c3 - b3*c2)) - (a2*(b1*c3 - b3*c1)) + (a3*(b1*c2 - b2*c1));
    // Implement 3x3 determinant formula]
}

void MatrixInverse(float mat[DIM][DIM], float result[DIM][DIM])
{
    // Variables
    int i,j;
    
    float det, reciprocal_det;
    
    // Intermediate matrices
    float sub[2][2] = {{},{}};
    float transpose[3][3] = {{},{},{}};
    float detmat[3][3] = {{},{},{}};
    float cofactor[3][3] = {{1,-1,1},{-1,1,-1},{1,-1,1}};  // Cofactor matrix    
    float adjugate[3][3] = {{},{},{}};
    
    
    // Check for zero determinant,if so, there is no inverse
    // Guess there's no return value, so we just hope its not 0??
    
    // rake determinant of matrix first (step 1)
    det = MatrixDeterminant(mat);
    // reciprocal tobeused later
    reciprocal_det = (1/det);
    // take transpose (step 2)
    MatrixTranspose(mat, transpose);
    
    // Find determinant of each 2x2 sub matrix and place in corresponding i,j
    // (corresponds to excluded row/column) (step 3)
    // store values in determinant value matrix
    for (i = 0; i < 3 ; i++){
        for (j = 0; j < 3; j++){
            MatrixSubmatrix(i,j,transpose, sub);
            detmat[i][j] = ((sub[0][0])*(sub[1][1])) - ((sub[1][0])*(sub[0][1]));
        }
    }
    
    // multiply by cofactor matrix (step 4)
    MatrixMultiply(detmat, cofactor, adjugate);
    // multiply whole matrix by reciprocal of determinant value
    MatrixScalarMultiply(reciprocal_det, adjugate, result);       
    // Thats it
}