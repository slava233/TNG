/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_primitives.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Main primitives module.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kononov.
 * LAST UPDATE : 28.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_primitives_h_
#define __tng_primitives_h_

/* Primitive types */
typedef enum tagtngPRIM_TYPE
{
  TNG_PRIM_POINTS,              /* Points */
  TNG_PRIM_LINES,               /* Lines */
  TNG_PRIM_LINE_STRIP,          /* Lines strip */
  TNG_PRIM_TRIMESH,             /* Triangles */
  TNG_PRIM_STRIP,               /* Triangles strip */
  TNG_PRIM_PATCH,               /* Patches */
  TNG_PRIM_LINE_STRIP_ADJACENCY /* Lines strip adjacency */
} tngPRIM_TYPE;

/* Primitive structure */
struct tagtngPRIM
{
  tngPRIM_TYPE Type; /* Primitive type */
  tngMATERIAL *Mtl;  /* Primitive material */
  MATR Trans;        /* Additional transformation matrix */
  INT
    VA, IBuf, VBuf,  /* OpenGL vertex array, vertex buffer and index buffer */
    NumOfI;          /* Number of indexes/vertexes/patches points */
  VEC
    Min,             /* Minimum vector */
    Max,             /* Maximum vector */
    Center;          /* Center vector */
};

/* Deferred primitive structure */
typedef struct tagtngPRIM_DEFERRED
{
  tngPRIM *Prim;  /* Deferred primitive reference */
  MATR World;     /* World matrix */
  CAMERA Camera;  /* Used camera */
} tngPRIM_DEFERRED;

/* Primitives subsystem structure */
typedef struct tagtngSUBSYSTEM_PRIMITIVES
{
  /* Primitive stock */
  STOCK Primitives;

  /* Defered primitive stock */
  STOCK PrimitivesDeferred;

  /* Primitive handle functions */

  /* Primitive create function.
   * ARGUMENTS: 
   *   - primitive type:
   *       tngPRIM_TYPE Type;
   *   - vertex data format:
   *       CHAR *VertexFormat;
   *   - vertex data:
   *       VOID *V;
   *   - number of vertexes:
   *       INT NumOfV;
   *   - index array:
   *       INT *Ind;
   *   - number of indices:
   *       INT NumOfI;
   * RETURNS:
   *   (tngPRIM *) pointer to created primitive.
   */
  tngPRIM * (*PrimCreate)( tngPRIM_TYPE Type, CHAR *VertexFormat, VOID *V, INT NumOfV, INT *Ind, INT NumOfI );

  /* Primitive deferring function.
   * ARGUMENTS:
   *   - primitive to defer:
   *       tngPRIM *Prim;
   *   - world CS matrix to work with:
   *       MATR World;
   * RETURNS: None.
   */
  VOID (*PrimDrawDeferred)( tngPRIM *Prim, MATR World );

  /* Draw primitive function.
   * ARGUMENTS:
   *   - primitive to draw:
   *       tngPRIM *Prim;
   *   - world CS matrix to work with:
   *       MATR World;
   * RETURNS: None.
   */
  VOID (*PrimDraw)( tngPRIM *Prim, MATR World );

  /* Draw all deferred primitives function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*PrimDrawAllDeferred)( VOID );

  /* Delete primitive function.
   * ARGUMENTS:
   *   - primitive to delete:
   *       tngPRIM *Prim;
   * RETURNS: None.
   */
  VOID (*PrimFree)( tngPRIM *Prim );
} tngSUBSYSTEM_PRIMITIVES;

/* Primitives subsystem initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemPrimitivesInit( VOID );

/* Primitives subsystem destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemPrimitivesDestructor( VOID );

#endif /* __tng_primitives_h_ */

/* END OF 'tng_primitives.h' FILE */
