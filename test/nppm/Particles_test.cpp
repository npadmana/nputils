#include "gtest/gtest.h"
#include "Particles.h"
#include "npRandom.h"

using namespace std;

struct ptest {
	float pos[3];
	int id;
};

typedef Particles<ptest> TestParticles;

void fillRandom2(TestParticles& p, int Ngrid) {
	int rank;
	float r1;
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
	npRandom rr(99+100*rank);
	typename TestParticles::Index lo, hi;

	npForEach(p, [&](ptest& ip) {
		r1 = rr()*Ngrid;
		for (int ii=0; ii<3; ++ii)
			ip.pos[ii] = periodic(r1*(ii+1), Ngrid);
		ip.id = 1;
	});
}



TEST(PeriodicTest, Test1) {
	EXPECT_DOUBLE_EQ(0.5, periodic(0.5, 1.0));
	EXPECT_DOUBLE_EQ(1.0, periodic(3.0, 2.0));
	EXPECT_DOUBLE_EQ(1.9, periodic(-0.1, 2.0));
}

TEST(PartitionTest, Test1) {
	// Some random numbers
	int narr[5] = {10, 100, 1000, 1237, 11318};
	int sizarr[5] = {1, 2, 5, 13, 21};

	for (int s1 : sizarr) {
		for (int n1 : narr) {
			int check=0;
			for (int ii=0; ii< s1; ++ii)
				check += np_partition(n1, s1, ii);
			EXPECT_EQ(n1, check);
		}
	}


}



TEST(ParticlesTest, TestAlloc0) {
	EXPECT_NO_THROW({
		TestParticles p1;
	});
}

TEST(ParticlesTest, TestAlloc1) {
	EXPECT_NO_THROW({
		TestParticles p1(100);
	});
}

TEST(ParticlesTest, TestAlloc2) {
	EXPECT_NO_THROW({
		TestParticles p1;
		p1.init(100);
	});
}

TEST(ParticlesTest, TestAlloc3) {
	TestParticles p1;
	p1.init(100);
	EXPECT_EQ(100, p1.npart);

	TestParticles p2(100);
	EXPECT_EQ(100, p2.npart);
}

TEST(ParticlesTest, TestOwnershipRange) {
	TestParticles p1(100);

	int rank, size;
	MPI_Comm_size(PETSC_COMM_WORLD, &size);
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);

	TestParticles::Index lo, hi;
	p1.getOwnershipRange(lo, hi);
	if (rank==(size-1))
		EXPECT_EQ(100, hi);
	if (rank==0)
		EXPECT_EQ(0, lo);
}

// Test operator[]
TEST(ParticlesTest, TestRandomAccess1) {
	TestParticles p1(10);

	// Fill
	npForEach(p1, [](ptest& ip){
		ip.pos[0] = 3.14;
		ip.pos[1] = 2.73;
		ip.pos[2] = 1.414;
		ip.id = 10;
	});

	// Now test reading this back
	npForEach(p1, [](ptest ip){
		EXPECT_FLOAT_EQ(3.14, ip.pos[0]);
		EXPECT_FLOAT_EQ(2.73, ip.pos[1]);
		EXPECT_FLOAT_EQ(1.414, ip.pos[2]);
		EXPECT_EQ(10, ip.id);
	});
}


// Simple slab decomposition
class domainfunc1 {
public:
	domainfunc1(int _idir, int _size, int _Ng) :
		size(_size), idir(_idir), Ngrid(_Ng) {}
	int operator() (ptest& p) {
		return static_cast<int>(p.pos[idir])/(Ngrid/size) ;
	}
private :
	int size, idir, Ngrid;
};

// Move everything to a single processor
class domainfunc2 {
public:
	domainfunc2(int i) : procnum(i) {};

	int operator() (ptest& p) {
		return procnum;
	}

private :
	int procnum;
};


TEST(ParticlesTest, TestDomainDecompose1) {
	int size, rank, proc;
	int npart1=1000;
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
	MPI_Comm_size(PETSC_COMM_WORLD, &size);
	int Ngrid = size*64;

	TestParticles p1(npart1);

	// Generate fake data
	fillRandom2(p1, Ngrid);

	// Compute the sum
	float tmpsum=0.0;
	float sum=0.0;
	npForEach(p1, [&](ptest p){
		tmpsum += p.pos[0];
	});
	MPI_Allreduce(&tmpsum, &sum, 1, MPI_FLOAT, MPI_SUM, PETSC_COMM_WORLD);


	for (int idir = 0; idir < 3; ++idir) {
		domainfunc1 dfunc(idir, size, Ngrid);

		// Now decompose
		p1.domainDecompose(dfunc);

		// Check to see that the number of particles is the same
		EXPECT_EQ(npart1, p1.npart);

		// Compute the sum
		tmpsum = 0.0;
		float sum2=0.0;
		npForEach(p1, [&](ptest p){
			tmpsum += p.pos[0];
		});
		MPI_Allreduce(&tmpsum, &sum2, 1, MPI_FLOAT, MPI_SUM, PETSC_COMM_WORLD);
		EXPECT_NEAR(sum, sum2, sum*1.e-6);


		// Check to see that the particle coordinates have not been shuffled
		npForEach(p1, [&](ptest& ipart){
			float r1 = ipart.pos[0];
			EXPECT_FLOAT_EQ(periodic(r1*2, Ngrid), ipart.pos[1]);
			EXPECT_FLOAT_EQ(periodic(r1*3, Ngrid), ipart.pos[2]);
		});


		// Check to see that the particles are on the correct slab
		npForEach(p1, [&](ptest p){
			EXPECT_EQ(rank, dfunc(p));
		});

	}

}

TEST(ParticlesTest, TestSet1) {
	const int N=10;
	TestParticles p(N);
	std::vector<TestParticles::Index> idx(N);
	std::vector<ptest> val(N);

	int rank;
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
	if (rank==0) {
		for (int ii=0; ii<N; ++ii) {
			idx[ii] = ii;
			(val[ii]).id = 2*ii + 4;
		}
		p.set(idx, val);
	}
	p.assemblyBegin(); p.assemblyEnd();

	TestParticles::Index lo, hi;
	p.getOwnershipRange(lo, hi); int ii = lo;
	npForEach(p, [&ii](ptest p1) {
		EXPECT_EQ(2*ii+4, p1.id);
		ii++;
	});

}



TEST(ParticlesTest, TestDomainDecompose2) {
	// First make sure that the number of processors
	// divides our chosen grid size (128)
	int size, rank, proc;
	int npart1 = 1000;
	int Ngrid = 128;
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
	MPI_Comm_size(PETSC_COMM_WORLD, &size);

	TestParticles p1(npart1);

	// Generate fake data
	fillRandom2(p1, Ngrid);

	// Compute the sum
	float tmpsum=0.0;
	float sum=0.0;
	npForEach(p1, [&](ptest p){
		tmpsum += p.pos[0];
	});
	MPI_Allreduce(&tmpsum, &sum, 1, MPI_FLOAT, MPI_SUM, PETSC_COMM_WORLD);

	for (int idir = 0; idir < size; ++idir) {
		domainfunc2 dfunc(idir);

		// Now decompose
		p1.domainDecompose(dfunc);

		// Check to see that the number of particles is the same
		EXPECT_EQ(npart1, p1.npart);

		// Compute the sum
		tmpsum = 0.0;
		float sum2=0.0;
		npForEach(p1, [&](ptest p){
			tmpsum += p.pos[0];
		});
		MPI_Allreduce(&tmpsum, &sum2, 1, MPI_FLOAT, MPI_SUM, PETSC_COMM_WORLD);
		EXPECT_NEAR(sum, sum2, 1.e-6*sum);



		// Check to see that the particle coordinates have not been shuffled
		npForEach(p1, [&](ptest& ipart){
			float r1 = ipart.pos[0];
			EXPECT_FLOAT_EQ(periodic(r1*2, Ngrid), ipart.pos[1]);
			EXPECT_FLOAT_EQ(periodic(r1*3, Ngrid), ipart.pos[2]);
		});


		// Check to see that the particles are on the correct slab
		npForEach(p1, [&](ptest p){
			EXPECT_EQ(rank, dfunc(p));
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


