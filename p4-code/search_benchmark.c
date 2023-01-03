// Complete this main to benchmark the search algorithms outlined in
// the project specification
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "search.h"
int main(int argc, char *argv[]){
  if(argc < 4){////not enough arguments, raise an error
    printf("usage: ./search_benchmark <minpow> <maxpow> <repeats> [alg1 alg2 ...]\n");
    printf("  minpow:  first data structure size is 2^minpow\n");
    printf("  maxpow:  last data structure size is 2^maxpow\n");
    printf("  repeats: number of times to loop search sequence to scale time\n");
    printf("with alg1 alg2 as a combination of:\n");
    printf("  la : Linear Array Search\n");
    printf("  ll : Linked List Search\n");
    printf("  ba : Binary Array Search\n");
    printf("  bt : Binary Tree Search\n");
    printf("  (default all)\n");
    return 1;
  }
  int linear_array = 1;//initialize all algs to 1, then when input only 4 argc, it will analysis all algs
  int linked_list = 1;
  int binary_array = 1;
  int binary_tree = 1;
  
  if(argc > 4){//have command line arguments
    linear_array = 0;
    linked_list = 0;
    binary_array = 0;
    binary_tree = 0;
    //char *str = argv[4];
    for(int i = 4; i < argc; i++){//read the command line and dicide which to run
      if(strcmp(argv[i], "la") == 0){
        linear_array = 1;
      }
      if(strcmp(argv[i], "ll") == 0){
        linked_list = 1;
      }
      if(strcmp(argv[i], "ba") == 0){
        binary_array = 1;
      }
      if(strcmp(argv[i], "bt") == 0){
        binary_tree = 1;
      }
      }
    }
  //printf("%d",linear_array);

  int minpow = atoi(argv[1]);//minpow is the second arguement and convert to int
  int maxpow = atoi(argv[2]);//maxpow is the third argument and convert to int
  int repeats = atoi(argv[3]);//repeats is the forth argument and convert to int
//print the title
  printf("%8s ","SIZE");
  printf("%8s ","NSEARCH");
//print which additional command line is called
  if(linear_array){
    printf("%10s ","array");
  }
  if(linked_list){
    printf("%10s ","list");
  }
  if(binary_array){
    printf("%10s ","binary");
  }
  if(binary_tree){
    printf("%10s ","tree");
  }
  printf("\n");

  int cur_search_size;//size
  clock_t start, end;//clock
  
  for(int j = minpow; j <= maxpow; j++){
    cur_search_size = 2<<(j-1);//power the current search size
    printf("%8d ", cur_search_size);
    printf("%8d ", cur_search_size*2*repeats);

    if(linear_array){
      int *l_array = make_evens_array(cur_search_size);//set up array
      start = clock();//start timer
      for(int i = 0; i < repeats; i++){
        for(int j = 0; j < cur_search_size*2; j++){
          linear_array_search(l_array, cur_search_size, j);//do searches
        }
      }
      end = clock();//stop timer
      double time = ((double)(end - start))/CLOCKS_PER_SEC;//calculate time
      printf("%10.4e ", time);//print output
      free(l_array);//free the array
    }
    if(linked_list){
      list_t *list = make_evens_list(cur_search_size);//set up list
      start = clock();//start timer
      for(int i = 0; i < repeats; i++){
        for(int j = 0; j < cur_search_size*2; j++){
          linkedlist_search(list, cur_search_size, j);//do searches
        }
      }
      end = clock();//stop timer
      double time = ((double)(end - start))/CLOCKS_PER_SEC;//calculate time
      printf("%10.4e ", time);//print output
      list_free(list);//free the list
    }
    if(binary_array){
      int *b_array = make_evens_array(cur_search_size);
      start = clock();
      for(int i = 0; i < repeats; i++){
        for(int j = 0; j < cur_search_size*2; j++){
          binary_array_search(b_array, cur_search_size, j);
        }
      }
      end = clock();
      double time = ((double)(end - start))/CLOCKS_PER_SEC;
      printf("%10.4e ", time);
      free(b_array);
    }
    if(binary_tree){
      bst_t *b_tree = make_evens_tree(cur_search_size);
      start = clock();
      for(int i = 0; i < repeats; i++){
        for(int j = 0; j < cur_search_size*2; j++){
          binary_tree_search(b_tree, cur_search_size, j);
        }
      }
      end = clock();
      double time = ((double)(end - start))/CLOCKS_PER_SEC;
      printf("%10.4e ", time);
      bst_free(b_tree);
    }
    cur_search_size *= 2;//increase size
    printf("\n");
  }
  return 0;
}

