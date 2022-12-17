#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "lab2.h"

void *validation_rows(void* param);
void *validation_cols(void* param);
void *valdiation_grid(void* param);


int** read_board_from_file(char* filename){
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    int** sudoku_board = (int**)malloc(sizeof(int*) * ROW_SIZE);
    for(int rows = 0; rows < ROW_SIZE; rows++){
	sudoku_board[rows] = (int*)malloc(sizeof(int) * COL_SIZE);

    for(int k = 0; k < ROW_SIZE; k++){
        for(int j = 0; j < COL_SIZE; j++){
            fscanf(fp, "%d", &sudoku_board[k][j]);
        }
    }
    fclose(fp);
    return sudoku_board;
}
}


int is_board_valid(){
    pthread_t* tid=
    (pthread_t*)malloc(sizeof(int) * NUM_OF_THREADS);
    param_struct* params = (param_struct*)malloc(sizeof(param_struct) * NUM_OF_THREADS);
    pthread_attr_t attr;

    int t_index = 0;
    int k,j;
    for (k = 0; k <9; k++){
        for(j = 0; j < 9; j++){
            if(k % 3 == 0 && j % 3 == 0){
                params[t_index].id = t_index;
                params[t_index].starting_rows = k;
                params[t_index].starting_cols = j;
                params[t_index].ending_rows = k + 2;
                params[t_index].ending_cols = j + 2;
                
                pthread_create(&(tid[t_index++]), &attr, valdiation_grid, &(params[t_index++]));
            }
            if(k == 0){
                params[t_index].id = t_index; 
                params[t_index].starting_rows = k;
                params[t_index].starting_cols = j;
                params[t_index].ending_rows = k;
                params[t_index].ending_cols = j + 8;
                pthread_create(&(tid[t_index++]), &attr, validation_cols, &(params[t_index++]));
            }
            if(j == 0){
                params[t_index].id = t_index; 
                params[t_index].starting_rows = k;
                params[t_index].starting_cols = j;
                params[t_index].ending_rows = k + 8;
                params[t_index].ending_cols = j;
                pthread_create(&(tid[t_index++]), &attr, validation_rows, &(params[t_index++]));
            }
        }
    }
    for(k = 0; k < NUM_OF_THREADS; k++){
        pthread_join(tid[k], NULL);

    }
    for(k = 0; k < NUM_OF_THREADS; k++){
        if(worker_validation[k] == 0){
            printf("invalid!\n");
            return EXIT_SUCCESS;
        }
    }
    printf("solution is valid!\n");
    return EXIT_SUCCESS;
    
}

void *validation_rows(void* param){
    param_struct *params = (param_struct*) param;
    int id = params -> id;
    int rows = params -> starting_rows;
    int cols = params -> starting_cols;
    int rows_end = params -> ending_rows;
    int cols_end = params -> ending_cols;

    int valid_arr[9] = {0};
    int k;
    for(k = 0; k <9; k++){
        int nums = sudoku_board[rows][k];
        if(nums < 1 || 9 < nums || valid_arr[nums - 1] == 1){
            pthread_exit(NULL);
        }
        else{
            valid_arr[nums - 1] = 1;
        }
    }
    worker_validation[id] == 1;
    pthread_exit(NULL);
}
void *validation_cols(void* param){
    param_struct *params = (param_struct*) param;
    int id = params -> id;
    int rows = params -> starting_rows;
    int cols = params -> starting_cols;
    int rows_end = params -> ending_rows;
    int cols_end = params -> ending_cols;

    int valid_arr[9] = {0};
    int k;
    for(k = 0; k < 9; k++){
        int nums = sudoku_board[k][cols];
        if(nums < 1 || 9 < nums || valid_arr[nums - 1] == 1){
            pthread_exit(NULL);
        }
        else{
            valid_arr[nums - 1] = 1;
        }
    }
    worker_validation[id]==1;
    pthread_exit(NULL);
}
void *valdiation_grid(void* param){
    param_struct *params = (param_struct*) param;
    int id = params -> id;
    int rows = params -> starting_rows;
    int cols = params -> starting_cols;
    int rows_end = params -> ending_rows;
    int cols_end = params -> ending_cols;

    int valid_arr[9] = {0};
    int k,j;
    for(k = rows; k < rows + 3; k++){
        for(j = cols; j < cols + 3; j++){
            int nums = sudoku_board[k][j];
            if(nums < 1 || 9 < nums || valid_arr[nums - 1] == 1){
                pthread_exit(NULL);
            }

            else{
                valid_arr[nums - 1] = 1;
            }
        }
    }
    worker_validation[id] == 1;
    pthread_exit(NULL);
}
