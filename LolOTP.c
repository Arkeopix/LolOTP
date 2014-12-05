#define _GNU_SOURCE
#include "lolOTP.h"
#include "base32.h"
#include <stdint.h>
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
	printf("usable_secret = %s\n", cpy);
  return cpy;
}

int     main(int argc, char *argv[]) {
  char	secret[1024], tmp[1024], input_time[20];
  unsigned char  tmp_hmac[SHA_DIGEST_LENGTH];
  unsigned char  hmac[SHA_DIGEST_LENGTH];
  char  four_bytes[32];
  int   code;

  if ((base32_decode(strdup(to_usable_secret(SECRET_DEV)), secret)) < 0) {
        fprintf(stderr, DECODE_BASE32_ERR);
        return -1;
  }
  printf("usable secret is %s\n", secret);
  sprintf(input_time, "%d", time(NULL) / 30);

  memset(&tmp, 0, sizeof(tmp));
  memcpy(tmp, secret, sizeof(secret));
  printf("tmp = %s\n", tmp);
  strcat(tmp, input_time);
  printf("tmp+input_time = %s\n", tmp);
  SHA1(tmp, strlen(tmp), tmp_hmac);
  memset(&tmp, 0, sizeof(tmp));
  printf("tmp hmac = %s\n", tmp_hmac);
  strcat(secret, tmp_hmac);
  printf("%s\n", secret);
  //SHA1(secret, strlen(secret), hmac);
  //printf("%s\n", hmac);

//  four_bytes = hmac + (sizeof(hmac) - 32);
//  code = (int)(four_bytes);
//  code = code % 1000000;
  printf("code is %d\n", code);
  return 0;
}
