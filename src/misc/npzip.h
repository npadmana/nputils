/*
 * npzip.h
 *
 *  Created on: Dec 28, 2012
 *      Author: npadmana
 */

#ifndef NPZIP_H_
#define NPZIP_H_

#include <boost/iterator/zip_iterator.hpp>
#include <boost/range.hpp>
#include <boost/tuple/tuple.hpp>

namespace Padmanabhan {

/** Generate a zip iterator that can be used with the new C++ for range loops.
 *
 *  Note that this uses boost tuples, not C++ 11 tuples, to be compatible with make_zip_iterator
 *  http://stackoverflow.com/questions/8511035/sequence-zip-function-for-c11
 */
template <typename... T>
auto zip(const T&... containers) -> boost::iterator_range<boost::zip_iterator<decltype(boost::make_tuple(std::begin(containers)...))>>
{
    auto zip_begin = boost::make_zip_iterator(boost::make_tuple(std::begin(containers)...));
    auto zip_end = boost::make_zip_iterator(boost::make_tuple(std::end(containers)...));
    return boost::make_iterator_range(zip_begin, zip_end);
}


}

#endif /* NPZIP_H_ */
