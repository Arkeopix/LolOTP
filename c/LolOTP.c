#define _GNU_SOURCE
#include "LolOTP.h"
#include "base32.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <openssl/hmac.h>
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

static		int bytes_to_int(char *b) {
  int		val = 0;
  int		j = 0;

  for (int i = 4; i > 0; --i) {
    val += (b[i] & 0xFF) << (8 * j);
    j++;
  }
  return val;
}

int		main(int argc, char *argv[]) {
  char		secret[1024];
  unsigned char	*digest, *ddigest;
  int		input_time;;
  unsigned char		ttime[10];
  unsigned char	time_bytes[4];
  char		last_byte, llast_byte;
  int		offset, ooffset;
  char		four_bytes[4], ffour_bytes[4];
  int		code, ccode;

  memset(secret, 0, 1024);
  if ((base32_decode(strdup(to_usable_secret(SECRET_DEV)), secret)) < 0) {
        fprintf(stderr, DECODE_BASE32_ERR);
        return -1;
  }
  input_time = time(NULL) / 30;
  sprintf((char*)ttime, "%d", (int)time(NULL) / 30);
  printf("input time / 30 char[10] =\t %s\n", ttime);
  printf("input time / 30 int =\t\t %d\n", input_time);

  time_bytes[3] = (input_time >> 24) &0xF;
  time_bytes[2] = (input_time >> 16) &0xF;
  time_bytes[1] = (input_time >> 8) &0xF;
  time_bytes[0] = input_time &0xFF;
  printf("time_bytes =\t\t\t %x %x %x %x\n\n", time_bytes[0], time_bytes[1], time_bytes[2], time_bytes[3]);
  
  digest = HMAC(EVP_sha1(), secret, sizeof(secret), time_bytes, sizeof(time_bytes), NULL, NULL);
  printf("hmac byte array = [%s]\n", digest);
  ddigest = HMAC(EVP_sha1(), secret, sizeof(secret), ttime, sizeof(ttime), NULL, NULL);
  printf("hmac string = [%s]\n\n", ddigest);

  last_byte = digest[strlen((char*)digest) -1];
  offset = last_byte & 0xF;
  printf("last byte = %x offset = %x\n", last_byte, offset);
  memcpy(four_bytes, digest + offset, 4);

  llast_byte = ddigest[strlen((char*)ddigest) -1];
  ooffset = llast_byte & 0xF;
  printf("last byte = %x offset = %x\n", llast_byte, ooffset);
  memcpy(ffour_bytes, ddigest + ooffset, 4);

  printf("hmac[offset] = %x\n", digest[offset]);
  printf("four_bytes = %x %x %x %x\n\n", four_bytes[0], four_bytes[1], four_bytes[2], four_bytes[3]);

  printf("hmac[offset] = %x\n", ddigest[ooffset]);
  printf("four_bytes = %x %x %x %x\n\n", ffour_bytes[0], ffour_bytes[1], ffour_bytes[2], ffour_bytes[3]);
 
  code = (bytes_to_int(four_bytes) & 0x7FFFFFFF) % 1000000;
  printf("code = %d\n", code);

  ccode = (bytes_to_int(ffour_bytes) & 0x7FFFFFFF) % 1000000;
  printf("code = %d\n", ccode);

  return 0;
}
