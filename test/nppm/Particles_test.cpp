#include "gtest/gtest.h"
#include "Particles.h"

TEST(PeriodicTest, Test1) {
	EXPECT_DOUBLE_EQ(0.5, periodic(0.5, 1.0));
	EXPECT_DOUBLE_EQ(0.5, periodic(0.5));
	EXPECT_DOUBLE_EQ(1.0, periodic(3.0, 2.0));
	EXPECT_DOUBLE_EQ(1.9, periodic(-0.1, 2.0));
}

TEST(ParticlesTest, TestAlloc1) {
	EXPECT_NO_THROW({
		Particles<3> p1(100);
	});
}

TEST(ParticlesTest, TestAlloc2) {
	EXPECT_NO_THROW({
		Particles<3> p1;
		p1.init(100);
	});
}

TEST(ParticlesTest, TestAlloc3) {
	Particles<3> p1(100);
	Particles<3> p2; p2.init(100);

	for (int ii=0; ii < 3; ++ii) {
		EXPECT_EQ(100, p1.ptrs[ii]->size());
		EXPECT_EQ(100, p2.ptrs[ii]->size());
	}
}

TEST(ParticlesTest, TestRandom) {
	EXPECT_NO_THROW({
		Particles<3> p1(100);
		p1.fillRandom();
	});
}


int main(int argc, char **argv) {
	safeCall(PetscInitialize(&argc,&argv,(char *) 0, PETSC_NULL), "Error initializing");
	::testing::InitGoogleTest(&argc, argv);
	int retval = RUN_ALL_TESTS();
	safeCall(PetscFinalize(),"Error finalizing");
	return retval;
}


