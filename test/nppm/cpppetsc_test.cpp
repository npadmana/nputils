#include "gtest/gtest.h"
#include "cpppetsc.h"

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
	PetscScalar *x = v1.get();
	for (PetscInt ii=lo; ii != hi; ++ii) EXPECT_DOUBLE_EQ(3.14, x[ii-lo]);
	v1.restore(x);
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




int main(int argc, char **argv) {
	safeCall(PetscInitialize(&argc,&argv,(char *) 0, PETSC_NULL), "Error initializing");
	::testing::InitGoogleTest(&argc, argv);
	int retval = RUN_ALL_TESTS();
	safeCall(PetscFinalize(),"Error finalizing");
	return retval;
}
