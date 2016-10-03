//Tyler Stuessi
//Jeremy Herwig
//
//Problem 2: Finding untargeted collisions
//Finding untargeted collisions using the small space
//Birthday attack
//
//Moved this over to python because comparing binary strings
//in python is annoying
//
//Using the sha256 implementation from openssl

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sha256.h"

using namespace std;

typedef unsigned char UCHAR;

long long num_hashes_computed = 0;

const int HASH_SIZE = 32;


//a and b are SHA256 hashes. This compares the first n bits
//of the two hashes to see if they are the same.
bool compare_n_bits(int n, UCHAR *a, UCHAR *b) {
	if(memcmp(a, b, HASH_SIZE) == 0) {
		return true;
	} else {
		return false;
	}
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

	memset((void *)(a+n/8+1), 0, HASH_SIZE-(n/8+1));
}

//generate a hash using SHA256
//ouput must be allocated for 20 bytes
void generate_hash(UCHAR *input, int size, UCHAR *output, int n) {
	sha256(input, size, output);
	convert_to_n_bits(n, output);
	num_hashes_computed++;
}

//utility function to print out a hash in hex
void print_hash(UCHAR *input) {
	for(int i = 0; i < HASH_SIZE; i++) {
		printf("%x",input[i]);
	}
	printf("\n");
}

void print_results(UCHAR *x, UCHAR *x_prime, UCHAR *hash_x, UCHAR *hash_x_prime) {
	printf("x = ");
	print_hash(x);
	printf("H(x) = ");
	print_hash(hash_x);
	printf("x_prime = ");
	print_hash(x_prime);
	printf("H(x_prime) = ");
	print_hash(hash_x_prime);
}

int main(int argc, char **argv) {
	if(argc != 2) {
		cout << "usage: " << argv[0] << " num_bits" << endl;
		return 0;
	}
	int n = atoi(argv[1]);

	int i, j;
	UCHAR *starting_hash, *x, *x_prime, *temp_hash1, *temp_hash_x, *temp_hash_x_prime;
	int hash_size = HASH_SIZE;
	char *starting_hash_c;

	//starting hash
	starting_hash_c = "stuessiherwig";
	starting_hash = (UCHAR *)starting_hash_c;

	//x and x_prime allocations (alongside temp hashes)
	x = (UCHAR *)malloc(sizeof(UCHAR)*HASH_SIZE);
	x_prime = (UCHAR *)malloc(sizeof(UCHAR)*HASH_SIZE);
	temp_hash1 = (UCHAR *)malloc(sizeof(UCHAR)*HASH_SIZE);
	temp_hash_x = (UCHAR *)malloc(sizeof(UCHAR)*HASH_SIZE);
	temp_hash_x_prime = (UCHAR *)malloc(sizeof(UCHAR)*HASH_SIZE);

	//unroll the first iteration of the loop 
	//to make memory management easier
	j = 1;
	generate_hash(starting_hash, strlen((char *)starting_hash), x, n);
	generate_hash(starting_hash, strlen((char *)starting_hash), temp_hash1, n);
	generate_hash(temp_hash1, hash_size, x_prime, n);

	//compare the two values
	//if they match, there is a cycle somewhere
	//else continue searching
	for(j = 2; compare_n_bits(n, x, x_prime) == false; j++) {
		generate_hash(x, HASH_SIZE, temp_hash_x, n);
		generate_hash(x_prime, HASH_SIZE, temp_hash1, n);
		generate_hash(temp_hash1, HASH_SIZE, temp_hash_x_prime, n);

		//copy temp variables back into original variables
		memcpy(x, temp_hash_x, HASH_SIZE);
		memcpy(x_prime, temp_hash_x_prime, HASH_SIZE);
	}

	//actually iterate through cycle and find grouping
	//again unroll the first iteration
	memcpy(x_prime, x, HASH_SIZE);
	generate_hash(starting_hash, strlen((char *)starting_hash), temp_hash_x, n);
	generate_hash(x_prime, HASH_SIZE, temp_hash_x_prime, n);

	if(compare_n_bits(n, temp_hash_x, temp_hash_x_prime)) {
		print_results(starting_hash, x_prime, temp_hash_x, temp_hash_x_prime);
		return 0;
	}

	while(!compare_n_bits(n, temp_hash_x, temp_hash_x_prime)) {
		memcpy(x, temp_hash_x, HASH_SIZE);
		memcpy(x_prime, temp_hash_x_prime,HASH_SIZE);

		generate_hash(x, HASH_SIZE, temp_hash_x, n);
		generate_hash(x_prime, HASH_SIZE, temp_hash_x_prime, n);
	}

	cout << "n = " << n<< endl;
	cout << "num_hashes_computed = " << num_hashes_computed << endl;
	print_results(x, x_prime, temp_hash_x, temp_hash_x_prime);

	return 0;
}
