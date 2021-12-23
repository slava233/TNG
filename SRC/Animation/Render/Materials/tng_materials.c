/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_materials.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Materials handle module.
 * PROGRAMMER  : CGSG'2018.
 *               Andrey Shayda.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#include <string.h>

/* Default material */
tngMATERIAL *TNG_MaterialDefault;

/* Material add function.
 * ARGUMENTS: 
 *   - name of material:
 *       CHAR *Name;
 *   - material parameter as a string:
 *       CHAR *Params;
 * RETURNS: 
 *   (tngMATERIAL *) pointer to added material.
 */ 
static tngMATERIAL * TNG_MaterialAdd( CHAR *Name, CHAR *Params )
{
  INT i, j;
  tngMATERIAL mtl = *TNG_MaterialDefault, *res;
  ARGS Args = Scanner(Params);

  strcpy(mtl.Name, Name);

  for (i = 0; i < Args.NumOfScans; i++)
  {
    FLT x = Args.Scan[i].Var[0],
        y = Args.Scan[i].Var[1],
        z = Args.Scan[i].Var[2];

    if (strcmp(Args.Scan[i].Name, "Ka") == 0 && Args.Scan[i].NumOfComp == 3)
      mtl.Ka = VecSet(x, y, z);
    else if (strcmp(Args.Scan[i].Name, "Ka") == 0 && Args.Scan[i].NumOfComp == 1)
      mtl.Ka = VecSet(x, x, x);
    else if (strcmp(Args.Scan[i].Name, "Kd") == 0 && Args.Scan[i].NumOfComp == 3)
      mtl.Kd = VecSet(x, y, z);
    else if (strcmp(Args.Scan[i].Name, "Kd") == 0 && Args.Scan[i].NumOfComp == 1)
      mtl.Kd = VecSet(x, x, x);
    else if (strcmp(Args.Scan[i].Name, "Ks") == 0 && Args.Scan[i].NumOfComp == 3)
      mtl.Ks = VecSet(x, y, z);
    else if (strcmp(Args.Scan[i].Name, "Ks") == 0 && Args.Scan[i].NumOfComp == 1)
      mtl.Ks = VecSet(x, x, x);
    else if (strcmp(Args.Scan[i].Name, "Ph") == 0)
      mtl.Ph = x;
    else if (strcmp(Args.Scan[i].Name, "Trans") == 0)
      mtl.Trans = x;
    else if (strcmp(Args.Scan[i].Name, "Tex") == 0)
      for (j = 0; j < Args.Scan[i].NumOfStr; j++)
        mtl.Tex[j] = TNG()->TextureAddFromFile(Args.Scan[i].Str[j]);
    else if (strcmp(Args.Scan[i].Name, "Shader") == 0)
      mtl.Shd = TNG()->ShaderAdd(Args.Scan[i].Str[0]);
  }

  res = StockAdd(&TNG()->Materials, &mtl);
  return res;
} /* End of 'TNG_MaterialAdd' function */

/* Material add to struct function.
 * ARGUMENTS: 
 *   - material structure pointer:
 *       tngMATERIAL *Mtl;
 * RETURNS: 
 *   (tngMATERIAL *) pointer to added material.
 */ 
static tngMATERIAL * TNG_MaterialAddStruct( tngMATERIAL *Mtl )
{
  return StockAdd(&TNG()->Materials, Mtl);
} /* End of 'TNG_MaterialAdd' function */

/* Materials apply function.
 * ARGUMENTS:
 *   - material pointer:
 *       tngMATERIAL *Mtl;
 * RETURNS:
 *   (UINT) shader program Id.
 */ 
static UINT TNG_MaterialApply( tngMATERIAL *Mtl )
{
  INT i, loc;
  UINT ProgId;
  
  /* Correction of material */
  if (Mtl == NULL)
    Mtl = TNG_MaterialDefault;
 
  /* Shader applying */
  ProgId = TNG()->ShaderApply(Mtl->Shd);

  if ((loc = glGetUniformLocation(ProgId, "Ka")) != -1)
    glUniform3fv(loc, 1, &Mtl->Ka.X);
  if ((loc = glGetUniformLocation(ProgId, "Kd")) != -1)
    glUniform3fv(loc, 1, &Mtl->Kd.X);
  if ((loc = glGetUniformLocation(ProgId, "Ks")) != -1)
    glUniform3fv(loc, 1, &Mtl->Ks.X);
  if ((loc = glGetUniformLocation(ProgId, "Ph")) != -1)
    glUniform1f(loc, Mtl->Ph);
  if ((loc = glGetUniformLocation(ProgId, "Trans")) != -1)
    glUniform1f(loc, Mtl->Trans);

  /* Texture apply */
  for (i = 0; i < TNG_MATERIAL_TEXTURES_COUNT; i++)
  {
    BOOL is = FALSE;
    static CHAR TexName[] = "IsTexture0";

    TexName[sizeof(TexName) - 2] = '0' + i;
    glActiveTexture(GL_TEXTURE0 + i);
    if (Mtl->Tex[i] != NULL)
    {
      is = TRUE;
      glBindTexture(Mtl->Tex[i]->IsCubeMap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, Mtl->Tex[i]->TexId);
    }
    else
      glBindTexture(GL_TEXTURE_2D, 0);
    if ((loc = glGetUniformLocation(ProgId, TexName)) != -1)
      glUniform1i(loc, is);
  }

  return ProgId;
} /* End of 'TNG_MaterialApply' function */

/* Filled materials subsytem data */
static tngSUBSYSTEM_MATERIALS SubsystemMaterials =
{
  {0},
  TNG_MaterialAdd,
  TNG_MaterialAddStruct,
  TNG_MaterialApply
};

/* Materials destruct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemMaterialsDestructor( VOID )
{
  StockClear(&TNG()->Materials);
} /* End of 'TNG_SubsystemMaterialsDestructoror' function */

/* Materials subsystem init function.
 * ARGUMENTS: None.
 * RETURNS: 
 *   (STOCK) Stock of materials.
 */ 
VOID TNG_SubsystemMaterialsInit( VOID )
{
  tngMATERIAL mtl_def =
  {
    "Default material", /* Name */
    {0.1, 0.1, 0.1},   /* Ka */
    {0.8, 0.8, 0.8},   /* Kd */
    {0.3, 0.3, 0.3},   /* Ks */
    30,                /* Ph */
    1,                 /* Trans */
    {NULL},            /* Tex */
    NULL               /* Shader */
  };

  TNG()->SubsystemMaterials = SubsystemMaterials;
  StockInit(&TNG()->Materials, 0, sizeof(tngMATERIAL), NULL);

  mtl_def.Shd = TNG()->ShaderAdd("Default");
  TNG_MaterialDefault = TNG()->MaterialAddStruct(&mtl_def);
} /* End of 'TNG_SubsystemMaterialsInit' function */

/* END OF 'tng_materials.c' FILE */
