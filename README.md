# Assignment 2: Webstore 
Author: Erik Johan Karlsson
Usage: GPLv3

# Build and Run Instructions

## The Terminal User Interface
 - Compile the program with UI `make ui`
 - Clean program with ui binary with `make clean_ui`
 - Run ui with `make run_ui`

## The tests
 - Compile tests with `make tests`
 - Clean tests with `make clean_tests`
 - Compile and Run tests with `make build_and_run_tests`





# Structure
   
### Folder: `src`
> The src directory holds all the source files.

The files contained in `src` is:

 - `backend.c`
 - `cart.c`
 - `cart.h`
 - `common`
 - `common.h`
 - `db.c`
 
 - `debug.c`
 - `hash_table`
 - `hash_table.h`
 - `iterator.c`
 - `iterator.h`
 - `list_linked`
 - `list_linked.h`
 - `main.c`
 - `merch.c`
 - `merch.h`
 - `test.c`

 - `test_cart_new.c`

 - `test_merch.c `
 - `test_merch.h`
 - `ui.c`
 - `ui.h`
 - `utils.c`
 - `utils.h`
 - `webstore`
 - `webstore.h`

### Folder: `test`
> The test directory houses all test scripts and programs source code
> and/or binaries.

### Folder: `cache`
> The cache folder holds temporary trash files or temporary files that
> may be generated during script execution etc.

### Folder: `bin`
> The bin directory holds all compiled binary, linker files being the
> outcome of building.

Usually the binary name is one of either;
 - `main.out`:    Result of running `make main`
 - `run_test`:    Result of running `make test`
 - `ui_demo.out`: Result of running `make ui`


## Preprogrammed input
> To test the program with simulated input, the program `ui_test.sh` can
> be used. Located in the `./test` directory. This allows for (within
> the `ui_test.sh` file) scripts of keypresses to be programmed.

To run the `ui_test.sh` on a program, use the program path as
argument.  Here is how the test binary is tested in the makefile:

- `./test/ui_test.sh ./bin/ui-demo.out`.


## Tools 
### `empty_io.sh`

Script that clears the contents of the files:
 - `/input.txt` 
 - `/output.txt`







