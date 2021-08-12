#include "common.h"
#include "hash_table.h"
#include "list_linked.h"
#include "stdio.h"
#include "hash_table.h"
#include "list_linked.h"
#include "utils.h"
#include "webstore.h"
#include "merch.h"
#include "cart.h"
#include "ui.h"


int main(int argc, char *argv[]) {

  webstore_t *store = store_create();

//  | Merch      | Shelf  | Amount | 
//  | Ferarri    | A01    | 3      |
//  | apple      | A10    | 1	   | 
//  | pear       | A10    | 321	   | 
//  | orange     | D21    | 123	   | 
  
  save_str(store, strdup("Ferarri"));
  save_str(store, strdup("Merci"));
  
  
  
  new_item(store, "Ferarri",  "wrooooom!", 10, "A01", 3); 
  new_item(store, "apple",  "a fruit", 10, "A10", 1);	 
  new_item(store, "pear",   "a fruit", 12, "A10", 1);	 
  new_item(store, "orange", "a fruit", 14, "D21", 123);


  store_destroy(store);
  return 0;
}


