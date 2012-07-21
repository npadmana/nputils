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
#include "npRandom.h"

/** Wrap things around in a periodic box
 *
 * @param x (double) -- value
 * @param L (double) -- dimension size
 * @return periodically wrapped coordinate
 */
double periodic(double x, double L);

/** A distributed particle class
 *
 * The particle data are stored in an array of auto_ptrs of CppPetscVecs.
 * The template parameter nvec sets the number of vectors.
 *
 * We disable copy constructors and assignment operators
 *
 */
template <int nvec>
class Particles {
public:

	/// Number of particles
	PetscInt npart;

	/// Array of auto_ptrs of CppPetscVecs
	std::auto_ptr<CppPetscVec> ptrs[nvec];

	/// Default constructor
	Particles() : npart(0) {}

	/** Constructor
	 *
	 * Allocate the arrays
	 *
	 * @param _npart (PetscInt) number of particles
	 * @param nlocal (PetscInt) local number of particles [PETSC_DECIDE]
	 */
	Particles(PetscInt _npart, PetscInt nlocal=PETSC_DECIDE) {
		init(_npart, nlocal);
	}

	/** Initialization routine
	 *
	 * It is useful to separate this from the constructor, since
	 * we likely will have cases to set up the memory after the initial
	 * construction
	 *
	 * @param _npart (PetscInt) number of particles
	 * @param nlocal (PetscInt) local number of particles [PETSC_DECIDE]
	 */
	void init(PetscInt _npart, PetscInt nlocal=PETSC_DECIDE) {
		npart = _npart;
		for (int ii=0; ii < nvec; ++ii) {
			(ptrs[ii]).reset(new CppPetscVec(npart, nlocal));
		}
	}

	/** Randomly fill with values from [0, 1)
	 *
	 * This is mostly for testing. Note that this fills all the vectors
	 * the same way.
	 */
	void fillRandom() {
		int rank;
		MPI_Comm_rank(PETSC_COMM_WORLD, &rank);

		npRandom rr(99+100*rank);
		for (int ii=0; ii < nvec; ++ii) {
			PetscScalar *x;
			PetscInt lo, hi;
			ptrs[ii]->getOwnershipRange(lo,hi);
			x = ptrs[ii]->get();
			for (PetscInt jj=lo; jj != hi; ++jj) x[jj-lo] = rr();
			ptrs[ii]->restore(&x);
		}
	}

	/** Slab decompose particles
	 *
	 * @param L (double) size of box
	 * @param nproc (int) number of slabs or processors
	 * @param ivec (int) vector to use for the decomposition (default=0)
	 *
	 */
	void SlabDecompose(double L, int nproc, int ivec=0) {
	  int rank, size, proc, Ng, rtot;
	  std::vector<int> narr, narr1;
	  std::vector<PetscInt> idx,icount, icount_check;
	  PetscInt nlocal, lo, hi;
	  PetscScalar *_px;

	  // Get the rank and size
	  MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
	  MPI_Comm_size(PETSC_COMM_WORLD, &size);
	  narr.resize(size); icount.resize(size); narr1.resize(size*size); icount_check.resize(size);
	  for (int ii=0; ii < size; ++ii) narr[ii] = 0;

	  // Get the local range
	  ptrs[ivec]->getOwnershipRange(lo, hi);
	  nlocal = hi-lo;

	  // Loop over all the particles, figuring out how many will go to each processor
	  _px = ptrs[ivec]->get();
	  for (PetscInt ii=0; ii < nlocal; ++ii) {
		// which processor will this live on
	    proc = static_cast<int>(periodic(_px[ii]/L,1.0) * size);
	    // narr = the number of particles being handled by this process
	    narr[proc]++;
	  }
	  ptrs[ivec]->restore(&_px);
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
	  rtot = 0;
	  for (int ii=0; ii < size; ++ii) {
	    icount[ii] += rtot;
	    rtot += narr[ii];
	    icount_check[ii] = icount[ii] + narr1[ii+rank*size];
	  }
	  if (rtot != npart) safeCall(99, "Assertion failed in particle distribution");

	  // USEFUL DEBUGGING
	  //  for (int ii=0; ii < size; ++ii) {
	  //    PetscSynchronizedPrintf(PETSC_COMM_WORLD,"%i %i %llu %llu\n",
	  //		  rank*size+ii,narr[ii], icount[ii], icount_check[ii]);
	  //  }
	  //  PetscSynchronizedFlush(PETSC_COMM_WORLD);


	  // Figure out where everyone needs to go.
	  _px = ptrs[ivec]->get();
	  for (PetscInt ii=0; ii < nlocal; ++ii) {
		proc = static_cast<int>(periodic(_px[ii]/L,1.0) * size);
	    idx.push_back(icount[proc]);
	    icount[proc]++;
	  }
	  ptrs[ivec]->restore(&_px);


	  // Assertion
	  for (int ii=0; ii < size; ++ii)
	    if (icount[ii] != icount_check[ii])
	    	safeCall(99,"Assertion failed : icount != icount_check");

	  // Generate the two index sets
	  IS from, to;
	  ISCreateStride(PETSC_COMM_WORLD, nlocal, lo, 1, &from);
	  ISCreateGeneral(PETSC_COMM_WORLD, nlocal, &idx[0], PETSC_USE_POINTER, &to);


	  // Create a new temporary vector to scatter to
	  CppPetscVec tmp(PETSC_DETERMINE, narr[rank]);
	  VecScatter vs;
	  safeCall(VecScatterCreate(ptrs[ivec]->data, from, tmp.data, to, &vs),
			  "Failed generating scatter set");
	  for (int ii=0; ii < nvec; ++ii) {
		  VecScatterBegin(vs, ptrs[ii]->data, tmp.data, INSERT_VALUES, SCATTER_FORWARD);
		  VecScatterEnd(vs, ptrs[ii]->data, tmp.data, INSERT_VALUES, SCATTER_FORWARD);
		  *ptrs[ii] = tmp;
	  }

	  // Clean up
	  ISDestroy(&from);
	  ISDestroy(&to);
	}


private :
	// Turn off copy constructors and assignment operators
	Particles(const Particles &p);
	Particles& operator= (const Particles &p);
};

#endif /* PARTICLES_H_ */
