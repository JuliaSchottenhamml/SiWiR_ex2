#ifndef	MATRIX_HPP_INCLUDED
#define	MATRIX_HPP_INCLUDED

#include	<immintrin.h>
#include <iostream>
#include <fstream>

class	Matrix{
public:
    double*		red;
    double*		black;
	
	//matrix dimensions
	int	dimRows;
	int	dimCols;

    //mesh size
    double hx;
    double hy;

    Matrix(double*	redData, double* blackData, const int nx, const int ny) : red(redData), black(blackData), dimRows(nx+1), dimCols(ny+1), hx(2.0/nx), hy(1.0/ny){
	}

	
	inline
	double& operator()(const int row, const int col) {
        if((row*col)%2 == 0)
        {
            return red[(row*dimCols+col)/2];
        }
        else
        {
            return black[(row*dimCols+col)/2];
        }
	}
	
	inline
	const double& operator()(const int row, const int col) const {
        if((row*col)%2 == 0)
        {
            return red[(row*dimCols+col)/2];
        }
        else
        {
            return black[(row*dimCols+col)/2];
        }
	}

	inline
    int getDimRows() const {
		return dimRows;
	}
	inline
    int getDimCols() const {
		return dimCols;
	}

    inline
    void print(std::string path)
    {
        std::ofstream solution;
        solution.open(path, std::ios_base::trunc);
        solution << "#X \t Y \t Z \n\n";
        for(int row = 0; row < dimRows; ++row)
        {
            for(int col = 0; col < dimCols; ++col)
            {
                double val = 0;
                if((row*col)%2 == 0)
                {
                    val = red[(row*dimCols+col)/2];
                }
                else
                {
                    val = black[(row*dimCols+col)/2];
                }
                solution << row*hx << "\t" << col*hy << "\t" << val << std::endl;
            }
            solution << "\n";
        }
        solution.close();
    }
};

#endif
