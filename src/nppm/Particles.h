/*
 * Particles.h
 *
 *  Created on: Jul 20, 2012
 *      Author: npadmana
 */

#ifndef PARTICLES_H_
#define PARTICLES_H_

#include <memory>
#include <vector>
#include "cpppetsc.h"
#include "nppm_algorithms.h"

/** Wrap things around in a periodic box
 *
 * @param x (double) -- value
 * @param L (double) -- dimension size
 * @return periodically wrapped coordinate
 */
double periodic(double x, double L);

/** Partition code
 *
 * @param nobj -- Number of objects to partition across n jobs
 * @param size -- number of jobs to partition across
 * @param rank -- rank of particular job
 *
 * @return number of objects local to job "rank"
 */
template <class IntType>
IntType partition(IntType nobj, int size, int rank) {
	IntType base = nobj/size;
	IntType remainder = nobj%size;

	if (rank >= (size-remainder)) base++;

	return base;
}


template <class T>
class Particles {

public :
	/// Set up typedefs for data storage and indices
	typedef  T Value;
	typedef CppPetscVec::Index Index;

	/// number of particles
	Index npart;


	/// Default constructor
	Particles() : npart(0) {};

	/** Constructor
	 *
	 * Allocate the arrays
	 *
	 * @param _npart (PetscInt) number of particles
	 * @param nlocal (PetscInt) local number of particles [PETSC_DECIDE]
	 */
	Particles(Index _npart, Index nlocal=PETSC_DECIDE) {
		init(_npart, nlocal);
	}

	/** Initialization routine
	 *
	 * It is useful to separate this from the constructor, since
	 * we likely will have cases to set up the memory after the initial
	 * construction.
	 *
	 * IMPORTANT : These are the number of particles, not the underlying
	 * vector size. The data are automatically scaled
	 *
	 * @param _npart (PetscInt) number of particles (can be PETSC_DETERMINE)
	 * @param nlocal (PetscInt) local number of particles [PETSC_DECIDE]
	 *
	 *
	 */
	void init(Index _npart, Index nlocal=PETSC_DECIDE) {
		npart = _npart;
		if (nlocal==PETSC_DECIDE) {
			int rank, size;
			MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
			MPI_Comm_size(PETSC_COMM_WORLD, &size);
			vec.init(PETSC_DETERMINE, partition(npart, size, rank)*nfac);
		} else {
			vec.init(PETSC_DETERMINE, nlocal*nfac);
		}
	}


	/** Return the ownership range
	 *
	 * This is again in particles, not the underlying representation
	 *
	 * @param lo (Index) : Return the lower index
	 * @param hi (Index) : Return the upper index
	 * @param ivec (Index) : use the ivec'th vector to do this.
	 */
	void getOwnershipRange(Index& lo, Index &hi) {
		vec.getOwnershipRange(lo, hi);
		lo /= nfac;
		hi /= nfac;
	}

	/** Get data for local use
	 *
	 */
	void get() {
		vec.get();
		_data = reinterpret_cast<Value *>(&vec[0]);
	}

	/** Restore data for global use
	 *
	 */
	void restore() {
		// Simple restore wrapper
		vec.restore();
	}

	/** Access operator []
	 *
	 * @param ii  (Index) index into the array, using local indices
	 *
	 * NOTE : No error bounds checking is done.
	 * NOTE : You must call get() before using this, and restore() after
	 */
	Value& operator[] (Index ii) {
		return _data[ii];
	}

	/** Const version of operator[]
	 *
	 */
	const Value& operator[] (Index ii) const {
		return _data[ii];
	}

	/** Domain decompose particles
	 *
	 * @param domainfunc : function that takes in T and returns
	 *              the processor the data needs to go to.
	 *              If -1, then the particle is not scattered.
	 *
	 *
	 */
	template <class Func>
	void domainDecompose(Func domainfunc, Particles<T>* outptr=NULL) {
		int rank, size;
		std::vector<int> narr, narr1;
		std::vector<Index> icount;
		Index nlocal, lo, hi;

		// Get local length etc....
		getOwnershipRange(lo, hi);
		nlocal = hi-lo;

		// Get the rank and size
		MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
		MPI_Comm_size(PETSC_COMM_WORLD, &size);

		// Figure out the mapping
		std::vector<int> iproc(nlocal);
		{
			int ii=0;
			int iproc1;
			npForEach(*this, [&](T &p){
				iproc[ii] = domainfunc(p);
				if (iproc[ii] >= size) {
					safeCall(99, "ERROR!! Processor out of range\n");
				}
				ii++;
			});
		}


		// Initialize
		narr.resize(size); icount.resize(size);
		narr1.resize(size*size);
		for (int ii=0; ii < size; ++ii) narr[ii] = 0;

		for (int ii : iproc) {
			if (ii > -1) narr[ii] += nfac; // Account for nfac
		}

		// Collect all the information
		MPI_Allgather(&narr[0], size, MPI_INT, &narr1[0], size, MPI_INT, PETSC_COMM_WORLD);

		// Figure out starting locations for particles
		for (int ii=0; ii < size; ++ii) {narr[ii] = 0; icount[ii] = 0;}
		for (int ii=0; ii < size; ++ii) {
			for (int jj=0; jj < size; ++jj)
				narr[ii] += narr1[ii+jj*size];
			for (int jj=0; jj < rank;++jj)
				icount[ii] += narr1[ii+jj*size];
		}
		Index rtot = 0;
		for (int ii=0; ii < size; ++ii) {
			icount[ii] += rtot;
			rtot += narr[ii];
		}


		CppPetscVec tmp(PETSC_DETERMINE, narr[rank]);

		Index lo1, hi1;
		vec.getOwnershipRange(lo1,hi1);

		IS from, to;
		VecScatter vs;

		// We will scatter each of the nfac elements separately
		std::vector<Index> idx1, idx2;

		// This sets up the scatters
		{
			int iproc1;
			for (Index ii=lo; ii < hi; ++ii) {
				iproc1 = iproc[ii-lo];
				if (iproc1 > -1) {
					idx1.push_back(ii*nfac);
					idx2.push_back(icount[iproc1]);
					icount[iproc1] += nfac;
				}
			}
		}

		for (int ifac=0; ifac < nfac; ++ifac) {
			ISCreateGeneral(PETSC_COMM_WORLD, idx1.size(),
					static_cast<Index*>(&idx1[0]), PETSC_USE_POINTER, &from);
			ISCreateGeneral(PETSC_COMM_WORLD, idx2.size(),
					static_cast<Index*>(&idx2[0]), PETSC_USE_POINTER, &to);

			safeCall(VecScatterCreate(vec.data, from, tmp.data, to, &vs),
					"Failed generating scatter set");

			VecScatterBegin(vs, vec.data, tmp.data, INSERT_VALUES, SCATTER_FORWARD);
			VecScatterEnd(vs, vec.data, tmp.data, INSERT_VALUES, SCATTER_FORWARD);


			ISDestroy(&from);
			ISDestroy(&to);
			VecScatterDestroy(&vs);

			// Move to the next element
			for (Index &jj : idx1) jj++;
			for (Index &jj : idx2) jj++;

		}

		if (outptr == NULL) outptr = this;
		outptr->npart = tmp.size()/nfac;
		outptr->vec.swap(tmp);
	}


private :
	// Turn off copy constructors and assignment operators
	Particles(const Particles<T> &p);
	Particles& operator=(const Particles<T> &p);

	// The underlying vector
	// This is private, since directly using it can be
	// tricky
	CppPetscVec vec;


	// Internal pointer to data
	Value *_data;

	// Assertion check that the structure can be mapped to the
	// underlying type.
	static_assert((sizeof(T)%sizeof(CppPetscVec::Value)) == 0,
			"Size of structure needs to be multiple of size of underlying type");

	// Map the internal PetscVec type to the type we want
	const int nfac = sizeof(T)/sizeof(CppPetscVec::Value);

};


#endif /* PARTICLES_H_ */
