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

	/// Indexing typedef
	typedef CppPetscVec::Index Index;
	typedef CppPetscVec Value;

	/// Number of particles
	Index npart;

	/// Array of auto_ptrs of CppPetscVecs
	std::auto_ptr<CppPetscVec> ptrs[nvec];
	// FIXME : This should probably be changed to unique_ptr, but it causes all sorts
    // of eclipse headaches.

	/// Default constructor
	Particles() : npart(0) {}

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
	 * construction
	 *
	 * @param _npart (PetscInt) number of particles
	 * @param nlocal (PetscInt) local number of particles [PETSC_DECIDE]
	 */
	void init(Index _npart, Index nlocal=PETSC_DECIDE) {
		npart = _npart;
		for (auto &pp : ptrs)
			pp.reset(new CppPetscVec(npart, nlocal));
	}

	/** Get data for local use
	 *
	 */
	void get() {
		for (auto &pp : ptrs) {
			pp->get();
		}
	}

	/** Restore data for global use
	 *
	 */
	void restore() {
		// Simple restore wrapper
		for (auto &pp : ptrs) pp->restore();
	}

	/** Return the ownership range for the lo and hi vectors
	 *
	 * @param lo (Index) : Return the lower index
	 * @param hi (Index) : Return the upper index
	 * @param ivec (Index) : use the ivec'th vector to do this.
	 */
	void getOwnershipRange(Index& lo, Index &hi, int ivec=0) {
		ptrs[ivec]->getOwnershipRange(lo, hi);
	}

	/** Pointer to particle array ii
	 *
	 * This is a useful level of reorganization, for converting
	 * structure of arrays into an array of structures.
	 *
	 * Used as Particles[icoord][ipart]
	 *
	 * This may not be the most efficient way to do this problem.
	 * See Particles_test for examples on how to use this
	 *
	 * We do not have a const correct version of this as yet.
	 */
	Value& operator[](Index ii) {
		return *ptrs[ii];
	}

	/** Slab decompose particles
	 *
	 * @param L (double) size of box
	 * @param nproc (int) number of slabs or processors
	 * @param ivec (int) vector to use for the decomposition (default=0)
	 *
	 */
//	void SlabDecompose(double L, int nproc, int ivec=0) {
//	  int rank, size, proc, Ng, rtot;
//	  std::vector<int> narr, narr1;
//	  std::vector<PetscInt> idx,icount, icount_check;
//	  PetscInt nlocal, lo, hi;
//	  PetscScalar *_px;
//
//	  // Get the rank and size
//	  MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
//	  MPI_Comm_size(PETSC_COMM_WORLD, &size);
//	  narr.resize(size); icount.resize(size); narr1.resize(size*size); icount_check.resize(size);
//	  for (int ii=0; ii < size; ++ii) narr[ii] = 0;
//
//	  // Get the local range
//	  ptrs[ivec]->getOwnershipRange(lo, hi);
//	  nlocal = hi-lo;
//
//	  // Loop over all the particles, figuring out how many will go to each processor
//	  _px = ptrs[ivec]->get();
//	  for (PetscInt ii=0; ii < nlocal; ++ii) {
//		// which processor will this live on
//	    proc = static_cast<int>(periodic(_px[ii]/L,1.0) * size);
//	    // narr = the number of particles being handled by this process
//	    narr[proc]++;
//	  }
//	  ptrs[ivec]->restore(&_px);
//	  // Collect all the information
//	  MPI_Allgather(&narr[0], size, MPI_INT, &narr1[0], size, MPI_INT, PETSC_COMM_WORLD);
//
//	  // Figure out starting locations for particles
//	  for (int ii=0; ii < size; ++ii) {narr[ii] = 0; icount[ii] = 0;}
//	  for (int ii=0; ii < size; ++ii) {
//	    for (int jj=0; jj < size; ++jj)
//	      narr[ii] += narr1[ii+jj*size];
//	    for (int jj=0; jj < rank;++jj)
//	      icount[ii] += narr1[ii+jj*size];
//	  }
//	  rtot = 0;
//	  for (int ii=0; ii < size; ++ii) {
//	    icount[ii] += rtot;
//	    rtot += narr[ii];
//	    icount_check[ii] = icount[ii] + narr1[ii+rank*size];
//	  }
//	  if (rtot != npart) safeCall(99, "Assertion failed in particle distribution");
//
//	  // USEFUL DEBUGGING
//	  //  for (int ii=0; ii < size; ++ii) {
//	  //    PetscSynchronizedPrintf(PETSC_COMM_WORLD,"%i %i %llu %llu\n",
//	  //		  rank*size+ii,narr[ii], icount[ii], icount_check[ii]);
//	  //  }
//	  //  PetscSynchronizedFlush(PETSC_COMM_WORLD);
//
//
//	  // Figure out where everyone needs to go.
//	  _px = ptrs[ivec]->get();
//	  for (PetscInt ii=0; ii < nlocal; ++ii) {
//		proc = static_cast<int>(periodic(_px[ii]/L,1.0) * size);
//	    idx.push_back(icount[proc]);
//	    icount[proc]++;
//	  }
//	  ptrs[ivec]->restore(&_px);
//
//
//	  // Assertion
//	  for (int ii=0; ii < size; ++ii)
//	    if (icount[ii] != icount_check[ii])
//	    	safeCall(99,"Assertion failed : icount != icount_check");
//
//	  // Generate the two index sets
//	  IS from, to;
//	  ISCreateStride(PETSC_COMM_WORLD, nlocal, lo, 1, &from);
//	  ISCreateGeneral(PETSC_COMM_WORLD, nlocal, &idx[0], PETSC_USE_POINTER, &to);
//
//
//	  // Create a new temporary vector to scatter to
//	  CppPetscVec tmp(PETSC_DETERMINE, narr[rank]);
//	  VecScatter vs;
//	  safeCall(VecScatterCreate(ptrs[ivec]->data, from, tmp.data, to, &vs),
//			  "Failed generating scatter set");
//	  for (int ii=0; ii < nvec; ++ii) {
//		  VecScatterBegin(vs, ptrs[ii]->data, tmp.data, INSERT_VALUES, SCATTER_FORWARD);
//		  VecScatterEnd(vs, ptrs[ii]->data, tmp.data, INSERT_VALUES, SCATTER_FORWARD);
//		  *ptrs[ii] = tmp;
//	  }
//
//	  // Clean up
//	  ISDestroy(&from);
//	  ISDestroy(&to);
//	}


private :
	// Turn off copy constructors and assignment operators
	Particles(const Particles &p);
	Particles& operator= (const Particles &p);

};

#endif /* PARTICLES_H_ */
