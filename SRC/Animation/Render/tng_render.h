/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_render.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Render header file.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kuralenok,
 *               Alexandr Vashpanov.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_render_h_
#define __tng_render_h_

#define GLEW_STATIC
#include <glew.h>

#include "../../tng_def.h"

#include "tng_target.h"
#include "Lights/tng_lights.h"
#include "Materials/tng_materials.h"
#include "Shaders/tng_shaders.h"
#include "Geometry/tng_geometry.h"
#include "Primitives/tng_primitives.h"
#include "Textures/tng_textures.h"
#include "Textures/Images/tng_images.h"
#include "render/fonts/tng_font.h"

/* Coordinate system types */
typedef enum tagtngCS
{
  TNG_CS_WORLD,  /* World coordinate system */
  TNG_CS_SCREEN, /* Screen space coordinate system */
} tngCS;

typedef struct tagtngSUBSYSTEM_RENDER
{
  INT
    FrameW, FrameH; /* Viewport size */

  CAMERA
    Camera; /* Camera structure */

  /* Current coordinate system */
  tngCS CS;

  /* OpenGL support */
  HDC hDC;     /* Back frame buffer device context */
  HGLRC hGLRC; /* OpenGL rendering context */

  /* Wireframe flag */
  BOOL IsWireFrame;

  /* Z buffer */
  tngTEXTURE *Zbuffer;

  /* Rendering prim start function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*RndStart)( VOID );

  /* Rendering deference prim start function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*RndStartDefer)( VOID );

  /* Rendering close function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */ 
  VOID (*RndClose)( VOID );

  /* Bit blt copy function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*RndCopyFrame)( HDC hDC );

  /* Resize init function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*RndResize)( VOID );

  /* Rendering end function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*RndEnd)( VOID );

  /* Render target pipeline resize function. 
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*PipelineResize)( VOID );

   /* Render target pipeline start frame render function. 
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*PipelineStart)( VOID );

  /* FBO defer start function.
   * ARGUMETS: None.
   * RETURNS: None.
   */
  VOID (*PipelineDeferStart)( VOID );

  /* Render target finish frame render function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*PipelineEnd)( VOID );

  /* Autosave render target textures function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*PipelineSave)( VOID );

  /* Set current coordinate system function.
   * ARGUMENTS:
   *   - new coordinate system:
   *       tngCS NewCS;
   * RETURNS:
   *   (tngCS) old coordinate system.
   */
  tngCS (*CSSet)( tngCS NewCS );

  /* Materials subsystem */
  union
  {
    tngSUBSYSTEM_MATERIALS;
    tngSUBSYSTEM_MATERIALS SubsystemMaterials;
  };

  /* Textures subsystem */
  union
  {
    tngSUBSYSTEM_TEXTURES;
    tngSUBSYSTEM_TEXTURES SubsystemTextures;
  };

  /* Images subsystem */
  union
  {
    tngSUBSYSTEM_IMAGES;
    tngSUBSYSTEM_IMAGES SubsystemImages;
  };

  /* Lights subsystem */
  union
  {
    tngSUBSYSTEM_LIGHTS;
    tngSUBSYSTEM_LIGHTS SubsystemLights;
  };

  /* Geometry subsystem */
  union
  {
    tngSUBSYSTEM_GEOMETRY;
    tngSUBSYSTEM_GEOMETRY SubsystemGeometry;
  };
  
  /* Target subsystem */
  union
  {
    tngSUBSYSTEM_TARGET;
    tngSUBSYSTEM_TARGET SubsystemTarget;
  };

  /* Shader subsystem */
  union
  {
    tngSUBSYSTEM_SHADER;
    tngSUBSYSTEM_SHADER SubsystemShader;
  };

  /* Primitives subsystem */
  union
  {
    tngSUBSYSTEM_PRIMITIVES;
    tngSUBSYSTEM_PRIMITIVES SubsystemPrimitives;
  };

  /* Fonts handle subsystem */
  union
  {
    tngSUBSYSTEM_FONT;
    tngSUBSYSTEM_FONT SubsystemFonts;
  };
} tngSUBSYSTEM_RENDER;

/* Rendering init function. 
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
VOID TNG_SubsystemRenderInit( VOID );

/* Target subsystem initilization function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemPipelineInit( VOID );

/* Render destruct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemRenderDestructor( VOID );

#endif /* __tng_render_h_ */

/* END OF 'tng_render.h' FILE */
