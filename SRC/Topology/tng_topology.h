/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_topology.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Topology header file.
 * PROGRAMMER  : CGSG'2018.
 *               Maxim Markov.
 * LAST UPDATE : 18.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#ifndef __tng_topology_h_
#define __tng_topology_h_

#include "tng_def.h"

/* Vertex representative type */
typedef struct tagtngVERTEX_DEF
{
  VEC P;         /* Vertex position */
  VEC2 T;        /* Texture coordinates */
  VEC N;         /* Normale coordinates */
  VEC4 C;        /* Vertex color */
  VEC Tangent;   /* Tangent */
  VEC Bitangent; /* Bitangent */
} tngVERTEX_DEF;

/* Topology representative type */
typedef struct tagtngTOPOLOGY
{
  INT W, H, NumOfVertexes; /* Size of topology, number of vertexes */
  tngVERTEX_DEF *V;        /* Vertex struct */
} tngTOPOLOGY;

/* Topology subsystem representative type */
typedef struct tagtngSUBSYSTEMTOPOLOGY
{
  /* Create primitive from function.
   * ARGUMENTS:
   *   - topology:
   *       tngTOPOLOGY *G;
   * RETURNS: 
   *   (tngPRIM) Primitive that would create.
   */
  tngPRIM * (*PrimFromTopo)( tngTOPOLOGY *G );

  /* Evaluate normals function.
   * ARGUMENTS: 
   *   -  topology:
   *       tngTOPOLOGY *G;
   * RETURNS: None.
   */
  VOID (*TopoAutoNormals)( tngTOPOLOGY *G );

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
  BOOL (*CreateSphere)( tngTOPOLOGY *G, VEC C, FLT R, INT W, INT H );

  /* Create plane topology function.
   * ARGUMENTS:
   *   - topology:
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
  BOOL (*CreatePlane)( tngTOPOLOGY *G, INT W, INT H, 
                       VEC P, VEC Du, VEC Dv, FLT Tx, FLT Ty );

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
  BOOL (*CreateTorus)( tngTOPOLOGY *G, VEC C, FLT R, FLT r, INT W, INT H );

  /* Free topology function.
   * ARGUMENTS:
   *   - topology:
   *       tngTOPOLOGY *G;
   * RETURNS: None.
   */
  VOID (*TopoFree)( tngTOPOLOGY *G );

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
  VOID (*TrimeshTangentSpace)( tngVERTEX_DEF *V, INT NumOfV, INT *Ind, INT NumOfI );

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
  VOID (*TrimeshAutonormals)( tngVERTEX_DEF *V, INT NumOfV, INT *Ind, INT NumOfI );

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
  BOOL (*CreateHeightField)( tngTOPOLOGY *G, CHAR *HeightFieldName, VEC P, VEC Du, VEC Dv, FLT Scale );
} tngSUBSYSTEMTOPOLOGY;

/* Geometry initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemTopologyInit( VOID );

#endif /* __tng_topology_h_ */

/* END OF 'tng_topology.h' FILE */
