#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "hash_table.h"
#include "iterator.h"
#include "list_linked.h"
#include "common.h"
#include "utils.h"
#include "cart.h"
#include "webstore.h"
#include "merch.h"
#include "ansi.h"
#include "macros.h"

/* _  _ ____ ____ ____ _  _ ____ _  _ ___  _ ____ ____    ____ ___  _ 
   |\/| |___ |__/ |    |__| |___ |\ | |  \ | [__  |___    |__| |__] | 
   |  | |___ |  \ |___ |  | |___ | \| |__/ | ___] |___    |  | |    | 

   [Erik Johan Karlsson]   [GPLv2]   [API for a merchendise database]
*/


bool valid_index(webstore_t *store, int index);
char *lookup_merch_name(webstore_t *store, int index); 

void print_merch(merch_t *merch);

void add_to_storage(webstore_t *store, char *name, char *shelf);
void destroy_storage(webstore_t *store);

void remove_shelf(webstore_t *store, char *shelf);
ioopm_list_t *get_locations(webstore_t *store, char *shelf);
bool storage_contains(webstore_t *store, char *name, char *shelf);

void list_shelfs(webstore_t *store, char *name);
void display_shelf(webstore_t *store, char *shelf);

void set_shelf(webstore_t *store, char *name,
	       char *shelf, size_t amount);

void store_destroy(webstore_t *store);
webstore_t *store_create();

void list_merchandise(webstore_t *store);
void remove_from_storage(webstore_t *, char *, char *);
void remove_from_storeage_merch(webstore_t *, char *, char *);
bool merch_in_stock(webstore_t *store, char *name);
int merch_stock_on_shelf(webstore_t *store, char *name, char *shelf);
bool sync_merch_stock(webstore_t *store, char *name);
size_t increase_stock(webstore_t *store, char *name,
		      char *shelf_name, size_t amount);

void show_stock(webstore_t *store);
void rename_merch(webstore_t *store, char *name, char *new_name);

size_t merch_stock(webstore_t *store, char *name);
void set_merch_stock(webstore_t *store, char *name,
		     size_t amount, char* location);

int merch_price(webstore_t *store, char *name);
void set_merch_price(webstore_t *store, char *name, size_t price);

char *merch_description(webstore_t *store, char *name);
void set_merch_description(webstore_t *store, char *name, char *desc);

void destroy_locs(webstore_t *store, char *name);


void add_merchendise(webstore_t *store, char *name, char *desc, size_t price);
void destroy_all_merch(webstore_t *store);
void remove_merchendise(webstore_t *store, char *name);
merch_t *create_merch(char *name, char *desc, size_t price, ioopm_list_t *locs);

shelf_t *create_shelf(char *shelf, size_t amount);
void destroy_shelf(shelf_t *shelf);

  
/// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// MERCH                                            ///
/// /// /// /// /// /// /// /// /// /// /// /// /// ///


// Tested 1
merch_t *create_merch(char *name, char *desc, 
		      size_t price, ioopm_list_t *locs){

  merch_t *item       = calloc(1, sizeof(merch_t));
  item->name          = name;
  item->desc          = desc;

  item->price         = price;
  item->total_amount  = 0;
  item->locs          = locs;

  return item;
}

// Tested 2
void destroy_merchendise(merch_t *merch){ // test

  if (!merch) return;
  
  merch->name = NULL;
  merch->desc = NULL;
  merch->price = (size_t)NULL;


  ioopm_link_t *shelf = merch->locs->first;
  shelf_t *shelf_data = NULL;

  while (shelf) {
    shelf_data = get_elem_ptr(shelf->element);
    destroy_shelf(shelf_data); 
    shelf_data = NULL;
    shelf      = shelf->next;                                       
  }
  // ioopm_linked_list_destroy(merch->locs);

  ioopm_linked_list_destroy(merch->locs);
  merch->locs = NULL;
  free(merch);  

}


// Tested 2
void remove_merchendise(webstore_t *store, char *name){
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    return; // The outcome is the same 
  } 
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db, 
					 ptr_elem(name)));

  ioopm_link_t *merch_data_locs = merch_data->locs->first;

// Remove merch from the shelfs containing its name
  while (merch_data_locs) {
    shelf_t *shelf = get_elem_ptr(merch_data_locs->element);      
    remove_name_from_shelf(store, shelf->shelf,  name);
    merch_data_locs = merch_data_locs->next;              

  }

  destroy_merchendise(merch_data);
  ioopm_hash_table_remove(store->merch_db, ptr_elem(name));
}

void parse_args(webstore_t *store, int argc, char *argv[]){
  arg_parse(argc, argv, store->opt);
}

bool merch_exist_p(webstore_t *store, char *name){
  return ioopm_hash_table_has_key(store->merch_db, ptr_elem(name));
}
void destroy_all_merch(webstore_t *store){
  // Remove all shelfs in storage_db, but not the hash-table.
  if ((!store) || (!store->merch_db)){
    perror("destroy_all_merch: Webstore is NULL\n");
    exit(1);
  }

  ioopm_list_t *names  = ioopm_hash_table_keys(store->merch_db);
  ioopm_link_t *current = names->first;

 if (!current){
   //   perror("destroy_all_merch: All mearch already destroyed.\n");
   ioopm_linked_list_destroy(names); 
   return;
 } 
 
 do {
   remove_merchendise(store, get_elem_ptr(current->element));
   current = current->next;

 } while (current);

 ioopm_linked_list_destroy(names); 
}

void add_merchendise(webstore_t *store,
		     char *name,
		     char *desc,
		     size_t price){

  
  if(store->merch_db == NULL){
    perror("add_merchendise: Merch database is NULL.\n");
    exit(1);
  }

  // Don't crash it
  else if (ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    return;
  }else {
    ioopm_list_t *locs = ioopm_linked_list_create();    
    merch_t *new_merch = create_merch(name, desc,
				      price, locs);

    ioopm_hash_table_insert(store->merch_db,
			    ptr_elem(name),
			    ptr_elem(new_merch));
    return; 
  }  
}


bool save_str(webstore_t *store, char *str_ptr){
  // Store the pointer to ‘str_ptr’ in the webstore
  // this to avoid memory leaks, and avoiding alot
  // of copying and freeing.

  if (!store) {
    perror("save_str: Uninitialized webstore, webstore is NULL");
    exit(1);
    
  } else if (!str_ptr){
    perror("save_str: String is NULL, cannot save it.\n");
    exit(1);
  }

  // If it has manually been emptied before
  if (!store->heap_strs){
    store->heap_strs = ioopm_linked_list_create();
  }
  
  ioopm_linked_list_append(store->heap_strs,
			   ptr_elem(str_ptr));


  return true;
}


// is_saved_str(store, str)
// Check if a `str' is saved in the heap list in `store'
// returns: True if `str' is saved else False
bool is_saved_str(webstore_t *store, char *str){  

  if (!store->heap_strs)
    return false;

  else if (!store->heap_strs->first) 
    return false;

  else if (!str) return false;

  ioopm_link_t *heap_strs = store->heap_strs->first;  

  while (heap_strs) {
    char *str_ptr = (char*)(get_elem_ptr(heap_strs->element));

    if (STR_EQ(str, str_ptr)) 
      return true;
    else heap_strs = heap_strs->next;           
  }
  return false;
}

void free_saved_strs(webstore_t *store){
  // Free all saved strings 
  ioopm_link_t *heap_alloc_strs = store->heap_strs->first;  
  // Id already freed, return
  if (!heap_alloc_strs) {
    if (store->heap_strs)
      ioopm_linked_list_destroy(store->heap_strs);  
    return; 
  }

  // Iterate through all strings freeing them
  while (heap_alloc_strs) {
    char *str = (get_elem_ptr(heap_alloc_strs->element));

    free(str); str  = NULL;
    heap_alloc_strs = heap_alloc_strs->next;           
  }

  ioopm_linked_list_destroy(store->heap_strs);
  store->heap_strs = NULL;
}



bool shelf_exists(webstore_t *store, char *shelf){

  // Unallocated webstore or shelf database
  if (store->storage_db == NULL)           return false;
  else if (!shelf || (!is_shelf(shelf)))   return false;
    
  return ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf));

}
		  
void set_merch_stock(webstore_t *store, char *name,
		     size_t amount, char* location){
  // Look in the merch db for the location (shelf)
  // if it exists its stock will be set to amount
  // else it will be added with its stock set to amount

  if((!store->merch_db) || (!name) || (!location)){
    perror("set_merch_stock: NULL Argument.\n");
    exit(1);
    
  }else if (!ioopm_hash_table_has_key(store->merch_db,
				      ptr_elem(name))){
    perror("set_merch_stock: Non existing merch.\n");
    exit(1);
  }
  // Extract location database bound to name
  elem_t elem_data =
    ioopm_hash_table_lookup(store->merch_db,
			    ptr_elem(name));
  
  merch_t      *merch_data = get_elem_ptr(elem_data);

  ioopm_link_t *merch_data_locs = merch_data->locs->first;
   
  if (merch_data->locs->size > 0){    
    shelf_t *shelf_data = get_elem_ptr(merch_data_locs->element);
    
    do {
      if (STR_EQ(shelf_data->shelf, location)){
	// Found existing shelf, set new amount, and exit
	
	shelf_data->amount        = amount;
	merch_data->total_amount -= shelf_data->amount;	
	return;
      }     
      merch_data_locs = merch_data_locs->next;
           
    } while (merch_data_locs != NULL);    
  }
  merch_data->total_amount += amount;

  shelf_t *new_shelf = create_shelf(location, amount);  
  ioopm_linked_list_append(merch_data->locs,
			   ptr_elem(new_shelf));
    
}

bool merch_in_stock(webstore_t *store, char *name){
  return ioopm_hash_table_has_key(store->merch_db,
				  ptr_elem(name));
}

// Remove new name functionality  
void rename_merch(webstore_t *store, char *name, char *new_name){
  // Edit a merch item, setting a new description,
  // new price, new name (only in merch db) if
  // the are Non-NULL
  if(!store){
    perror("merchendise_edit: Unallowed NULL argument.\n");
    exit(1);
    
  }else if(!store->merch_db){
    perror("merchendise_edit: Database is NULL.\n");
    exit(1);
    
  }else if (!ioopm_hash_table_has_key(store->merch_db,
				     ptr_elem(name))){
    perror("merchendise_edit: Non existing merch.\n");
    exit(1);
  }     
  // Name already exists 
  if (ioopm_hash_table_has_key(store->merch_db,
			       ptr_elem(new_name))){
    perror("merchendise_edit: Unallowed name change.\n");
    exit(1);
  }
  // Get the related struct
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));
  // Insert the old data under the changed name
  merch_data->name = new_name;
  ioopm_hash_table_insert(store->merch_db,
			  ptr_elem(new_name),
			  ptr_elem(merch_data));        
  // Remove the old mapping (should not free the underlaying data) 
  ioopm_hash_table_remove(store->merch_db, ptr_elem(name));
}

ioopm_list_t *merch_locs(webstore_t *store, char *name){

  merch_t *merch_data         =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  ioopm_list_t *merch_locs    = merch_data->locs;
  return merch_locs;
}


char *merch_description(webstore_t *store, char *name){
  // Return the description of merch item
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merch_description: Non existing merch.\n");
    exit(1);
    
  }else if ((!name) || (!store)){
    perror("merch_description: Unallowed NULL argument.\n");
    exit(1);
  }  
  merch_t *data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));            
  return data->desc;    
}
void set_merch_description(webstore_t *store, char *name, char *desc){
  // Set the description of merch item
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merch_description: Non existing merch.\n");
    exit(1);
    
  }else if ((!name) || (!store) || (!desc)){
    perror("merch_description: Unallowed NULL argument.\n");
    exit(1);
  }
  
  merch_t *data = get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
						       ptr_elem(name)));            
  //  free(data->desc);
  data->desc = desc;
}

int merch_price(webstore_t *store, char *name){
  // Return the price of the specified merch name
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merch_price: Non existing merch.\n");
    exit(1);
    
  }else if ((name == NULL) || (store == NULL)){
    perror("merch_price: Unallowed NULL argument.\n");
    exit(1);
  }
  merch_t *data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));            
  return data->price;    
}

void set_merch_price(webstore_t *store, char *name, size_t price){
  // Return the price of the specified merch name
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merch_price: Non existing merch.\n");
    exit(1);
  }else if ((name == NULL) || (store == NULL)){
    perror("merch_price: Unallowed NULL argument.\n");
    exit(1);
  }
  merch_t *data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));            
  data->price = price;
}

size_t merch_stock(webstore_t *store, char *name){
  // Calculate and return the total amount of a
  // merch in stock

  if (!ioopm_hash_table_has_key(store->merch_db,
				ptr_elem(name))){
    perror("merch_stock: Non existing merch.\n");
    exit(1);
  }
  
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  

  ioopm_link_t *merch_data_locs = merch_data->locs->first;  
  size_t amount = 0;
  
  if (!merch_data_locs) return 0;
 
  // Iterate through all of locs adding up
  // the amounts
  while (merch_data_locs != NULL) {
    shelf_t *shelf_data = (get_elem_ptr(merch_data_locs->element));

    amount         += shelf_data->amount;
    merch_data_locs = merch_data_locs->next;           
  }

  return amount;
}

void list_merchandise(webstore_t *store){
  // List all merchendise as a short list of 20
  // prompting for displaying more 
  if (!store){
    perror("list_merchendise: Unallowed NULL argument.\n");
    exit(1);
  }
  // All existing merch names
  ioopm_list_t *list_merch    =
    ioopm_hash_table_values(store->merch_db);

  ioopm_list_iterator_t *iter =
    ioopm_list_iterator(list_merch);

  merch_t *current            =
    get_elem_ptr(ioopm_iterator_current(iter));
  
  int continue_alert_number   = 20;
  
  // Iterate through all names
  for (int i = 1;; i++){
    // Prompt for continuing to display merch
    if ((i % (continue_alert_number) == 0) && \
	!choice_prompt("Continue printing?")) break;
    // Print current merch
    printf("┏──╸ No.%d \n", i);
    print_merch(current);

    if(ioopm_iterator_has_next(iter)){
      current = get_elem_ptr(ioopm_iterator_next(iter));      
    }else { break; }    
  }     
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list_merch);  
}

void destroy_locs(webstore_t *store, char *name){ 
  // Free up a merch locs list (list & shelfs)

  if ((!store) || (!name)){
    perror("destroy_locs: Unallowed NULL arguments.\n");
    exit(1);
  }
  
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  
  ioopm_link_t *merch_data_locs = merch_data->locs->first;  

  // Free each shelf in the merch locs
  while (merch_data_locs != NULL) {
    free(get_elem_ptr(merch_data_locs->element));
    merch_data_locs = merch_data_locs->next;;           
  }
  // Free the locs list
  ioopm_linked_list_destroy(merch_data->locs);
}

/// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// STORE                                            ///
/// /// /// /// /// /// /// /// /// /// /// /// /// ///

webstore_t *store_create(){
  // Allocate the argument handler, both hash tables
  // and the shopping cart list. And the whole webstore.

  webstore_t *new_webstore = calloc(1, sizeof(webstore_t));
  
  new_webstore->opt = create_arg_opt();

  // Storage and Merch databases
  new_webstore->merch_db   =
    ioopm_hash_table_create(extract_int_hash_key,
			    eq_elem_int, eq_elem_string);
  new_webstore->storage_db =
    ioopm_hash_table_create(extract_int_hash_key,
			    eq_elem_int, eq_elem_string);	

  //linked list that holds all shopping carts
  new_webstore->all_shopping_carts = 
    ioopm_linked_list_create();

  // Save heap allocated strings
    new_webstore->heap_strs = 
    ioopm_linked_list_create();
  
  
  //ioopm_linked_list_append(new_webstore->all_shopping_carts,
  //			   ptr_elem(create_cart(new_webstore)));
  



  return new_webstore;
}

void store_destroy(webstore_t *store){
  // Deallocate the argument handler, both hash tables
  // and the shopping cart list. And the whole webstore.
  if (!store){
    perror("store_destroy: Webstore is NULL.\n");
    exit(1);
  }

  
  destroy_all_merch(store); 
  destroy_storage(store);   // Free all shelfs (with merch names)

  if (store->merch_db)
      ioopm_hash_table_destroy(store->merch_db);

  if (store->storage_db)
    ioopm_hash_table_destroy(store->storage_db);

  if (store->all_shopping_carts)
    destroy_all_carts(store);

  //  Ioopm_linked_list_destroy(store->all_shopping_carts);
  if (store->opt)
    destroy_arg_opt(store->opt);

  if (store->heap_strs)
    free_saved_strs(store);


  free(store);
}

/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// SHELF                                                  ///
/// /// /// /// /// /// /// /// /// /// /// /// /// ///

shelf_t *create_shelf(char *shelf, size_t amount){
  // Allocate and create shelf

  // Check for sub-zero stock
  if (amount < 0) {
    perror("create_shelf: Negative stock.\n");
    exit(1);

  } else if ((shelf == NULL) || (!is_shelf(shelf))){
    perror("create_shelf: Shelf name is incorrectly formatted.\n");
    exit(1);
  }

  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));    
  new_shelf->shelf   = shelf;
  new_shelf->amount  = amount;

  return new_shelf;
}

void destroy_shelf(shelf_t *shelf){
  // Deallocate a shelf
  if (shelf == NULL){
    perror("destroy_shelf: Unallowed NULL argument.\n");
    exit(1);
  }
  shelf->shelf  = NULL;
  shelf->amount = (size_t)NULL;
  free(shelf);
  shelf = NULL;
}


int merch_stock_on_shelf(webstore_t *store, char *name, char *shelf){
  // Return the amount merchendise at a specific shelf

  if ((!store) || (!name) || (!shelf)) {
    perror("merch_stock_on_shelf: Unallowed NULL arguments");    
  }    
  else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    return 0;
  }else {
  
    merch_t *merch_data =
      get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					   ptr_elem(name)));  
    ioopm_link_t *merch_data_locs = merch_data->locs->first;  

    if (merch_data_locs == NULL) return 0;
  
    shelf_t *shelf_data = NULL;

    do {
      shelf_data = get_elem_ptr(merch_data_locs->element);

      if (STR_EQ(shelf_data->shelf, shelf))
	return shelf_data->amount;
    
      merch_data_locs = merch_data_locs->next;

    } while (merch_data_locs);

  }  
   //  perror("merch_stock_on_shelf: The merch has no stock on the shelf.\n");
    return 0;
}
  
void remove_shelf_from_merch(webstore_t *store, char *name,
                             char *shelf) { // Rename


  if (!store || !name || !shelf) {
    perror("merch_stock_on_shelf: Unallowed NULL Argument");
    exit(1) ;
  } else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))) {
    // if it does not exist, removal of it results in the same state    
    return;
  }
  else {
    
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
    					 ptr_elem(name)));
  ioopm_link_t *shelf_l      = merch_data->locs->first;
  ioopm_link_t *shelf_prev_l = merch_data->locs->first;
  
  shelf_t *shelf_data   = NULL;

  if (!shelf_l)
    return;

  assert(merch_data);
  
  do {
      shelf_data = get_elem_ptr(shelf_l->element);
      assert(shelf_data);
      if (STR_EQ(shelf_data->shelf, shelf)) {


        free(shelf_data);
        // CASE: Only shelf in merch        
        if (shelf_l == shelf_prev_l) {
          free(shelf_l);
	  shelf_l = NULL;
        }else {
	  if (shelf_l->next)
	    shelf_prev_l->next = shelf_l->next;
	  else
	    shelf_prev_l->next = NULL;

          free(shelf_l);
	  return;          
        }        
      }                 
      shelf_l = shelf_l->next;
    } while (shelf_l);
  }
  return;
}



void remove_from_storage(webstore_t *store, char *name, char *shelf){ // Rename

  if (ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))) {
    remove_name_from_shelf(store, shelf, name);
  }
  //
  //
  //  if ((!prev) || (!db_names) || (!shelfs))
  //    return;
  //  for (int i = 1; i <= db_names->size; i++) {
  //    char *name2 = get_elem_ptr(ioopm_linked_list_get(db_names, i));
  //    if (!name)  ioopm_linked_list_remove(db_names, i);
  //
  //    if (STR_EQ(name2, name)){
  //      return;
  //    }
  //  }
  //}
}  
  /*
  // Return if empty

  // dbnames size segfaults
  if   ((!db_names)       ||
       (!db_names->first) ||
       (db_names->size < 1))
    return;
  
  
  
  ioopm_link_t *db_item = db_names->first;
  ioopm_link_t *db_prev = db_names->first;
  // Print what shelf
  
  // Print all items
  while (db_item) {
    // Already exists in database

    if ((char*)get_elem_ptr(db_item->element) == name){
      if (db_item->next)
	db_prev->next = db_item->next;
      
      db_names->size--;
      free(db_item);

    }else db_prev = db_item;
      
    db_item = db_item->next;    
    
  

  
  SLOG(store, "rem_merch: removed");
  SLOG(store, removed_name);
}*/ 


void display_shelf(webstore_t *store, char *shelf){
  // Prettyprint the contents of a shelf
  if ((!store) || (!shelf)){
    perror("display_storage: Unallowed NULL arguments.\n");
    exit(1);
  }  
  // Names stored at requested shelf location
  ioopm_list_t *db_names = get_locations(store, shelf);
  ioopm_link_t *db_item  = db_names->first;
  // Print what shelf
  printf("┃ > %s:", shelf);
  // Print all items

  do { // Already exists in database
    printf(" %s", (char*)get_elem_ptr(db_item->element));
					      
    db_item = db_item->next;    
  } while (db_item);
  
  printf("\n");
}
 bool shelf_contains(webstore_t *store, char *shelf, char *name){
  // Prettyprint the contents of a shelf
  if ((!store) || (!shelf)){
    perror("display_storage: Unallowed NULL arguments.\n");
    exit(1);
  }  
  // Names stored at requested shelf location
  ioopm_list_t *db_names = get_locations(store, shelf);
  ioopm_link_t *db_item  = db_names->first;
  // Print what shelf

  // Print all items

  do { // Already exists in database
    if (strcmp((char *)get_elem_ptr(db_item->element), name))
      return true;
					      
    db_item = db_item->next;    
  } while (db_item);
  
  return false;
}

void list_shelfs(webstore_t *store, char *name){
  merch_t *merch_data         =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  ioopm_list_t *merch_data_locs    = merch_data->locs;
  ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_data_locs);

  shelf_t *shelf = get_elem_ptr(ioopm_iterator_current(iter));

  printf("\n┃ -  Listing shelfs containing %s  -\n", name);
  
  for (int i = 1;; i++){
    printf("┃ [%d] > %sx%ld\n",
	   i,
	   shelf->shelf,
	   shelf->amount);
      
    if(ioopm_iterator_has_next(iter)){
        
      shelf   = get_elem_ptr(ioopm_iterator_next(iter));

    }else { break; }    
  }    
  ioopm_iterator_destroy(iter);
}

void remove_name_from_shelf(webstore_t *store, char *shelf, char *name){ // rename

  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    //    perror("remove_name_from_shelf: Non existing shelf name.\n");
    return;
  }
//  else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
//    //    perror("remove_name_from_shelf: Non existing merch name.\n");
//    return;
//  }
  ioopm_list_t *list         =
    get_elem_ptr(ioopm_hash_table_lookup(store->storage_db,
					 ptr_elem(shelf)));
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);

  char *current_name = get_elem_ptr(ioopm_iterator_current(iter));
  
  for (int i = 1;; i++){

    if(STR_EQ(current_name, name)){
      ioopm_iterator_remove(iter);
      ioopm_iterator_destroy(iter);
      return;
    }
    if(ioopm_iterator_has_next(iter)){
        
      shelf = get_elem_ptr(ioopm_iterator_next(iter));

    }else { break; }    
  }    
  ioopm_iterator_destroy(iter); 
}
bool storage_contains(webstore_t *store, char *name, char *shelf){ // rename

  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    //    perror("remove_name_from_shelf: Non existing shelf name.\n");
    return false;
  }
//  else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
//    //    perror("remove_name_from_shelf: Non existing merch name.\n");
//    return;
//  }
  ioopm_list_t *list         =
    get_elem_ptr(ioopm_hash_table_lookup(store->storage_db,
					 ptr_elem(shelf)));
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);

  char *current_name = get_elem_ptr(ioopm_iterator_current(iter));
  
  for (int i = 1;; i++){

    if (STR_EQ(current_name, name)) {
      ioopm_iterator_destroy(iter);      
      return true;
    }
    if(ioopm_iterator_has_next(iter)){
        
      shelf = get_elem_ptr(ioopm_iterator_next(iter));

    }else { break; }    
  }
  ioopm_iterator_destroy(iter);
  return false;  
}

char *get_shelf_after_shelf_nr(webstore_t *store, int shelf_nr, char *name){
  
  merch_t *merch_data         =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
					 
  ioopm_list_t *merch_data_locs    = merch_data->locs;
  size_t total = ioopm_linked_list_size(merch_data_locs);

  if(total < (size_t)shelf_nr){
    printf("\n┃  -!-  Impossible Shelf  -!-\n");
    return NULL; 
  }

  ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_data_locs);

  shelf_t *shelf = get_elem_ptr(ioopm_iterator_current(iter));
  
  for (int i = 1;; i++){
    if(shelf_nr == i){
      ioopm_iterator_destroy(iter);
      return shelf->shelf; 
    }
    
    if(ioopm_iterator_has_next(iter)){
        
      shelf   = get_elem_ptr(ioopm_iterator_next(iter));

    }else { break; }    
  }    
  

  ioopm_iterator_destroy(iter);
  printf("\n┃  -!-  Impossible Shelf  -!-\n");  
  return NULL; 
}

/// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// STORAGE                                          ///
/// /// /// /// /// /// /// /// /// /// /// /// /// ///

ioopm_list_t *get_locations(webstore_t *store, char *shelf){
  // Return the associated list to a shelf in storage
  
  if (!store || !shelf || !is_shelf(shelf)){
    //    perror("get_locations: Unallowed NULL arguments.\n");
    return NULL; // Can this be a problem??
  }else 
    return get_elem_ptr(ioopm_hash_table_lookup(store->storage_db,
					      ptr_elem(shelf)));
}

void remove_shelf(webstore_t *store, char *shelf){
  // Remove a storage shelf from the storage_db hash-table.
  
  if ((store == NULL) || (shelf == NULL)){
    perror("remove_shelf: Unallowed NULL arguments.\n");
    return;
  }
  // Shelf does not exist in storage db  
  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    perror("remove_shelf: Cannot remove, non-existing shelf.\n");
    return;
  } 
    
  ioopm_list_t *storage_list = get_locations(store, shelf);    

  ioopm_linked_list_destroy(storage_list); 
  ioopm_hash_table_remove(store->storage_db, ptr_elem(shelf));

   
}

void destroy_storage(webstore_t *store){
  // Remove all shelfs in storage_db, but not the hash-table.
  if (store == NULL) return;  
  else if  (!store->storage_db) return;
  
 ioopm_list_t *shelfs  = ioopm_hash_table_keys(store->storage_db);
 ioopm_link_t *current = shelfs->first;
 
 if (!current){
   ioopm_linked_list_destroy(shelfs); 
   return;
 }

 // Iterate all shelfs removing them
 while (current) {  
   remove_shelf(store, get_elem_ptr(current->element));
   current = current->next;
 } 

  ioopm_linked_list_destroy(shelfs); 
}

void add_to_storage(webstore_t *store, char *name, char *shelf){

  if (store == NULL){
    perror("add_to_storage: Unallowed NULL argument.\n");
    return;
  }else if (STR_EQ(name, "") || (STR_EQ(shelf, ""))){
    perror("add_to_storage: Cannot add empty names or shelfs.\n");
  }
  
  // Create a new storage list if shelf does not exist
  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    ioopm_list_t *storage_list = ioopm_linked_list_create();
    ioopm_linked_list_append(storage_list, ptr_elem(name));
    ioopm_hash_table_insert(store->storage_db,
			    ptr_elem(shelf),
			    ptr_elem(storage_list));    
  }
  
  ioopm_list_t *list         =
    get_elem_ptr(ioopm_hash_table_lookup(store->storage_db,
					 ptr_elem(shelf)));
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  char *current_name = get_elem_ptr(ioopm_iterator_current(iter));
  
  for (int i = 1;; i++){

    if (STR_EQ(current_name, name)) {
      ioopm_iterator_destroy(iter);      
      return;
    }
    if(ioopm_iterator_has_next(iter)){
        
      shelf = get_elem_ptr(ioopm_iterator_next(iter));
    }else { break; }    
  }
  ioopm_iterator_destroy(iter);      
  ioopm_linked_list_append(list, ptr_elem(name));
}


//bool storage_contains(webstore_t *store, char *name, char *shelf){
//
//  if (!store || !shelf || !name){
//    perror("storage_contains: Unallowed NULL argument.\n");
//    exit(1);
//  }
//  if (!shelf_exists(store, shelf)) {
// 
//    return false;
//  }
//
//  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf)))
//    return false;
//  
//  
//  // Names stored at requested shelf location
//  ioopm_list_t *db_names = get_locations(store, shelf);
//  bool list_contains_p;
//  elem_t
//  if (db_names)
//    list_contains_p = ioopm_linked_list_any(db_names, eq_elem_string,
//					    name);
//  else
//    list_contains_p = false;
//    
//  return list_contains_p;
//}
//  //  
//  ioopm_link_t *db_item = NULL;
//  
//  if (db_names)    
//    db_item  = db_names->first;
//  else {
//    return false;
//  }
//
//  while (db_item) {
//    if (STR_EQ(get_elem_ptr(db_item->element), name))
//      return true;
//    
//    db_item = db_item->next;           
//  }
//
//  // If shelf does not exist, it cannot contain item  
//  
//  // Does not exist in database
//  return false;
//}

// add merch on shelf
void set_shelf(webstore_t *store, char *name,
	       char *shelf, size_t amount){
  // Add / Update shelf to both the merch database and the
  // storage database. If it already exists, update amount.
  
  // Add name to shelf if it already doesnt not contain it.
  if (!storage_contains(store, name, shelf))
    add_to_storage(store, name, shelf);

  
  set_merch_stock(store, name, amount, shelf);
  //  sync_merch_stock(store, name);
}


// bool sync_merch_stock(store, name)
// Calculate the total stock of a merch item from
// the locs list and update merch->total_amount
// reflecting that.
// Return: If a syncronization happened the function returns true.
bool sync_merch_stock(webstore_t *store, char *name){

  
  if (!store || !name){
    perror("sync_merch_stock: Unallowed NULL argument\n");
    exit(1);

    
  } else if (!ioopm_hash_table_has_key(store->merch_db,
				       ptr_elem(name))){
    perror("sync_merch_stock: Non existing merch.\n");
    exit(1);
  }
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  size_t old_amount           = merch_data->total_amount;
  merch_data->total_amount = merch_stock(store, name);
  // Return true if the total amount was changed

  
  ioopm_link_t *db_item  = merch_data->locs->first;
 
   while (db_item) {
    shelf_t *shelf = get_elem_ptr(db_item->element);
   
    // Remove stock gradually from shelfs
    if (shelf->amount > 0){

      // If not on the correct shelf, fix it
      if (!storage_contains(store, merch_data->name, shelf->shelf)){
	add_to_storage(store, merch_data->name, shelf->shelf);
      }

    }
    db_item = db_item->next;           
  }
    

  ///  merch_data->total_amount = new_amount;
  
  if (old_amount != merch_data->total_amount) return true;
  else return false;
}

size_t decrease_equal_stock(webstore_t *store, char *name, size_t amount){
  // Increase (or decrease) the stock at an existing
  // shelf. A negative (amount) decreases stock, positive
  // increases.  
  if ((!store) || (!name)){
    perror("increase_stock: Unallowed NULL argument\n");
    exit(1);

  } else if (!ioopm_hash_table_has_key(store->merch_db,
				       ptr_elem(name))){
    perror("increase_stock: Non existing merch.\n");
    exit(1);
  }
  // Add a specified amount of an item at a shelf.
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  // Get the current amount of the item on the shelf
  int new_amount = 0;
  int tmp_amount = amount;
  ioopm_link_t *db_item  = merch_data->locs->first;


  // Remove amount from shelf n
  // If (new amount < 0) then shelf amount = 0, continue on -new_amount

  do {
    shelf_t *shelf = get_elem_ptr(db_item->element);

    // Remove stock gradually from shelfs
    if (shelf->amount > 0){
      new_amount = shelf->amount - tmp_amount;

      if (new_amount < 0){	
	shelf->amount = 0;
	tmp_amount = -new_amount;

      }else{
	shelf->amount = new_amount;
        break;
      } 

    } else{
	shelf->amount = new_amount;
	break;
    } 
					      
    db_item = db_item->next;           
  } while (db_item != NULL);

  // Update the total stock
  sync_merch_stock(store, name);
  // Return the new stock at the shelf
  return new_amount;
}
size_t increase_stock(webstore_t *store, char *name,
		      char *shelf_name, size_t amount){
  // Increase (or decrease) the stock at an existing
  // shelf. A negative (amount) decreases stock, positive
  // increases.  
  if ((!store) || (!name) || (!shelf_name)){
    perror("increase_stock: Unallowed NULL argument\n");
    exit(1);
    
  } else if (!ioopm_hash_table_has_key(store->merch_db,
				       ptr_elem(name))){
    perror("increase_stock: Non existing merch.\n");
    exit(1);

  } else if (!ioopm_hash_table_has_key(store->storage_db,
				      ptr_elem(shelf_name))){
    perror("increase_stock: Storage doesnt contain shelf.\n");
    exit(1);
  }
  
  // Add a specified amount of an item at a shelf.
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db, ptr_elem(name)));

  assert(merch_data); // 
  
  // Get the current amount of the item on the shelf
  size_t old_amount = merch_stock_on_shelf(store, name, shelf_name);
  size_t new_amount = old_amount + (size_t)amount;

  // Update the shelf stock
  set_merch_stock(store, name, new_amount, shelf_name);
  // Update the total stock
  merch_data->total_amount += (size_t)amount;
  // Return the new stock at the shelf
  return new_amount;
}

int count_loc_dep_stock(webstore_t *store){
  // [...]


  assert(store);

  ioopm_list_t *shelfs = ioopm_hash_table_keys(store->storage_db);

  if (!shelfs) return 0;

  ioopm_link_t *shelf  = shelfs->first;
  ioopm_link_t *name   = NULL;
  char * current_shelf = NULL;

  int total = 1;     
  while (shelf) {    
    if (get_elem_ptr(shelf->element)){
      current_shelf = get_elem_ptr(shelf->element);
      name          = get_locations(store, current_shelf)->first;
      
      if (name)
        while (name) {

          total++;
          name = name->next;
	}       
    }
    // Next shelf
    shelf = shelf->next;
  }


  ioopm_linked_list_destroy(shelfs);

  return total;
}

void show_stock(webstore_t *store){
  // Prettyprint all shelfs, together with all merchendise metadata.
  // Displays: All shelfs, All merch (in shelfs), their;
  // Name, Description, Price, Stock Amount.

  // Pretty Print Template used
  // --- <shelf>
  // <name> - <price>Kr < OUT OF STOCK| IN STOCK> (<stock>st)
  // <description>
  // [...]

  assert(store);
  
  ioopm_list_t *shelfs = ioopm_hash_table_keys(store->storage_db);

  if (shelfs == NULL) {
    printf("┃ > Empty\n\n");
    return;
  }

  ioopm_link_t *shelf = shelfs->first;
  ioopm_link_t *name  = NULL;

  char * current_shelf = NULL;
  char *current_name   = NULL;
  int current_stock    = 0;
  int current_nr       = 1; 
  
  do {
    if (get_elem_ptr(shelf->element)){
      current_shelf = get_elem_ptr(shelf->element);

      name = get_locations(store, current_shelf)->first;
      if (name){
	printf("┃ - %s -\n┃\n", current_shelf);      
      
	do {
	  current_name  = (char *)get_elem_ptr(name->element);
	  current_stock = merch_stock_on_shelf(store, (char*)current_name,
					       (char *)current_shelf);      
	  if (current_stock > 0){
	    printf("┃ [%d] %s\n", current_nr, current_name); 
	    printf("┃ > Price: %dKr\n", merch_price(store, current_name));
	    printf("┃ > Stock: %dSt\n", current_stock);
	    printf("┃ > Desc: %s\n",    merch_description(store, current_name));
	    puts("┃");
	    
	    // Next Name
	    current_nr += 1;
	    
	  }//else printf("| %s (No Stock)\n", current_name);
	  
	  name = name->next;
	  
	} while (name);
      }
    }    // Next shelf
    shelf = shelf->next;
    
  } while (shelf);

    puts("\n");
    ioopm_linked_list_destroy(shelfs);
}

char *get_merch_name_in_storage(webstore_t *store, int nr_merch){
  
  ioopm_list_t *shelfs = ioopm_hash_table_keys(store->storage_db);

  ioopm_link_t *shelf  = shelfs->first;
  ioopm_link_t *name;
  char *current_name;
  int current_nr = 0; 
  
  while (shelf) {
    char * current_shelf = (char *)get_elem_ptr(shelf->element);
    name          = get_locations(store, current_shelf)->first;
      
    while (name) {
      {
	current_name  = (char *)get_elem_ptr(name->element);
	
	current_nr += 1;
	if(current_nr == nr_merch){ 
	  ioopm_linked_list_destroy(shelfs);
	  return current_name; 
	}

	// Next Name
      } name          = name->next;
      
    }

    // Next shelf
    
    shelf         = shelf->next;
    
  } 
  
  return NULL;
  // <-----------------------------------------------------
  //    ioopm_linked_list_destroy(shelfs);
}

char *lookup_merch_name(webstore_t *store, int index){ 
  // Return the name of the merch at a specified
  // index in the list returned by hash_table_values
  if (store == NULL){
    perror("lookup_merch_name: Unallowed NULL argument.\n");
    return "";
  }  
  ioopm_list_t *list_merch =
    ioopm_hash_table_values(store->merch_db); 

  if (index < 0){
    printf("┃  \e[31mToo Small Index.\n\e[0m");
    ioopm_linked_list_destroy(list_merch);  
    return "";
   } 
  if (((size_t)index >= ioopm_linked_list_size(list_merch))){
    printf("┃  Too Large index.\n");
    ioopm_linked_list_destroy(list_merch);  
    return "";
  }
  elem_t value_ptr         =
    ioopm_linked_list_get(list_merch, index);
  merch_t *merch           =
    get_elem_ptr(value_ptr);
  char *merch_name = merch->name;
  
  ioopm_linked_list_destroy(list_merch);  
  return merch_name;
}



bool is_merch(webstore_t *store, int id){
  return valid_index(store, id);
}

bool valid_index(webstore_t *store, int index){
  // Return true if index is less or equal to
  // the amount of merch in the merch database

  int total = count_loc_dep_stock(store);
  
  if (index > total){
    PROMPT_INVALID("Too small index");

    return false;
  }  
  else if (index < 0){
    perror("valid_index: Too Small Index.\n");

    return false;
  }  
  else if(index-1 >= total){

    return false;
  }

  return true;
}



void print_merch(merch_t *merch){
  printf("┃ > %sItem:%s          %s%s%s\n", FG_GREEN,   NORMAL,
	 FG_BLUE, merch->name,  NORMAL);  
  printf("┃ > %sDescription:%s   %s%s%s\n", FG_GREEN,   NORMAL,
	 FG_BLUE, merch->desc,  NORMAL);
  printf("┃ > %sPrice:%s         %s%ld%s\n",FG_GREEN,   NORMAL,
	 FG_BLUE, merch->price, NORMAL);
  printf("┃ > %sStock (Total):%s %s%ld%s\n", FG_GREEN,  NORMAL,
	 FG_BLUE, merch->total_amount,  NORMAL);
}

/// Other


// list_merchandise(webstore_t *store);
// list all items in store
// TODO REQ: Alphabetical order (soft req) - qsort
// TODO REQ: print 20 at time
//TODO REQ: ask if more
// list of all ptr of merch

