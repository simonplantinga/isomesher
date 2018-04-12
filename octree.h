//############################################################################
// octree.h
//############################################################################

#ifndef OCTREE_H
#define OCTREE_H

#include <boost/numeric/interval.hpp>

using namespace boost;
using namespace numeric;
using namespace interval_lib;

typedef interval<double> Interval;

#include <stack>

using namespace std;

class OctNode;
class Octree;

typedef stack<OctNode *> NodeList;

//----------------------------------------------------------------------------
class MeshVertex
{
public:
  double x1, y1, z1, f1;
  double x2, y2, z2, f2;
  void GetPos( double &x, double &y, double &z, double iso )
    {
      double t = ( iso - f1 ) / ( f2 - f1 );
      x = t * ( x2 - x1 ) + x1;
      y = t * ( y2 - y1 ) + y1;
      z = t * ( z2 - z1 ) + z1;
    }
};

class MeshTriangle
{
public:
  MeshVertex v1, v2, v3;
  MeshTriangle *next;
};

//----------------------------------------------------------------------------
// Node of the octree
class OctNode
{
public:
  OctNode( OctNode *myparent );
  ~OctNode();
  void SetRange( const Interval &x, const Interval &y, const Interval &z )
    { x_range = x; y_range = y; z_range = z; }
  void GetRange( Interval &x, Interval &y, Interval &z )
    { x = x_range; y = y_range; z = z_range; }
  void SetChildType( int tp ) { child_type = tp; }
  int  GetChildType() { return child_type; }
  bool IsLeaf() { return is_leaf; }
  int  GetDepth() { return depth; }
//  void SetData( int d ) { data = d; }
//  int  GetData() { return data; }
  void Split();
  void SetGradientOk( bool b ) { gradient_ok = b; }
  bool GetGradientOk() { return gradient_ok; }
  void RemoveTriangles()
    {
      MeshTriangle *t;
      while ( first_triangle != NULL )
        {
          t = first_triangle->next;
          delete first_triangle;
          first_triangle = t;
        }
    }
  void SetEmpty( bool b ) { empty = b; }
  bool IsEmpty() { return empty; }
  void DrawBox();
private:
  OctNode  *parent;
  int       child_type;
  OctNode  *child[8];   // index using bits: 0zyx
  bool      is_leaf;
  int       depth;
  Interval  x_range;
  Interval  y_range;
  Interval  z_range;
//  int       data;
  bool      empty;      // true if surface outside cell
  bool      gradient_ok;

public:
  MeshTriangle *first_triangle;

  friend class Octree;
};

//----------------------------------------------------------------------------
class Octree
{
public:
  Octree();
  Octree( const Interval &x, const Interval &y, const Interval &z );
  ~Octree();
  void SetRange( const Interval &x, const Interval &y, const Interval &z );
  void GetRange( Interval &x, Interval &y, Interval &z );
  void Balance();
  void SplitNode( OctNode *node );
  OctNode *GetRoot();
  OctNode *GetChild( OctNode *node, int childtype );
  OctNode *GetParent( OctNode *node );
  OctNode *GetNeighbour( OctNode *node, int direction );
  OctNode *GetCorner( OctNode *node, int direction );
  int NumLeaves();
private:
  void InsertLeaves( NodeList *l, OctNode *n );

  OctNode *root;
  int num_leaves;
};

#endif  // OCTREE_H

