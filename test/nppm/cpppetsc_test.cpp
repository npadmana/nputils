#include "gtest/gtest.h"
#include "cpppetsc.h"
#include "nppm_algorithms.h"

TEST(CppPetsc, AllocNull) {
	EXPECT_NO_THROW({
		CppPetscVec v1;
	});
}

TEST(CppPetsc, AllocN) {
	EXPECT_NO_THROW({
		CppPetscVec v1(100);
	});
}

TEST(CppPetsc, Set) {
	CppPetscVec v1(10);
	PetscInt lo, hi;

	v1 = 3.14;
	v1.getOwnershipRange(lo, hi);
	v1.get();
	for (PetscInt ii=lo; ii != hi; ++ii) EXPECT_DOUBLE_EQ(3.14, v1[ii-lo]);
	v1.restore();
}

TEST(CppPetsc, CopyConstructor) {
	CppPetscVec v1(10);
	v1 = 3.14;
	CppPetscVec v2(v1);

	PetscBool flg;
	VecEqual(v1.data, v2.data, &flg);
	EXPECT_EQ(PETSC_TRUE, flg);
}

TEST(CppPetsc, CopyConstructor2) {
	CppPetscVec v1(10);
	v1 = 3.14;
	CppPetscVec v2 = v1;

	// We don't use the operator version here
	PetscBool flg;
	VecEqual(v1.data, v2.data, &flg);
	EXPECT_EQ(PETSC_TRUE, flg);
}


TEST(CppPetsc, CopyConstructor3) {
	CppPetscVec v1(10);
	CppPetscVec v2(v1, true);

	EXPECT_TRUE(v1.is_compatible(v2));
}


TEST(CppPetsc, Assignment) {
	CppPetscVec v1(10);
	v1 = 3.14;
	CppPetscVec v2;
	v2 = v1;

	PetscBool flg;
	VecEqual(v1.data, v2.data, &flg);
	EXPECT_EQ(PETSC_TRUE, flg);
}

TEST(CppPetsc, SelfAssignment) {
	EXPECT_NO_THROW({
		CppPetscVec v1(10);
		v1 = v1;
	});
}

TEST(CppPetsc, Equal) {
	CppPetscVec v1(10);
	v1 = 3.14;
	CppPetscVec v2 = v1;
	EXPECT_TRUE(v1==v2);
	EXPECT_TRUE(v2==v1);
}

TEST(CppPetsc, Equal2) {
	CppPetscVec v1(10);
	v1 = 3.14;
	CppPetscVec v2(10);
	v2 = 3.15;
	EXPECT_FALSE(v1==v2);
	EXPECT_FALSE(v2==v1);
}

TEST(CppPetsc, Equal3) {
	CppPetscVec v1(10), v2(11);
	v1 = 3.14;
	v2 = 3.14;
	EXPECT_FALSE(v1==v2);
	EXPECT_FALSE(v2==v1);
}

TEST(CppPetsc, Sum) {
	CppPetscVec v1(10);
	v1 = 3.14;
	EXPECT_DOUBLE_EQ(31.4, v1.sum());
}

TEST(CppPetsc, Shift) {
	CppPetscVec v1(10), v2(10);
	v1 = 1.0;
	v2 = 0.0;
	v2 += 1.0;
	EXPECT_TRUE(v1==v2);
}

TEST(CppPetsc, Scale) {
	CppPetscVec v1(10), v2(10);
	v1 = 3.14;
	v2 = 1.0;
	v2 *= 3.14;
	EXPECT_TRUE(v1==v2);
}

TEST(CppPetsc, AXPY1) {
	CppPetscVec v1(10), v2(10);
	v1 = 1.0;
	v2 = 1.0;
	v2.AXPY(v1, 3.14);
	npForEach(v2, [](CppPetscVec::Value x) {EXPECT_DOUBLE_EQ(4.14,x);});
}

TEST(CppPetsc, AXPY2) {
	CppPetscVec v1(10);
	v1 = 1.0;
	v1.AXPY(v1, 3.14);
	npForEach(v1, [](CppPetscVec::Value x) {EXPECT_DOUBLE_EQ(4.14,x);});
}

TEST(CppPetsc, AYPX1) {
	CppPetscVec v1(10), v2(10);
	v1 = 2.0;
	v2 = 1.0;
	v2.AYPX(v1, 3.14);
	npForEach(v2, [](CppPetscVec::Value x) {EXPECT_DOUBLE_EQ(5.14,x);});
}

TEST(CppPetsc, AYPX2) {
	CppPetscVec v1(10);
	v1 = 1.0;
	v1.AYPX(v1, 4.14);
	npForEach(v1, [](CppPetscVec::Value x) {EXPECT_DOUBLE_EQ(5.14,x);});
}

TEST(CppPetsc, AXPBY1) {
	CppPetscVec v1(10), v2(10);
	v1 = 1.0;
	v2 = 2.0;
	v2.AXPBY(v1, 3.14, 2.0);
	npForEach(v2, [](CppPetscVec::Value x) {EXPECT_DOUBLE_EQ(7.14,x);});
}

TEST(CppPetsc, AXPBY2) {
	CppPetscVec v1(10);
	v1 = 1.0;
	v1.AXPBY(v1, 3.14, 2.0);
	npForEach(v1, [](CppPetscVec::Value x) {EXPECT_DOUBLE_EQ(5.14,x);});
}


TEST(CppPetsc, Swap1) {
	CppPetscVec v1(10), v2(20);
	EXPECT_EQ(10, v1.size());
	EXPECT_EQ(20, v2.size());
	v1.swap(v2);
	EXPECT_EQ(10, v2.size());
	EXPECT_EQ(20, v1.size());
}

TEST(CppPetsc, Swap2) {
	CppPetscVec v1(10), v2(20);
	v1 = 3.14; v2=2.73;
	v1.swap(v2);
	npForEach(v1, [](CppPetscVec::Value x) {EXPECT_DOUBLE_EQ(2.73,x);});
	npForEach(v2, [](CppPetscVec::Value x) {EXPECT_DOUBLE_EQ(3.14,x);});
}

TEST(CppPetsc, SetValues1) {
	const int N=10;
	int rank;
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);

	CppPetscVec vec(N);
	std::vector<CppPetscVec::Index> idx(N);
	std::vector<CppPetscVec::Value> val(N);

	// Set up
	if (rank==0) {
		for (int ii=0; ii<N; ++ii) {
			idx[ii] = ii;
			val[ii] = 3.14*ii + 2.73;
		}
		vec.set(idx, val, INSERT_VALUES);
	}
	vec.assemblyBegin(); vec.assemblyEnd();

	// Test
	CppPetscVec::Index lo, hi, ii;
	vec.getOwnershipRange(lo, hi); ii=lo;
	npForEach(vec, [&ii, &lo](double x){
		EXPECT_NEAR(3.14*ii+2.73, x, 1.e-7);
		ii++;
	});
}

TEST(CppPetsc, SetValues2) {
	const int N=10;
	int rank;
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);

	CppPetscVec vec(N);
	std::vector<CppPetscVec::Index> idx(N);
	std::vector<CppPetscVec::Value> val(N);

	// Set up
	vec = 2.73;
	if (rank==0) {
		for (int ii=0; ii<N; ++ii) {
			idx[ii] = ii;
			val[ii] = 3.14*ii;
		}
		vec.set(idx, val, ADD_VALUES);
	}
	vec.assemblyBegin(); vec.assemblyEnd();

	// Test
	CppPetscVec::Index lo, hi, ii;
	vec.getOwnershipRange(lo, hi); ii=lo;
	npForEach(vec, [&ii, &lo](double x){
		EXPECT_NEAR(3.14*ii+2.73, x, 1.e-7);
		ii++;
	});
}

TEST(CppPetsc, SetValues3) {
	const int N=10;
	int rank;
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);

	CppPetscVec vec(N);
	std::vector<CppPetscVec::Index> idx(N);
	std::vector<CppPetscVec::Value> val(N);

	// Set up
	if (rank==0) {
		for (int ii=0; ii<N; ++ii) {
			idx[ii] = ii;
			val[ii] = 3.14*ii;
		}
		vec.set(idx, &val[0], INSERT_VALUES);
	}
	vec.assemblyBegin(); vec.assemblyEnd();

	// Test
	CppPetscVec::Index lo, hi, ii;
	vec.getOwnershipRange(lo, hi); ii=lo;
	npForEach(vec, [&ii, &lo](double x){
		EXPECT_NEAR(3.14*ii, x, 1.e-7);
		ii++;
	});
}



TEST(npForEach, Test1) {
	CppPetscVec v1(10), v2(10);
	v1 = 3.14;
	npForEach(v2, [](PetscScalar& x){x=3.14;});

	PetscBool flg;
	VecEqual(v1.data, v2.data, &flg);
	EXPECT_EQ(PETSC_TRUE, flg);
}

TEST(npForEach, Test2) {
	CppPetscVec v1(10), v2(10);
	v1 = 3.14;
	v2 = 2.0;
	npForEach(v1, v2, [](CppPetscVec::Value &x, CppPetscVec::Value &y){x = x+y;});
	npForEach(v1, [](CppPetscVec::Value x) {EXPECT_DOUBLE_EQ(5.14, x);});
}

TEST(npForEach, Test3) {
	CppPetscVec v1(10), v2(10), v3(10);
	v1 = 3.14;
	v2 = 2.0;
	v3 = 2.0;
	npForEach(v1, v2, v3, [](CppPetscVec::Value &x, CppPetscVec::Value &y, CppPetscVec::Value &z){z = x+y-2*z;});
	npForEach(v3, [](CppPetscVec::Value x) {EXPECT_DOUBLE_EQ(1.14, x);});
}

TEST(CppPetscMat, AllocNull) {
	EXPECT_NO_THROW({
		CppPetscMat m1;
	});
}

TEST(CppPetscMat, Alloc1) {
	EXPECT_NO_THROW({
		CppPetscMat m1(10,5);
	});
}

TEST(CppPetscMat, Alloc2) {
	EXPECT_NO_THROW({
		CppPetscMat m1(10,5,1,1);
	});
}


TEST(CppPetscMat, Alloc3) {
	std::vector<CppPetscMat::Index> d_nnz(10), o_nnz(10);
	for (auto &i : d_nnz) i=1;
	for (auto &i : o_nnz) i=1;
	EXPECT_NO_THROW({
		CppPetscMat m1(10,5, d_nnz, o_nnz);
	});
}

TEST(CppPetscMat, Size1) {
	int size;
	MPI_Comm_size(PETSC_COMM_WORLD, &size);
	CppPetscMat::Index M, N;
	const int ny = 5; const int nAx = 8;
	CppPetscMat m1(ny, nAx);
	m1.size(M, N);
	EXPECT_EQ(ny*size, M);
	EXPECT_EQ(nAx*size, N);
}


TEST(CppPetscMat, Size2) {
	int size;
	MPI_Comm_size(PETSC_COMM_WORLD, &size);
	CppPetscMat::Index M, N;
	const int ny = 5; const int nAx = 8;
	CppPetscMat m1(ny, nAx, 1, 2);
	m1.size(M, N);
	EXPECT_EQ(ny*size, M);
	EXPECT_EQ(nAx*size, N);
}

TEST(CppPetscMat, OwnershipRange1) {
	int rank;
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
	CppPetscMat::Index lo, hi;
	const int ny = 5; const int nAx = 8;
	CppPetscMat m1(ny, nAx);
	m1.getOwnershipRange(lo, hi);
	EXPECT_EQ(ny*rank, lo);
	EXPECT_EQ(lo+ny, hi);
}





int main(int argc, char **argv) {
	safeCall(PetscInitialize(&argc,&argv,(char *) 0, PETSC_NULL), "Error initializing");
	::testing::InitGoogleTest(&argc, argv);
	int retval = RUN_ALL_TESTS();
	safeCall(PetscFinalize(),"Error finalizing");
	return retval;
}
