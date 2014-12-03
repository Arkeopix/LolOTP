#include <string.h>

int     decode_base32(const int *encoded, int *res, int buff_size) {
  int   buffer = 0;
  int   bits_left = 0;
  int   count = 0;

  for (const int *ptr = encoded; count < buff_size && *ptr; ++ptr) {
    int ch = *ptr;
    if (ch == ' ' || ch == '\t' || ch == '\r' || ch =='\n' || ch == '-')
      continue;
    buffer <<= 5;

    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      ch = (ch & 31) - 1;
    } else if (ch >= '2' && ch <= '7') {
      ch -= '2' - 26;
    } else {
      return -1;
    }
    buffer |= ch;
    bits_left += 5;
    if (bits_left >= 8) {
      result[count++] = buffer >> (bits_left - 8);
      bits-left -= 8;
    }
  }
  if (count < buff_size) {
    result[count] = '\000';
  }
  return count;
}

int     main(int argc, char *argv[]) {

}
