#include "lolOTP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <openssl/sha.h>
#include <time.h>

static  char    *to_usable_secret(char *s) {
  int   i = 0, j = 0;
  char  *cpy = strdup(s);

  while (s[i] != '\0') {
    if (s[i] == ' ') {
      i++;
    } else {
      cpy[j] = toupper(s[i]);
      i++;j++;
    }
  }
  cpy[j] = '\0';
  return cpy;
}

int     main(int argc, char *argv[]) {
  char   secret[SECRET_SIZE];
  int   input_time;
  char  hmac[SHA_DIGEST_LENGTH * 2];
  char  four_bytes[32];
  int   code;

  if ((b32d(to_usable_secret(SECRET_DEV,
      strlen(to_usable_secret(SECRET_DEV)), secret, b32e_rfc4648)) < 0) {
        fprintf(stderr, DECODE_BASE32_ERR);
        return -1;
  }
  printf("usable secret is [%s]\n", secret);
  input_time = time(NULL) / 30;
//  SHA1((unsigned char*)secret + SHA1(secret + input_time, SHA_DIGEST_LENGTH * 2, NULL),
//    SHA_DIGEST_LENGTH * 2, hmac);
//  four_bytes = hmac + (sizeof(hmac) - 32);
//  code = (int)(four_bytes);
//  code = code % 1000000;
  printf("code is %d\n", code);
  return 0;
}
