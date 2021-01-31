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
#include "cart2.h"

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


#define ID_BOUNDS_MAX 100000
#define ID_BOUNDS_MIN 0
///
/*HELP FUNCTIONS*/
///

bool is_money(int size){
  return (size < MAX_ALLOWED_PRICE) && (size > MIN_ALLOWED_PRICE);
}
bool is_merch(webstore_t *store, int id){
  return valid_index(store, id);
}

bool valid_id(webstore_t *store, int id){
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
  ioopm_link_t *current = (store->all_shopping_carts)->first; 

  while (current != NULL) {
    cart_t *cart = get_elem_ptr(current->element);
    if (!cart)          break;
    else if (cart->id == id) return true;

    current = current->next;
  } 

  return false;  
}

cart_t *get_cart(webstore_t *store, int id){
  if (!store->all_shopping_carts){
    perror("get_cart: Cart database is deallocated.\n");
    return NULL;
  }
  
  ioopm_link_t *current = (store->all_shopping_carts)->first; 
 
  while (current != NULL) {
    cart_t *cart = get_elem_ptr(current->element);
    if (!cart)             break;
    else if (cart->id == id) return cart;

    current = current->next;
  } 
  
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
    return false;
  }
  return ioopm_hash_table_size(cart->merch_in_cart);
}

void list_all_cart_id(webstore_t *store){

  if (!store->all_shopping_carts){
    perror("list_all_cart_id: Cart database is deallocated.\n");
    return;
  }

  puts("┏──╸ Existing Cart ID's ");
  ioopm_link_t *current = (store->all_shopping_carts)->first; 

  while (current != NULL) {
    
    cart_t *cart = get_elem_ptr(current->element);
    if (!cart){
      perror("list_all_cart_id: Cart database contains unallocated cart");
      return;
    }
    printf("┃ Cart Id.%d\n", cart->id);
    current = current->next;
  } 
}

///
/*FUNCTIONS*/
///

cart_t *create_cart(webstore_t *store){
  if (!store->all_shopping_carts){
    perror("get_cart: Cart database is deallocated.\n");
    return NULL;
  }

    cart_t *new_cart = calloc(1, sizeof(cart_t));
    
    //ht containing all merch in cart with string key and int elem 
    new_cart->merch_in_cart =
      ioopm_hash_table_create(string_knr_hash,
			      eq_elem_string,
			      eq_elem_int); 
    
    // Set id of cart to the total existing amount

    new_cart->id = ioopm_linked_list_size(store->all_shopping_carts);

    if(!valid_id(store, new_cart->id)){
      perror("create_cart: Automatic choice of cart ID failed.\n");
      // Deallocate allocated structs
      ioopm_hash_table_destroy(new_cart->merch_in_cart);
      free(new_cart);
      return NULL;
    }
    
    // Set the new cart active
    store->active_cart = new_cart->id;
    
    //Add cart to the list of all shopping carts
    //if its not the first time the cart is added, add!

      printf("┃ A new cart with Id.%d has been created!\n",
	     new_cart->id);
      ioopm_linked_list_append(store->all_shopping_carts,
			       ptr_elem(new_cart));
    return new_cart;
}

void remove_cart(webstore_t *store){

  if (!store->all_shopping_carts){
    perror("remove_cart: Cart database is deallocated.\n");
    return;
  }
    cart_t *cart = get_cart(store, store->active_cart);
    
    if(!cart){
      ioopm_hash_table_destroy(cart->merch_in_cart);    
      perror("remove_cart: There is no cart to remove.\n");
      return;
    }
    if (!ioopm_linked_list_is_empty(store->all_shopping_carts))
      ioopm_linked_list_remove(store->all_shopping_carts, store->active_cart);
    
    free(cart);
}


void add_to_cart(webstore_t *store, char *name, int amount){
  if (!store->all_shopping_carts){
    perror("add_to_cart: Cart database is deallocated.\n");
    return;
  }
  else if(amount <= 0){
    perror("add_to_cart: Cannot add under 0 merch.\n");
    return;
  }
    
  else if(!valid_id(store, store->active_cart)){
    perror("add_to_cart: Invalid cart ID.\n");
    return; 
  }

  int total_stock      = merch_stock(store, name);
  cart_t *current_cart = get_cart(store, store->active_cart);

  if (!current_cart){
    perror("add_to_cart: No active cart.\n");
    return;
  }
  
  int current          =
    get_elem_int(ioopm_hash_table_lookup(current_cart->merch_in_cart,
					 str_elem(name)));
			     
    
  // If the asked amount and the amount
  // already in the cart exceeds the amount of merch in the store
  if (total_stock < (current + amount)){
    perror("add_to_cart: Not enough in stock.\n");
    return;
  }
    
    
  // Avoid duplicate merch in db, update existing entry
  if (!ioopm_hash_table_has_key(current_cart->merch_in_cart,
				ptr_elem(name))){
    int existing_amount =
      get_elem_int(ioopm_hash_table_lookup(current_cart->merch_in_cart,
					   str_elem(name)));
	
    ioopm_hash_table_insert(current_cart->merch_in_cart,
			    str_elem(name),
			    int_elem(existing_amount + amount));

  } else {
    // This should only happen if the current_stock == 0 	  
    ioopm_hash_table_insert(current_cart->merch_in_cart,
			    str_elem(name), int_elem(amount));
  }
}


/*
  void add_to_cart(webstore_t *store, int id, char *merch_to_add_name, int amount){
    
  if(amount <= 0){
  perror("ADD TO CART: The amount of merch added to the cart must be 0 or higher.\n");
  return;
    }
    
    if(!valid_id(store,id)){
        perror("ADD TO CART: The id of the cart is invalid.\n");
        return; 
    }
    
    cart_t *current_cart = get_cart(store, id);
    
    int amount_of_merch_in_cart_already =  amount_of_merch_in_cart(current_cart, merch_to_add_name);
    int amount_of_merch_in_store = merch_stock(store, merch_to_add_name);//the amount of this merch in the store
    
    //If the asked amount and the amount already in the cart exceeds the amount of merch in the store
    if(amount_of_merch_in_store-(amount_of_merch_in_cart_already+amount) < 0){
        perror("ADD TO CART: Try to get more merch than there is in the store.\n");
        return;
    }
    
    //add merch to cart
    if(amount_of_merch_in_cart_already > 0){
        ioopm_hash_table_insert(current_cart->merch_in_cart, str_elem(merch_to_add_name), int_elem(amount_of_merch_in_cart_already + amount));
    }
    else{
        ioopm_hash_table_insert(current_cart->merch_in_cart, str_elem(merch_to_add_name), int_elem(amount));
    }
}*/

void cart_destroy(cart_t *cart){
  if (!cart){
    perror("merch_in_cart: No active cart.\n");
    return;
  }
  ioopm_hash_table_destroy(cart->merch_in_cart);
  free(cart);   
  cart = NULL;
}

void destroy_all_carts(webstore_t *store){
  if(!store->all_shopping_carts){
    perror("destroy_all_carts: All carts are deallocated.\n");    
  }
  //printf("┃ size linked list: %d \n", store->all_shopping_carts->size);
  //  printf("┃ element: %ls", store->all_shopping_carts)->first->elemen
  ioopm_link_t *current = (store->all_shopping_carts)->first;
  cart_t *current_cart  = NULL;
  
  do {
    current_cart = get_elem_ptr(current->element);    
    cart_destroy(current_cart);
    
    current = current->next;    
  } while (current != NULL);

  ioopm_linked_list_destroy(store->all_shopping_carts);  
  //free(store->all_shopping_carts);
  store->all_shopping_carts = NULL;
}

void remove_from_cart(webstore_t *store, int id, char *merch_to_remove_name, int amount_to_remove){
  if (!store->all_shopping_carts){
    perror("remove_from_cart: Cart database is deallocated.\n");
    return;
  }
    cart_t *current_cart = get_cart(store, id);

    if (!current_cart){
      perror("remove_from_cart: No active cart.\n");
      return;
    }
    
    else if(!merch_in_cart(current_cart, merch_to_remove_name)){
        perror("REMOVE FROM CART: There is no such merch in the cart.\n");
        return; 
    }
    else if(!valid_id(store,id)){
        perror("REMOVE FROM CART: The id of the cart is invalid.\n");
        return; 
    }
    
    //char *merch_to_remove_name = lookup_merch_name(store, nr_merch);
    
    int amount_of_merch =  amount_of_merch_in_cart(current_cart, merch_to_remove_name);
    
    //Tries to remove more merch than there is
    if(amount_of_merch < amount_to_remove){
        perror("amount_of_merch: Can´t reomve more merch than there is in the cart.\n");
        return;
    }
    
    //if the amount of merch in the cart is 0 then remove the merch, else just decrease the amount
    if(amount_of_merch == amount_to_remove){
        ioopm_hash_table_remove(current_cart->merch_in_cart, str_elem(merch_to_remove_name));;
        //printf("┃ You have removed %s from cart with id %d", removed, current_cart->id);
    }else{
        ioopm_hash_table_insert(current_cart->merch_in_cart, str_elem(merch_to_remove_name), int_elem(amount_of_merch-amount_to_remove)); 
    }
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

/*
char *shelf_with_most_stock(webstore_t *store, char *name){
    ioopm_list_t *locs = merch_locs(store, name);
    
    shelf_t *current_shelf;
    int current_amount;
    int most_stock = 0; 
    shelf_t *shelf_max = NULL;  
    size_t no_locs = ioopm_linked_list_size(locs);
    
    printf("┃ no_locs:%d\n", no_locs);
    if(no_locs == 1){
        
        shelf_max = ioopm_linked_list_get(locs, 0).p;

        
    }else{
            
        ioopm_list_iterator_t *iter = ioopm_list_iterator(locs); 
        
        for (int i = 0; i < no_locs; i++) {
            current_shelf = get_elem_ptr(ioopm_iterator_current(iter));
            current_amount = merch_stock_on_shelf(store, name, current_shelf->shelf);
            
            if(current_amount > most_stock){
                most_stock = current_amount; 
                shelf_max = current_shelf; 
            }
            if(ioopm_iterator_has_next(iter)){
                ioopm_iterator_next(iter); 
            }
        }
        ioopm_iterator_destroy(iter);
    }
    
    return shelf_max->shelf; 
    ioopm_linked_list_destroy(locs);
}

void change_stock_in_webstore(webstore_t *store, char *current_name, int current_amount){
    char *location; 
    while(true){
        location = shelf_with_most_stock(store, current_name); 
        printf("┃ location: %s\n", location); 
        printf("┃ current_amount: %d\n", current_amount); 
        if(merch_stock_on_shelf(store, current_name, location) >= current_amount){
            printf("┃ merch_stock_on_shelf(store, current_name, location): %d\n", merch_stock_on_shelf(store, current_name, location)); 
            int new_stock = (merch_stock_on_shelf(store, current_name, location)-current_amount); 
            printf("┃ stock: %d\n", new_stock); 
            printf("┃ location: %s\n", location); 
            set_merch_stock(store, current_name, new_stock, location);
            
            break; 
        }else{
            current_amount = current_amount - merch_stock_on_shelf(store, current_name, location);
            printf("┃ merch_stock_on_shelf(store, current_name, location): %d\n", merch_stock_on_shelf(store, current_name, location)); 
            printf("┃ current_amount after else: %d\n", current_amount); 
            set_merch_stock(store, current_name, 0, location);
        }
    }
}
*/
void checkout(webstore_t *store){
  int id = store->active_cart;
  cart_t *current_cart = get_cart(store, id);
  
  if (!current_cart){
    perror("checkout: Cart requested is deallocated.\n");
    return;
  }
  else if (!current_cart->merch_in_cart){
    perror("checkout: Cart has no merch.\n");
    return;
  }
  else if (ioopm_hash_table_is_empty(current_cart->merch_in_cart)){
    perror("checkout: No cart exists.\n");
    return;
  }
  int total = 0;
  total     = calculate_cost(store, store->active_cart);
  
  char *current_name = NULL;
  int current_amount = 0;
  ioopm_list_t *names = ioopm_hash_table_keys(current_cart->merch_in_cart);

  
  if(cart_db_size(current_cart) == 1){
    current_name = get_elem_str(ioopm_linked_list_get(names, 0));
    current_amount = amount_of_merch_in_cart(current_cart, current_name);
    //    change_stock_in_webstore(store, current_name, current_amount);
    increase_equal_stock(store, current_name, current_amount);
    
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
      increase_equal_stock(store, current_name, current_amount);
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

  // If cart is empty
  if(cart_is_empty(cart)){
    printf("┃ Nothing to Show, Cart Id.%d is Empty.\n",
	   cart->id);    
    return;
  }

  printf("┃┏─────────╸ Cart Id.%d\n", cart->id);
  
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
      printf("┃┏────╸Item No.%d\n",(int)i+1);
      printf("┃┃ Name:   %s\n",   kv_array[i].key);
      printf("┃┃ Amount: %d\n", kv_array[i].value);
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


void add_to_cart_prompt(webstore_t *store, int id){
  int nr_merch = 0;
  
  list_merchandise(store); 

  // Set a correct amount, if incorrect return

  printf("┃┏────╸ Select Merchendise\n");
  do {
    nr_merch  = ask_question_int("┃┃ Number: ");    
  }while (!valid_index(store, nr_merch));

  char *merch_name        = lookup_merch_name(store, nr_merch-1);

  printf("┃┏────╸ Adding %s [Cart %d]\n",
	 merch_name, (int)id);
  
  size_t merch_amount;

  // Set a correct amount, if incorrect return
  do {
    merch_amount = ask_question_int("┃┃ Amount: ");
    
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
  int nr_merch     = 0;

  printf("┏─╸Remove From Cart Nr.%d\n",
	 (int)store->active_cart);
  
  if(cart_is_empty(get_cart(store,id))){
    puts("┃ The cart is empty! There is nothing to remove!"); 
    return;
    
  }else{
    // Disp cart if answer is yes 
    if (choice_prompt("Display Cart?"))
      display_cart(get_cart(store,id));
	

    // Re-ask until one is pleased return on error


  do {
    nr_merch = ask_question_int("┃ Merch Id.");
  }while (!is_merch(store, nr_merch));
    
    char *merch_name = get_merch_name_in_cart(get_cart(store,id), nr_merch);

    // Re-ask until one is pleased return on error
    do {
      merch_amount = ask_question_int("┃ Amount: ");
    } while (merch_amount <= (int)merch_stock(store, merch_name));


    remove_from_cart(store, id, merch_name, merch_amount); 
  }
}

void remove_from_active_cart_prompt(webstore_t *store){
  remove_from_cart_prompt(store);
}
