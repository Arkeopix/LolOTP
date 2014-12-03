#include "lolOTP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <openssl/sha.h>
#include <time.h>

static const char base32_alphabet[32] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', '2', '3', '4', '5', '6', '7',
};

int     base32_decode(const char *in, int len, char *out) {
  
}

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

  if ((base32_decode(to_usable_secret(SECRET_DEV,
      strlen(to_usable_secret(SECRET_DEV)), secret)) < 0) {
        fprintf(stderr, DECODE_BASE32_ERR);
        return -1;
  }

  base32_decode(secret, SECRET_SIZE, to_usable_secret(SECRET_DEV), strlen(to_usable_secret(SECRET_DEV)));
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
