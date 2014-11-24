#ifndef	GRID_HPP_INCLUDED
#define	GRID_HPP_INCLUDED

#include	<malloc.h>
#include	<iostream>
#include	<cstring>

static const int ALIGNMENT = 0x10;

class	Grid{
public:
	double* __restrict__ data;
	
	int	sizeX;
	int	sizeY;

	int	ld;

	///basic constructor
	Grid(const int _sizeX, const int _sizeY) : sizeX(_sizeX), sizeY(_sizeY + 1), ld( (_sizeX&0xFFFFFFFE)+10){
		//std::cout << ld << "\t" << ld * sizeof(double) << std::endl;
		data = (double*) memalign(ALIGNMENT, sizeof(double) * ld * sizeY);
		memset(data, 0, sizeof(double) * ld * sizeY);
		++data;
	}
	~Grid(){
		--data;
		free(data);
	}

	inline
	double& operator()(const int x, const int y) {
		//std::cout << x << "\t" << y << std::endl;
		return data[y * ld + x];
	}
	
	inline
	const double& operator()(const int x, const int y) const {
		//std::cout << x << "\t" << y << std::endl;
		return data[y * ld + x];
	}
	
	inline
	double& get(int x, int y) {
		++x;
		x /= 2;
		return data[y * ld + x];
	}
	
	inline
	const double& get(int x, int y) const {
		++x;
		x /= 2;
		return data[y * ld + x];
	}
};

#endif
