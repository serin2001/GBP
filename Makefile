# Paths
BOOST_INCLUDE = /usr/local/include/boost    # Change to your Boost library include path
BOOST_LIB = /usr/local/lib                   # Change to your Boost library path

# C++ compiler
CXX = g++
CXXFLAGS = -std=c++20 -O2 -fopenmp -march=native 

SOURCES = $(wildcard Code/*.cpp Code/BetweennessApprox/*.cpp)

# Define LIBS to include NetworKit and its dependencies (requires correct installation paths)
# This assumes a standard installation of NetworKit system-wide.
LIBS = -lnetworkit -lnlopt

CBAG: $(SOURCES)
	$(CXX) -I $(BOOST_INCLUDE) -L $(BOOST_LIB) $(SOURCES) $(CXXFLAGS) -o CBAG $(LIBS)

clean:
	rm CBAG
