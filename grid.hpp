#ifndef	GRID_HPP_INCLUDED
#define	GRID_HPP_INCLUDED

#include	<stdlib.h>
#include	<malloc.h>
#include	<iostream>

static const int ALIGNMENT = 0x100;

class	Grid{
public:
	double*		data;
	
	int	sizeX;
	int	sizeY;

	int	ld;

	///basic constructor
	Grid(const int _sizeX, const int _sizeY) : sizeX(_sizeX), sizeY(_sizeY), ld((_sizeX+10)&0xFFFFFFFE){
		data = (double*) memalign(ALIGNMENT, sizeof(double) * ld * sizeY);
		std::cout << ld << "\t" << data << std::endl;
		//data = (double*) malloc(sizeof(double) * ld * sizeY);
	}
	~Grid(){
		free(data);
	}

	inline
	double& operator()(const int x, const int y) {
		return data[y * ld + (x>>1)];
	}
	
	inline
	const double& operator()(const int x, const int y) const {
		return data[y * ld + (x>>1)];
	}
};

#endif
