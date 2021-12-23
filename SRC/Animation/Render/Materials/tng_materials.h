/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_materials.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Materials handle header module.
 * PROGRAMMER  : CGSG'2018.
 *               Andrey Shayda.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_materials_
#define __tng_materials_

#include "../../../tng_def.h"

/* Length of materials name */
#define TNG_MATERIAL_NAME_LENGTH 256

/* Number of textures pointers in material */
#define TNG_MATERIAL_TEXTURES_COUNT 8

/* Material structure */
struct tagtngMATERIAL
{
  CHAR Name[TNG_MATERIAL_NAME_LENGTH];  /* Name of material */
  VEC Ka, Kd, Ks;                   /* Illumination coefficients */
  FLT Ph;                           /* Phong coefficent */
  FLT Trans;                        /* Materials transparency factor */
  tngTEXTURE *Tex[TNG_MATERIAL_TEXTURES_COUNT]; /* Array of textures */
  tngSHADER *Shd;                   /* Shader pointer */
};

/* Materials subsystem */
typedef struct tagtngSUBSYSTEM_MATERIALS
{
  /* Stock of materials */
  STOCK Materials;

  /* Material add function.
   * ARGUMENTS: 
   *   - name of material:
   *       CHAR *Name;
   *   - material parameter as a string:
   *       CHAR *Params;
   * RETURNS: 
   *   (tngMATERIAL *) pointer to added material.
   */ 
  tngMATERIAL * (*MaterialAdd)( CHAR *Name, CHAR *Params );

  /* Material add function.
   * ARGUMENTS: 
   *   - material structure pointer:
   *       tngMATERIAL *Mtl;
   * RETURNS: 
   *   (tngMATERIAL *) pointer to added material.
   */ 
  tngMATERIAL * (*MaterialAddStruct)( tngMATERIAL *Mtl );

  /* Materials apply function.
   * ARGUMENTS:
   *   - material pointer:
   *       tngMATERIAL *Mtl;
   * RETURNS:
   *   (UINT) shader program Id.
   */ 
  UINT (*MaterialApply)( tngMATERIAL *Mtl );
} tngSUBSYSTEM_MATERIALS;

/* Materials subsystem init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
VOID TNG_SubsystemMaterialsInit( VOID );

/* Materials destruct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemMaterialsDestructor( VOID );

#endif /* __tng_materials_ */

/* END OF 'tng_materials.h' FILE */