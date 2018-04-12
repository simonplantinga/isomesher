//############################################################################
// testfunc.h
//############################################################################
#ifndef TESTFUNC_H
#define TESTFUNC_H

#include "surface.h"


#define FUNCTION_1
//#define FUNCTION_2
//#define FUNCTION_3
//#define FUNCTION_4
//#define FUNCTION_5

//############################################################################
// Class for an implicit surface (tangle cube)
//############################################################################

#ifdef FUNCTION_1
//----------------------------------------------------------------------------
// tangle-cube: x^4 - 5*x^2 + y^4 - 5*y^2 + z^4 - 5*z^2 + 10
class TestFunc : public Surface
{
public:
  TestFunc() {}
  ~TestFunc() {}  

  // compute function value or derivative at (x,y,z)
  double F(double x, double y, double z)
  { 
    double xs = x*x;
    double ys = y*y;
    double zs = z*z;
    return xs*xs - 5*xs + ys*ys - 5*ys + zs*zs - 5*zs + 10; 
  }

  double Fx(double x, double, double)
  { return 4*x*x*x - 10*x; }

  double Fy(double, double y, double)
  { return 4*y*y*y - 10*y; }

  double Fz(double, double, double z)
  { return 4*z*z*z - 10*z; }

  // compute function value or derivative over 3D interval
  Interval F(Interval x, Interval y, Interval z)
  { 
    Interval xs = x*x;
    Interval ys = y*y;
    Interval zs = z*z;
    return xs*xs - 5*xs + ys*ys - 5*ys + zs*zs - 5*zs + 10; 
  }

  Interval Fx(Interval x, Interval, Interval)
  { return 4*x*x*x - 10*x; }

  Interval Fy(Interval, Interval y, Interval)
  { return 4*y*y*y - 10*y; }

  Interval Fz(Interval, Interval, Interval z)
  { return 4*z*z*z - 10*z; }

  // return bounding box for the curve
  //
  //
  double MinX() { return -3; }  
  double MaxX() { return  3; }
  double MinY() { return -3; }
  double MaxY() { return  3; }
  double MinZ() { return -3; }
  double MaxZ() { return  3; }
  /*

  double MinX() { return  1.5; }  
  double MaxX() { return  4; }
  double MinY() { return  1.5; }
  double MaxY() { return  4; }
  double MinZ() { return  1.5; }
  double MaxZ() { return  4; }
  */
};
#endif  // FUNCTION_1

//############################################################################

#ifdef FUNCTION_2
//----------------------------------------------------------------------------
// chair
// (x^2+y^2+z^2-a*k^2)^2-b*((z-k)^2-2*x^2)*((z+k)^2-2*y^2)=0
// with k=5, a=0.95 and b=0.8
#define K 5.00
#define A 0.95
#define B 0.80
class TestFunc : public Surface
{
public:
  TestFunc() {}
  ~TestFunc() {}

  double F(double x, double y, double z)
  {
    double t = x*x + y*y + z*z - A*K*K;
    return t*t - B*((z-K)*(z-K)-2*x*x)*((z+K)*(z+K)-2*y*y);
  }
  double Fx(double x, double y, double z)
  {
    return 4*x*(x*x+y*y+z*z-A*K*K)+4*B*x*((z+K)*(z+K)-2*y*y);
  }
  double Fy(double x, double y, double z)
  {
    return 4*y*(x*x+y*y+z*z-A*K*K)+4*B*y*((z-K)*(z-K)-2*x*x);
  }
  double Fz(double x, double y, double z)
  {
    return 4*z*(x*x+y*y+z*z-A*K*K)
            -2*B*(z-K)*((z+K)*(z+K)-2*y*y)
            -2*B*(z+K)*((z-K)*(z-K)-2*x*x);
  }

  Interval F(Interval x, Interval y, Interval z)
  {
    Interval t = sqr(x) + sqr(y) + sqr(z) - A*K*K;
    return sqr(t) - B*(sqr(z-K)-2*sqr(x))*(sqr(z+K)-2*sqr(y));
  }
  Interval Fx(Interval x, Interval y, Interval z)
  {
    return 4*x*(sqr(x)+sqr(y)+sqr(z)-A*K*K)+4*B*x*(sqr(z+K)-2*sqr(y));
  }
  Interval Fy(Interval x, Interval y, Interval z)
  {
    return 4*y*(sqr(x)+sqr(y)+sqr(z)-A*K*K)+4*B*y*(sqr(z-K)-2*sqr(x));
  }
  Interval Fz(Interval x, Interval y, Interval z)
  {
    return 4*z*(sqr(x)+sqr(y)+sqr(z)-A*K*K)
            -2*B*(z-K)*(sqr(z+K)-2*sqr(y))
            -2*B*(z+K)*(sqr(z-K)-2*sqr(x));
  }

  double MinX() { return -5; }  
  double MaxX() { return  5; }
  double MinY() { return -5; }
  double MaxY() { return  5; }
  double MinZ() { return -5; }
  double MaxZ() { return  5; }
/*
  double MinX() { return -5; }  
  double MaxX() { return  5; }
  double MinY() { return  0; }
  double MaxY() { return 10; }
  double MinZ() { return -1; }
  double MaxZ() { return  9; }
*/
};

#endif  // FUNCTION_2

//############################################################################

#ifdef FUNCTION_3
//----------------------------------------------------------------------------
// Heart
// (2*x^2+y^2+z^2-1)^3-(1/10)*x^2*z^3-y^2*z^3-0.001 = 0 
class TestFunc : public Surface
{
public:
  TestFunc() {}
  ~TestFunc() {}

  double F(double x, double y, double z)
  {
    double t = 2*x*x+y*y+z*z-1;
    return t*t*t - 0.1*x*x*z*z*z-y*y*z*z*z-0.001;
  }
  double Fx(double x, double y, double z)
  {
    double t = 2*x*x+y*y+z*z-1;
    return 12*x*t*t-0.2*x*z*z*z;
  }
  double Fy(double x, double y, double z)
  {
    double t = 2*x*x+y*y+z*z-1;
    return 6*y*t*t-2*y*z*z*z;
  }
  double Fz(double x, double y, double z)
  {
    double t = 2*x*x+y*y+z*z-1;
    return 6*z*t*t-0.3*x*x*z*z-3*y*y*z*z;
  }

  Interval F(Interval x, Interval y, Interval z)
  {
    Interval t = 2*sqr(x)+sqr(y)+sqr(z)-1;
    return t*sqr(t) - 0.1*sqr(x)*z*sqr(z)-sqr(y)*z*sqr(z)-0.001;
  }
  Interval Fx(Interval x, Interval y, Interval z)
  {
    Interval t = 2*sqr(x)+sqr(y)+sqr(z)-1;
    return 12*x*sqr(t)-0.2*x*z*sqr(z);
  }
  Interval Fy(Interval x, Interval y, Interval z)
  {
    Interval t = 2*sqr(x)+sqr(y)+sqr(z)-1;
    return 6*y*sqr(t)-2*y*z*sqr(z);

  }
  Interval Fz(Interval x, Interval y, Interval z)
  {
    Interval t = 2*sqr(x)+sqr(y)+sqr(z)-1;
    return 6*z*sqr(t)-0.3*sqr(x)*sqr(z)-3*sqr(y)*sqr(z);
  }

  double MinX() { return -2; }  
  double MaxX() { return  2; }
  double MinY() { return -2; }
  double MaxY() { return  2; }
  double MinZ() { return -2; }
  double MaxZ() { return  2; }
/*
  double MinX() { return  0.4; }    // 0.3    0.4
  double MaxX() { return  0.7; }    // 0.8    0.8
  double MinY() { return -0.1; }    //-0.1   -0.1 
  double MaxY() { return  0.2; }    // 0.4    0.3
  double MinZ() { return  0.4; }    // 0.2    0.3
  double MaxZ() { return  0.7; }    // 0.7    0.7
*/
};

#endif  // FUNCTION_3

//############################################################################
#ifdef FUNCTION_4
//----------------------------------------------------------------------------
// non-algebraic surface
// -0.4*(sin(5x)+sin(5y)+cos(5z)) + 0.1x^2 + 0.3y^2 + 0.2z^2 - 0.5
class TestFunc : public Surface
{
public:
  TestFunc() {}
  ~TestFunc() {}

  double F(double x, double y, double z)
  {
    return -0.4*(sin(5*x)+sin(5*y)+cos(5*z))+0.1*x*x+0.3*y*y+0.2*z*z-0.5;
  }
  double Fx(double x, double y, double z)
  {
    return -2*cos(5*x)+0.2*x;
  }
  double Fy(double x, double y, double z)
  {
    return -2*cos(5*y)+0.6*y;
  }
  double Fz(double x, double y, double z)
  {
    return 2*sin(5*z)+0.4*x;
  }
  Interval F(Interval x, Interval y, Interval z)
  {
    return -0.4*(sin(5*x)+sin(5*y)+cos(5*z))+0.1*sqr(x)+0.3*sqr(y)+0.2*sqr(z)-0.5;
  }
  Interval Fx(Interval x, Interval y, Interval z)
  {
    return -2*cos(5*x)+0.2*x;
  }
  Interval Fy(Interval x, Interval y, Interval z)
  {
    return -2*cos(5*y)+0.6*y;
  }
  Interval Fz(Interval x, Interval y, Interval z)
  {
    return 2*sin(5*z)+0.4*x;
  }
  /* old BB: -5 .. 5 */
  double MinX() { return 1; }  
  double MaxX() { return 5; }
  double MinY() { return 1; }
  double MaxY() { return 5; }
  double MinZ() { return 1; }
  double MaxZ() { return 5; }
};

#endif  // FUNCTION_4

//############################################################################
#ifdef FUNCTION_5
//----------------------------------------------------------------------------
// sphere
class TestFunc : public Surface
{
public:
  TestFunc() {}
  ~TestFunc() {}

  double F(double x, double y, double z)
  {
    return x*x+y*y+z*z-1;
  }
  double Fx(double x, double y, double z)
  {
    return -2*x;
  }
  double Fy(double x, double y, double z)
  {
    return -2*y;
  }
  double Fz(double x, double y, double z)
  {
    return 2*z;
  }
  Interval F(Interval x, Interval y, Interval z)
  {
    return x*x+y*y+z*z-1;
  }
  Interval Fx(Interval x, Interval y, Interval z)
  {
    return 2*x;
  }
  Interval Fy(Interval x, Interval y, Interval z)
  {
    return 2*y;
  }
  Interval Fz(Interval x, Interval y, Interval z)
  {
    return 2*z;
  }
  double MinX() { return -1; }  
  double MaxX() { return  1; }
  double MinY() { return -1; }
  double MaxY() { return  1; }
  double MinZ() { return -1; }
  double MaxZ() { return  1; }
};

#endif  // FUNCTION_5


#endif  // TESTFUNC_H
