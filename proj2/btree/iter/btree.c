/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * Autor: Boris Semanco - xseman06
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 * 
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  // initiliaze only if the tree wasn't initiliazed yet
  if ((*tree) == NULL) {
    return;
  }
  (*tree) = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t *current = tree;
  while(current != NULL){
    // if a node with the key was found return true and set the value to value of the node
    if (current->key == key){
      (*value) = current->value;
      return true;
    }
    // if searched key is greater then the key of current node got to right subtree else left
    else if (key > current->key){
      current = current->right;
    }
    else{
      current = current->left;
    }
  }
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  // prepare a new node 
  bst_node_t *new_node = malloc(sizeof(bst_node_t));
  if (new_node == NULL)
    return;

  new_node->key = key;
  new_node->value = value;
  new_node->left = NULL;
  new_node->right = NULL;

  // if tree is empty just insert it
  if (*tree == NULL){
    *tree = new_node;
    return;
  }

  bst_node_t *current = (*tree);
  bst_node_t *parent = NULL;
  //now just insert node on its place
  while(current != NULL){
    // if node with the key exists change the value
    if(current->key == key ){
        current->value = value;
        free(new_node);
        return;
      }
    // stores current to parent and change current
    parent = current;
    if (key > current->key)
      current = current->right;
    else
      current = current->left;
  }

  // insert the new node on its position
  if (key > parent->key)
    parent->right = new_node;
  else
    parent->left = new_node;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *parent = NULL;
  bst_node_t *current = (*tree);

  // Check if the tree has a right child before proceeding
    if (current->right == NULL) {
        // There is no right child
        // Update the target as needed or handle the scenario based on the context
        // For instance, if it's intended to replace the root with its left child (if exists), it can be handled here
        return;
    }

  while(current->right != NULL){
    // move to the rightmost
    parent = current;
    current = current->right;
  }
  // change the target's values and delete the rightmost node
  parent->right = NULL;
  target->key = current->key;
  target->value = current->value;
  free(current);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  int value;
  // if tree is empty or it doesn't contain a node with the key do nothing
  if(!bst_search((*tree), key, &value)){
    return;
  }
  bst_node_t *parent = NULL;
  bst_node_t *current = (*tree);
  
  // move to the position of the node to be deleted
  while(current != NULL){
    if (current->key == key)
      break;
    parent = current;
    if (key > current->key)
      current = current->right;
    else
      current = current->left;
  }

  // if its a leaf just erase it
  if (current->left == NULL && current->right == NULL){
    if (current->key > parent->key)
      parent->right = NULL;
    else
      parent->left = NULL;

    free(current);
  }

  // if current node has only left subtree
  else if (current->left != NULL && current->right == NULL){
    if (current->key > parent->key)
      parent->right = current->left;
    else
      parent->left = current->left;
    free(current);
  }

  // if current node has only right subtree
  else if (current->left == NULL && current->right != NULL){
    if (current->key > parent->key)
      parent->right = current->right;
    else
      parent->left = current->right;
    free(current);
  }

  // if current node has both subtrees replace the values with the rightmost node of the left subtree
  else {
    bst_replace_by_rightmost(current, &current->left);
  }



}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  bst_node_t *current = (*tree);

  // if a tree is already empty do nothing
  if (current == NULL)
    return;

  // allocating memory and initing stack
  stack_bst_t *stack = malloc(sizeof (stack_bst_t));
  if (stack == NULL)
    return;
  stack_bst_init(stack);
  // pushing on stack root of the tree
  stack_bst_push(stack, current);

  while(!stack_bst_empty(stack)){
    current = stack_bst_pop(stack);

    // if it has left subtree push it to the stack
    if (current->left != NULL)
      stack_bst_push(stack, current->left);
    // if it has right subtree push it to the stack
    if (current->right != NULL)
      stack_bst_push(stack, current->right);
    // remove current node
    free(current);
  }
  free(stack);
  (*tree) = NULL;
  
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  bst_node_t *node = tree;
  while(node != NULL){
    stack_bst_push(to_visit, node);
    bst_add_node_to_items(node, items);
    node = node->left;
  }

}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL)
    return;
    
  stack_bst_init(stack);
  bst_leftmost_preorder(tree, stack, items);
  while(!stack_bst_empty(stack)){
    tree = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_leftmost_preorder(tree->right, stack, items);
  }
  free(stack);
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *node = tree;
  while(node != NULL){
    stack_bst_push(to_visit, node);
    node = node->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL)
    return;

  bst_node_t *current = tree;
  stack_bst_init(stack);
  bst_leftmost_inorder(current, stack);
  while(!stack_bst_empty(stack)){
    current = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_add_node_to_items(current, items);
    bst_leftmost_inorder(current->right, stack);
  }
  free(stack);
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  bst_node_t *node = tree;
  while(node != NULL){
    stack_bst_push(to_visit, node);
    stack_bool_push(first_visit, true);
    node = node->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  bst_node_t *node = tree;
  bool from_left;
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bool_t *bool_stack = malloc(sizeof(stack_bool_t));
  if (stack == NULL || bool_stack == NULL){
    return;
  }
  stack_bst_init(stack);
  stack_bool_init(bool_stack);
  bst_leftmost_postorder(node, stack, bool_stack);

  while(!stack_bst_empty(stack)){
    node = stack_bst_top(stack);
    from_left = stack_bool_top(bool_stack);
    stack_bool_pop(bool_stack);

    if (from_left){
      stack_bool_push(bool_stack, false);
      bst_leftmost_postorder(node->right, stack, bool_stack);
    }
    else {
      stack_bst_pop(stack);
      bst_add_node_to_items(node, items);
    }
  }
  free(stack);
  free(bool_stack);
}
