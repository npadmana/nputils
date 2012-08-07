#include "cpppetsc.h"

CppPetscVec::CppPetscVec(Index ntot, Index nlocal) {
	safeCall(VecCreateMPI(PETSC_COMM_WORLD, nlocal, ntot, &data),
		"Error allocating vector"); };

CppPetscVec::~CppPetscVec( ){
	if (data != PETSC_NULL) safeCall(VecDestroy(&data),
		"Error destroying vector");};

void CppPetscVec::getOwnershipRange(Index &lo, Index &hi) {
	VecGetOwnershipRange(data, &lo, &hi);
}

void CppPetscVec::get() {
	VecGetArray(data, &_data);
}

void CppPetscVec::restore() {
	VecRestoreArray(data, &_data);
}

void CppPetscVec::operator= (CppPetscVec::Value x) {
	VecSet(data, x);
}

CppPetscVec::Index CppPetscVec::size() {
	Index n;
	VecGetSize(data, &n);
	return n;
}

CppPetscVec& CppPetscVec::operator=(const CppPetscVec& x) {
	if (this == &x) {
		return *this;
	}

	if (data != PETSC_NULL) VecDestroy(&data);
	VecDuplicate(x.data, &data);
	VecCopy(x.data, data);

	return *this;
}

bool CppPetscVec::operator ==(const CppPetscVec& vec) {
	PetscBool flg;
	VecEqual(data, vec.data, &flg);
	return (flg==PETSC_TRUE);
}

CppPetscVec::Value CppPetscVec::sum() {
	Value retval;
	VecSum(data, &retval);
	return retval;
}

CppPetscVec::CppPetscVec(const CppPetscVec& x) {
	VecDuplicate(x.data, &data);
	VecCopy(x.data, data);
}

CppPetscVec::Value& CppPetscVec::operator[](Index ii) {
	return _data[ii];
}

const CppPetscVec::Value& CppPetscVec::operator[](Index ii) const {
	return _data[ii];
}


