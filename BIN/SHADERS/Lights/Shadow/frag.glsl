/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Shadow shaders.
 *               Fragment shader.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas,
 *               Vasilisa Dmitrieva.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 430

/* Out color to screen directly */
layout(location = 0) out vec4 OutColor;

/* Shadow matrix */
uniform mat4 MatrShadow;

/* Position of object */
layout(binding = 5) uniform sampler2D TexPos;

/* Main function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{ 
  vec4 Pos = texture2D(TexPos, gl_FragCoord.xy); /* Position of object */
  mat4 Lt = 
    {
      {0.5, 0, 0, 0},
      {0, 0.5, 0, 0},
      {0, 0, 0.5, 0},
      {0.5, 0.5, 0.5, 1}
    };

  vec4 p = MatrShadow * Lt * Pos;
  OutColor = vec4(1, 0, 0, 1);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
