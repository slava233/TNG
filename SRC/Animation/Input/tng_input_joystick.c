/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_input_joystick.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Input joystick module.
 * PROGRAMMER  : CGSG'2018.
 *               Artem Fabrika.
 * LAST UPDATE : 28.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#pragma comment(lib, "winmm")

/* Joystick axis obtain value macro */
#define TNG_JOY_GET_AXIS(A) \
  (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1)

/* Joystick response function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_JoystickResponse( VOID )
{
  INT i;

  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          TNG()->JBut[i] = (ji.dwButtons >> i) & 1;
          TNG()->JButClick[i] = TNG()->JBut[i] && !TNG()->JButOld[i];
          TNG()->JButOld[i] = TNG()->JBut[i];
        }

        /* Axes */
        TNG()->JX = TNG_JOY_GET_AXIS(X);
        TNG()->JY = TNG_JOY_GET_AXIS(Y);
        TNG()->JZ = TNG_JOY_GET_AXIS(Z);
        TNG()->JR = TNG_JOY_GET_AXIS(R);

        /* Point of view */
        TNG()->JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
} /* End of 'TNG_JoystickResponse' function */

/* Joystick init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_JoystickInit( VOID )
{
  INT i;

  /* Filled joystick response data */
  TNG()->JoystickResponse = TNG_JoystickResponse;

  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          TNG()->JBut[i] = (ji.dwButtons >> i) & 1;
          TNG()->JButClick[i] = TNG()->JBut[i];
        }

        /* Axes */
        TNG()->JX = TNG_JOY_GET_AXIS(X);
        TNG()->JY = TNG_JOY_GET_AXIS(Y);
        TNG()->JZ = TNG_JOY_GET_AXIS(Z);
        TNG()->JR = TNG_JOY_GET_AXIS(R);

        /* Point of view */
        TNG()->JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
} /* End of 'TNG_JoystickInit' function */

/* END OF THE 'tng_input_joystick.c' FILE */
