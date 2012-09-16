/*
 * Histogram2D.h
 *
 *  Created on: Sep 14, 2012
 *      Author: npadmana
 */

#ifndef HISTOGRAM2D_H_
#define HISTOGRAM2D_H_

#include <tuple>
#include <vector>
#include "gsl/gsl_histogram2d.h"

/** Wrapper around the GSL histogram 2D class
 *
 */
class Histogram2D {
private :
	gsl_histogram2d *hist;
public:
	/** Basic constructor class
	 *
	 */
	Histogram2D() : hist(NULL) {};

	/** Basic constructor for uniform bins
	 *
	 * @param nx : Number of xbins
	 * @param xmin : minimum value in x
	 * @param xmax : maximum value in x
	 * @param ny : Number of ybins
	 * @param ymin : minimum value in y
	 * @param ymax : maximum value in y
	 *
	 */
	Histogram2D(int nx, double xmin, double xmax, int ny, double ymin, double ymax);

	/** Copy constructor
	 *
	 * @param hist (const Histogram2D&)
	 */
	Histogram2D(const Histogram2D &h1);

	/** Assignment operator
	 *
	 * @param hist (const Histogram2D&)
	 */
	Histogram2D& operator= (const Histogram2D &h1);


	/** Destructor
	 */
	~Histogram2D();

	/** Get the number of bins
	 *
	 * returns a std::tuple<int, int> with (nx, ny)
	 */
	std::tuple<int, int> nbins();


	/** Reset the bins
	 *
	 */
	void reset();

	/** Get range of the i'th bin in the x direction
	 *
	 *  Note the lower limit is inclusive, and the upper limit is exclusive.
	 *
	 *  @param i (int) : i'th bin
	 *
	 *  returns std::tuple<double, double>
	 */
	std::tuple<double, double> xrange(int i);

	/** Get range of the j'th bin in the y direction
	 *
	 * Note the lower limit is inclusive, while the upper limit is exclusive
	 *
	 * @param j (int) : j'th bin
	 *
	 * returns std::tuple<double, double>
	 */
	std::tuple<double, double> yrange(int j);

	/** Get the value in the i,j bin
	 *
	 * @param i (int) : i bin
	 * @param j (int) : j bin
	 *
	 * returns (double) bin contents
	 */
	double operator()(int i, int j);


	/** Increment x,y with weight.
	 *
	 * If x,y not in histogram range, this is silently dropped.
	 * The weight is optional, and defaults to 1
	 *
	 * @param x (double) : xval
	 * @param y (double) : yval
	 * @param w (double) : weight [defaults to 1.0]
	 */
	void add(double x, double y, double weight=1.0);

	/** Increment x,y with weight.
	 *
	 * If x,y not in histogram range, this is silently dropped.
	 * The weight is optional, and defaults to 1
	 * This version assumes the same weight for everything.
	 *
	 * @param x (vector<double>) : xval
	 * @param y (vector<double>) : yval
	 * @param w (vector<double>) : weight [defaults to 1.0]
	 */
	void add(const std::vector<double> &x, const std::vector<double> &y, double w=1.0);

	/** Increment x,y with weight.
	 *
	 * If x,y not in histogram range, this is silently dropped.
	 * The weight is optional, and defaults to 1
	 * This version assumes the same weight for everything.
	 *
	 * @param x (vector<double>) : xval
	 * @param y (vector<double>) : yval
	 * @param w (vector<double>) : weight [defaults to 1.0]
	 */
	void add(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &w);


};

#endif /* HISTOGRAM2D_H_ */
