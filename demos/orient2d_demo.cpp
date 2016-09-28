
#include <cmath>
#include <cfloat>

#include <predicates.hpp>
#include "write_png.h"

#include<stdio.h>
#include<simd/simd.h>

#include <CoreServices/CoreServices.h>


using namespace predicates;

int main()
{
  const double x1[] = {0.5, 0.5};
  const double x2[] = {12.0, 12.0};
  const double x3[] = {24.0, 24.0};
  
  
  // const double x1[] = {0.5, 0.5};
  // const double x2[] = {-100, -100};
  // const double x3[] = {100, 100};
  
  const vector_double2 _x2 = {x2[0], x2[1]};
  const vector_double2 _x3 = {x3[0], x3[1]};

  const size_t nx = 256, ny = 256;

  // auto predicate1 = [&](const double * x) { return orient2d(x,  x2, x3); };
  // auto predicate2 = [&](const double * x) { return orient2dfast(x, x2,  x3); };
  // auto predicate3 = [&](const double * x) {
  //   const vector_double2 _x = {x[0], x[1]};
  //
  //   return simd_orient(_x2-_x, _x3-_x);
  //   //return orient2d(x, x2, x3 );
  // };

  auto predicate1 = [&](const double * x) { return orient2d( x, x2, x3); };
  auto predicate2 = [&](const double * x) { return orient2dfast(x, x2, x3); };
  auto predicate3 = [&](const double * x) { 
    const vector_double2 _x = {x[0], x[1]};
    
    return simd_orient(_x, _x2, _x3);
    //return orient2d(x, x2, x3 ); 
    

  };


  double A[nx * ny];

  auto callback =
    [&](const double p, const double *, const size_t i, const size_t j)
    {
      A[nx * i + j] = p;
    };

    double t0 = CFAbsoluteTimeGetCurrent();
    perturb2d(predicate1, x1, nx, ny, callback);
  	double elapsed = CFAbsoluteTimeGetCurrent() - t0;
  	printf("Elapsed time for predicate1=%fms\n", elapsed*1000);
    
    matrix_to_png("orientation1.png", nx, ny, A);

  	t0 = CFAbsoluteTimeGetCurrent();
    perturb2d(predicate2, x1, nx, ny, callback);
  	elapsed = CFAbsoluteTimeGetCurrent() - t0;
  	printf("Elapsed time for predicate2=%fms\n", elapsed*1000);
	
    matrix_to_png("orientation2.png", nx, ny, A);

  	t0 = CFAbsoluteTimeGetCurrent();
    perturb2d(predicate3, x1, nx, ny, callback);
  	elapsed = CFAbsoluteTimeGetCurrent() - t0;
  	printf("Elapsed time for predicate3=%fms\n", elapsed*1000);
	
    matrix_to_png("orientation3.png", nx, ny, A);

  return 0;
}
