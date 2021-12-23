/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_topology.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Topology handle.
 *               G3DM and  handle.
 * PROGRAMMER  : CGSG'2018.
 *               Maxim Markov.
 * LAST UPDATE : 20.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../tng.h"

#include <stdio.h>

/* Helpful triangle tangent space evaluation function.
 * ARGUMETNS:
 *   - vertices to count tangent space:
 *       tngVERTEX_DEF *V0, *V1, *V2;
 * RETURNS: None.
 */
static VOID TNG_TriangleTangentSpace( tngVERTEX_DEF *V0, tngVERTEX_DEF *V1, tngVERTEX_DEF *V2 )
{
    VEC2
    T0 = V0->T,
    T1 = V1->T,
    T2 = V2->T;
  FLT
    s1 = T1.X - T0.X,
    t1 = T1.Y - T0.Y,
    s2 = T2.X - T0.X, 
    t2 = T2.Y - T0.Y,
    det = s1 * t2 - s2 * t1;
  VEC
    q1 = VecSubVec(V1->P, V0->P),
    q2 = VecSubVec(V2->P, V0->P);
  VEC
    T = VecDivNum(VecSubVec(VecMulNum(q1, t2), VecMulNum(q2, t1)), det),
    B = VecDivNum(VecSubVec(VecMulNum(q2, s1), VecMulNum(q1, s2)), det);
  
  V0->Tangent = VecAddVec(V0->Tangent, T);
  V1->Tangent = VecAddVec(V1->Tangent, T);
  V2->Tangent = VecAddVec(V2->Tangent, T);
  V0->Bitangent = VecAddVec(V0->Bitangent, B);
  V1->Bitangent = VecAddVec(V1->Bitangent, B);
  V2->Bitangent = VecAddVec(V2->Bitangent, B);
} /* End of 'TNG_TriangleTangentSpace' function */

/* Tangent space evaluation function.
 * ARGUMENTS: 
 *   - vertex struct to evaluate normals for:
 *       tngVERTEX_DEF *V;
 *   - number of vertexes:
 *       INT NumOfV;
 *   - indexes array:
 *       INT *Ind;
 *   - number of indexes:
 *       INT NumOfI;
 * RETURNS: None.
 */
static VOID TNG_PrimTrimeshTangentSpace( tngVERTEX_DEF *V, INT NumOfV, INT *Ind, INT NumOfI )
{
  INT i;

  for (i = 0; i < NumOfV; i++)
    V[i].Tangent = V[i].Bitangent = VecSet(0, 0, 0);
  for (i = 0; i < NumOfI; i += 3)
  {
    INT 
      n0 = Ind[i],
      n1 = Ind[i + 1],
      n2 = Ind[i + 2];
    TNG_TriangleTangentSpace(&V[n0], &V[n1], &V[n2]);
  }
  for (i = 0; i < NumOfV; i++)
  {
    V[i].Tangent = VecNormalize(VecSubVec(V[i].Tangent, VecMulNum(V[i].N, VecDot(V[i].Tangent, V[i].N))));
    V[i].Bitangent = VecNormalize(VecSubVec(VecSubVec(V[i].Bitangent, VecMulNum(V[i].N, VecDot(V[i].Bitangent, V[i].N))), VecMulNum(V[i].Tangent, VecDot(V[i].Bitangent, V[i].Tangent))));
  }
} /* End of 'TNG_PrimTrimeshTangentSpace' function */

/* Trimesh autonormals evaluation function.
 * ARGUMENTS: 
 *   - vertex struct to evaluate normals for:
 *       tngVERTEX_DEF *V;
 *   - number of vertexes:
 *       INT NumOfV;
 *   - indexes array:
 *       INT *Ind;
 *   - number of indexes:
 *       INT NumOfI;
 * RETURNS: None.
 */
static VOID TNG_TrimeshAutonormals( tngVERTEX_DEF *V, INT NumOfV, INT *Ind, INT NumOfI )
{
  INT i;

  for (i = 0; i < NumOfV; i++)
    V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < NumOfI; i += 3)
  {
    INT n0 = Ind[i], n1 = Ind[i + 1], n2 = Ind[i + 2];
    VEC
      P0 = V[n0].P,
      P1 = V[n1].P,
      P2 = V[n2].P,
      S1 = VecSubVec(P1, P0),
      S2 = VecSubVec(P2, P0),
      N = VecNormalize(VecCross(S1, S2));

    V[n0].N = VecAddVec(V[n0].N, N);
    V[n1].N = VecAddVec(V[n1].N, N);
    V[n2].N = VecAddVec(V[n2].N, N);
  }

  for (i = 0; i < NumOfV; i++)
    V[i].N = VecNormalize(V[i].N);
} /* End of 'TNG_TrimeshAutonormals' function */

/* Create trimesh topology function.
 * ARGUMENTS:
 *   -  topology:
 *       tngTOPOLOGY *G;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE if not.
 */
static tngPRIM * TNG_TrimeshCreate( tngTOPOLOGY *G, INT NumOfVertexes, tngVERTEX_DEF *V )
{
  INT *Ind, ni = 0;
  tngPRIM *Pr = {0};

  memset(G, 0, sizeof(tngTOPOLOGY));

  G->NumOfVertexes = NumOfVertexes;
  if ((G->V = malloc(sizeof(tngVERTEX_DEF) * G->NumOfVertexes)) == NULL)
    return Pr;
  if ((Ind = malloc(sizeof(INT) * NumOfVertexes)) == NULL)
    return Pr;
  while (ni != G->NumOfVertexes)
  {
    G->V[ni] = V[ni];
    Ind[ni++] = ni;
  }
  TNG_TrimeshAutonormals(G->V, G->NumOfVertexes, Ind, ni);
  Pr = TNG()->PrimCreate(TNG_PRIM_STRIP, "f3f2f3f4f3f3", G->V, G->NumOfVertexes, Ind, ni);
  return Pr;
} /* End of 'TNG_TrimeshCreate' function */

/* Create topology function.
 * ARGUMENTS:
 *   -  topology:
 *       tngTOPOLOGY *G;
 *   - size of :
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE if not.
 */
static BOOL TNG_TopoCreate( tngTOPOLOGY *G, INT W, INT H )
{
  INT i, j, k;

  memset(G, 0, sizeof(tngTOPOLOGY));

  if ((G->V = malloc(sizeof(tngVERTEX_DEF) * W * H)) == NULL)
    return FALSE;
  G->W = W;
  G->H = H;
  for (i = 0, k = 0; i < G->H; i++)
    for (j = 0; j < G->W; j++, k++)
    {
      G->V[k].P = VecSet(i - W / 2, 0, H / 2 - j);
      G->V[k].N = VecSet(0, 1, 0);
      G->V[k].C = Vec4Set(1, 1, 1, 1);
      G->V[k].T = Vec2Set(j / (G->W - 1.0),
                          i / (G->H - 1.0));
    }
  return TRUE;
} /* End of 'TNG_TopoCreate' function */

/* Free topology function.
 * ARGUMENTS:
 *   -  topology:
 *       tngTOPOLOGY *G;
 * RETURNS: None.
 */
static VOID TNG_TopoFree( tngTOPOLOGY *G )
{
  if (G->V != NULL)
    free(G->V);
  memset(G, 0, sizeof(tngTOPOLOGY));
} /* End of 'TNG_RndGridFree' function */

/* Evaluate normals topo function.
 * ARGUMENTS: 
 *   - topology:
 *       tngTOPOLOGY *G;
 * RETURNS: None.
 */
static VOID TNG_TopoAutoNormals( tngTOPOLOGY *G )
{
  INT i, j, k;

  for (i = 0, k = 0; i < G->H; i++)
    for (j = 0; j < G->W; j++)
      G->V[k++].N = VecSet(0, 0, 0);

  for (i = 0, k = 0; i < G->H - 1; i++)
    for (j = 0; j < G->W - 1; j++, k++)
    {
      VEC N;
      tngVERTEX_DEF
        *p00 = &G->V[k],
        *p01 = &G->V[k + 1],
        *p10 = &G->V[k + G->W],
        *p11 = &G->V[G->W + k + 1];

      N = VecNormalize(VecCross(VecSubVec(p00->P, p10->P), VecSubVec(p11->P, p10->P)));
      p00->N = VecAddVec(p00->N, N);
      p11->N = VecAddVec(p11->N, N);
      p10->N = VecAddVec(p10->N, N);

      /* Lower right */
      N = VecNormalize(VecCross(VecSubVec(p11->P, p01->P), VecSubVec(p00->P, p01->P)));
      p00->N = VecAddVec(p00->N, N);
      p11->N = VecAddVec(p11->N, N);
      p01->N = VecAddVec(p01->N, N);
    }
  for (i = 0; i < G->W * G->H; i++)
    G->V[i].N = VecNormalize(G->V[i].N);
} /* End of 'TNG_TopoAutoNormals' function */

/* Create primitive from function.
 * ARGUMENTS:
 *   - topology:
 *       tngTOPOLOGY *G;
 * RETURNS: 
 *   (tngPRIM *) Primitive.
 */
static tngPRIM * TNG_PrimFromTopo( tngTOPOLOGY *G )
{
  tngPRIM *Pr = {0};
  INT *Ind, i, j, k, v;

  if ((Ind = malloc(sizeof(INT) * ((G->W * 2 + 1) * (G->H - 1) - 1))) == NULL)
    return Pr;

  /* Index array fill */
  for (i = 0, k = 0, v = 0; i < G->H - 1; i++)
  {
    for (j = 0; j < G->W; j++)
    {
      Ind[k++] = v + G->W;
      Ind[k++] = v++;
    }
    if (i != G->H - 2)
      Ind[k++] = -1;
  }

  /* Create primitive */
  Pr = TNG()->PrimCreate(TNG_PRIM_STRIP, "f3f2f3f4f3f3", G->V, G->W * G->H, Ind, (G->W * 2 + 1) * (G->H - 1) - 1);
  free(Ind);
  return Pr;
} /* End of 'TNG_PrimFromTopo' funciton */

/* Create sphere topology function.
 * ARGUMENTS:
 *   - topology:
 *       tngTOPOLOGY *G;
 *   - center of sphere:
 *       VEC C;
 *   - radius of sphere:
 *       FLT R;
 *   - size of sphere:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE overwise.
 */
static BOOL TNG_TopoCreateSphere( tngTOPOLOGY *G, VEC C, FLT R, INT W, INT H )
{
  INT i, j, v;

  if (!TNG_TopoCreate(G, W, H))
    return FALSE;

  for (i = 0, v = 0; i < H; i++)
  {
    DBL
      theta = (H - 1 - i) * MTH_PI / (H - 1),
      sit = sin(theta), cot = cos(theta);
    for (j = 0; j < W; j++, v++)
    {
      DBL
        phi = j * 2 * MTH_PI / (W - 1),
        sip = sin(phi), cop = cos(phi),
        x = sit * sip,
        y = cot,
        z = sit * cop;
      G->V[v].P = VecSet(C.X + x * R,
                         C.Y + y * R,
                         C.Z + z * R);
      G->V[v].N = VecSet(x, y, z);
    }
  }
  return TRUE;
} /* End of 'TNG_TopoCreateSphere' function */

/* Create torus topology function.
 * ARGUMENTS:
 *   - topology:
 *       tngTOPOLOGY *G;
 *   - center of torus:
 *       VEC C;
 *   - radius of main circle of torus:
 *       FLT R;
 *   - radius of torus tube:
 *       FLT r;
 *   - size of torus:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE overwise.
 */
static BOOL TNG_TopoCreateTorus( tngTOPOLOGY *G, VEC C, FLT R, FLT r, INT W, INT H )
{
  INT i, k, j;
  FLT theta, psi, x, y, z;

  memset(G, 0, sizeof(tngTOPOLOGY));
  if (!TNG_TopoCreate(G, W, H))
    return FALSE;

  for (theta = 0, i = 0, k = 0; i < G->H; theta += 2 * MTH_PI / (G->H - 1), i++)
    for (psi = 0, j = 0; j < G->W; psi += 2 * MTH_PI / (G->W - 1), j++, k++)
    {
      x = (R + r * cos(theta)) * cos(psi);
      y = (R + r * cos(theta)) * sin(psi);
      z = r * sin(theta);
      G->V[k].P = VecSet(C.X + x * R,
                         C.Y + y * R,
                         C.Z + z * R);
      G->V[k].N = VecSet(x, y, z);
      G->V[k].C = Vec4Set(1, 1, 1, 1);
    }
  return TRUE;
} /* End of 'TNG_TopoCreateTorus' function */

/* Create plane topology function.
 * ARGUMENTS:
 *   -  topology:
 *       tngTOPOLOGY *G;
 *   - size of sphere:
 *       INT W, H;
 *   - center of plane:
 *       VEC P;
 *   - vertexes of direcrions:
 *       VEC Du, VEC Dv;
 *   - texture x and y correction:
 *       FLT Tx, Ty;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL TNG_TopoCreatePlane( tngTOPOLOGY *G, INT W, INT H, 
       VEC P, VEC Du, VEC Dv, FLT Tx, FLT Ty )
{
  INT i, j, v;
  VEC N;

  if (!TNG_TopoCreate(G, W, H))
    return FALSE;

  N = VecNormalize(VecCross(Du, Dv));

  /* Setup all vertexes */
  for (i = 0, v = 0; i < H; i++)
  {
    DBL sy = i / (H - 1.0);

    for (j = 0; j < W; j++, v++)
    {
      DBL sx = j / (W - 1.0);

      G->V[v].P = VecAddVec(P, VecAddVec(VecMulNum(Du, sx), VecMulNum(Dv, sy)));
      G->V[v].N = N;
      G->V[v].T.X *= Tx;
      G->V[v].T.Y *= Ty;
    }
  }
  return TRUE;
} /* End of 'TNG_TopoCreatePlane' function */


/* Create plane topology function.
 * ARGUMENTS:
 *   -  topology:
 *       tngTOPOLOGY *G;
 *   - file name:
 *       VHAR HeightFieldFileName;
 *   - center of plane:
 *       VEC P;
 *   - vertexes of direcrions:
 *       VEC Du, VEC Dv;
 *   - scale:
 *       FLT Scale;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL TNG_TopoCreateHeightField( tngTOPOLOGY *G, CHAR *HeightFieldFileName, VEC P, VEC Du, VEC Dv, FLT Scale )
{
  INT i, j, v;
  tngIMAGE Img;
  VEC N;

  memset(G, 0, sizeof(tngTOPOLOGY));
  if (!TNG()->ImageLoad(&Img, HeightFieldFileName))
    return FALSE;
  if (!TNG()->CreatePlane(G, Img.W, Img.H, P, Du, Dv, 1, 1))
  {
    TNG()->ImageFree(&Img);
    return FALSE;
  }
  N = G->V[0].N;
  /* Setup all vertices */
  for (i = 0, v = 0; i < G->H; i++)
    for (j = 0; j < G->W; j++, v++)
    {
      DWORD col = Img.Pixels[i * Img.H + j];

      G->V[v].P = VecAddVec(G->V[v].P,
        VecMulNum(N, ((col >> 8) & 0xFF) / 255.0 * Scale));
    }
  TNG()->TopoAutoNormals(G);
  TNG()->ImageFree(&Img);
  return TRUE;
} /* End of 'TNG_TopoCreateHeightField' function */

#if 0
/* Create cube topology function.
 * ARGUMENTS:
 *   - topology:
 *       tngTOPOLOGY *G;
 *   - center of cube:
 *       VEC C;
 *   - side of cube:
 *       FLT Side;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE overwise.
 */
static BOOL TNG_TopoCreateCube( tngTOPOLOGY *G, VEC C, FLT Side )
{
  if (!TNG_TopoCreate(G, 4, 6))
    return FALSE;

  /* First Side */
  G->V[0].P = VecSet(C.X, C.Y, C.Z);
  G->V[1].P = VecSet(C.X + Side, C.Y, C.Z);
  G->V[2].P = VecSet(C.X + Side, C.Y + Side, C.Z);
  G->V[3].P = VecSet(C.X, C.Y + Side, C.Z);


  /* Second side */
  G->V[4].P = VecSet(C.X, C.Y + Side, C.Z);
  G->V[5].P = VecSet(C.X + Side, C.Y + Side, C.Z);
  G->V[6].P = VecSet(C.X + Side, C.Y + Side, C.Z + Side);
  G->V[7].P = VecSet(C.X, C.Y + Side, C.Z + Side);

  /* Third side */
  G->V[8].P = VecSet(C.X, C.Y + Side, C.Z + Side);
  G->V[9].P = VecSet(C.X, C.Y, C.Z + Side);
  G->V[10].P = VecSet(C.X, C.Y, C.Z);
  G->V[11].P = VecSet(C.X, C.Y + Side, C.Z);

  /* Fourth side */
  G->V[12].P = VecSet(C.X + Side, C.Y, C.Z);
  G->V[13].P = VecSet(C.X + Side, C.Y + Side, C.Z);
  G->V[14].P = VecSet(C.X + Side, C.Y + Side, C.Z + Side);
  G->V[15].P = VecSet(C.X + Side, C.Y, C.Z + Side);
  
  /* Fourth side */
  G->V[16].P = VecSet(C.X + Side, C.Y, C.Z + Side);
  G->V[17].P = VecSet(C.X + Side, C.Y, C.Z);
  G->V[18].P = VecSet(C.X, C.Y, C.Z);
  G->V[19].P = VecSet(C.X, C.Y, C.Z + Side);

  /* Fifth side */
  G->V[20].P = VecSet(C.X, C.Y, C.Z + Side);
  G->V[21].P = VecSet(C.X, C.Y + Side, C.Z + Side);
  G->V[22].P = VecSet(C.X, C.Y + Side, C.Z + Side);
  G->V[23].P = VecSet(C.X + Side, C.Y, C.Z + Side);

  return TRUE;
}
#endif

/* Geometry initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemTopologyInit( VOID )
{
  TNG()->CreateSphere = TNG_TopoCreateSphere;
  TNG()->CreatePlane = TNG_TopoCreatePlane;
  TNG()->CreateTorus = TNG_TopoCreateTorus;
  TNG()->PrimFromTopo = TNG_PrimFromTopo;
  TNG()->TopoAutoNormals = TNG_TopoAutoNormals;
  TNG()->TrimeshTangentSpace = TNG_PrimTrimeshTangentSpace;
  TNG()->TopoFree = TNG_TopoFree;
  TNG()->TrimeshAutonormals = TNG_TrimeshAutonormals;
  TNG()->CreateHeightField = TNG_TopoCreateHeightField;
  /*
  TNG()->CreateCube = TNG_TopoCreateCube;
  */
} /* End of 'TNG_SubsystemGeometryInit' funciton */

/* END OF 'tng_topology.c' FILE */
