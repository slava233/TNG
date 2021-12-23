/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : eval.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Point light source shaders.
 *               Tesselation evaluation shader.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas,
 *               Vasilisa Dmitrieva.
 * LAST UPDATE : 21.04.2019.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420

/* Tessalation formats */
layout(quads, fractional_even_spacing) in;

/* View * Proj matrix */
//uniform mat4 MatrVP;
/* Cut off distance of light source */
uniform vec2 LightCutOffDist;
/* Light position */
uniform vec3 LightPos;
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
uniform vec3 CamUp;
uniform vec3 CamRight;
uniform float Time;

/*
layout(binding = 8) uniform Matrix
{
  mat4 MatrVP;
};
*/
uniform mat4 MatrVP;


/* Get point on sphere.
 * ARGUMENTS:
 *   - parameters depends on which phy & theta angles are going:
 *       float u, v;
 * RETURNS:
 *   (vec3) point.
 */
vec3 get_p( float u, float v )
{
  float phy = u * 2 * acos(-1);
  float theta = v * acos(-1);
  float r = LightCutOffDist.y * 1.2;
  vec3 p;

  p.x = r * sin(phy) * sin(theta);
  p.y = r * cos(theta);
  p.z = r * cos(phy) * sin(theta);

  return p;
} /* End of 'get_p' funciton */

/* Main function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  vec3 p = get_p(gl_TessCoord.x, gl_TessCoord.y);
  gl_Position = MatrVP * vec4(LightPos + p, 1);
} /* End of 'main' function */

/* END OF 'eval.glsl' FILE */
