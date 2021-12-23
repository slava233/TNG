/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_geometry.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Main geometry support module.
 * PROGRAMMER  : CGSG'2018.
 *               Evgenii Hlevnoi.
 * LAST UPDATE : 26.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../tng_render.h"

#include <stdio.h>

/* Geometry initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_GeometryInit( VOID )
{
  StockInit(&TNG()->Geometry, 0, sizeof(tngGEOMETRY), TNG()->GeometryDelete);
} /* End of 'TNG_RndGeometryInit' function */

/* Geometry load from g3dm file function.
 * ARGUMENTS:
 *   - geometry object pointer:
 *       tngGEOMETRY *Geom;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (tngGEOMETRY *) pointer to geometry object.
 */
static tngGEOMETRY * TNG_GeometryLoad( CHAR *FileName )
{
  FILE *F;
  INT p, m, t, size;
  DWORD Sign;
  DWORD NumOfPrims;
  DWORD NumOfMaterials;
  DWORD NumOfTextures;
  tngGEOMETRY *Geom = NULL;
  INT (*TexNums)[8], *MtlNums;
  tngMATERIAL **Mtls;
  tngTEXTURE **Txts;

  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  if (fread(&Sign, 4, 1, F) != 1 ||
      Sign != *(DWORD *)"G3DM" ||
      fread(&NumOfPrims, 4, 1, F) != 1 ||
      fread(&NumOfMaterials, 4, 1, F) != 1 ||
      fread(&NumOfTextures, 4, 1, F) != 1 ||
      (Geom = TNG()->GeometryAdd(NumOfPrims)) == NULL)
  {
    fclose(F);
    return NULL;
  }

  size = sizeof(tngMATERIAL *) * NumOfMaterials + sizeof(INT) * (8 * NumOfMaterials + NumOfPrims) + sizeof(tngTEXTURE *) * NumOfTextures;
  Mtls = malloc(size);
  memset(Mtls, 0, size);
  TexNums = (INT (*)[8])(Mtls + NumOfMaterials);
  MtlNums = (INT *)(TexNums + NumOfMaterials);
  Txts = (tngTEXTURE **)(MtlNums  + NumOfPrims);

  memset(TexNums, 255, sizeof(INT (*)[8]) * NumOfMaterials);

  for (p = 0; p < NumOfPrims; p++)
  {
    DWORD NumOfVertexes;
    DWORD NumOfFacetIndexes;
    tngVERTEX_DEF *V;
    INT *I, size, i;
    static CHAR *Buf;

    fread(&NumOfVertexes, 4, 1, F);
    fread(&NumOfFacetIndexes, 4, 1, F);
    fread(&MtlNums[p], 4, 1, F);

    size = sizeof(tngVERTEX_DEF) * NumOfVertexes + sizeof(INT) * NumOfFacetIndexes;
    if ((V = malloc(size)) == NULL)
    {
      free(Mtls);
      TNG()->GeometryFree(Geom);
      fclose(F);
      return NULL;
    }
    memset(V, 0, size);
    I = (INT *)(V + NumOfVertexes);

    for (i = 0; i < NumOfVertexes; i++)
    {
      fread(&V[i].P, 1, sizeof(VEC), F);
      fread(&V[i].T, 1, sizeof(VEC2), F);
      fread(&V[i].N, 1, sizeof(VEC), F);
      fread(&V[i].C, 1, sizeof(VEC4), F);
    }
    fread(I, 4, NumOfFacetIndexes, F);

    /* Creating primitive */
    TNG()->TrimeshTangentSpace(V, NumOfVertexes, I, NumOfFacetIndexes);
    Geom->Prims[p] = TNG()->PrimCreate(TNG_PRIM_TRIMESH, "f3f2f3f4f3f3", V, NumOfVertexes, I, NumOfFacetIndexes);
    free(V);
  }

  /* Materials */
  for (m = 0; m < NumOfMaterials; m++)
  {
    tngMATERIAL mtl = {0};  /* Material to read */
    CHAR ShaderString[300]; /* Additional shader information */
    DWORD Shader;           /* Shader number (uses after load into memory) */
    CHAR Buf[300];

    fread(Buf, 1, 300, F);
    strncpy(mtl.Name, Buf, TNG_MATERIAL_NAME_LENGTH - 1);
    fread(&mtl.Ka, sizeof(VEC), 1, F);
    fread(&mtl.Kd, sizeof(VEC), 1, F);
    fread(&mtl.Ks, sizeof(VEC), 1, F);
    fread(&mtl.Ph, sizeof(FLT), 1, F);
    fread(&mtl.Trans, sizeof(FLT), 1, F);

    for (p = 0; p < 8; p++)
      fread(&TexNums[m][p], sizeof(INT), 1, F);

    fread(ShaderString, 1, 300, F);
    fread(&Shader, 4, 1, F);
    Mtls[m] = TNG()->MaterialAddStruct(&mtl);
  }

  for (p = 0; p < NumOfPrims; p++)
    if (MtlNums[p] >= 0 && MtlNums[p] < NumOfMaterials)
      Geom->Prims[p]->Mtl = Mtls[MtlNums[p]];

  /* Textures */
  for (t = 0; t < NumOfTextures; t++)
  {
    CHAR Name[300];     /* Texture name */
    DWORD W, H;         /* Texture image size in pixels */
    DWORD C;            /* Texture image components (1-mono, 3-bgr or 4-bgra) */
    BYTE *TextureImage; /* Pointer on texture */

    fread(Name, 1, 300, F);
    fread(&W, 4, 1, F);
    fread(&H, 4, 1, F);
    fread(&C, 4, 1, F);
    if ((TextureImage = malloc(W * H * C)) != NULL)
    {
      fread(TextureImage, C, W * H, F);
      Txts[t] = TNG()->TextureAdd(Name, TNG_TEXFMT_BGRA8, W, H, TextureImage);
      free(TextureImage);
    }
  }

  for (m = 0; m < NumOfMaterials; m++)
    for (t = 0; t < 8; t++)
      if (TexNums[m][t] != -1)
        Mtls[m]->Tex[t] = Txts[TexNums[m][t]];

  fclose(F);
  free(Mtls);

  /* Determine bound box */
  Geom->Min = Geom->Prims[0]->Min;
  Geom->Max = Geom->Prims[0]->Max;
  for (p = 1; p < Geom->NumOfPrims; p++)
  {
    Geom->Min = VecMin(Geom->Min, Geom->Prims[p]->Min);
    Geom->Max = VecMax(Geom->Max, Geom->Prims[p]->Max);
  }
  Geom->Center = VecDivNum(VecAddVec(Geom->Min, Geom->Max), 2);

  return Geom;
} /* End of 'TNG_GeometryLoad' function */

/* Geometry draw function.
 * ARGUMENTS: 
 *   - geometry object pointer:
 *       tngGEOMETRY *Geom;
 *   - world matrix:
 *       MATR World;
 * RETURNS: None.
 */
static VOID TNG_GeometryDraw( tngGEOMETRY *Geom, MATR World )
{ 
  INT i;
  MATR M;

  if (Geom == NULL)
    return;
  M = MatrMulMatr(Geom->Trans, World);

  /* TNG()->ShaderAddOn0 = Geom->NumOfPrims; */
  for (i = 0; i < Geom->NumOfPrims; i++)
  {
    /* TNG()->ShaderAddOn1 = i; */
    if (Geom->Prims[i]->Mtl != NULL && Geom->Prims[i]->Mtl->Trans == 1)
      TNG()->PrimDraw(Geom->Prims[i], M);
  }
} /* End of 'TNG_GeometryDraw' function */

/* Transparent Geometry draw function.
 * ARGUMENTS: 
 *   - geometry object pointer:
 *       tngGEOMETRY *Geom;
 *   - world matrix:
 *       MATR World;
 * RETURNS: None.
 */
static VOID TNG_GeometryDrawDeferred( tngGEOMETRY *Geom, MATR World )
{ 
  INT i;
  MATR M;

  if (Geom == NULL)
    return;
  M = MatrMulMatr(Geom->Trans, World);

  /* TNG()->ShaderAddOn0 = Geom->NumOfPrims; */

  /* Draw transparent back faced primitives */
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (i = 0; i < Geom->NumOfPrims; i++)
  {
    /* TNG()->ShaderAddOn1 = i; */
    if (Geom->Prims[i]->Mtl != NULL && Geom->Prims[i]->Mtl->Trans != 1)
      TNG()->PrimDraw(Geom->Prims[i], M);
  }

  /* Draw transparent front faced primitives */
  glCullFace(GL_BACK);
  for (i = 0; i < Geom->NumOfPrims; i++)
  {
    /* TNG()->ShaderAddOn1 = i; */
    if (Geom->Prims[i]->Mtl != NULL && Geom->Prims[i]->Mtl->Trans != 1)
      TNG()->PrimDraw(Geom->Prims[i], M);
  }
  glDisable(GL_CULL_FACE);
} /* End of 'TNG_GeometryDrawDeferred' function */

/* Geometry free strategy function.
 * ARGUMENTS:
 *   - Self-pointer to geometry object:
 *       tngGEOMETRY *Geom;
 * RETURNS: None.
 */
static VOID TNG_GeometryDelete( tngGEOMETRY *Geom )
{
  if (Geom == NULL)
    return;
  if (Geom->Prims != NULL)
  {
    INT i;

    for (i = 0; i < Geom->NumOfPrims; i++)
      TNG()->PrimFree(Geom->Prims[i]);
    free(Geom->Prims);
  }
  memset(Geom, 0, sizeof(tngGEOMETRY));
} /* End of 'TNG_RndGeometryFree' function */

/* Geometry delete function.
 * ARGUMENTS:
 *   - Self-pointer to geometry object:
 *       tngGEOMETRY *Geom;
 * RETURNS: None.
 */
static VOID TNG_GeometryFree( tngGEOMETRY *Geom )
{
  StockRemove(Geom);
} /* End of 'TNG_GeometryFree' function */

/* Geometry object add function.
 * ARGUMENTS:
 *   - Pointer to first geometry primitive:
 *       tngPRIM *Prims;
 *   - Number of primitives in geometry object:
 *       INT NumOfPrims;
 *   - Geometry object translation matrix:
 *       MATR Trans;
 * RETURNS:
 *   (tngGEOMETRY *) Pointer to Geometry object or NULL if failed.
 */
static tngGEOMETRY * TNG_GeometryAdd( INT NumOfPrims )
{
  tngGEOMETRY Geom;

  Geom.Prims = malloc(sizeof(tngPRIM *) * NumOfPrims);
  Geom.NumOfPrims = NumOfPrims;
  Geom.Trans = MatrIdentity();
  return StockAdd(&TNG()->Geometry, &Geom);
} /* End of 'TNG_GeometryAdd' function */

/* Filled geometry subsytem data */
static tngSUBSYSTEM_GEOMETRY SubsystemGeometry =
{
  {0},                         /* Initialize geometry stock */
  TNG_GeometryInit,            /* Geometry initialization function */
  TNG_GeometryLoad,            /* Geometry load from g3dm file function */
  TNG_GeometryDraw,            /* Geometry drawing function */
  TNG_GeometryDrawDeferred, /* Geometry drawing function */
  TNG_GeometryDelete,          /* Geometry deinitialization strategy */
  TNG_GeometryFree,            /* Geometry object deinitialization function */
  TNG_GeometryAdd              /* Geometry adding function */
};

/* Subsystem geometry initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemGeometryInit( VOID )
{
  TNG()->GeometryInit = TNG_GeometryInit;
  TNG()->GeometryLoad = TNG_GeometryLoad;
  TNG()->GeometryDraw = TNG_GeometryDraw;
  TNG()->GeometryDrawDeferred = TNG_GeometryDrawDeferred;
  TNG()->GeometryDelete = TNG_GeometryDelete;
  TNG()->GeometryFree = TNG_GeometryFree;
  TNG()->GeometryAdd = TNG_GeometryAdd;

  TNG_GeometryInit();
} /* End of 'TNG_SubsystemGeometryInit' funciton */

/* Geometry stock deinitialization function function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemGeometryDestructor( VOID )
{
  StockClear(&TNG()->Geometry);
} /* End of 'TNG_SubsystemGeometryDestructor' function */

/* END OF 'tng_geometry.c' FILE  */
