/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_pd2_default.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit support module.
 *               Sample unit to show lights sources.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas.
 * LAST UPDATE : 21.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../../tng.h"

#include <stdio.h>

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
  tngLIGHT *L1, *L2, *L3; /* Light sources */
  tngPRIM *Prim;       /* Fake primitive to draw */
  tngIMAGE Img;        /* Test image */
} tngUNIT_PD2_DEFAULT;

/* Test unit PD2 initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_PD2_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_PD2_DEFAULT *Uni, tngANIM *Ani )
{
  INT i;
  tngTOPOLOGY Topo;
  /* VEC P[3] = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}};
  INT Ind[3] = {0, 1, 2}; */

  Uni->Object = Ani->GeometryLoad("BIN/MODELS/btr.g3dm");
  Ani->CreatePlane(&Topo, 30, 30, VecSet(-25, 0, 25), VecSet(50, 0, 0), VecSet(0, 0, -50), 1, 1);
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

  Uni->Prim->Mtl = Ani->MaterialAdd("PD2 Test", "Ka(0.1),Kd(0.2),Ks(0.8),Ph(30),Tex(tst_kd.bmp,tst_nm.bmp)");
  Uni->Prim->Mtl->Tex[0] = NULL;
  Uni->Prim->Mtl->Tex[1] = Ani->TextureAddFromFile("bin/textures/test/pil_nm.bmp");
  Uni->Prim->Mtl->Tex[2] = NULL;
  Uni->Prim->Mtl->Tex[3] = NULL;
  Uni->L1 = Ani->LightAdd("Type(2),Dir(1,2,3),Color(1,1,1),Pos(2,5,0),CutOffDist(30,47),CutOffAngle(4,3),IsShadowCast(1)");
  /**/
  Uni->L2 = Ani->LightAdd("Type(1),Dir(1,2,3),Color(1,0.5,0.5),Pos(0,5,2),CutOffDist(30,47),CutOffAngle(4,3),IsShadowCast(0)");
  Uni->L3 = TNG()->LightAdd("Type(1),Dir(1,2,3),Color(0.5,1,0.5),Pos(2,5,2),CutOffDist(30,47),CutOffAngle(4,3),IsShadowCast(0)");
  /**/
  Ani->ImageLoad(&Uni->Img, "a");
  TNG()->CubeMapCreate("SkyStd");
} /* End of 'TNG_UnitInit' function */

/* Test unit PD2 deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_PD2_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_PD2_DEFAULT *Uni, tngANIM *Ani )
{
  Ani->GeometryDelete(Uni->Object);
  Ani->PrimFree(Uni->Prim);
} /* End of 'TNG_UnitClose' function */

/* Test unit PD2 inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_PD2_DEFAULT *Uni, tngANIM *Ani )
{
  /* Setup fake rotation angles and camera direction */
  Uni->RotY += (Ani->Mdx  * Ani->Keys[VK_LBUTTON] + Ani->Keys[VK_RIGHT] - Ani->Keys[VK_LEFT]) * Ani->GlobalDeltaTime;
  Uni->RotX += (Ani->Mdy  * Ani->Keys[VK_LBUTTON] + Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])* Ani->GlobalDeltaTime;
  Uni->CamDir += Ani->Mdz * Ani->GlobalDeltaTime + Ani->Keys[VK_PRIOR] - Ani->Keys[VK_NEXT];

  if (Uni->L1 != NULL)
    Uni->L1->Dir = VecNormalize(VecSet(2 * sin(Ani->Time), 1, 2 * cos(Ani->Time)));
  if (Uni->L2 != NULL)
    Uni->L2->Pos.Z = 8 * sin(Ani->Time * 1.2 + 8);
  if (Uni->L3 != NULL)
  {
    Uni->L3->Pos.X = 4 * sin(Ani->Time * 1.0 + 0);
    Uni->L3->Pos.Z = 4 * cos(Ani->Time * 1.0 + 0);
  }

  /* Setup camera by angles */
  /* CamSet(&TNG()->Camera,
    PointTransform(VecSet(0, 0, Uni->CamDir * 0.1 + Ani->JZ * 5), 
    MatrMulMatr(MatrRotateX(-9 * Uni->RotX + Ani->JY * 90), 
                MatrRotateY(-47 * Uni->RotY + Ani->JX * 90))), 
                VecSet(0, 0, 0), VecSet(0, 1, 0)); */
  /* Uni->Lig->Pos = VecSet(8 * sin(TNG()->Time), 0, 3 * sin(TNG()->Time)); */
} /* End of 'TNG_UnitResponse' function */

/* Test unit PD2 render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_PD2_DEFAULT *Uni, tngANIM *Ani )
{
  CHAR Buf[100];
  tngPRIM *Pr;
  /*
  struct
  {
    VEC P;
    VEC2 T;
    VEC4 C;
  } V[4] =
  {
    {{0, 102, 0}, {0, 1}, {1, 1, 1, 1}},
    {{0, 0, 0}, {0, 0}, {1, 1, 1, 1}},
    {{102, 102, 0}, {1, 1}, {1, 1, 1, 1}},
    {{102, 0, 0}, {1, 0}, {1, 1, 1, 1}},
  };
  */

  Ani->LightDraw(Uni->L1);
  Ani->LightDraw(Uni->L2);
  Ani->LightDraw(Uni->L3);

  

  sprintf(Buf, "FPS: %.5f", Ani->FPS);
  Pr = Ani->FntStringToPrim(Ani->FntDefault, Buf, Vec4Set(1, 1, 1, 1), 25);
  /*
  Pr1 = Ani->PrimCreate(TNG_PRIM_STRIP, "f3f2f4", V, 4, NULL, 0);
  Pr1->Mtl = Pr->Mtl;
  */

  Ani->GeometryDraw(Uni->Object, MatrScale(VecSet1(0.01)));
  Ani->PrimDraw(Uni->Prim, MatrTranslate(VecSet(0, -0.001, 0)));

  Ani->CSSet(TNG_CS_SCREEN);
  /* Ani->PrimDrawDeferred(Pr, MatrTranslate(VecSet(18, Ani->FrameH - 18, 0))); */
  Ani->CSSet(TNG_CS_WORLD);
  Ani->PrimFree(Pr);
} /* End of 'TNG_UnitRender' function */

/* Test unit PD2 deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_PD2_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitRender' function */

/* Test unit PD2 deffered render distance to viewer obtain function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT_PD2_DEFAULT *Uni, tngANIM *Ani )
{
  return -1;
} /* End of 'TNG_UnitGetDistance' function */

/* Test unit PD2 creation function.
 * ARGUMENTS:
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (tngUNIT *) pointer to created unit.
 */
tngUNIT * TNG_UnitCreatePD2Default( tngANIM *Ani )
{
  tngUNIT_PD2_DEFAULT *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_PD2_DEFAULT *)TNG()->UnitCreate(sizeof(tngUNIT_PD2_DEFAULT))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;
  
  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreatePD2Default' function */

/* END OF 'tng_unit_pd2_default.c' FILE */
