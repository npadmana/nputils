/*
 * npmisc.h
 *
 *  A collection of useful typedefs, utilities etc that I find myself using all the time. This is
 *  designed to just be a header file, and isn't particularly specialized.
 *
 *  Everything here is in namespace Padmanabhan, which should avoid all conflicts.
 *
 *  Created on: Dec 27, 2012
 *      Author: npadmana
 */

#ifndef NPMISC_H_
#define NPMISC_H_

#include <vector>
#include <utility>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/range.hpp>
#include "boost/tuple/tuple.hpp"


namespace Padmanabhan {

// Constants go here.
const double PI = 3.1415926535897932385;
const double D2R = 0.017453292519943295769; // Degrees to radians



// Useful typedefs go here
typedef std::vector<double> dvector;
typedef std::pair<double, double> dpair;

/** Generate a zip iterator that can be used with the new C++ for range loops.
 *
 *
 *  http://stackoverflow.com/questions/8511035/sequence-zip-function-for-c11
 */
template <typename... T>
auto zip(const T&... containers) -> boost::iterator_range<boost::zip_iterator<decltype(boost::make_tuple(std::begin(containers)...))>>
{
    auto zip_begin = boost::make_zip_iterator(boost::make_tuple(std::begin(containers)...));
    auto zip_end = boost::make_zip_iterator(boost::make_tuple(std::end(containers)...));
    return boost::make_iterator_range(zip_begin, zip_end);
}

/** Statistics
 *
 * A convenience function to calculate the mean and standard deviation of a
 * container that can be iterated over
 *
 *  @param x (T) : container that can be iterated over
 *
 *  returns a dpair with the mean and standard deviation
 */
template <class T>
dpair stats(T x) {
	double mean=0.0, stddev=0.0;
	double xsize = x.size();
	for (auto v1 : x) mean+=v1; mean /= xsize;
	for (auto v1 : x) stddev += (v1-mean)*(v1-mean); stddev = sqrt(stddev/xsize);
	return make_pair(mean, stddev);
}




}



#endif /* NPMISC_H_ */
