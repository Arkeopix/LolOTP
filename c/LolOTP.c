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
  return cpy;
}

int		main(int argc, char *argv[]) {
  char		secret[1024], tmp[1024];
  char		tmp_hmac[SHA_DIGEST_LENGTH];
  char		hmac[SHA_DIGEST_LENGTH];
  char		input_time[10];
  char		*four_bytes;
  long		code;

  if ((base32_decode(strdup(to_usable_secret(SECRET_DEV)), secret)) < 0) {
        fprintf(stderr, DECODE_BASE32_ERR);
        return -1;
  }
  //  printf("usable secret is %s\n", secret);
  sprintf(input_time, "%d", (int)time(NULL) / 30);

  memset(&tmp, 0, sizeof(tmp));
  memcpy(tmp, secret, sizeof(secret));
  strcat((char*)tmp, input_time);
  //printf("tmp + input_time = %s, size = %d\n", tmp, (int)sizeof(tmp));
  SHA1((unsigned char*)tmp, sizeof(tmp), (unsigned char*)tmp_hmac);
  memset(&tmp, 0, sizeof(tmp));
  //printf("tmp hmac = %s\n", tmp_hmac);
  strcat((char*)secret, (char*)tmp_hmac);
  //printf("%s\n", secret);
  SHA1((unsigned char*)secret, sizeof(secret), (unsigned char*)hmac);
  //printf("final hmac is %s\n", hmac);

  if ((four_bytes = malloc(((32 + 1) * sizeof(char)))) == NULL) {
    return -1;
  }

  four_bytes = hmac + (SHA_DIGEST_LENGTH - 32);
  code = (long)four_bytes;
  //printf("last 4 bytes = %d\n", (int)code);
  code = code % 1000000;
  printf("code is %d\n", (int)code);
  return 0;
}
