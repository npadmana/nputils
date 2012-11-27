#include "gtest/gtest.h"
#include "npRandom.h"

TEST(Random, Dir3d) {
	int niter = 100;
	npRandom ran1(100);
	for (int ii=0; ii < niter; ++ii) {
		Eigen::Vector3d vec = ran1.dir3d();
		EXPECT_DOUBLE_EQ(1.0, vec.norm());
	}
}


TEST(Random, FillVector) {
	npRandom ran1(100);
	std::vector<double> out = ran1(10);
	EXPECT_EQ(10, out.size());
}
