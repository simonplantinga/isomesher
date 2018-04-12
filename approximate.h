//############################################################################
// approximate.h
//############################################################################

#ifndef APPROXIMATE_H
#define APPROXIMATE_H

#include "octree.h"
#include "surface.h"
#include "tetramesh.h"

#include <list>

using namespace std;

// subdivision level for estimating F over an interval
const int ESTIMATE_LEVEL = 1;

class Point
{
public:
  double x, y, z;
};

class Edge
{
public:
  Edge( double x_1, double y_1, double z_1, 
        double x_2, double y_2, double z_2 )
    { x1=x_1; y1=y_1; z1=z_1; x2=x_2; y2=y_2; z2=z_2; }
  bool HasPoint( double x, double y, double z )
    {
      if (x1==x && y1==y && z1==z ) return true;
      if (x2==x && y2==y && z2==z )
        { double t; t=x1;x1=x2;x2=t; t=y1;y1=y2;y2=t; t=z1;z1=z2;z2=t;
          return true; }
      return false;
    }
  double x1, y1, z1, x2, y2, z2;
};

typedef list<Edge *> EdgeList;



//----------------------------------------------------------------------------
// store info for construction of tetrahedra
struct VertexInfo {
  // center of box
  double cx, cy, cz;
  // vertices around face
  double x[8], y[8], z[8];
  // gradienttest holds for this node
  bool gradient_ok;
};

class Approximate
{
public:
  Approximate( Surface *s );
  ~Approximate() {}
  Octree *GetOctree() { return &T; }
  double GetFmin() { return tetramesh.GetFmin(); }
  double GetFmax() { return tetramesh.GetFmax(); }

private:
  void IntervalSplit( OctNode *node, int mindepth, int maxdepth );
  bool IntervalTest( const Interval &ix, const Interval &iy, const Interval &iz );
  void GetVertex( OctNode *node, int dir1, int dir2, 
                  MeshVertex *v, double iso );
  void TriangulateCell( Octree &T, OctNode *node );
  void TetrahedrizeFace( OctNode *cnode, OctNode *node, int dir );
  void Tetrahedrize( OctNode *node );
  void TetrahedrizeLeaf( OctNode *node );
  void AddTetra( VertexInfo *vi, int p1, int p2, int p3 );
  Interval EstimateF( Interval ix, Interval iy, Interval iz );

  Octree T;
  Surface *surface;
  TetraMesh tetramesh;
};

#endif  // APPROXIMATE_H

