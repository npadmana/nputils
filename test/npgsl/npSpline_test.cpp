#include "gtest/gtest.h"
#include "npSpline.h"
#include <vector>
#include <algorithm>

using namespace std;

// Set up a test fixture for Splines
class SplineTest : public ::testing::Test {
protected:
	Spline sp1; // Linear spline
	int N;

	virtual void SetUp() {
		N=20;
		vector<double> x(N), y(N);
		for (int ii=0; ii < x.size(); ++ii) {
			x[ii] = ii;
			y[ii] = 2*ii;
		}
	sp1 = Spline(x, y, gsl_interp_linear);
	}
};

// Test values at break points
TEST_F(SplineTest, TestAtVals) {
	double x;
	for (int ii=1;ii < (N-1); ++ii) {
		x = static_cast<double>(ii);
		EXPECT_DOUBLE_EQ(2.0*x, sp1(x));
	}
}

TEST_F(SplineTest, TestCopyConstructor) {
	Spline sp2(sp1);
	double x;
	for (int ii=1;ii < (N-1); ++ii) {
		x = static_cast<double>(ii);
		EXPECT_DOUBLE_EQ(2.0*x, sp2(x));
	}
}

TEST_F(SplineTest, TestAssignmentOperator) {
	Spline sp2;
	sp2 = sp1;
	double x;
	for (int ii=1;ii < (N-1); ++ii) {
		x = static_cast<double>(ii);
		EXPECT_DOUBLE_EQ(2.0*x, sp2(x));
	}
}

TEST_F(SplineTest, TestVal) {
	for (int ii=0;ii < (N-1); ++ii) {
		EXPECT_NEAR(2.0*(ii+0.3), sp1(ii+0.3), 1.e-7);
	}
}
