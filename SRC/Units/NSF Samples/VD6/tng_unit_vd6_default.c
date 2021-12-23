/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_vd6_default.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit support module.
 * PROGRAMMER  : CGSG'2018.
 *               Dmitrieva Vasilisa.
 * LAST UPDATE : 28.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../../tng.h"

/* Test PD2 unit represenation type */
typedef struct tagtngUNIT_PD2_DEFAULT
{
  tngUNIT;             /* Field which representate unit */
  INT Distance;        /* Distance to viewer */
  FLT
    CamDir,            /* Camera directory */
    RotX,              /* Rotate x angle */
    RotY;              /* Rotate y angle */
  tngGEOMETRY *Object; /* Object to load & draw */
  tngPRIM *Prim;       /* Fake primitive to draw */
} tngUNIT_VD6_DEFAULT;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_VD6_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_VD6_DEFAULT *Uni, tngANIM *Ani )
{
  VEC P[3] = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}};
  INT Ind[3] = {0, 1, 2};

  Uni->Prim = Ani->PrimCreate(TNG_PRIM_TRIMESH, "f3", P, 3, Ind, 3);
} /* End of 'TNG_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_VD6_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_VD6_DEFAULT *Uni, tngANIM *Ani )
{
  Ani->PrimFree(Uni->Prim);
} /* End of 'TNG_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_VD6_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_VD6_DEFAULT *Uni, tngANIM *Ani )
{
  /* Setup fake rotation angles and camera direction */
  Uni->RotY += (Ani->Mdx  * Ani->Keys[VK_LBUTTON] + Ani->Keys[VK_RIGHT] - Ani->Keys[VK_LEFT]) * Ani->GlobalDeltaTime;
  Uni->RotX += (Ani->Mdy  * Ani->Keys[VK_LBUTTON] + Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])* Ani->GlobalDeltaTime;
  Uni->CamDir += Ani->Mdz * Ani->GlobalDeltaTime + Ani->Keys[VK_PRIOR] - Ani->Keys[VK_NEXT];
} /* End of 'TNG_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_VD6_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_VD6_DEFAULT *Uni, tngANIM *Ani )
{
  Ani->PrimDraw(Uni->Prim, MatrIdentity());
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_VD6_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_VD6_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render distance to viewer obtain function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_VD6_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT_VD6_DEFAULT *Uni, tngANIM *Ani )
{
  return -1;
} /* End of 'TNG_UnitGetDistance' function */

/* Unit default creation function.
 * ARGUMENTS: 
 *   - Animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (tngUNIT *) created unit.
 */
tngUNIT * TNG_UnitCreateVD6Default( tngANIM *Ani )
{
  tngUNIT_VD6_DEFAULT *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_VD6_DEFAULT *)TNG()->UnitCreate(sizeof(tngUNIT_VD6_DEFAULT))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;

  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreateVD6Default' function */

/* END OF 'tng_unit_vd6_default.c' FILE  */