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


int main(int argc, char **argv) {
	safeCall(PetscInitialize(&argc,&argv,(char *) 0, PETSC_NULL), "Error initializing");
	::testing::InitGoogleTest(&argc, argv);
	int retval = RUN_ALL_TESTS();
	safeCall(PetscFinalize(),"Error finalizing");
	return retval;
}
