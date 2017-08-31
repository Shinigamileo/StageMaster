/*
 * ----------------------------------------------------------------------------
 * "THE NO-WARE LICENSE" (Revision -0):
 * Léo Barré wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff.
 * ...That's it.
 * ----------------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bittab.h>



#ifdef MINIZINC /*----------------------------------------------------------*/

#define MODE "MINIZINC"

#define BEFORE_SUBSETS(V)    u8 bools = 0
#define BEFORE_A_SUBSET(V,v)						\
  if (bools&2) printf(","); else bools|=2; bools&=2; printf("{")
#define BEFORE_IN_SET(V,v,i) 
#define IF_IN_SET(V,v,i)					\
  if (bools&1) printf(","); else bools|=1; printf("%s",V[i])
#define IF_NOT_IN_SET(V,v,i) 
#define AFTER_IN_SET(V,v,i)  
#define AFTER_A_SUBSET(V,v)  printf("}")
#define AFTER_SUBSETS(V)     

#elif PARAMETERS /*---------------------------------------------------------*/

#define MODE "PARAMETERS"

#define BEFORE_SUBSETS(V)    u8 bools = 0
#define BEFORE_A_SUBSET(V,v)						\
  if (bools&2) printf(" "); else bools|=2; bools&=2; printf("\"")
#define BEFORE_IN_SET(V,v,i)
#define IF_IN_SET(V,v,i)					\
  if (bools&1) printf(" "); else bools|=1; printf("%s",V[i])
#define IF_NOT_IN_SET(V,v,i)
#define AFTER_IN_SET(V,v,i)
#define AFTER_A_SUBSET(V,v)  printf("\"")
#define AFTER_SUBSETS(V)

#else/*---------------------------------------------------------------------*/

#define MODE "DEFAULT"

#define BEFORE_SUBSETS(V)
#define BEFORE_A_SUBSET(V,v) 
#define BEFORE_IN_SET(V,v,i)
#define IF_IN_SET(V,v,i)     printf("%s ",V[i])
#define IF_NOT_IN_SET(V,v,i)
#define AFTER_IN_SET(V,v,i)
#define AFTER_A_SUBSET(V,v)  printf("\n")
#define AFTER_SUBSETS(V)

#endif/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

int main(int argc, char* argv[]){

  if (argc > 1 && !strcmp(argv[1],"--mode"))
    printf(MODE);
  else{
    u32 n = argc-1;
    char** V = argv+1;
    
    u32 lim = (1UL << n);
    
    BEFORE_SUBSETS(V);
    for (u32 v = 0; v < lim; ++v){
      BEFORE_A_SUBSET(V,v);
      for (u32 i = 0; i < n; ++i){
	BEFORE_IN_SET(V,v,i);
	if (BIT_get(32,v,i)){
	  IF_IN_SET(V,v,i);
	}
	else{
	  IF_NOT_IN_SET(V,v,i);
	}
	AFTER_IN_SET(V,v,i);
      }
      AFTER_A_SUBSET(V,v);
    }
    AFTER_SUBSETS(V);
  }
  return EXIT_SUCCESS;
}
