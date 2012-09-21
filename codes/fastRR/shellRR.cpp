/* Code for generating random points in a shell, and the calculating RR for
 * them.
 *
 */

#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <fstream>
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
typedef tuple<double, double> paird;

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

void compute_rmu(const Vector3d& x, const Vector3d& y, double &rr, double &mu) {
	const double eps=1.e-12; // Avoid divide by zero

	Vector3d s, l;
	s = x-y;
	l = (x+y)/2.0;
	rr = s.norm();
	mu = l.dot(s)/(l.norm()*(rr+eps));
}

int main(int argc, char** argv) {


	int nR, nrbins, nmubins, nsims;
	unsigned long int seed;
	double rmin, rmax, r0, r1, mu0, mu1;
	string outfn;

	// Get the input parameters -- pull them into their own scope
	{
		try {
			string config_fn;

			po::options_description desc("Allowed options");
			desc.add_options()
	    				("help", "produce help message")
	    				("config", po::value<string>(&config_fn), "Configuration filename")
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
	    				("nsims", po::value<int>(&nsims)->default_value(1), "Number of simulations")
	    				("output",po::value<string>(&outfn)->default_value("shellRR.out"), "Output file")
	    				;

			po::variables_map vm;
			po::store(po::parse_command_line(argc, argv, desc), vm);
			po::notify(vm);

			if (vm.count("help")) {
				cout << desc << "\n";
				return 1;
			}

			if (vm.count("config")) {
				ifstream ifs(config_fn.c_str());
				if (!ifs) {
					cout << "Error opening configuration file " << config_fn << endl;
					return 1;
				}
				po::store(po::parse_config_file(ifs, desc), vm);
				po::notify(vm);
				ifs.close();
			}
		}
		catch (exception &e) {
			cout << e.what() << "\n";
			return 1;
		}

	}

	ofstream ofs(outfn);
	if (!ofs) {
		cout << "Unable to open output file\n";
		return 1;
	}
	ofs << format("# Using %1% random points from r=%2% to %3%\n") % nR % rmin % rmax;
	ofs << format("# Histogramming r from %1% to %2% in %3% bins\n") % r0 % r1 % nrbins;
	ofs << format("# Histogramming mu from %1% to %2% in %3% bins\n") % mu0 % mu1 % nmubins;

	// Now compute the histogram
	Histogram2D h1(nrbins, r0, r1, nmubins, mu0, mu1);
	double rr, mu;

	for (int isim=0; isim < nsims; ++isim) {
		Pos pp = generate(nR, rmin, rmax, seed+isim);
		for (int ii=0; ii < nR; ++ii)
			for (int jj=0; jj < nR; ++jj) {
				compute_rmu(pp[ii], pp[jj], rr, mu);
				h1.add(rr, mu);
			}
	}

	// Print out histogram
	format histprint("%5i %5i %9.3f %9.3f %7.4f %7.4f %20.2f\n");
	vector<paird> xr(nrbins), yr(nmubins);
	for (int ii=0; ii < nrbins; ++ii) xr[ii] = h1.xrange(ii);
	for (int ii=0; ii < nmubins; ++ii) yr[ii] = h1.yrange(ii);
	for (int ii=0; ii < nrbins; ++ii) {
		for (int jj=0; jj < nmubins; ++jj) {
			ofs << histprint % ii % jj % get<0>(xr[ii]) % get<1>(xr[ii])
					% get<0>(yr[jj]) % get<1>(yr[jj]) % h1(ii,jj);
		}
	}

	ofs.close();

}
