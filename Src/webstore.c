#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "hash_table.h"
#include "iterator.h"
#include "list_linked.h"
#include "common.h"
#include "utils.h"
#include "webstore.h"

#define get_elem_ptr(e) e.p
#define MLOG(store, fun, name, msg)		\
  if (store->opt->log_p)\
    merch_log(fun, name, msg, 0);

#define OMLOG(store, fun, name, msg,i)		\
  if (store->opt->log_p)\
    merch_log(fun, name, msg, i);

#define QLOG(store, fun, msg)		\
  if (store->opt->log_p)\
    merch_log(fun, "", msg, 0);

void merch_log(char *function, char *name, char *message, int number){
  printf(R_STR_STR, function);
  printf(":%d %s", number, name);
  printf(B_STR_STR, message);
  printf("\n");
}



/// Merch


void merch_delete(webstore_t *store, char *name){
  // Free Locs  
  locs_delete(store, name);  

  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));

  ioopm_hash_table_remove(store->merch_db,
			  ptr_elem(name));    
  // Free Merchendise
  free(merch_data);
  
  // Remove from Hash Table
}

merch_t *create_merch(char *name,
		      char *desc,
		      size_t price,
		      ioopm_list_t *locs){

  merch_t *item = calloc(1, sizeof(merch_t));

  item->name          = name;
  item->desc          = desc;
  item->price         = price;

  item->total_amount  = 0;
  item->locs          = locs;

  return item;
}


void print_merch(merch_t *merch){
  printf("| Item:          %s\n", merch->name);  
  printf("| Description:   %s\n", merch->desc);
  printf("| Price:         %ld\n",merch->price);
  printf("| Stock (Total): %ld\n", merch->total_amount);
}

void add_merchendise(webstore_t *store,
		     char *name,
		     char *desc,
		     size_t price){

  // ERROR IF merch_db is NULL
  if(store->merch_db == NULL){
    perror("ADD_MERCHENDISE: Uninitizalized Merch-Database,\
            the database has not been initialized.\n");
    exit(-1); // REMOVE THIS LATER
    return; // ERROR
  }
  // ERROR IF merch_db has key  
  else if (ioopm_hash_table_has_key(store->merch_db,
				    ptr_elem(name))){
    perror("ADD_MERCHENDISE: Duplicate Merch, \
            the name is already registered in Merch-Table.\n");
    exit(-1); // REMOVE THIS LATER
    return; // ERROR
  }

  else {    
    ioopm_list_t *locs = ioopm_linked_list_create();    
    merch_t *new_merch = create_merch(name, desc,
				      price, locs);

    ioopm_hash_table_insert(store->merch_db,
			    ptr_elem(name),
			    ptr_elem(new_merch));
    return; // SUCCESS
  }  
}

void remove_merchendise(webstore_t *store, char *name){
  if (!ioopm_hash_table_has_key(store->merch_db,
				ptr_elem(name))){
    perror("remove_merchendise: Non existing item, \
            The name to be removed does not exist.\n");
    exit(-1); // REMOVE THIS LATER
    return; // ERROR
  }  

  merch_delete(store, name);  
  return;
}

void prompt_remove_merchendise(webstore_t *store){
  list_merchandise(store);
  int number = ask_question_int("\
Enter the number of item that you wish to remove: \n");
  while(!valid_index(store, number)){
    number = ask_question_int("\
Enter a valid number of item that you wish to remove: \n");
  }
  char *name = lookup_merch_name(store, number);
  remove_merchendise(store, name);
  return; // SUCCESS
}

merch_t *merch_change_internal_name_function(merch_t *merch_data,
					     void *new_name){
  merch_data->name = (char*)new_name;
  return merch_data;
}

merch_t *merch_change_description_function(merch_t *merch_data,
					   void *new_desc){
  if (merch_data == NULL){
    perror("merch_change_description_function: Merch,\
            the merchendise has not been initialized.\n");
    exit(-1); // REMOVE THIS LATER
  }
  else { merch_data->desc = (char*)new_desc; }
  return merch_data;
}

merch_t *merch_change_price_function(merch_t *merch_data,
				     void *new_price){ 
  if (merch_data == NULL){
    perror("merch_change_price_function: Merch,\
            the merchendise has not been initialized.\n");
    exit(-1); // REMOVE THIS LATER
  }
  else { merch_data->price = (size_t)new_price; }
  return merch_data;
}

merch_t *merch_change_locs_function(merch_t *merch_data,
				    void *new_locs){ 
  if (merch_data == NULL){
    perror("merch_change_locs_function: Merch,\
            the merchendise has not been initialized.\n");
    exit(-1); // REMOVE THIS LATER
  }
  else { merch_data->locs = (ioopm_list_t*)new_locs; }
  return merch_data;
}

void merchendise_new_internal_name(webstore_t *store,
				   char *name,
				   char *new_name){
  merchendise_modify(store, (char*)name,
			    merch_change_internal_name_function,
			    (char*)new_name);
}
void merchendise_new_desc(webstore_t *store,
			  char *name,
			  char *edited_desc){
  
   merchendise_modify(store, (char*)name,
			    merch_change_description_function,
			    (char*)edited_desc);
}
void merchendise_new_price(webstore_t *store,
			   char *name,
			   size_t new_price){
  
   merchendise_modify(store, (char*)name,
			    merch_change_price_function,
			    (size_t*)new_price);
}
void merchendise_new_locs(webstore_t *store,
			  char *name,
			  ioopm_list_t *new_locs){
  
  merchendise_modify(store, (char*)name,
			    merch_change_locs_function,
			    (ioopm_list_t*)new_locs);
}

bool merch_in_stock(webstore_t *store, char *name){
  return ioopm_hash_table_has_key(store->merch_db,
				  ptr_elem(name));
}

  
void merchendise_edit(webstore_t *store, char *name,
		      size_t *new_price, char *new_desc,
		      char   *new_name){
  // Edit a merch item, setting a new description,
  // new price, new name (only in merch db) if
  // the are Non-NULL
  if((store == NULL)){
    perror("merchendise_edit: Unallowed NULL argument.\n");
  }else if(store->merch_db == NULL){
    perror("merchendise_edit: Database is NULL.\n");
  }else if (!ioopm_hash_table_has_key(store->merch_db,
				     ptr_elem(name))){
    perror("merchendise_edit: Non existing merch.\n");    
  }   
  // Change Price if (new_price) is non NULL
  if (new_price != NULL){
    merchendise_new_price(store, name, (size_t) new_price);
  }// Change Desc  if (new_desc)  is non NULL
  if (new_desc != NULL){
    merchendise_new_desc(store, name, new_desc);
  }// Change Name  if (new_name)  is non NULL
  if (new_name != NULL){
    // Name already exists 
    if (ioopm_hash_table_has_key(store->merch_db,
				 ptr_elem(new_name))){
      perror("merchendise_edit: Unallowed name change.\n");
    }
    // Get the related struct
    merch_t *merch_data =
      get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					   ptr_elem(name)));
    // Insert the old data under the changed name
    ioopm_hash_table_insert(store->merch_db,
			    ptr_elem(new_name),
			    ptr_elem(merch_data));        
    // Remove the old mapping (should not free the underlaying data) 
    ioopm_hash_table_remove(store->merch_db, ptr_elem(name));
  }
}
void merchendise_modify(webstore_t *store, char *name,
			merch_modify_function *fun,
			void *fun_arg){
  if ((store == NULL) || (name == NULL) || (fun == NULL)){
    perror("merchendise_modify: Unallowed NULL arguments.\n");
  } else if(store->merch_db == NULL){
    perror("merchendise_modify: Merch database is NULL.\n");
  } else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merchendise_modify: Non existing merch.\n");    
  } else {
    
    merch_t *data =
      get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					   ptr_elem(name)));    
    merch_t *new_data = fun(data, fun_arg);

   // Remove the old merch from the merch database
   ioopm_hash_table_remove(store->merch_db,
			   ptr_elem(name));

   // Reinsert the modified merch into the database
   ioopm_hash_table_insert(store->merch_db,
			   ptr_elem(name),
			   ptr_elem(new_data));   
   return;
  }    
}
char *merch_get_desc_function(merch_t *merch_data){
  // Helper function for extracting destription
  // from a merch
  return strdup(merch_data->desc);
}
  
char *merch_description(webstore_t *store, char *name){
  // Return the description of merch item
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merchendise_modify: Non existing merch.\n");
  }else if ((name == NULL) || (store == NULL)){
    perror("merchendise_modify: Unallowed NULL argument.\n");
  }
  
  merch_t *data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));        
    
  return strdup(data->desc);    
}

int merch_price(webstore_t *store, char *name){
  // Return the price of the specified merch name
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merchendise_modify: Non existing merch.\n");
  }else if ((name == NULL) || (store == NULL)){
    perror("merchendise_modify: Unallowed NULL argument.\n");
  }
  merch_t *data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));            
  return data->price;    
}


char *lookup_merch_name(webstore_t *store, int index){ 
  // Return the name of the merch at a specified
  // index in the list returned by hash_table_values
  if ((store == NULL) || (index <= 0)){
    perror("lookup_merch_name: Unallowed NULL argument.\n");
  }  
  ioopm_list_t *list_merch =
    ioopm_hash_table_values(store->merch_db);
  
  if ((index <= 0) || (ioopm_linked_list_size(list_merch))){
    perror("lookup_merch_name: Impossible index.\n");
  }
  elem_t value_ptr         =
    ioopm_linked_list_get(list_merch, index);
  merch_t *merch           =
    get_elem_ptr(value_ptr);
  char *merch_name = merch->name;
  
  ioopm_linked_list_destroy(list_merch);  
  return merch_name;
}

bool valid_index(webstore_t *store, int index){
  // Return true if index is less or equal to
  // the amount of merch in the merch database
  ioopm_list_t *list = ioopm_hash_table_values(store->merch_db);

  if ((index <= 0) || (ioopm_linked_list_size(list))){
    perror("lookup_merch_name: Impossible index.\n");
  }  
  else if(index-1 >= list->size){
    ioopm_linked_list_destroy(list);
    return false;
  }
  ioopm_linked_list_destroy(list);
  return true;
}

bool continue_printing(){
  // Prompt for a yes "y"/"Y" returning true (else false)
  char *ans = ask_question_string("Continue? y/n \n");

  if((strcmp(ans, "y") == 0) || (strcmp(ans, "Y") == 0))      
    return true;    
  else
    return false;
}


void list_merchandise(webstore_t *store){
  // List all merchendise as a short list of 20
  // prompting for displaying more 
  if (store == NULL){
    perror("list_merchendise: Unallowed NULL argument.\n");
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
	!continue_printing()) break;
    // Print current merch
    printf("Merch Item [No.%d]\n", i);
    print_merch(current);
    printf("End \n\n");

    if(ioopm_iterator_has_next(iter)){
      current = get_elem_ptr(ioopm_iterator_next(iter));      
    }else { break; }    
  }     
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list_merch);  
}

//show shelf and quantity FIX LATER
/*
void show_stock(webstore_t *store){
  ioopm_list_t *list_shelfs = ioopm_hash_table_keys(store->storage_db);
  //ioopm_list_iterator_t *iter = ioopm_list_iterator(list_shelfs);
  
  //shelf_t *current = NULL;
  //current = get_elem_ptr(ioopm_iterator_current(iter)); //lista över items 
  
  for(int i =0; i < ioopm_linked_list_size(list_shelfs); i++){
    ioopm_list_t *db_names = look_in_storage(store, shelf);
    shelf_t shelf_key = get_elem_ptr(ioopm_linked_list_get(list_shelfs, i));
    ioopm_list_t *list_names = get_elem_ptr(ioopm_hash_table_lookup(store->storage_db, ioopm_linked_list_get(list_shelfs, i)));
    int stock = ioopm_linked_list_size(list_names);
    char *shelf_name = shelf_key->shelf; 
    printf("Shelf [%s]:%d\n", shelf_name, stock);
    
  }
  ioopm_linked_list_destroy(list_shelfs);
}

*/
/// Shelf

shelf_t *create_shelf(char *shelf, int amount){
  // Allocate a shelf
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));

  new_shelf->shelf  = shelf;
  new_shelf->amount = amount;

  return new_shelf;
}

void shelf_delete(shelf_t *shelf){
  // Deallocate a shelf
  if (shelf == NULL){
    perror("shelf_delete: Unallowed NULL argument.\n");
  }
  free(shelf);
}

/// Store

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

  return new_webstore;
}

void store_destroy(webstore_t *store){
  // Deallocate the argument handler, both hash tables
  // and the shopping cart list. And the whole webstore.
  if (store == NULL){
    perror("store_destroy: Webstore is NULL.\n");
  }  
  destroy_arg_opt(store->opt);  
  ioopm_hash_table_destroy(store->merch_db);
  ioopm_hash_table_destroy(store->storage_db);
  ioopm_linked_list_destroy(store->all_shopping_carts);  
  free(store);
}


/// Shelf

void add_shelf(webstore_t *store, char *name, shelf_t *shelf){
  // Add a (must be new) shelf to the storage database containing a name

  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){ 
   perror("add_shelf: Cannot add a new shelf that exists.\n");
  }
  else if ((store == NULL) || (name == NULL) || (shelf == NULL)){
    perror("add_shelf: Unallowed NULL arguments.\n");
  }
  change_shelf(store, name, shelf->amount, shelf->shelf);
}

void display_storage(webstore_t *store, char *shelf){
  // Prettyprint the contents of a shelf
  if ((store == NULL) || (shelf == NULL)){
    perror("display_storage: Unallowed NULL arguments.\n");
  }  

  // Names stored at requested shelf location
  ioopm_list_t *db_names = look_in_storage(store, shelf);
  ioopm_link_t *db_item = db_names->first;
  // Print what shelf
  printf("Shelf[%s]:", shelf);
  // Print all items
  do {
    // Already exists in database
    printf(" %s", (char*)get_elem_ptr(db_item->element));
					      
    db_item = db_item->next;    
  } while (db_item != NULL);
  
  printf("\n");
}

// No need to test (used alot in other functions)
ioopm_list_t *look_in_storage(webstore_t *store, char *shelf){
  // Return the associated list to a shelf in storage
  if ((store == NULL) || (shelf == NULL)){
    perror("look_in_storage: Unallowed NULL arguments.\n");
  }  
  return get_elem_ptr(ioopm_hash_table_lookup(store->storage_db,
					      ptr_elem(shelf)));
}

void remove_storage_location(webstore_t *store, char *shelf){
  // Remove a storage shelf from the storage_db hash-table.
  
  if ((store == NULL) || (shelf == NULL)){
    perror("remove_storage_location: Unallowed NULL arguments.\n");
  }
  // Shelf does not exist in storage db  
  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    perror("remove_storage_location: Cannot remove, non-existing shelf.\n");
  } else {
    
    ioopm_list_t *storage_list = look_in_storage(store, shelf);    
    // Remove storage list
    ioopm_linked_list_destroy(storage_list);
    // Remove from hash table 
    ioopm_hash_table_remove(store->storage_db, ptr_elem(shelf));			   
  }  
}

void remove_all_storage_locations(webstore_t *store){
  // Remove all shelfs in storage_db, but not the hash-table.
  if (store == NULL){
    perror("remove_all_storage_locations: Webstore is NULL\n");
  }
  
 ioopm_list_t *shelfs  = ioopm_hash_table_keys(store->storage_db);
 ioopm_link_t *current = shelfs->first;

 do {
   remove_storage_location(store, get_elem_ptr(current->element));
   current = current->next;
 } while (current != NULL);

 ioopm_linked_list_destroy(shelfs); 
}


void add_to_storage(webstore_t *store, char *name, char *shelf){

  if ((store == NULL) || (name == NULL) || (shelf == NULL)){
    perror("add_to_storage: Unallowed NULL argument\n");
  }
  
  // Create a new storage list if shelf does not exist
  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    ioopm_list_t *storage_list = ioopm_linked_list_create();
    ioopm_linked_list_append(storage_list, ptr_elem(name));
    ioopm_hash_table_insert(store->storage_db,
			    ptr_elem(shelf),
			    ptr_elem(storage_list));    
  }
  
  // Names stored at requested shelf location
  ioopm_list_t *db_names = look_in_storage(store, shelf);
  ioopm_link_t *db_item = db_names->first;

  do {
    // Already exists in database
    if (STR_EQ(db_item->element.c, name)) return;
					      
    db_item = db_item->next;           
  } while (db_item != NULL);

  ioopm_linked_list_append(db_names, ptr_elem(name));
}

   
void change_shelf(webstore_t *store,
			 char *name,
			 int amount,
			 char* location){
  
  QLOG(store, "change_shelf", name);

  if(store->merch_db == NULL){
    perror("change_shelf: Uninitizalized Merch-Database,\
            the database has not been initialized.\n");
    return; // REMOVE THIS LATER
    
  }else if (!ioopm_hash_table_has_key(store->merch_db,
				      ptr_elem(name))){
    perror("change_shelf: 404 Merch Not Found, \
            item name not in the Merch Database.\n");
    return; // REMOVE THIS LATER
  }

  // Extract location database bound to name
  elem_t elem_data =
    ioopm_hash_table_lookup(store->merch_db,
			    ptr_elem(name));
  merch_t      *merch_data = get_elem_ptr(elem_data);  
  ioopm_link_t *merch_locs = merch_data->locs->first;
  
 
  if(merch_data->locs == NULL){ 
    perror("change_shelf: No Merch Database"); return;    
  }if(amount < 0){
    perror("change_shelf: Negative amount"); return;
  }

  QLOG(store, "change_shelf", name);
  
  if (merch_data->locs->size > 0){
    shelf_t *shelf_data = get_elem_ptr(merch_locs->element);
    do {

      if (STR_EQ(shelf_data->shelf, location)){
	// Found existing shelf, set new amount, and exit
	shelf_data->amount = amount;
	return;
      }     

      merch_locs = merch_locs->next;
           
    } while (merch_locs != NULL);

  }

  QLOG(store, "change_shelf", name);

  shelf_t *new_shelf = create_shelf(location, amount);  
  ioopm_linked_list_append(merch_data->locs,
			   ptr_elem(new_shelf));
    
  // FIX: Added destructors

  QLOG(store, "change_shelf", name);
  
      
}

bool storage_contains(webstore_t *store, char *shelf, char *name){
  if ((store == NULL) | (shelf == NULL) | (name == NULL)){
    perror("storage_contains: Unallowed NULL argument.\n");
  }
  
  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    perror("storage_contains: Unallowed non existing shelf.\n");
    return false;    
  }
  
  // Names stored at requested shelf location
  ioopm_list_t *db_names = look_in_storage(store, shelf);
  ioopm_link_t *db_item  = db_names->first;

  do {
    // Already exists in database
    if (STR_EQ(db_item->element.c, name))
      return true;
					      
    db_item = db_item->next;           
  } while (db_item != NULL);
  // Does not exist in database
  return false;
}

void global_change_shelf(webstore_t *store, char *name,
             char *shelf, size_t amount){
  // Add / Update shelf to both the merch database and the
  // storage database. If it already exists, update amount.  
  change_shelf(store, name, amount, shelf);

  // Add name to shelf if it already doesnt not contain it.
  if (!storage_contains(store, name, shelf))
    add_to_storage(store, name, shelf);  
}


ioopm_list_t *merch_locs(webstore_t *store, char *name){

  merch_t *merch_data         =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  ioopm_list_t *merch_locs    = merch_data->locs;
  return merch_locs;
}

void list_shelfs(webstore_t *store, char *name){
  merch_t *merch_data         =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  ioopm_list_t *merch_locs    = merch_data->locs;
  ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_locs);

  shelf_t *shelf = get_elem_ptr(ioopm_iterator_current(iter));

  printf("Shelfs containing'%s':\n", name);
  
  for (int i = 1;; i++){         

    
    printf("Shelf %s : %ld\n",
	   shelf->shelf,
	   shelf->amount);
      
    if(ioopm_iterator_has_next(iter)){
        
      shelf   = get_elem_ptr(ioopm_iterator_next(iter));

    }else { break; }

    
  }
     
  // FIX: Added destructors
  ioopm_iterator_destroy(iter);

  
}
int merch_locs_total(webstore_t *store, char *name){
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  

  ioopm_link_t *merch_locs = merch_data->locs->first;  
  int amount = 0;
  
  shelf_t *shelf_data;
  while (merch_locs != NULL) {

    shelf_data = (get_elem_ptr(merch_locs->element));
    amount    += shelf_data->amount;
    merch_locs = merch_locs->next;
           
  }

  return amount;
}
int merch_locs_at_shelf(webstore_t *store, char *name, char *shelf){
  // Return the amount merchendise at a specific shelf
  
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  

  ioopm_link_t *merch_locs = merch_data->locs->first;
  
  int amount = 0;
  
  shelf_t *shelf_data;
  while (merch_locs != NULL) {

    shelf_data = (get_elem_ptr(merch_locs->element));

    if (STR_EQ(shelf_data->shelf, shelf)) 
      amount    += shelf_data->amount;
    
    merch_locs = merch_locs->next;
           
  }

  return amount;
}


void locs_delete(webstore_t *store, char *name){ 
  // Free up a merch locs list (list & shelfs)

  if ((store == NULL) || (name == NULL)){
    perror("locs_delete: Unallowed NULL arguments.\n");
  }
  
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  
  ioopm_link_t *merch_locs = merch_data->locs->first;  

  // Free each shelf in the merch locs
  while (merch_locs != NULL) {
    free(get_elem_ptr(merch_locs->element));
    merch_locs = merch_locs->next;;           
  }
  // Free the locs list
  ioopm_linked_list_destroy(merch_data->locs);
}

/// Other


// list_merchandise(webstore_t *store);
// list all items in store
// TODO REQ: Alphabetical order (soft req) - qsort
// TODO REQ: print 20 at time
//TODO REQ: ask if more
// list of all ptr of merch

