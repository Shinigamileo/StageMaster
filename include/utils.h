/*
 * ----------------------------------------------------------------------------
 * "THE NO-WARE LICENSE" (Revision -0):
 * Léo Barré wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff.
 * ...That's it.
 * ----------------------------------------------------------------------------
 */
#ifndef UTILS_H
#define UTILS_H

#include "ansicodes.h"

/////////////////////////////////////////////////////////////////////////
#ifdef  _STDIO_H                                                       //
// If the test isn't satisfied, display the error message and exit     //
// Note: not optimized, so if nothing to put as the __VA_ARGS__...     //
//       still put something, or it won't compile                      //
#define check_err(test,errmsg,...)					\
  MACRO(if (! (test)){							\
      fprintf(stderr,							\
	      ANSI_F_RED "[ERROR]" ANSI_RESET " : " errmsg "\n",	\
	      __VA_ARGS__);						\
      exit(EXIT_FAILURE);						\
    } )                                                                //
#endif//_STDIO_H                                                       //
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
#ifndef _GETOPT_H                                                      //
#ifdef  _STRING_H                                                      //
// To easily get options without the usefull but heavy getopt.h        //
int optind = 1;                                                        //
#define check_opt(opt) (optind < argc && !strcmp(argv[optind],opt))    //
#endif//_STRING_H                                                      //
#endif//_GETOPT_H                                                      //
/////////////////////////////////////////////////////////////////////////
  
// Concatenate 'a' and 'b'
#define _hidden_CAT(a,b) a ## b
#define CAT(a,b) _hidden_CAT(a,b)

// Used only to prevent gcc to spot an error when not using return value
#define IGNORE(f)	       ((void)(f+1))

// Used to force you to put a semicolon at the end of a macro. Sucker !
#define MACRO(corpse)          do{ corpse }while(0)

// Returns the last line (as in a functionnal programming language)
#define FUNC(corpse)           ({ corpse })
// I hear you say "but Rick, haven't you heard about 'static inline' ?"
// and I tell you : "MY NAME'S NOT RIIIIIIICK !"

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

#endif//UTILS_H
