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
 * NOTE : These classes have their copy and assignment constructors disabled
 * for now.
 */
class CppPetscVec {
public :
	/// Actual data storage
	Vec data;

	/// Trivial constructor
	CppPetscVec() : data(PETSC_NULL) {};

	/** Constructor
	 *
	 * @param ntot (PetscInt) total number of particles
	 * @param nlocal (PetscInt) local number of particles [PETSC_DECIDE]
	 */
	CppPetscVec(PetscInt ntot, PetscInt nlocal=PETSC_DECIDE)
	   { safeCall(VecCreateMPI(PETSC_COMM_WORLD, nlocal, ntot, &data),
			   "Error allocating vector"); };

	/// Destructor
	~CppPetscVec() {if (data != PETSC_NULL) safeCall(VecDestroy(&data),
			"Error destroying vector");};

	/** Get the ownership range
	 *
	 * @param lo (PetscInt) -- the lower range returned
	 * @param hi (PetscInt) -- the upper range returned
	 */
	void getOwnershipRange(PetscInt &lo, PetscInt &hi) {
		VecGetOwnershipRange(data, &lo, &hi);
	}

	/** Get array
	 *
	 * Return a pointer to the data
	 *
	 * @return PetscScalar * pointer to the data array
	 */
	PetscScalar * get() {
		PetscScalar *_x;
		VecGetArray(data, &_x);
		return _x;
	}

	/** Restore array
	 *
	 * Make the array global again
	 *
	 * @param x (PetscScalar*) -- the pointer from get()
	 */
	void restore(PetscScalar *x) {
		VecRestoreArray(data, &x);
	}

	/** Set to a scalar value
	 *
	 * overloaded operator=
	 *
	 * @param x (PetscScalar) -- value to set to
	 */
	void operator= (PetscScalar x) {
		VecSet(data, x);
	}


private :
	// disable copy constructor and assignment operator
	CppPetscVec& operator= (const CppPetscVec& v);
	CppPetscVec(const CppPetscVec& v);
};


#endif /* CPPPETSC_H_ */
