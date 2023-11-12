/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * Autor: Boris Semanco - xseman06
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t *node = tree;
  // if the tree is empty, there's no need to search
  if (node == NULL) {
    return false;
  }
  
  // if a node with the key was found return true and set the value to value of the node
  if (node->key == key) {
    (*value) = node->value;
    return true;
  }
  // if searched key is greater then the key of current node got to right subtree else left
  else if (key > node->key) 
    return bst_search(node->right, key, value);
  else 
    return bst_search(node->left, key, value);
  
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *current = (*tree);
  // bst_node_t *parent = NULL;
  
  // if the node is null insert the new node on this place
  if (current == NULL){
    bst_node_t *new_node = malloc(sizeof(bst_node_t));
    new_node->key = key;
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;
    *tree = new_node;
    return;
  }

  // if node with the key exists change the value
  if(current->key == key ){
      current->value = value;
      return;
    }

  // if key is greater then current key go to right else left
  if (key > current->key){
    bst_insert(&current->right, key, value);
    return;
  }
  else {
    bst_insert(&current->left, key, value);
    return;
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *node = (*tree);

  // if it is at the rightest position
  if (node->right == NULL) {
    // sets target values to the rightest noeds values
    target->key = node->key;
    target->value = node->value;
    // store the node to temp variable for deletion
    bst_node_t *temp = node;
    // set the current node's left child as the new subtree eliminating the rightmost node
    *tree = node->left; 
    free(temp);
  } 
  // if it isn't at the rightest position go to right subtree
  else {
    bst_replace_by_rightmost(target, &(node->right));
  }
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  int value;

  // no node with the key, nothing to delete
  if (!bst_search(*tree, key, &value)) {
    return; 
  }

  // moving in the tree to the right position
  if (key < (*tree)->key) {
    bst_delete(&(*tree)->left, key);
  } 
  else if (key > (*tree)->key) {
    bst_delete(&(*tree)->right, key);
  } 
  else {
    // found the node to delete
    if ((*tree)->left == NULL) {
      bst_node_t *temp = *tree;
      // deletion by replacing with right child
      *tree = (*tree)->right;
      free(temp);
    } 
    else if ((*tree)->right == NULL) {
      bst_node_t *temp = *tree;
      // deletion by replacing with left child
      *tree = (*tree)->left;
      free(temp);
    } 
    else {
      // replace the data of the current node with the data of the rightmost node in the left subtree
      bst_replace_by_rightmost(*tree, &(*tree)->left);
    }
  }
}


/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  if ((*tree) == NULL){
    return;
  }

  // go through right and left subtrees and delete them all
  bst_dispose(&(*tree)->left);
  bst_dispose(&(*tree)->right);

  free((*tree));
  (*tree) = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if(tree != NULL){
    bst_add_node_to_items(tree, items);
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
  }
  
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL){
    bst_inorder(tree->left, items);
    bst_add_node_to_items(tree, items);
    bst_inorder(tree->right, items);
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL){
    bst_postorder(tree->left, items);
    bst_postorder(tree->right, items);
    bst_add_node_to_items(tree, items);
  }
}
