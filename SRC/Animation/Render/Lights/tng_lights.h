/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_lights.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Lights sources handle declaration module.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas,
 *               Vasilisa Dmitrieva.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_lights_h_
#define __tng_lights_h_

#include "../../../tng_def.h"

/* Light sources types */
typedef enum tagtngLIGHT_TYPE
{
  TNG_LIGHT_AMBIENT = 0,         /* Ambient light type */
  TNG_LIGHT_POINT = 1,           /* Point light type */
  TNG_LIGHT_DIRECTIONAL = 2,     /* Directional light type */
  TNG_LIGHT_SPOT = 3             /* Spot light type */
} tngLIGHT_TYPE;

/* Light representation type */
typedef struct tagtngLIGHT
{
  tngLIGHT_TYPE Type; /* Light type */
  VEC
    Pos,              /* Position of point & spot light types */
    Dir,              /* Direction of directional and spot light */
    Color,            /* Color of light */
    Att;              /* Attenuation coefficients: CC, CL, CQ */
  VEC2
    CutOffAngle,      /* Cut off Angle for spotlight */
    CutOffDist;       /* Distance to cut off for spot and point light */
  BOOL 
    IsShadowCast,     /* Is shadow castes flag */
    IsRender;         /* Is rendering on current frame */
} tngLIGHT;

/* Lights sources subsystem representation type */
typedef struct tagtngSUBSYSTEM_LIGHTS
{
  /* Stock for lights */
  STOCK Lights;
  /* Shadow render target */
  tngTARGET *ShadowFBO;

  /* Light sources deintialization function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*LightsClose)( VOID );

  /* Light mark for draw function.
   * ARGUMETNS:
   *   - light to mark for draw:
   *       tngLIGHT *Lig;
   * RETURNS: None.
   */
  VOID (*LightDraw)( tngLIGHT *Lig );

  /* Lights clear IsRender flag function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*LightClearIsRenderFlag)( VOID );

  /* Light source redraw(deffered lighting) functuion.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*LightsRedraw)( VOID );

  /* Light source add function.
   * ARGUMENTS:
   *   - light description:
   *       CHAR *LightDescr;
   * RETURNS:
   *   (tngLIGHT *) Pointer to light or NULL if failed.
   */
  tngLIGHT * (*LightAdd)( CHAR *LightDescr );
} tngSUBSYSTEM_LIGHTS;

/* Lights sources subsystem initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemLightsInit( VOID );

/* Lights sources subsystem destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemLightsDestructor( VOID );

#endif /* __tng_lights_h_ */

/* END OF 'tng_lights.h' FILE */
