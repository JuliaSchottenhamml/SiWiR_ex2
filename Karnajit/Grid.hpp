#include <stdlib.h>
class Grid{
  public:
        double* data;
        int dimx,dimy;
        
        int ld;
         
        Grid(const int _dimx, const int _dimy): dimx(_dimx), dimy(_dimy), ld(_dimx+10)
        {
              data = (double*) calloc(sizeof(double) * ld * dimy);
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
    
    }
