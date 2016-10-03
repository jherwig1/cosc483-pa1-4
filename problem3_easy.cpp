/* An easier, brute force attack for a targeted collison
 */

#include <iostream>
#include <cstdlib>
#include <openssl/sha.h>
#include <cstring>
#include <iomanip>

using namespace std;

typedef unsigned char UCHAR;

const int HASH_SIZE = 20;

UCHAR * gen_random_uchar(UCHAR *temp, size_t n) {
    for(int i = 0; i < n; i++) {
        temp[i] = (UCHAR)rand();
    }

    return temp;
}

void gen_hash(UCHAR *input, size_t size, UCHAR *output) {
    SHA256(input, size, output);
}

void convert_to_n_bits(int n, UCHAR *a) {
    //leave all the first bits the same, then
    //make sure that only the meaningful bits of the
    //last block are left
    if(n % 8 != 0) {
        a[n/8] = a[n/8] & ~( (1 << (8-(n%8))) -1);
    } else {
        a[n/8] = 0;
    }

    memset((void *)(a+n/8+1), 0, 20-(n/8+1));
}

bool compare_n_bits(UCHAR *temp1, UCHAR *temp2, size_t hash_size, size_t n) {
	UCHAR *a = (UCHAR *)malloc(hash_size);
	UCHAR *b = (UCHAR *)malloc(hash_size);

	memcpy(a, temp1, hash_size);
	memcpy(b, temp2, hash_size);

	convert_to_n_bits(n, a);
	convert_to_n_bits(n, b);

	bool ret = (memcmp(a, b, hash_size) == 0);

	//free(a);
	//free(b);

	return ret;
}

ostream & operator<<(ostream &out, UCHAR *temp) {
	for(int i = 0; i < HASH_SIZE; i++) {
		cout << setw(2) << setfill('0') << hex << int(temp[i]) << dec;
	}
	return out;
}


int main(int argc, char **argv) {
	if(argc != 2) {
		cout << "Usage: " << argv[0] << " num_bits" << endl;
	}

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

	while(!compare_n_bits(temp2, base_hash, HASH_SIZE, n)) {
		gen_random_uchar(temp1, HASH_SIZE);
		gen_hash(temp1, HASH_SIZE, temp2);
	}

	cout << n << " BIT COLLISION FOUND" << endl;
	cout << "y    = " << starting_hash_c << endl;
	cout << "H(y) = " << base_hash << endl;
	cout << "x    = " << temp1 << endl;
	cout << "H(x) = " << temp2 << endl;

	return 0;
}


	

