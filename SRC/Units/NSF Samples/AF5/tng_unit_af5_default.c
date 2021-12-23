/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_af5_default.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Camera unit module.
 * PROGRAMMER  : CGSG'2018.
 *               Artem Fabrika.
 * LAST UPDATE : 21.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#include <stdio.h>

/* AF5 default unit struct */
typedef struct tagtngUNIT_AF5_DEFAULT
{
  tngUNIT;         /* Unit base fields */
  DBL 
    Rx,             /* Rotate x angle */
    Ry,             /* Rotate y angle */
    Crx,            /* Camera x angle */
    Cry,            /* Camera y angle */
    Crz,            /* Camera z angle */
    Cx,             /* Camera x move */
    Cy,             /* Camera y move */
    Cz,             /* Camera z move */
    CamDir;         /* Camera directory */
  BOOL 
    IsCamFree,      /* Is camera blocked */
    IsStoppedXUp,   /* Is camera rotate stopped up on X axe */
    IsStoppedXDown; /* Is camera rotate stopped down on X axe */
  VEC CamLoc;       /* Camera location */
  tngPRIM *Prim;       /* Fake primitive to draw */
  tngGEOMETRY *Object; /* Object to load & draw */

} tngUNIT_AF5_DEFAULT;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_AF5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_AF5_DEFAULT *Uni, tngANIM *Ani )
{
  Uni->IsCamFree = TRUE;
} /* End of 'TNG_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_AF5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_AF5_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_AF5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_AF5_DEFAULT *Uni, tngANIM *Ani )
{
  static int flag;         /* Flag for first pass */
  FLT degx;                /* Angle x rotation in degrees */
  DBL kDir, kDirM, CSpeed; /* Distance speed coeffients */

  /* Check if window active */
  if (!Ani->IsActive)
    return;

  /* Exit - Escape */
  if (Ani->Keys[VK_ESCAPE])
  {
    Ani->WinClose();
    exit(0);
  }
  /* Full screen window - Alt + Enter */
  else if (Ani->KeysClick[VK_RETURN] && Ani->Keys[VK_MENU])
    TNG()->WinFlipFullScreen();

  /* Turn wire frame view - Shift + W */
  else if (Ani->KeysClick['W'] && Ani->Keys[VK_SHIFT])
  {
    Ani->IsWireFrame = !Ani->IsWireFrame;
    glPolygonMode(GL_FRONT_AND_BACK,
      (Ani->IsWireFrame) ? GL_LINE : GL_FILL);
    Uni->IsCamFree = FALSE;
  }
  else if (Ani->Keys['W'] && Ani->Keys[VK_SHIFT])
    Uni->IsCamFree = FALSE;

  /* Make screenshot - Ctrl + Shift + S */
  else if (Ani->KeysClick['S'] && Ani->Keys[VK_CONTROL] && Ani->Keys[VK_SHIFT])
  {
    TNG()->PipelineSave();
    Uni->IsCamFree = FALSE;
  }

  /* Pause - P */
  else if (Ani->KeysClick['P'])
    TNG()->IsPause = !TNG()->IsPause;
  else
    Uni->IsCamFree = TRUE;

  /* Count distance speed coeffients */
  kDir = abs(Uni->CamDir) * Ani->GlobalDeltaTime * sqrt(Ani->GlobalDeltaTime) * 16;
    if (abs(kDir) <= 0.001)
      kDir = 0.2;
   kDirM = abs(Uni->CamDir) / 9.0;
    if (abs(kDirM) <= 0.001)
      kDirM = 0.3;

  /* Setup rotation angles and camera direction */
  Uni->IsStoppedXUp = FALSE;
  Uni->IsStoppedXDown = FALSE;
  if (Uni->CamDir == 0)
    CSpeed = 1;
  else
    CSpeed = 7;

  /* Angle check */
  degx = -9 * Uni->Rx + Ani->JY * 90;
  if (degx > 89.99999)
  {
    degx = 89.99999;
    Uni->IsStoppedXDown = TRUE;
  }
  else if (degx < -89.99999)
  {
    degx = -89.99999;
    Uni->IsStoppedXUp = TRUE;
  }
  
  Uni->Ry += (Ani->Mdx * Ani->Keys[VK_LBUTTON] / 4 + Ani->Keys[VK_RIGHT] - Ani->Keys[VK_LEFT]) * Ani->GlobalDeltaTime;
  if (Uni->IsStoppedXUp == TRUE)
    if (Ani->Keys[VK_DOWN] != 0)
      Uni->Rx += (Ani->Mdy * Ani->Keys[VK_LBUTTON] / 4 + Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]) * Ani->GlobalDeltaTime * 3;
    else
      ;
  else
  if (Uni->IsStoppedXDown == TRUE)
    if (Ani->Keys[VK_UP] != 0)
      Uni->Rx += (Ani->Mdy * Ani->Keys[VK_LBUTTON] / 4 + Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]) * Ani->GlobalDeltaTime * 3;
    else
      ;
  else
    Uni->Rx += (Ani->Mdy * Ani->Keys[VK_LBUTTON] / 4 + Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]) * Ani->GlobalDeltaTime * 3;

  if (Uni->IsCamFree == TRUE)
  {
    if (!Ani->Keys[VK_SHIFT])
    {
      Uni->Crx += (Ani->Mdx * Ani->Keys[VK_RBUTTON] / -2 + Ani->Keys['D'] - Ani->Keys['A']) * Ani->GlobalDeltaTime * kDir;
      Uni->Cry += (Ani->Mdy * Ani->Keys[VK_RBUTTON] / 2 + Ani->Keys['Q'] - Ani->Keys['E']) * Ani->GlobalDeltaTime * kDir;
      Uni->Crz += (Ani->Keys['S'] - Ani->Keys['W']) * Ani->GlobalDeltaTime * kDir;
    }
  }
  Uni->CamDir += Ani->Mdz * Ani->GlobalDeltaTime * kDirM + (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]) * kDir;
  Uni->Cx += (Ani->Keys[VK_NUMPAD6] - Ani->Keys[VK_NUMPAD4]) * Ani->GlobalDeltaTime * CSpeed;
  Uni->Cy += (Ani->Keys[VK_NUMPAD8] - Ani->Keys[VK_NUMPAD2]) * Ani->GlobalDeltaTime * CSpeed;
  Uni->Cz += (Ani->Keys[VK_NUMPAD6] - Ani->Keys[VK_NUMPAD4]) * Ani->GlobalDeltaTime * CSpeed;

  if (Uni->CamDir == 0)
    Uni->CamLoc = PointTransform(VecSet(Uni->Crx, Uni->Cry, Uni->Crz + 0.1 + Ani->JZ * 5),
                                 MatrMulMatr(MatrRotateX(degx),
                                             MatrRotateY(-47 * Uni->Ry + Ani->JX * 90)));
  else
    Uni->CamLoc = PointTransform(VecSet(Uni->Crx, Uni->Cry, Uni->Crz + Uni->CamDir * 0.1 + Ani->JZ * 5),
                                 MatrMulMatr(MatrRotateX(degx),
                                             MatrRotateY(-47 * Uni->Ry + Ani->JX * 90)));

  /* Setup camera by angles */
  if (flag == 0)
  {
    Uni->CamLoc = VecSet1(8);
    Uni->CamDir = 150;
    flag = !flag;
  }
  CamSet(&TNG()->Camera,
         Uni->CamLoc,
         VecSet(Uni->Cx + Uni->Crx, Uni->Cy + Uni->Cry, Uni->Cz + Uni->Crz),
         VecSet(0, 1, 0));
} /* End of 'TNG_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_AF5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_AF5_DEFAULT *Uni, tngANIM *Ani )
{
  CHAR Buf[100];
  tngPRIM *Pr;

  sprintf(Buf, "FPS: %.5f", Ani->FPS);
  Pr = Ani->FntStringToPrim(Ani->FntDefault, Buf, Vec4Set(1, 1, 1, 1), 25);

  /* Ani->GeometryDraw(Uni->Object, MatrScale(VecSet1(0.01)));
  Ani->PrimDraw(Uni->Prim, MatrTranslate(VecSet(0, -0.001, 0))); */

  Ani->CSSet(TNG_CS_SCREEN);
  Ani->PrimDrawDeferred(Pr, MatrTranslate(VecSet(18, Ani->FrameH - 18, 0)));
  Ani->CSSet(TNG_CS_WORLD);
  Ani->PrimFree(Pr);
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_AF5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_AF5_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitDefferRender' function */

/* Unit deffered render distance to viewer obtain function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_AF5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT_AF5_DEFAULT *Uni, tngANIM *Ani )
{
  return -1;
} /* End of 'TNG_UnitGetDistance' function */

/* Unit default creation function.
 * ARGUMENTS:
 *   - Animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
tngUNIT * TNG_UnitCreateAF5Default( tngANIM *Ani )
{
  tngUNIT_AF5_DEFAULT *Uni;

  if ((Uni = (tngUNIT_AF5_DEFAULT *)TNG()->UnitCreate(sizeof(tngUNIT_AF5_DEFAULT))) == NULL)
    return NULL;

  /* Memory allocation */
  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;

  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreateAF5Default' function */

/* END OF 'tng_unit_af5_default.c' FILE */
