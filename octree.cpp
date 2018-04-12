//############################################################################
// octree.cpp
//
// copyright (c) 2005 by S.H. Plantinga
//############################################################################

#include "octree.h"

//----------------------------------------------------------------------------
// The child type is given by the bit mask 0zyx, so we have
//
// z=0 +--+--+   z=1 +--+--+
//     | 2| 3|       | 6| 7|
//     +--+--+       +--+--+
//     | 0| 1|       | 4| 5|
//     +--+--+       +--+--+
//
// For the GetNeighbour function the directions are:
//
// 0 = x+
// 1 = x-
// 2 = y+
// 3 = y-
// 4 = z+
// 5 = z-
//
//----------------------------------------------------------------------------

//############################################################################
// OctNode implementation
//############################################################################
OctNode::OctNode( OctNode *myparent )
{
  parent = myparent;
  is_leaf = true;
  first_triangle = NULL;
  empty = false;
  if ( myparent == NULL )
    depth = 0;
  else
    depth = myparent->depth + 1;
}

//----------------------------------------------------------------------------
OctNode::~OctNode()
{
  if ( !is_leaf )
    for ( int i=0; i<8; i++ )
      delete child[i];
}

//----------------------------------------------------------------------------
void
OctNode::Split()
{
  Interval x1, x2, y1, y2, z1, z2;

  x1 = Interval( lower(x_range),  median(x_range) );
  x2 = Interval( median(x_range), upper(x_range)  );
  y1 = Interval( lower(y_range),  median(y_range) );
  y2 = Interval( median(y_range), upper(y_range)  );
  z1 = Interval( lower(z_range),  median(z_range) );
  z2 = Interval( median(z_range), upper(z_range)  );

  for ( int i=0; i<8; i++ )
    {
      child[i] = new OctNode( this );
      child[i]->SetChildType( i );
    }

  child[0]->SetRange( x1, y1, z1 );
  child[1]->SetRange( x2, y1, z1 );
  child[2]->SetRange( x1, y2, z1 );
  child[3]->SetRange( x2, y2, z1 );
  child[4]->SetRange( x1, y1, z2 );
  child[5]->SetRange( x2, y1, z2 );
  child[6]->SetRange( x1, y2, z2 );
  child[7]->SetRange( x2, y2, z2 );

  is_leaf = false;
}

//----------------------------------------------------------------------------
/*
void OctNode::DrawBox()
{
  Vec p1, p2, p3, p4, p5, p6, p7, p8;

  p1 = Vec( x_range.inf(), y_range.inf(), z_range.inf() );
  p2 = Vec( x_range.sup(), y_range.inf(), z_range.inf() );
  p3 = Vec( x_range.sup(), y_range.sup(), z_range.inf() );
  p4 = Vec( x_range.inf(), y_range.sup(), z_range.inf() );
  p5 = Vec( x_range.inf(), y_range.inf(), z_range.sup() );
  p6 = Vec( x_range.sup(), y_range.inf(), z_range.sup() );
  p7 = Vec( x_range.sup(), y_range.sup(), z_range.sup() );
  p8 = Vec( x_range.inf(), y_range.sup(), z_range.sup() );

  glColor3f( 1.0, 1.0, 0.5 );

  glBegin( GL_LINES );
  glVertex3fv( p1 );    glVertex3fv( p2 );
  glVertex3fv( p2 );    glVertex3fv( p3 );
  glVertex3fv( p3 );    glVertex3fv( p4 );
  glVertex3fv( p4 );    glVertex3fv( p1 );
  glVertex3fv( p5 );    glVertex3fv( p6 );
  glVertex3fv( p6 );    glVertex3fv( p7 );
  glVertex3fv( p7 );    glVertex3fv( p8 );
  glVertex3fv( p8 );    glVertex3fv( p5 );
  glVertex3fv( p1 );    glVertex3fv( p5 );
  glVertex3fv( p2 );    glVertex3fv( p6 );
  glVertex3fv( p3 );    glVertex3fv( p7 );
  glVertex3fv( p4 );    glVertex3fv( p8 );
  glEnd();
}
*/
//############################################################################
// Octree implementation
//############################################################################
Octree::Octree()
{
  root = new OctNode( NULL );
  num_leaves = 1;
}

Octree::Octree( const Interval &x, const Interval &y, const Interval &z )
{
  root = new OctNode( NULL );
  root->SetRange( x, y, z );
  num_leaves = 1;
}

//----------------------------------------------------------------------------
Octree::~Octree()
{
  delete root;
}

//----------------------------------------------------------------------------
void Octree::SetRange( const Interval &x, const Interval &y, const Interval &z
)
{
  root->SetRange( x, y, z );
}

//----------------------------------------------------------------------------
void Octree::GetRange( Interval &x, Interval &y, Interval &z )
{
  root->GetRange( x, y, z );
}

//----------------------------------------------------------------------------
void
Octree::InsertLeaves( NodeList *l, OctNode *n )
{
  if ( n->is_leaf )
    l->push( n );
  else
    {
      for ( int i=0; i<8; i++ )
        InsertLeaves( l, n->child[i] );
    }
}

//----------------------------------------------------------------------------
void
Octree::Balance()
{
  static int c1[6] = { 0, 1, 0, 2, 0, 4 };
  static int c2[6] = { 2, 3, 1, 3, 1, 5 };
  static int c3[6] = { 4, 5, 4, 6, 2, 6 };
  static int c4[6] = { 6, 7, 5, 7, 3, 7 };
  NodeList nodes;
  OctNode *n;
  OctNode *neighbour;

  // insert all the leaves of T into a linear list
  InsertLeaves( &nodes, root );

  // while list not empty
  while ( !nodes.empty() )
    {
      // remove a leaf from list
      n = nodes.top();
      nodes.pop();

      // we might have pushed a neighbour that was already on the list
      if ( !n->is_leaf )
        continue;

      // test if leaf has to be split
      for ( int d=0; d<6; d++ )
        {
          neighbour = GetNeighbour( n, d );
          if ( neighbour != NULL 
                && !neighbour->is_leaf 
                && ( !neighbour->child[c1[d]]->is_leaf ||
                     !neighbour->child[c2[d]]->is_leaf ||
                     !neighbour->child[c3[d]]->is_leaf ||
                     !neighbour->child[c4[d]]->is_leaf ) )
            {
              // check for neighbours that need splitting
              for ( int d=0; d<6; d++ )
                {
                  neighbour = GetNeighbour( n, d );
                  if ( neighbour != NULL && neighbour->depth < n->depth )
                    nodes.push( neighbour );
                }

              // need to split this node
              n->Split();
              num_leaves += 7;

              // add children to list
              for ( int i=0; i<8; i++ )
                nodes.push( n->child[i] );

              // don't need to test other directions
              break;
            }
        }
    }
}

//----------------------------------------------------------------------------
void
Octree::SplitNode( OctNode *node )
{
  node->Split();
  num_leaves += 7;
}

//----------------------------------------------------------------------------
OctNode *
Octree::GetRoot()
{
  return root;
}

//----------------------------------------------------------------------------
OctNode *
Octree::GetChild( OctNode *node, int childtype )
{
  return node->child[childtype];
}

//----------------------------------------------------------------------------
OctNode *
Octree::GetParent( OctNode *node )
{
  return node->parent;
}
      
//----------------------------------------------------------------------------
// Return the deepest neighbour node whose depth is at most the depth of 
// *node, or NULL if there is no such node
OctNode *
Octree::GetNeighbour( OctNode *node, int direction )
{
  static int mask[6] = { 1, 1, 2, 2, 4, 4 };
  static int val[6]  = { 1, 0, 2, 0, 4, 0 };

  OctNode *m;

  if ( node->parent == NULL )
    return NULL;
  
  if ( ( node->child_type & mask[direction] ) != val[direction] )
    return node->parent->child[node->child_type ^ mask[direction]];

  m = GetNeighbour( node->parent, direction );
  if ( m == NULL || m->is_leaf )
    return m;

  return m->child[node->child_type ^ mask[direction]];
}

//----------------------------------------------------------------------------
OctNode *
Octree::GetCorner( OctNode *node, int direction )
{
  while ( !node->is_leaf )
    node = node->child[direction];
  return node;
}

//----------------------------------------------------------------------------
int
Octree::NumLeaves()
{
  return num_leaves;
}

//----------------------------------------------------------------------------

