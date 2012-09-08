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
#include "petscmat.h"
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
	 * @param beta (PetscScalar)
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

	/** Set values in a block, across processors
	 *
	 * This overloaded version uses a pointer instead of a vector, mostly
	 * for ugly memory tricks.
	 *
	 * @param idx vector<Index> (indices to fill in)
	 * @param val Value* (values)
	 * @param iora (ADD_VALUES/INSERT_VALUES)
	 *
	 * IMPORTANT NOTE : This is not a collective operation
	 * IMPORTANT NOTE : You must call assemblyBegin and assemblyEnd before
	 *   using the vector.
	 */
	void set(const std::vector<Index> &idx, const Value* val,
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

/** Wrapper class for PETSc matrices.
 *
 * As with CppPetscVec, this wraps Petsc matrices, freeing the user
 * from the tedium of remembering to free matrices etc. As with the
 * vector classes, the underlying data member is accessible.
 *
 * The current version is specialized to sparse MPI AIJ matrices.
 *
 * Unlike vectors, the various ways to play with matrices is more limited,
 * since I only need very specific functionality. This may change with time.
 *
 * Matrices are not assignable or copyable.
 */
class CppPetscMat {

public :
	/// Integer indexing typedef
	typedef PetscInt Index;

	/// Value typedef
	typedef PetscScalar Value;

	Mat data;

	/// Default constructor
	CppPetscMat() : data(PETSC_NULL) {};

	/// Destructor
	~CppPetscMat();

	/** Constructor
	 *
	 * This version simply specifies the number of local rows
	 * and columns. We assume that matrices are created after vectors,
	 * in which case the local information is known.
	 *
	 * No pre-allocation is done in this case.
	 *
	 * The matrix is assumed to have dimensions for the matrix-vector product y=Ax
	 *
	 * @param  ny (Index) local number of rows for the vector y in y=Ax
	 * @param  nAx (Index) local number of rows for the the vector x in y=Ax
	 */
	CppPetscMat(Index ny, Index nAx);


	/** Constructor
	 *
	 * This version specifies the number of local rows
	 * and columns. We assume that matrices are created after vectors,
	 * in which case the local information is known.
	 *
	 * It also allows specifying the number of non-zero elements in the
	 * "diagonal" and "off-diagonal matrices" (assumed to be the same for every row)
	 *
	 * The matrix is assumed to have dimensions for the matrix-vector product y=Ax
	 *
	 * @param  ny (Index) local number of rows for the vector y in y=Ax
	 * @param  nAx (Index) local number of rows for the the vector x in y=Ax
	 * @param  d_nz (Index) number of nonzero elts per row (same for all rows) in the diagonal part
	 * @param  o_nz (Index) number of nonzero elts per row (same for all rows) in the off-diagonal part
	 *
	 * Read the PETSC documentation for definitions of d_nz and o_nz.
	 */
	CppPetscMat(Index ny, Index nAx, Index d_nz, Index o_nz);


	/** Constructor
	 *
	 * This version specifies the number of local rows
	 * and columns. We assume that matrices are created after vectors,
	 * in which case the local information is known.
	 *
	 * It also allows specifying the number of non-zero elements per row in the
	 * "diagonal" and "off-diagonal matrices".
	 *
	 * The matrix is assumed to have dimensions for the matrix-vector product y=Ax
	 *
	 * @param  ny (Index) local number of rows for the vector y in y=Ax
	 * @param  nAx (Index) local number of rows for the the vector x in y=Ax
	 * @param  d_nnz vector<Index> number of nonzero elts per row in the diagonal part
	 * @param  o_nnz vector<Index> number of nonzero elts per row in the off-diagonal part
	 *
	 * Read the PETSC documentation for definitions of d_nnz and o_nnz.
	 *
	 */
	CppPetscMat(Index ny, Index nAx, const std::vector<Index>& d_nnz,
			const std::vector<Index>& o_nnz);



	/* Returns the size of the matrix
	 *
	 * @param M (Index) [output] size of the y array
	 * @param N (Index) [output] size of the Ax array
	 */
	void size(Index& M, Index& N);

	/* Returns the local range of rows stored
	 *
	 * @param lo (Index) holds the lower row index
	 * @param hi (Index) holds the upper range index
	 *
	 * The local rows are lo <= irow < hi
	 */
	void getOwnershipRange(Index& lo, Index& hi);


	/* Set a single element in the matrix
	 *
	 * @param m (Index) : row index in the matrix
	 * @param n (Index) : column index in the matrix
	 * @param val (Value) : value
	 * @param iora (InsertMode) : ADD_VALUES/INSERT_VALUES
	 */
	void set(Index m, Index n, Index val, InsertMode iora);

	/* Set a logically dense block into the matrix
	 *
	 * @param m vector<Index> : indices of rows
	 * @param n vector<Index> : indices of columns
	 * @param val vector<Value> : indices of values
	 * @param iora (InsertMode) : ADD_VALUES/INSERT_VALUES
	 *
	 */
	void set(const std::vector<Index>& m, const std::vector<Index>& n, const std::vector<Value>& val, InsertMode iora);

	/* Start the matrix assembly
	 *
	 * @param type [MatAssemblyType (see PETSc doc)] : MAT_FLUSH_ASSEMBLY/MAT_FINAL_ASSEMBLY; defaults to MAT_FINAL_ASSEMBLY
	 */
	void assemblyBegin(MatAssemblyType type=MAT_FINAL_ASSEMBLY);

	/* End the matrix assembly
	 *
	 * @param type [MatAssemblyType (see PETSc doc)] : MAT_FLUSH_ASSEMBLY/MAT_FINAL_ASSEMBLY; defaults to MAT_FINAL_ASSEMBLY
	 */
	void assemblyEnd(MatAssemblyType type=MAT_FINAL_ASSEMBLY);


	/* Matrix multiplication
	 *
	 * Compute y=Ax
	 *
	 * @param x (CppPetscVec) : the vector x (in)
	 * @param y (CppPetscVec) : the vector y (out)
	 *
	 * NOTE : x and y cannot be the same vector
	 */
	void mult(CppPetscVec& x, CppPetscVec& y);

	/* Matrix transposed multiplication
	 *
	 * Compute y=A^t x
	 *
	 * @param x (CppPetscVec) : the vector x (in)
	 * @param y (CppPetscVec) : the vector y (out)
	 *
	 * NOTE : x and y cannot be the same vector
	 */
	void multTransposed(CppPetscVec& x, CppPetscVec& y);


private :
	// Not copyable or assignable
	CppPetscMat(const CppPetscMat& x);
	CppPetscMat& operator= (const CppPetscMat& x);
};




#endif /* CPPPETSC_H_ */
