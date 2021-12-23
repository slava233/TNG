/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_eh5_default.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Default unit module.
 * PROGRAMMER  : CGSG'2018.
 *               Evgenii Hlevnoi.
 * LAST UPDATE : 26.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

/* EH5 default unit type */
typedef struct tagtngUNIT_EH5_DEFAULT
{
  tngUNIT;                /* Unit type field */
  INT Distance;           /* Distance to viewer */
  tngGEOMETRY *Nature[3]; /* Geometry objects pointers massive */
  tngPRIM *Prim;          /* Primitive to draw */
  tngTOPOLOGY Topo;       /* Topology object to draw */
  FLT
    CamDir,               /* Camera directory */
    RotX,                 /* Rotate x angle */
    RotY;                 /* Rotate y angle */
  tngLIGHT *Ligs[11];     /* Light sources */
} tngUNIT_EH5_DEFAULT;

/* EH5 unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_EH5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_EH5_DEFAULT *Uni, tngANIM *Ani )
{
  INT i;

  Uni->Nature[0] = Ani->GeometryLoad("BIN/MODELS/wood.g3dm");
  Uni->Nature[1] = Ani->GeometryLoad("BIN/MODELS/Palm.g3dm");
  Uni->Nature[2] = Ani->GeometryLoad("BIN/MODELS/tree.g3dm");

  Ani->CreatePlane(&Uni->Topo, 30, 30, VecSet(-200, 0, 200), VecSet(400, 0, 0), VecSet(0, 0, -400),
    0.5, 0.5);
  for (i = 0; i < Uni->Topo.W * Uni->Topo.H; i++)
  {
    Uni->Topo.V[i].T.X *= 8;
    Uni->Topo.V[i].T.Y *= 8;
    Uni->Topo.V[i].N = VecSet(0, 1, 0);
    Uni->Topo.V[i].Tangent = VecSet(1, 0, 0);
    Uni->Topo.V[i].Bitangent = VecSet(0, 0, -1);
  }
  Uni->Prim = Ani->PrimFromTopo(&Uni->Topo);
  Ani->TopoFree(&Uni->Topo);

  Uni->Prim->Mtl = Ani->MaterialAdd("EH5 Test", "Ka(0.1),Kd(0.1),Ks(0.1),Ph(1000),Tex(tst_kd.bmp,tst_nm.bmp)");
  Uni->Prim->Mtl->Tex[0] = Ani->TextureAddFromFile("bin/textures/test/eh5_sand.bmp");
  Uni->Prim->Mtl->Tex[1] = Ani->TextureAddFromFile("bin/textures/test/eh5_sand_nm.bmp");
  Uni->Prim->Mtl->Tex[2] = NULL;
  Uni->Prim->Mtl->Tex[3] = NULL;
  
  Uni->Ligs[0] = Ani->LightAdd("Type(1),Dir(1,2,3),Color(0.5,0.3,0.4),Pos(-8,20,-10),CutOffDist(300,400),CutOffAngle(20,30),IsShadowCast(1)");
  Uni->Ligs[1] = Ani->LightAdd("Type(1),Dir(1,2,3),Color(0.2,0.4,0.3),Pos(7,20,4),CutOffDist(300,400),CutOffAngle(20,25),IsShadowCast(1)");
  Uni->Ligs[2] = Ani->LightAdd("Type(1),Dir(1,2,3),Color(0.4,0.5,0.3),Pos(5,20,5),CutOffDist(300,400),CutOffAngle(20,25),IsShadowCast(1)");
  Uni->Ligs[3] = Ani->LightAdd("Type(2),Dir(0,0,0),Color(0.07,0.08,0.081),Pos(-1000,500,-1000),CutOffAngle(60,80),IsShadowCast(1)");
  Uni->Ligs[4] = Ani->LightAdd("Type(2),Dir(0,0,0),Color(0.07,0.08,0.081),Pos(0,500,-1000),CutOffAngle(60,80),IsShadowCast(1)");
  Uni->Ligs[5] = Ani->LightAdd("Type(2),Dir(0,0,0),Color(0.07,0.08,0.081),Pos(1000,500,-1000),CutOffAngle(60,80),IsShadowCast(1)");
  Uni->Ligs[6] = Ani->LightAdd("Type(2),Dir(0,0,0),Color(0.07,0.08,0.081),Pos(1000,500,0),CutOffAngle(60,80),IsShadowCast(1)");
  Uni->Ligs[7] = Ani->LightAdd("Type(2),Dir(0,0,0),Color(0.07,0.08,0.081),Pos(1000,500,1000),CutOffAngle(60,80),IsShadowCast(1)");
  Uni->Ligs[8] = Ani->LightAdd("Type(2),Dir(0,0,0),Color(0.07,0.08,0.081),Pos(0,500,1000),CutOffAngle(60,80),IsShadowCast(1)");
  Uni->Ligs[9] = Ani->LightAdd("Type(2),Dir(0,0,0),Color(0.07,0.08,0.081),Pos(-1000,500,1000),CutOffAngle(60,80),IsShadowCast(1)");
  Uni->Ligs[10] = Ani->LightAdd("Type(2),Dir(0,0,0),Color(0.07,0.08,0.081),Pos(-1000,500,0),CutOffAngle(60,80),IsShadowCast(1)");

  TNG()->CubeMapCreate("SkyLagoon");
} /* End of 'TNG_UnitInit' function */

/* EH5 unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_EH5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_EH5_DEFAULT *Uni, tngANIM *Ani )
{
  INT i;

  for (i = 0; i < 3; i++)
    Ani->GeometryDelete(Uni->Nature[i]);

  Ani->PrimFree(Uni->Prim);
} /* End of 'TNG_UnitClose' function */

/* EH5 unit response function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_EH5_DEFAULT *Uni, tngANIM *Ani )
{
  /* Setup fake rotation angles and camera direction */
  Uni->RotY += (Ani->Mdx  * Ani->Keys[VK_LBUTTON] + Ani->Keys[VK_RIGHT] - Ani->Keys[VK_LEFT]) * Ani->GlobalDeltaTime;
  Uni->RotX += (Ani->Mdy  * Ani->Keys[VK_LBUTTON] + Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])* Ani->GlobalDeltaTime;
  Uni->CamDir += Ani->Mdz * Ani->GlobalDeltaTime + Ani->Keys[VK_PRIOR] - Ani->Keys[VK_NEXT];

  /* Light source move */
  Uni->Ligs[0]->Pos.X = 10 * sin(Ani->Time);
  Uni->Ligs[0]->Pos.Z = 12 * cos(Ani->Time - 1.5);
  Uni->Ligs[1]->Pos.X = 13 * cos(Ani->Time);
  Uni->Ligs[1]->Pos.Z = 17 * sin(Ani->Time + 1);
  Uni->Ligs[2]->Pos.X = 14 * cos(Ani->Time - 2.5);
  Uni->Ligs[2]->Pos.Z = 20 * sin(Ani->Time + 8);

  /* Setup camera by angles */

  /*
  CamSet(&TNG_Anim.Camera,
    PointTransform(VecSet(0, 0, Uni->CamDir * 0.1 + Ani->JZ * 5),
    MatrMulMatr(MatrRotateX(-9 * Uni->RotX + Ani->JY * 90),
                MatrRotateY(-47 * Uni->RotY + Ani->JX * 90))),
                VecSet(0, 0, 0), VecSet(0, 1, 0));
  */
  
  /* Registrate unit to deferred draw */
  /* Ani->UnitRegisterDefferedRender((tngUNIT *)Uni); */
} /* End of 'TNG_UnitResponse' function */

/* EH5 unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_EH5_DEFAULT *Uni, tngANIM *Ani )
{
  INT i, j;
  DBL Scheme[5][5] =
  {
    {-1, 0.5, 0, 0.3, 0.9},
    {0, 0.7, 0, 0.2, 0},
    {0.6, 0, 0.7, 0, 0.6},
    {0, 0.4, 0.1, 0.6, 0.2},
    {-1, 0.6, 0, 0.8, -1}
  };

  for (i = 0; i < 11; i++)
    Ani->LightDraw(Uni->Ligs[i]);

  for (i = 0; i < 5; i++)
    for (j = 0; j < 5; j++)
      if (Scheme[i][j] != 0 && Scheme[i][j] != -1)
        Ani->GeometryDraw(Uni->Nature[1], MatrMulMatr3(MatrRotateY(-90),
          MatrScale(VecSet1(Scheme[i][j] * 4)), MatrTranslate(VecSet(i * 30 - 75, 0.0012, j * 35 - 75))));
      else if (Scheme[i][j] == -1)
        Ani->GeometryDraw(Uni->Nature[0], MatrMulMatr3(MatrRotateY(-90), MatrScale(VecSet1(5)),
          MatrTranslate(VecSet(i * 30 - 75, 1, j * 35 - 75))));

  Ani->PrimDraw(Uni->Prim, MatrIdentity());
} /* End of 'TNG_UnitRender' function */

/* EH5 unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_EH5_DEFAULT *Uni, tngANIM *Ani )
{
  INT i, j;
  DBL Scheme[5][5] =
  {
    {0, 0.5, 0, 0.3, 0.9},
    {0, 0.7, 0, 0.2, 0},
    {0.6, 0, 0.7, -1, 0.6},
    {0, 0.4, 0.1, 0.6, 0.2},
    {0, 0.6, 0, 0.8, 0}
  };

  for (i = 0; i < 5; i++)
    for (j = 0; j < 5; j++)
      if (Scheme[i][j] == -1)
        Ani->GeometryDrawDeferred(Uni->Nature[1], MatrMulMatr(MatrMulMatr(MatrScale(VecSet1(Scheme[i][j])),
          MatrRotateY(-90)), MatrTranslate(VecSet(i * 9 - 15, 0.0012, j * 10 - 15))));
} /* End of 'TNG_UnitRender' function */

/* EH5 unit get distance to viewer function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT_EH5_DEFAULT *Uni, tngANIM *Ani )
{
  return -1;
} /* End of 'TNG_UnitGetDistance' function */

/* EH5 unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (tngUNIT *) pointer to created unit.
 */
tngUNIT_EH5_DEFAULT * TNG_UnitCreateEH5Default( VOID )
{
  tngUNIT_EH5_DEFAULT *Uni;

  if ((Uni = (tngUNIT_EH5_DEFAULT *)TNG_Anim.UnitCreate(sizeof(tngUNIT_EH5_DEFAULT))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;

  return Uni;
} /* End of 'TNG_UnitCreateEH5Default' function */

/* END OF 'tng_unit_eh5_default.c' FILE */
