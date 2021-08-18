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
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  parse_args(store, argc, argv);

  event_loop_menu(store); 
   
  store_destroy(store);
  
  return 0; 
}
