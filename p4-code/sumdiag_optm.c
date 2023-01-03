// optimized versions of matrix diagonal summing
#include "matvec.h"

// You can write several different versions of your optimized function
// in this file and call one of them in the last function.

int sumdiag_VER1(matrix_t *mat, vector_t *vec) {//calculate each diag by row
  // OPTIMIZED CODE HERE
  matrix_t mat1 = *mat;
  vector_t vec1 = *vec;
  if(vec1.len != (mat1.rows + mat1.cols -1)){
    printf("sumdiag_base: bad sizes\n");
    return 1;
  }
  for(int i=0; i<vec1.len; i++){                     // initialize vector of diagonal sums
    VSET(vec1,i,0);                                   // to all 0s
  }
  for(int i = 0; i < mat1.rows; i++){//find only one row in matrix for each step
    int row_num = mat1.rows - i - 1;
    for(int j = 0; j < mat1.cols; j++, row_num++){
      // int ij_val = MGET(mat1, i, j);//get the matrix element based on i and j 
      // int vec_d = VGET(vec1, row_num);
      VSET(vec1, row_num, MGET(mat1, i, j) + VGET(vec1, row_num));
    }
  }
  return 0;                                          // return success
}

int sumdiag_VER2(matrix_t *mat, vector_t *vec) {//divide the matrix to several 2*2 small matrix and calucate one by one
  // OPTIMIZED CODE HERE
  matrix_t mat1 = *mat;
  vector_t vec1 = *vec;
  if(vec1.len != (mat1.rows + mat1.cols -1)){
    printf("sumdiag_base: bad sizes\n");
    return 1;
  }
  for(int i=0; i<vec1.len; i++){                     // initialize vector of diagonal sums
    VSET(vec1,i,0);                                   // to all 0s
  }
  int i;
  for (i = 0; i < mat1.rows - 2; i += 2) {//jump 2 each time to make several 2*2 matrices
    int diag_num = mat1.rows - i - 1; //number of the diagonal
    int j;
    for (j = 0; j < mat1.cols - 2; j += 2, diag_num += 2){
      VSET(vec1, diag_num, MGET(mat1, i, j) + VGET(vec1, diag_num));//calucate sum of position (1,1)
      VSET(vec1, diag_num+1, MGET(mat1, i, j+1) + VGET(vec1, diag_num+1));//calcute sum of position (1,2)
      VSET(vec1, diag_num-1, MGET(mat1, i+1, j) + VGET(vec1, diag_num-1));//calcute sum of position (2,1)
      VSET(vec1, diag_num, MGET(mat1, i+1, j+1) + VGET(vec1, diag_num));//calcute sum of position (2,2)
      //the last one (2,2) is on the same diag of the first one (1,1)
    }
    for (; j < mat1.cols; j++, diag_num++) {//if the length of matrix is not even, then remain one column
      VSET(vec1, diag_num, MGET(mat1, i, j) + VGET(vec1, diag_num));
      VSET(vec1, diag_num-1, MGET(mat1, i+1, j) + VGET(vec1, diag_num - 1));
    }
  }

  for (; i < mat1.rows; i++) {//if the length of matrix is not even, then remain one row
    int diag_num = mat1.rows - i - 1;
    int j;
    for (j = 0; j < mat1.cols-2; j+=2, diag_num+=2){
      VSET(vec1, diag_num, MGET(mat1, i, j) + VGET(vec1, diag_num));
      VSET(vec1, diag_num+1, MGET(mat1, i, j+1) + VGET(vec1, diag_num+1));
    }
    for (; j < mat1.cols; j++, diag_num++) {
      VSET(vec1, diag_num, MGET(mat1, i, j) + VGET(vec1, diag_num));
    }
  }
  return 0;
}

int sumdiag_VER3(matrix_t *mat, vector_t *vec) {//for each loop, calculate 5 elemnets on one row
  // OPTIMIZED CODE HERE
  matrix_t mat1 = *mat;
  vector_t vec1 = *vec;
  if(vec1.len != (mat1.rows + mat1.cols -1)){
    printf("sumdiag_base: bad sizes\n");
    return 1;
  }
  for(int i=0; i<vec1.len; i++){                     // initialize vector of diagonal sums
    VSET(vec1,i,0);                                   // to all 0s
  }
  int i;
  for(i = 0; i < mat1.rows; i++){    //loop through all rows
    int j;  //column variable
    for(j = 0; j < mat1.cols-5; j+=5){        //loop through all columns
      int diag_num = j - i + mat1.cols - 1;   //calculate the number of diagonal               
      VSET(vec1, diag_num, MGET(mat1, i, j) + VGET(vec1, diag_num));  //calucate sum of position (1,1)
      VSET(vec1, diag_num+1, MGET(mat1, i, j+1) + VGET(vec1, diag_num+1));//calucate sum of position (1,2)
      VSET(vec1, diag_num+2, MGET(mat1, i, j+2) + VGET(vec1, diag_num+2));//calucate sum of position (1,3)
      VSET(vec1, diag_num+3, MGET(mat1, i, j+3) + VGET(vec1, diag_num+3));//calucate sum of position (1,4)
      VSET(vec1, diag_num+4, MGET(mat1, i, j+4) + VGET(vec1, diag_num+4));//calucate sum of position (1,5)
    }
    for(; j < mat1.cols; j++){       //if the length can not divide by 5, set the reminder
      int diag_num = j - i + mat1.cols - 1;                    
      VSET(vec1, diag_num, MGET(mat1, i, j) + VGET(vec1, diag_num)); 
    }
  }
  return 0;
  }

int sumdiag_OPTM(matrix_t *mat, vector_t *vec) {
  // call your preferred version of the function
  return sumdiag_VER3(mat, vec);
}
