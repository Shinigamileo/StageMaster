/*
 * ----------------------------------------------------------------------------
 * "THE NO-WARE LICENSE" (Revision -0):
 * Léo Barré wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff.
 * ...That's it.
 * ----------------------------------------------------------------------------
 */
#ifndef CUBE_H
#define CUBE_H

#include <stdlib.h>
#include <time.h>

#include <ecrypt-scipher.h>

#define CUBE_SIZE 63    // default size of cube

// number of tests to be done to prove linearity (according to each test)
#define NB_LINTEST    20
#define NB_KEYS_BLR   10





/////////////////////////////////////////////////////////////
//                                                         //
//                        CUBE FCTS                        //
//                                                         //
/////////////////////////////////////////////////////////////

// Here lies the main structure of a cube (Orignal Cube : dont steel)
typedef struct{
  
  u32 keysize;    // size of the stream cipher's key
  u32 ivsize;     // size of the stream cipher's IV (initial value)
  
  u32 CV[CUBE_SIZE]; // CV: vars which will form the maxterm
  u8  CVlen;         // number of vars in CV
  
  u32 SV[CUBE_SIZE]; // SV: vars which will be used for tests
  u8  SVlen;         // number of vars in SV
  
} cube;


/* given a cube, initializes it properly */
#define cube_init(c,keyseyes,ivseyes)		\
  srand(time(NULL));				\
  MACRO( c.keysize = keyseyes;			\
	 c.ivsize  = ivseyes;			\
	 c.CVlen   = 0;				\
	 c.SVlen   = 0;				\
	 )


void cube_add(cube* c, u32 v); // add a variable in the cube
void cube_del(cube* c, u32 v); // remove a variable from the cube





/////////////////////////////////////////////////////////////
//                                                         //
//                         OPTIONS                         //
//                                                         //
/////////////////////////////////////////////////////////////

typedef u32 c_opts;

// #define COPS_DATAS
#define COPS_general_get(mode,o)   BIT_get(32,o,COPS_##mode##_index)
#define COPS_general_set(mode,o,i) BIT_set(32,o,COPS_##mode##_index,i)

//
// General
//

#define COPS_mobius_index 0
#define COPS_mobius_get(o)   COPS_general_get(mobius,o)
#define COPS_mobius_set(o,i) COPS_general_set(mobius,o,i)

//
// Cube Attack
//

#define COPS_blr_index 1
#define COPS_blr_get(o)   COPS_general_get(blr,o)
#define COPS_blr_set(o,i) COPS_general_set(blr,o,i)

//
// Cube Test
//

#define COPS_neutral_index 1
#define COPS_neutral_get(o)   COPS_general_get(neutral,o)
#define COPS_neutral_set(o,i) COPS_general_set(neutral,o,i)





/////////////////////////////////////////////////////////////
//                                                         //
//                          DATAS                          //
//                                                         //
/////////////////////////////////////////////////////////////

// #define CDAT_CUBE 
// #define CDAT_LENGTH

//#define CEQS_DATAS
#define CEQS_SIZE(o)							\
  ((1UL<<(CDAT_CUBE->CVlen*(COPS_mobius_get(o))))			\
   *(CDAT_CUBE->keysize+1)*UBTAB_SIZE(8,CDAT_LENGTH))
#define CEQS_in(v,c)							\
  (CEQS_DATAS+((v)*(CDAT_CUBE->keysize+1)+c)*UBTAB_SIZE(8,CDAT_LENGTH))

#define CEQS_coeff(i,k) CEQS_in(i,k)                  // Value of key k_'k''s coeff
#define CEQS_constnt(i)	CEQS_in(i,CDAT_CUBE->keysize) // Value of the constant

// #define CREP_DATAS
#define CREP_SIZE(o)							\
  ((1UL<<(CDAT_CUBE->CVlen*(COPS_mobius_get(o))))			\
   *((2)*UBTAB_SIZE(8,CDAT_LENGTH)))
#define CREP_in(v,c)					\
  (CREP_DATAS+((v)*(2)+c)*UBTAB_SIZE(8,CDAT_LENGTH))

#define CREP_deg1(v)    CREP_in(v,0)  // deg 1 monomials ?
#define CREP_affine(v)  CREP_in(v,1)  // linear equation ?

void data_dump(u8 *eqs, u8 *report, cube *c, u32 opts, u32 from, u32 length);





/////////////////////////////////////////////////////////////
//                                                         //
//                         SQUEEZE                         //
//                                                         //
/////////////////////////////////////////////////////////////
/**
 * @brief squeeze the cube and produce equations
 *
 * @param c       cube to use
 * @param datas   where will lie all gathered informations
 * @param length  size of the stream
 *
 * @details: until now, datas will be splited as follow :
 *             - keys'coefficients       { c->keysize UBTABs of size length }
 *             - constants'coefficients  {      1 UBTAB of size length      }
 *             - not constant ?          {      1 UBTAB of size length      }
 *             - linear ?                {      1 UBTAB of size length      }
 *                  
 *
 * @return 0: all equations are constant
 *         1: at least on of them is linear (clarified in 'linear')
 *         2: all non constant equations are at least quadratic
 */
u8 cube_attack( cube *c, u32 opts, u8 *eqs, u8 *report,
		u8 *biv, u32 warmup, u32 length         );

/**
 * @brief squeeze the cube
 *
 * @param c        cube to use
 * @param key      cipher's key
 * @param iv       cipher's IV
 * @param stream   buffer to use
 * @param length   stream's length
 * @param tmobius  number of times
 * @param v        variable under tmobius
 * @param storage  where the informations will lay
 */
#define cube_squeeze(c,key,biv,stream,length,tmobius,v,storage)		\
  MACRO(ECRYPT_ctx __ctx;						\
	u8 __iv[UBTAB_SIZE(8,c->ivsize)];				\
	/* Setting the key (won't change) */				\
	ECRYPT_keysetup(&__ctx,key,c->keysize,c->ivsize);		\
	for(u64 _vector = 0; _vector < (1UL<<c->CVlen); ++_vector){	\
	  /* Building the IV to use  */					\
	  UBTAB_pour(8,biv,__iv,c->ivsize);				\
	  for (u64 _e = 0; _e < c->CVlen; ++_e)				\
	    UBTAB_xet(8,__iv,c->CV[_e],BIT_get(64,_vector,_e));		\
	  ECRYPT_ivsetup(&__ctx,__iv);					\
	  /* Warming up */						\
	  ECRYPT_warmup(&__ctx,warmup);					\
	  /* Gathering the informations */				\
	  ECRYPT_keystream_bytes(&__ctx,stream,UBTAB_SIZE(8,length));	\
	  /* Executing body for each */					\
	  for (u64 v = 0; v < tmobius; ++v)				\
	    if (BIT_in(64,_vector,~v))					\
	      UBTAB_xor(8,storage,storage,stream,length);		\
	})
  

#endif//CUBE_H
