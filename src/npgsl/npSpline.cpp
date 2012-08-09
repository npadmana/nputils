#include "npSpline.h"

using namespace std;

Spline::Spline(const vector<double> x, const vector<double> y, const gsl_interp_type *sptype) {

	// Cache vectors
	_x = x; _y = y;
	interptype = sptype;
	init();
};

void Spline::init() {
	int n = _x.size();
	// Set up the interpolation
	acc = gsl_interp_accel_alloc();
	sp = gsl_interp_alloc(interptype, n);
	gsl_interp_init(sp, static_cast<double *>(&_x[0]), static_cast<double *>(&_y[0]), n);
}

// Now define the copy constructor and assignment operator
Spline::Spline(const Spline& sp1) {
	_x = sp1._x; _y = sp1._y; interptype = sp1.interptype;
	init();
}

Spline& Spline::operator=(const Spline& sp1) {
	if (this == &sp1) {
		return *this;
	}

	_x = sp1._x; _y = sp1._y; interptype = sp1.interptype;
	init();
	return *this;
}


Spline::~Spline() {
	if (acc != NULL) gsl_interp_accel_free(acc);
	if (sp != NULL) gsl_interp_free(sp);

}

double Spline::operator()(double x) {
	return gsl_interp_eval(sp, static_cast<double *>(&_x[0]), static_cast<double *>(&_y[0]), x, acc);
}
