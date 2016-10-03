#ifndef _SHA256_
#define _SHA256_

typedef unsigned char uchar;
typedef unsigned int uint;

typedef struct hash {
	uint data[64];
	uint values[8];
} *HASH;

void sha256(uchar *input, int size, uchar *output);

#endif
