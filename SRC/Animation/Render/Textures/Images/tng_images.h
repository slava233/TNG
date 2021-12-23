/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_textures.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Images module header file.
 * PROGRAMMER  : CGSG'2018.
 *               Andrey Shayda.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_images_h_
#define __tng_images_h_

#include "../../../../tng_def.h"

/* Image representation structure */
struct tagtngIMAGE
{
  INT W, H;           /* Image size */
  DWORD *Pixels;      /* Image pixel colors */
  DWORD **Rows;       /* Image rows array */
  BYTE (**RowsC)[4];  /* Image rows array by components */
}; 

/* Images subsystem */
typedef struct tagtngSUBSYSTEM_IMAGES
{
  /* Image create functions.
   * ARGUMENTS:
   *   - image to be created:
   *       tngIMAGE *Im;
   *   - image size:
   *       INT W, H;
   * RETURNS: None.
   */
  BOOL (*ImageCreate)( tngIMAGE *Im, INT W, INT H );

  /* Get pixel value function.
   * ARGUMENTS:
   *   - image structure pointer:
   *       tngIMAGE *Im;
   *   - coordinates of pixel:
   *       INT X, Y;
   * RETURNS: 
   *   (VEC4) Vector of color.
   */
  VEC4 (*ImageGet)( tngIMAGE *Im, INT X, INT Y );

  /* Flip image vertical function.
   * ARGUMENTS:
   *   - image which must be flipped:
   *       tngIMAGE *Im;
   * RETURNS: None.
   */
  VOID (*ImageFlipVertical)( tngIMAGE *Im );

  /* Image load function.
   * ARGUMENTS:
   *   - image to be load:
   *       tngIMAGE *Im;
   *   - image file name:
   *       CHAR *FileName;
   * RETURNS:
   *   (BOOL) TRUE if success, FALSE otherwise.
   */
  BOOL (*ImageLoad)( tngIMAGE *Im, CHAR *FileName );

  /* Save image in G24 file format function.
   * ARGUMENTS:
   *   - image to save:
   *       tngIMAGE *Im;
   *   - name of file:
   *       CHAR *FileName;
   * RETURNS: None.
   */
  VOID (*ImageSaveG24)( tngIMAGE *Im, CHAR *FileName );
  /* Save image in G32 file function.
   * ARGUMENTS:
   *   - image to be save:
   *       tngIMAGE *Im;
   *   - name of file:
   *       CHAR *FileName;
   * RETURNS: None.
   */
  VOID (*ImageSaveG32)( tngIMAGE *Im, CHAR *FileName );

  /* Save image in BMP file.
   * ARGUMENTS:
   *   - image to save:
   *       tngIMAGE *Im;
   *   - name of file:
   *       CHAR *FileName;
   * RETURNS: None.
   */
  VOID (*ImageSaveBMP)( tngIMAGE *Im, CHAR *FileName );

  /* Save image in *.PNG file function
   * ARGUMENTS:
   *   - image to be save:
   *       tngIMAGE Im;
   *   - Name of file:
   *       CHAR *FileName;
   * RETURNS:
   *   BOOL (TRUE - all save right, FALSE - save error);
   */
  BOOL (*ImageSavePNG)( tngIMAGE *Im, CHAR *FileName );

  /* Save image JPG foromat function.
   * ARGUMENTS:
   *   - picture data pointer:
   *       tngIMAGE *P;
   *   - new picture size:
   *       CHAR *FileName;
   * RETURNS:
   *   (BOOL) TRUE for success, FALSE if no memory, or bad file.
   */
  BOOL (*ImageSaveJPG)( tngIMAGE *P, CHAR *FileName ); 

  /* Image free function.
   * ARGUMENTS:
   *   - Image to be free up:
   *       tngIMAGE *Im;
   * RETURNS: None.
   */
  VOID (*ImageFree)( tngIMAGE *Im );
} tngSUBSYSTEM_IMAGES;

/* Images destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemImagesDestructor( VOID );

/* Images subsystem init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
VOID TNG_SubsystemImagesInit( VOID );

#endif /* __tng_images_h_ */

/* END OF 'tng_images.h' FILE */
