/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : camera.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Camera handle module.
 * PROGRAMMER  : CGSG'2018.
 *               Evgenii Hlevnoi.
 * LAST UPDATE : 29.03.2019.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __camera_h_
#define __camera_h_

#include "vectors.h"
#include "matr.h"

/* Set camera parameters function.
 * ARGUMENTS:
 *   - camera structure pointer:
 *       CAMERA *Cam;
 *   - location, point of interest and up view 3-component vectors:
 *       VEC Loc, At, Up;
 * RETURNS: None.
 */
__inline VOID CamSet( CAMERA *Cam, VEC Loc, VEC At, VEC Up )
{
  Cam->Loc = Loc;
  Cam->At = At;
  Cam->ViewMatr = MatrView(Loc, At, Up);
  Cam->VP = MatrMulMatr(Cam->ViewMatr, Cam->ProjMatr);
} /* End of 'CamSet' function */

/* Obtain camera right direction function.
 * ARGUMENTS:
 *   - camera structure pointer:
 *       CAMERA *Cam;
 * RETURNS:
 *   (VEC) right direction vector.
 */
__inline VEC CamRight( CAMERA *Cam )
{
  return VecSet(Cam->ViewMatr.M[0][0], Cam->ViewMatr.M[1][0], Cam->ViewMatr.M[2][0]);
} /* End of 'CamRight' function */

/* Obtain camera up direction function.
 * ARGUMENTS:
 *   - camera structure pointer:
 *       CAMERA *Cam;
 * RETURNS:
 *   (VEC) up direction vector.
 */
__inline VEC CamUp( CAMERA *Cam )
{
  return VecSet(Cam->ViewMatr.M[0][1], Cam->ViewMatr.M[1][1], Cam->ViewMatr.M[2][1]);
} /* End of 'CamUp' function */

/* Obtain camera forward direction function.
 * ARGUMENTS:
 *   - camera structure pointer:
 *       CAMERA *Cam;
 * RETURNS:
 *   (VEC) forward direction vector.
 */
__inline VEC CamDir( CAMERA *Cam )
{
  return VecSet(-Cam->ViewMatr.M[0][2], -Cam->ViewMatr.M[1][2], -Cam->ViewMatr.M[2][2]);
} /* End of 'CamDir' function */

/* Camera setting projection function.
 *   - camera structure pointer:
 *       CAMERA *Cam;
 *   - frame size in pixels:
 *       INT W, H;
 * RETURNS: None.
 */
__inline VOID CamProjSet( CAMERA *Cam, INT W, INT H )
{
  FLT ratio_x, ratio_y;

  ratio_x = ratio_y = Cam->ProjSize / 2;

  if (W > H) 
    ratio_x *= (FLT)W / H;
  else               
    ratio_y *= (FLT)H / W;

  Cam->ProjW = ratio_x * 2;
  Cam->ProjH = ratio_y * 2;

  Cam->ProjMatr =
    MatrFrustum(-ratio_x, ratio_x, -ratio_y, ratio_y,
      Cam->ProjDist, Cam->ProjFarClip);
  Cam->VP = MatrMulMatr(Cam->ViewMatr, Cam->ProjMatr);
} /* End of 'CamProjSet' function */

/* Set camera default parameters function.
 *   - camera structure pointer:
 *       CAMERA *Cam;
 * RETURNS: None.
 */
__inline VOID CamSetDefault( CAMERA *Cam )
{
  /* Set viewer */
  Cam->Loc = VecSet(0, 0, 1);
  Cam->At = VecSet(0, 0, 0);
  Cam->ViewMatr = MatrView(Cam->Loc, Cam->At, VecSet(0, 1, 0));

  /* Set projection */
  Cam->ProjSize = 0.1;
  Cam->ProjDist = Cam->ProjSize;
  Cam->ProjFarClip = 1000;
  CamProjSet(Cam, 1, 1);
} /* End of 'CamSetDefault' function */

#endif /* __camera_h_ */

/* END OF 'camera.h' FILE */
