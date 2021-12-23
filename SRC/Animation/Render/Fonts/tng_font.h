/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_font.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Fonts support declaration module.
 * PROGRAMMER  : CGSG'2018.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_font_h_
#define __tng_font_h_

#include "../../../tng_def.h"

typedef struct tagtngSUBSYSTEM_FONT
{
  /* Default font pointer */
  VOID *FntDefault;

  /* Build primitive from string with specified font function.
   * ARGUMENTS:
   *   - font structure pointer:
   *       VOID *Fnt;
   *   - string to be converted:
   *       CHAR *Str;
   *   - primitive color:
   *       VEC4 Color;
   *   - font letter size:
   *       FLT FontSize;
   * RETURNS: None.
   *   (tngPRIM *) builded primitive pointer.
   */
  tngPRIM * (*FntStringToPrim)( VOID *Fnt, CHAR *Str, VEC4 Color, FLT FontSize );
} tngSUBSYSTEM_FONT;

/* Fonts support init function. 
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
VOID TNG_SubsystemFontsInit( VOID );

/* Fonts support destruct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemFontsDestructor( VOID );

#endif /* __tng_font_h_ */

/* END OF 'tng_font.h' FILE */
