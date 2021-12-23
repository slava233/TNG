/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_input_keyboard.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Input keyboard module.
 * PROGRAMMER  : CGSG'2018.
 *               Artem Fabrika.
 * LAST UPDATE : 28.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

/* Array of pressed keys */
static BYTE IsKeyPressed[256];

/* Keyboard response function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_KeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(TNG()->Keys);
  for (i = 0; i < 256; i++)
  {
    TNG()->Keys[i] >>= 7;
    TNG()->KeysClick[i] = TNG()->Keys[i] && !TNG()->KeysOld[i];
  }
  memcpy(TNG()->KeysOld, TNG()->Keys, 256);
} /* End of 'TNG_KeyboardResponse' function */

/* Keyboard init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_KeyboardInit( VOID )
{
  INT i;

  /* Filled keyboard response data */
  TNG()->KeyboardResponse = TNG_KeyboardResponse;

  GetKeyboardState(TNG()->Keys);
  for (i = 0; i < 256; i++)
  {
    TNG()->Keys[i] >>= 7;
    TNG()->KeysClick[i] = TNG()->Keys[i];
  }
} /* End of 'TNG_KeyboardInit' function */

/* Checked if keyboard buttons pressed function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID IsKbdHit( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
    ;
} /* End of 'IsKbdHit' function */

/* Checked if get symbol function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID IsGetChar ( VOID )
{
} /* End of 'IsGetChar' function */

/* Clear keyboard buttons array function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID KbdClear( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
    IsKeyPressed[i] = 0;
} /* End of 'KbdClear' function */

/* END OF 'tng_input_keyboard.c' FILE */
