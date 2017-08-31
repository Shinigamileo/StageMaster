/*
 * ----------------------------------------------------------------------------
 * "THE NO-WARE LICENSE" (Revision -0):
 * Léo Barré wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff.
 * ...That's it.
 * ----------------------------------------------------------------------------
 */
#ifndef ECRYPT_SCIPHER_H
#define ECRYPT_SCIPHER_H

/*
 * Ceci est un header amateur supporté par l'article L122 -A
 * du code de la propriété intellectuelle.
 * Le fichier 'ecrypt-sync.h' d'origine appartient à l'ESTREAM.
 * Supportez l'œuvre originale !
 */

#include <bittab.h>

/* ------------------------------------------------------------------------- */

/* Data structures */

/* 
 * ECRYPT_ctx is the structure containing the representation of the
 * internal state of your cipher. 
 */

typedef struct
{
  u64 init[2];
  u64 state[6];

} ECRYPT_ctx;

/* ------------------------------------------------------------------------- */

/* Mandatory functions */

/*
 * Key and message independent initialization. This function will be
 * called once when the program starts (e.g., to build expanded S-box
 * tables).
 */
void ECRYPT_init(void);

/*
 * Key setup. It is the user's responsibility to select the values of
 * keysize and ivsize from the set of supported values specified
 * above.
 */
void ECRYPT_keysetup(
  ECRYPT_ctx* ctx, 
  const u8* key, 
  u32 keysize,                /* Key size in bits. */ 
  u32 ivsize);                /* IV size in bits. */ 

/*
 * IV setup. After having called ECRYPT_keysetup(), the user is
 * allowed to call ECRYPT_ivsetup() different times in order to
 * encrypt/decrypt different messages with the same key but different
 * IV's.
 */
void ECRYPT_ivsetup(
  ECRYPT_ctx* ctx, 
  const u8* iv);

/*////////////////////////////////////////////////////////////////////////////////
// Hi ! My name is Léo Barré. You've maybe already seen me in some             //
// obscure theatric compositions, where I play a perverted old man or a guru. //
// I interrupt this header to precise that the warm'up (aka the empty rounds //
// a stream cipher can do to enhance complexity) were done inside the       //
// ECRYPT_ivsetup function at first(which wasn't even clarified --')       //
// I removed it, since I wanna perform some attacks, so at state zero.    //
// If you still wanna use it, I moved it inside the following function   //
/*/////////////////////// which will warm up the stream for ya.         //
void ECRYPT_warmup(   //  You're welcome.                              //
  ECRYPT_ctx* ctx,   ///////////////////////////////////////////////////
  u32 warmup);      //
/////////////////////

/*
 * Encryption/decryption of arbitrary length messages.
 *
 * For efficiency reasons, the API provides two types of
 * encrypt/decrypt functions. The ECRYPT_encrypt_bytes() function
 * (declared here) encrypts byte strings of arbitrary length, while
 * the ECRYPT_encrypt_blocks() function (defined later) only accepts
 * lengths which are multiples of ECRYPT_BLOCKLENGTH.
 * 
 * The user is allowed to make multiple calls to
 * ECRYPT_encrypt_blocks() to incrementally encrypt a long message,
 * but he is NOT allowed to make additional encryption calls once he
 * has called ECRYPT_encrypt_bytes() (unless he starts a new message
 * of course). For example, this sequence of calls is acceptable:
 *
 * ECRYPT_keysetup();
 *
 * ECRYPT_ivsetup();
 * ECRYPT_encrypt_blocks();
 * ECRYPT_encrypt_blocks();
 * ECRYPT_encrypt_bytes();
 *
 * ECRYPT_ivsetup();
 * ECRYPT_encrypt_blocks();
 * ECRYPT_encrypt_blocks();
 *
 * ECRYPT_ivsetup();
 * ECRYPT_encrypt_bytes();
 * 
 * The following sequence is not:
 *
 * ECRYPT_keysetup();
 * ECRYPT_ivsetup();
 * ECRYPT_encrypt_blocks();
 * ECRYPT_encrypt_bytes();
 * ECRYPT_encrypt_blocks();
 */

/*/////////////////////////////////////////////////////////////////////////
// Hi ! Me again. Just wanna add up that it's way better to get ciphers //
// of size in 64*N : since the stream update 64 bits all together each //
// time it's called, if you don't wanna lose informations, you should //
// use all 64 of them.                                               //
// It's a pretty important information but, again, they didn't      //
// clarified it...                                                 //
/*//////////////////////////////////////////////////////////////////

/* ------------------------------------------------------------------------- */

/* Optional features */

/* 
 * For testing purposes it can sometimes be useful to have a function
 * which immediately generates keystream without having to provide it
 * with a zero plaintext. If your cipher cannot provide this function
 * (e.g., because it is not strictly a synchronous cipher), please
 * reset the ECRYPT_GENERATES_KEYSTREAM flag.
 */

#define ECRYPT_GENERATES_KEYSTREAM
#ifdef ECRYPT_GENERATES_KEYSTREAM

void ECRYPT_keystream_bytes(
  ECRYPT_ctx* ctx,
  u8* keystream,
  u32 length);                /* Length of keystream in bytes. */

/*//////////////////////////////////////////////////////////////////////////
// Hi ! Me again, again. Last but not least, remember to give the length //
// as bytes, and not bits. I don't get why, here, they decided to       //
// completely change their type of parameter, which is totally         //
// unintuitive, but they do it anyway... Remember well.               //
/*/////////////////////////////////////////////////////////////////////

#endif

/* ------------------------------------------------------------------------- */

#endif//ECRYPT_SCIPHER_H
