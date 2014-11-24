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
	Grid(const int _sizeX, const int _sizeY) : sizeX(_sizeX), sizeY(_sizeY), ld(_sizeX+10){
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
		return data[y * ld + x];
	}
	
	inline
	const double& operator()(const int x, const int y) const {
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
