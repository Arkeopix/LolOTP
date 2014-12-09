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

static		int bytes_to_int(unsigned char *b) {
  int		val = 0;
  int		j = 0;

  for (int i = 4; i > 0; --i) {
    val += (b[i] & 0xFF) << (8 * j);
    j++;
  }
  return val;
}

int		main(int argc, char *argv[]) {
  unsigned char		secret[20];
  unsigned char		*digest;
  unsigned char		time_bytes[4];
  unsigned char		last_byte;
  int		offset;
  unsigned char		four_bytes[4];
  int		code;

  
  memset(secret, 0, 1024);
  if ((base32_decode((unsigned char*)strdup(to_usable_secret(SECRET_DEV)), secret)) < 0) {
    fprintf(stderr, DECODE_BASE32_ERR);
    return -1;
  }
 
  //input_time = time(NULL) / 30;
  printf("Secret: ");
  for (int i = 0; i < 20; ++i) {
    printf("%02x", secret[i]);
  }
  printf("\n");
  int_to_bytes.integer = 4726863;

  /* input_time = 4726863; */
  /* memcpy(time_bytes, &input_time, sizeof(time_bytes)); */
  printf("time_bytes = "); // good
  for (const unsigned char* p = int_to_bytes.bytes; *p; ++p) {
    printf("%02x", *p);
  }
  printf("\n");
  unsigned char test[] = { time_bytes[0], time_bytes[1], time_bytes[2], time_bytes[3] };
  digest = HMAC(EVP_sha1(), secret, 20, test, 4, NULL, NULL); // not good ====> Mauvais decodage
  printf("hmac byte array = %x%x%x%x%x\n", digest[0], digest[1], digest[2], digest[3], digest[4]);

  last_byte = digest[strlen((char*)digest) -1];
  offset = last_byte & 0xF;
  printf("last byte = %x offset = %x\n", last_byte, offset);
  memcpy(four_bytes, digest + offset, 4);

  printf("hmac[offset] = %x\n", digest[offset]);
  printf("four_bytes = %x %x %x %x\n\n", four_bytes[0], four_bytes[1], four_bytes[2], four_bytes[3]);
 
  code = (bytes_to_int(four_bytes) & 0x7FFFFFFF) % 1000000;
  printf("code = %d\n", code);

  return 0;
}
