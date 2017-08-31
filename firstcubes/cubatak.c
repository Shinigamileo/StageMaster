/*
 * ----------------------------------------------------------------------------
 * "THE NO-WARE LICENSE" (Revision -0):
 * Léo Barré wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff.
 * ...That's it.
 * ----------------------------------------------------------------------------
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <bittab.h>
#include <utils.h>

#include <cubatak.h>



    /*************************************/
   /*                                   */
  /*               CUBE                */
 /*                                   */
/*************************************/

/* -------------------------------------------------------------------------- */

void cube_add(cube* c, u32 v){
  check_err(v < c->ivsize,
	    "cube's cipher's variables range from 0 to %u",c->ivsize-1);
  check_err(c->CVlen < CUBE_SIZE,
	    "cube full. default size = %u",CUBE_SIZE);
  c->CV[c->CVlen++] = v;
}

/* -------------------------------------------------------------------------- */

void cube_del(cube* c, u32 v){
  for (int i = 0; i < c->CVlen; ++i)
    if (c->CV[i] == v){
      c->CV[i] = c->CV[--c->CVlen];
    }
}

/* -------------------------------------------------------------------------- */

void data_dump(u8 *eqs, u8 *report, cube *c, u32 opts, u32 from, u32 to){

  u32 length = to-from+1;
  u64 timobius = 1UL << (c->CVlen*COPS_mobius_get(opts));
  
  u8 digs = 0;
  for (u32 l = length-1; l; l/=10)
    ++digs;
  
#define CDAT_CUBE   c
#define CDAT_LENGTH length
#define CEQS_DATAS  eqs
#define CREP_DATAS  report

  
  for (u64 acc = 0, v = timobius-1; acc < timobius; ++acc, --v){
  
    u32 zero_alle = 0;
    u32 zero_last = 0;
    u32 zero_freq = 0;
    
    u32 nb_linstnt = 0;
    u32 nb_quadrup = 0;
    
    printf(ANSI_BOLD""ANSI_F_BLUE
	   "     _______________________________\n"
	   "    /  _________________________   /\n"
	   "   /  /                        /  /\n"
	   "  /  /          "ANSI_INVERSE"CUBE"ANSI_INVERSE_off"          /  /\n"
	   " /  /________________________/  /\n"
	   "/______________________________/\n"
	   " "ANSI_F_DEFAULT""ANSI_UNDERLINE"{");
    for (u32 i = 0; i < c->CVlen; ++i)
      if (!BIT_get(64,v,i))
	printf(" %u",c->CV[i]);
    puts(" }\n"ANSI_RESET);
    for (u32 i = 0; i < length; ++i){
    
      // First line: informations
      printf(ANSI_INVERSE"[%0*u]"ANSI_RESET": "ANSI_UNDERLINE,digs,from+i);
      if (COPS_blr_get(opts)){
	if     (!UBTAB_get(8,CREP_affine(v),i))
	  printf(ANSI_F_MAGENTA "quadratic?"ANSI_RESET);
	else if (UBTAB_get(8,CREP_deg1(v),i))
	  printf(ANSI_F_GREEN     "linear"  ANSI_RESET);
	else
	  printf(ANSI_F_CYAN     "constant" ANSI_RESET);
      }
      printf("\n"ANSI_INVERSE" "ANSI_RESET"\t");
      
      // Second line: equation
      if (UBTAB_get(8,CREP_deg1(v),i) || !UBTAB_get(8,CREP_affine(v),i)){
	u32 k = 0;
	if (UBTAB_get(8,CREP_affine(v),i)){
	  ++nb_linstnt;
	  for (; !UBTAB_get(8,CEQS_coeff(v,k),i); ++k);
	  printf("k%u",k++);
	}
	else{
	  ++nb_quadrup;
	  printf(ANSI_F_MAGENTA"P2(k)  "ANSI_RESET);
	}
	for (; k < c->keysize; ++k)
	  if (UBTAB_get(8,CEQS_coeff(v,k),i)) printf(" + k%u",k);
	if (UBTAB_get(8,CEQS_constnt(v),i))  printf(" + 1");
      }
      else{
	++nb_linstnt;
	printf("%u",UBTAB_get(8,CEQS_constnt(v),i));
	if (!from && !UBTAB_get(8,CEQS_constnt(v),i)){
	  if (zero_alle == i) ++zero_alle;
	  ++zero_freq;
	  zero_last = i;
	}
      }
      puts("\n");
    }
    if (!from)
      printf("ZEROS:\n"
	     "all until:\t%u\n"
	     "last at:\t%u\n"
	     "number:\t\t%u\n\n",
	     zero_alle,zero_last,zero_freq);
    printf("deg < 2: %u\n"
	   "    > 1: %u\n\n",
	   nb_linstnt,nb_quadrup);
  }
  
#undef CDAT_CUBE
#undef CDAT_LENGTH
#undef CEQS_DATAS
#undef CREP_DATAS
}





/* -------------------------------------------------------------------------- */



///////////////////////////
//                       //
//      CUBE ATTACK      ///////////
//                       //      //                                  
///////////////////////////     //                                  
                               /////////////////////////////////////////
                                                                     //
                                                                    //
u8 cube_attack( cube *c, u32 opts, u8 *eqs, u8 *report,         ////////////////
	        u8 *biv, u32 warmup, u32 length         )      //             //
                                                              //              //
                                                      /////////               //
                                                     //                       //
//////////////////////////////////////////////////////                        //
                                                                              //
{                                                                             //
                                                                              //
  /* timobius: 1 if no mobius, 2^(c->CVlen) else                         */   //
  u64 timobius = 1UL << (c->CVlen*COPS_mobius_get(opts));                     //
  /* key:      table we'll use to push the keys                          */   //
  u8 key [UBTAB_SIZE(8,c->keysize)]; UBTAB_fill(8,key,c->keysize,0);          //
  /* iv:       table we'll use to push the ivs                           */   //
  u8 iv  [UBTAB_SIZE(8,c->ivsize )];                                          //
  if (!biv) UBTAB_fill(8,iv, c->ivsize, 0);                                   //
  else UBTAB_pour(8,biv,iv,c->ivsize);                                        //
  /* stream:   table we'll use to gather the different ciphers           */   //
  u8 stream[UBTAB_SIZE(8,length)];                                            //
                                                                              //
  srand(time(NULL));                                                          //
                                                                              //
#define CDAT_CUBE   c                                                         //
#define CDAT_LENGTH length                                                    //
#define CEQS_DATAS  eqs                                                       //
#define CREP_DATAS  report                                                    //
                                                                              //
  for (u64 v = 0; v < timobius; ++v)                                          //
    UBTAB_fill(8,CEQS_constnt(v),length,0); // constant row is empty          //
                                                                              //
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#ifdef UMP                                                                    //
  puts("////////////////////////////////////////");                           //
  puts("//                                    //");                           //
  puts("//      STEP 1: get the constant      //");                           //
  puts("//                                    //");                           //
  puts("////////////////////////////////////////");                           //
#endif                                                                        //
                                                                              //
  cube_squeeze( c, key, iv, stream, length, timobius,                         //
		v, CEQS_constnt(v));                                          //
                                                                              //
  // coeff[0] = coeff[1] = ... = coeff[c->keysize]                            //
  for (u64 v = 0; v < timobius; ++v)                                          //
    for (u32 k = 0; k < c->keysize; ++k)                                      //
      UBTAB_pour(8,CEQS_constnt(v),CEQS_coeff(v,k),length);                   //
                                                                              //
                                                                              //
                                                                              //
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#ifdef UMP                                                                    //
  puts("///////////////////////////////////////////////////");                //
  puts("//                                               //");                //
  puts("//      STEP 2: get the different monomials      //");                //
  puts("//                                               //");                //
  puts("///////////////////////////////////////////////////");                //
#endif                                                                        //
                                                                              //
  for (u8 k = 0; k < c->keysize; ++k){                                        //
    UBTAB_fill(8,key,c->keysize,0);                                           //
    UBTAB_xet(8,key,k,1);                                                     //
    cube_squeeze( c, key, iv, stream, length, timobius,                       //
		  v, CEQS_coeff(v,k));                                        //
  }                                                                           //
                                                                              //
                                                                              //
                                                                              //
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#ifdef UMP                                                                    //
  puts("/////////////////////////");                                          //
  puts("//                     //");                                          //
  puts("//      STEP 3: ?      //"); //=> linearity tests                     //
  puts("//                     //");                                          //
  puts("/////////////////////////");                                          //
#endif                                                                        //
                                                                              //
  for (u64 v = 0; v < timobius; ++v){                                         //
    UBTAB_fill(8,CREP_deg1(v),length,0);                                      //
    UBTAB_fill(8,CREP_affine(v),length,1);                                    //
  }                                                                           //
                                                                              //
  if (COPS_blr_get(opts)){                                                    //
    // tests the "constantness" of all equations                              //
    for (u64 v = 0; v < timobius; ++v)                                        //
      for (u32 k = 0; k < c->keysize; ++k)                                    //
	for (u8 j = 0; j < UBTAB_SIZE(8,length); ++j)                         //
	  CREP_deg1(v)[j] |= CEQS_coeff(v,k)[j];                              //
                                                                              //
    // now linear tests                                                       //
    // Blum-Luby-Rubinfeld test: f(x) + f(y) + f(0) = f(x+y) ?                //
    // variation proposed by Fouque and Vannet for cube attack :              //
    // we construct {K_1,...,K_r}, 'r' random keys, which we use              //
    // for the BLR test (we can store each f(K_i))                            //
    // => less squeezing to do (2^v calculi can be quite slow)                //
    u8 keys[NB_KEYS_BLR][UBTAB_SIZE(8,c->keysize)];                           //
    u8 squeezes[NB_KEYS_BLR][timobius][UBTAB_SIZE(8,length)];                 //
    u8 results[timobius][UBTAB_SIZE(8,length)];                               //
    for (u8 k = 0; k < NB_KEYS_BLR; ++k){                                     //
      //  --> Constructing the keys                                           //
      for (u8 i = 0; i < UBTAB_SIZE(8,c->keysize); ++i)                       //
	keys[k][i] = (u8)rand();                                              //
      //  --> Calculating their squeezes                                      //
      for (u64 v = 0; v < timobius; ++v)                                      //
	UBTAB_fill(8,squeezes[k][v],length,0);                                //
      cube_squeeze( c, keys[k], iv, stream, length, timobius,                 //
		    v, squeezes[k][v]);                                       //
    }                                                                         //
    //  -=> Now testing all configs                                           //
    for (u8 k1 = 0; k1 < NB_KEYS_BLR-1; ++k1)                                 //
      for (u8 k2 = k1+1; k2 < NB_KEYS_BLR; ++k2){                             //
	for (u64 v = 0; v < timobius; ++v){                                   //
	  UBTAB_pour(8,CEQS_constnt(v),results[v],length);                    //
	  UBTAB_xor(8,results[v],results[v],squeezes[k1][v],length);          //
	  UBTAB_xor(8,results[v],results[v],squeezes[k2][v],length);          //
	}                                                                     //
	UBTAB_pour(8,keys[k1],key,c->keysize);                                //
	UBTAB_xor(8,key,key,keys[k2],c->keysize);                             //
	cube_squeeze( c, key, iv, stream, length, timobius,                   //
		      v, results[v]);                                         //
	for (u64 v = 0; v < timobius; ++v)                                    //
	  for (u8 i = 0; i < UBTAB_SIZE(8,length); ++i)                       //
	    CREP_affine(v)[i] &= ~results[v][i];                              //
      }                                                                       //
  }                                                                           //
                                                                              //
                                                                              //
                                                                              //
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#ifdef UMP                                                                    //
  puts("/////////////////////////");                                          //
  puts("//                     //");                                          //
  puts("//   STEP 4: profits   //");                                          //
  puts("//                     //");                                          //
  puts("/////////////////////////");                                          //
#endif                                                                        //
                                                                              //
  return 0;                                                                   //
                                                                              //
#undef CDAT_CUBE                                                              //
#undef CDAT_LENGTH                                                            //
#undef CEQS_DATAS                                                             //
#undef CREP_DATAS                                                             //
}                                                                             //
                                                                              //
////////////////////////////////////////////////////////////////////////////////




/* -------------------------------------------------------------------------- */

//////////////////////////
//                      //
//         MAIN         //
//                      //
////////////////////////// 

#ifndef ECRYPT_MAXKEYSIZE
#define ECRYPT_MAXKEYSIZE 80
#endif//ECRYPT_MAXKEYSIZE
#ifndef ECRYPT_MAXIVSIZE
#define ECRYPT_MAXIVSIZE 80
#endif//ECRYPT_MAXIVSIZE

static inline int usage(const char* proc, int status){
  if (status == EXIT_FAILURE)
    printf(ANSI_F_RED"\nOption not recognized\n"ANSI_F_DEFAULT);
  printf( "Usage: %s [OPTIONS] [-from 'b'] [-to 'l'] [-CV 'v1' 'v2' ...]\n\n",
	  proc );
  if (status == EXIT_FAILURE)
    printf( ANSI_F_CYAN"%s -h"ANSI_F_DEFAULT" to display help\n\n",
	    proc);
  else
    printf(ANSI_UNDERLINE"Options:\n"ANSI_UNDERLINE_off
	   
	   ANSI_BOLD"-h"ANSI_BOLD_off
	   "\t\tDisplay this help\n"
	   
	   ANSI_BOLD"-noBLR"ANSI_BOLD_off
	   "\t\tPrevent linearity testing\n"
	   
	   ANSI_BOLD"-Moebius"ANSI_BOLD_off
	   "\tConstruct all sub-cubes\n"
	   
	   ANSI_BOLD"-from"ANSI_BOLD_off
	   "\t\tPrecise the first round to consider (default: 0)\n"
	   
	   ANSI_BOLD"-to"ANSI_BOLD_off
	   "\t\tPrecise the last round to consider (default: 1152)\n"
	   
	   ANSI_BOLD"-CV"ANSI_BOLD_off
	   "\t\tCube's set of IVs\n"
	   
	   "\n");
  exit(status);
}

int main(int argc, char **argv){
  ECRYPT_init();

  u32 _keysize = ECRYPT_MAXKEYSIZE;
  u32 _ivsize  = ECRYPT_MAXIVSIZE;

  u32 _from = 0;
  u32 _to = 1152;
  u32 _opts = 0;
  COPS_blr_set(_opts,1);
  COPS_mobius_set(_opts,0);
  
  cube rubik;
  cube_init(rubik,_keysize,_ivsize);

  u8 iv[UBTAB_SIZE(8,_ivsize)];
  UBTAB_fill(8,iv,_ivsize,0);

  while (optind < argc){
    if (check_opt("-h")){
      usage(argv[0],EXIT_SUCCESS);
    }
    if (check_opt("-to")){
      _to = atoi(argv[++optind]);
      ++optind;
    }
    else if (check_opt("-from")){
      _from = atoi(argv[++optind])/128*128; // Oui, c'est moche.
      ++optind;
    }
    else if (check_opt("-noBLR")){
      COPS_blr_set(_opts,0);
      ++optind;
    }
    else if (check_opt("-Moebius")){
      COPS_mobius_set(_opts,1);
      ++optind;
    }
    else if (check_opt("-IV")){
      while (++optind < argc && argv[optind][0] != '-')
	UBTAB_set(8,iv,atoi(argv[optind]),1);
    }
    else if (check_opt("-CV")){
      while (++optind < argc && argv[optind][0] != '-')
	cube_add(&rubik,atoi(argv[optind]));
    }
    else{
      usage(argv[0],EXIT_FAILURE);
    } 
  }

  
#define CDAT_CUBE   (&rubik)
#define CDAT_LENGTH (_to - _from + 1)
#define CEQS_DATAS  eqs

  
  printf("\nkey's size: %u\n"
	 "iv's  size: %u\n\n"
	 "cube's size: %u",
	 _keysize,_ivsize,rubik.CVlen);
  
  for (u8 j = 0; j < rubik.CVlen; ++j){
    if (!(j%20))
      printf("\n    ");
    printf(" %02u", rubik.CV[j]);
  }
  printf("\n\nfrom %u to %u\n\n",_from,_to);

  printf("default IV:\n");
  UBTAB_print(8,iv,rubik.ivsize); puts("\n");

  printf("modes:\t");
  if (COPS_blr_get(_opts))
    printf(ANSI_F_GREEN"BLR"ANSI_F_DEFAULT);
  else
    printf(ANSI_F_RED"No BLR"ANSI_F_DEFAULT);
  printf("   ");
  if (COPS_mobius_get(_opts))
    printf(ANSI_F_GREEN"Möbius"ANSI_F_DEFAULT);
  else
    printf(ANSI_F_RED"No Möbius"ANSI_F_DEFAULT);
  puts("\n");
  
  u8 *eqs    = malloc(CEQS_SIZE(_opts)*sizeof(u8));
  u8 *report = malloc(CREP_SIZE(_opts)*sizeof(u8));

  printf("%lu\n\n",SIZE_MAX);
  
  printf("%lu\n",  CEQS_SIZE(_opts)*sizeof(u8));
  printf("%lu\n\n",sizeof(eqs));
  printf("%lu\n",  CREP_SIZE(_opts)*sizeof(u8));
  printf("%lu\n\n",sizeof(report));
  
  cube_attack(&rubik,_opts,eqs,report,iv,_from/128,CDAT_LENGTH);
  
  data_dump(eqs,report,&rubik,_opts,_from,_to);

  free(eqs   );
  free(report);

  exit(EXIT_SUCCESS);
}
