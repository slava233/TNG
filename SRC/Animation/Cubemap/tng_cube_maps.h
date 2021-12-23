/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_cube_maps.h
 * PURPOSE     : WinAPI animation system.
 *               Cube Maping system implementation.
 *               Resourses handle.
 * PROGRAMMER  : CGSG'2018.
 *               Alexandr Vashpanov.
 * LAST UPDATE : 12.04.2019
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_cube_maps_h_
#define __tng_cube_maps_h_

#include "../../tng_def.h"

typedef struct tagtngSUBSYSTEM_SKYBOX
{
  /* Cube map texture */
  tngTEXTURE *SkyBox;

  /* Initialization of Skybox. 
   * ARGUMENTS:
   *   - filename to load image:
   *       CHAR *Name;
   * RETURNS: None.
   */
  VOID (*CubeMapDraw)( VOID );

  /* function initialisation of cube map function.
   * ARGUMENTS: 
   *   - filename to load image:
   *       CHAR *BaseName;
   *   - Extension of filename with image:
   *       CHAR * Extension;
   * RETURNS:
   *   (tngTEXTURE) Texture of cube map
   */
  tngTEXTURE  * (*LoadCubeMap)( CHAR *BaseName, CHAR *Extension );

  /* Free Skybox function. 
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*CubeMapFree)( VOID );

  /*  Create of Skybox. 
   * ARGUMENTS:
   *   - filename to load image:
   *       CHAR *Name;
   * RETURNS: None.
   */
  VOID (*CubeMapCreate)( CHAR *Name );
 
} tngSUBSYSTEM_SKYBOX;

/* Subsystem of Skybox function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsistemCubeMapInit( VOID );

#endif
/* END OF 'tng_cube_maps.c' FILE */
