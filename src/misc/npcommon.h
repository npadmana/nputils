/*
 * npcommon.h
 *
 *  A collection of useful typedefs, utilities etc that I find myself using all the time.
 *
 *  There is a relatively high bar to be included here, it should be generically useful. The other headers here
 *  are more of a grab-bag.
 *
 *  Everything here is in namespace Padmanabhan, which should avoid all conflicts.
 *
 *  Created on: Dec 27, 2012
 *      Author: npadmana
 */

#ifndef NPCOMMON_H_
#define NPCOMMON_H_

#include <vector>


namespace Padmanabhan {

// Constants go here.
const double PI = 3.1415926535897932385;
const double D2R = 0.017453292519943295769; // Degrees to radians


// Useful typedefs go here
typedef std::vector<double> dvector;




}



#endif /* NPCOMMON_H_ */
