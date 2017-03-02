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

void doWeibull(int numPts) {
    int ndims = 3;
    unsigned int WeibullSeed = 136316322;
    unsigned int DitherSeed = 908123483;
    double shape = 1.5;       // Shape parameter. shape < 1 -> Slight decrease in failure rate with time.
    double disp = 0.0;    // Displacement parameter.
    double lambda = 1.0;  // Scale parameter.

    distributions::Data weibullData;
    weibullData.numPts = numPts;
    weibullData.x = static_cast<double*>(_mm_malloc(numPts*sizeof(double), 64));
    double *ditherData = static_cast<double*>(_mm_malloc(ndims*numPts*sizeof(double), 64));
    VSLStreamStatePtr WeibullStream, DitherStream;
    vslNewStream(&WeibullStream, VSL_BRNG_SFMT19937, WeibullSeed);
    vslNewStream(&DitherStream, VSL_BRNG_SFMT19937, DitherSeed);
    vdRngWeibull(VSL_RNG_METHOD_WEIBULL_ICDF_ACCURATE, WeibullStream, numPts, weibullData.x, shape, disp, lambda);
    vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, DitherStream, ndims*numPts, ditherData, 0.0, 1.0e-3);
    vslDeleteStream(&WeibullStream);
    vslDeleteStream(&DitherStream);

    int nwalkers = 264, nsteps = 1000, numThreads = 264;
    double mcmcA = 2.0;
    void *p2WeibullData = &weibullData;
    unsigned int zSSeed = 90238173, walkerSeed = 23166731, moveSeed = 673646293;
    double *initPos = static_cast<double*>(_mm_malloc(ndims*nwalkers*sizeof(double), 64));
    double *Chain = static_cast<double*>(_mm_malloc(ndims*nwalkers*nsteps*sizeof(double), 64));
    double *LnPrior = static_cast<double*>(_mm_malloc(nwalkers*nsteps*sizeof(double), 64));
    double *LnLikelihood = static_cast<double*>(_mm_malloc(nwalkers*nsteps*sizeof(double), 64));
    for (size_t i = 0; i < nwalkers; ++i) {
        initPos[ndims*i] = shape + ditherData[i*ndims];
        initPos[ndims*i + 1] = disp + ditherData[i*ndims + 1];
        initPos[ndims*i + 2] = lambda + ditherData[i*ndims + 2];
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

    cout << "Weibull distrib" << endl;
    cout << "<Shape>: " << para1 << endl;
    cout << "<Disp>: " << para2 << endl;
    cout << "<Scale>: " << para3 << endl;
    cout << "Writing results..." << endl;

    ofstream weibullFile;
    weibullFile.open ("weibullData.txt");
    weibullFile << "numPts: " << numPts << endl;
    weibullFile << "Shape: " << shape << "; <Shape>: " << para1 << endl;
    weibullFile << "Displ: " << disp << "; <Displ>: " << para2 << endl;
    weibullFile << "Scale: " << lambda << "; <Scale>: " << para3 << endl;
    for (size_t i = 0; i < numPts; ++i) {
        weibullFile << weibullData.x[i] << endl;
    }
    weibullFile.close();

    _mm_free(weibullData.x);
    _mm_free(ditherData);
    _mm_free(initPos);
    _mm_free(Chain);
    _mm_free(LnPrior);
    _mm_free(LnLikelihood);
}

void doExponential(int numPts) {
    int ndims = 2;
    unsigned int ExponentialSeed = 136316322;
    unsigned int DitherSeed = 908123483;
    double disp = 0.0;    // Displacement parameter.
    double scale = 1.5;       // Scale parameter.

    distributions::Data exponentialData;
    exponentialData.numPts = numPts;
    exponentialData.x = static_cast<double*>(_mm_malloc(numPts*sizeof(double), 64));
    double *ditherData = static_cast<double*>(_mm_malloc(ndims*numPts*sizeof(double), 64));
    VSLStreamStatePtr ExponentialStream, DitherStream;
    vslNewStream(&ExponentialStream, VSL_BRNG_SFMT19937, ExponentialSeed);
    vslNewStream(&DitherStream, VSL_BRNG_SFMT19937, DitherSeed);
    vdRngExponential(VSL_RNG_METHOD_EXPONENTIAL_ICDF_ACCURATE, ExponentialStream, numPts, exponentialData.x, disp, scale);
    vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, DitherStream, ndims*numPts, ditherData, 0.0, 1.0e-3);
    vslDeleteStream(&ExponentialStream);
    vslDeleteStream(&DitherStream);

    int nwalkers = 264, nsteps = 1000, numThreads = 264;
    double mcmcA = 2.0;
    void *p2ExponentialData = &exponentialData;
    unsigned int zSSeed = 90238173, walkerSeed = 23166731, moveSeed = 673646293;
    double *initPos = static_cast<double*>(_mm_malloc(ndims*nwalkers*sizeof(double), 64));
    double *Chain = static_cast<double*>(_mm_malloc(ndims*nwalkers*nsteps*sizeof(double), 64));
    double *LnPrior = static_cast<double*>(_mm_malloc(nwalkers*nsteps*sizeof(double), 64));
    double *LnLikelihood = static_cast<double*>(_mm_malloc(nwalkers*nsteps*sizeof(double), 64));
    for (size_t i = 0; i < nwalkers; ++i) {
        initPos[ndims*i] = disp + ditherData[i*ndims];
        initPos[ndims*i + 1] = scale + ditherData[i*ndims + 1];
    }
    for (size_t i = 0; i < nsteps*nwalkers*ndims; ++i) {
        Chain[i] = 0.0;
    }
    for (size_t i = 0; i < nsteps*nwalkers; ++i) {
        LnPrior[i] = 0.0;
        LnLikelihood[i] = 0.0;
    }

    mcmc::EnsembleSampler newEnsemble = mcmc::EnsembleSampler(ndims, nwalkers, nsteps, numThreads, mcmcA, distributions::Exponential, p2ExponentialData, zSSeed, walkerSeed, moveSeed);
    newEnsemble.runMCMC(initPos);
    newEnsemble.getChain(Chain);
    newEnsemble.getChainVals(LnPrior, LnLikelihood);

    // Compute averages for each parameter...
    double para1 = 0.0, para2 = 0.0, numEst = 0.0;
    for (size_t stepNum = nsteps/2; stepNum < nsteps; ++stepNum) {
        for (size_t walkerNum = 0; walkerNum < nwalkers; ++walkerNum) {
            para1 += Chain[stepNum*nwalkers*ndims + walkerNum*ndims];
            para2 += Chain[stepNum*nwalkers*ndims + walkerNum*ndims + 1];
            numEst += 1.0;
        }
    }
    para1 /= numEst;
    para2 /= numEst;

    cout << "Exponential Distrib" << endl;
    cout << "<Disp>: " << para1 << endl;
    cout << "<Scale>: " << para2 << endl;
    cout << "Writing results..." << endl;

    ofstream exponentialFile;
    exponentialFile.open ("exponentialData.txt");
    exponentialFile << "numPts: " << numPts << endl;
    exponentialFile << "Displ: " << disp << "; <Displ>: " << para1 << endl;
    exponentialFile << "Scale: " << scale << "; <Scale>: " << para2 << endl;
    for (size_t i = 0; i < numPts; ++i) {
        exponentialFile << exponentialData.x[i] << endl;
    }
    exponentialFile.close();
    ofstream exponentialChainFile;
    exponentialChainFile.open ("exponentialChain.txt");
    exponentialChainFile << "numDim: " << ndims << endl;
    exponentialChainFile << "numWalkers: " << nwalkers << endl;
    exponentialChainFile << "numSteps: " << nsteps << endl;
    for (size_t stepNum = 0; stepNum < nsteps; ++stepNum) {
        for (size_t walkerNum = 0; walkerNum < nwalkers; ++walkerNum) {
            for (size_t dimNum = 0; dimNum < ndims; ++dimNum) {
                exponentialChainFile << Chain[stepNum*nwalkers*ndims + walkerNum*ndims + dimNum] << " ";
            }
            exponentialChainFile << LnLikelihood[stepNum*nwalkers + walkerNum] << endl;
        }
    }
    exponentialChainFile.close();

    _mm_free(exponentialData.x);
    _mm_free(ditherData);
    _mm_free(initPos);
    _mm_free(Chain);
    _mm_free(LnPrior);
    _mm_free(LnLikelihood);
}

void doRayleigh(int numPts) {
    int ndims = 2;
    unsigned int RayleighSeed = 136316322;
    unsigned int DitherSeed = 908123483;
    double disp = 0.0;    // Displacement parameter.
    double scale = 1.5;       // Scale parameter.

    distributions::Data rayleighData;
    rayleighData.numPts = numPts;
    rayleighData.x = static_cast<double*>(_mm_malloc(numPts*sizeof(double), 64));
    double *ditherData = static_cast<double*>(_mm_malloc(ndims*numPts*sizeof(double), 64));
    VSLStreamStatePtr RayleighStream, DitherStream;
    vslNewStream(&RayleighStream, VSL_BRNG_SFMT19937, RayleighSeed);
    vslNewStream(&DitherStream, VSL_BRNG_SFMT19937, DitherSeed);
    vdRngRayleigh(VSL_RNG_METHOD_RAYLEIGH_ICDF_ACCURATE, RayleighStream, numPts, rayleighData.x, disp, scale);
    vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, DitherStream, ndims*numPts, ditherData, 0.0, 1.0e-3);
    vslDeleteStream(&RayleighStream);
    vslDeleteStream(&DitherStream);

    int nwalkers = 264, nsteps = 1000, numThreads = 264;
    double mcmcA = 2.0;
    void *p2RayleighData = &rayleighData;
    unsigned int zSSeed = 90238173, walkerSeed = 23166731, moveSeed = 673646293;
    double *initPos = static_cast<double*>(_mm_malloc(ndims*nwalkers*sizeof(double), 64));
    double *Chain = static_cast<double*>(_mm_malloc(ndims*nwalkers*nsteps*sizeof(double), 64));
    double *LnPrior = static_cast<double*>(_mm_malloc(nwalkers*nsteps*sizeof(double), 64));
    double *LnLikelihood = static_cast<double*>(_mm_malloc(nwalkers*nsteps*sizeof(double), 64));
    for (size_t i = 0; i < nwalkers; ++i) {
        initPos[ndims*i] = disp + ditherData[i*ndims];
        initPos[ndims*i + 1] = scale + ditherData[i*ndims + 1];
    }
    for (size_t i = 0; i < nsteps*nwalkers*ndims; ++i) {
        Chain[i] = 0.0;
    }
    for (size_t i = 0; i < nsteps*nwalkers; ++i) {
        LnPrior[i] = 0.0;
        LnLikelihood[i] = 0.0;
    }

    mcmc::EnsembleSampler newEnsemble = mcmc::EnsembleSampler(ndims, nwalkers, nsteps, numThreads, mcmcA, distributions::Rayleigh, p2RayleighData, zSSeed, walkerSeed, moveSeed);
    newEnsemble.runMCMC(initPos);
    newEnsemble.getChain(Chain);
    newEnsemble.getChainVals(LnPrior, LnLikelihood);

    // Compute averages for each parameter...
    double para1 = 0.0, para2 = 0.0, numEst = 0.0;
    for (size_t stepNum = nsteps/2; stepNum < nsteps; ++stepNum) {
        for (size_t walkerNum = 0; walkerNum < nwalkers; ++walkerNum) {
            para1 += Chain[stepNum*nwalkers*ndims + walkerNum*ndims];
            para2 += Chain[stepNum*nwalkers*ndims + walkerNum*ndims + 1];
            numEst += 1.0;
        }
    }
    para1 /= numEst;
    para2 /= numEst;

    cout << "Rayleigh Distrib" << endl;
    cout << "<Disp>: " << para1 << endl;
    cout << "<Scale>: " << para2 << endl;
    cout << "Writing results..." << endl;

    ofstream rayleighFile;
    rayleighFile.open ("rayleighData.txt");
    rayleighFile << "numPts: " << numPts << endl;
    rayleighFile << "Displ: " << disp << "; <Displ>: " << para1 << endl;
    rayleighFile << "Scale: " << scale << "; <Scale>: " << para2 << endl;
    for (size_t i = 0; i < numPts; ++i) {
        rayleighFile << rayleighData.x[i] << endl;
    }
    rayleighFile.close();

    _mm_free(rayleighData.x);
    _mm_free(ditherData);
    _mm_free(initPos);
    _mm_free(Chain);
    _mm_free(LnPrior);
    _mm_free(LnLikelihood);
}

int main() {
    int numPts = 10000;
    doWeibull(numPts);
    doExponential(numPts);
    doRayleigh(numPts);
}
