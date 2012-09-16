/*
 * npRandom.cpp
 *
 *  Created on: Jul 20, 2012
 *      Author: npadmana
 */

#include "npRandom.h"
#include "gsl/gsl_randist.h"

npRandom::npRandom(unsigned long int seed) {
	  ran = gsl_rng_alloc(gsl_rng_mt19937);
	  gsl_rng_set(ran, seed);
}

npRandom::~npRandom() {
	gsl_rng_free(ran);
}

double npRandom::operator ()() {
	return gsl_rng_uniform(ran);
}

Eigen::Vector3d npRandom::dir3d() {
	Eigen::Vector3d out;
	double x, y, z;
	gsl_ran_dir_3d(ran, &x, &y, &z);
	out << x,y,z;
	return out;
}
