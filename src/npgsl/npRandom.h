/*
 * npRandom.h
 *
 *  Created on: Jul 20, 2012
 *      Author: npadmana
 */

#ifndef NPRANDOM_H_
#define NPRANDOM_H_

#include <gsl/gsl_rng.h>
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

#endif /* NPRANDOM_H_ */
