#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "hash_table.h"
#include "iterator.h"
#include "list_linked.h"
#include "common.h"
#include "utils.h"
#include "merch.h"
#include "webstore.h"
#include "cart.h"
#include "ansi.h"


// Macro: CLEAR()
// Does: Clear the terminal screen
#define CLEAR() printf("\033[H\033[J")

// Macro: newline
// Does: print a newline
#define NEWLINE puts("")


// Macro: BIPROMPT(swe, eng, what)
// Does: If LANG is set to:
//       - SWEDISH run swe
//       - ENGLISH run eng
//       then run what
#define BIPROMPT(swe, eng, what)		\
  SWE(swe);					\
  ENG(eng);					\
  what

// Macro: LOG_STR(str)
// Does:  Print a log statement of string
// Args:  str: Message to be logged
#define LOG_STR(str)  \
    if (store->opt->log_p) \
      printf("┃ %s%s[LOG]%s %s\n",FG_MAGENTA, BG_BLACK, NORMAL, str)


// Macro: LOG_STR_INT(str, i)
// Does:  Print a log statement of string containing a integer
// Args:  str: Message to be logged
//          i: Integer to be logged
#define LOG_STR_INT(str, i)			\
    if (store->opt->log_p) \
      printf("┃ %s%s[LOG]%s %s (%d)\n", BG_BLACK, FG_MAGENTA, NORMAL,  str, i)

// Macro: IF_LOG(expr)
// Does:  Evaluate expr if the program was executed with --log
// Args:  expr: expression to evaluate
#define IF_LOG(expr)  \
    if (store->opt->log_p) \
      expr

// Macro: PROMPT_INVALID(msg)
// Does:  Act as a function which prints a invalid prompt
// Args: msg: Invalid message to prompt (Red w. ANSI codes)
#define PROMPT_INVALID(msg)					\
  printf("┃\n┃%s%s -!- %s -!- %s\n┃\n", BG_BLACK, FG_RED, msg, NORMAL)


// Macro: PROMPT_VALID(msg)
// Does:  Act as a function which prints a invalid prompt
// Args:  msg: Valud message to prompt (Green w. ANSI codes)
#define PROMPT_VALID(msg)					\
  printf("┃\n┃%s%s -+- %s -+- %s\n┃\n", BG_WHITE, FG_GREEN, msg, NORMAL)

// Macro: PROMPT_BOLD(msg)
// Does:  Act as a function which prints a invalid prompt
// Args:  msg: Message to be printed in bold (Bold w. ANSI codes)
#define PROMPT_BOLD(msg)				\
  printf("┃ > %s%s%s\n", BOLD, msg, NORMAL)


// Macro: PROMPT_W_ID(msg, i)
// Does:  Act as a function which prints out some integer
//        together with a message
// Args:  msg: Message to be printed in bold (Blue w. ANSI codes)
//          i: ID to be printed in blue (Blue w. ANSI codes)
#define PROMPT_W_ID(msg, i)						\
  printf("┃ %s[%d]%s %s%s%s\n", FG_BLUE, i, NORMAL, BOLD, msg, NORMAL)

// Macro: PROMPT_W_ID(msg, i)
// Does:  Act as a function which prints out some integer
//        together with a message
// Args:  msg: Message to be printed in bold (Blue w. ANSI codes)
//          i: ID to be printed in blue (Blue w. ANSI codes)
#define PROMPT_SHELF(merch_name, id, stock)		       \
  printf("┃ %s[%d]%s %s%s%s %dst\n", FG_BLUE, id, NORMAL, BOLD, \
	 merch_name, NORMAL, stock)

// Macro: PROMPT_ITALIC(msg)
// Does:  Act as a function which prints a message in italic style
// Args:  msg: Message to be printed in italic (Italic. ANSI codes)
#define PROMPT_ITALIC(msg)				\
  printf("┃ > %s%s%s\n", ITALIC, msg, NORMAL)


// Macro: PROMPT_MENU_HEADER(msg)
// Does:  Act as a function which prints a menu header
// Args:  msg: Message to be printed in bold (Bold. ANSI codes)
#define PROMPT_MENU_HEADER(msg)				\
  printf("┃ ===[  %s%s%s  ]===\n", BOLD, msg, NORMAL)


// Macro: PROMPT_MENU_W_KEY(msg, key)
// Does: Act as a function which prints a invalid prompt 
// Args: msg: Menu item
//       key: Char key to activate it
#define PROMPT_MENU_W_KEY(msg, key)					\
  printf("┃ %s[%c]%s %s%s%s\n", GREEN, key, NORMAL, ITALIC, msg, NORMAL);


// Macro: current_cart_id 
// Does:  synonym to the active cart attribute of store
// Note: This must ONLY be used when store is in scope, otherwise ==> ERROR
// Args:  none
#define current_cart_id	store->active_cart


// Macro: SWEDISH
// Does:  Act as flag for enabling the Swedish language
// Args:  none
#define SWEDISH 0


// Macro: ENGLISH
// Does:  Act as flag for enabling the English language
// Args:  none
#define ENGLISH 1


// Macro: LANG
// Does:  hold key for what language is used
//        SWEDISH or ENGLISH
// Args:  none
#define LANG ENGLISH


// Macro: SWE(thing)
// Does:  Commit some expression in Swedish
// Args:  thing: Expression to perform in SWEDISH
#define SWE(thing) if (LANG == ENGLISH) { thing; }


// Macro: ENG(thing) 
// Does:  Commit some expression in English
// Args:  thing: Expression to perform in ENGLISH
#define ENG(thing) if (LANG == ENGLISH) { thing; }


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
 
// Function:  char unicode_edit_cart_menu(webstore_t *store);
// Arguments: webstore_t *store
//   
//   
//  
//   
// Returns: char
void display_shelf(webstore_t *store, char *shelf);
