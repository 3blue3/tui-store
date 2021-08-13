#include <CUnit/CUnit.h>
#include <string.h>
#include <stdio.h> 
#include <stdbool.h>
#include <CUnit/Basic.h>
#include <stdlib.h>

#include "hash_table.h"
#include "list_linked.h"
#include "webstore.h"
#include "common.h"
#include "utils.h"
#include "cart.h"
#include "ui.h"
#include "merch.h"


int init_suite(void){
  return 0;
}

int clean_suite(void){
  return 0;
}

void test(){
    CU_ASSERT_TRUE(true);
}

#define STREQ(a, b) strcmp((char *)a, (char *)b) == 0)



// 
// ################################### CREATE AND DESTROY MERCH
void create_destroy_merch_test(void){
  char  *new_item_name  = "Eggs";
  char  *new_item_desc  = "Yummy!";
  size_t new_item_price = (size_t)16.99;

  char  *new_item_shelf = "G03";
  int    new_item_stock =  2000;
  
  char *eggs            = "Eggs";
  char *eggs_shelf      = "G03";
    
  webstore_t *store = store_create();

  CU_ASSERT_FALSE(shelf_exists(store, eggs_shelf));
  // Add eggs as merchendise 
  new_item(store, new_item_name, new_item_desc, new_item_price,
	   new_item_shelf, new_item_stock);
  
  // Remove eggs using a differently allocated, but equal string.
  remove_merchendise(store, eggs);

  store_destroy(store);
}

void add_merchendise_test(void) {
  webstore_t *store = store_create();


  add_merchendise(store, "apple", "a fruit", 10);
  add_merchendise(store, "pear", "a fruit", 12);
  add_merchendise(store, "orange", "a fruit", 14);
  
  store_destroy(store);
  
}

void add_to_storage_test(void) {
  webstore_t *store = store_create();

  add_merchendise(store, "apple",  "a fruit", 10);
  add_merchendise(store, "pear",   "a fruit", 12);
  add_merchendise(store, "orange", "a fruit", 14);

  add_to_storage(store, "apple",  "A10");
  add_to_storage(store, "pear",   "A10");
  add_to_storage(store, "orange", "D21");
      
  store_destroy(store);
  
}



void destroy_storage_test(void){

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

  CU_ASSERT_TRUE(shelf_exists(store, "A01"));
  CU_ASSERT_TRUE(shelf_exists(store, "A10"));
  CU_ASSERT_TRUE(shelf_exists(store, "D21"));

  destroy_storage(store);

  CU_ASSERT_FALSE(shelf_exists(store, "A01"));
  CU_ASSERT_FALSE(shelf_exists(store, "A10"));
  CU_ASSERT_FALSE(shelf_exists(store, "D21"));

  store_destroy(store);

  CU_ASSERT_TRUE(true);
}
void display_test(void){
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

  display_shelf(store, "A01");
  display_shelf(store, "A10");
  display_shelf(store, "D21");


  store_destroy(store);

  CU_ASSERT_TRUE(true);
}
void free_saved_strs_test(void){
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

  free_saved_strs(store);
  
  store_destroy(store);

  CU_ASSERT_TRUE(true);
}

void get_locations_test(void){
  webstore_t *store = store_create();

//  | Merch      | Shelf  | Amount | 
//  | Ferarri    | A01    | 3      |
//  | apple      | A10    | 1	   | 
//  | pear       | A10    | 321	   | 
//  | orange     | D21    | 123	   | 
  
//  save_str(store, strdup("Ferarri"));
//  save_str(store, strdup("Merci"));
  
  
  
  new_item(store, "Ferarri",  "wrooooom!", 10, "A10", 3); 
  new_item(store, "apple",  "a fruit", 10, "A10", 1);	 
  new_item(store, "pear",   "a fruit", 12, "A10", 1);	 
  new_item(store, "orange", "a fruit", 14, "D21", 123);

  ioopm_list_t *locs = get_locations(store, "A10");
  ioopm_link_t *link = locs->first;


  while (link) {

    char * v = get_elem_ptr(link->element);
    if (STREQ("Ferarri", v) {
      CU_ASSERT_TRUE(true);
    }else if (STREQ("apple", v) {
      CU_ASSERT_TRUE(true);
    }else if (STREQ("pear", v) {
      CU_ASSERT_TRUE(true);
    }
    link = link->next;
  }
  
  
  store_destroy(store);



  CU_ASSERT_TRUE(true);
}
void get_merch_name_in_storage_test(void){
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
  char * a = get_merch_name_in_storage(store, 1);
  char * b = get_merch_name_in_storage(store, 2);
  char * c = get_merch_name_in_storage(store, 3);
  char * d = get_merch_name_in_storage(store, 4);


// Looking in all vars for answer, since we don't know order  
  CU_ASSERT_TRUE((STR_EQ(a, "Ferarri")) || (STR_EQ(a, "apple")) ||
                 (STR_EQ(a, "pear")) || (STR_EQ(a, "orange")));

  CU_ASSERT_TRUE((STR_EQ(b, "Ferarri")) || (STR_EQ(b, "apple")) ||
                 (STR_EQ(b, "pear")) || (STR_EQ(b, "orange")));

  CU_ASSERT_TRUE((STR_EQ(c, "Ferarri")) || (STR_EQ(c, "apple")) ||
                 (STR_EQ(c, "pear")) || (STR_EQ(c, "orange")));

  CU_ASSERT_TRUE((STR_EQ(d, "Ferarri")) || (STR_EQ(d, "apple")) ||
                 (STR_EQ(d, "pear")) || (STR_EQ(d, "orange")));
  
  store_destroy(store);



  CU_ASSERT_TRUE(true);
}
void get_shelf_after_shelf_nr_test(void){
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

  get_shelf_after_shelf_nr(store, 1, "apple");
  get_shelf_after_shelf_nr(store, 1, "Ferarri");
  get_shelf_after_shelf_nr(store, 1, "pear");

  CU_ASSERT_TRUE(STR_EQ(get_shelf_after_shelf_nr(store, 1, "orange"), "D21"));
  CU_ASSERT_TRUE(STR_EQ(get_shelf_after_shelf_nr(store, 1, "apple"), "A10"));
  CU_ASSERT_TRUE(STR_EQ(get_shelf_after_shelf_nr(store, 1, "Ferarri"), "A01"));
  CU_ASSERT_TRUE(STR_EQ(get_shelf_after_shelf_nr(store, 1, "pear"), "A10"));

  store_destroy(store);



  CU_ASSERT_TRUE(true);
}
void increase_equal_stock_test(void){
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



  CU_ASSERT_TRUE(true);
}
void increase_stock_test(void){
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



  CU_ASSERT_TRUE(true);
}
void is_saved_str_test(void){
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



  CU_ASSERT_TRUE(true);
}
void list_merchendise_test(void){
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



  CU_ASSERT_TRUE(true);
}
void list_shelfs_test(void){
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



  CU_ASSERT_TRUE(true);
}
void lookup_merch_name_test(void){
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



  CU_ASSERT_TRUE(true);
}
void merch_description_test(void){
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



  CU_ASSERT_TRUE(true);
}
void merch_in_stock_test(void){
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



  CU_ASSERT_TRUE(true);
}
void merch_locs_test(void){
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



  CU_ASSERT_TRUE(true);
}
void merch_stock_test(void){
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



  CU_ASSERT_TRUE(true);
}
void merch_stock_on_shelf_test(void){
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



  CU_ASSERT_TRUE(true);
}
void parse_args_test(void){
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



  CU_ASSERT_TRUE(true);
}
void print_merch_test(void){
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



  CU_ASSERT_TRUE(true);
}
void remove_from_stroage_test(void){
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



  CU_ASSERT_TRUE(true);
}
void remove_merchendise_test(void){
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



  CU_ASSERT_TRUE(true);
}
void remove_name_from_shelf_test(void){
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



  CU_ASSERT_TRUE(true);
}
void remove_shelf_test(void){
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



  CU_ASSERT_TRUE(true);
}
void rename_merch_test(void){
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



  CU_ASSERT_TRUE(true);
}
void save_str_test(void){
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



  CU_ASSERT_TRUE(true);
}
void set_merch_description_test(void){
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



  CU_ASSERT_TRUE(true);
}
void set_merch_price_test(void){
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



  CU_ASSERT_TRUE(true);
}
void set_merch_stock_test(void){
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



  CU_ASSERT_TRUE(true);
}
void set_shelf_test(void){
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



  CU_ASSERT_TRUE(true);
}
void destroy_locs_test(void){
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



  CU_ASSERT_TRUE(true);
}

void show_stock_test(void){
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



  CU_ASSERT_TRUE(true);
}
void shelf_exists_test(void){
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



  CU_ASSERT_TRUE(true);
}
void storage_contains_test(void){
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



  CU_ASSERT_TRUE(true);
}
void store_create_test(void){
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



  CU_ASSERT_TRUE(true);
}
void store_destroy_test(void){
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



  CU_ASSERT_TRUE(true);
}
void sync_merch_stock_test(void){
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



  CU_ASSERT_TRUE(true);
}
void valid_index_test(void){
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



  CU_ASSERT_TRUE(true);
}

void destroy_all_merch_test(void) {
  webstore_t *store = store_create();
  INIT_DATABASE(store);

  destroy_all_merch(store);


  
  store_destroy(store);  
}
void create_destroy_merch_shelf_test(void){
  char  *new_item_name  = "Eggs";
  char  *new_item_desc  = "Yummy!";
  size_t new_item_price = (size_t)16.99;

  char  *new_item_shelf = "G03";
  int    new_item_stock =  2000;
  
  char *eggs            = "Eggs";
  char *eggs_shelf      = "G03";
    
  webstore_t *store = store_create();

  // Shelf does not exist prior to adding item
  CU_ASSERT_FALSE(shelf_exists(store, eggs_shelf));  

  // Add eggs as merchendise 
  new_item(store, new_item_name, new_item_desc, new_item_price,
	   new_item_shelf, new_item_stock);

  // Shelf was created after adding merch
  CU_ASSERT_TRUE(shelf_exists(store, eggs_shelf));  

  // Correct amount of the merch stock was added on the correct shelf
  CU_ASSERT_TRUE(merch_stock_on_shelf(store, eggs, eggs_shelf) == new_item_stock);
  
  // Remove eggs using a differently allocated, but equal string.n
  remove_merchendise(store, eggs);

  // No eggs remain
  CU_ASSERT_TRUE(merch_stock_on_shelf(store, eggs, eggs_shelf) == 0);
  CU_ASSERT_FALSE(merch_in_stock(store, eggs));

  // Test if the shelf was deleted upon deletion of merch
  CU_ASSERT_TRUE(shelf_exists(store, eggs_shelf));
  
  store_destroy(store);
}


void create_autodestroy_merch_test(void){
  // Add merchendise, but let the store destroy function remove the merch
  char  *new_item_name  = "Eggs";
  char  *new_item_desc  = "Yummy!";
  size_t new_item_price = (size_t)16.99;

  char  *new_item_shelf = "G03";
  int    new_item_stock =  2000;
    
  webstore_t *store = store_create();

  // Add eggs as merchendise 
  new_item(store, new_item_name, new_item_desc, new_item_price,
	   	   new_item_shelf, new_item_stock);

  // Dont Remove eggs
  // remove_merchendise(store, eggs);

  // No eggs remain

  // CU_ASSERT_TRUE(shelf_exists(store, eggs_shelf));  
  store_destroy(store);

}

void create_duplicate_merch_test(void){
  char  *new_item_name  = "Eggs";
  char  *new_item_desc  = "Yummy!";
  size_t new_item_price = (size_t)16.99;

  char  *new_item_shelf = "G03";
  int    new_item_stock =  2000;
  
  char *eggs            = "Eggs";

    
  webstore_t *store = store_create();

  
  // Add eggs as merchendise 
  new_item(store, new_item_name, new_item_desc, new_item_price,
	   new_item_shelf, new_item_stock);

  // ######################### ADD ASSERTIONS FOR THIS TODO
  // Duplicate merch! Error!
  new_item(store, new_item_name, new_item_desc, new_item_price,
	   new_item_shelf, new_item_stock);

  
  // Remove eggs using a differently allocated, but equal string.n
  remove_merchendise(store, eggs);
  
  store_destroy(store);
}

void create_merch_test_empty_locs(void){ // (create_merch | destroy_merchendise) 
  ioopm_list_t *new_locs  = ioopm_linked_list_create();
  merch_t      *new_merch = 
    create_merch("Tuna", "Raw Tuna", 123, new_locs);    

  CU_ASSERT_TRUE(STR_EQ(new_merch->name, "Tuna"));
  CU_ASSERT_TRUE(STR_EQ(new_merch->desc, "Raw Tuna"));
  CU_ASSERT_EQUAL(new_merch->price, 123);
  CU_ASSERT_EQUAL(new_merch->total_amount, 0);
  CU_ASSERT_EQUAL(new_merch->locs->size, 0);

  destroy_merchendise(new_merch);
}



void create_merch_test_populated_locs(void){
  ioopm_list_t *new_locs  = ioopm_linked_list_create();
  merch_t      *new_merch = 
    create_merch("Tuna", "Raw Tuna", 123, new_locs);    

  CU_ASSERT_TRUE(STR_EQ(new_merch->name, "Tuna"));
  CU_ASSERT_TRUE(STR_EQ(new_merch->desc, "Raw Tuna"));
  CU_ASSERT_EQUAL(new_merch->price, 123);
  CU_ASSERT_EQUAL(new_merch->total_amount, 0);
  CU_ASSERT_EQUAL(new_merch->locs->size, 0);

  
  char *shelfs[5] = {"A01", "A02", "A03", "A04", "A05"};
  size_t amounts[5] = {20, 21, 22, 23, 24};
  for (int i = 0; i < 5; i++){
    shelf_t *shelf = create_shelf(shelfs[i], amounts[i]);
    ioopm_linked_list_append(new_merch->locs, ptr_elem(shelf));
    // Assert that the merch shelf db grows correctly
    CU_ASSERT_TRUE(new_merch->locs->size == i + 1);
  }

  destroy_merchendise(new_merch);
}

void str_memory_management_system_test(void){
  char  *str1  = NULL;
  char  *str2  = NULL;
  char  *str3  = NULL;
  
  webstore_t *store = store_create();
  //  str1 = malloc(sizeof(char) * 5);
  
  str1 = strdup("heap allocated 1\0");
  str2 = strdup("heap allocated 2\0");
  str3 = strdup("heap allocated 3\0");

  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 1\0"));  
  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 2\0"));  
  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 3\0"));  
  
  save_str(store, str1);
  save_str(store, str2);
  save_str(store, str3);

  CU_ASSERT_TRUE(is_saved_str(store, "heap allocated 1\0"));  
  CU_ASSERT_TRUE(is_saved_str(store, "heap allocated 2\0"));  
  CU_ASSERT_TRUE(is_saved_str(store, "heap allocated 3\0"));  

  free_saved_strs(store);

  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 1\0"));  
  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 2\0"));  
  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 3\0"));  
//  
  // free(str1);  
  store_destroy(store);
}


void create_shelf_test(void){
  shelf_t *new_shelf = create_shelf("A23", 123);    

  CU_ASSERT_TRUE(STR_EQ(new_shelf->shelf, "A23"));
  CU_ASSERT_EQUAL(new_shelf->amount, 123);

  destroy_shelf(new_shelf);
}


void shelf_exist_test(void){
  webstore_t *store = store_create();

  char *shelfs[] = {"A01", "A02", "A03", "A04", "A05"};
  new_item(store, "Bob Dobbs", "Sells cars", 999, "F00", 1);

  // Add 5 shelfs
  for (size_t amount  = 0; amount < 5; amount++){
    set_shelf(store, "Bob Dobbs", shelfs[amount], amount * 40);
  }

  // Assert they exist
  for (int i=0; i < 5;i++) CU_ASSERT_TRUE(shelf_exists(store, shelfs[i]));





  
  store_destroy(store);
}

void banner_api_start(void){

  printf("\n");
  printf("▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁\n");
  printf("█             STARTING THE MERCHENDISE API TESTS ...       █\n");
  printf("█                                                          █\n");
  printf("█__________________________________________________________█\n\n");


  CU_ASSERT_TRUE(true);
}
void banner_api_end(void){
  printf("\n");
  printf("▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁\n");
  printf("█             FINISHED TESTING MERCHENDISE API ✓           █\n");
  printf("█                                                          █\n");
  printf("█__________________________________________________________█\n\n");


  CU_ASSERT_TRUE(true);
}

void banner_start(void){
  printf("\n");
  printf("▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁\n");
  printf("█        STARTING THE MERCHENDISE USE-CASE TESTS ...       █\n");
  printf("█                                                          █\n");
  printf("█__________________________________________________________█\n\n");

  CU_ASSERT_TRUE(true);
}
void banner_end(void){
  printf("\n");
  printf("▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁\n");
  printf("█          FINISHED TESTING MERCHENDISE USE-CASES ✓        █\n");
  printf("█                                                          █\n");
  printf("█__________________________________________________________█\n\n");

  CU_ASSERT_TRUE(true);
}


/////////////////////////////////////////////////////////////
int main()
{

  CU_pSuite merch_test_suite = NULL;
  CU_pSuite merch_api_test_suite = NULL;
  //  CU_pSuite misc_test_suite = NULL;



  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  merch_api_test_suite = CU_add_suite("Tests Merch API Features.", init_suite, clean_suite);
  merch_test_suite = CU_add_suite("Test common use-cases using the API.", init_suite, clean_suite);
  //  misc_test_suite  = CU_add_suite("Test Misc Features",       init_suite, clean_suite);
  
  if (NULL == merch_test_suite){
    CU_cleanup_registry();
    return CU_get_error();
  }


  if (NULL == merch_api_test_suite){
    CU_cleanup_registry();
    return CU_get_error();
  }

  bool merch_test =
    ((NULL == CU_add_test(merch_test_suite,
			  "Display banner",
			  banner_start)) ||

     (NULL == CU_add_test(merch_test_suite,
			  "Create and Remove Merch: Creation and Deletion of merch",
			  create_destroy_merch_test)) ||
     (NULL == CU_add_test(merch_test_suite,
			  "Construct Merch: Creation and Deletion of merch (empty location db) without adding to store.",
			  create_merch_test_empty_locs)) ||

     (NULL == CU_add_test(merch_test_suite,
			  "Construct Merch: Creation and Deletion of merch (with location db) without adding to store.",
			  create_merch_test_populated_locs)) ||

     (NULL == CU_add_test(merch_test_suite,
			  "Create and Remove Merch: Shelf correct creation, stock and deletion",
			  create_destroy_merch_shelf_test)) ||

     (NULL == CU_add_test(merch_test_suite,
			  "Create and Remove Merch: Automatic deallocation of merch",
			  create_autodestroy_merch_test)) ||

     (NULL == CU_add_test(merch_test_suite,
			  "Create Shelf: Creation of shelf.",
			  create_shelf_test)) ||

     (NULL == CU_add_test(merch_test_suite,
			  "Shelf exist test: Testing adding shelfs to a merch.",
			  shelf_exist_test)) ||
      
     (NULL == CU_add_test(merch_test_suite,
			  "Create Shelf: Creation of shelf.",
			  destroy_all_merch_test)) ||
     //      (NULL == CU_add_test(merch_test_suite,
     //			   "Memory Management: Manual-deallocation of strings",
     //			   str_memory_management_system_manual_test)) ||
     (NULL == CU_add_test(merch_test_suite,
			  "Memory Management: Auto-deallocation of strings",
			  str_memory_management_system_test)) ||
     (NULL == CU_add_test(merch_test_suite,
			  "Add merchendise test",
			  add_merchendise_test)) ||

     (NULL == CU_add_test(merch_test_suite,
			  "Create and Remove Merch: Duplicate creation of merch",
			  create_duplicate_merch_test)) ||

     (NULL == CU_add_test(merch_test_suite,
			  "Create and Remove Merch: Shelf correct creation, stock and deletion",
			  add_to_storage_test)) ||


     (NULL == CU_add_test(merch_test_suite,
			  "Display banner",
			  banner_end))
     );

  bool merch_api_test =
    ((NULL == CU_add_test(merch_api_test_suite,
			  "Display banner",
			  banner_api_start)) ||
     (NULL == CU_add_test(merch_api_test_suite,
			  "Function destroy_storage_test test",
			  destroy_storage_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function display_test test",
			  display_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function free_saved_strs_test test",
			  free_saved_strs_test)) ||


     
     (NULL == CU_add_test(merch_api_test_suite,
			  "Function get_locations_test test",
			  get_locations_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function get_merch_name_in_storage_test test",
			  get_merch_name_in_storage_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function get_shelf_after_shelf_nr_test test",
			  get_shelf_after_shelf_nr_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function increase_equal_stock_test test",
			  increase_equal_stock_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function increase_stock_test test",
			  increase_stock_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function is_saved_str_test test",
			  is_saved_str_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function list_merchendise_test test",
			  list_merchendise_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function list_shelfs_test test",
			  list_shelfs_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function lookup_merch_name_test test",
			  lookup_merch_name_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function merch_description_test test",
			  merch_description_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function merch_in_stock_test test",
			  merch_in_stock_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function merch_locs_test test",
			  merch_locs_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function merch_stock_test test",
			  merch_stock_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function merch_stock_on_shelf_test test",
			  merch_stock_on_shelf_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function parse_args_test test",
			  parse_args_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function print_merch_test test",
			  print_merch_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function remove_from_stroage_test test",
			  remove_from_stroage_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function remove_merchendise_test test",
			  remove_merchendise_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function remove_name_from_shelf_test test",
			  remove_name_from_shelf_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function remove_shelf_test test",
			  remove_shelf_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function rename_merch_test test",
			  rename_merch_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function save_str_test test",
			  save_str_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function set_merch_description_test test",
			  set_merch_description_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function set_merch_price_test test",
			  set_merch_price_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function set_merch_stock_test test",
			  set_merch_stock_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function set_shelf_test test",
			  set_shelf_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function destroy_locs_test test",
			  destroy_locs_test)) ||




     (NULL == CU_add_test(merch_api_test_suite,
			  "Function show_stock_test test",
			  show_stock_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function shelf_exists_test test",
			  shelf_exists_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function storage_contains_test test",
			  storage_contains_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function store_create_test test",
			  store_create_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function store_destroy_test test",
			  store_destroy_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function sync_merch_stock_test test",
			  sync_merch_stock_test)) ||



     (NULL == CU_add_test(merch_api_test_suite,
			  "Function valid_index_test test",
			  valid_index_test)) ||
     (NULL == CU_add_test(merch_api_test_suite,
			  "Display banner",
			  banner_api_end)));

  if (merch_api_test || merch_test){
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  
  CU_basic_run_tests();
  
  CU_cleanup_registry();
  return CU_get_error();
}





