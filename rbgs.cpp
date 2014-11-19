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
	if (getenv("OMP_NUM_THREADS")){
		std::cout << "OMP_NUM_THREADS," << getenv("OMP_NUM_THREADS") << std::endl;
	}
	if (getenv("GOMP_CPU_AFFINITY")){
		std::cout << "GOMP_CPU_AFFINITY," << getenv("GOMP_CPU_AFFINITY") << std::endl;
	}
	//std::cout << "numThreads," << omp_get_num_threads() << std::endl;

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
	
    /*
	#pragma omp parallel for
	for (int y = 0; y < ny+1; ++y){
		for (int x = (y&0x1); x < nx+2; x+=2){
			if (y == ny){
				uRed(x,y) = border( x*hx, y*hy);
			} else {
				uRed(x,y) = 0;
			}
		}
    }#
    */
	
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


    #pragma omp parallel for
    for (int y = 1; y < ny; y+=2){
        for (int x = 1; x < nx; x+=4){
            if(y == ny)
            {
                if (y == ny){
                    uRed(x,y) = border( x*hx, y*hy);
                } else {
                    uRed(x,y) = 0;
                }
            }
            /*
            //uRed(x,y) = preF * (fRed(x,y) + invHx2 * (uBlack(x-1,y) + uBlack(x+1,y)) + invHy2 * (uBlack(x, y-1) + uBlack(x,y+1)) );
            //std::cout << "A" << std::endl;
            __m128d*	FRed = (__m128d*) &fRed(x,y);
            //std::cout << "B" << std::endl;
            __m128d*	north = (__m128d*) &uBlack(x,y+1);
            //std::cout << "C" << std::endl;
            __m128d*	west = (__m128d*) (&uBlack(x-1,y));
            //std::cout << "D" << std::endl;
            __m128d*	south = (__m128d*) &uBlack(x,y-1);
            //std::cout << "E" << std::endl;
            __m128d	east = _mm_loadu_pd(&uBlack(x+1,y));
            //std::cout << x << "\t" << y << "\t" << &uRed(x,y) << "\t" << west << "\t" << east << std::endl;
            _mm_stream_pd(&uRed(x,y), PreF * ((*FRed) + InvHx2 * ((*west) + (east)) + InvHy2 * ((*north) + (*south))) );
            //_mm_stream_pd*/
        }

        for (int x = 0; x < nx; x+=4){
            if(y == ny)
            {
                if (y == ny){
                    uRed(x,y) = border( x*hx, y*hy);
                } else {
                    uRed(x,y) = 0;
                }
            }
            /*
            //uRed(x,y) = preF * (fRed(x,y) + invHx2 * (uBlack(x-1,y) + uBlack(x+1,y)) + invHy2 * (uBlack(x, y-1) + uBlack(x,y+1)) );
            //std::cout << "A" << std::endl;
            __m128d*	FRed = (__m128d*) &fRed(x,y+1);
            //std::cout << "B" << std::endl;
            __m128d*	north = (__m128d*) &uBlack(x,y+2);
            //std::cout << "C" << std::endl;
            __m128d	west = _mm_loadu_pd (&uBlack(x-1,y+1));
            //std::cout << "D" << std::endl;
            __m128d*	south = (__m128d*) &uBlack(x,y);
            //std::cout << "E" << std::endl;
            __m128d*	east = (__m128d*) (&uBlack(x+1,y+1));
            //std::cout << x << "\t" << y << "\t" << &uRed(x,y) << "\t" << west << "\t" << east << std::endl;
            _mm_stream_pd(&uRed(x,y+1), PreF * ((*FRed) + InvHx2 * ((west) + (*east)) + InvHy2 * ((*north) + (*south))) );
            //_mm_stream_pd*/
        }
    }

	//calc u
	
	__m128d	PreF = _mm_set_pd(preF, preF);
	__m128d	InvHx2 = _mm_set_pd(invHx2, invHx2);
	__m128d	InvHy2 = _mm_set_pd(invHy2, invHy2);
	
	//std::cout << 0 << "\t" << 0 << "\t" << &uRed(0,0) << std::endl;
	//std::cout << 2 << "\t" << 0 << "\t" << &uRed(2,0) << std::endl;
	//std::cout << 0 << "\t" << 1 << "\t" << &uRed(0,1) << std::endl;
	
	for (int runs = 0; runs<c; ++runs){
		#pragma omp parallel for
		for (int y = 1; y < ny; y+=2){
			for (int x = 1; x < nx; x+=4){
				//uRed(x,y) = preF * (fRed(x,y) + invHx2 * (uBlack(x-1,y) + uBlack(x+1,y)) + invHy2 * (uBlack(x, y-1) + uBlack(x,y+1)) );
				//std::cout << "A" << std::endl;
				__m128d*	FRed = (__m128d*) &fRed(x,y);
				//std::cout << "B" << std::endl;
				__m128d*	north = (__m128d*) &uBlack(x,y+1);
				//std::cout << "C" << std::endl;
				__m128d*	west = (__m128d*) (&uBlack(x-1,y));
				//std::cout << "D" << std::endl;
				__m128d*	south = (__m128d*) &uBlack(x,y-1);
				//std::cout << "E" << std::endl;
				__m128d	east = _mm_loadu_pd(&uBlack(x+1,y));
				//std::cout << x << "\t" << y << "\t" << &uRed(x,y) << "\t" << west << "\t" << east << std::endl;
				_mm_stream_pd(&uRed(x,y), PreF * ((*FRed) + InvHx2 * ((*west) + (east)) + InvHy2 * ((*north) + (*south))) );
				//_mm_stream_pd*/
			}
			uRed(0, y) = 0;
			uRed(nx, y) = 0;
			
			for (int x = 0; x < nx; x+=4){
				//uRed(x,y) = preF * (fRed(x,y) + invHx2 * (uBlack(x-1,y) + uBlack(x+1,y)) + invHy2 * (uBlack(x, y-1) + uBlack(x,y+1)) );
				//std::cout << "A" << std::endl;
				__m128d*	FRed = (__m128d*) &fRed(x,y+1);
				//std::cout << "B" << std::endl;
				__m128d*	north = (__m128d*) &uBlack(x,y+2);
				//std::cout << "C" << std::endl;
				__m128d	west = _mm_loadu_pd (&uBlack(x-1,y+1));
				//std::cout << "D" << std::endl;
				__m128d*	south = (__m128d*) &uBlack(x,y);
				//std::cout << "E" << std::endl;
				__m128d*	east = (__m128d*) (&uBlack(x+1,y+1));
				//std::cout << x << "\t" << y << "\t" << &uRed(x,y) << "\t" << west << "\t" << east << std::endl;
				_mm_stream_pd(&uRed(x,y+1), PreF * ((*FRed) + InvHx2 * ((west) + (*east)) + InvHy2 * ((*north) + (*south))) );
				//_mm_stream_pd*/
			}
			uRed(0, y+1) = 0;
			uRed(nx, y+1) = 0;
		}
		
		#pragma omp parallel for
		for (int y = 1; y < ny; ++y){
			for (int x = (y&0x1)+1; x < nx; x+=2){
				uBlack(x,y) = preF * (fBlack(x,y) + invHx2 * (uRed(x-1,y) + uRed(x+1,y)) + invHy2 * (uRed(x, y-1) + uRed(x,y+1)) );
			}
			uBlack(nx, y) = 0;
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
