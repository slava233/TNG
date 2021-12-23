/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : vert.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Spot light source shaders.
 *               Vertex shader.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas,
 *               Vasilisa Dmitrieva.
 * LAST UPDATE : 28.03.2019.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420

/* Light position */
uniform vec3 LightPos;

/* Main functoin.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  gl_Position = vec4(LightPos, 1);
} /* End of 'main' function */

/* END OF 'vert.glsl' FILE */
