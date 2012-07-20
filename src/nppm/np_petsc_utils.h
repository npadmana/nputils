/*
 * petsc_utils.h
 *
 *  Created on: Jul 20, 2012
 *      Author: npadmana
 */

#ifndef PETSC_UTILS_H_
#define PETSC_UTILS_H_

#include "petscsys.h"
#include <mpi.h>

/** Error handling for PETSc.
 *
 *  Call PETScError and then MPI Abort. Note that this uses the
 *  C++ MPI::COMM_WORLD.Abort to raise a C++ exception
 */
void safeCall(PetscErrorCode n, const char * mess);


#endif /* PETSC_UTILS_H_ */
