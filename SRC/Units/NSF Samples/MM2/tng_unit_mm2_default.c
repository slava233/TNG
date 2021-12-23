/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_mm2_default.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit support module.
 * PROGRAMMER  : CGSG'2018.
 *               Markov Maxim.
 * LAST UPDATE : 21.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../../tng.h"

/* MM2 test structure for unit */
typedef struct tagtngUNIT_MM2_DEFAULT
{
  tngUNIT;                                                                      /* Unit base fields */
  tngPRIM *PrimitiveTorus, *PrimitiveTorus1, *PrimitiveSphere, *PrimitivePlane; /* Primitives */
  tngGEOMETRY *Car;                                                             /* Geometry objects pointers massive */
  VEC CarPosition;                                                               /* Car position */
  FLT
    CamDir,                                                                     /* Camera directory */
    RotX,                                                                       /* Rotate x angle */
    RotY;                                                                       /* Rotate y angle */
  INT ID;                                                                       /* Unit id */
  tngLIGHT *L1;                                                                 /* Light source */
} tngUNIT_MM2_DEFAULT;

/* MM2 test unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_MM2_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_MM2_DEFAULT *Uni, tngANIM *Ani )
{
  tngTOPOLOGY Sphere, Torus, Torus1, Plane;

  Ani->CreateTorus(&Torus, VecSet(0, 1, 0), 0.5, 0.2, 30, 18);
  Uni->PrimitiveTorus = Ani->PrimFromTopo(&Torus);
  Ani->CreateSphere(&Sphere, VecSet(0, 1, 0), 0.1, 30, 18);
  Uni->PrimitiveSphere = Ani->PrimFromTopo(&Sphere);
  Ani->CreatePlane(&Torus1, 10, 10, VecSet(-2.5, 0.05, 2.5), VecSet(5, 0, 0), VecSet(0, 0, -5), 10, 10);
  Uni->PrimitiveTorus1 = Ani->PrimFromTopo(&Torus1);
  Ani->CreateHeightField(&Plane, "BIN/TEXTURES/HeightFields/HF1.bmp", VecSet(-2.5, 0, 2.5), VecSet(5, 0, 0), VecSet(0, 0, -5), 0.5);
  Uni->PrimitivePlane = Ani->PrimFromTopo(&Plane);
  Uni->PrimitivePlane->Mtl = Ani->MaterialAdd("MM2 Test", "Ka(0.1,0.1,0.1),Kd(0.2,0.2,0.2),Ks(0.8,0.8,0.8),Ph(30),Tex(tst_kd.bmp,tst_nm.bmp)");
  Uni->PrimitivePlane->Mtl->Tex[0] = Ani->TextureAddFromFile("bin/textures/HeightFields/HF1TEX.bmp");
  Uni->PrimitivePlane->Mtl->Tex[1] = NULL;
  Uni->PrimitivePlane->Mtl->Tex[2] = NULL;
  Uni->PrimitivePlane->Mtl->Tex[3] = NULL;
  Uni->PrimitiveTorus1->Mtl = Ani->MaterialAdd("MM2 Test", "Ka(0.3,0.5,0.7),Kd(0.2,0.2,0.2),Ks(0.8,0.8,0.8),Ph(30),Tex(tst_kd.bmp,tst_nm.bmp)");
  Uni->PrimitiveTorus->Mtl = Ani->MaterialAdd("MM2 Test", "Ka(0.1,0.1,0.1),Kd(0.2,0.2,0.2),Ks(0.8,0.8,0.8),Ph(30),Tex(tst_kd.bmp,tst_nm.bmp)");
  Uni->PrimitiveTorus->Mtl->Tex[0] = Ani->TextureAddFromFile("bin/textures/HeightFields/Brick.bmp");
  Uni->PrimitiveTorus->Mtl->Tex[1] = NULL;
  Uni->PrimitiveTorus->Mtl->Tex[2] = NULL;
  Uni->PrimitiveTorus->Mtl->Tex[3] = NULL;
  
  Uni->PrimitiveSphere->Mtl = Ani->MaterialAdd("MM2 Test", "Ka(0.1,0.1,0.1),Kd(0.2,0.2,0.2),Ks(0.8,0.8,0.8),Ph(30),Tex(tst_kd.bmp,tst_nm.bmp)");
  Uni->PrimitiveSphere->Mtl->Tex[0] = Ani->TextureAddFromFile("bin/textures/HeightFields/soccer.png");
  Uni->PrimitiveSphere->Mtl->Tex[1] = NULL;
  Uni->PrimitiveSphere->Mtl->Tex[2] = NULL;
  Uni->PrimitiveSphere->Mtl->Tex[3] = NULL;
  Uni->L1 = Ani->LightAdd("Type(2),Dir(1,2,3),Color(0.3,0.5,0.5),Pos(0, 1, 0.7),CutOffDist(300,400),CutOffAngle(40,30),IsShadowCast(0)");
  Ani->Camera.Loc = VecSet(1, 2, 1);
  Ani->CubeMapCreate("SkyNebula");
  /* Free topology */
  Ani->TopoFree(&Torus);
  Ani->TopoFree(&Torus1);
  Ani->TopoFree(&Sphere);
  Ani->TopoFree(&Plane);
} /* End of 'TNG_UnitInit' function */

/* MM2 test unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_MM2_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_MM2_DEFAULT *Uni, tngANIM *Ani )
{
  Ani->PrimFree(Uni->PrimitiveSphere);
  Ani->PrimFree(Uni->PrimitivePlane);
  Ani->PrimFree(Uni->PrimitiveTorus);
  Ani->PrimFree(Uni->PrimitiveTorus1);
} /* End of 'TNG_UnitClose' function */

/* MM2 test unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_MM2_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_MM2_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitResponse' function */

/* MM2 test unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_MM2_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_MM2_DEFAULT *Uni, tngANIM *Ani )
{
  Ani->PrimDraw(Uni->PrimitiveSphere, MatrTranslate(VecSet(2 * sin(Ani->Time), 1, cos(Ani->Time * 1.3))));
  Ani->PrimDraw(Uni->PrimitiveTorus1, MatrIdentity());
  Ani->PrimDraw(Uni->PrimitiveTorus, MatrTranslate(VecSet(-2 * sin(Ani->Time), 1, -cos(Ani->Time * 1.3))));
  Ani->PrimDraw(Uni->PrimitivePlane, MatrIdentity());
  Ani->LightDraw(Uni->L1);
} /* End of 'TNG_UnitRender' function */

/* MM2 test Unit topology default creation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
tngUNIT * TNG_UnitCreateMM2Default( tngANIM *Ani )
{
  tngUNIT_MM2_DEFAULT *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_MM2_DEFAULT *)Ani->UnitCreate(sizeof(tngUNIT_MM2_DEFAULT))) == NULL)
    return (tngUNIT *)Uni;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;

  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreateMM2Default' function */

/* END OF 'tng_unit_MM2_default.c' FILE */
