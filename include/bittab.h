/*
 * ----------------------------------------------------------------------------
 * "THE NO-WARE LICENSE" (Revision -0):
 * Léo Barré wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff.
 * ...That's it.
 * ----------------------------------------------------------------------------
 */
#ifndef _BITTAB_H
#define _BITTAB_H

#include <stdio.h>
#include <stdint.h>

#include "utils.h"

typedef uint8_t           u8;
typedef uint16_t          u16;
typedef uint32_t          u32;
typedef uint64_t          u64;
typedef unsigned __int128 u128;
#define u(k) CAT(u,k) // to use the different types automatically

// The following is used in the macros which need a bit :
// in the case of ints of size 64, we need the bit to be long
#define _b128(b) CAT(b,L)
#define _b64(b)  CAT(b,L)
#define _b32(b)  b
#define _b16(b)  b
#define _b8(b)   b
#define _b(k,b)  CAT(_b,k)(b)

// There is almost the same, but for printing in a right format
#define _format128 "%lu"
#define _format64  "%lu"
#define _format32  "%u"
#define _format16  "%u"
#define _format8   "%u"
#define _format(k) CAT(_format,k)

/* Note:
 * For those wondering about my macros'variables'names, in my defense
 * I wanted to make them more exotic, but I was told it wouldn't
 * be such a good idea...
 * So I'm now forced to show how much of a fanboy I am, only to
 * prevent anyone to label its variables like mine.
 * And if, despite all of that, you still manage to call them like mine...
 * ...
 * ...you're gonna have a BAD TIME.
 * Just kiddin' ! Let's eat spaghetti and
 * butterscotch-cinnamon pie together someday :3
 */

/*
#define _shr   >>
#define _shl   <<
#define _sh(d) CAT(_sh,d)
#define _tipr(k)  0
#define _tipl(k)  k-1
#define _tip(d,k) CAT(_tip,d)(k)
*/
	




    /*************************************/
   /*                                   */
  /*               MASKS               */
 /*                                   */
/*************************************/

/* Here are some masks which can be usefull
 */

// 0b10110111011110111110111111011111...d0
#define MASK_rise_64 0xFFDFF7FBFBF7DEEDUL
#define MASK_rise_32 0xFBF7DEED
#define MASK_rise_16 0xDEED
#define MASK_rise_8  0xED
#define MASK_rise(k) CAT(MASK_rise_,k) 


// 0b1110111011101110111011101110111011101110111011101110111011101110...
#define MASK_4x4_128 0x77777777777777777777777777777777UL
#define MASK_4x4_64  0x7777777777777777UL
#define MASK_4x4_32  0x77777777
#define MASK_4x4_16  0x7777
#define MASK_4x4_8   0x77
#define MASK_4x4(k)  CAT(MASK_4x4_,k) 


// 0b0101010101010101010101010101010101010101010101010101010101010101...
#define MASK_1o_128  0x55555555555555555555555555555555UL
#define MASK_1o_64   0x5555555555555555UL
#define MASK_1o_32   0x55555555
#define MASK_1o_16   0x5555
#define MASK_1o_8    0x55
#define MASK_1o(k)   CAT(MASK_1o_, k)

// 0b0011001100110011001100110011001100110011001100110011001100110011...
#define MASK_2o_128  0x33333333333333333333333333333333UL
#define MASK_2o_64   0x3333333333333333UL
#define MASK_2o_32   0x33333333
#define MASK_2o_16   0x3333
#define MASK_2o_8    0x33
#define MASK_2o(k)   CAT(MASK_2o_, k)

// 0b0000111100001111000011110000111100001111000011110000111100001111...
#define MASK_4o_128  0x0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0FUL
#define MASK_4o_64   0x0F0F0F0F0F0F0F0FUL
#define MASK_4o_32   0x0F0F0F0F
#define MASK_4o_16   0x0F0F
#define MASK_4o_8    0x0F
#define MASK_4o(k)   CAT(MASK_4o_, k)

// 0b0000000011111111000000001111111100000000111111110000000011111111...
#define MASK_8o_64  0x00FF00FF00FF00FFUL
#define MASK_8o_32  0x00FF00FF
#define MASK_8o_16  0x00FF
#define MASK_8o(k)  CAT(MASK_8o_, k)

// 0b0000000000000000111111111111111100000000000000001111111111111111...
#define MASK_16o_128 0x0000FFFF0000FFFF0000FFFF0000FFFFUL
#define MASK_16o_64  0x0000FFFF0000FFFFUL
#define MASK_16o_32  0x0000FFFF
#define MASK_16o(k)  CAT(MASK_16o_,k)

// 0b0000000000000000000000000000000011111111111111111111111111111111...
#define MASK_32o_128 0x00000000FFFFFFFF00000000FFFFFFFFUL
#define MASK_32o_64  0x00000000FFFFFFFFUL
#define MASK_32o(k)  CAT(MASK_16o_,k)
	
#define MASK_o(ov,k) CAT(CAT(MASK_,ov),o)(k)



/* And some functions using masks tagging along */

/* swap the bit table by groups of size 'o' (to use in proper swap function)
 * ex: wMASK_swapy(8,4,0b[1010][1100]) => 0b[1100][1010] */
#define wMASK_swap(k,o,b)					\
  b = (((b) >> o) & MASK_o(o,k)) | (((b) & MASK_o(o,k)) << o)

/* weigh the groups of '2*o' bits (to use in proper hamming weight function)
 * ex: wMASK_hammy(8,1,0b{10}{11}{01}{00}) => 0b{01}{10}{01}{00} */
// Note: haven't implemented yet, 'cause not essential, so do as your please
#define wMASK_hamy(k,o,b)				\
  ((((b) >> o) & MASK_o(o,k)) + ((b) & MASK_o(o,k)))





    /*************************************/
   /*                                   */
  /*             BIT TABLE             */
 /*                                   */
/*************************************/

/* In order to work with tables of bits, it has always been
 * more beneficial to use, instead of a litteral table of bits,
 * an int of preset size 'k', which can behave like a bit table,
 * but better, faster, stronger (I know you just sang it in your head)
 * For example : [0,1,0,1,1,1] => [0b111010] = [58]
 * REMEMBER : the tables are stored as in Little Endian
 *            (first index as least significant bit)
 * The following are macros to use to interact with those objects.
 */


// get the 'i'th bit in 'b'
#define BIT_get(k,b,i)				\
  ( ( (b) & (_b(k,1)<<(i)) )>>(i) & _b(k,1) )


// set 'i'th bit of b to 'bit'
#define BIT_set(k,b,i,bit)				\
  b = ( (b ^ (b & (_b(k,1)<<(i)))) | ((u(k))bit<<(i)) )


// sum 'bit' to the 'i'th bit of 'b' (xor + set = xet)
#define BIT_xet(k,b,i,bit)			\
  (b) ^= (((u(k))bit)<<(i))


// and 'bit' to the 'i'th bit of b (and + set = aet)
// ...how come my nicknames are rubbish ?
#define BIT_aet(k,b,i,bit)			\
  (b) &= (((u(k))bit)<<(i))


/* to pop & push, aka posh (or poop if you're immature enough)
 * ex: posh([1,0,1,1,1],0) = 1, ==>[0,1,0,1,1]                 */
#define BIT_posh(k,b,bit)			\
  FUNC( u(k) _asgore = BIT_get(k,b,k-1);	\
	(b) = ((b) << 1);			\
	(b) |= ((u(k))bit);			\
	_asgore; )


// Returns if 'b1' is in 'b2'
#define BIT_in(k,b1,b2)\
  (((b1) & (b2)) == (b1))


// reverse entirely the bit table (b2f = back to front)
#define BIT_b2f_8(k,b)  wMASK_swap(k,1,b) + wMASK_swap(k,2,b); wMASK_swap(k,4,b)
#define BIT_b2f_16(k,b) BIT_b2f_8(k,b);    wMASK_swap(k,8,b)
#define BIT_b2f_32(k,b) BIT_b2f_16(k,b);   wMASK_swap(k,16,b)
#define BIT_b2f_64(k,b) BIT_b2f_32(k,b);   b = (((b) >> 32) | ((b) << 32))
#define BIT_b2f(k,b) MACRO( CAT(BIT_b2f_,k)(k,b); )


// returns the Hamming weight of the bit table
#define BIT_hamwei_8(k,b)  wMASK_hamy(k,4,wMASK_hamy(k,2,wMASK_hamy(k,1,b)))
#define BIT_hamwei_16(k,b) wMASK_hamy(k,8,BIT_hamwei_8(k,b))
#define BIT_hamwei_32(k,b) wMASK_hamy(k,16,BIT_hamwei_16(k,b))
#define BIT_hamwei_64(k,b) BIT_hamwei_32(k,b) + (BIT_hamwei_32(k,b) >> 32)
#define BIT_hamwei(k,b) ( CAT(BIT_hamwei_,k)(k,b) )


/* prints. That's about it. */
#define BIT_fprintl(f,k,b,l)					\
  MACRO( for (u(k) _metatton = 0; _metatton < l; ++_metatton)	\
	   fprintf(f,_format(k), BIT_get(k,b,_metatton)); )
#define BIT_printl(k,b,l) BIT_print(stdout,k,b,l)
#define BIT_print(k,b)    BIT_printl(k,b,k)
	




    /***********************************/
   /*                                 */
  /*              UBTAB              */
 /*                                 */
/***********************************/

/* Here will lie the different macros to use hUge Bit TABles.
 * An UBTAB is just a list of bit tables as heretofore defined.
 * Think of it as an expansion of what we already talk about.
 * ex: [1,0,0,1,0,1,1,1,0,1] => [0b11101001,0b10] = [233,2]
 * REMEMBER : even if each Bit table in the UBTAB is stored as
 *            in Little Endian, the "Master Table" is sorted
 *            in the right order (just as in Little Endian storage)
 */

// Give the size of a bit table divided in numbers of k bits
#define UBTAB_SIZE(k,size)     ((size+k-1)/k)


// like BIT_get, but on UBTAB
#define UBTAB_get(k,ub,i)			\
  BIT_get(k,ub[(i)/k],(i)%k)


// ...like BIT_set, but on UBTAB
#define UBTAB_set(k,ub,i,bit)			\
  BIT_set(k,ub[(i)/k],(i)%k,bit)


// like... (yawn) ...BIT_xet, but on UBTAB
#define UBTAB_xet(k,ub,i,bit)			\
  BIT_xet(k,ub[(i)/k],(i)%k,bit)

// ...like... BIT_aet... but... U...UBTAB...
#define UBTAB_aet(k,ub,i,bit)			\
  BIT_aet(k,ub[(i)/k],(i)%k,bit)


// Zzzzz BIT_posh Zzzzz Zzzzzzzzzz UBTAB Zzzzzzz
#define UBTAB_posh(k,ub,l,bit)						\
  FUNC( u(k) _DOG = (u(k)) bit;						\
	for (u8 _dog = 0; _dog < UBTAB_SIZE(k,l); ++_dog)		\
	  _DOG = BIT_posh(k,ub[_dog],_DOG);				\
	_DOG; )
	   

// fill an UBTAB with 0 or 1 of size 'l'
#define UBTAB_fill(k,ub,l,bit)						\
  for (u(k) _temmie = 0; _temmie < UBTAB_SIZE(k,l); ub[_temmie++] = bit*(~0))


// xor 2 UBTABs of size 'l'
#define UBTAB_xor(k,ubin,ub1,ub2,l)					\
  MACRO( for (u(k) _sans = 0; _sans < UBTAB_SIZE(k,l); ++_sans)		\
	   ubin[_sans] = ub1[_sans] ^ ub2[_sans]; )


// "and" 2 UBTABs of size, you guessed it, 'l'
#define UBTAB_and(k,ubin,ubfrom,l)					\
  MACRO( for (u(k) _papyrus = 0; _papyrus < UBTAB_SIZE(k,l); ++_papyrus) \
	   ubin[_papyrus] &= ubfrom[_papyrus]; )


// not-xor 2 UBTABs of size ,whatatwist, 'l'
#define UBTAB_nxor(k,ubin,ubfrom,l)					\
  MACRO( for (u(k) _muffet = 0; _muffet < UBTAB_SIZE(k,l); ++_muffet)	\
	   ubin[_muffet] ^= ~(ubfrom[_muffet]); )
  


// "pour" the first UBTAB into the second (like a 'clone' function)
#define UBTAB_pour(k,ubfrom,ubin,l)					\
  MACRO( for (u(k) _pourri = 0; _pourri < UBTAB_SIZE(k,l); ++_pourri)	\
	   (ubin[_pourri] = ubfrom[_pourri]); )


/* Reverse an entire UBTAB of size 'l', meaning the 'l'th bit will be
 * the first after that (ex: [{1,0,1,0,0,0,1,0][1,1,0},0,0,0,0,0]
 *                       ==> [{0,1,1,0,1,0,0,0][1,0,1},0,0,0,0,0]  */
// Note: to debug, use that : UBTAB_print(k,ub,UBTAB_SIZE(k,l)*k); puts("");
#define UBTAB_b2f(k,ub,l)						\
  MACRO( for (u(k) _somei = 0; _somei < UBTAB_SIZE(k,l); ++_somei)	\
	   BIT_b2f(k,ub[_i]);						\
	 for (u(k) _somei = 0; _somei < UBTAB_SIZE(k,l)>>1; ++_somei){	\
	   u(k) tmp = ub[_somei];					\
	   ub[_somei] = ub[UBTAB_SIZE(k,l)-1-_somei];			\
	   ub[UBTAB_SIZE(k,l)-1-_i] = tmp;				\
	 }								\
	 for (u(k) _somei = 0; _somei < UBTAB_SIZE(k,l)-1; ++_somei)	\
	   ub[_somei] = (ub[_somei] >> (k-(l%k))) | (ub[_somei+1] << ((l%k))); \
	 ub[UBTAB_SIZE(k,l)-1] = (ub[UBTAB_SIZE(k,l)-1] >> (k-(l%k)));)

/* Returns the Hamming weight of the entire UBTAB */
#define UBTAB_hamwei(k,ub,l)						\
  FUNC( u(k) _miyamoto = 0;						\
	for (u(k) _musashi = 0; _musashi < UBTAB_SIZE(k,l); ++_musashi) \
	  _miyamoto += BIT_hamwei(k,ub[_musashi]);			\
	_miyamoto; )
	  

/* To print entirely an UBTAB of size... (drum roll) 'l' (burst of applause) */
#define UBTAB_fprint(f,k,ub,l)						\
  MACRO( for (u(k) _nyah = 0; _nyah < UBTAB_SIZE(k,l)-1; ++_nyah)	\
	   BIT_fprintl(f,k,ub[_nyah],k);				\
	 BIT_fprintl(f,k,ub[UBTAB_SIZE(k,l)-1],l-(UBTAB_SIZE(k,l)-1)*k);)
#define UBTAB_print(k,ub,l) UBTAB_fprint(stdout,k,ub,l)

#endif//_BITTAB_H
