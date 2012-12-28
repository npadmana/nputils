/*
 * npvecmath.h
 *
 *  Vector math utility functions
 *
 *  Created on: Dec 28, 2012
 *      Author: npadmana
 */

#ifndef NPVECMATH_H_
#define NPVECMATH_H_

#include <vector>
#include <cmath>

namespace Padmanabhan {

/** Compute the mean of a vector
 *
 *   @param x : vector<T>
 *
 *   returns a double; note that this is always the case, to generally prevent loss of
 *   accuracy.
 */
template <class T>
double vecmean(const std::vector<T> &x) {
	double mean {0.0};
	for (auto x1 : x) mean += x1;
	mean /= x.size();
	return mean;
}

/** Compute the stddev of a vector
 *
 *   @param x : vector<T>
 *
 *   returns a double; note that this is always the case, to generally prevent loss of
 *   accuracy.
 */
template <class T>
double vecstddev(const std::vector<T> &x) {
	double mean = vecmean(x);
	double stddev = 0.0;
	for (auto x1 : x) stddev += (x1-mean)*(x1-mean);
	stddev = sqrt(stddev/x.size());
	return stddev;
}

/** Compute the stddev of a vector
 *
 *   @param x : vector<T>
 *   @param mean : double -- input the mean, if you know it.
 *
 *   returns a double; note that this is always the case, to generally prevent loss of
 *   accuracy.
 */
template <class T>
double vecstddev(const std::vector<T> &x, double mean) {
	double stddev = 0.0;
	for (auto x1 : x) stddev += (x1-mean)*(x1-mean);
	stddev = sqrt(stddev/x.size());
	return stddev;
}






} // End namespace

#endif /* NPVECMATH_H_ */
