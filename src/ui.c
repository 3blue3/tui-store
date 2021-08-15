#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "hash_table.h"
#include "iterator.h"
#include "list_linked.h"
#include "common.h"
#include "utils.h"
#include "merch.h"
#include "webstore.h"
#include "ansi.h"
#include "cart.h"
#include "ui.h"

void unicode_merch_menu(webstore_t *store){
  char command[10] = "";

  ENG(do {
      puts("┃ > Webstore    GPLv2🄯");
      puts("┃ [N]ew Merch         ┃");
      puts("┃ [R]emove Merch      ┃");
      puts("┃ [I]nspect Merch ID  ┃");
      puts("┃ [E]dit Merch        ┃");
      puts("┃ [L]ist Database     ┃");
      puts("┃ [B]ack              ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'N') || (command[0] == 'n')) 
	add_new_merch_prompt(store);
      else if ((command[0] == 'R') || (command[0] == 'r')) 
	remove_merch_prompt(store);
      else if ((command[0] == 'E') || (command[0] == 'e')) 
	edit_merch_prompt(store);
      else if ((command[0] == 'I') || (command[0] == 'i'))
	lookup_merch_prompt(store);
      else if ((command[0] == 'L') || (command[0] == 'l'))
	show_stock(store);
      else if ((command[0] == 'B') || (command[0] == 'b') ||
	       (command[0] == 'Q') || (command[0] == 'q'))
	break;

    } while (true));
  SWE(do {
      puts("┏──╸Webb Butik ╺──────┓");
      puts("| [N]y Vara           ┃");
      puts("┃ [R]adera Vara       ┃");
      puts("┃ [E]dita Vara        ┃");
      puts("┃ [S]ök ID            ┃");
      puts("┃ [L]ista Databasen   ┃");
      puts("┃ [B]akåt             ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);

      if      ((command[0] == 'N') || (command[0] == 'n'))
	add_new_merch_prompt(store);
      else if ((command[0] == 'R') || (command[0] == 'r'))
	remove_merch_prompt(store);
      else if ((command[0] == 'E') || (command[0] == 'e'))
	edit_merch_prompt(store);
      else if ((command[0] == 'S') || (command[0] == 's'))
	lookup_merch_prompt(store);
      else if ((command[0] == 'L') || (command[0] == 'l')) 
	show_stock(store);     
      else if ((command[0] == 'B') || (command[0] == 'b') ||
	       (command[0] == 'Q') || (command[0] == 'q'))
	break;

    } while (true));
}






void unicode_cart_menu(webstore_t *store){
  char command[10] ;

  ENG(do {
      puts("┏──╸Cart Interface ╺──┓");

      if (cart_exists(store)){
	printf("┃ [N]ew Cart    Id. %d ┃\n",
	       store->active_cart);
      }else { puts("┃ [N]ew Cart [No Cart]┃");      	
      }
      puts("┃ [R]emove Cart       ┃");
      puts("┃ [E]dit Cart         ┃");
      puts("┃ [A]ll Items         ┃");
      puts("┃ [D]isplay Cart      ┃");
      puts("┃ [L]ist all Carts    ┃");
      puts("┃ [I]d of Cart        ┃");
      puts("┃ [F]inal cost        ┃");
      puts("┃ [C]heck out  [B]ack ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'N') || (command[0] == 'n'))
	append_cart(store);
      else if ((command[0] == 'R') || (command[0] == 'r'))
	remove_cart_prompt(store);
      else if ((command[0] == 'E') || (command[0] == 'e')) 
	unicode_edit_cart_menu(store);
      else if ((command[0] == 'l') || (command[0] == 'L'))
	list_all_cart_id(store);
      else if (((command[0] == 'D') || (command[0] == 'd')) &&
	       (valid_id(store, store->active_cart)))
	display_cart(get_cart(store, store->active_cart));
      else if ((command[0] == 'A') || (command[0] == 'a'))
	show_stock(store);
      else if ((command[0] == 'I') || (command[0] == 'i'))
	print_cart_id(store);         

      else if ((command[0] == 'F') || (command[0] == 'f'))
	final_cost_menu(store);
      else if ((command[0] == 'B') || (command[0] == 'b') ||
	       (command[0] == 'Q') || (command[0] == 'q'))
	break;
      else if ((command[0] == 'C') || (command[0] == 'c'))
	checkout(store);

    } while (true));
  SWE(do {
      puts("┏──╸Webb Butik ╺───────────┓");

      if (cart_exists(store)){
	printf("┃ [N]y Kundvagn     Id. %d ┃\n",
	       store->active_cart);
      }else { puts("┃ [N]y Kundvagn  [Inget Id]┃"); }      

      puts("┃ [R]adera Kundvagn        ┃");
      puts("┃ [E]dita Kundvagn         ┃");
      puts("┃ [V]isa Kundvagn          ┃");
      puts("┃ [S]e alla Kundvagnar     ┃");
      puts("┃ [I]d på Kundvagn         ┃");
      puts("┃ [A]lla Varor             ┃");
      puts("┃ [T]otal Kostnad          ┃");
      puts("┃ [K]öp            [B]akåt ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'N') || (command[0] == 'n'))
	append_cart(store); 
      else if ((command[0] == 'R') || (command[0] == 'r'))
	remove_cart_prompt(store);
      else if ((command[0] == 'A') || (command[0] == 'a'))
	show_stock(store);
      else if ((command[0] == 'E') || (command[0] == 'e'))
	unicode_edit_cart_menu(store);
      else if ((command[0] == 's') || (command[0] == 'S'))
	list_all_cart_id(store);

      else if ((command[0] == 'I') || (command[0] == 'i'))
	print_cart_id(store);         
      else if (((command[0] == 'V') || (command[0] == 'v')) &&
	       (valid_id(store, store->active_cart)))
	display_cart(get_cart(store, store->active_cart));
      else if ((command[0] == 'T') || (command[0] == 't')) 
	final_cost_menu(store);
      else if ((command[0] == 'K') || (command[0] == 'k'))
	checkout(store);
      else if ((command[0] == 'B') || (command[0] == 'b') ||
	       (command[0] == 'Q') || (command[0] == 'q'))
	break;

    } while (true));
} 


int unicode_edit_merch_menu(webstore_t *store, char *name){
  char command[10] = "";

  ENG(do {
      puts("┏──╸Merch Edit ╺──────┓");
      puts("┃ [D]escription       ┃");
      puts("┃ [P]rice             ┃");
      puts("┃ [S]tock      [B]ack ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'D') || (command[0] == 'd')) 
	set_merch_desc_menu(store, name);
      else if ((command[0] == 'P') || (command[0] == 'p')) 
	set_merch_price_menu(store, name);
      else if ((command[0] == 'S') || (command[0] == 's')) 
	update_shelf_stock_menu(store, name);
      else if ((command[0] == 'B') || (command[0] == 'b') ||
	       (command[0] == 'Q') || (command[0] == 'q'))
	break;

    } while (true));
  SWE(do {
      puts("┏──╸Redigera Vara ╺───┓");
      puts("┃ [B]eskrivning       ┃");
      puts("┃ [P]ris              ┃");
      puts("┃ [A]ntal  [T]illbaka ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
      
      if      ((command[0] == 'B') || (command[0] == 'b')) 
      	set_merch_desc_menu(store, name);
      else if ((command[0] == 'P') || (command[0] == 'p')) 
	set_merch_price_menu(store, name);
      else if ((command[0] == 'A') || (command[0] == 'a')) 
	update_shelf_stock_menu(store, name);
      else if ((command[0] == 'T') || (command[0] == 't') ||
	       (command[0] == 'Q') || (command[0] == 'q'))
	break;

    } while (true));
 
  return 0;
} 

char unicode_edit_cart_menu(webstore_t *store){
  char command[10] = "";

  ENG(do {
      puts("┏──╸Merch Edit ╺──────┓");
      puts("┃ [A]dd to Cart       ┃");
      puts("┃ [R]emove from Cart  ┃");
      puts("┃ [D]isplay Cart      ┃");
      puts("┃ [C]urrent Cart ID   ┃");
      puts("┃ [S]et ID            ┃");
      puts("┃ [B]ack              ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'A') || (command[0] == 'a'))
	add_to_active_cart_prompt(store);           
      else if ((command[0] == 'R') || (command[0] == 'r')) 
	remove_from_cart_prompt(store);
      else if ((command[0] == 'C') || (command[0] == 'c'))
	print_cart_id(store);       
      else if ((command[0] == 'S') || (command[0] == 's'))
	change_cart_id_prompt(store);
      else if ((command[0] == 'D') || (command[0] == 'd'))
	display_cart_id_prompt(store);
      else if ((command[0] == 'B') || (command[0] == 'b') ||
	       (command[0] == 'Q') || (command[0] == 'q'))
	return 'b';    
      // Change this up if time
      
    } while (true));
  SWE(do {
      puts("┏──╸Redigera Kundvagn ╺───┓");
      puts("┃ [L]ägg Till Vara        ┃");
      puts("┃ [T]a Bort Vara          ┃");
      puts("┃ [V]isa kundvagn         ┃");
      puts("┃ [A]ktiv kundvagn        ┃");
      puts("┃ [S]ätt ID               ┃");
      puts("┃ [B]akåt                 ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    

      if ((command[0] == 'T') || (command[0] == 't')) 
	remove_from_cart_prompt(store);
      else if ((command[0] == 'L') || (command[0] == 'l'))
	add_to_active_cart_prompt(store);
      else if ((command[0] == 'A') || (command[0] == 'a'))
	print_cart_id(store);       
      else if ((command[0] == 'V') || (command[0] == 'v'))
	display_cart_id_prompt(store);
      else if ((command[0] == 'S') || (command[0] == 's'))
	change_cart_id_prompt(store);
      else if ((command[0] == 'B') || (command[0] == 'b') ||
	       (command[0] == 'Q') || (command[0] == 'q'))
	break;
    } while (true));
 
  return 0;
} 

void event_loop_menu(webstore_t *store){
  char command[10] = "";

  ENG(do {
      puts("┏──╸Webstore ╺────┓");
      puts("┃ [W]arehouse     ┃");
      puts("┃ [C]art          ┃");
      puts("┃ [S]et Active ID ┃");
      puts("┃ [Q]uit          ┃");
      puts("┗━━━━━━━━━━━━━━━━━┛");
      printf(">");

      read_string(command, 10);
    
      if      ((command[0] == 'W') || (command[0] == 'w'))
	unicode_merch_menu(store);
      else if ((command[0] == 'C') || (command[0] == 'c'))
	unicode_cart_menu(store);
      else if ((command[0] == 'S') || (command[0] == 's'))
	change_cart_id_prompt(store);
      else if ((command[0] == 'Q') || (command[0] == 'q'))
	return;

    } while (true));

  SWE(do {
      puts("┏──╸Affär ╺─────┓");
      puts("┃ [V]aruhus     ┃");
      puts("┃ [K]org        ┃");
      puts("┃ [S]ätt ID     ┃");
      puts("┃ [A]vsluta     ┃");
      puts("┗━━━━━━━━━━━━━━━┛");
      printf(">");
      
      read_string(command, 10);
    
      if      ((command[0] == 'V') || (command[0] == 'v'))
	event_loop_webstore(store);
      else if ((command[0] == 'K') || (command[0] == 'k'))
	event_loop_cart(store);
      else if ((command[0] == 'S') || (command[0] == 's'))
	change_cart_id_prompt(store);
      else if ((command[0] == 'A') || (command[0] == 'a') ||
	       (command[0] == 'Q') || (command[0] == 'q'))
	return;

    } while (true));
}

void print_cart_id(webstore_t *store){
  ENG(puts("┏──╸ Active Cart ID"));
  SWE(puts("┏──╸ Aktiv Kundvagn ID"))
  
  SWE(printf("┃ %d\n",  store->active_cart));
  ENG(printf("┃ %d\n",         store->active_cart));  
}

void change_cart_id_prompt(webstore_t *store){
  int new_id = 0;
  // Prompt and change the cart ID  
  list_all_cart_id(store);     
  ENG(puts("┏──╸ Change Active Cart [-1 to cancel]"));
  SWE(puts("┏──╸ Ändra Aktiv Kundvagn [-1 för att avbryta]"))

    do {

      SWE(new_id = ask_question_int("┃ Byt till Kundvagn ID: "));
      ENG(new_id = ask_question_int("┃ Change to Cart ID: "));
      if (new_id == -1){
	return;
      }
      if ((!cart_id_exists(store, new_id) &&
	   (!choice_prompt("Incorrect ID, Try again?"))))
	return;
	
    } while(!valid_id(store, new_id));
  

  store->active_cart = new_id;
  //  display_cart_id_prompt(store);
  //  if (choice_prompt("Show Active ID?"))
  //    print_cart_id(store);
  
  puts("┗──────────────────────────╸");
}

void display_cart_id_prompt(webstore_t *store){
  display_cart(get_cart(store, store->active_cart));
}


int final_cost_menu(webstore_t *store){
  int cost = calculate_cost(store, current_cart_id);

  ENG(printf("┏──╸ Total Price\n"));
  ENG(printf("┃ Cart Id.%d\n┃ Total Price: %d SEK\n",
	     current_cart_id, cost));
  SWE(printf("┏──╸ Total Summa\n"));
  SWE(printf("┃ Kundvagn Id.%d\n┃ Summa: %d SEK\n",
	     current_cart_id, cost));

  return cost;
}

void set_merch_desc_menu(webstore_t *store, char *name){
  char *desc_merch = NULL;

  ENG(puts("┏──╸Set New Description  "));
  SWE(puts("┏──╸Sätt Ny Beskrivning   "));
      
  ENG(desc_merch = ask_question_string("| New Description: "));      
  SWE(desc_merch = ask_question_string("| Ny Beskrivning: "));
      
  set_merch_description(store, name, desc_merch);
  save_str(store, desc_merch);
}

void set_merch_price_menu(webstore_t *store, char *name){
  int price        = 0;

  ENG(puts("┏──╸Set New Price  (SEK)"));
  SWE(puts("┏──╸Sätt Nytt Price (SEK)"));


  
  do {
    SWE(price = ask_question_int("┃ Pris: "));
    ENG(price = ask_question_int("┃ Price: "));
    // Check for validity of price
  } while ((price < MIN_ALLOWED_PRICE) ||
	   (price > MAX_ALLOWED_PRICE));
   
  set_merch_price(store, name, price);
}

void update_shelf_stock_menu(webstore_t *store, char *name){
  char *location = NULL;
  int amount     = 0;
  
  while(!location){
    
    ENG(puts("┏──╸ All Shelfs Containing the Merch..."));
    SWE(puts("┏──╸ Alla Hyllor Innehållande Varan..."));

    
    list_shelfs(store, name);
	
    ENG(puts("\n┏──╸ Enter an Valid Shelf."));
    SWE(puts("\n┏──╸ Skriv en Giltigt Hylla."));

    puts("┃ Format: [A-Z][0-9][0-9]");

    do {
      ENG(location = ask_question_string("┃ Shelf: "));
      SWE(location = ask_question_string("┃ Hylla: "));
    } while (!is_shelf(location));

    save_str(store, location);

  }
  int current_amount = merch_stock_on_shelf(store, name, location);
  // Ask for stock amount

  
  // Add name to shelf if it already doesnt not contain it. If
  // consumer chooses to do so.
  
  ENG(printf("\n┏──╸ Update Stock [-1 för att avbryta].\n"));
  SWE(printf("\n┏──╸ Updatera Antal [-1 to cancel].\n"));

  SWE(printf("┃ Nuvarande Antal: %d\n", current_amount));
  ENG(printf("┃ Current Stock: %d\n",   current_amount));

  
  do {
    SWE(amount = ask_question_int("┃ Antal: "));
    ENG(amount = ask_question_int("┃ Amount: "));

    // Ability to cancel
    if (amount == -1) return;
    
  } while ((amount > MAX_ALLOWED_STOCK) ||
	   (amount < MIN_ALLOWED_STOCK));

  // Update stock if there is a shelf with stock
  if ((amount == 0) && (current_amount == 0)){
    perror("update_shelf_stock_menu: Cannot add empty shelf!");
    return;
  }

  //remove_from_storage(store, name, location);
  //remove_name_from_shelf(store, location, name);


      // Add / Update shelf to both the merch database and the
  // storage database. If it already exists, update amount.
  
  if (amount == 0){
    printf("############# Rem shelf");
    remove_name_from_shelf(store, location, name);
  
    return;
  }
  set_merch_stock(store, name, amount, location);
}

void add_new_merch_prompt(webstore_t *store){
  char *desc_merch = NULL;
  char *shelf_name = NULL;
  char *name_merch = NULL;
      
  size_t price     = 0;
  size_t amount    = 0;      

  ENG(puts("┏──╸Add New Merch     ╺─────╸"));
  SWE(puts("┏──╸Lägg Till Ny Vara ╺─────╸"));
        
  SWE(name_merch = ask_question_string("┃ Namn: "));
  ENG(name_merch = ask_question_string("┃ Name: "));
      
  SWE(desc_merch = ask_question_string("┃ Beskrivning: "));
  ENG(desc_merch = ask_question_string("┃ Description: "));
      
  SWE(price = ask_question_int("┃ Pris: "));
  ENG(price = ask_question_int("┃ Price: "));


  do { // Enter shelf until it is valid	  
    SWE(shelf_name = ask_question_string("┃ Hylla: "));
    ENG(shelf_name = ask_question_string("┃ Shelf: "));     
  } while (!is_shelf(shelf_name));
	
  ENG(amount = ask_question_int("┃ Amount: "));
  SWE(amount = ask_question_int("┃ Antal: "));

  save_str(store, name_merch);
  save_str(store, desc_merch);
  save_str(store, shelf_name);

  NEW_ITEM(store, name_merch, desc_merch, price);
  SET_ITEM_LOC(store, name_merch, shelf_name, amount);
     
  

  puts("┗──────────────────────────╸");  
}

void remove_merch_prompt(webstore_t *store){
  int id           = 0;
  char *name_merch = NULL;
  
  show_stock(store);
  
  ENG(puts("┏──╸Remove Merch   [Type -1 to exit]"));
  SWE(puts("┏──╸Ta Bort en Vara [Skriv -1 för att avsluta]"));

  do {  
    ENG(id = ask_question_int("┃ Merch ID: "));
    SWE(id = ask_question_int("┃ Varu ID: "));
    if (id == -1) return;
  } while (!valid_index(store, id) || (id < 1));

    
  name_merch = get_merch_name_in_storage(store, id);
    
  printf("┃ %s Removed!\n", name_merch); 
  remove_merchendise(store, name_merch);
  puts("┗───────────────────────────────╸");
  
}
void lookup_merch_prompt(webstore_t *store){
  int id           = 0;
  char *name_merch = NULL;

  do {
    ENG(puts("┏──╸Lookup Merch Id   [Type -1 to exit]"));
    SWE(puts("┏──╸Kolla upp Varu-Id  [Skriv -1 för att avsluta]  "));

    do {
      ENG(id     = ask_question_int("┃ Merch ID: "));
      SWE(id     = ask_question_int("┃ Varu-ID: "));
      if (id == -1) return;
    } while (!valid_index(store, id) || (id < 1));

    name_merch = get_merch_name_in_storage(store, id);

    if (!name_merch){
      ENG(puts("┃ Not a Valid Merch ID!"));
      SWE(puts("┃ Inte ett riktigt varu-ID!"));
    }else {
      ENG(printf("┃ Merch Nr.%d is %s\n", id,
		 name_merch));
      SWE(printf("┃ Merch Nr.%d is %s\n", id,
		 name_merch));
	
    }
    puts("┗───────────────────────────────╸");        

  }while (name_merch == NULL);
}
  

void edit_merch_prompt(webstore_t *store){
  int id           = 0;
  char *name_merch = NULL;

  if (choice_prompt("Display Stock?"))
    show_stock(store);

  do{
    ENG(puts("┏──╸ Edit Merch Id     "));
    SWE(puts("┏──╸ Ändra Varu-ID     "));
    ENG(id     = ask_question_int("┃ Edit ID: "));
    SWE(id     = ask_question_int("┃ Ändra ID: "));

  } while (!is_merch(store, id));
    
  name_merch = get_merch_name_in_storage(store, id);

  // Call new event loop
  unicode_edit_merch_menu(store, name_merch);
}         

int main(int argc, char *argv[]) {
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  parse_args(store, argc, argv);

  event_loop_menu(store); 
   
  store_destroy(store);
  
  return 0; 
}
