//############################################################################
// approximate.cpp
//############################################################################

#include "approximate.h"

//----------------------------------------------------------------------------
Approximate::Approximate( Surface *s )
{
  // save surface
  surface = s;

  // set range of octree to bounding cube of surface
  T.SetRange( Interval( surface->MinX(), surface->MaxX() ), 
              Interval( surface->MinY(), surface->MaxY() ),
              Interval( surface->MinZ(), surface->MaxZ() ) );

  // split octree
  IntervalSplit( T.GetRoot() );

  // balance the octree
  T.Balance();

  // convert to tetrahedra
  cerr << "Tetrahedrizing start\n";
  Tetrahedrize( T.GetRoot() );
  cerr << "Tetrahedrizing done\n";
  cerr << tetramesh.NumTetra() << " tetrahedra\n";

  tetramesh.ComputeGradients( surface );
  tetramesh.ComputeRange( surface );
}

//----------------------------------------------------------------------------
// compute approximation for iso assuming current approximation for old iso
void Approximate::UpdateApproximation( double iso )
{
  if ( iso != iso_value )
    tetramesh.NewIso( iso );
  iso_value = iso;
}

//----------------------------------------------------------------------------
// Return true iff the interval condition <grad F, grad F> > 0 holds
bool 
Approximate::IntervalTest(  
              const Interval &ix, const Interval &iy, const Interval &iz )
{
  Interval ifx, ify, ifz, I;

  ifx = surface->Fx( ix, iy, iz );
  ify = surface->Fy( ix, iy, iz );
  ifz = surface->Fz( ix, iy, iz );

  // note: don't use square function sqr() for this
  I = ifx * ifx + ify * ify + ifz * ifz;
  if ( I.contains( 0.0 ) )
    return false;
  else
    return true;
}

//----------------------------------------------------------------------------
// Split nodes of the octree until each cell does not contain surface or
// the interval condition holds
void 
Approximate::IntervalSplit( OctNode *node, int mindepth, int maxdepth )
{
  Interval ix, iy, iz;

  // get range of node
  node->GetRange( ix, iy, iz );

  // if maximum depth reached we're done
  if ( node->GetDepth() == maxdepth )
    return;

  // test for minimal depth and check gradient test
  if ( ( node->GetDepth() < mindepth ) || !IntervalTest( ix, iy, iz ) )
    {
      // split node and recurse over children
      T.SplitNode( node );
      for ( int i=0; i<8; i++ )
        IntervalSplit( T.GetChild( node, i ), mindepth, maxdepth );
    }
}

//----------------------------------------------------------------------------
// Convert node to tetrahedra by processing children or converting this node
void
Approximate::Tetrahedrize( OctNode *node )
{
  // convert or process children
  if ( node->IsLeaf() )
    TetrahedrizeLeaf( node );
  else
    {
      for ( int i=0; i<8; i++ )
        Tetrahedrize( T.GetChild( node, i ) );
      return;
    }
}

//----------------------------------------------------------------------------
void
Approximate::TetrahedrizeLeaf( OctNode *node )
{
  // opposite direction
  static int opposite[6] = { 1, 0, 3, 2, 5, 4 };

  // lists of children we have to process for a given direction
  static int chld[6][4] = 
    { 
      {0,2,4,6},
      {1,3,5,7},
      {0,1,4,5},
      {2,3,6,7},
      {0,1,2,3},
      {4,5,6,7} 
    };

  // create tetrahedra for all 6 directions
  for ( int dir=0; dir<6; dir++ )
    {
      OctNode *n = T.GetNeighbour( node, dir );
      if ( n != NULL && !n->IsLeaf() )
        {
          // subdivided neighbour, so add edges for its children
          for ( int i=0; i<4; i++ )
            TetrahedrizeFace( node, 
                  T.GetChild( n, chld[dir][i] ), opposite[dir] );
        }
      else
        {
          // neighbour not subdivided, so add edges for this cell
          TetrahedrizeFace( node, node, dir );
        }
    }
}

//----------------------------------------------------------------------------
void
Approximate::AddTetra( VertexInfo *vi, int p1, int p2, int p3 )
{
  // construct a tetrahedron with vertices
  //    ( vi->cx, vi->cy, vi->cz )
  //    ( vi->x[p1], vi->y[p1], vi->z[p1] )
  //    ( vi->x[p2], vi->y[p2], vi->z[p2] )
  //    ( vi->x[p3], vi->y[p3], vi->z[p3] )
  // vi->gradient_ok contains result of interval test over full node.
  // recompute function range over bounding box of tetrahedron
  // and function value at the vertices.
  double fc, f1, f2, f3;
  Interval ix, iy, iz, frange;

  fc = surface->F( vi->cx, vi->cy, vi->cz );
  f1 = surface->F( vi->x[p1], vi->y[p1], vi->z[p1] );
  f2 = surface->F( vi->x[p2], vi->y[p2], vi->z[p2] );
  f3 = surface->F( vi->x[p3], vi->y[p3], vi->z[p3] );

  ix = Interval(vi->cx).hull(vi->x[p1]).hull(vi->x[p2]).hull(vi->x[p3]); 
  iy = Interval(vi->cy).hull(vi->y[p1]).hull(vi->y[p2]).hull(vi->y[p3]); 
  iz = Interval(vi->cz).hull(vi->z[p1]).hull(vi->z[p2]).hull(vi->z[p3]); 

  // more accurate by subdivision
  // frange = surface->F( ix, iy, iz );
  frange = EstimateF( ix, iy, iz );

  tetramesh.AddTetra( vi->cx,    vi->cy,    vi->cz, fc,
                      vi->x[p1], vi->y[p1], vi->z[p1], f1,
                      vi->x[p2], vi->y[p2], vi->z[p2], f2,
                      vi->x[p3], vi->y[p3], vi->z[p3], f3,
                      frange, vi->gradient_ok );
}

//----------------------------------------------------------------------------
// Tetrahedrize face in direction 'dir' of cell 'node'.
// The node *cnode contains the center vertex.
// The opposite cell in this direction is not subdivided.
void
Approximate::TetrahedrizeFace( OctNode *cnode, OctNode *node, int dir )
{
  // opposite direction
  static int opposite[6] = { 1, 0, 3, 2, 5, 4 };

  // perpendicular directions such that first two should be connected in
  // case of four edges with vertices; rotating ccw
  static int perpend[6][4] =
    {
      {4,3,5,2},
      {4,3,5,2},
      {4,1,5,0},
      {4,1,5,0},
      {2,1,3,0},
      {2,1,3,0}
    };

  // loop over four edges of this face to determine if subdivided
  // start at vertex with maximal coordinates and walk around
  // compute vertices in ccw order starting at max vertex
  bool divided_edge[4];  
 
  // in ccw direction seen from positive axis
  for ( int i=0; i<4; i++ )
    {
      // check if edge might contain two vertices
      divided_edge[i] = false;

      // first check neighbour next to this cell
      OctNode *n = T.GetNeighbour( node, perpend[dir][i] );
      if ( n != NULL )
        {
          if ( !n->IsLeaf() )
            {
              // note: n has same size as node
              divided_edge[i] = true;
            }
          else
            {
              // note: n is leaf greater or same size as node
              // only need to check diagonal neighbour is n has same size
              if ( n->GetDepth() == node->GetDepth() )
                {
                  // check neighbour diagonal to this one
                  n = T.GetNeighbour( n, dir );
                  if ( n != NULL && !n->IsLeaf() )
                    {
                      divided_edge[i] = true;
                    }
                }
            }
        }
    }

  // vertex info
  struct VertexInfo vi;

  // compute center
  Interval ix, iy, iz;

  cnode->GetRange( ix, iy, iz );
  vi.cx = ix.mid();
  vi.cy = iy.mid();
  vi.cz = iz.mid();

  // save gradient result
  vi.gradient_ok = IntervalTest( ix, iy, iz );

  // get range of node containing face
  node->GetRange( ix, iy, iz );

  // collect eight vertices around face
  double x[8], y[8], z[8];

  if ( dir == 0 || dir == 1 )
    {
      for ( int i=0; i<8; i++ )
        if ( dir == 0 )
          vi.x[i] = ix.sup();
        else
          vi.x[i] = ix.inf();
      vi.y[0] = iy.sup(); vi.z[0] = iz.sup();
      vi.y[1] = iy.mid(); vi.z[1] = iz.sup();
      vi.y[2] = iy.inf(); vi.z[2] = iz.sup();
      vi.y[3] = iy.inf(); vi.z[3] = iz.mid();
      vi.y[4] = iy.inf(); vi.z[4] = iz.inf();
      vi.y[5] = iy.mid(); vi.z[5] = iz.inf();
      vi.y[6] = iy.sup(); vi.z[6] = iz.inf();
      vi.y[7] = iy.sup(); vi.z[7] = iz.mid();
    }
  else if ( dir == 2 || dir == 3 )
    {
      for ( int i=0; i<8; i++ )
        if ( dir == 2 )
          vi.y[i] = iy.sup();
        else
          vi.y[i] = iy.inf();
      vi.x[0] = ix.sup(); vi.z[0] = iz.sup();
      vi.x[1] = ix.mid(); vi.z[1] = iz.sup();
      vi.x[2] = ix.inf(); vi.z[2] = iz.sup();
      vi.x[3] = ix.inf(); vi.z[3] = iz.mid();
      vi.x[4] = ix.inf(); vi.z[4] = iz.inf();
      vi.x[5] = ix.mid(); vi.z[5] = iz.inf();
      vi.x[6] = ix.sup(); vi.z[6] = iz.inf();
      vi.x[7] = ix.sup(); vi.z[7] = iz.mid();
    }
  else // dir == 4 || dir == 5
    {
      for ( int i=0; i<8; i++ )
        if ( dir == 4 )
          vi.z[i] = iz.sup();
        else
          vi.z[i] = iz.inf();
      vi.x[0] = ix.sup(); vi.y[0] = iy.sup();
      vi.x[1] = ix.mid(); vi.y[1] = iy.sup();
      vi.x[2] = ix.inf(); vi.y[2] = iy.sup();
      vi.x[3] = ix.inf(); vi.y[3] = iy.mid();
      vi.x[4] = ix.inf(); vi.y[4] = iy.inf();
      vi.x[5] = ix.mid(); vi.y[5] = iy.inf();
      vi.x[6] = ix.sup(); vi.y[6] = iy.inf();
      vi.x[7] = ix.sup(); vi.y[7] = iy.mid();
    }

  // construct tetrahedra; convert divided_edge[] to mask for switch
  int divmask = 0;
  for ( int i=0; i<4; i++ )
    if ( divided_edge[i] )
      divmask |= ( 1 << i );

  switch ( divmask )
    {
      case 0:
        AddTetra(&vi,0,2,4); AddTetra(&vi,0,4,6);
        break;
      case 1:
        AddTetra(&vi,0,1,6); AddTetra(&vi,1,2,4); AddTetra(&vi,1,4,6);
        break;
      case 2:
        AddTetra(&vi,0,2,3); AddTetra(&vi,0,3,6); AddTetra(&vi,3,4,6);
        break;
      case 3:
        AddTetra(&vi,0,1,6); AddTetra(&vi,1,2,3); AddTetra(&vi,1,3,6);
        AddTetra(&vi,3,4,6);
        break;
      case 4:
        AddTetra(&vi,0,2,5); AddTetra(&vi,2,4,5); AddTetra(&vi,0,5,6);
        break;
      case 5:
        AddTetra(&vi,0,5,6); AddTetra(&vi,0,1,5); AddTetra(&vi,1,4,5);
        AddTetra(&vi,1,2,4);
        break;
      case 6:
        AddTetra(&vi,0,2,3); AddTetra(&vi,0,3,5); AddTetra(&vi,3,4,5);
        AddTetra(&vi,0,5,6);
        break;
      case 7:
        AddTetra(&vi,0,5,6); AddTetra(&vi,0,1,5); AddTetra(&vi,1,2,3);
        AddTetra(&vi,1,3,5); AddTetra(&vi,3,4,5);
        break;
      case 8:
        AddTetra(&vi,0,2,7); AddTetra(&vi,4,6,7); AddTetra(&vi,2,4,7);
        break;
      case 9:
        AddTetra(&vi,0,1,7); AddTetra(&vi,1,2,4); AddTetra(&vi,1,4,7);
        AddTetra(&vi,4,6,7);
        break;
      case 10:
        AddTetra(&vi,0,2,3); AddTetra(&vi,0,3,7); AddTetra(&vi,3,4,7);
        AddTetra(&vi,4,6,7);
        break;
      case 11:
        AddTetra(&vi,0,1,7); AddTetra(&vi,1,2,3); AddTetra(&vi,1,3,7);
        AddTetra(&vi,3,4,7); AddTetra(&vi,4,6,7);
        break;
      case 12:
        AddTetra(&vi,0,2,7); AddTetra(&vi,2,5,7); AddTetra(&vi,2,4,5);
        AddTetra(&vi,5,6,7);
        break;
      case 13:
        AddTetra(&vi,0,1,7); AddTetra(&vi,1,5,7); AddTetra(&vi,5,6,7);
        AddTetra(&vi,1,2,4); AddTetra(&vi,1,4,5);
        break;
      case 14:
        AddTetra(&vi,0,2,3); AddTetra(&vi,0,3,7); AddTetra(&vi,3,4,5);
        AddTetra(&vi,3,5,7); AddTetra(&vi,5,6,7);
        break;
      case 15:
        AddTetra(&vi,0,1,7); AddTetra(&vi,1,2,3); AddTetra(&vi,3,4,5); 
        AddTetra(&vi,5,6,7); AddTetra(&vi,1,3,5); AddTetra(&vi,1,5,7);
        break;
    }  
}

//----------------------------------------------------------------------------
Interval
Approximate::EstimateF( Interval ix, Interval iy, Interval iz )
{
  if ( ESTIMATE_LEVEL == 1 )
    return surface->F( ix, iy, iz );
  
  Interval result;
  Interval sx, sy, sz;
  int i, j, k;
  
  // init to value at corner
  result = Interval( surface->F( ix.inf(), iy.inf(), iz.inf() ) );
  for ( i=0; i<ESTIMATE_LEVEL; i++ )
    for ( j=0; j<ESTIMATE_LEVEL; j++ )
      for ( k=0; k<ESTIMATE_LEVEL; k++ )
        {
          sx = Interval(ix.inf()+i*(ix.diam()/ESTIMATE_LEVEL),
                        ix.inf()+(i+1)*(ix.diam()/ESTIMATE_LEVEL) );
          sy = Interval(iy.inf()+j*(iy.diam()/ESTIMATE_LEVEL),
                        iy.inf()+(j+1)*(iy.diam()/ESTIMATE_LEVEL) );
          sz = Interval(iz.inf()+k*(iz.diam()/ESTIMATE_LEVEL),
                        iz.inf()+(k+1)*(iz.diam()/ESTIMATE_LEVEL) );
          result = result.hull( surface->F( sx, sy, sz ) );
        }
  return result;
}

//----------------------------------------------------------------------------


