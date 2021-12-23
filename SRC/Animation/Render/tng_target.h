/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_target.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Target header file.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kuralenok,
 *               Alexandr Vashpanov.
 * LAST UPDATE : 29.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_target_h_
#define __tng_target_h_

#include "../../tng_def.h"

/* Maximum number of attachments */
#define TNG_TARGET_ATTACHMENT_MAX_COUNT 8

/* Render target representation type */
typedef struct tagtngTARGET
{
  INT W, H, NumOfCA, NumOfDA; /* Size of target, number of color & depth attachments */
  tngTEX_FMT AttachmentsFmt[TNG_TARGET_ATTACHMENT_MAX_COUNT]; /* Array format of attachments */
  tngTEXTURE *Attachments[TNG_TARGET_ATTACHMENT_MAX_COUNT];   /* Array of attachment texture pointers */
  UINT FBO, RenderBuf;        /* Target frame buffer object, target render buffer */
} tngTARGET;

typedef struct tagtngSUBSYSTEM_TARGET
{
  /* Stock of targets */
  STOCK Targets;

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
  tngTARGET * (*TargetCreate)( INT W, INT H, CHAR *CAttach, CHAR *DAttach, BOOL IsDepth );

  /* Render target buffers free function. 
   * ARGUMENTS:
   *   - render target to delete:
   *       tngTARGET *Trg;
   * RETURNS: None.
   */
   VOID (*TargetFree)( tngTARGET *Trg );

  /* Render target resize function. 
   * ARGUMENTS:
   *   - render target to resize:
   *       tngTARGET *Trg;
   *   - new size of attachments:
   *       INT NewW, NewH;
   * RETURNS: None.
   */
  VOID (*TargetResize)( tngTARGET *Trg, INT NewW, INT NewH );
} tngSUBSYSTEM_TARGET;

/* Textures destruct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemTargetDestructor( VOID );

/* Target subsystem initilization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemTargetInit( VOID );

#endif /* __tng_target_h_ */

/* END OF 'tng_target.h' FILE */