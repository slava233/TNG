/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_textures.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Textures handle header module.
 * PROGRAMMER  : CGSG'2018.
 *               Andrey Shayda.
 * LAST UPDATE : 17.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_textures_h_
#define __tng_textures_h_

#include "../../../tng_def.h"

/* Length of texture name */
#define TNG_TEXTURE_NAME_LENGTH 256

/* Textures formats enum */
enum tagtngTEX_FMT
{
  TNG_TEXFMT_RGBA8 = GL_RGBA8,
  TNG_TEXFMT_RGB8 = GL_RGB8, 
  TNG_TEXFMT_RGB32F = GL_RGB32F,
  TNG_TEXFMT_RGBA32F = GL_RGBA32F,
  TNG_TEXFMT_R32F = GL_R32F,
  TNG_TEXFMT_RGBA32UI = GL_RGBA32UI,
  TNG_TEXFMT_DEPTH24 = GL_DEPTH_COMPONENT24,
  TNG_TEXFMT_BGRA8 = GL_BGRA
};

/* Texture structure */
struct tagtngTEXTURE
{
  CHAR Name[TNG_TEXTURE_NAME_LENGTH]; /* Image file name */
  INT W, H;       /* Texture size in pixels */
  BOOL IsCubeMap; /* Cube map texture flag */
  UINT TexId;     /* OpenGL texture Id */
}; 

/* Textures subsystem */
typedef struct tagtngSUBSYSTEM_TEXTURES
{
  /* Stock of textures */
  STOCK Textures;

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
  tngTEXTURE * (*TextureAdd)( CHAR *Name, tngTEX_FMT Format, INT W, INT H, VOID *ImageData );
  
  /* Add texture image from file function.
   * ARGUMENTS:
   *   - texture image file name:
   *       CHAR *FileName;
   * RETURNS:
   *   (tngTEXTURE *) Texture pointer.
   */ 
  tngTEXTURE * (*TextureAddFromFile)( CHAR *FileName );

  /* Texture parameters apply.
   * ARGUMENTS:
   *   - texture pointer:
   *       tngTEXTURE *T;
   *   - clamp and filter parameters:
   *       INT Clamp, Mag_Filter, Min_Filter;
   * RETURNS: 
   *   (tngTEXTURE *) texture pointer.
   */
  tngTEXTURE * (*TextureParams)( tngTEXTURE *T, INT Clamp, INT Mag_Filter, INT Min_Filter );

  /* Texture parameters apply.
   * ARGUMENTS:
   *   - texture pointer:
   *       tngTEXTURE *T;
   *   - number of texture:
   *       INT Num;
   * RETURNS: None.
   */
  VOID (*TextureApply)( tngTEXTURE *T, INT Num );
  
  /* Texture delete function.
   * ARGUMENTS: 
   *   - Texture pointer
   *       tngTEXTURE *Txt;
   * RETURNS: None.
   */
  VOID (*TextureFree)( tngTEXTURE *Txt );
} tngSUBSYSTEM_TEXTURES;

/* Textures destruct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemTexturesDestructor( VOID );

/* Texturess subsystem init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
VOID TNG_SubsystemTexturesInit( VOID );

#endif /* __tng_textures_h_ */

/* END OF 'tng_textures.h' FILE */
