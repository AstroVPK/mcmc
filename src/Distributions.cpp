#include <cstddef>
#include <mathimf.h>
#include <cstdio>

#include "Constants.hpp"
#include "Distributions.hpp"

using namespace std;

double distributions::Weibull(double *param, double x, double &LnPriorVal, double &LnLikelihoodVal) {
	double const k = param[0], disp = param[1], lambda = param[2];
	double kOverLambda = log(k/lambda), kMinusOne = k - 1.0, xMinusDispOverLambda = (x - disp)/lambda;
	double LnLike = kOverLambda + kMinusOne*log(xMinusDispOverLambda) - pow(xMinusDispOverLambda, k);
	if (std::isnan(LnLike)) { // http://stackoverflow.com/questions/33770374/why-is-isnan-ambiguous-and-how-to-avoid-it
		LnLike = -mcmc::infiniteVal;
	}
	LnLikelihoodVal = LnLike;
	LnPriorVal = 0.0;
	return LnLike;
}

double distributions::Weibull(double *param, distributions::Data *DataPtr, double &LnPriorVal, double &LnLikelihoodVal) {
	double const k = param[0], disp = param[1], lambda = param[2];
	distributions::Data DataVal = *DataPtr;
	double LnLike = 0.0;
	double kOverLambda = DataVal.numPts*log(k/lambda), kMinusOne = k - 1.0, xMinusDispOverLambda;
	for (size_t ptNum = 0; ptNum < DataVal.numPts; ++ptNum) {
		xMinusDispOverLambda = (DataVal.x[ptNum] - disp)/lambda;
		LnLike += kMinusOne*log(xMinusDispOverLambda) - pow(xMinusDispOverLambda, k);
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
