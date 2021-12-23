/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : vert.glsl
 * PURPOSE     : Target vertex shader.
 *               Rendering system implementation.
 *               Resourses handle.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kuralenok,
 *               Alexandr Vashpanov.
 * LAST UPDATE : 01.04.2019
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 420

/* Main funciton.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  /* Set up fake gl_Position */
  gl_Position = vec4(0, 0, 0, 1);
} /* End of 'main' funciton */

/* END OF 'vert.glsl' FILE */
