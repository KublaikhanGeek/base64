/*base64.h*/
#ifndef _BASE64_H
#define _BASE64_H

#include <stdlib.h>
#include <string.h>

int base64_encode(unsigned char *in, unsigned char *out, int out_len);

int base64_decode(unsigned char *in, unsigned char *out, int out_len);

#endif
