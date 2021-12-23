/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_as4_default.c
 * PURPOSE     : Tough Nature Graphics project.
 *               AS4 example unit module.
 * PROGRAMMER  : CGSG'2018.
 *               Andrey Shayda.
 * LAST UPDATE : 26.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

/* AS4 default unit structure */
typedef struct tagtngUNIT_AS4_DEFAULT
{
  tngUNIT;                                                    /* Unit base fields */
  tngTOPOLOGY Sphere;                                         /* Topology object */
  tngLIGHT *Lig;                                              /* Light structure */
  tngPRIM *PrimitiveEarth, *PrimitiveRocks[4];                /* Primitives */
  FLT
    CamDir,                                                   /* Camera directory */
    RotX,                                                     /* Rotate x angle */
    RotY;                                                     /* Rotate y angle */
  INT ID;                                                     /* Unit id */
} tngUNIT_AS4_DEFAULT;

/* AS4 default unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_AS4_DEFAULT *Uni, tngANIM *Ani )
{
  Ani->CreateSphere(&Uni->Sphere, VecSet(0, 0, 0), 5, 300, 180);
  Uni->PrimitiveEarth = Ani->PrimFromTopo(&Uni->Sphere);
  Uni->PrimitiveEarth->Mtl = Ani->MaterialAdd("Earth material", "Tex(BIN/TEXTURES/Solar_sample/Earth.bmp)");
  Ani->TopoFree(&Uni->Sphere);
  Ani->CreateSphere(&Uni->Sphere, VecSet(0, 0, 0), 2, 60, 36);
  Uni->PrimitiveRocks[0] = Ani->PrimFromTopo(&Uni->Sphere);
  Uni->PrimitiveRocks[1] = Ani->PrimFromTopo(&Uni->Sphere);
  Uni->PrimitiveRocks[2] = Ani->PrimFromTopo(&Uni->Sphere);
  Uni->PrimitiveRocks[3] = Ani->PrimFromTopo(&Uni->Sphere);
  Uni->PrimitiveRocks[0]->Mtl = Ani->MaterialAdd("Asteroid material-1", 
    "Ka(1),Kd(1),Ks(1),Tex(BIN/TEXTURES/Solar_sample/mars.bmp)");
  Uni->PrimitiveRocks[1]->Mtl = Ani->MaterialAdd("Asteroid material-2", 
    "Ka(1),Kd(1),Ks(1),Tex(BIN/TEXTURES/Solar_sample/jupiter.bmp)");
  Uni->PrimitiveRocks[2]->Mtl = Ani->MaterialAdd("Asteroid material-3", 
    "Ka(1),Kd(1),Ks(1),Tex(BIN/TEXTURES/Solar_sample/neptun.bmp)");
  Uni->PrimitiveRocks[3]->Mtl = Ani->MaterialAdd("Asteroid material-4", 
    "Ka(1),Kd(1),Ks(1),Tex(BIN/TEXTURES/Solar_sample/moon.bmp)");
  Uni->Lig = Ani->LightAdd("Type(1),Color(1,1,1),Pos(25,25,25),CutOffDist(100,180),CutOffAngle(4,3),IsShadowCast(0)");
  Ani->CubeMapCreate("SkyNebula");
} /* End of 'TNG_UnitInit' function */

/* AS4 default unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_AS4_DEFAULT *Uni, tngANIM *Ani )
{
  Ani->TopoFree(&Uni->Sphere);
  Ani->PrimFree(Uni->PrimitiveEarth);
  Ani->PrimFree(Uni->PrimitiveRocks[0]);
  Ani->PrimFree(Uni->PrimitiveRocks[1]);
  Ani->PrimFree(Uni->PrimitiveRocks[2]);
  Ani->PrimFree(Uni->PrimitiveRocks[3]);
} /* End of 'TNG_UnitClose' function */

/* AS4 default unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_AS4_DEFAULT *Uni, tngANIM *Ani )
{

} /* End of 'TNG_UnitResponse' function */

/* AS4 default unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_AS4_DEFAULT *Uni, tngANIM *Ani )
{
  MATR M = MatrMulMatr(MatrRotateY(-30 * Ani->Time), MatrRotateZ(10));
  
  Ani->LightDraw(Uni->Lig);
  Ani->PrimDraw(Uni->PrimitiveEarth, M);
  M = MatrMulMatr3(MatrRotateY(-10 * Ani->Time), MatrTranslate(VecSet(15, 0, 0)), MatrRotateY(10 * Ani->Time));
  Ani->PrimDraw(Uni->PrimitiveRocks[0], M);
  M = MatrMulMatr3(MatrRotateZ(-47 * Ani->Time), MatrTranslate(VecSet(12, 12, 0)), MatrRotateZ(47 * Ani->Time));
  Ani->PrimDraw(Uni->PrimitiveRocks[1], M);
  M = MatrMulMatr3(MatrRotateX(-100 * Ani->Time), MatrTranslate(VecSet(0, 11, 0)), MatrRotateX(100 * Ani->Time));
  Ani->PrimDraw(Uni->PrimitiveRocks[2], M);
  M = MatrMulMatr3(MatrRotateY(-19 * Ani->Time), MatrTranslate(VecSet(0, 0, 30)), MatrRotateY(19 * Ani->Time));
  Ani->PrimDraw(Uni->PrimitiveRocks[3], M);
} /* End of 'TNG_UnitRender' function */

/* AS4 default unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_AS4_DEFAULT *Uni, tngANIM *Ani )
{

} /* End of 'TNG_UnitRender' function */

/* AS4 default unit creation function.
 * ARGUMENTS:
 *   - anim structure pointer:
 *       tngANIM *Ani;
 * RETURNS:
 *   (tngUNIT *) pointer to created unit.
 */
tngUNIT_AS4_DEFAULT * TNG_UnitCreateAS4Default( tngANIM *Ani )
{
  tngUNIT_AS4_DEFAULT *Uni;

  if ((Uni = (tngUNIT_AS4_DEFAULT *)TNG_Anim.UnitCreate(sizeof(tngUNIT_AS4_DEFAULT))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  
  return Uni;
} /* End of 'TNG_UnitCreate' function */

/* END OF 'tng_unit_as4_default.c' FILE  */