#include <cstdlib>  
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "Grid.hpp"


static const int	ALIGNMENT = 0x40;   
const double PI =  3.1415926535897932384626433832795;
const double k =  4 * M_PI * M_PI;


using namespace std;

double cal_fx(double x, double y)
{
    return 4.0*PI*PI* (sinh(2.0*M_PI*y)) * (sin(2.0*M_PI*x));
}

double cal_bndry(double x, double y)
{
        return sin(2.0*M_PI*x)*sinh(2.0*M_PI*y);
}

void red_black_GS( Grid gRed, Grid gBlack, Grid fRed, Grid fBlack, int nx, int ny, int iters)
{ 
    double a, b, c;  
    
    double fx, bx, ;
    double hx,hy;
    int tot_dim = (nx+1)*(ny+1);
    
    hx = 2/nx;
    hy = 1/ny;    
          
    b = 1.0/(hx*hx);
    c = 1.0/ (hy * hy);
    a= 1.0/(2*b)+(2*c)+ k * k;
   
   // std::cout << " A22";  
   #pragma omp parralel for
   for(int i=0;i<ny+1;i++)
   { 
        for(int j=i&0x1; j< nx+2 ;j+=2)
        {
            if(i==ny)
            gRed(j,i) = cal_bndry(j*hx, i*hy);
             else
            gRed(j,i) = 0;
        }   
        
    }
    
      #pragma omp parralel for
   for(int i=0;i<ny+1;i++)
   { 
        for(int j=i&0x1; j< nx+2 ;j+=2)
        {
             if(i==ny)
            gBlack(j,i) = cal_bndry(j*hx, i*hy);
             else
            gBlack(j,i) = 0;            
        }   
        
    }
    
       // std::cout << " A22";  
   #pragma omp parralel for
   for(int i=0;i<ny+1;i++)
   { 
        for(int j=i&0x1; j< nx+2 ;j+=2)
        {
            fRed(j,i) = cal_fx(j*hx, i*hy);
        }   
        
    }
    
      #pragma omp parralel for
   for(int i=0;i<ny+1;i++)
   { 
        for(int j=i&0x1; j< nx+2 ;j+=2)
        {
            fBlack(j,i) = cal_fx(j*hx, i*hy);
        }   
        
    }
  
   for(int k=0; k< iters; k++)
   { 
        #pragma omp parralel for
     for(int y=1;y<ny+1;y++)
        { 
        for(int x=i&0x1; x< nx+2 ;x+=2)
        {
            gRed(x,y) = a*(fRed(x,y)+ b*( gBlack(x+1,y) + gBlack(x-1,y)) + c* ( gBlack(x,y+1) + gBlack(x,y-1));            
        }        
    } 
    
    #pragma omp parralel for
    for(int y=1;y<ny+1;y++)
        { 
        for(int x=i&0x1; x< nx+2 ;x+=2)
        {
            gBlack(x,y) = a*(fBlack(x,y)+ b*( gRed(x+1,y) + gRed(x-1,y)) + c* ( gRed(x,y+1) + gRed(x,y-1));
        }        
    }   
   }   
    
}



int main(int argc, char *argv[])
{
    int nx, ny;
   int iters;
   //double hx, hy;
   //std::cout << "11 ";
   double time =0;
   
   
   if ( argc < 2)
   {
        std::cout << "Invalid number of parametrs.. Program exiting" ;
        
        exit(1);
        }
   
   nx=atoi(argv[1]);
   ny=atoi(argv[2]);
   iters = atoi(argv[3]);
   int tot_dim = (nx+1)*(ny+1);
   Grid gRed((nx/2)+1,(ny+1);
   Grid gBlack((nx/2)+1,(ny+1);
   //Grid gRed((nx/2)+1,(ny+1);
   //Grid gBlack((nx/2)+1,(ny+1);
   //std::cout << "\n" << tot_dim << "\n";    
 #ifdef USE_LIKWID
	likwid_markerInit();
	likwid_markerStartRegion("dummy");
#endif

siwir::Timer	timer;
     
   red_black_GS(gRed,gBlack,nx,ny,iters);
   
   time = timer.elapsed();
	std::cout << "time," << time << std::endl;

#ifdef USE_LIKWID
	likwid_markerStopRegion("dummy");
	likwid_markerClose();
#endif
   
   
   
     for(int i = 0; i< tot_dim; i++)
    { 
        for(int j=0; j< tot_dim; j++)
        {
            std::cout << mat[i][j] << "    ";
        }
        std::cout << b[i] << "\n";
    }
   
   //Matrix A = loadMatrix(mat);
   
   //redBlackGaussSeidel(A);
    
   return 0;   
}

