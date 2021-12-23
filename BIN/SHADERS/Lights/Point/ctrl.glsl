/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ctrl.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Point light source shaders.
 *               Tesselation control shader.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas,
 *               Vasilisa Dmitrieva.
 * LAST UPDATE : 17.04.2019.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420

/* Time */
//uniform float Time;

/*
layout(binding = 7) uniform Standart
{
  vec3 CamLoc;
  float Time;
};
*/

uniform vec3 CamLoc;
uniform float Time;

/* Number of out vertices equals second parameter of glPatchParameteri function */
layout(vertices = 1) out;

/* Main function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  /* Moving data from tesselation control shader to tesselation evaluation shader */
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

  /* split value */
  float n = 64;

  // Outer split values
  gl_TessLevelOuter[0] = n;  	 // by U0
  gl_TessLevelOuter[1] = n / 2;  // by V0
  gl_TessLevelOuter[2] = n;  	 // by U1
  gl_TessLevelOuter[3] = n / 2;  // by V1

  // Inner split values
  gl_TessLevelInner[0] = n;  	 // by U
  gl_TessLevelInner[1] = n / 2;  // by V
} /* End of 'main' function */

/* END OF 'crtl.glsl' function */
