#include <string.h>
#include<stdio.h> 
#include <stdbool.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "hash_table.h"
#include "list_linked.h"
#include "webstore.h"
#include "common.h"
#include "cart.h"
#include "merch.h"

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}


/////////////////////////////////////////////////////////////

void test(){
    CU_ASSERT_TRUE(true);
}

void create_destroy_cart(){
  webstore_t *store = store_create();
  cart_t *cart = create_cart(store);
  remove_cart(store, cart->id);
  store_destroy(store);
}

void add_to_cart_test(){
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);

  cart_t *cart = create_cart(store);
  
  int stock_apple_before = merch_stock(store, "Apple");
  add_to_cart(store, cart->id, "Apple", 10); 
  int stock_apple_after = merch_stock(store, "Apple");
  CU_ASSERT_EQUAL(stock_apple_before, stock_apple_after); 
  CU_ASSERT_EQUAL(get_amount_of_merch_in_cart(cart, "Apple"), 10);
  CU_ASSERT_TRUE(merch_in_cart(cart, "Apple"));
  
  int stock_orange_before = merch_stock(store, "Orange");
  add_to_cart(store, cart->id, "Orange", 8); 
  int stock_orange_after = merch_stock(store, "Orange");
  CU_ASSERT_EQUAL(stock_orange_before,stock_orange_after); 
  CU_ASSERT_EQUAL(get_amount_of_merch_in_cart(cart, "Orange"), 8);
  CU_ASSERT_TRUE(merch_in_cart(cart, "Orange"));

  
  int stock_coconut_before = merch_stock(store, "Coconut");
  add_to_cart(store, cart->id, "Coconut", 1); 
  int stock_coconut_after = merch_stock(store, "Coconut");
  CU_ASSERT_EQUAL(stock_coconut_before, stock_coconut_after); 
  CU_ASSERT_EQUAL(get_amount_of_merch_in_cart(cart, "Coconut"), 1);
  CU_ASSERT_TRUE(merch_in_cart(cart, "Coconut"));
  
  
  remove_cart(store, cart->id);
  store_destroy(store);
}

void add_zero_amount_to_cart(){
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);

  cart_t *cart = create_cart(store);
  
  add_to_cart(store, cart->id, "Apple", 0); 
  CU_ASSERT_FALSE(merch_in_cart(cart, "Apple"));
  
  remove_cart(store, cart->id);
  store_destroy(store);
}

void add_larger_than_stock_to_cart(){
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  cart_t *cart = create_cart(store);
  
  add_to_cart(store, cart->id, "Apple", merch_stock(store, "Apple")+1); 
  CU_ASSERT_FALSE(merch_in_cart(cart, "Apple"));
  
  remove_cart(store, cart->id);
  store_destroy(store);
}

void add_merch_to_cart_multi_times(){
  
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  cart_t *cart = create_cart(store);
  
  add_to_cart(store, cart->id, "Apple", 10); 
  CU_ASSERT_EQUAL(get_amount_of_merch_in_cart(cart, "Apple"), 10);
  
  add_to_cart(store, cart->id, "Apple", 10); 
  CU_ASSERT_EQUAL(get_amount_of_merch_in_cart(cart, "Apple"), 20);
  
  remove_cart(store, cart->id);
  store_destroy(store);
}

void add_non_existing_merch_to_cart(){
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  cart_t *cart = create_cart(store);
  
  add_to_cart(store, cart->id, "Soda", 10); 
  CU_ASSERT_FALSE(merch_in_cart(cart, "Soda"));
  
  remove_cart(store, cart->id);
  store_destroy(store);
}

void remove_from_cart_test(){
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  cart_t *cart = create_cart(store);
  
  add_to_cart(store, cart->id, "Apple", 10); 
  add_to_cart(store, cart->id, "Orange", 8); 
  
  remove_from_cart(store, cart->id, "Apple", 5); 
  CU_ASSERT_EQUAL(get_amount_of_merch_in_cart(cart, "Apple"), 5);
  
  remove_from_cart(store, cart->id, "Orange", 7); 
  CU_ASSERT_EQUAL(get_amount_of_merch_in_cart(cart, "Orange"), 1);
  
  remove_cart(store, cart->id);
  store_destroy(store);
}

void remove_merch_compleatly(){
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  cart_t *cart = create_cart(store);
  
  add_to_cart(store, cart->id, "Apple", 10); 
  
  remove_from_cart(store, cart->id, "Apple", 10); 
  CU_ASSERT_FALSE(merch_in_cart(cart, "Apple"));
  
  remove_cart(store, cart->id);
  store_destroy(store);
}
 
void remove_merch_multiple_times(){
  
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  cart_t *cart = create_cart(store);
  
  int amount = 10; 
  
  add_to_cart(store, cart->id, "Apple", amount); 
  
  for(int i = 0; i<9; i++){
    remove_from_cart(store, cart->id, "Apple", 1); 
    CU_ASSERT_TRUE(merch_in_cart(cart, "Apple"));
    
    amount -= 1; 
    CU_ASSERT_EQUAL(get_amount_of_merch_in_cart(cart, "Apple"), amount);
  }
  
  remove_from_cart(store, cart->id, "Apple", 1); 
  CU_ASSERT_FALSE(merch_in_cart(cart, "Apple"));
  
  remove_cart(store, cart->id);
  store_destroy(store);
  
}

void remove_zero_from_merch(){
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  cart_t *cart = create_cart(store);
  
  int amount = 10; 
  add_to_cart(store, cart->id, "Apple", amount); 
  
  remove_from_cart(store, cart->id, "Apple", 0); 
  CU_ASSERT_TRUE(merch_in_cart(cart, "Apple"));
  CU_ASSERT_EQUAL(get_amount_of_merch_in_cart(cart, "Apple"), amount);
  
  remove_cart(store, cart->id);
  store_destroy(store);
}

void remove_non_existing_merch(){
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  cart_t *cart = create_cart(store);
  
  add_to_cart(store, cart->id, "Apple", 10); 
  
  CU_ASSERT_FALSE(merch_in_cart(cart, "Soda"));
  remove_from_cart(store, cart->id, "Soda", 10); 
  CU_ASSERT_FALSE(merch_in_cart(cart, "Soda"));
  
  remove_cart(store, cart->id);
  store_destroy(store);
}

void calculate_cost_test(){
  
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  cart_t *cart = create_cart(store);
  
  int cost = 0;
  
  add_to_cart(store, cart->id, "Apple", 10); 
  int cost_apple = get_amount_of_merch_in_cart(cart, "Apple") * merch_price(store, "Apple");
  cost = calculate_cost(store, cart->id);
  CU_ASSERT_EQUAL(cost, cost_apple);
  
  add_to_cart(store, cart->id, "Orange", 8); 
  int cost_orange = get_amount_of_merch_in_cart(cart, "Orange") * merch_price(store, "Orange");
  cost = calculate_cost(store, cart->id);
  CU_ASSERT_EQUAL(cost, (cost_apple + cost_orange));

  add_to_cart(store, cart->id, "Coconut", 1); 
  int cost_coconut = get_amount_of_merch_in_cart(cart, "Coconut") * merch_price(store, "Coconut");
  cost = calculate_cost(store, cart->id);
  CU_ASSERT_EQUAL(cost, (cost_apple + cost_orange + cost_coconut));
  
  remove_cart(store, cart->id);
  store_destroy(store);

}

void calculate_cost_empty_cart(){
  
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  cart_t *cart = create_cart(store);
  
  int cost = calculate_cost(store, cart->id);
  
  CU_ASSERT_EQUAL(cost, 0);
  
  remove_cart(store, cart->id);
  store_destroy(store);
  
}

void display_cart_test(){
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  cart_t *cart = create_cart(store);
  
  display_cart(cart);
  
  add_to_cart(store, cart->id, "Apple", 10); 
  add_to_cart(store, cart->id, "Orange", 8); 
  add_to_cart(store, cart->id, "Coconut", 1); 
  
  display_cart(cart);
  
  remove_cart(store, cart->id);
  store_destroy(store);
}

void checkout_test(){
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  cart_t *cart = create_cart(store);
    
  add_to_cart(store, cart->id, "Apple", 10); 
  add_to_cart(store, cart->id, "Orange", 8); 
  add_to_cart(store, cart->id, "Coconut", 1); 
  
  int a_before = merch_stock_on_shelf(store, "Apple", "F12"); 
  int o_before = merch_stock_on_shelf(store, "Orange", "F12");
  int c_before = merch_stock_on_shelf(store, "Coconut", "F12");
  
  show_stock(store); 
  checkout(store, cart->id); 
  show_stock(store);
  
  CU_ASSERT_EQUAL(merch_stock_on_shelf(store, "Apple", "F12"), a_before-10); 
  CU_ASSERT_EQUAL(merch_stock_on_shelf(store, "Orange", "F12"), o_before-8); 
  CU_ASSERT_EQUAL(merch_stock_on_shelf(store, "Coconut", "F12"), c_before-1); 

  store_destroy(store);
}

/*
void checkout_multi_locs_test(){
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  
  //set_shelf(store, "Apple", "C99", 12);
  set_shelf(store, "Coconut", "M01", 10);
  // M01: Coconut 10st
  // F12: Coconut 16st
  
  cart_t *cart = create_cart(store);
    
  //add_to_cart(store, cart->id, "Apple", 10); 
  //add_to_cart(store, cart->id, "Orange", 8); 
  add_to_cart(store, cart->id, "Coconut", 20); 
  
  //int a_before = merch_stock_on_shelf(store, "Apple", "F12"); 
  //int o_before = merch_stock_on_shelf(store, "Orange", "F12");
  int c_before = merch_stock_on_shelf(store, "Coconut", "F12");
  // 16st
  
  show_stock(store); 
  checkout(store, cart->id); 
  show_stock(store); 
  
  //CU_ASSERT_EQUAL(merch_stock_on_shelf(store, "Apple", "F12"), a_before-10); 
  //CU_ASSERT_EQUAL(merch_stock_on_shelf(store, "Orange", "F12"), o_before-8); 
  //printf("%d\n", merch_stock_on_shelf(store, "Coconut", "F12")); 
  CU_ASSERT_EQUAL(merch_stock_on_shelf(store, "Coconut", "F12"), 16); 
  //printf("%d\n", merch_stock(store, "Coconut")); 
  CU_ASSERT_EQUAL(merch_stock_on_shelf(store, "Coconut", "M01"), 10); 

  store_destroy(store);
}*/

/////////////////////////////////////////////////////////////
int main()
{
  CU_pSuite test_suite1 = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  test_suite1 = CU_add_suite("Tests API", init_suite, clean_suite);
  if (NULL == test_suite1){
      CU_cleanup_registry();
      return CU_get_error();
  }

  if ((NULL == CU_add_test(test_suite1, "Create Destroy Cart Test", create_destroy_cart))   ||
     (NULL == CU_add_test(test_suite1, "Add to Cart Test", add_to_cart_test)) ||
     (NULL == CU_add_test(test_suite1, "Add Zero Amount to Cart Test",   add_zero_amount_to_cart))   ||
     (NULL == CU_add_test(test_suite1, "Add Larger Than Stock to Cart Test",   add_larger_than_stock_to_cart))   ||
     (NULL == CU_add_test(test_suite1, "Add Merch to Cart Multi Times Test",   add_merch_to_cart_multi_times))   ||
     (NULL == CU_add_test(test_suite1, "Add Non Exsisting Merch to Cart Test",   add_non_existing_merch_to_cart)) ||
     
     (NULL == CU_add_test(test_suite1, "Remove From Cart Test",   remove_from_cart_test)) ||
     (NULL == CU_add_test(test_suite1, "Remove From Cart Compleatly Test",   remove_merch_compleatly)) ||
     (NULL == CU_add_test(test_suite1, "Remove From Cart Multi Times Test",   remove_merch_multiple_times)) ||
     (NULL == CU_add_test(test_suite1, "Remove Zero From Cart Test",   remove_zero_from_merch)) ||
     (NULL == CU_add_test(test_suite1, "Remove Non Existing From Cart Test",   remove_non_existing_merch)) ||
     
     (NULL == CU_add_test(test_suite1, "Calculate Cost Test",   calculate_cost_test)) ||
     (NULL == CU_add_test(test_suite1, "Calculate Cost Of Empty Cart Test",   calculate_cost_empty_cart)) ||
     (NULL == CU_add_test(test_suite1, "Display Test",   display_cart_test)) ||
     (NULL == CU_add_test(test_suite1, "Checkout Test",   checkout_test)) //||
    // (NULL == CU_add_test(test_suite1, "Checkout Multi Locs Test",   checkout_multi_locs_test))
     )
     {
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
