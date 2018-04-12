//############################################################################
// surface.h
//############################################################################
#ifndef SURFACE_H
#define SURFACE_H

#include <boost/numeric/interval.hpp>

using namespace boost;
using namespace numeric;
using namespace interval_lib;

typedef interval<double> Interval;

//############################################################################
// Abstract class for an implicit surface
//############################################################################
class Surface
{
public:
  Surface() {}            // create surface 
  virtual ~Surface() {}   // destroy surface

  // compute function value or derivative at (x,y)
  virtual double F(double x, double y, double z) = 0;
  virtual double Fx(double x, double y, double z) = 0;
  virtual double Fy(double x, double y, double z) = 0;
  virtual double Fz(double x, double y, double z) = 0;

  // compute function value or derivative over 3D interval
  virtual Interval F(Interval x, Interval y, Interval z) = 0;
  virtual Interval Fx(Interval x, Interval y, Interval z) = 0;
  virtual Interval Fy(Interval x, Interval y, Interval z) = 0;
  virtual Interval Fz(Interval x, Interval y, Interval z) = 0;

  // return bounding box for the surface
  virtual double MinX() = 0;
  virtual double MaxX() = 0;
  virtual double MinY() = 0;
  virtual double MaxY() = 0;
  virtual double MinZ() = 0;
  virtual double MaxZ() = 0;
};

#endif  // SURFACE_H
//############################################################################

