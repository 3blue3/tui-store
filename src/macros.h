#include <stdio.h>


#ifndef __MACROS_H__
#define __MACROS_H__

#include <stdbool.h> 
#include <err.h>
#include <stdlib.h>





/*    ___________________________
      Inline function definitions
*/




/* 
 _______  ______  ______  _____   ______
 |______ |_____/ |_____/ |     | |_____/
 |______ |    \_ |    \_ |_____| |    \_
                                        
 _     _ _______ __   _ ______         _____ __   _  ______
 |_____| |_____| | \  | |     \ |        |   | \  | |  ____
 |     | |     | |  \_| |_____/ |_____ __|__ |  \_| |_____|

   ABORT THROW  THROW_QUIET  CATCH EXIT_ON_ERROR CONT_ON_ERROR 
     |    |       |              \           |    /              
     [Signal Errors]              [Handle signals] 
                       TRY  
                        |
                   [Environment]






 */                                                                    
 


/* THROW(error_msg, critical_p) 
   
 @  Like THROW, but without displaying a message, and
   without the ability to exit the program.
*/
#define THROW_QUIET err_event = true; break; 


/* THROW(error_msg, critical_p) 

    If critical_p print error_msg to stderr and exit with failure 
    else print error_msg to stderr and exit tryblock.
*/

#define THROW(error_msg, critical_p)		\
  err_event = true;				\
  fprintf(stderr, error_msg);			\
  if (critical_p) exit(EXIT_FAILURE);		\
  else break

/*  TRY{...ABORT;...} CATCH|CONT_ON_ERROR|EXIT_ON_ERROR

   Exit a TRY block, without any error.
   (Must be called outside of any loops, 
    or switch statements) inside the TRY block!
 */

#define ABORT break					

/* TRY{...} CONT_ON_

   Added after a TRY block ignore a raised error
   and continue executing.
 */

#define CONT_ON_ERROR while(false);

/* TRY{...} EXIT_ON_ERROR(status)

   Added after a TRY block, and will exit with code 
   status on a thrown error.
 */

#define EXIT_ON_ERROR(status)			\
  while(false); if(err_event) exit(status)

/* TRY{...} CATCH{...}

   Added after a TRY block, and will (on thrown error)
o   run its code block. It will not execute if no error occurred
 */

#define CATCH while(false);if(err_event)

/* TRY{...} CATCH|CONT_ON_ERROR|EXIT_ON_ERROR

   Evaluate expr, if the macro THROW is activated, it will either
   exit with failure, or run the error expr if the error was non
   critical. 
   Must be followed by on of: {CATCH, CONT_ON_ERROR, EXIT_ON_ERROR}
 */

#define TRY \
  bool err_event = false;do


/* STR(expr)

   Expand into the stringinized EXPR (as a const char *)   
 */
#define STR(expr) #expr


/* PERROR_EXPR(expr) 

   Print EXPR to standard error outoput formatted 
   to look like a error message. In the same format
   as the source code.
 */
#define PERROR_EXPR(expr)				\
  fprintf(stderr, "Error executing\n>   %s\n", STR(expr))


/* PRINT_EXPR(expr) 

   Print EXPR to standard output, note without any newline. 
   In the same format as the source code.
 */
#define PRINT_EXPR(expr)			\
  fprintf(stdout, "%s", STR(expr))


/* PREXEC(expr)

   Print and execute EXPR to standard output, note
   without any newline. In the same format  as the
   source code.
 */
#define PREXEC(expr)			\
  fprintf(stdout, "%s", STR(expr)) \
    expr

#define PROMPTF(head, expr, tail)					\
  printf("┃ %s%s", BOLD, head);						\
  printf(expr);								\
  printf("%s%s\n", tail, NORMAL);

#define COLORIZE(bg, fg, str)						\
    printf("%s%s", bg, fg);  printf(expr); printf("%s", NORMAL);


//#define TRY(expr,  error)						\
//  bool err_event = false;						\
//  do {									\
//    expr								\
//  } while (false);							\
//  if (err_event) error							\
//  err_event = false
   
#endif

/* -------------------------------------------------------------------
   _  _ ____ ____ ____ ____    ____ _  _ ____ _  _ ___  _    ____ ____
   |  | [__  |__| | __ |___    |___  \/  |__| |\/| |__] |    |___ [__ 
   |__| ___] |  | |__] |___    |___ _/\_ |  | |  | |    |___ |___ ___]
                   
*/ 


/* THROW 
int main () {
  TRY {
        printf("A\n");

	printf("B\n");

	THROW("ERROR", true) 

	printf("X,..\n");
  } CATCH {
    printf("C!!!\n"); 

  }
    printf("X!!!\n"); 

  return 0;
} ===> Process *binary* exited abnormally with code 1
int main () {
  TRY {
        printf("A\n");

	printf("B\n");

	THROW("ERROR", false) 

	printf("X\n");
  } CATCH {
    printf("C\n"); 

  }
    printf("D\n"); 

  return 0;
} ===> Process *binary* exited abnormally with code 1
*/






/* CATCH
int main () {
  TRY {
        printf("A\n");

	printf("B\n");

	ABORT;
	printf("X,..\n");
  } CATCH {
    printf("X!!!\n"); 

  }
    printf("C!!!\n"); 

  return 0;
}

int main () {
  TRY {
  printf("A\n");

	printf("B\n");


	printf("X,..\n");
  } CATCH {
    printf("X!!!\n"); 

  }
    printf("C!!!\n"); 

  return 0;
}

int main () {
  TRY {
        printf("A\n");

	printf("B\n");
	THROW("ERROR\n", false);

	printf("X,..\n");

  } CATCH {
    printf("C!!!\n"); 

  }
    printf("C!!!\n"); 

  return 0;
}
*/
/* ABORT
int main () {
  TRY {
        printf("A\n");
	printf("B\n");
	ABORT;
	printf("X,..\n");

  } EXIT_ON_ERROR(EXIT_FAILURE);

    printf("C!!!\n"); 
  return 0;

int main () {
  TRY {
        printf("A\n");
	printf("B\n");
	ABORT;
	printf("X,..\n");

  } CATCH { 
	printf("X,..\n");
  }

    printf("C!!!\n"); 
  return 0;
}
*/

/* CONT_ON_ERROR
int main () {
  TRY {
        printf("A\n");
	printf("B\n");

	THROW("ERROR\n", false);

	printf("X\n");

  } CONT_ON_ERROR);

    printf("C\n"); 
  return 0;

int main () {
  TRY {
        printf("A\n");
	printf("B\n");
	printf("C\n");

  } CONT_ON_ERROR;

    printf("D\n"); 
  return 0;
}



*/

/* EXIT_ON_ERROR
int main () {
  TRY {
        printf("A\n");
	printf("B\n");

	THROW("ERROR\n", false);
	
	printf("X\n");

  } EXIT_ON_ERROR(EXIT_FAILURE);

    printf("X\n"); 
  return 0;

int main () {
  TRY {
        printf("A\n");
	printf("B\n");
	printf("C,..\n");

  } EXIT_ON_ERROR(EXIT_FAILURE);

    printf("D!!!\n"); 
  return 0;
}
*/
