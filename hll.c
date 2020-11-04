// Copyright 2020 Dalimon Iarina

#include <stdio.h>
#include <stdlib.h>

#define MMAX 2048  //  val lui 2 la puterea a 11
#define K 11  //  nr de biti in fct de care se det bucketul

unsigned int hash_function_int(void *a) {
    /*
     * Credits: https://stackoverflow.com/a/12996028/7883884
     */
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}

//  fct de mai jos determina al numarul bucketului in care va fi
//  introdus numarul calculat
unsigned int get_the_1st_k_bits(unsigned int index) {
	unsigned int index_copy;
	int rest_bits;

	rest_bits = 8 * sizeof(unsigned int) - K;
	index_copy = index;
	index_copy = index >> rest_bits;
	return index_copy;
}

//  fct de mai jos imi ia restul de bits
unsigned int get_the_last_bits(unsigned int index) {
	unsigned int index_copy;

	index_copy = index;
	index_copy = index << K;
	return index_copy;
}

//  fct de mai jos ridica un nr la o anumita putere
int power(int n, int k) {
	int x;
	x = n;
	if (k == 1) {
		return x;
	} else {
		return n * power(n, k - 1);
	}
}

//  fct de mai jos calc nr de 0 cerut in cerinta
int get_nr_of_0(unsigned int index) {
	int nr = 0, i, masc, pow;

	pow = 8 * sizeof(index) - 1;
	masc = 1 * power(10, pow);
	pow++;
	for(i = 0; i < pow; ++i) {
  		if ((index & (masc >> i)) == 0) {
    		++nr;
  		} else {
  			return nr;
  		}
	}
	return nr;
}

//  fct de mai jos returneaza maximul dintre 2 nr
int maxi(int a, int b) {
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

//  fct de mai jos calculeaza z din enunt
float calc_z(int m[MMAX]) {
	int i;
	float sum = 0, pow, z;
	for (i = 0; i < MMAX; i++) {
		if (m[i] != -1) {
			pow = 1 << m[i];
			pow = 1 / pow;
			sum += pow;
		}
	}
	z = 1 / sum;
	return z;
}

//  fct de mai jos calculeaza e din anunt
int calc_e(float z) {
	int e;
	float alfa, e_float;
	alfa = 1.079 / MMAX;
	alfa = alfa + 1;
	alfa = 0.7213 / alfa;
	e_float = alfa * MMAX * MMAX * z;

	e = (int)e_float * 2;
	return e;
}

int main(int argc, char *argv[]) {
	FILE *f;
	float z;
	int *m, nr_zero, i, nr, e;
	unsigned int index, index_k, index_copy;

	m = malloc(MMAX * sizeof(int));
	if (m == NULL) {
		return -1;
	}
	for (i = 0; i < MMAX; i++) {
		m[i] = -1;
	}

	f = fopen(argv[argc-1], "rt");
	if (f == NULL) {
		return -1;
	}

	while (!feof(f)) {
		fscanf(f, "%d", &nr);
		index = hash_function_int(&nr);
		index_k = get_the_1st_k_bits(index);
		index_copy = get_the_last_bits(index);
		nr_zero = get_nr_of_0(index_copy);

		if (m[index_k] == -1) {
			m[index_k] = nr_zero;
		} else {
			m[index_k] = maxi(m[index_k], nr_zero);
		}
	}

	z = calc_z(m);
	e = calc_e(z);

	printf("%d\n", e);
	fclose(f);
	free(m);
	return 0;
}
