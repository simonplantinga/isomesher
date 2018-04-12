//############################################################################
// tetramesh.cpp
//############################################################################
#include "tetramesh.h"

#include <QGLViewer/qglviewer.h>


//#define DRAWTETRA

//----------------------------------------------------------------------------
// construct mesh for a tetrahedron
void Tetrahedron::ReMesh( double iso )
{
  // collect sign of vertices in 4-bit mask
  int mask = 0;
  for (int i=0; i<4; i++)
    if ( vertex[i].F >= iso )
      mask |= ( 1 << i );

  int i1, i2;     // 0, 1 or 2 vertices with same sign
  int s;          // number of i-vertices
  int j1, j2, j3; // remaining vertices

  switch ( mask )
    {
      case  0:                               s=0; break;  // 0000
      case  1: i1=0;       j1=1; j2=2; j3=3; s=1; break;  // 0001
      case  2: i1=1;       j1=0; j2=2; j3=3; s=1; break;  // 0010
      case  3: i1=0; i2=1; j1=2; j2=3;       s=2; break;  // 0011
      case  4: i1=2;       j1=0; j2=1; j3=3; s=1; break;  // 0100
      case  5: i1=0; i2=2; j1=1; j2=3;       s=2; break;  // 0101
      case  6: i1=1; i2=2; j1=0; j2=3;       s=2; break;  // 0110
      case  7: i1=3;       j1=0; j2=1; j3=2; s=1; break;  // 0111
      case  8: i1=3;       j1=0; j2=1; j3=2; s=1; break;  // 1000
      case  9: i1=1; i2=2; j1=0; j2=3;       s=2; break;  // 1001
      case 10: i1=0; i2=2; j1=1; j2=3;       s=2; break;  // 1010
      case 11: i1=2;       j1=0; j2=1; j3=3; s=1; break;  // 1011
      case 12: i1=0; i2=1; j1=2; j2=3;       s=2; break;  // 1100
      case 13: i1=1;       j1=0; j2=2; j3=3; s=1; break;  // 1101
      case 14: i1=0;       j1=1; j2=2; j3=3; s=1; break;  // 1110
      case 15:                               s=0; break;  // 1111
    }

  if ( s == 0 )
    {
      // all vertices same sign -> no mesh
      num_triangles = 0;
    }
  if ( s == 1 )
    {
      // one vertex with opposite sign -> one triangle
      triangle[0].p1[0] = vertex[i1]; triangle[0].p2[0] = vertex[j1];
      triangle[0].p1[1] = vertex[i1]; triangle[0].p2[1] = vertex[j2];
      triangle[0].p1[2] = vertex[i1]; triangle[0].p2[2] = vertex[j3];
      num_triangles = 1;
    }
  else if ( s == 2 )
    {
      // two vertices with same sign -> quad consisting of two triangles
      triangle[0].p1[0] = vertex[i1]; triangle[0].p2[0] = vertex[j1];
      triangle[0].p1[1] = vertex[i1]; triangle[0].p2[1] = vertex[j2];
      triangle[0].p1[2] = vertex[i2]; triangle[0].p2[2] = vertex[j1];

      triangle[1].p1[0] = vertex[i1]; triangle[1].p2[0] = vertex[j2];
      triangle[1].p1[1] = vertex[i2]; triangle[1].p2[1] = vertex[j1];
      triangle[1].p1[2] = vertex[i2]; triangle[1].p2[2] = vertex[j2];
      num_triangles = 2;
    }
}

//----------------------------------------------------------------------------
void Tetrahedron::Draw( double iso )
{
//  glColor3f( 0.8, 0.2, 0.2 );
 
  double t, x, y, z;
 
  if ( !gradient_ok && frange.contains( iso ) )
    {
      glBegin( GL_TRIANGLES );
      glColor3f( 1.0, 0.2, 0.2 );
      glNormal3f( nx, ny, nz );
      for ( int i=0; i<4; i++ )
        for ( int j=0; j<4; j++ )
          {
            if ( j != i )
              glVertex3f( vertex[j].x, vertex[j].y, vertex[j].z );
          }
      glEnd();
    }
  else
    {
      for ( int i=0; i<num_triangles; i++ )
        { 
          double x[3], y[3], z[3];
          // draw triangle[i]
          glBegin( GL_TRIANGLES );
          glColor3f( 0.7, 0.7, 1.0 );
          glNormal3f( nx, ny, nz );
          for ( int v=0; v<3; v++ )
            { 
              // vertex on edge p1[v]-p2[v]
              t = ( iso - triangle[i].p1[v].F ) 
                    / ( triangle[i].p2[v].F - triangle[i].p1[v].F );
  
              x[v] = triangle[i].p1[v].x 
                    + t * (triangle[i].p2[v].x-triangle[i].p1[v].x);
              y[v] = triangle[i].p1[v].y
                    + t * (triangle[i].p2[v].y-triangle[i].p1[v].y);
              z[v] = triangle[i].p1[v].z
                    + t * (triangle[i].p2[v].z-triangle[i].p1[v].z);

              glVertex3f( x[v], y[v], z[v] );
            }
          glEnd();
          // draw edges
          glBegin( GL_LINES );
          glColor3f( 0.1, 0.3, 0.1 );
          glVertex3f( x[0]+0.001*nx, y[0]+0.001*ny, z[0]+0.001*nz );
          glVertex3f( x[1]+0.001*nx, y[1]+0.001*ny, z[1]+0.001*nz );
          glVertex3f( x[2]+0.001*nx, y[2]+0.001*ny, z[2]+0.001*nz );
          glVertex3f( x[0]+0.001*nx, y[0]+0.001*ny, z[0]+0.001*nz );
          glEnd();
        }
   }
}

//----------------------------------------------------------------------------
void TetraMesh::AddTetra( double x1, double y1, double z1, double f1,
                          double x2, double y2, double z2, double f2,
                          double x3, double y3, double z3, double f3,
                          double x4, double y4, double z4, double f4,
                          Interval frange, bool gradient_ok )
{
  Tetrahedron *tetra = new Tetrahedron();

  tetra->vertex[0].x = x1; tetra->vertex[0].y = y1; tetra->vertex[0].z = z1;
  tetra->vertex[0].F = f1;
  tetra->vertex[1].x = x2; tetra->vertex[1].y = y2; tetra->vertex[1].z = z2;
  tetra->vertex[1].F = f2;
  tetra->vertex[2].x = x3; tetra->vertex[2].y = y3; tetra->vertex[2].z = z3;
  tetra->vertex[2].F = f3;
  tetra->vertex[3].x = x4; tetra->vertex[3].y = y4; tetra->vertex[3].z = z4;
  tetra->vertex[3].F = f4;
  tetra->frange = frange;
  tetra->gradient_ok = gradient_ok;

  tetra->next = tetralist;
  tetralist = tetra;

  num_tetra++;
}

//----------------------------------------------------------------------------
void TetraMesh::Draw()
{
  Tetrahedron *tetra;

  tetra = tetralist;
  while ( tetra != NULL )
    {
      tetra->Draw( isovalue );
      tetra = tetra->next;
    }
}

//----------------------------------------------------------------------------
void TetraMesh::NewIso( double iso )
{
  RemeshAll(iso);
  isovalue = iso;
}

//----------------------------------------------------------------------------
void TetraMesh::ComputeRange( Surface *surface )
{
  double f;
  Tetrahedron *tetra = tetralist;

  if ( tetra != NULL )
    {
      f = surface->F(tetra->vertex[0].x,tetra->vertex[0].y,tetra->vertex[0].z);
      Fmin = f;
      Fmax = f;
    }
  else
    {
      Fmin = 0.0;
      Fmax = 0.0;
    }

  while ( tetra != NULL )
    { 
      for ( int i=0; i<4; i++ )
        {
          f = surface->F(tetra->vertex[i].x,tetra->vertex[i].y,
                              tetra->vertex[i].z);
          if ( f < Fmin )
            Fmin = f;
          if ( f > Fmax )
            Fmax = f;
        }
      tetra = tetra->next;
    }
}

//----------------------------------------------------------------------------
void TetraMesh::ComputeGradients( Surface *surface )
{
  Tetrahedron *tetra;

  tetra = tetralist;
  while ( tetra != NULL )
    {
      double x, y, z;
      double nx, ny, nz, t;
      x = 0; y = 0; z = 0;
      for ( int i=0; i<4; i++ )
        {
          x += tetra->vertex[i].x;
          y += tetra->vertex[i].y;
          z += tetra->vertex[i].z;
        }
      x /= 4.0; y /= 4.0; z /= 4.0;
      nx = surface->Fx( x, y, z );
      ny = surface->Fy( x, y, z );
      nz = surface->Fz( x, y, z );
      t = sqrt( nx*nx + ny*ny + nz*nz );
      tetra->SetGradient( nx/t, ny/t, nz/t );
      tetra = tetra->next;
    }
}

