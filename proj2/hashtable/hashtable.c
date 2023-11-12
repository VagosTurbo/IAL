/*
 * Tabulka s rozptýlenými položkami
 *
 * Autor: Boris Semanco - xseman06
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  for (int i = 0; i < HT_SIZE; i++) {
    (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  
  // go through all lines
  for (int i = 0; i < HT_SIZE; i++){
    ht_item_t *item = (*table)[i];
    // go through all items in a line
    while (item != NULL) {
      if (strcmp(item->key, key) == 0) {
        return item;
      }
      item = item->next;
    }
  }
  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  if (HT_SIZE == 0) {
    return;
  }
  ht_item_t *item = ht_search(table, key);
  // if the item with the key is found, change its value
  if (item != NULL) {
    item->value = value;
    return;
  }
  
  // gets index of the key and allocates memory for a new item
  int index = get_hash(key);
  ht_item_t *new_item = (ht_item_t *)malloc(sizeof(ht_item_t));
  
  if (new_item != NULL) {
    new_item->key = key;
    new_item->value = value;
    new_item->next = (*table)[index];

    if ((*table)[index] == NULL) {
      // if the line was empty, insert the new item as the first item
      (*table)[index] = new_item;
    }
    else {
      // if the line was not empty, insert the new item as the first item
      new_item->next = (*table)[index];
      (*table)[index] = new_item;
    }
  }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if(HT_SIZE == 0){
    return NULL;
  }
  // searches the key in the table and returns its value, if the key is not found returns NULL
  ht_item_t *item = ht_search(table, key);
  if ( item != NULL){
    return &(item->value);
  }
  return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  if (HT_SIZE == 0) {
    return;
  }
  // gets index of the key and the first item in the line
  int index = get_hash(key);
  ht_item_t *item = (*table)[index];
  ht_item_t *prev_item = NULL;
  // go through all items in a line
  while (item != NULL) {
    // if the key is found, delete the item
    if (strcmp(item->key, key) == 0) {
      // if the item is the first item in the line, set the next item as the first item
      if (prev_item == NULL) {
        (*table)[index] = item->next;
      } 
      else {
        // set the next item as the next item of the previous item
        prev_item->next = item->next;
      }
      free(item);
      return;
    }
    // go to the next item
    prev_item = item;
    item = item->next;
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  // go through all lines in the table
  for (int i = 0; i < HT_SIZE; i++) {
    ht_item_t *item = (*table)[i];
    // go through all items in a line and free them
    while (item != NULL) {
      ht_item_t *next_item = item->next;
      free(item);
      item = next_item;
    }
    // set the line to NULL
    (*table)[i] = NULL;
  }
}
