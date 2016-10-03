/* Tyler Stuessi
 * Jeremy Herwig
 *
 * Problem 3: Finding targeted collisions
 * We are trying to find an n-bit targeted collison with "tylerjeremy"
 * using the algorithm described in the text.
 *
 */

//includes
#include <iostream>
#include <openssl/sha.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <map>
#include <iomanip>

using namespace std;

typedef unsigned char UCHAR;

const size_t HASH_SIZE = 20;

class hash_value {
	public:
		hash_value() {val=NULL; size=0;}
		hash_value(const hash_value &b) {
			this->resize(b.size);
			memcpy(this->val, b.val, b.size);
		}
		~hash_value() {
			//cout << "got inside destructor with size = " << size << endl;
			//cout << "got inside destructor with addres " << this << endl;
			if(size > 0) free(val);
			//cout << "got past free" << endl;
		}

		void operator=(const hash_value &b) {
			//cout << "got inside = with size = " << b.size << endl;
			if(this->size > 0) {
				free(this->val);
				this->size = 0;
			}

			this->resize(b.size);
			memcpy(this->val, b.val, b.size);
		}

		void resize(size_t n) { val=(UCHAR *)malloc(n); size = n; }
		UCHAR *val;
		int size;
};

ostream & operator<<(ostream &out, const hash_value &a) {
	for(int i = 0; i < a.size; i++) {
		cout << setw(2) << setfill('0') << hex << int(a.val[i]) << dec;
	}

	return out;
}

class hash_compare {
	public:
		bool operator()(const hash_value &a, const hash_value &b) const;
};

//less than operator operator that I can pass to the map.
//Longer strings are considered greater
bool hash_compare::operator()(const hash_value &a, const hash_value &b) const {
	if(a.size < b.size) {
		return true;
	} else if(a.size > b.size) {
		return false;
	} else {
		return memcmp((void *)(a.val), (void *)(b.val), a.size) < 0;
	}
}

UCHAR * gen_random_uchar(size_t n) {
	UCHAR *temp = (UCHAR *)malloc(n);
	for(int i = 0; i < n; i++) {
		temp[i] = (UCHAR)rand();
	}

	return temp;
}

void gen_hash(UCHAR *input, size_t size, UCHAR *output) {
	SHA256(input, size, output);
}


void generate_rainbow_table(map<hash_value, hash_value, hash_compare> &rainbow_table, int s, int t) {
	hash_value SP, temp, EP;

	//generate s random starting points, compute their endpoint (after t hashes) and store them
	for(int i = 0; i < s; i++) {
		SP.val = gen_random_uchar(HASH_SIZE);
		SP.size = HASH_SIZE;

		temp = SP;
		EP.resize(HASH_SIZE);
		for(int j = 0; j < t; j++) {
			gen_hash(temp.val, temp.size, EP.val);
			temp = EP;
		}

		rainbow_table[EP] = SP;
	}
}

//void find_collision(map<hash_value, hash_value, hash_compare> &rainbow_table, char *starting_hash_c, int s, int t) {



int main(int argc, char **argv) {
	int n, s, t;
	map<hash_value, hash_value, hash_compare> rainbow_table;
	map<hash_value, hash_value, hash_compare>::iterator it;
	char *starting_hash_c;

	if(argc != 4) {
		cout << "Usage: " << argv[0] << " num_bits s t" << endl;
		return 0;
	}

	//get n
	n = atoi(argv[1]);
	s = atoi(argv[2]);
	t = atoi(argv[3]);

	starting_hash_c = "tylerjeremey";

	generate_rainbow_table(rainbow_table, s, t);
	/*for(it = rainbow_table.begin(); it != rainbow_table.end(); it++) {
		cout << it->first << " " << it->second << endl;
	}*/
//	find_collision(rainbow_table, starting_hash_c, s, t);
	return 0;
}







