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



TEST(npForEach, Test1) {
	CppPetscVec v1(10), v2(10);
	v1 = 3.14;
	npForEach(v2, [](PetscScalar& x){x=3.14;});

	PetscBool flg;
	VecEqual(v1.data, v2.data, &flg);
	EXPECT_EQ(PETSC_TRUE, flg);
}



int main(int argc, char **argv) {
	safeCall(PetscInitialize(&argc,&argv,(char *) 0, PETSC_NULL), "Error initializing");
	::testing::InitGoogleTest(&argc, argv);
	int retval = RUN_ALL_TESTS();
	safeCall(PetscFinalize(),"Error finalizing");
	return retval;
}
