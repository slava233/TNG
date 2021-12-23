/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_sk4_target.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit support module.
 *               Sample unit to show lights sources.
 * PROGRAMMER  : CGSG'2018.
 *               Kuralenok Svyatoslav.
 * LAST UPDATE : 17.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../../tng.h"

#include <stdio.h>

/* Test SK4 unit represenation type */
typedef struct tagtngUNIT_SK4_DEFAULT
{
  tngUNIT;                          /* Field which representate unit */
  INT Distance;                     /* Distance to viewer */
  FLT
    CamDir,                         /* Camera directory */
    RotX,                           /* Rotate x angle */
    RotY;                           /* Rotate y angle */
  tngGEOMETRY *Object;              /* Object to load & draw */
  tngLIGHT *L1, *L2, *L3, *L4, *L5; /* Light sources */
  tngPRIM *Prim;                    /* Fake primitive to draw */
  tngIMAGE Img;                     /* Test image */
} tngUNIT_SK4_DEFAULT;

/* Test unit SK4 initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK4_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_SK4_DEFAULT *Uni, tngANIM *Ani )
{
  INT i;
  tngTOPOLOGY Topo;

  Uni->Object = Ani->GeometryLoad("BIN/MODELS/btr.g3dm");
  Ani->CreatePlane(&Topo, 2, 2, VecSet(-25, 0, 25), VecSet(50, 0, 0), VecSet(0, 0, -50), 1, 1);
  for (i = 0; i < Topo.W * Topo.H; i++)
  {
    Topo.V[i].T.X *= 8;
    Topo.V[i].T.Y *= 8;
    Topo.V[i].N = VecSet(0, 1, 0);
    Topo.V[i].Tangent = VecSet(1, 0, 0);
    Topo.V[i].Bitangent = VecSet(0, 0, -1);
  }
  Uni->Prim = Ani->PrimFromTopo(&Topo);
  Ani->TopoFree(&Topo);

  Uni->Prim->Mtl = Ani->MaterialAdd("SK4 Test", "Ka(0.1),Kd(0.1),Ks(0.1),Ph(100),Tex()");
  Uni->Prim->Mtl->Tex[0] = Ani->TextureAddFromFile("bin/textures/test/TRUCK.BMP");
  Uni->Prim->Mtl->Tex[1] = NULL;
  Uni->Prim->Mtl->Tex[2] = NULL;
  Uni->Prim->Mtl->Tex[3] = NULL;
  Uni->L1 = Ani->LightAdd("Type(1),Dir(1,2,3),Color(0.5,0.8,0.8),Pos(2,5,0),CutOffDist(33,33),CutOffAngle(20,25),IsShadowCast(0)");
  Uni->L2 = Ani->LightAdd("Type(1),Dir(1,2,3),Color(0.8,0.5,0.5),Pos(0,5,2),CutOffDist(33,33),CutOffAngle(20,25),IsShadowCast(0)");
  Uni->L3 = Ani->LightAdd("Type(1),Dir(1,2,3),Color(0.5,0.8,0.5),Pos(2,5,2),CutOffDist(33,33),CutOffAngle(20,25),IsShadowCast(0)");
  Uni->L4 = Ani->LightAdd("Type(1),Dir(1,2,3),Color(0.8,0,0.8),Pos(3,4,2),CutOffDist(33,33),CutOffAngle(20,25),IsShadowCast(0)");
  Uni->L5 = Ani->LightAdd("Type(1),Dir(1,2,3),Color(1,1,1),Pos(2,10,2),CutOffDist(33,33),CutOffAngle(20,25),IsShadowCast(0)");
} /* End of 'TNG_UnitInit' function */

/* Test unit SK4 deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK4_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_SK4_DEFAULT *Uni, tngANIM *Ani )
{
  Ani->GeometryDelete(Uni->Object);
  Ani->PrimFree(Uni->Prim);
} /* End of 'TNG_UnitClose' function */

/* Test unit SK4 inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_SK4_DEFAULT *Uni, tngANIM *Ani )
{
  /* Setup fake rotation angles and camera direction */
  Uni->RotY += (Ani->Mdx  * Ani->Keys[VK_LBUTTON] + Ani->Keys[VK_RIGHT] - Ani->Keys[VK_LEFT]) * Ani->GlobalDeltaTime;
  Uni->RotX += (Ani->Mdy  * Ani->Keys[VK_LBUTTON] + Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])* Ani->GlobalDeltaTime;
  Uni->CamDir += Ani->Mdz * Ani->GlobalDeltaTime + Ani->Keys[VK_PRIOR] - Ani->Keys[VK_NEXT];

  /*Uni->L1->Pos.X = 8 * sin(Ani->Time);
  Uni->L2->Pos.Z = 8 * sin(Ani->Time * 1.2 + 8);
  Uni->L3->Pos.X = 9 * sin(Ani->Time * 1.3 + 6);
  Uni->L4->Pos.Z = 4 * cos(Ani->Time * 3.3 + 2);
  Uni->L4->Pos.X = 4 * sin(Ani->Time * 2.3 + 2);
  Uni->L5->Pos.Z = 4 * sin(Ani->Time * 3.3 + 2);
  Uni->L5->Pos.X = 4 * sin(Ani->Time * 3.3 + 2);*/
} /* End of 'TNG_UnitResponse' function */

/* Test unit SK4 render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_SK4_DEFAULT *Uni, tngANIM *Ani )
{
  CHAR Buf[100];
  tngPRIM *Pr;

  /*Ani->LightDraw(Uni->L1);
  Ani->LightDraw(Uni->L2);
  Ani->LightDraw(Uni->L3);
  Ani->LightDraw(Uni->L4);*/
  Ani->LightDraw(Uni->L5);

  sprintf(Buf, "FPS: %.5f", Ani->FPS);
  Pr = Ani->FntStringToPrim(Ani->FntDefault, Buf, Vec4Set(1, 1, 1, 1), 25 * 2);

  ///Ani->GeometryDraw(Uni->Object, MatrScale(VecSet1(0.01)));
  Ani->PrimDraw(Uni->Prim, MatrTranslate(VecSet(0, -0.001, 0)));

  Ani->CSSet(TNG_CS_SCREEN);
  Ani->CSSet(TNG_CS_WORLD);
  Ani->PrimFree(Pr);
} /* End of 'TNG_UnitRender' function */

/* Test unit SK4 deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_SK4_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitRender' function */

/* Test unit SK4 deffered render distance to viewer obtain function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT_SK4_DEFAULT *Uni, tngANIM *Ani )
{
  return -1;
} /* End of 'TNG_UnitGetDistance' function */

/* Test unit SK4 creation function.
 * ARGUMENTS:
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (tngUNIT *) pointer to created unit.
 */
tngUNIT * TNG_UnitCreateSK4Default( tngANIM *Ani )
{
  tngUNIT_SK4_DEFAULT *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_SK4_DEFAULT *)TNG()->UnitCreate(sizeof(tngUNIT_SK4_DEFAULT))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;
  
  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreate' function */

/* END OF 'tng_unit_sk4_target.c' FILE */
