/*
 * ----------------------------------------------------------------------------
 * "THE NO-WARE LICENSE" (Revision -0):
 * Léo Barré wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff.
 * ...That's it.
 * ----------------------------------------------------------------------------
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <ecrypt-scipher.h>

#include <utils.h>

#define _X2						\
  "\u2571\u2572 \u2571\u2572\n"				\
  "\u2572 \u2573 \u2571\u2577   2\n"			\
  " \u2573 \u2573 \u251c\u256e\u257f\n"			\
  "\u2571 \u2573 \u2572\u2575\u2575\u2575\n"		\
  "\u2572\u2571 \u2572\u2571\n"				\

#ifndef WTC // What To Count : by default, the non-neutrality
#define WTC 1
#endif//WTC
  
#define keysize 80
#define  ivsize 80

#define OPDFAULT_r   1200
#define OPDFAULT_n   100
#define OPDFAULT_iv							\
  "c00c00c00c00c00c00c00c00c00c00c00c00c00c00c00c00c00c00c00c00c00c00c00c00n00n00n0"
#define OPDFAULT_key							\
  "r00r00r00r00r00r00r00r00r00r00r00r00r00r00r00r00r00r00r00r00r00r00010r00r00r00r0"
  

static inline int usage(char* argv[], int index, int status){
  if (status == EXIT_FAILURE)
    fprintf(stderr,ANSI_F_RED"\nOption '%s' not recognized\n"ANSI_F_DEFAULT,
	    argv[index]);
  fprintf(stderr,"Usage: %s  [-k KEY_MODEL] [-x IV_MODEL] [OPTIONS] [-r ROUND] [-n NBTESTS]\n\n",
	  argv[0] );
  if (status == EXIT_FAILURE)
    fprintf(stderr,
	    ANSI_F_CYAN"%s -h"ANSI_F_DEFAULT" to display help\n\n",
	    argv[0]);
  else
    fprintf(stderr,
	    ANSI_UNDERLINE"Options:"ANSI_UNDERLINE_off"\n\n"
	   
	    " "ANSI_BOLD"-h"ANSI_BOLD_off"\n"
	    "Display this help\n"
	    "\n"
	    
	    " "ANSI_BOLD"-x IV_MODEL"ANSI_BOLD_off"\n"
	    "Model of IV to cube on\n"
	    "keys: - c   : cube's term bit\n"
	    "      - n   : cube's neutral bit\n"
	    "      - 0,1 : fixed bit\n"
	    "(default: %s)\n"
	    "\n"
	    
	    " "ANSI_BOLD"-k KEY_MODEL"ANSI_BOLD_off"\n"
	    "Model of key to test\n"
	    "keys: - r   : random bit\n"
	    "      - 0,1 : fixed bit\n"
	    "(default: %s)\n"
	    "\n"
	    
	    " "ANSI_BOLD"-r ROUND"ANSI_BOLD_off"\n"
	    "Round, until which to test (default: %u)\n"
	    "\n"
	    
	    " "ANSI_BOLD"-n NBKEYS"ANSI_BOLD_off"\n"
	    "Number of keys to generate (default: %u)\n"
	    "\n"
	    
	    " "ANSI_BOLD"-t SIZE"ANSI_BOLD_off"\n"
	    "Defines the size of each experiment of the test\n"
	    "(must divide NBKEYS, default: NBKEYS)\n"
	    "\n"
	    
	    " "ANSI_BOLD"-o [FILE]"ANSI_BOLD_off"\n"
	    "Writes output in FILE\n"
	    "If FILE isn't defined, writes in file "
	    "\"<IV_MODEL>_<KEY_MODEL>_<ROUND>.kw\"\n"
	    "Doesn't overwrites if the file already exists\n"
	    "\n"
	    
	    " "ANSI_BOLD"--noweak"ANSI_BOLD_off"\n"
	    "Run tests through all kind of keys\n"
	    "\n"
	    
	    " "ANSI_BOLD"--seed SEED"ANSI_BOLD_off"\n"
	    "Seed to give to Random Numbers Generator\n"
	    "\n"
	    
	    ,OPDFAULT_iv,OPDFAULT_key,OPDFAULT_r,OPDFAULT_n);
  exit(status);
}
  

int main(int argc, char *argv[]){
  
  ////////////////////////////////////////
  //                                    //
  //                HEAD                //
  //                                    //
  ////////////////////////////////////////
  
  u32 length      = OPDFAULT_r + 1; // stream's length
  u32 tests_nb    = OPDFAULT_n;     // number of total tests
  u32 tests_step  = OPDFAULT_n;
  char * iv_model = OPDFAULT_iv;
  char *key_model = OPDFAULT_key;
  FILE *output    = stdout;
  u32 seed = time(NULL);

#define option_set(optab,option,val) BIT_set(32,optab,CAT(option,_optind),val)
#define option_get(optab,option)     BIT_get(32,optab,CAT(option,_optind))
#define trans_optind  0 // transitionnal results displayed ?
#define outpt_optind  1 // output somewhere else ?
#define nfile_optind  2 // output is a new file ?
  u32 optab = 0;  

  for (u32 optind = 1; optind < argc; ++optind){
    if (check_opt("-h")){
      usage(argv,optind,EXIT_SUCCESS);
    }
    else if (check_opt("-r")){
      length = atoi(argv[++optind]) + 1;
    }
    else if (check_opt("-n")){
      tests_nb = atoi(argv[++optind]);
      if (!option_get(optab,trans))
	tests_step = tests_nb;
    }
    else if (check_opt("-t")){
      option_set(optab,trans,1);
      tests_step = atoi(argv[++optind]);
    }
    else if (check_opt("-x")){
      iv_model = argv[++optind];
    }
    else if (check_opt("-k")){
      key_model = argv[++optind];
    }
    else if (check_opt("--noweak")){
      key_model = "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr";
    }
    else if (check_opt("--seed")){
      seed = atoi(argv[++optind]);
    }
    else if (check_opt("-o")){
      option_set(optab,outpt,1);
      if (optind+1 < argc && argv[optind+1][0] != '-'){
	if ((output = fopen(argv[++optind],"r"))){
	  fclose(output);
	  option_set(optab,nfile,1);
	}
	output = fopen(argv[optind],"a");
      }
    }
    else{
      usage(argv,optind,EXIT_FAILURE);
    } 
  }

  check_err(!option_get(optab,trans) || tests_nb % tests_step == 0,
	    "step for tests (here %d) must divide the number of tests (here %d)",
	    tests_step,tests_nb);


  
  ////////////////////////////////////////
  //                                    //
  //                NECK                //
  //                                    //
  ////////////////////////////////////////
  
  srand(seed);

  // construction of iv model
  u8 cube_ens[ivsize];
  u64 cube_size = 0;
  u8   ntr_ens[ivsize];
  u64  ntr_size = 0;
  u8 iv[UBTAB_SIZE(8,ivsize)];
  for (u8 i = 0; i < ivsize; ++i)
    switch(iv_model[i]){
    case 'c': /* cube */
      cube_ens[cube_size++] = ivsize-1-i;
      break;
    case 'n': /* neutral */
      ntr_ens[ntr_size++] = ivsize-1-i;
      break;
    case '0':
    case '1':
      UBTAB_set(8,iv,ivsize-1-i,(iv_model[i]-'0'));
      break;
    default:
      check_err(0,
		"IV bit %d '%c' not allowed. Characters accepted : 0 1 c n",
		i, iv_model[i]);
      break;
    }

  // construction of weak key model
  u8 key[UBTAB_SIZE(8,keysize)];
  u8 key_morphr[UBTAB_SIZE(8,keysize)];
  u8 key_weaker[UBTAB_SIZE(8,keysize)];
  for (u8 i = 0; i < keysize; ++i)
    switch(key_model[i]){
    case 'r': /* random */
      UBTAB_set(8,key_morphr,keysize-1-i,1);
      UBTAB_set(8,key_weaker,keysize-1-i,0);
      break;
    case '0':
    case '1':
      UBTAB_set(8,key_morphr,keysize-1-i,0);
      UBTAB_set(8,key_weaker,keysize-1-i,(key_model[i]-'0'));
      break;
    default:
      check_err(0,
		"Key bit %d '%c' not allowed. Characters accepted : 0 1 r",
		i, key_model[i]);
      break;
    }

  // where to output
  if (option_get(optab,outpt) && output == stdout){
    char tmp_buffer[256];
    sprintf(tmp_buffer,"%s_%s_%u.kw",
	    iv_model, key_model, length-1);
    if ((output = fopen(tmp_buffer,"r"))){
      fclose(output);
      option_set(optab,nfile,1);
    }
    output = fopen(tmp_buffer,"a");
  }
  
  //clock_t t1, t2;  // to measure time spent

  ECRYPT_ctx ctx;                             // Trivium's context
  u8 streams[ntr_size+1][UBTAB_SIZE(8,length)]; // to store Trivium's streams
  u8 tmp_stream[UBTAB_SIZE(8,length)];        // other storage, but temporarier

  u32* hcount_t = malloc(ntr_size*length*sizeof(*hcount_t));
#define hcount(i,j) /* hcount[nbntr][length] */	\
    hcount_t[i*length + j]

  char time_buffer[32];
  u32 time_ms;
  struct tm* time_info;
  struct timeval time_val;
#define whenwerewe()							\
  MACRO(gettimeofday(&time_val, NULL);					\
	time_ms = lrint(time_val.tv_usec/1000.0);			\
	if (time_ms >= 1000){						\
	  time_ms -= 1000;						\
	  time_val.tv_sec++;						\
	}								\
	time_info = localtime(&time_val.tv_sec);			\
	strftime(time_buffer, 32, "%Y/%m/%d %H:%M:%S", time_info);	\
	fprintf(output,"%s:%03u",time_buffer,time_ms);)
  
  //////////////////////////////////////
  //                                  //
  //               BODY               //
  //                                  //
  //////////////////////////////////////
  
  ECRYPT_init();

  if (!option_get(optab,nfile))
    fprintf(output,
	    "%d rounds\n"
	    "%s\n"
	    "%s\n"
	    "#\n",
	    length-1,
	    iv_model,
	    key_model
	    );
  else
    fprintf(output,
	    "----------------------------------------"
	    "----------------------------------------"
	    "\n");
  fflush(output);
  
  for (int testd = 0; testd < tests_nb; testd += tests_step){
    
    for (u32 i = 0; i < ntr_size; ++i)
      for (u32 j = 0; j < length; ++j)
	hcount(i,j) = 0;
      
    for (int testu = 0; testu < tests_step; ++testu){

      /* Creation of key according to model */
      for (u8 k = 0; k < UBTAB_SIZE(8,keysize); ++k)
	key[k] = ((u8)rand() & key_morphr[k]) | key_weaker[k];

      fprintf(output,"  ");
      whenwerewe();
      fprintf(output," | ");
      UBTAB_fprint(output,8,key,keysize);
      fprintf(output,"\n");
      
      //t1 = clock();
      
      /* Higher-order differential (or cube) part */
      ECRYPT_keysetup(&ctx,key,keysize,ivsize);
      for (u8 v = 0; v < ntr_size+1; ++v){
	for (u8 i = 0; i < ntr_size; ++i)
	  UBTAB_set(8,iv,ntr_ens[i],BIT_get(64,1<<v,i));
	UBTAB_fill(8,streams[v],length,0);
        
	for (u64 vc = 0; vc < (1 << (cube_size)); ++vc){
	  for (u8 i = 0; i < cube_size; ++i)
	    UBTAB_set(8,iv,cube_ens[i],BIT_get(64,vc,i));
	  ECRYPT_ivsetup(&ctx,iv);
	  
	  ECRYPT_keystream_bytes(&ctx,tmp_stream,UBTAB_SIZE(8,length));
	  UBTAB_xor(8,streams[v],streams[v],tmp_stream,length);
	}
      }
      /* Adding to count */
      for (int i = 0; i < ntr_size; ++i){
	UBTAB_xor(8,tmp_stream,streams[i],streams[ntr_size],length);
	for (int il = 0; il < length; ++il)
	  hcount(i,il) += (1-WTC) ^ UBTAB_get(8,tmp_stream,il);
      }
    
      //t2 = clock();
      //fprintf(output," %.3fs\n",(float)(t2-t1)/CLOCKS_PER_SEC);
    }
      
    /* Display transitionnal results */
      
    fprintf(output,"# ");
    whenwerewe();
    fprintf(output,"\n");
    for (u32 i = 0; i < length; ++i){
      for (u32 j = 0; j < ntr_size; ++j)
	fprintf(output,"%u ",hcount(j,i));
      fprintf(output,"\n");
    }
    fprintf(output,"#\n");
    fflush(output);
      
  }
  
  free(hcount_t);
  
}
