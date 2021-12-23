/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_lighting_demo.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit support module.
 *               Lighting demonstration unit.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas.
 * LAST UPDATE : 20.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../tng.h"

#include <stdio.h>

/* Lighting demonstration unit represenation type */
typedef struct tagtngUNIT_LIGHTING_DEMO
{
  tngUNIT;                /* Field which representate unit */
  tngGEOMETRY *Object;    /* Object to load & draw */
  tngLIGHT *RL, *GL, *BL; /* Light sources(red, green and blue light sources) */
  tngPRIM *Plane;         /* Fake primitive for plane */
  tngTOPOLOGY Topo;       /* Topology for plane */
  tngIMAGE Img;           /* Test image */
} tngUNIT_LIGHTING_DEMO;

/* Lighting demonstration unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_LIGHTING_DEMO *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_LIGHTING_DEMO *Uni, tngANIM *Ani )
{
  INT i;

  /* Load 'G3DM' object */
  Uni->Object = Ani->GeometryLoad("BIN/MODELS/Nissan_Pathfinder.g3dm");
  /* Create topology(plane) */ 
  Ani->CreatePlane(&Uni->Topo, 30, 30, VecSet(-25, 0, 25), VecSet(50, 0, 0), VecSet(0, 0, -50), 1, 1);
  /* Fix plane parameters */
  for (i = 0; i < Uni->Topo.W * Uni->Topo.H; i++)
  {
    Uni->Topo.V[i].T.X *= 8;
    Uni->Topo.V[i].T.Y *= 8;
    Uni->Topo.V[i].N = VecSet(0, 1, 0);
    Uni->Topo.V[i].Tangent = VecSet(1, 0, 0);
    Uni->Topo.V[i].Bitangent = VecSet(0, 0, -1);
  }
  /* Generate primitive from topology */
  Uni->Plane = Ani->PrimFromTopo(&Uni->Topo);
  /* Free topology */
  Ani->TopoFree(&Uni->Topo);

  /* Add normal map to plane */
  Uni->Plane->Mtl = Ani->MaterialAdd("Lighting Demo", "Ka(0.1,0.1,0.1),Kd(0.2,0.2,0.2),Ks(0.8,0.8,0.8),Ph(30),Tex(tst_kd.bmp,tst_nm.bmp)");
  Uni->Plane->Mtl->Tex[0] = Ani->TextureAddFromFile("BIN/TEXTURES/Test/chess_txt.png");
  Uni->Plane->Mtl->Tex[1] = Ani->TextureAddFromFile("BIN/TEXTURES/Test/chess_nm.tga");
  Uni->Plane->Mtl->Tex[2] = NULL;
  Uni->Plane->Mtl->Tex[3] = NULL;
  /* Add light sources */
  Uni->RL = Ani->LightAdd("Type(2),Dir(1,2,3),Color(1,0,0),Pos(5,5,0),CutOffDist(12,17),CutOffAngle(4,3),IsShadowCast(0)");
  Uni->GL = Ani->LightAdd("Type(2),Dir(1,1,5),Color(0,1,0),Pos(0,5,4),CutOffDist(12,17),CutOffAngle(4,3),IsShadowCast(0)");
  Uni->BL = Ani->LightAdd("Type(2),Dir(2,0,6),Color(0,0,1),Pos(-5,5,0),CutOffDist(12,17),CutOffAngle(4,3),IsShadowCast(0)");

  Ani->ImageLoad(&Uni->Img, "a");

  Ani->CubeMapCreate("SkyNebula");
} /* End of 'TNG_UnitInit' function */

/* Test unit PD2 deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_LIGHTING_DEMO *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_LIGHTING_DEMO *Uni, tngANIM *Ani )
{
  Ani->GeometryDelete(Uni->Object);
  Ani->PrimFree(Uni->Plane);
} /* End of 'TNG_UnitClose' function */

/* Test unit PD2 inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_LIGHTING_DEMO *Uni, tngANIM *Ani )
{
  if (Ani->KeysClick['O'])
    Uni->RL->Type = Uni->GL->Type = Uni->BL->Type = TNG_LIGHT_DIRECTIONAL;
  else if (Ani->KeysClick['P'])
    Uni->RL->Type = Uni->GL->Type = Uni->BL->Type = TNG_LIGHT_POINT;
  else if (Ani->KeysClick['I'])
    Uni->RL->Type = Uni->GL->Type = Uni->BL->Type = TNG_LIGHT_AMBIENT;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_SHIFT] && Ani->Keys[VK_CONTROL] && Ani->Keys['R'])
    Uni->RL->CutOffDist.X += 2, Uni->RL->CutOffDist.Y += 2;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_SHIFT] && Ani->Keys[VK_CONTROL] && Ani->Keys['R'])
    Uni->RL->CutOffDist.X -= 2, Uni->RL->CutOffDist.Y -= 2;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_SHIFT] && Ani->Keys[VK_CONTROL] && Ani->Keys['G'])
    Uni->GL->CutOffDist.X += 2, Uni->GL->CutOffDist.Y += 2;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_SHIFT] && Ani->Keys[VK_CONTROL] && Ani->Keys['G'])
    Uni->GL->CutOffDist.X -= 2, Uni->GL->CutOffDist.Y -= 2;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_SHIFT] && Ani->Keys[VK_CONTROL] && Ani->Keys['B'])
    Uni->BL->CutOffDist.X += 2, Uni->BL->CutOffDist.Y += 2;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_SHIFT] && Ani->Keys[VK_CONTROL] && Ani->Keys['B'])
    Uni->BL->CutOffDist.X -= 2, Uni->BL->CutOffDist.Y -= 2;
  /* Manage light direction */
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['X'] && Ani->Keys['R'])
    Uni->RL->Dir.X++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['X'] && Ani->Keys['R'])
    Uni->RL->Dir.X--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['Y'] && Ani->Keys['R'])
    Uni->RL->Dir.Y++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['Y'] && Ani->Keys['R'])
    Uni->RL->Dir.Y--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['Z'] && Ani->Keys['R'])
    Uni->RL->Dir.Z++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['Z'] && Ani->Keys['R'])
    Uni->RL->Dir.Z--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['X'] && Ani->Keys['B'])
    Uni->BL->Dir.X++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['X'] && Ani->Keys['B'])
    Uni->BL->Dir.X--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['Y'] && Ani->Keys['B'])
    Uni->BL->Dir.Y++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['Y'] && Ani->Keys['B'])
    Uni->BL->Dir.Y--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['Z'] && Ani->Keys['B'])
    Uni->BL->Dir.Z++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['Z'] && Ani->Keys['B'])
    Uni->BL->Dir.Z--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['X'] && Ani->Keys['G'])
    Uni->GL->Dir.X++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['X'] && Ani->Keys['G'])
    Uni->GL->Dir.X--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['Y'] && Ani->Keys['G'])
    Uni->GL->Dir.Y++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['Y'] && Ani->Keys['G'])
    Uni->GL->Dir.Y--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['Z'] && Ani->Keys['G'])
    Uni->GL->Dir.Z++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_SHIFT] && Ani->Keys['Z'] && Ani->Keys['G'])
    Uni->GL->Dir.Z--;
  /* Manage position of light */
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['X'] && Ani->Keys['R'])
    Uni->RL->Pos.X++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['X'] && Ani->Keys['R'])
    Uni->RL->Pos.X--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['Y'] && Ani->Keys['R'])
    Uni->RL->Pos.Y++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['Y'] && Ani->Keys['R'])
    Uni->RL->Pos.Y--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['Z'] && Ani->Keys['R'])
    Uni->RL->Pos.Z++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['Z'] && Ani->Keys['R'])
    Uni->RL->Pos.Z--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['X'] && Ani->Keys['B'])
    Uni->BL->Pos.X++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['X'] && Ani->Keys['B'])
    Uni->BL->Pos.X--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['Y'] && Ani->Keys['B'])
    Uni->BL->Pos.Y++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['Y'] && Ani->Keys['B'])
    Uni->BL->Pos.Y--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['Z'] && Ani->Keys['B'])
    Uni->BL->Pos.Z++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['Z'] && Ani->Keys['B'])
    Uni->BL->Pos.Z--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['X'] && Ani->Keys['G'])
    Uni->GL->Pos.X++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['X'] && Ani->Keys['G'])
    Uni->GL->Pos.X--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['Y'] && Ani->Keys['G'])
    Uni->GL->Pos.Y++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['Y'] && Ani->Keys['G'])
    Uni->GL->Pos.Y--;
  else if (Ani->KeysClick[VK_OEM_PLUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['Z'] && Ani->Keys['G'])
    Uni->GL->Pos.Z++;
  else if (Ani->KeysClick[VK_OEM_MINUS] && Ani->Keys[VK_CONTROL] && Ani->Keys['Z'] && Ani->Keys['G'])
    Uni->GL->Pos.Z--;
} /* End of 'TNG_UnitResponse' function */

/* Test unit PD2 render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_LIGHTING_DEMO *Uni, tngANIM *Ani )
{
  CHAR Buf[100];
  tngPRIM *Pr;

  sprintf(Buf, "FPS: %.5f", Ani->FPS);
  Pr = Ani->FntStringToPrim(Ani->FntDefault, Buf, Vec4Set(1, 1, 1, 1), 8);

  Ani->LightDraw(Uni->RL);
  Ani->LightDraw(Uni->GL);
  Ani->LightDraw(Uni->BL); 
  Ani->GeometryDraw(Uni->Object, MatrMulMatr(MatrRotateX(-90), MatrTranslate(VecSet(0, 2.87, 0))));
  Ani->PrimDraw(Uni->Plane, MatrTranslate(VecSet(0, -0.001, 0)));
  /* Ani->PrimDraw(Pr, MatrTranslate(VecSet(0, 0, 0))); */
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
static VOID TNG_UnitDefferRender( tngUNIT_LIGHTING_DEMO *Uni, tngANIM *Ani )
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
static FLT TNG_UnitGetDistance( tngUNIT_LIGHTING_DEMO *Uni, tngANIM *Ani )
{
  return -1;
} /* End of 'TNG_UnitGetDistance' function */

/* Lighting demonstration unit creation function.
 * ARGUMENTS:
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (tngUNIT *) pointer to created unit.
 */
tngUNIT * TNG_UnitCreateLightingDemo( tngANIM *Ani )
{
  tngUNIT_LIGHTING_DEMO *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_LIGHTING_DEMO *)TNG()->UnitCreate(sizeof(tngUNIT_LIGHTING_DEMO))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;
  
  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreate' function */

/* END OF 'tng_unit_lighting_demo.c' FILE */
