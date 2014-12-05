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

static  void dec_to_bin(char c, char *out, int mod) {
  for (int i = (mod == 0 ? 7 : 4); i >= 0; --i) {
    strcat(out, (c & (1 << i)) ? "1" : "0");
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

static  char *string_to_bin(char *s) {
  char  *p = s, *bin;
  char  tmp_bin[9];

  if ((bin = malloc(1024 * sizeof(char))) == NULL) {
    return NULL;
  }
  memset(bin, 0, sizeof(bin));
  while (*p != '\0') {
    memset(&tmp_bin, 0, sizeof(tmp_bin));
    dec_to_bin(*p, tmp_bin, 0);
    strcat(bin, tmp_bin);
    p++;
  }
  bin[strlen(bin)] = '\0';
  return bin;
}

static  char *to_base32_alpha(int chunck) {
  return base32_alpha[bin_to_dec(chunck)];
}

static  char *to_ascii(int chunck) {
  char  *tmp;

  if ((tmp = malloc(sizeof(char))) == NULL) {
    return NULL;
  }
  memset(tmp, 0, sizeof(tmp));
  tmp[0] = bin_to_dec(chunck);
  return tmp;
}

static  void strip_padd(char *s) {
  char* i = s;
  char* j = s;

  while (*j != 0) {
    *i = *j++;
    if (*i != '=')
      i++;
  }
  *i = 0;
}

static  char *get_bin_from_b32_alpha(char c) {
  int   i = 0;
  char  *bin;


  if ((bin = malloc(9 * sizeof(char))) == NULL) {
    return NULL;
  }
  memset(bin, 0, sizeof(bin));
  while (*base32_alpha[i] != c) {
    i++;
  }
  dec_to_bin(i, bin, 1);
  return bin;
}

int    base32_decode(char *in, char *out) {
  char  *bin, *coded;
  char  chunck[8];

  strip_padd(in);
  if ((bin = malloc(124 * sizeof(char))) == NULL) {
    out = NULL;
    return -1;
  }
  memset(bin, 0, sizeof(bin));

  for (int i = 0; i < strlen(in); i++) {
    strcat(bin, get_bin_from_b32_alpha(in[i]));
  }
  for (int i = 0; i < strlen(bin); i += 8) {
    memcpy(chunck, &bin[i], 8);
    chunck[8] = '\0';
    if (strlen(chunck) % 8 == 0) {
      strcat(out, to_ascii(atoi(chunck)));
    }
  }
  return 0;
}

int     base32_encode(char *in, char *out) {
  char  *bin, *tmp;
  char  chunck[5];

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

//int main() {
//  char out[512];
//  char out_decode[512];

//  memset(&out, 0, sizeof(out));
//  memset(&out_decode, 0, sizeof(out_decode));
//  base32_encode("s4aigvwwcfh7f235tbzao5r6gkl7wo5r", out);
//  printf("out = %s\n", out);
//  base32_decode(out, out_decode);
//  printf("out %s\n", out_decode);
//  return 0;
//}
