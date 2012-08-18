/*
 * cpppetsc.h
 *
 *  Created on: Jul 19, 2012
 *      Author: npadmana
 */

#ifndef CPPPETSC_H_
#define CPPPETSC_H_

#include <vector>

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

	/** Initialization, outside of a constructor
	 *
	 * @param ntot (PetscInt) total number of particles
	 * @param nlocal (PetscInt) local number of particles [PETSC_DECIDE]
	 *
	 */
	void init(Index ntot, Index nlocal=PETSC_DECIDE);

	/// Destructor
	~CppPetscVec();

	/** Get the ownership range
	 *
	 * @param lo (PetscInt) -- the lower range returned
	 * @param hi (PetscInt) -- the upper range returned
	 */
	void getOwnershipRange(Index &lo, Index &hi) const;

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
	Index size() const;

	/** Return the sum of all elements
	 *
	 * @return sum (Value : PetscScalar) -- sum of all the elements
	 */
	Value sum();

	/** Add a scalar value to all elements of a vector
	 *
	 * @param x (PetscScalar [Value]) -- compute vec += x
	 */
	CppPetscVec& operator+= (const Value& x);

	/** Multiply elementwise by a scalar
	 *
	 * @param x (PetscScalar [Value]) -- compute vec *= x elementwise
	 */
	CppPetscVec& operator*= (const Value& x);

	/** Compute y = alpha x + y
	 *
	 * @param x (CppPetscVec)
	 * @param alpha (PetscScalar)
	 *
	 * Note : This uses VecAXPY is x is different from y, else just scales by alpha+1
	 */
	void AXPY(const CppPetscVec& x, const Value& alpha);

	/** Compute y = x + alpha y
	 *
	 * @param x (CppPetscVec)
	 * @param alpha (PetscScalar)
	 *
	 * Note : This uses VecAXPY is x is different from y, else just scales by alpha+1
	 */
	void AYPX(const CppPetscVec& x, const Value& alpha);

	/** Compute y = alpha x + beta y
	 *
	 * @param x (CppPetscVec)
	 * @param alpha (PetscScalar)
	 *
	 * Note : This uses VecAXPY is x is different from y, else just scales by alpha+1
	 */
	void AXPBY(const CppPetscVec& x, const Value& alpha, const Value& beta);



	/** Assignment operator
	 *
	 * Note that this makes a deep copy, and is therefore expensive.
	 */
	CppPetscVec& operator= (const CppPetscVec& v);

	/** Copy constructor
	 *
	 * Note that this makes a deep copy and is expensive.
	 */
	CppPetscVec(const CppPetscVec& v, bool shallowcopy=false);


	/** Swap the underlying Petsc vectors
	 *
	 * @param v (vector to swap with)
	 */
	void swap(CppPetscVec& v);

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

	/** Tests compatibility with another vector
	 *
	 * @param x (CppPetscVec)
	 *
	 * @returns true if vectors have the same layout
	 */
	bool is_compatible(const CppPetscVec& x) {
		Index lo1, lo2, hi1, hi2;
		this->getOwnershipRange(lo1, hi1);
		x.getOwnershipRange(lo2, hi2);
		return (lo1==lo2) && (hi1==hi2);
	}

	/** Set values in a block, across processors
	 *
	 * @param idx vector<Index> (indices to fill in)
	 * @param val vector<Value> (values)
	 * @param iora (ADD_VALUES/INSERT_VALUES)
	 *
	 * IMPORTANT NOTE : This is not a collective operation
	 * IMPORTANT NOTE : You must call assemblyBegin and assemblyEnd before
	 *   using the vector.
	 */
	void set(const std::vector<Index> &idx, const std::vector<Value> &val,
			InsertMode iora);

	/** Begin vector assembly
	 *
	 * Separate routine if you want to interleave computation and communication
	 */
	void assemblyBegin();

	/** Begin vector assembly
	 *
	 * Separate routine if you want to interleave computation and communication
	 */
	void assemblyEnd();


private :
	Value *_data;
};


#endif /* CPPPETSC_H_ */
