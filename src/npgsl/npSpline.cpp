#include "npSpline.h"

using namespace std;

Spline::Spline(vector<double> x, vector<double> y, const gsl_interp_type *sptype) {
	acc = gsl_interp_accel_alloc();
	int n = x.size();
	sp = gsl_spline_alloc(sptype, n);
	gsl_spline_init(sp, &x[0], &y[0], n);
}

Spline::~Spline() {
	gsl_interp_accel_free(acc);
	gsl_spline_free(sp);

}

double Spline::operator()(double x) {
	return gsl_spline_eval(sp, x, acc);
}
