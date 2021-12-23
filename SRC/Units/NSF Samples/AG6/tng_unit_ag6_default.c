/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_ag6_default.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit support module.
 * PROGRAMMER  : CGSG'2018.
 *               Alexander Girvits.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../../tng.h"

/* Main structure for unit of this ("tng_unit_ag6_default.c") file  */
typedef struct tagtngUNIT_AG6_DEFAULT
{
  tngUNIT;                /* Field which representate unit */
  INT Distance;           /* Distance to viewer */
  FLT
    CamDir,               /* Camera directory */
    RotX,                 /* Rotate x angle */
    RotY;                 /* Rotate y angle */
  tngGEOMETRY *Object;    /* Object to load & draw */
  tngLIGHT *L1, *L2, *L3; /* Light sources */
  tngPRIM *Prim;          /* Fake primitive to draw */
  tngIMAGE Img;           /* Test image */
} tngUNIT_AG6_DEFAULT;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_AG6_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_AG6_DEFAULT *Uni, tngANIM *Ani )
{
  INT i;
  tngTOPOLOGY Topo;

  Uni->Object = Ani->GeometryLoad("BIN/MODELS/Nissan_Pathfinder.g3dm");
  Ani->CreatePlane(&Topo, 30, 30, VecSet(-25, 0, 25), VecSet(50, 0, 0), VecSet(0, 0, -50), 1, 1);

  for (i = 0; i < Topo.W * Topo.H; i++)
  {
    Topo.V[i].T.X *= 30;
    Topo.V[i].T.Y *= 30;
    Topo.V[i].N = VecSet(0, 1, 0);
    Topo.V[i].Tangent = VecSet(1, 0, 0);
    Topo.V[i].Bitangent = VecSet(0, 0, -1);
  }

  Uni->Prim = Ani->PrimFromTopo(&Topo);
  Ani->TopoFree(&Topo);

  Uni->Prim->Mtl = Ani->MaterialAdd("AG6 Test", "Ka(0.1),Kd(0.2),Ks(0.8),Ph(30),Tex(tst_kd.bmp,tst_nm.bmp)");

  Uni->Prim->Mtl->Tex[0] = NULL;
  Uni->Prim->Mtl->Tex[1] = Ani->TextureAddFromFile("bin/textures/test/pil_nm.bmp");
  Uni->Prim->Mtl->Tex[2] = NULL;
  Uni->Prim->Mtl->Tex[3] = NULL;

  Uni->L1 = Ani->LightAdd("Type(2),Dir(1,2,3),Color(1,1,1),Pos(1,5,4),CutOffDist(30,47),CutOffAngle(4,3),IsShadowCast(0)");
  Uni->L2 = Ani->LightAdd("Type(1),Dir(1,2,3),Color(1,1,1),Pos(0,5,2),CutOffDist(30,47),CutOffAngle(4,3),IsShadowCast(0)");

  TNG()->CubeMapCreate("SkyStorm");
} /* End of 'TNG_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_AG6_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_AG6_DEFAULT *Uni, tngANIM *Ani )
{
  Ani->GeometryDelete(Uni->Object);
  Ani->PrimFree(Uni->Prim);
} /* End of 'TNG_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_AG6_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_AG6_DEFAULT *Uni, tngANIM *Ani )
{

} /* End of 'TNG_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_AG6_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_AG6_DEFAULT *Uni, tngANIM *Ani )
{
  Ani->LightDraw(Uni->L1);
  Ani->LightDraw(Uni->L2);
  Ani->LightDraw(Uni->L3);

  Ani->GeometryDraw(Uni->Object, MatrMulMatr(MatrRotateX(-90), MatrTranslate(VecSet(0, 2.85, 1))));

  Ani->PrimDraw(Uni->Prim, MatrTranslate(VecSet(0, -0.001, 0)));
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_AG6_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_AG6_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render distance to viewer obtain function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_AG6_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT_AG6_DEFAULT *Uni, tngANIM *Ani )
{
  return -1;
} /* End of 'TNG_UnitGetDistance' function */

/* Unit default creation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
tngUNIT * TNG_UnitCreateAG6Default( tngANIM *Ani )
{
  tngUNIT_AG6_DEFAULT *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_AG6_DEFAULT *)TNG()->UnitCreate(sizeof(tngUNIT_AG6_DEFAULT))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;

  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreateAG6Default' function */