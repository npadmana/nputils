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

	int nR;
	unsigned long int seed;
	double rmin, rmax;

	// Get the input parameters -- pull them into their own scope
	{
		po::options_description desc("Allowed options");
		desc.add_options()
	    		("help", "produce help message")
	    		("nR", po::value<int>(&nR)->default_value(100), "Number of R points")
	    		("rmin", po::value<double>(&rmin)->default_value(500.0), "Minimum r value")
	    		("rmax", po::value<double>(&rmax)->default_value(2000.0), "Maximum r value")
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
	cout << "# Generating points...\n";
	Pos pp = generate(nR, rmin, rmax, seed);
	cout << "# Points generated...\n";

}
