/*
 * nppm_algorithms.h
 *
 *  Created on: Jul 21, 2012
 *      Author: npadmana
 */

#ifndef NPPM_ALGORITHMS_H_
#define NPPM_ALGORITHMS_H_

/** A simple transformation algorithm
 *
 * This run over all the local values, and calls a function
 * on them. Modeled on the STL algorithms
 *
 * @param v (Templated Vec) vector; the template must define getOwnershipRange,
 * get and restore methods. See CppPetscVec for an example
 * @param func class with operator () defined; takes in T& x as an argument.
 *
 * The code is equivalent to the following :
 *
 * v.getOwnershipRange(lo, hi);
 * T* _x = v.get();
 * for (ii=0; ii < (hi-lo); ++ii) func(_x[ii]);
 * v.restore(_x);
 *
 * */
template
<class Vec, class Function>
void npForEach(Vec& v, Function func) {
	typename Vec::Index lo, hi, ii;
	typename Vec::Value* _x;
	v.getOwnershipRange(lo, hi);
	_x = v.get();
	for (ii=0; ii < (hi-lo); ++ii) func(_x[ii]);
	v.restore(&_x);
}




#endif /* NPPM_ALGORITHMS_H_ */
