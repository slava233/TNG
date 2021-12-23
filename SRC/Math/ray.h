/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ray.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Rays mathematics module.
 * PROGRAMMER  : CGSG'2018.
 *               Evgenii Hlevnoi.
 * LAST UPDATE : 29.03.2019.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __ray_h_
#define __ray_h_

#include "vectors.h"

/***
 * Evaluation functions
 ***/

/* Set 3-component ray function.
 * ARGUMENTS:
 *   - float origin and direction 3-component vectors
 *       VEC Org, Direction;
 * RETURNS:
 *   (RAY) which is new 3-component ray
*/
__inline RAY RaySet( VEC Org, VEC Direction )
{
  RAY r;

  r.Org = VecSet(Org.X, Org.Y, Org.Z);
  r.Dir = VecSet(Direction.X, Direction.Y, Direction.Z);
  return r;
} /* End of 'RaySet' function */

/* 3-component ray multiple by matrix function..
 * ARGUMENTS:
 *   - 3-component ray:
 *       RAY R;
 *   - matrix:
 *       MATR M;
 * RETURNS:
 *   (RAY) what is new 3-component ray.
*/
__inline RAY RayMulMatr( RAY R, MATR M )
{  
  return
    RaySet(PointTransform(R.Org, M), VecTransform(R.Dir, M));
} /* End of 'RayMulMatr' function */

#endif /* __ray_h_ */

/* END OF 'ray.h' FILE */
