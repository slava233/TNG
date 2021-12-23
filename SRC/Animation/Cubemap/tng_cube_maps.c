/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_cube_maps.c
 * PURPOSE     : WinAPI animation system.
 *               Cube Maping system implementation.
 *               Resourses handle.
 * PROGRAMMER  : CGSG'2018.
 *               Alexandr Vashpanov.
 * LAST UPDATE : 15.04.2019
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#include <stdio.h>
#include <string.h>

static BOOL TNG_IsFirst = TRUE;
static BOOL TNG_IsLoad = FALSE;

/* Shader of CubeMap */
tngSHADER *SkyShd; 

/* function initialisation of cube map function.
 * ARGUMENTS: 
 *   - filename to load image:
 *       CHAR *BaseName;
 *   - Extension of filename with image:
 *       CHAR * Extension;
 * RETURNS:
 *   (tngTEXTURE *) Texture of cube map.
 */
tngTEXTURE * TNG_LoadCubeMap( CHAR *BaseName, CHAR *Extension ) 
{
  INT i;
  UINT TexId, W = 256, H = 256;
  /* Name of image file */
  CHAR TexName[300];
  /* Image for cube map */
  tngIMAGE Data[6] = {0};
  /* Textures for cube map */
  INT TextureType[] =
  {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
  };
  /* Name suffixes of images */
  CHAR *sufficxes[] = {"XPOS", "XNEG", "YPOS", "YNEG", "ZPOS", "ZNEG"};
  /* Cube map */
  tngTEXTURE SkyBox;

  /* Load images */
  for (i = 0; i < 6; i++)
  {
    /* Make name of image file */
    sprintf(TexName, "BIN/TEXTURES/Skybox/%s/%s%s", BaseName, sufficxes[i], Extension);

    /* Load image from file */
    TNG()->ImageLoad(&Data[i], TexName);
  }

  /* Activate texture of cube map */
  glActiveTexture(GL_TEXTURE0);

  /* Generation cube map texture */
  glGenTextures(1, &TexId);

  /* Activate cube map texture*/
  glBindTexture(GL_TEXTURE_CUBE_MAP, TexId);

  /* Size of cube map*/
  W = Data[0].W;
  H = Data[0].H;
  ///glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, W, H);
  
  for (i = 0; i < 6; i++)
    gluBuild2DMipmaps(TextureType[i], 4, Data[i].W, Data[i].H, GL_BGRA, GL_UNSIGNED_BYTE, Data[i].Pixels);
  /*
    glTexSubImage2D(TextureType[i], 0, 0, 0, Data[i].W, Data[i].H,
                    GL_RGBA, GL_UNSIGNED_BYTE, Data);
  */
  /* free cube map image */
  for (i = 0; i < 6; i++)
    TNG()->ImageFree(&Data[i]);
  /* Filtraton and repeate texture */
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  /* Fill the struct */
  SkyBox.TexId = TexId;
  SkyBox.H = H;
  SkyBox.W = W;

  TNG_IsLoad = TRUE;

  return StockAdd(&TNG()->Textures, &SkyBox);
} /* End of 'TNG_LoadCubeMap' function */

/* Free Skybox function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_CubeMapFree( VOID )
{
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
} /* End of 'TNG_CubeMapFree' function */

/* Initialization of Skybox. 
 * ARGUMENTS:
 *   - filename to load image:
 *       CHAR *Name;
 * RETURNS: None.
 */
VOID TNG_CubeMapDraw( VOID )
{
  INT ProgId, loc;
  VEC v;

  if (TNG()->IsWireFrame)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  if (TNG_IsFirst)
  {
   /// TNG()->SkyBox = NULL;
    SkyShd = TNG()->ShaderAdd("Sky_box");
    TNG_IsFirst = FALSE;
  }
  if (TNG_IsLoad)
  {
    glActiveTexture(GL_TEXTURE0 + 7);
    glBindTexture(GL_TEXTURE_CUBE_MAP, TNG()->SkyBox->TexId);
    ProgId = TNG()->ShaderApply(SkyShd);

    /* Get Uniformes */
    if ((loc = glGetUniformLocation(ProgId, "CamLoc")) != -1)
      glUniform3fv(loc, 1, &TNG()->Camera.Loc.X);
    v = CamRight(&TNG()->Camera);
    if ((loc = glGetUniformLocation(ProgId, "CamRight")) != -1)
      glUniform3fv(loc, 1, &v.X);
    v = CamUp(&TNG()->Camera);
    if ((loc = glGetUniformLocation(ProgId, "CamUp")) != -1)
      glUniform3fv(loc, 1, &v.X);
    v = CamDir(&TNG()->Camera);
    if ((loc = glGetUniformLocation(ProgId, "CamDir")) != -1)
      glUniform3fv(loc, 1, &v.X);
    if ((loc = glGetUniformLocation(ProgId, "CamProjDist")) != -1)
      glUniform1f(loc, TNG()->Camera.ProjDist);
    if ((loc = glGetUniformLocation(ProgId, "CamProjDist")) != -1)
      glUniform1f(loc, TNG()->Camera.ProjDist);
    if ((loc = glGetUniformLocation(ProgId, "CamProjSize")) != -1)
      glUniform1f(loc, TNG()->Camera.ProjSize);
    if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
      glUniform1f(loc, TNG()->Time);
    if ((loc = glGetUniformLocation(ProgId, "FrameW")) != -1)
      glUniform1f(loc, TNG()->FrameW);
    if ((loc = glGetUniformLocation(ProgId, "FrameH")) != -1)
      glUniform1f(loc, TNG()->FrameH);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(FALSE);
    glDrawArrays(GL_POINTS, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(TRUE);

    if (TNG()->IsWireFrame)
    {             
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
  }
} /* End of 'TNG_CubeMapInit' function */

/*  Create of Skybox. 
 * ARGUMENTS:
 *   - filename to load image:
 *       CHAR *Name;
 * RETURNS: None.
 */
VOID TNG_CubeMapCreate( CHAR *Name )
{
  TNG()->SkyBox = TNG()->LoadCubeMap(Name, ".BMP");
}
/* Subsystem of Skybox function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsistemCubeMapInit( VOID )
{
  TNG()->CubeMapFree = TNG_CubeMapFree;
  TNG()->CubeMapDraw = TNG_CubeMapDraw;
  TNG()->LoadCubeMap = TNG_LoadCubeMap;
  TNG()->CubeMapCreate = TNG_CubeMapCreate;
} /* End of 'TNG_SubsistemCubeMapInit' function */

/* END OF 'tng_cube_maps.c' FILE */                          