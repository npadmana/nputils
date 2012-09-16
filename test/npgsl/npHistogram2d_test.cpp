#include "gtest/gtest.h"
#include "npHistogram2D.h"


TEST(Hist2D, Alloc1) {
	EXPECT_NO_THROW({
		Histogram2D h1;
	});
}

TEST(Hist2D, Alloc2) {
	EXPECT_NO_THROW({
		Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	});
}

TEST(Hist2D, NxNy) {
	Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	std::tuple<int,int> tmp = h1.nbins();
	EXPECT_EQ(5, std::get<0>(tmp));
	EXPECT_EQ(2, std::get<1>(tmp));
}

TEST(Hist2D, XRange) {
	Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	double dx = 1.0/5;
	for (int ii=0; ii < 5; ++ii) {
		std::tuple<double, double> tmp = h1.xrange(ii);
		EXPECT_NEAR(ii*dx,std::get<0>(tmp), 1.e-6);
		EXPECT_NEAR((ii+1)*dx,std::get<1>(tmp), 1.e-6);
	}
}

TEST(Hist2D, YRange) {
	Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	double dy = 1.0/2;
	for (int jj=0; jj < 2; ++jj) {
		std::tuple<double, double> tmp = h1.yrange(jj);
		EXPECT_NEAR(jj*dy,std::get<0>(tmp), 1.e-6);
		EXPECT_NEAR((jj+1)*dy,std::get<1>(tmp), 1.e-6);
	}
}

TEST(Hist2D, Reset1) {
	Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	h1.reset();
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj<2; ++jj)
			EXPECT_DOUBLE_EQ(0.0, h1(ii,jj));
}

TEST(Hist2D, Reset2) {
	Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	h1.add(0.3, 0.3, 1.0);
	h1.add(0.1, 0.7, 1.0);
	h1.reset();
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj<2; ++jj)
			EXPECT_DOUBLE_EQ(0.0, h1(ii,jj));
}



TEST(Hist2D, Fill1) {
	Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	h1.add(1.1, -1.1, 10.0);
	h1.add(0.5, 1.1, 10.0);
	h1.add(-0.1, 0.3, 1.0);
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj<2; ++jj)
			EXPECT_DOUBLE_EQ(0.0, h1(ii,jj));
}

TEST(Hist2D, Fill2) {
	Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj < 2; ++jj)
			h1.add(ii*0.2+0.1, jj*0.5+0.25);
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj<2; ++jj)
			EXPECT_DOUBLE_EQ(1.0, h1(ii,jj));
}

TEST(Hist2D, Fill3) {
	Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj < 2; ++jj)
			h1.add(ii*0.2+0.1, jj*0.5+0.25, 3.14);
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj<2; ++jj)
			EXPECT_DOUBLE_EQ(3.14, h1(ii,jj));
}

TEST(Hist2D, Fill4) {
	Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	std::vector<double> xx, yy;
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj < 2; ++jj) {
			xx.push_back(ii*0.2 + 0.1);
			yy.push_back(jj*0.5 + 0.25);
		}
	h1.add(xx, yy);
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj<2; ++jj)
			EXPECT_DOUBLE_EQ(1.0, h1(ii,jj));
}

TEST(Hist2D, Fill5) {
	Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	std::vector<double> xx, yy;
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj < 2; ++jj) {
			xx.push_back(ii*0.2 + 0.1);
			yy.push_back(jj*0.5 + 0.25);
		}
	h1.add(xx, yy, 3.14);
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj<2; ++jj)
			EXPECT_DOUBLE_EQ(3.14, h1(ii,jj));
}

TEST(Hist2D, Fill6) {
	Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	std::vector<double> xx, yy, ww;
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj < 2; ++jj) {
			xx.push_back(ii*0.2 + 0.1);
			yy.push_back(jj*0.5 + 0.25);
			ww.push_back(2.712);
		}
	h1.add(xx, yy, ww);
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj<2; ++jj)
			EXPECT_DOUBLE_EQ(2.712, h1(ii,jj));
}

TEST(Hist2D, CopyConstructor) {
	Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj < 2; ++jj)
			h1.add(ii*0.2+0.1, jj*0.5+0.25, 3.14);
	Histogram2D h2(h1);
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj<2; ++jj)
			EXPECT_DOUBLE_EQ(3.14, h2(ii,jj));
}

TEST(Hist2D, AssignmentOperator) {
	Histogram2D h2;
	{ // Just so that h1 goes out of scope
		Histogram2D h1(5, 0.0, 1.0, 2, 0.0, 1.0);
		for (int ii=0; ii < 5; ++ii)
			for (int jj=0; jj < 2; ++jj)
				h1.add(ii*0.2+0.1, jj*0.5+0.25, 3.14);
		h2 = h1;
	}
	for (int ii=0; ii < 5; ++ii)
		for (int jj=0; jj<2; ++jj)
			EXPECT_DOUBLE_EQ(3.14, h2(ii,jj));
}



