/*
 * npRandom.h
 *
 *  Created on: Jul 20, 2012
 *      Author: npadmana
 */

#ifndef NPRANDOM_H_
#define NPRANDOM_H_

#include <gsl/gsl_rng.h>
#include <gsl/gsl_qrng.h>
#include <vector>
#include <Eigen/Core>

/** Wrapper around the GSL random number generators
 *
 *  Automatically switches to using the Mersenne Twister
 *
 *  NOTE : This class cannot be copied or assigned.
 */
class npRandom {
public:

	/** Constructor
	 *
	 * @param seed (unsigned long int)
	 */
	npRandom(unsigned long int seed);

	/// Destructor
	~npRandom();

	/// Returns a random number in [0,1)
	double operator()();

	/** Fill an n-dimensional vector with random numbers in [0,1)
	 *
	 * @param n (integer)
	 *   returns vector<double>
	 */
	std::vector<double> operator()(int n);

	/** Returns a random 3D direction
	 *
	 * returns Eigen::Vector3d(x,y,z)
	 *   such that sqrt(x^2 + y^2 + z^2) = 1.
	 */
	Eigen::Vector3d dir3d();

private :
	// Disable copy and assignment
	npRandom(const npRandom& x);
	npRandom& operator=(const npRandom& x);

	// Random number engine
	gsl_rng *ran;



};

/** Wrapper around the GSL quasi-random number generators
 *
 *  The default here is the Niederreiter sequence, but Sobol, Halton, and
 *  reverse Halton sequences are also available.
 *
 *  These are gsl_qrng_niederreiter_2, gsl_qrng_sobol, gsl_qrng_halton, and
 *  gsl_qrng_reversehalton.
 *
 *  NOTE : This class cannot be copied or assigned.
 */
class npQuasiRandom {
public:

	/** Constructor
	 *
	 * @param dim (int)
	 */
	npQuasiRandom(int _dim, const gsl_qrng_type *type= gsl_qrng_niederreiter_2);

	/// Destructor
	~npQuasiRandom();

	/// Returns the next quasi-random number in [0,1)^dim
	std::vector<double> operator()();

private :
	// Disable copy and assignment
	npQuasiRandom(const npQuasiRandom& x);
	npQuasiRandom& operator=(const npQuasiRandom& x);

	// Random number engine
	gsl_qrng *qran;

	// Dimension
	int dim;
};





#endif /* NPRANDOM_H_ */
