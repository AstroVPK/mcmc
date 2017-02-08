#include <cstddef>
#include <mathimf.h>

using namespace std;

namespace distributions {

struct Data {
	size_t numPts;
	double* x;
    };

double Weibull(double *param, double x, double &LnPriorVal, double &LnLikelihoodVal);

double Weibull(double *param, Data *DataPtr, double &LnPriorVal, double &LnLikelihoodVal);

double Weibull(double *param, void *VoidPtr, double &LnPriorVal, double &LnLikelihoodVal);

}
