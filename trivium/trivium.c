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

#include <ecrypt-scipher.h>

int main(int argc, char* argv[]){
  FILE* params = fopen(argv[1],"r");
  char buffer[90];

  IGNORE(fgets(buffer,10,params));
  int l = atoi(buffer);

  IGNORE(fgets(buffer,90,params));
  u8 K[UBTAB_SIZE(8,80)];
  for (int k = 0; k < 80; ++k)
    UBTAB_set(8,K,k,(buffer[k]-'0'));

  IGNORE(fgets(buffer,90,params));
  u8 IV[UBTAB_SIZE(8,80)];
  for (int iv = 0; iv < 80; ++iv)
    UBTAB_set(8,IV,iv,(buffer[iv]-'0'));

  u8 stream[l];

  ECRYPT_ctx ctx;
  ECRYPT_keysetup(&ctx,K,80,80);
  ECRYPT_ivsetup(&ctx,IV);
  ECRYPT_keystream_bytes(&ctx,stream,UBTAB_SIZE(8,l));

  UBTAB_print(8,stream,l);
  
  return EXIT_SUCCESS;
}
