/*
 * Tabulka s rozptýlenými položkami
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
  
  for (int i = 0; i < HT_SIZE; i++){
    ht_item_t *item = (*table)[i];
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
  int index = get_hash(key);
  ht_item_t *new_item = (ht_item_t *)malloc(sizeof(ht_item_t));
  
  if (new_item != NULL) {
    new_item->key = key;
    new_item->value = value;
    new_item->next = (*table)[index];

    if ((*table)[index] == NULL) {
      // If the index is empty, insert the new item as the first item
      (*table)[index] = new_item;
    }
    else {
      // If the index is not empty, insert the new item as the first item and make it point to the previous first item
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
  int index = get_hash(key);
  ht_item_t *item = (*table)[index];
  ht_item_t *prev_item = NULL;
  while (item != NULL) {
    if (strcmp(item->key, key) == 0) {
      if (prev_item == NULL) {
        (*table)[index] = item->next;
      } else {
        prev_item->next = item->next;
      }
      free(item);
      return;
    }
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
  for (int i = 0; i < HT_SIZE; i++) {
    ht_item_t *item = (*table)[i];
    while (item != NULL) {
      ht_item_t *next_item = item->next;
      free(item);
      item = next_item;
    }
    (*table)[i] = NULL;
  }
}
