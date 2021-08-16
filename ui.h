#ifndef __UI_H__
#define __UI_H__
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "store_data.h"
#include "webstore.h"
#include "cart.h"

void unicode_cart_menu(webstore_t *store);
void remove_cart_prompt(webstore_t *store);
void remove_from_active_cart_prompt(webstore_t *store);
void remove_from_cart_prompt(webstore_t *store);
void add_to_cart_prompt(webstore_t *store, int cart_id);
void add_to_active_cart_prompt(webstore_t *store);
void list_shelfs(webstore_t *store, char *name);
void display_shelf(webstore_t *store, char *shelf);

bool prompt_string(char *buffer, int buffer_size,
		   char *prompt, char *question,
		   char *prompt_again);


// Function:  void event_loop_webstore(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: void 
void event_loop_webstore(webstore_t *store);

// Function:  void event_loop_cart(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: void 
void event_loop_cart(webstore_t *store);

// Function:  int final_cost_menu(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: int 
int final_cost_menu(webstore_t *store);

// Function:  void change_active_cart_id(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: void 
void change_active_cart_id(webstore_t *store);

// Function:  char unicode_edit_cart_menu(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: char 
char unicode_edit_cart_menu(webstore_t *store);

// Function:  void event_loop_menu(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: void 
void event_loop_menu(webstore_t *store);

// Function:  void print_cart_id(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: void 
void print_cart_id(webstore_t *store);

// Function:  void change_cart_id_prompt(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: void 
void change_cart_id_prompt(webstore_t *store);

// Function:  void display_cart_id_prompt(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: void 
void display_cart_id_prompt(webstore_t *store);

// Function:  int final_cost_menu(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: int 
int final_cost_menu(webstore_t *store);

// Function:  void set_merch_desc_menu(webstore_t *store, char *name);
// Arguments: webstore_t *store, char *name
//   
//   
//  
//   
// Returns: void 
void set_merch_desc_menu(webstore_t *store, char *name);

// Function:  void set_merch_price_menu(webstore_t *store, char *name);
// Arguments: webstore_t *store, char *name
//   
//   
//  
//   
// Returns: void 
void set_merch_price_menu(webstore_t *store, char *name);

// Function:  void update_shelf_stock_menu(webstore_t *store, char *name);
// Arguments: webstore_t *store, char *name
//   
//   
//  
//   
// Returns: void 
void update_shelf_stock_menu(webstore_t *store, char *name);

// Function:  void add_new_merch_prompt(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: void 
void add_new_merch_prompt(webstore_t *store);

// Function:  void remove_merch_prompt(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: void 
void remove_merch_prompt(webstore_t *store);

// Function:  void lookup_merch_prompt(webstore_t *store);
// Arguments: webstore_t *store
//  - Prompt for the name associated with the merch 
//    bound to said ID.
//  - In a loop until either, correct ID, or input = "-1"
//    in which it returns
// Returns: void
void lookup_merch_prompt(webstore_t *store);




// Function:  void edit_merch_prompt(webstore_t *store);
// Arguments: webstore_t *store
// Prompt for editing of some merch,    
//  - Prompts a choice of Displaying IDs
//  - Then prompt what id to edit
//  - Call unicode_edit_merch_menu
// Returns: void 
void edit_merch_prompt(webstore_t *store);

// Function:  int unicode_edit_merch_menu(webstore_t *store, char *name);
// Arguments: webstore_t *store, char *name
//   
//   
//  
//   
// Returns: int 
int unicode_edit_merch_menu(webstore_t *store, char *name);

// Function:  void unicode_cart_menu(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: void 
void unicode_cart_menu(webstore_t *store);

// Function:  void unicode_merch_menu(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: void 
void unicode_merch_menu(webstore_t *store);

// Function:  char unicode_edit_cart_menu(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: char 
char unicode_edit_cart_menu(webstore_t *store);
 
// Function:  display_shelf(webstore_t *store, char *shelf);
// Arguments: webstore_t *store:
//                  char *shelf:
//   
//  
//   
// Returns: void
void display_shelf(webstore_t *store, char *shelf);



// Function: list_merchandise(webstore_t *store);
// Arguments: webstore_t *store:
//           
//   
//  
//   
// Returns: void
void list_merchandise(webstore_t *store);




// Function: void display_cart(cart_t *cart)
// Arguments: cart_t *cart
//           
//   
//  
//   
// Returns: void
void display_cart(cart_t *cart);
bool choice_prompt(char *prompt);
bool prompt_shelf(char *buffer, int buffer_size );
bool continue_printing();
#endif
