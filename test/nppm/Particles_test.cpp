#include "gtest/gtest.h"
#include "Particles.h"
#include "npRandom.h"

using namespace std;

template <int nvec>
void fillRandom(Particles<nvec>& p) {
	int rank;
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
	npRandom rr(99+100*rank);
	typename Particles<nvec>::Index lo, hi;
	p.getOwnershipRange(lo, hi);

	p.get();
	for (int ii=0; ii < nvec; ++ii) {
		for (int jj=0; jj < (hi-lo); ++jj) {
			p[ii][jj] = rr();
		}
	}
	p.restore();
}

template <int nvec>
void fillRandom2(Particles<nvec>& p, int Ngrid) {
	int rank;
	double r1;
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
	npRandom rr(99+100*rank);
	typename Particles<nvec>::Index lo, hi;
	p.getOwnershipRange(lo, hi);

	p.get();
	for (int jj=0; jj < (hi-lo); ++jj) {
		r1 = rr()*Ngrid;
		p[0][jj] = r1;
		for (int ii=1; ii < nvec; ++ii) {
			p[ii][jj] = periodic(r1*(ii+1), Ngrid);
		}
	}
	p.restore();
}



TEST(PeriodicTest, Test1) {
	EXPECT_DOUBLE_EQ(0.5, periodic(0.5, 1.0));
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

// Make sure all the particle arrays
// have the same ownership ranges
TEST(ParticlesTest, TestOwnershipRange) {
	Particles<3> p1(100);
	Particles<3>::Index lo, hi, lo1, hi1;

	p1.getOwnershipRange(lo, hi);
	for (int ii=0; ii < 3; ++ii) {
		p1.getOwnershipRange(lo1, hi1, ii);
		EXPECT_EQ(lo, lo1);
		EXPECT_EQ(hi, hi1);
	}
}

// Test operator[]
TEST(ParticlesTest, TestRandomAccess1) {
	Particles<3> p1(10);
	Particles<3>::Index lo, hi;
	for (int ii=0; ii< 3; ++ii)
		*(p1.ptrs)[ii] = 3.14*(ii+1);

	// Using a straight nested for loop
	p1.getOwnershipRange(lo, hi);
	p1.get();
	for (int ii=0; ii < 3; ++ii)
		for (int jj=0; jj < (hi-lo); ++jj)
			EXPECT_DOUBLE_EQ(3.14*(ii+1), static_cast<double>(p1[ii][jj]));
	p1.restore();
}

TEST(ParticlesTest, TestRandomAccess2) {
	Particles<3> p1(100);
	Particles<3>::Index lo, hi;
	p1.getOwnershipRange(lo, hi);
	p1.get();
	for (int ii=0; ii<3; ++ii)
		for (int jj=0; jj < (hi-lo); ++jj)
			p1[ii][jj] = 3.14*(ii+1);
	p1.restore();


	// Using a straight nested for loop
	p1.get();
	for (int ii=0; ii < 3; ++ii)
		for (int jj=0; jj < (hi-lo); ++jj)
			EXPECT_DOUBLE_EQ(3.14*(ii+1), static_cast<double>(p1[ii][jj]));
	p1.restore();
}

TEST(ParticlesTest, TestSlabDecompose) {
	// First make sure that the number of processors
	// divides our chosen grid size (128)
	int size, rank, proc;
	int Ngrid = 128;
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
	MPI_Comm_size(PETSC_COMM_WORLD, &size);
	ASSERT_EQ(0, Ngrid%size);

	Particles<3> p1(1000);

	// Generate fake data
	fillRandom2(p1, Ngrid);

	for (int idir = 0; idir < 3; ++idir) {
		// Now decompose
		p1.SlabDecompose(Ngrid, idir);

		// Check to see that the particle coordinates have not been shuffled
		npForEach(*p1.ptrs[0], *p1.ptrs[1], *p1.ptrs[2], [](double x, double y, double z){
			EXPECT_DOUBLE_EQ(periodic(x*2, 128), y);
			EXPECT_DOUBLE_EQ(periodic(x*3, 128), z);
		});


		// Check to see that the particles are on the correct slab
		npForEach(*p1.ptrs[idir], [&](double x){
			proc = static_cast<int>(x)/(Ngrid/size);
			EXPECT_EQ(rank, proc);
		});
	}

}




int main(int argc, char **argv) {
	safeCall(PetscInitialize(&argc,&argv,(char *) 0, PETSC_NULL), "Error initializing");
	::testing::InitGoogleTest(&argc, argv);
	int retval = RUN_ALL_TESTS();
	safeCall(PetscFinalize(),"Error finalizing");
	return retval;
}


