# setup kali environment
#
# source this file from your ~/.bashrc
#
# relative to <kali>/bin/
MCMC=$(cd "$(dirname "$BASH_SOURCE")/.."; pwd)
MCMC_CYTHON=$(cd "$(dirname "$BASH_SOURCE")/../python"; pwd)
MCMC_CYTHON_LIB=$(cd "$(dirname "$BASH_SOURCE")/../lib"; pwd)

export MCMC
export PYTHONPATH="$MCMC_CYTHON:$PYTHONPATH"
export PYTHONPATH="$MCMC_CYTHON_LIB:$PYTHONPATH"
echo "notice: mcmc tools have been set up."
