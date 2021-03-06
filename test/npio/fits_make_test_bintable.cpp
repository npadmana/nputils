// cookbook CCfits demonstration program
//	Astrophysics Science Division,
//	NASA/ Goddard Space Flight Center
//	HEASARC
//	http://heasarc.gsfc.nasa.gov
//	e-mail: ccfits@legacy.gsfc.nasa.gov
//
//	Original author: Ben Dorman


// The CCfits headers are expected to be installed in a subdirectory of
// the include path.

// The <CCfits> header file contains all that is necessary to use both the CCfits
// library and the cfitsio library (for example, it includes fitsio.h) thus making
// all of cfitsio's symbolic names available.


// this includes 12 of the CCfits headers and will support all CCfits operations.
// the installed location of the library headers is $(ROOT)/include/CCfits

// to use the library either add -I$(ROOT)/include/CCfits or #include <CCfits/CCfits>
// in the compilation target.


#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <CCfits/CCfits>
// The library is enclosed in a namespace.

using namespace std;
using namespace CCfits;



int main();
int writeBinary();

int main()
{


     FITS::setVerboseMode(true);

     try

     {

        if (!writeBinary()) cerr << " writeBinary()  \n";

     }
     catch (FitsException&)
     // will catch all exceptions thrown by CCfits, including errors
     // found by cfitsio (status != 0).
     {

        cerr << " Fits Exception Thrown by test function \n";

     }
    return 0;
}


int writeBinary() {
    auto_ptr<FITS> pFits(0);

    try
    {

        const string fileName("!atestfil.fit");
        pFits.reset( new FITS(fileName,Write) );
    }
    catch (CCfits::FITS::CantOpen&)
    {
          return -1;
    }

    // Define the data structures
    unsigned long rows(3);
    string hduName("TABLE_BINARY");
    vector<string> colName(4,"");
    vector<string> colForm(4,"");
    vector<string> colUnit(4,"");

    // Now fill in the column descriptions
    colName[0] = "int32"; colForm[0] = "J";
    colName[1] = "float"; colForm[1] = "E";
    colName[2] = "double"; colForm[2] = "D";
    colName[3] = "int64"; colForm[3] = "K";

    // Now create an appropriate HDU
    Table* newTable = pFits->addTable(hduName,rows,colName,colForm,colUnit);

    // Now we start filling in columns

    // column 0
    {
    	vector<int> vec(3, 96); // Simple fill in
        newTable->column(colName[0]).write(vec,1);
    }

    // column 1
    {
    	vector<float> vec(3, 2.726);
    	newTable->column(colName[1]).write(vec, 1);
    }

    // column 2
    {
    	vector<double> vec(3, 3.1415926);
    	newTable->column(colName[2]).write(vec, 1);
    }

    // column 4
    {
    	vector<int64_t> vec(3, -32);
    	vec[2] = 10;
    	newTable->column(colName[3]).write(vec, 1);
    }


    return 0;
}



