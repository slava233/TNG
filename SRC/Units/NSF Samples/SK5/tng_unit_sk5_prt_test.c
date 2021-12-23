/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_sk5_default.c
 * PURPOSE     : Tough Nature Graphics project.
 *               SK5 Unit default.
 * PROGRAMMER  : CGSG'2018.
 *               Sofya Kopeykina.
 * LAST UPDATE : 20.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

/* Main discription of default unit */
typedef struct tagtngUNIT_SK5_PRT_TEST
{
  tngUNIT;                /* Unit base fields */
  tngPRIM *EmiPrim;       /* Drawing particles primitive */
  /* Fountain parameters */
  tngEMITTER Emi;         /* Particles generator */
  VEC EmiPos;             /* Emitter position */
  INT EmiStrayAmount;     /* Stray count */
  VEC PrtYCrdSpeed;       /* Start speed direction up */
  FLT PrtStartSpeed;      /* Particles starting speed */
  VEC EmiAclr;            /* Particles acceleration */
  FLT EmiAngVlc;          /* Particles angular velocity */
  FLT EmiScaleFactor;     /* Particles life scale factor */
  /* Place for shot */
  tngPRIM *Place;
  tngPRIM *PlaceTwo;
  MATR WaterTranslate;
  VEC r;
  tngLIGHT *L1;
} tngUNIT_SK5_PRT_TEST;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK5_PRT_TEST *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_SK5_PRT_TEST *Uni, tngANIM *Ani )
{
  tngTOPOLOGY Tpg;
  Ani->CreateHeightField(&Tpg, "BIN/TEXTURES/HeightFields/HF1.BMP",
    VecSet(-25, 0, 25), VecSet(50, 0, 0), VecSet(0, 0, -50), 5);
  Uni->Place = Ani->PrimFromTopo(&Tpg);
  Uni->Place->Mtl = Ani->MaterialAdd("SK5 Test", "Ka(0.1),Kd(0.2),Ks(0.8),Ph(100),Tex(tst_kd.bmp,tst_nm.bmp)");
  Ani->TopoFree(&Tpg);

  Ani->CreatePlane(&Tpg, 50, 50, VecSet(0, 0, 0), VecSet(50, 0, 0), VecSet(0, 0, -50), 0, 0);
  Uni->PlaceTwo = Ani->PrimFromTopo(&Tpg);
  Uni->PlaceTwo->Mtl = Ani->MaterialAdd("SK5 Test water fake", "Ka(0, 0.44, 0.7)");
  Uni->Place->Mtl->Tex[1] = NULL;
  Uni->Place->Mtl->Tex[0] = Ani->TextureAddFromFile("BIN/TEXTURES/HeightFields/HF1TEX.BMP");
  Uni->Place->Mtl->Tex[2] = NULL;
  Uni->Place->Mtl->Tex[3] = NULL;
  Uni->L1 = Ani->LightAdd("Type(2),Dir(10,6,10),Color(1,1,1),Pos(-25,15,25),CutOffDist(30,40),CutOffAngle(40,30),IsShadowCast(0)");
  Ani->TopoFree(&Tpg);

  /* Set fountain parameters */
  TNG()->EmitterCreate(&Uni->Emi);
  Uni->Emi.InjectionSpeed = 100;
  Uni->Emi.PrtLifeTime = 17;

  Uni->EmiPos = VecSet(25, 0, 25);
  Uni->EmiStrayAmount = 10;
  Uni->PrtYCrdSpeed = VecSet(0, 9, 0);
  Uni->PrtStartSpeed = 8;
  Uni->EmiAclr = VecSet(0, -0.8, 0);
  Uni->EmiAngVlc = (MTH_PI / 2) / Uni->Emi.PrtLifeTime;
  Uni->EmiScaleFactor = 1;

  /* Create particles primitive */
  Uni->EmiPrim = Ani->PrimCreate(TNG_PRIM_POINTS, "f3", &Uni->EmiPos, 1, NULL, 0);
  Uni->EmiPrim->Mtl = Ani->MaterialAdd("Particles material", "Tex(BIN//TEXTURES//Particles//DUST_1.tga),Shader(Dust)");
  Uni->r = VecSet(-25, 0.43, 25);
  Uni->WaterTranslate = MatrTranslate(Uni->r);
  Ani->CubeMapCreate("SkyStorm");
} /* End of 'TNG_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK5_PRT_TEST *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_SK5_PRT_TEST *Uni, tngANIM *Ani )
{
  TNG()->EmitterFree(&Uni->Emi);
  Ani->PrimFree(Uni->EmiPrim);
} /* End of 'TNG_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK5_PRT_TEST *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_SK5_PRT_TEST *Uni, tngANIM *Ani )
{
  /*if (Ani->Keys[VK_CONTROL] && Ani->Keys[VK_MENU] && Ani->Keys['U'])
  {
    Uni->r = VecAddVec(Uni->r, VecSet(0, 0.0002, 0));
    Uni->WaterTranslate = MatrTranslate(Uni->r);
  }
  else if (Ani->Keys[VK_CONTROL] && Ani->Keys[VK_MENU] && Ani->Keys['L'])
  {
    Uni->r = VecSubVec(Uni->r, VecSet(0, 0.0002, 0));
    Uni->WaterTranslate = MatrTranslate(Uni->r);
  }*/
  Uni->r = VecAddVec(Uni->r, VecSet(0, 0 * sin(Ani->DeltaTime * 1000) * 0.001, 0));
  Uni->WaterTranslate = MatrTranslate(Uni->r);

  Ani->ParticlesHandleTime(&Uni->Emi, Ani->DeltaTime);
  Ani->UnitRegisterDefferedRender((tngUNIT *)Uni);
} /* End of 'TNG_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK5_PRT_TEST *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_SK5_PRT_TEST *Uni, tngANIM *Ani )
{
  Ani->PrimDraw(Uni->PlaceTwo, Uni->WaterTranslate);
  Ani->PrimDraw(Uni->Place, MatrIdentity());
  Ani->LightDraw(Uni->L1);
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK5_PRT_TEST *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_SK5_PRT_TEST *Uni, tngANIM *Ani )
{
  INT i = 0, j;
  tngPARTICLE *prt = Uni->Emi.ParticleQueue;
  MATR ForwardMovement, RotaryMotion, PositionTranslate, LifeScale;
  FLT PosIdentity = 2 * MTH_PI / Uni->EmiStrayAmount, NextIdentity = (1 / Uni->Emi.PrtLifeTime) * 3;
  VEC PosMas[3] =
  {
    {10, 0, 8},
    {2, 0, -17},
    {-19, 0, -28}
  };

  glBlendFunc(GL_ONE, GL_ONE);

  while (prt != NULL)
  {
    FLT PosAngle = (i % Uni->EmiStrayAmount) * PosIdentity, Time = prt->Age;
    VEC Speed = VecMulNum(VecNormalize(VecSet(cos(PosAngle), Uni->PrtYCrdSpeed.Y, sin(PosAngle))), Uni->PrtStartSpeed);
    
    ForwardMovement = MatrTranslate(VecAddVec(VecMulNum(Speed, Time), VecMulNum(Uni->EmiAclr, Time * Time / 2)));
    RotaryMotion = MatrIdentity();/* MatrRotateY(PosAngle + Uni->EmiAngVlc * Time); */
    LifeScale = MatrScale(VecSet1(NextIdentity * Uni->Emi.ParticleQueue[i].Age));/*MatrIdentity(); /* MatrScale(VecSet1(sin(MTH_PI / (Uni->Emi.ParticleQueue[i].TotalTime - 1) * Time) / Uni->EmiScaleFactor)); */

    for (j = 0; j < 3; j++)
      Ani->PrimDraw(Uni->EmiPrim, MatrMulMatr3(MatrMulMatr3(LifeScale, ForwardMovement, RotaryMotion), MatrTranslate(PosMas[j]), MatrScale(VecSet1(0.05))));
    prt = prt->Next;
    i++;
  }
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  /*Ani->PrimDrawAllDeferred();*/
} /* End of 'TNG_UnitRender' function */

/* Unit particle test creation function.
 * ARGUMENTS: 
 *   - Animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (tngUNIT *) created unit.
 */
tngUNIT * TNG_UnitCreateSK5PrtTest( tngANIM *Ani )
{
  tngUNIT_SK5_PRT_TEST *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_SK5_PRT_TEST *)TNG()->UnitCreate(sizeof(tngUNIT_SK5_PRT_TEST))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;

  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreateSK5PrtTest' function */

/* END OF 'tng_unit_sk5_default.c' FILE */
