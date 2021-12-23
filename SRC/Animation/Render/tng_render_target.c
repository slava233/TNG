/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_render_target.c
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation.
 *               Resourses handle.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kuralenok,
 *               Alexandr Vashpanov.
 * LAST UPDATE : 19.04.2019
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#include <stdio.h>
#include <string.h>

/* Render target buffers direct (by inside attachment descriptions) create function.
 * ARGUMENTS:
 *   - target struct pointer:
 *       tngTARGET *Trg;
 * RETURNS: None.
 */
static VOID TNG_TargetCreateLocal( tngTARGET *Trg )
{
  INT i;
  UINT draw_buffers[TNG_TARGET_ATTACHMENT_MAX_COUNT] = {GL_NONE};
  CHAR Name[] = "Target0";

  /* Create all attachments */
  for (i = 0; i < Trg->NumOfCA + Trg->NumOfDA; i++)
  {
    INT w1, h1;

    Name[sizeof(Name) - 2] = '0' + i;
    Trg->Attachments[i] = TNG()->TextureAdd(Name, Trg->AttachmentsFmt[i], Trg->W, Trg->H, NULL);

    glBindTexture(GL_TEXTURE_2D, Trg->Attachments[i]->TexId);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w1);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h1);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  /* Create FBO */
  glGenFramebuffers(1, &Trg->FBO);

  /* Activate FBO */
  glBindFramebuffer(GL_FRAMEBUFFER, Trg->FBO);

  /* Attach color attachments */
  for (i = 0; i < Trg->NumOfCA; i++)
  {
    draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, Trg->Attachments[i]->TexId, 0);
  }

  /* Attach depth attachments */
  for (i = 0; i < Trg->NumOfDA; i++)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Trg->Attachments[Trg->NumOfCA + i]->TexId, 0);

  glDrawBuffers(COM_MAX(Trg->NumOfCA, Trg->NumOfDA), draw_buffers);

  /* Create and attach render buffer */
  if (Trg->RenderBuf)
  {
    glGenRenderbuffers(1, &Trg->RenderBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, Trg->RenderBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Trg->W, Trg->H);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Trg->RenderBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }

  i = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (i != GL_FRAMEBUFFER_COMPLETE)
  {
    i++;
    i--;
  }
  if (i == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
    i = 2;
  if (i == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT)
    i = 3;
  if (i == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
    i = 4;
  if (i == GL_FRAMEBUFFER_UNSUPPORTED)
    i = 5;
} /* End of 'TNG_TargetCreateLocal' function */

/* Render target buffers create function. 
 * ARGUMENTS:
 *   - size of attachment:
 *       INT W, H;
 *   - description of attachments:
 *       CHAR *CA, DA;
 *   - flag for depth render buffer creation:
 *       BOOL IsDepth;
 * RETURNS:
 *   (tngTARGET *) pointer to target struct.
 */
static tngTARGET * TNG_TargetCreate( INT W, INT H, CHAR *CAttach, CHAR *DAttach, BOOL IsDepth )
{
  INT i, j;
  ARGS Args1 = Scanner(CAttach), Args2 = Scanner(DAttach);
  tngTEX_FMT Type = 0;
  tngTARGET Trg = {0};
  static struct
  {
    CHAR *Str;
    tngTEX_FMT Fmt;
  } Dict[] =
  {
    {"RGBA32F", TNG_TEXFMT_RGBA32F},
    {"RGBA8", TNG_TEXFMT_RGBA8},
    {"RGB8", TNG_TEXFMT_RGB8},
    {"RGB32F", TNG_TEXFMT_RGB32F},
    {"R32F", TNG_TEXFMT_R32F}
  };

  Trg.NumOfCA = COM_MIN(Args1.NumOfScans, TNG_TARGET_ATTACHMENT_MAX_COUNT);
  Trg.NumOfDA = COM_MIN(Args2.NumOfScans, 1);
  Trg.W = W;
  Trg.H = H;

  for (i = 0; i < Trg.NumOfCA; i++)
  {
    CHAR *arg = Args1.Scan[i].Str[0];

    Type = TNG_TEXFMT_RGBA32F;
    for (j = 0; j < sizeof(Dict) / sizeof(Dict[0]); j++)
      if (strcmp(arg, Dict[j].Str) == 0)
      {
        Type = Dict[j].Fmt;
        break;
      }
    Trg.AttachmentsFmt[i] = Type;
  }
  for (i = 0; i < Trg.NumOfDA; i++)
  {
    Type = TNG_TEXFMT_DEPTH24;
    /*
    if (strcmp(Args2.Scan[0].Str[0], "D24") == 0)
      Type = TNG_TEXFMT_DEPTH24;
    */
    Trg.AttachmentsFmt[Trg.NumOfCA + i] = Type;
  }
  if (IsDepth)
    Trg.RenderBuf = 1;

  TNG_TargetCreateLocal(&Trg);
  return StockAdd(&TNG()->Targets, &Trg);
} /* End of 'TNG_TargetCreate' function */

/* Render target buffers free function. 
 * ARGUMENTS:
 *   - render target to delete:
 *       tngTARGET *Trg;
 * RETURNS: None.
 */
static VOID TNG_TargetFree( tngTARGET *Trg )
{
  StockRemove(Trg);
} /* End of 'TNG_TargetFree' function */

/* Render target buffers strategy free function. 
 * ARGUMENTS:
 *   - render target to delete:
 *       tngTARGET *Trg;
 * RETURNS: None.
 */
static VOID TNG_TargetDelete( tngTARGET *Trg )
{
  INT i;

  glBindFramebuffer(GL_FRAMEBUFFER, Trg->FBO);

  /* Delete color attachments */
  for (i = 0; i < Trg->NumOfCA; i++)
  {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
    TNG()->TextureFree(Trg->Attachments[i]);
  }

  /* Delete depth attachments */
  for (i = 0; i < Trg->NumOfDA; i++)
  {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
    TNG()->TextureFree(Trg->Attachments[Trg->NumOfCA + i]);
  }
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
  if (Trg->RenderBuf != 0)
    glDeleteRenderbuffers(1, &Trg->RenderBuf);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &Trg->FBO);
} /* End of 'TNG_TargetDelete' function */

/* Render target resize function. 
 * ARGUMENTS:
 *   - render target to resize:
 *       tngTARGET *Trg;
 *   - new size of attachments:
 *       INT NewW, NewH;
 * RETURNS: None.
 */
static VOID TNG_TargetResize( tngTARGET *Trg, INT NewW, INT NewH )
{
  TNG_TargetDelete(Trg);
  Trg->W = NewW;
  Trg->H = NewH;
  TNG_TargetCreateLocal(Trg);
} /* End of 'TNG_TargetResize' function */

/* Target destruct function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemTargetDestructor( VOID )
{
  StockClear(&TNG()->Targets);
} /* End of 'TNG_SubsystemTargetDestructor' function */

/* Target subsystem initilization function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemTargetInit( VOID )
{
  TNG()->TargetFree = TNG_TargetFree;
  TNG()->TargetCreate = TNG_TargetCreate;
  TNG()->TargetResize = TNG_TargetResize;

  StockInit(&TNG()->Targets, TNG_TARGET_ATTACHMENT_MAX_COUNT, sizeof(tngTARGET), TNG_TargetDelete);
} /* End of 'TNG_SubsystemTargetInit' function */

/* END OF 'tng_render_target.c' FILE */
