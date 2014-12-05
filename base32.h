#ifndef   BASE_32_H_
# define  BASE_32_H_

# define  NOT_STANDARD_B32_STRING "char %c is not UPPERCASE\n"

int       base32_encode(char *in, char *out);
int       base32_decode(char *in, char *out);

#endif    /* !BASE_32_H_ */
