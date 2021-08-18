#ifndef __EXTRA_H__
#define __EXTRA_H__

#include <stdbool.h> 
#include <err.h>
#include <stdlib.h>
#include <stdio.h>

/*  _______  ______  ______  _____   ______
    |______ |_____/ |_____/ |     | |_____/
    |______ |    \_ |    \_ |_____| |    \_
                                           
    _     _ _______ __   _ ______         _____ __   _  ______
    |_____| |_____| | \  | |     \ |        |   | \  | |  ____
    |     | |     | |  \_| |_____/ |_____ __|__ |  \_| |_____|
   _____   _____  ___________  _____ _____________ _____________
   EVENT   THROW  THROW_QUIET  CATCH EXIT_ON_ERROR CONT_ON_ERROR 
      ↑         ↖      ↑           ↖    ↑          ↗              
 [Error state][Signal Errors]       [Handle signals] 
                          ___
                          TRY  
                           ↑
                      [Environment]
 */      
                                                              
 

/* ABORT()
   
   Like ABORT, but without displaying a message, and
   setting the event number to zero
*/
#define ABORT()			\
  event = 0;			\
  break; 

/* EVENT(n) 
   
   Like THROW, but without displaying a message, and
   setting the event number to n
*/
#define EVENT(n)			\
  event = n				\


/* THROW(expr, error_msg, error_nr) 

    If the function returns a zero or less integer, set error_nr is
    equal to it print the expr, error_msg, error_nr to stderr and exit
    with failure else print error_msg to stderr and exit tryblock.
*/

#define THROW(expr, error_msg)				\
  ans = expr;								\
  event = ans < event ? ans : event;					\
  if (event <= 0){							\
    fprintf(stderr, "[Err %d] %s\n[Expr] %s",				\
	    event, error_msg, #expr); if (0 > event){			\
      exit(event);							\
    }else{ break;}     						\
  }

/*  TRY{...STOP;...} CATCH|CONT_ON_ERROR|EXIT_ON_ERROR

   Exit a TRY block, without any error.
   (Must be called outside of any loops, 
    or switch statements) inside the TRY block!
 */

#define STOP break					

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
  while(false); if(event <= 0) exit(event)

/* TRY{...} CATCH{...}

   Added after a TRY block, and will (on thrown error)
   run its code block. It will not execute if no error occurred
 */

#define CATCH while(false);if(!event)

/* TRY{...} CATCH|CONT_ON_ERROR|EXIT_ON_ERROR

   Evaluate expr, if the macro THROW is activated, it will either
   exit with failure, or run the error expr if the error was non
   critical. 
   Must be followed by on of: {CATCH, CONT_ON_ERROR, EXIT_ON_ERROR}
 */

#define TRY \
  int event = 1;do


/* STR(expr)

   Expand into the stringinized EXPR (as a const char *)   
 */
#define STR(expr) #expr


/* PERROR_EXPR(expr) 

n   Print EXPR to standard error outoput formatted 
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

#define ERROR_EXPR(fmt_type, expr, msg)					\
  fprintf(stderr, "[Error] %s\n", msg);					\
  fprintf(stderr, "[Expr]  %s\n", STR(expr));				\
  fprintf(stderr, "[Eval]");						\
  fprintf(stderr, fmt_type, expr);					\
  fprintf(stderr, "\n");						\
  exit(EXIT_FAILURE)

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

/* _  _ ____ ____  __  ____    ____ _  _ ____ _  _ ,__  _    ____ ____
   |  | [__' |__| | _' |_,'    |_,'  \/  |__| |\/| |__] |    |_,' [__' 
   |__| ,__] |  | \__] |__,    |__, _/\_ |  | |  | |    |___,|__, ,__]

*/ 


/*
int main () {
  TRY {
        printf("A\n");
	printf("B\n");
	THROW(-2 * (1 + 2);, "ERROR", true);
	printf("X\n");
  } CATCH {
    printf("C\n");    
  }
    printf("X\n"); 

    return 0;
}
> A
> B
> [Err -6] ERROR
> [Expr] -2 * (1 + 2);
> Process c-eval exited abnormally with code 250
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
