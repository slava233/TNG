/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_sk3_primitive.c
 * PURPOSE     : Tough Nature Graphics project.
 *               SK3 Unit default.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kononov.
 * LAST UPDATE : 26.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../../tng.h"

#include <stdio.h>

#define SK3_MAX_PRIMITIVES 4

/* Main structure for unit of this ("tng_unit_sk3_primitive.c") file  */
typedef struct tagtngUNIT_SK3_PRIMITIVE
{
  tngUNIT;                          /* Unit base fields */
  tngPRIM 
    *Primitive[SK3_MAX_PRIMITIVES]; /* Primitive */
  tngGEOMETRY *Object;              /* Object to load & draw */
  VEC Pos;                          /* Aircraft position */
  tngLIGHT *L1;                     /* Light */
  MATR ScaleAirCraft;               /* Model scale matrix */
} tngUNIT_SK3_PRIMITIVE;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK3_PRIMITIVE *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_SK3_PRIMITIVE *Uni, tngANIM *Ani )
{
  tngTOPOLOGY Tpg;

  /* Creating sky */
  Ani->CubeMapCreate("SkyNebula");

  /* Creating light */
  Uni->L1 = Ani->LightAdd("Type(2),Dir(1,2,3),Color(1,1,1),Pos(0, 0, 0),CutOffDist(30,40),CutOffAngle(40,30),IsShadowCast(1)");

  /* Creating height field */
  Ani->CreateHeightField(&Tpg, "BIN/TEXTURES/HeightFields/HF1.BMP",
    VecSet(-25, 0, 25), VecSet(50, 0, 0), VecSet(0, 0, -50), 5);
  Uni->Primitive[0] = Ani->PrimFromTopo(&Tpg);

  Ani->TopoFree(&Tpg);

  /* Applying texture and material to height field */
  Uni->Primitive[0]->Mtl = Ani->MaterialAdd("SK3 Height Field", "Ka(0.1),Kd(0.2),Ks(0.8),Ph(30),Tex(tst_kd.bmp,tst_nm.bmp)");
  Uni->Primitive[0]->Mtl->Tex[0] = Ani->TextureAddFromFile("BIN/TEXTURES/HeightFields/HF1TEX.BMP");
  Uni->Primitive[0]->Mtl->Tex[1] = NULL;
  Uni->Primitive[0]->Mtl->Tex[2] = NULL;
  Uni->Primitive[0]->Mtl->Tex[3] = NULL;

  /* Aircraft loading */
  Uni->Object = Ani->GeometryLoad("BIN/MODELS/ARC170.g3dm");

  /* Scale matrix */
  Uni->ScaleAirCraft = MatrScale(VecSet1(5));

  /* Aircraft position init */
  Uni->Pos = VecSet(0, 3, 0);
} /* End of 'TNG_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK3_PRIMITIVE *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_SK3_PRIMITIVE *Uni, tngANIM *Ani )
{
  INT i;

  for (i = 0; i < SK3_MAX_PRIMITIVES; i++)
    Ani->PrimFree(Uni->Primitive[i]);

  Ani->GeometryDelete(Uni->Object);
} /* End of 'TNG_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK3_PRIMITIVE *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_SK3_PRIMITIVE *Uni, tngANIM *Ani )
{
  /* Driving aircraft */
  Uni->Pos.X += (Ani->Keys['L'] - Ani->Keys['J']) * Ani->GlobalDeltaTime;
  Uni->Pos.Y += (Ani->Keys['I'] - Ani->Keys['K']) * Ani->GlobalDeltaTime;
  Uni->Pos.Z += (Ani->Keys['O'] - Ani->Keys['U']) * Ani->GlobalDeltaTime;
} /* End of 'TNG_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK3_PRIMITIVE *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_SK3_PRIMITIVE *Uni, tngANIM *Ani )
{
  CHAR Buf[100];
  tngPRIM *Pr;

  /* Applying light */
  Ani->LightDraw(Uni->L1);

  /* Drawing heigth field */
  Ani->PrimDraw(Uni->Primitive[0], MatrMulMatr(
                                     MatrTranslate(VecSet(0, 0, 0)),
                                     MatrScale(VecSet1(5))));

  /* Aircraft drawing */
  Ani->GeometryDraw(Uni->Object, MatrMulMatr3(
                                   MatrUnitFit(Uni->Object->Min, Uni->Object->Max),
                                   Uni->ScaleAirCraft,
                                   MatrTranslate(Uni->Pos)));

  /* Creating primitive from string */
  sprintf(Buf, "J X- L X+ K Y- I Y+ U Z- O Z+");
  Pr = Ani->FntStringToPrim(Ani->FntDefault, Buf, Vec4Set(1, 1, 1, 1), 25);

  /* Deferred primitive drawing */
  Ani->CSSet(TNG_CS_SCREEN);
  Ani->PrimDrawDeferred(Pr, MatrTranslate(VecSet(18, Ani->FrameH - 50, 0)));
  Ani->CSSet(TNG_CS_WORLD);

  Ani->PrimFree(Pr);
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK3_PRIMITIVE *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_SK3_PRIMITIVE *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitDefferRender' function */

/* Unit deffered render distance to viewer obtain function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK3_PRIMITIVE *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT_SK3_PRIMITIVE *Uni, tngANIM *Ani )
{
  return -1;
} /* End of 'TNG_UnitGetDistance' function */

/* Unit primitive creation function.
 * ARGUMENTS: 
 *   - Animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (tngUNIT *) created unit.
 */
tngUNIT * TNG_UnitCreateSK3Primitive( tngANIM *Ani )
{
  tngUNIT_SK3_PRIMITIVE *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_SK3_PRIMITIVE *)TNG()->UnitCreate(sizeof(tngUNIT_SK3_PRIMITIVE))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;

  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreateSK3Primitive' function */

/* END OF 'tng_unit_sk3_primitive.c' FILE  */
