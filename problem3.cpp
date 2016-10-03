/* Tyler Stuessi
 * Jeremy Herwig
 *
 * Finding a targeted collision on SHA256
 * (Using brute force attack)
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>

#include "sha256.h"

using namespace std;

typedef unsigned char UCHAR;

//SHA256 returns 32 byte hashes
const int HASH_SIZE = 32;

//generates a random byte string to hash
UCHAR * gen_random_uchar(UCHAR *temp, size_t n) {
    for(int i = 0; i < n; i++) {
        temp[i] = (UCHAR)rand();
    }

    return temp;
}

//wrapper function to generate hash
void gen_hash(UCHAR *input, size_t size, UCHAR *output) {
    sha256(input, size, output);
}

//Make out easier to output the hash
ostream & operator<<(ostream &out, UCHAR *temp) {
	for(int i = 0; i < HASH_SIZE; i++) {
		cout << setw(2) << setfill('0') << hex << int(temp[i]) << dec;
	}
	return out;
}

//convert the base to an n bit string padded with zeros
//for comaprison purposes
void convert_to_n_bits(int n, UCHAR *a) {
    if(n % 8 != 0) {
		//ugly, but this is basically a mask
        a[n/8] = a[n/8] & ~( (1 << (8-(n%8))) -1);
    } else {
        a[n/8] = 0;
    }

    memset((void *)(a+n/8+1), 0, HASH_SIZE-(n/8+1));
}

//compare n bits of each string to test if they are a collision
bool compare_n_bits(UCHAR *temp1, UCHAR *temp2, size_t hash_size, size_t n) {
	convert_to_n_bits(n, temp1);
	convert_to_n_bits(n, temp2);

	bool ret = (memcmp(temp1, temp2, hash_size) == 0);

	return ret;
}



int main(int argc, char **argv) {
	if(argc != 2) {
		cout << "Usage: " << argv[0] << " num_bits" << endl;
		return 1;
	}

	srand(time(0));

	int n = atoi(argv[1]);

	char *starting_hash_c;
	starting_hash_c = "tylerjeremy";

	UCHAR *starting_hash = (UCHAR *)starting_hash_c;
	UCHAR *base_hash = (UCHAR *)malloc(HASH_SIZE);

	UCHAR *temp1 = (UCHAR *)malloc(HASH_SIZE);
	UCHAR *temp2 = (UCHAR *)malloc(HASH_SIZE);

	//generate starting hash
	gen_hash(starting_hash, strlen((char *)starting_hash), base_hash);

	//generate first test hash
	gen_random_uchar(temp1, HASH_SIZE);
	gen_hash(temp1, HASH_SIZE, temp2);

	//brute force a collision
	while(!compare_n_bits(temp2, base_hash, HASH_SIZE, n)) {
		gen_random_uchar(temp1, HASH_SIZE);
		gen_hash(temp1, HASH_SIZE, temp2);
	}

	cout << n << " BIT COLLISION FOUND" << endl;
	cout << "y    = " << starting_hash_c << endl;
	gen_hash(starting_hash, strlen((char *)starting_hash), base_hash);
	cout << "H(y) = " << base_hash << endl;
	cout << "x    = " << temp1 << endl;
	gen_hash(temp1, HASH_SIZE, temp2);
	cout << "H(x) = " << temp2 << endl;

	free(base_hash);
	free(temp1);
	free(temp2);

	return 0;
}


	

