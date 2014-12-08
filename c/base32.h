#ifndef   BASE_32_H_
# define  BASE_32_H_

# define  NOT_STANDARD_B32_STRING	"unsigned char %c is not UPPERCASE\n"
# define  COMMON_STRING_MALLOC	       	1024 * sizeof(unsigned char)

int       base32_encode(unsigned char *in, unsigned char *out);
int       base32_decode(unsigned char *in, unsigned char *out);

#endif    /* !BASE_32_H_ */
