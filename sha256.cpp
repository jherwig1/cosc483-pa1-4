/* Jeremy Herwig, Tyler Stuessi
 * SHA256 C Implementation
 * CS483 Applied Crypto Programming Assignment 1
 */
#include <stdlib.h>
#include <string.h>

typedef unsigned char uchar;
typedef unsigned int uint;
#define RR(a,b) (((a) >> (b)) | ((a) << ((sizeof(a) * 8) -(b))))

typedef struct hash {
	uint data[64];
	uint values[8];
} *HASH;

uint h0 = 0x6a09e667;
uint h1 = 0xbb67ae85;
uint h2 = 0x3c6ef372;
uint h3 = 0xa54ff53a;
uint h4 = 0x510e527f;
uint h5 = 0x9b05688c;
uint h6 = 0x1f83d9ab;
uint h7 = 0x5be0cd19;

const uint k[] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
	0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74,0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
	0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
	0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
	0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
	0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};


/* Set the output at the end of hashing */
void set_hash(uint src, uchar dest[]) {
	/* Each uint is 4 bytes */
	dest[0] = (uchar) ( (src) >> 24);
	dest[1] = (uchar) ( (src) >> 16);
	dest[2] = (uchar) ( (src) >> 8);
	dest[3] = (uchar) (src);
}


/* This is almost line for line the psuedo-code */
void compress(HASH s) {
	int i;
	uint s1, ch, temp1, s0, maj, temp2;
	uint a, b, c, d, e, f, g, h;

	a = s->values[0];
	b = s->values[1];
	c = s->values[2];
	d = s->values[3];
	e = s->values[4];
	f = s->values[5];
	g = s->values[6];
	h = s->values[7];

	/* Do the compression */
	for (i=0; i < 64; i++) {
		s1 = RR(e, 6) ^ RR(e, 11) ^ RR(e, 25);
		ch = (e & f) ^ ((~e) & g);
		temp1 = (h + s1 + ch + k[i] + s->data[i]);
		s0 = RR(a, 2) ^ RR(a, 13) ^ RR(a, 22);
		maj = (a & b) ^ (a & c) ^ (b & c);
		temp2 = (s0 + maj);

		h = g;
		g = f;
		f = e;
		e = (d + temp1);
		d = c;
		c = b;
		b = a;
		a = (temp1 + temp2);
	}

	h0 = (h0 + a);
	h1 = (h1 + b);
	h2 = (h2 + c);
	h3 = (h3 + d);
	h4 = (h4 + e);
	h5 = (h5 + f);
	h6 = (h6 + g);
	h7 = (h7 + h);
}

/* Handle one chunk (512 bits) of data */
void process_chunk(HASH sha_hash, uchar *chunk) {

	int i;
	uint* w = sha_hash->data;
	int k;

	/* Set the first 16 bytes of the word array to be the data from the chunk.
	 * Since each word (element of w) is 32 bits, they each hold 4 bytes of
	 * the chunk. The bit shifting places the bytes from chunk in the correct
	 * order in w.
	 */

	for (i=0; i < 16; i++) {
		w[i] = ( (uint) chunk[4*i] << 24) \
			| ( (uint) chunk[4*i + 1] << 16) \
			| ( (uint) chunk[4*i + 2] << 8) \
			| (uint) chunk[4*i + 3];
	}


	/* Fill in the rest of the words. Pretty much copied straight from
	 * pseudo-code. Not sure what it's really doing (but that's pretty much
	 * me with all of crypto anyways).
	 */
	for (i = 16; i< 64; i++) {
		uint s0 = (RR(w[i-15], 7)) ^ ( RR(w[i-15],18)) \
			^ (w[i-15] >> 3);
		uint s1 = (RR(w[i-2], 17)) ^ (RR(w[i-2], 19)) \
			^ (w[i-2] >> 10);
		w[i] = (w[i-16] + s0 + w[i-7] + s1);
	}

	/* Set the initial values */
	sha_hash->values[0] = h0;
	sha_hash->values[1] = h1;
	sha_hash->values[2] = h2;
	sha_hash->values[3] = h3;
	sha_hash->values[4] = h4;
	sha_hash->values[5] = h5;
	sha_hash->values[6] = h6;
	sha_hash->values[7] = h7;

	/* Compress the chunk */
	compress(sha_hash);
}


void sha256(uchar *input, int size, uchar *output) {

	uint bitlen1, bitlen2, bittotal, ka, kb, num_bytes;
	int i, j;

	ka = 0;
	bitlen1 = size * 8;
	bitlen2 = bitlen1;

	/* Calculate the length of the message to operate on.
	 * I know there's some bit magic way to do this, but I'm not a magician.
	 */

	bitlen2 = bitlen2 % 512;
	if (bitlen2 <= 448)
		ka = 448 - bitlen2;
	else if (bitlen2 < 512)
		ka = 512 + 448 - bitlen2;

	/* ka is the number of bits of zero, kb has the 64 bits for the message
	 * length added. The number of extra bytes is computed from this.
	 */
	kb = ka + 64;
	num_bytes = kb / 8;
	if (kb % 8 != 0) // kb requires an extra byte for all the zeros
		num_bytes +=1;

	/* Create a new buffer to copy the input in and append the '1' bit and
	 * length of the message. You could just operate on the message 512
	 * at a time and check for the final chunk and avoid allocating more
	 * memory, but that's harder.
	 */

	uchar *data = (uchar *) calloc(1, size + num_bytes);
	memcpy(data, input, size);

	/* Append a 1 bit to the message */
	data[size] = 128;

	/* Append the length of the message to the end of the new buffer.
	 * The length of the buffer is size + num_bytes, so start at the
	 * first byte back and put the lsb of the length, 2 blocks back and second
	 * lsb, ..., to maintain big endianess.
	 */

	int64_t len = size * 8;
	*(data + size + num_bytes - 1) = (uint) ((len));
	*(data + size + num_bytes - 2) = (uint) ((len) >> 8);
	*(data + size + num_bytes - 3) = (uint) ((len) >> 16);
	*(data + size + num_bytes - 4) = (uint) ((len) >> 24);

	HASH sha_hash = (HASH) malloc(sizeof(struct hash));
	bittotal = bitlen1 + ka + 64;

	/* Process the message in chunks of 512 bits */
	for (j=0; j < bittotal / 512; j++) {
		process_chunk(sha_hash, data + j*64);
	}

	/* Append h0, ..., h7 for the output */
	set_hash(h0, output);
	set_hash(h1, output + 4);
	set_hash(h2, output + 8);
	set_hash(h3, output + 12);
	set_hash(h4, output + 16);
	set_hash(h5, output + 20);
	set_hash(h6, output + 24);
	set_hash(h7, output + 28);

	free(sha_hash);
	free(data);
}













