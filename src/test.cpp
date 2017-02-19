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
#include <mkl.h>
#include <mkl_types.h>
#include <iostream>
#include <fstream>

#include "Distributions.hpp"
#include "MCMC.hpp"

using namespace std;

int main() {

// First draw from a Weibul distribution.
int ndims = 3;
int numPts = 10000;
unsigned int WeibullSeed = 136316322;
unsigned int DitherSeed = 908123483;
double k = 1.5;       // Shape parameter. k < 1 -> Slight decrease in failure rate with time.
double disp = 0.0;    // Displacement parameter.
double lambda = 1.0;  // Scale parameter.

distributions::Data weibullData;
weibullData.numPts = numPts;
weibullData.x = static_cast<double*>(_mm_malloc(numPts*sizeof(double), 64));
double *ditherData = static_cast<double*>(_mm_malloc(ndims*numPts*sizeof(double), 64));
VSLStreamStatePtr WeibullStream, DitherStream;
vslNewStream(&WeibullStream, VSL_BRNG_SFMT19937, WeibullSeed);
vslNewStream(&DitherStream, VSL_BRNG_SFMT19937, DitherSeed);
vdRngWeibull(VSL_RNG_METHOD_WEIBULL_ICDF_ACCURATE, WeibullStream, numPts, weibullData.x, k, disp, lambda);
vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, DitherStream, ndims*numPts, ditherData, 0.0, 1.0e-3);
vslDeleteStream(&WeibullStream);
vslDeleteStream(&DitherStream);

ofstream weibullFile;
weibullFile.open ("weibullData.txt");
for (size_t i = 0; i < numPts; ++i) {
    weibullFile << weibullData.x[i] << endl;
}
weibullFile.close();

int nwalkers = 80, nsteps = 1000, numThreads = 8;
double mcmcA = 2.0;
void *p2WeibullData = &weibullData;
unsigned int zSSeed = 90238173, walkerSeed = 23166731, moveSeed = 673646293;
double *initPos = static_cast<double*>(_mm_malloc(ndims*nwalkers*sizeof(double), 64));
double *Chain = static_cast<double*>(_mm_malloc(ndims*nwalkers*nsteps*sizeof(double), 64));
double *LnPrior = static_cast<double*>(_mm_malloc(nwalkers*nsteps*sizeof(double), 64));
double *LnLikelihood = static_cast<double*>(_mm_malloc(nwalkers*nsteps*sizeof(double), 64));
for (size_t i = 0; i < nwalkers; ++i) {
    initPos[ndims*i] = 1.5 + ditherData[i*ndims];
    initPos[ndims*i + 1] = 0.0 + ditherData[i*ndims + 1];
    initPos[ndims*i + 2] = 0.5 + ditherData[i*ndims + 2];
}
for (size_t i = 0; i < nsteps*nwalkers*ndims; ++i) {
    Chain[i] = 0.0;
}
for (size_t i = 0; i < nsteps*nwalkers; ++i) {
    LnPrior[i] = 0.0;
    LnLikelihood[i] = 0.0;
}

mcmc::EnsembleSampler newEnsemble = mcmc::EnsembleSampler(ndims, nwalkers, nsteps, numThreads, mcmcA, distributions::Weibull, p2WeibullData, zSSeed, walkerSeed, moveSeed);
newEnsemble.runMCMC(initPos);
newEnsemble.getChain(Chain);
newEnsemble.getChainVals(LnPrior, LnLikelihood);

// Compute averages for each parameter...
double para1 = 0.0, para2 = 0.0, para3 = 0.0, numEst = 0.0;
for (size_t stepNum = nsteps/2; stepNum < nsteps; ++stepNum) {
    for (size_t walkerNum = 0; walkerNum < nwalkers; ++walkerNum) {
        para1 += Chain[stepNum*nwalkers*ndims + walkerNum*ndims];
        para2 += Chain[stepNum*nwalkers*ndims + walkerNum*ndims + 1];
        para3 += Chain[stepNum*nwalkers*ndims + walkerNum*ndims + 2];
        numEst += 1.0;
    }
}
para1 /= numEst;
para2 /= numEst;
para3 /= numEst;

cout << "<Shape>: " << para1 << endl;
cout << "<Disp>: " << para2 << endl;
cout << "<Scale>: " << para3 << endl;

_mm_free(weibullData.x);
_mm_free(ditherData);
_mm_free(initPos);
_mm_free(Chain);
_mm_free(LnPrior);
_mm_free(LnLikelihood);
}
