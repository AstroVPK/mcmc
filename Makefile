#
# A simple makefile for managing build of project composed of C source files.
#


# It is likely that default C compiler is already gcc, but explicitly
# set, just to be sure
CC = icpc

# The CFLAGS variable sets compile flags for gcc:
#  -g        compile with debug information
#  -Wall     give verbose compiler warnings
#  -O0       do not optimize generated code
#  -std=c99  use the C99 standard language definition
CFLAGS = -g -Wall -O3 -std=c++11 -qopenmp -qopenmp-simd -DMKL_ILP64 -I${MKLROOT}/include

# The LDFLAGS variable sets flags for linker
#  -limf   says to link in libimf (the Intel math library)
LDFLAGS = -limf -lmkl_intel_ilp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl
LDLINE =  -L${MKLROOT}/lib/intel64

# In this section, you list the files that are part of the project.
# If you add/change names of source files, here is where you
# edit the Makefile.
SOURCES = src/test.cpp src/Constants.cpp src/Distributions.cpp src/MCMC.cpp
OBJECTS = $(SOURCES:.c=.o)
TARGET = test
INCLINE = -I include/

# The first target defined in the makefile is the one
# used when make is invoked with no argument. Given the definitions
# above, this Makefile file will build the one named TARGET and
# assume that it depends on all the named OBJECTS files.

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLINE) -o $@ $^ $(LDFLAGS) $(LDLINE)

# Phony means not a "real" target, it doesn't build anything
# The phony target "clean" is used to remove all compiled object files.

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core
