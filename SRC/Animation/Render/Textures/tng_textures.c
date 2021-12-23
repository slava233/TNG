/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_textures.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Textures handle module.
 * PROGRAMMER  : CGSG'2018.
 *               Andrey Shayda.
 * LAST UPDATE : 17.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#include <stdio.h>

/* Texture add function.
 * ARGUMENTS: 
 *   - texture name:
 *       CHAR *Name;
 *   - texture format type:
 *       tngTEX_FMT Format;
 *   - texture image size:
 *       INT W, H;
 *   - texture image pixel color data (DWORD colors):
 *       VOID *ImageData;
 * RETURNS: 
 *   (tngTEXTURE *) Texture pointer.
 */ 
static tngTEXTURE * TNG_TextureAdd( CHAR *Name, tngTEX_FMT Format, INT W, INT H, VOID *ImageData )
{
  tngTEXTURE Txt, *res; 

  /* Allocate texture space */
  glGenTextures(1, &Txt.TexId);
  glBindTexture(GL_TEXTURE_2D, Txt.TexId);

  if (Format != TNG_TEXFMT_BGRA8)
    glTexStorage2D(GL_TEXTURE_2D, 1, Format, W, H);

  /* Upload texture */
  if (ImageData != NULL)
  {
    INT sem = GL_RED, typ = GL_UNSIGNED_BYTE;

    switch (Format)
    {
    case TNG_TEXFMT_BGRA8:
      sem = GL_BGRA;
      typ = GL_UNSIGNED_BYTE;
      break;
    case TNG_TEXFMT_R32F:
      sem = GL_RED;
      typ = GL_FLOAT;
      break;
    case TNG_TEXFMT_RGB32F:
      sem = GL_RGB;
      typ = GL_FLOAT;
      break;
    case TNG_TEXFMT_RGB8:
      sem = GL_RGB;
      typ = GL_UNSIGNED_BYTE;
      break;
    case TNG_TEXFMT_RGBA8:
      sem = GL_RGBA;
      typ = GL_UNSIGNED_BYTE;
      break;
    case TNG_TEXFMT_RGBA32F:
      sem = GL_RGBA;
      typ = GL_FLOAT;
      break;
    case TNG_TEXFMT_RGBA32UI:
      sem = GL_RGBA;
      typ = GL_UNSIGNED_INT;
      break;
    }

    if (Format == TNG_TEXFMT_BGRA8)
      gluBuild2DMipmaps(GL_TEXTURE_2D, 4, W, H, sem, typ, ImageData);
    else
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H, sem, typ, ImageData);
  }
  if (Format == TNG_TEXFMT_BGRA8)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  strncpy(Txt.Name, Name, 255);
  Txt.W = W;
  Txt.H = H;
  Txt.IsCubeMap = FALSE;

  glBindTexture(GL_TEXTURE_2D, 0);

  res = StockAdd(&TNG()->Textures, &Txt);
  
  return res;
} /* End of 'TNG_TextureAdd' function */

/* Add texture image from file function.
 * ARGUMENTS:
 *   - texture image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (tngTEXTURE *) Texture pointer.
 */ 
static tngTEXTURE * TNG_TextureAddFromFile( CHAR *FileName )
{
  tngTEXTURE *tex = NULL;
  tngIMAGE Im;
  
  TNG()->ImageLoad(&Im, FileName);

  tex = TNG()->TextureAdd(FileName, TNG_TEXFMT_BGRA8, Im.W, Im.H, Im.Pixels);

  TNG()->ImageFree(&Im);

  return tex;
} /* End of 'TNG_TextureAddFromFile' function */

/* Texture parameters apply.
 * ARGUMENTS:
 *   - texture pointer:
 *       tngTEXTURE *T;
 *   - number of texture:
 *       INT Num;
 * RETURNS: None.
 */
static VOID TNG_TextureApply( tngTEXTURE *T, INT Num )
{
  glActiveTexture(GL_TEXTURE0 + Num);
  glBindTexture(T->IsCubeMap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, T->TexId);
} /* End of 'TNG_TextureApply' function */

/* Texture parameters apply.
 * ARGUMENTS:
 *   - texture pointer:
 *       tngTEXTURE *T;
 *   - clamp and filter parameters:
 *       INT Clamp, Mag_Filter, Min_Filter;
 * RETURNS: 
 *   (tngTEXTURE *) texture pointer.
 */
static tngTEXTURE * TNG_TextureParams( tngTEXTURE *T, INT Clamp, INT Mag_Filter, INT Min_Filter )
{
  glBindTexture(GL_TEXTURE_2D, T->TexId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Mag_Filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Min_Filter);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Clamp);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Clamp);

  glBindTexture(GL_TEXTURE_2D, 0);

  return T;
} /* End of 'TNG_TextureParams' function */

/* Texture strategy delete function.
 * ARGUMENTS: 
 *   - Texture pointer
 *       tngTEXTURE *Txt;
 * RETURNS: None.
 */ 
static VOID TNG_TextureDelete( tngTEXTURE *Txt )
{
  if (Txt == NULL)
    return;

  glDeleteTextures(1, &Txt->TexId);
} /* End of 'TNG_TextureDelete' function */

/* Texture free function.
 * ARGUMENTS: 
 *   - texture pointer:
 *       tngTEXTURE *Txt;
 * RETURNS: None.
 */ 
static VOID TNG_TextureFree( tngTEXTURE *Txt )
{
  if (Txt == NULL)
    return;
  StockRemove(Txt);
} /* End of 'TNG_TextureFree' function */

/* Filled textures subsytem data */
static tngSUBSYSTEM_TEXTURES SubsystemTextures =
{
  {0},
  TNG_TextureAdd,
  TNG_TextureAddFromFile,
  TNG_TextureParams,
  TNG_TextureApply,
  TNG_TextureFree
};

/* Textures destruct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemTexturesDestructor( VOID )
{
  StockClear(&TNG()->Textures);
} /* End of 'TNG_SubsystemTexturesDestructor' function */

/* Textures subsystem init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
VOID TNG_SubsystemTexturesInit( VOID )
{
  TNG()->SubsystemTextures = SubsystemTextures;

  StockInit(&TNG()->Textures, 0, sizeof(tngTEXTURE), TNG_TextureDelete);
} /* End of 'TNG_SubsystemTexturesInit' function */

/* END OF 'tng_textures.c' FILE */
