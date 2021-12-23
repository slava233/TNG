/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_lights.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Lights sources handle functions.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas,
 *               Vasilisa Dmitrieva.
 * LAST UPDATE : 26.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../../tng.h"

#include <string.h>

/* Light sources draw shaders */
static tngSHADER
  *TNG_LightShdAmbient, /* Shader for ambient light type */
  *TNG_LightShdPoint,   /* Shader for point light type */
  *TNG_LightShdDir,     /* Shader for directional light type */
  *TNG_LightShdSpot,    /* Shader for spot light type */
  *TNG_LightShdDraw;    /* Shader for light source draw */

/* Draw light source texture */
static tngTEXTURE *TNG_LightGlowTex;

/* Shadow map size */
static INT TNG_ShadowMapSize = 2048;

/* Flag for shadow for direction and point light */
static INT IsShdDir, IsShdPoint;

/* Texture for shadow for point light */
static UINT ShdCubeMap;

/* Matrix for shadow */
static MATR LightMatr;

/* Parametrs for camera for point light */
typedef struct tagShadowDir
{
  GLenum CubeFace; /* CubeMap surfase */
  VEC At, Up;      /* Vectors for view matrix */
} ShadowDir;

/* Light sources deintialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_LightsClose( VOID )
{
  StockRelease(&TNG()->Lights);
} /* End of 'TNG_LightsClose' function */

/* Shadow for direction light initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_DirShadowInit( VOID )
{
  /* Border color */
  FLT Border[4] = {1, 1, 0, 0};

  /* Create and setup shadow fbo */
  TNG()->ShadowFBO = TNG()->TargetCreate(TNG_ShadowMapSize, TNG_ShadowMapSize, "", "DA(D24)", FALSE);
  TNG()->TextureParams(TNG()->ShadowFBO->Attachments[0], GL_CLAMP_TO_BORDER, GL_LINEAR, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, TNG()->ShadowFBO->Attachments[0]->TexId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, Border);
  glBindTexture(GL_TEXTURE_2D, 0);
} /* End of 'TNG_DirShadowInit' function */

/* Shadow for point light initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_PointShadowInit( VOID )
{
  INT i;
  /* Border color */
  FLT Border[4] = {1, 1, 0, 0};

  /* Create and setup shadow fbo */
  TNG()->ShadowFBO = TNG()->TargetCreate(TNG_ShadowMapSize, TNG_ShadowMapSize, "", "DA(D24)", FALSE);
  TNG()->TextureParams(TNG()->ShadowFBO->Attachments[0], GL_CLAMP_TO_BORDER, GL_LINEAR, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, TNG()->ShadowFBO->Attachments[0]->TexId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, Border);
  /* Create the cube map */
  glGenTextures(1, &ShdCubeMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, ShdCubeMap);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
  for (i = 0 ; i < 6 ; i++) 
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, TNG_ShadowMapSize, 
                 TNG_ShadowMapSize, 0, GL_RED, GL_FLOAT, NULL);

  glBindFramebuffer(GL_FRAMEBUFFER, TNG()->ShadowFBO->FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, TNG()->ShadowFBO->Attachments[0]->TexId, 0);

  /* Disable write ahd read to the color buffer */
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
} /* End of 'TNG_PointShadowInit' function */

/* Light source add function.
 * ARGUMENTS:
 *   - light description:
 *       CHAR *LightDescr;
 * RETURNS:
 *   (tngLIGHT *) Pointer to light or NULL if failed.
 */
static tngLIGHT * TNG_LightAdd( CHAR *LightDescr )
{
  INT i;
  ARGS Args = Scanner(LightDescr);
  tngLIGHT L = {0}, *res;

  for (i = 0; i < Args.NumOfScans; i++)
  {
    FLT x = Args.Scan[i].Var[0],
        y = Args.Scan[i].Var[1],
        z = Args.Scan[i].Var[2];

    if (strcmp(Args.Scan[i].Name, "Type") == 0)
      L.Type = x == 0 ? TNG_LIGHT_AMBIENT : x == 1 ? TNG_LIGHT_POINT : x == 2 ? TNG_LIGHT_DIRECTIONAL : TNG_LIGHT_SPOT;
    else if (strcmp(Args.Scan[i].Name, "IsShadowCast") == 0)
    {
      L.IsShadowCast = x;
      if (!IsShdDir && L.Type == 2)
        TNG_DirShadowInit(), IsShdDir = !IsShdDir;
      else if (!IsShdPoint && L.Type == 1)
        TNG_PointShadowInit(), IsShdPoint = !IsShdPoint;
    }
    else if (strcmp(Args.Scan[i].Name, "Color") == 0)
      L.Color = VecSet(x, y, z);
    else if (strcmp(Args.Scan[i].Name, "Att") == 0)
      L.Att = VecSet(x, y, z);
    else if (strcmp(Args.Scan[i].Name, "CutOffAngle") == 0)
      L.CutOffAngle = Vec2Set(x, y);
    else if (strcmp(Args.Scan[i].Name, "CutOffDist") == 0)
      L.CutOffDist = Vec2Set(x, y);
    else if (strcmp(Args.Scan[i].Name, "Dir") == 0)
      L.Dir = VecSet(x, y, z);
    else if (strcmp(Args.Scan[i].Name, "Pos") == 0)
      L.Pos = VecSet(x, y, z);
    else if (strcmp(Args.Scan[i].Name, "IsRender") == 0)
      L.IsRender = x;
  }

  res = StockAdd(&TNG()->Lights, &L);
  return res;
} /* End of 'TNG_LightAdd' function */

/* Light mark for draw function.
 * ARGUMETNS:
 *   - light to mark for draw:
 *       tngLIGHT *Lig;
 * RETURNS: None.
 */
static VOID TNG_LightDraw( tngLIGHT *Lig )
{
  if (Lig != NULL)
    Lig->IsRender = TRUE;
} /* End of 'TNG_LightDraw' function */

/* Lights clear IsRender flag function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_LightClearIsRenderFlag( VOID )
{
  tngLIGHT *L = StockStart(&TNG()->Lights);

  while (L != NULL)
  {
    if (L->Type != TNG_LIGHT_AMBIENT)
      L->IsRender = FALSE;
    L = StockNext(L);
  }
} /* End of 'TNG_LightClearIsRenderFlag' function */

/* Shadow for direction light redraw functuion.
 * ARGUMENTS:
 *   - light source pointer:
 *       tngLIGHT *Lig;
 * RETURNS: None.
 */
static VOID TNG_DirShadowsRedraw( tngLIGHT *Lig )
{
  tngUNIT **U;
  MATR
    A = TNG()->Camera.ViewMatr, /* Save view matrix */
    B = TNG()->Camera.ProjMatr, /* Save projection matrix */
    C = TNG()->Camera.VP;       /* Save view * projection matrix */

  INT SaveTrgTex[8];
  /* Save fbo number */
  UINT defaultFBO;
  INT i;           

  /* Save target textures */
  for (i = 0; i < 8; i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &SaveTrgTex[i]);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  /* Recalculate matrixes for shadows */
  TNG()->Camera.ProjMatr = MatrOrtho(-10, 10, -10, 10, -10, 20);
  TNG()->Camera.ViewMatr = MatrView(Lig->Dir, VecSet(0, 0, 0), VecSet(0, 1, 0));
  TNG()->Camera.VP = MatrMulMatr(TNG()->Camera.ViewMatr, TNG()->Camera.ProjMatr);
  LightMatr = TNG()->Camera.VP;
  /* Get old FBO Id */
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &defaultFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, TNG()->ShadowFBO->FBO);
  glClear(GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, TNG_ShadowMapSize, TNG_ShadowMapSize);
  /* Resoling depth fight issures */
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2, 4);
  
  /* Render all scene for shadows */
  U = StockStart(&TNG()->Units);
  while (U != NULL)
  {
    (*U)->Render(*U, TNG());
    U = StockNext(U);
  } 
  /* Restore target textures */
  for (i = 0; i < 8; i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, SaveTrgTex[i]);
  }
  glDisable(GL_POLYGON_OFFSET_FILL);
  glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
  TNG()->Camera.ViewMatr = A;
  TNG()->Camera.ProjMatr = B;
  TNG()->Camera.VP = C;
  glViewport(0, 0, TNG()->FrameW, TNG()->FrameH);
} /* End of 'TNG_DirShadowsRedraw' function */

/* Shadow for point light redraw functuion.
 * ARGUMENTS:
 *   - light source pointer:
 *       tngLIGHT *Lig;
 * RETURNS: None.
 */
static VOID TNG_PointShadowsRedraw( tngLIGHT *Lig )
{
  INT SaveTrgTex[8];
  /* Save fbo number */
  UINT defaultFBO;
  INT i; 
  tngUNIT **U;
  MATR
    A = TNG()->Camera.ViewMatr, /* Save view matrix */
    B = TNG()->Camera.ProjMatr, /* Save projection matrix */
    C = TNG()->Camera.VP;       /* Save view * projection matrix */ 
  ShadowDir ShdDir[6] = 
  {
    {GL_TEXTURE_CUBE_MAP_POSITIVE_X, {1.0, 0.0, 0.0},  {0.0, -1.0, 0.0}},
    {GL_TEXTURE_CUBE_MAP_NEGATIVE_X, {-1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}},
    {GL_TEXTURE_CUBE_MAP_POSITIVE_Y, {0.0, 1.0, 0.0},  {0.0, 0.0, -1.0}},
    {GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0 }},
    {GL_TEXTURE_CUBE_MAP_POSITIVE_Z, {0.0, 0.0, 1.0},  {0.0, -1.0, 0.0}},
    {GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, {0.0, 0.0, -1.0}, {0.0, -1.0, 0.0}}
  };          

  /* Save target textures */
  for (i = 0; i < 8; i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &SaveTrgTex[i]);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  /* Get old FBO Id */
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &defaultFBO);
  glClear(GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, TNG_ShadowMapSize, TNG_ShadowMapSize);
  /* Resoling depth fight issures */
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2, 4);

  for (i = 0 ; i < 6; i++) 
  {
    glBindFramebuffer(GL_FRAMEBUFFER, TNG()->ShadowFBO->FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ShdDir[i].CubeFace, ShdCubeMap, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); 

    /* Recalculate matrixes for shadows */
    TNG()->Camera.ProjMatr = MatrOrtho(-10, 10, -10, 10, -10, 20);
    TNG()->Camera.ViewMatr = MatrView(Lig->Pos, ShdDir[i].At, ShdDir[i].Up);
    TNG()->Camera.VP = MatrMulMatr(TNG()->Camera.ViewMatr, TNG()->Camera.ProjMatr);
    
    /* Render all scene for shadows */
    U = StockStart(&TNG()->Units);
   // while (U != NULL)
    {
      (*U)->Render(*U, TNG());
      //U = StockNext(U);
    } 
  } 
  /* Restore target textures */
  for (i = 0; i < 8; i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, SaveTrgTex[i]);
  }
  glDisable(GL_POLYGON_OFFSET_FILL);
  glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
  TNG()->Camera.ViewMatr = A;
  TNG()->Camera.ProjMatr = B;
  TNG()->Camera.VP = C;
  glViewport(0, 0, TNG()->FrameW, TNG()->FrameH);
} /* End of 'TNG_PointShadowsRedraw' function */

/* Light source redraw(deffered lighting) functuion.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_LightsRedraw( VOID )
{
  INT loc;
  /* Shaders pointers */
  tngSHADER *ShadersId[] = 
  {
    TNG_LightShdAmbient,
    TNG_LightShdPoint,
    TNG_LightShdDir,
    TNG_LightShdSpot,
  };
  tngLIGHT *Lig = StockStart(&TNG()->Lights);

  /* Set blending for rendering light sources */
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  glDepthMask(FALSE);
  glDisable(GL_DEPTH_TEST);
  while (Lig != NULL)
  {
    UINT ProgId;

    if (Lig->IsRender)
    {
      VEC v;

      if (Lig->IsShadowCast && Lig->Type == 2)
      {
        glDepthMask(TRUE);
        glEnable(GL_DEPTH_TEST);
        TNG_DirShadowsRedraw(Lig);
        glDepthMask(FALSE);
        glDisable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0 + 8);
        glBindTexture(GL_TEXTURE_2D, TNG()->ShadowFBO->Attachments[0]->TexId);
        ProgId = TNG()->ShaderApply(ShadersId[Lig->Type]);
        if ((loc = glGetUniformLocation(ProgId, "MatrShadow")) != -1)
          glUniformMatrix4fv(loc, 1, FALSE, LightMatr.M[0]);
      }
      else if (Lig->IsShadowCast && Lig->Type == 1)
      {
        glDepthMask(TRUE);
        glEnable(GL_DEPTH_TEST);
        TNG_PointShadowsRedraw(Lig);
        glDepthMask(FALSE);
        glDisable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0 + 8);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ShdCubeMap);
        /*
        ProgId = TNG()->ShaderApply(ShadersId[Lig->Type]);
        if ((loc = glGetUniformLocation(ProgId, "MatrShadow")) != -1)
          glUniformMatrix4fv(loc, 1, FALSE, LightMatr.M[0]);
        */
      }
      ProgId = TNG()->ShaderApply(ShadersId[Lig->Type]);

      /*
      TNG()->ShaderAddComponent(ProgId, "Standart()", "Lights", 0);
      TNG()->ShaderAddComponentMatrix(ProgId, MatrIdentity(), TNG()->Camera.VP);
      */

      /* Setup default values to light shaders */
      if ((loc = glGetUniformLocation(ProgId, "ShadowCast")) != -1)
        glUniform1i(loc, Lig->IsShadowCast);
      if ((loc = glGetUniformLocation(ProgId, "MatrV")) != -1)
        glUniformMatrix4fv(loc, 1, FALSE, TNG()->Camera.ViewMatr.M[0]);
      if ((loc = glGetUniformLocation(ProgId, "MatrVP")) != -1)
        glUniformMatrix4fv(loc, 1, FALSE, TNG()->Camera.VP.M[0]);
      if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
        glUniform1f(loc, TNG()->Time);
      if ((loc = glGetUniformLocation(ProgId, "CamLoc")) != -1)
        glUniform3fv(loc, 1, &TNG()->Camera.Loc.X);
      v = CamRight(&TNG()->Camera);
      if ((loc = glGetUniformLocation(ProgId, "CamRight")) != -1)
        glUniform3fv(loc, 1, &v.X);
      v = CamUp(&TNG()->Camera);
      if ((loc = glGetUniformLocation(ProgId, "CamUp")) != -1)
        glUniform3fv(loc, 1, &v.X);
      if ((loc = glGetUniformLocation(ProgId, "IsWireFrame")) != -1)
        glUniform1i(loc, TNG()->IsWireFrame);
      if ((loc = glGetUniformLocation(ProgId, "FrameW")) != -1)
        glUniform1f(loc, TNG()->FrameW);
      if ((loc = glGetUniformLocation(ProgId, "FrameH")) != -1)
        glUniform1f(loc, TNG()->FrameH);

      /* Setup light sources to shader values */
      if ((loc = glGetUniformLocation(ProgId, "LightPos")) != -1)
        glUniform3fv(loc, 1, &Lig->Pos.X);
      if ((loc = glGetUniformLocation(ProgId, "LightColor")) != -1)
        glUniform3fv(loc, 1, &Lig->Color.X);
      if ((loc = glGetUniformLocation(ProgId, "LightDir")) != -1)
        glUniform3fv(loc, 1, &Lig->Dir.X);
      if ((loc = glGetUniformLocation(ProgId, "LightAtt")) != -1)
        glUniform3fv(loc, 1, &Lig->Att.X);
      if ((loc = glGetUniformLocation(ProgId, "LightCutOffAngle")) != -1)
        glUniform2fv(loc, 1, &Lig->CutOffAngle.X);
      if ((loc = glGetUniformLocation(ProgId, "LightCutOffDist")) != -1)
        glUniform2fv(loc, 1, &Lig->CutOffDist.X);

      /* Drawing light sources switch case */
      switch (Lig->Type)
      {
      case TNG_LIGHT_AMBIENT:
      case TNG_LIGHT_DIRECTIONAL:
        glBindVertexArray(0);
        glDrawArrays(GL_POINTS, 0, 1);
        break;
      case TNG_LIGHT_SPOT:
      case TNG_LIGHT_POINT:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glBindVertexArray(0);
        glPatchParameteri(GL_PATCH_VERTICES, 1);
        glDrawArrays(GL_PATCHES, 0, 1);
        glDisable(GL_CULL_FACE);
        break;
      }
    }
    Lig = StockNext(Lig);
  }

  /* Test light sources draw */
  Lig = StockStart(&TNG()->Lights);
  while (Lig != NULL)
  {
    if (Lig->IsRender && Lig->Type == TNG_LIGHT_POINT || Lig->Type == TNG_LIGHT_SPOT)
    {
      MATR WVP = TNG()->Camera.VP;
      VEC
        Right = CamRight(&TNG()->Camera),
        Up = CamUp(&TNG()->Camera);
      
      TNG()->ShaderApply(TNG_LightShdDraw);
      glUniform3fv(0, 1, &Lig->Pos.X);
      glUniform2fv(1, 1, &Lig->CutOffDist.X);
      glUniformMatrix4fv(2, 1, FALSE, WVP.M[0]);
      glUniform3fv(6, 1, &Right.X);
      glUniform3fv(7, 1, &Up.X);
      glUniform3fv(8, 1, &Lig->Color.X);
      glActiveTexture(GL_TEXTURE0 + 6);
      glBindTexture(GL_TEXTURE_2D, TNG_LightGlowTex->TexId);
      glDrawArrays(GL_POINTS, 0, 1);
    }
    Lig = StockNext(Lig);
  }

  /* Set blending to normal rendering */
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(TRUE);
  glUseProgram(0);
} /* End of 'TNG_LightsRedraw' function */

/* Filled lights subsytem data */
static tngSUBSYSTEM_LIGHTS SubsystemLights =
{
  {0}, /* Stock for lights */
  {0}, /* Shadow render target */
  /* Lights handle functions */
  TNG_LightsClose,
  TNG_LightDraw,
  TNG_LightClearIsRenderFlag,
  TNG_LightsRedraw,
  TNG_LightAdd
};

/* Light source strategy free function.
 * ARGUMENTS:
 *   - light source to delete:
 *       tngLIGHT *Lig;
 * RETURNS: None.
 */
static VOID TNG_LightDelete( tngLIGHT *Lig )
{
  StockRemove(Lig);
} /* End of 'TNG_LightDelete' function */

/* Lights sources subsystem initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemLightsInit( VOID )
{
  TNG()->SubsystemLights = SubsystemLights;

  /* Initialization of lights sources stock */
  StockInit(&TNG()->Lights, 0, sizeof(tngLIGHT), TNG_LightDelete);
  
  /* Add shaders for different light sources */
  TNG_LightShdAmbient = TNG()->ShaderAdd("Lights/Ambient");
  TNG_LightShdPoint = TNG()->ShaderAdd("Lights/Point");
  TNG_LightShdDir = TNG()->ShaderAdd("Lights/Directional");
  TNG_LightShdSpot = TNG()->ShaderAdd("Lights/Spot");
  TNG_LightShdDraw = TNG()->ShaderAdd("Lights/Draw");

  TNG_LightGlowTex = TNG()->TextureAddFromFile("BIN/TEXTURES/GLOW.G32");

  /* Add base ambient light source */
  TNG()->LightAdd("Type(0),IsShadowCast(0),IsRender(1)");
} /* End of 'TNG_SubsystemLightsInit' funciton */

/* Lights sources subsystem destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemLightsDestructor( VOID )
{
  StockClear(&TNG()->Lights);
  TNG()->TargetFree(TNG()->ShadowFBO);
  TNG()->TextureFree(TNG_LightGlowTex);
} /* End of 'TNG_SubsystemLightsDestructor' funciton */

/* END OF 'tng_lights.c' FILE */
