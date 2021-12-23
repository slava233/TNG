/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : geom.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Directional light source shaders.
 *               Geomerty shader.
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

/* Format of in vertices */
layout(points) in;
/* Format and max_vertices of out vertices */
layout(triangle_strip, max_vertices = 4) out;

/* Main function.
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
void main( void )
{
  /* Setup rectangle on screen */
  gl_Position = vec4(-1, 1, 0, 1);
  EmitVertex();

  gl_Position = vec4(-1, -1, 0, 1);
  EmitVertex();

  gl_Position = vec4(1, 1, 0, 1);
  EmitVertex(); 

  gl_Position = vec4(1, -1, 0, 1);
  EmitVertex();
  EndPrimitive();
} /* End of 'main' function */

/* END OF 'geom.glsl' FILE */
