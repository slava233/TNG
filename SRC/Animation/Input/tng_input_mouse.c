/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_input_mouse.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Input mouse module.
 * PROGRAMMER  : CGSG'2018.
 *               Artem Fabrika.
 * LAST UPDATE : 28.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

/* Mouse response function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_MouseResponse( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(TNG()->hWnd, &pt);
  TNG()->Mdx = pt.x - TNG()->Mx;
  TNG()->Mdy = pt.y - TNG()->My;
  TNG()->Mx = pt.x;
  TNG()->My = pt.y;
  TNG()->Mdz = TNG()->MouseWheel;
  TNG()->Mz += TNG()->MouseWheel;
  TNG()->MouseWheel = 0;
} /* End of 'TNG_MouseResponse' function */

/* Mouse init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_MouseInit( VOID )
{
  POINT pt;

  /* Filled mouse response data */
  TNG()->MouseResponse = TNG_MouseResponse;

  GetCursorPos(&pt);
  ScreenToClient(TNG()->hWnd, &pt);
  TNG()->Mx = pt.x;
  TNG()->My = pt.y;
} /* End of 'TNG_MouseInit' function */

/* END OF 'tng_input_mouse.c' FILE */
