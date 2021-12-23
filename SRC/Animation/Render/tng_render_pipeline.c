/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_render_pipeline.c
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation.
 *               Resourses handle.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kuralenok,
 *               Alexandr Vashpanov.
 * LAST UPDATE : 17.04.2019
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#include <stdio.h>

/* Render passes shaders */
static tngSHADER *TNG_RndTargetPass1Shader, *TNG_RndTargetPass2Shader;

/* Render targets passes */
static tngTARGET *TNG_RenderTargetPass1, *TNG_RenderTargetPass2;

/* Save polygon mode data */
static INT TNG_RnderPipelineSaveWireFrame[2];

/* Draw debag windows flag */
static UINT TNG_IsDebug = 1, TNG_IsBlur = 0,TNG_IsSobel = 0, TNG_IsEmbossing = 0, TNG_IsHigh_pass = 0, TNG_IsMonochrome = 0, TNG_IsRobert = 0, TNG_IsNorm = 1;

/* Render target pipeline resize function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_RenderPipelineResize( VOID )
{
  TNG()->TargetResize(TNG_RenderTargetPass1, TNG()->FrameW, TNG()->FrameH);
  TNG()->TargetResize(TNG_RenderTargetPass2, TNG()->FrameW, TNG()->FrameH);
} /* End of 'TNG_RenderPipelineResize' function */

/* Render target pipeline start frame render function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_RenderPipelineStart( VOID )
{
  glClearColor(1, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, TNG_RenderTargetPass1->FBO);
  
  TNG()->LightClearIsRenderFlag();
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3, 0.5, 0.8, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  TNG()->CubeMapDraw();
} /* End of 'TNG_RenderPipelineStart' function */

/* FBO defer start function.
 * ARGUMETS: None.
 * RETURNS: None.
 */
static VOID TNG_RenderPipelineDeferStart( VOID )
{
  INT i, loc, n = COM_MIN(8, TNG_RenderTargetPass1->NumOfCA + TNG_RenderTargetPass1->NumOfDA);
  UINT ProgId;

  glFinish();
  glBindFramebuffer(GL_FRAMEBUFFER, TNG_RenderTargetPass2->FBO);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* FBO textures apply */
  for (i = 0; i < n; i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, TNG_RenderTargetPass1->Attachments[i]->TexId);
  }

  glGetIntegerv(GL_POLYGON_MODE, TNG_RnderPipelineSaveWireFrame);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  TNG()->LightsRedraw();

  ProgId = TNG()->ShaderApply(TNG_RndTargetPass1Shader);
  if ((loc = glGetUniformLocation(ProgId, "FrameW")) != -1)
    glUniform1f(loc, TNG()->FrameW);
  if ((loc = glGetUniformLocation(ProgId, "FrameH")) != -1)
    glUniform1f(loc, TNG()->FrameH);
  if ((loc = glGetUniformLocation(ProgId, "FPS")) != -1)
    glUniform1f(loc, TNG()->FPS);
  if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, TNG()->Time);
  if ((loc = glGetUniformLocation(ProgId, "GTime")) != -1)
    glUniform1f(loc, TNG()->GlobalTime);

  for (i = 0; i < n; i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, TNG_RenderTargetPass1->Attachments[i]->TexId);
  }
  glDrawArrays(GL_POINTS, 0, 1);
} /* End of 'TNG_RnderPipelineDeferStart' function */

/* Render target finish frame render function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_RenderPipelineEnd( VOID )
{
  INT
    i, loc,
    n = COM_MIN(8, TNG_RenderTargetPass2->NumOfCA + TNG_RenderTargetPass2->NumOfDA),
    n0 = COM_MIN(8, TNG_RenderTargetPass1->NumOfCA + TNG_RenderTargetPass1->NumOfDA);
  UINT ProgId;
  CHAR Buf[15];
  tngPRIM *Pr;

  glFinish();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  /* FBO textures apply */
  ProgId = TNG()->ShaderApply(TNG_RndTargetPass2Shader);
  for (i = 0; i < n; i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, TNG_RenderTargetPass2->Attachments[i]->TexId);
  }
  for (i = 0; i < n0; i++)
  {
    glActiveTexture(GL_TEXTURE0 + n + i);
    glBindTexture(GL_TEXTURE_2D, TNG_RenderTargetPass1->Attachments[i]->TexId);
  }
  if (TNG()->KeysClick['D'] && TNG()->Keys[VK_SHIFT])
    TNG_IsDebug = !TNG_IsDebug;
  if (TNG()->KeysClick['S'] && TNG()->Keys[VK_SHIFT])
  {
    TNG_IsSobel = !TNG_IsSobel;
    memset(Buf, 0, sizeof(CHAR));
    sprintf(Buf, "Sobel");
    Pr = TNG()->FntStringToPrim(TNG()->FntDefault, Buf, Vec4Set(1, 1, 1, 1), 25);
  }
  if (TNG()->KeysClick['B'] && TNG()->Keys[VK_SHIFT])
  {
    TNG_IsBlur = !TNG_IsBlur;
    memset(Buf, 0, sizeof(CHAR));
    sprintf(Buf, "Blur");
    Pr = TNG()->FntStringToPrim(TNG()->FntDefault, Buf, Vec4Set(1, 1, 1, 1), 25);
  }
  if (TNG()->KeysClick['E'] && TNG()->Keys[VK_SHIFT])
  {
    TNG_IsEmbossing = !TNG_IsEmbossing;
    memset(Buf, 0, sizeof(CHAR));
    sprintf(Buf, "Embossing");
    Pr = TNG()->FntStringToPrim(TNG()->FntDefault, Buf, Vec4Set(1, 1, 1, 1), 25);
  }
  if (TNG()->KeysClick['H'] && TNG()->Keys[VK_SHIFT])
  {
    TNG_IsHigh_pass = !TNG_IsHigh_pass;
    memset(Buf, 0, sizeof(CHAR));
    sprintf(Buf, "High pass");
    Pr = TNG()->FntStringToPrim(TNG()->FntDefault, Buf, Vec4Set(1, 1, 1, 1), 25);
  }
  if (TNG()->KeysClick['M'] && TNG()->Keys[VK_SHIFT])
  {
    TNG_IsMonochrome = !TNG_IsMonochrome;
    memset(Buf, 0, sizeof(CHAR));
    sprintf(Buf, "Monochrome");
    Pr = TNG()->FntStringToPrim(TNG()->FntDefault, Buf, Vec4Set(1, 1, 1, 1), 25);
  }
  if (TNG()->KeysClick['R'] && TNG()->Keys[VK_SHIFT])
  {
    TNG_IsRobert = !TNG_IsRobert;
    memset(Buf, 0, sizeof(CHAR));
    sprintf(Buf, "Robert's");
    Pr = TNG()->FntStringToPrim(TNG()->FntDefault, Buf, Vec4Set(1, 1, 1, 1), 25);
  }
  if (TNG()->KeysClick['N'] && TNG()->Keys[VK_SHIFT])
  {
    TNG_IsNorm = !TNG_IsNorm;
    memset(Buf, 0, sizeof(CHAR));
    sprintf(Buf, "Normal color");
    Pr = TNG()->FntStringToPrim(TNG()->FntDefault, Buf, Vec4Set(1, 1, 1, 1), 25);
  }

  if ((loc = glGetUniformLocation(ProgId, "FrameW")) != -1)
    glUniform1f(loc, TNG()->FrameW);
  if ((loc = glGetUniformLocation(ProgId, "FrameH")) != -1)
    glUniform1f(loc, TNG()->FrameH);
  if ((loc = glGetUniformLocation(ProgId, "FPS")) != -1)
    glUniform1f(loc, TNG()->FPS);
  if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, TNG()->Time);
  if ((loc = glGetUniformLocation(ProgId, "GTime")) != -1)
    glUniform1f(loc, TNG()->GlobalTime);
  if ((loc = glGetUniformLocation(ProgId, "IsDebug")) != -1)
    glUniform1f(loc, TNG_IsDebug);
  if ((loc = glGetUniformLocation(ProgId, "IsBlur")) != -1)
    glUniform1f(loc, TNG_IsBlur);
  if ((loc = glGetUniformLocation(ProgId, "IsSobel")) != -1)
    glUniform1f(loc, TNG_IsSobel);
  if ((loc = glGetUniformLocation(ProgId, "IsEmbossing")) != -1)
    glUniform1f(loc, TNG_IsEmbossing);
  if ((loc = glGetUniformLocation(ProgId, "IsHigh_pass")) != -1)
    glUniform1f(loc, TNG_IsHigh_pass);
  if ((loc = glGetUniformLocation(ProgId, "IsMonochrome")) != -1)
    glUniform1f(loc, TNG_IsMonochrome);
  if ((loc = glGetUniformLocation(ProgId, "IsRobert")) != -1)
    glUniform1f(loc, TNG_IsRobert);
  if ((loc = glGetUniformLocation(ProgId, "IsNorm")) != -1)
    glUniform1f(loc, TNG_IsNorm);
  glDrawArrays(GL_POINTS, 0, 1);

  glDisable(GL_DEPTH_TEST);
  glDepthMask(FALSE);
  /*
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  */
  TNG()->PrimDrawAllDeferred();
  glEnable(GL_DEPTH_TEST);
  glDepthMask(TRUE);

  glFinish();
  glPolygonMode(GL_FRONT, TNG_RnderPipelineSaveWireFrame[0]);
  glPolygonMode(GL_BACK, TNG_RnderPipelineSaveWireFrame[1]);
} /* End of 'TNG_RnderPipelineEnd' function */

/* Save texture to BMP file function.
 * ARGUMENTS:
 *   - filename to save:
 *       CHAR *FileName;
 *   - OpenGL texture Id:
 *       UINT TexId;
 * RETURNS: None.
 */
static VOID TNG_PipelineSaveTextureImage( CHAR *FileName, UINT TexId )
{
  INT w, h, bpl;
  FILE *F;
  VOID *mem;
  BITMAPFILEHEADER bmf;
  BITMAPINFOHEADER bmi;

  /* Select texture */
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, TexId);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
  bpl = ((w * 3 + 3) >> 2) << 2;

  if ((mem = malloc(h * bpl)) != NULL)
  {
    /* Fill headers */
    bmf.bfType = 'B' | ('M' << 8);
    bmf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bpl * h;
    bmf.bfReserved1 = bmf.bfReserved2 = 0;
    bmf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biBitCount = 24;
    bmi.biCompression = BI_RGB;
    bmi.biPlanes = 1;
    bmi.biSizeImage = bpl * h;
    bmi.biWidth = w;
    bmi.biHeight = h;
    bmi.biXPelsPerMeter = bmi.biYPelsPerMeter = 30;
    bmi.biClrImportant = 0;
    bmi.biClrUsed = 0;

    /* Read image */
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, mem);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Store to file */
    if ((F = fopen(FileName, "wb")) != NULL)
    {
      fwrite(&bmf, sizeof(BITMAPFILEHEADER), 1, F);
      fwrite(&bmi, sizeof(BITMAPINFOHEADER), 1, F);
      fwrite(mem, bpl, h, F);
      fclose(F);
    }
    free(mem);
  }
} /* End of 'TNG_PipelineSaveTextureImage' function */

/* Save texture to BMP file function.
 * ARGUMENTS:
 *   - filename to save:
 *       CHAR *FileName;
 *   - OpenGL texture Id:
 *       UINT TexId;
 *   - component number:
 *       INT Component;
 * RETURNS: None.
 */
static VOID TNG_PipelineSaveTextureImageComponent( CHAR *FileName, UINT TexId, INT Component )
{
  INT i, w, h, bpl;
  FILE *F;
  VOID *mem;
  RGBQUAD pal[256] = {{0}};
  BITMAPFILEHEADER bmf;
  BITMAPINFOHEADER bmi;

  /* Setup palette */
  for (i = 0; i < 256; i++)
    pal[i].rgbRed = pal[i].rgbGreen = pal[i].rgbBlue = i;

  /* Select texture */
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, TexId);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
  bpl = ((w + 3) >> 2) << 2;

  if ((mem = malloc(h * bpl)) != NULL)
  {
    /* Fill headers */
    bmf.bfType = 'B' | ('M' << 8);
    bmf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 1024 + bpl * h;
    bmf.bfReserved1 = bmf.bfReserved2 = 0;
    bmf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 1024;

    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biBitCount = 8;
    bmi.biCompression = BI_RGB;
    bmi.biPlanes = 1;
    bmi.biSizeImage = bpl * h;
    bmi.biWidth = w;
    bmi.biHeight = h;
    bmi.biXPelsPerMeter = bmi.biYPelsPerMeter = 30;
    bmi.biClrImportant = 0;
    bmi.biClrUsed = 0;

    /* Read image */
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_ALPHA, GL_UNSIGNED_BYTE, mem);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Map alpha */
    for (i = 0; i < w * h; i++)
    {
      FLT t = ((BYTE *)mem)[i] / 255.0;

      t = t * 30 - 22;
      if (t < 0)
        t = 0;
      else
        if (t > 8)
          t = 8;
      t /= 8;
      t = 1 - t;
      ((BYTE *)mem)[i] = t * 255;
    }

    /* Store to file */
    if ((F = fopen(FileName, "wb")) != NULL)
    {
      fwrite(&bmf, sizeof(BITMAPFILEHEADER), 1, F);
      fwrite(&bmi, sizeof(BITMAPINFOHEADER), 1, F);
      fwrite(pal, 4, 256, F);
      fwrite(mem, bpl, h, F);
      fclose(F);
    }
    free(mem);
  }
} /* End of 'TNG_PipelineSaveTextureImageComponent' function */

/* Autosave render target textures function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_PipelineSave( VOID )
{
  INT i, n;
  SYSTEMTIME st;
  static CHAR Buf[1000];

  GetLocalTime(&st);

  /* FBO 1 textures save */
  n = TNG_RenderTargetPass1->NumOfCA + TNG_RenderTargetPass1->NumOfDA;
  for (i = 0; i < n; i++)
  {
    /* Store to file */
    sprintf(Buf, "BIN/SHOTS/TNG_Trg_%04d%02d%02d_%02d%02d%02d_f%02d.bmp",
      st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, i);
    TNG_PipelineSaveTextureImage(Buf, TNG_RenderTargetPass1->Attachments[i]->TexId);
  }

  /* FBO 1 textures save */
  n = TNG_RenderTargetPass2->NumOfCA + TNG_RenderTargetPass1->NumOfDA;
  for (i = 0; i < n; i++)
  {
    /* Store to file */
    sprintf(Buf, "BIN/SHOTS/TNG_Trg_%04d%02d%02d_%02d%02d%02d_s%02d.bmp",
      st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, i);
    TNG_PipelineSaveTextureImage(Buf, TNG_RenderTargetPass2->Attachments[i]->TexId);
  }

  /* Depth */
  sprintf(Buf, "BIN/SHOTS/TNG_Trg_%04d%02d%02d_%02d%02d%02d_d.bmp",
    st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
  TNG_PipelineSaveTextureImageComponent(Buf, TNG_RenderTargetPass1->Attachments[1]->TexId, 0);
} /* End of 'TNG_PipelineSave' function */

/* Target subsystem initilization function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemPipelineInit( VOID )
{
  TNG()->PipelineResize = TNG_RenderPipelineResize;
  TNG()->PipelineStart = TNG_RenderPipelineStart;
  TNG()->PipelineDeferStart = TNG_RenderPipelineDeferStart;
  TNG()->PipelineEnd = TNG_RenderPipelineEnd; 
  TNG()->PipelineSave = TNG_PipelineSave;

  TNG_RenderTargetPass1 = TNG()->TargetCreate(1, 1, "CA(RGBA32F)x6", "", TRUE);
  TNG_RenderTargetPass2 = TNG()->TargetCreate(1, 1, "CA(RGBA32F)", "", FALSE);
  TNG()->Zbuffer = TNG_RenderTargetPass1->Attachments[4];

  /* Initialize pass shaders */
  TNG_RndTargetPass1Shader = TNG()->ShaderAdd("Render_target_end");
  TNG_RndTargetPass2Shader = TNG()->ShaderAdd("Render_target_end2");
} /* End of 'TNG_RnderPipelineEnd' function */

/* END OF 'tng_render_pipeline.c' FILE */

