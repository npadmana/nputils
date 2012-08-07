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

void CppPetscVec::AXPY(const CppPetscVec& x, const CppPetscVec::Value& alpha) {
	// Check to see if x is the same as this
	if (this == &x) {
		VecScale(data, alpha+1);
	} else {
		VecAXPY(data, alpha, x.data);
	}
}

void CppPetscVec::AYPX(const CppPetscVec& x, const Value& alpha) {
	// Check to see if x is the same as this
	if (this == &x) {
		VecScale(data, alpha+1);
	} else {
		VecAYPX(data, alpha, x.data);
	}
}

void CppPetscVec::AXPBY(const CppPetscVec& x, const CppPetscVec::Value& alpha, const CppPetscVec::Value& beta) {
	// Check to see if x is the same as this
		if (this == &x) {
			VecScale(data, alpha+beta);
		} else {
			VecAXPBY(data, alpha, beta, x.data);
		}
}

const CppPetscVec::Value& CppPetscVec::operator[](Index ii) const {
	return _data[ii];
}

CppPetscVec& CppPetscVec::operator+= (const CppPetscVec::Value& x) {
	VecShift(data, x);
	return *this;
}

CppPetscVec& CppPetscVec::operator*= (const CppPetscVec::Value& x) {
	VecScale(data, x);
	return *this;
}

