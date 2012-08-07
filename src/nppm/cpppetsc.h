/*
 * cpppetsc.h
 *
 *  Created on: Jul 19, 2012
 *      Author: npadmana
 */

#ifndef CPPPETSC_H_
#define CPPPETSC_H_

#include "petscvec.h"
#include "np_petsc_utils.h"

/** Wrapper class for PETSc vectors.
 *
 * This is a useful abstraction since it frees one from the need
 *  to explicitly clean up vectors and allows one to use PETSc vectors
 *  safely inside classes. The actual PETSc vector is directly accessible
 *  through the data member.
 *
 *  This function automatically assumes PETSC_COMM_WORLD as the communicator.
 *
 */
class CppPetscVec {
public :
	/// Integer indexing typedef
	typedef PetscInt Index;

	/// Value typedef
	typedef PetscScalar Value;

	/// Actual data storage
	Vec data;

	/// Trivial constructor
	CppPetscVec() : data(PETSC_NULL) {};

	/** Constructor
	 *
	 * @param ntot (PetscInt) total number of particles
	 * @param nlocal (PetscInt) local number of particles [PETSC_DECIDE]
	 */
	CppPetscVec(Index ntot, Index nlocal=PETSC_DECIDE);

	/// Destructor
	~CppPetscVec();

	/** Get the ownership range
	 *
	 * @param lo (PetscInt) -- the lower range returned
	 * @param hi (PetscInt) -- the upper range returned
	 */
	void getOwnershipRange(Index &lo, Index &hi);

	/** Get array
	 *
	 * Return a pointer to the data
	 *
	 */
	void get();

	/** Restore array
	 *
	 * Make the array global again
	 *
	 */
	void restore();

	/** Set to a scalar value
	 *
	 * overloaded operator=
	 *
	 * @param x (PetscScalar) -- value to set to
	 */
	void operator= (Value x);

	/** Compare two vectors
	 *
	 * overloaded operator==
	 *
	 * @param vec (const CppPetscVec &) -- vector to compare against
	 */
	bool operator== (const CppPetscVec& vec);


	/** Return the global size
	 *
	 * @return nn (PetscInt)
	 */
	Index size();

	/** Return the sum of all elements
	 *
	 * @return sum (Value : PetscScalar) -- sum of all the elements
	 */
	Value sum();

	/** Assignment operator
	 *
	 * Note that this makes a deep copy, and is therefore expensive.
	 */
	CppPetscVec& operator= (const CppPetscVec& v);

	/** Copy constructor
	 *
	 * Note that this makes a deep copy and is expensive.
	 */
	CppPetscVec(const CppPetscVec& v);


	/** Access operator []
	 *
	 * @param ii  (Index) index into the array, using local indices
	 *
	 * NOTE : No error bounds checking is done.
	 * NOTE : You must call get() before using this, and restore() after
	 */
	Value& operator[] (Index ii);

	/** Const version of operator[]
	 *
	 */
	const Value& operator[] (Index ii) const;


private :
	Value *_data;
};


#endif /* CPPPETSC_H_ */
