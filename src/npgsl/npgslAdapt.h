/*
 * gslAdapt.h
 *
 *  Created on: Oct 25, 2012
 *      Author: npadmana
 */

#ifndef NPGSLADAPT_H_
#define NPGSLADAPT_H_

template<class F>
static double gslFunctionAdapter( double x, void* p)
{
    // Here I do recover the "right" pointer, safer to use static_cast
    // than reinterpret_cast.
    F* function = static_cast<F*>( p );
    return (*function)( x );
}

/** Helper function to wrap normal C++

template<class F>
gsl_function convertToGslFunction( const F& f )
{
    gsl_function gslFunction;

    const void* p = &f;
    gslFunction.function = &gslFunctionAdapter<F>;
    // Just to eliminate the const.
    gslFunction.params = const_cast<void*>(p);

    return gslFunction;
}


#endif /* GSLADAPT_H_ */
