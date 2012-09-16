/*
 * Histogram2D.cpp
 *
 *  Created on: Sep 14, 2012
 *      Author: npadmana
 */

#include "npHistogram2D.h"


Histogram2D::Histogram2D(int nx, double xmin, double xmax, int ny, double ymin,
		double ymax) {
	hist = gsl_histogram2d_alloc(nx, ny);
	gsl_histogram2d_set_ranges_uniform(hist, xmin, xmax, ymin, ymax);

}

Histogram2D::~Histogram2D() {
	if (hist != NULL) gsl_histogram2d_free(hist);
}

std::tuple<int, int> Histogram2D::nbins() {
	return std::make_tuple(gsl_histogram2d_nx(hist), gsl_histogram2d_ny(hist));
}

void Histogram2D::reset() {
	gsl_histogram2d_reset(hist);
}

std::tuple<double, double> Histogram2D::xrange(int i) {
	double xmin, xmax;
	gsl_histogram2d_get_xrange(hist, i, &xmin, &xmax);
	return std::make_tuple(xmin, xmax);
}

std::tuple<double, double> Histogram2D::yrange(int j) {
	double ymin, ymax;
	gsl_histogram2d_get_yrange(hist, j, &ymin, &ymax);
	return std::make_tuple(ymin, ymax);
}

double Histogram2D::operator()(int i, int j) {
	return gsl_histogram2d_get(hist, i, j);
}

void Histogram2D::add(double x, double y, double weight) {
	gsl_histogram2d_accumulate(hist, x, y, weight);
}

void Histogram2D::add(const std::vector<double> &x, const std::vector<double> &y, double w) {
	for (int ii=0; ii<x.size(); ++ii)
		add(x[ii], y[ii], w);
}

Histogram2D::Histogram2D(const Histogram2D& h1) {
	hist =  gsl_histogram2d_clone(h1.hist);
}

Histogram2D& Histogram2D::operator =(const Histogram2D& h1) {
	if (this == &h1) {
		return *this;
	}

	if (hist!=NULL) gsl_histogram2d_free(hist);
	hist =  gsl_histogram2d_clone(h1.hist);

	return *this;
}

void Histogram2D::add(const std::vector<double> &x, const std::vector<double> &y,
		const std::vector<double> &w) {
	for (int ii=0; ii<x.size(); ++ii)
		add(x[ii], y[ii], w[ii]);
}





