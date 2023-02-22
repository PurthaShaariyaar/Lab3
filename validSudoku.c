/**
 * @file validSudoku.c
 * @brief Program identifies whether a 9 x 9 Sudoku puzzle board is valid or not.
 *        Designed by implementing threads to check columns, rows, and 3 x 3 grid areas of the board.
 * @version 1.0 
 * @date 2023-02-20
 * 
 * @copyright Copyright (c) 2023
 */


/**
 * @brief Import all necessary libraries. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/**
 * @brief Define a preprocessor to replace a constant macro. 
 *        Initializing number of threads to be 27: 
 *          [9 threads per row, 9 threads per column, 9 threads per 3 x 3 blocks]
 * 
 */

#define numThreads 27

/**
 * @brief Initiate an array valid of 27 elements set to 0. Upon checking row, column and 3 x 3 block 
 *        worker thread can update to 1, implying validity. 
 * 
 */

int valid[numThreads] = {0};


/**
 * @brief Parent threads creating worker threads and passing location. Simplest method to pass parameters
 *        to threads is via creating a data structure using a struct. 
 * 
 */

typedef struct {
  int row;
  int column; 
} parameters; 

/**
 * @brief Test random Sudoku puzzle to be solved. 
 * 
 */

int sudoku[9][9] = {
	{6, 2, 4, 5, 3, 9, 1, 8, 7},
	{5, 1, 9, 7, 2, 8, 6, 3, 4},
	{8, 3, 7, 6, 1, 4, 2, 9, 5},
	{1, 4, 3, 8, 6, 5, 7, 2, 9},
	{9, 5, 8, 2, 4, 7, 3, 6, 1},
	{7, 6, 2, 3, 9, 1, 4, 5, 8},
	{3, 7, 1, 9, 5, 6, 8, 4, 2},
	{4, 9, 6, 1, 8, 2, 5, 7, 3},
	{2, 8, 5, 4, 7, 3, 9, 1, 6}
};

/**
 * @brief Function verifies whether rows of the sudoku board are valid. 
 * 
 * @param param pointer to the struct data type to store row and columns. 
 * @return void* 
 */

void *validRow(void* param) {
  
  // Initialize and set pointers to struct data strcuture 
  parameters *params = (parameters*) param; 
  int row = params->row; 
  int col = params->column;

  // If row > 8 or col != 0 --> invalid row section --> exit out of thread 
  if (row > 0 || col != 0) {
		fprintf(stderr, "Invalid row or column for row subsection! row=%d, col=%d\n", row, col);
    pthread_exit(NULL); 
  }

  // Initialize valid row array of size 9 and set all elements to 0, update if each position valid to 1 
  int validRowArray[9] = {0};

  // Iterate through the whole row --> validate row array and update to 1 
  // Set thread to 1, if encountered again --> no updates --> exit out of thread
  for (int i = 0; i < 9; i++) {
    int num = sudoku[row][i]; 
    if (num < 1 || num > 9 || validRowArray[num - 1] == 1) {
      pthread_exit(NULL);
    } else {
      validRowArray[num - 1] = 1; 
    }
  }

  // Iteration traversed and verified that row is valid --> update valid for rows to be 1 
  valid[9 + row] = 1; 
  pthread_exit(NULL);
}

/**
 * @brief Function verifies whether columns of the sudoku board are valid. 
 * 
 * @param param pointer to the struct data type to store row and columns.
 * @return void* 
 */

void *validColumn(void* param) {

  // Initialize and set pointers to struct data strcuture 
	parameters *params = (parameters*) param;
	int row = params->row;
	int col = params->column;		

  // If col > 8 or row != 0 --> invalid row section --> exit out of thread 
	if (col > 0 || row != 0) {
		fprintf(stderr, "Invalid row or column for col subsection! row=%d, col=%d\n", row, col);
    pthread_exit(NULL); 
	}

  // Initialize valid column array of size 9 and set all elements to 0, update if each position valid to 1 
  // Exit out of thread
	int validColArray[9] = {0};

  // Iterate through the whole column --> validate column array and update to 1 
  // Set thread to 1, if encountered again --> no updates --> exit out of thread
	for (int i = 0; i < 9; i++) {
		int num = sudoku[i][col];
		if (num < 1 || num > 9 || validColArray[num - 1] == 1) {
			pthread_exit(NULL);
		} else {
			validColArray[num - 1] = 1;		
		}
	}

  // Iteration traversed and verified that column is valid --> update valid for columns to be 1 
  // Exit out of thread
	valid[18 + col] = 1;
	pthread_exit(NULL);
}

/**
 * @brief 
 * 
 * @param param 
 * @return void* 
 */

void *threeBlock(void* param) {

  // Initialize and set pointers to struct data strcuture 
  parameters *params = (parameters*) param; 
  int row = params->row;
  int col = params->column; 

  // 3 x 3 Block verification to check it's a correct block 
  if (row > 6 || col > 6 || row % 3 != 0 || col % 3 != 0) {
		fprintf(stderr, "Invalid row or column for subsection! row=%d, col=%d\n", row, col);
    pthread_exit(NULL);
  }

  // Initialize valid array of size 9 and set all elements to 0, update if each position valid to 1 
  // Exit out of thread
  int validThreeArray[9] = {0};

  // Iterate through the 3 x 3 blocks of the 9 x 9 board 
  for (int i = 0; i < row + 3; i++) {
    for (int j = col; j < col + 3; j++) {
      int num = sudoku[i][j]; 
      if (num < 1 || num > 9 || validThreeArray[num - 1] == 1) {
        pthread_exit(NULL);
      } else {
        validThreeArray[num - 1] = 1; 
      }
    }
  }

  // Iteration traversed verified all 3 x 3 blocks are valid --> update valid by 1 
  // Exit out of thread 
  valid[row + col / 3] = 1; 
  pthread_exit(NULL);
}

/**
 * @brief Main driver function iterates through whole Sudoku board
 *        creates a worker thread to iterate the row, column and  
 *        3 x 3 sections --> validating each section. 
 * 
 * @return int 
 */

int main() {
  
  // Create worker thread identifiers for the number of threads 
  pthread_t threads[numThreads];

  // Initiate a threadIndex 
  int threadIndex = 0; 

  // Iterate through all rows and columns --> create a worker thread for each 
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
    // Section off 3 x 3 blocks using modulus %3 to create a worker thread for the block sections
      if (i % 3 == 0 && j % 3 == 0) {
        parameters *data = (parameters*) malloc(sizeof(parameters));
        data->row = i; 
        data->column = j; 
        pthread_create(&threads[threadIndex++], NULL, threeBlock, data); 
      }
      if (i == 0) {
        parameters *colData = (parameters*) malloc(sizeof(parameters));
        colData->row = i; 
        colData->column = j; 
        pthread_create(&threads[threadIndex++], NULL, validColumn, colData);
      }
      if (j == 0) {
        parameters *rowData = (parameters*) malloc(sizeof(parameters));
        rowData->row = i; 
        rowData->column = j; 
        pthread_create(&threads[threadIndex++], NULL, validRow, rowData);
      }
    }
  }

  // Once all threads have completed --> join all thraeds
  for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL); 
  }

  // Iterate through the valid array check if any entries are 0 --> invalid Sudoku
  for (int i = 0; i < numThreads; i++) {
    if (valid[i] == 0) {
      printf("Sudoku is not valid.");
      return EXIT_SUCCESS;
    }
  }

  // Iteration of valid array has been completed --> valid Suduko if all entires 1 
  printf("Sudoku is valid.");
  return EXIT_SUCCESS;
}