/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/

char my_tolower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}


void letter_count(bst_node_t **tree, char *input) {
    
    bst_init(tree);
    int value;
    // go through input string
    for (int i = 0; input[i] != '\0' ; i++){
        char key = my_tolower(input[i]);
        // if a char in the input string isn't in range of a-z or ' ' make it '_'
        if (key != ' '  && (key < 'a' || key > 'z')){
            key = '_';
        }
        // searches the key in the tree, if there is a node with the key increment it's value, else insert a new node to the tree
        if (bst_search((*tree), key, &value)){
            bst_node_t *current = (*tree);
            while(current != NULL){
                // if node with a key is found increment its value
                if(current->key == key ){
                    (current->value)++;
                    break;
                }

                if (key > current->key)
                current = current->right;
                else
                current = current->left;
            }
        }
        else{
            bst_insert(&(*tree), key, 1);
        }
    }

}

// returns a count of nodes in a tree
int get_node_count(bst_node_t *root) {
    if (root == NULL) {
        return 0;
    } else {
        return 1 + get_node_count(root->left) + get_node_count(root->right);
    }
}



bst_node_t* create_balanced_tree(bst_node_t **nodes, int start, int end) {
    if (start > end) {
        return NULL;
    }

    // gets a middle index
    int mid = (start + end) / 2;

    // makes a new root from the middle if the arraz of nodes
    bst_node_t *root = nodes[mid];

    // recursively make left and right subtrees
    root->left = create_balanced_tree(nodes, start, mid - 1);
    root->right = create_balanced_tree(nodes, mid + 1, end);

    return root;
}



bst_items_t* allocate_node_array(int count) {
    bst_items_t *node_array = (bst_items_t*)malloc(sizeof(bst_items_t));
    if (node_array == NULL) {
        return NULL;
    }

    // set the cap on the count and size to 0
    node_array->capacity = count;
    node_array->size = 0;

    // allocate memory for the array of nodes
    node_array->nodes = (bst_node_t**)malloc(count * sizeof(bst_node_t*));
    if (node_array->nodes == NULL) {
        // if there is an error allocating array of nodes free previously allocated memory
        free(node_array);
        return NULL;
    }

    return node_array;
}

/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {

    // gets nodes count and init an array for nodes
    int count = get_node_count(*tree);
    bst_items_t *nodes = allocate_node_array(count);

    // gets the array of nodes in inorder
    bst_inorder((*tree), nodes);


    // rewrite the tree in balanced version
    *tree = create_balanced_tree(nodes->nodes, 0, nodes->size - 1);

    // free the array of nodes and the array itself
    free(nodes->nodes);
    free(nodes);
}
