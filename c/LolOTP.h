#ifndef   LOL_OTP_H_
# define  LOL_OTP_H_

# define  SECRET_DEV        "4444 4444 4444 4444 4444 4444 4444 4444"
# define  SECRET_SIZE       80
# define  DECODE_BASE32_ERR "Error decoding base32 secret\n"

union {
  int		integer;
  unsigned char	bytes[4];
}		int_to_bytes;

#endif    /* !LOL_OTP_H_ */
