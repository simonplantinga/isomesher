class OffMesh 
{
  int numVertices;
public:
  void Clear;
  void AddTriangle(..);
};

/*
OFF FILE FORMAT:


OFF numVertices numFaces numEdges
x y z
x y z
... numVertices like above
NVertices v1 v2 v3 ... vN
MVertices v1 v2 v3 ... vM
... numFaces like above


Note that vertices are numbered starting at 0 (not starting at 1), and that numEdges will always be zero.

A simple example for a cube:

OFF
8 6 0
-0.500000 -0.500000 0.500000
0.500000 -0.500000 0.500000
-0.500000 0.500000 0.500000
0.500000 0.500000 0.500000
-0.500000 0.500000 -0.500000
0.500000 0.500000 -0.500000
-0.500000 -0.500000 -0.500000
0.500000 -0.500000 -0.500000
4 0 1 3 2
4 2 3 5 4
4 4 5 7 6
4 6 7 1 0
4 1 7 5 3
4 6 0 2 4
*/

