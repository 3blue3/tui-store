#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "hash_table.h"
#include "iterator.h"
#include "list_linked.h"
#include "common.h"
#include "utils.h"
#include "webstore.h"
#include "cart.h"

#define get_elem_ptr(e) e.p
#define get_elem_int(e) e.i
#define get_elem_str(e) e.c

#define CURRENT_ITER_STR(iter) \
  get_elem_str(ioopm_iterator_current(iter))

#define CURRENT_ITER_INT(iter) \
  get_elem_int(ioopm_iterator_current(iter))

#define CURRENT_ITER_PTR(iter) \
  get_elem_ptr(ioopm_iterator_current(iter))

#define ITER_HAS_NXT(iter) \
  ioopm_iterator_has_next(iter)

#define ITER_NXT(iter) \
  ioopm_iterator_next(iter)

#define ITER_INIT(iter)	\
  ioopm_list_iterator(iter)
#define ITER_DEST(iter)	\
  ioopm_iterator_destroy(iter)

#define LOG_CART(str)  \
    if (store->opt->log_p) \
      printf("LOG[cart = %d]: %s\n", (int)store->active_cart, str)

#define ID_BOUNDS_MAX 100000
#define ID_BOUNDS_MIN 0
///
/*HELP FUNCTIONS*/
///

bool is_money(int size){
  return (size < MAX_ALLOWED_PRICE) && (size > MIN_ALLOWED_PRICE);
}

void display_header(char *str, bool sectionp){
  int len = strlen(str);

  //  Top boarder
  printf("     ");
  for (int i = 0; i<len + 4;i++) printf("_");
  printf("     \n");

  // Label
  if (sectionp) printf("┃"); 
  else printf(" ");

  printf(" -  ┃ ");
  printf(str);
  printf(" ┃  - \n");
  
  // Bottom border


  if (sectionp) printf("┃"); 
  else printf(" ");

  printf("    '");
  for (int i = 0; i<len + 2;i++) printf("¨");
  printf("'     \n");
}


bool cart_id_exists(webstore_t *store, int id){
  ioopm_link_t *current = (store->all_shopping_carts)->first; 

  while (current != NULL) {
    cart_t *cart = get_elem_ptr(current->element);
    if (!cart){
      if (store->opt->log_p)
	printf("┃ Lookup for cart id: %d returned non-existing!\n",
	       id);
      break;
    }
  
  
    else if (cart->id == id) return true;

    current = current->next;
  } 

  return false;
}

bool valid_id(webstore_t *store, int id){
  // Valid Cart Id
  
  if ((ioopm_linked_list_size(store->all_shopping_carts) == 0) &&
	    (id == 0)){
    // Zero is always a valid cart id when no
    // previous carts exist.
    return true;
  }

  else if (id == store->active_cart){
    // The active cart is always valid
    return true;
  }    
  
  else if (id < ID_BOUNDS_MIN){
    perror("valid_id: Id is under 0.\n");
    return false;
  }
  else if (id >= ID_BOUNDS_MAX){
    perror("valid_id: Id is over the max-bounds.\n");
    return false;
  }
  
  return cart_id_exists(store, id);  
}

cart_t *get_cart(webstore_t *store, int id){
  // Return a pointer to the cart belonging to id
  
  if (!store->all_shopping_carts){
    perror("get_cart: Cart database is deallocated.\n");
    return NULL;
  }
  
  ioopm_link_t *current = (store->all_shopping_carts)->first; 
 
  while (current != NULL) {
    cart_t *cart = get_elem_ptr(current->element);
    if (!cart)              break;
    else if (cart->id == id) return cart;

    current = current->next;
  } 

  //  perror("get_cart: Non-existing cart.\n"); 
  return NULL;
}

int amount_of_merch_in_cart(cart_t *cart, char *merch_name){
  if (!cart){
    perror("amount_of_merch_in_cart: No active cart.\n");
    return 0;
  }
    // Return the amount of stock a merch has in cart
    if(ioopm_hash_table_has_key(cart->merch_in_cart, str_elem(merch_name)))
        return get_elem_int(ioopm_hash_table_lookup(cart->merch_in_cart,
						    str_elem(merch_name)));
    else return 0;
}


static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(* (char * const *) p1, * (char * const *) p2);
}

void sort_keys(entry_ht_t *keys, size_t no_keys)
{
  qsort(keys, no_keys, sizeof(entry_ht_t), cmpstringp);
}


bool cart_is_empty(cart_t *cart){
  if (!cart){
    perror("add_to_cart: No active cart.\n");
    return true;
  }

    return ioopm_hash_table_is_empty(cart->merch_in_cart);
}

//size_t cart_db_size(cart_t *cart){
size_t cart_db_size(cart_t *cart){
  if (!cart){
    perror("cart_db_size: No active cart.\n");
    return -1;
       
  }
  return ioopm_hash_table_size(cart->merch_in_cart);
}

void list_all_cart_id(webstore_t *store){

  if (!store->all_shopping_carts){
    // perror("list_all_cart_id: Cart database is deallocated.\n");
    return;
  }

  ioopm_link_t *current = (store->all_shopping_carts)->first; 
  if (current == NULL){
    printf("┃ > No carts exists\n\n");
    return;
  }


  display_header("All Carts", true);
  
  while (current != NULL) {
    
    cart_t *cart = get_elem_ptr(current->element);
    if (!cart){
      perror("list_all_cart_id: Cart database contains unallocated cart\nexiting...");
      exit(1);
    }

    printf("┃ > Cart %d\n", cart->id);
    current = current->next;
  }

  puts("");
}

///
/*FUNCTIONS*/
///

cart_t *append_cart(webstore_t *store){
  if (!store->all_shopping_carts){
    LOG_CART("Cart DB, was deallocated, creating a new one, current ID ==>");
    store->all_shopping_carts = ioopm_linked_list_create();
  }


  
  // Create and add a new cart
  LOG_CART("Appending a cart");
  cart_t *cart = create_cart(store);
  
  LOG_CART("Appended a cart");  
  LOG_STR_INT("Returned Id ==> ", cart->id);

  assert(cart->id == store->active_cart);


//    if (store->all_shopping_carts->size < 0){
//      LOG_CART("Cart list size was not updated");
//      perror("Size of cart list is under one.");
//      exit(1);
//    }


    if (store->all_shopping_carts)
      if (store->all_shopping_carts->size > 1){
	ioopm_linked_list_prepend(store->all_shopping_carts, ptr_elem(cart));
	LOG_CART("Prepended new cart");
      }else{
	ioopm_linked_list_append(store->all_shopping_carts, ptr_elem(cart));
	LOG_CART("Appended new cart");
      }
    
    else {
      LOG_CART("No shopping carts exist");
      perror("No shopping carts exist, deallocated cart db.");
      exit(1);
    }

    assert(cart_exists(store)); // Correctly added

    
    LOG_STR_INT("Cart list size after adding ==>", store->all_shopping_carts->size);  

    return cart;
}


cart_t *create_cart(webstore_t *store){
  // Create a cart, and return it
  

  cart_t *new_cart = calloc(1, sizeof(cart_t));
    
  //Hash Table containing all merch in cart with string key and int elem 
    new_cart->merch_in_cart =
      ioopm_hash_table_create(string_knr_hash,
			      eq_elem_string,
			      eq_elem_int); 
    
    // Set id of cart to the total amount of existing carts
    LOG_CART("create_cart: Creating a new cart...\n");



    new_cart->id = get_largest_cart_id(store) + 1;
    
    LOG_STR_INT("Found New Cart ID ==> ", new_cart->id);
    
    // Mark the new cart as active
    store->active_cart = new_cart->id;
  
    SLOG(store, "create_cart: Created a new cart...\n");
    // Logging setup
    if (store->opt->log_p)
      printf("A new cart with Id %d has been created.\n",
	     new_cart->id);


  return new_cart;
}
int get_largest_cart_id(webstore_t *store) {
  //// This will find the largest used cart id
  //  ==>  (largest id + 1 always is a safe id) 
    int largest_id = 0;
    ioopm_link_t *current = store->all_shopping_carts->first;
    cart_t *current_cart = NULL;

    LOG_STR("Finding the largest Cart ID");
    while (current){
      current_cart = get_elem_ptr(current->element);
      LOG_STR_INT("Cart ID ==>", (int)current_cart->id);
      if (current_cart->id > largest_id)
	largest_id = current_cart->id;
      
    current = current->next;
  }
    return largest_id;
}

void remove_cart(webstore_t *store){
  // Remove the active cart
  LOG_CART("Remove Cart");
  
  if (!store->all_shopping_carts){
    perror("remove_cart: Cart database is deallocated.\n");
    return;
  }else if(!cart_id_exists(store, store->active_cart)){
    perror("remove_cart: Non existing active cart.\n");
    return; 
  }


  if (!ioopm_linked_list_is_empty(store->all_shopping_carts)){
    // Get the active cart
    cart_t *cart = get_cart(store, store->active_cart);    

    if(!cart){
      perror("remove_cart: There is no cart to remove.\n");
      return;
    }


  ioopm_link_t *prev    = (store->all_shopping_carts)->first; 
  ioopm_link_t *current = (store->all_shopping_carts)->first; 
 
  while (current != NULL) {
    cart_t *iter_cart = get_elem_ptr(current->element);
    if (iter_cart->id == cart->id) {

      if (prev == current){
	store->all_shopping_carts->first = current->next;
	store->all_shopping_carts->size--;
	free(current);
	
      } else {

	prev->next = current->next;
	store->all_shopping_carts->size--;
	free(current);
      }
      	break;
    }
    prev = current;
    current = current->next;
  } 

  cart_destroy(cart);
  cart = NULL;
  }
  store->active_cart = get_largest_cart_id(store);
  LOG_CART("A new cart id has been set.");
}


void add_to_cart(webstore_t *store, char *name, int amount){ // Needs to save sting before ths 
  
  if (!store->all_shopping_carts){
    LOG_STR("add_to_cart: Cart database is deallocated.\n");
    return;
  }
  else if(amount <= 0){
    printf("| You do not add anything...\n");
    return;
  }


  // Should never get here
  else if(!valid_id(store, store->active_cart)){ 
    perror("add_to_cart: Invalid cart ID.\n");
    assert(!valid_id(store, store->active_cart));
    exit(1);
  }


  // Should never get here
  else if(!cart_id_exists(store, store->active_cart)){ 
    perror("add_to_cart: No active cart exists.\n");
    assert(!cart_id_exists(store, store->active_cart));
    exit(1); 
  }


  int total_stock      = merch_stock(store, name);
  cart_t *current_cart = get_cart(store, store->active_cart);
  int current          =
    get_elem_int(ioopm_hash_table_lookup(current_cart->merch_in_cart,
					 str_elem(name)));

  
  // If current_cart is NULL, there exists no active cart
  if (!current_cart){
    perror("add_to_cart: No active cart exists.\n");
    assert(!cart_id_exists(store, store->active_cart));
    return;
    
  }else if (current_cart->id != store->active_cart){
    perror("add_to_cart: Invalid ID lookup.\n");
    assert(cart_id_exists(store, current_cart->id));
    assert(current_cart->merch_in_cart != NULL);
    //    assert(ioopm_hash_table_size(current_cart->merch_in_cart) !=
    exit(1);     
  }


  LOG_STR_INT("Amount to add  ==>", amount);
  LOG_STR_INT("Amount in cart ==>", current);
  LOG_STR_INT("Maximum Stock  ==>", total_stock);

  // If the asked amount and the amount
  // already in the cart exceeds the amount of merch in the store
  if (total_stock == (current + amount)){
    printf("| You have now added all of %s to cart.\n", name);
  ioopm_hash_table_insert(current_cart->merch_in_cart, str_elem(name), int_elem(total_stock));

  }
  else if (total_stock < (current + amount)){
    printf("| %s has %d left in stock\n", name, (total_stock - current));
    printf("| The store does not have %dst in stock\n", amount);
    return;
  }else {

  printf("| %s: %dst +%dst\n\n", name, amount, current);
  ioopm_hash_table_insert(current_cart->merch_in_cart, str_elem(name), int_elem(current + amount));
  
  }
}

void cart_destroy(cart_t *cart){
  // Deallocate one cart
  if (!cart){
    perror("merch_in_cart: cart is NULL.\n");
    assert(cart);
  
}
  
  ioopm_hash_table_destroy(cart->merch_in_cart);
  cart->merch_in_cart = NULL;
  cart->id            = 0;

  free(cart);   
}

void destroy_all_carts(webstore_t *store){
  if(!store->all_shopping_carts) return;
    
  LOG_STR_INT("Active Cart id", store->active_cart);
  LOG_STR_INT("Amount of carts", store->all_shopping_carts->size);

  ioopm_link_t *current = (store->all_shopping_carts)->first;
  cart_t *current_cart  = NULL;
  
  while (current) {
    current_cart = get_elem_ptr(current->element);

    cart_destroy(current_cart);
    current->element = ptr_elem(NULL);
    current = current->next;    
  } 

  ioopm_linked_list_destroy(store->all_shopping_carts);  
  //free(store->all_shopping_carts);
  store->all_shopping_carts = NULL;
}

void remove_from_cart(webstore_t *store, int id, char *merch_name,
		      int amount_to_remove){

  if (!store->all_shopping_carts){
    perror("remove_from_cart: Cart database is deallocated.\n");
    return;
  }
  else if (!valid_id(store,id)){
    perror("remove_from_cart: The id of the cart is invalid.\n");
    return; 
  }else if(!cart_id_exists(store,id)){
    perror("remove_from_cart: No active cart exists.\n");
    return;
  }
  
  cart_t *current_cart = get_cart(store, id);

  if (!current_cart){
    perror("remove_from_cart: No active cart.\n");
    return;
  }    
  else if(!merch_in_cart(current_cart, merch_name)){
    perror("remove_from_cart: There is no such merch in the cart.\n");
    return; 
  }  
    
  int amount_of_merch = amount_of_merch_in_cart(current_cart,
						merch_name);
    
  // Cannot remove more merch than currently in cart
  if(amount_of_merch < amount_to_remove){
    perror("amount_of_merch: Cannot remove more stock than in cart.\n");
    perror("amount_of_merch: Removing all merch in cart.\n");
  }


  // First remove current merch entry
  ioopm_hash_table_remove(current_cart->merch_in_cart,
			  str_elem(merch_name));

  // Reinsert it with new amount if it is >0
  if (amount_of_merch - amount_to_remove > 0)
    ioopm_hash_table_insert(current_cart->merch_in_cart,
			    str_elem(merch_name),
			    int_elem(amount_of_merch - amount_to_remove)); 
}


int active_cart_cost(webstore_t *store){
  if (!store->all_shopping_carts){
    perror("active_cart_cost: Cart database is deallocated.\n");
    return 0;
 }

  return calculate_cost(store, store->active_cart);
}

int calculate_cost(webstore_t *store, int id){
  if (!store->all_shopping_carts){
    perror("calculate_cost: Cart database is deallocated.\n");
    return 0;
  }
  else if(!valid_id(store,id)){
    perror("calculate_cost: The cart id is invalid.\n");
    return 0; 
  }else if(!cart_id_exists(store,id)){
    perror("calculate_cost: The cart id is invalid.\n");
    return 0; 
  }
    
  cart_t *current_cart = get_cart(store, id);
  if (!current_cart){
    perror("calculate_cost: No active cart.\n");
    return 0;
  }
  // Individual Merch Price
  int price ;
  // Stock amount
  int current_amount;

  int total_price      = 0;
    
  //if the cart is empty
  if(cart_is_empty(current_cart)){
    perror("calculate_cost: Cart contains nothing.\n"); 
    return total_price;
  }
    
  ioopm_list_t *names =
    ioopm_hash_table_keys(current_cart->merch_in_cart);
  ioopm_list_t *amounts =
    ioopm_hash_table_values(current_cart->merch_in_cart);
    
    
  if(cart_db_size(current_cart) == 1){
    price = merch_price(store, get_elem_str(ioopm_linked_list_get(names, 0)));
    current_amount = get_elem_int(ioopm_linked_list_get(amounts, 0));
    total_price = price * current_amount; 
  }
    
  else{

    size_t no_names = ioopm_linked_list_size(names);
        
    ioopm_list_iterator_t *iter_n = ITER_INIT(names); 
    ioopm_list_iterator_t *iter_a = ITER_INIT(amounts);
        
    //checks the price for each merch in the cart
    //and multiplies it with the amount
    for (size_t i = 0; i < no_names; i++) {      
      char *current_name;
      // Sum up all (of merch in cart)
      // prices * amount 
      // for the final price
      current_name   = CURRENT_ITER_STR(iter_n);
      current_amount = CURRENT_ITER_INT(iter_a);
      price = merch_price(store, current_name);
      total_price += (price*current_amount);

      // Continue iterating through merch in cart 
      if(ITER_HAS_NXT(iter_n) && ITER_HAS_NXT(iter_a)){
	ITER_NXT(iter_n); ITER_NXT(iter_a);
      }

    }
        
    ITER_DEST(iter_n);
    ITER_DEST(iter_a);
  }
    
  ioopm_linked_list_destroy(names);
  ioopm_linked_list_destroy(amounts);

  return total_price;
}


void checkout(webstore_t *store){
  int id = store->active_cart;
  cart_t *current_cart = get_cart(store, id);

  if (!current_cart){
    printf( "┃ > There are no carts in the store,\n");
    printf("     create one by pressing 'n'.\n\n");
    return;

  } else{
    assert(current_cart->merch_in_cart);    
  }
		

  if (ioopm_hash_table_is_empty(current_cart->merch_in_cart)){
    printf( "┃ > You don't have any items add items\n");
    printf( "    by pressing 'e', followed by 'a'.\n\n");
    return;
}
  int total = calculate_cost(store, store->active_cart);
  
  char *current_name = NULL;
  int current_amount = 0;
  ioopm_list_t *names = ioopm_hash_table_keys(current_cart->merch_in_cart);

  
  if (cart_db_size(current_cart) == 1){
    current_name = get_elem_str(ioopm_linked_list_get(names, 0));
    current_amount = amount_of_merch_in_cart(current_cart, current_name);
    //    change_stock_in_webstore(store, current_name, current_amount);
    decrease_equal_stock(store, current_name, current_amount);
    
    printf("┃ Buying %s (%dst)x(%dkr)\n",
	   current_name,
	   current_amount,
	   merch_price(store, current_name));
	  
  }
    
  else{
        
    size_t no_names = ioopm_linked_list_size(names);
        
    ioopm_list_iterator_t *iter_n = ioopm_list_iterator(names); 

    for (int i = 0; i < no_names; i++) {
      current_name = get_elem_str(ioopm_iterator_current(iter_n));
      current_amount = amount_of_merch_in_cart(current_cart, current_name);
      decrease_equal_stock(store, current_name, current_amount);
      //      change_stock_in_webstore(store, current_name, current_amount); 

      printf("┃ Buying %s (%dst)x(%dkr)\n",
	     current_name,
	     current_amount,
	     merch_price(store, current_name));

      if(ioopm_iterator_has_next(iter_n)){
	ioopm_iterator_next(iter_n); 
      }
    }
        
    ioopm_iterator_destroy(iter_n);
  }
  ioopm_linked_list_destroy(names);
  remove_cart(store);
  printf("┃ Total price: %d\n", total);

}

bool cart_exists(webstore_t *store){
  int id = store->active_cart;

  cart_t *current_cart = get_cart(store, id);
  if (!current_cart){
    return false;
  }else 
    return true;
}


/*  void checkout(webstore_t *store){
    ioopm_linked_list_destroy(names);
    remove_cart(store, current_cart->id); 
    }
*/
/*
  void checkout(webstore_t *store){
  cart_t *cart = get_cart(store, store->active_cart);


  ioopm_list_t *names   =
  ioopm_hash_table_keys(cart->merch_in_cart);

  ioopm_list_t *amounts =
  ioopm_hash_table_values(cart->merch_in_cart);

  ioopm_list_iterator_t *iter_n = ITER_INIT(names); 
  ioopm_list_iterator_t *iter_a = ITER_INIT(amounts);

  // Transfer all stock amounts & merch names
  // into an array for sorting    
  for (size_t i = 0; i < ioopm_linked_list_size(names); i++) {
  size_t current_amount = CURRENT_ITER_INT(iter_a);
  char *current_name    = CURRENT_ITER_STR(iter_n);
      
  // Create value for array
  change_stock_relative_amount(store, current_name, -current_amount);
  // Iterate through all merch names and
  // stock amounts
  if(ITER_HAS_NXT(iter_n) && ITER_HAS_NXT(iter_a)){
  ITER_NXT(iter_n); ITER_NXT(iter_a);
  }
  }
    
    
  ioopm_linked_list_destroy(names);
  ioopm_linked_list_destroy(amounts);
  ITER_DEST(iter_n);
  ITER_DEST(iter_a);
  
  }*/

void display_cart(cart_t *cart){ //id?
  

  if (!cart){
    puts("┃ Non existing cart ID, create a new one.");
    //    perror("display_cart: Cart is deallocated.\n");
    return;
  }
  printf("     ______________________   \n");
  printf("┃    ┃ Displaying Cart %d3 ┃   \n", (int)cart->id);
  printf("     '¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨'   \n");
  // If cart is empty
  if(cart_is_empty(cart)){
    printf("┃ > Cart %d is Empty.\n\n",
	   cart->id);    
    return;
  }

  
  ioopm_list_t *names   =
    ioopm_hash_table_keys(cart->merch_in_cart);

  ioopm_list_t *amounts =
    ioopm_hash_table_values(cart->merch_in_cart);

  size_t no_names               =
    ioopm_linked_list_size(names);     

  ioopm_list_iterator_t *iter_n = ITER_INIT(names); 
  ioopm_list_iterator_t *iter_a = ITER_INIT(amounts);
    
  entry_ht_t kv_array[no_names];
    
  // Transfer all stock amounts & merch names
  // into an array for sorting    
    for (size_t i = 0; i < no_names; i++) {
      int current_amount = CURRENT_ITER_INT(iter_a);;
      char *current_name = CURRENT_ITER_STR(iter_n);
      
      // Create value for array
      entry_ht_t keyval = (entry_ht_t) {
	.key   = current_name,
	.value = current_amount
      };
	
      kv_array[i] = keyval;
      // Iterate through all merch names and
      // stock amounts
      if(ITER_HAS_NXT(iter_n) && ITER_HAS_NXT(iter_a)){
	ITER_NXT(iter_n); ITER_NXT(iter_a);
      }
    }
    
    sort_keys(kv_array, no_names);
    
    for (size_t i = 0; i < no_names; ++i) {
      printf("┏────╸Item %d\n",(int)i+1);
      printf("┃ Name:   %s\n",   kv_array[i].key);
      printf("┃ Amount: %d\n", kv_array[i].value);
    }
    
    ioopm_linked_list_destroy(names);
    ioopm_linked_list_destroy(amounts);
    ITER_DEST(iter_n);
    ITER_DEST(iter_a);
        
}

char *get_merch_name_in_cart(cart_t *cart, int nr_merch){
  // Return the merch item associated with nr_merch

  if (!cart){
    perror("get_merch_name_in_cart: No active cart.\n");
    return NULL;
  }


  ioopm_list_t *names = ioopm_hash_table_keys(cart->merch_in_cart);
  ioopm_list_t *amounts = ioopm_hash_table_values(cart->merch_in_cart);
    
  size_t no_names = ioopm_linked_list_size(names);
      
  ioopm_list_iterator_t *iter_n = ITER_INIT(names); 
  ioopm_list_iterator_t *iter_a = ITER_INIT(amounts);
    
  entry_ht_t kv_array[no_names];
    
  elem_t current_name;
  elem_t current_amount;
    
  for (int i = 0; i < no_names; i++) {
    current_name = ioopm_iterator_current(iter_n);
    current_amount = ioopm_iterator_current(iter_a);
    entry_ht_t keyval = (entry_ht_t) {.key = (char *) current_name.p, .value = current_amount.i};
    kv_array[i] = keyval;
    if(ITER_HAS_NXT(iter_n) && ITER_HAS_NXT(iter_a)){
      ITER_NXT(iter_n); 
      ITER_NXT(iter_a);
    }
  }
    
  sort_keys(kv_array, no_names);
    
  for (int i = 0; i < no_names; ++i) {
    if((i+1) == nr_merch){
      return kv_array[i].key;
    }
  }
  perror("get_merch_name_in_cart: Merch not found.\n");
  return "";
}

size_t nr_of_merch_in_cart(cart_t *cart){
  if (!cart){
    perror("get_amount_of_merch_in_cart: No active cart.\n");
    return 0;
  }

  return ioopm_hash_table_size(cart->merch_in_cart);
}

int get_amount_of_merch_in_cart(cart_t *cart, char *merch_name){
  if (!cart){
    perror("get_amount_of_merch_in_cart: No active cart.\n");
    return 0;
  }

    
  return get_elem_int(ioopm_hash_table_lookup(cart->merch_in_cart, str_elem(merch_name))); 
}

bool merch_in_cart(cart_t *cart, char *merch_name){
  if (!cart ){
    perror("merch_in_cart: No active cart.\n");
    return false;
  }

  return ioopm_hash_table_has_key(cart->merch_in_cart, str_elem(merch_name));
}

////
/* PROMPT FUNCTIONS */
///

void add_to_active_cart_prompt(webstore_t *store){
  add_to_cart_prompt(store, store->active_cart);
}

void add_to_cart_prompt(webstore_t *store, int cart_id){
  int merch_id = 0;
  
  list_merchandise(store); 

  // Set a correct amount, if incorrect return

  printf("┏────╸ Select Merchandise\n");
  do {
    merch_id  = ask_question_int("┃ Number: ");    
  }while (!valid_index(store, merch_id));

  char *merch_name        = lookup_merch_name(store, merch_id-1);

  printf("┏────╸ Adding %s [Cart %d]\n",
	 merch_name, (int)cart_id);
  
  size_t merch_amount;
  
  // Set a correct amount, if incorrect return
  do {
    merch_amount = ask_question_int("┃ Amount: ");
    
  }while ((MAX_ALLOWED_STOCK < merch_amount) ||
	  (MIN_ALLOWED_STOCK > merch_amount));
  
  add_to_cart(store, merch_name, merch_amount);
}

void remove_from_cart_prompt(webstore_t *store){

  if (!store->all_shopping_carts){
    perror("Remove_from_cart: Cart database is deallocated.\n");
    return;
  }

  int id = store->active_cart;
  
  int merch_amount = 0;
  int merch_id     = 0;

  printf("┏─╸Remove From Cart Nr.%d\n",
	 (int)store->active_cart);
  
  if(cart_is_empty(get_cart(store,id))){
    puts("┃ The cart is empty! There is nothing to remove!"); 
    return;
    
  }else{
    // Display cart if answer is yes 
    if (choice_prompt("Display Cart?"))
      display_cart(get_cart(store,id));

    printf("┏────╸ Remove Item\n");
    do {
      merch_id = ask_question_int("┃ Id: ");
    }while (!valid_index(store, merch_id));
  
    // Lookup the merch id
    char *merch_name = get_merch_name_in_cart(get_cart(store,id), merch_id);

    // Get total amount of merch in storage
    int current_merch_stock = (int)merch_stock(store, merch_name);

    // Re-ask until one is pleased return on error
    do {
      merch_amount = ask_question_int("┃ Amount: ");
      
    } while (!(merch_amount <= current_merch_stock) &&
	     (MIN_ALLOWED_STOCK < merch_amount) &&
	     (MAX_ALLOWED_STOCK > merch_amount));
	     

    remove_from_cart(store, id, merch_name, merch_amount); 
  }
}

void remove_from_active_cart_prompt(webstore_t *store){
  remove_from_cart_prompt(store);
}

void remove_cart_prompt(webstore_t *store){

  if (!store->all_shopping_carts){
    perror("remove_cart_prompt: Cart database is deallocated.\n");
    return;
  }

  if ((!cart_id_exists(store, store->active_cart))){
      printf("┃ No cart exists.\n");
      return;
  }
  
  int id = store->active_cart;

  printf("┏─╸Remove Active Cart?\n┃ Id:%d\n", id);
  
  if (choice_prompt("Remove Cart?")){
    remove_cart(store);
  }
  
  printf("┃ Cart Id %d has been removed.\n", id);    
}



























