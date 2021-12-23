/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_input_main.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Input main module.
 * PROGRAMMER  : CGSG'2018.
 *               Artem Fabrika.
 * LAST UPDATE : 28.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../tng.h"

/* Input response function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_InputResponse( VOID )
{
  TNG()->KeyboardResponse();
  TNG()->MouseResponse();
  TNG()->JoystickResponse();
} /* End of 'TNG_InputResponse' function */

/* Input init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemInputInit( VOID )
{
  /* Filled common input response data */
  TNG()->InputResponse = TNG_InputResponse;

  TNG_KeyboardInit();
  TNG_MouseInit();
  TNG_JoystickInit();
} /* End of 'TNG_SubsystemInputInit' function */

/* END OF 'tng_input_main.c' FILE */
