/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_vg4_helic.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit sample module.
 *               Test unit VG4 helic module.
 * PROGRAMMER  : CGSG'2018.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 18.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../../tng.h"

#include <stdio.h>

/* Test unit VG4 helic represenation type */
typedef struct tagtngUNIT_VG4_HELIC
{
  tngUNIT;             /* Field which representate unit */
  tngGEOMETRY *Heli;   /* Helicopter object */
  VEC
    CamPos,            /* Viewer position */
    Pos,               /* Helicopter position */
    ViewShift;         /* POI shift position */
  FLT
    WingR,             /* Wing rotation angle */
    Head,              /* Helicopter head direction angle */
    V,                 /* Helicopter linear velocity */
    A;                 /* Helicopter linear acceleration */
} tngUNIT_VG4_HELIC;

/* Test unit VG4 helic initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_VG4_HELIC *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_VG4_HELIC *Uni, tngANIM *Ani )
{
  Uni->Heli = Ani->GeometryLoad("BIN/MODELS/UH60.g3dm");
  if (Uni->Heli != NULL)
    Uni->Heli->Trans = MatrMulMatr4(MatrRotateZ(-90),
                                    MatrUnitFitCenterZ(Uni->Heli->Min, Uni->Heli->Max),
                                    MatrRotateX(-90),
                                    MatrScale(VecSet1(5)));
  Uni->V = 0;
  Uni->A = 0;
  Uni->Head = 0;
  Uni->WingR = 0;

  Uni->CamPos = VecSet(10, 10, 10);
  Uni->Pos = VecSet(0, 0, 0);
  Uni->ViewShift = VecSet(-8, 8, 0);
} /* End of 'TNG_UnitInit' function */

/* Test unit VG4 helic deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_VG4_HELIC *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_VG4_HELIC *Uni, tngANIM *Ani )
{
  Ani->GeometryDelete(Uni->Heli);
} /* End of 'TNG_UnitClose' function */

/* Test unit VG4 helic inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_VG4_HELIC *Uni, tngANIM *Ani )
{
  VEC cam_pos;
  MATR mrot;

  if (Uni->Heli == NULL)
    return;

  Uni->Heli->Prims[11]->Trans =
    MatrMulMatr3(MatrTranslate(VecDivNum(VecAddVec(Uni->Heli->Prims[11]->Min, Uni->Heli->Prims[11]->Max), -2)),
                 MatrRotateX(8 * 90 * Ani->Time),
                 MatrTranslate(VecDivNum(VecAddVec(Uni->Heli->Prims[11]->Min, Uni->Heli->Prims[11]->Max), 2)));

  Uni->ViewShift.X += 8 * (Ani->Keys['F'] - Ani->Keys['G']) * Ani->DeltaTime;
  Uni->ViewShift.Y += 8 * (Ani->Keys['T'] - Ani->Keys['V']) * Ani->DeltaTime;
  Uni->ViewShift.Z += 8 * (Ani->Keys['R'] - Ani->Keys['C']) * Ani->DeltaTime;

  Uni->WingR += 102 * (30 * Uni->V + Uni->Pos.Y) * Ani->DeltaTime;
  Uni->Heli->Prims[12]->Trans = MatrRotateZ(Uni->WingR);
  Uni->Heli->Prims[13]->Trans = MatrRotateZ(Uni->WingR);

  Uni->Head += 50 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]) * Ani->DeltaTime;
  Uni->Pos.Y += (Ani->Keys[VK_PRIOR] - Ani->Keys[VK_NEXT]) * Ani->DeltaTime;

  mrot = MatrRotateY(Uni->Head);
  Uni->A = 8 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]) * Ani->DeltaTime;
  Uni->V += Uni->A * Ani->DeltaTime;
  Uni->Pos = VecAddVec(Uni->Pos, VecMulMatr(VecSet(Uni->V, 0, 0), mrot));

  cam_pos = VecAddVec(Uni->Pos, VecMulMatr(Uni->ViewShift, mrot));
  Uni->CamPos = VecAddVec(Uni->CamPos, VecMulNum(VecSubVec(cam_pos, Uni->CamPos), Ani->DeltaTime));
  CamSet(&TNG()->Camera, Uni->CamPos, Uni->Pos, VecSet(0, 1, 0));
  /* Setup camera by angles */
  /* CamSet(&TNG()->Camera,
    PointTransform(VecSet(0, 0, Uni->CamDir * 0.1 + Ani->JZ * 5), 
    MatrMulMatr(MatrRotateX(-9 * Uni->RotX + Ani->JY * 90), 
                MatrRotateY(-47 * Uni->RotY + Ani->JX * 90))), 
                VecSet(0, 0, 0), VecSet(0, 1, 0)); */
  /* Uni->Lig->Pos = VecSet(8 * sin(TNG()->Time), 0, 3 * sin(TNG()->Time)); */
} /* End of 'TNG_UnitResponse' function */

/* Test unit VG4 helic render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_VG4_HELIC *Uni, tngANIM *Ani )
{
  MATR
    mrot,
    m =
      MatrMulMatr5(MatrTranslate(VecSet(0, -4.7, 0)),
                   MatrRotateX(47 * sin(Ani->Time)),
                   MatrRotateZ(47 * sin(Ani->Time * 1.2)),
                   MatrScale(VecSet1(1.5)),
                   MatrTranslate(VecSet(0, 8.5, 0)));

  mrot = MatrRotateY(Uni->Head);
  Ani->GeometryDraw(Uni->Heli, MatrMulMatr3(MatrRotateZ(-Uni->V * 300), mrot, MatrTranslate(Uni->Pos)));
} /* End of 'TNG_UnitRender' function */

/* Test unit VG4 helic deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_VG4_HELIC *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitRender' function */

/* Test unit VG4 helic deffered render distance to viewer obtain function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT_VG4_HELIC *Uni, tngANIM *Ani )
{
  return -1;
} /* End of 'TNG_UnitGetDistance' function */

/* Test unit VG4 helic creation function.
 * ARGUMENTS:
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (tngUNIT *) pointer to created unit.
 */
tngUNIT * TNG_UnitCreateVG4Helic( tngANIM *Ani )
{
  tngUNIT_VG4_HELIC *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_VG4_HELIC *)TNG()->UnitCreate(sizeof(tngUNIT_VG4_HELIC))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;
  
  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreate' function */

/* END OF 'tng_unit_vg4_helic.c' FILE */
