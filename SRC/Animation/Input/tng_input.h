/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_input.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Input header file.
 * PROGRAMMER  : CGSG'2018.
 *               Artem Fabrika.
 * LAST UPDATE : 28.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_input_h_
#define __tng_input_h_

/* Keyboard response subsystem structure */
typedef struct tagtngSUBSYSTEM_KEYB
{
  /* Keyboard variables */
  BYTE
    Keys[256],      /* Keys statement on current frame */
    KeysOld[256],   /* Keys statement on previous frame */
    KeysClick[256]; /* Is key pressed */

  /* Keyboard response function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*KeyboardResponse)( VOID );
} tngSUBSYSTEM_KEYB;

/* Mouse response subsystem structure */
typedef struct tagtngSUBSYSTEM_MOUSE
{
  /* Mouse variables */
  INT
    Mx, My, Mz,    /* Mouse axes */
    Mdx, Mdy, Mdz; /* Delta mouse */

  /* Mouse response function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*MouseResponse)( VOID );
} tngSUBSYSTEM_MOUSE;

/* Joystick response subsystem structure */
typedef struct tagtngSUBSYSTEM_JS
{
  /* Joystick variables */
  BYTE 
    JBut[32],         /* Joystick keys statement on current frame */
    JButOld[32],      /* Joystick keys statement on previous frame */
    JButClick[32];    /* Is joystick key pressed */
  INT JPov;           /* Joystick point-of-view control */
  DBL JX, JY, JZ, JR; /* Joystick axes */

  /* Joystick response function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*JoystickResponse)( VOID );
} tngSUBSYSTEM_JS;

/* Input devices subsystem structure */
typedef struct tagtngSUBSYSTEM_INPUT
{
  /* Keyboard structure */
  union
  {
    tngSUBSYSTEM_KEYB;
    tngSUBSYSTEM_KEYB Keyboard;
  };

  /* Mouse structure */
  union
  {
    tngSUBSYSTEM_MOUSE;
    tngSUBSYSTEM_MOUSE Mouse;
  };

  /* Joystick structure */
  union
  {
    tngSUBSYSTEM_JS;
    tngSUBSYSTEM_JS Joystick;
  };

  /* Input system response function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*InputResponse)( VOID );
} tngSUBSYSTEM_INPUT;

/***
 * Init functions block
 **/

/* Keyboard init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_KeyboardInit( VOID );

/* Mouse init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_MouseInit( VOID );

/* Joystick init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_JoystickInit( VOID );

/* Input system init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemInputInit( VOID );

#endif /* __tng_input_h_ */

/* END OF 'tng_input.h' FILE */
