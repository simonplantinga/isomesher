//############################################################################
// tetramesh.h
//############################################################################

#include "surface.h"

class TVertex 
{
public:
  double x, y, z;
  double F;
};

//----------------------------------------------------------------------------
// triangle
class Triangle
{
public:
  TVertex p1[3], p2[3];   // vertex i=0..2 on edge p1[i]-p2[i]
};

//----------------------------------------------------------------------------
class Tetrahedron
{
public:
  Tetrahedron() { num_triangles = 0; }
  void ReMesh( double iso );
  void Draw( double iso );
  void SetGradient( double gx, double gy, double gz )
    { nx = gx; ny = gy; nz = gz; }

  TVertex vertex[4];
  Interval frange;
  bool gradient_ok;
  double nx, ny, nz;  // gradient

  int num_triangles;
  Triangle triangle[2];
  Tetrahedron *next;
};

//----------------------------------------------------------------------------
class TetraMesh
{
public:
  TetraMesh() { num_tetra = 0; tetralist = NULL; }

  void AddTetra( double x1, double y1, double z1, double f1,
                 double x2, double y2, double z2, double f2,
                 double x3, double y3, double z3, double f3,
                 double x4, double y4, double z4, double f4,
                 Interval frange, bool gradient_ok );
  int NumTetra() { return num_tetra; }
  void Draw();
  void RemeshAll(double iso) { Tetrahedron *t = tetralist; 
      while (t!=NULL) {t->ReMesh(iso); t=t->next;} }
  void ComputeRange( Surface *surface );
  void ComputeGradients( Surface *surface );
  void NewIso( double iso );
  double GetFmin() { return Fmin; }
  double GetFmax() { return Fmax; }
    
private:
  Tetrahedron *tetralist;
  int num_tetra;
  double isovalue;
  double Fmin, Fmax;
};

