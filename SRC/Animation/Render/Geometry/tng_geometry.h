/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_primitives.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Main geometry module.
 * PROGRAMMER  : CGSG'2018.
 *               Evgenii Hlevnoi.
 * LAST UPDATE : 26.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_geometry_h_
#define __tng_geometry_h_

#include "../../../tng_def.h"

/* Geometry representation type */
typedef struct tagtngGEOMETRY
{
  tngPRIM **Prims; /* Primitive pointers massive */
  INT NumOfPrims;  /* Number of primitives in geometry object */
  MATR Trans;      /* Geometry object translation matrix */ 
  VEC
    Min, Max,      /* Bound box points (AABB) */
    Center;        /* Center vector */
} tngGEOMETRY;

/* Geometry subsytem inheritance structure */
typedef struct tagtngSUBSYSTEM_GEOMETRY
{
  /* Geometry objects stock */
  STOCK Geometry;

  /* Geometry initialize function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*GeometryInit)( VOID );

  /* Geometry load from g3dm file function.
   * ARGUMENTS:
   *   - geometry object pointer:
   *       tngGEOMETRY *Geom;
   *   - file name:
   *       CHAR *FileName;
   * RETURNS:
   *   (tngGEOMETRY) pointer to geometry object.
   */
  tngGEOMETRY * (*GeometryLoad)( CHAR *FileName );

  /* Geometry draw function.
   * ARGUMENTS: 
   *   - geometry object pointer:
   *       tngGEOMETRY *Geom;
   *   - world matrix:
   *       MATR World;       
   * RETURNS: None.
   */
  VOID (*GeometryDraw)( tngGEOMETRY *Geom, MATR World );

  /* Transparent Geometry draw function.
   * ARGUMENTS: 
   *   - geometry object pointer:
   *       tngGEOMETRY *Geom;
   *   - world matrix:
   *       MATR World;
   * RETURNS: None.
   */
  VOID (*GeometryDrawDeferred)( tngGEOMETRY *Geom, MATR World );

  /* Geometry free strategy function.
   * ARGUMENTS:
   *   - Self-pointer to geometry object:
   *       tngGEOMETRY *Geom;
   * RETURNS: None.
   */
  VOID (*GeometryDelete)( tngGEOMETRY *Geom );

  /* Geometry delete function.
   * ARGUMENTS:
   *   - Self-pointer to geometry object:
   *       tngGEOMETRY *Geom;
   * RETURNS: None.
   */
  VOID (*GeometryFree)( tngGEOMETRY *Geom );

  /* Geometry object add function.
   * ARGUMENTS:
   *   - Number of primitives in geometry object:
   *       INT NumOfPrims;
   * RETURNS:
   *   (tngGEOMETRY *) Pointer to Geometry object or NULL if failed.
   */
  tngGEOMETRY * (*GeometryAdd)( INT NumOfPrims );
} tngSUBSYSTEM_GEOMETRY;

/* Geometry initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemGeometryInit( VOID );

/* Geometry stock deinitialization function function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemGeometryDestructor( VOID );

#endif /* __tng_geometry_h_ */

/* END OF 'tng_geometry.h' FILE */


