/*
 * Particles.h
 *
 *  Created on: Jul 20, 2012
 *      Author: npadmana
 */

#ifndef PARTICLES_H_
#define PARTICLES_H_

#include <memory>
#include "cpppetsc.h"

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

private :
	// Turn off copy constructors and assignment operators
	Particles(const Particles &p);
	Particles& operator= (const Particles &p);
};

#endif /* PARTICLES_H_ */
