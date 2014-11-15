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
#include	"grid.hpp"

static const double k = 2*M_PI;

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

constexpr	double f(const double x, const double y){
	return 4.0*M_PI*M_PI*sin(2.0*M_PI*x)*sinh(2.0*M_PI*y);
}

constexpr	double border(const double x, const double y){
	return sin(2.0*M_PI*x)*sinh(2.0*M_PI*y);
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
	
	double	hx = 2.0/nx;
	double	hy = 1.0/ny;
	
	double	invHx2 = 1.0/hx/hx;
	double	invHy2 = 1.0/hy/hy;
	
	double	preF = 1.0 / (2*invHx2 + 2*invHy2 + k*k);
	
	//init f
	
	Grid	fRed( (nx+2)/2, (ny+1) );
	Grid	fBlack( (nx+2)/2, (ny+1) );
	
	#pragma omp parallel for
	for (int y = 0; y < ny+1; ++y){
		for (int x = (y&0x1); x < nx+2; x+=2){
			fRed(x,y) = f( x*hx, y*hy );
		}
	}
	
	#pragma omp parallel for
	for (int y = 0; y < ny+1; ++y){
		for (int x = ((y&0x1)^0x1); x < nx+2; x+=2){
			fBlack(x,y) = f( x*hx, y*hy );
		}
	}
	
	//init u
	
	Grid	uRed( (nx+2)/2, (ny+1) );
	Grid	uBlack( (nx+2)/2, (ny+1) );
	
	#pragma omp parallel for
	for (int y = 0; y < ny+1; ++y){
		for (int x = (y&0x1); x < nx+2; x+=2){
			if (y == ny){
				uRed(x,y) = border( x*hx, y*hy);
			} else {
				uRed(x,y) = 0;
			}
		}
	}
	
	#pragma omp parallel for
	for (int y = 0; y < ny+1; ++y){
		for (int x = ((y&0x1)^0x1); x < nx+2; x+=2){
			if (y == ny){
				uBlack(x,y) = border( x*hx, y*hy);
			} else {
				uBlack(x,y) = 0;
			}
		}
	}
	
	//calc u
	
	for (int runs = 0; runs<c; ++runs){
		#pragma omp parallel for
		for (int y = 1; y < ny; ++y){
			for (int x = ((y&0x1)^0x1)+1; x < nx; x+=2){
				uRed(x,y) = preF * (fRed(x,y) + invHx2 * (uBlack(x-1,y) + uBlack(x+1,y)) + invHy2 * (uBlack(x, y-1) + uBlack(x,y+1)) );
			}
		}
		
		#pragma omp parallel for
		for (int y = 1; y < ny; ++y){
			for (int x = (y&0x1)+1; x < nx; x+=2){
				uBlack(x,y) = preF * (fBlack(x,y) + invHx2 * (uRed(x-1,y) + uRed(x+1,y)) + invHy2 * (uRed(x, y-1) + uRed(x,y+1)) );
			}
		}
	}

	time = timer.elapsed();
	std::cout << "time," << time << std::endl;

#ifdef USE_LIKWID
	likwid_markerStopRegion("dummy");
	likwid_markerClose();
#endif

	///******************************************************
	///********************** OUTPUT ************************
	///******************************************************
	
	double	r = 0;
	double	r2 = 0;
	
	for (int y = 1; y < ny; ++y){
		for (int x = ((y&0x1)^0x1)+1; x < nx; x+=2){
			r = (fRed(x,y) + invHx2 * (uBlack(x-1,y) + uBlack(x+1,y)) + invHy2 * (uBlack(x, y-1) + uBlack(x,y+1)) ) - uRed(x,y)/preF;
			r2 += r*r;
		}
	}
	
	for (int y = 1; y < ny; ++y){
		for (int x = (y&0x1)+1; x < nx; x+=2){
			r = (fBlack(x,y) + invHx2 * (uRed(x-1,y) + uRed(x+1,y)) + invHy2 * (uRed(x, y-1) + uRed(x,y+1)) ) - uBlack(x,y)/preF;
			r2 += r*r;
		}
	}
	
	r2 = r2 / (nx-1) / (ny-1);
	r2 = sqrt(r2);
	
	std::cout << "residuum," << r2 << std::endl;
	
	std::ofstream	fOut("data/solution.txt");
	for (int y = 0; y < ny+1; ++y){
		for (int x = 0; x < nx+1; ++x){
			if (y%2 == 0){
				if (x%2 == 0){
					fOut << x*hx << "\t" << y*hy << "\t" << uRed(x,y) << std::endl;
				} else {
					fOut << x*hx << "\t" << y*hy << "\t" << uBlack(x,y) << std::endl;
				}
			} else {
				if (x%2 == 1){
					fOut << x*hx << "\t" << y*hy << "\t" << uRed(x,y) << std::endl;
				} else {
					fOut << x*hx << "\t" << y*hy << "\t" << uBlack(x,y) << std::endl;
				}			
			}
		}
		fOut << std::endl;
	}
	fOut.close();
};
