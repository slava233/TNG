/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_shaders.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Shaders handle declaration.
 * PROGRAMMER  : CGSG'2018.
 *               Girvits Alexander.
 * LAST UPDATE : 17.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_shaders_h_
#define __tng_shaders_h_

#include "../../../tng_def.h"

#define TNG_NAME_SIZE 100
#define TNG_MAX_VALUES 8

/* Struct for shaders */
typedef struct tagtngSUBSYSTEM_SHADER
{
  /* Add shader program from file function.
   * ARGUMENTS:
   *   - shader files prefix:
   *       CHAR *FileNamePrefix;
   * RETURNS: 
   *  (tngSHADER *) shader index or -1 if error is occured.
   */
  tngSHADER * (*ShaderAdd)( CHAR *FileNamePrefix );

  /* Update all shaders function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*ShadersUpdate)( VOID );
  
  /* Remove shader program function.
   * ARGUMENTS:
   *   - shader program pointer:
   *       tngSHADER *Shd;   
   * RETURNS: None.
   */
  VOID (*ShaderDelete)( tngSHADER *Shd );

  /* Correct shader number function.
  * ARGUMENTS:
  *   - shader program pointer:
  *       tngSHADER *Shd; 
  * RETURNS: 
  *   (UINT) shader program Id.
  */
  UINT (*ShaderApply)( tngSHADER *Shd );

  /* Add component block into shader function.
   * ARGUMENTS: 
   *   - shader program Id:
   *       UINT PrgNo;
   *   - string with shader block:
   *       CHAR *Str;
   *   - name block:
   *       CHAR *NameBlock;
   *   - index block:
   *       INT Bind.
   * RETURNS: None.
   */
  VOID (*ShaderAddComponent)( UINT PrgNo, CHAR *Str, CHAR *NameBlock, INT Bind );

  /* Add component block into shader function.
   * ARGUMENTS: 
   *   - shader program Id:
   *       UINT PrgNo;
   *   - matrix world:
   *       MATR World;
   *   - multiplication matrixes view and proj:
   *       MATR MatrVP.
   * RETURNS: None.
   */
  VOID (*ShaderAddComponentMatrix)( UINT PrgNo, MATR MatrW, MATR MatrVP );

  /* Add component light block into shader function.
   * ARGUMENTS: 
   *   - shader program Id:
   *       UINT PrgNo;
   *   - values block:
   *       tngBLOCK Values;
   *   - index block:
   *       INT Bind.
   * RETURNS: None.
   */
  VOID (*ShaderAddComponentLight)( UINT PrgNo, tngLIGHT *Light, INT Bind );
} tngSUBSYSTEM_SHADER;

/* Shaders sources subsystem initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemShadersInit( VOID );

/* Shader sources subsystem destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemShadersDestructor( VOID );

#endif /* __tng_shaders_h_ */

/* END OF 'tng_shaders.h' FILE */
