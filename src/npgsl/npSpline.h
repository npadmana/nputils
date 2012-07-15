/*
 * npSpline.h
 *
 *  Created on: Jul 15, 2012
 *      Author: npadmana
 */

#ifndef NPSPLINE_H_
#define NPSPLINE_H_

#include <vector>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_interp.h>

/** \brief A basic wrapper around the GSL spline functions.
 *
 */
class Spline {
private :
	gsl_spline * sp;
	gsl_interp_accel * acc;
public :
	/** \brief Constructor
	 *
	 *  @param x (vector<double>) -- the x parameter
	 *  @param y (vector<double>) -- the y parameter
	 *  @param sptype -- gsl spline type (defaults to gsl_interp_cspline)
	 *        The options here are :
	 *           gsl_interp_linear
	 *           gsl_interp_polynomial
	 *           gsl_interp_cspline [Default]
	 *           gsl_interp_cspline_periodic
	 *           gsl_interp_akima
	 *           gsl_interp_akima_periodic
	 */
	Spline(std::vector<double> x, std::vector<double> y, const gsl_interp_type *sptype = gsl_interp_cspline);
	~Spline();

	/** \brief Evaluate the spline at x
	 *
	 *  @param x (double) -- location to evaluate spline at
	 */
	double operator()(double x);
};




#endif /* NPSPLINE_H_ */
