#include "lolOTP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <openssl/sha.h>
#include <time.h>

static unsigned char b32e_rfc4648[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
static unsigned char b32d_rfc4648[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,26,27,28,29,30,31, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25, 0, 0, 0, 0, 0,
	0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char *b32e_5(unsigned char *in, int len, unsigned char *out, unsigned char *table) { // in = 5 bytes, out = 8 bytes
	int x, y;
	int p, t=0;
	for(x=0, y=4; x < 5; ++x, --y) {
		p = in[x];
		t += p << ((+y)*8);
	}
	for(x=0, y=7; x < 8; ++x, --y) {
		out[x] = table[((t >> ((+y)*5)) & 0x1F)];
	}
	if(len<5)
		switch(len) { // why? because fuck it, thats why
			case 4: // 1
				out[7] = '=';
				break;
			case 3: // 3
				out[7] = '=';
				out[6] = '=';
				out[5] = '=';
				break;
			case 2: // 4
				out[7] = '=';
				out[6] = '=';
				out[5] = '=';
				out[4] = '=';
				break;
			case 1: // 6
				out[7] = '=';
				out[6] = '=';
				out[5] = '=';
				out[4] = '=';
				out[3] = '=';
				out[2] = '=';
				break;
			default:
				break;
		}
	return out;
}

char *b32d_8(unsigned char *in, unsigned char *out, unsigned char *table) { // in = 8 bytes, out = 5 bytes
	int x, y;
	int t=0, a;
	for(x=0; x < 8; ++x)
		if(in[x] == '=')
			in[x] = 0; // convert padding to zero;
	for(x=0, y=7; x < 8; ++x, --y) {
		a = table[in[x]];
		t += (a << ((+y)*5));
	}
	for(x=0, y=4; x < 5; ++x, --y)
		out[x] = (t >> ((+y)*8));
	return out;
}

char *b32e(char *in, int len, char *out, char *table) {
	int x, y;
	for(x=0, y=0; x < len; x+=5, y+=8)
		b32e_5(in+x, len-x, out+y, table);
	return out;
}

char *b32d(char *in, int len, char *out, char *table) {
	int x, y;
	for(x=0, y=0; x < len; x+=8, y+=5)
		b32d_8(in+x, out+y, table);
	return out;
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
