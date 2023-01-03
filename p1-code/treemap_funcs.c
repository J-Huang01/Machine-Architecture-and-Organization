#include "treemap.h"
#include <stdlib.h>//new
#include <string.h>//new
#include <stdio.h>

// treemap_funcs.c: Provides a small library of functions that operate on
// binary search trees mapping strings keys to string values.

void treemap_init(treemap_t *tree)
// Initialize the given tree to have a null root making it size 0.
{
  tree->root = NULL;
  return;
}

int treemap_add(treemap_t *tree, char key[], char val[])
// Inserts given key/value into a binary search tree. Uses an
// ITERATIVE (loopy) approach to insertion which starts a pointer at
// the root of the tree and changes its location until the correct
// insertion point is located. If the key given already exists in the
// tree, no new node is created; the existing value is changed to the
// parameter 'val' and 0 is returned.  If no node with the given key
// is found, a new node is created and with the given key/val, added
// to the tree, and 1 is returned. Makes use of strcpy() to ease
// copying characters between memory locations.
{
node_t *current = tree->root;//a pointer to the current node

if (tree->root == NULL){//if the tree is empty
  node_t *newnode = malloc(sizeof(node_t));//a pointer to the node that attempt to add
  strcpy(newnode->key, key);
  strcpy(newnode->val, val);//copy key and val to new node
  newnode->left = NULL;
  newnode->right = NULL;
  tree->root = newnode;// newnode is now the root
  return 1;
}
while(1){
  int compare_val = strcmp(key, current->key);//compare the new key with the current key
  if(compare_val == 0){//key exist
    strcpy(current->val, val);//change the old val to new val
    return 0;
  }
  else if(compare_val < 0){//add to left branch  
    if(current->left == NULL){
      node_t *newnode = malloc(sizeof(node_t));//a pointer to the node that attempt to add
      strcpy(newnode->key, key);
      strcpy(newnode->val, val);
      current->left = newnode;
      newnode->left = NULL;
      newnode->right = NULL;
      return 1;
    }
    else{
      current = current->left;//move the current node to its left
    }
  }
  
  else if(compare_val > 0){//add to right branch
    if (current->right == NULL){
      node_t *newnode = malloc(sizeof(node_t));//a pointer to the node that attempt to add
      strcpy(newnode->key, key);
      strcpy(newnode->val, val);
      current->right = newnode;
      newnode->left = NULL;
      newnode->right = NULL;
      return 1;
    }
    else{
      current = current->right;//move the current node to its right
    }
  }

}
}

char *treemap_get(treemap_t *tree, char key[])
// Searches the tree for given 'key' and returns its associated
// value. Uses an ITERATIVE (loopy) search approach which starts a
// pointer at the root of the tree and changes it until the search key
// is found or determined not to be in the tree. If a matching key is
// found, returns a pointer to its value. If no matching key is found,
// returns NULL.
{
  node_t *current = tree->root;
  while (current != NULL){
    int compare_val = strcmp(key, current->key);//compare given key to current key
    if (compare_val == 0){
      return current->val;
    }
    else if (compare_val < 0){
      current = current->left;
    }
    else if (compare_val > 0){
      current = current->right;
    }
  }
  return NULL;
}

void treemap_clear(treemap_t *tree)
// Eliminate all nodes in the tree setting its contents empty. Uses
// recursive node_remove_all() function to free memory for all nodes.
{
  node_remove_all(tree->root);
  treemap_init(tree);//initialize tree
  
}
void node_remove_all(node_t *cur)
// Recursive helper function which visits all nodes rooted at node
// `cur` and frees the memory associated with them. This requires a
// post-order traversal: visit left sub-tree, visit right sub-tree,
// then free the `cur` node.
{
  // recursion to both left and right branch
  if(cur != NULL){
    node_remove_all(cur->left);//left recursion
    node_remove_all(cur->right);//right recursion
    free(cur);//free cur node
  }
  
}

int treemap_size(treemap_t *tree)
// Returns the number of nodes currently in the tree. Uses
// node_count_all() to recursively count all nodes.
{
  node_t *cur = tree->root;
  if(cur == NULL){
    return 0;
  }
  return node_count_all(cur);
}


int node_count_all(node_t *cur)
// Counts all nodes in the tree rooted at `cur`. Uses recursion to
// descend to the left and right branch if present and count nodes
// there, adding 1 for the `cur` if non-null. Returns 0 if `cur` is
// NULL.
{
  if(cur == NULL){
      return 0;
  }
  int count = 1;
  if((cur->left != NULL)&&(cur->right!= NULL)){//recursion to both left and right
      count += node_count_all(cur->left) + node_count_all(cur->right);
  }
  else if(cur->left != NULL){//recursion to left branch
      count += node_count_all(cur->left);
  }
  else if(cur->right != NULL){//recursion to right branch
      count += node_count_all(cur->right);
  }
  return count;


}
void treemap_print_revorder(treemap_t *tree)
// Prints the key/val pairs of the tree in reverse order at differing
// levels of indentation which shows all elements and their structure
// in the tree. Visually the tree can be rotated clockwise to see its
// structure. See the related node_print_revorder() for additional
// detals.
{
  node_print_revorder(tree->root, 0);
}
void node_print_revorder(node_t *cur, int indent)
// Recursive helper function which prints all key/val pairs in the
// tree rooted at node 'cur' in reverse order. Traverses right
// subtree, prints cur node's key/val, then traverses left tree.
// Parameter 'indent' indicates how far to indent (2 spaces per indent
// level).
//
// For example: a if the root node "El" is passed into the function
// and it has the following structure:
// 
//         ___El->strange_____     
//        |                   |   
// Dustin->corny       ___Mike->stoic
//                    |              
//               Lucas->brash     
// 
// the recursive calls will print the following output:
// 
//   Mike -> stoic                 # root->right
//     Lucas -> brash              # root->right->left
// El -> strange                   # root
//   Dustin -> corny               # root->left
{
  if (cur == NULL){
    return;
  }
  //first traverses right subtree
  node_print_revorder(cur->right, indent + 2);
  for (int i = 0; i < indent; i++){//indent as space
    printf(" ");
  }
  printf("%s -> %s\n", cur->key, cur->val);//print out both key and value.
  //the traverses left subtree
  node_print_revorder(cur->left, indent + 2);
  return;
  
}

void treemap_print_preorder(treemap_t *tree)
// Print all the data in the tree in pre-order with indentation
// corresponding to the depth of the tree. Makes use of
// node_write_preorder() for this.
{
  node_write_preorder(tree->root, stdout, 0);
}

void treemap_save(treemap_t *tree, char *fname)
// Saves the tree by opening the named file, writing the tree to it in
// pre-order with node_write_preorder(), then closing the file.
{
  FILE *file = fopen(fname, "w");//open the file
  node_write_preorder(tree->root, file, 0);
  fclose(file);
}
void node_write_preorder(node_t *cur, FILE *out, int depth)
// Recursive helper function which writes/prints the tree in pre-order
// to the given open file handle. The parameter depth gives how far to
// indent node data, 2 spaces per unit depth. Depth increases by 1 on
// each recursive call. The function prints the cur node data,
// traverses the left tree, then traverses the right tree.
{
  if(cur == NULL){
    return;
  }
  for(int i = 0; i < depth; i++){//indent as space
    fprintf(out, " ");
  }
  //fprintf(out,"%*s %s\n",2*(depth-1)+(int)strlen(cur->key),cur->key,cur->val);
  // when use it the depth should be 1
  fprintf(out, "%s %s\n", cur->key, cur->val);//print the key and value
  node_write_preorder(cur->left, out, depth+1);//left recursion
  node_write_preorder(cur->right, out, depth+1);//right recursion
  return;
}
int treemap_load(treemap_t *tree, char *fname )
// Clears the given tree then loads new elements to it from the
// named. Repeated calls to treemap_add() are used to add strings read
// from the file.  If the tree is stored in pre-order in the file, its
// exact structure will be restored.  Returns 1 if the tree is loaded
// successfully and 0 if opening the named file fails in which case no
// changes are made to the tree.
{
  FILE *file = fopen(fname, "r");
  if(file == NULL){
    printf("ERROR: could not open file '%s'\n", fname);
    return 0;//open the named file fails
  }
  treemap_clear(tree);//clear the tree
  char key[64];
  char val[64];
  while(fscanf(file, "%s %s", key, val) != EOF){
    treemap_add(tree, key, val);
  }
  fclose(file);
  return 1;
}