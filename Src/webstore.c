#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "hash_table.h"
#include "list_linked.h"
#include "utils.h"
#include "webstore.h"



/// Merch

merch_t *create_merch(char *name,
		      char *desc,
		      size_t price,
		      ioopm_list_t *locs){

  merch_t *item = calloc(1, sizeof(merch_t));

  item->name          = calloc(1, sizeof(char*));
  item->name          = name;
  item->desc          = calloc(1, sizeof(char*));
  item->desc          = desc;
  item->price         = price;

  item->total_amount  = 0;
  item->locs          = locs;

  return item;
}

void print_merch(merch_t *merch){
  printf("Item: %ld$ %ldx%s [%s]", merch->price,
	 merch->total_amount, merch->name,
	 merch->desc);
}



/// Shelf

shelf_t *create_shelf(char *shelf, int amount){
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));

  new_shelf->shelf  = shelf;
  new_shelf->amount = amount;

  return new_shelf;
}



/// Store

webstore_t *store_create(){
  webstore_t *new_webstore = calloc(1, sizeof(webstore_t));

  new_webstore->merch_db   = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
  new_webstore->storage_db = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);

  return new_webstore;
}
void store_destroy(webstore_t *store){

  ioopm_hash_table_destroy(store->merch_db);
  ioopm_hash_table_destroy(store->storage_db);
}

void add_merchendise(webstore_t *store, char *name, char *desc, size_t price){

  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    // Can add to warehouse
    ioopm_list_t *locs = ioopm_linked_list_create();
    merch_t *new_merch = create_merch(name, desc, price, locs);

    ioopm_hash_table_insert(store->merch_db, ptr_elem(name), ptr_elem(new_merch));

    //TODO: item->total_amount++;
    return; // SUCCESS
  }
}

void remove_merchendise(webstore_t *store, char *name){

    ioopm_hash_table_remove(store->merch_db, ptr_elem(name));
    //TODO: item->total_amount--;
    return; // SUCCESS
  }
}

void list_merchandise(webstore_t *store){
  //list all items in store
  //Alphabetical order (soft req) - qsort
  //print 20 at time
  //ask if more
  elem_t current_merch_ptr;
  ioopm_list_t *list_merch = ioopm_hash_table_values(store->merch_db); //list of all ptr of merch
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list_merch);
  for (int i = 0; i < list_merch->size; i++){
     if(ioopm_iterator_has_next(iter){
       current_merch_ptr = ioopm_iterator_next(iter_k);
       merch_t *merch = &current_merch_ptr; //??
       print_merchmerch);
     }
  }
}
