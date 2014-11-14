#include	<stdlib.h>
#include	<iostream>
#include	<sstream>
#include	<fstream>
#include	<cmath>
#include	<immintrin.h>

#ifdef USE_LIKWID
extern "C" {
#include <likwid.h>
}
#endif

#include	"Timer.h"
#include    "matrix.hpp"

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

    int nx = 0;
	int	ny = 0;
	int	c = 0;
	nx = StringTo<int>(argv[1]);
	ny = StringTo<int>(argv[2]);
	c = StringTo<int>(argv[3]);

    double *red = new double(((nx+1)*(ny+1))/2);
    double *black = new double(((nx+1)*(ny+1))/2);

    for(int i = 0; i < ((nx+1)*(ny+1))/2; ++i)
    {
        red[i] = i;
        black[i] = 1.5*i;
    }


    Matrix u (red, black, nx, ny);

    u.print("data/solution.txt");

	
	///******************************************************
	///********************** CALCULATION *******************
	///******************************************************
	double time = 0;
	
#ifdef USE_LIKWID
	likwid_markerInit();
	likwid_markerStartRegion("dummy");
#endif

	siwir::Timer	timer;

	std::cout << nx << "\t" << ny << "\t" << c <<std::endl;
    std::cout << u(2,1) << std::endl;
	time = timer.elapsed();
	std::cout << time << std::endl;

#ifdef USE_LIKWID
	likwid_markerStopRegion("dummy");
	likwid_markerClose();
#endif

	///******************************************************
	///********************** OUTPUT ************************
	///******************************************************

    free(red);
    free(black);

	};
