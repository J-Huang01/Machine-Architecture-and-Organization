// main function for a treemap manager, student version

#include <stdio.h>
#include <string.h>
#include "treemap.h"

int main(int argc, char *argv[]){
  int echo = 0;                                // controls echoing, 0: echo off, 1: echo on
  if(argc > 1 && strcmp("-echo",argv[1])==0) { // turn echoing on via -echo command line option
    echo=1;
  }

  printf("TreeMap Editor\n");
  printf("Commands:\n");
  printf("  quit:            exit the program\n");
  printf("  print:           shows contents of the tree in reverse sorted order\n");
  printf("  add <key> <val>: inserts the given key/val into the tree, duplicate keys are ignored\n");
  printf("  get <key>:       prints FOUND if the name is in the tree, NOT FOUND otherwise\n");
  printf("  clear:           eliminates all key/vals from the tree\n");
  printf("  size:            prints the total number of nodes in the tree\n");
  printf("  preorder:        prints contents of the tree in pre-order which is how it will be saved\n");
  printf("  save <file>:     writes the contents of the tree in pre-order to the given file\n");
  printf("  load <file>:     clears the current tree and loads the one in the given file\n");
  
  char cmd[128];
  treemap_t treemap;
  int success;
  treemap_init(&treemap);
  int size;

  while(1){
    printf("TM> ");                 // print prompt
    success = fscanf(stdin,"%s",cmd); // read a command
    if(success==EOF){                 // check for end of input
      printf("\n");                   // found end of input
      break;                          // break from loop
    }

    if( strcmp("quit", cmd)==0 ){     // check for quit command
      if(echo){
        printf("quit\n");
      }
      break;                          // break from loop
    }

    else if( strcmp("print", cmd)==0 ){   // print command
      if(echo){
        printf("print\n");
      }
      treemap_print_revorder(&treemap);
    }

    else if( strcmp("add", cmd)==0 ){ // add function 
      char key[64];
      char val[64];
      fscanf(stdin,"%s %s",key, val);            // read string to insert
      if(echo){
        printf("add %s %s\n",key, val);
      }

      success = treemap_add(&treemap, key, val); // call add function
      if(!success){                      // check for success
        printf("modified existing key\n");
      }
    }

    else if( strcmp("get", cmd)==0 ){     // get command
      fscanf(stdin,"%s",cmd);          // read a key
      if(echo){
        printf("get %s\n",cmd);
      }

      char *ith; // get key
      ith = treemap_get(&treemap, cmd);
      if(ith == NULL){                    // check for success
        printf("NOT FOUND\n");
      }
      else {
        printf("FOUND: %s\n",ith);
      }
    }

    else if( strcmp("clear", cmd)==0 ){   // clear command
      if(echo){
        printf("clear\n");
      }
      treemap_clear(&treemap);
    }

    else if(strcmp("size", cmd) == 0){//contains command
      if(echo){
        printf("size\n");
      }

      size = treemap_size(&treemap);   
      printf("%d nodes\n", size);
      
    }

    else if (strcmp("preorder", cmd) == 0){//preorder command
      if(echo){
        printf("preorder\n");
      }
      treemap_print_preorder(&treemap);
    }

    else if (strcmp("save", cmd) == 0){//save command
      fscanf(stdin, "%s", cmd);
      if(echo){
        printf("save %s\n", cmd);
      }
      treemap_save(&treemap,cmd);
    }

    else if (strcmp("load", cmd) == 0){
      fscanf(stdin, "%s", cmd);
      if(echo){
        printf("load %s\n", cmd);
      }
      if(!treemap_load(&treemap, cmd)){
        printf("load failed\n");
      }
    }
    
    else{                                 // unknown command
      if(echo){
        printf("%s\n",cmd);
      }
      printf("unknown command %s\n",cmd);
    }
   }  

  // end main while loop
  treemap_clear(&treemap);                      // clean up the treemap
  return 0;
}
 