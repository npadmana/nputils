/*
 * npRandom.cpp
 *
 *  Created on: Jul 20, 2012
 *      Author: npadmana
 */

#include "npRandom.h"

npRandom::npRandom(unsigned long int seed) {
	  gsl_rng *ran = gsl_rng_alloc(gsl_rng_mt19937);
	  gsl_rng_set(ran, seed);
}

npRandom::~npRandom() {
	gsl_rng_free(ran);
}

double npRandom::operator ()() {
	return gsl_rng_uniform(ran);
}

