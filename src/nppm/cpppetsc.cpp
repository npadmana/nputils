#include "cpppetsc.h"

CppPetscVec::CppPetscVec(PetscInt ntot, PetscInt nlocal) {
	safeCall(VecCreateMPI(PETSC_COMM_WORLD, nlocal, ntot, &data),
		"Error allocating vector"); };

CppPetscVec::~CppPetscVec( ){
	if (data != PETSC_NULL) safeCall(VecDestroy(&data),
		"Error destroying vector");};

void CppPetscVec::getOwnershipRange(PetscInt &lo, PetscInt &hi) {
	VecGetOwnershipRange(data, &lo, &hi);
}

PetscScalar * CppPetscVec::get() {
	PetscScalar *_x;
	VecGetArray(data, &_x);
	return _x;
}

void CppPetscVec::restore(PetscScalar *x) {
	VecRestoreArray(data, &x);
}

void CppPetscVec::operator= (PetscScalar x) {
	VecSet(data, x);
}

PetscInt CppPetscVec::size() {
	PetscInt n;
	VecGetSize(data, &n);
	return n;
}
