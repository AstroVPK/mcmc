#ifdef __INTEL_COMPILER
    #include <mathimf.h>
    #if defined __APPLE__ && defined __MACH__
        #include <malloc/malloc.h>
    #else
        #include <malloc.h>
    #endif
#else
    #include <math.h>
    #include <mm_malloc.h>
#endif
#include <cmath>
#include <complex>
#include "Constants.hpp"

using namespace std;

extern complex<double> mcmc::complexZero (0.0,0.0);
extern complex<double> mcmc::complexOne (1.0,0.0);
extern double mcmc::pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;
extern double mcmc::twoPi = 2.0*mcmc::pi;
extern double mcmc::fourPiSq = 4.0*pow(mcmc::pi, 2.0);
extern double mcmc::piSq = pow(mcmc::pi, 2.0);
extern double mcmc::halfPi = mcmc::pi/2.0;
extern double mcmc::e = 2.71828182845904523536028747135266249775724709369995;
extern double mcmc::infiniteVal = HUGE_VAL;
extern double mcmc::log2OfE = log2(mcmc::e);
extern double mcmc::log2Pi = log2(2.0*mcmc::pi)/mcmc::log2OfE;
