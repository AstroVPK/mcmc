#include <cstddef>
#include <mathimf.h>
#include <cstdio>
#include <iostream>

#include "Constants.hpp"
#include "Distributions.hpp"

using namespace std;

double distributions::Weibull(double *param, double x, double &LnPriorVal, double &LnLikelihoodVal) {
    double kOverLambda = log(param[0]/param[2]), kMinusOne = param[0] - 1.0, xMinusDispOverLambda = (x - param[1])/param[2];
    double LnLike = kOverLambda + kMinusOne*log(xMinusDispOverLambda) - pow(xMinusDispOverLambda, param[0]);
    if (std::isnan(LnLike)) { // http://stackoverflow.com/questions/33770374/why-is-isnan-ambiguous-and-how-to-avoid-it
        LnLike = -mcmc::infiniteVal;
    }
    LnLikelihoodVal = LnLike;
    LnPriorVal = 0.0;
    return LnLike;
}

double distributions::Weibull(double *param, distributions::Data *DataPtr, double &LnPriorVal, double &LnLikelihoodVal) {
    distributions::Data DataVal = *DataPtr;
    double LnLike = 0.0;
    double kOverLambda = DataVal.numPts*log(param[0]/param[2]), kMinusOne = param[0] - 1.0, xMinusDispOverLambda;
    for (size_t ptNum = 0; ptNum < DataVal.numPts; ++ptNum) {
        xMinusDispOverLambda = (DataVal.x[ptNum] - param[1])/param[2];
        LnLike += kMinusOne*log(xMinusDispOverLambda) - pow(xMinusDispOverLambda, param[0]);
    }
    LnLike += kOverLambda;
    if (std::isnan(LnLike)) {  // http://stackoverflow.com/questions/33770374/why-is-isnan-ambiguous-and-how-to-avoid-it
        LnLike = -mcmc::infiniteVal;
    }
    LnLikelihoodVal = LnLike;
    LnPriorVal = 0.0;
    return LnLike;
}

double distributions::Weibull(double *param, void *VoidPtr, double &LnPriorVal, double &LnLikelihoodVal) {
    Data *DataPtr = static_cast<Data *>(VoidPtr);
    return Weibull(param, DataPtr, LnPriorVal, LnLikelihoodVal);
}

double distributions::Exponential(double *param, double x, double &LnPriorVal, double &LnLikelihoodVal) {
    if (param[1] <= 0.0) {
        LnLikelihoodVal = -mcmc::infiniteVal;
        LnPriorVal = 0.0;
        return -mcmc::infiniteVal;
    }
    if (x < param[0]) {
        LnLikelihoodVal = -mcmc::infiniteVal;
        LnPriorVal = 0.0;
        return -mcmc::infiniteVal;
    }
    double LnLike = -1.0*log(param[1]) - ((x - param[0])/param[1]);
    if (std::isnan(LnLike)) { // http://stackoverflow.com/questions/33770374/why-is-isnan-ambiguous-and-how-to-avoid-it
        LnLike = -mcmc::infiniteVal;
    }
    LnLikelihoodVal = LnLike;
    LnPriorVal = 0.0;
    return LnLike;
}

double distributions::Exponential(double *param, distributions::Data *DataPtr, double &LnPriorVal, double &LnLikelihoodVal) {
    if (param[1] <= 0.0) {
        LnPriorVal = 0.0;
        LnLikelihoodVal = -mcmc::infiniteVal;
        return -mcmc::infiniteVal;
    }
    distributions::Data DataVal = *DataPtr;
    double LnLike = 0.0;
    size_t ptCtr = 0;
    for (size_t ptNum = 0; ptNum < DataVal.numPts; ++ptNum) {
        if (DataVal.x[ptNum] > param[0]) {
            LnLike += -1.0*(DataVal.x[ptNum] - param[0])/param[1];
            ptCtr = ptCtr + 1;
        } else {
            LnLike += -mcmc::infiniteVal;
        }
    }
    LnLike = LnLike - ptCtr*log(param[1]);
    if (std::isnan(LnLike)) {  // http://stackoverflow.com/questions/33770374/why-is-isnan-ambiguous-and-how-to-avoid-it
        LnLike = -mcmc::infiniteVal;
    }
    LnLikelihoodVal = LnLike;
    LnPriorVal = 0.0;
    return LnLike;
}

double distributions::Exponential(double *param, void *VoidPtr, double &LnPriorVal, double &LnLikelihoodVal) {
    Data *DataPtr = static_cast<Data *>(VoidPtr);
    return Exponential(param, DataPtr, LnPriorVal, LnLikelihoodVal);
}

double distributions::Rayleigh(double *param, double x, double &LnPriorVal, double &LnLikelihoodVal) {
    if (param[1] <= 0.0) {
        LnLikelihoodVal = -mcmc::infiniteVal;
        LnPriorVal = 0.0;
        return -mcmc::infiniteVal;
    }
    double LnLike = log(2.0*(x - param[0])) - 2.0*log(param[1]) - pow((x - param[0])/param[1], 2.0);
    if (std::isnan(LnLike)) { // http://stackoverflow.com/questions/33770374/why-is-isnan-ambiguous-and-how-to-avoid-it
        LnLike = -mcmc::infiniteVal;
    }
    LnLikelihoodVal = LnLike;
    LnPriorVal = 0.0;
    return LnLike;
}

double distributions::Rayleigh(double *param, distributions::Data *DataPtr, double &LnPriorVal, double &LnLikelihoodVal) {
    if (param[1] <= 0.0) {
        LnLikelihoodVal = -mcmc::infiniteVal;
        LnPriorVal = 0.0;
        return -mcmc::infiniteVal;
    }
    distributions::Data DataVal = *DataPtr;
    double LnLike = 0.0;
    size_t ptCtr = 0;
    for (size_t ptNum = 0; ptNum < DataVal.numPts; ++ptNum) {
        if (DataVal.x[ptNum] > param[0]) {
            LnLike = LnLike + log(2.0*(DataVal.x[ptNum] - param[0])) - pow((DataVal.x[ptNum] - param[0])/param[1], 2.0);
            ptCtr = ptCtr + 1;
        } else {
            LnLike += -mcmc::infiniteVal;
        }
    }
    LnLike = LnLike - 2.0*ptCtr*log(param[1]);
    if (std::isnan(LnLike)) {  // http://stackoverflow.com/questions/33770374/why-is-isnan-ambiguous-and-how-to-avoid-it
        LnLike = -mcmc::infiniteVal;
    }
    LnLikelihoodVal = LnLike;
    LnPriorVal = 0.0;
    return LnLike;
}

double distributions::Rayleigh(double *param, void *VoidPtr, double &LnPriorVal, double &LnLikelihoodVal) {
    Data *DataPtr = static_cast<Data *>(VoidPtr);
    return Rayleigh(param, DataPtr, LnPriorVal, LnLikelihoodVal);
}
