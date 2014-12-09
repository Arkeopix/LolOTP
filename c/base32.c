#include "base32.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

static char *base32_alpha[32] = {
  "A", "B", "C", "D", "E", "F", "G", "H",
  "I", "J", "K", "L", "M", "N", "O", "P",
  "Q", "R", "S", "T", "U", "V", "W", "X",
  "Y", "Z", "2", "3", "4", "5", "6", "7",
};

static  void dec_to_bin(unsigned char c, unsigned char *out, int mod) {
  for (int i = (mod == 0 ? 7 : 4); i >= 0; --i) {
    strcat((char*)out, (c & (1 << i)) ? "1" : "0");
  }
}

static  int bin_to_dec(int bin) {
  int   dec = 0, rem = 0, i = 0;

  while (bin > 0) {
    rem = bin % 10;
    bin = bin / 10;
    dec += rem * pow(2,i);
    i++;
  }
  return dec;
}

static  unsigned char *string_to_bin(unsigned char *s) {
  unsigned char  *p = s, *bin;
  unsigned char  tmp_bin[9];

  if ((bin = malloc(COMMON_STRING_MALLOC)) == NULL) {
    return NULL;
  }
  memset(bin, 0, COMMON_STRING_MALLOC);
  while (*p != '\0') {
    memset(&tmp_bin, 0, sizeof(tmp_bin));
    dec_to_bin(*p, tmp_bin, 0);
    strcat((char*)bin, (char*)tmp_bin);
    p++;
  }
  bin[strlen((char*)bin)] = '\0';
  return bin;
}

static  unsigned char *to_base32_alpha(int chunck) {
  return base32_alpha[bin_to_dec(chunck)];
}

static  unsigned char *to_ascii(int chunck) {
  unsigned char  *tmp;

  if ((tmp = malloc(sizeof(unsigned char))) == NULL) {
    return NULL;
  }
  memset(tmp, 0, sizeof(unsigned char));
  tmp[0] = bin_to_dec(chunck);
  return tmp;
}

static  void strip_padd(unsigned char *s) {
  unsigned char* i = s;
  unsigned char* j = s;

  while (*j != 0) {
    *i = *j++;
    if (*i != '=')
      i++;
  }
  *i = 0;
}

static  unsigned char *get_bin_from_b32_alpha(unsigned char c) {
  int   i = 0;
  unsigned char  *bin;


  if ((bin = malloc(9 * sizeof(unsigned char))) == NULL) {
    return NULL;
  }
  memset(bin, 0, 9*sizeof(unsigned char));
  while (*base32_alpha[i] != c) {
    i++;
  }
  dec_to_bin(i, bin, 1);
  return bin;
}

int    base32_decode(unsigned char *in, unsigned char *out) {
  unsigned char  *bin;
  unsigned char  chunck[8];

  strip_padd(in);
  if ((bin = malloc(124 * sizeof(unsigned char))) == NULL) {
    out = NULL;
    return -1;
  }
  memset(bin, 0, 124*sizeof(unsigned char));
  for (int i = 0; i < strlen((const char *)in); i++) {
    strcat((char *)bin, get_bin_from_b32_alpha(in[i]));
  }
  for (int i = 0; i < strlen(bin); i += 8) {
    memcpy(chunck, &bin[i], 8);
    chunck[8] = '\0';
    if (strlen(chunck) % 8 == 0) {
      //      printf("%s\n", out);
      strcat(out, to_ascii(atoi(chunck)));
    }
  }
  return 0;
}

int     base32_encode(unsigned char *in, unsigned char *out) {
  unsigned char  *bin;
  unsigned char  chunck[5];

  bin = string_to_bin(in);
  for (int i = 0; i < strlen(bin); i += 5) {
    memcpy(chunck, &bin[i], 5);
    chunck[5] = '\0';
    while (strlen(chunck) % 5 != 0) {
      strcat(chunck, "0");
    }
    strcat(out, to_base32_alpha(atoi(chunck)));
  }
  while (strlen(out) % 8 != 0) {
    strcat(out, "=");
  }
  return 0;
}

/*int main() {
  unsigned char out[512];
  unsigned char out_decode[512];

  memset(&out, 0, sizeof(out));
  memset(&out_decode, 0, sizeof(out_decode));
  base32_encode("s4aigvwwcfh7f235tbzao5r6gkl7wo5r", out);
  printf("out = %s\n", out);
  base32_decode(out, out_decode);
  printf("out %s\n", out_decode);
  return 0;
}
*/
