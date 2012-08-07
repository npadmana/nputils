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
	v.getOwnershipRange(lo, hi);
	v.get();
	for (ii=0; ii < (hi-lo); ++ii) func(v[ii]);
	v.restore();
}

/** Similar to npForEach for 1 vector, except overloaded for 2 vectors.
 *
 * Note that the two vectors must have the same distributions.
 * FIXME : No checking on the distributions is currently done.
 */
template
<class Vec, class Function>
void npForEach(Vec& v1, Vec& v2, Function func) {
	typename Vec::Index lo, hi, ii;
	v1.getOwnershipRange(lo, hi);
	v1.get(); v2.get();
	for (ii=0; ii < (hi-lo); ++ii) func(v1[ii], v2[ii]);
	v1.restore(); v2.restore();
}

/** Similar to npForEach for 1 vector, except overloaded for 3 vectors.
 *
 * Note that the vectors must have the same distributions.
 * FIXME : No checking on the distributions is currently done.
 */
template
<class Vec, class Function>
void npForEach(Vec& v1, Vec& v2, Vec& v3, Function func) {
	typename Vec::Index lo, hi, ii;
	v1.getOwnershipRange(lo, hi);
	v1.get(); v2.get(); v3.get();
	for (ii=0; ii < (hi-lo); ++ii) func(v1[ii], v2[ii], v3[ii]);
	v1.restore(); v2.restore(); v3.restore();
}


#endif /* NPPM_ALGORITHMS_H_ */
