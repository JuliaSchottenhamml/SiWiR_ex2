#include	<stdlib.h>
#include	<iostream>
#include	<sstream>
#include	<fstream>
#include	<cmath>
#include	<immintrin.h>
#include	<omp.h>

#ifdef USE_LIKWID
extern "C" {
#include <likwid.h>
}
#endif

#include	"Timer.h"

/**
  Converts a string to an arbitrary type. >> operator must be defined for the target type.
  @param string string which should be converted
  @return converted string
 **/
template<typename T>
T StringTo(const std::string& string){
	T valor;

	std::stringstream stream(string);
	stream >> valor;
	return valor;
}

int main(int argc, char **argv) {
	///******************************************************
	///********************** INPUT *************************
	///******************************************************
	if (argc != 4) {
		std::cout << "Invalid number of arguments!" << std::endl;
		std::cout << "./rgbs nx ny c" << std::endl;
		exit(EXIT_FAILURE);
	}

	int 	nx = 0;
	int	ny = 0;
	int	c = 0;
	nx = StringTo<int>(argv[1]);
	ny = StringTo<int>(argv[2]);
	c = StringTo<int>(argv[3]);
	
	std::cout << "nx," << nx << std::endl;
	std::cout << "ny," << ny << std::endl;
	std::cout << "c," << c <<std::endl;
	if (getenv("GOMP_CPU_AFFINITY")){
		std::cout << "GOMP_CPU_AFFINITY," << getenv("GOMP_CPU_AFFINITY") << std::endl;
	}
	std::cout << "numThreads," << omp_get_num_threads() << std::endl;

	///******************************************************
	///********************** CALCULATION *******************
	///******************************************************
	double time = 0;
	
#ifdef USE_LIKWID
	likwid_markerInit();
	likwid_markerStartRegion("dummy");
#endif

	siwir::Timer	timer;

	time = timer.elapsed();
	std::cout << "time," << time << std::endl;

#ifdef USE_LIKWID
	likwid_markerStopRegion("dummy");
	likwid_markerClose();
#endif

	///******************************************************
	///********************** OUTPUT ************************
	///******************************************************

	};
