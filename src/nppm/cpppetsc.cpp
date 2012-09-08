#include "cpppetsc.h"

CppPetscVec::CppPetscVec(Index ntot, Index nlocal) {
	safeCall(VecCreateMPI(PETSC_COMM_WORLD, nlocal, ntot, &data),
		"Error allocating vector"); };

CppPetscVec::~CppPetscVec( ){
	if (data != PETSC_NULL) safeCall(VecDestroy(&data),
		"Error destroying vector");};

void CppPetscVec::init(Index ntot, Index nlocal) {
	if (data != PETSC_NULL) safeCall(VecDestroy(&data),
			"Error destroying vector");
	safeCall(VecCreateMPI(PETSC_COMM_WORLD, nlocal, ntot, &data),
		"Error allocating vector"); };

void CppPetscVec::getOwnershipRange(Index &lo, Index &hi) const {
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

CppPetscVec::Index CppPetscVec::size() const {
	Index n;
	VecGetSize(data, &n);
	return n;
}

CppPetscVec& CppPetscVec::operator=(const CppPetscVec& x) {
	PetscPrintf(PETSC_COMM_WORLD, "NOTE : Assignment operator executed!!\n");
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

CppPetscVec::CppPetscVec(const CppPetscVec& x, bool shallowcopy) {
	VecDuplicate(x.data, &data);
	if (!shallowcopy) {
		PetscPrintf(PETSC_COMM_WORLD, "NOTE : DeepCopy constructor executed!!\n");
		VecCopy(x.data, data);
	}
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

void CppPetscVec::swap(CppPetscVec& v) {
	Vec tmp;
	tmp = v.data;
	v.data = data;
	data = tmp;
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

void CppPetscVec::set(const std::vector<Index> &idx, const std::vector<Value> &val,
		InsertMode iora) {

	// Get the number of elements
	Index nelts = idx.size();

	VecSetValues(data, nelts, &idx[0], &val[0], iora);
}

void CppPetscVec::set(const std::vector<Index> &idx, const Value* val,
		InsertMode iora) {

	// Get the number of elements
	Index nelts = idx.size();

	VecSetValues(data, nelts, &idx[0], val, iora);
}


void CppPetscVec::assemblyBegin() {
	VecAssemblyBegin(data);
}

void CppPetscVec::assemblyEnd() {
	VecAssemblyEnd(data);
}


CppPetscMat::~CppPetscMat() {
	if (data != PETSC_NULL) safeCall(MatDestroy(&data),
			"Error destroying matrix");
}

CppPetscMat::CppPetscMat(Index ny, Index nAx) {
	safeCall(
			MatCreateAIJ(PETSC_COMM_WORLD, ny, nAx, PETSC_DETERMINE, PETSC_DETERMINE, 0, 0, PETSC_NULL, PETSC_NULL, &data),
			"Error creating matrix");
}

CppPetscMat::CppPetscMat(Index ny, Index nAx, Index d_nz, Index o_nz) {
	safeCall(
				MatCreateAIJ(PETSC_COMM_WORLD, ny, nAx, PETSC_DETERMINE, PETSC_DETERMINE, d_nz, PETSC_NULL, o_nz, PETSC_NULL, &data),
				"Error creating matrix");
}

CppPetscMat::CppPetscMat(Index ny, Index nAx, const std::vector<Index>& d_nnz,
		const std::vector<Index>& o_nnz) {
	if (d_nnz.size() != ny) safeCall(99, "d_nnz vector has the wrong size");
	if (o_nnz.size() != ny) safeCall(99, "o_nnz vector has the wrong size");
	safeCall(
				MatCreateAIJ(PETSC_COMM_WORLD, ny, nAx, PETSC_DETERMINE, PETSC_DETERMINE, 0, &d_nnz[0], 0, &o_nnz[0], &data),
				"Error creating matrix");
}

void CppPetscMat::size(Index& M, Index& N) {
	MatGetSize(data, &M, &N);
}

void CppPetscMat::getOwnershipRange(Index& lo, Index& hi) {
	MatGetOwnershipRange(data, &lo, &hi);
}

void CppPetscMat::set(Index m, Index n, Index val, InsertMode iora) {
	MatSetValue(data, m, n, val,  iora);
}

void CppPetscMat::set(const std::vector<Index>& m, const std::vector<Index>& n,
		const std::vector<Value>& val, InsertMode iora) {
	MatSetValues(data, m.size(), &m[0], n.size(), &n[0], &val[0], iora);
}

void CppPetscMat::assemblyBegin(MatAssemblyType type) {
	MatAssemblyBegin(data, type);
}

void CppPetscMat::assemblyEnd(MatAssemblyType type) {
	MatAssemblyEnd(data, type);
}

void CppPetscMat::mult(CppPetscVec& x, CppPetscVec& y) {
	MatMult(data, x.data, y.data);
}

void CppPetscMat::multTransposed(CppPetscVec& x, CppPetscVec& y) {
	MatMultTranspose(data, x.data, y.data);
}






