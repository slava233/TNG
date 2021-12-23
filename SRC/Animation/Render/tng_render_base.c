/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_render_base.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Rendering base.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kuralenok,
 *               Alexandr Vashpanov.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#include <time.h>

#include "tng_render.h"
#include "tng_render_pipeline.h"

#pragma comment(lib, "glew32s")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

/* Resize init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_RndResize( VOID )
{
  TNG()->FrameW = TNG()->W;
  TNG()->FrameH = TNG()->H;
  glViewport(0, 0, TNG()->FrameW, TNG()->FrameH);

  CamProjSet(&TNG()->Camera, TNG()->FrameW, TNG()->FrameH);
  TNG()->PipelineResize();
} /* End of 'TNG_RndResize' function */

/* Rendering deference prim start function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_RndStartDefer( VOID )
{
  TNG()->PipelineDeferStart();
} /* End of 'TNG_RndStartDefer' function */

/* Rendering prim start function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_RndStart( VOID )
{
  TNG()->ShadersUpdate();

  TNG()->PipelineStart();
} /* End of 'TNG_RndStart' function */

/* Rendering close function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
static VOID TNG_RndClose( VOID )
{
} /* End of 'TNG_RndClose' function */

/* Rendering end function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_RndEnd( VOID )
{
  TNG()->PipelineEnd();
} /* End of 'TNG_RndEnd' function */

/* Bit blt copy function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_RndCopyFrame( HDC hDC )
{
  SwapBuffers(TNG()->hDC);
} /* End of 'TNG_RndCopyFrame' function */

/* Set current coordinate system function.
 * ARGUMENTS:
 *   - new coordinate system:
 *       tngCS NewCS;
 * RETURNS:
 *   (tngCS) old coordinate system.
 */
static tngCS TNG_RndCSSet( tngCS NewCS )
{
  tngCS OldCS;
  static CAMERA
    SaveWrdCam;

  /* No deals if not CS change */
  if ((OldCS = TNG()->CS) == NewCS)
    return OldCS;

  switch (TNG()->CS = NewCS)
  {
  case TNG_CS_WORLD:
    TNG()->Camera = SaveWrdCam;
    break;
  case TNG_CS_SCREEN:
    SaveWrdCam = TNG()->Camera;
    TNG()->Camera.ProjW = TNG()->FrameW;
    TNG()->Camera.ProjH = TNG()->FrameH;
    TNG()->Camera.ViewMatr = MatrIdentity();
    TNG()->Camera.ProjMatr =
      MatrOrtho(0, TNG()->FrameW - 1, 0, TNG()->FrameH - 1, -30, 30);
    TNG()->Camera.VP = MatrMulMatr(TNG()->Camera.ViewMatr, TNG()->Camera.ProjMatr);
    break;
  }
  return OldCS;
} /* End of 'TNG_RndCSSet' function */

/* Filled timer subsytem data */
static tngSUBSYSTEM_RENDER SubsystemRender =
{
  1, 1,       /* Viewport size */
  {0},        /* Camera */
  TNG_CS_WORLD, /* Coordinate system */
  NULL,       /* Back frame buffer device context */
  NULL,       /* OpenGL rendering context */
  FALSE       /* Wireframe flag */
};

/* Subsystem of render function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemRenderInit( VOID )
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  TNG()->SubsystemRender = SubsystemRender;
  TNG()->RndStart = TNG_RndStart;
  TNG()->RndStartDefer = TNG_RndStartDefer;
  TNG()->RndEnd = TNG_RndEnd;
  TNG()->RndResize = TNG_RndResize;
  TNG()->RndClose = TNG_RndClose;
  TNG()->RndCopyFrame = TNG_RndCopyFrame;
  TNG()->CSSet = TNG_RndCSSet;

  TNG()->hDC = GetDC(TNG()->hWnd);
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(TNG()->hDC, &pfd);
  DescribePixelFormat(TNG()->hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(TNG()->hDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  TNG()->hGLRC = wglCreateContext(TNG()->hDC);
  wglMakeCurrent(TNG()->hDC, TNG()->hGLRC);

  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
    exit(0);
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  CamSetDefault(&TNG()->Camera);
  CamProjSet(&TNG()->Camera, TNG()->FrameW, TNG()->FrameH);
  CamSet(&TNG()->Camera, VecSet(8, 8, 8), VecSet(0, 0, 0), VecSet(0, 1, 0));
  TNG()->CSSet(TNG_CS_SCREEN);
  TNG()->CSSet(TNG_CS_WORLD);

  TNG_SubsystemShadersInit();
  TNG_SubsystemPrimitivesInit();
  TNG_SubsystemMaterialsInit();
  TNG_SubsystemTexturesInit();
  TNG_SubsystemImagesInit();
  TNG_SubsystemTargetInit();
  TNG_SubsystemPipelineInit();
  TNG_SubsystemLightsInit();
  TNG_SubsystemGeometryInit();
  TNG_SubsystemFontsInit();
} /* End of 'TNG_SubsystemRenderInit' function */

/* Render destruct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemRenderDestructor( VOID )
{
  TNG_SubsystemFontsDestructor();
  TNG_SubsystemMaterialsDestructor();
  TNG_SubsystemTexturesDestructor();
  TNG_SubsystemImagesDestructor();
  TNG_SubsystemLightsDestructor();
  TNG_SubsystemPrimitivesDestructor();
  TNG_SubsystemTargetDestructor();
  TNG_SubsystemShadersDestructor();
  TNG_SubsystemGeometryDestructor();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(TNG()->hGLRC);
  ReleaseDC(TNG()->hWnd, TNG()->hDC); 
} /* End of 'TNG_SubsystemRenderDestructor' function */

/* END OF 'tng_render_base.c' FILE */
