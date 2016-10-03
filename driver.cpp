#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <openssl/sha.h>
#include "sha256.h"

typedef unsigned char uchar;

void print_hash(uchar *input) {
	for(int i = 0; i < 32; i++) {
		printf("%x",input[i]);
	}
	printf("\n");
}


int main() {
	int i;
	uchar *input = (uchar *) malloc(3);
	uchar *output = (uchar *) malloc(32);
	uchar *output2 = (uchar *) malloc(32);

	input[0] = 'a';
	input[1] = 'b';
	input[2] = 'c';

	sha256(input, 3, output);
	print_hash(output);
	SHA256(input, 3, output2);
	print_hash(output2);


	free(input);
	free(output);
	free(output2);


}
