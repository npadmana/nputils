/* Code for generating random points in a shell, and the calculating RR for
 * them.
 *
 */

#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <cmath>
#include "boost/program_options.hpp"
#include "boost/format.hpp"

#include "npRandom.h"
#include "npHistogram2D.h"

using namespace std;
using namespace Eigen;
using boost::format;

namespace po = boost::program_options;

typedef vector<Vector3d> Pos;

Pos generate(int npart, double rmin, double rmax, unsigned long int seed) {
	Pos out;

	double x, dr3, rmin3, rr;
	rmin3 = pow(rmin, 3);
	dr3 = pow(rmax,3) - rmin3;
	npRandom ran1(seed);

	out.resize(npart);
	for (int ii=0; ii < npart; ++ii) {
		rr = cbrt(dr3*ran1() + rmin3);
		out[ii] = rr*ran1.dir3d();
	}

	return out;
}

void compute_rmu(Vector3d x, Vector3d y, double &rr, double &mu) {
	Vector3d s, l;
	s = x-y;
	l = (x+y)/2.0;
	rr = s.norm();
	mu = l.dot(s)/(l.norm()*rr);
}

int main(int argc, char** argv) {

	int nR, nrbins, nmubins;
	unsigned long int seed;
	double rmin, rmax, r0, r1, mu0, mu1;

	// Get the input parameters -- pull them into their own scope
	{
		po::options_description desc("Allowed options");
		desc.add_options()
	    		("help", "produce help message")
	    		("nR", po::value<int>(&nR)->default_value(100), "Number of R points")
	    		("rmin", po::value<double>(&rmin)->default_value(500.0), "Minimum r value for shell")
	    		("rmax", po::value<double>(&rmax)->default_value(2000.0), "Maximum r value for shell")
	    		("r0", po::value<double>(&r0)->default_value(10.0), "Minimum r value for histogram")
	    		("r1", po::value<double>(&r1)->default_value(100.0), "Maximum r value for histogram")
	    		("mu0", po::value<double>(&mu0)->default_value(-1.0), "Minimum mu value for histogram")
	    		("mu1", po::value<double>(&mu1)->default_value(1.0), "Maximum mu value for histogram")
	    		("nrbins", po::value<int>(&nrbins)->default_value(30), "Number of r bins")
	    		("nmubins", po::value<int>(&nmubins)->default_value(200), "Number of mu bins")
	    		("seed", po::value<unsigned long int>(&seed)->default_value(99), "Seed value")
	    		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("help")) {
			cout << desc << "\n";
			return 1;
		}

	}

	cout << format("# Using %1% random points from r=%2% to %3%\n") % nR % rmin % rmax;
	cout << format("# Histogramming r from %1% to %2% in %3% bins\n") % r0 % r1 % nrbins;
	cout << format("# Histogramming mu from %1% to %2% in %3% bins\n") % mu0 % mu1 % nmubins;
	cout << "# Generating points...\n";
	Pos pp = generate(nR, rmin, rmax, seed);
	cout << "# Points generated...\n";

	// Now compute the histogram
	Histogram2D h1(nrbins, r0, r1, nmubins, mu0, mu1);

	cout << "# Starting histogram....\n";
	double rr, mu;
	for (int ii=0; ii < nR; ++ii)
		for (int jj=0; jj < nR; ++jj) {
			compute_rmu(pp[ii], pp[jj], rr, mu);
			h1.add(rr, mu);
		}
	cout << "# Histogram completed.....\n";

}
