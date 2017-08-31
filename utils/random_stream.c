/*
 * ----------------------------------------------------------------------------
 * "THE NO-WARE LICENSE" (Revision -0):
 * Léo Barré wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff.
 * ...That's it.
 * ----------------------------------------------------------------------------
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include <bittab.h>


#ifndef EFAULT_SIZE
#define EFAULT_SIZE 80
#endif//EFAULT_SIZE


#ifdef MINIZINC /*----------------------------------------------------------*/

#define print_stream(stream,l)				\
  MACRO( printf("[");					\
	 for (u32 i = 0; i < l; ++i)			\
	   printf("%s%u",				\
		  i?",":"",UBTAB_get(32,stream,i));	\
	 printf("]"); )

#else/*---------------------------------------------------------------------*/

#define print_stream(stream,l)			\
  UBTAB_print(32,stream,l)

#endif/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

int main (int argc, char* argv[]){

  u32 l = EFAULT_SIZE;
  u32 seed = time(NULL);
  
  int optind = 1;
  while (optind < argc && argv[optind][0] == '-'){
    if (!strcmp(argv[optind],"-seed")){
      seed = atoi(argv[++optind]);
      ++optind;
    }
    else if (!strcmp(argv[optind],"-l")){
      l = atoi(argv[++optind]);
      ++optind;
    }
  }

  srand(seed);

  u32 stream[UBTAB_SIZE(32,l)];
  for (u32 i = 0; i < UBTAB_SIZE(32,l); ++i)
    stream[i] = rand();
  print_stream(stream,l);
}
